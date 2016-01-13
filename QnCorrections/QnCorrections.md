QnCorrections framework description {#mainpage}
===================================

## Event classes
If you are interesting in doing Flow analysis most probably you want to classify your events under different event classes. For supporting that, the correction framework defines two classes: QnCorrectionEventClassVariable and QnCorrectionEventClassVariablesSet.

### QnCorrectionEventClassVariable
Allows you to identify one variable you want to use to classify your events, their z vertex coordinate, for instance, and associate it a concrete binning and text label to be used when such variale is presented on the different histograms.

As you know all variables of interest whitin the correction framework own a unique Id. This Id is also passed when you instantiate a QnCorrectionEventClassVariable object.

The class incorporates a wide set of constructors that basically cover the different possibilities of declaring a concrete binning: explicit, uniform and with segments of different granularity. 

### QnCorrectionEventClassVariablesSet
Encapsulates all variables you want to be used to classify your events, their z vertex coordinate and their multiplicities, for instance, as an array that contains the QnCorrectionEventClassVariable objects.

You are free to define an event classes variables set per detector configuration. This variables set will be incorporated to the different histograms the detector configuration will need to perform the intended corrections. The variables set will be the base for supporting the needed histogram multidimensionality.