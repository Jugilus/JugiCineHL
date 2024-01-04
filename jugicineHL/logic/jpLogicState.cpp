#include "pugixml/pugixml.hpp"
#include "jmSystem.h"

//#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"

#include "jpQueries.h"
#include "jpVariables.h"
#include "jpUtilities.h"
#include "jpCompound.h"
#include "jpComponentsCommon.h"
#include "jpPlayedScene.h"
#include "jpObjectFactory.h"
#include "jpLogicStateCfg.h"
#include "jpLogicAction.h"
#include "jpLogicState.h"



namespace jugimap{




LogicState::LogicState(LogicStateCfg *_cfg, BaseObject* _parent)
{

    mCfg = _cfg;
    mParentObject = _parent;
    mName = _cfg->name();

    mBaseType = BaseObjectType::LOGIC_STATE;

    mCustomState = _cfg->createCustomState(this);

}



LogicState::~LogicState()
{

    for(Action* ga : mActions){
        delete ga;
    }

    for(LogicState *gs : mChildStates){
        delete  gs;
    }

    for(Signal *s : mCustomUpdatedSignals){
        delete s;
    }


    delete mCustomState;

}



bool LogicState::build(LogicStateCfg* _logicStateCfg)
{

    for(ActionCfg &cfg : _logicStateCfg->actionsCfgs()){
        Action *a = new Action(this);
        if(a->build(cfg)==false){
            return false;
        }
        mActions.push_back(a);
    }

    if(mCustomState->build()==false){
        return false;
    }

    for(LogicStateCfg* cfg : _logicStateCfg->childStatesCfgs()){
        LogicState* childState = new LogicState(cfg, this);
        if(childState->build(cfg)==false){
            return false;
        }
        mChildStates.push_back(childState);
    }

    for(ItemData* isd : _logicStateCfg->sourceDataStorage().objects()){
        //if(mDataStorage.addObject(ItemData::create(isd))==nullptr){
        if(mDataStorage.addObject(ItemData::copy(isd))==nullptr){
            return false;
        }
    }

    return true;

}



bool LogicState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing logic state '" + mName + "' ...");


    mParentPlayerScene = _scene;

    //---
    for(Action* ga : mActions){
        if(ga->initConnections(_scene)==false){
            return false;
        }
    }

    if(mCustomState->initConnections(_scene)==false){
        return false;
    }


    //---
    for(LogicState *gs : mChildStates){
        if(gs->initConnections(_scene)==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;
}


void LogicState::collectSignals(SignalStorage &_signalStorage, std::string _prevIdentifier)
{

    std::string identifier = _prevIdentifier;
    if(mParentObject->baseType()==BaseObjectType::LOGIC_STATE){

        if(identifier.empty()==false){
            identifier += ":";
        }
        identifier += mName;
        _signalStorage.addSignal_query(&mSigState, "STATE:" + identifier);

        if(mCustomState){
            mCustomState->collectSignals(_signalStorage, identifier);
        }


    }else{
        // root logic state is just the base container and is never called
    }


    //---
    for(LogicState *gs : mChildStates){
        gs->collectSignals(_signalStorage, identifier);
    }

}



bool LogicState::startingPhaseUpdate()
{

    if(mNextChildState){

        if(mActiveChildState){
            mActiveChildState->preUpdate_finishState();

            mNextChildState->customState()->initTransitionFrom(mActiveChildState->customState());
        }else{
            mNextChildState->customState()->initTransitionFrom(nullptr);
        }

        mPreviousChildState = mActiveChildState;
        mActiveChildState = mNextChildState;

        mNextChildState = nullptr;

    }


    bool startingPhase = false;

    if(mCustomState->startingPhaseUpdate()==true){
        startingPhase = true;
    }

    if(mActiveChildState){
        if(mActiveChildState->startingPhaseUpdate()==true){
            startingPhase = true;
        }
    }

    mSigState.setValue(true);

    return startingPhase;

}



// PRE UPDATE

void LogicState::preUpdate(UpdateMode &_updateMode)
{

    if(mNextChildState){

        //---
        if(mActiveChildState){
            mActiveChildState->preUpdate_finishState();
            mNextChildState->customState()->initTransitionFrom(mActiveChildState->customState());
        }else{
            mNextChildState->customState()->initTransitionFrom(nullptr);
        }

        mPreviousChildState = mActiveChildState;
        mActiveChildState = mNextChildState;
        mNextChildState = nullptr;

        mActiveChildState->preUpdate_startState();
    }


    mCustomState->preUpdate(_updateMode);


    if(mActiveChildState){
        mActiveChildState->preUpdate(_updateMode);
    }

}



void LogicState::preUpdate_startState()
{


    mCustomState->start();

    if(mCustomUpdatedSignals.empty()==false){
        Signal::resetSignals(mCustomUpdatedSignals);
    }

    if(mActiveChildState){
        mActiveChildState->preUpdate_startState();
    }

    mSigState.setValue(true);

}


void LogicState::preUpdate_finishState()
{

    mCustomState->finish();

    if(mCustomUpdatedSignals.empty()==false){
        Signal::resetSignals(mCustomUpdatedSignals);
    }

    //---
    if(mActiveChildState){
        mActiveChildState->preUpdate_finishState();
    }

    mSigState.setValue(false);

}


void LogicState::update(UpdateMode &_updateMode)
{

    for(Action* ga : mActions){
        if(ga->update(_updateMode)==true){
            //if(_updateMode.loopAllStateActions==false){
            if(ga->breakOnExecuted()){
                break;
            }
        }
    }

    mCustomState->update(_updateMode);

    //---
    for(Signal *s : mCustomUpdatedSignals){
        if(s->type()==SignalType::BOOL){
            UpdatedBoolSignal* ces = static_cast<UpdatedBoolSignal*>(s);
            ces->update();
        }
    }

    //---
    if(mActiveChildState){
        mActiveChildState->update(_updateMode);
    }

}


void LogicState::postUpdate(UpdateMode &_updateMode)
{

    mCustomState->postUpdate(_updateMode);

    if(mActiveChildState){
        mActiveChildState->postUpdate(_updateMode);
    }

    mSigState.postUpdate();

}


void LogicState::drawShapes(jugimap::ShapeDrawer &_drawer, int _flags)
{

    mCustomState->drawShapes(_drawer, _flags);

    if(mActiveChildState){
        mActiveChildState->drawShapes(_drawer, _flags);
    }

}


void LogicState::setNextChildState(LogicState *_gameState)
{

    if(mActiveChildState==_gameState){
        return;
    }

    //---
    //if(StdVector::contains(mChildStates, _gameState)==false){       // debug check
    //    assert(false);
    //}

    mNextChildState = _gameState;

}


LogicState * LogicState::findChildState(const std::string &_stateName,  bool _setErrorMessage)
{

    for(LogicState* ls : mChildStates){
        if(ls->name()==_stateName){
            return ls;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A state with name '" + _stateName + "' not found!");
    }

    return nullptr;
}


BaseObject* LogicState::rootParentObject()
{

    BaseObject* parent = mParentObject;

    while(parent){

        if(parent->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState *state = static_cast<LogicState *>(parent);
            parent = state->parentObject();

        }else{
            return parent;
        }

    }

    assert(false);
    return nullptr;

}


LogicState* LogicState::rootLogicState()
{

    BaseObject* object = this;

    while(true){

        LogicState *state = static_cast<LogicState *>(object);

        assert(state->parentObject());

        if(state->parentObject()->baseType()!=BaseObjectType::LOGIC_STATE){
            return state;

        }else{
            object = state->parentObject();

        }
    }

    assert(false);
    return nullptr;
}


LogicState* LogicState::findChildStateViaPath(const std::string &_path, bool _setErrorMessage)
{

    std::vector<std::string> statePathParts = StdString::splitString(_path, ":");

    return findChildStateViaPath(statePathParts, _setErrorMessage);

}


LogicState* LogicState::findChildStateViaPath(std::vector<std::string>& statePathParts, bool _setErrorMessage)
{

    LogicState *state = this;

    bool errorInPath = true;
    int i=0;

    while(i<statePathParts.size()){

        const std::string &stateName = statePathParts.at(i);
        errorInPath = false;
        bool stateFound = false;

        for(LogicState* s : state->childStates()){
            if(s->name() == stateName){
                state = s;
                stateFound = true;
                break;
            }
        }

        if(stateFound==false){
            errorInPath = true;
            break;
        }

        i++;
    }

    if(errorInPath){
        //if(_setErrorMessage){
        //    dbgSystem.addMessage("State with path '" + _path + "' not found!");
        //}
        return nullptr;
    }

    return state;

}



//==================================================================================================

/*

LogicFunction::LogicFunction(LogicFunctionCfg *_cfg, BaseObject * _parent)
{

    mCfg = _cfg;
    mParentObject = _parent;
    mName = _cfg->name();

    mBaseType = BaseObjectType::LOGIC_FUNCTION;


}



bool LogicFunction::build()
{

    dbgSystem.addMessage("Building logic function '" + mName +"' ...");


    //for(const std::string &s : mCfg->arguments()){
    //    mArguments.addObject(FunctionArgument(s));
    //}


    for(ItemSourceData *isd : mCfg->sourceData().objects()){
        if(isd->mType==ItemDataType::INT){
            mData.addObject(new IntItemData(static_cast<IntSourceItemData*>(isd)));

        }else if(isd->mType==ItemDataType::FLOAT){
            mData.addObject(new FloatItemData(static_cast<FloatSourceItemData*>(isd)));

        }else if(isd->mType==ItemDataType::BOOL){
            mData.addObject(new BoolItemData(static_cast<BoolSourceItemData*>(isd)));

        }
    }

    //---
    for(ActionCfg &cfg : mCfg->actionsCfgs()){
        Action *a = new Action(this);
        if(a->build(cfg)==false){
            return false;
        }
        mActions.push_back(a);
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool LogicFunction::initConnections(PlayedScene *_scene, SimpleNoNameStorage<FunctionArgument> *_arguments, BaseObject *_rootParentObject, LogicState *_parentState )
{

    dbgSystem.addMessage("Initializing logic function '" + mName +"' ...");


    mRootParentObject = _rootParentObject;
    mParentState = _parentState;
    mArguments = _arguments;


    dbgSystem.removeLastMessage();
    return true;

}


bool LogicFunction::run(SimpleNoNameStorage<FunctionArgument> *_arguments, BaseObject *_rootParentObject, LogicState *_parentState )
{

    mRootParentObject = _rootParentObject;
    mParentState = _parentState;
    mArguments = _arguments;


    //----
    UpdateMode updateMode;

    for(Action* ga : mActions){
        if(ga->update(updateMode)==true){
            //if(updateMode.loopAllStateActions==false){
            //    break;
            //}
        }
    }



    return true;

}


//=================================================================================================




bool FunctionStorage::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading source items and actions... !");



    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="function"){

            std::string name = _node.attribute("name").as_string();
            if(name.empty()){
                dbgSystem.addMessage("Missing function name!");
                return false;
            }

            if(mFunctions.getObject(itemName,false)){
                dbgSystem.addMessage("Item with name '" + itemName +"' already exist!");
                return nullptr;

            }

            SourceItem *sourceItem =  mFunctions.addObject(new LogicFunction(itemName));

            if(sourceItem->initCfg(this, _node)==false){
                return nullptr;
            }


            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="item"){
                    if(initCfg_addSourceItem(nChild)==nullptr){
                        return false;
                    }
                }
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }

    }


    dbgSystem.removeLastMessage();

    return true;

}


bool FunctionStorage::buildObjects(PlayedScene *_scene)
{

    dbgSystem.addMessage("Building source items and actions... !");

    for(SourceItem *si : mSourceItems.objects()){
        if(si->build(this)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();

    return true;

}


SourceItem *FunctionStorage::initCfg_addSourceItem(const pugi::xml_node &_node)
{

    std::string itemName = _node.attribute("name").as_string();
    if(itemName.empty()){
        dbgSystem.addMessage("Missing item name!");
        return nullptr;
    }

    if(mSourceItems.getObject(itemName,false)){
        dbgSystem.addMessage("Item with name '" + itemName +"' already exist!");
        return nullptr;

    }

    SourceItem *sourceItem =  mSourceItems.addObject(new SourceItem(itemName));

    if(sourceItem->initCfg(this, _node)==false){
        return nullptr;
    }

    return sourceItem;

}

*/


//==================================================================================================

/*

Action::Action(BaseObject* _parentObject) : mParentObject(_parentObject)
{
    mBaseType = BaseObjectType::ACTION;
}


Action::~Action()
{

    for(ActionCondition *ac : mConditions){
        delete ac;
    }

    for(ActionCommand *ac : mCommands){
        delete ac;
    }

}


bool Action::build(ActionCfg &_actionCfg)
{

    for(ActionConditionCfg &cfg : _actionCfg.mConditions){

        ActionCondition* condition = actionFactory->createActionCondition(cfg.name, this, cfg.value);
        if(condition==false){
            dbgSystem.addMessage("Action condition '" + cfg.name +"' not found!");
            return false;
        }
        mConditions.push_back(condition);
    }

    for(ActionCommandCfg &cfg : _actionCfg.mCommands){

        ActionCommand *command = actionFactory->createActionCommands(cfg.name, this, cfg.value);
        if(command==false){
            dbgSystem.addMessage("Action command '" + cfg.name +"' not found!");
            return false;
        }
        mCommands.push_back(command);
    }

    mDisabled = _actionCfg.mDisabled;
    mDbgId = _actionCfg.mDbgId;

    //----
    //BaseObject * parentObject = mParentAction->parentObject();
    //LogicState * parentState = nullptr;


    //assert(mParentState);
    //BaseObject * rootParentObject = nullptr;
    //if(parentState){
    //    mRootParentObject = mParentState->rootParentObject();
    //}


    return true;
}


bool Action::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing action ...");

    mParentPlayerScene = _scene;


    for(ActionCondition* ac : mConditions){
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }

    for(ActionCommand* ac : mCommands){
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool Action::update(UpdateMode &_updateMode)
{

    if(_updateMode.modalOverlay){
        return false;
    }

    if(mDisabled){
        return false;
    }

    for(ActionCondition* ac : mConditions){
        if(ac->isTrue()==false){
            return false;
        }
    }

    for(ActionCommand* ac : mCommands){
        ac->run(_updateMode);
    }


    return true;
}


ParentObjects Action::parentObjects()
{
    ParentObjects po;

    if(mParentObject->baseType()==BaseObjectType::LOGIC_STATE){
        po.parentLogicState = static_cast<LogicState*>(mParentObject);

    }else if(mParentObject->baseType()==BaseObjectType::LOGIC_FUNCTION){
         po.parentLogicFunction = static_cast<LogicFunction*>(mParentObject);
         po.parentLogicState = po.parentLogicFunction->parentState();
    }

    assert(po.parentLogicState);

    po.rootParentObject = po.parentLogicState->rootParentObject();

    return po;
}
*/

//-----------------------------------------------------------------------------------------


OverlayState::OverlayState(Compound *_compound)
{
    mBaseType = BaseObjectType::OVERLAY_STATE;

    mCompound = _compound;
    mCompoundTransition.reset(new CompoundTransition());

}


}
