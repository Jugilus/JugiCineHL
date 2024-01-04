#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
#include "jmCommonFunctions.h"
#include "jmSceneLayout.h"
#include "jmMap.h"
#include "jmUtilities.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"

#include "jpTransition.h"
#include "jpTransitionOperations.h"

#include "jpGuiSystem.h"
#include "jpComponentsCommon.h"
#include "jpGfxObjectsCommon.h"
#include "jpCompound.h"



namespace jugimap{


/*

CompoundCfg::Transition::~Transition()
{

    for(TransitionOpCfg* cfg : transitionOps){
        delete cfg;
    }

}
*/

//--------------------------------------------------------------------------



bool CompoundCfg::initCfg(PlayedScene *_scene, const pugi::xml_node &_node)
{


    dbgSystem.addMessage("Loading compound '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="overlay"){
                    overlay = a.as_bool(false);

                }else if(attributeName=="modal"){
                    modal = a.as_bool(false);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }

            }

        }else if(nodeName=="gfxObject"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="map"){
                    gfxObject_map = a.as_string();

                }else if(attributeName=="spriteLayer"){
                    gfxObject_spriteLayer = a.as_string();

                }else if(attributeName=="sprite"){
                    gfxObject_sprite = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }

            }


        }else if(nodeName=="logicObjects"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="component"){
                    component = a.as_string();

                //}else if(attributeName=="overlayComponent"){
                //    overlayComponent = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="widgets"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){

                std::string widgetSignature = std::string(a.name());
                std::string sWidgetNames = a.as_string();

                std::vector<std::string>widgetNames = StdString::splitString(sWidgetNames, ",");

                for(std::string &name : widgetNames){
                    widgets.push_back(WidgetName(widgetSignature, name));
                }
            }

        }else if(nodeName=="defaultTransition"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                defaultTransition.transitionOps.push_back(TransitionOpCfg());
                TransitionOpCfg &transitionCfg = defaultTransition.transitionOps.back();

                if(transitionCfg.initCfg(nChild)==false){
                    return false;
                }
            }


        }else if(nodeName=="transition"){

            transitions.push_back(Transition());

            Transition &transition = transitions.back();

            transition.otherCompound = n.attribute("otherCompound").as_string();
            if(transition.otherCompound.empty()){
                dbgSystem.addMessage("Missing attribute 'otherCompound' in node '" + nodeName + "'!");
                return false;
            }

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                transition.transitionOps.push_back(TransitionOpCfg());
                TransitionOpCfg &transitionCfg = transition.transitionOps.back();

                if(transitionCfg.initCfg(nChild)==false){
                    return false;
                }
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    if(gfxObject_map.empty()){
        dbgSystem.addMessage("Missing 'map' attribute in 'gfxObject' node!");
        return false;
    }
    if(gfxObject_sprite.empty() && gfxObject_spriteLayer.empty()){
        dbgSystem.addMessage("Missing 'spriteLayer' or 'sprite' attribute in 'gfxObject' node!");
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}



//CompoundTransitionMember::~CompoundTransitionMember()
//{
//    delete transitionMember;
//}


//---------------------------------------------------------------------------


Compound::Compound(CompoundCfg *_cfg) : mCfg(_cfg)
{
    mBaseType = BaseObjectType::COMPOUND;

}


Compound::~Compound()
{
    //if(mData){
    //    delete mData;
    //}

    //---
    for(CompoundTransitionMember &ctm : mCompoundTransitionMembers){
        delete ctm.transitionMember;
    }

}



bool Compound::build(PlayedScene *_scene)
{

    dbgSystem.addMessage("Building compound '" + mCfg->name + "'...");

    mScene = _scene;
    mName = mCfg->name;


    //--- GFX OBJECT
    std::vector<void*>objects;
    GSpritesObjectType gfxObjectType = GSpritesObjectType::NOT_DEFINED;
    std::string objectName;

    if(mCfg->gfxObject_spriteLayer.empty()==false){
        objectName = mCfg->gfxObject_spriteLayer;
        gfxObjectType = GSpritesObjectType::SPRITE_LAYER;

    }else if(mCfg->gfxObject_sprite.empty()==false){
        objectName = mCfg->gfxObject_sprite;
        gfxObjectType = GSpritesObjectType::SPRITE;
    }

    if(collectObjects(objects, _scene, gfxObjectType, objectName, mCfg->gfxObject_map)==false){
        return false;
    }

    mGfxObject.reset(new GSpritesObject(objects, gfxObjectType));
    mGfxObject->setVisible(false);
    //mGfxObject = new GSpritesObject(objects, gfxObjectType);


    //--- TRANSITIONS
    for(CompoundCfg::Transition &t : mCfg->transitions){

        mCompoundTransitionMembers.push_back(CompoundTransitionMember());
        CompoundTransitionMember &ctm = mCompoundTransitionMembers.back();
        ctm.transitionMember = new TransitionMember();
        ctm.otherCompound = t.otherCompound;

        ctm.transitionMember->setSpritesObject(mGfxObject.get());
        //ctm.transitionMember->setSpritesObject(mGfxObject);

        if(buildTransitionOps(ctm.transitionMember, t.transitionOps)==false){
            return false;
        }
    }

    mDefaultTransitionMember.reset(new TransitionMember());
    mDefaultTransitionMember->setSpritesObject(mGfxObject.get());
    if(buildTransitionOps(mDefaultTransitionMember.get(), mCfg->defaultTransition.transitionOps)==false){
        return false;
    }


    //---
    dbgSystem.removeLastMessage();
    return true;

}


bool Compound::buildTransitionOps(TransitionMember* _tm, std::vector<TransitionOpCfg> & _transitionOps)
{

    for(TransitionOpCfg &c : _transitionOps){

        TransitionOp *to = nullptr;

        if(c.transitionOp== "translateIntoScreen"){

            to = new TranslateInOutOfScreen(_tm, &c, TransitionType::IN);

        }else if(c.transitionOp=="translateOutOfScreen"){

            to = new TranslateInOutOfScreen(_tm, &c, TransitionType::OUT);

        }else if(c.transitionOp=="alphaFadeIn"){

            to = new AlphaFadeInOut(_tm, &c, TransitionType::IN);

        }else if(c.transitionOp=="alphaFadeOut"){

            to = new AlphaFadeInOut(_tm, &c, TransitionType::OUT);

        }else if(c.transitionOp=="scaleIn"){

            to = new ScaleInOut(_tm, &c, TransitionType::IN);

        }else if(c.transitionOp=="scaleOut"){

            to = new ScaleInOut(_tm, &c, TransitionType::OUT);

        }

        if(to){
            if(to->transitionType()==TransitionType::IN){
                _tm->transitionsIn().push_back(to);

            }else if(to->transitionType()==TransitionType::OUT){
                _tm->transitionsOut().push_back(to);
            }

        }else{
            dbgSystem.addMessage("Unknown transition operation '" + c.transitionOp +"'!");
            return false;
        }
    }

    return true;
}


bool Compound::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("Initializing compound '" + mCfg->name + "'...");


    //---
    for(CompoundTransitionMember &ctm : mCompoundTransitionMembers){
        if(ctm.transitionMember->initConnections(_scene)==false){
            return false;
        }
    }
    if(mDefaultTransitionMember->initConnections(_scene)==false){
        return false;
    }


    //---
    if(mCfg->component.empty()==false){

        mComponent = _scene->componentsGroup()->getComponent(mCfg->component);
        if(mComponent==nullptr){
            return false;
        }
    }

    //---
    //if(mCfg->overlayComponent.empty()==false){

    //    mComponent = _scene->overlayComponentsGroup()->getComponent(mCfg->overlayComponent);
    //    if(mComponent==nullptr){
    //        return false;
    //    }
    //}


    if(mComponent){
        mComponent->setParentCompound(this);
    }


    /*
    if(mComponent){
        for(GuiWidget* w : mComponent->usedWidgets()){
            //if(StdVector::contains(mUsedWidgets, w)==false){
                mUsedNonComponentWidgets.push_back(w);
            //}
        }
    }
    */

    //---
    if(mCfg->widgets.empty()==false){

        for(CompoundCfg::WidgetName &wn : mCfg->widgets){

            WidgetType type = GetGuiWidgetKindFromString(wn.signature);
            if(type==WidgetType::NOT_DEFINED){
                dbgSystem.addMessage("Unknown widget signature '" + wn.signature +"' !");
                return false;
            }

            GuiWidget *widget = _scene->guiSystem()->findWidget(wn.name, type);
            if(widget==nullptr){
                return false;
            }

            //if(StdVector::contains(mUsedWidgets, widget)==false){
                mUsedNonComponentWidgets.push_back(widget);
            //}
        }
    }



    dbgSystem.removeLastMessage();
    return true;

}


void Compound::start(CompoundTransition *_compoundTransition)
{

    if(_compoundTransition->currentCompound() == _compoundTransition->nextCompound()){
        mState = State::NORMAL;
        return;
    }


    GuiSystem *guiSystem = mScene->guiSystem();

    if(mCfg->modal){
        mScene->guiSystem()->addModalBlockLevelToUsedWidgets();     // mark current used widgets as modal blocked!
    }

    guiSystem->appendToUsedWidgets(mUsedNonComponentWidgets);
    if(mComponent){
        mScene->guiSystem()->appendToUsedWidgets(mComponent->usedWidgets());
    }

    //---
    mCompoundTransition = _compoundTransition;

    mCompoundTransition->start(TransitionType::IN, TransitionType::OUT);

    if(mCompoundTransition->status()==ActionStatus::STARTED){
        mState = State::TRANSITION_IN;
        mScene->addLongRunningAction(this);     // make scene aware that a long action is running
    }else{
        mState = State::NORMAL;
    }

    //---
    if(mComponent){
        mComponent->start();
    }

}


bool Compound::startingPhaseUpdate(CompoundTransition *_compoundTransition)
{

    start(_compoundTransition);

    if(mComponent){
        return mComponent->startingPhaseUpdate();
    }

    return false;
}


void Compound::preUpdate(UpdateMode &_updateMode)
{
    if(mComponent){
        mComponent->preUpdate(_updateMode);
    }
}


void Compound::update(UpdateMode &_updateMode)
{

    if(mState==State::TRANSITION_IN){

        mCompoundTransition->update(_updateMode);

        if(mCompoundTransition->status()==ActionStatus::IDLE){
            mScene->removeLongRunningAction(this);
            mState = State::NORMAL;
        }

    }else if(mState==State::NORMAL){

        if(mComponent){
            mComponent->update(_updateMode);
        }


    }else if(mState==State::TRANSITION_OUT){

        mCompoundTransition->update(_updateMode);

        if(mCompoundTransition->status()==ActionStatus::IDLE){
            close();
        }
    }

}


void Compound::postUpdate(UpdateMode &_updateMode)
{

    if(mComponent){
        mComponent->postUpdate(_updateMode);
    }
}


void Compound::startOutTransition()
{

    mScene->guiSystem()->removeFromUsedWidgets(mUsedNonComponentWidgets);
    if(mComponent){
        mScene->guiSystem()->removeFromUsedWidgets(mComponent->usedWidgets());
    }

    //---

    mCompoundTransition->start(TransitionType::OUT, TransitionType::IN);

    if(mCompoundTransition->status()==ActionStatus::STARTED){
        mState = State::TRANSITION_OUT;
        mScene->addLongRunningAction(this);     // make scene aware that a long action is running

    }else{
        close();
    }

    //---
    if(mComponent){
        mComponent->onStateEnded();
    }
}


void Compound::close()
{

    mScene->removeLongRunningAction(this);      // doesnt matter if there was long running transition or not

    //mScene->guiSystem()->removeFromUsedWidgets(mUsedNonComponentWidgets);
    //if(mComponent){
    //    mScene->guiSystem()->removeFromUsedWidgets(mComponent->usedWidgets());
    //}
    if(mCfg->modal){
        mScene->guiSystem()->removeModalBlockLevelToUsedWidgets();   // remove a modal blocking level!
    }

    //---
    //mTrigger_Closed->doSetTriggered();
    //mScene->activatedTriggersUpdater().addTrigger(mTrigger_Closed);

    //---
    mState = State::CLOSED;
    mCompoundTransition = nullptr;

    mScene->endOverlayCompound(this);

}


void Compound::drawShapes(ShapeDrawer &drawer, int _flags)
{
    if(mComponent){
        mComponent->drawShapes(drawer, _flags);
    }

}


void Compound::onStateEnded()
{

    GuiSystem *guiSystem = dynamic_cast<GuiSystem *>(mScene->componentsGroup()->getComponent("guiSystem", false));  assert(guiSystem);
    guiSystem->removeFromUsedWidgets(mUsedNonComponentWidgets);

    if(mComponent){
        mComponent->onStateEnded();
    }



}



//---------------------------------------------------------------------------


void CompoundTransition::reset()
{
    mStatus = ActionStatus::IDLE;

    mNextCompound = nullptr;
    mCurrentCompound = nullptr;
    mNextCompoundTransitionMember = nullptr;
    mCurrentCompoundTransitionMember = nullptr;

}


void CompoundTransition::init(Compound *_currentCompound, Compound *_nextCompound)
{

    reset();

    if(_currentCompound){

        mCurrentCompound = _currentCompound;

        if(_nextCompound){
            for(CompoundTransitionMember &ctm : _currentCompound->compoundTransitionMembers()){
                if(ctm.otherCompound == _nextCompound->name()){
                    mCurrentCompoundTransitionMember = ctm.transitionMember;
                }
            }
        }
        if(mCurrentCompoundTransitionMember==nullptr){
            if(_nextCompound && _nextCompound->overlay()==false){
                mCurrentCompoundTransitionMember = _currentCompound->defaultTransitionMember();
            }
        }
    }


    if(_nextCompound){

        mNextCompound = _nextCompound;

        if(_currentCompound){
            for(CompoundTransitionMember &ctm : _nextCompound->compoundTransitionMembers()){
                if(ctm.otherCompound == _currentCompound->name()){
                    mNextCompoundTransitionMember = ctm.transitionMember;
                }
            }
        }
        if(mNextCompoundTransitionMember==nullptr){
            mNextCompoundTransitionMember = _nextCompound->defaultTransitionMember();
        }
    }

}


void CompoundTransition::start(TransitionType _nextType, TransitionType _currentType)
{

    mStatus = ActionStatus::IDLE;

    if(mNextCompound == mCurrentCompound){
        return;
    }

    int statusFlag = static_cast<int>(ActionStatus::IDLE);

    if(mNextCompoundTransitionMember){
        mNextCompoundTransitionMember->execute(_nextType);
        statusFlag |= static_cast<int>(mNextCompoundTransitionMember->status());
    }

    if(mCurrentCompoundTransitionMember){
        mCurrentCompoundTransitionMember->execute(_currentType);
        statusFlag |= static_cast<int>(mCurrentCompoundTransitionMember->status());
    }

    if(statusFlag & static_cast<int>(ActionStatus::STARTED)){
        mStatus = ActionStatus::STARTED;
    }

}


void CompoundTransition::update(UpdateMode &_updateMode)
{

    if(mStatus==ActionStatus::IDLE){
        return;
    }

    //--- trace status of transitions
    mStatus = ActionStatus::STARTED;


    int statusFlag = static_cast<int>(ActionStatus::IDLE);

    if(mNextCompoundTransitionMember){
        mNextCompoundTransitionMember->update();
        statusFlag |= static_cast<int>(mNextCompoundTransitionMember->status());
    }

    if(mCurrentCompoundTransitionMember){
        mCurrentCompoundTransitionMember->update();
        statusFlag |= static_cast<int>(mCurrentCompoundTransitionMember->status());
    }

    if(statusFlag & static_cast<int>(ActionStatus::ACTIVE)){
        mStatus = ActionStatus::ACTIVE;

    }else if(statusFlag & static_cast<int>(ActionStatus::FINISHED)){
        mStatus = ActionStatus::FINISHED;

    }else if(statusFlag == static_cast<int>(ActionStatus::IDLE)){
        mStatus = ActionStatus::IDLE;

    }

}




//----------------------------------------------------------------------------


CompoundStorage::~CompoundStorage()
{

    for(Compound *c : mCompounds){
        delete c;
    }

}


bool CompoundStorage::initCfg(PlayedScene * _scene, const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading compounds ...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="compound"){

            mCompoundCfgs.push_back(CompoundCfg());

            CompoundCfg &cfg = mCompoundCfgs.back();

            cfg.name = n.attribute("name").as_string();

            if(cfg.name.empty()){
                dbgSystem.addMessage("No name specified for a compound!");
                return false;
            }

            if(cfg.initCfg(_scene, n)==false){
                return false;
            }
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}




bool CompoundStorage::buildObjects(PlayedScene *_scene)
{

    dbgSystem.addMessage("Building components...");


    for(CompoundCfg &cfg : mCompoundCfgs){
        Compound *c = new Compound(&cfg);

        if(c->build(_scene)==false){
            return false;
        }

        mCompounds.push_back(c);

    }

    //---
    dbgSystem.removeLastMessage();
    return true;
}



bool CompoundStorage::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("Initializing compounds...");


    for(Compound *c : mCompounds){
        if(c->initConnections(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();

    return true;
}


Compound* CompoundStorage::getCompund(const std::string &_name, bool _setErrorMessage)
{

    for(Compound* c : mCompounds){
        if(c->name()==_name){
            return c;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Compound '" + _name +"' not found!");
    }

    return nullptr;

}

//================================================================================================


/*

CompoundFactoryManager::~CompoundFactoryManager()
{

    for(CompoundFactory * f : mFactories){
        delete f;
    }

}


void CompoundFactoryManager::addFactory(CompoundFactory *_factory)
{
    mFactories.push_back(_factory);
}



Compound* CompoundFactoryManager::createCompound(const std::string &_typeSignature, bool _setErrorMessage)
{

    for(CompoundFactory* f : mFactories){
        if(f->typeSignature() == _typeSignature){
            return  f->createCompound();
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' for a compound not recognized!");
    }
    return nullptr;

}

*/



}
