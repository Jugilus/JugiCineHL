#include <ncine/Application.h>
#include <ncine/IInputManager.h>
#include <ncine/IFile.h>
#include <ncine/FileSystem.h>
#include <ncine/Keys.h>


#include "main.h"
#include "jmStreams.h"
#include "jmSignal.h"
#include "jpObjectFactory.h"
#include "jpInput.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpPlayer.h"
//#include "entities/jpB2World.h"


nctl::UniquePtr<ncine::IAppEventHandler> createAppEventHandler()
{

    MyEventHandler *eh = new MyEventHandler();
    return nctl::UniquePtr<ncine::IAppEventHandler>(eh);
}


//==========================================================================================


void MyEventHandler::onPreInit(ncine::AppConfiguration &config)
{


    //gSignalTest.test();
    //return;


#if defined(__ANDROID__)
    config.dataPath() = "asset::";
#elif defined(__EMSCRIPTEN__)
    config.dataPath() = "";

#else
    #ifdef NCPROJECT_DEFAULT_DATA_DIR
    config.dataPath() = NCPROJECT_DEFAULT_DATA_DIR;
    #else
    //config.dataPath() = "data/";
    config.dataPath() = "";
    #endif
#endif




    // if the player application is not located in the root directory of the game project we need first to set
    // the current directory to the project directory (useful during development)
#if defined(HARDCODED_PROJECT_PATH)
    bool ok = ncine::FileSystem::setCurrentDir("c:/Development/JugiMapProject/JugiCineHL/examples/Testbed");
#endif

    jugimap::actionFactory = new jugimap::ActionFactory();
    jugimap::player = new jugimap::Player();
    jugimap::player->preInit();

    //jugimap::Player * player = static_cast<jugimap::Player *>(jugimap::gameApp);

    //config.resolution.set(1400, 900);
    config.resolution.set(jugimap::app->designResolution().width, jugimap::app->designResolution().height);
    config.windowTitle = nctl::String(jugimap::app->name().c_str());

    config.deferShaderQueries = false;
    config.withVSync = true;
    config.resizable = false;
    config.fullScreen = false;

    //config.withDebugOverlay = true;
    //config.withGlDebugContext = true;
}



void MyEventHandler::onInit()
{

    //if(gSignalTest.mRunning){
    //    return;
    //}


    //----
    ncine::theApplication().setAutoSuspension(true);

    ncine::theApplication().screenViewport().setClearColor(190/255.0f, 190/255.0f, 190/255.0f, 255/255.0f);
    //jugimap::gWorldInfo.init();

    #if !defined (__ANDROID__)
        //jugimap::JugiMapBinaryLoader::pathPrefix = std::string(ncine::FileSystem::dataPath().data());
    #endif

    jugimap::player->init();

}



void MyEventHandler::onFrameStart()
{

    //if(gSignalTest.mRunning){
    //    ncine::theApplication().quit();
    //    return;
    //}


    jugimap::player->update(ncine::theApplication().interval());

}



void MyEventHandler::onPostUpdate()
{

    //if(gSignalTest.mRunning){
    //    return;
    //}

    //jugiApp::application->PostUpdate(ncine::theApplication().interval());

    //jugiApp::application->drawShapes();

    ncine::Sprite * dbgSpr = jugimap::dbgNcSprite;

    jugimap::app->drawShapes();


}


//#ifdef __ANDROID__

void MyEventHandler::onTouchDown(const ncine::TouchEvent &event)
{

    int pointerID = 0;
    int pointerIndex = event.findPointerIndex(pointerID++);

    while(pointerIndex != -1){
        const ncine::TouchEvent::Pointer &pointer = event.pointers[pointerIndex];
        jugimap::touch._setFingerState(pointerID, true, jugimap::Vec2i(pointer.x, pointer.y));

        pointerIndex = event.findPointerIndex(pointerID++);
    }


//#ifdef __ANDROID__

    jugimap::mouse._setButtonState(jugimap::MouseButton::LEFT, true);

//#endif

}


void MyEventHandler::onTouchUp(const ncine::TouchEvent &event)
{

    int pointerID = 0;
    int pointerIndex = event.findPointerIndex(pointerID++);

    while(pointerIndex != -1){
        const ncine::TouchEvent::Pointer &pointer = event.pointers[pointerIndex];
        jugimap::touch._setFingerState(pointerID, false, jugimap::Vec2i(pointer.x, pointer.y));

        pointerIndex = event.findPointerIndex(pointerID++);
    }


//#ifdef __ANDROID__

    jugimap::mouse._setButtonState(jugimap::MouseButton::LEFT, false);

//#endif

}


void MyEventHandler::onTouchMove(const ncine::TouchEvent &event)
{

    int pointerID = 0;
    int pointerIndex = event.findPointerIndex(pointerID++);

    while(pointerIndex != -1){
        const ncine::TouchEvent::Pointer &pointer = event.pointers[pointerIndex];
        jugimap::touch._setFingerState(pointerID, true, jugimap::Vec2i(pointer.x, pointer.y));

        pointerIndex = event.findPointerIndex(pointerID++);
    }


//#ifdef __ANDROID__

    jugimap::mouse._setPosition(jugimap::Vec2i(event.pointers[0].x, event.pointers[0].y));

//#endif

}


void MyEventHandler::onMouseButtonPressed(const ncine::MouseEvent &event)
{
    if(event.isLeftButton()){
        jugimap::mouse._setButtonState(jugimap::MouseButton::LEFT, true);

    }else if(event.isMiddleButton()){
        jugimap::mouse._setButtonState(jugimap::MouseButton::MIDDLE, true);

    }else if(event.isRightButton()){
        jugimap::mouse._setButtonState(jugimap::MouseButton::RIGHT, true);

    }
}


void MyEventHandler::onMouseButtonReleased(const ncine::MouseEvent &event)
{
    if(event.isLeftButton()){
        jugimap::mouse._setButtonState(jugimap::MouseButton::LEFT, false);

    }else if(event.isMiddleButton()){
        jugimap::mouse._setButtonState(jugimap::MouseButton::MIDDLE, false);

    }else if(event.isRightButton()){
        jugimap::mouse._setButtonState(jugimap::MouseButton::RIGHT, false);
    }
}


void MyEventHandler::onMouseMoved(const ncine::MouseState &state)
{
    jugimap::mouse._setPosition(jugimap::Vec2i(state.x, state.y));
}


void MyEventHandler::onScrollInput(const ncine::ScrollEvent &event)
{
    jugimap::mouse._setWheel(jugimap::Vec2i(event.x, event.y));

}


void MyEventHandler::onKeyPressed(const ncine::KeyboardEvent &event)
{

    if(event.sym==ncine::KeySym::UNKNOWN) return;

    //jugimap::KeyCode k = keysConversionTable[(int)event.sym];
    //jugimap::keyboard._SetKeyState(k, true);
    jugimap::keyboard._setKeyState(event.sym, true);

}


void MyEventHandler::onKeyReleased(const ncine::KeyboardEvent &event)
{

    if(event.sym==ncine::KeySym::UNKNOWN) return;

    //jugimap::KeyCode k = keysConversionTable[(int)event.sym];
    //jugimap::keyboard._SetKeyState(k, false);
     jugimap::keyboard._setKeyState(event.sym, false);

}


void MyEventHandler::onJoyButtonPressed(const ncine::JoyButtonEvent &event)
{

    //if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;

    //jugimap::joysticks[event.joyId]._SetButtonState(event.buttonId, true);

}


void MyEventHandler::onJoyButtonReleased(const ncine::JoyButtonEvent &event)
{

    //if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;

    //jugimap::joysticks[event.joyId]._SetButtonState(event.buttonId, false);

}


void MyEventHandler::onJoyMappedButtonPressed(const ncine::JoyMappedButtonEvent &event)
{

    if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;


    jugimap::joysticks[event.joyId]._setButtonStateViaMappedButton(event.buttonName, true);

}


void MyEventHandler::onJoyMappedButtonReleased(const ncine::JoyMappedButtonEvent &event)
{

    if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;


    jugimap::joysticks[event.joyId]._setButtonStateViaMappedButton(event.buttonName, false);

}


void MyEventHandler::onJoyHatMoved(const ncine::JoyHatEvent &event)
{

    if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;


    if(event.hatId==0){

        if(event.hatState==ncine::HatState::CENTERED){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::NONE);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::NONE);

        }else if(event.hatState==ncine::HatState::UP){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::NONE);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::UP);

        }else if(event.hatState==ncine::HatState::RIGHT_UP){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::RIGHT);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::UP);

        }else if(event.hatState==ncine::HatState::RIGHT){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::RIGHT);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::NONE);

        }else if(event.hatState==ncine::HatState::RIGHT_DOWN){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::RIGHT);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::DOWN);

        }else if(event.hatState==ncine::HatState::DOWN){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::NONE);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::DOWN);

        }else if(event.hatState==ncine::HatState::LEFT_DOWN){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::LEFT);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::DOWN);

        }else if(event.hatState==ncine::HatState::LEFT){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::LEFT);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::NONE);

        }else if(event.hatState==ncine::HatState::LEFT_UP){
            jugimap::joysticks[event.joyId]._setPOV_X(jugimap::JoystickPOV_X::LEFT);
            jugimap::joysticks[event.joyId]._setPOV_Y(jugimap::JoystickPOV_Y::UP);

        }
    }
}


void MyEventHandler::onJoyAxisMoved(const ncine::JoyAxisEvent &event)
{

    if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;

    if(event.axisId==0){
        jugimap::joysticks[event.joyId]._setXaxis(event.normValue);

    }else if(event.axisId==1){
        jugimap::joysticks[event.joyId]._setYaxis(event.normValue);

    }else if(event.axisId==2){
        jugimap::joysticks[event.joyId]._setZaxis(event.normValue);
    }

}


void MyEventHandler::onJoyConnected(const ncine::JoyConnectionEvent &event)
{

    if(event.joyId<0 || event.joyId > jugimap::joysticks.size()) return;

    jugimap::joysticks[event.joyId]._setConnected(true);


    ncine::IInputManager &inputManager = ncine::theApplication().inputManager();
    std::string joystickName = inputManager.joyName(event.joyId);
    if(joystickName.empty()==false){
        jugimap::joysticks[event.joyId]._setName(joystickName);
    }

    //jugimap::print(" joystick name:"+ jugimap::joysticks[event.joyId].name());
}


void MyEventHandler::onJoyDisconnected(const ncine::JoyConnectionEvent &event)
{

    if(event.joyId<0 || event.joyId>jugimap::joysticks.size()) return;

    jugimap::joysticks[event.joyId]._setConnected(false);
}


void MyEventHandler::onTextInput(const ncine::TextInputEvent &event)
{

    //jugimap::DummyFunction();
    //int code = event.text[0]; // | (event.text[2]<<8) | (event.text[1]<<16) | (event.text[0]<<24);
    //int code = event.text[0] - '0';

    std::string letter(event.text);
    jugimap::keyboard._setTextInputChar(letter);

    //utf8::append(code, letter);
    jugimap::DummyFunction();
}


void MyEventHandler::onResizeWindow(int width, int height)
{

    jugimap::app->onResizeWindow(width, height);

}


void MyEventHandler::onShutdown()
{
    delete jugimap::actionFactory;
    delete jugimap::player;

}


//================================================================================================


void SignalTest::test()
{

    test_BitsetSignal_2();

    //test_IntSignal_2();

}


void SignalTest::test_BitsetSignal_1()
{

    jugimap::BitsetSignal sigBitset;

    sigBitset.setDefaultValue(value3 | value5);
    sigBitset.reset();

    dbgPrint_BitsetSignal(sigBitset);


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setValue(value0 | value4);
    sigBitset.setValue(value4);
    sigBitset.setValue(value6 | value7);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setValue(value0 | value6);
    //sigBitset.setFlags(value1, true);
    //sigBitset.setFlags(value4, false);
    //sigBitset.setFlags(value0, false);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setFlags(value4, false);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setFlags(value2, true);
    sigBitset.setValue(value1 | value3 | value5);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.reset(value2 | value5);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.reset();
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setFlags( value1 | value2 | value7, true);
    sigBitset.setFlags(value3, false);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();

}



void SignalTest::test_BitsetSignal_2()
{

    jugimap::BitsetSignal sigBitset;

    sigBitset.setDefaultValue(value3 | value5);
    sigBitset.reset();

    dbgPrint_BitsetSignal(sigBitset);


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setValue_onNextFrame(value0 | value4);
    sigBitset.setValue_onNextFrame(value4);
    sigBitset.setValue_onNextFrame(value6 | value7);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setValue_onNextFrame(value0 | value6);
    //sigBitset.setFlags(value1, true);
    //sigBitset.setFlags(value4, false);
    //sigBitset.setFlags(value0, false);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setFlags_onNextFrame(value4, false);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setFlags_onNextFrame(value2, true);
    sigBitset.setValue_onNextFrame(value1 | value3 | value5);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
   // sigBitset.reset(value2 | value5);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.reset();
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigBitset.setFlags_onNextFrame( value1 | value2 | value7, true);
    sigBitset.setFlags_onNextFrame(value3, false);
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_BitsetSignal(sigBitset);
    jugimap::gSignalUpdater.postUpdateSignals();

}


void SignalTest::dbgPrint_BitsetSignal(jugimap::BitsetSignal sigBitField, unsigned int nBits)
{

    std::string sValue;

    for(unsigned int i=0; i<nBits; i++){
        unsigned int bitValue = (unsigned int)1 << i;
        sValue = "Value" + std::to_string(i);
        sValue += " active:" + std::to_string(int(sigBitField.active(bitValue)));
        sValue += " start:" + std::to_string(int(sigBitField.activeStarted(bitValue)));
        sValue += " end:" + std::to_string(int(sigBitField.activeEnded(bitValue)));
        sValue += " changed:" + std::to_string(int(sigBitField.activeChanged(bitValue)));
        sValue += " notActive:" + std::to_string(int(sigBitField.notActive(bitValue)));

        jugimap::print(sValue);
     }

    jugimap::print("");

}


void SignalTest::test_IntSignal_1()
{

    jugimap::IntSignal sigInt{"", intValue2};

    dbgPrint_IntSignal(sigInt);


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.setValue(intValue0);
    sigInt.setValue(intValue4);
    sigInt.setValue(intValue6);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.setValue(intValue1);
    sigInt.setValue(intValue6);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.setValue(intValue4);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.reset(intValue5);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


}


void SignalTest::test_IntSignal_2()
{

    jugimap::IntSignal sigInt{"", intValue2};

    dbgPrint_IntSignal(sigInt);


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.setValue_onNextFrame(intValue0);
    sigInt.setValue_onNextFrame(intValue4);
    sigInt.setValue_onNextFrame(intValue6);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.setValue_onNextFrame(intValue1);
    sigInt.setValue_onNextFrame(intValue6);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.setValue_onNextFrame(intValue4);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    sigInt.reset(intValue5);
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


    //--- preUpdate
    jugimap::gSignalUpdater.preUpdateSignals();
    //--- update
    dbgPrint_IntSignal(sigInt);
    jugimap::gSignalUpdater.postUpdateSignals();


}



void SignalTest::dbgPrint_IntSignal(jugimap::IntSignal sigInt, unsigned int nValues)
{

    std::string sValue;

    for(unsigned int i=0; i<nValues; i++){
        sValue = "Value" + std::to_string(i);
        sValue += " active:" + std::to_string(int(sigInt.active(i)));
        sValue += " start:" + std::to_string(int(sigInt.activeStarted(i)));
        sValue += " end:" + std::to_string(int(sigInt.activeEnded(i)));
        sValue += " changed:" + std::to_string(int(sigInt.activeChanged(i)));
        sValue += " notActive:" + std::to_string(int(sigInt.notActive(i)));

        jugimap::print(sValue);
     }

    jugimap::print("");

}



SignalTest gSignalTest;



