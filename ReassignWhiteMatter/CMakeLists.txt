# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8)

# This project is designed to be built outside the Insight source tree.
project(ReassignWhiteMatter)

# Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
INCLUDE(${GenerateCLP_USE_FILE})

# Find ITK
find_package(ITK REQUIRED)
INCLUDE (${ITK_USE_FILE})

GENERATECLP(ReassignWhiteMatter_CLP ReassignWhiteMatter.xml)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(ConnectedComponents_SOURCE main.cxx Parameters.cxx ParametersXMLFile.cxx)
SET(ConnectedComponents_HEADERS Parameters.h ParametersXMLFile.h)

add_executable(ReassignWhiteMatter ${ConnectedComponents_SOURCE} ${ReassignWhiteMatter_CLP})

TARGET_LINK_LIBRARIES(ReassignWhiteMatter ${ITK_LIBRARIES})

install(TARGETS ReassignWhiteMatter RUNTIME DESTINATION bin)
