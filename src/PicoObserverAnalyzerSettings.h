#ifndef PICOOBSERVER_ANALYZER_SETTINGS
#define PICOOBSERVER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class PicoObserverAnalyzerSettings : public AnalyzerSettings
{
public:
	PicoObserverAnalyzerSettings();
	virtual ~PicoObserverAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings(const char* settings);
	virtual const char* SaveSettings();

	
    Channel mInputChannelX;
    Channel mInputChannelY;
    Channel mSyncChannel;
    Channel mClockChannel;

  protected:
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelXInterface;
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelYInterface;
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mSyncChannelInterface;
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mClockChannelInterface;
};

#endif //PICOOBSERVER_ANALYZER_SETTINGS
