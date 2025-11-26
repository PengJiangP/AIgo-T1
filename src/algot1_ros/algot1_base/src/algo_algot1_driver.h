/**
 * @file: algo_algot1_driver1.h
 * @author: jack <283422622@qq.com>
 * @date: Nov.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of an MyAlgoDevice and MyAlgoProject class.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */
#ifndef ALGO_ALGOT1_DRIVER1_H_
#define ALGO_ALGOT1_DRIVER1_H_

#include "AlgoSDK.h"
#include "uliti/AlgoString.h"
#include "uliti/AlgoCoordTrans.h"
#include "uliti/AlgoOpenCv.h"
#include "uliti/AlgoTurbJpeg.h"

#include <iostream>
#include <thread>
#include <memory.h>
#include <mutex>

#ifdef ROS1
    #include <ros/ros.h>
    #include <sensor_msgs/CameraInfo.h>
    #include <sensor_msgs/Image.h>
    #include <sensor_msgs/Imu.h>
    #include <sensor_msgs/NavSatFix.h>
    #include <sensor_msgs/NavSatStatus.h>
    #include <nav_msgs/Path.h>
    #include <nav_msgs/Odometry.h>
    #include <geometry_msgs/PoseStamped.h>
    #include <tf/transform_broadcaster.h>
    #ifdef USE_OPENCV
    #include <cv_bridge/cv_bridge.h>
    #endif
    typedef ros::Time RosTime;
#endif ROS1

#ifdef ROS2
    #include <rclcpp/rclcpp.hpp>
    #include <rclcpp/executor.hpp>
    #include <sensor_msgs/msg/camera_info.hpp>
    #include <sensor_msgs/msg/image.hpp>
    #include <sensor_msgs/msg/imu.hpp>
    #include <sensor_msgs/msg/nav_sat_fix.hpp>
    #include <sensor_msgs/msg/nav_sat_status.hpp>
    #include <nav_msgs/msg/path.hpp>
    #include <nav_msgs/msg/odometry.hpp>
    #include <geometry_msgs/msg/pose_stamped.hpp>
    #include <tf2_ros/transform_broadcaster.h>
    #include <tf2_geometry_msgs/tf2_geometry_msgs.h>
    #include <cv_bridge/cv_bridge.h>
    typedef rclcpp::Time RosTime;
    #include "algot1_msgs/msg/gnss_state.hpp"
#endif

namespace Algo1010
{
 

    class AlgoT1RosDriver : public AlgoT1Project
    {

    public:
        AlgoT1RosDriver();

        ~AlgoT1RosDriver();

        void publishGnss1Data(AlgoDataPacket *packet, char *data, int len) {}

        void publishGnss2Data(AlgoDataPacket *packet, char *data, int len) {}

        void publishCam0Image(AlgoCameraDataPacket *packet, char *data, int len);

        void publishCam1Image(AlgoCameraDataPacket *packet, char *data, int len);

        void publishCam2Image(AlgoCameraDataPacket *packet, char *data, int len);

        void publishInspvaxaData(AlgoINSPVAXADataPacket *packet, char *data, int len);

        void publishGpggaData(AlgoGPGGADataPacket *packet, char *data, int len);

        void publishGprmcData(AlgoGPRMCDataPacket *packet, char *data, int len);

        void publishRtcmData(AlgoDataPacket *packet, char *data, int len) {}

        void publishMemsData(AlgoImuDataPacket *packet, char *data, int len);

        void publishPppb2bData(AlgoDataPacket *packet, char *data, int len) {}

        void publishWheelData(AlgoDataPacket *packet, char *data, int len) {}

        void publishVinsData(AlgoVINSPOSDataPacket *packet, char *data, int len);

        void publishGnssState(AlgoGPGGADataPacket *packet, char *data, int len);\
        double gps_to_unix(int gps_week, double week_seconds);

    private:
#ifdef ROS1
        ros::NodeHandle *node_;
        ros::Publisher cam0_publisher_;
        ros::Publisher cam1_publisher_;
        ros::Publisher cam2_publisher_;
        ros::Publisher imu_publisher_; 
        ros::Publisher pose_inspvaxa_publisher_;
        ros::Publisher pose_gpgga_publisher_;
        ros::Publisher pose_gprmc_publisher_;
        ros::Publisher pose_vins_publisher_;
        ros::Publisher path_inspvaxa_publisher_;
        ros::Publisher path_gpgga_publisher_;
        ros::Publisher path_gprmc_publisher_;
        ros::Publisher path_vins_publisher_;
        ros::Publisher odom_gpgga_publisher_;
        ros::Publisher odom_inspvaxa_publisher_;
        ros::Publisher diy_gnss_state_publisher_;  
        ros::Publisher diy_gins_publisher_; 

        nav_msgs::Path path_inspvaxa_;
        nav_msgs::Path path_gpgga_;
        nav_msgs::Path path_gprmc_;
        nav_msgs::Path path_vins_;
        ros::Publisher imu_fusion_publisher_;
#endif

#ifdef ROS2
        rclcpp::Node *node_;
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr cam0_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr cam1_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr cam2_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr pose_inspvaxa_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr pose_gpgga_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr pose_gprmc_publisher_;
        rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr pose_vins_publisher_;
        rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_inspvaxa_publisher_;
        rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_gpgga_publisher_;
        rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_gprmc_publisher_;
        rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_vins_publisher_;
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_gpgga_publisher_;
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_inspvaxa_publisher_;

        rclcpp::Publisher<algot1_msgs::msg::GnssState>::SharedPtr diy_gnss_state_publisher_;
        algot1_msgs::msg::GnssState diy_gnss_state_;

        nav_msgs::msg::Path path_inspvaxa_;
        nav_msgs::msg::Path path_gpgga_;
        nav_msgs::msg::Path path_gprmc_;
        nav_msgs::msg::Path path_vins_;

        geometry_msgs::msg::Quaternion createQuaternionMsgFromYaw(double yaw);
#endif
        std::string topic_gnss1_ = "/gnss0/raw";
        std::string topic_gnss2_ = "/gnss1/raw";
        std::string topic_cam0_ = "/cam0";
        std::string topic_cam1_ = "/cam1";
        std::string topic_cam2_ = "/cam2";
        std::string topic_imu_ = "/imu0";
        std::string topic_pppb2b_ = "/pppb2b";
        std::string topic_wheelspeed_ = "/wheelspeed";

        std::string topic_inspvaxa_ = "/pose/inspvaxa";
        std::string topic_gpgga_ = "/pose/gpgga";
        std::string topic_gprmc_ = "/pose/gpgmc";
        std::string topic_vins_ = "/pose/vins";

        std::string topic_path_inspvaxa_ = "/path/inspvaxa";
        std::string topic_path_gpgga_ = "/path/gpgga";
        std::string topic_path_gprmc_ = "/path/gprmc";
        std::string topic_path_vins_ = "/path/vins";

        std::string topic_diy_gnssstate_ = "/gnss_state";
        std::string topic_diy_gins_ = "/gins";

        AlgoCoordTrans m_coordTrans;
        double ref_B_ = 0, ref_L_ = 0, ref_H_ = 0;

        // newest data
        std::mutex mutex_inspvaxa_;
        AlgoINSPVAXADataPacket packet_inspvaxa_;
        std::mutex mutex_gpgga_;
        AlgoINSPVAXADataPacket packet_gpgga_;

    };

}

#endif //! ALGO_ALGOT1_DRIVER_H_
