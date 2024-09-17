# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\amplifiers_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\amplifiers_autogen.dir\\ParseCache.txt"
  "amplifiers_autogen"
  )
endif()
