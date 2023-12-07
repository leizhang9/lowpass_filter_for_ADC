/* Task: Implement a module for processing/filtering raw ADC measurements.
 * This module should filter the raw data to reduce noise,
 * and scale them to volts (compensating  different ADC supply
 * voltages by using data from the reference voltage channel).
 *
 * Raw ADC data is provided with 10kHz, as function parameter
 * for adc_processRawData.
 * Filtered and scaled ADC values should be accessible over the
 * adc_getVoltage function.
 * Raw data contains a 12bit value for every voltage input required
 * by the application (U1 through U6), and one reference voltage.
 * ADC voltages can be described with this formula:
 *
 * VChannel = Vdd * ADC_Channel / 4095
 * 
 * VChannel:    Actual external voltage on the corresponding ADC channel [Volts]
 * Vdd:         Actual supply voltage [Volts]
 * ADC_Channel: ADC measurement [12bit value]
 * 
 * The reference voltage channel will only be a specific 
 * 12bit value (stored in device ROM at address 0x1FFF0080) when the
 * supply voltage is exactly 3.3V, following this formula:
 * 
 * Vdd = 3.3V * ADC_VRef_Expected@3.3V / ADC_VRef_Measured
 * 
 * Vdd:                    Actual supply voltage [Volts]
 * ADC_VRef_Expected@3.3V: Expected ADC output with 3.3V Vdd (stored in
                           device ROM at address 0x1FFF0080) [12bit value]
 * ADC_VRef_Measured:      Actual VRef measurement [12bit value]
 * 
 * All frequency components above 200Hz may be filtered out,
 * because they are not relevant for the application.
 * 
 * Suggested unit tests:
 *  => Check that frequencies above 1kHz are filtered out
 *  => Check that frequencies below 50Hz are NOT filtered out
 *  => Check that correct voltages are returned with different Vdd
       (supply voltage) levels
 *  => Check for sensible behavior when trying to access the Vref channel
       itself or invalid channels
 */
#define VREF_CALIBRATION_ADDR 0x1FFF0080

#include <stdint.h>

typedef enum AdcChannel {
    AdcChannel_U1 = 0,
    AdcChannel_U2,
    AdcChannel_U3,
    AdcChannel_U4,
    AdcChannel_U5,
    AdcChannel_U6,

    AdcChannel_Vref,

    AdcChannel_Cnt,
} AdcChannel;

typedef struct AdcRawData {
    uint16_t values[AdcChannel_Cnt];
} AdcRawData;

void adc_init(void);
void adc_processRawData(const AdcRawData *data);
float adc_getVoltage(AdcChannel ch);

