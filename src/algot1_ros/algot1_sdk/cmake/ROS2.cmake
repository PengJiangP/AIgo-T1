list(APPEND CMAKE_PREFIX_PATH "/opt/ros/foxy")

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

include_directories(
  include
  ${PROJECT_SOURCE_DIR}
)

set(DEPENDENCIES
  "geometry_msgs"
  "nav_msgs"
  "std_msgs"
  "tf2"
  "tf2_ros"
)

## Main body
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/algot1 SRC_LIST1)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/rapidjson SRC_LIST2)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/parser SRC_LIST3)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/uliti SRC_LIST4)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/stream SRC_LIST5)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/uliti)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3} ${SRC_LIST4} ${SRC_LIST5}) 
target_link_libraries(${PROJECT_NAME} ${thirdparty_libraries})
ament_target_dependencies(${PROJECT_NAME} tf2 tf2_ros std_msgs nav_msgs sensor_msgs geometry_msgs)

 ## Add catkin install targets
 install(TARGETS ${PROJECT_NAME}
 RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
# Header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/  DESTINATION include/${PROJECT_NAME})
# Config files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/config/  DESTINATION config)

ament_package()