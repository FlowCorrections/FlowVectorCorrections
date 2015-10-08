# this script prepares the FlowVectorCorrections package for insertion in AliROOT
# it prepends all file and class names with Ali
# $1 points to the source directory
# $2 points to the target directory

inputfolder=$1
outputfolder=$2

if [ $# -lt 1 ]; then
  inputfolder=../QnCorrections
fi
if [ $# -lt 2 ]; then
  outputfolder=AliQnCorrections
fi

rsync -av $inputfolder/ $outputfolder

listclasses="Manager Axes DataVector QnVector Cuts Steps Histograms Configuration Constants"

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

replacenames ()
{
  file=$1
  for j in $listclasses; do
    sed -i "s/QnCorrections${j}/AliQnCorrections${j}/g" $file
  done
  sed -i "s/QNCORRECTIONS/ALIQNCORRECTIONS/g" $file
}


