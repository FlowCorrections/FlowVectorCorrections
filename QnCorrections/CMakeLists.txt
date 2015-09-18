cmake_minimum_required(VERSION 2.8)


project(FLOWVECTOR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake/)

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})


set (HEADERS
QnCorrectionsConstants.h 
QnCorrectionsAxes.h 
QnCorrectionsConfiguration.h 
QnCorrectionsCuts.h 
QnCorrectionsDataVector.h 
QnCorrectionsHistograms.h 
QnCorrectionsManager.h 
QnCorrectionsQnVector.h 
QnCorrectionsSteps.h
)


set (SOURCES
QnCorrectionsAxes.cxx 
QnCorrectionsConfiguration.cxx 
QnCorrectionsCuts.cxx 
QnCorrectionsDataVector.cxx 
QnCorrectionsHistograms.cxx 
QnCorrectionsManager.cxx 
QnCorrectionsQnVector.cxx 
QnCorrectionsSteps.cxx
)

ROOT_GENERATE_DICTIONARY(G__FlowVector ${HEADERS} LINKDEF PWGPPevcharQnLinkDef.h)


#---Create a shared library with generated dictionary
add_library(FlowVector SHARED ${SOURCES} G__FlowVector.cxx)
target_link_libraries(FlowVector ${ROOT_LIBRARIES})

