#include "ncine/Application.h"
#include "pugixml/pugixml.hpp"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
#include "jmSystem.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

//#include "gui/jpGuiCommon.h"
//#include "jmGuiBar.h"
//#include "jmGuiTextInput.h"
#include "gui/widgets/jpGuiButton.h"
#include "gui/widgets/jpGuiSlider.h"

#include "jpSceneLogicState.h"
#include "jpCompound.h"
#include "jpVariables.h"
#include "jpQueries.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpUtilities.h"
#include "jpTransition.h"
#include "jpSettings.h"
#include "jpSystemVarious.h"
#include "jpGfxObjectsCommon.h"
#include "jpComponentsCommon.h"
#include "jpItemsCommon.h"
#include "jpUser.h"
#include "jpCommands_Logic.h"



namespace jugimap{




/*

std::string SetActiveState::type = "setActiveState";


SetActiveState::SetActiveState(Action*_parentGameAction, const pugi::xml_node &node)
{
    mParentGameAction = _parentGameAction;
    mStatePath = node.attribute("state").as_string("");

}


bool SetActiveState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mGameState = ObtainLogicStateFromPath(_scene, mParentGameAction, mStatePath);

    if(mGameState==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void SetActiveState::execute()
{

    if(mGameState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
        LogicState *parentState = static_cast<LogicState*>(mGameState->parentObject());
        parentState->setActiveChildState(mGameState);
    }

}

*/

//---------------------------------------------------------------------------


std::string SetNextState::doType = "doSetState";



SetNextState::SetNextState(Action*_parentAction, const std::string &_value) : ActionCommand(_parentAction)
{
    mValue = _value;
}


bool SetNextState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    assert(mParentAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE);
    LogicState * currentState = static_cast<LogicState*>(mParentAction->parentObject());

    mGameState = ObtainLogicStateFromPath(_scene, currentState, mValue);


    if(mGameState==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}



void SetNextState::run(UpdateMode &_updateMode)
{

    if(mGameState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
        LogicState *parentState = static_cast<LogicState*>(mGameState->parentObject());
        parentState->setNextChildState(mGameState);
    }

}



//---------------------------------------------------------------------------
/*

std::string SetChildState::doType = "doSetChildState";



SetChildState::SetChildState(Action*_parentAction, const std::string &_value) : ActionCommand(_parentAction)
{
    mValue = _value;
}


bool SetChildState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    assert(mParentAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE);

    std::vector<std::string> statePathParts = StdString::splitString(mValue, ":");


    LogicState * currentState = static_cast<LogicState*>(mParentAction->parentObject());

    mGameState = currentState->findChildStateViaPath(statePathParts);

    if(mGameState==nullptr){
        dbgSystem.addMessage("State with path '" + mValue + "' not found!");
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}


void SetChildState::execute()
{

    if(mGameState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
        LogicState *parentState = static_cast<LogicState*>(mGameState->parentObject());
        parentState->setNextChildState(mGameState);
    }

}


void SetChildState::update(UpdateMode _updateMode)
{
    execute();

}

*/


//---------------------------------------------------------------------------


std::string DoSetSignalB::doType = "doSetSignal";


DoSetSignalB::DoSetSignalB(Action *_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool DoSetSignalB::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(actor->sourceEntity()->name()=="wallSwitchB"){
    //    DummyFunction();
    //}

    BaseObject * parentObject = mParentAction->parentObject();
    LogicState * parentState = nullptr;
    if(parentObject->baseType()==BaseObjectType::LOGIC_STATE){
        parentState = static_cast<LogicState*>(parentObject);
    }

    BaseObject * rootParentObject = nullptr;
    if(parentState){
        rootParentObject = parentState->rootParentObject();
    }


    std::string signalPath = mValue;
    std::string dataAsSignal;

    std::vector<std::string>parts = StdString::splitString(mValue, "=");
    if(parts.size()==2){
        const std::string & value = parts[1];
        if(value.substr(0,7) == "SIGNAL/"){
            dataAsSignal = value.substr(7);
            signalPath = parts[0];
        }
    }

    if(dataAsSignal.empty()==false){
        SignalQuery tmpQuery;
        tmpQuery.valueNotRequired = true;   // !!
        app->signalParserManager()->parseSignalAccessor(_scene, dataAsSignal, tmpQuery, rootParentObject, parentObject);

        if(tmpQuery.isValid()==false){
            return false;
        }

        mSignalSetter.mSigData = tmpQuery.mSignal;
        mSignalSetter.mSigDataFlag = tmpQuery.mIntValue;
    }


    app->signalParserManager()->parseSignalAccessor(_scene, signalPath, mSignalSetter, rootParentObject, parentObject);

    if(mSignalSetter.isValid()==false){
        return false;
    }

    //---
    if(mSignalSetter.mSigData){
        if(mSignalSetter.mSignal->type() != mSignalSetter.mSigData->type()){
            dbgSystem.addMessage("Data signal is not of the same type as the signal which is being set!");
            return false;
        }
    }


    /*
    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    if(actor->sourceEntity()->name()=="wallSwitchB"){
        DummyFunction();
    }

    //ObtainSignalFromPath(mValue, scene, actor, mParentBehaviorAction->parentBehaviorState(), mSignalSetter);
    app->signalParserManager()->parseSignalAccessor(_scene, mValue, mSignalSetter, actor, mParentBehaviorAction->parentBehaviorState());

    if(mSignalSetter.isValid()==false){
        return false;
    }

    if(mSignalSetter.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalSetter.mSignal);
        mSignalSetter.mOwnedSignal = true;   // custom signals are owned by signal accessors
    }
    */


    if(mSignalSetter.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        if(parentState){
            parentState->customUpdatedSignals().push_back(mSignalSetter.mSignal);
            //mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalSetter.mSignal);
            mSignalSetter.mOwnedSignal = true;   // custom signals are owned by signal accessors
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


void DoSetSignalB::run(UpdateMode &_updateMode)
{


    mSignalSetter.set();


}


/*

std::string AResetAction::type = "resetAction";


AResetAction::AResetAction(Action*_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mActionPath = _node.attribute("action").as_string("");

}


bool AResetAction::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mAction = ObtainActionFromPath(_scene, mParentGameAction, mActionPath);

    if(mAction==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void AResetAction::execute()
{

    mAction->reset();

}

*/


//---------------------------------------------------------------------------

/*
std::string CSetSystemParameter::type = "setSystemParameter";


CSetSystemParameter::CSetSystemParameter(Action*_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mParameterName = _node.attribute("parameter").as_string("");
    mWidgetName = _node.attribute("valueFromWidget").as_string("");

}


CSetSystemParameter::CSetSystemParameter(Action*_parentGameAction, const std::string &_value)
{
    mParentGameAction = _parentGameAction;
    mValue = _value;

}


bool CSetSystemParameter::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mSystemParameter = GetSystemParameterFromString(mParameterName);

    if(mSystemParameter==SystemParameter::NOT_DEFINED){
        dbgSystem.addMessage("Wrong system parameter '"+ mParameterName + "'!");
        return false;
    }

    app->systemSettings()->appendToUsedSystemParameters(mSystemParameter);

    //---
    mWidget = ObtainGuiWidget(_scene, mWidgetName, WidgetType::NOT_DEFINED);
    if(mWidget==nullptr){
        return false;
    }

    //--- verify that the widget is of the right type
    if(mSystemParameter==SystemParameter::FULL_SCREEN ||
        mSystemParameter==SystemParameter::MUSIC_ACTIVE ||
        mSystemParameter==SystemParameter::SFX_ACTIVE ||
        mSystemParameter==SystemParameter::SPEECH_ACTIVE)
    {
        if(mWidget->type()!=WidgetType::BUTTON){
            dbgSystem.addMessage("The widget '" + mWidgetName + "' is of type '" + GetGuiWidgetKindString(mWidget->type()) + "' but it must be '" + GetGuiWidgetKindString(WidgetType::BUTTON) + "'");
            return false;

        }else if(static_cast<GuiButton*>(mWidget)->IsCheckable()==false){
            dbgSystem.addMessage("The button '" + mWidgetName + "' must be checkable!");
            return false;
        }

        GuiButton *b = static_cast<GuiButton*>(mWidget);

        if(mSystemParameter==SystemParameter::FULL_SCREEN){
            b->SetChecked(app->systemSettings()->isFullScreen());

        }else if(mSystemParameter==SystemParameter::MUSIC_ACTIVE){
            b->SetChecked(app->systemSettings()->isMusicActive());

        }else if(mSystemParameter==SystemParameter::SFX_ACTIVE){
            b->SetChecked(app->systemSettings()->isSfxActive());

        }else if(mSystemParameter==SystemParameter::SPEECH_ACTIVE){
            b->SetChecked(app->systemSettings()->isSpeechActive());

        }

    }else if(mSystemParameter==SystemParameter::MUSIC_VOLUME ||
             mSystemParameter==SystemParameter::SFX_VOLUME ||
             mSystemParameter==SystemParameter::SPEECH_VOLUME)
    {
        if(mWidget->type()!=WidgetType::SLIDER){
            dbgSystem.addMessage("The widget '" + mWidgetName + "' is of type '" + GetGuiWidgetKindString(mWidget->type()) + "' but it must be '" + GetGuiWidgetKindString(WidgetType::SLIDER) + "'");
            return false;
        }

        GuiSlider *s = static_cast<GuiSlider*>(mWidget);
        s->SetValueMin(0);
        s->SetValueMax(100);

        if(mSystemParameter==SystemParameter::MUSIC_VOLUME){
            s->SetValue(app->systemSettings()->musicVolume());

        }else if(mSystemParameter==SystemParameter::SFX_VOLUME){
            s->SetValue(app->systemSettings()->sfxVolume());

        }else if(mSystemParameter==SystemParameter::SPEECH_VOLUME){
            s->SetValue(app->systemSettings()->speechVolume());

        }
    }

    dbgSystem.removeLastMessage();
    return true;
}


void CSetSystemParameter::execute()
{

    if(mSystemParameter==SystemParameter::FULL_SCREEN){
        GuiButton *w = static_cast<GuiButton*>(mWidget);
        app->systemSettings()->setFullScreen(w->IsChecked());

    }else if(mSystemParameter==SystemParameter::MUSIC_ACTIVE){
        GuiButton *w = static_cast<GuiButton*>(mWidget);
        app->systemSettings()->setMusicActive(w->IsChecked());

    }else if(mSystemParameter==SystemParameter::SFX_ACTIVE){
        GuiButton *w = static_cast<GuiButton*>(mWidget);
        app->systemSettings()->setSfxActive(w->IsChecked());

    }else if(mSystemParameter==SystemParameter::SPEECH_ACTIVE){
        GuiButton *w = static_cast<GuiButton*>(mWidget);
        app->systemSettings()->setSpeechActive(w->IsChecked());

    }else if(mSystemParameter==SystemParameter::MUSIC_VOLUME){
        GuiSlider *w = static_cast<GuiSlider*>(mWidget);
        app->systemSettings()->setMusicVolume(w->GetValue());

    }else if(mSystemParameter==SystemParameter::SFX_VOLUME){
        GuiSlider *w = static_cast<GuiSlider*>(mWidget);
        app->systemSettings()->setSfxVolume(w->GetValue());

    }else if(mSystemParameter==SystemParameter::SPEECH_VOLUME){
        GuiSlider *w = static_cast<GuiSlider*>(mWidget);
        app->systemSettings()->setSpeechVolume(w->GetValue());
    }

    app->delayedTaskManager()->addDelayedTask(DelayedTask(DelayedTask::Task::SAVE_SETTINGS_INI_FILE, 4));

}

*/

//----------------------------------------------------------------------------

/*
std::string ASetGlobalVariable::typeGlobal = "setGlobalVariable";
std::string ASetGlobalVariable::typeScene = "setSceneVariable";
std::string ASetGlobalVariable::typeEntity = "setEntityVariable";


ASetGlobalVariable::ASetGlobalVariable(Action*_parentGameAction, const pugi::xml_node &_node, VariableOrigin _variableOrigin)
{

    mParentGameAction = _parentGameAction;
    mVariableName = _node.attribute("variable").as_string("");
    mVariableValue = _node.attribute("value").as_string("");
    //mVariableTypeString = _node.attribute("type").as_string("");
    mVariableOrigin = _variableOrigin;

}


bool ASetGlobalVariable::initConnections(PlayedScene *_scene)
{

    if(mVariableOrigin==VariableOrigin::GLOBAL){
        dbgSystem.addMessage("init '" + typeGlobal + "'");

        mVariable = app->globalVariables()->getVariable(mVariableName);


    }else if(mVariableOrigin==VariableOrigin::SCENE){
        dbgSystem.addMessage("init '" + typeScene + "'");

        mVariable = _scene->variables().getVariable(mVariableName);


    }else if(mVariableOrigin==VariableOrigin::ENTITY){
        dbgSystem.addMessage("init '" + typeEntity + "'");

        assert(false);
    }


    if(mVariable==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}



void ASetGlobalVariable::execute()
{

    if(mVariable->type()==VariableType::INTEGER){
        static_cast<IntVar*>(mVariable)->setValue(StdString::stringToInt(mVariableValue, 0));

    }else if(mVariable->type()==VariableType::FLOAT){
        static_cast<FloatVar*>(mVariable)->setValue(StdString::stringToFloat(mVariableValue, 0.0f));

    }else if(mVariable->type()==VariableType::BOOL){
        static_cast<BoolVar*>(mVariable)->setValue(StdString::stringToBool(mVariableValue));

    }if(mVariable->type()==VariableType::STRING){
        static_cast<StringVar*>(mVariable)->setValue(mVariableValue);

    }

}

*/


//----------------------------------------------------------------------------

/*

std::string CActivateTrigger::type = "activateTrigger";


CActivateTrigger::CActivateTrigger(Action*_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mTriggerName = _node.attribute("trigger").as_string("");

}


bool CActivateTrigger::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mTrigger = ObtainTrigger(mParentGameAction, mTriggerName);

    if(mTrigger==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void CActivateTrigger::execute()
{

    mTrigger->doSetTriggered();
    mParentGameAction->parentPlayerScene()->activatedTriggersUpdater().addTrigger(mTrigger);

}

*/

//----------------------------------------------------------------------------

/*

std::string APlayScene::type = "playScene";


APlayScene::APlayScene(Action*_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mScenePath = _node.attribute("scene").as_string("");

}


bool APlayScene::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");


    std::string directValue;
    mVarSceneName = ObtainStringVariableFromPath(mScenePath,  _scene, directValue);

    if(directValue.empty() && mVarSceneName==nullptr){
        return false;
    }
    if(mVarSceneName==nullptr){
        mVarSceneName = static_cast<StringVar*>(_scene->localVariablesStorage().addAnonymousVariable(new StringVar("", directValue)));
    }


    dbgSystem.removeLastMessage();
    return true;

}


void APlayScene::execute()
{

    print(" Scene to play: " + mVarSceneName->value());


}

*/

//----------------------------------------------------------------------------

/*


std::string ADoTransition::type = "doTransition";


ADoTransition::ADoTransition(Action*_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mTransitionName = _node.attribute("transition").as_string("");
    mExclusive = _node.attribute("exclusive").as_bool("true");
    std::string show = _node.attribute("show").as_string("");
    mShownObjectsNames = StdString::splitString(show, ",");
    std::string hide = _node.attribute("hide").as_string("");
    mHiddenObjectsNames = StdString::splitString(hide, ",");

}


ADoTransition::ADoTransition(const std::string &_transition, const std::string &_show, const std::string &_hide)
{
    mTransitionName = _transition;
    mShownObjectsNames = StdString::splitString(_show, ",");
    mHiddenObjectsNames = StdString::splitString(_hide, ",");

}


bool ADoTransition::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mTransition = _scene->transitionsGroup()->getTransition(mTransitionName);

    if(mTransition==nullptr){
        return false;
    }


    //--- obtain transitioned objects which will be shown and objects which will be hidden
    if(mExclusive){

        // one object is defined to be shown or hidden, the other objects are oposite

        if(mShownObjectsNames.empty()==false){
            const std::string &trObjectName =  mShownObjectsNames.front();

            for(TransitionMember *to : mTransition->transitionedObjects()){
                if(to->name()==trObjectName){
                    //to->setTransitionType(TransitionType::IN);
                    mShownMembers.push_back(to);

                }else{
                    //to->setTransitionType(TransitionType::OUT);
                    mHiddenMembers.push_back(to);
                }
            }


        }else if(mHiddenObjectsNames.empty()==false){
            const std::string &trObjectName =  mHiddenObjectsNames.front();

            for(TransitionMember *to : mTransition->transitionedObjects()){
                if(to->name()==trObjectName){
                   // to->setTransitionType(TransitionType::OUT);
                    mHiddenMembers.push_back(to);

                }else{
                    //to->setTransitionType(TransitionType::IN);
                    mShownMembers.push_back(to);
                }
            }
        }

    }else{

        // each object is defined manually to be shown or hidden

        for(const std::string &s : mShownObjectsNames){
            TransitionMember *to = mTransition->getObjectTransition(s);
            if(to==nullptr){
                return false;
            }
            //to->setTransitionType(TransitionType::IN);
            mShownMembers.push_back(to);
        }

        for(const std::string &s : mHiddenObjectsNames){
            TransitionMember *to = mTransition->getObjectTransition(s);
            if(to==nullptr){
                return false;
            }
            //to->setTransitionType(TransitionType::OUT);
            mHiddenMembers.push_back(to);
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


void ADoTransition::execute()
{

    mStatus = ActionStatus::IDLE;
    int statusFlag = static_cast<int>(ActionStatus::IDLE);

    for(TransitionMember *to : mShownMembers){
        to->execute(TransitionType::IN);
        statusFlag |= static_cast<int>(to->status());
    }
    for(TransitionMember *to : mHiddenMembers){
        to->execute(TransitionType::OUT);
        statusFlag |= static_cast<int>(to->status());
    }

    if(statusFlag & static_cast<int>(ActionStatus::STARTED)){
        mStatus = ActionStatus::STARTED;
    }


}


void ADoTransition::update(UpdateMode _updateMode)
{

    if(mStatus==ActionStatus::IDLE){
        return;
    }

    //--- trace status of transitions
    mStatus = ActionStatus::STARTED;


    int statusFlag = static_cast<int>(ActionStatus::IDLE);

    for(TransitionMember *to : mShownMembers){
        to->update();
        statusFlag |= static_cast<int>(to->status());
    }
    for(TransitionMember *to : mHiddenMembers){
        to->update();
        statusFlag |= static_cast<int>(to->status());
    }

    if(statusFlag & static_cast<int>(ActionStatus::ACTIVE)){
        mStatus = ActionStatus::ACTIVE;

    }else if(statusFlag & static_cast<int>(ActionStatus::FINISHED)){
        mStatus = ActionStatus::FINISHED;

    }else if(statusFlag == static_cast<int>(ActionStatus::IDLE)){
        mStatus = ActionStatus::IDLE;

    }

}

*/

//---------------------------------------------------------------------------

/*

std::string ASetGfxObject::type = "setGfxObject";



ASetGfxObject::ASetGfxObject(Action*_parentGameAction, const pugi::xml_node &node)
{

    mParentGameAction = _parentGameAction;
    mCfg.reset(new Cfg());

    mCfg->mGfxObject = node.attribute("gfxObject").as_string("");
    mCfg->mVisible = node.attribute("visible").as_string("");
    mCfg->mAlpha = node.attribute("alpha").as_string("");
    mCfg->mColor = node.attribute("color").as_string("");
    mCfg->mText = node.attribute("text").as_string("");
}


bool ASetGfxObject::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");


    mGfxObject = _scene->gfxObjectsGroup()->getGfxObject(mCfg->mGfxObject);
    if(mGfxObject==nullptr){
        return false;
    }

    bool error= false;

    if(mCfg->mVisible.empty()==false){
        bool directValue;

        mVisible = ObtainBoolVariableFromPath(mCfg->mVisible, _scene, directValue, error);
        if(error){
            return false;
        }
        if(mVisible==nullptr){
            mVisible = static_cast<BoolVar*>(_scene->localVariablesStorage().addAnonymousVariable(new BoolVar("", directValue)));
        }

        mProperties |= static_cast<int>(Property::VISIBLE);
    }

    //---
    if(mCfg->mAlpha.empty()==false){
        float directValue;

        mAlpha = ObtainFloatVariableFromPath(mCfg->mAlpha, _scene, directValue, error);
        if(error){
            return false;
        }
        if(mAlpha==nullptr){
            mAlpha = static_cast<FloatVar*>(_scene->localVariablesStorage().addAnonymousVariable(new FloatVar("", directValue)));
        }

        mProperties |= static_cast<int>(Property::ALPHA);
    }

    //---
    if(mCfg->mColor.empty()==false){

        // TO DO !
    }

    //---
    if(mCfg->mText.empty()==false){
        std::string directValue;

        mText = ObtainStringVariableFromPath(mCfg->mText, _scene, directValue);
        if(directValue.empty() && mText==nullptr){
            return false;
        }
        if(mText==nullptr){
            mText = static_cast<StringVar*>(_scene->localVariablesStorage().addAnonymousVariable(new StringVar("", directValue)));
        }

        mProperties |= static_cast<int>(Property::TEXT);
    }



    dbgSystem.removeLastMessage();

    return true;

}


void ASetGfxObject::execute()
{

    if(mGfxObject->baseType()==BaseObjectType::GFX_OBJECT__SPRITES){
        GSpritesObject *spritesObject = static_cast<GSpritesObject*>(mGfxObject);

        if(spritesObject->objectType()==GfxObjectType::SPRITE){

            for(void* obj : spritesObject->objects()){
                Sprite * sprite = static_cast<Sprite*>(obj);

                if(mProperties & static_cast<int>(Property::ALPHA)){
                    sprite->setAlpha(mAlpha->value());

                }else if(mProperties & static_cast<int>(Property::VISIBLE)){
                    sprite->setVisible(mVisible->value());

                }else if(mProperties & static_cast<int>(Property::COLOR)){

                }
            }

        }else if(spritesObject->objectType()==GfxObjectType::SPRITE_LAYER){

            for(void* obj : spritesObject->objects()){
                SpriteLayer * spriteLayer = static_cast<SpriteLayer*>(obj);

                if(mProperties & static_cast<int>(Property::ALPHA)){
                    spriteLayer->setAlpha(mAlpha->value());

                }else if(mProperties & static_cast<int>(Property::VISIBLE)){
                    spriteLayer->setVisible(mVisible->value());

                }else if(mProperties & static_cast<int>(Property::COLOR)){

                }
            }
        }


    }else if(mGfxObject->baseType()==BaseObjectType::GFX_OBJECT__TEXT_SPRITE){
        GTextSprite *textSpriteObject = static_cast<GTextSprite*>(mGfxObject);

        if(mProperties & static_cast<int>(Property::ALPHA)){
            textSpriteObject->textSprite()->setAlpha(mAlpha->value());

        }else if(mProperties & static_cast<int>(Property::VISIBLE)){
            textSpriteObject->textSprite()->setVisible(mVisible->value());

        }else if(mProperties & static_cast<int>(Property::COLOR)){


        }else if(mProperties & static_cast<int>(Property::TEXT)){
            textSpriteObject->textSprite()->setText(mText->value());
        }
    }

}

*/



//---------------------------------------------------------------------------

/*

std::string AShowOverlayComponent::type = "showOverlayComponent";


AShowOverlayComponent::AShowOverlayComponent(Action*_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
    mCfg.reset(new Cfg());

    mCfg->mComponent = _node.attribute("component").as_string("");
    mModal = _node.attribute("modal").as_bool(true);
    mCfg->mTransition = _node.attribute("transition").as_string("");
    mCfg->mShownGfxObject = _node.attribute("show").as_string("");


    //---
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="data"){
            mCfg->mTextSegment = n.attribute("textSegment").as_string("");
        }
    }
}


bool AShowOverlayComponent::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mComponent = dynamic_cast<OverlayComponent*>(_scene->overlayComponentsGroup()->getComponent(mCfg->mComponent));
    if(mComponent==nullptr){
        return false;
    }

    mAOpenTransition.reset(new ADoTransition(mCfg->mTransition, mCfg->mShownGfxObject, ""));
    mACloseTransition.reset(new ADoTransition(mCfg->mTransition, "", mCfg->mShownGfxObject));

    //---
    if(mAOpenTransition->initConnections(_scene)==false){
        return false;
    }

    //---
    if(mACloseTransition->initConnections(_scene)==false){
        return false;
    }

    //DATA
    if(mCfg->mTextSegment.empty()==false){
        TextSegment *ts = ObtainTextSegment(mCfg->mTextSegment);
        if(ts==nullptr){
            return false;
        }

        if(mComponentData.get()==nullptr){
            mComponentData.reset(new ComponentData());
        }

        mComponentData->mTextSegments.push_back(ts);
    }

    dbgSystem.removeLastMessage();
    return true;

}


void AShowOverlayComponent::execute()
{

    mComponent->setTransitions(mAOpenTransition.get(), mACloseTransition.get());
    mComponent->_setModal(mModal);
    mComponent->setData(mComponentData.get());
    mParentGameAction->parentPlayerScene()->startOverlayComponent(mComponent);

}

*/

//---------------------------------------------------------------------------


//std::string AShowOverlayCompound::type = "showOverlayCompound";
std::string AShowOverlayCompound::doType = "doShowOverlayCompound";


/*
AShowOverlayCompound::AShowOverlayCompound(Action*_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mCfg.reset(new Cfg());

    mCfg->mCompound = _node.attribute("compound").as_string("");

    //---
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="data"){
            mCfg->mTextSegment = n.attribute("textSegment").as_string("");
        }
    }
}
*/

AShowOverlayCompound::AShowOverlayCompound(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool AShowOverlayCompound::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");


    if(mValue.empty()==false){


        std::vector<std::string>parts = StdString::splitString(mValue, "/");
        if(parts.size() < 1){
            dbgSystem.addMessage("Parsing string '" +mValue + "' error!");
            return false;
        }

        std::string compound;
        std::string data;

        if(parts.size() > 0){
            compound = parts[0];
        }
        if(parts.size() > 1){
            data = parts[1];
        }

        if(compound.empty()){
            dbgSystem.addMessage("Parsing string '" +mValue + "' error!");
            return false;
        }

        mCompound = _scene->compoundStorage()->getCompund(compound);
        if(mCompound==nullptr){
            return false;
        }

        if(mCompound->overlay()==false){
            dbgSystem.addMessage("Compound '" + mCompound->name() + "' is not an overlay compound!");
            return false;
        }

        //---
        if(data.empty()==false){
            std::vector<std::string>dataElements = StdString::splitString(data, ",");

            for(const std::string &dataElement : dataElements){

                if(dataElement.size()>14 && dataElement.substr(0,13)=="TEXT_SEGMENT:"){

                    std::string dataContent = dataElement.substr(13);

                    TextSegment *ts = ObtainTextSegment2(dataContent);
                    if(ts==nullptr){
                        return false;
                    }

                    if(mComponentData.get()==nullptr){
                        mComponentData.reset(new ComponentData());
                    }

                    mComponentData->mTextSegments.push_back(ts);
                }

            }
        }



    }else{

        mCompound = _scene->compoundStorage()->getCompund(mCfg->mCompound);
        if(mCompound==nullptr){
            return false;
        }

        if(mCompound->overlay()==false){
            dbgSystem.addMessage("Compound '" + mCompound->name() + "' is not an overlay compound!");
            return false;
        }


        //DATA
        if(mCfg->mTextSegment.empty()==false){
            TextSegment *ts = ObtainTextSegment(mCfg->mTextSegment);
            if(ts==nullptr){
                return false;
            }

            if(mComponentData.get()==nullptr){
                mComponentData.reset(new ComponentData());
            }

            mComponentData->mTextSegments.push_back(ts);
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}



void AShowOverlayCompound::run(UpdateMode &_updateMode)
{

    LogicState *currentState =  static_cast<LogicState*>(mParentAction->parentObject());

    mCompound->component()->setData(mComponentData.get());

    SceneLogicStateData *data = dynamic_cast<SceneLogicStateData*>(currentState->customState());

    if(data){
        mParentAction->parentPlayerScene()->startOverlayCompound(data->compound(), mCompound);
    }

}



//---------------------------------------------------------------------------



std::string ADbgPrint::doType = "doDbgPrint";



ADbgPrint::ADbgPrint(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool ADbgPrint::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");

    mText = mValue;

    dbgSystem.removeLastMessage();
    return true;

}




void ADbgPrint::run(UpdateMode &_updateMode)
{
    print(mText);
}




//---------------------------------------------------------------------------


//std::string AQuitGame::type = "quitGame";
std::string AQuitGame::doType = "doQuitGame";

/*
AQuitGame::AQuitGame(Action*_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;

}
*/

AQuitGame::AQuitGame(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}



bool AQuitGame::initConnections(PlayedScene *_scene)
{
    return true;
}



void AQuitGame::run(UpdateMode &_updateMode)
{
    ncine::theApplication().quit();
}



}
