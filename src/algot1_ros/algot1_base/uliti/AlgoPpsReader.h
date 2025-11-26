#ifndef _ALGO_PPS_READER_H_
#define _ALGO_PPS_READER_H_

#include <fcntl.h>
#include <unistd.h>
#include <linux/pps.h>
#include <gpiod.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace Algo1010;

class AlgoPpsReader
{
public:
    AlgoPpsReader() {}
    ~AlgoPpsReader() {}

public:
    int start(AlgoDevice *device)
    {
        // 启动USB摄像头读取线程
        bool UseUsbCamera = true; // 是否使用usb相机
        if (UseUsbCamera)
        {
            std::thread cameraThread(ppsReaderThread, static_cast<RosAlgoDeviceAlgoT1 *>(device));
            cameraThread.detach(); // 分离线程，让它独立运行
        }
        else
        {
            std::cout << "No use usb camera" << std::endl;
        }
    }

    static int ppsReaderThread(RosAlgoDeviceAlgoT1 *device)
    {
        // 1. Open and read PPS device
        int pps_fd = open("/dev/pps1", O_RDONLY);
        if (pps_fd < 0)
        {
            std::cerr << "Error opening PPS device. Check permissions/device path.\n";
            return 1;
        }

        while (true)
        {
            pps_ktime pps_data;
            if (read(pps_fd, &pps_data, sizeof(pps_data)) != sizeof(pps_data))
            {
                std::cerr << "Failed to read PPS data.\n";
                close(pps_fd);
                return 1;
            }
            close(pps_fd);

            // Export GPIO (e.g., GPIO17)
            std::ofstream export_file("/sys/class/gpio/export");
            export_file << "17";
            export_file.close();

            // Set direction to output
            std::ofstream direction_file("/sys/class/gpio/gpio17/direction");
            direction_file << "out";
            direction_file.close();

            // Control GPIO
            std::ofstream value_file("/sys/class/gpio/gpio17/value");
            for (int i = 0; i < 30; ++i)
            {
                value_file << "1"; // Set high
                value_file.flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(33));
            }
            value_file << "0"; // Cleanup
            value_file.close();

            std::cerr << "pps flaG=" << pps_data.flags << " sec=" << pps_data.sec << " nsec=" << pps_data.nsec << std::endl;
        }

        return 0;
    }
};

#endif //!_ALGO_PPS_READER_H_