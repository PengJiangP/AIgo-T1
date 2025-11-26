#ifndef _ALGO_USB_CAM_H_
#define _ALGO_USB_CAM_H_

#include "algo_algot1_driver.h"
#include "algo_algot1_device.h"

using namespace Algo1010;

class AlgoUsbCam
{
public:
    AlgoUsbCam() {}
    ~AlgoUsbCam() {}

    int m_camId = 0;
    RosAlgoDeviceAlgoT1 *m_device = nullptr;

public:
    int start(AlgoDevice *device, int camId = 0)
    {
        m_device = (RosAlgoDeviceAlgoT1 *)device;
        m_camId = camId;

        // 启动USB摄像头读取线程
        std::thread cameraThread(usbCameraReaderThread, this);
        cameraThread.detach(); // 分离线程，让它独立运行
    }

    static void usbCameraReaderThread(AlgoUsbCam *usbcam)
    {
        // 打开第一个USB摄像头
        cv::VideoCapture cap(usbcam->m_camId, cv::CAP_V4L2);
        if (!cap.isOpened())
        {
            std::cerr << "Failed to open USB camera!" << std::endl;
            return;
        }

        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);

        int frame_count = 0; // 从0开始计数
        cv::Mat frame;
        AlgoCameraDataPacket packet;
        while (true)
        {
            cap >> frame; // 读取一帧
            if (frame.empty())
            {
                std::cerr << "Failed to read frame from camera!" << std::endl;
                continue;
            }

            // 将OpenCV的Mat转换为JPEG数据
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            char *data = reinterpret_cast<char *>(buffer.data());
            int len = buffer.size();
            packet.cameraId = 0;                       // 摄像头ID=0
            packet.cameraMode = 1;                     // 1=RGB?
            packet.imageIndex = frame_count++;         // 默认从0开始
            packet.imageType = 1;                      // 1=JPEG格式
            packet.imageWidth = frame.cols;            // 图像宽度
            packet.imageHeight = frame.rows;           // 图像高度
            packet.pbufSize = len;                     // JPEG数据大小
            packet.pbuf = data;                        // 指向JPEG数据的指针
            packet.cur_sec = ros::Time::now().toSec(); // ROS时间戳

            if(usbcam->m_camId == 0)
                usbcam->m_device->Cam0DataCallback(0, &packet, data, len);
            else
                usbcam->m_device->Cam2DataCallback(0, &packet, data, len);
        }
    }
};

#endif //!_ALGO_USB_CAM_H_