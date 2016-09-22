cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project (lzLib)

# 版本号
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

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
  "${PROJECT_SOURCE_DIR}/VersionLinux64.h.in"
  "${PROJECT_SOURCE_DIR}/include/Version.h"
  )
elseif (${SetPlatform} MATCHES "Linux32")
configure_file (
  "${PROJECT_SOURCE_DIR}/VersionLinux32.h.in"
  "${PROJECT_SOURCE_DIR}/include/Version.h"
  )
elseif (${SetPlatform} MATCHES "Windows64")
configure_file (
  "${PROJECT_SOURCE_DIR}/VersionWindows64.h.in"
  "${PROJECT_SOURCE_DIR}/include/Version.h"
  )
elseif (${SetPlatform} MATCHES "Windows32")
configure_file (
  "${PROJECT_SOURCE_DIR}/VersionWindows32.h.in"
  "${PROJECT_SOURCE_DIR}/include/Version.h"
  )
else()
configure_file (
  "${PROJECT_SOURCE_DIR}/VersionWindows32.h.in"
  "${PROJECT_SOURCE_DIR}/include/Version.h"
  )
endif()

# 设定安装目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

# 添加模块子文件夹  
add_subdirectory(modules) 
add_subdirectory(sample)

# 将二进制文件树添加到包含文件的搜索路径中，这样我们可以找到TutorialConfig.h
include_directories("${PROJECT_SOURCE_DIR}")

# 添加可执行文件
# add_library(Tutorial tutorial.cxx)
# ADD_LIBRARY(Tutorial SHARED tutorial.cxx)

install(FILES "${PROJECT_SOURCE_DIR}/include/Version.h" DESTINATION include)
install(FILES "${PROJECT_SOURCE_DIR}/include/lzTypes.h" DESTINATION include)

FILE(GLOB_RECURSE G3LOG_FILE_LIST "${PROJECT_SOURCE_DIR}/3rdparty/g3log/include/g3log/*.h*")
install(FILES ${G3LOG_FILE_LIST} DESTINATION include/g3log)