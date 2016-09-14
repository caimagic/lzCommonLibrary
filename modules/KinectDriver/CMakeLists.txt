set(SRC_DIR src)
set(INC_DIR inc)
set(LIB_DIR lib)

# 指定代码目录
aux_source_directory(${SRC_DIR} SRC_LIST)
# LIST(APPEND SRC_LIST ..)

# 指定头文件目录
include_directories(
	${INC_DIR}
	${PROJECT_SOURCE_DIR}/include)

link_directories(${LIB_DIR})

# 生成动态库目标
add_library(KinectDriver SHARED ${SRC_LIST})

# 生成静态库目标
add_library(KinectDriverStatic STATIC ${SRC_LIST})

# 指定静态库的输出名称
set_target_properties(KinectDriverStatic PROPERTIES OUTPUT_NAME "KinectDriver")

# 使动态库和静态库同时存在
set_target_properties(KinectDriver PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(KinectDriverStatic PROPERTIES CLEAN_DIRECT_OUTPUT 1)

install(TARGETS KinectDriver KinectDriverStatic
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

file (GLOB INCLUDE_FILE "inc/*.h*")
install(FILES ${INCLUDE_FILE} DESTINATION include/KinectDriver)