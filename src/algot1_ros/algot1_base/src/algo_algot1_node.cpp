/**
 * @file: AlgoDemo_AlgoT1.h
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of an MyAlgoDevice and MyAlgoProject class.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#include "algo_algot1_driver.h"
#include "algo_algot1_device.h"

#include "uliti/AlgoCmdLine.h"
#include "uliti/AlgoDirectory.h"

#include "uliti/AlgoPpsReader.h"
#include "uliti/AlgoUsbCam.h"

using namespace Algo1010;

int main(int argc, char *argv[])
{
    std::cout << "Run AlgoDemo_AlgoT1..." << std::endl;

    int m_configJsonType = 0; // 0 - NET, 1 - COM
    std::string m_configJson;

    int m_isSave = 0; // 0 - not save, 1 - save
    std::string m_outputPath;

    bool usb_cam0 = 0;
    bool usb_cam1 = 0;

#ifdef ROS1
    // read or parser config params
    CmdLine cmd;
    cmd.add(CmdLine::make_option('i', m_configJson, "config"));
    cmd.add(CmdLine::make_option('t', m_configJsonType, "configType"));
    cmd.add(CmdLine::make_option('s', m_isSave, "isSave"));
    cmd.add(CmdLine::make_option('o', m_outputPath, "rootSave"));
    cmd.add(CmdLine::make_option('u', usb_cam0, "usb_cam0"));
    cmd.add(CmdLine::make_option('v', usb_cam1, "usb_cam1"));
    try
    {
        cmd.process(argc, argv);

        // check config
        if (!cmd.used('i'))
        {
            std::cout << "No param of config" << std::endl;
            throw std::string("Invalid parameter.");
        }

        AlgoString::string_trim(m_configJson);
        if (!AlgoDirectory::fileExists(m_configJson))
        {
            std::cout << "Config file not exist:" << m_configJson << std::endl;
            throw std::string("Invalid parameter.");
        }

        // check output
        if (!cmd.used('o'))
        {
            std::cout << "No param of output" << std::endl;
            throw std::string("Invalid parameter.");
        }

        AlgoString::string_trim(m_outputPath);
        if (!AlgoDirectory::directoryExists(m_outputPath))
        {
            std::cout << "Output path not exist:" << m_outputPath << std::endl;
            throw std::string("Invalid parameter.");
        }

        // check config
        if (!cmd.used('s'))
        {
            std::cout << "No param of is_save" << std::endl;
            throw std::string("Invalid parameter.");
        }
    }
    catch (const std::string &s)
    {
        std::cout << "Usage: " << argv[0] << '\n'
                  << "[-i|--config] json config file.\n";
        std::cout << s;
        return 0;
    }

    ros::init(argc, argv, "algot1");

#endif

#ifdef ROS2
    rclcpp::init(argc, argv);
    rclcpp::Node::SharedPtr node = std::make_shared<rclcpp::Node>("algot1");
    node->declare_parameter("config_pathname", rclcpp::ParameterValue(""));
    node->get_parameter_or<std::string>("config_pathname", m_configJson, "");
    std::cout << "config_pathname = " << m_configJson << std::endl;
    AlgoString::string_trim(m_configJson);

    if (!AlgoDirectory::fileExists(m_configJson))
    {
        std::cout << "Config file not exist:" << m_configJson << std::endl;
        throw std::string("Invalid parameter.");
    }
#endif
    ////////////////////////////////////////////////////////////////////////////
    // 创建一个线程，吧_deviceAlgoT1传进去，new_deviceAlgoT1

    // init project for save data
    AlgoT1Project *_project = new AlgoT1Project();
    if (m_isSave)
    {
        if (!_project->create(m_outputPath))
        {
            std::cout << "Create project failed!" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "Create project root ok!" << m_outputPath << std::endl;
        }
    }

    AlgoT1RosDriver *_rosDriver = new AlgoT1RosDriver();

    AlgoDevice *_deviceAlgoT1 = NULL;
    if (m_configJsonType == 0)
        _deviceAlgoT1 = new RosAlgoDeviceAlgoT1(_rosDriver, _project);
    else // m_configJsonType == 1
        _deviceAlgoT1 = new RosAlgoDeviceAlgoT1COM(_rosDriver, _project);

    // 1.load config
    std::cout << "loading config..." << std::endl;
    if (!_deviceAlgoT1->load(m_configJson))
    {
        std::cout << "Device load config failed!" << std::endl;
        return 0;
    }
    std::cout << "Device load config successed!" << std::endl;

    // 2.start read threads.
    if (!_deviceAlgoT1->start())
    {
        std::cout << "Device start failed!" << std::endl;
        return 0;
    }

    std::cout << "Device start successed!" << std::endl;

    // 启动USB摄像头读取线程
    AlgoUsbCam usbCam0;
    if(usb_cam0)
        usbCam0.start(_deviceAlgoT1,2);

    AlgoUsbCam usbCam1;
    if(usb_cam1)
        usbCam1.start(_deviceAlgoT1,0);

    // 启动PPS读取线程
    AlgoPpsReader ppsReader;
    ppsReader.start(_deviceAlgoT1);

// 3.main thread waitted and show data from rviz;
#ifdef ROS1
    ros::spin();
#endif

#ifdef ROS2
    rclcpp::spin(node);
#endif

    // 4.stop read threads
    _rosDriver->close();
    _deviceAlgoT1->stop();

    delete _deviceAlgoT1;
    delete _rosDriver;

    std::cout << "Device stopped!" << std::endl;

    return 0;
}
