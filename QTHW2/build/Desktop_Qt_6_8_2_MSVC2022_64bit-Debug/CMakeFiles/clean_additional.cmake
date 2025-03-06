# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QTHW2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QTHW2_autogen.dir\\ParseCache.txt"
  "QTHW2_autogen"
  )
endif()
