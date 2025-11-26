
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
add_subdirectory(demo)

############################################################
# Step4. Install 
############################################################
# Library
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib${PROJECT_NAME})
# Header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/  DESTINATION include)
# Config files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/config/  DESTINATION config)

# Config files
#==================生成目标文件的xxxTarget.cmake======================
# 会将生成的库libXXX.so安装到${CMAKE_INSTALL_PREFIX}/lib下
install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
	ARCHIVE DESTINATION lib
 	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
# 生成 xxxTargets.cmake文件
install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	DESTINATION lib/cmake/${PROJECT_NAME}
)

