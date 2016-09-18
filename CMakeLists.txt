cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project (lzLib)

# 版本号
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

# 配置一个头文件，通过它向源代码中传递一些CMake设置。
configure_file (
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_SOURCE_DIR}/include/Version.h"
  )

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)
  
option(USE_32BITS "Compile for 64bits processors" ON)

# 添加模块子文件夹  
add_subdirectory(modules)

# 将二进制文件树添加到包含文件的搜索路径中，这样我们可以找到TutorialConfig.h
include_directories("${PROJECT_SOURCE_DIR}")

# 添加可执行文件
# add_library(Tutorial tutorial.cxx)
# ADD_LIBRARY(Tutorial SHARED tutorial.cxx)

install(FILES "${PROJECT_SOURCE_DIR}/include/Version.h" DESTINATION include)
install(FILES "${PROJECT_SOURCE_DIR}/include/lzTypes.h" DESTINATION include)

FILE(GLOB_RECURSE G3LOG_FILE_LIST "${PROJECT_SOURCE_DIR}/3rdparty/g3log/include/g3log/*.h*")
install(FILES ${G3LOG_FILE_LIST} DESTINATION include/g3log)