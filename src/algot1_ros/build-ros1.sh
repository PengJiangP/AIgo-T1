echo "Start to Compile algot1_ros ROS1......"

workdir=$(cd $(dirname $0); pwd)
cd $workdir/../../

source /opt/ros/noetic/setup.bash
catkin_make  -DCATKIN_WHITELIST_PACKAGES="algot1_sdk;algot1_msgs;algot1_base" -DROS1=1

source ./devel/setup.bash
roslaunch algot1_base algo-algot1.launch
#roslaunch algot1_base algo-algot1-com.launch
