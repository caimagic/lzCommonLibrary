set(SRC_DIR src)
set(INC_DIR inc)
set(BIN_DIR bin)

if(USE_32BITS)
set(LIB_DIR lib/x86)
else()
set(LIB_DIR lib/x64)
endif(USE_32BITS)

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

FILE(GLOB_RECURSE LIB_LIST "${LIB_DIR}/*.lib")
FOREACH(src ${LIB_LIST})
MESSAGE( ${src} )
ENDFOREACH()

target_link_libraries(KinectDriver ${LIB_LIST})

install(TARGETS KinectDriver KinectDriverStatic
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

file (GLOB INCLUDE_FILE_LIST "inc/*.h*")
install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/KinectDriver)

file (GLOB BIN_LIST "${BIN_DIR}/*.dll*")
install(FILES ${BIN_LIST} DESTINATION bin)