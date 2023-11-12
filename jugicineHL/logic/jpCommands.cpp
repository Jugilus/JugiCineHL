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
#include "gui/widgets/jpGuiTable.h"
//#include "gui/widgets/jpGuiSlider.h"

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
#include "jpCommands.h"



namespace jugimap{




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
    if(mSignalSetter.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        if(parentState){
            parentState->customUpdatedSignals().push_back(mSignalSetter.mSignal);
            //mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalSetter.mSignal);
            mSignalSetter.mOwnedSignal = true;   // custom signals are owned by signal accessors
        }
    }
    */

    dbgSystem.removeLastMessage();
    return true;

}


void DoSetSignalB::run(UpdateMode &_updateMode)
{


    mSignalSetter.set();


}


//---------------------------------------------------------------------------


std::string AShowOverlayCompound::doType = "doShowOverlayCompound";


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

    SceneCustomLogicState *data = dynamic_cast<SceneCustomLogicState*>(currentState->customState());

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
