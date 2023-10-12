#ifndef PLAYER__COMPONENTS__COMMON__H
#define PLAYER__COMPONENTS__COMMON__H


#include <memory>
#include "jpSettings.h"
#include "jpVariables.h"


namespace pugi {
class xml_node;
}


namespace jugimap{


class PlayedScene;
class ADoTransition;
class ComponentData;
class TextSegment;
class Sprite;
class ShapeDrawer;



class Component : public BaseObject
{
public:

    Component(){ mType = BaseObjectType::COMPONENT; }
    virtual ~Component();

    virtual bool initCfg(PlayedScene *, const pugi::xml_node &){ return true; }
    virtual bool initConnections(PlayedScene* _scene){ return true; }

    virtual void start(){}
    virtual bool startingPhaseUpdate(){ return false; }
    virtual void update(UpdateMode _updateMode){}
    virtual void drawShapes(ShapeDrawer &drawer, int _flags=0){}
    virtual void onStateEnded(){}

    const std::string &name(){ return mName; }

    VariableManager &variables(){ return mVariables;}

    void setData(ComponentData *_data){ mData = _data; }
    ComponentData *data(){ return mData; }


protected:
    std::string mName;
    VariableManager mVariables;
    ComponentData *mData = nullptr;             // LINK ?

};


class OverlayComponent : public Component
{
public:

    enum class State
    {
        TRANSITION_IN,
        NORMAL,
        TRANSITION_OUT,
        CLOSED

    };


    OverlayComponent(){ mType = BaseObjectType::OVERLAY_COMPONENT; }
    virtual ~OverlayComponent(){}

    virtual void update(UpdateMode _updateMode) override;

    bool startOpenTransition();
    bool startCloseTransition();


    State state(){ return mState; }
    bool isClosed(){ return mState==State::CLOSED; }

    void _setModal(bool _modal) { mModal = _modal; }
    bool modal(){ return mModal; }

    void setTransitions(ADoTransition *_openTransition, ADoTransition *_closeTransition);


protected:
    PlayedScene *mParentPlayerScene = nullptr;          // LINK
    bool mModal = false;

    ADoTransition *mAOpenTransition = nullptr;          // LINK
    ADoTransition *mACloseTransition = nullptr;         // LINK

    State mState = State::NORMAL;

    std::vector<GuiWidget*>mUsedWidgets;

    //--- triggers
    TriggerVariable *mTrigger_Closed = nullptr;          // LINK

};


class ComponentsGroup
{
public:

    bool initConnections(PlayedScene* _scene);


    Component* getComponent(const std::string &_name, bool _setErrorMessage=true);
    std::vector<Component*> &components(){ return mComponents; }


private:
    std::vector<Component*>mComponents;

};



class ComponentData
{
public:

    std::vector<TextSegment*>mTextSegments;         // LINKs
    std::vector<Sprite*>mSprites;                   // LINKs

};



//------------------------------------------------------------------------------------


class CustomComponentFactory
{
public:

    virtual ~CustomComponentFactory(){}


    CustomComponentFactory(const std::string &_typeSignature) : mTypeSignature(_typeSignature) {}

    const std::string & typeSignature(){ return mTypeSignature;}

    virtual Component* createComponent() = 0;


protected:
    std::string mTypeSignature;

};


class CustomComponentFactoryManager
{
public:

    ~CustomComponentFactoryManager();

    void addFactory(CustomComponentFactory *_factory);

    Component* createCustomComponent(const std::string &_typeSignature, bool _setErrorMessage=true);

private:
    std::vector<CustomComponentFactory*>mFactories;

};


}



#endif
