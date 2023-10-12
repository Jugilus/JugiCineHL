#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiBar.h"
#include "jmGuiButton.h"
#include "jmGuiSlider.h"
#include "jmGuiTextInput.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"
#include "jpPlayedApp.h"
#include "jpUser.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpCommands_Gui.h"



namespace jugimap{




std::string SetButtonState::type = "setButtonState";


SetButtonState::SetButtonState(Action*_parentGameAction, const pugi::xml_node &node)
{

    mParentGameAction = _parentGameAction;
    mButtonName = node.attribute("button").as_string("");
    mCheckedString = node.attribute("checked").as_string("");
    mEnabledString = node.attribute("enabled").as_string("");
    mVisibleString = node.attribute("visible").as_string("");

}


bool SetButtonState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    //mButtonState = getButtonStateFromString(mButtonStateString);
    //if(mButtonState==ButtonState::NOT_DEFINED){
    //    dbgSystem.addMessage("Wrong buttonState '" + mButtonStateString + "'!");
    //    return false;
    //}

    mButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mButtonName, GuiWidgetKind::BUTTON));
    if(mButton==nullptr){
        dbgSystem.addMessage("Button '" + mButtonName + "' not found!");
        return false;
    }


    bool error= false;

    if(mCheckedString.empty()==false){
        mVarChecked = ObtainBoolVariableFromPath(mCheckedString, _scene, mChecked, error);
        if(error){
            return false;
        }else{
            if(mButton->IsCheckable()==false){
                dbgSystem.addMessage("Button '" + mButtonName + "' can not have set 'checked' because it was not defined as checkable!");
                return false;

            }
            /*
            if(mVarChecked){
                if(mVarChecked->value()==false){
                    if(mButton->GetExclusiveGroup()){
                        dbgSystem.addMessage("Button '" + mButtonName + "' can not be set 'unchecked' because it is in an exclusive group!");
                        return false;
                    }
                }
            }
            */
        }
    }

    if(mEnabledString.empty()==false){
        mVarEnabled = ObtainBoolVariableFromPath(mCheckedString, _scene, mEnabled, error);
        if(error){
            return false;
        }
    }

    if(mVisibleString.empty()==false){
        mVarVisible = ObtainBoolVariableFromPath(mCheckedString, _scene, mVisible, error);
        if(error){
            return false;
        }
    }

    /*
    if(mButtonState==ButtonState::CHECKED && mButton->IsCheckable()==false){
        dbgSystem.addMessage("Button '" + mButtonName + "' can not be set 'checked' because it was not defined as checkable!");
        return false;
    }

    if(mButtonState==ButtonState::UNCHECKED){
        if(mButton->IsCheckable()==false){
            dbgSystem.addMessage("Button '" + mButtonName + "' can not be set 'unchecked' because it was not defined as checkable!");
            return false;

        }else if(mButton->GetExclusiveGroup()){
            dbgSystem.addMessage("Button '" + mButtonName + "' can not be set 'unchecked' because it is in an exclusive group!");
            return false;
        }

    }
    */

    dbgSystem.removeLastMessage();
    return true;

}


void SetButtonState::execute()
{

    // checked
    if(mCheckedString.empty()==false){

        bool checked = mChecked;
        if(mVarChecked){
            checked = mVarChecked->value();
        }
        mButton->SetChecked(checked);

    }

    // enabled
    if(mEnabledString.empty()==false){

        bool enabled = mEnabled;
        if(mVarEnabled){
            enabled = mVarEnabled->value();
        }
        mButton->SetDisabled(!enabled);
    }

    // visible
    if(mVisibleString.empty()==false){

        bool visible = mVisible;
        if(mVarVisible){
            visible = mVarVisible->value();
        }
        mButton->SetVisible(visible);
    }


    /*
    if(mButtonState==ButtonState::CHECKED){
        mButton->SetChecked(true);

    }else if(mButtonState==ButtonState::UNCHECKED){
        mButton->SetChecked(false);

    }else if(mButtonState==ButtonState::DISABLED){
        mButton->SetDisabled(true);

    }else if(mButtonState==ButtonState::ENABLED){
        mButton->SetDisabled(false);

    }else if(mButtonState==ButtonState::HIDDEN){
        mButton->SetVisible(false);

    }else if(mButtonState==ButtonState::VISIBLE){
        mButton->SetVisible(true);

    }
    */

}

/*
SetButtonState::ButtonState SetButtonState::getButtonStateFromString(const std::string &_state)
{

    if(_state=="checked"){
        return ButtonState::CHECKED;

    }else if(_state=="unchecked"){
        return ButtonState::UNCHECKED;

    }else if(_state=="disabled"){
        return ButtonState::DISABLED;

    }else if(_state=="enabled"){
        return ButtonState::ENABLED;

    }else if(_state=="hidden"){
        return ButtonState::HIDDEN;

    }else if(_state=="visible"){
        return ButtonState::VISIBLE;
    }

    return ButtonState::NOT_DEFINED;

}
*/


//---------------------------------------------------------------------------------------



std::string SetTextInput::type = "setTextInput";


SetTextInput::SetTextInput(Action*_parentGameAction, const pugi::xml_node &node)
{

    mParentGameAction = _parentGameAction;
    mTextInputName = node.attribute("textInput").as_string("");
    mInitialTextPath = node.attribute("initialText").as_string("");
    mStartString = node.attribute("start").as_string("");

}


bool SetTextInput::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");


    mTextInput = dynamic_cast<GuiTextInput*>(ObtainGuiWidget(_scene, mTextInputName, GuiWidgetKind::TEXT_INPUT));
    if(mTextInput==nullptr){
        dbgSystem.addMessage("Text input '" + mTextInputName + "' not found!");
        return false;
    }

    if(mInitialTextPath.empty()==false){
        if(ObtainPlainText(mInitialTextPath, mInitialText)==false){
            return false;
        }
    }

    if(mStartString.empty()==false){

        if(mStartString=="instantly"){
            mStart = Start::INSTANTLY;

        }else if(mStartString=="on_clicked"){
            mStart = Start::ON_CLICKED;

        }else{
            dbgSystem.addMessage("The 'start' parameter has wrong value '" + mStartString + "'!");
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


void SetTextInput::execute()
{

    mTextInput->setText(mInitialText);

    if(mStart==Start::INSTANTLY){
        mTextInput->start();
    }

}





//---------------------------------------------------------------------------------------



std::string SetTableItemsGroup::type = "setTableItemsGroup";


SetTableItemsGroup::SetTableItemsGroup(Action*_parentGameAction, const pugi::xml_node &node)
{

    mParentGameAction = _parentGameAction;
    mTableName = node.attribute("table").as_string("");
    mItemsGroupName = node.attribute("itemsGroup").as_string("");
    //mSystemItemsGroupName = node.attribute("systemItemsGroup").as_string("");

}



bool SetTableItemsGroup::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");


    mTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, mTableName, GuiWidgetKind::TABLE));
    if(mTable==nullptr){
        return false;
    }

    //---
    //if(mSystemItemsGroupName=="usersNames"){
    //    mItemsGroup = app->usersDatabase()->usersNamesItems();

    //}else{
        mItemsGroup = ObtainItemsGroup(_scene, mItemsGroupName);
    //}

    if(mItemsGroup==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void SetTableItemsGroup::execute()
{
    mTable->setItemsGroup(mItemsGroup);
}









}
