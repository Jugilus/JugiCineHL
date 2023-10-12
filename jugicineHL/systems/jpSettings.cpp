#include "ncine/Application.h"
#include "ncine/IGfxDevice.h"
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiText.h"
#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "jmSpriteLayer.h"
#include "jmCompositeSprite.h"
#include "jmUtilities.h"
#include "jmGlobal.h"
#include "jmStreams.h"
#include "jpPlayedApp.h"
#include "jpItemsTable.h"
#include "jpUser.h"
#include "jpSettings.h"


namespace jugimap{




SystemSettings::SystemSettings(PlayedApp *_playerApp)
{

    mParentPlayerApp = _playerApp;


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

    /*
    if(loadSettingsFromIniFile()==false){
        saveSettingsIniFile();
    }

    //---

    ncine::IGfxDevice &gfxDevice = ncine::theApplication().gfxDevice();
    //if(isFullScreen()!=gfxDevice.isFullScreen()){
    //    gfxDevice.setFullScreen(isFullScreen());
    //}
    */

}


//bool SystemSettings::init()
//{

    //if(loadSettingsFromIniFile()==false){

//}



void SystemSettings::setFullScreen(bool _fullScreen)
{
    BoolVar *fullScreen = dynamic_cast<BoolVar*>(mVariables.getVariable("fullScreen"));     assert(fullScreen);

    bool fullScreenPrev = fullScreen->value();

    fullScreen->setValue(_fullScreen);

    ncine::IGfxDevice &gfxDevice = ncine::theApplication().gfxDevice();
    const ncine::IGfxDevice::VideoMode &currentMode = gfxDevice.currentVideoMode(0);
    int primaryMonitorIndex = gfxDevice.primaryMonitorIndex();
    const ncine::IGfxDevice::Monitor &monitor = gfxDevice.monitor(primaryMonitorIndex);


    if(_fullScreen){
        gfxDevice.setFullScreen(true);

    }else{
        Vec2i windowSize = app->designResolution().designResolution();
        Vec2i windowPos(monitor.position.x + (currentMode.width-windowSize.x)/2, monitor.position.y + (currentMode.height-windowSize.y)/2);

        gfxDevice.setFullScreen(false);
        gfxDevice.setWindowPosition(windowPos);
        if(fullScreenPrev){
            gfxDevice.setWindowSize(windowSize);
        }
    }

}


bool SystemSettings::isFullScreen()
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("fullScreen"));              assert(v);
    return v->value();
}


void SystemSettings::setPixelZoom(int _pixelZoom)
{
    IntVar *v =  dynamic_cast<IntVar*>(mVariables.getVariable("pixelZoom"));     assert(v);
    v->setValue(_pixelZoom);

}


int SystemSettings::pixelZoom()
{
    IntVar *v =  dynamic_cast<IntVar*>(mVariables.getVariable("pixelZoom"));     assert(v);
    return v->value();

}



void SystemSettings::setMusicActive(bool _musicActive)
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("musicActive"));     assert(v);
    v->setValue(_musicActive);

}


bool SystemSettings::isMusicActive()
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("musicActive"));     assert(v);
    return v->value();
}


void SystemSettings::setSfxActive(bool _sfxActive)
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("sfxActive"));     assert(v);
    v->setValue(_sfxActive);

}


bool SystemSettings::isSfxActive()
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("sfxActive"));     assert(v);
    return v->value();
}


void SystemSettings::setSpeechActive(bool _speechActive)
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("speechActive"));     assert(v);
    v->setValue(_speechActive);

}


bool SystemSettings::isSpeechActive()
{
    BoolVar *v = dynamic_cast<BoolVar*>(mVariables.getVariable("speechActive"));     assert(v);
    return v->value();
}


void SystemSettings::setMusicVolume(int _musicVolume)
{
    IntVar *v = dynamic_cast<IntVar*>(mVariables.getVariable("musicVolume"));     assert(v);
    v->setValue(_musicVolume);

}


int SystemSettings::musicVolume()
{
    IntVar *v = dynamic_cast<IntVar*>(mVariables.getVariable("musicVolume"));     assert(v);
    return v->value();
}


void SystemSettings::setSfxVolume(int _sfxVolume)
{
    IntVar *v = dynamic_cast<IntVar*>(mVariables.getVariable("sfxVolume"));     assert(v);
    v->setValue(_sfxVolume);

}


int SystemSettings::sfxVolume()
{
    IntVar *v = dynamic_cast<IntVar*>(mVariables.getVariable("sfxVolume"));     assert(v);
    return v->value();
}



void SystemSettings::setSpeechVolume(int _speechVolume)
{
    IntVar *v = dynamic_cast<IntVar*>(mVariables.getVariable("speechVolume"));     assert(v);
    v->setValue(_speechVolume);

}


int SystemSettings::speechVolume()
{
    IntVar *v = dynamic_cast<IntVar*>(mVariables.getVariable("speechVolume"));     assert(v);
    return v->value();
}



void SystemSettings::setActiveLanguage(const std::string &_language)
{
    StringVar *v = dynamic_cast<StringVar*>(mVariables.getVariable("language"));     assert(v);
    v->setValue(_language);

    textLibrary.setActiveLanguage(v->value());
}


const std::string & SystemSettings::activeLanguage()
{
    StringVar *v = dynamic_cast<StringVar*>(mVariables.getVariable("language"));     assert(v);
    return v->value();

}



bool SystemSettings::saveSettingsIniFile()
{


    std::string filePath = mParentPlayerApp->saveDataDir() + "/settings.ini";
    TextStreamWriter stream(filePath);

    if(stream.IsOpen()==false){
        return false;
    }

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


    stream.Close();

    print("Saved settings ini file!");

    return true;
}



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


}
