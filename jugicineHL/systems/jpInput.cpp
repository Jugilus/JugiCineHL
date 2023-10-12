#include <ncine/Application.h>
#include <ncine/IInputManager.h>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmStreams.h"
#include "jmCompositeSprite.h"
#include "jpItemsTable.h"
#include "jpPlayedApp.h"
#include "jpInputCustomizer.h"
#include "jmUtilities.h"
#include "jpUtilities.h"
#include "jpInput.h"


namespace jugimap{




GameInputCommand::GameInputCommand(const pugi::xml_node &_node)
{

    mName = _node.attribute("name").as_string("");

    mCfg.reset(new Cfg());

    mCfg->mDescription = _node.attribute("description").as_string("");
    mCfg->mDefKeyboardKey = _node.attribute("defKeyboardKey").as_string("");
    mCfg->mDefJoystickCommand = _node.attribute("defJoystickCommand").as_string("");
    mCfg->mTrigger = _node.attribute("trigger").as_string("heldDown");

}


bool GameInputCommand::init()
{

    dbgSystem.addMessage("init input command '" + mName + "'");


    bool ok = false;
    if(mCfg->mDescription.empty()==false){
        ok = ObtainPlainText(mCfg->mDescription, mDescription);
        if(ok==false){
            return false;
        }
    }


    // keyboard keys
    mDefKeyboardKey = keyboard.keyCodeForKeyName(mCfg->mDefKeyboardKey);
    if(mDefKeyboardKey==KeyCode::UNKNOWN){
        dbgSystem.addMessage("Wrong keyboard key code '" + mCfg->mDefKeyboardKey +"'");
        return false;
    }

    // joystick
    ok = GetJoystickCommandFromString(mCfg->mDefJoystickCommand, mDefJoystickCommand);
    if(ok==false){
        return false;
    }

    //trigger
    //mTrigger = getTriggerFromString(mCfg->mTrigger);
    //if(mTrigger==Trigger::NOT_DEFINED){
    //    return false;
    //}


    //---
    dbgSystem.removeLastMessage();
    return true;

}


void GameInputCommand::reset()
{
    BoolSignal::reset();

    mXAxis = mYAxis = mZAxis = 0.0f;

}


/*
GameInputCommand::Trigger GameInputCommand::getTriggerFromString(const std::string &_trigger)
{

    if(_trigger=="heldDown"){
        return Trigger::HELD_DOWN;

    }else if(_trigger=="pressed"){
        return Trigger::PRESSED;

    }else if(_trigger=="released"){
        return Trigger::RELEASED;
    }

    dbgSystem.addMessage("Wrong trigger '"+ _trigger +"'!");

    return Trigger::NOT_DEFINED;

}
*/

//_-----------------------------------------------------------------------




//void InputDeviceProfile::update()
//{



//}


//------------------------------------------------------------------------------

std::string KeyboardProfile::defName = "Keyboard";

KeyboardProfile::KeyboardProfile() : InputDeviceProfile(defName)
{
}


void KeyboardProfile::update()
{

    for(KeyboardInputCommand &c : mKeyboardInputCommands){

        //c.mGameInputCommand->reset();
        //c.mGameInputCommand->Reset();

        if(keyboard.isKeyDown(c.mKeyCode)){
            //c.mGameInputCommand->mActive = true;
            c.mGameInputCommand->_setActive(true);
        }
        if(keyboard.isKeyPressed(c.mKeyCode)){
            //c.mGameInputCommand->mStartedBeingActive = true;
            c.mGameInputCommand->_setActiveStarted(true);
        }
        if(keyboard.isKeyReleased(c.mKeyCode)){
            //c.mGameInputCommand->mEndedBeingActive = true;
            c.mGameInputCommand->_setActiveEnded(true);
        }
    }

}


KeyboardInputCommand* KeyboardProfile::getCommandWithKeyCode(KeyCode _keyCode)
{

    for(KeyboardInputCommand &c : mKeyboardInputCommands){
        if(c.mKeyCode==_keyCode){
            return &c;
        }
    }

    return nullptr;

}



//------------------------------------------------------------------------------

std::string JoystickProfile::defName = "Joystick";

JoystickProfile::JoystickProfile(Joystick *_joystick) : InputDeviceProfile(defName)
{
    mJoystick = _joystick;

}


void JoystickProfile::update()
{

    for(JoystickInputCommand &c : mJoystickInputCommands){

        //c.mGameInputCommand->reset();
        //c.mGameInputCommand->Reset();

        if(c.mJoystickControl.mButtonName != ncine::ButtonName::UNKNOWN){

            if(mJoystick->isButtonDown(c.mJoystickControl.mButtonName)){
                c.mGameInputCommand->_setActive(true);
            }
            if(mJoystick->isButtonPressed(c.mJoystickControl.mButtonName)){
                c.mGameInputCommand->_setActiveStarted(true);
            }
            if(mJoystick->isButtonReleased(c.mJoystickControl.mButtonName)){
                c.mGameInputCommand->_setActiveEnded(true);
            }

        }else if(c.mJoystickControl.mPovX != JoystickPOV_X::NONE){

            if(mJoystick->isPOV_XDown(c.mJoystickControl.mPovX)){
                c.mGameInputCommand->_setActive(true);
            }
            if(mJoystick->isPOV_XPressed(c.mJoystickControl.mPovX)){
                c.mGameInputCommand->_setActiveStarted(true);
            }
            if(mJoystick->isPOV_XReleased(c.mJoystickControl.mPovX)){
                c.mGameInputCommand->_setActiveEnded(true);
            }

        }else if(c.mJoystickControl.mPovY != JoystickPOV_Y::NONE){

            if(mJoystick->isPOV_YDown(c.mJoystickControl.mPovY)){
                c.mGameInputCommand->_setActive(true);
            }
            if(mJoystick->isPOV_YPressed(c.mJoystickControl.mPovY)){
                c.mGameInputCommand->_setActiveStarted(true);
            }
            if(mJoystick->isPOV_YReleased(c.mJoystickControl.mPovY)){
                c.mGameInputCommand->_setActiveEnded(true);
            }

        }else if(c.mJoystickControl.mAxis != JoystickAxis::NONE){

            if(c.mJoystickControl.mAxis==JoystickAxis::X){
                c.mGameInputCommand->mXAxis = mJoystick->xAxis();

            }else if(c.mJoystickControl.mAxis==JoystickAxis::Y){
                c.mGameInputCommand->mYAxis = mJoystick->yAxis();

            }else if(c.mJoystickControl.mAxis==JoystickAxis::Z){
                c.mGameInputCommand->mZAxis = mJoystick->zAxis();

            }

        }

    }

}


JoystickInputCommand *JoystickProfile::getCommandWithButtonName(ncine::ButtonName _buttonName)
{

    for(JoystickInputCommand &c : mJoystickInputCommands){
        if(c.mJoystickControl.mButtonName==_buttonName){
            return &c;
        }
    }

    return nullptr;

}

//------------------------------------------------------------------------


InputProfiles::~InputProfiles()
{


    for(KeyboardProfile *kp : mKeyboardProfiles){
        delete kp;
    }
    for(JoystickProfile *jp : mJoystickProfiles){
        delete jp;
    }
}


void InputProfiles::save(BinaryStreamWriter &stream)
{

    stream.writeInt(jpSaveSignatures::INPUT_PROFILES);

    //---
    stream.writeInt(jpSaveSignatures::KEYBOARD_PROFILES);
    stream.writeInt(mKeyboardProfiles.size());

    for(KeyboardProfile *p : mKeyboardProfiles){
        stream.writeInt(jpSaveSignatures::KEYBOARD_PROFILE);
        stream.writeInt(p->keyboardInputCommands().size());
        for(KeyboardInputCommand &c : p->keyboardInputCommands()){
            stream.writeInt(static_cast<int>(c.mKeyCode));
        }
    }

    //---
    stream.writeInt(jpSaveSignatures::JOYSTICK_PROFILES);
    stream.writeInt(mJoystickProfiles.size());

    for(JoystickProfile *p : mJoystickProfiles){
        stream.writeInt(jpSaveSignatures::JOYSTICK_PROFILE);
        stream.writeInt(p->joystickInputCommands().size());
        for(JoystickInputCommand &c : p->joystickInputCommands()){
            stream.writeInt(static_cast<short int>(c.mJoystickControl.mButtonName));
            stream.writeInt(static_cast<int>(c.mJoystickControl.mPovX));
            stream.writeInt(static_cast<int>(c.mJoystickControl.mPovY));
            stream.writeInt(static_cast<int>(c.mJoystickControl.mAxis));
        }
    }

}


void InputProfiles::load(StdBinaryFileStreamReader &stream)
{

    int signature = stream.ReadInt();
    assert(signature==jpSaveSignatures::KEYBOARD_PROFILES);


    int numKeyboardProfiles = stream.ReadInt();
    for(int i=0; i<numKeyboardProfiles; i++){

        int signature = stream.ReadInt();
        assert(signature==jpSaveSignatures::KEYBOARD_PROFILE);

        KeyboardProfile *profile = new KeyboardProfile();

        int nCommands = stream.ReadInt();

        for(int j=0; j<nCommands; j++){

            profile->keyboardInputCommands().push_back(KeyboardInputCommand());

            int keyCode = stream.ReadInt();
            profile->keyboardInputCommands().back().mKeyCode = static_cast<KeyCode>(keyCode);
        }

        mKeyboardProfiles.push_back(profile);
    }

    //---
    signature = stream.ReadInt();
    assert(signature==jpSaveSignatures::JOYSTICK_PROFILES);

    int numJoystickProfiles = stream.ReadInt();

    for(int i=0; i<numJoystickProfiles; i++){

        int signature = stream.ReadInt();
        assert(signature==jpSaveSignatures::JOYSTICK_PROFILE);


        JoystickProfile *profile = new JoystickProfile(nullptr);

        int nCommands = stream.ReadInt();
        for(int j=0; j<nCommands; j++){

            profile->joystickInputCommands().push_back(JoystickInputCommand());

            short int iButtonName = stream.ReadInt();
            int iPovX = stream.ReadInt();
            int iPovY = stream.ReadInt();
            int iAxis = stream.ReadInt();
            profile->joystickInputCommands().back().mJoystickControl.mButtonName = static_cast<ncine::ButtonName>(iButtonName);
            profile->joystickInputCommands().back().mJoystickControl.mPovX = static_cast<JoystickPOV_X>(iPovX);
            profile->joystickInputCommands().back().mJoystickControl.mPovY = static_cast<JoystickPOV_Y>(iPovY);
            profile->joystickInputCommands().back().mJoystickControl.mAxis = static_cast<JoystickAxis>(iAxis);
        }

        mJoystickProfiles.push_back(profile);
    }

}


//_-----------------------------------------------------------------------


InputSystem::InputSystem(PlayedApp *_playerApp)
{
    mParentPlayerApp = _playerApp;

    Joystick::createButtonNameStrings();
}


InputSystem::~InputSystem()
{
    for(GameInputCommand* c : mGameInputCommands){
        delete c;
    }
}


bool InputSystem::initCfg(const pugi::xml_node &_node)
{

    mCfg.reset(new Cfg());
    mCfg->mDefKeyboardName = _node.attribute("defKeyboardName").as_string("");
    mCfg->mDefJoystickName = _node.attribute("defJoystickName").as_string("");

    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        if(childNodeName=="command"){
            if(addGameInputCommand(nChild)==false){
                return false;
            }
        }
    }

    return true;
}


bool InputSystem::initConnections()
{

    if(mCfg.get()==nullptr){        // input commands were not defined in _application.xml
        return true;
    }

    dbgSystem.addMessage("init 'input manager' ");


    std::string defName;
    bool ok = false;

    if(mCfg->mDefKeyboardName.empty()==false){
        ok = ObtainPlainText(mCfg->mDefKeyboardName, defName);
        if(ok==false){
            return false;
        }
        KeyboardProfile::defName = defName;
    }


    if(mCfg->mDefJoystickName.empty()==false){
        ok = ObtainPlainText(mCfg->mDefJoystickName, defName);
        if(ok==false){
            return false;
        }
        JoystickProfile::defName = defName;
    }


    for(GameInputCommand* c : mGameInputCommands){
        if(c->init()==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();

    return true;
}



void InputSystem::update()
{

    assert(mActiveInputProfiles);

    for(GameInputCommand *c  : mGameInputCommands){
        c->reset();
    }

    //---
    for(KeyboardProfile *p : mActiveInputProfiles->mKeyboardProfiles){
        p->update();
    }
    for(JoystickProfile *p : mActiveInputProfiles->mJoystickProfiles){
        p->update();
    }

    //dbgPrint();

}




void InputSystem::setActiveUserProfiles(InputProfiles *_inputProfiles)
{

     mActiveInputProfiles = _inputProfiles;
     if(mInputCustomizer){
         mInputCustomizer->rebuildItems();
     }

}


bool InputSystem::addGameInputCommand(const pugi::xml_node &_node)
{

    GameInputCommand* c = new GameInputCommand(_node);

    if(getGameInputCommand(c->name(), false)){
        dbgSystem.addMessage("An input command with name '" + c->name() + "' already exists!");
        delete c;
        return false;
    }

    mGameInputCommands.push_back(c);

    return true;
}


GameInputCommand *InputSystem::getGameInputCommand(const std::string &_name, bool _setErrorMessage)
{

    for(GameInputCommand *c : mGameInputCommands){
        if(c->name() == _name){
            return c;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A game input command '" + _name + "' not found!");
    }

    return nullptr;

}


InputProfiles *InputSystem::newDefaultInputProfiles()
{

    InputProfiles *inputProfiles = new InputProfiles();

    //for(int i=1; i<=mNumberOfKeyboardProfiles; i++){

        //std::string name = (mNumberOfKeyboardProfiles>1)? mDefKeyboardName +" #" + std::to_string(i) : mDefKeyboardName;
        KeyboardProfile* kp = new KeyboardProfile();

        for(GameInputCommand* c : mGameInputCommands){
            kp->keyboardInputCommands().push_back(KeyboardInputCommand());
            kp->keyboardInputCommands().back().mGameInputCommand = c;
            kp->keyboardInputCommands().back().mKeyCode = c->defaultKeyboardKey();
        }

        inputProfiles->mKeyboardProfiles.push_back(kp);
    //}

    // for(int i=0; i<joysticks.size(); i++){
    // std::string name = mDefJoystickName +" #" + std::to_string(i+1);
    for(Joystick &j : joysticks){
        JoystickProfile* jp = new JoystickProfile(&j);

        for(GameInputCommand* c : mGameInputCommands){
            jp->joystickInputCommands().push_back(JoystickInputCommand());
            jp->joystickInputCommands().back().mGameInputCommand = c;
            jp->joystickInputCommands().back().mJoystickControl = c->defaultJoystickCommand();
        }

        inputProfiles->mJoystickProfiles.push_back(jp);
    }

    return inputProfiles;

}



void InputSystem::dbgPrint()
{

    for(GameInputCommand *c  : mGameInputCommands){
        if(c->activeStarted()){
            print("Input command '" + c->name() + " pressed!");
        }
        if(c->active()){
            print("Input command '" + c->name() + " down!");
        }
        if(c->activeEnded()){
            print("Input command '" + c->name() + " released!");
        }
        if(c->xAxis() != 0.0f ){
            print("Input command '" + c->name() + " X axis=" + std::to_string(c->xAxis()));
        }
        if(c->yAxis() != 0.0f ){
            print("Input command '" + c->name() + " Y axis=" + std::to_string(c->yAxis()));
        }
        if(c->zAxis() != 0.0f ){
            print("Input command '" + c->name() + " Z axis=" + std::to_string(c->zAxis()));
        }
    }


}




}
