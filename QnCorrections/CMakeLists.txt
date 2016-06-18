cmake_minimum_required(VERSION 2.8)


project(FLOWVECTOR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake/)

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})


set (SOURCES
  QnCorrectionsCorrectionOnInputData.cxx
  QnCorrectionsCorrectionOnQvector.cxx
  QnCorrectionsCorrectionsSetOnInputData.cxx
  QnCorrectionsCorrectionsSetOnQvector.cxx
  QnCorrectionsCorrectionStepBase.cxx
  QnCorrectionsCutAbove.cxx
  QnCorrectionsCutBelow.cxx
  QnCorrectionsCutOutside.cxx
  QnCorrectionsCutsBase.cxx
  QnCorrectionsCutSetBit.cxx
  QnCorrectionsCutsSet.cxx
  QnCorrectionsCutValue.cxx
  QnCorrectionsCutWithin.cxx
  QnCorrectionsDataVector.cxx
  QnCorrectionsDataVectorChannelized.cxx
  QnCorrectionsDetector.cxx
  QnCorrectionsDetectorConfigurationBase.cxx
  QnCorrectionsDetectorConfigurationChannels.cxx
  QnCorrectionsDetectorConfigurationsSet.cxx
  QnCorrectionsDetectorConfigurationTracks.cxx
  QnCorrectionsEventClassVariable.cxx
  QnCorrectionsEventClassVariablesSet.cxx
  QnCorrectionsHistogram.cxx
  QnCorrectionsHistogramBase.cxx
  QnCorrectionsHistogramChannelized.cxx
  QnCorrectionsHistogramChannelizedSparse.cxx
  QnCorrectionsHistogramSparse.cxx
  QnCorrectionsInputGainEqualization.cxx
  QnCorrectionsLog.cxx
  QnCorrectionsManager.cxx
  QnCorrectionsProfile.cxx
  QnCorrectionsProfileChannelized.cxx
  QnCorrectionsProfileChannelizedIngress.cxx
  QnCorrectionsProfileComponents.cxx
  QnCorrectionsProfileCorrelationComponents.cxx
  QnCorrectionsProfileCorrelationComponentsHarmonics.cxx
  QnCorrectionsQnVector.cxx
  QnCorrectionsQnVectorBuild.cxx
  QnCorrectionsQnVectorAlignment.cxx
  QnCorrectionsQnVectorRecentering.cxx
)

string(REPLACE ".cxx" ".h" HEADERS "${SRCS}")

ROOT_GENERATE_DICTIONARY(G__FlowVector ${HEADERS} LINKDEF QnLinkDef.h)


#---Create a shared library with generated dictionary
add_library(FlowVector SHARED ${SOURCES} G__FlowVector.cxx)
target_link_libraries(FlowVector ${ROOT_LIBRARIES})

