# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8.3)

# This project is designed to be built outside the Insight source tree.
project(SpreadFA)

# Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
INCLUDE(${GenerateCLP_USE_FILE})

# Find ITK
find_package(ITK REQUIRED)
INCLUDE (${ITK_USE_FILE})

GENERATECLP(SpreadFA_CLP SpreadFA.xml)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(SpreadFA_SOURCE main.cxx)
SET(SpreadFA_HEADERS itkSpreadFA.h)

add_executable(SpreadFA ${SpreadFA_SOURCE} ${SpreadFA_CLP})

TARGET_LINK_LIBRARIES(SpreadFA ${ITK_LIBRARIES})

install(TARGETS SpreadFA RUNTIME DESTINATION bin)
