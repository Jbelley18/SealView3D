# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "3DViewer_autogen"
  "CMakeFiles\\3DViewer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\3DViewer_autogen.dir\\ParseCache.txt"
  )
endif()
