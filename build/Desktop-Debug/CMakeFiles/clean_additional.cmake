# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/aircraft_hanoi_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/aircraft_hanoi_autogen.dir/ParseCache.txt"
  "aircraft_hanoi_autogen"
  )
endif()
