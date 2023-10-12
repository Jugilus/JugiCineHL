#include "jmCommon.h"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmSprite.h"
#include "jmSourceSprite.h"
#include "jmGlobal.h"
#include "jmInput.h"



namespace jugimap{
	
	




//======================================================================================

Keyboard::Keyboard()
{

    mKeys.resize(static_cast<int>(KeyCode::COUNT));
    mKeyNames.resize(mKeys.size(), "");


    mKeyNames[(int)KeyCode::BACKSPACE] = "BACKSPACE";
    mKeyNames[(int)KeyCode::TAB] = "TAB";
    mKeyNames[(int)KeyCode::RETURN] = "RETURN";
    mKeyNames[(int)KeyCode::ESCAPE] = "ESCAPE";
    mKeyNames[(int)KeyCode::SPACE] = "SPACE";
    mKeyNames[(int)KeyCode::QUOTE] = "QUOTE";

    mKeyNames[(int)KeyCode::PLUS] = "PLUS";
    mKeyNames[(int)KeyCode::COMMA] = "COMMA";
    mKeyNames[(int)KeyCode::MINUS] = "MINUS";
    mKeyNames[(int)KeyCode::PERIOD] = "PERIOD";
    mKeyNames[(int)KeyCode::SLASH] = "SLASH";
    mKeyNames[(int)KeyCode::N0] = "N0";
    mKeyNames[(int)KeyCode::N1] = "N1";
    mKeyNames[(int)KeyCode::N2] = "N2";
    mKeyNames[(int)KeyCode::N3] = "N3";
    mKeyNames[(int)KeyCode::N4] = "N4";
    mKeyNames[(int)KeyCode::N5] = "N5";
    mKeyNames[(int)KeyCode::N6] = "N6";
    mKeyNames[(int)KeyCode::N7] = "N7";
    mKeyNames[(int)KeyCode::N8] = "N8";
    mKeyNames[(int)KeyCode::N9] = "N9";

    mKeyNames[(int)KeyCode::SEMICOLON] = "SEMICOLON";
    mKeyNames[(int)KeyCode::LEFTBRACKET] = "LEFTBRACKET";
    mKeyNames[(int)KeyCode::BACKSLASH] = "BACKSLASH";
    mKeyNames[(int)KeyCode::RIGHTBRACKET] = "RIGHTBRACKET";
    mKeyNames[(int)KeyCode::BACKQUOTE] = "BACKQUOTE";

    mKeyNames[(int)KeyCode::A] = "A";
    mKeyNames[(int)KeyCode::B] = "B";
    mKeyNames[(int)KeyCode::C] = "C";
    mKeyNames[(int)KeyCode::D] = "D";
    mKeyNames[(int)KeyCode::E] = "E";
    mKeyNames[(int)KeyCode::F] = "F";
    mKeyNames[(int)KeyCode::G] = "G";
    mKeyNames[(int)KeyCode::H] = "H";
    mKeyNames[(int)KeyCode::I] = "I";
    mKeyNames[(int)KeyCode::J] = "J";
    mKeyNames[(int)KeyCode::K] = "K";
    mKeyNames[(int)KeyCode::L] = "L";
    mKeyNames[(int)KeyCode::M] = "M";
    mKeyNames[(int)KeyCode::N] = "N";
    mKeyNames[(int)KeyCode::O] = "O";
    mKeyNames[(int)KeyCode::P] = "P";
    mKeyNames[(int)KeyCode::Q] = "Q";
    mKeyNames[(int)KeyCode::R] = "R";
    mKeyNames[(int)KeyCode::S] = "S";
    mKeyNames[(int)KeyCode::T] = "T";
    mKeyNames[(int)KeyCode::U] = "U";
    mKeyNames[(int)KeyCode::V] = "V";
    mKeyNames[(int)KeyCode::W] = "W";
    mKeyNames[(int)KeyCode::X] = "X";
    mKeyNames[(int)KeyCode::Y] = "Y";
    mKeyNames[(int)KeyCode::Z] = "Z";
    mKeyNames[(int)KeyCode::DELETE] = "DELETE";

    mKeyNames[(int)KeyCode::KP0] = "KP0";
    mKeyNames[(int)KeyCode::KP1] = "KP1";
    mKeyNames[(int)KeyCode::KP2] = "KP2";
    mKeyNames[(int)KeyCode::KP3] = "KP3";
    mKeyNames[(int)KeyCode::KP4] = "KP4";
    mKeyNames[(int)KeyCode::KP5] = "KP5";
    mKeyNames[(int)KeyCode::KP6] = "KP6";
    mKeyNames[(int)KeyCode::KP7] = "KP7";
    mKeyNames[(int)KeyCode::KP8] = "KP8";
    mKeyNames[(int)KeyCode::KP9] = "KP9";

    mKeyNames[(int)KeyCode::KP_PERIOD] = "KP_PERIOD";
    mKeyNames[(int)KeyCode::KP_DIVIDE] = "KP_DIVIDE";
    mKeyNames[(int)KeyCode::KP_MULTIPLY] = "KP_MULTIPLY";
    mKeyNames[(int)KeyCode::KP_MINUS] = "KP_MINUS";
    mKeyNames[(int)KeyCode::KP_PLUS] = "KP_PLUS";
    mKeyNames[(int)KeyCode::KP_ENTER] = "KP_ENTER";
    mKeyNames[(int)KeyCode::KP_EQUALS] = "KP_EQUALS";

    mKeyNames[(int)KeyCode::UP] = "UP";
    mKeyNames[(int)KeyCode::DOWN] = "DOWN";
    mKeyNames[(int)KeyCode::RIGHT] = "RIGHT";
    mKeyNames[(int)KeyCode::LEFT] = "LEFT";
    mKeyNames[(int)KeyCode::INSERT] = "INSERT";
    mKeyNames[(int)KeyCode::HOME] = "HOME";
    mKeyNames[(int)KeyCode::END] = "END";
    mKeyNames[(int)KeyCode::PAGEUP] = "PAGEUP";
    mKeyNames[(int)KeyCode::PAGEDOWN] = "PAGEDOWN";

    mKeyNames[(int)KeyCode::F1] = "F1";
    mKeyNames[(int)KeyCode::F2] = "F2";
    mKeyNames[(int)KeyCode::F3] = "F3";
    mKeyNames[(int)KeyCode::F4] = "F4";
    mKeyNames[(int)KeyCode::F5] = "F5";
    mKeyNames[(int)KeyCode::F6] = "F6";
    mKeyNames[(int)KeyCode::F7] = "F7";
    mKeyNames[(int)KeyCode::F8] = "F8";
    mKeyNames[(int)KeyCode::F9] = "F9";
    mKeyNames[(int)KeyCode::F10] = "F10";
    mKeyNames[(int)KeyCode::F11] = "F11";
    mKeyNames[(int)KeyCode::F12] = "F12";
    mKeyNames[(int)KeyCode::F13] = "F13";
    mKeyNames[(int)KeyCode::F14] = "F14";
    mKeyNames[(int)KeyCode::F15] = "F15";

    mKeyNames[(int)KeyCode::NUM_LOCK] = "NUM_LOCK";
    mKeyNames[(int)KeyCode::CAPS_LOCK] = "CAPS_LOCK";
    mKeyNames[(int)KeyCode::SCROLL_LOCK] = "SCROLL_LOCK";
    mKeyNames[(int)KeyCode::RSHIFT] = "RSHIFT";
    mKeyNames[(int)KeyCode::LSHIFT] = "LSHIFT";
    mKeyNames[(int)KeyCode::RCTRL] = "RCTRL";
    mKeyNames[(int)KeyCode::LCTRL] = "LCTRL";
    mKeyNames[(int)KeyCode::RALT] = "RALT";
    mKeyNames[(int)KeyCode::LALT] = "LALT";
    mKeyNames[(int)KeyCode::RSUPER] = "RSUPER";
    mKeyNames[(int)KeyCode::LSUPER] = "LSUPER";
    mKeyNames[(int)KeyCode::PRINTSCREEN] = "PRINTSCREEN";
    mKeyNames[(int)KeyCode::PAUSE] = "PAUSE";
    mKeyNames[(int)KeyCode::MENU] = "MENU";
    mKeyNames[(int)KeyCode::PAUSE] = "PAUSE";

    // SDL only keysyms
    mKeyNames[(int)KeyCode::CLEAR] = "CLEAR";            // Android too
    mKeyNames[(int)KeyCode::EXCLAIM] = "EXCLAIM";
    mKeyNames[(int)KeyCode::QUOTEDBL] = "QUOTEDBL";
    mKeyNames[(int)KeyCode::HASH] = "HASH";
    mKeyNames[(int)KeyCode::DOLLAR] = "DOLLAR";
    mKeyNames[(int)KeyCode::AMPERSAND] = "AMPERSAND";
    mKeyNames[(int)KeyCode::LEFTPAREN] = "LEFTPAREN";
    mKeyNames[(int)KeyCode::RIGHTPAREN] = "RIGHTPAREN";
    mKeyNames[(int)KeyCode::ASTERISK] = "ASTERISK";
    mKeyNames[(int)KeyCode::COLON] = "COLON";
    mKeyNames[(int)KeyCode::LESS] = "LESS";
    mKeyNames[(int)KeyCode::EQUALS] = "EQUALS";           // Android too
    mKeyNames[(int)KeyCode::GREATER] = "GREATER";
    mKeyNames[(int)KeyCode::QUESTION] = "QUESTION";
    mKeyNames[(int)KeyCode::AT] = "AT";                   // Android too
    mKeyNames[(int)KeyCode::CARET] = "CARET";
    mKeyNames[(int)KeyCode::UNDERSCORE] = "UNDERSCORE";
    mKeyNames[(int)KeyCode::MODE] = "MODE";
    mKeyNames[(int)KeyCode::APPLICATION] = "APPLICATION";
    mKeyNames[(int)KeyCode::HELP] = "HELP";
    mKeyNames[(int)KeyCode::SYSREQ] = "SYSREQ";               // Android too
    mKeyNames[(int)KeyCode::POWER] = "POWER";                 // Android too
    mKeyNames[(int)KeyCode::UNDO] = "UNDO";

    // GLFW only keysyms
    mKeyNames[(int)KeyCode::WORLD1] = "WORLD1";
    mKeyNames[(int)KeyCode::WORLD2] = "WORLD2";

    // Android only keysyms
    mKeyNames[(int)KeyCode::SOFT_LEFT] = "SOFT_LEFT";
    mKeyNames[(int)KeyCode::SOFT_RIGHT] = "SOFT_RIGHT";
    mKeyNames[(int)KeyCode::BACK] = "BACK";
    mKeyNames[(int)KeyCode::CALL] = "CALL";
    mKeyNames[(int)KeyCode::ENDCALL] = "ENDCALL";
    mKeyNames[(int)KeyCode::STAR] = "STAR";
    mKeyNames[(int)KeyCode::POUND] = "POUND";
    mKeyNames[(int)KeyCode::DPAD_CENTER] = "DPAD_CENTER";
    mKeyNames[(int)KeyCode::VOLUME_UP] = "VOLUME_UP";
    mKeyNames[(int)KeyCode::VOLUME_DOWN] = "VOLUME_DOWN";
    mKeyNames[(int)KeyCode::CAMERA] = "CAMERA";
    mKeyNames[(int)KeyCode::SYM] = "SYM";
    mKeyNames[(int)KeyCode::EXPLORER] = "EXPLORER";
    mKeyNames[(int)KeyCode::ENVELOPE] = "ENVELOPE";
    mKeyNames[(int)KeyCode::NUM] = "NUM";
    mKeyNames[(int)KeyCode::HEADSETHOOK] = "HEADSETHOOK";
    mKeyNames[(int)KeyCode::FOCUS] = "FOCUS";
    mKeyNames[(int)KeyCode::NOTIFICATION] = "NOTIFICATION";
    mKeyNames[(int)KeyCode::MEDIA_PLAY_PAUSE] = "MEDIA_PLAY_PAUSE";
    mKeyNames[(int)KeyCode::MEDIA_STOP] = "MEDIA_STOP";
    mKeyNames[(int)KeyCode::MEDIA_NEXT] = "MEDIA_NEXT";
    mKeyNames[(int)KeyCode::MEDIA_PREVIOUS] = "MEDIA_PREVIOUS";
    mKeyNames[(int)KeyCode::MEDIA_REWIND] = "MEDIA_REWIND";
    mKeyNames[(int)KeyCode::MEDIA_FAST_FORWARD] = "MEDIA_FAST_FORWARD";
    mKeyNames[(int)KeyCode::MUTE] = "MUTE";
    mKeyNames[(int)KeyCode::PICTSYMBOLS] = "PICTSYMBOLS";
    mKeyNames[(int)KeyCode::SWITCH_CHARSET] = "SWITCH_CHARSET";
    mKeyNames[(int)KeyCode::BUTTON_A] = "BUTTON_A";
    mKeyNames[(int)KeyCode::BUTTON_B] = "BUTTON_B";
    mKeyNames[(int)KeyCode::BUTTON_C] = "BUTTON_C";
    mKeyNames[(int)KeyCode::BUTTON_X] = "BUTTON_X";
    mKeyNames[(int)KeyCode::BUTTON_Y] = "BUTTON_Y";
    mKeyNames[(int)KeyCode::BUTTON_Z] = "BUTTON_Z";
    mKeyNames[(int)KeyCode::BUTTON_L1] = "BUTTON_L1";
    mKeyNames[(int)KeyCode::BUTTON_R1] = "BUTTON_R1";
    mKeyNames[(int)KeyCode::BUTTON_L2] = "BUTTON_L2";
    mKeyNames[(int)KeyCode::BUTTON_R2] = "BUTTON_R2";
    mKeyNames[(int)KeyCode::BUTTON_THUMBL] = "BUTTON_THUMBL";
    mKeyNames[(int)KeyCode::BUTTON_THUMBR] = "BUTTON_THUMBR";
    mKeyNames[(int)KeyCode::BUTTON_START] = "BUTTON_START";
    mKeyNames[(int)KeyCode::BUTTON_SELECT] = "BUTTON_SELECT";
    mKeyNames[(int)KeyCode::BUTTON_MODE] = "BUTTON_MODE";
    // From API level 13
    mKeyNames[(int)KeyCode::FUNCTION_KEY] = "FUNCTION_KEY";
    mKeyNames[(int)KeyCode::MOVE_HOME] = "MOVE_HOME";
    mKeyNames[(int)KeyCode::MOVE_END] = "MOVE_END";
    mKeyNames[(int)KeyCode::FORWARD] = "FORWARD";
    mKeyNames[(int)KeyCode::MEDIA_PLAY] = "MEDIA_PLAY";
    mKeyNames[(int)KeyCode::MEDIA_PAUSE] = "MEDIA_PAUSE";
    mKeyNames[(int)KeyCode::MEDIA_CLOSE] = "MEDIA_CLOSE";
    mKeyNames[(int)KeyCode::MEDIA_EJECT] = "MEDIA_EJECT";
    mKeyNames[(int)KeyCode::MEDIA_RECORD] = "MEDIA_RECORD";
    mKeyNames[(int)KeyCode::KP_COMMA] = "KP_COMMA";
    mKeyNames[(int)KeyCode::KP_LEFTPAREN] = "KP_LEFTPAREN";
    mKeyNames[(int)KeyCode::KP_RIGHTPAREN] = "KP_RIGHTPAREN";
    mKeyNames[(int)KeyCode::VOLUME_MUTE] = "VOLUME_MUTE";
    mKeyNames[(int)KeyCode::INFO] = "INFO";
    mKeyNames[(int)KeyCode::CHANNEL_UP] = "CHANNEL_UP";
    mKeyNames[(int)KeyCode::CHANNEL_DOWN] = "CHANNEL_DOWN";
    mKeyNames[(int)KeyCode::ZOOM_IN] = "ZOOM_IN";
    mKeyNames[(int)KeyCode::ZOOM_OUT] = "ZOOM_OUT";
    mKeyNames[(int)KeyCode::TV] = "TV";
    mKeyNames[(int)KeyCode::WINDOW] = "WINDOW";
    mKeyNames[(int)KeyCode::GUIDE] = "GUIDE";
    mKeyNames[(int)KeyCode::DVR] = "DVR";
    mKeyNames[(int)KeyCode::BOOKMARK] = "BOOKMARK";
    mKeyNames[(int)KeyCode::CAPTIONS] = "CAPTIONS";
    mKeyNames[(int)KeyCode::SETTINGS] = "SETTINGS";
    mKeyNames[(int)KeyCode::TV_POWER] = "TV_POWER";
    mKeyNames[(int)KeyCode::TV_INPUT] = "TV_INPUT";
    mKeyNames[(int)KeyCode::STB_POWER] = "STB_POWER";
    mKeyNames[(int)KeyCode::STB_INPUT] = "STB_INPUT";
    mKeyNames[(int)KeyCode::AVR_POWER] = "AVR_POWER";
    mKeyNames[(int)KeyCode::AVR_INPUT] = "AVR_INPUT";
    mKeyNames[(int)KeyCode::PROG_RED] = "PROG_RED";
    mKeyNames[(int)KeyCode::PROG_GREEN] = "PROG_GREEN";
    mKeyNames[(int)KeyCode::PROG_YELLOW] = "PROG_YELLOW";
    mKeyNames[(int)KeyCode::PROG_BLUE] = "PROG_BLUE";
    mKeyNames[(int)KeyCode::APP_SWITCH] = "APP_SWITCH";
    mKeyNames[(int)KeyCode::BUTTON_1] = "BUTTON_1";
    mKeyNames[(int)KeyCode::BUTTON_2] = "BUTTON_2";
    mKeyNames[(int)KeyCode::BUTTON_3] = "BUTTON_3";
    mKeyNames[(int)KeyCode::BUTTON_4] = "BUTTON_4";
    mKeyNames[(int)KeyCode::BUTTON_5] = "BUTTON_5";
    mKeyNames[(int)KeyCode::BUTTON_6] = "BUTTON_6";
    mKeyNames[(int)KeyCode::BUTTON_7] = "BUTTON_7";
    mKeyNames[(int)KeyCode::BUTTON_8] = "BUTTON_8";
    mKeyNames[(int)KeyCode::BUTTON_9] = "BUTTON_9";
    mKeyNames[(int)KeyCode::BUTTON_10] = "BUTTON_10";
    mKeyNames[(int)KeyCode::BUTTON_11] = "BUTTON_11";
    mKeyNames[(int)KeyCode::BUTTON_12] = "BUTTON_12";
    mKeyNames[(int)KeyCode::BUTTON_13] = "BUTTON_13";
    mKeyNames[(int)KeyCode::BUTTON_14] = "BUTTON_14";
    mKeyNames[(int)KeyCode::BUTTON_15] = "BUTTON_15";
    mKeyNames[(int)KeyCode::BUTTON_16] = "BUTTON_16";

}


void Keyboard::_setKeyState(KeyCode _keyCode, bool _keyDown)
{

    assert((int)_keyCode>=0 && (int)_keyCode<mKeys.size());

    BoolSignal &k = mKeys[(int)_keyCode];
    k._Set(_keyDown);

    //---
    mPressedKey = _keyCode;
    mPressedKeyButton._Set(_keyDown);

}


bool Keyboard::isKeyDown(KeyCode _keyCode)
{
    return mKeys[(int)_keyCode].active();
}


bool Keyboard::isKeyPressed(KeyCode _keyCode)
{
    return mKeys[(int)_keyCode].activeStarted();
}


bool Keyboard::isKeyReleased(KeyCode _keyCode)
{
    return mKeys[(int)_keyCode].activeEnded();
}



KeyCode Keyboard::pressedKey()
{
    if(mPressedKey != KeyCode::UNKNOWN){
        if(mPressedKeyButton.activeStarted()){
            return mPressedKey;
        }
    }

    return KeyCode::UNKNOWN;

}


KeyCode Keyboard::downKey()
{

    if(mPressedKey != KeyCode::UNKNOWN){
        if(mPressedKeyButton.active()){
            return mPressedKey;
        }
    }

    return KeyCode::UNKNOWN;

}


KeyCode Keyboard::releasedKey()
{

    if(mPressedKey != KeyCode::UNKNOWN){
        if(mPressedKeyButton.activeEnded()){
            return mPressedKey;
        }
    }

    return KeyCode::UNKNOWN;

}



void Keyboard::reset()
{
    for(BoolSignal &k : mKeys){
        k.reset();
    }
    //charPressed = 0;
    mPressedChar = "";
    mPressedKeyButton.reset();
}


void Keyboard::resetPerUpdate()
{

    for(BoolSignal &k : mKeys){
        k.postUpdate();
    }
    //charPressed = 0;
    mPressedChar = "";
    mPressedKeyButton.postUpdate();
}



KeyCode Keyboard::keyCodeForKeyName(const std::string &_name)
{

    KeyCode keyCode = KeyCode::UNKNOWN;

    int index=-1;
    for(int i=0; i<mKeyNames.size(); i++){
        if(mKeyNames.at(i) == _name){
            index = i;
            break;
        }
    }

    if(index != -1){
        keyCode = static_cast<KeyCode>(index);
    }

    return keyCode;
}


const std::string & Keyboard::keyNameFromKeyCode(KeyCode _keyCode)
{

    int index = static_cast<int>(_keyCode);

    if(index>=0 && index<mKeyNames.size()){
        return  mKeyNames[index];
    }

    static std::string unknownKey = "unknown key";
    return unknownKey;

}


	
Keyboard keyboard;


//======================================================================================


void Mouse::deleteContent()
{
    mCursorSprite = nullptr;
}


void Mouse::reset()
{

    for(BoolSignal &b : mButtons){
        b.reset();
    }

    mPosChanged = false;
    mWheelXChanged = false;
    mWheelYChanged = false;
}


void Mouse::resetPerUpdate()
{

    //--- clear 'per update' parameters
    for(BoolSignal &b : mButtons){
        b.postUpdate();
    }

    mPosChanged = false;
    mWheelXChanged = false;
    mWheelYChanged = false;

}


void Mouse::_setButtonState(MouseButton _mouseButton, bool _down)
{

    assert((int)_mouseButton>=0 && (int)_mouseButton<mButtons.size());

    BoolSignal &b = mButtons[(int)_mouseButton];

    b._Set(_down);

}

void Mouse::_setWheel(Vec2i _wheel)
{

    mWheelXChanged = ! (_wheel.x != mWheel.x);
    mWheelYChanged = ! (_wheel.y != mWheel.y);
    mWheel = _wheel;
}


void Mouse::setCursorSprite(Sprite *_cursorSprite)
{
    mCursorSprite = _cursorSprite;
    mCursorSprite->setPosition(Vec2f(mPos.x, mPos.y));
    mCursorSprite->setVisible(true);
}


void Mouse::clearCursorSprite()
{
    if(mCursorSprite==nullptr) return;
    mCursorSprite->setVisible(false);
    mCursorSprite = nullptr;
}


//void Mouse::SetSystemMouseCursorVisibility(bool _visible)
//{
//    commands->SetSystemMouseCursorVisible(_visible);
//}


Mouse mouse;


//======================================================================================


void Touch::reset()
{
    for(Finger &f : mFingers){
        f.reset();
    }
}


void Touch::resetPerUpdate()
{
    for(Finger &f : mFingers){
        f.postUpdate();
    }

}


void Touch::_setFingerState(int _finger, bool _down, Vec2i _position)
{

    if(_finger<0 || _finger>=mFingers.size()) return;

    Finger &f = mFingers[_finger];
    f._Set(_down);
    f.position = _position;

}


Touch touch;


//======================================================================================


std::vector<std::string> Joystick::mButtonNames;



void Joystick::createButtonNameStrings()
{
    if(mButtonNames.empty()){

        mButtonNames.resize(20);
        mButtonNames[static_cast<short int>(ncine::ButtonName::A)] = "A";
        mButtonNames[static_cast<short int>(ncine::ButtonName::B)] = "B";
        mButtonNames[static_cast<short int>(ncine::ButtonName::X)] = "X";
        mButtonNames[static_cast<short int>(ncine::ButtonName::Y)] = "Y";
        mButtonNames[static_cast<short int>(ncine::ButtonName::BACK)] = "BACK";
        mButtonNames[static_cast<short int>(ncine::ButtonName::GUIDE)] = "GUIDE";
        mButtonNames[static_cast<short int>(ncine::ButtonName::START)] = "START";
        mButtonNames[static_cast<short int>(ncine::ButtonName::LSTICK)] = "LSTICK";
        mButtonNames[static_cast<short int>(ncine::ButtonName::RSTICK)] = "RSTICK";
        mButtonNames[static_cast<short int>(ncine::ButtonName::LBUMPER)] = "LBUMPER";
        mButtonNames[static_cast<short int>(ncine::ButtonName::RBUMPER)] = "RBUMPER";
        mButtonNames[static_cast<short int>(ncine::ButtonName::DPAD_UP)] = "DPAD_UP";
        mButtonNames[static_cast<short int>(ncine::ButtonName::DPAD_DOWN)] = "DPAD_DOWN";
        mButtonNames[static_cast<short int>(ncine::ButtonName::DPAD_LEFT)] = "DPAD_LEFT";
        mButtonNames[static_cast<short int>(ncine::ButtonName::DPAD_RIGHT)] = "DPAD_RIGHT";
        mButtonNames[static_cast<short int>(ncine::ButtonName::MISC1)] = "MISC1";
        mButtonNames[static_cast<short int>(ncine::ButtonName::PADDLE1)] = "PADDLE1";
        mButtonNames[static_cast<short int>(ncine::ButtonName::PADDLE2)] = "PADDLE2";
        mButtonNames[static_cast<short int>(ncine::ButtonName::PADDLE3)] = "PADDLE3";
        mButtonNames[static_cast<short int>(ncine::ButtonName::PADDLE4)] = "PADDLE4";
    }
}




int Joystick::pressedButton()
{
    if(mIndexPressedButton != -1 && mPressedButton.activeStarted()){
        return  mIndexPressedButton;
    }

    return -1;
}


int Joystick::downButton()
{
    if(mIndexPressedButton != -1 && mPressedButton.active()){
        return  mIndexPressedButton;
    }

    return -1;

}


int Joystick::releasedButton()
{
    if(mIndexPressedButton != -1 && mPressedButton.activeEnded()){
        return  mIndexPressedButton;
    }

    return -1;
}


void Joystick::reset()
{
    for(BoolSignal &b : mButtons){
        b.reset();
    }
    for(BoolSignal &b : mPovXButtons){
        b.postUpdate();
    }
    for(BoolSignal &b : mPovYButtons){
        b.postUpdate();
    }

    //mJoystickAction.Reset();
    mPressedButton.reset();
}



void Joystick::resetPerUpdate()
{
    for(BoolSignal &b : mButtons){
        b.postUpdate();
    }
    for(BoolSignal &b : mPovXButtons){
        b.postUpdate();
    }
    for(BoolSignal &b : mPovYButtons){
        b.postUpdate();
    }

    //mJoystickAction.ResetPerUpdate();
    mPressedButton.postUpdate();

}



void Joystick::_setPOV_X(JoystickPOV_X _povX)
{

    BoolSignal &b = mPovXButtons[(int)_povX];

    b._Set(true);

    for(int i=0; i< mPovXButtons.size(); i++){
        if(i != (int)_povX){
           mPovXButtons[i]._Set(false);
        }
    }

    //---
    //mJoystickAction.mPovX = _povX;

}


void Joystick::_setPOV_Y(JoystickPOV_Y _povY)
{

    BoolSignal &b = mPovYButtons[(int)_povY];

    b._Set(true);

    for(int i=0; i< mPovYButtons.size(); i++){
        if(i != (int)_povY){
            mPovYButtons[i]._Set(false);
        }
    }

    //---
    //mJoystickAction.mPovY = _povY;
}


void Joystick::_setXaxis(float _xAxis)
{

    mXAxis = _xAxis;

    //---
    //mJoystickAction.mAxis = JoystickAxis::X;
}


void Joystick::_setYaxis(float _yAxis)
{
    mYAxis = _yAxis;

    //---
    //mJoystickAction.mAxis = JoystickAxis::Y;
}


void Joystick::_setZaxis(float _zAxis)
{
    mZAxis = _zAxis;

    //---
    //mJoystickAction.mAxis = JoystickAxis::Z;
}



void Joystick::_setButtonState(int _buttonIndex, bool _down)
{

    //assert(_buttonIndex>=0 && _buttonIndex<buttons.size());
    if(_buttonIndex<0 || _buttonIndex>=mButtons.size()) return;


    BoolSignal &b = mButtons[_buttonIndex];
    b._Set(_down);

    //---
    //mJoystickAction.mButtonName = static_cast<ncine::ButtonName>(_buttonIndex);

}



void Joystick::_setButtonStateViaMappedButton(ncine::ButtonName _buttonName, bool _down)
{

    //assert(_buttonIndex>=0 && _buttonIndex<buttons.size());

    int index = static_cast<short int>(_buttonName);

    if(index<0 || index>=mButtons.size()) return;


    BoolSignal &b = mButtons[index];
    b._Set(_down);

    //---
    //mJoystickAction.mButtonName = _buttonName;
    mIndexPressedButton = index;
    mPressedButton._Set(_down);
}


std::array<Joystick,2>joysticks;


//------------------------------------------------------------------------------


std::string JoystickControl::getName()
{

    if(mButtonName != ncine::ButtonName::UNKNOWN){

        return GetJoystickButtonNameString(mButtonName);

    }else if(mPovX != JoystickPOV_X::NONE){

        return GetJoystickPovXString(mPovX);

    }else if(mPovY != JoystickPOV_Y::NONE){

        return GetJoystickPovYString(mPovY);

    }else if(mAxis != JoystickAxis::NONE){

        return GetJoystickAxisString(mAxis);
    }
    return "error";

}


ncine::ButtonName GetJoystickButtonNameFromString(const std::string &_name)
{

    ncine::ButtonName bn = ncine::ButtonName::UNKNOWN;

    int index = -1;
    for(int i=0; i<Joystick::mButtonNames.size(); i++){
        if(Joystick::mButtonNames[i] == _name){
            index = i;
            break;
        }
    }

    if(index != -1){
        bn = static_cast<ncine::ButtonName>(index);
    }

    return bn;
}


std::string GetJoystickButtonNameString(ncine::ButtonName _name)
{

    int index = static_cast<short int>(_name);
    if(index>=0 && index<Joystick::mButtonNames.size()){

        return Joystick::mButtonNames[index];
    }

    return "unknown";

}


JoystickPOV_X GetJoystickPovXfromString(const std::string &_name)
{

    if(_name=="DPAD_LEFT"){
        return JoystickPOV_X::LEFT;

    }else if(_name=="DPAD_RIGHT"){
        return JoystickPOV_X::RIGHT;
    }

    return JoystickPOV_X::NONE;
}


std::string GetJoystickPovXString(JoystickPOV_X _povX)
{

    if(_povX==JoystickPOV_X::LEFT){
        return "DPAD_LEFT";

    }else if(_povX==JoystickPOV_X::RIGHT){
        return "DPAD_RIGHT";

    }

    return "none";

}


JoystickPOV_Y GetJoystickPovYfromString(const std::string &_name)
{

    if(_name=="DPAD_UP"){
        return JoystickPOV_Y::UP;

    }else if(_name=="DPAD_DOWN"){
        return JoystickPOV_Y::DOWN;
    }

    return JoystickPOV_Y::NONE;
}


std::string GetJoystickPovYString(JoystickPOV_Y _povY)
{

    if(_povY==JoystickPOV_Y::UP){
        return "DPAD_UP";

    }else if(_povY==JoystickPOV_Y::DOWN){
        return "DPAD_DOWN";
    }

    return "none";
}


JoystickAxis GetJoystickAxisFromString(const std::string &_name)
{

    if(_name=="axisX"){
        return JoystickAxis::X;

    }else if(_name=="axisY"){
        return JoystickAxis::Y;

    }else if(_name=="axisZ"){
        return JoystickAxis::Z;
    }

    return JoystickAxis::NONE;
}


std::string GetJoystickAxisString(JoystickAxis _axis)
{

    if(_axis==JoystickAxis::X){
        return "axisX";

    }else if(_axis==JoystickAxis::Y){
        return "axisY";

    }else if(_axis==JoystickAxis::Z){
        return "axisZ";
    }

    return "none";
}


bool GetJoystickCommandFromString(const std::string &_name, JoystickControl &p)
{

    p.mPovX = GetJoystickPovXfromString(_name);
    if(p.mPovX != JoystickPOV_X::NONE) return true;

    p.mPovY = GetJoystickPovYfromString(_name);
    if(p.mPovY != JoystickPOV_Y::NONE) return true;

    p.mAxis = GetJoystickAxisFromString(_name);
    if(p.mAxis != JoystickAxis::NONE) return true;

    p.mButtonName = GetJoystickButtonNameFromString(_name);
    if(p.mButtonName == ncine::ButtonName::UNKNOWN){
        dbgSystem.addMessage("Wrong joystick input command '" + _name +"'!");
        return false;
    }

    return true;
}



//-----------------------------------------------------------------------------------------------


void DeleteInputSupportingObjects()
{

    mouse.deleteContent();

}



}






