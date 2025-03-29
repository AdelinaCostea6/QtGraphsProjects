# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Kruskal_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Kruskal_autogen.dir\\ParseCache.txt"
  "Kruskal_autogen"
  )
endif()
