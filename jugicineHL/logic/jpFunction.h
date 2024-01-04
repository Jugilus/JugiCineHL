#ifndef PLAYER__LOGIC__FUNCTION_H
#define PLAYER__LOGIC__FUNCTION_H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "data/jpDataUtilities.h"
#include "jpGlobal.h"
#include "jpSettings.h"
#include "jpLogicStateCfg.h"
#include "jpLogicCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


/*

class PlayedScene;
class Action;
class LogicState;
class LogicFunction;
struct ParentObjects;





//-------------------------------------------------------------------------


class LogicFunctionCfg
{
public:

    LogicFunctionCfg(const std::string &_name) : mName(_name){}

    bool initCfg(const pugi::xml_node &_node);


    const std::string & name(){ return mName; }
    SimpleNoNameStorage<FunctionArgument> & argumentsCfg(){ return mArgumentCfg; }
    std::vector<ActionCfg> & actionsCfgs(){ return mActionsCfgs; }
    SimpleNoNameStorage<ItemSourceData*> & sourceData(){ return mSourceData; }


private:
    std::string mName;
    SimpleNoNameStorage<FunctionArgument>mArgumentCfg;
    std::vector<ActionCfg>mActionsCfgs;
    SimpleNoNameStorage<ItemSourceData*>mSourceData;

};



class LogicFunction : public BaseObject
{
public:

    LogicFunction(LogicFunctionCfg* _cfg, BaseObject * _parent);

    bool build();

    bool initConnections(PlayedScene *_scene, SimpleNoNameStorage<FunctionArgument> *_arguments, ParentObjects& _parentObjects);
    bool run(SimpleNoNameStorage<FunctionArgument> *_arguments, ParentObjects& _parentObjects);


    const std::string & name(){ return mName;}
    LogicFunctionCfg* cfg(){ return mCfg; }
    SimpleNoNameStorage<FunctionArgument> * arguments(){ return mArguments; }
    SimpleNoNameStorage<ItemData*> &data(){ return mData; }
    std::vector<Action*> &actions(){ return mActions; }

    LogicState* parentState(){ return mParentState; }
    BaseObject* rootParentObject(){ return mRootParentObject; }


private:
    std::string mName;
    LogicFunctionCfg* mCfg = nullptr;                               // LINK
    BaseObject *mParentObject = nullptr;

    LogicState *mParentState = nullptr;
    BaseObject *mRootParentObject = nullptr;


    SimpleNoNameStorage<FunctionArgument>*mArguments = nullptr;         // LINK
    SimpleNoNameStorage<ItemData*>mData;
    std::vector<Action*>mActions;                                       // OWNED

};


//------------------------------------------------------------------------------------------

class FunctionStorage
{
public:

    bool initCfg(const pugi::xml_node &_node);
    bool buildObjects(PlayedScene *_scene);

    SimpleNoNameStorage<LogicFunctionCfg*> & functionCfgs(){ return mFunctionCfgs; }
    SimpleNoNameStorage<LogicFunction*> & functions(){ return mFunctions; }

    //SimpleNoNameStorage<ItemDataAction*> & itemDataActions(){ return mItemDataActions; }

private:

    SimpleNoNameStorage<LogicFunctionCfg*>mFunctionCfgs;
    SimpleNoNameStorage<LogicFunction*>mFunctions;

};


*/


}


#endif
