#include "pugixml/pugixml.hpp"
#include "map/jmMap.h"
#include "map/jmSpriteLayer.h"
#include "jmFrameAnimation.h"
#include "jmTimelineAnimationInstance.h"
#include "jmVectorLayer.h"
#include "jmVectorShape.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmSourceSprite.h"
#include "jmUtilities.h"
#include "jmSceneLayout.h"
#include "jmGlobal.h"
#include "jmApp.h"
#include "jmSystem.h"
#include "jmText.h"

#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"
#include "gui/widgets/jpGuiTable.h"
#include "jpInput.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpItemsCommon.h"
#include "jpLogicState.h"
#include "jpLogicAction.h"

#include "jpUtilities.h"


namespace jugimap{




PlayedScene* ObtainPlayerSceneOfGameAction(Action *sourceAction)
{

    while(true)
    {
        BaseObject* parent = sourceAction->parentObject();      assert(parent);

        if(parent->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState* state = static_cast<LogicState*>(parent);
            parent = state->parentObject();

        }else if(parent->baseType()==BaseObjectType::PLAYER_SCENE){
            PlayedScene* scene = static_cast<PlayedScene*>(parent);
            return  scene;

        }else{
            assert(false);
        }
    }

    assert(false);
    return nullptr;
}

/*

LogicState* ObtainLogicStateFromPath(PlayedScene *scene, Action *sourceAction, const std::string &statePath)
{


    if(statePath.empty()){

        // empty statePath indicates that we use state parent of the sourceAction

        if(sourceAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState *parentState = static_cast<LogicState*>(sourceAction->parentObject());
            return parentState;
        }else{
            dbgSystem.addMessage("The state name is empty but parent is not a state!");
            return nullptr;
        }
    }

    std::vector<std::string>statePathParts = StdString::splitString(statePath, "/");


    if(statePathParts.size()==1){

        // only name of the state -> we seek for it only in the scope where sourceAction is located

        const std::string &stateName = statePathParts.front();

        if(sourceAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState *parentState = static_cast<LogicState*>(sourceAction->parentObject());
            for(LogicState* gs : parentState->childStates()){
                if(gs->name() == stateName){
                    return gs;
                }
            }
        }

        // if state was not found there is also possibility it is a child state of the root onUpdate state !
        for(LogicState* gs : scene->updateState()->childStates()){
            if(gs->name() == stateName){
                return gs;
            }
        }


        dbgSystem.addMessage("State of name '" + statePath + "' not found! Make sure to use full name path for states which are not in the same scope as the action!");
        return nullptr;



    }else if(statePathParts.size()>1){

        // full 'path' of the state -> every part is a name of subsequential state parent


        LogicState *state = scene->updateState();

        bool errorInPath = false;
        int i=0;

        while(i<statePathParts.size()){

            const std::string &stateName = statePathParts.at(i);
            bool stateFound = false;
            for(LogicState* s : state->childStates()){
                if(s->name() == stateName){
                    state = s;
                    stateFound = true;
                    break;
                }
            }

            if(stateFound==false){
                errorInPath = true;
                break;
            }

            i++;
        }

        if(errorInPath){
            dbgSystem.addMessage("State of name '" + statePath + "' not found!");
            return nullptr;
        }


        return state;

    }


    //dbgSystem.addMessage("Path is an empty string!");

    return nullptr;

}
*/



LogicState* ObtainLogicStateFromPath2(PlayedScene *scene, LogicState *_currentState, const std::string &statePath)
{

    assert(_currentState->parentObject());


    std::vector<std::string>statePathParts = StdString::splitString(statePath, ":");



    if(statePathParts.size()==1){

        const std::string &stateName = statePathParts[0];


        // only name of the state -> we seek among the children of the parent logic state

        if(_currentState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState *parentState = static_cast<LogicState*>(_currentState->parentObject());

            for(LogicState* gs : parentState->childStates()){
                if(gs->name() == stateName){
                    return gs;
                }
            }
        }


        // If the state was not found among the children of the parent, it is also possibility it is a child state of the root logic state!
        LogicState *rootLogicState = _currentState->rootLogicState();
        for(LogicState* gs : rootLogicState->childStates()){
            if(gs->name() == stateName){
                return gs;
            }
        }

        // Check out also within scene update root logic state !
        for(LogicState* gs : scene->updateState()->childStates()){
            if(gs->name() == stateName){
                return gs;
            }
        }


        dbgSystem.addMessage("State of name '" + statePath + "' not found! Make sure to use full name path for states which are not in the same scope as the action!");
        return nullptr;



    }else if(statePathParts.size()>1){

        // full 'path' of the state -> every part is a name of subsequential state parent

        /*
        LogicState *state = _currentState->rootLogicState();

        bool errorInPath = false;
        int i=0;

        while(i<statePathParts.size()){

            const std::string &stateName = statePathParts.at(i);
            bool stateFound = false;
            for(LogicState* s : state->childStates()){
                if(s->name() == stateName){
                    state = s;
                    stateFound = true;
                    break;
                }
            }

            if(stateFound==false){
                errorInPath = true;
                break;
            }

            i++;
        }

        if(errorInPath){
            dbgSystem.addMessage("State of name '" + statePath + "' not found!");
            return nullptr;
        }
        */

        //---
        LogicState *rootState = _currentState->rootLogicState();

        LogicState *state = rootState->findChildStateViaPath(statePathParts);
        if(state){
            return state;
        }

        //---
        state = scene->updateState()->findChildStateViaPath(statePathParts);
        if(state){
            return state;
        }

        dbgSystem.addMessage("State of name '" + statePath + "' not found!");
        return nullptr;

    }


    //dbgSystem.addMessage("Path is an empty string!");

    return nullptr;

}


LogicState* ObtainLogicStateFromPath(PlayedScene *_scene, LogicState *_currentState, const std::string &_statePath)
{

    std::vector<std::string>statePathParts = StdString::splitString(_statePath, ":");

    LogicState * state = nullptr;

    // seek first from the parent state
    if(_currentState->parentObject()->baseType() == BaseObjectType::LOGIC_STATE){
        LogicState * parentState = static_cast<LogicState*>(_currentState->parentObject());
        state = parentState->findChildStateViaPath(statePathParts);
        if(state){
            return state;
        }
    }


    // seek next from the root state
    LogicState * rootState = _currentState->rootLogicState();
    state = rootState->findChildStateViaPath(statePathParts);
    if(state){
        return state;
    }


    // seek next from the scene root
    state = _scene->updateState()->findChildStateViaPath(statePathParts);
    if(state){
        return state;
    }


    dbgSystem.addMessage("State with path '" + _statePath + "' not found!");

    return state;

}


LogicState* ObtainLogicStateFromPath_SeekFromRootState(PlayedScene* _scene, LogicState* _rootState, const std::string &_statePath)
{

    std::vector<std::string>statePathParts = StdString::splitString(_statePath, ":");

    LogicState * state = nullptr;


    state = _rootState->findChildStateViaPath(statePathParts);
    if(state){
        return state;
    }

    // seek next from the scene root
    state = _scene->updateState()->findChildStateViaPath(statePathParts);
    if(state){
        return state;
    }


    dbgSystem.addMessage("State with path '" + _statePath + "' not found!");
    return state;
}


/*
Action* ObtainActionFromPath(PlayedScene* scene, Action *sourceAction, const std::string &actionPath)
{

    std::vector<std::string>actionPathParts = StdString::splitString(actionPath, "/");

    if(actionPathParts.size()==1){

        // only name of the action -> we seek for it only in the scope of the sourceAction

        const std::string &actionName = actionPathParts.front();

        if(sourceAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState *parentState = static_cast<LogicState*>(sourceAction->parentObject());
            for(Action* a : parentState->actions()){
                if(a->name() == actionName){
                    return a;
                }
            }
        }

    }else if(actionPathParts.size()>1){

        // full 'path' of the state -> every part is a name of subsequential parent


        const std::string &actionName = actionPathParts.back();

        LogicState *state = scene->updateState();

        bool errorInPath = false;
        int i=0;

        while(i<actionPathParts.size()-1)       // -1 as the last string is the name of action (other are the names of the states)
        {

            const std::string &stateName = actionPathParts.at(i);
            bool stateFound = false;
            for(LogicState* s : state->childStates()){
                if(s->name() == stateName){
                    state = s;
                    stateFound = true;
                    break;
                }
            }

            if(stateFound==false){
                errorInPath = true;
                break;
            }

            i++;
        }

        if(errorInPath == false){
            for(Action* a : state->actions()){
                if(a->name() == actionName){
                    return a;
                }
            }
        }

        dbgSystem.addMessage("Error in action path '" + actionPath + "' !");
        return nullptr;
    }


    dbgSystem.addMessage("Error! Action path is an empty string!");
    return nullptr;


}

*/



GuiWidget* ObtainGuiWidget(PlayedScene* scene, const std::string &widgetName, WidgetType guiWidgetKind)
{

    GuiWidget *w = scene->guiSystem()->findWidget(widgetName, guiWidgetKind);

    return w;

    /*
    if(w){
        if(w->GetKind()==guiWidgetKind || guiWidgetKind==GuiWidgetKind::NOT_DEFINED){
            return w;
        }else{
            dbgSystem.addMessage("A widget with name '" + widgetName + "' exists but is not of type '" + GetGuiWidgetKindString(guiWidgetKind)+ "' !");
            return nullptr;
        }
    }

    dbgSystem.addMessage(GetGuiWidgetKindString(guiWidgetKind) + " with name '" + widgetName + "' not found!");


    return nullptr;
    */

}


ItemsGroup* ObtainItemsGroup(PlayedScene* scene, const std::string &itemsGroupName)
{

    for(ItemsGroup *ig : scene->itemsLibrary()->itemsGroups()){
        if(ig->name() == itemsGroupName){
            return ig;
        }
    }

    dbgSystem.addMessage("An items group with name '" + itemsGroupName + "' not found!");


    return nullptr;

}


/*
SceneWidget *ObtainSceneWidget(PlayerScene* scene, Action *sourceAction, const std::string &widgetPath, GuiWidgetKind guiWidgetKind)
{


    std::vector<std::string>widgetPathParts = StdString::splitString(widgetPath, "/");

    if(widgetPathParts.size()==1){

        // only name of the widget

        const std::string &widgetName = widgetPathParts.front();

        if(sourceAction->parentObject()->type()==BaseObjectType::GAME_STATE){
            LogicState *parentState = static_cast<LogicState*>(sourceAction->parentObject());
            for(SceneWidget* sw : parentState->sceneWidgets()){
                if(sw->name() == widgetName && sw->widgetKind()==guiWidgetKind){
                    return sw;

                }else{
                    settings.SetErrorMessage("Widget with path '" + widgetPath + "' exists but is not of type '" + GetGuiWidgetKindString(guiWidgetKind)+ "' !");
                    return nullptr;
                }
            }

        }else{
            settings.SetErrorMessage("Parent of widget with path '" + widgetPath + "' is not of LogicState type!");
            return nullptr;
        }


    }else if(widgetPathParts.size()>1){


        // full 'path' of the state -> every part is a name of subsequential parent state

        const std::string &widgetName = widgetPathParts.back();

        LogicState *state = scene->updateState();

        bool errorInPath = false;
        int i=0;

        while(i<widgetPathParts.size()-1)       // -1 as the last string is the name of action (other are the names of the states)
        {

            const std::string &stateName = widgetPathParts.at(i);
            bool stateFound = false;
            for(LogicState* s : state->childStates()){
                if(s->name() == stateName){
                    state = s;
                    stateFound = true;
                    break;
                }
            }

            if(stateFound==false){
                errorInPath = true;
                break;
            }

            i++;
        }

        if(errorInPath == false){

            for(SceneWidget * sw : state->sceneWidgets()){
                if(sw->name() == widgetName){

                    if(sw->widgetKind()==guiWidgetKind){
                        return sw;

                    }else{
                        settings.SetErrorMessage("Widget with path '" + widgetPath + "' exists but is not of type '" + GetGuiWidgetKindString(guiWidgetKind)+ "' !");
                        return nullptr;
                    }
                }
            }
        }

        settings.SetErrorMessage("Error in widget path '" + widgetPath + "' !");
        return nullptr;

    }


    settings.SetErrorMessage("Error! Scene widget path is an empty string!");
    return nullptr;

}
*/


bool collectObjects(std::vector<void*>&objects, PlayedScene *scene, GSpritesObjectType objectType, const std::string &objectName, const std::string &srcMapName)
{

    Map *map = scene->getMap(srcMapName);


    if(map==nullptr){
        dbgSystem.addMessage("Map '" + srcMapName + "'+ not found!");
        return false;

    }else if(map->type()!=MapType::SCREEN){
        dbgSystem.addMessage("Map '" + srcMapName + "'+ is not a screen map!");
        return false;
    }


    if(objectType==GSpritesObjectType::SPRITE){

        std::vector<Sprite*>collectedSprites;
        CollectSpritesWithName(map, collectedSprites, objectName);

        if(collectedSprites.empty()){
            dbgSystem.addMessage("Sprite(s) '" + objectName + "' not found!");
            return false;
        }

        for(Sprite* s : collectedSprites){
            objects.push_back(s);
        }


    }else if(objectType==GSpritesObjectType::SPRITE_LAYER){

        std::vector<Layer*>collectedLayers;
        CollectLayersWithName(map, collectedLayers, objectName, LayerKind::SPRITE_LAYER);

        if(collectedLayers.empty()){
            dbgSystem.addMessage("Layer(s) '" + objectName + "' not found!");
            return false;
        }

        for(Layer* l: collectedLayers){
            objects.push_back(l);
        }

    }else if(objectType==GSpritesObjectType::MAP){

        objects.push_back(map);
    }

    return  true;
}



TextSegment* ObtainTextSegment(const std::string &textSegmentPath)
{

    std::vector<std::string>textSegmentParts = StdString::splitString(textSegmentPath, "/");

    if(textSegmentParts.size()==2){

        const std::string &bookName = textSegmentParts[0];
        int segmentID = StdString::stringToInt(textSegmentParts[1], 0);

        TextBook *book = textLibrary.FindBook(bookName);
        if(book==nullptr){
            dbgSystem.addMessage("Text book with name '" + bookName + "' not found!");
            return nullptr;
        }
        if(book->isExternalDataLoaded()==false){
            book->loadExternalData();
        }

        if(segmentID<0 || segmentID>=book->textSegments().size()){
            dbgSystem.addMessage("Text segment with index '"+ std::to_string(segmentID) + "' in the book '" + bookName + "'  is out of bounds!");
            return nullptr;

        }

        TextSegment *ts = book->textSegments()[segmentID];
        return ts;
    }

    return nullptr;

}



TextSegment* ObtainTextSegment2(const std::string &textSegmentPath)
{

    std::vector<std::string>textSegmentParts = StdString::splitString(textSegmentPath, ":");

    if(textSegmentParts.size()==2){

        const std::string &bookName = textSegmentParts[0];
        int segmentID = StdString::stringToInt(textSegmentParts[1], 0);

        TextBook *book = textLibrary.FindBook(bookName);
        if(book==nullptr){
            dbgSystem.addMessage("Text book with name '" + bookName + "' not found!");
            return nullptr;
        }
        if(book->isExternalDataLoaded()==false){
            book->loadExternalData();
        }

        if(segmentID<0 || segmentID>=book->textSegments().size()){
            dbgSystem.addMessage("Text segment with index '"+ std::to_string(segmentID) + "' in the book '" + bookName + "'  is out of bounds!");
            return nullptr;

        }

        TextSegment *ts = book->textSegments()[segmentID];
        return ts;
    }


    dbgSystem.addMessage("Parsing string '" +textSegmentPath + "' error!");
    return nullptr;

}



bool ObtainPlainText(const std::string &textPath, std::string &text)
{

    std::vector<std::string>textParts = StdString::splitString(textPath, "/");

    if(textParts.size()==1){

        // string
        text = textParts[0];


    }else if(textParts.size()==2){

        // text segment
        TextSegment* ts = ObtainTextSegment(textPath);
        if(ts==nullptr){
            return false;
        }
        text = ts->plainText();

    }else{

        dbgSystem.addMessage("Wrong text path '" + textPath + "'!");
        return false;
    }

    StdString::removeStringWhiteSpacesOnStartAndEnd(text);

    return true;
}


/*
TriggerVariable* ObtainTrigger(Action *sourceAction, const std::string &triggerName)
{


    if(sourceAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){

        LogicState *state = static_cast<LogicState*>(sourceAction->parentObject());

        // traverse through states until we found

        while(state){

            if(state->triggers()){
                TriggerVariable *t = state->triggers()->getTrigger(triggerName, false);
                if(t){
                    return t;
                }
            }

            if(state->parentObject()==nullptr){
                break;

            }else if(state->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
                state = static_cast<LogicState*>(state->parentObject());

            }else if(state->parentObject()->baseType()==BaseObjectType::PLAYER_SCENE){
                PlayedScene* scene = static_cast<PlayedScene*>(state->parentObject());
                TriggerVariable *t = scene->triggers().getTrigger(triggerName, false);
                if(t){
                    return t;
                }
                state = nullptr;

            }else{
                break;
            }
        }
    }

    dbgSystem.addMessage("Trigger '" + triggerName +"' not found!");

    return nullptr;
}
*/


SourceSprite* ObtainSourceSpriteWithConstantParameter(const std::vector<SourceSprite*> &sourceSprites, const std::string &pName, const std::string &pValue)
{

    for(SourceSprite *ss : sourceSprites){
        if(ss->constantParameters().exists(pName, pValue)){
            return ss;
        }
    }

    dbgSystem.addMessage("Source sprite with parameter name '" + pName + "' and value '" + pValue + "' not found!");

    return nullptr;
}


/*
Scene* ObtainScene(const std::string &scenePath)
{


    std::string sceneName;
    std::vector<std::string>sceneNameParts = StdString::splitString(scenePath, "/");


    if(sceneNameParts.size()==1){

        // only name of the scene

        sceneName = sceneNameParts[0];


    }else if(sceneNameParts.size()==2){

        // the scene name is stored in a parameter
        // first part is the origin of the parameter and the second part is the parameter name


        const std::string &origin = sceneNameParts[0];
        const std::string &parameter = sceneNameParts[1];
        Parameter *p = nullptr;

        if(origin=="system"){
            p = system.getParameter(parameter);
        }else{
            dbgSystem.addMessage("Wrong origin '" + origin + "' of the parameter '" + parameter + "'" );
            return nullptr;
        }

        if(p==nullptr){
            return nullptr;
        }
        if(p->type()!=ValueType::STRING){
            dbgSystem.addMessage("Wrong type of the parameter '" + parameter +"'. It is '" + GetValueTypeString(p->type()) + "' but it should be '" + GetValueTypeString(ValueType::STRING) +"' !");
            return nullptr;
        }

        StringParameter *sp = static_cast<StringParameter*>(p);
        sceneName = sp->value();
    }

    if(sceneName.empty()){
        dbgSystem.addMessage("Can not obtain scene name from string '" + scenePath + "' !" );
        return nullptr;
    }

    App *app = gameApp;

    Scene* scene = app->getScene(sceneName);

    if(scene==nullptr){
        dbgSystem.addMessage("Scene with name '" + sceneName +"' not found! ");
    }

    return scene;


}


Parameter* ObtainParameter(const std::string &path)
{


    std::vector<std::string>pathParts = StdString::splitString(path, "/");

    Parameter *p = nullptr;

    if(pathParts.size()==2){

        // the scene name is stored in a parameter
        // first part is the origin of the parameter and the second part is the parameter name


        const std::string &origin = pathParts[0];
        const std::string &pName = pathParts[1];


        if(origin=="system"){
            p = system.getParameter(pName);

        }else{
            dbgSystem.addMessage("Wrong origin '" + origin + "' of the parameter '" + pName + "'" );
            return nullptr;
        }

        if(p==nullptr){
            dbgSystem.addMessage("Can not obtain parameter from path '" + path + "' !" );
            return nullptr;
        }


    }else{

        dbgSystem.addMessage("Wrong path '" + path +"'!");
    }


    return p;

}
*/

/*
Variable* ObtainVariableFromPath(const std::string &path, PlayerScene *scene, std::string &directValue)
{

    std::vector<std::string>pathParts = StdString::splitString(path, "/");

    Variable *var = nullptr;

    if(pathParts.size()==2){

        // the scene name is stored in a parameter
        // first part is the origin of the parameter and the second part is the parameter name


        const std::string &varOrigin = pathParts[0];
        const std::string &varName = pathParts[1];


        if(varOrigin=="var"){
            var = scene->variables().getVariable(varName);          // variable is stored in current scene and accessible from it

        } else if(varOrigin=="globVar"){
            Player *playedApp = dynamic_cast<Player*>(scene->parentApp());   assert(playedApp);
            var = playedApp->globalVariables()->getVariable(varName);                // variable is stored as global variable

        }else{
            dbgSystem.addMessage("Wrong origin '" + varOrigin + "' of the variable '" + varName + "'" );
            return nullptr;
        }

        if(var==nullptr){
            dbgSystem.addMessage("Can not obtain variable from path '" + path + "' !" );
            return nullptr;
        }


    }else{

        dbgSystem.addMessage("Wrong path '" + path +"'!");
    }


    return var;

}
*/



Variable* ObtainVariableFromPath(const std::string &path, PlayedScene *scene, std::string &directValue)
{

    std::vector<std::string>pathParts = StdString::splitString(path, "/");


    if(pathParts.size()==0){
        dbgSystem.addMessage("Error in variable path '" + path + "'" );
        return nullptr;

    }else if(pathParts.size()==1){

        // direct value
        directValue = pathParts[0];
        return nullptr;


    }else if(pathParts.size()==2){

        const std::string &varOrigin = pathParts[0];
        const std::string &varName = pathParts[1];

        if(varOrigin=="scene"){

            Variable *var = scene->variables().getVariable(varName);          // variable is stored in current scene and accessible from it
            return var;

        }else if(varOrigin=="app"){

            Variable *var = app->globalVariables()->getVariable(varName);        // variable is stored as global variable
            return var;

        }else if(varOrigin=="system"){

            Variable *var = app->systemVariables()->getVariable(varName);        // variable is stored as global variable
            return var;

        }
    }


    // 2 or more parts of the path
    //----------------------------------------------------------------------------------------------------------------------

    GuiWidget* widget = nullptr;
    BaseObject* object = nullptr;


    int i=0;
    while(i<pathParts.size()){

        std::string varOrigin = pathParts[i];
        std::vector<std::string>originParts = StdString::splitString(varOrigin, ":");


        if(widget){

            // there may be cases later for this situations
        }
        if(object){

            // there may be cases later for this situations
        }


        if(originParts.size()==1){

            // until I make cases where this situation is possible treat is an error
            dbgSystem.addMessage("Wrong origin object '" + varOrigin + "' of the variable path '" + path + "'" );
            return nullptr;


        }else if(originParts.size()==2){

            const std::string &originObject = originParts[0];
            const std::string &objectName = originParts[1];

            if(originObject=="table"){

                widget = ObtainGuiWidget(scene, objectName, WidgetType::TABLE);

                if(widget==nullptr){
                    return nullptr;
                }
            }
        }


        if(i+1 == int(pathParts.size())-1){      //  last part MUST be the name of the variable !

            const std::string &varName = pathParts[i+1];

            if(widget){
                if(widget->type()==WidgetType::TABLE){
                    GuiTable *table = static_cast<GuiTable*>(widget);

                    //Variable *var = table->variables()->getVariable(varName);
                    //return var;

                }

            }else if(object){


            }else{
                dbgSystem.addMessage("Error in variable path '" + path + "'" );

            }

        }

        i++;
    }

    dbgSystem.addMessage("Error in variable path '" + path + "'" );

    return nullptr;

}



Variable* ObtainVariableFromPath(const std::string &path, PlayedScene *scene)
{

    std::string dummy;
    Variable *v = ObtainVariableFromPath(path, scene, dummy);

    if(dummy.empty()==false){
        dbgSystem.addMessage("Error in variable path '" + path + "'" );
    }

    return v;


}



IntVar* ObtainIntVariableFromPath(const std::string &path, PlayedScene *scene , int &directValue, bool &error)
{

    error = true;
    std::string dValue;
    Variable *v = ObtainVariableFromPath(path, scene, dValue);


    if(dValue.empty()==false){
        error = false;
        directValue = StdString::stringToInt(dValue, 0);


    }else if(v){
        if(v->type()!=VariableType::INTEGER){
            dbgSystem.addMessage("Variable '"+ v->name() +"' is of type '" + GetVariableTypeString(v->type()) + "' but is should be '" + GetVariableTypeString(VariableType::INTEGER)+ "'!");
            return nullptr;

        }else{
            error = false;
            return static_cast<IntVar*>(v);
        }
    }


    return nullptr;
}


FloatVar* ObtainFloatVariableFromPath(const std::string &path, PlayedScene *scene , float &directValue, bool &error)
{

    error = true;
    std::string dValue;
    Variable *v = ObtainVariableFromPath(path, scene, dValue);


    if(dValue.empty()==false){
        error = false;
        directValue = StdString::stringToFloat(dValue, 0.0f);

    }else if(v){
        if(v->type()!=VariableType::FLOAT){
            dbgSystem.addMessage("Variable '"+ v->name() +"' is of type '" + GetVariableTypeString(v->type()) + "' but is should be '" + GetVariableTypeString(VariableType::FLOAT)+ "'!");
            return nullptr;

        }else{
            error = false;
            return static_cast<FloatVar*>(v);
        }
    }

    return nullptr;
}


BoolVar* ObtainBoolVariableFromPath(const std::string &path, PlayedScene *scene , bool &directValue, bool &error)
{

    error = true;
    std::string dValue;
    Variable *v = ObtainVariableFromPath(path, scene, dValue);


    if(dValue.empty()==false){
        directValue = StdString::stringToBool(dValue);
        error = false;

    }else if(v){
        if(v->type()!=VariableType::BOOL){
            dbgSystem.addMessage("Variable '"+ v->name() +"' is of type '" + GetVariableTypeString(v->type()) + "' but is should be '" + GetVariableTypeString(VariableType::BOOL)+ "'!");
            return nullptr;

        }else{
            error = false;
            return static_cast<BoolVar*>(v);
        }
    }

    return nullptr;
}



StringVar* ObtainStringVariableFromPath(const std::string &path, PlayedScene *scene , std::string &directValue)
{


    Variable *v = ObtainVariableFromPath(path, scene, directValue);


    if(directValue.empty()==false){
        // ok


    }else if(v){
        if(v->type()!=VariableType::STRING){
            dbgSystem.addMessage("Variable '"+ v->name() +"' is of type '" + GetVariableTypeString(v->type()) + "' but is should be '" + GetVariableTypeString(VariableType::STRING)+ "'!");
            return nullptr;

        }else{

            return static_cast<StringVar*>(v);
        }
    }


    return nullptr;
}



//==============================================================================================



AnimationInstance* ObtainFrameAnimationInstance(StandardSprite *_sprite, const std::string &aniName)
{

    jugimap::FrameAnimation * fameAnimation = FindFrameAnimationWithName(_sprite, aniName);
    if(fameAnimation){
        return new FrameAnimationInstance(fameAnimation, _sprite);
    }

    dbgSystem.addMessage("Animation '"+ aniName +"' not found!");

    return nullptr;

}


jugimap::AnimationInstance *ObtainAnimationInstance(Sprite *_sprite, const std::string &aniName, bool setErrorMessage)
{

    if(_sprite->type()==SpriteType::STANDARD){
        jugimap::FrameAnimation * fameAnimation = FindFrameAnimationWithName(static_cast<StandardSprite*>(_sprite), aniName);
        if(fameAnimation){
            return new FrameAnimationInstance(fameAnimation, _sprite);
        }
    }

    jugimap::TimelineAnimation * timelineAnimation = FindTimelineAnimationWithName(_sprite, aniName);
    if(timelineAnimation){
        return new TimelineAnimationInstance(timelineAnimation, _sprite);
    }


    if( setErrorMessage){
        dbgSystem.addMessage("Animation '"+ aniName +"' not found!");
    }

    return nullptr;

}





//==============================================================================================


VectorShape* _ObtainVectorShapeWithAPathPointPosition(SpriteLayer *spriteLayer, Vec2f pos, float &distMin, int &index)
{

    for(Sprite *s : spriteLayer->sprites()){
        if(s->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(s);
            VectorShape * vs = ObtainVectorShapeWithAPathPointPosition(cs->map(), pos, distMin, index);
            if(vs) return vs;
        }
    }
    return nullptr;
}


VectorShape* ObtainVectorShapeWithAPathPointPosition(Map *map, Vec2f pos, float &distMin, int &index)
{

    VectorShape *vectorShape= nullptr;


    for(Layer *l : map->layers()){
        if(l->kind()==LayerKind::VECTOR_LAYER){
            VectorLayer * vl = static_cast<VectorLayer*>(l);

            for(VectorShape* vs : vl->shapes()){
                //--- obtain starting path point from the sprite position

                for(unsigned int i=0; i<vs->pathPoints().size(); i++){
                    float dist = DistanceTwoPoints(pos, vs->pathPoints()[i]);
                    if(dist<distMin){
                        distMin = dist;
                        index = i;
                        vectorShape = vs;
                    }
                }
            }

        }else if(l->kind()==LayerKind::SPRITE_LAYER){
            SpriteLayer * sl = static_cast<SpriteLayer*>(l);
            VectorShape * childVectorShape = _ObtainVectorShapeWithAPathPointPosition(sl, pos, distMin, index);
            if(childVectorShape){
                vectorShape = childVectorShape;
            }
        }
    }

    return vectorShape;
}



std::string obtainStateIdentifierString(LogicState *_logicState, const std::string &_prefix)
{

    std::string identifier;
    LogicState* state = _logicState;

    while(true){

        identifier = ":" + state->name() + identifier;

        if(state->parentObject() && state->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState* parentState= static_cast<LogicState*>(state->parentObject());

            if(parentState->parentObject() && parentState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){    // the last parent logic state is not included !
                state = parentState;
                continue;
            }
        }
        break;

    }

    identifier = _prefix + identifier;

    return identifier;

}








}
