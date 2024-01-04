#ifndef PLAYER__LOGIC_STATE_COMMANDS_GUI_H
#define PLAYER__LOGIC_STATE_COMMANDS_GUI_H


#include <string>
#include <vector>

#include <jmTween.h>

#include "data/jpDataUtilities.h"
#include "jpQueries.h"
#include "jpLogicCommon.h"
#include "jpLogicAction.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class SceneWidget;
class GuiButton;
class GuiTable;
class GuiTableNew;
class GuiTextInput;
class ItemsGroup;
class Item;
class PlayedScene;
class ComponentData;
class LogicFunction;



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


class DoSetSignal : public ActionCommand
{
public:
    static std::string doType;



    DoSetSignal(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    SignalSetter mSignalSetter;

};



class DoSetSignalNEW : public ActionCommand
{
public:
    static std::string doType;



    DoSetSignalNEW(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    SignalSetter mSignalSetter;

};

//----------------------------------------------------------------------------------

/*
class DoSetData : public ActionCommand
{
public:
    static std::string doType;


    DoSetData(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    DataSetter mDataSetter;


};
*/


class DoSetDataNEW : public ActionCommand
{
public:
    static std::string doType;


    DoSetDataNEW(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    DataSetter mDataSetter;


};


//----------------------------------------------------------------------------------


class DoSetItem : public ActionCommand
{
public:
    static std::string doType;


    DoSetItem(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    ItemSetter mItemMover;


};



/*

class DoFunction : public ActionCommand
{
public:
    static std::string doType;


    DoFunction(Action*_parentGameAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    LogicFunction *mFunction = nullptr;      // LINK
    SimpleNoNameStorage<FunctionArgument>mFunctionArguments;


};

*/


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


struct PrintParsingHelper
{

    BaseObject *rootParentObject = nullptr;             // LINK
    LogicState *logicState = nullptr;                   // LINK

    std::string textKind;
    std::string originObject;
    std::string originData;

    OriginObjectObtainer *originObtainer = nullptr;     // LINK

    std::string text;

    ItemDataPair itemDataPair;

    bool parseMainPaths(const std::string &_fullPath);

};




class ADbgPrint : public ActionCommand
{
public:
    static std::string doType;

    enum class Kind
    {
        LOCAL_DATA,
        ITEM_DATA,
        ORIGIN_OBJECT

    };



    ADbgPrint(Action*_parentGameAction, const std::string &_value);
    ~ADbgPrint();

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;



private:
    Kind mKind = Kind::LOCAL_DATA;

    ItemDataPair mItemDataPair;
    std::string mText;

    DataToText mDataToText;

    OriginObjectObtainer *mOriginObtainer = nullptr;     // LINK;

};


//----------------------------------------------------------------------------------



class ADbgPrintNEW : public ActionCommand
{
public:
    static std::string doType;

    enum class Kind
    {
        DIRECT_TEXT,
        DATA

    };



    ADbgPrintNEW(Action*_parentGameAction, const std::string &_value);
    ~ADbgPrintNEW();

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;



private:
    Kind mKind = Kind::DIRECT_TEXT;

    //ItemDataPair mItemDataPair;
    //std::string mText;

    DataToText mDataToText;

    //OriginObjectObtainer *mOriginObtainer = nullptr;     // LINK;

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


//----------------------------------------------------------------------------------



class SetTableNewItemsGroup : public ActionCommand
{
public:
     static std::string doType;

    SetTableNewItemsGroup(Action*_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    GuiTableNew *mTable = nullptr;          // LINK
    ItemDataPair mItemDataPair;


};


//----------------------------------------------------------------------------------



class AssignItemToWidget : public ActionCommand
{
public:
     static std::string doType;

    AssignItemToWidget(Action*_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void run(UpdateMode &_updateMode) override;


private:
    GuiWidget *mWidget = nullptr;          // LINK
    ItemDataPair mItemDataPair;


};




}

#endif // PLAYER__ACTION_COMMANDS_GUI_H
