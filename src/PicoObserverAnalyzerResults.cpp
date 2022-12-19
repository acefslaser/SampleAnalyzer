#include "PicoObserverAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "PicoObserverAnalyzer.h"
#include "PicoObserverAnalyzerSettings.h"
#include <iostream>
#include <fstream>

PicoObserverAnalyzerResults::PicoObserverAnalyzerResults(PicoObserverAnalyzer* analyzer, PicoObserverAnalyzerSettings* settings) : AnalyzerResults(), mSettings(settings), mAnalyzer(analyzer) {}
PicoObserverAnalyzerResults::~PicoObserverAnalyzerResults() {}

void PicoObserverAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base) {
	ClearResultStrings();
	Frame frame = GetFrame(frame_index);

	if( frame.mFlags == 0 )
    {
        if( mSettings->mInputChannelX == channel )
        {
            char number_str[ 128 ];
            AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 16, number_str, 128 );
            AddResultString( number_str );
        }
        if( mSettings->mInputChannelY == channel )
        {
            char number_str[ 128 ];
            AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 16, number_str, 128 );
            AddResultString( number_str );
        }    
	}
}

void PicoObserverAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id) {
	std::ofstream file_stream(file, std::ios::out);

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for(U32 i=0; i < num_frames; i++) {
        char time_str[ 128 ];
        Frame frame = GetFrame( i );
        AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);

		file_stream << time_str << "," << number_str << std::endl;

		if(UpdateExportProgressAndCheckForCancel(i, num_frames) == true) {
			file_stream.close();
			return;
		}
	}
	file_stream.close();
}

void PicoObserverAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base) {
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame(frame_index);
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);
	AddTabularText(number_str);
#endif
}

void PicoObserverAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base) {}
void PicoObserverAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base) {}