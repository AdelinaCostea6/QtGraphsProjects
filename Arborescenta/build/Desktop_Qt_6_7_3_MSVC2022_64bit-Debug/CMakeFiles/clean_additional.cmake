# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Arborescenta_autogen"
  "CMakeFiles\\Arborescenta_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Arborescenta_autogen.dir\\ParseCache.txt"
  )
endif()
