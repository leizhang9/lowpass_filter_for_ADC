#define BOOST_TEST_MODULE ADC

#include <boost/test/unit_test.hpp>

#include<math.h>
#include<stdlib.h>
#include "adc.c"

BOOST_AUTO_TEST_CASE(customTest1) // Check that frequencies below 50Hz are NOT filtered out
{
	adc_init();
    AdcRawData rawData = {0};
    float frequency = 30; // below 50Hz
    int numSamples = 10000;
    float amplitude = 100;
    int sampleRate = 10000;
    
    bool isnotFiltered = false;
	for (int i = 0; i < numSamples; i++) {
		float time = i / (float)sampleRate;
        rawData.values[AdcChannel_U1] = (uint16_t)(amplitude * (sin(2 * 3.1415 * frequency * time) + 1) ); //+1 to make it positive
        adc_processRawData(&rawData);
        
    	if ((i > numSamples - 334) && (i < numSamples) ) { //over one period 
        	if (abs(filteredValues[AdcChannel_U1] - amplitude) > 0.8*amplitude) { 
            	isnotFiltered = true;
            	break;
            }
        }
    }
    BOOST_CHECK_MESSAGE(isnotFiltered, "Low frequency signal is filtered.");
}


BOOST_AUTO_TEST_CASE(customTest2) // Check that frequencies over 1kHz are filtered out
{
	adc_init();
    AdcRawData rawData = {0};
    float frequency = 2000; // over 1kHz
    int numSamples = 10000;
    float amplitude = 100;
    int sampleRate = 10000;
    
    bool isFiltered = true;
	for (int i = 0; i < numSamples; i++) {
		float time = i / (float)sampleRate;
        rawData.values[AdcChannel_U2] = (uint16_t)(amplitude * (sin(2 * 3.1415 * frequency * time) + 1) ); //+1 to make it positive
        adc_processRawData(&rawData);
        
    	if ((i > numSamples - 6) && (i < numSamples) ) { //over one period 
        	if (abs(filteredValues[AdcChannel_U2] - amplitude) > 0.1*amplitude) { 
        		printf("filteredvalue: %f", filteredValues[AdcChannel_U2]);
            	isFiltered = false;
            	break;
            }
        }
    }
    BOOST_CHECK_MESSAGE(isFiltered, "High frequency signal is not filtered.");
}


BOOST_AUTO_TEST_CASE(customTest3) // Test for behavior when accessing invalid channels
{
    adc_init();
    float invalidChannelVoltage = adc_getVoltage(AdcChannel_Cnt);
    BOOST_CHECK_EQUAL(invalidChannelVoltage, -1);
}
