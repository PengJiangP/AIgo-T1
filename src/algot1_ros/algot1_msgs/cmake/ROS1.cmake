
find_package(catkin REQUIRED COMPONENTS
  std_msgs
  message_generation
)

## Generate messages in the 'msg' folder
add_message_files(
    FILES
    GnssState.msg
    Gins.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(CATKIN_DEPENDS std_msgs message_runtime)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES scout_python
#  CATKIN_DEPENDS rospy scout_msgs std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
#  include
  ${catkin_INCLUDE_DIRS}
)
