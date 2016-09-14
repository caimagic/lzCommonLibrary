set(TARGET_NAME lz_KinectDriver)
set(TARGET_NAME_STATIC lz_KinectDriverStatic)
set(SRC_DIR src)
set(INC_DIR inc)
set(BIN_DIR bin)

if(USE_32BITS)
set(LIB_DIR_LIST lib/x86 ../Core/lib/x86)
else()
set(LIB_DIR_LIST lib/x64 ../Core/lib/x64)
endif(USE_32BITS)


# 指定代码目录
aux_source_directory(${SRC_DIR} SRC_LIST)
# LIST(APPEND SRC_LIST ..)

# 指定头文件目录
include_directories(
	${INC_DIR}
	${PROJECT_SOURCE_DIR}/include
	../Core/inc)

link_directories(${LIB_DIR_LIST})

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

install(TARGETS ${TARGET_NAME} ${TARGET_NAME_STATIC}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

file (GLOB INCLUDE_FILE_LIST "inc/*.h*")
install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/${TARGET_NAME})

file (GLOB BIN_LIST "${BIN_DIR}/*.dll*")
install(FILES ${BIN_LIST} DESTINATION bin)