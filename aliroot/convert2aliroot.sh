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
DataVectorChannelized
Detector
DetectorConfigurationBase
DetectorConfigurationChannels
DetectorConfigurationsSet
DetectorConfigurationTracks
EventClassVariable
EventClassVariablesSet
HistogramBase
HistogramChannelized
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

for j in $listclasses; do
  mv $outputfolder/QnCorrections${j}.cxx $outputfolder/AliQnCorrections${j}.cxx
  mv $outputfolder/QnCorrections${j}.h $outputfolder/AliQnCorrections${j}.h
done

listC=`find $outputfolder/ -name '*.cxx'`
listH=`find $outputfolder/ -name '*.h'`

for k in $listC; do
  replacenames "$k"
done
for k in $listH; do
  replacenames "$k"
done



