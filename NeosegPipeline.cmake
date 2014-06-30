# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8)

# This project is designed to be built outside the Insight source tree.
project(NeosegPipeline-AllExecutables)

include(${CMAKE_CURRENT_SOURCE_DIR}/Common.cmake)

if( BUILD_NeosegPipeline )
  add_subdirectory( NeosegPipeline )
endif()

if( BUILD_WeightedLabelsAverage )
  add_subdirectory( WeightedLabelsAverage )
endif()

if( BUILD_SpreadFA )
  add_subdirectory( SpreadFA )
endif()

if( BUILD_ReassignWhiteMatter )
  add_subdirectory( ReassignWhiteMatter )
endif()

