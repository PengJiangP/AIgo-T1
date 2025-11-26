list(APPEND CMAKE_PREFIX_PATH "/opt/ros/foxy")

add_definitions(-DROS2)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED) 
find_package(rosgraph_msgs REQUIRED) 
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(cv_bridge REQUIRED)

if(USE_OPENCV)
  add_definitions(-DUSE_OPENCV)
  find_package(OpenCV REQUIRED)
endif(USE_OPENCV)

find_package(algot1_sdk REQUIRED)
find_package(algot1_msgs REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}
  include
  src
  uliti
  ${algot1_sdk_INCLUDE_DIRS}    
  ${algot1_msgs_INCLUDE_DIRS}    
)
link_directories(
        ${OpenCV_LIBRARY_DIRS}
        ${catkin_LIBRARY_DIRS}
        ${algot1_sdk_LIBRARY_DIRS}     
        ${algot1_msgs_LIBRARY_DIRS}         
)

message(STATUS "-----------algot1_msgs_LIBRARY_DIRS = ${algot1_msgs_LIBRARY_DIRS}")
message(STATUS "-----------algot1_sdk_LIBRARY_DIRS = ${algot1_sdk_LIBRARY_DIRS}")

## Main body
add_executable(algo_algot1_node
  src/algo_algot1_driver.h
  src/algo_algot1_device.h
  src/algo_algot1_driver.cpp
  src/algo_algot1_device.cpp
  src/algo_algot1_node.cpp
  uliti/AlgoTurboJpeg.cpp
  uliti/AlgoOpenCv.cpp
)
target_link_libraries(algo_algot1_node algot1_sdk)
ament_target_dependencies(algo_algot1_node rclcpp tf2 tf2_ros tf2_geometry_msgs std_msgs nav_msgs sensor_msgs geometry_msgs rosgraph_msgs cv_bridge algot1_msgs)
install(TARGETS algo_algot1_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/)  
install(DIRECTORY rviz
  DESTINATION share/${PROJECT_NAME}/) 
install(FILES config/algo-algot1.json
  DESTINATION lib/${PROJECT_NAME}/)

ament_package()
