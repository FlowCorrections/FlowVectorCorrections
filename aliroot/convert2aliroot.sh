#!/bin/bash
# this script prepares the FlowVectorCorrections package for insertion in AliROOT
# it prepends all file and class names with Ali
# $1 points to the source directory
# $2 points to the target directory


replacenames ()
{
  file=$1
  for j in $listclasses; do
    sed -i "s/QnCorrections${j}/AliQnCorrections${j}/g" $file
  done
  sed -i "s/QNCORRECTIONS/ALIQNCORRECTIONS/g" $file
}

replacetracingnames ()
{
  file=$1
  for j in $tracingnames; do
    sed -i "s/QnCorrections${j}/Ali${j}/g" $file
  done
}


inputfolder=$1
outputfolder=$2

if [ $# -lt 1 ]; then
  inputfolder=../QnCorrections
fi
if [ $# -lt 2 ]; then
  outputfolder=AliQnCorrections
fi

rsync -av $inputfolder/ $outputfolder

listclasses="CorrectionOnInputData
CorrectionOnQvector
CorrectionsSetOnInputData
CorrectionsSetOnQvector
CorrectionStepBase
CutAbove
CutsBase
CutBelow
CutSetBit
CutOutside
CutsSet
CutValue
CutWithin
DataVector
Detector
EventClassVariable
Histogram
InputGainEqualization
Manager
Profile
QnVector"

listclassesfiles="CorrectionOnInputData
CorrectionOnQvector
CorrectionsSetOnInputData
CorrectionsSetOnQvector
CorrectionStepBase
CutAbove
CutsBase
CutBelow
CutSetBit
CutOutside
CutsSet
CutValue
CutWithin
DataVector
DataVectorChannelized
Detector
DetectorConfigurationBase
DetectorConfigurationChannels
DetectorConfigurationsSet
DetectorConfigurationTracks
EventClassVariable
EventClassVariablesSet
Histogram
HistogramBase
HistogramChannelized
HistogramChannelizedSparse
HistogramSparse
InputGainEqualization
Manager
Profile
ProfileChannelized
ProfileChannelizedIngress
ProfileComponents
ProfileCorrelationComponents
ProfileCorrelationComponentsHarmonics
QnVector
QnVectorBuild
QnVectorRecentering
QnVectorAlignment"

for j in $listclassesfiles; do
  mv $outputfolder/QnCorrections${j}.cxx $outputfolder/AliQnCorrections${j}.cxx
  mv $outputfolder/QnCorrections${j}.h $outputfolder/AliQnCorrections${j}.h
done

# remove the framework tracing support
rm $outputfolder/QnCorrectionsLog.*

listC=`find $outputfolder/ -name '*.cxx'`
listH=`find $outputfolder/ -name '*.h'`
listTXT=`find $outputfolder/ -name '*.txt'`
listMD=`find $outputfolder/ -name '*.md'`

for k in $listC; do
  replacenames "$k"
done
for k in $listH; do
  replacenames "$k"
done
for k in $listTXT; do
  replacenames "$k"
done
for k in $listMD; do
  replacenames "$k"
done

# handle now the tracing fucntions
tracingnames="Log Info Warning Error Fatal"

# switch to ALICE logging
for k in $listC; do
  replacetracingnames "$k"
done
for k in $listH; do
  replacetracingnames "$k"
done

