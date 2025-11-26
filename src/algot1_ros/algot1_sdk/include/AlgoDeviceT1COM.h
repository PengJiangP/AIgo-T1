#ifndef _ALGO_DEVICE_T1_COM_H_
#define _ALGO_DEVICE_T1_COM_H_

#include "AlgoSDK.h"

namespace Algo1010
{

  class AlgoDeviceT1COM : public AlgoDevice
  {
  public:
    AlgoDeviceT1COM();

    ~AlgoDeviceT1COM();

    void dataCallback(int handle, AlgoT1DataType dataType, AlgoDataPacket *packet, char *data, int len);

    virtual void InspvaxaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;

    virtual void GpggaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;

  public:
    bool load(std::string path);

    bool start();

    bool stop();
  };

}

#endif //!_ALGO_DEVICE_T1_COM_H_