echo "Start to Compile algot1_ros ROS2......"

workdir=$(cd $(dirname $0); pwd)
cd $workdir/../../

source /opt/ros/foxy/setup.bash
colcon build --packages-select algot1_msgs algot1_sdk algot1_base --cmake-args -DROS2=1
#colcon build --packages-select algot1_msgs algot1_sdk algot1_base --event-handlers console_direct+ --cmake-args -DCMAKE_VERBOSE_MAKEFILE=ON -DROS2=1

source ./install/setup.bash
ros2 launch algot1_base algo-algot1-launch.py
