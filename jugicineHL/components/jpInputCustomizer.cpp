#include <chrono>
#include <assert.h>
#include <sstream>
#include <ncine/IInputManager.h>
#include <ncine/Application.h>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiButton.h"
#include "items/jpItemsCommon.h"
#include "jpItemSlotData.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUser.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpInput.h"
#include "jpInputCustomizer.h"



namespace jugimap{



using namespace jugimap;




InputCustomizer::InputCustomizer(const pugi::xml_node &_node)
{

    mName = "inputCustomizer";

    mCfg.reset(new Cfg());
    //mPaths->mDevices = _node.attribute("devices").as_string("");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="useWidgets"){
            mCfg->mDevicesTable = n.attribute("inputDevicesTable").as_string("");
            mCfg->mButtonsTable = n.attribute("inputButtonsTable").as_string("");
            mCfg->mBRestoreDefaultButtons = n.attribute("restoreDefaultButton").as_string("");
        }
    }
}


bool InputCustomizer::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init cfg '" + mName + "'");

    mParentPlayerScene = _scene;

    //---
    mDevicesTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, mCfg->mDevicesTable, GuiWidgetKind::TABLE));
    if(mDevicesTable==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mDevicesTable);

    //---
    mButtonsTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, mCfg->mButtonsTable, GuiWidgetKind::TABLE));
    if(mButtonsTable==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mButtonsTable);

    //---
    mBRestoreDefaultButtons = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mCfg->mBRestoreDefaultButtons, GuiWidgetKind::BUTTON));
    if(mBRestoreDefaultButtons==nullptr){
        return false;
    }
    mUsedWidgets.push_back(mBRestoreDefaultButtons);


    //---
    mInputDeviceItems.reset(new ItemsGroup("inputDevicesNames", this));
    mDevicesTable->setItemsGroup(mInputDeviceItems.get());

    app->inputSystem()->_setInputCustomizer(this);


    //---
    dbgSystem.removeLastMessage();
    return true;
}


void InputCustomizer::start()
{

    print("InputCustomizer::start()");

    mParentPlayerScene->widgetManager()->appendToUsedWidgets(mUsedWidgets);

    if(hasInputDevicesChanged()){
        rebuildItems();
    }
    mButtonsTable->ScrollToTheStartOfItems();

    mChanged = false;

}


void InputCustomizer::update(UpdateMode _updateMode)
{

    if(_updateMode==UpdateMode::MODAL_OVERLAY){
        return;
    }


    if(mDevicesTable->IsValueChanged()){
        InputDeviceItem * i = dynamic_cast<InputDeviceItem*>(mDevicesTable->selectedItem());
        if(i){
            mButtonsTable->setItemsGroup(i->buttonsItems());
        }
        //print("Selected :"+ mDevicesTable->selectedItem()->name());
    }


    InputDeviceItem *selectedInputDeviceItem =  dynamic_cast<InputDeviceItem*>(mDevicesTable->selectedItem());

    if(selectedInputDeviceItem){
        InputDeviceProfile *deviceProfile = selectedInputDeviceItem->deviceProfile();

        //--- KEABOARD
        if(dynamic_cast<KeyboardProfile*>(deviceProfile)){

            GameItem *selectedItem = mButtonsTable->selectedItem();
            if(selectedItem){

                KeyboardInputCommandItem *ki = static_cast<KeyboardInputCommandItem*>(selectedItem);

                //---- change the key button keyCode if a key was pressed
                KeyCode key = keyboard.pressedKey();
                if(key != KeyCode::UNKNOWN){
                    ki->setKeyCode(key);
                    mButtonsTable->updateSlotContentForItem(ki);
                    mChanged = true;
                }
            }

            //---- set default key codes
            if(mBRestoreDefaultButtons->IsPressed()){
                for(GameItem *i : selectedInputDeviceItem->buttonsItems()->items()){
                    KeyboardInputCommandItem *ki = static_cast<KeyboardInputCommandItem*>(i);
                    if(ki->keyboardInputCommand()->mKeyCode != ki->keyboardInputCommand()->mGameInputCommand->defaultKeyboardKey()){
                        ki->setKeyCode(ki->keyboardInputCommand()->mGameInputCommand->defaultKeyboardKey());
                        mButtonsTable->updateSlotContentForItem(ki);
                    }
                }
                mChanged = true;
            }


        //--- JOYSTICK
        }else if(dynamic_cast<JoystickProfile*>(deviceProfile)){

            JoystickProfile *joystickProfile = static_cast<JoystickProfile*>(deviceProfile);

            GameItem *selectedItem = mButtonsTable->selectedItem();
            if(selectedItem){

                 JoystickInputCommandItem *ji = static_cast<JoystickInputCommandItem*>(selectedItem);

                 short int buttonIndex = joystickProfile->joystick()->pressedButton();

                 if(buttonIndex != -1){

                     ncine::ButtonName buttonName = static_cast<ncine::ButtonName>(buttonIndex);

                     if(buttonName != ncine::ButtonName::UNKNOWN){

                         print("Pressed button '" + GetJoystickButtonNameString(buttonName));

                         bool forbidden = (buttonName==ncine::ButtonName::DPAD_UP || buttonName==ncine::ButtonName::DPAD_DOWN ||
                                           buttonName==ncine::ButtonName::DPAD_LEFT || buttonName==ncine::ButtonName::DPAD_RIGHT);

                         if(forbidden==false){                  // key is not already used
                             JoystickControl a;
                             a.mButtonName = buttonName;
                             ji->setJoystickCommand(a);
                             mButtonsTable->updateSlotContentForItem(ji);
                             mChanged = true;
                         //    print("Button '" + GetNcineButtonNameString(buttonName) +" ' already used!");
                         }

                     }else{
                         print("Wrong buttonIndex '" + std::to_string(buttonIndex));
                     }
                 }
            }

             //----
             if(mBRestoreDefaultButtons->IsPressed()){
                 for(GameItem *i : selectedInputDeviceItem->buttonsItems()->items()){
                     JoystickInputCommandItem *ji = static_cast<JoystickInputCommandItem*>(i);
                     if(ji->joystickInputCommand()->mJoystickControl != ji->joystickInputCommand()->mGameInputCommand->defaultJoystickCommand()){
                         ji->setJoystickCommand(ji->joystickInputCommand()->mGameInputCommand->defaultJoystickCommand());
                         mButtonsTable->updateSlotContentForItem(ji);
                     }
                 }
                 mChanged = true;
             }
        }
    }

}


void InputCustomizer::onStateEnded()
{

    if(mChanged){
        app->usersDatabase()->saveUser(app->usersDatabase()->activeUser());
    }

    print("InputCustomizer::onStateEnded()");

}


void InputCustomizer::rebuildItems()
{

    print("InputCustomizer::rebuildItems()");

    ncine::IInputManager &ncInputManager = ncine::theApplication().inputManager();
    InputSystem &inputSystem = *app->inputSystem();

    mStoredDeviceNames.clear();
    mButtonsTable->setItemsGroup(nullptr);

    //--- rebuild all device profiles
    mInputDeviceItems->deleteAllItems();

    assert(inputSystem.activeUserProfiles());

    for(KeyboardProfile *p : inputSystem.activeUserProfiles()->mKeyboardProfiles){
        mInputDeviceItems->addItem(new InputDeviceItem(p->name(), mInputDeviceItems.get(), p));
        mStoredDeviceNames.push_back(p->name());
    }

    for(JoystickProfile *p : inputSystem.activeUserProfiles()->mJoystickProfiles){
        if(p->joystick()->isConnected()){
            mInputDeviceItems->addItem(new InputDeviceItem(p->joystick()->name(), mInputDeviceItems.get(), p));
            mStoredDeviceNames.push_back(p->joystick()->name());
        }
    }

    //--- set buttons
    if(mInputDeviceItems->items().empty()==false){
        //mDevicesTable->updateSlots();
        mDevicesTable->setSelectedItem(mInputDeviceItems->items().front());
        mDevicesTable->ScrollToTheStartOfItems();

        InputDeviceItem * i = dynamic_cast<InputDeviceItem*>(mDevicesTable->selectedItem());
        if(i){
            mButtonsTable->setItemsGroup(i->buttonsItems());
            //mDevicesTable->updateSlots();
        }
    }

    mButtonsTable->ScrollToTheStartOfItems();


    mStoredUserName = "";
    assert(app->usersDatabase()->activeUser());
    mStoredUserName = app->usersDatabase()->activeUser()->name();

}


bool InputCustomizer::hasInputDevicesChanged()
{

    if(mStoredDeviceNames.empty()){         // initial state
        return true;
    }

    //--- check if the user changed
    if(app->usersDatabase()->activeUser()){
        if(mStoredUserName != app->usersDatabase()->activeUser()->name()){
            return true;
        }
    }

    InputSystem &inputSystem = *app->inputSystem();

    unsigned int index=0;

    //--- keyboard profiles
    for(KeyboardProfile* p : inputSystem.activeUserProfiles()->mKeyboardProfiles){
        if(index<mStoredDeviceNames.size()){
            if(p->name() != mStoredDeviceNames[index]){      // this shouldn't be actually possible
                return true;
            }
        }else{
            return true;
        }
        index++;
    }

    //--- joystick profiles
    //for(JoystickProfiles &ps : inputSystem.joystickProfilesPerJoystick()){
        //if(ps.mJoystick->isConnected()){
            for(JoystickProfile *p : inputSystem.activeUserProfiles()->mJoystickProfiles){
                if(p->joystick()->isConnected()){
                    if(index<mStoredDeviceNames.size()){
                        if(p->joystick()->name() != mStoredDeviceNames[index]){      // this is possible if different joystick has been connected
                            return true;
                        }
                    }else{
                        return true;
                    }
                    index++;
                }
            }
        //}
    //}

    if(index != mStoredDeviceNames.size()){     // this is possible if a joystick has been disconected
        return true;
    }


    return false;

}


//----------------------------------------------------------------------------------------------



InputDeviceItem::InputDeviceItem(const std::string &_name, ItemsGroup *_parentItemsGroup, InputDeviceProfile* _deviceProfile)
    : GameItem(_name, _parentItemsGroup)
{

    mDeviceProfile = _deviceProfile;
    mButtonsItems = new ItemsGroup(mDeviceProfile->name(), this);

    mGuiSlotData = new GuiSlotData(this);

    GuiSlotDataTextSprite* ic = new GuiSlotDataTextSprite(mGuiSlotData);
    ic->mText = _name;
    ic->mSlotTextSpriteName = "itemName";

    mGuiSlotData->components().push_back(ic);


    //---- create button items
    if(dynamic_cast<KeyboardProfile*>(mDeviceProfile)){
        KeyboardProfile* kp = static_cast<KeyboardProfile*>(mDeviceProfile);

        for(KeyboardInputCommand &c : kp->keyboardInputCommands()){
           KeyboardInputCommandItem *ki = new KeyboardInputCommandItem(c.mGameInputCommand->name(), mButtonsItems, &c);
           mButtonsItems->addItem(ki);
           updateDuplicateKeyboardKeyMarkers(ki->keyboardInputCommand()->mKeyCode);
        }


    }else if(dynamic_cast<JoystickProfile*>(mDeviceProfile)){
        JoystickProfile* jp = static_cast<JoystickProfile*>(mDeviceProfile);

        for(JoystickInputCommand &c : jp->joystickInputCommands()){
            JoystickInputCommandItem *jp = new JoystickInputCommandItem(c.mGameInputCommand->name(), mButtonsItems, &c);
            mButtonsItems->addItem(jp);
            updateDuplicateJoystickButtonMarkers(jp->joystickInputCommand()->mJoystickControl);
        }
    }


}



InputDeviceItem::~InputDeviceItem()
{
    delete mButtonsItems;
}


void InputDeviceItem::updateDuplicateKeyboardKeyMarkers(KeyCode _keyCode)
{

    assert(dynamic_cast<KeyboardProfile*>(mDeviceProfile));

    std::vector<KeyboardInputCommandItem*>itemsWithKeyCode;

    for(GameItem *i : mButtonsItems->items()){
        if(static_cast<KeyboardInputCommandItem*>(i)->keyboardInputCommand()->mKeyCode == _keyCode){
            itemsWithKeyCode.push_back(static_cast<KeyboardInputCommandItem*>(i));
        }
    }


    if(itemsWithKeyCode.size()==1){
        KeyboardInputCommandItem* ki = itemsWithKeyCode.front();
        //GuiSlotDataSprite *c = static_cast<GuiSlotDataSprite*>(ki->slotData()->components().at(2));    // "duplicateButton";
        GuiSlotDataSprite *c = dynamic_cast<GuiSlotDataSprite*>(ki->slotData()->getComponent("duplicateButton"));
        if(c->mVisible){
            c->mVisible = false;
            if(mButtonsItems->table()){
                mButtonsItems->table()->updateSlotContentForItem(ki);
            }
        }


    }else if(itemsWithKeyCode.size()>1){
        for(KeyboardInputCommandItem* ki : itemsWithKeyCode){
            //GuiSlotDataSprite *c = static_cast<GuiSlotDataSprite*>(ki->slotData()->components().at(2));    // "duplicateButton";
            GuiSlotDataSprite *c = dynamic_cast<GuiSlotDataSprite*>(ki->slotData()->getComponent("duplicateButton"));
            if(c->mVisible==false){
                c->mVisible = true;
                if(mButtonsItems->table()){
                    mButtonsItems->table()->updateSlotContentForItem(ki);
                }
            }
        }
    }
}


void InputDeviceItem::updateDuplicateJoystickButtonMarkers(JoystickControl _joystickCommand)
{

    assert(dynamic_cast<JoystickProfile*>(mDeviceProfile));

    std::vector<JoystickInputCommandItem*>itemsWithJoystickCommand;

    for(GameItem *i : mButtonsItems->items()){
        if(static_cast<JoystickInputCommandItem*>(i)->joystickInputCommand()->mJoystickControl == _joystickCommand){
            itemsWithJoystickCommand.push_back(static_cast<JoystickInputCommandItem*>(i));
        }
    }


    if(itemsWithJoystickCommand.size()==1){
        JoystickInputCommandItem* ki = itemsWithJoystickCommand.front();
        //GuiSlotDataSprite *c = static_cast<GuiSlotDataSprite*>(ki->slotData()->components().at(2));    // "duplicateButton";
        GuiSlotDataSprite *c = dynamic_cast<GuiSlotDataSprite*>(ki->slotData()->getComponent("duplicateButton"));
        if(c->mVisible){
            c->mVisible = false;
            if(mButtonsItems->table()){
                mButtonsItems->table()->updateSlotContentForItem(ki);
            }
        }


    }else if(itemsWithJoystickCommand.size()>1){
        for(JoystickInputCommandItem* ki : itemsWithJoystickCommand){
            //GuiSlotDataSprite *c = static_cast<GuiSlotDataSprite*>(ki->slotData()->components().at(2));    // "duplicateButton";
            GuiSlotDataSprite *c = dynamic_cast<GuiSlotDataSprite*>(ki->slotData()->getComponent("duplicateButton"));
            if(c->mVisible==false){
                c->mVisible = true;
                if(mButtonsItems->table()){
                    mButtonsItems->table()->updateSlotContentForItem(ki);
                }
            }
        }
    }

}



//------------------------------------------------------------------------------------------------


KeyboardInputCommandItem::KeyboardInputCommandItem(const std::string &_name, ItemsGroup *_itemsGroup, KeyboardInputCommand *_command)
    : GameItem(_name, _itemsGroup)
{

    mKeyboardInputCommand = _command;

    mGuiSlotData = new GuiSlotData(this);

    GuiSlotDataTextSprite* tc = new GuiSlotDataTextSprite(mGuiSlotData);
    tc->mSlotTextSpriteName = "inputCommandDescription";
    tc->mText = mKeyboardInputCommand->mGameInputCommand->description();
    mGuiSlotData->components().push_back(tc);

    //---
    tc = new GuiSlotDataTextSprite(mGuiSlotData);
    tc->mSlotTextSpriteName = "inputCommandButton";
    tc->mText = keyboard.keyNameFromKeyCode(mKeyboardInputCommand->mKeyCode);
    mGuiSlotData->components().push_back(tc);

    //--- duplicate button icon
    GuiSlotDataSprite *sc = new GuiSlotDataSprite(mGuiSlotData, GuiSlotDataSprite::Mode::MODIFY_SLOT_SPRITE);
    sc->mSlotSpriteName = "duplicateButton";
    sc->mVisible = false;
    mGuiSlotData->components().push_back(sc);

}


void KeyboardInputCommandItem::setKeyCode(KeyCode _keyCode)
{

    KeyCode keyCodeOld = mKeyboardInputCommand->mKeyCode;
    mKeyboardInputCommand->mKeyCode = _keyCode;

    //GuiSlotDataTextSprite *cKeyCodeString = static_cast<GuiSlotDataTextSprite*>(mGuiSlotData->components().at(1));    // "inputCommandButton";
    GuiSlotDataTextSprite *c = dynamic_cast<GuiSlotDataTextSprite*>(mGuiSlotData->getComponent("inputCommandButton"));
    c->mText = keyboard.keyNameFromKeyCode(mKeyboardInputCommand->mKeyCode);


    InputDeviceItem *inputDeviceItem = dynamic_cast<InputDeviceItem*>(mParentItemsGroup->parent());
    inputDeviceItem->updateDuplicateKeyboardKeyMarkers(keyCodeOld);
    inputDeviceItem->updateDuplicateKeyboardKeyMarkers(_keyCode);

}


//_------------------------------------------------------------------------------------------------


JoystickInputCommandItem::JoystickInputCommandItem(const std::string &_name, ItemsGroup *_itemsGroup, JoystickInputCommand *_command)
    : GameItem(_name, _itemsGroup)
{

    mJoystickInputCommand = _command;

    // only joystrick buttons can be changed (hats and axis can not)
    if(_command->mJoystickControl.mButtonName == ncine::ButtonName::UNKNOWN){
        mSelectable = false;
    }


    mGuiSlotData = new GuiSlotData(this);
    GuiSlotDataTextSprite* ic = new GuiSlotDataTextSprite(mGuiSlotData);
    ic->mSlotTextSpriteName = "inputCommandDescription";
    ic->mText = mJoystickInputCommand->mGameInputCommand->description();

    mGuiSlotData->components().push_back(ic);

    //---
    ic = new GuiSlotDataTextSprite(mGuiSlotData);
    ic->mSlotTextSpriteName = "inputCommandButton";
    ic->mText = mJoystickInputCommand->mJoystickControl.getName();

    mGuiSlotData->components().push_back(ic);

    //--- duplicate button icon
    GuiSlotDataSprite *sc = new GuiSlotDataSprite(mGuiSlotData, GuiSlotDataSprite::Mode::MODIFY_SLOT_SPRITE);
    sc->mSlotSpriteName = "duplicateButton";
    sc->mVisible = false;
    mGuiSlotData->components().push_back(sc);
}



void JoystickInputCommandItem::setJoystickCommand(JoystickControl _joystickControl)
{

    JoystickControl joystickControlOLD = mJoystickInputCommand->mJoystickControl;
    mJoystickInputCommand->mJoystickControl = _joystickControl;

    //GuiSlotDataTextSprite *ic = static_cast<GuiSlotDataTextSprite*>(mGuiSlotData->components().at(1));    // "inputCommandButton";
    GuiSlotDataTextSprite *c = dynamic_cast<GuiSlotDataTextSprite*>(mGuiSlotData->getComponent("inputCommandButton"));
    c->mText = mJoystickInputCommand->mJoystickControl.getName();


    InputDeviceItem *inputDeviceItem = dynamic_cast<InputDeviceItem*>(mParentItemsGroup->parent());
    inputDeviceItem->updateDuplicateJoystickButtonMarkers(joystickControlOLD);
    inputDeviceItem->updateDuplicateJoystickButtonMarkers(_joystickControl);
}



}
