cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project (sample)

# 版本号
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(TARGET_NAME mysample)
set(SRC_DIR src)

# 指定头文件目录
set(INC_DIR_LIST inc)

# 确定平台环境和编译位数
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	if (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
		set (SetPlatform Linux64)
	else()
		set (SetPlatform Linux32)
	endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	if (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
		set (SetPlatform Windows64)
	else()
		set (SetPlatform Windows32)
	endif()
endif()

# 配置一个头文件，通过它向源代码中传递一些CMake设置。
if (${SetPlatform} MATCHES "Linux64")
configure_file (
  "${PROJECT_SOURCE_DIR}/version/VersionLinux64.h.in"
  "${PROJECT_SOURCE_DIR}/version/Version.h"
  )
elseif (${SetPlatform} MATCHES "Linux32")
configure_file (
  "${PROJECT_SOURCE_DIR}/version/VersionLinux32.h.in"
  "${PROJECT_SOURCE_DIR}/version/Version.h"
  )
elseif (${SetPlatform} MATCHES "Windows64")
configure_file (
  "${PROJECT_SOURCE_DIR}/version/VersionWindows64.h.in"
  "${PROJECT_SOURCE_DIR}/version/Version.h"
  )
elseif (${SetPlatform} MATCHES "Windows32")
configure_file (
  "${PROJECT_SOURCE_DIR}/version/VersionWindows32.h.in"
  "${PROJECT_SOURCE_DIR}/version/Version.h"
  )
else()
configure_file (
  "${PROJECT_SOURCE_DIR}/version/VersionWindows32.h.in"
  "${PROJECT_SOURCE_DIR}/version/Version.h"
  )
endif()

# 设定安装目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

# 宏 遍历文件夹
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# 指定源代码
aux_source_directory(${SRC_DIR} SRC_LIST)
SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/modules)
FOREACH(subdir ${SUBDIRS})
  FILE(GLOB_RECURSE SRC_FILE_LIST "${PROJECT_SOURCE_DIR}/modules/${subdir}/src/*.c*")
  LIST(APPEND SRC_LIST ${SRC_FILE_LIST})
ENDFOREACH()

# 生成可执行文件
add_executable(${TARGET_NAME} ${SRC_LIST})

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(FILES "${PROJECT_SOURCE_DIR}/version/Version.h" DESTINATION include)
FILE(GLOB_RECURSE INCLUDE_FILE_LIST "${INC_DIR_LIST}/*.h*")
install(FILES ${INCLUDE_FILE_LIST} DESTINATION include/${TARGET_NAME})
