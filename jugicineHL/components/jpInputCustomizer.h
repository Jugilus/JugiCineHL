#ifndef PLAYER__COMPONENTS__INPUT_CUSTOMIZER__H
#define PLAYER__COMPONENTS__INPUT_CUSTOMIZER__H


#include <memory>
#include "jpComponentsCommon.h"
#include "jpItemsCommon.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class GuiTable;
class GuiButton;
class ItemsGroup;
class InputDeviceProfile;
struct KeyboardInputCommand;
struct JoystickInputCommand;
class InputDeviceItem;



class InputCustomizer : public Component
{
public:


    InputCustomizer(const pugi::xml_node &_node);

    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void update(UpdateMode _updateMode) override;
    void onStateEnded() override;

    // rebuild items which represents input devices and input commands
    void rebuildItems();


private:

    PlayedScene *mParentPlayerScene = nullptr;              // LINK
    GuiTable* mDevicesTable = nullptr;                      // LINK
    GuiTable* mButtonsTable = nullptr;                      // LINK
    std::vector<GuiWidget*>mUsedWidgets;                    // LINKS
    GuiButton *mBRestoreDefaultButtons;                     // LINK
    std::unique_ptr<ItemsGroup>mInputDeviceItems;

    std::vector<std::string>mStoredDeviceNames;             // store device names, so that the next time the customizer is displayed we can check if there were any changes ( joystick connected or disconnected )
    std::string mStoredUserName;

    bool mChanged = false;


    //--- cfg
    struct Cfg
    {
        std::string mDevicesTable;
        std::string mButtonsTable;
        std::string mBRestoreDefaultButtons;
    };
    std::unique_ptr<Cfg>mCfg;


    // check if the inout devices changed since the last use of InputCustomizer
    bool hasInputDevicesChanged();

    // find KeyboardInputCommand in active KeyboardProfile
    KeyboardInputCommand* keyboardInputCommandForKey(KeyCode _keyCode);

};


//------------------------------------------------------------------------------

class InputDeviceItem : public GameItem
{
public:


    InputDeviceItem(const std::string &_name, ItemsGroup *_parentItemsGroup, InputDeviceProfile* _deviceProfile);

    ~InputDeviceItem() override;

    InputDeviceProfile *deviceProfile(){ return mDeviceProfile; }

    ItemsGroup* buttonsItems(){ return mButtonsItems;}

    void updateDuplicateKeyboardKeyMarkers(KeyCode _keyCode);

    void updateDuplicateJoystickButtonMarkers(JoystickControl _joystickCommand);

private:
    InputDeviceProfile *mDeviceProfile = nullptr;       // LINK
    ItemsGroup* mButtonsItems = nullptr;                // OWNED

};


//------------------------------------------------------------------------------


class KeyboardInputCommandItem : public GameItem
{
public:


    KeyboardInputCommandItem(const std::string &_name, ItemsGroup *_itemsGroup, KeyboardInputCommand *_command);


    KeyboardInputCommand *keyboardInputCommand(){ return mKeyboardInputCommand; }

    void setKeyCode(KeyCode _keyCode);


private:
    KeyboardInputCommand* mKeyboardInputCommand = nullptr;                // LINK

};


//------------------------------------------------------------------------------


class JoystickInputCommandItem : public GameItem
{
public:


    JoystickInputCommandItem(const std::string &_name, ItemsGroup *_itemsGroup, JoystickInputCommand *_command);


    JoystickInputCommand *joystickInputCommand(){ return mJoystickInputCommand; }

    void setJoystickCommand(JoystickControl _joystickControl);


private:
    JoystickInputCommand* mJoystickInputCommand = nullptr;                // LINK

};




}






#endif
