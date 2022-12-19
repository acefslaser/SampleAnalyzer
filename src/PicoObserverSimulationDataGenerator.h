#ifndef PICOOBSERVER_SIMULATION_DATA_GENERATOR
#define PICOOBSERVER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class PicoObserverAnalyzerSettings;

class PicoObserverSimulationDataGenerator
{
public:
	PicoObserverSimulationDataGenerator();
	~PicoObserverSimulationDataGenerator();

	void Initialize(U32 simulation_sample_rate, PicoObserverAnalyzerSettings* settings);
	U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel);

protected:
	PicoObserverAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationDataX;
    SimulationChannelDescriptor mSerialSimulationDataY;
    SimulationChannelDescriptor mSerialSimulationDataZ;
    SimulationChannelDescriptor mSerialSimulationDataSync;
};
#endif //PICOOBSERVER_SIMULATION_DATA_GENERATOR