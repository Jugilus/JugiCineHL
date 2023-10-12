#include <sstream>
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
#include "jmGuiCommon.h"
#include "jmGuiButton.h"




namespace jugimap {






//====================================================================================================



bool IsGuiButtonStateMouseOver(GuiButtonVisualState _buttonVisualState)
{
    if(_buttonVisualState==GuiButtonVisualState::NORMAL || _buttonVisualState==GuiButtonVisualState::CHECKED || _buttonVisualState==GuiButtonVisualState::DISABLED){
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


void GuiButtonSpriteFloatPropertyController::SetStartingState(GuiButtonVisualState _buttonVisualState)
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


void GuiButtonSpriteFloatPropertyController::Update(GuiButtonVisualState _buttonVisualState)
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


void GuiButtonSpriteBoolPropertyController::SetStartingState(GuiButtonVisualState _buttonVisualState)
{

    currentValue = GetSpritePropertyValue();
    newValue = propertyDataForStates[(int)_buttonVisualState];

    if(newValue != currentValue){
        ApplyPropertyValueToSprite(newValue);
    }

}


void GuiButtonSpriteBoolPropertyController::Update(GuiButtonVisualState _buttonVisualState)
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


void GuiButtonSpriteIntPropertyController::SetStartingState(GuiButtonVisualState _buttonVisualState)
{

    currentValue = GetSpritePropertyValue();
    newValue = propertyDataForStates[(int)_buttonVisualState];

    if(newValue != currentValue){
        ApplyPropertyValueToSprite(newValue);
    }
}


void GuiButtonSpriteIntPropertyController::Update(GuiButtonVisualState _buttonVisualState)
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


void GuiButtonSpriteColorPropertyController::SetStartingState(GuiButtonVisualState _buttonVisualState)
{

    currentValue = GetSpriteOverlayColor();
    newValue = propertyDataForStates[(int)_buttonVisualState].value;
    //newValue = propertyDataForStates[(int)_buttonVisualState]->value;
    targetValue = newValue;

    if(newValue != currentValue){
        ApplyOverlayColorToSprite(newValue);
    }

}


void GuiButtonSpriteColorPropertyController::Update(GuiButtonVisualState _buttonVisualState)
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

        if(_buttonVisualState==GuiButtonVisualState::CHECKED || _buttonVisualState==GuiButtonVisualState::CHECKED_CURSOR_DOWN){
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

    for(GuiButtonSpritePropertyController * c : spriteControllers){
        delete c;
    }
}


GuiButton::GuiButton(Sprite *_sprite)
{

    kind = GuiWidgetKind::BUTTON;
    rootSprite = _sprite;


}


bool GuiButton::init()
{

    name = rootSprite->parameters().value("wName");

    dbgSystem.addMessage("init button '" + name + "'");

    tag = rootSprite->parameters().intValue("wTag");
    designKind = rootSprite->sourceSprite()->constantParameters().value("wDesign");
    checkable = rootSprite->sourceSprite()->constantParameters().boolValue("wCheckable");


    if(rootSprite->parameters().exists("wInitialState")){
        std::string initialStateS = rootSprite->parameters().value("wInitialState");
        StdString::removeStringWhiteSpacesOnStartAndEnd(initialStateS);
        if(initialStateS=="normal"){
            initialState = GuiButtonVisualState::NORMAL;

        }else if(initialStateS=="checked"){
            initialState = GuiButtonVisualState::CHECKED;

        }else if(initialStateS=="disabled"){
            initialState = GuiButtonVisualState::DISABLED;
        }
    }

    //if(name=="Play"){
    //    DummyFunction();
    //}

    if(rootSprite->parameters().exists("wExclusiveGroup")){
        std::string exclusiveGroupName = rootSprite->parameters().value("wExclusiveGroup");
        exclusiveGroup = guiButtonExclusiveGroups.FindGroup(exclusiveGroupName);
        if(exclusiveGroup==nullptr){
            exclusiveGroup = new GuiButtonExclusiveGroup(exclusiveGroupName);
            guiButtonExclusiveGroups.AddGroup(exclusiveGroup);
        }
        exclusiveGroup->AddButton(this);
    }


    //if(_sprite->GetKind()==SpriteKind::TEXT){
    //    static_cast<TextSprite*>(_sprite)->SetClickable(true);
       // DummyFunction();
    //}
    BuildControllersForSprite(rootSprite);


    std::vector<Sprite*>sprites;

    // controllers for text
    if(rootSprite->type()==SpriteType::COMPOSITE){

        //--- highlight sprite
        sprites.clear();
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(rootSprite), sprites, "wHighlightMarker");
        if(sprites.empty()==false){
            highlightMarkerSprite = sprites.front();
            highlightMarkerSprite->setVisible(false);
        }

        sprites.clear();
        //CollectSpritesWithOrigConstParameter(static_cast<CompositeSprite*>(rootSprite), sprites, "wLabel");
        CollectSpritesWithConstParameter(static_cast<CompositeSprite*>(rootSprite), sprites, "wLabel");
        if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){
            labelSprite = static_cast<TextSprite*>(sprites.front());
        }

    }else if(rootSprite->type()==SpriteType::TEXT){
        labelSprite = static_cast<TextSprite*>(rootSprite);
    }


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();

    return true;
}



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
        c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
        //c->propertyDataForStates[(int)GuiButtonState::NORMAL] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::NORMAL_MOUSE_OVER] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::NORMAL_MOUSE_DOWN] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::CHECKED] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::CHECKED_MOUSE_OVER] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::CHECKED_MOUSE_DOWN] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER];
        }

        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED];
        }


        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = GuiButtonSpriteFloatPropertyData::ParseFromString(cp.value(parameterName));
            //c->propertyDataForStates[(int)GuiButtonState::DISABLED] = std::make_shared<GuiButtonSpriteFloatPropertyData>(GuiButtonSpriteFloatPropertyData::ParseFromString(Parameter::Value(cp, parameterName)));

        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_transitionDur";
        if(cp.exists(parameterName)){
            c->ParseMouseOverTransitionData(cp.value(parameterName));
        }

        spriteControllers.push_back(c);
    }

}


void GuiButton::_AddIntPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{


    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteIntPropertyController * c = new GuiButtonSpriteIntPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER];
        }

        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED];
        }

        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = _ParseIntPropertyFromParameter(cp, parameterName, propertyKind);
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        spriteControllers.push_back(c);
    }

}


void GuiButton::_AddBooleanPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{


    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteBoolPropertyController * c = new GuiButtonSpriteBoolPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL] = bool(std::stoi(cp.value(parameterName)));

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER];
        }


        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED];
        }


        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = bool(std::stoi(cp.value(parameterName)));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        spriteControllers.push_back(c);
    }

}


void GuiButton::_AddColorPropertyControllerIfNeeded(Sprite *_sprite, CParameters &cp, const std::string &propertyName, GuiButtonSpritePropertyKind propertyKind)
{

    std::string parameterName = propertyName;

    if(cp.exists(parameterName)){

        GuiButtonSpriteColorPropertyController * c = new GuiButtonSpriteColorPropertyController(_sprite, propertyKind);
        c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));

        parameterName = propertyName + "_CO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_CD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checked";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
        }

        parameterName = propertyName + "_checkedCO";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED];
        }

        parameterName = propertyName + "_checkedCD";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_DOWN] = c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_CURSOR_OVER];
        }

        parameterName = propertyName + "_HL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_CURSOR_OVER];
        }

        parameterName = propertyName + "_checkedHL";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::CHECKED_HIGHLIGHTED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL_HIGHLIGHTED];
        }

        parameterName = propertyName + "_disabled";
        if(cp.exists(parameterName)){
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = GuiButtonSpriteColorPropertyData::ParseFromString(cp.value(parameterName));
        }else{
            c->propertyDataForStates[(int)GuiButtonVisualState::DISABLED] = c->propertyDataForStates[(int)GuiButtonVisualState::NORMAL];
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
            rootSprite->setColorOverlayActive(true);
        }

        spriteControllers.push_back(c);
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


void GuiButton::CleanupFlags()
{

    GuiWidget::CleanupFlags();

    if(state==GuiButtonVisualState::NORMAL_CURSOR_OVER || state==GuiButtonVisualState::NORMAL_CURSOR_DOWN || state==GuiButtonVisualState::NORMAL_HIGHLIGHTED){
        state = GuiButtonVisualState::NORMAL;

    }else if(state==GuiButtonVisualState::CHECKED_CURSOR_OVER || state==GuiButtonVisualState::CHECKED_CURSOR_DOWN || state==GuiButtonVisualState::CHECKED_HIGHLIGHTED){
        state = GuiButtonVisualState::CHECKED;

    }

    for(GuiButtonSpritePropertyController* c : spriteControllers){
        c->SetStartingState(state);
    }
}


void GuiButton::setInitialState(GuiButtonVisualState _initialState)
{
    initialState = _initialState;
}


void GuiButton::SetToInitialState()
{

    //SetState(initialState);

    state = initialState;

    if(state==GuiButtonVisualState::CHECKED && exclusiveGroup){
        checked = true;

        for(GuiButton * b : exclusiveGroup->GetButtons()){
            if(b==this) continue;

            b->state = GuiButtonVisualState::NORMAL;
            for(GuiButtonSpritePropertyController* c : b->spriteControllers){
                c->SetStartingState(b->state);
            }
            b->checked = false;
        }
    }


    for(GuiButtonSpritePropertyController* c : spriteControllers){
        c->SetStartingState(state);
    }

}


void GuiButton::SetDisabled(bool _disabled)
{

    if(disabled==_disabled) return;

    GuiWidget::SetDisabled(_disabled);

    if(disabled){
        if(state != GuiButtonVisualState::DISABLED){
            state = GuiButtonVisualState::DISABLED;
            for(GuiButtonSpritePropertyController* c : spriteControllers){
                c->SetStartingState(state);
            }
        }
    }else{

        if(state == GuiButtonVisualState::DISABLED){
            if(checked){
                state = GuiButtonVisualState::CHECKED;
            }else{
                state = GuiButtonVisualState::NORMAL;
            }
            for(GuiButtonSpritePropertyController* c : spriteControllers){
                c->SetStartingState(state);
            }
        }
    }

}


void GuiButton::SetVisible(bool _visible)
{

    if(visible==_visible) return;

    GuiWidget::SetVisible(_visible);

    if(visible){
        if(state==GuiButtonVisualState::NORMAL_CURSOR_OVER || state==GuiButtonVisualState::NORMAL_CURSOR_DOWN){
            state = GuiButtonVisualState::NORMAL;
        }else if(state==GuiButtonVisualState::CHECKED_CURSOR_OVER || state==GuiButtonVisualState::CHECKED_CURSOR_DOWN){
            state = GuiButtonVisualState::CHECKED;
        }
        for(GuiButtonSpritePropertyController* c : spriteControllers){
            c->SetStartingState(state);
        }
    }

    rootSprite->setVisible(visible);

}


void GuiButton::SetChecked(bool _checked)
{

    if(checkable==false) return;

    checked = _checked;

    //set visual state
    if(disabled) return;

    if(checked){
        if(state != GuiButtonVisualState::CHECKED){
            state = GuiButtonVisualState::CHECKED;
            for(GuiButtonSpritePropertyController* c : spriteControllers){
                c->SetStartingState(state);
            }
        }

        if(exclusiveGroup){
            for(GuiButton * b : exclusiveGroup->GetButtons()){
                if(b==this) continue;

                b->state = GuiButtonVisualState::NORMAL;
                for(GuiButtonSpritePropertyController* c : b->spriteControllers){
                    c->SetStartingState(b->state);
                }
                b->checked = false;
            }
        }

    }else{

        if(exclusiveGroup==nullptr){        // It is not possible to set 'checked' to false if button is in a exclusive group !

            if(state != GuiButtonVisualState::NORMAL){
                state = GuiButtonVisualState::NORMAL;
                for(GuiButtonSpritePropertyController* c : spriteControllers){
                    c->SetStartingState(state);
                }
            }
        }
    }


}


void GuiButton::Update()
{

    assert(mInitialized);

    if(disabled) return;
    if(visible==false) return;


    pressed = false;


    GuiCursorDeviceInput &mouseInput = guiCursorDeviceInput;


    cursorOver = rootSprite->pointInside(guiCursorDeviceInput.GetCursorInMapPosition());
    if(cursorOver== false && guiKeyboardAndJoystickInput.GetHighlightedWidget()==this){
        cursorOver = true;
    }

    cursorDown = guiCursorDeviceInput.IsCursorDown();
    if(cursorDown==false && guiKeyboardAndJoystickInput.GetHighlightedWidget()==this){
        if(guiKeyboardAndJoystickInput.GetHighlighedDown()){
            cursorDown = true;
        }
    }

    //if(cursorOver){
    //    DbgOutput("button: " + GetName()+ "  state:" + std::to_string(int(state)));
    //}


    if(state==GuiButtonVisualState::NORMAL){

        if(cursorOver){
            state = GuiButtonVisualState::NORMAL_CURSOR_OVER;
        }


    }else if(state==GuiButtonVisualState::NORMAL_CURSOR_OVER){

        if(cursorOver==false){
            state = GuiButtonVisualState::NORMAL;

        }else if(cursorDown){
            state = GuiButtonVisualState::NORMAL_CURSOR_DOWN;

            if(checkable){
                state = GuiButtonVisualState::CHECKED_CURSOR_DOWN;

                if(exclusiveGroup){
                    for(GuiButton * b : exclusiveGroup->GetButtons()){
                        if(b==this) continue;

                        b->state = GuiButtonVisualState::NORMAL;
                        for(GuiButtonSpritePropertyController* c : b->spriteControllers){
                            c->SetStartingState(b->state);
                        }
                        b->checked = false;
                    }
                }

            }
            pressed = true;
        }


    }else if(state==GuiButtonVisualState::NORMAL_CURSOR_DOWN){

        //if(sprite->PointInside(mousePos)==false){
        //    state = GuiButtonState::NORMAL;
        //    mouse.Reset();

        //}else
        //if(mouse.ButtonDown(MouseButton::LEFT)==false){
        if(cursorDown==false){
            state = GuiButtonVisualState::NORMAL_CURSOR_OVER;
        }


    }else if(state==GuiButtonVisualState::CHECKED){

        if(cursorOver){
            state = GuiButtonVisualState::CHECKED_CURSOR_OVER;
        }


    }else if(state==GuiButtonVisualState::CHECKED_CURSOR_OVER){

        if(cursorOver==false){
            state = GuiButtonVisualState::CHECKED;

        }else if(cursorDown){
            state = GuiButtonVisualState::CHECKED_CURSOR_DOWN;
            if(checkable && exclusiveGroup==nullptr && mForceChecked==false){
                state = GuiButtonVisualState::NORMAL_CURSOR_DOWN;
            }
            pressed = true;
        }

    }else if(state==GuiButtonVisualState::CHECKED_CURSOR_DOWN){

        //if(sprite->PointInside(mousePos)==false){
        //    state = GuiButtonState::CHECKED;
        //    mouse.Reset();

        //}else
        //if(mouse.ButtonDown(MouseButton::LEFT)==false){
        if(cursorDown==false){
            state = GuiButtonVisualState::CHECKED_CURSOR_OVER;

        }
    }

    //---
    cursorDown = (state==GuiButtonVisualState::NORMAL_CURSOR_DOWN || state==GuiButtonVisualState::CHECKED_CURSOR_DOWN);
    checked = (state==GuiButtonVisualState::CHECKED || state==GuiButtonVisualState::CHECKED_CURSOR_OVER || state==GuiButtonVisualState::CHECKED_CURSOR_DOWN);


    //---
    for(GuiButtonSpritePropertyController* c : spriteControllers){
        c->Update(state);
    }


    //---
    if(cursorOver || cursorDown || pressed){
        GuiWidget::_SetInteractedWidget(this);

        if(callbackObj){
            if(pressed){
                callbackObj->OnPressed(this);
            }
            if(cursorOver){
                callbackObj->OnCursorOver(this);
            }
            if(cursorDown){
                callbackObj->OnCursorDown(this);
            }
        }
    }

}


void GuiButton::SetTextLabel(const std::string &_label)
{
    if(labelSprite){
        labelSprite->setText(_label);
    }
}


std::string GuiButton::GetTextLabel()
{

    if(labelSprite){
        return labelSprite->text();
    }

    return "";
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





}
