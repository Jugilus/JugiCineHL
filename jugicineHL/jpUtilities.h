#ifndef PLAYER_UTILITIES_H
#define PLAYER_UTILITIES_H


#include <string>
#include <vector>

#include "jmGlobal.h"
#include "jpGlobal.h"
#include "jpVariables.h"
#include "jpSettings.h"


namespace jugimap{

class AnimationInstance;
class FrameAnimationInstance;
class StandardSprite;

class SourceSprite;
class TextSegment;
class SceneWidget;
class GuiWidget;
class ItemsGroup;
class PlayedScene;
class GuiButton;
class Layer;
class ActionTrigger;
class ActionCommand;
class Action;
class LogicState;
class Scene;
class Variable;
class TriggerVariable;
class Entity;
class BehaviorState;
class VectorShape;
struct FilteredContactTrigger;
struct SignalAccessor;
struct MovementEngineData;
struct TaskEngineData;




PlayedScene* ObtainPlayerSceneOfGameAction(Action *sourceAction);


LogicState* ObtainLogicStateFromPath(PlayedScene* scene, jugimap::Action *sourceAction, const std::string &statePath);


Action* ObtainActionFromPath(PlayedScene* scene, Action *sourceAction, const std::string &actionPath);


GuiWidget* ObtainGuiWidget(PlayedScene* scene, const std::string &widgetName, GuiWidgetKind guiWidgetKind);


ItemsGroup* ObtainItemsGroup(PlayedScene* scene, const std::string &itemsGroupName);


//SceneWidget* ObtainSceneWidget(PlayerScene* scene, Action *sourceAction, const std::string &widgetPath, GuiWidgetKind guiWidgetKind);


bool collectObjects(std::vector<void*>&objects, PlayedScene *scene, GfxObjectType objectType, const std::string &objectName, const std::string &srcMapName);


TextSegment* ObtainTextSegment(const std::string &textSegmentPath);


bool ObtainPlainText(const std::string &textPath, std::string &text);


TriggerVariable* ObtainTrigger(Action *sourceAction, const std::string &triggerName);


SourceSprite* ObtainSourceSpriteWithConstantParameter(const std::vector<SourceSprite*> &sourceSprites, const std::string &pName, const std::string &pValue);




//Scene* ObtainScene(const std::string &scenePath);


//Parameter* ObtainParameter(const std::string &path);


Variable* ObtainVariableFromPath(const std::string &path, PlayedScene *scene , std::string &directValue);


Variable* ObtainVariableFromPath(const std::string &path, PlayedScene *scene);


IntVar* ObtainIntVariableFromPath(const std::string &path, PlayedScene *scene , int &directValue, bool &error);


FloatVar* ObtainFloatVariableFromPath(const std::string &path, PlayedScene *scene , float &directValue, bool &error);


BoolVar* ObtainBoolVariableFromPath(const std::string &path, PlayedScene *scene , bool &directValue, bool &error);


StringVar* ObtainStringVariableFromPath(const std::string &path, PlayedScene *scene , std::string &directValue);


//---------------------------------------------------------------



//bool ObtainTriggersFromPath(std::vector<ButtonState*>&triggers, const std::string &_path, PlayedScene *_scene, Entity *_actor, BehaviorState * _state, TriggerResult &_triggerResult);

//bool ObtainTriggersFromPath(std::vector<ButtonState*>&triggers, const std::string &path, PlayedScene *scene, Actor *actor);


//----------------------------------------------------------------


jugimap::AnimationInstance *ObtainFrameAnimationInstance(StandardSprite *_sprite, const std::string &aniName);

jugimap::AnimationInstance *ObtainAnimationInstance(Sprite *_sprite, const std::string &aniName, bool throwError=true);



//----------------------------------------------------------------

VectorShape* ObtainVectorShapeWithAPathPointPosition(Map *map, Vec2f pos, float &distMin, int &index);



}

#endif // PLAYER_UTILITIES_H
