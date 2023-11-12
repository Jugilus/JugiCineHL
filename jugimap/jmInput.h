#ifndef JUGIMAP_INPUT_H
#define JUGIMAP_INPUT_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>
#include <string>
#include <ncine/Keys.h>
#include <ncine/InputEvents.h>
#include "jmSignal.h"
#include "jmCommon.h"


namespace jugimap{
	
class Sprite;


/// \addtogroup Input
/// @{







/*
///\brief The key codes of keyboard keys
enum class KeyCode : int
{

    UNKNOWN = 0,                ///< Unknown

    BACKSPACE=8,                ///< Backspace
    TAB=9,                      ///< Tab
    ENTER=13,                   ///< Enter
    PAUSE=19,                   ///< Pause
    ESCAPE=27,                  ///< Escape
    SPACE=32,                   ///< Space

    PAGEUP=33,                  ///< Page Up
    PAGEDOWN=34,                ///< Page Down
    END=35,                     ///< End
    HOME=36,                    ///< Home

    LEFT=37,                    ///< Left
    UP=38,                      ///< Up
    RIGHT=39,                   ///< Right
    DOWN=40,                    ///< Down

    PRINT=42,                   ///< Print
    INSERT=45,                  ///< Insert
    DELETEkey=46,               ///< Delete

    NUM_0=48,                   ///< 0
    NUM_1=49,                   ///< 1
    NUM_2=50,                   ///< 2
    NUM_3=51,                   ///< 3
    NUM_4=52,                   ///< 4
    NUM_5=53,                   ///< 5
    NUM_6=54,                   ///< 6
    NUM_7=55,                   ///< 7
    NUM_8=56,                   ///< 8
    NUM_9=57,                   ///< 9

    A=65,                       ///< A
    B=66,                       ///< B
    C=67,                       ///< C
    D=68,                       ///< D
    E=69,                       ///< E
    F=70,                       ///< F
    G=71,                       ///< G
    H=72,                       ///< G
    I=73,                       ///< I
    J=74,                       ///< J
    K=75,                       ///< K
    L=76,                       ///< L
    M=77,                       ///< M
    N=78,                       ///< N
    O=79,                       ///< O
    P=80,                       ///< P
    Q=81,                       ///< Q
    R=82,                       ///< R
    S=83,                       ///< S
    T=84,                       ///< T
    U=85,                       ///< U
    V=86,                       ///< V
    W=87,                       ///< W
    X=88,                       ///< X
    Y=89,                       ///< Y
    Z=90,                       ///< Z



    F1=112,                     ///< F1
    F2=113,                     ///< F2
    F3=114,                     ///< F3
    F4=115,                     ///< F4
    F5=116,                     ///< F5
    F6=117,                     ///< F6
    F7=118,                     ///< F7
    F8=119,                     ///< F8
    F9=120,                     ///< F9
    F10=121,                    ///< F10
    F11=122,                    ///< F11
    F12=123,                    ///< F12


    // 165 - 190 - reserved for upper case letters using printable characters table
    TILDE=192,                  ///< Tilde
    HYPHEN=193,                 ///< Hyphen
    EQUAL=194,                  ///< Equal
    SEMICOLON=195,              ///< Semicolon
    QUOTE=196,                  ///< Quote
    COMMA=197,                  ///< Comma
    PERIOD=198,                 ///< Period
    SLASH=199,                  ///< Slash
    LEFT_BRACKET=200,           ///< Left Bracket
    RIGHT_BRACKET=201,          ///< Right Bracket
    BACKSLASH=202,              ///< Backslash

    LEFT_SHIFT=210,             ///< Left Shift
    RIGHT_SHIFT=211,            ///< Right Shift
    LEFT_CONTROL=212,           ///< Left Control
    RIGHT_CONTROL=213,          ///< Right Control
    LEFT_ALT=214,               ///< Left Alt
    RIGHT_ALT=215,              ///< Right Alt

};

*/


using KeyCode = ncine::KeySym;


///\brief The Keyboard class provide information about keyboard input.
class Keyboard
{
public:

    Keyboard();

    /// Returns true if a key with the given *_keyCode* is pressed; otherwise returns false.
    bool isKeyPressed(KeyCode _keyCode);

    /// Returns true if a key with the given *_keyCode* is hold down; otherwise returns false.
    bool isKeyDown(KeyCode _keyCode);

    /// Returns true if a key with the given *_keyCode* is released; otherwise returns false.
    bool isKeyReleased(KeyCode _keyCode);

    /// Returns the ascii code of a printable character if pressed; otherwise returns 0;
    const std::string & getPressedChar(){ return mPressedChar; }

    KeyCode pressedKey();

    KeyCode downKey();

    KeyCode releasedKey();

    /// Reset all key states.
    void reset();

    KeyCode keyCodeForKeyName(const std::string &_name);

    const std::string &keyNameFromKeyCode(KeyCode _keyCode);

    //-------------------------------------------------------
    std::vector<BoolSignal> & keys(){ return mKeys; }
    std::vector<std::string> & keyNames(){ return mKeyNames; }
    BoolSignal * keySignalForKeyName(const std::string &_name, bool _setErrorMessage=true);


    void resetPerUpdate();

    void _setKeyState(KeyCode _keyCode, bool _keyDown);
    void _setTextInputChar(const std::string &_letter){ mPressedChar = _letter ;}



private:
    std::vector<BoolSignal>mKeys;
    std::vector<std::string>mKeyNames;
    KeyCode mPressedKey = KeyCode::UNKNOWN;
    BoolSignal mPressedKeyButton;
    std::string mPressedChar;

};


/// Global Keyboard object used for querying keyboard input.
extern Keyboard keyboard;



//-------------------------------------------------------------------------------------


///\brief The mouse buttons.
enum class MouseButton
{
    NONE = 0,           ///< None
    LEFT = 1,           ///< Left button.
    MIDDLE = 2,         ///< Middle button.
    RIGHT = 3,          ///< Right button.
};


///\brief The Mouse class provide information about mouse input.
class Mouse
{
public:

    void deleteContent();

    /// Returns the x coordinate of the mouse cursor on screen.
    int posX(){ return mPos.x; }

    /// Returns the y coordinate of the mouse cursor on screen.
    int posY(){ return mPos.y; }

    /// Returns the position of the mouse cursor on screen.
    Vec2i pos(){ return mPos; }

    /// Returns true if the position of the cursor changed; otherwise returns false.
    bool isPositionChanged(){ return mPosChanged; }


    /// Returns the value of horizontal wheel.
    int wheelX(){ return mWheel.x; }

    /// Returns the value of vertical wheel.
    int wheelY(){ return mWheel.y; }

    /// Returns the values of vertical and horizonatal wheel.
    Vec2i wheel(){ return mWheel; }

    /// Returns true if the value of horizontal wheel changed; otherwise returns false.
    bool isWheelXChanged(){ return mWheelXChanged; }

    /// Returns true if the value of horizontal wheel changed; otherwise returns false.
    bool isWheelYChanged(){ return mWheelYChanged; }


    /// Returns true if the given *_mouse button* is pressed; otherwise returns false.
    bool isButtonPressed(MouseButton _mouseButton){ return mButtons[(int)_mouseButton].activeStarted(true); }

    /// Returns true if the given *_mouse button* is hold down; otherwise returns false.
    bool isButtonDown(MouseButton _mouseButton){ return mButtons[(int)_mouseButton].active(true); }

    /// Returns true if the given *_mouse button* is released; otherwise returns false.
    bool isButtonReleased(MouseButton _mouseButton){ return mButtons[(int)_mouseButton].activeEnded(true); }

    /// \brief Set the cursor sprite to the given *_cursorSprite*.
    ///
    /// This function does not hide system cursor. That can be done with EngineApp::SetSystemMouseCursorVisible.
    void setCursorSprite(jugimap::Sprite *_cursorSprite);

    /// \brief Clear the cursor sprite.
    ///
    /// This function will hide the cursor sprite and set its link pointer to nullptr.
    void clearCursorSprite();

    /// Returns the cursor sprite if exists; otherwise returns nullptr.
    Sprite* cursorSprite(){ return mCursorSprite; }

    /// Reset all buttons states.
    void reset();

    //----

    void resetPerUpdate();

    void _setPosition(Vec2i _pos) { mPosChanged = !(_pos == mPos);   mPos = _pos; }
    void _setWheel(Vec2i _wheel);
    void _setButtonState(MouseButton _mouseButton, bool _down);


private:

    Vec2i mPos;
    Vec2i mWheel;
    std::array<BoolSignal,4> mButtons;

    bool mWheelYChanged = false;
    bool mWheelXChanged = false;
    bool mPosChanged =false;

    //----
    Sprite *mCursorSprite = nullptr;             // LINK

};


/// Global Mouse object used for querying mouse input.
extern Mouse mouse;



//-------------------------------------------------------------------------------------



struct Finger : public BoolSignal
{
    int id = 0;
    Vec2i position;
};


///\brief The Touch class provide information about touch input.
class Touch
{
public:


    /// Returns true if the given *_finger* is pressed; otherwise returns false.
    bool isFingerPressed(int _finger){ return mFingers[_finger].activeStarted(true); }

    /// Returns true if the given *_finger* is hold down; otherwise returns false.
    bool isFingerDown(int _finger){ return mFingers[_finger].active(true); }

    /// Returns true if the given *_finger* is released; otherwise returns false.
    bool isFingerReleased(int _finger){ return mFingers[_finger].activeEnded(true); }

    /// Returns the x coordinate of the given *_finger* on screen.
    int fingerX(int _finger){ return mFingers[_finger].position.x; }

    /// Returns the y coordinate of the given *_finger* on screen.
    int fingerY(int _finger){ return mFingers[_finger].position.y; }

    /// Returns position of the given *_finger* on screen.
    Vec2i fingerPos(int _finger){ return mFingers[_finger].position; }

    /// Returns a reference to the vector of finger states.
    std::array<Finger, 10> & fingers(){ return mFingers;}

    /// Reset all finger states.
    void reset();

    //----
    void resetPerUpdate();

    //----
    void _setFingerState(int _finger, bool _down, Vec2i _position);

private:

    std::array<Finger, 10>mFingers;

};

/// Global Touch object used for querying touch input.
extern Touch touch;


//-------------------------------------------------------------------------------------


/// POV X values.
enum class JoystickPOV_X
{
    NONE,
    RIGHT,
    LEFT
};

/// POV Y values.
enum class JoystickPOV_Y
{
    NONE,
    UP,
    DOWN,
};


enum class JoystickAxis
{
    NONE,
    X,
    Y,
    Z
};


struct JoystickControl
{
    ncine::ButtonName mButtonName = ncine::ButtonName::UNKNOWN;
    JoystickPOV_X mPovX = JoystickPOV_X::NONE;
    JoystickPOV_Y mPovY = JoystickPOV_Y::NONE;
    JoystickAxis mAxis = JoystickAxis::NONE;


    bool operator==(const JoystickControl &_other)
    { return (mButtonName==_other.mButtonName && mPovX==_other.mPovX && mPovY==_other.mPovY && mAxis==_other.mAxis); }

    bool operator!=(const JoystickControl &_other){ return !operator==(_other); }

    std::string getName();
};



/// The Joystick class provide information about joystick input.
class Joystick
{
public:

    static std::vector<std::string>mButtonNames;
    static void createButtonNameStrings();


    /// Returns true if a button with the given *_buttonIndex* is pressed; otherwise returns false.
    bool isButtonPressed(int _buttonIndex){ return mButtons[_buttonIndex].activeStarted(true); }

    /// Returns true if a button with the given *_buttonIndex* is hold down; otherwise returns false.
    bool isButtonDown(int _buttonIndex){ return mButtons[_buttonIndex].active(true); }

    /// Returns true if a button with the given *_buttonIndex* is released; otherwise returns false.
    bool isButtonReleased(int _buttonIndex){ return mButtons[_buttonIndex].activeEnded(true); }

    /// Returns true if a button with the given *_buttonIndex* is pressed; otherwise returns false.
    bool isButtonPressed(ncine::ButtonName _buttonName){ return mButtons[static_cast<short int>(_buttonName)].activeStarted(true); }

    /// Returns true if a button with the given *_buttonIndex* is hold down; otherwise returns false.
    bool isButtonDown(ncine::ButtonName _buttonName){ return mButtons[static_cast<short int>(_buttonName)].active(true); }

    /// Returns true if a button with the given *_buttonIndex* is released; otherwise returns false.
    bool isButtonReleased(ncine::ButtonName _buttonName){ return mButtons[static_cast<short int>(_buttonName)].activeEnded(true); }

    /// Returns true if the given *_povX* button is pressed; otherwise returns false.
    bool isPOV_XPressed(JoystickPOV_X _povX){ return mPovXButtons[(int)_povX].activeStarted(true); }

    /// Returns true if the given *_povX* button is hold down; otherwise returns false.
    bool isPOV_XDown(JoystickPOV_X _povX){ return mPovXButtons[(int)_povX].active(true); }

    /// Returns true if the given *_povX* button is released; otherwise returns false.
    bool isPOV_XReleased(JoystickPOV_X _povX){ return mPovXButtons[(int)_povX].activeEnded(true); }

    /// Returns true if the given *_povY* button is pressed; otherwise returns false.
    bool isPOV_YPressed(JoystickPOV_Y _povY){ return mPovYButtons[(int)_povY].activeStarted(true); }

    /// Returns true if the given *_povY* button is hold down; otherwise returns false.
    bool isPOV_YDown(JoystickPOV_Y _povY){ return mPovYButtons[(int)_povY].active(true); }

    /// Returns true if the given *_povY* button is released; otherwise returns false.
    bool isPOV_YReleased(JoystickPOV_Y _povY){ return mPovYButtons[(int)_povY].activeEnded(true); }

    /// Returns the value of x axis.
    float xAxis(){ return mXAxis; }

    /// Returns the value of y axis.
    float yAxis(){ return mYAxis; }

    /// Returns the value of z axis.
    float zAxis(){ return mZAxis; }


    /// Returns true if this joystick is connected; otherwise returns false.
    bool isConnected(){ return mConnected; }

    /// Reset all button states.
    void reset();

    /// Returns a reference to the array of button states.
    std::array<BoolSignal,20>& buttons(){ return mButtons;}

    int pressedButton();

    int downButton();

    int releasedButton();


    /// \brief Return the joystick name.
    ///
    /// This function does not provide the real joystick name on all engines.
    const std::string &name(){ return mName; }


    void resetPerUpdate();

    //----
    void _setPOV_X(JoystickPOV_X _povX);
    void _setPOV_Y(JoystickPOV_Y _povY);
    void _setXaxis(float _xAxis);
    void _setYaxis(float _yAxis);
    void _setZaxis(float _zAxis);
    void _setButtonState(int _buttonIndex, bool _down);
    void _setButtonStateViaMappedButton(ncine::ButtonName _buttonName, bool _down);
    void _setConnected(bool _connected){ mConnected = _connected; }
    void _setName(const std::string &_name){ mName = _name; }


private:
    std::array<BoolSignal,20>mButtons;
    std::array<BoolSignal,3>mPovXButtons;
    std::array<BoolSignal,3>mPovYButtons;


    float mXAxis = 0.0;
    float mYAxis = 0.0;
    float mZAxis = 0.0;

    //JoystickAction mJoystickAction;
    int mIndexPressedButton = -1;               //
    BoolSignal mPressedButton;

    bool mConnected = false;
    std::string mName = "Generic joystick";

};


/// Global Joystick objects used for querying joysticks input.
extern std::array<Joystick,2>joysticks;


ncine::ButtonName GetJoystickButtonNameFromString(const std::string &_name);

std::string GetJoystickButtonNameString(ncine::ButtonName _name);

JoystickPOV_X GetJoystickPovXfromString(const std::string &_name);

std::string GetJoystickPovXString(JoystickPOV_X _povX);

JoystickPOV_Y GetJoystickPovYfromString(const std::string &_name);

std::string GetJoystickPovYString(JoystickPOV_Y _povY);

JoystickAxis GetJoystickAxisFromString(const std::string &_name);

std::string GetJoystickAxisString(JoystickAxis _axis);

bool GetJoystickCommandFromString(const std::string &_name, JoystickControl &p);




void DeleteInputSupportingObjects();

/// @}      //end addtogroup Input
///
	





}



#endif // JUGIMAP_INPUT_H
