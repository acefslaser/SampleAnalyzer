#ifndef PICOOBSERVER_ANALYZER_H
#define PICOOBSERVER_ANALYZER_H

#include <Analyzer.h>
#include <AnalyzerHelpers.h>
#include "PicoObserverAnalyzerResults.h"
#include "PicoObserverSimulationDataGenerator.h"

class PicoObserverAnalyzerSettings;
class ANALYZER_EXPORT PicoObserverAnalyzer : public Analyzer2
{
public:
	PicoObserverAnalyzer();
	virtual ~PicoObserverAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels);
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr<PicoObserverAnalyzerSettings> mSettings;
    std::auto_ptr<PicoObserverAnalyzerResults> mResults;
    AnalyzerChannelData* mSerialX;
    AnalyzerChannelData* mSerialY;
    AnalyzerChannelData* mSync;
    AnalyzerChannelData* mClock;

    U64 mCurrentSample;
	DataBuilder xResult;
    DataBuilder yResult;

	PicoObserverSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer* analyzer);

#endif //PICOOBSERVER_ANALYZER_H
