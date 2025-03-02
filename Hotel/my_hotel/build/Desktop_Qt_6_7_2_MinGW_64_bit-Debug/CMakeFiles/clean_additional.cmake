# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\my_hotel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\my_hotel_autogen.dir\\ParseCache.txt"
  "my_hotel_autogen"
  )
endif()
