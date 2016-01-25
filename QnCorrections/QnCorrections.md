QnCorrections framework description {#mainpage}
===================================

## Event classes
If you are interesting in doing Flow analysis most probably you want to classify your events under different event classes. For supporting that, the correction framework defines two classes: QnCorrectionEventClassVariable and QnCorrectionEventClassVariablesSet.

### QnCorrectionsEventClassVariable
Allows you to identify one variable you want to use to classify your events, their z vertex coordinate, for instance, and associate it a concrete binning and text label to be used when such variale is presented on the different histograms.

As you know all variables of interest whitin the correction framework own a unique Id. This Id is also passed when you instantiate a QnCorrectionEventClassVariable object.

The class incorporates a wide set of constructors that basically cover the different possibilities of declaring a concrete binning: explicit, uniform and with segments of different granularity.

Examples:
* constructor form an array of pairs, where the 1st element of each pair is the upper edge of a coarse bin, and the 2nd element is the number of fine bins inside the coarse bin. The 1st element of the 1st pair is the lowest value and the 2nd element of the first pair is the number of coarse bins plus one (i.e. the total number of pairs).
~~~{.cxx}
  Double_t VtxZaxes[][2] = { { -10.0, 4} , {-7.0, 1}, {7.0, 8}, {10.0, 1}};
  QnCorrectionsEventClassVariable * fpVertexZEventClass = new QnCorrectionsEventClassVariable(kVertexZ, VarNames[kVertexZ], VtxZaxes);
~~~
will build an event class variable associated to the kVertexZ variable and with an associated binning {-10.0, -7.0, -5.25, -3.5, -1.75, 0.0, 1.75, 3.5, 5.25, 7.0, 10.0}.
* constructor from number of bins and minimum and maximum values.
~~~{.cxx}
  QnCorrectionsEventClassVariable * fpCentralityEventClass = new QnCorrectionsEventClassVariable(kCentrality, VarNames[kCentrality], 10, 0.0, 100.0);
~~~

### QnCorrectionsEventClassVariablesSet
Encapsulates all variables you want to be used to classify your events, their z vertex coordinate and their multiplicities, for instance, as an array that contains the QnCorrectionEventClassVariable objects.

You are free to define an event classes variables set per detector configuration. This variables set will be incorporated to the different histograms the detector configuration will need to perform the intended corrections. The variables set will be the base for supporting the needed histogram multidimensionality.

Example:
* once you have built the previous variables you define the variables set and incorporate them
~~~{.cxx}
  const Int_t nEventClassesDimensions = 2;
  QnCorrectionsEventClassVariablesSet fEventClasses = QnCorrectionsEventClassVariablesSet(nEventClassesDimensions);
  fEventClasses[0] = fpVertexZEventClass;
  fEventClasses[1] = fpCentralityEventClass;
~~~
now you are able to access to your event classes variables and their access members
  * with an iterator 
~~~{.cxx}
  TObjArrayIter *myiter = new TObjArrayIter(&fEventClasses); // requieres a pointer that's what '&' does 
  QnCorrectionsEventClassVariable *nextVar = (QnCorrectionsEventClassVariable*) myiter->Next();

  while (nextVar) {
    printf("Variable id: %d\n  name: %s\n  bins: %f", nextVar->GetVariableId(), nextVar->GetVariableLabel(), nextVar->GetBinEdge(0));
    for (Int_t bin = 1; bin < nextVar->GetNBins() + 1; bin++) {
      printf(", %f", nextVar->GetBinEdge(bin));
    }
    printf("\n");
    nextVar = (QnCorrectionsEventClassVariable*) myiter->Next();
  }
~~~
  * or directly
~~~{.cxx}
  for (Int_t ixvar = 0; ixvar < fEventClasses.GetEntriesFast(); ixvar++) {
    printf("Variable id: %d\n  name: %s\n  bins: %f",
        fEventClasses.At(ixvar)->GetVariableId(),
        fEventClasses.At(ixvar)->GetVariableLabel(),
        fEventClasses.At(ixvar)->GetBinEdge(0));
    for (Int_t bin = 1; bin < fEventClasses.At(ixvar)->GetNBins() + 1; bin++) {
      printf(", %f", fEventClasses.At(ixvar)->GetBinEdge(bin));
    }
    printf("\n");
  }
~~~

## Histograms
The correction framework makes an extensive use of profile histograms in order to provide the proper scope to the different averages needed to elaborate the intended Q vector corrections. As was previouslly described one of the key components of the correction framework is its concept of multidimensional event classes: you decide to define your event classes based on a concrete set of variables usually greather than one. This goes straight on to multidimensional profiles. 

So far ROOT only supports up to tridimensional profiles, but we wanted to give more flexibility and, due to the fact we expect not that much bins per variable you will use to define your event classes, we decided to build multidimensional profiles functionality. With this aim we provide a base class, QnCorrectionsHistogramBase, that should not be instantiated, and which declares the whole interface for the correction framework histogram classes, a single multidimensional histogram profile class, QnCorrectionsProfile, a components oriented multidimensional histogram profile class, QnCorrectionsComponentsProfile, which provide support for both components, X and Y, for a set of selected harmonics, and a correlation components oriented multidimenional histogram profile class, QnCorrectionsCorrelationComponentsProfile, which provide support for the correlation components, XX, XY, YX and YY, for a set of selected harmonics. 

### QnCorrectionsHistogramBase
Is the base for the whole set of histogram classes and declares their interfaces for the basic methods. It is not oriented to be instantiated and as an additional development support will include run time messages that will orient you when you are not doing a proper use of its descendant classes.

### QnCorrectionsProfile
Implements a multidimensional histogram profile. You construct one of them passing as parameter an event classes variables set. Then you ask for the actual support histograms creation and then you use it as you will expect from a conventional histogram.

Example:
* based on the event classes variables set you had created above
~~~{.cxx}
    QnCorrectionsProfile *myProfile = new QnCorrectionsProfile("QnCorrectionsProfile", "myProfile", fEventClasses);
    myProfile->CreateProfileHistograms(myList); // the list should collect all histograms you want them going to a file latter on
~~~
* now your code will fill a data bank structure that the framework will receive with each singular event. This data bank of course includes the actual values of the variables you identified as the event classes variables. So, when is time to fill your profile you do it, as you expected,
~~~{.cxx}
    myProfile->Fill(varContainer, weight);
~~~
* accessing the content of your profile is straight forward, as you expected,
~~~{.cxx}
    Double_t myProfileBinContent = myProfile->GetBinContent(myProfile->GetBin(varContainer));
    Double_t myProfileBinError = myProfile->GetBinError(myProfile->GetBin(varContainer));
~~~

### QnCorrectionsComponentsProfile

### QnCorrectionsCorrelationComponentsProfile
