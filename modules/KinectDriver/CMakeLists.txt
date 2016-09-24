set(TARGET_NAME lz_KinectDriver)
set(TARGET_NAME_STATIC lz_KinectDriverStatic)
set(SRC_DIR src)
set(BIN_DIR bin)

# 指定源代码
aux_source_directory(${SRC_DIR} SRC_LIST)
SUBDIRLIST(SUBDIRS src)
FOREACH(subdir ${SUBDIRS})
  FILE(GLOB_RECURSE SRC_FILE_LIST "src/${subdir}/*.c*")
  LIST(APPEND SRC_LIST ${SRC_FILE_LIST})
ENDFOREACH()


# 指定头文件目录
set(INC_DIR_LIST inc ${PROJECT_SOURCE_DIR}/include)

# modules文件夹下头文件
SUBDIRLIST(SUBDIRS inc)
FOREACH(subdir ${SUBDIRS})
  #ADD_SUBDIRECTORY(${subdir})
  LIST(APPEND INC_DIR_LIST inc/${subdir})
ENDFOREACH()
include_directories(${INC_DIR_LIST})

# 3rdparty文件夹下头文件
SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/3rdparty)
FOREACH(subdir ${SUBDIRS})
  #ADD_SUBDIRECTORY(${subdir})
  LIST(APPEND INC_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/include)
ENDFOREACH()
include_directories(${INC_DIR_LIST})

# 生成动态库目标
add_library(${TARGET_NAME} SHARED ${SRC_LIST})

# 生成静态库目标
add_library(${TARGET_NAME_STATIC} STATIC ${SRC_LIST})

# 指定静态库的输出名称
# set_target_properties(${TARGET_NAME_STATIC} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")

# 使动态库和静态库同时存在
set_target_properties(${TARGET_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${TARGET_NAME_STATIC} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 添加编译库
# FILE(GLOB_RECURSE LIB_FILE_LIST "GTest/src/*.c*")
# add_library(libgtest ${LIB_FILE_LIST})
# target_link_libraries(${TARGET_NAME} libgtest) 

# 添加静态库文件和目录
if (${SetPlatform} MATCHES "Linux64")
	##
elseif (${SetPlatform} MATCHES "Linux32")
	##
elseif (${SetPlatform} MATCHES "Windows64")
	
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/g3log/lib/x64)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
	
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/kinect20/lib/x64)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
	
elseif (${SetPlatform} MATCHES "Windows32")
	
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/g3log/lib/x86)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
	
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/kinect20/lib/x86)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
	
else()
	
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/g3log/lib/x86)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
	
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/kinect20/lib/x86)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
endif()

## none
SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/3rdparty)
FOREACH(subdir ${SUBDIRS})
# 指定库文件目录
if (${SetPlatform} MATCHES "Linux64")
	#MESSAGE(STATUS "platform: ${SetPlatform}, no link") 
	#link_directories(${LIB_DIR_LIST})
elseif (${SetPlatform} MATCHES "Linux32")
	#MESSAGE(STATUS "platform: ${SetPlatform}, no link") 
	#link_directories(${LIB_DIR_LIST})
elseif (${SetPlatform} MATCHES "Windows64")
	#set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/lib/x64)
	#link_directories(${LIB_DIR_LIST})
	#foreach(libs ${LIB_DIR_LIST})
	#	FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
	#	message(STATUS "lib ${LIB_LIST}")
	#	target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	#endforeach()
elseif (${SetPlatform} MATCHES "Windows32")
	#set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/lib/x86)
	#link_directories(${LIB_DIR_LIST})
	#foreach(libs ${LIB_DIR_LIST})
	#	FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
	#	message(STATUS "lib ${LIB_LIST}")
	#	target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	#endforeach()
else()
	#set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/lib/x86)
	#link_directories(${LIB_DIR_LIST})
	#foreach(libs ${LIB_DIR_LIST})
	#	FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
	#	message(STATUS "lib ${LIB_LIST}")
	#	target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	#endforeach()
endif()
ENDFOREACH()

install(TARGETS ${TARGET_NAME} ${TARGET_NAME_STATIC}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

# 添加需要引用的静态库
#foreach(incs ${INC_DIR_LIST})
#	FILE(GLOB_RECURSE INCLUDE_FILE_LIST "${incs}/*.h*")
#	install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/${TARGET_NAME})
#endforeach()

FILE(GLOB_RECURSE INCLUDE_FILE_LIST "inc/*.h*")
install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/${TARGET_NAME})
	
file (GLOB BIN_LIST "${BIN_DIR}/*.dll*")
install(FILES ${BIN_LIST} DESTINATION bin)