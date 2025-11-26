#include "AlgoDeviceT1COM.h"
#include "AlgoDeviceT1COM_Private.h"

#include "uliti/AlgoStreamBase.h"
#include "uliti/AlgoParserBase.h"
#include "uliti/AlgoThreadBase.h"
#include "parser/AlgoImageParser.h"
#include "parser/AlgoTextParser.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <functional>
#include <mutex>
#include <iostream>
#include <fstream>

namespace Algo1010
{

  AlgoDeviceT1COM::AlgoDeviceT1COM()
  {
    m_private = (void *)new AlgoDeviceT1COM_Private(this);
  }

  AlgoDeviceT1COM::~AlgoDeviceT1COM()
  {
    delete (AlgoDeviceT1COM_Private *)m_private;
  }

  bool AlgoDeviceT1COM::load(std::string path)
  {
    AlgoDeviceT1COM_Private *privateData = ((AlgoDeviceT1COM_Private *)m_private);

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
    for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
    {
      std::string netkey = ("com_") + std::to_string(netId + 1) + ("_settings");
      if (!domAlgoT1.HasMember(netkey.c_str()))
        continue;

      rapidjson::Value &domCom1 = domAlgoT1[netkey.c_str()];
      int com1_enable = domCom1["enable"].GetInt();
      std::string com1_type = domCom1["type"].GetString();
      std::string com1_name = domCom1["name"].GetString();
      int com1_bitrate = domCom1["bitrate"].GetInt();
      int com1_bytesize = domCom1["bytesize"].GetInt();
      std::string com1_parity = domCom1["parity"].GetString();
      int com1_stopbits = domCom1["stopbits"].GetInt();
      std::string com1_cmd = domCom1["startcmd"].GetString();

      std::string com1_path = AlgoStreamBase::toPath4Com(com1_name, com1_bitrate, com1_parity, com1_bytesize, com1_stopbits);
      int com1_nettype = AlgoStreamBase::netName2netType(com1_type);
      privateData->m_streamCom[netId].init(netkey, com1_nettype, com1_path, com1_cmd);
    }

    return true;
  }

  bool AlgoDeviceT1COM::start()
  {
    AlgoDeviceT1COM_Private *privateData = ((AlgoDeviceT1COM_Private *)m_private);
    for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
    {
      privateData->m_threadCom[netId]->start();
    }

    return true;
  }

  bool AlgoDeviceT1COM::stop()
  {
    AlgoDeviceT1COM_Private *privateData = ((AlgoDeviceT1COM_Private *)m_private);
    for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
    {
      privateData->m_threadCom[netId]->stop();
    }

    return true;
  }

  void AlgoDeviceT1COM::dataCallback(int handle, AlgoT1DataType dataType, AlgoDataPacket *packet, char *data, int len)
  {
    switch (dataType)
    {
    case AlgoT1DataType::ALGO_DAT_INSPVAXA:
      InspvaxaDataCallback(handle, packet, data, len);
      break;
    case AlgoT1DataType::ALGO_DAT_GPGGA:
      GpggaDataCallback(handle, packet, data, len);
      break;
    default:
      break;
    }
  }

}