# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Tema3Graf_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Tema3Graf_autogen.dir\\ParseCache.txt"
  "Tema3Graf_autogen"
  )
endif()
