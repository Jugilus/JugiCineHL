#ifndef PLAYER__COMPONENTS__COMMON__H
#define PLAYER__COMPONENTS__COMMON__H


#include <memory>
#include "jpSettings.h"
#include "jpVariables.h"


namespace pugi {
class xml_node;
}


namespace jugimap{


class GuiWidget;
class PlayedScene;
class ADoTransition;
class ComponentData;
class TextSegment;
class Sprite;
class ShapeDrawer;
class Compound;
struct SignalQuery;
struct SignalSetter;

class ParsedSignalPath;



class Component : public BaseObject
{
public:

    Component(){ mBaseType = BaseObjectType::COMPONENT; }
    virtual ~Component();

    virtual bool initCfg(PlayedScene *, const pugi::xml_node &){ return true; }
    virtual bool buildObjects(PlayedScene *_scene){ return true; }
    virtual bool initConnections(PlayedScene* _scene){ return true; }

    virtual void start(){}
    virtual bool startingPhaseUpdate(){ start();  return false; }

    virtual void preUpdate(UpdateMode &_updateMode){}
    virtual void update(UpdateMode &_updateMode){}
    virtual void postUpdate(UpdateMode &_updateMode){}

    virtual void drawShapes(ShapeDrawer &drawer, int _flags=0){}
    virtual void onStateEnded(){}

    bool startCloseTransition();

    const std::string &name(){ return mName; }

    VariableManager &variables(){ return mVariables;}

    void setData(ComponentData *_data){ mData = _data; }
    ComponentData *data(){ return mData; }

    std::vector<GuiWidget*>& usedWidgets(){return mUsedWidgets;}

    bool isInitialized(){ return mInitialized; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    void setParentCompound(Compound* _compound){ mParentCompound = _compound; }
    Compound* parentCompound(){ return mParentCompound;}

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_path, bool _setErrorMessage=true);



protected:
    std::string mName;
    PlayedScene *mParentPlayerScene = nullptr;          // LINK

    VariableManager mVariables;
    ComponentData *mData = nullptr;             // LINK ?

    std::vector<GuiWidget*>mUsedWidgets;

    bool mInitialized = false;

    Compound* mParentCompound = nullptr;              // link


    //--- triggers
    TriggerVariable *mTrigger_Closed = nullptr;          // LINK

};



class ComponentsGroup
{
public:

    ~ComponentsGroup();
    bool buildObjects(PlayedScene* _scene);
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
