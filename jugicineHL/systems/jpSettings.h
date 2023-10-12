#ifndef PLAYER__SYSTEM__SETTINGS_H
#define PLAYER__SYSTEM__SETTINGS_H


#include <string>
#include <vector>

#include "jmGuiCommon.h"
#include "jpVariables.h"
#include "jpGlobal.h"



namespace jugimap{


class PlayedApp;




class SystemSettings
{
public:

    SystemSettings(PlayedApp *_playerApp);

    //bool init();


    void setFullScreen(bool _fullScreen);
    bool isFullScreen();


    void setPixelZoom(int _pixelZoom);
    int pixelZoom();


    void setMusicActive(bool _musicActive);
    bool isMusicActive();


    void setSfxActive(bool _sfxActive);
    bool isSfxActive();


    void setSpeechActive(bool _speechActive);
    bool isSpeechActive();


    void setMusicVolume(int _musicVolume);
    int  musicVolume();


    void setSfxVolume(int _sfxVolume);
    int  sfxVolume();


    void setSpeechVolume(int _speechVolume);
    int  speechVolume();


    const std::string &activeLanguage();
    void setActiveLanguage(const std::string &_language);


    int usedSystemParameters(){ return mUsedSystemParameters; }

    void appendToUsedSystemParameters(SystemParameter sp){ mUsedSystemParameters |= static_cast<int>(sp); }

    bool saveSettingsIniFile();

    bool loadSettingsFromIniFile();



private:
    PlayedApp* mParentPlayerApp = nullptr;

    int mUsedSystemParameters = 0;


    //language

    VariableManager mVariables;

};




}


#endif // PLAYER_BASE_OBJECTS_H
