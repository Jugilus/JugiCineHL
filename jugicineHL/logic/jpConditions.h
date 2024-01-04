#ifndef PLAYER__LOGIC_STATE_CONDITIONS_H
#define PLAYER__LOGIC_STATE_CONDITIONS_H

#include <memory>
#include <string>
#include <vector>

#include <jmTween.h>
#include <jmInput.h>

#include "jpQueries.h"
#include "data/jpDataUtilities.h"
#include "jpSettings.h"
#include "jpLogicAction.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class PlayedScene;
class TriggerVariable;




/*
class IfCondition : public ActionCondition
{
public:
    static std::string type;

    IfCondition(const pugi::xml_node &_node);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;

private:
    std::vector<Variable*>mVariables;
    std::vector<Variable*>mVariableValues;

    //--- cfg parameters
    struct Cfg
    {
        std::string mVariables;
    };
    std::unique_ptr<Cfg>mCfg;

};
*/


//==============================================================


class ACOnSignal : public ActionCondition
{
public:
    static std::string type;

    ACOnSignal(Action *_parentAction, const std::string &_value);


    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTrue() override;


private:
    SignalQuery mSignalQuery;

};


//==============================================================


class ACOnSignalNEW : public ActionCondition
{
public:
    static std::string type;

    ACOnSignalNEW(Action *_parentAction, const std::string &_value);
    ACOnSignalNEW(ActionTrigger *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTrue() override;


private:
    SignalQuery mSignalQuery;
    SignalParsingInfo mSignalParsingInfo;

};


//==============================================================




class ACOnSignals : public ActionCondition
{
public:
    static std::string type;


    ACOnSignals(Action *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    bool isTrue() override;


private:

    CompositeQuery mCompositeQuery;

};

//==============================================================




class ACOnSignalsNEW : public ActionCondition
{
public:
    static std::string type;


    ACOnSignalsNEW(Action *_parentAction, const std::string &_value);
    ACOnSignalsNEW(ActionTrigger *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    bool isTrue() override;


private:

    CompositeQuery mCompositeQuery;

};


//==============================================================

/*

class ACOnData : public ActionCondition
{
public:
    static std::string type;

    ACOnData(Action *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTrue() override;


private:
    DataQuery mDataQuery;



};

*/

//==============================================================


class ACOnDataNEW : public ActionCondition
{
public:
    static std::string type;

    ACOnDataNEW(Action *_parentAction, const std::string &_value);
    ACOnDataNEW(ActionTrigger *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTrue() override;


private:
    DataQuery mDataQuery;



};


//==============================================================


class ACOnItem : public ActionCondition
{
public:
    static std::string type;

    ACOnItem(Action *_parentAction, const std::string &_value);
    ACOnItem(ActionTrigger *_parentAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isTrue() override;


private:
    ItemQuery mItemQuery;



};


}

#endif // PLAYER__ACTION_TRIGGERS_LOGIC_H
