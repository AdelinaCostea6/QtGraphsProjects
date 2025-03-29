# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Ford_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Ford_autogen.dir\\ParseCache.txt"
  "Ford_autogen"
  )
endif()
