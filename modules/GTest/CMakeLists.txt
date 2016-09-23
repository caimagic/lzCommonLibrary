set(TARGET_NAME gtest)
set(TARGET_NAME_STATIC gtestStatic)
set(SRC_DIR src)
set(BIN_DIR bin)
set(INC_DIR_LIST include ./)

# 指定代码目录
aux_source_directory(${SRC_DIR} SRC_LIST)
# LIST(APPEND SRC_LIST ..)

# 指定头文件目录
include_directories(${INC_DIR_LIST})

# 指定库文件目录
if (${SetPlatform} MATCHES "Linux64")
	#MESSAGE(STATUS "platform: ${SetPlatform}, no link") 
elseif (${SetPlatform} MATCHES "Linux32")
	#MESSAGE(STATUS "platform: ${SetPlatform}, no link") 
elseif (${SetPlatform} MATCHES "Windows64")
	#set(LIB_DIR_LIST lib/x64 ../../3rdparty/g3log/lib/x64)
	#link_directories(${LIB_DIR_LIST})
elseif (${SetPlatform} MATCHES "Windows32")
	#set(LIB_DIR_LIST lib/x86 ../../3rdparty/g3log/lib/x86)
	#link_directories(${LIB_DIR_LIST})
else()
	#set(LIB_DIR_LIST lib/x86 ../../3rdparty/g3log/lib/x86)
	#link_directories(${LIB_DIR_LIST})
endif()

# 生成动态库目标
add_library(${TARGET_NAME} SHARED ${SRC_LIST})

# 生成静态库目标
add_library(${TARGET_NAME_STATIC} STATIC ${SRC_LIST})

# 指定静态库的输出名称
# set_target_properties(${TARGET_NAME_STATIC} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")

# 使动态库和静态库同时存在
set_target_properties(${TARGET_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${TARGET_NAME_STATIC} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 添加需要引用的静态库
foreach(libs ${LIB_DIR_LIST})
FILE(GLOB_RECURSE LIB_LIST "${libs}/*.lib")
target_link_libraries(${TARGET_NAME} ${LIB_LIST})
endforeach()

# 设定安装目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

install(TARGETS ${TARGET_NAME} ${TARGET_NAME_STATIC}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

# 安装头文件
#foreach(incs ${INC_DIR_LIST})
#	FILE(GLOB_RECURSE INCLUDE_FILE_LIST "${incs}/*.h*")
#	install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/${TARGET_NAME})
#endforeach()

install(DIRECTORY include/ DESTINATION include/${TARGET_NAME})

#FILE(GLOB_RECURSE INCLUDE_FILE_LIST "include/*.h*")
#install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/${TARGET_NAME})

file (GLOB BIN_LIST "${BIN_DIR}/*.dll*")
install(FILES ${BIN_LIST} DESTINATION bin)