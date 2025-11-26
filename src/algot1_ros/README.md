# algot1_ros

algot1_ros is the usage of algot1 based on ROS1(Ubuntu20-noetic) or ROS2(Ubuntu20-foxy).

## Getting started

### 1. Install ROS1-noetic and dependecies.

install ros1(noetic) for ubuntu20(amd):
```
https://blog.csdn.net/PlutooRx/article/details/127558240
https://developer.aliyun.com/article/1321397
```
install dependecies:
```
sudo apt install ros-noetic-gps-umd
sudo apt install libopencv-dev
```

install gcc-7/g++-7
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y gcc-7 g++-7

# 10，20这些表示的是优先级，可根据需要选定，优先级高的即为默认的
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 20 --slave /usr/bin/g++ g++ /usr/bin/g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 10 --slave /usr/bin/g++ g++ /usr/bin/g++-7
sudo update-alternatives --config gcc
```

### 2. Install ROS2-foxy and dependecies.
install ros2(foxy) for ubuntu20(amd):
```
https://blog.csdn.net/feimeng116/article/details/106602562
https://www.guyuehome.com/10226
```
install dependecies:
```
sudo apt install ros-foxy-gps-umd
sudo apt install libopencv-dev
```

## Clone repository

Firstly,make a ros workspace; then, git clone an existing Git repository with the following command:

```
cd ROS_WS/src
git clone http://git.algotechrobot.com:6060/open/algot1_ros.git algot1_ros
# or ... (The domain name is under filing for record)
git clone http://60.205.125.5:6060/open/algot1_ros.git algot1_ros
11.26中午提交的
```

## Build and Run With ROS

### 1. For ROS1:
```
cd ROS_WS
source /opt/ros/noetic/setup.bash
catkin_make  -DCATKIN_WHITELIST_PACKAGES="algot1_sdk;algot1_msgs;algot1_base" -DROS1=1

source ./devel/setup.bash
roslaunch algot1_base algo-algot1.launch
#roslaunch algot1_base algo-algot1-com.launch
```

### 2. For ROS2:
```
cd ROS_WS/
source /opt/ros/foxy/setup.bash
colcon build --packages-select algot1_msgs algot1_sdk algot1_base --cmake-args -DROS2=1
#colcon build --packages-select algot1_msgs algot1_sdk algot1_base --event-handlers console_direct+ --cmake-args -DCMAKE_VERBOSE_MAKEFILE=ON -DROS2=1

source ./install/setup.bash
ros2 launch algot1_base algo-algot1-launch.py
```

## Build and Debug Without ROS

1. Open code directory of "ROS_WS/src/algot1_ros" in VSCode.

2. Run Task of "build-algot1_sdk-debug".

3. Run and Debug: AlgoT1 Debug.


## Configuration

You can change topic name and origin by yourself in lannch file,"algo-algot1.launch".

```
<?xml version="1.0"?>
<launch>
    <arg name="topic_cam0" default="/cam0" />
    <arg name="topic_cam1" default="/cam1" />
    <arg name="topic_imu" default="/imu0" />

    <!--ENU origin,Geodesy BLH, unit: deg deg m -->
    <arg name="ref_B" default="031.1669249" />
    <arg name="ref_L" default="121.2888731" />
    <arg name="ref_H" default="0" />
</launch>
```

## Rviz PrtSc

![algot1_ros_topic](/algot1_base/doc/algot1_ros_rviz.png)

## 修改
1. 时间戳修改为unix时间

2. inspva输出经纬度和姿态
