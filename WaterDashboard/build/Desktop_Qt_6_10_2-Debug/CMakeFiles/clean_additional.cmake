# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appWaterDashboard_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appWaterDashboard_autogen.dir/ParseCache.txt"
  "appWaterDashboard_autogen"
  )
endif()
