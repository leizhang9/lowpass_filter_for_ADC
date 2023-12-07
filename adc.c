#include "adc.h"
#define SAMPLE_RATE 10000 // 10kHz
#define FILTER_TAU 0.001

static float filteredValues[AdcChannel_Cnt];

void adc_init(void)
{
	for (int i = 0; i < AdcChannel_Cnt; i++) {
        filteredValues[i] = 0;
    }
}

void adc_processRawData(const AdcRawData *data)
{
	float dt = 1.0f / SAMPLE_RATE;
    for (int i = 0; i < AdcChannel_Cnt; i++) {
        // first order low pass filter
        filteredValues[i] += (data->values[i] - filteredValues[i]) * dt / FILTER_TAU;
    }
}

float adc_getVoltage(AdcChannel ch)
{
	if (ch < 0 || ch >= AdcChannel_Cnt) {
        return -1.0; 
    }

    uint16_t refValue = *(uint32_t*)VREF_CALIBRATION_ADDR;
    //uint16_t refValue = 42; // for debug
    float Vdd = 3.3f * refValue / filteredValues[AdcChannel_Vref];
    return Vdd * filteredValues[ch] / 4095;
}

