#ifndef PLAYER__SYSTEM__INPUT_H
#define PLAYER__SYSTEM__INPUT_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <ncine/InputEvents.h>


#include "jmSignal.h"
#include "jmInput.h"
#include "jpSettings.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class InputCustomizer;
class UserProfile;
class PlayedScene;
class KeyboardProfile;
class JoystickProfile;
class BinaryStreamWriter;
class StdBinaryFileStreamReader;
class InputSystem;


//class GameInputCommand : public BaseObject
class GameInputCommand : public BoolSignal
{
public:
    friend class KeyboardProfile;
    friend class JoystickProfile;


    /*
    enum class Trigger
    {
        PRESSED,
        HELD_DOWN,
        RELEASED,
        NOT_DEFINED
    };
    */

    GameInputCommand(const pugi::xml_node &_node);

    bool init(InputSystem *_inputSystem);

    //---
    const std::string &name(){ return mName; }

    //bool startedBeingActive(){ return mStartedBeingActive; }
    //bool active(){ return mActive; }
    //bool endedBeingActive(){ return mEndedBeingActive; }


    //---
    void reset();
    void setSignalValues(bool _active, bool _activeStarted, bool _activeEnded);
    void manageBlockingCommands();

    float xAxis(){ return mXAxis; }
    float yAxis(){ return mYAxis; }
    float zAxis(){ return mZAxis; }


    KeyCode defaultKeyboardKey(){ return mDefKeyboardKey; }
    JoystickControl defaultJoystickCommand(){ return mDefJoystickCommand; }

    const std::string &description(){ return mDescription; }




private:
    std::string mName;


    //Trigger mTrigger = Trigger::HELD_DOWN;
    //bool mStartedBeingActive = false;
    //bool mActive = false;
    //bool mEndedBeingActive = false;
    float mXAxis =0.0f;
    float mYAxis =0.0f;
    float mZAxis =0.0f;


    //default input
    std::string mDescription;
    KeyCode mDefKeyboardKey = KeyCode::UNKNOWN;
    JoystickControl mDefJoystickCommand;

    std::vector<GameInputCommand*>mBlockingCommands;


    struct Cfg
    {
        std::string mDescription;
        std::string mDefKeyboardKey;
        std::string mDefJoystickCommand;
        std::string mTrigger;
        std::vector<std::string>mBlockingCommands;
    };

    //---
    std::unique_ptr<Cfg>mCfg;


    //Trigger getTriggerFromString(const std::string &_trigger);

};

//------------------------------------------------------------------------------------


class InputDeviceProfile : public BaseObject
{
public:


    InputDeviceProfile(const std::string &_name) : mName(_name){}

    const std::string &name(){ return mName; }

    bool isActive(){ return mActive;}
    void _setActive(bool _active){ mActive = _active; }


    virtual void update() = 0;


protected:
    std::string mName;
    bool mActive = false;

};


//-----------------------------------------------------------------------------------------


struct KeyboardInputCommand
{
    GameInputCommand* mGameInputCommand = nullptr;      // LINK
    KeyCode mKeyCode;
};



class KeyboardProfile : public InputDeviceProfile
{
public:
    static std::string defName;

    KeyboardProfile();

    void update() override;

    void getDirectionSignals(bool &right, bool &left, bool &up, bool &down);

    std::vector<KeyboardInputCommand> & keyboardInputCommands(){ return mKeyboardInputCommands; }

    KeyboardInputCommand* getCommandWithKeyCode(KeyCode _keyCode);


private:
    std::vector<KeyboardInputCommand>mKeyboardInputCommands;

};


//-----------------------------------------------------------------------------------------


struct JoystickInputCommand
{
    GameInputCommand* mGameInputCommand = nullptr;      // LINK
    JoystickControl mJoystickControl;
};



class JoystickProfile : public InputDeviceProfile
{
public:
    static std::string defName;

    JoystickProfile(Joystick * _joystick);

    void update() override;

    void getDirectionSignals(bool &right, bool &left, bool &up, bool &down);

    std::vector<JoystickInputCommand> & joystickInputCommands(){ return mJoystickInputCommands; }
    Joystick *joystick(){ return mJoystick; }
    void _setJoystick(Joystick *_joystick){ mJoystick = _joystick; }

    JoystickInputCommand *getCommandWithButtonName(ncine::ButtonName _buttonName);



private:
    std::vector<JoystickInputCommand>mJoystickInputCommands;
    Joystick * mJoystick = nullptr;                             // LINK

};


//struct JoystickProfiles
//{
//    std::vector<JoystickProfile*>mProfiles;
//    Joystick * mJoystick = nullptr;                             // LINK
//};


//-----------------------------------------------------------------------------------------


struct InputProfiles
{

    ~InputProfiles();
    std::vector<KeyboardProfile*>mKeyboardProfiles;         // OWNED
    std::vector<JoystickProfile*>mJoystickProfiles;         // OWNED

    void save(BinaryStreamWriter &stream);
    void load(StdBinaryFileStreamReader &stream);

};


class InputSystem : public BaseObject
{
public:

    InputSystem(PlayedApp *_playerApp);
    ~InputSystem();


    bool initCfg(const pugi::xml_node &_node);

    bool initConnections();

    void preUpdate();

    //--- device profiles
    //bool addKeyboardProfile(const std::string &_name);
    //bool addGamepadProfile(const std::string &_name);

    //InputDeviceProfile* getInputDeviceProfile(const std::string &_name, bool _setErrorMessage = true);
    //std::vector<InputDeviceProfile*> &inputDeviceProfiles(){ return mInputDeviceProfiles; }

    //void addJoystick(int joystickId);
    //void removeJoystick(int joystickId);


    //std::vector<KeyboardProfile*> &keyboardProfiles(){ return mKeyboardProfiles; }
    //std::vector<JoystickProfile*> &joystickProfiles(){ return mJoystickProfiles; }





    InputProfiles *activeUserProfiles(){ return mActiveInputProfiles; }
    void setActiveUserProfiles(InputProfiles *_inputProfiles);

    //--- input commands
    //bool initInputCommands();

    bool addGameInputCommand(const pugi::xml_node &_node);
    GameInputCommand* getGameInputCommand(const std::string &_name, bool _setErrorMessage = true);
    std::vector<GameInputCommand*> &gameInputCommands(){ return mGameInputCommands; }


    InputProfiles * newDefaultInputProfiles();

    BoolSignal & rightDirection(){ return mSigRightDirection; }
    BoolSignal & leftDirection(){ return mSigLeftDirection; }
    BoolSignal & upDirection(){ return mSigUpDirection; }
    BoolSignal & downDirection(){ return mSigDownDirection; }

    //---
    //bool saveProfilesIniFile();
    //void loadProfilesIniFile();

    void _setInputCustomizer(InputCustomizer* _inputCustomizer){ mInputCustomizer = _inputCustomizer; }


private:
    PlayedApp* mParentPlayerApp = nullptr;
    //int mNumberOfKeyboardProfiles = 1;
    //int mNumberOfProfilesPerJoystick = 1;

    //std::string mDefKeyboardName = "keyboard";
    //std::string mDefJoystickName = "joystick";

    std::vector<GameInputCommand*>mGameInputCommands;       // OWNED


    //std::vector<KeyboardProfile*>mKeyboardProfiles;         // OWNED
    //std::vector<JoystickProfile*>mJoystickProfiles;         // OWNED

    InputProfiles *mActiveInputProfiles = nullptr;          // LINK to active users profiles

    InputCustomizer* mInputCustomizer = nullptr;            // LINK

    BoolSignal mSigRightDirection;
    BoolSignal mSigLeftDirection;
    BoolSignal mSigUpDirection;
    BoolSignal mSigDownDirection;


    //--- Cfg
    struct Cfg
    {
        std::string mDefKeyboardName;
        std::string mDefJoystickName;
    };

    std::unique_ptr<Cfg>mCfg;


    void dbgPrint();


};




}


#endif // PLAYER_SYSTEM_H
