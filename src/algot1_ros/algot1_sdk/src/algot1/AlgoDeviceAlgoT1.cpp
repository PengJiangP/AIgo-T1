#include "AlgoDeviceT1.h"
#include "AlgoDeviceAlgoT1_Private.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <iostream>
#include <fstream>

namespace Algo1010
{

  AlgoDeviceAlgoT1::AlgoDeviceAlgoT1()
  {
    m_private = (void *)new AlgoDeviceAlgoT1_Private(this);
  }

  AlgoDeviceAlgoT1::~AlgoDeviceAlgoT1()
  {
    delete (AlgoDeviceAlgoT1_Private *)m_private;
  }

  bool AlgoDeviceAlgoT1::load(std::string path)
  {
    // 1 read json text
    std::ifstream in(path);
    if (!in.is_open())
    {
      printf("fail to read json file: %s\n", path.c_str());
      return false;
    }
    std::string json_content((std::istreambuf_iterator<char>(in)),
                             std::istreambuf_iterator<char>());
    in.close();

    // 2 parse dom
    rapidjson::Document dom;
    if (dom.Parse(json_content.c_str()).HasParseError())
    {
      printf("fail to parse json file: %s\n", path.c_str());
      return false;
    }

    // 3 read dom value
    if (!dom.HasMember("AlgoT1"))
    {
      printf("no 'AlgoT1' in json file: %s\n", path.c_str());
      return false;
    }

    rapidjson::Value &domAlgoT1 = dom["AlgoT1"];
    if (!domAlgoT1.HasMember("net_cam1_settings"))
      return false;
    if (!domAlgoT1.HasMember("net_cam2_settings"))
      return false;

    AlgoDeviceAlgoT1_Private *privateData = ((AlgoDeviceAlgoT1_Private *)m_private);

    // set cam1
    rapidjson::Value &domCam1 = domAlgoT1["net_cam1_settings"];
    int cam1_port = domCam1["port"].GetInt();
    std::string cam1_ip = domCam1["ip"].GetString();
    std::string cam1_type = domCam1["type"].GetString();

    std::string cam1_path = AlgoStreamBase::toPath4Net(cam1_ip, cam1_port);
    int cam1_nettype = AlgoStreamBase::netName2netType(cam1_type);
    privateData->m_streamCam[0].init("net_cam1_settings", cam1_nettype, cam1_path);

    // set cam2
    rapidjson::Value &domCam2 = domAlgoT1["net_cam2_settings"];
    int cam2_port = domCam2["port"].GetInt();
    std::string cam2_ip = domCam2["ip"].GetString();
    std::string cam2_type = domCam2["type"].GetString();

    std::string cam2_path = AlgoStreamBase::toPath4Net(cam2_ip, cam2_port);
    int cam2_nettype = AlgoStreamBase::netName2netType(cam2_type);
    privateData->m_streamCam[1].init("net_cam2_settings", cam2_nettype, cam2_path);

    for (int netId = 0; netId < 8; netId++)
    {
      // get dom and check
      std::string netkey = ("net_") + std::to_string(netId + 1) + ("_settings");
      if (!domAlgoT1.HasMember(netkey.c_str()))
        return false;

      // read dom value
      rapidjson::Value &domNet = domAlgoT1[netkey.c_str()];
      int port = domNet["port"].GetInt();
      std::string ip = domNet["ip"].GetString();
      std::string type = domNet["type"].GetString();
      std::string cmd = domNet["startcmd"].GetString();

      // set net
      std::string netpath = AlgoStreamBase::toPath4Net(ip, port);
      int nettype = AlgoStreamBase::netName2netType(type);
      privateData->m_streamNet[netId].init(netkey, nettype, netpath,cmd);
    }

    // set vins
    rapidjson::Value &domVins = domAlgoT1["net_vins_settings"];
    int vins_port = domVins["port"].GetInt();
    std::string vins_ip = domVins["ip"].GetString();
    std::string vins_type = domVins["type"].GetString();

    std::string vins_path = AlgoStreamBase::toPath4Net(vins_ip, vins_port);
    int vins_nettype = AlgoStreamBase::netName2netType(vins_type);
    privateData->m_streamVins.init("net_vins_settings", vins_nettype, vins_path);

    return true;
  }

  bool AlgoDeviceAlgoT1::start()
  {
    AlgoDeviceAlgoT1_Private *privateData = ((AlgoDeviceAlgoT1_Private *)m_private);

    privateData->m_threadCam[0]->start();
    privateData->m_threadCam[1]->start();
    for (int netId = 0; netId < 8; netId++)
    {
      privateData->m_threadNet[netId]->start();
    }
    privateData->m_threadVins->start();

    return true;
  }

  bool AlgoDeviceAlgoT1::stop()
  {
    AlgoDeviceAlgoT1_Private *privateData = ((AlgoDeviceAlgoT1_Private *)m_private);

    privateData->m_threadCam[0]->stop();
    privateData->m_threadCam[1]->stop();
    for (int netId = 0; netId < 8; netId++)
    {
      privateData->m_threadNet[netId]->stop();
    }
    privateData->m_threadVins->stop();

    return true;
  }

  void AlgoDeviceAlgoT1::dataCallback(int handle, AlgoT1DataType dataType, AlgoDataPacket *packet, char *data, int len)
  {
    switch (dataType)
    {
    case AlgoT1DataType::ALGO_DAT_CAM0:
      Cam0DataCallback(handle, (AlgoCameraDataPacket *)packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_CAM1:
      Cam1DataCallback(handle, (AlgoCameraDataPacket *)packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_GNSS1:
      Gnss1DataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_GNSS2:
      Gnss2DataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_MEMS:
      MemsDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_RTCM:
      RtcmDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_INSPVAXA:
      InspvaxaDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_GPGGA:
      GpggaDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_PPPB2B:
      Pppb2bDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_GPRMC:
      GprmcDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_VINS:
      VinsDataCallback(handle, packet, data, len);
      break;
    default:
      break;
    }
  }

}