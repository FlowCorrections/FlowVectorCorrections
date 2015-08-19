# FlowVectorCorrections
General framework for flow vector corrections


//////////////////////////////////////////////////////////////
//
//    Flow vector correction options:
//
//    
//    Qn Normalization method: 0: Calibration of Q/sqrt(M)
//                             1: Calibration of Q/M
//                             2: Calibration of Q/|Q|
//                             3: Calibration of Q
//
//    Data vector equalization method :  0: M/<M>
//                                       1: 1+(M-<M>)/sigma(M)
//
//    Channel list      : Array of channel numbers that are included in Q-vector calculation
//    Channel group list: Array of group numbers to identify group for each channel. Gain is equalized to the mean raw multiplicity of the group
//
//    Twist and Scaling method: 
//                              0: Double harmonic track wise (advisable for tracking detector)
//                              1: Double harmonic Q wise
//                              2: Correlations
//
//    Flow vector name : Name to give your Q-vector (has to be unique)
//
//    Correlation Q-vector names: The Q-vectors used to perform the twist and scaling with correlations 
//
//    
///////////////////////////////////////////////////////////////




