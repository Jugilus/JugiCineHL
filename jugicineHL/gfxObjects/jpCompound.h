#ifndef PLAYER__COMPOUND__H
#define PLAYER__COMPOUND__H


#include <memory>
#include "jpSettings.h"
#include "jpVariables.h"
#include "jpTransitionOperations.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class TextSprite;
class PlayedScene;
class GfxObject;
class GSpritesObject;
class GuiWidget;
class Component;
class TransitionMember;
class ShapeDrawer;
//class CompoundTransition;
class CompoundTransition;




struct CompoundCfg
{

    bool initCfg(PlayedScene *_scene, const pugi::xml_node &_node);

    std::string name;
    bool overlay = false;
    bool modal = false;

    std::string gfxObject_map;
    std::string gfxObject_spriteLayer;
    std::string gfxObject_sprite;

    std::string component;
    //std::string overlayComponent;

    struct WidgetName
    {
        WidgetName(const std::string &_signature, const std::string &_name) : signature(_signature), name(_name){}
        std::string signature;
        std::string name;
    };

    std::vector<WidgetName>widgets;

    //---

    struct Transition
    {
        //~Transition();

        std::string otherCompound;
        std::vector<TransitionOpCfg>transitionOps;
    };

    Transition defaultTransition;
    std::vector<Transition>transitions;



};



struct CompoundTransitionMember
{

    //TransitionMember *compoundA = nullptr;
    //TransitionMember *compoundB = nullptr;

    //~CompoundTransitionMember();

    std::string otherCompound;
    TransitionMember *transitionMember = nullptr;

};



class Compound : public BaseObject
{

public:

    enum class State
    {
        TRANSITION_IN,
        NORMAL,
        TRANSITION_OUT,
        CLOSED

    };


    Compound(CompoundCfg *_cfg);
    virtual ~Compound();

    bool build(PlayedScene* _scene);
    bool initConnections(PlayedScene* _scene);

    void start(CompoundTransition* _compoundTransition);
    bool startingPhaseUpdate(CompoundTransition *_compoundTransition);

    void preUpdate(UpdateMode &_updateMode);
    void update(UpdateMode &_updateMode);
    void postUpdate(UpdateMode &_updateMode);

    void startOutTransition();

    void drawShapes(ShapeDrawer &drawer, int _flags=0);
    void onStateEnded();


    void updateInTransition();
    void updateOutTransition();


    const std::string &name(){ return mName; }
    std::vector<CompoundTransitionMember> &compoundTransitionMembers(){ return mCompoundTransitionMembers; }
    TransitionMember* defaultTransitionMember(){ return mDefaultTransitionMember.get(); }

    Component * component(){ return mComponent; }
    CompoundTransition* compoundTransition(){ return mCompoundTransition; }
    //void setCompoundTransition(TwoCompoundsTransition* _compoundTransition){ mCompoundTransition = _compoundTransition; }


    State state(){ return mState; }
    bool isClosed(){ return mState==State::CLOSED; }

    bool modal(){return mCfg->modal;}
    bool overlay(){ return mCfg->overlay;}

    std::vector<GuiWidget*> &usedNonComponentWidgets(){ return mUsedNonComponentWidgets; }

private:
    CompoundCfg *mCfg = nullptr;                    //LINK
    PlayedScene *mScene = nullptr;
    std::string mName;
    VariableManager mVariables;
    //ComponentData *mData = nullptr;             // LINK ?

    State mState = State::NORMAL;


    std::unique_ptr<GSpritesObject>mGfxObject;
    //GSpritesObject *mGfxObject = nullptr;
    Component *mComponent = nullptr;                    // LINK
    std::vector<GuiWidget*>mUsedNonComponentWidgets;                //LINKS

    std::vector<CompoundTransitionMember>mCompoundTransitionMembers;
    std::unique_ptr<TransitionMember>mDefaultTransitionMember;

    //TransitionMember *mActiveTransitionMember = nullptr;          // LINK
    //TransitionMember *mOutTransitionMember = nullptr;         // LINK

    bool mInitialized = false;

    //Compound *mOtherCompound = nullptr;

    CompoundTransition *mCompoundTransition = nullptr;

    //--- triggers
    //TriggerVariable *mTrigger_Closed = nullptr;          // LINK

    bool buildTransitionOps(TransitionMember* _tm, std::vector<TransitionOpCfg> & _transitionOps);

    void close();

};



class CompoundTransition
{
public:

    void init(Compound *_currentCompound, Compound *_nextCompound);
    void reset();

    //---
    void start(TransitionType _nextType, TransitionType _currentType );
    void update(UpdateMode &_updateMode);

    ActionStatus status(){ return mStatus; }

    Compound* nextCompound(){ return mNextCompound; }
    Compound* currentCompound(){ return mCurrentCompound; }

private:
    Compound *mNextCompound = nullptr;                               // LINKS
    Compound *mCurrentCompound = nullptr;                              // LINKS
    TransitionMember *mNextCompoundTransitionMember = nullptr;       // LINK
    TransitionMember *mCurrentCompoundTransitionMember = nullptr;       // LINK

    ActionStatus mStatus = ActionStatus::IDLE;

};



class CompoundStorage
{
public:

    ~CompoundStorage();

    bool initCfg(PlayedScene * _scene, const pugi::xml_node &_node);
    bool buildObjects(PlayedScene* _scene);
    bool initConnections(PlayedScene* _scene);


    Compound* getCompund(const std::string &_name, bool _setErrorMessage=true);
    std::vector<Compound*> &compounds(){ return mCompounds; }


private:
    std::vector<CompoundCfg>mCompoundCfgs;
    std::vector<Compound*>mCompounds;

};




//------------------------------------------------------------------------------------

/*

class CompoundFactory
{
public:

    virtual ~CompoundFactory(){}


    CompoundFactory(const std::string &_typeSignature) : mTypeSignature(_typeSignature) {}

    const std::string & typeSignature(){ return mTypeSignature;}

    virtual Compound* createCompound() = 0;


protected:
    std::string mTypeSignature;

};


class CompoundFactoryManager
{
public:

    ~CompoundFactoryManager();

    void addFactory(CompoundFactory *_factory);

    Compound* createCompound(const std::string &_typeSignature, bool _setErrorMessage=true);

private:
    std::vector<CompoundFactory*>mFactories;

};


*/



}






#endif
