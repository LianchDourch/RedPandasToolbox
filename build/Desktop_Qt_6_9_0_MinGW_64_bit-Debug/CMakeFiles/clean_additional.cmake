# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RedPandasToolbox_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RedPandasToolbox_autogen.dir\\ParseCache.txt"
  "RedPandasToolbox_autogen"
  )
endif()
