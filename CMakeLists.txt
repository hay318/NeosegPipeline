# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8)

# This project is designed to be built outside the Insight source tree.
project(NeosegPipeline)

# Find Qt
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
INCLUDE(${QT_USE_FILE})

# Find Slicer3
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

# Find GenerateCLP
FIND_PACKAGE(GenerateCLP REQUIRED)
INCLUDE(${GenerateCLP_USE_FILE})

# MotherWindow.h generation
QT4_WRAP_UI(UI_SOURCES Window.ui)

# Meta Object Compiler
QT4_WRAP_CPP(MOC_SOURCES DerivedWindow.h MainScriptThread.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

GENERATECLP(NeosegPipeline_CLP NeosegPipeline.xml) 

SET(NeosegPipeline_SOURCE main.cxx NeosegPipelineTool.cxx DerivedWindow.cxx XmlReader.cxx XmlWriter.cxx Pipeline.cxx PipelineParameters.cxx AntsParameters.cxx NeosegParameters.cxx ExecutablePaths.cxx Script.cxx PreProcessingData.cxx AtlasRegistration.cxx AtlasGeneration.cxx NeosegExecution.cxx DtiRegistration.cxx MainScriptThread.cxx)
SET(NeosegPipeline_HEADERS NeosegPipelineTool.h DerivedWindow.h XmlReader.h XmlWriter.h Pipeline.h  PipelineParameters.h Atlas.h Neo.h AntsParameters.h NeosegParameters.h ExecutablePaths.h Script.h PreProcessingData.h AtlasRegistration.h AtlasGeneration.h NeosegExecution.h DtiRegistration.h PriorProbability.h TemplateImage.h MainScriptThread.h)

add_executable(NeosegPipeline ${NeosegPipeline_SOURCE} ${NeosegPipeline_HEADERS} ${MOC_SOURCES} ${UI_SOURCES} ${NeosegPipeline_CLP})

TARGET_LINK_LIBRARIES(NeosegPipeline ${QT_LIBRARIES})
