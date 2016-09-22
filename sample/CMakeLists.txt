# 宏
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

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

#添加所有的文件夹
FOREACH(subdir ${SUBDIRS})
  ADD_SUBDIRECTORY(${subdir})
  MESSAGE(STATUS "ADD_SUBDIRECTORY: ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}") 
ENDFOREACH()