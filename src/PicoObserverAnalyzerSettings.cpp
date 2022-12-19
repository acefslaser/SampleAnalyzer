#include "PicoObserverAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


PicoObserverAnalyzerSettings::PicoObserverAnalyzerSettings()
:   mInputChannelX(UNDEFINED_CHANNEL), 
	mInputChannelY(UNDEFINED_CHANNEL), 
    mSyncChannel(UNDEFINED_CHANNEL)
{
    mInputChannelXInterface.reset(new AnalyzerSettingInterfaceChannel());
    mInputChannelXInterface->SetTitleAndTooltip("X", "Standard PICO Observer");
    mInputChannelXInterface->SetChannel(mInputChannelX);
    mInputChannelYInterface.reset(new AnalyzerSettingInterfaceChannel());
    mInputChannelYInterface->SetTitleAndTooltip("Y", "Standard PICO Observer");
    mInputChannelYInterface->SetChannel(mInputChannelY);
    mSyncChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mSyncChannelInterface->SetTitleAndTooltip("Sync", "Standard PICO Observer");
    mSyncChannelInterface->SetChannel(mSyncChannel);
    mClockChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mClockChannelInterface->SetTitleAndTooltip( "Clock", "Standard PICO Observer" );
    mClockChannelInterface->SetChannel( mClockChannel );

	AddInterface(mInputChannelXInterface.get());
    AddInterface(mInputChannelYInterface.get());
    AddInterface( mSyncChannelInterface.get() );
    AddInterface( mClockChannelInterface.get() );

	AddExportOption(0, "Export as text/csv file");
	AddExportExtension(0, "text", "txt");
	AddExportExtension(0, "csv", "csv");

	ClearChannels();
    AddChannel(mInputChannelX, "X", false);
    AddChannel(mInputChannelY, "Y", false);
    AddChannel( mSyncChannel, "Sync", false );
    AddChannel( mClockChannel, "Clock", false );
}

PicoObserverAnalyzerSettings::~PicoObserverAnalyzerSettings()
{
}

bool PicoObserverAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannelX = mInputChannelXInterface->GetChannel();
    mInputChannelY = mInputChannelYInterface->GetChannel();
    mSyncChannel = mSyncChannelInterface->GetChannel();
    mClockChannel = mClockChannelInterface->GetChannel();

	ClearChannels();
    AddChannel(mInputChannelX, "X", true);
    AddChannel(mInputChannelY, "Y", true);
    AddChannel( mSyncChannel, "Sync", true );
    AddChannel( mClockChannel, "Clock", true );

	return true;
}

void PicoObserverAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelXInterface->SetChannel(mInputChannelX);
    mInputChannelYInterface->SetChannel(mInputChannelY);
    mSyncChannelInterface->SetChannel( mSyncChannel );
    mClockChannelInterface->SetChannel( mClockChannel );
}

void PicoObserverAnalyzerSettings::LoadSettings(const char* settings)
{
	SimpleArchive text_archive;
	text_archive.SetString(settings);

	text_archive >> mInputChannelX;
    text_archive >> mInputChannelY;
    text_archive >> mSyncChannel;
    text_archive >> mClockChannel;

	ClearChannels();
    AddChannel(mInputChannelX, "X", true);
    AddChannel(mInputChannelY, "Y", true);
    AddChannel( mSyncChannel, "Sync", true );
    AddChannel( mClockChannel, "Clock", true );

	UpdateInterfacesFromSettings();
}

const char* PicoObserverAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannelX;
    text_archive << mInputChannelY;
    text_archive << mSyncChannel;
    text_archive << mClockChannel;

	return SetReturnString(text_archive.GetString());
}
