# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QTConsole_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QTConsole_autogen.dir\\ParseCache.txt"
  "QTConsole_autogen"
  )
endif()
