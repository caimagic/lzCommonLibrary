set(TARGET_NAME TestKinectDriver)
set(SRC_DIR src)
set(BIN_DIR bin)

# 指定源代码
aux_source_directory(${SRC_DIR} SRC_LIST)
SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/modules)
FOREACH(subdir ${SUBDIRS})
  FILE(GLOB_RECURSE SRC_FILE_LIST "${PROJECT_SOURCE_DIR}/modules/${subdir}/src/*.c*")
  LIST(APPEND SRC_LIST ${SRC_FILE_LIST})
ENDFOREACH()


# 指定头文件目录
set(INC_DIR_LIST inc ../GTest ../GTest/include ${PROJECT_SOURCE_DIR}/include )

# modules文件夹下头文件
SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/modules)
FOREACH(subdir ${SUBDIRS})
  #ADD_SUBDIRECTORY(${subdir})
  MESSAGE(STATUS "subdir: ${PROJECT_SOURCE_DIR}/modules/${subdir}") 
  LIST(APPEND INC_DIR_LIST ${PROJECT_SOURCE_DIR}/modules/${subdir}/inc)
ENDFOREACH()
include_directories(${INC_DIR_LIST})

# 3rd文件夹下头文件
SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/3rdparty)
FOREACH(subdir ${SUBDIRS})
  #ADD_SUBDIRECTORY(${subdir})
  MESSAGE(STATUS "subdir: ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}") 
  LIST(APPEND INC_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/include)
ENDFOREACH()
include_directories(${INC_DIR_LIST})


# 生成可执行文件
add_executable(${TARGET_NAME} ${SRC_LIST})

target_link_libraries(${TARGET_NAME} libgtest) 

# 添加静态库文件和目录
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
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/lib/x64)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
elseif (${SetPlatform} MATCHES "Windows32")
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/lib/x86)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
else()
	set(LIB_DIR_LIST ${PROJECT_SOURCE_DIR}/3rdparty/${subdir}/lib/x86)
	link_directories(${LIB_DIR_LIST})
	foreach(libs ${LIB_DIR_LIST})
		FILE(GLOB_RECURSE LIB_LIST "${libs}/*[^Static].lib")
		message(STATUS "lib ${LIB_LIST}")
		target_link_libraries(${TARGET_NAME} ${LIB_LIST})
	endforeach()
endif()
ENDFOREACH()

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)