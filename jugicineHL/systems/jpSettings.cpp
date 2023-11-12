#include "ncine/Application.h"
#include "ncine/IGfxDevice.h"
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmText.h"
#include "jmAnimationCommon.h"
#include "jmSpriteLayer.h"
#include "jmCompositeSprite.h"
#include "jmUtilities.h"
#include "jmGlobal.h"
#include "jmStreams.h"

//#include "jmGuiCommon.h"
//#include "jmGuiBar.h"
#include "jpSystemVarious.h"
#include "jpQueries.h"
#include "jpPlayedApp.h"
//#include "jpItemsTable.h"
#include "jpUser.h"
#include "jpSettings.h"


namespace jugimap{






int GetSystemParameterFromString(const std::string &parameter)
{

    if(parameter=="fullScreen"){
        return SystemParameter::FULL_SCREEN;

    }else if(parameter=="pixelZoom"){
        return SystemParameter::PIXEL_ZOOM;

    }else if(parameter=="musicActive"){
        return SystemParameter::MUSIC_ACTIVE;

    }else if(parameter=="sfxActive"){
        return SystemParameter::SFX_ACTIVE;

    }else if(parameter=="speechActive"){
        return SystemParameter::SPEECH_ACTIVE;

    }else if(parameter=="musicVolume"){
        return SystemParameter::MUSIC_VOLUME;

    }else if(parameter=="sfxVolume"){
        return SystemParameter::SFX_VOLUME;

    }else if(parameter=="speechVolume"){
        return SystemParameter::SPEECH_VOLUME;

    }else if(parameter=="language"){
        return SystemParameter::LANGUAGE;

    }

    return SystemParameter::NOT_DEFINED;

}


//----------------------------------------------------------------------------------------------


SystemSettings::SystemSettings(PlayedApp *_playerApp)
{

    mParentPlayerApp = _playerApp;


    /*
    // system settings
    mVariables.addVariable(new BoolVar("fullScreen", false));
    mVariables.addVariable(new IntVar("pixelZoom", 100));

    mVariables.addVariable(new BoolVar("musicActive", true));
    mVariables.addVariable(new IntVar("musicVolume", 100));

    mVariables.addVariable(new BoolVar("sfxActive", true));
    mVariables.addVariable(new IntVar("sfxVolume", 100));

    mVariables.addVariable(new BoolVar("speechActive", true));
    mVariables.addVariable(new IntVar("speechVolume", 100));

    mVariables.addVariable(new StringVar("language", textLibrary.activeLanguage()));

    //---
    */



    mAllSignals.addObject(new BoolSignal("fullScreen", false, SystemParameter::FULL_SCREEN));
    mAllSignals.addObject(new IntSignal( "pixelZoom", 100, SystemParameter::PIXEL_ZOOM));

    mAllSignals.addObject(new BoolSignal("musicActive", true, SystemParameter::MUSIC_ACTIVE));
    mAllSignals.addObject(new FloatSignal("musicVolume", 100, SystemParameter::MUSIC_VOLUME));

    mAllSignals.addObject(new BoolSignal("sfxActive", true, SystemParameter::SFX_ACTIVE));
    mAllSignals.addObject(new FloatSignal("sfxVolume", 100, SystemParameter::SFX_VOLUME));

    mAllSignals.addObject(new BoolSignal("speechActive", true, SystemParameter::SPEECH_ACTIVE));
    mAllSignals.addObject(new FloatSignal("speechVolume", 100, SystemParameter::SPEECH_VOLUME));

    const std::string & activeLanguage = textLibrary.activeLanguage();
    mAllSignals.addObject(new StringSignal("language", activeLanguage, SystemParameter::LANGUAGE));

    //---
    //mSignalCallback.reset(new SettingsSignalCallback());

    for(Signal* s : mAllSignals.objects()){
        s->setCallback(this);
    }

}



/*
void SystemSettings::update()
{

    if(mSigFullScreen.valueChanged()){
        //setFullScreen(mSigFullScreen.value());

        bool fullScreen = mSigFullScreen.value();

        ncine::IGfxDevice &gfxDevice = ncine::theApplication().gfxDevice();
        const ncine::IGfxDevice::VideoMode &currentMode = gfxDevice.currentVideoMode(0);
        int primaryMonitorIndex = gfxDevice.primaryMonitorIndex();
        const ncine::IGfxDevice::Monitor &monitor = gfxDevice.monitor(primaryMonitorIndex);

        if(fullScreen){
            gfxDevice.setFullScreen(true);

        }else{
            Vec2i windowSize = app->designResolution().designResolution();
            Vec2i windowPos(monitor.position.x + (currentMode.width-windowSize.x)/2, monitor.position.y + (currentMode.height-windowSize.y)/2);

            bool fullScreenPrev = gfxDevice.isFullScreen();

            gfxDevice.setFullScreen(false);
            gfxDevice.setWindowPosition(windowPos);
            if(fullScreenPrev){
                gfxDevice.setWindowSize(windowSize);
            }
        }
    }


    if(mSigPixelZoom.valueChanged()){



    }

    if(mSigMusicActive.valueChanged()){



    }

    if(mSigMusicVolume.valueChanged()){



    }

    if(mSigSfxActive.valueChanged()){



    }

    if(mSigSfxVolume.valueChanged()){



    }

    if(mSigSpeechActive.valueChanged()){



    }

    if(mSigSpeechVolume.valueChanged()){



    }

    if(mSigLanguage.valueChanged()){

        const std::string language = mSigLanguage.value();
        textLibrary.setActiveLanguage(language);

    }


}
*/

/*
void SystemSettings::onSetBySignalSetter(Signal *_signal)
{


    for(Signal* s : mAllSignals.objects()){
        if(s == _signal){
            if(s->type()==SignalType::BOOL){

                BoolSignal *sig = static_cast<BoolSignal*>(s);
                bool value = sig->value_currentOrNextIfSet();

                if(s->id()==SystemParameter::FULL_SCREEN){
                    setFullScreen(value);

                }else if(s->id()==SystemParameter::MUSIC_ACTIVE){
                    setMusicActive(value);

                }else if(s->id()==SystemParameter::SFX_ACTIVE){
                    setMusicActive(value);

                }else if(s->id()==SystemParameter::SPEECH_ACTIVE){
                    setMusicActive(value);

                }

            }else if(s->type()==SignalType::INT){

                IntSignal *sig = static_cast<IntSignal*>(s);
                int value = sig->value_currentOrNextIfSet();

                if(s->id()==SystemParameter::PIXEL_ZOOM){
                    setPixelZoom(value);
                }

            }else if(s->type()==SignalType::FLOAT){

                FloatSignal *sig = static_cast<FloatSignal*>(s);
                float value = sig->value_currentOrNextIfSet();

                if(s->id()==SystemParameter::MUSIC_VOLUME){
                    setMusicVolume(value);

                }else if(s->id()==SystemParameter::SFX_VOLUME){
                    setSfxVolume(value);

                }else if(s->id()==SystemParameter::SPEECH_VOLUME){
                    setSpeechVolume(value);

                }

            }else if(s->type()==SignalType::STRING){

                StringSignal *sig = static_cast<StringSignal*>(s);
                const std::string &value = sig->value_currentOrNextIfSet();

                if(s->id()==SystemParameter::LANGUAGE){
                    setActiveLanguage(value);
                }

            }

            app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));

        }
    }
}
*/


bool SystemSettings::onSignalSet(Signal *_signal)
{

    for(Signal* s : mAllSignals.objects()){
        if(s == _signal){
            if(s->type()==SignalType::BOOL){

                BoolSignal *sig = static_cast<BoolSignal*>(s);
                bool value = sig->value();

                if(s->id()==SystemParameter::FULL_SCREEN){
                    setFullScreen(value);

                }else if(s->id()==SystemParameter::MUSIC_ACTIVE){
                    setMusicActive(value);

                }else if(s->id()==SystemParameter::SFX_ACTIVE){
                    setMusicActive(value);

                }else if(s->id()==SystemParameter::SPEECH_ACTIVE){
                    setMusicActive(value);

                }

            }else if(s->type()==SignalType::INT){

                IntSignal *sig = static_cast<IntSignal*>(s);
                int value = sig->value();

                if(s->id()==SystemParameter::PIXEL_ZOOM){
                    setPixelZoom(value);
                }

            }else if(s->type()==SignalType::FLOAT){

                FloatSignal *sig = static_cast<FloatSignal*>(s);
                float value = sig->value();

                if(s->id()==SystemParameter::MUSIC_VOLUME){
                    setMusicVolume(value);

                }else if(s->id()==SystemParameter::SFX_VOLUME){
                    setSfxVolume(value);

                }else if(s->id()==SystemParameter::SPEECH_VOLUME){
                    setSpeechVolume(value);

                }

            }else if(s->type()==SignalType::STRING){

                StringSignal *sig = static_cast<StringSignal*>(s);
                const std::string &value = sig->value();

                if(s->id()==SystemParameter::LANGUAGE){
                    setActiveLanguage(value);
                }

            }

            app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));

        }
    }

    return true;
}


void SystemSettings::setFullScreen(bool _fullScreen)
{
    //BoolVar *fullScreen = dynamic_cast<BoolVar*>(mVariables.getVariable("fullScreen"));     assert(fullScreen);

    //bool fullScreenPrev = fullScreen->value();

    //fullScreen->setValue(_fullScreen);

    ncine::IGfxDevice &gfxDevice = ncine::theApplication().gfxDevice();
    const ncine::IGfxDevice::VideoMode &currentMode = gfxDevice.currentVideoMode(0);
    int primaryMonitorIndex = gfxDevice.primaryMonitorIndex();
    const ncine::IGfxDevice::Monitor &monitor = gfxDevice.monitor(primaryMonitorIndex);



    if(_fullScreen){
        gfxDevice.setFullScreen(true);

    }else{
        Vec2i windowSize = app->designResolution().designResolution();
        Vec2i windowPos(monitor.position.x + (currentMode.width-windowSize.x)/2, monitor.position.y + (currentMode.height-windowSize.y)/2);

        bool fullScreenPrev = gfxDevice.isFullScreen();

        gfxDevice.setFullScreen(false);
        gfxDevice.setWindowPosition(windowPos);
        if(fullScreenPrev){
            gfxDevice.setWindowSize(windowSize);
        }
    }

}


bool SystemSettings::isFullScreen()
{

    BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("fullScreen"));              assert(sig);
    return sig->value();

}


void SystemSettings::setPixelZoom(int _pixelZoom)
{

    //IntSignal *sig = dynamic_cast<IntSignal*>(mAllSignals.getObject("pixelZoom"));     assert(sig);
    //sig->setValue(_pixelZoom);
}


int SystemSettings::pixelZoom()
{

    IntSignal *sig = dynamic_cast<IntSignal*>(mAllSignals.getObject("pixelZoom"));              assert(sig);
    return sig->value();

}



void SystemSettings::setMusicActive(bool _musicActive)
{

    //BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("musicActive"));              assert(sig);
    //sig->setValue(_musicActive);

}


bool SystemSettings::isMusicActive()
{

    BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("musicActive"));              assert(sig);
    return sig->value();

}


void SystemSettings::setSfxActive(bool _sfxActive)
{

    //BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("sfxActive"));              assert(sig);
    //sig->setValue(_sfxActive);

}


bool SystemSettings::isSfxActive()
{

    BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("sfxActive"));              assert(sig);
    return sig->value();

}


void SystemSettings::setSpeechActive(bool _speechActive)
{

    //BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("speechActive"));              assert(sig);
    //sig->setValue(_speechActive);

}


bool SystemSettings::isSpeechActive()
{

    BoolSignal *sig = dynamic_cast<BoolSignal*>(mAllSignals.getObject("speechActive"));              assert(sig);
    return sig->value();

}


void SystemSettings::setMusicVolume(float _musicVolume)
{

    //FloatSignal *sig = dynamic_cast<FloatSignal*>(mAllSignals.getObject("musicVolume"));              assert(sig);
    //sig->setValue(_musicVolume);


}


int SystemSettings::musicVolume()
{

    FloatSignal *sig = dynamic_cast<FloatSignal*>(mAllSignals.getObject("musicVolume"));              assert(sig);
    return sig->value();

}


void SystemSettings::setSfxVolume(float _sfxVolume)
{
    //FloatSignal *sig = dynamic_cast<FloatSignal*>(mAllSignals.getObject("sfxVolume"));              assert(sig);
    //sig->setValue(_sfxVolume);

}


int SystemSettings::sfxVolume()
{

    FloatSignal *sig = dynamic_cast<FloatSignal*>(mAllSignals.getObject("sfxVolume"));              assert(sig);
    return sig->value();
}



void SystemSettings::setSpeechVolume(float _speechVolume)
{
    //FloatSignal *sig = dynamic_cast<FloatSignal*>(mAllSignals.getObject("speechVolume"));              assert(sig);
    //sig->setValue(_speechVolume);

}


int SystemSettings::speechVolume()
{
    FloatSignal *sig = dynamic_cast<FloatSignal*>(mAllSignals.getObject("speechVolume"));              assert(sig);
    return sig->value();
}



void SystemSettings::setActiveLanguage(const std::string &_language)
{

    //StringSignal *sig = dynamic_cast<StringSignal*>(mAllSignals.getObject("language"));              assert(sig);
    //sig->setValue(_language);

    textLibrary.setActiveLanguage(_language);

}


const std::string & SystemSettings::activeLanguage()
{

    StringSignal *sig = dynamic_cast<StringSignal*>(mAllSignals.getObject("language"));              assert(sig);
    return sig->value();

}


void SystemSettings::appendToUsedSystemParameters(int sp)
{

    for(Signal* s : mAllSignals.objects()){
        if(s->id()==sp){
            mUsedSignals.push_back(s);
            break;
        }
    }

    /*
    mUsedSystemParameters |= static_cast<int>(sp);

    if(sp==SystemParameter::FULL_SCREEN){
        mUsedSignals.push_back(&mSigFullScreen);

    }else if(sp==SystemParameter::PIXEL_ZOOM){
        mUsedSignals.push_back(&mSigPixelZoom);

    }else if(sp==SystemParameter::MUSIC_ACTIVE){
        mUsedSignals.push_back(&mSigMusicActive);

    }else if(sp==SystemParameter::MUSIC_VOLUME){
        mUsedSignals.push_back(&mSigMusicVolume);

    }else if(sp==SystemParameter::SFX_ACTIVE){
        mUsedSignals.push_back(&mSigSfxActive);

    }else if(sp==SystemParameter::SFX_VOLUME){
        mUsedSignals.push_back(&mSigSfxVolume);

    }else if(sp==SystemParameter::SPEECH_ACTIVE){
        mUsedSignals.push_back(&mSigSpeechActive);

    }else if(sp==SystemParameter::SPEECH_VOLUME){
        mUsedSignals.push_back(&mSigSpeechVolume);

    }else if(sp==SystemParameter::LANGUAGE){
        mUsedSignals.push_back(&mSigLanguage);

    }

    */

}


bool SystemSettings::saveSettingsIniFile()
{

    std::string filePath = mParentPlayerApp->saveDataDir() + "/settings.ini";
    TextStreamWriter stream(filePath);

    if(stream.IsOpen()==false){
        return false;
    }

    /*
    for(Variable* v : mVariables.variables()){
        SystemParameter sp = GetSystemParameterFromString(v->name());
        if(sp != SystemParameter::NOT_DEFINED){     // system parameter

            if(mUsedSystemParameters==0 || mUsedSystemParameters & static_cast<int>(sp)){
                v->writeAsIniEntry(stream);
            }

        }else{  // custom parameter
            v->writeAsIniEntry(stream);
        }
    }
    */


    for(Signal *s : mUsedSignals){
        s->writeValueAsIniEntry(stream, true);
    }



    stream.Close();

    print("Saved settings ini file!");

    return true;
}


/*
bool SystemSettings::loadSettingsFromIniFile()
{

    std::string filePath = mParentPlayerApp->saveDataDir() + "/settings.ini";
    StdTextFileStreamReader stream(filePath);

    if(stream.IsOpen()==false){
        return false;
    }

    std::string line;

    while(stream.Eof()==false){

        stream.ReadLine(line);
        std::vector<std::string>parts = StdString::splitString(line ,":");
        if(parts.size()!=2){
            continue;
        }

        const std::string &pName = parts[0];
        const std::string &pValue = parts[1];
        Variable* v = mVariables.getVariable(pName, false);     assert(v);
        Variable::setValueFromString(v, pValue);

        SystemParameter sp = GetSystemParameterFromString(pName);
        if(sp != SystemParameter::NOT_DEFINED){

            if(sp==SystemParameter::FULL_SCREEN){
                setFullScreen(static_cast<BoolVar*>(v)->value());

            }else if(sp==SystemParameter::PIXEL_ZOOM){
                setPixelZoom(static_cast<IntVar*>(v)->value());

            }else if(sp==SystemParameter::MUSIC_ACTIVE){
                setMusicActive(static_cast<BoolVar*>(v)->value());

            }else if(sp==SystemParameter::SFX_ACTIVE){
                setSfxActive(static_cast<BoolVar*>(v)->value());

            }else if(sp==SystemParameter::SPEECH_ACTIVE){
                setSpeechActive(static_cast<BoolVar*>(v)->value());

            }else if(sp==SystemParameter::MUSIC_VOLUME){
                IntVar *iv = static_cast<IntVar*>(v);
                iv->setValue(ClampValue(iv->value(),0,100));
                setMusicVolume(iv->value());

            }else if(sp==SystemParameter::SFX_VOLUME){
                IntVar *iv = static_cast<IntVar*>(v);
                iv->setValue(ClampValue(iv->value(),0,100));
                setSfxVolume(iv->value());

            }else if(sp==SystemParameter::SPEECH_VOLUME){
                IntVar *iv = static_cast<IntVar*>(v);
                iv->setValue(ClampValue(iv->value(),0,100));
                setSpeechVolume(iv->value());

            }else if(sp==SystemParameter::LANGUAGE){
                StringVar *iv = static_cast<StringVar*>(v);
                setActiveLanguage(iv->value());
            }
        }
    }

    stream.Close();

    return true;

}
*/



bool SystemSettings::loadSettingsFromIniFile()
{

    std::string filePath = mParentPlayerApp->saveDataDir() + "/settings.ini";
    StdTextFileStreamReader stream(filePath);

    if(stream.IsOpen()==false){
        return false;
    }

    std::string line;

    while(stream.Eof()==false){

        stream.ReadLine(line);
        std::vector<std::string>parts = StdString::splitString(line ,":");
        if(parts.size()!=2){
            continue;
        }

        const std::string &pName = parts[0];
        const std::string &pValue = parts[1];

        for(Signal* s : mAllSignals.objects()){
            if(s->name()==pName){
                s->resetValueFromString(pValue);
                break;
            }
        }

        /*
        if(pName==mSigFullScreen.name()){
            mSigFullScreen.resetValueFromString(pValue);
            setFullScreen(mSigFullScreen.value());

        }else if(pName==mSigPixelZoom.name()){
            mSigPixelZoom.resetValueFromString(pValue);
            setPixelZoom(mSigPixelZoom.value());

        }else if(pName==mSigMusicActive.name()){
            mSigMusicActive.resetValueFromString(pValue);
            setMusicActive(mSigMusicActive.value());

        }else if(pName==mSigMusicVolume.name()){
            mSigMusicVolume.resetValueFromString(pValue);
            setMusicVolume(mSigMusicVolume.value());

        }else if(pName==mSigSfxActive.name()){
            mSigSfxActive.resetValueFromString(pValue);
            setSfxActive(mSigSfxActive.value());

        }else if(pName==mSigSfxVolume.name()){
            mSigSfxVolume.resetValueFromString(pValue);
            setSfxVolume(mSigSfxVolume.value());

        }else if(pName==mSigSpeechActive.name()){
            mSigSpeechActive.resetValueFromString(pValue);
            setSpeechActive(mSigSpeechActive.value());

        }else if(pName==mSigSpeechVolume.name()){
            mSigSpeechVolume.resetValueFromString(pValue);
            setSpeechVolume(mSigSpeechVolume.value());

        }else if(pName==mSigLanguage.name()){
            mSigLanguage.resetValueFromString(pValue);
            setActiveLanguage(mSigLanguage.value());

        }
        */
    }

    stream.Close();

    return true;

}




void SystemSettings::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{


    for(Signal* s : mAllSignals.objects()){
        if(s->name()==_psp.signalFullName()){
            _psp.obtainValue(_signalQuery, s);

            StdVector::addElementIfNotInVector(mUsedSignals, s);
            break;
        }
    }


    if(_signalQuery.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void SystemSettings::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    for(Signal* s : mAllSignals.objects()){
        if(s->name()==_psp.signalFullName()){
             _signalSetter.mSignal = s;

             StdVector::addElementIfNotInVector(mUsedSignals, s);
            break;
        }
    }

    if(_signalSetter.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


//--------------------------------------------------------------------------------------



/*

void SettingsSignalCallback::onSetBySignalSetter(BoolSignal *_signal, bool _newValue)
{

    SystemSettings *settings = app->systemSettings();

    SimpleStorage<Signal*>signalsStorage = settings->signalsStorage();

    for(Signal* s : signalsStorage.objects()){
        if(s->type()!=SignalType::BOOL) continue;

        if(s==_signal){
            if(s->id()==SystemParameter::FULL_SCREEN){
                settings->setFullScreen(_newValue);

            }else if(s->id()==SystemParameter::MUSIC_ACTIVE){
                settings->setMusicActive(_newValue);

            }else if(s->id()==SystemParameter::SFX_ACTIVE){
                settings->setMusicActive(_newValue);

            }else if(s->id()==SystemParameter::SPEECH_ACTIVE){
                settings->setMusicActive(_newValue);

            }

            app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));
        }
    }
}


void SettingsSignalCallback::onSetBySignalSetter(IntSignal *_signal, int _newValue)
{

    SystemSettings *settings = app->systemSettings();

    SimpleStorage<Signal*>signalsStorage = settings->signalsStorage();

    for(Signal* s : signalsStorage.objects()){
        if(s->type()!=SignalType::INT) continue;

        if(s==_signal){
            if(s->id()==SystemParameter::PIXEL_ZOOM){
                settings->setPixelZoom(_newValue);
            }

            app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));
        }
    }

}


void SettingsSignalCallback::onSetBySignalSetter(FloatSignal *_signal, float _newValue)
{

    SystemSettings *settings = app->systemSettings();

    SimpleStorage<Signal*>signalsStorage = settings->signalsStorage();

    for(Signal* s : signalsStorage.objects()){
        if(s->type()!=SignalType::FLOAT) continue;

        if(s==_signal){
            if(s->id()==SystemParameter::MUSIC_VOLUME){
                settings->setMusicVolume(_newValue);

            }else if(s->id()==SystemParameter::SFX_VOLUME){
                settings->setSfxVolume(_newValue);

            }else if(s->id()==SystemParameter::SPEECH_VOLUME){
                settings->setSpeechVolume(_newValue);

            }

            app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));
        }
    }

}


void SettingsSignalCallback::onSetBySignalSetter(StringSignal *_signal, const std::string &_newValue)
{

    SystemSettings *settings = app->systemSettings();

    SimpleStorage<Signal*>signalsStorage = settings->signalsStorage();

    for(Signal* s : signalsStorage.objects()){
        if(s->type()!=SignalType::STRING) continue;

        if(s==_signal){
            if(s->id()==SystemParameter::LANGUAGE){
                settings->setActiveLanguage(_newValue);
            }

            app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));
        }
    }

}

*/



}
