catkin_package()

add_definitions(-DROS1)

if(USE_OPENCV)
  add_definitions(-DUSE_OPENCV)
  #OpenCV4
  find_package(OpenCV REQUIRED)
  include_directories(${OpenCV_INCLUDE_DIRS})
  link_directories(${OpenCV_LIBRARY_DIRS})
  message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
  message(STATUS "OpenCV_LIBRARY_DIRS = ${OpenCV_LIBRARY_DIRS}")
  message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
  find_package(catkin QUIET COMPONENTS roscpp rosbag sensor_msgs cv_bridge )
else()
  find_package(catkin QUIET COMPONENTS roscpp rosbag sensor_msgs)
endif()

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  roscpp
  geometry_msgs
  tf
  message_generation
  algot1_msgs
)

# Include our header files
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${catkin_INCLUDE_DIRS}
        include
        src
        uliti
        ../algot1_sdk/include
)

# Set link libraries used by all binaries
list(APPEND thirdparty_libraries
        ${catkin_LIBRARIES}
)

include_directories(
  uliti
  src
  ${catkin_INCLUDE_DIRS}
)

add_executable(algo_algot1_node
  src/algo_algot1_node.cpp
  src/algo_algot1_driver.cpp
  uliti/AlgoTurboJpeg.cpp
  uliti/AlgoOpenCv.cpp
)
target_link_libraries(algo_algot1_node algot1_sdk pthread 
${thirdparty_libraries} ${OpenCV_LIBS} ${Turbojpeg_LIBRARIES} ${livoxsdk2_LIBRARIES})

install(TARGETS algo_algot1_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
