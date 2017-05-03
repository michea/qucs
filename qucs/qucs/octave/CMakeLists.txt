# octave scripts

SET(BASICS
getQucsVariable.m	plotCartesian2D.m	plotFFT2V.m
loadQucsDataSet.m	plotCartesian2D2V.m	plotTimeFFT.m
loadQucsVariable.m	plotFFT.m		showQucsDataSet.m
plotCartesianN2D2V.m loadQucsNDataSet.m parseQucsNDataSet.m
createPulseNDataSet.m
)

INSTALL(FILES ${BASICS} DESTINATION share/qucs/octave/)

