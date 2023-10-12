#ifndef PLAYER__ACTION_COMMANDS_LOGIC_H
#define PLAYER__ACTION_COMMANDS_LOGIC_H

#include <memory>
#include <string>
#include <vector>

#include <jmTween.h>
#include "jmColor.h"
#include "jpVariables.h"
#include "jpActionsCommon.h"




namespace pugi{
class xml_node;
}

namespace jugimap{


class Transition;
class TransitionMember;
class Value;
//class Parameter;
class StringParameter;
class PlayedScene;
class Scene;
class ItemsGroup;
class GfxObject;

class IntVar;
class FloatVar;
class BoolVar;
class StringVar;
class Variable;
class TriggerVariable;
class OverlayComponent;
class ComponentData;





class SetActiveState : public ActionCommand
{
public:
    static std::string type;


    SetActiveState(Action*_parentGameAction, const pugi::xml_node &node);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;


private:
    LogicState *mGameState = nullptr;           // link

    //--- cfg parameters
    std::string mStatePath;

};


//----------------------------------------------------------------------------------


class AResetAction : public ActionCommand
{
public:
    static std::string type;

    AResetAction(Action*_parentGameAction, const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;


private:
    Action *mAction = nullptr;

    //--- cfg parameters
    std::string mActionPath;
};



//----------------------------------------------------------------------------------


class CSetSystemParameter : public ActionCommand
{
public:
    static std::string type;

    CSetSystemParameter(Action*_parentGameAction, const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;

private:
    GuiWidget *mWidget = nullptr;               // LINK to widget which provide value for command
    SystemParameter mSystemParameter = SystemParameter::NOT_DEFINED;

    //--- cfg parameters
    std::string mParameterName;
    std::string mWidgetName;

};



//----------------------------------------------------------------------------------



class ASetGlobalVariable : public ActionCommand
{
public:

    static std::string typeGlobal;
    static std::string typeScene;
    static std::string typeEntity;


    enum class VariableOrigin
    {
        GLOBAL,
        SCENE,
        ENTITY
    };



    ASetGlobalVariable(Action*_parentGameAction, const pugi::xml_node &_node, VariableOrigin _variableOrigin);

    bool initConnections(PlayedScene *_scene);

    void execute();


private:

    Variable *mVariable = nullptr;            // LINK
    VariableOrigin mVariableOrigin;
    //VariableType mVariableType = VariableType::NOT_DEFINED;

    //cfg parameters
    std::string mVariableName;
    std::string mVariableValue;
    //std::string mVariableTypeString;


};


//----------------------------------------------------------------------------------


class CActivateTrigger : public ActionCommand
{
public:

    static std::string type;


    CActivateTrigger(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *_scene);

    void execute();


private:

    TriggerVariable *mTrigger = nullptr;                // LINK

    //cfg parameters
    std::string mTriggerName;


};



//----------------------------------------------------------------------------------



class APlayScene : public ActionCommand
{
public:
    static std::string type;

    APlayScene(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;

private:

    //Scene* mScene = nullptr;

    //StringParameter *mPSceneName = nullptr;              // LINK  if sceneName is stored in a variable
    //std::string mSceneName;                                 //
    StringVar *mVarSceneName = nullptr;                     //

    //--- cfg parameters
    std::string mScenePath;

};


//----------------------------------------------------------------------------------


class ADoTransition : public ActionCommand
{
public:
    static std::string type;

    ADoTransition(Action*_parentGameAction, const pugi::xml_node &_node);
    ADoTransition(const std::string &_transition, const std::string &_show, const std::string &_hide);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;
    void update(UpdateMode _updateMode) override;


private:

    Transition *mTransition = nullptr;                      // LINK
    std::vector<TransitionMember*>mShownMembers;           // LINKS
    std::vector<TransitionMember*>mHiddenMembers;           // LINKS
    bool mExclusive = true;

    //--- cfg parameters
    std::string mTransitionName;
    std::vector<std::string>mShownObjectsNames;
    std::vector<std::string>mHiddenObjectsNames;


};



//----------------------------------------------------------------------------------

/*

class ASetObjectProperty : public ActionCommand
{
public:

    ASetObjectProperty(Action*_parentGameAction, const pugi::xml_node &node);

protected:
    ObjectId mObjectType;
    std::vector<void*>mObjects;

    //--- cfg parameters
    std::string mObjectName;
    std::string mSrcMapName;

};


//-----------


class ASetVisible : public ASetObjectProperty
{
public:
    static std::string type;
    static std::vector<ObjectId>allowedObjects;

    ASetVisible(Action*_parentGameAction, const pugi::xml_node &node);
    bool initConnections(PlayerScene *_scene) override;

    //---
    void execute() override;

private:
    bool mVisible = false;
};


//-----------


class ASetAlpha : public ASetObjectProperty
{
public:
    static std::string type;
    static std::vector<ObjectId>allowedObjects;

    ASetAlpha(Action*_parentGameAction, const pugi::xml_node &node);
    bool initConnections(PlayerScene *_scene) override;

    //---
    void execute() override;

private:
    float mAlpha = 1.0f;
};

*/

//----------

class ASetGfxObject : public ActionCommand
{
public:
    static std::string type;
    static std::vector<GfxObjectType>allowedObjects;


    enum class Property
    {
        NONE = 0,
        VISIBLE = 1,
        ALPHA = 2,
        COLOR = 4,
        TEXT = 8
    };


    ASetGfxObject(Action*_parentGameAction, const pugi::xml_node &node);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;


private:
    int mProperties = static_cast<int>(Property::NONE);

    GfxObject* mGfxObject = nullptr;    //LINK

    BoolVar* mVisible = nullptr;        // LINK
    FloatVar* mAlpha = nullptr;         // LINK
    IntVar* mColor = nullptr;           // LINK
    StringVar* mText = nullptr;         // LINK

    struct Cfg
    {
        std::string mGfxObject;
        std::string mVisible;
        std::string mAlpha;
        std::string mColor;
        std::string mText;

    };

    std::unique_ptr<Cfg>mCfg;


};



//----------------------------------------------------------------------------------


class AShowOverlayComponent : public ActionCommand
{
public:
    static std::string type;

    AShowOverlayComponent(Action*_parentGameAction, const pugi::xml_node &_node);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;


private:

    OverlayComponent *mComponent = nullptr;                        // LINK
    bool mModal = true;
    std::unique_ptr<ADoTransition> mAOpenTransition;
    std::unique_ptr<ADoTransition> mACloseTransition;

    std::unique_ptr<ComponentData> mComponentData;


    //--- cfg parameters
    struct Cfg
    {
        std::string mComponent;
        std::string mShownGfxObject;
        std::string mTransition;

        // data
        std::string mTextSegment;
    };
    std::unique_ptr<Cfg>mCfg;

};



//----------------------------------------------------------------------------------



class AQuitGame : public ActionCommand
{
public:
    static std::string type;

    AQuitGame(Action*_parentGameAction, const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene) override;

    //---
    void execute() override;

private:

};






}

#endif // PLAYER__ACTION_COMMANDS_LOGIC_H
