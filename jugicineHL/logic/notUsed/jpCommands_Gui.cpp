#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"
#include "jpPlayedApp.h"
#include "jpUser.h"
#include "jpUtilities.h"

#include "gui/jpGuiCommon.h"
#include "gui/widgets/jpGuiBar.h"
#include "gui/widgets/jpGuiButton.h"
#include "gui/widgets/jpGuiSlider.h"
#include "gui/widgets/jpGuiTextInput.h"
#include "gui/widgets/jpGuiTable.h"

#include "jpCommands_Gui.h"



namespace jugimap{



/*
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

    mButton = dynamic_cast<GuiButton*>(ObtainGuiWidget(_scene, mButtonName, WidgetType::BUTTON));
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
        mButton->setDisabled(!enabled);
    }

    // visible
    if(mVisibleString.empty()==false){

        bool visible = mVisible;
        if(mVarVisible){
            visible = mVarVisible->value();
        }
        mButton->setVisible(visible);
    }




}
*/

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

/*

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


    mTextInput = dynamic_cast<GuiTextInput*>(ObtainGuiWidget(_scene, mTextInputName, WidgetType::TEXT_INPUT));
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

*/




//---------------------------------------------------------------------------------------



//std::string SetTableItemsGroup::type = "setTableItemsGroup";
std::string SetTableItemsGroup::doType = "doSetTableItemsGroup";

/*
SetTableItemsGroup::SetTableItemsGroup(Action*_parentGameAction, const pugi::xml_node &node)
{

    mParentGameAction = _parentGameAction;
    mTableName = node.attribute("table").as_string("");
    mItemsGroupName = node.attribute("itemsGroup").as_string("");
    //mSystemItemsGroupName = node.attribute("systemItemsGroup").as_string("");

}
*/

SetTableItemsGroup::SetTableItemsGroup(Action*_parentAction, const std::string &_value) : ActionCommand((_parentAction))
{
    mValue = _value;
}



bool SetTableItemsGroup::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");


    if(mValue.empty()==false){

        std::vector<std::string>parts = StdString::splitString(mValue, ",");

        if(parts.size()!=2){
            dbgSystem.addMessage("Wrong path '" + mValue + "' !");
        }

        const std::string &table = parts[0];
        const std::string &itemsGroup = parts[1];

        mTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, table, WidgetType::TABLE));
        if(mTable==nullptr){
            return false;
        }

        mItemsGroup = ObtainItemsGroup(_scene, itemsGroup);
        if(mItemsGroup==nullptr){
            return false;
        }


    }else{

        mTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, mTableName, WidgetType::TABLE));
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


    }



    dbgSystem.removeLastMessage();
    return true;

}



void SetTableItemsGroup::run(UpdateMode &_updateMode)
{

    mTable->setItemsGroup(mItemsGroup);

}








}
