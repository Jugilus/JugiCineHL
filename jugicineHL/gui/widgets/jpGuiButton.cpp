#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSourceSprite.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmInput.h"
#include "jmMap.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmUtilities.h"

#include "jpQueries.h"
#include "jpObjectParser.h"
#include "jpGuiCommon.h"
#include "jpGuiButton.h"




namespace jugimap {




ButtonState GetButtonStateFromString(const std::string &state)
{

    if(state=="NORMAL"){
        return ButtonState::NORMAL;

    }else if(state=="NORMAL_HOVERED"){
        return ButtonState::NORMAL_HOVERED;

    }else if(state=="NORMAL_PRESSED"){
        return ButtonState::NORMAL_PRESSED;

    }else if(state=="CHECKED"){
        return ButtonState::CHECKED;

    }else if(state=="CHECKED_HOVERED"){
        return ButtonState::CHECKED_HOVERED;

    }else if(state=="CHECKED_PRESSED"){
        return ButtonState::CHECKED_PRESSED;

    }else if(state=="NORMAL_HIGHLIGHTED"){
        return ButtonState::NORMAL_HIGHLIGHTED;

    }else if(state=="CHECKED_HIGHLIGHTED"){
        return ButtonState::CHECKED_HIGHLIGHTED;

    }else if(state=="DISABLED"){
        return ButtonState::DISABLED;

    }


    return ButtonState::UNKNOWN;

}


std::vector<std::pair<std::string, int>>gButtonStateNamedValues
{
    {"NORMAL", 0},
    {"NORMAL_HOVERED", 1},
    {"NORMAL_PRESSED", 2},
    {"CHECKED", 3},
    {"CHECKED_HOVERED", 4},
    {"CHECKED_PRESSED", 5},
    {"NORMAL_HIGHLIGHTED", 6},
    {"CHECKED_HIGHLIGHTED", 7},
    {"DISABLED", 8}
};



bool ButtonCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="checkable"){
                    checkable = a.as_bool(false);

                }else if(attributeName=="initialState"){
                    std::string sInitialState = a.as_string();
                    if(sInitialState=="NORMAL"){
                        initialState = ButtonState::NORMAL;

                    }else if(sInitialState=="CHECKED"){
                        initialState = ButtonState::CHECKED;

                    }else if(sInitialState=="DISABLED"){
                        initialState = ButtonState::DISABLED;

                    }else{
                        dbgSystem.addMessage("Wrong initial state '" + sInitialState +"' !");
                        return false;
                    }

                }else if(attributeName=="exclusiveGroup"){
                    exclusiveGroup = a.as_string();

                }else if(attributeName=="statesTransitionDuration"){
                    statesTransitionDuration = a.as_string();

                //}else if(attributeName=="highlightSprite"){
                //    highlightSprite = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }

            }

        }else if(nodeName=="style"){

            spritePropertyCfgs.push_back(SpritePropertyCfg());
            SpritePropertyCfg &spc = spritePropertyCfgs.back();

            if(spc.initCfg(n)==false){
                return false;
            }



        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }



    dbgSystem.removeLastMessage();
    return true;

}


bool ButtonCfg::SpritePropertyCfg::initCfg(const pugi::xml_node &_node)
{

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="sprite"){

            sprite = a.as_string();

            std::vector<std::string> sParts = StdString::splitString(sprite, ",");
            if(sParts.size()==2){
                sprite = sParts[0];
                spriteID = StdString::stringToInt(sParts[1], 0);
            }

        }else if(attributeName=="property"){

            std::string sProperty = a.as_string();

            if(sProperty=="SCALE"){
                property = GuiButtonSpritePropertyKind::SCALE;
                parNamePreffix = "wScale";

            }else if(sProperty=="ROTATION"){
                property = GuiButtonSpritePropertyKind::ROTATION;
                parNamePreffix = "wRotation";

            }else if(sProperty=="ALPHA"){
                property = GuiButtonSpritePropertyKind::ALPHA;
                parNamePreffix = "wAlpha";

            }else if(sProperty=="LABEL_COLOR"){
                property = GuiButtonSpritePropertyKind::LABEL_COLOR;
                parNamePreffix = "wLabelColor";

            }else if(sProperty=="COLOR_OVERLAY"){
                property = GuiButtonSpritePropertyKind::COLOR_OVERLAY;
                parNamePreffix = "wColorOverlay";

            }else if(sProperty=="VISIBILITY"){
                property = GuiButtonSpritePropertyKind::VISIBILITY;
                parNamePreffix = "wVisible";

            }else if(sProperty=="FRAME"){
                property = GuiButtonSpritePropertyKind::FRAME;
                parNamePreffix = "wFrame";

            }else{
                dbgSystem.addMessage("Wrong property '" + sProperty +"' !");
                return false;

            }

        }else if(attributeName=="statesTransitionDelta"){
            statesTransitionDelta = a.as_string();

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        loopCfgs.push_back(AniLoopCfg());
        AniLoopCfg &alc = loopCfgs.back();

        if(nodeName=="state_NORMAL"){
            alc.parNameSuffix = "";

        }else if(nodeName=="state_NORMAL_OVER"){
            alc.parNameSuffix = "_CO";

        }else if(nodeName=="state_NORMAL_PRESSED"){
            alc.parNameSuffix = "_CD";

        }else if(nodeName=="state_CHECKED"){
            alc.parNameSuffix = "_checked";

        }else if(nodeName=="state_CHECKED_OVER"){
            alc.parNameSuffix = "_checkedCO";

        }else if(nodeName=="state_CHECKED_PRESSED"){
            alc.parNameSuffix = "_checkedCD";

        }else if(nodeName=="state_HIGHLIGHTED"){
            alc.parNameSuffix = "_HL";

        }else if(nodeName=="state_CHECKED_HIGHLIGHTED"){
            alc.parNameSuffix = "_checkedHL";

        }else if(nodeName=="state_DISABLED"){
            alc.parNameSuffix = "_disabled";

        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }

        for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
            std::string attributeName =std::string(a.name());

            if(attributeName=="def"){
                alc.def = a.as_string();

            }else if(attributeName=="min"){
                alc.min = a.as_string();

            }else if(attributeName=="max"){
                alc.max = a.as_string();

            }else if(attributeName=="duration"){
                alc.duration = alc.duration;

            }else if(attributeName=="mode"){
                std::string sLoopMode = a.as_string();
                if(sLoopMode=="REVERSE"){
                    alc.loopMode = AniLoopCfg::LoopMode::REVERSE;

                }else if(sLoopMode=="RESTART"){
                    alc.loopMode = AniLoopCfg::LoopMode::RESTART;

                }else{
                    dbgSystem.addMessage("Wrong loop mode '" + sLoopMode +"' !");
                    return false;
                }

            }else if(attributeName=="visible"){
                alc.def = a.as_string();

            }else if(attributeName=="frame"){
                alc.def = a.as_string();

            }else{
                dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                return false;
            }
        }

        //---
        if(alc.min != "" && alc.max != ""){
            alc.parValue = alc.def + "," + alc.min + "," + alc.max + "," + alc.duration;

            if(alc.loopMode==AniLoopCfg::LoopMode::RESTART){
                alc.parValue += ", restart";
            }

        }else{
            alc.parValue = alc.def;
        }
    }


    return true;

}



bool ButtonCfg::setSpriteStyleParameters(Sprite *_rootSprite)
{

    if(name=="checkBoxA"){
        DummyFunction();
    }

    for(SpritePropertyCfg &styleCfg : spritePropertyCfgs){

        std::vector<Sprite*>sprites;
        if(_rootSprite->name()==styleCfg.sprite || styleCfg.sprite=="ROOT_SPRITE"){
            sprites.push_back(_rootSprite);
        }

        if(_rootSprite->type()==SpriteType::COMPOSITE){
            CompositeSprite *cs = static_cast<CompositeSprite*>(_rootSprite);
            CollectSpritesWithName(cs->map(), sprites, styleCfg.sprite);
        }

        if(styleCfg.spriteID != 0){
            for(int i = int(sprites.size())-1; i>=0; i-- ){
                if(sprites[i]->id() != styleCfg.spriteID ){
                    sprites.erase(sprites.begin()+i);
                }
            }
        }

        if(sprites.empty()){
            dbgSystem.addMessage("Button style error! No sprite with name '" + styleCfg.sprite+ "' and id '"+ std::to_string(styleCfg.spriteID) + "' found!");
            return false;
        }

        for(Sprite *s : sprites){

            for(AniLoopCfg &aniCfg : styleCfg.loopCfgs){
                std::string parName = styleCfg.parNamePreffix + aniCfg.parNameSuffix;
                if(s->sourceSprite()->constantParameters().exists(parName)==false){
                    s->sourceSprite()->constantParameters().push_back((CParameter(parName, aniCfg.parValue)));
                }else{
                    DummyFunction();
                }

                if(statesTransitionDuration != "" && styleCfg.statesTransitionDelta !=""){
                    std::string parName = styleCfg.parNamePreffix + "_transitionDur";

                    if(s->sourceSprite()->constantParameters().exists(parName)==false){
                        std::string parValue = styleCfg.statesTransitionDelta +", " + statesTransitionDuration;
                        s->sourceSprite()->constantParameters().push_back((CParameter(parName, parValue)));
                    }else{
                        DummyFunction();
                    }
                }
            }
        }
    }

    return true;

}



//====================================================================================================



bool IsGuiButtonStateMouseOver(ButtonState _buttonState)
{
    if(_buttonState==ButtonState::NORMAL || _buttonState==ButtonState::CHECKED || _buttonState==ButtonState::DISABLED){
        return false;
    }
    return true;
}


GuiButtonSpriteFloatPropertyData GuiButtonSpriteFloatPropertyData::ParseFromString(const std::string &text)
{

    GuiButtonSpriteFloatPropertyData fv;

    std::vector<std::string>subparameters = StdString::splitString(text, ",");

    if(subparameters.size()==1){
        fv.value = std::stof(subparameters.front());

    }else if(subparameters.size()>=4){
        fv.value = std::stof(subparameters[0]);
        fv.valueMin = std::stof(subparameters[1]);
        fv.valueMax = std::stof(subparameters[2]);
        fv.loopDurationMS = 1000 * std::stof(subparameters[3]);
        fv.loopType = LoopType::REVERT_DIRECTION;
        if(subparameters.size()==5){
            std::string loopType = subparameters[4];
            StdString::removeStringWhiteSpacesOnStartAndEnd(loopType);
            if(loopType=="restart"){
                fv.loopType = LoopType::RESTART;
            }
        }

    }
    return fv;
}


void GuiButtonSpriteFloatPropertyController::ParseMouseOverTransitionData(const std::string &text)
{

    std::vector<std::string>subparameters = StdString::splitString(text, ",");

    if(subparameters.size()==2){
        float delta = std::stof(subparameters[0]);
        int durationMS = 1000 * std::stof(subparameters[1]);
        mouseOverTransitionSpeed = delta / durationMS;
    }


}


void GuiButtonSpriteFloatPropertyController::SetStartingState(ButtonState _buttonVisualState)
{

    currentValue = GetSpritePropertyValue();

    newValue = propertyDataForStates[(int)_buttonVisualState].value;
    //newValue = propertyDataForStates[(int)_buttonVisualState]->value;
    targetValue = newValue;


    //if(controlledProperty == GuiButtonSpritePropertyKind::POSITION_X || controlledProperty == GuiButtonSpritePropertyKind::POSITION_Y){
        // Position 'newValue' is delta position while 'currentValue' is absolute sprite position !
    //    if(AreEqual(currentValue+newValue, currentValue)==false){
    //        ApplyPropertyValueToSprite(currentValue+newValue);
    //    }

    //}else{
        if(AreEqual(newValue, currentValue)==false){
            ApplyPropertyValueToSprite(newValue);
        }
    //}




}


void GuiButtonSpriteFloatPropertyController::Update(ButtonState _buttonVisualState)
{

    float value = propertyDataForStates[(int)_buttonVisualState].value;
    float valueMin = propertyDataForStates[(int)_buttonVisualState].valueMin;
    float valueMax = propertyDataForStates[(int)_buttonVisualState].valueMax;
    int loopDurationMS = propertyDataForStates[(int)_buttonVisualState].loopDurationMS;
    GuiButtonSpriteFloatPropertyData::LoopType loopType = propertyDataForStates[(int)_buttonVisualState].loopType;

    //currentValue = GetSpritePropertyValue();


    if(_buttonVisualState != oldButtonVisualState){

        targetValue = value;

        if(IsGuiButtonStateMouseOver(_buttonVisualState)!=IsGuiButtonStateMouseOver(oldButtonVisualState)){
            if(mouseOverTransitionSpeed>0.0001){
                mouseOverTransition = true;
            }
        }
    }

    float deltaValue = 0.0;

    //----
    if(mouseOverTransition){

        deltaValue = time.GetLogicTimeMS() * mouseOverTransitionSpeed;

        if(currentValue < targetValue){
            newValue = currentValue + deltaValue;
            if(newValue >= targetValue){
                newValue = targetValue;
                mouseOverTransition = false;
            }

        }else{
            newValue = currentValue - deltaValue;
            if(newValue <= targetValue){
                newValue = targetValue;
                mouseOverTransition = false;
            }
        }

    }else{

        if(loopDurationMS>0 && std::abs(valueMax - valueMin) > 0.0001){

            deltaValue = time.GetLogicTimeMS() * (valueMax - valueMin) / loopDurationMS;

            if(loopType == GuiButtonSpriteFloatPropertyData::LoopType::REVERT_DIRECTION){

                newValue = currentValue + deltaValue*reverseSign;

                if(newValue <= valueMin){
                    newValue = valueMin;
                    reverseSign = -reverseSign;

                }else if(newValue >= valueMax){
                    newValue = valueMax;
                    reverseSign = -reverseSign;
                }


            }else if(loopType == GuiButtonSpriteFloatPropertyData::LoopType::RESTART){

                newValue = currentValue + deltaValue;

                if(newValue>valueMax){
                    newValue = valueMin;
                }
            }

        }else{

            newValue = targetValue;

        }

    }

    if(AreEqual(newValue, currentValue)==false){
        ApplyPropertyValueToSprite(newValue);
        currentValue = newValue;
    }


    oldButtonVisualState = _buttonVisualState;

}


float GuiButtonSpriteFloatPropertyController::GetSpritePropertyValue()
{

    if(controlledProperty == GuiButtonSpritePropertyKind::SCALE){
        return sprite->scale().x;

    }else if(controlledProperty == GuiButtonSpritePropertyKind::ROTATION){
        return sprite->rotation();

    }else if(controlledProperty == GuiButtonSpritePropertyKind::ALPHA){
        return sprite->alpha();

    }

    return 0.0;
}


void GuiButtonSpriteFloatPropertyController::ApplyPropertyValueToSprite(float _value)
{

    if(controlledProperty == GuiButtonSpritePropertyKind::SCALE){
        sprite->setScale(Vec2f(_value, _value));

    }else if(controlledProperty == GuiButtonSpritePropertyKind::ROTATION){
        //if(settings.IsYCoordinateUp()==false){
        if(settings.yOrientation()==Yorientation::DOWN){
            _value = -_value;
        }
        sprite->setRotation(_value);


    }else if(controlledProperty == GuiButtonSpritePropertyKind::ALPHA){
        sprite->setAlpha(_value);
    }

}


//====================================================================================================


void GuiButtonSpriteBoolPropertyController::SetStartingState(ButtonState _buttonVisualState)
{

    currentValue = GetSpritePropertyValue();
    newValue = propertyDataForStates[(int)_buttonVisualState];

    if(newValue != currentValue){
        ApplyPropertyValueToSprite(newValue);
    }

}


void GuiButtonSpriteBoolPropertyController::Update(ButtonState _buttonVisualState)
{

    oldValue = currentValue;
    currentValue = GetSpritePropertyValue();

    newValue = propertyDataForStates[(int)_buttonVisualState];

    if(newValue != currentValue){
        ApplyPropertyValueToSprite(newValue);
    }

    oldButtonVisualState = _buttonVisualState;

}


bool GuiButtonSpriteBoolPropertyController::GetSpritePropertyValue()
{

    if(controlledProperty == GuiButtonSpritePropertyKind::VISIBILITY){
        return sprite->isVisible();
    }

    return false;
}


void GuiButtonSpriteBoolPropertyController::ApplyPropertyValueToSprite(bool _value)
{

    if(controlledProperty == GuiButtonSpritePropertyKind::VISIBILITY){
        sprite->setVisible(_value);
    }

}


//====================================================================================================


void GuiButtonSpriteIntPropertyController::SetStartingState(ButtonState _buttonVisualState)
{

    currentValue = GetSpritePropertyValue();
    newValue = propertyDataForStates[(int)_buttonVisualState];

    if(newValue != currentValue){
        ApplyPropertyValueToSprite(newValue);
    }
}


void GuiButtonSpriteIntPropertyController::Update(ButtonState _buttonVisualState)
{

    oldValue = currentValue;
    currentValue = GetSpritePropertyValue();

    newValue = propertyDataForStates[(int)_buttonVisualState];

    if(newValue != currentValue){
        ApplyPropertyValueToSprite(newValue);
    }

    oldButtonVisualState = _buttonVisualState;

}


int GuiButtonSpriteIntPropertyController::GetSpritePropertyValue()
{

    if(controlledProperty == GuiButtonSpritePropertyKind::FRAME){

        return static_cast<StandardSprite*>(sprite)->activeImageFrameIndex();

    }else if(controlledProperty == GuiButtonSpritePropertyKind::VISIBILITY){
        return static_cast<int>(sprite->isVisible());
    }


    return 0;
}


void GuiButtonSpriteIntPropertyController::ApplyPropertyValueToSprite(int _value)
{

    if(controlledProperty == GuiButtonSpritePropertyKind::FRAME){
        static_cast<StandardSprite*>(sprite)->setActiveImageByFrameIndex(_value);

    }else if(controlledProperty == GuiButtonSpritePropertyKind::VISIBILITY){
        sprite->setVisible(static_cast<bool>(_value));
    }

}


//====================================================================================================



GuiButtonSpriteColorPropertyData GuiButtonSpriteColorPropertyData::ParseFromString(const std::string &text)
{

    GuiButtonSpriteColorPropertyData fv;

    std::vector<std::string>subparameters = StdString::splitString(text, ",");

    if(subparameters.size()==1){
        fv.value = ColorRGBAf(ColorRGBA::ParseFromHexString(subparameters.front()));

    }else if(subparameters.size()>=4){
        fv.value = ColorRGBAf(ColorRGBA::ParseFromHexString(subparameters[0]));
        fv.valueMin = ColorRGBAf(ColorRGBA::ParseFromHexString(subparameters[1]));
        fv.valueMax = ColorRGBAf(ColorRGBA::ParseFromHexString(subparameters[2]));
        fv.loopDurationMS = 1000 * std::stof(subparameters[3]);

        fv.loopType = LoopType::REVERT_DIRECTION;
        if(subparameters.size()==5){
            std::string loopType = subparameters[4];
            StdString::removeStringWhiteSpacesOnStartAndEnd(loopType);
            if(loopType=="restart"){
                fv.loopType = LoopType::RESTART;
            }
        }
    }

    return fv;
}


void GuiButtonSpriteColorPropertyController::ParseMouseOverTransitionTime(const std::string &text)
{

    std::vector<std::string>subparameters = StdString::splitString(text, ",");

    if(subparameters.size()==2){
        float delta = std::stof(subparameters[0]);
        delta = ClampValue(delta, 0.0f, 1.0f);
        int durationMS = 1000 * std::stof(subparameters[1]);
        mouseOverTransitionSpeed = delta / durationMS;
    }


}


void GuiButtonSpriteColorPropertyController::ParseColorOverlayBlend(std::string text)
{

    StdString::removeStringWhiteSpacesOnStartAndEnd(text);

    if(text=="normal"){
        blend = ColorOverlayBlend::NORMAL;

    }else if(text=="multiply"){
        blend = ColorOverlayBlend::MULTIPLY;

    }else if(text=="linDodge"){
        blend = ColorOverlayBlend::LINEAR_DODGE;

    }else if(text=="color"){
        blend = ColorOverlayBlend::COLOR;

    }

    sprite->setColorOverlayBlend(blend);

}


void GuiButtonSpriteColorPropertyController::SetStartingState(ButtonState _buttonVisualState)
{

    currentValue = GetSpriteOverlayColor();
    newValue = propertyDataForStates[(int)_buttonVisualState].value;
    //newValue = propertyDataForStates[(int)_buttonVisualState]->value;
    targetValue = newValue;

    if(newValue != currentValue){
        ApplyOverlayColorToSprite(newValue);
    }

}


void GuiButtonSpriteColorPropertyController::Update(ButtonState _buttonVisualState)
{


    ColorRGBAf value = propertyDataForStates[(int)_buttonVisualState].value;
    ColorRGBAf valueMin = propertyDataForStates[(int)_buttonVisualState].valueMin;
    ColorRGBAf valueMax = propertyDataForStates[(int)_buttonVisualState].valueMax;
    int loopDurationMS = propertyDataForStates[(int)_buttonVisualState].loopDurationMS;
    GuiButtonSpriteColorPropertyData::LoopType loopType = propertyDataForStates[(int)_buttonVisualState].loopType;
    //int reverseSign = propertyDataForStates[(int)_buttonVisualState]->reverseSign;

    //oldValue = currentValue;
    //currentValue = GetSpriteOverlayColor();

    if(_buttonVisualState != oldButtonVisualState){

        if(_buttonVisualState==ButtonState::CHECKED || _buttonVisualState==ButtonState::CHECKED_PRESSED){
            DummyFunction();
        }

        targetValue = value;

        if(IsGuiButtonStateMouseOver(_buttonVisualState)!=IsGuiButtonStateMouseOver(oldButtonVisualState)){
            if(mouseOverTransitionSpeed>0.0001){
                mouseOverTransition = true;
            }
        }

        reachedR = reachedG = reachedB = reachedA = false;
    }


    //----
    if(mouseOverTransition){

        if(reachedR==false){
            int sign = (currentValue.r <= targetValue.r)? 1 : -1;
            float deltaValueR = time.GetLogicTimeMS() * mouseOverTransitionSpeed * sign;
            newValue.r = currentValue.r + deltaValueR;

            CheckColorReach(newValue.r, targetValue.r, sign, reachedR);
        }


        if(reachedG==false){
            int sign = (currentValue.g <= targetValue.g)? 1 : -1;
            float deltaValueG = time.GetLogicTimeMS() * mouseOverTransitionSpeed * sign;
            newValue.g = currentValue.g + deltaValueG;

            CheckColorReach(newValue.g, targetValue.g, sign, reachedG);
        }


        if(reachedB==false){
            int sign = (currentValue.b <= targetValue.b)? 1 : -1;
            float deltaValueB = time.GetLogicTimeMS() * mouseOverTransitionSpeed * sign;
            newValue.b = currentValue.b + deltaValueB;

            CheckColorReach(newValue.b, targetValue.b, sign, reachedB);
        }


        if(reachedA==false){
            int sign = (currentValue.a <= targetValue.a)? 1 : -1;
            float deltaValueA =  time.GetLogicTimeMS() * mouseOverTransitionSpeed * sign;
            newValue.a = currentValue.a + deltaValueA;

            CheckColorReach(newValue.a, targetValue.a, sign, reachedA);
        }


        if(reachedR && reachedG && reachedB && reachedA){
            newValue = targetValue;
            reachedR = reachedG = reachedB = reachedA = false;
            mouseOverTransition = false;
        }


    }else{

        if(loopDurationMS>0 && valueMax!=valueMin){


            if(keyboard.isKeyPressed(KeyCode::W)){
                DummyFunction();
            }

            if(loopType == GuiButtonSpriteColorPropertyData::LoopType::REVERT_DIRECTION){
            //---------------------------------------------------------------------------------------

                if(reachedR==false){

                    float deltaValueR = reverseSign * time.GetLogicTimeMS() * (valueMax.r - valueMin.r) / loopDurationMS;
                    newValue.r = currentValue.r + deltaValueR;
                    int sign = (deltaValueR>=0)? 1 : -1;

                    CheckColorReach(newValue.r, valueMin.r, valueMax.r, sign, reachedR);
                }


                if(reachedG==false){

                    float deltaValueG = reverseSign *  time.GetLogicTimeMS() * (valueMax.g - valueMin.g) / loopDurationMS;
                    newValue.g = currentValue.g + deltaValueG;
                    int sign = (deltaValueG>=0)? 1 : -1;

                    CheckColorReach(newValue.g, valueMin.g, valueMax.g, sign, reachedG);
                }


                if(reachedB==false){

                    float deltaValueB = reverseSign * time.GetLogicTimeMS() * (valueMax.b - valueMin.b) / loopDurationMS;
                    newValue.b = currentValue.b + deltaValueB;
                    int sign = (deltaValueB>=0)? 1 : -1;

                    CheckColorReach(newValue.b, valueMin.b, valueMax.b, sign, reachedB);
                }

                if(reachedA==false){
                    float deltaValueA =  reverseSign * time.GetLogicTimeMS() * (valueMax.a - valueMin.a) / loopDurationMS;
                    newValue.a = currentValue.a + deltaValueA;
                    int sign = (deltaValueA>=0)? 1 : -1;

                    CheckColorReach(newValue.a, valueMin.a, valueMax.a, sign, reachedA);
                }

                if(reachedR && reachedG && reachedB && reachedA){
                    reverseSign = -reverseSign;
                    reachedR = reachedG = reachedB = reachedA = false;

                    print(" reached all!");
                }


            }else if(loopType == GuiButtonSpriteColorPropertyData::LoopType::RESTART){
            //---------------------------------------------------------------------------------------


                if(reachedR==false){

                    float deltaValueR = time.GetLogicTimeMS() * (valueMax.r - valueMin.r) / loopDurationMS;
                    newValue.r = currentValue.r + deltaValueR;
                    int sign = (deltaValueR>=0)? 1 : -1;

                    /*
                    if(newValue.r <= valueMin.r){
                        newValue.r = valueMin.r;
                        reachedR = true;

                    }else if(newValue.r >= valueMax.r){
                        newValue.r = valueMax.r;
                        reachedR = true;
                    }
                    */

                    CheckColorReach(newValue.r, valueMin.r, valueMax.r, sign, reachedR);
                }


                if(reachedG==false){

                    float deltaValueG = time.GetLogicTimeMS() * (valueMax.g - valueMin.g) / loopDurationMS;
                    newValue.g = currentValue.g + deltaValueG;
                    int sign = (deltaValueG>=0)? 1 : -1;

                    /*
                    if(newValue.g <= valueMin.g){
                        newValue.g = valueMin.g;
                        reachedG = true;

                    }else if(newValue.g >= valueMax.g){
                        newValue.g = valueMax.g;
                        reachedG = true;
                    }
                    */

                    CheckColorReach(newValue.g, valueMin.g, valueMax.g, sign, reachedG);

                }


                if(reachedB==false){

                    float deltaValueB = time.GetLogicTimeMS() * (valueMax.b - valueMin.b) / loopDurationMS;
                    newValue.b = currentValue.b + deltaValueB;
                    int sign = (deltaValueB>=0)? 1 : -1;

                    /*
                    if(newValue.b <= valueMin.b){
                        newValue.b = valueMin.b;
                        reachedB = true;

                    }else if(newValue.b >= valueMax.b){
                        newValue.b = valueMax.b;
                        reachedB = true;
                    }
                    */

                    CheckColorReach(newValue.b, valueMin.b, valueMax.b, sign, reachedB);

                }

                if(reachedA==false){
                    float deltaValueA =  time.GetLogicTimeMS() * (valueMax.a - valueMin.a) / loopDurationMS;
                    newValue.a = currentValue.a + deltaValueA;
                    int sign = (deltaValueA>=0)? 1 : -1;

                    /*
                    if(newValue.a <= valueMin.a){
                        newValue.a = valueMin.a;
                        reachedA = true;

                    }else if(newValue.a >= valueMax.a){
                        newValue.a = valueMax.a;
                        reachedA = true;

                    }
                    */

                    CheckColorReach(newValue.a, valueMin.a, valueMax.a, sign, reachedA);

                }

                if(reachedR && reachedG && reachedB && reachedA){
                    newValue = valueMin;
                    reachedR = reachedG = reachedB = reachedA = false;
                }
            }

        }else{

            newValue = targetValue;
        }
    }


    if(newValue != currentValue){
        ApplyOverlayColorToSprite(newValue);
        currentValue = newValue;
    }

    oldButtonVisualState = _buttonVisualState;

}


void GuiButtonSpriteColorPropertyController::CheckColorReach(float &color, float targetColor, int sign, bool &reached)
{

    if(sign==1){
        if(color >= targetColor){
            color = targetColor;
            reached = true;
        }

    }else{
        if(color <= targetColor){
            color = targetColor;
            reached = true;
        }
    }
}


void GuiButtonSpriteColorPropertyController::CheckColorReach(float &color, float targetColor1, float targetColor2, int sign, bool &reached)
{

    if(sign==1){
        if(targetColor1>=targetColor2){

            if(color >= targetColor1){
                color = targetColor1;
                reached = true;
            }

        }else{

            if(color >= targetColor2){
                color = targetColor2;
                reached = true;
            }

        }

    }else{

        if(targetColor1<=targetColor2){

            if(color <= targetColor1){
                color = targetColor1;
                reached = true;
            }

        }else{

            if(color <= targetColor2){
                color = targetColor2;
                reached = true;
            }
        }
    }
}


ColorRGBAf GuiButtonSpriteColorPropertyController::GetSpriteOverlayColor()
{

    if(controlledProperty == GuiButtonSpritePropertyKind::COLOR_OVERLAY){
        return ColorRGBAf(sprite->overlayColorRGBA());

    }else if(controlledProperty == GuiButtonSpritePropertyKind::LABEL_COLOR){
        if(sprite->type()==SpriteType::TEXT){
            TextSprite *s = static_cast<TextSprite*>(sprite);
            return ColorRGBAf(s->color());
        }
    }

    return ColorRGBAf();
}


void GuiButtonSpriteColorPropertyController::ApplyOverlayColorToSprite(ColorRGBAf _value)
{

    if(controlledProperty == GuiButtonSpritePropertyKind::COLOR_OVERLAY){
        sprite->setColorOverlayRGBA(ColorRGBA(255*_value.r, 255*_value.g, 255*_value.b, 255*_value.a));

    }else if(controlledProperty == GuiButtonSpritePropertyKind::LABEL_COLOR){
        if(sprite->type()==SpriteType::TEXT){
            TextSprite *s = static_cast<TextSprite*>(sprite);
            s->setColor(ColorRGBA(255*_value.r, 255*_value.g, 255*_value.b, 255*_value.a));
        }
    }


}



//====================================================================================================


GuiButton::~GuiButton()
{

    for(GuiButtonSpritePropertyController * c : mSpriteControllers){
        delete c;
    }

}


GuiButton::GuiButton(ButtonCfg* _cfg, Sprite *_rootSprite)  : GuiWidget(_cfg->factory)
{
    mType = WidgetType::BUTTON;
    mCfg = _cfg;
    mRootSprite = _rootSprite;

    //---
    //mSignals.push_back(&mSigHovered);
    //mSignals.push_back(&mSigPressed);
    //mSignals.push_back(&mSigChecked);
    //mSignals.push_back(&mSigState);

}

/*
GuiButton::GuiButton(Sprite *_sprite)
{

    mType = WidgetType::BUTTON;
    mRootSprite = _sprite;

}
*/

bool GuiButton::initConnections(GuiSystem *_guiSystem)
{

    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");

    if(mCfg->name=="radioBoxA"){
        DummyFunction();
    }

    mRootMap = mRootSprite->parentLayer()->rootMap();
    mName = mRootSprite->parameters().value("wName");

    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }

    mCheckable = mCfg->checkable;
    mInitialState = mCfg->initialState;
    if(mRootSprite->parameters().exists("wInitialState")){
        std::string initialStateS = mRootSprite->parameters().value("wInitialState");
        StdString::removeStringWhiteSpacesOnStartAndEnd(initialStateS);
        if(initialStateS=="normal"){
            mInitialState = ButtonState::NORMAL;

        }else if(initialStateS=="checked"){
            mInitialState = ButtonState::CHECKED;

        }else if(initialStateS=="disabled"){
            mInitialState = ButtonState::DISABLED;
        }
    }


    //if(name=="Play"){
    //    DummyFunction();
    //}
    std::string exclusiveGroupName = mCfg->exclusiveGroup;
    if(mRootSprite->parameters().exists("wExclusiveGroup")){
        exclusiveGroupName = mRootSprite->parameters().value("wExclusiveGroup");
    }
    if(exclusiveGroupName.empty()==false){
        mExclusiveGroup = guiButtonExclusiveGroups.FindGroup(exclusiveGroupName);
        if(mExclusiveGroup==nullptr){
            mExclusiveGroup = new GuiButtonExclusiveGroup(exclusiveGroupName);
            guiButtonExclusiveGroups.AddGroup(mExclusiveGroup);
        }
        mExclusiveGroup->AddButton(this);
    }


    //if(_sprite->GetKind()==SpriteKind::TEXT){
    //    static_cast<TextSprite*>(_sprite)->SetClickable(true);
       // DummyFunction();
    //}

    if(mCfg->setSpriteStyleParameters(mRootSprite)==false){
        return false;
    }

    BuildControllersForSprite(mRootSprite);


    std::vector<Sprite*>sprites;

    // controllers for text
    if(mRootSprite->type()==SpriteType::COMPOSITE){

        //--- highlight sprite
        sprites.clear();
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(mRootSprite), sprites, "wHighlightMarker");
        if(sprites.empty()==false){
            mHighlightMarkerSprite = sprites.front();
            mHighlightMarkerSprite->setVisible(false);
        }

        sprites.clear();
        //CollectSpritesWithOrigConstParameter(static_cast<CompositeSprite*>(rootSprite), sprites, "wLabel");
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(mRootSprite), sprites, "wLabel");
        if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){
            mLabelSprite = static_cast<TextSprite*>(sprites.front());
        }

    }else if(mRootSprite->type()==SpriteType::TEXT){
        mLabelSprite = static_cast<TextSprite*>(mRootSprite);
    }

   // mSignalCallbacks.push_back(new SetCheckedButtonSignalCallback(this));
    mSigChecked_setter.setCallback(this);
    mSigDisabled.setCallback(this);
    mSigVisible.setCallback(this);
    IntSignalExtraData *extraData = static_cast<IntSignalExtraData *>(mSigState.createExtraData());
    extraData->namedValues = &gButtonStateNamedValues;

    mInitialized = true;

    dbgSystem.removeLastMessage();
    return true;

}


/*
bool GuiButton::init()
{

    mName = mRootSprite->parameters().value("wName");

    dbgSystem.addMessage("init button '" + mName + "'");

    mTag = mRootSprite->parameters().intValue("wTag");
    mDesignKind = mRootSprite->sourceSprite()->constantParameters().value("wDesign");
    mCheckable = mRootSprite->sourceSprite()->constantParameters().boolValue("wCheckable");


    if(mRootSprite->parameters().exists("wInitialState")){
        std::string initialStateS = mRootSprite->parameters().value("wInitialState");
        StdString::removeStringWhiteSpacesOnStartAndEnd(initialStateS);
        if(initialStateS=="normal"){
            mInitialState = ButtonState::NORMAL;

        }else if(initialStateS=="checked"){
            mInitialState = ButtonState::CHECKED;

        }else if(initialStateS=="disabled"){
            mInitialState = ButtonState::DISABLED;
        }
    }

    //if(name=="Play"){
    //    DummyFunction();
    //}

    if(mRootSprite->parameters().exists("wExclusiveGroup")){
        std::string exclusiveGroupName = mRootSprite->parameters().value("wExclusiveGroup");
        mExclusiveGroup = guiButtonExclusiveGroups.FindGroup(exclusiveGroupName);
        if(mExclusiveGroup==nullptr){
            mExclusiveGroup = new GuiButtonExclusiveGroup(exclusiveGroupName);
            guiButtonExclusiveGroups.AddGroup(mExclusiveGroup);
        }
        mExclusiveGroup->AddButton(this);
    }


    //if(_sprite->GetKind()==SpriteKind::TEXT){
    //    static_cast<TextSprite*>(_sprite)->SetClickable(true);
       // DummyFunction();
    //}
    BuildControllersForSprite(mRootSprite);


    std::vector<Sprite*>sprites;

    // controllers for text
    if(mRootSprite->type()==SpriteType::COMPOSITE){

        //--- highlight sprite
        sprites.clear();
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(mRootSprite), sprites, "wHighlightMarker");
        if(sprites.empty()==false){
            mHighlightMarkerSprite = sprites.front();
            mHighlightMarkerSprite->setVisible(false);
        }

        sprites.clear();
        //CollectSpritesWithOrigConstParameter(static_cast<CompositeSprite*>(rootSprite), sprites, "wLabel");
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(mRootSprite), sprites, "wLabel");
        if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){
            mLabelSprite = static_cast<TextSprite*>(sprites.front());
        }

    }else if(mRootSprite->type()==SpriteType::TEXT){
        mLabelSprite = static_cast<TextSprite*>(mRootSprite);
    }


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();

    return true;
}
*/


void GuiButton::BuildControllersForSprite(Sprite * _sprite)
{


    //if(_sprite->GetKind()==SpriteKind::STANDARD){

        _AddControllersForSprite(_sprite);


    //}else
    if(_sprite->type()==SpriteType::COMPOSITE){

        CompositeSprite *cs = static_cast<CompositeSprite*>(_sprite);

        for(SpriteLayer *sl : cs->map()->spriteLayers()){
            for(Sprite *s : sl->sprites()){
                _AddControllersForSprite(s);

                if(s->type()==SpriteType::COMPOSITE){
                    BuildControllersForSprite(s);
                }
            }
        }
    }
}


void GuiButton::_AddControllersForSprite(Sprite * _sprite)
{

    //std::vector<Parameter>& cp = _sprite->GetSourceSprite()->GetConstantParameters();
    //if(_sprite->sourceSpriteReplacements()){
    //    DummyFunction();
    //}
    //CParameters cp = _sprite->originalSourceSprite()->constantParameters();
    CParameters cp = _sprite->sourceSprite()->constantParameters();

    if(cp.empty()) return;


    std::vector<std::string>propertyNames;
    std::vector<GuiButtonSpritePropertyKind>propertyKinds;


    //---- float properties
    propertyNames = std::vector<std::string>{ "wAlpha", "wScale", "wRotation"};
    propertyKinds = std::vector<GuiButtonSpritePropertyKind>{ GuiButtonSpritePropertyKind::ALPHA, GuiButtonSpritePropertyKind::SCALE, GuiButtonSpritePropertyKind::ROTATION};

    for(int i=0; i<propertyNames.size(); i++){

        const std::string &propertyName = propertyNames[i];
        GuiButtonSpritePropertyKind propertyKind = propertyKinds[i];

        _AddFloatPropertyControllerIfNeeded(_sprite, cp, propertyName, propertyKind);
    }


    //---- boolean properties
    /*
    propertyNames = std::vector<std::string>{ "wVisible"};
    propertyKinds = std::vector<GuiButtonSpritePropertyKind>{ GuiButtonSpritePropertyKind::VISIBILITY};

    for(int i=0; i<propertyNames.size(); i++){

        const std::string &propertyName = propertyNames[i];
        GuiButtonSpritePropertyKind propertyKind = propertyKinds[i];

        _AddBooleanPropertyControllerIfNeeded(_sprite, cp, propertyName, propertyKind);
    }
    */


    //---- int properties
    propertyNames = std::vector<std::string>{ "wFrame", "wVisible" };
    propertyKinds = std::vector<GuiButtonSpritePropertyKind>{ GuiButtonSpritePropertyKind::FRAME, GuiButtonSpritePropertyKind::VISIBILITY};


    for(int i=0; i<propertyNames.size(); i++){

        const std::string &propertyName = propertyNames[i];
        GuiButtonSpritePropertyKind propertyKind = propertyKinds[i];

        if(propertyKind==GuiButtonSpritePropertyKind::FRAME && _sprite->type()!=SpriteType::STANDARD) continue;

        _AddIntPropertyControllerIfNeeded(_sprite, cp, propertyName, propertyKind);

    }


    //---- color properties
    propertyNames = std::vector<std::string>{ "wLabelColor" , "wColorOverlay"};
    propertyKinds = std::vector<GuiButtonSpritePropertyKind>{ GuiButtonSpritePropertyKind::LABEL_COLOR, GuiButtonSpritePropertyKind::COLOR_OVERLAY};


    for(int i=0; i<propertyNames.size(); i++){

        const std::string &propertyName = propertyNames[i];
        GuiButtonSpritePropertyKind propertyKind = propertyKinds[i];

        if(propertyKind==GuiButtonSpritePropertyKind::LABEL_COLOR && _sprite->type()!=SpriteType::TEXT) continue;
        if(propertyKind==GuiButtonSpritePropertyKind::COLOR_OVERLAY && _sprite->type()!=SpriteType::STANDARD) continue;

        _AddColorPropertyControllerIfNeeded(_sprite, cp, propertyName, propertyKind);

    }

}


void GuiButton::_AddFloatPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{


    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteFloatPropertyController * c = new GuiButtonSpriteFloatPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)ButtonState::NORMAL] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
        //c->propertyDataForStates[(int)GuiButtonState::NORMAL] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::NORMAL_MOUSE_OVER] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::NORMAL_MOUSE_DOWN] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::CHECKED] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::CHECKED_MOUSE_OVER] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = c->propertyDataForStates[(int)ButtonState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::CHECKED_MOUSE_DOWN] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED];
        }

        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED];
        }


        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::DISABLED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::DISABLED] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)ButtonState::DISABLED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_transitionDur";
        if(cp.exists(parameterName)){
            c->ParseMouseOverTransitionData(cp.value(parameterName));
        }

        mSpriteControllers.push_back(c);
    }

}


void GuiButton::_AddIntPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{


    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteIntPropertyController * c = new GuiButtonSpriteIntPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)ButtonState::NORMAL] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = c->propertyDataForStates[(int)ButtonState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED];
        }

        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED];
        }

        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::DISABLED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)ButtonState::DISABLED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        mSpriteControllers.push_back(c);
    }

}


void GuiButton::_AddBooleanPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{


    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteBoolPropertyController * c = new GuiButtonSpriteBoolPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)ButtonState::NORMAL] = bool(std::stoi(cp.value(parameterName)));

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = c->propertyDataForStates[(int)ButtonState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED];
        }


        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED];
        }


        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::DISABLED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)ButtonState::DISABLED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        mSpriteControllers.push_back(c);
    }

}


void GuiButton::_AddColorPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{

    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteColorPropertyController * c = new GuiButtonSpriteColorPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)ButtonState::NORMAL] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_PRESSED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED] = c->propertyDataForStates[(int)ButtonState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_PRESSED] = c->propertyDataForStates[(int)ButtonState::CHECKED_HOVERED];
        }

        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HOVERED];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)ButtonState::NORMAL_HIGHLIGHTED];
        }

        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)ButtonState::DISABLED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)ButtonState::DISABLED] = c->propertyDataForStates[(int)ButtonState::NORMAL];
        }

        parameterName = propertyName + "_transitionDur";
        if(cp.exists(parameterName)){
            c->ParseMouseOverTransitionTime(cp.value(parameterName));
        }

        parameterName = propertyName + "_blend";
        if(cp.exists(parameterName)){
            c->ParseColorOverlayBlend(cp.value(parameterName));
        }


        if(propertyKind==GuiButtonSpritePropertyKind::COLOR_OVERLAY){
            mRootSprite->setColorOverlayActive(true);
        }

        mSpriteControllers.push_back(c);
    }

}


int GuiButton::_ParseIntPropertyFromParameter(CParameters &cp, const std::string &parameterName, GuiButtonSpritePropertyKind propertyKind)
{

    if(propertyKind==GuiButtonSpritePropertyKind::LABEL_COLOR){

        std::string hexColorS = cp.value(parameterName);
        StdString::removeStringWhiteSpacesOnStartAndEnd(hexColorS);

        if(hexColorS.length()!=6){
            return 0;

        }else{
            std::string hexR = hexColorS.substr(0,2);
            std::string hexG = hexColorS.substr(2,2);
            std::string hexB = hexColorS.substr(4,2);

            int r = std::stoi(hexR, nullptr, 16);
            int g = std::stoi(hexG, nullptr, 16);
            int b = std::stoi(hexB, nullptr, 16);

            int rgba = r | (g << 8) | (b << 16) | (255 << 24);

            return rgba;
        }

    }else{

        //return std::stoi(Parameter::GetValue(cp, parameterName));
        return cp.intValue(parameterName);
    }

}


void GuiButton::cleanupFlags()
{

    GuiWidget::cleanupFlags();

    if(mState==ButtonState::NORMAL_HOVERED || mState==ButtonState::NORMAL_PRESSED || mState==ButtonState::NORMAL_HIGHLIGHTED){
        mState = ButtonState::NORMAL;

    }else if(mState==ButtonState::CHECKED_HOVERED || mState==ButtonState::CHECKED_PRESSED || mState==ButtonState::CHECKED_HIGHLIGHTED){
        mState = ButtonState::CHECKED;

    }

    for(GuiButtonSpritePropertyController* c : mSpriteControllers){
        c->SetStartingState(mState);
    }

    mSigPressed.reset();
    mSigHovered.reset();
    //mSigState._setFullValue_immediately(static_cast<int>(mState));
    mSigState.reset(static_cast<int>(mState));
    //mSigChecked._setFullValue(mSigChecked.active());
    mSigChecked.reset(mSigChecked.active(true));

}


void GuiButton::setInitialState(ButtonState _initialState)
{
    mInitialState = _initialState;
}


void GuiButton::setToInitialState()
{

    //SetState(initialState);

    mState = mInitialState;

    if(mState==ButtonState::CHECKED && mExclusiveGroup){
        mChecked = true;

        for(GuiButton * b : mExclusiveGroup->GetButtons()){
            if(b==this) continue;

            b->mState = ButtonState::NORMAL;
            for(GuiButtonSpritePropertyController* c : b->mSpriteControllers){
                c->SetStartingState(b->mState);
            }
            b->mChecked = false;
        }
    }


    for(GuiButtonSpritePropertyController* c : mSpriteControllers){
        c->SetStartingState(mState);
    }

    mSigPressed.reset();
    mSigHovered.reset();
    mSigState.reset(static_cast<int>(mState));
    mSigChecked.reset(mChecked);
    mSigChecked_setter.reset(mChecked);

}


void GuiButton::setDisabled(bool _disabled)
{

    if(mDisabled==_disabled) return;

    GuiWidget::setDisabled(_disabled);

    if(mDisabled){
        if(mState != ButtonState::DISABLED){
            mState = ButtonState::DISABLED;
            for(GuiButtonSpritePropertyController* c : mSpriteControllers){
                c->SetStartingState(mState);
            }
        }
    }else{

        if(mState == ButtonState::DISABLED){
            if(mChecked){
                mState = ButtonState::CHECKED;
            }else{
                mState = ButtonState::NORMAL;
            }
            for(GuiButtonSpritePropertyController* c : mSpriteControllers){
                c->SetStartingState(mState);
            }
        }
    }

}


void GuiButton::setVisible(bool _visible)
{

    if(mVisible==_visible) return;

    GuiWidget::setVisible(_visible);

    if(mVisible){
        if(mState==ButtonState::NORMAL_HOVERED || mState==ButtonState::NORMAL_PRESSED){
            mState = ButtonState::NORMAL;
        }else if(mState==ButtonState::CHECKED_HOVERED || mState==ButtonState::CHECKED_PRESSED){
            mState = ButtonState::CHECKED;
        }
        for(GuiButtonSpritePropertyController* c : mSpriteControllers){
            c->SetStartingState(mState);
        }
    }

    mRootSprite->setVisible(mVisible);

}


void GuiButton::SetChecked(bool _checked)
{

    if(mCheckable==false) return;

    mChecked = _checked;

    //set visual state
    if(mDisabled) return;

    if(mChecked){
        if(mState != ButtonState::CHECKED){
            mState = ButtonState::CHECKED;
            for(GuiButtonSpritePropertyController* c : mSpriteControllers){
                c->SetStartingState(mState);
            }
        }

        if(mExclusiveGroup){
            for(GuiButton * b : mExclusiveGroup->GetButtons()){
                if(b==this) continue;

                b->mState = ButtonState::NORMAL;
                for(GuiButtonSpritePropertyController* c : b->mSpriteControllers){
                    c->SetStartingState(b->mState);
                }
                b->mChecked = false;
            }
        }

    }else{

        if(mExclusiveGroup==nullptr){        // It is not possible to set 'checked' to false if button is in a exclusive group !

            if(mState != ButtonState::NORMAL){
                mState = ButtonState::NORMAL;
                for(GuiButtonSpritePropertyController* c : mSpriteControllers){
                    c->SetStartingState(mState);
                }
            }
        }
    }



}


void GuiButton::update()
{

    assert(mInitialized);

    if(mDisabled) return;
    if(mVisible==false) return;

    //---
    //if(mSigChecked.activeChanged(true)){
    //    SetChecked(mSigChecked.value());
    //}

    //----

    mPressedStarted = false;

    GuiCursorDeviceInput &mouseInput = guiCursorDeviceInput;


    mHovered = mRootSprite->pointInside(guiCursorDeviceInput.getCursorInMapPosition());
    if(mHovered== false && guiKeyboardAndJoystickInput.highlightedWidget()==this){
        mHovered = true;
    }

    mPressed = guiCursorDeviceInput.isPressed();
    if(mPressed==false && guiKeyboardAndJoystickInput.highlightedWidget()==this){
        if(guiKeyboardAndJoystickInput.isHighlighedPressed()){
            mPressed = true;
        }
    }

    //if(cursorOver){
    //    DbgOutput("button: " + GetName()+ "  state:" + std::to_string(int(state)));
    //}


    if(mState==ButtonState::NORMAL){

        if(mHovered){
            mState = ButtonState::NORMAL_HOVERED;
        }


    }else if(mState==ButtonState::NORMAL_HOVERED){

        if(mHovered==false){
            mState = ButtonState::NORMAL;

        }else if(mPressed){
            mState = ButtonState::NORMAL_PRESSED;

            if(mCheckable){
                mState = ButtonState::CHECKED_PRESSED;

                if(mExclusiveGroup){
                    for(GuiButton * b : mExclusiveGroup->GetButtons()){
                        if(b==this) continue;

                        b->mState = ButtonState::NORMAL;
                        for(GuiButtonSpritePropertyController* c : b->mSpriteControllers){
                            c->SetStartingState(b->mState);
                        }
                        b->mChecked = false;
                    }
                }

            }
            mPressedStarted = true;
        }


    }else if(mState==ButtonState::NORMAL_PRESSED){

        //if(sprite->PointInside(mousePos)==false){
        //    state = GuiButtonState::NORMAL;
        //    mouse.Reset();

        //}else
        //if(mouse.ButtonDown(MouseButton::LEFT)==false){
        if(mPressed==false){
            mState = ButtonState::NORMAL_HOVERED;
        }


    }else if(mState==ButtonState::CHECKED){

        if(mHovered){
            mState = ButtonState::CHECKED_HOVERED;
        }


    }else if(mState==ButtonState::CHECKED_HOVERED){

        if(mHovered==false){
            mState = ButtonState::CHECKED;

        }else if(mPressed){
            mState = ButtonState::CHECKED_PRESSED;
            if(mCheckable && mExclusiveGroup==nullptr && mForceChecked==false){
                mState = ButtonState::NORMAL_PRESSED;
            }
            mPressedStarted = true;
        }

    }else if(mState==ButtonState::CHECKED_PRESSED){

        //if(sprite->PointInside(mousePos)==false){
        //    state = GuiButtonState::CHECKED;
        //    mouse.Reset();

        //}else
        //if(mouse.ButtonDown(MouseButton::LEFT)==false){
        if(mPressed==false){
            mState = ButtonState::CHECKED_HOVERED;

        }
    }

    //---
    mPressed = (mState==ButtonState::NORMAL_PRESSED || mState==ButtonState::CHECKED_PRESSED);
    mChecked = (mState==ButtonState::CHECKED || mState==ButtonState::CHECKED_HOVERED || mState==ButtonState::CHECKED_PRESSED);


    //---
    for(GuiButtonSpritePropertyController* c : mSpriteControllers){
        c->Update(mState);
    }


    //---
    if(mHovered || mPressed || mPressedStarted){
        GuiWidget::_SetInteractedWidget(this);

        if(mCallbackObj){
            if(mPressedStarted){
                mCallbackObj->OnPressedStarted(this);
            }
            if(mHovered){
                mCallbackObj->OnHovered(this);
            }
            if(mPressed){
                mCallbackObj->OnCursorDown(this);
            }
        }
    }


    //---
    mSigHovered.setValue(mHovered);
    mSigPressed.setValue(mPressed);
    mSigChecked.setValue(mChecked);
    mSigState.setValue(static_cast<int>(mState));

}


void GuiButton::SetTextLabel(const std::string &_label)
{
    if(mLabelSprite){
        mLabelSprite->setText(_label);
    }
}


std::string GuiButton::GetTextLabel()
{

    if(mLabelSprite){
        return mLabelSprite->text();
    }

    return "";
}

/*
void GuiButton::onSetBySignalSetter(Signal *_signal)
{

    if(_signal==&mSigChecked){
        bool value = mSigChecked.value_currentOrNextIfSet();
        SetChecked(value);

    }else if(_signal==&mSigDisabled){
        bool value = mSigDisabled.value_currentOrNextIfSet();
        setDisabled(value);

    }if(_signal==&mSigVisible){
        bool value = mSigVisible.value_currentOrNextIfSet();
        setVisible(value);

    }

}
*/

bool GuiButton::onSignalSet(Signal *_signal)
{

    if(GuiWidget::onSignalSet(_signal)==true){
        return true;
    }

    if(_signal==&mSigChecked_setter){
        bool value = mSigChecked_setter.value();
        SetChecked(value);
        mSigChecked.setValue(value);
        return true;
    }

    return false;
}


void GuiButton::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    GuiWidget::obtainSignal_signalQuery(_signalQuery, _psp, _setErrorMessage);

    if(_signalQuery.signal()){
        return;
    }

    //---
    if(_psp.signalFullName()=="PRESSED"){
        _psp.obtainValue(_signalQuery, &mSigPressed);

    }else if(_psp.signalFullName()=="HOVERED"){
        _psp.obtainValue(_signalQuery, &mSigHovered);

    }else if(_psp.signalFullName()=="CHECKED"){
        _psp.obtainValue(_signalQuery, &mSigChecked);

    }else if(_psp.signalFullName()=="STATE"){
        _psp.obtainValue(_signalQuery, &mSigState);

    }

    if(_setErrorMessage && _signalQuery.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void GuiButton::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    GuiWidget::obtainSignal_signalSetter(_signalSetter, _psp, _setErrorMessage);

    if(_signalSetter.signal()){
        return;
    }


    //---
    if(_psp.signalFullName()=="CHECKED"){
        _psp.obtainValue(_signalSetter, &mSigChecked_setter);
    }

    if(_setErrorMessage && _signalSetter.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


//===================================================================================================


void GuiButtonExclusiveGroup::AddButton(GuiButton *_button)
{

    if(std::find(buttons.begin(), buttons.end(), _button) != buttons.end()) return;
    buttons.push_back(_button);
}



GuiButtonExclusiveGroups::~GuiButtonExclusiveGroups()
{
    for(GuiButtonExclusiveGroup *g : groups){
        delete g;
    }
}


void GuiButtonExclusiveGroups::deleteContent()
{
    for(GuiButtonExclusiveGroup *g : groups){
        delete g;
    }
    groups.clear();
}


GuiButtonExclusiveGroup* GuiButtonExclusiveGroups::FindGroup(const std::string &_name)
{

    for(GuiButtonExclusiveGroup *g : groups){
        if(g->GetName()==_name){
            return g;
        }
    }

    return nullptr;
}


GuiButtonExclusiveGroups guiButtonExclusiveGroups;


//===================================================================================================

/*
void SetCheckedButtonSignalCallback::onSetBySignalSetter(BoolSignal *_signal, bool _value)
{

    button->SetChecked(_value);

}


//========================================================================================
*/


GuiWidgetCfg* ButtonFactory::createWidgetCfg(const std::string &_name)
{
    return new ButtonCfg(this, _name);
}


GuiWidget* ButtonFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    return new GuiButton(static_cast<ButtonCfg*>(_cfg), _rootSprite);
}






}
