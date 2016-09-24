# 添加编译库
# 指定头文件目录
set(INC_DIR_LIST GTest GTest/include)
include_directories(${INC_DIR_LIST})

FILE(GLOB_RECURSE LIB_FILE_LIST "GTest/src/*.c*")
add_library(libgtest ${LIB_FILE_LIST})

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
  if (${subdir} MATCHES GTest)
	# none
	MESSAGE(STATUS "GTest not") 
  else()
	ADD_SUBDIRECTORY(${subdir})
	MESSAGE(STATUS "${subdir} do") 
  endif(${subdir} MATCHES GTest)
ENDFOREACH()