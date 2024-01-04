#ifndef PLAYER__LOGIC_STATE_CFG_H
#define PLAYER__LOGIC_STATE_CFG_H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "jpGlobal.h"
#include "data/jpData.h"
#include "jpSettings.h"




namespace pugi{
class xml_node;
}

namespace jugimap{


//class ActionTrigger;
class ActionCommand;
class LogicState;
class CustomLogicState;



struct ActionConditionCfg
{

    ActionConditionCfg(const std::string &_name, const std::string &_value) : name(_name), value(_value){}

    std::string name;
    std::string value;
};


struct ActionCommandCfg
{

    ActionCommandCfg(const std::string &_name, const std::string &_value) : name(_name), value(_value){}

    std::string name;
    std::string value;
};


struct ActionCfg
{

    bool initCfg(const pugi::xml_node &_node);


    bool mBreakOnExecuted = true;
    bool mDisabled = false;
    std::string mDbgId;
    std::vector<ActionConditionCfg>mConditions;
    std::vector<ActionCommandCfg>mCommands;
    std::vector<ActionCfg>mChildActions;
};



struct ActionTriggerCfg
{
    bool initCfg(const pugi::xml_node &_node);

    std::vector<ActionConditionCfg>mConditions;
};




class LogicStateCfg
{
public:

    LogicStateCfg(const std::string &_name);
    virtual ~LogicStateCfg();

    virtual bool initCfg(const pugi::xml_node &_node);


    virtual LogicStateCfg* createLogicStateCfg(const std::string &_name) = 0;
    virtual CustomLogicState* createCustomState(LogicState *_parentState) = 0;


    const std::string & name(){return mName;}

    std::vector<ActionCfg> & actionsCfgs(){ return mActionsCfgs; }
    std::vector<LogicStateCfg*> & childStatesCfgs(){ return mChildStatesCfgs; }
    SimpleNoNameStorage<ItemData*> &sourceDataStorage(){ return mSourceDataStorage; }

protected:
    std::string mName;
    std::vector<ActionCfg>mActionsCfgs;           // OWNED
    std::vector<LogicStateCfg*>mChildStatesCfgs;              // OWNED

    SimpleNoNameStorage<ItemData*>mSourceDataStorage;


};





}

#endif // PLAYER__ACTION_COMMON_H
