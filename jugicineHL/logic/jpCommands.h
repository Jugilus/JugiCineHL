#ifndef PLAYER__LOGIC_STATE_COMMANDS_GUI_H
#define PLAYER__LOGIC_STATE_COMMANDS_GUI_H


#include <string>
#include <vector>

#include <jmTween.h>

#include "jpQueries.h"
#include "jpLogicState.h"


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
class ComponentData;



class SetNextState : public ActionCommand
{
public:
    static std::string doType;

    SetNextState(Action*_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    LogicState *mGameState = nullptr;           // link

};


class DoSetSignalB : public ActionCommand
{
public:
    static std::string doType;



    DoSetSignalB(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    SignalSetter mSignalSetter;

};



class AShowOverlayCompound : public ActionCommand
{
public:
    //static std::string type;
    static std::string doType;

    //AShowOverlayCompound(Action*_parentGameAction, const pugi::xml_node &_node);
    AShowOverlayCompound(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:

    Compound *mCompound = nullptr;                        // LINK
    std::unique_ptr<ComponentData> mComponentData;



    //--- cfg parameters
    struct Cfg
    {
        std::string mCompound;

        // data
        std::string mTextSegment;
    };
    std::unique_ptr<Cfg>mCfg;

};


//----------------------------------------------------------------------------------



class ADbgPrint : public ActionCommand
{
public:
    static std::string doType;

    ADbgPrint(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;



private:

    std::string mText;

};



//----------------------------------------------------------------------------------



class AQuitGame : public ActionCommand
{
public:
    static std::string doType;

    AQuitGame(Action*_parentGameAction, const std::string &_value);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;

private:

};


//----------------------------------------------------------------------------------



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
