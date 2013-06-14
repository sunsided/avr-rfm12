/*
 * rfm12configuration.cpp
 *
 * Created: 14.06.2013 20:00:26
 *  Author: Markus
 */ 

#include "rfm12configuration.hpp"

using namespace rfm12;
using namespace rfm12::commands;

/**
* \brief Configures the given RFM12 instance
*/
void configureRfm12(Rfm12 *rfm12)
{
	// Set sleep mode
	PowerManagementCommand powerMgmt;
	powerMgmt.setLowBatteryDetectorEnabled(true);
	powerMgmt.setCrystalOscillatorEnabled(false);
	powerMgmt.setClockOutputEnabled(false);
	rfm12->executeCommand(powerMgmt);
	
	// 1000 0000 .... .... Configuration Setting Command
	ConfigSetCommand configSet;
	configSet.setFrequencyBand(FREQ_BAND_433);
	configSet.setCrystalCapacitance(CL_120);
	configSet.setDataRegisterEnabled(true);
	configSet.setFifoEnabled(true);
	rfm12->executeCommand(configSet);
	
	// 1010 .... .... .... Frequency Setting Command
	FrequencyCommand frequencySetting;
	frequencySetting.setByFrequency(433.0F);
	rfm12->executeCommand(frequencySetting);
	
	// 1100 0110 .... .... Data Rate Command
	DataRateCommand dataRate;
	//dataRate.setBitRate(49.2F);
	dataRate.setBitRate(0.6F);
	rfm12->executeCommand(dataRate);

	// 1001 0... .... .... Receiver Control Command
	ReceiverControlCommand receiverControl;
	receiverControl.setPin16Mode(PIN16_VDI_OUT);
	receiverControl.setVdiResponseTime(VDI_FAST);
	receiverControl.setReceiverBasebandBandwidth(RBBW_134);
	receiverControl.setLnaGain(LNGAIN_0);
	receiverControl.setRssiDetectorThreshold(RSSI_91);
	rfm12->executeCommand(receiverControl);
	
	// 1100 0010 .... .... Data Filter Command
	DataFilterCommand dataFilter;
	dataFilter.setClockRecoveryAutoLockModeEnabled(true);
	dataFilter.setClockRecoveryFastLockEnabled(false);
	dataFilter.setDataFilterType(FILTER_DIGITAL);
	dataFilter.setDqdTrheshold(4);
	rfm12->executeCommand(dataFilter);
	
	FifoAndResetModeCommand fifoAndResetMode;
	SynchronPatternCommand synchronPattern;

	uint8_t group = 212; // 212 ist einzige für RFM12 -- sind zwar RFM12B, aber schaden kann es ja nicht
	if (group != 0) {
		// 1100 1010 .... .... FIFO and Reset Mode Command
		fifoAndResetMode.setFifoInterruptFillLevel(8);
		fifoAndResetMode.setSynchronPatternLength(SP_TWO_BYTE);
		fifoAndResetMode.setFifoFillAfterSynchronMatchEnabled(false);
		fifoAndResetMode.setFifoFillStartCondition(FIFOSTART_SYNCHRON);
		fifoAndResetMode.setSensitiveResetMode(RESETMODE_NONSENSITIVE);
		
		// 1100 1110 .... .... Synchron Pattern Command
		synchronPattern.setSynchronByte(group);
		} else {
		// 1100 1010 .... .... FIFO and Reset Mode Command
		fifoAndResetMode.setFifoInterruptFillLevel(8);
		fifoAndResetMode.setSynchronPatternLength(SP_ONE_BYTE);
		fifoAndResetMode.setFifoFillAfterSynchronMatchEnabled(false);
		fifoAndResetMode.setFifoFillStartCondition(FIFOSTART_SYNCHRON);
		fifoAndResetMode.setSensitiveResetMode(RESETMODE_NONSENSITIVE);
		
		// 1100 1110 .... .... Synchron Pattern Command
		synchronPattern.setSynchronByte(0x2D);
	}
	
	rfm12->executeCommand(fifoAndResetMode);
	rfm12->executeCommand(synchronPattern);
	
	// 1100 0100 .... .... AFC Command
	AfcCommand afcCommand;
	afcCommand.setAutomaticOperationMode(AUTOMODE_VDI_HIGH);
	afcCommand.setRangeLimit(MAXDEVIATION_UNRESTRICTED);
	afcCommand.setStrobeEdgeEnabled(false);
	afcCommand.setFineModeEnabled(false);
	afcCommand.setFrequencyOffsetRegisterEnabled(true);
	afcCommand.setFrequencyOffsetCalculationEnabled(true);
	rfm12->executeCommand(afcCommand);
	
	// 1001 100. .... .... TX Configuration Command
	TxConfigCommand txConfig;
	txConfig.setFsk(FSKDF_90KHZ);
	txConfig.setOutputPower(OUTPOW_FULL);
	rfm12->executeCommand(txConfig);
	
	// 1100 1100 0.... .... PLL Setting Command
	PllSettingCommand pllSetting;
	pllSetting.setOutputClockBufferTimeControl(MCCLKFRQ_5OR10MHZ);
	pllSetting.setPhaseDetectorDelayEnabled(false);
	pllSetting.setPllDitheringEnabled(false);
	pllSetting.setPllBandwidth(PLLBW_MAX_2560KBPS);
	rfm12->executeCommand(pllSetting);
	
	// 111. .... .... ....Wake-Up Timer Command
	WakeupTimerCommand wakeupTimer;
	wakeupTimer.disableWakeupTimer();
	rfm12->executeCommand(wakeupTimer);
	
	// 1100 1000 .... .... Low Duty-Cycle Command
	LowDutyCycleCommand lowDutyCycle;
	lowDutyCycle.setEnabled(false);
	rfm12->executeCommand(lowDutyCycle);
	
	// 1100 0000 .... .... Low Battery Detector and Microcontroller Clock Divider Command
	BatteryDetectorAndClockDividerCommand batteryAndClock;
	batteryAndClock.setClockDivider(CLKOUTFREQ_1660kHZ);
	batteryAndClock.setVoltageThreshould(BATTHRESH_3150mV);
	rfm12->executeCommand(batteryAndClock);
}