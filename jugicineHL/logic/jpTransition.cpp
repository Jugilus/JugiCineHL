#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmSprite.h"
#include "jmSystem.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

#include "jpGfxObjectsCommon.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpObjectFactory.h"
#include "jpTransitionOperations.h"
#include "jpTransition.h"



namespace jugimap{




TransitionMember::TransitionMember(Transition*_parentTransition, const pugi::xml_node &_node)
{

    //mParentTransition = _parentTransition;
    //mName = _node.attribute("name").as_string("");
    //mObjectType = GetObjectTypeFromString(_node.attribute("objectType").as_string(""));
    //mObjectName = _node.attribute("objectName").as_string("");
   // mSrcMapName = _node.attribute("objectSourceMap").as_string("");

    mSpritesObjectName = _node.attribute("spritesObject").as_string("");

}


TransitionMember::~TransitionMember()
{

    for(TransitionOp* t :mTransitionsIn){
        delete t;
    }

    for(TransitionOp* t :mTransitionsOut){
        delete t;
    }


}


bool TransitionMember::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("init TransitionedObject");


    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        // childNodeName is the type of the transition command
        TransitionOp *a = actionFactory->createTransitionInOut(childNodeName, this, nChild);



        if(a){
            if(a->transitionType()==TransitionType::IN){
                mTransitionsIn.push_back(a);
            }else if(a->transitionType()==TransitionType::OUT){
                mTransitionsOut.push_back(a);
            }

        }else{
            dbgSystem.addMessage("Transition command '" + childNodeName +"' not found!");
            return false;
        }
    }

    // If no transitions are defined add visible transition !
    //if()


    dbgSystem.removeLastMessage();
    return true;

}



bool TransitionMember::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing transition member...");


    //if(collectObjects(mObjects, _scene, mObjectType, mObjectName, mSrcMapName)==false){
    //    return false;
    //}

    if(mSpritesObject==nullptr){
        mSpritesObject = dynamic_cast<GSpritesObject*>(_scene->gfxObjectsGroup()->getGfxObject(mSpritesObjectName));
    }
    if(mSpritesObject==nullptr){
        return false;
    }


    for(TransitionOp* t : mTransitionsIn){
        if(t->initConnections(_scene)==false){
            return false;
        }
    }

    for(TransitionOp* t : mTransitionsOut){
        if(t->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


const std::string & TransitionMember::name()
{
    return mSpritesObject->name();
}


std::vector<void*>& TransitionMember::objects()
{
    return mSpritesObject->objects();

}


GfxObjectType TransitionMember::objectType()
{
    return mSpritesObject->objectType();
}



void TransitionMember::execute(TransitionType _transitionType)
{

    mTransitionType = _transitionType;


    mUsedTransitions = nullptr;

    if(mTransitionType==TransitionType::IN && mTransitionsIn.empty()==false){
        mUsedTransitions = &mTransitionsIn;

    }else if(mTransitionType==TransitionType::OUT && mTransitionsOut.empty()==false){
        mUsedTransitions = &mTransitionsOut;
    }

    if(mUsedTransitions){

        int tasksStatus = static_cast<int>(ActionStatus::IDLE);

        for(TransitionOp* t : *mUsedTransitions){
            t->execute();
            tasksStatus |= static_cast<int>(t->status());
        }

        if(tasksStatus & static_cast<int>(ActionStatus::STARTED)){
            mStatus = ActionStatus::STARTED;
        }


    }else{

        std::vector<void*> &objects = mSpritesObject->objects();
        GfxObjectType objectType =  mSpritesObject->objectType();

        // If no transitions are defined we set visible property!

        bool visible = (mTransitionType==TransitionType::OUT)? false : true;

        if(objectType==GfxObjectType::SPRITE){

            for(void* obj : objects){
                Sprite * sprite = static_cast<Sprite*>(obj);
                sprite->setVisible(visible);
            }

        }else if(objectType==GfxObjectType::SPRITE_LAYER){

            for(void* obj : objects){
                SpriteLayer * spriteLayer = static_cast<SpriteLayer*>(obj);
                spriteLayer->setVisible(visible);
                //ensure animated properties does interfere
                //if(visible){
                    for(jugimap::Sprite* s : spriteLayer->sprites()){
                        if(s->animatedProperties()){
                            s->animatedProperties()->Reset();
                            s->updateAllNodeParameters();
                        }
                    }
                //}
            }

        }else if(objectType==GfxObjectType::MAP){

            for(void* obj : objects){
                Map * map = static_cast<Map*>(obj);
                map->setVisible(visible);
            }
        }

        mStatus = ActionStatus::IDLE;

    }
}


void TransitionMember::update()
{

    if(mUsedTransitions==nullptr){
        return;
    }


    //--- trace status of transitions
    mStatus = ActionStatus::IDLE;

    int statusFlag = static_cast<int>(ActionStatus::IDLE);

    for(TransitionOp* t : *mUsedTransitions){
        t->update();

        statusFlag |= static_cast<int>(t->status());
    }

    if(statusFlag & static_cast<int>(ActionStatus::ACTIVE)){
        mStatus = ActionStatus::ACTIVE;

    }else if(statusFlag & static_cast<int>(ActionStatus::FINISHED)){
        mStatus = ActionStatus::FINISHED;
        onTransitionFinished();

    }else if(statusFlag == static_cast<int>(ActionStatus::IDLE)){
        mStatus = ActionStatus::IDLE;

    }


}


void TransitionMember::onTransitionFinished()
{

    if(mSkipOnTransitionFinished) return;

    std::vector<void*> &objects = mSpritesObject->objects();
    GfxObjectType objectType =  mSpritesObject->objectType();

    bool visible = (mTransitionType==TransitionType::IN);


    if(objectType==GfxObjectType::SPRITE){

        for(void* obj : objects){
            Sprite * s = static_cast<Sprite*>(obj);
            s->setVisible(visible);
        }

    }else if(objectType==GfxObjectType::SPRITE_LAYER){

        for(void* obj : objects){
            SpriteLayer * sl = static_cast<SpriteLayer*>(obj);
            sl->setVisible(visible);
        }

    }else if(objectType==GfxObjectType::MAP){

    }

}



//-------------------------------------------------------------------------------------------------


std::string Transition::type = "transition";


Transition::Transition(BaseObject *_parent)
{

    mParentObject = _parent;

}


Transition::~Transition()
{
    for(TransitionMember* to : mObjects){
        delete to;
    }
}


bool Transition::initCfg(const pugi::xml_node &_node)
{

    mName = _node.attribute("name").as_string("");

    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        if(childNodeName=="member"){

            TransitionMember * to = new TransitionMember(this, nChild);
            if(to->initCfg(nChild)==false){
                delete to;
                return false;
            }
            mObjects.push_back(to);
        }
    }

    return true;
}



bool Transition::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing transition '" + mName +"'");

    for(TransitionMember* tm : mObjects){
        if(tm->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



/*
void ATransition::execute()
{

    int tasksStatus = static_cast<int>(ActionStatus::NOT_DEFINED);

    for(TransitionedObject* to : mObjects){
        to->execute();
        tasksStatus |= static_cast<int>(t->status());
    }

    if(tasksStatus & static_cast<int>(ActionStatus::STARTED)){
        mStatus = ActionStatus::STARTED;
    }

}
*/

/*
void Transition::update()
{

    for(GraphicObjectTransition* to : mObjects){
        to->update();
    }

}
*/

TransitionMember* Transition::getObjectTransition(const std::string &_name)
{

    for(TransitionMember *to : mObjects){
        if(to->name()==_name){
            return to;
        }
    }

    dbgSystem.addMessage("Graphic object transition'" + _name + "' not found!");

    return nullptr;

}


//-------------------------------------------------------------------------------------------------


TransitionsGroup::~TransitionsGroup()
{
    for(Transition* a : mTransitions){
        delete a;
    }
}


bool TransitionsGroup::initConnections(PlayedScene *_scene)
{


    for(Transition* t : mTransitions){
        if(t->initConnections(_scene)==false){
            return false;
        }
    }

    return true;
}


Transition* TransitionsGroup::getTransition(const std::string &_name)
{

    for(Transition* a : mTransitions){
        if(a->name()==_name){
            return a;
        }
    }

    dbgSystem.addMessage("Transition '" + _name + "' not found!");

    return nullptr;
}





}
