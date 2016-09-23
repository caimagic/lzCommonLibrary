set(TARGET_NAME lzTest)
set(SRC_DIR src)
set(BIN_DIR bin)

# 指定代码目录
aux_source_directory(${SRC_DIR} SRC_LIST)
# LIST(APPEND SRC_LIST ..)

# 指定头文件目录
set(INC_DIR_LIST inc ${PROJECT_SOURCE_DIR}/modules/GTest/include ${PROJECT_SOURCE_DIR}/modules/GTest ${PROJECT_BINARY_DIR}/install)
include_directories(${INC_DIR_LIST})

# 指定库文件目录

if (${SetPlatform} MATCHES "Linux64")
	#MESSAGE(STATUS "platform: ${SetPlatform}, no link") 
	#link_directories(${LIB_DIR_LIST})
elseif (${SetPlatform} MATCHES "Linux32")
	#MESSAGE(STATUS "platform: ${SetPlatform}, no link") 
	#link_directories(${LIB_DIR_LIST})
elseif (${SetPlatform} MATCHES "Windows64")
	set(LIB_DIR_LIST ${PROJECT_BINARY_DIR}/install/lib)
	link_directories(${LIB_DIR_LIST})
elseif (${SetPlatform} MATCHES "Windows32")
	set(LIB_DIR_LIST ${PROJECT_BINARY_DIR}/install/lib)
	link_directories(${LIB_DIR_LIST})
else()
	set(LIB_DIR_LIST ${PROJECT_BINARY_DIR}/install/lib)
	link_directories(${LIB_DIR_LIST})
endif()

# 添加源代码文件
FILE(GLOB_RECURSE LIB_FILE_LIST "${PROJECT_SOURCE_DIR}/modules/GTest/src/*.c*")
add_library(libgtest ${LIB_FILE_LIST})

# 生成可执行文件
add_executable(${TARGET_NAME} ${SRC_LIST})

target_link_libraries(${TARGET_NAME} libgtest) 

# 添加需要引用的静态库
foreach(libs ${LIB_DIR_LIST})
FILE(GLOB_RECURSE LIB_LIST "${libs}/*.lib")
target_link_libraries(${TARGET_NAME} ${LIB_LIST})
endforeach()

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)