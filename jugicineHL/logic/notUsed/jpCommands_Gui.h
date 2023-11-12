#ifndef PLAYER__ACTION_COMMANDS_GUI_H
#define PLAYER__ACTION_COMMANDS_GUI_H


#include <string>
#include <vector>

#include <jmTween.h>
#include "jpActionsCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class SceneWidget;
class GuiButton;
class GuiTable;
class GuiTextInput;
class ItemsGroup;
class PlayedScene;


/*
class SetButtonState : public ActionCommand
{
public:
    static std::string type;

    SetButtonState(Action*_parentGameAction, const pugi::xml_node &node);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;


private:


    GuiButton *mButton = nullptr;           // link
    //ButtonState mButtonState = ButtonState::NOT_DEFINED;


    bool mChecked = false;                                  // direct value
    bool mEnabled = true;
    bool mVisible = true;
    BoolVar *mVarChecked = nullptr;                         // LINK
    BoolVar *mVarEnabled = nullptr;                         // LINK
    BoolVar *mVarVisible = nullptr;                         // LINK


    //--- cfg parameters
    std::string mButtonName;
    std::string mCheckedString;
    std::string mEnabledString;
    std::string mVisibleString;


};
*/


/*

class SetTextInput : public ActionCommand
{
public:
    static std::string type;


    enum class Start
    {
        INSTANTLY,
        ON_CLICKED
    };


    SetTextInput(Action*_parentGameAction, const pugi::xml_node &node);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;




private:


    GuiTextInput *mTextInput = nullptr;           // link
    Start mStart = Start::ON_CLICKED;
    std::string mInitialText;

    //--- cfg parameters
    std::string mTextInputName;
    std::string mInitialTextPath;
    std::string mStartString;

};

*/


class SetTableItemsGroup : public ActionCommand
{
public:
     static std::string doType;

    SetTableItemsGroup(Action*_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    GuiTable *mTable = nullptr;             // LINK
    ItemsGroup *mItemsGroup = nullptr;      // LINK

    //--- cfg parameters
    std::string mTableName;
    std::string mItemsGroupName;

};



}

#endif // PLAYER__ACTION_COMMANDS_GUI_H
