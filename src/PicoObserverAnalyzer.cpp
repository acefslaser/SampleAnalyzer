#include "PicoObserverAnalyzer.h"
#include "PicoObserverAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

PicoObserverAnalyzer::PicoObserverAnalyzer()
:	Analyzer2(),  
	mSettings(new PicoObserverAnalyzerSettings()),
	mSimulationInitilized(false)
{
	SetAnalyzerSettings(mSettings.get());
}

PicoObserverAnalyzer::~PicoObserverAnalyzer()
{
	KillThread();
}

void PicoObserverAnalyzer::SetupResults()
{
	mResults.reset(new PicoObserverAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());
    if(mSettings->mInputChannelX != UNDEFINED_CHANNEL)
        mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannelX);
    if( mSettings->mInputChannelY != UNDEFINED_CHANNEL )
        mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannelY );
}

void PicoObserverAnalyzer::WorkerThread()
{
	mSerialX = GetAnalyzerChannelData(mSettings->mInputChannelX);
    mSerialY = GetAnalyzerChannelData(mSettings->mInputChannelY);
    mSync = GetAnalyzerChannelData( mSettings->mSyncChannel );
    mClock = GetAnalyzerChannelData( mSettings->mClockChannel );

	if( mSync->GetBitState() == BIT_LOW )
    {
        mSync->AdvanceToNextEdge();
    }
    else
    {
        mSync->AdvanceToNextEdge();
        mSync->AdvanceToNextEdge();
    }

	mCurrentSample = mSync->GetSampleNumber();


	for(;;)
	{

        mResults->AddMarker( mCurrentSample, AnalyzerResults::Dot, mSettings->mSyncChannel );
        mClock->AdvanceToAbsPosition( mCurrentSample );
        
        mSerialX->AdvanceToAbsPosition( mCurrentSample );
        mSerialY->AdvanceToAbsPosition( mCurrentSample );
        mResults->AddMarker( mCurrentSample, mSerialX->GetBitState() == BIT_LOW ? AnalyzerResults::Dot : AnalyzerResults::ErrorSquare,
                             mSettings->mInputChannelX );
        mResults->AddMarker( mCurrentSample, mSerialY->GetBitState() == BIT_LOW ? AnalyzerResults::Dot : AnalyzerResults::ErrorSquare,
                             mSettings->mInputChannelY );
        mClock->AdvanceToNextEdge();
        mClock->AdvanceToNextEdge();
        mCurrentSample = mClock->GetSampleNumber();
        
        mSerialX->AdvanceToAbsPosition( mCurrentSample );
        mSerialY->AdvanceToAbsPosition( mCurrentSample );
        mResults->AddMarker( mCurrentSample, mSerialX->GetBitState() == BIT_LOW ? AnalyzerResults::Dot : AnalyzerResults::ErrorSquare,
                             mSettings->mInputChannelX );
        mResults->AddMarker( mCurrentSample, mSerialY->GetBitState() == BIT_LOW ? AnalyzerResults::Dot : AnalyzerResults::ErrorSquare,
                             mSettings->mInputChannelY );
        mClock->AdvanceToNextEdge();
        mClock->AdvanceToNextEdge();
        mCurrentSample = mClock->GetSampleNumber();

        mSerialX->AdvanceToAbsPosition( mCurrentSample );
        mSerialY->AdvanceToAbsPosition( mCurrentSample );
        mResults->AddMarker( mCurrentSample, mSerialX->GetBitState() == BIT_HIGH ? AnalyzerResults::Dot : AnalyzerResults::ErrorSquare,
                             mSettings->mInputChannelX );
        mResults->AddMarker( mCurrentSample, mSerialY->GetBitState() == BIT_HIGH ? AnalyzerResults::Dot : AnalyzerResults::ErrorSquare,
                             mSettings->mInputChannelY );
        mClock->AdvanceToNextEdge();
        mClock->AdvanceToNextEdge();
        mCurrentSample = mClock->GetSampleNumber();

        U64 first_sample = mCurrentSample;

        U64 x_pos = 0;
        xResult.Reset( &x_pos, AnalyzerEnums::MsbFirst, 16 );
        U64 y_pos = 0;
        yResult.Reset( &y_pos, AnalyzerEnums::MsbFirst, 16 );

        for( int i = 0; i < 16; i++ )
        {
            mSerialX->AdvanceToAbsPosition( mCurrentSample );
            mSerialY->AdvanceToAbsPosition( mCurrentSample );
            xResult.AddBit( mSerialX->GetBitState() );
            yResult.AddBit( mSerialY->GetBitState() );
            mClock->AdvanceToNextEdge();
            mClock->AdvanceToNextEdge();
            mCurrentSample = mClock->GetSampleNumber();
        }

        Frame xFrame;
        xFrame.mStartingSampleInclusive = first_sample;
        xFrame.mEndingSampleInclusive = mClock->GetSampleNumber();
        xFrame.mData1 = x_pos;
        xFrame.mData2  = y_pos;
        mResults->AddFrame( xFrame );
        
        mSync->AdvanceToNextEdge();
        mSync->AdvanceToNextEdge();
        mCurrentSample = mSync->GetSampleNumber();
        ReportProgress( mCurrentSample );
        
	}
}

bool PicoObserverAnalyzer::NeedsRerun()
{
	return false;
}

U32 PicoObserverAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels)
{
	if(mSimulationInitilized == false)
	{
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 PicoObserverAnalyzer::GetMinimumSampleRateHz()
{
    return 0; //mSettings->mBitRate * 4;
}

const char* PicoObserverAnalyzer::GetAnalyzerName() const
{
	return "PICO Observer";
}

const char* GetAnalyzerName()
{
	return "PICO Observer";
}

Analyzer* CreateAnalyzer()
{
	return new PicoObserverAnalyzer();
}

void DestroyAnalyzer(Analyzer* analyzer)
{
	delete analyzer;
}