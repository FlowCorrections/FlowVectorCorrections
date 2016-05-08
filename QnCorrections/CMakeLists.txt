cmake_minimum_required(VERSION 2.8)


project(FLOWVECTOR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake/)

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})


set (HEADERS
  QnCorrectionsCorrectionSteps.h
  QnCorrectionsCuts.h
  QnCorrectionsDataVector.h
  QnCorrectionsDetector.h
  QnCorrectionsEventClasses.h
  QnCorrectionsHistograms.h
  QnCorrectionsInputGainEqualization.h
  QnCorrectionsLog.h
  QnCorrectionsManager.h
  QnCorrectionsQnVector.h
  QnCorrectionsQnVectorRecentering.h
  QnCorrectionsQnVectorAlignment.h
)


set (SOURCES
  QnCorrectionsCorrectionSteps.cxx
  QnCorrectionsCuts.cxx
  QnCorrectionsDataVector.cxx
  QnCorrectionsDetector.cxx
  QnCorrectionsEventClasses.cxx
  QnCorrectionsHistograms.cxx
  QnCorrectionsInputGainEqualization.cxx
  QnCorrectionsLog.cxx
  QnCorrectionsManager.cxx
  QnCorrectionsQnVector.cxx
  QnCorrectionsQnVectorRecentering.cxx
  QnCorrectionsQnVectorAlignment.cxx
)

ROOT_GENERATE_DICTIONARY(G__FlowVector ${HEADERS} LINKDEF QnLinkDef.h)


#---Create a shared library with generated dictionary
add_library(FlowVector SHARED ${SOURCES} G__FlowVector.cxx)
target_link_libraries(FlowVector ${ROOT_LIBRARIES})

