#ifndef PLAYER__SYSTEM__SETTINGS_H
#define PLAYER__SYSTEM__SETTINGS_H

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "jmStorage.h"

#include "jmSignal.h"
#include "jpVariables.h"
#include "jpGlobal.h"



namespace jugimap{


class PlayedApp;

class SignalQuery;
class SignalSetter;
class ParsedSignalPath;




//------------------------------------------------------------------------------------


struct SystemParameter
{
    static const int NOT_DEFINED = 0;
    static const int FULL_SCREEN = 1;
    static const int PIXEL_ZOOM = 2;
    static const int MUSIC_ACTIVE = 3;
    static const int SFX_ACTIVE = 4;
    static const int SPEECH_ACTIVE = 5;
    static const int MUSIC_VOLUME = 6;
    static const int SFX_VOLUME = 7;
    static const int SPEECH_VOLUME = 8;
    static const int LANGUAGE = 9;

};


int GetSystemParameterFromString(const std::string &parameter);


//------------------------------------------------------------------------------------


class SystemSettings : public SignalCallback
{
public:

    SystemSettings(PlayedApp *_playerApp);

    //bool init();

    //void update();

    SimpleStorage<Signal*> & signalsStorage(){ return mAllSignals; }


     //void onSetBySignalSetter(Signal* _signal) override;

     bool onSignalSet(Signal *_signal) override;

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


    void setMusicVolume(float _musicVolume);
    int  musicVolume();


    void setSfxVolume(float _sfxVolume);
    int  sfxVolume();


    void setSpeechVolume(float _speechVolume);
    int  speechVolume();


    const std::string &activeLanguage();
    void setActiveLanguage(const std::string &_language);


    //int usedSystemParameters(){ return mUsedSystemParameters; }

    void appendToUsedSystemParameters(int sp);

    bool saveSettingsIniFile();
    bool loadSettingsFromIniFile();


    void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);


private:
    PlayedApp* mParentPlayerApp = nullptr;

    //int mUsedSystemParameters = 0;


    //language

   // VariableManager mVariables;


    SimpleStorage<Signal*>mAllSignals{"SystemSettingSignals"};

    //std::vector<Signal*>mSignals;                   // OWNED

    std::vector<Signal*>mUsedSignals;               // LINKS



    //std::unique_ptr<SignalCallback>mSignalCallback;

};


/*

class SettingsSignalCallback : public SignalCallback
{
public:


    //void onSetBySignalSetter(BoolSignal *_signal, bool _newValue) override;
    //void onSetBySignalSetter(IntSignal *_signal, int _newValue) override;
    //void onSetBySignalSetter(FloatSignal *_signal, float _newValue) override;
    //void onSetBySignalSetter(StringSignal *_signal, const std::string &_newValue) override;

    void onSetBySignalSetter(Signal* _signal) override;

};

*/



}


#endif // PLAYER_BASE_OBJECTS_H
