cmake_minimum_required(VERSION 2.8)
project(AlgoSDKDemo)

# Set C/C++ Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -O0")

############################################################
# Add Dependicies
############################################################

# AlgoSDK
set(AlgoSDK_INCLUDE_DIRS ../include)
set(AlgoSDK_LIBRARY_DIRS ${CMAKE_BINARY_DIR})
include_directories(${AlgoSDK_INCLUDE_DIRS})
link_directories(${AlgoSDK_LIBRARY_DIRS})
message(STATUS "AlgoSDK_INCLUDE_DIRS = ${AlgoSDK_INCLUDE_DIRS}")
message(STATUS "AlgoSDK_LIBRARY_DIRS = ${AlgoSDK_LIBRARY_DIRS}")
############################################################
# Add Executable
############################################################

# AlgoDemo_AlgoT1 using NET
add_executable(AlgoDemo_AlgoT1 AlgoDemo_AlgoT1.cpp)
target_link_libraries(AlgoDemo_AlgoT1 algot1_sdk pthread)

# AlgoDemo_AlgoT1 using Serial
add_executable(AlgoDemo_AlgoT1COM AlgoDemo_AlgoT1COM.cpp)
target_link_libraries(AlgoDemo_AlgoT1COM algot1_sdk pthread)


############################################################
# Install 
############################################################
# Library
install(TARGETS AlgoDemo_AlgoT1 DESTINATION bin)
install(TARGETS AlgoDemo_AlgoT1COM DESTINATION bin)