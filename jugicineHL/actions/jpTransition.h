#ifndef PLAYER__ACTION_TRANSITION_H
#define PLAYER__ACTION_TRANSITION_H


#include <string>
#include <vector>

#include <jmTween.h>
#include "jpActionsCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class PlayedScene;
class Transition;
class TransitionOp;
class GSpritesObject;





enum class TransitionType
{
    IN,
    OUT,
    NOT_DEFINED
};



class TransitionMember
{
public:

    TransitionMember(Transition*_parentTransition, const pugi::xml_node &_node);
    ~TransitionMember();

    bool initCfg(const pugi::xml_node &_node);
    bool init(PlayedScene *_scene);

    void execute(TransitionType _transitionType);
    void update();
    void onTransitionFinished();


    const std::string & name();
    std::vector<void*>& objects();
    GfxObjectType objectType();


    ActionStatus status(){ return mStatus; }

    TransitionType transitionType(){ return mTransitionType; }

    void _setSkipOnTransitionFinished(bool _skip){ mSkipOnTransitionFinished = _skip; }


private:
    Transition *mParentTransition = nullptr;                           // LINK
    //std::vector<void*>mObjects;                                         // LINKS
    //std::string mName;
    GSpritesObject * mSpritesObject = nullptr;

    TransitionType mTransitionType = TransitionType::NOT_DEFINED;

    std::vector<TransitionOp*>mTransitionsIn;                        // OWNED
    std::vector<TransitionOp*>mTransitionsOut;                       // OWNED
    std::vector<TransitionOp*>*mUsedTransitions = nullptr;           // LINK to either mTransitionsIn or mTransitionsOut
    ActionStatus mStatus = ActionStatus::IDLE;

    bool mSkipOnTransitionFinished = false;

    //--- cfg
    //ObjectId mObjectType = ObjectId::NOT_DEFINED;
    //std::string mObjectName;
    //std::string mSrcMapName;
    std::string mSpritesObjectName;


};



class Transition
{
public:
    static std::string type;
    static std::vector<GfxObjectType>allowedObjects;


    Transition(BaseObject* _parent);
    ~Transition();
    bool initCfg(const pugi::xml_node &_node);
    bool init(PlayedScene *_scene);

    //void execute() override;
    //void update();

    const std::string &name(){ return mName; }
    BaseObject* parentObject(){ return mParentObject; }

    std::vector<TransitionMember*> &transitionedObjects(){ return mObjects; }
    TransitionMember* getObjectTransition(const std::string & _name);


private:
    BaseObject* mParentObject = nullptr;
    std::string mName;
    std::vector<TransitionMember*>mObjects;               // OWNED
};



class TransitionsGroup
{
public:

    ~TransitionsGroup();

    bool init(PlayedScene *_scene);
    Transition* getTransition(const std::string &_name);
    std::vector<Transition*> &transitions(){ return mTransitions; }

private:
    std::vector<Transition*>mTransitions;


};




}

#endif // PLAYER__ACTION_TRANSITION_H
