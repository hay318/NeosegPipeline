# This is the root ITK CMakeList file: 
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# This project is designed to be built outside the Insight source tree.
PROJECT(NeosegPipeline)

# Find Qt
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
INCLUDE(${QT_USE_FILE})

#Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
INCLUDE(${GenerateCLP_USE_FILE})

#Find ITK
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})


SET(LIBRARY_NAME CountNumberOfLabels)
SET(${LIBRARY_NAME}_SOURCES ${LIBRARY_NAME}.cxx ${LIBRARY_NAME}.h)
ADD_LIBRARY(${LIBRARY_NAME} ${${LIBRARY_NAME}_SOURCES})
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${ITK_LIBRARIES} )

INSTALL(TARGETS ${LIBRARY_NAME} ARCHIVE DESTINATION bin)

QT4_ADD_RESOURCES(RESOURCES resources.qrc)
QT4_WRAP_UI(UI_SOURCES Window.ui neosegParameters.ui ABCParameters.ui About.ui)
QT4_WRAP_CPP(MOC_SOURCES DerivedWindow.h MainScriptThread.h)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

GENERATECLP(NeosegPipeline_CLP NeosegPipeline.xml)

# NeosegPipeline #
SET(NeosegPipeline_SOURCE  main.cxx
                           NeosegPipelineTool.cxx 
                           DerivedWindow.cxx
                           XmlReader.cxx 
                           XmlWriter.cxx
                           Pipeline.cxx
                           PipelineParameters.cxx
                           AntsParameters.cxx
                           NeosegParameters.cxx
                           ExecutablePaths.cxx
                           LibraryPaths.cxx
                           Script.cxx
                           PreProcessingData.cxx
                           AtlasPopulationRegistration.cxx
                           AtlasRegistration.cxx
                           AtlasGeneration.cxx
                           ExistingAtlasRegistration.cxx
                           NeosegExecution.cxx
                           DtiRegistration.cxx
                           MainScriptThread.cxx
                           ABCExecution.cxx
)

SET(NeosegPipeline_HEADERS NeosegPipelineTool.h
                           DerivedWindow.h
                           About.h
                           XmlReader.h
                           XmlWriter.h
                           Pipeline.h
                           PipelineParameters.h
                           Atlas.h
                           Neo.h
                           AntsParameters.h
                           NeosegParameters.h
                           ExecutablePaths.h
                           LibraryPaths.h
                           Script.h
                           PreProcessingData.h
                           AtlasPopulationRegistration.h
                           AtlasRegistration.h
                           AtlasGeneration.h
                           ExistingAtlasRegistration.h
                           NeosegExecution.h
                           DtiRegistration.h
                           PriorProbability.h
                           TemplateImage.h
                           MainScriptThread.h
                           Origin.h
                           MinMax.h
                           ABCExecution.h
)

FILE(READ NeosegPipeline.xml var)

STRING(REGEX MATCH "<version>.*</version>" ext "${var}")
STRING(REPLACE "<version>" "" version_number ${ext} )
STRING(REPLACE "</version>" "" version_number ${version_number})

ADD_DEFINITIONS(-DNEOSEGPIPELINE_VERSION="${version_number}")

ADD_EXECUTABLE(NeosegPipeline ${NeosegPipeline_SOURCE} ${NeosegPipeline_HEADERS} ${MOC_SOURCES} ${UI_SOURCES} ${NeosegPipeline_CLP} ${RESOURCES})
TARGET_LINK_LIBRARIES(NeosegPipeline ${QT_LIBRARIES} ${ITK_LIBRARIES} CountNumberOfLabels)

INSTALL(TARGETS NeosegPipeline RUNTIME DESTINATION bin)





