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
#include "jpGuiCommon.h"
#include "jpGuiSystem.h"
#include "jpGuiButton.h"
#include "jpGuiSlider.h"



namespace jugimap {


bool SliderCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="def"){
                    def = a.as_float(def);

                }else if(attributeName=="min"){
                    min = a.as_float(min);

                }else if(attributeName=="max"){
                    max = a.as_float(max);

                }else if(attributeName=="snapDivision"){
                    snapDivision = a.as_int(snapDivision);

                }else if(attributeName=="delta"){
                    deltaValue = a.as_float(def);
                    deltaInPercents = false;

                }else if(attributeName=="deltaPerc"){
                    deltaValue = a.as_float(def);
                    deltaInPercents = true;

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="slidingLine"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    slidingLineSprite = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="slidingButton"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    slidingButtonSprite = a.as_string();

                }else if(attributeName=="button"){
                    slidingButton = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="incrementButton"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    incrementButtonSprite = a.as_string();

                }else if(attributeName=="button"){
                    incrementButton = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="decrementButton"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    decrementButtonSprite = a.as_string();

                }else if(attributeName=="button"){
                    decrementButton = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="label"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="textSprite"){
                    valueLabelTextSprite = a.as_string();

                }else if(attributeName=="displayedPrecision"){
                    valueLabelPrecision = a.as_int();

                }else if(attributeName=="suffix"){
                    valueLabelSuffix = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="highlightSprite"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    highlightSprite = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


// --------------------------------------------------------------------------------------------


GuiSlider::GuiSlider(SliderCfg* _cfg, CompositeSprite *_rootSprite)
{

    mType = WidgetType::SLIDER;
    mCfg = _cfg;
    mRootSprite = _rootSprite;

}

/*
GuiSlider::GuiSlider(CompositeSprite * _sprite)
{

    mType = WidgetType::SLIDER;
    mRootSprite = _sprite;

}
*/

GuiSlider::~GuiSlider()
{

    if(mSlidingButton) delete mSlidingButton;
    if(mIncrementButton) delete mIncrementButton;
    if(mDecrementButton) delete mDecrementButton;

}


bool GuiSlider::initConnections(GuiSystem * _guiSystem)
{

    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");


    mName = mRootSprite->parameters().value("wName");
    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }


    //--- value range
    mValue = mCfg->def;
    mValueMin = mCfg->min;
    mValueMax = mCfg->max;

    std::string valueRange = mRootSprite->sourceSprite()->constantParameters().value("wValueRange");
    std::vector<std::string>values = StdString::splitString(valueRange, ",");

    if(values.size()>=2){
        mValueMin = std::stof(values[0]);
        mValueMax = std::stof(values[1]);
        mValue = (mValueMin+mValueMax)/2.0;
    }
    if(values.size()==3){
        mNumSnapDivisions = std::stoi(values[2]);
    }

    if(mValueMin>mValueMax){
        std::swap(mValueMin, mValueMax);
    }

    //--- value
    if(mRootSprite->parameters().exists("wValue")){
        mValue = mRootSprite->parameters().floatValue("wValue");
    }
    mValue = ClampValue(mValue, mValueMin, mValueMax);


    std::vector<Sprite*>sprites;


    //--- sliding scale sprite
    if(mCfg->slidingLineSprite.empty()==false){

        sprites.clear();
        CollectSpritesWithName(mRootSprite, sprites, mCfg->slidingLineSprite);

        if(sprites.empty()){
            dbgSystem.addMessage("Sliding line sprite '" + mCfg->slidingLineSprite +"' not found!");
            return false;
        }
        if(sprites.front()->type() != SpriteType::STANDARD){
            dbgSystem.addMessage("Sliding line sprite '" + mCfg->slidingLineSprite +"' is not a standard sprite!");
            return false;
        }

        mSlidingScaleSprite = static_cast<StandardSprite*>(sprites.front());

        //--- slider button positions for min and max value
        VectorShape* sh = FindSpriteExtraShapeWithProperties(mSlidingScaleSprite->activeImage(), 1, false, ShapeKind::SINGLE_POINT);
        if(sh==nullptr){
            dbgSystem.addMessage("The sliding line sprite does not have defined minimum position (missing image's 'extra' vector shape point)!");
            return false;
        }

        mSlidingPosMin = sh->singlePointF();
        mSlidingScaleSprite->activeImage()->revertPositioningFromNcineAncher(mSlidingPosMin);
        mSlidingPosMin += mSlidingScaleSprite->position();       // point is 'relative' to sliderScaleSprite, we need position relative to 'sprite'

        sh = FindSpriteExtraShapeWithProperties(mSlidingScaleSprite->activeImage(), 2, false, ShapeKind::SINGLE_POINT);
        if(sh==nullptr){
            dbgSystem.addMessage("The sliding line sprite does not have defined maximum position (missing image's 'extra'  vector shape point)!");
            return false;
        }
        mSlidingPosMax = sh->singlePointF();
        mSlidingScaleSprite->activeImage()->revertPositioningFromNcineAncher(mSlidingPosMax);
        mSlidingPosMax += mSlidingScaleSprite->position();       // point is 'relative' to sliderScaleSprite, we need position relative to 'sprite'
    }



   //--- sliding button
    if(mCfg->slidingButton.empty()==false && mCfg->slidingButtonSprite.empty()==false){

        sprites.clear();
        CollectSpritesWithName(mRootSprite, sprites, mCfg->slidingButtonSprite);
        if(sprites.empty()){
            dbgSystem.addMessage("Sliding button sprite '" + mCfg->slidingButtonSprite +"' not found!");
            return false;
        }

        ButtonCfg * buttonCfg = dynamic_cast<ButtonCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->slidingButton));
        if(buttonCfg==nullptr){
            dbgSystem.addMessage("Sliding button '" + mCfg->slidingButton +"' not found!");
            return false;
        }

        mSlidingButton = new GuiButton(buttonCfg, sprites.front());
        mSlidingButton->setChildWidget(true);
        if(mSlidingButton->initConnections(_guiSystem)==false){
            return false;
        }
        mSlidingButton->setName("slidingButton");
    }



    //--- increment button
    if(mCfg->incrementButton.empty()==false && mCfg->incrementButtonSprite.empty()==false){
        sprites.clear();

        CollectSpritesWithName(mRootSprite, sprites, mCfg->incrementButtonSprite);
        if(sprites.empty()){
            dbgSystem.addMessage("Increment button sprite '" + mCfg->incrementButtonSprite +"' not found!");
            return false;
        }

        ButtonCfg * buttonCfg = dynamic_cast<ButtonCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->incrementButton));
        if(buttonCfg==nullptr){
            dbgSystem.addMessage("Incement button '" + mCfg->incrementButton +"' not found!");
            return false;
        }

        mIncrementButton = new GuiButton(buttonCfg, sprites.front());
        mIncrementButton->setChildWidget(true);
        if(mIncrementButton->initConnections(_guiSystem)==false){
            return false;
        }
        mIncrementButton->setName("incrementButton");
    }


    //--- decrement button
    if(mCfg->decrementButton.empty()==false && mCfg->decrementButtonSprite.empty()==false){
        sprites.clear();

        CollectSpritesWithName(mRootSprite, sprites, mCfg->decrementButtonSprite);
        if(sprites.empty()){
            dbgSystem.addMessage("Decrement button sprite '" + mCfg->decrementButtonSprite +"' not found!");
            return false;
        }

        ButtonCfg * buttonCfg = dynamic_cast<ButtonCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->decrementButton));
        if(buttonCfg==nullptr){
            dbgSystem.addMessage("Decement button '" + mCfg->decrementButton +"' not found!");
            return false;
        }

        mDecrementButton = new GuiButton(buttonCfg, sprites.front());
        mDecrementButton->setChildWidget(true);
        if(mDecrementButton->initConnections(_guiSystem)==false){
            return false;
        }
        mIncrementButton->setName("decrementButton");
    }


    //--- valueLabel
    if(mCfg->valueLabelTextSprite.empty()==false){
        sprites.clear();

        CollectSpritesWithName(mRootSprite, sprites, mCfg->valueLabelTextSprite);
        if(sprites.empty()){
            dbgSystem.addMessage("Value label sprite '" + mCfg->valueLabelTextSprite +"' not found!");
            return false;
        }

        if(sprites.front()->type() != SpriteType::TEXT){
            dbgSystem.addMessage("Value label sprite '" + mCfg->slidingLineSprite +"' is not a text sprite!");
            return false;
        }

        mValueLabel = static_cast<TextSprite*>(sprites.front());
        mValueLabelPrecision = mCfg->valueLabelPrecision;
        mValueLabelSuffix = mCfg->valueLabelSuffix;
        StdString::removeStringWhiteSpacesOnStartAndEnd(mValueLabelSuffix);
    }



    //--- delta value step
    mDeltaValue = mCfg->deltaValue;
    mDeltaValueInPercents = mCfg->deltaInPercents;
    if(mDeltaValueInPercents){
        mDeltaValue /= 100.0;
    }



    //--- highlight sprite
    if(mCfg->highlightSprite.empty()==false){
        sprites.clear();

        CollectSpritesWithName(mRootSprite, sprites, mCfg->highlightSprite);
        if(sprites.empty()){
            dbgSystem.addMessage("Highlight sprite '" + mCfg->highlightSprite +"' not found!");
            return false;
        }

        mHighlightMarkerSprite = sprites.front();
        mHighlightMarkerSprite->setVisible(false);
    }


    //---
    //mSignalCallbacks.push_back(new SetSliderValueSignalCallback(this));
    mSigValue.setCallback(this);
    mSigDisabled.setCallback(this);
    mSigVisible.setCallback(this);


    mInitialized = true;

    dbgSystem.removeLastMessage();

    return true;

}


/*
bool GuiSlider::init()
{

    mName = mRootSprite->parameters().value("wName");

    //---
    dbgSystem.addMessage("init slider '" + mName + "'");

    //---
    mTag = mRootSprite->parameters().intValue("wTag");
    mDesignKind = mRootSprite->sourceSprite()->constantParameters().value("wDesign");


    //--- value range
    std::string valueRange = mRootSprite->sourceSprite()->constantParameters().value("wValueRange");
    std::vector<std::string>values = StdString::splitString(valueRange, ",");

    if(values.size()>=2){
        mValueMin = std::stof(values[0]);
        mValueMax = std::stof(values[1]);
        if(mValueMin>mValueMax){
            std::swap(mValueMin, mValueMax);
        }
        mValue = (mValueMin+mValueMax)/2.0;
    }
    if(values.size()==3){
        mNumSnapDivisions = std::stoi(values[2]);
    }


    //--- value
    if(mRootSprite->parameters().exists("wValue")){
        mValue = mRootSprite->parameters().floatValue("wValue");
    }
    mValue = ClampValue(mValue, mValueMin, mValueMax);


    std::vector<Sprite*>sprites;

    //--- sliding button
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wSlidingButton");
    if(sprites.empty()==false){
        mSlidingButton = new GuiButton(sprites.front());
        if(mSlidingButton->init()==false){
            return false;
        }
        mSlidingButton->setName("slidingButton");
    }

    //--- sliding scale sprite
    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wSlidingScale");
    if(sprites.empty()==false && sprites.front()->type()==SpriteType::STANDARD){
        mSlidingScaleSprite = static_cast<StandardSprite*>(sprites.front());

        //--- slider button positions for min and max value
        VectorShape* sh = FindSpriteExtraShapeWithProperties(mSlidingScaleSprite->activeImage(), 1, false, ShapeKind::SINGLE_POINT);
        if(sh==nullptr){
            dbgSystem.addMessage("The sliding line sprite does not have defined minimum position (missing image's 'extra' vector shape point)!");
            return false;
        }

        mSlidingPosMin = sh->singlePointF();
        mSlidingScaleSprite->activeImage()->revertPositioningFromNcineAncher(mSlidingPosMin);
        mSlidingPosMin += mSlidingScaleSprite->position();       // point is 'relative' to sliderScaleSprite, we need position relative to 'sprite'

        sh = FindSpriteExtraShapeWithProperties(mSlidingScaleSprite->activeImage(), 2, false, ShapeKind::SINGLE_POINT);
        if(sh==nullptr){
            dbgSystem.addMessage("The sliding line sprite does not have defined maximum position (missing image's 'extra'  vector shape point)!");
            return false;
        }
        mSlidingPosMax = sh->singlePointF();
        mSlidingScaleSprite->activeImage()->revertPositioningFromNcineAncher(mSlidingPosMax);
        mSlidingPosMax += mSlidingScaleSprite->position();       // point is 'relative' to sliderScaleSprite, we need position relative to 'sprite'
    }


    //--- increment button
    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wIncrementButton");
    if(sprites.empty()==false){
        mIncrementButton = new GuiButton(sprites.front());
        if(mIncrementButton->init()==false){
            return false;
        }
        mIncrementButton->setName("incrementButton");
    }


    //--- decrement button
    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wDecrementButton");
    if(sprites.empty()==false){
        mDecrementButton = new GuiButton(sprites.front());
        if(mDecrementButton->init()==false){
            return false;
        }
        mDecrementButton->setName("decrementButton");
    }


    //--- valueLabel
    sprites.clear();
    //CollectSpritesWithOrigConstParameter(rootSprite, sprites, "wLabel");
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wLabel");
    if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){

        mValueLabel = static_cast<TextSprite*>(sprites.front());
        mValueLabelPrecision = 0;
        mValueLabelSuffix = "";

        std::string sliderValueLabel =  mValueLabel->sourceSprite()->constantParameters().value("wLabel");
        if(sliderValueLabel!=""){

            values = StdString::splitString(sliderValueLabel, ",");
            std::string labelSpriteName;

            if(values.size()==1){
                mValueLabelPrecision = (int)std::stof(values[0]);

            }else if(values.size()==2){
                mValueLabelPrecision = (int)std::stof(values[0]);
                mValueLabelSuffix = values[1];
                StdString::removeStringWhiteSpacesOnStartAndEnd(mValueLabelSuffix);
            }
        }
    }


    //--- delta value step
    std::string sDeltaValue = mRootSprite->sourceSprite()->constantParameters().value("wDeltaValue");
    if(sDeltaValue!=""){
        StdString::removeStringWhiteSpacesOnStartAndEnd(sDeltaValue);
        if(sDeltaValue.back()=='%'){
            mDeltaValueInPercents = true;
            sDeltaValue = sDeltaValue.substr(0, sDeltaValue.length()-1);

        }else{
            mDeltaValueInPercents = false;
        }


        mDeltaValue = std::stof(sDeltaValue);
        if(mDeltaValueInPercents){
            mDeltaValue /= 100.0;
        }
    }


    //--- highlight sprite
    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wHighlightMarker");
    if(sprites.empty()==false){
        mHighlightMarkerSprite = sprites.front();
        mHighlightMarkerSprite->setVisible(false);
    }


    mInitialized = true;

    dbgSystem.removeLastMessage();

    return true;

}
*/

void GuiSlider::setToInitialState()
{

    if(mSlidingButton) mSlidingButton->setToInitialState();
    if(mIncrementButton) mIncrementButton->setToInitialState();
    if(mDecrementButton) mDecrementButton->setToInitialState();

    SetSnapDivision(mNumSnapDivisions);

    mValue = ClampValue(mValue, mValueMin, mValueMax);

    SetSlidingButtonPositionFromValues();
    SetValueLabelText();

}


void GuiSlider::setDisabled(bool _disabled)
{

    if(mDisabled==_disabled) return;

    GuiWidget::setDisabled(_disabled);

    //--- reset interaction flags
    if(_disabled){
        mValueChangedViaStepButtons = false;
    }

    //---
    if(mIncrementButton){
        mIncrementButton->setDisabled(_disabled);
    }
    if(mDecrementButton){
        mDecrementButton->setDisabled(_disabled);
    }
    if(mSlidingButton){
        mSlidingButton->setDisabled(_disabled);

    }
}


void GuiSlider::setVisible(bool _visible)
{

    if(mVisible==_visible) return;

    GuiWidget::setVisible(_visible);

    //--- reset interaction flags
    if(_visible==false){
        mValueChangedViaStepButtons = false;
    }

    mRootSprite->setVisible(mVisible);
}


void GuiSlider::SetValue(float _value)
{

    mValue = ClampValue(_value, mValueMin, mValueMax);

    //----
    SetSlidingButtonPositionFromValues();
    SetValueLabelText();
}


void GuiSlider::SetValueMin(float _valueMin)
{

    mValueMin = _valueMin;
    mValue = ClampValue(mValue, mValueMin, mValueMax);

    SetSlidingButtonPositionFromValues();
}


void GuiSlider::SetValueMax(float _valueMax)
{

    mValueMax = _valueMax;
    mValue = ClampValue(mValue, mValueMin, mValueMax);

    SetSlidingButtonPositionFromValues();
}


void GuiSlider::SetDeltaValue(float _deltaValue, bool _inPercents)
{
    //mDeltaValue = _deltaValue/100.0; mDeltaValueInPercents = _inPercents;
    mDeltaValue = _deltaValue;
    mDeltaValueInPercents = _inPercents;
}


void GuiSlider::update()
{

    assert(mInitialized);

    if(mDisabled) return;
    if(mVisible==false) return;

    float valueStored = mValue;
    mValueChanged = false;
    mValueChangedViaStepButtons = false;


    GuiCursorDeviceInput &id = guiCursorDeviceInput;


    if(mSlidingButton){

        mSlidingButton->update();

        if(mState==GuiSliderState::IDLE){

            if(mSlidingButton->isPressed()){
                Vec2f buttonPos = mSlidingButton->GetSprite()->position();
                Vec2f localCursorPos = mRootSprite->positionFromAbsPosition(guiCursorDeviceInput.getCursorInMapPosition());

                Vec2f projMousePos;
                distToLine(localCursorPos, mSlidingPosMin, mSlidingPosMax, projMousePos);
                mStartMovingOffset = mSlidingButton->GetSprite()->position() - projMousePos;
                mState = GuiSliderState::MOVING;

                //DbgOutput("GuiSlider START MOVING startMovingOffset x:" + std::to_string(startMovingOffset.x) +"  y:" + std::to_string(startMovingOffset.y));

            }

        }else if(mState==GuiSliderState::MOVING){

            if(mSlidingButton->isPressed()){

                //Vec2f localMousePos = sprite->ConvertFromWorldPos(mousePos);
                Vec2f localCursorPos = mRootSprite->positionFromAbsPosition(guiCursorDeviceInput.getCursorInMapPosition());

                Vec2f projMousePos;
                distToLine(localCursorPos, mSlidingPosMin, mSlidingPosMax, projMousePos);

                mVirtualSlidingPos = projMousePos + mStartMovingOffset;

                mVirtualSlidingPos = GetSnappedPosition(mVirtualSlidingPos);

                mVirtualSlidingPos.x = ClampValue(mVirtualSlidingPos.x, std::min(mSlidingPosMin.x, mSlidingPosMax.x), std::max(mSlidingPosMin.x, mSlidingPosMax.x));
                mVirtualSlidingPos.y = ClampValue(mVirtualSlidingPos.y, std::min(mSlidingPosMin.y, mSlidingPosMax.y), std::max(mSlidingPosMin.y, mSlidingPosMax.y));


                Vec2f buttonPos = mSlidingButton->GetSprite()->position();

                //if(virtualSlidingPos.Equals(buttonPos)==false){
                if(IsEqual(mVirtualSlidingPos, buttonPos)==false){
                    mSlidingButton->GetSprite()->setPosition(mVirtualSlidingPos);
                    SetValueFromSlidingButtonPosition();

                    //DbgOutput("GuiSlider MOVING newButtonPos x:" + std::to_string(virtualSlidingPos.x) +"  y:" + std::to_string(virtualSlidingPos.y));
                }

            }else{
                mState = GuiSliderState::IDLE;
            }
        }

        //---
        if(GuiWidget::GetInteracted() == mSlidingButton){
            GuiWidget::_SetInteractedWidget(this);
        }

    }


    float deltaValueActual = mDeltaValue;
    if(mDeltaValueInPercents){
        deltaValueActual = (mDeltaValue)*(mValueMax-mValueMin);
    }
    if(mNumSnapDivisions>1){
        deltaValueActual = (mValueMax-mValueMin)/mNumSnapDivisions;
    }

    //----
    bool increment = false;
    if(mIncrementButton){
        mIncrementButton->update();
        if(mIncrementButton->isPressedStarted()){
            increment = true;
        }

        //---
        if(GuiWidget::GetInteracted() == mIncrementButton){
            GuiWidget::_SetInteractedWidget(this);
        }
    }

    if(increment==false){
        if(guiKeyboardAndJoystickInput.highlightedWidget()==this){
            if(guiKeyboardAndJoystickInput.isHighlightedChangedValue()==1){
                increment = true;
            }
        }
    }

    if(increment){
        mValue += deltaValueActual;
        mValue = ClampValue(mValue, mValueMin, mValueMax);

        //----
        SetSlidingButtonPositionFromValues();
        SetValueLabelText();

        //---
        //if(interactedGuiWidget.GetWidget() == incrementButton){
            GuiWidget::_SetInteractedWidget(this);
        //}


    }

    //----
    bool decrement = false;
    if(mDecrementButton){
        mDecrementButton->update();
        if(mDecrementButton->isPressedStarted()){
            decrement = true;
        }

        //---
        if(GuiWidget::GetInteracted() == mDecrementButton){
            GuiWidget::_SetInteractedWidget(this);
        }
    }

    if(decrement==false){
        if(guiKeyboardAndJoystickInput.highlightedWidget()==this){
            if(guiKeyboardAndJoystickInput.isHighlightedChangedValue()==-1){
                decrement = true;
            }
        }
    }

    if(decrement){
        mValue -= deltaValueActual;
        mValue = ClampValue(mValue, mValueMin, mValueMax);

        //----
        SetSlidingButtonPositionFromValues();
        SetValueLabelText();

        //---
        //if(interactedGuiWidget.GetWidget() == decrementButton){
            GuiWidget::_SetInteractedWidget(this);
        //}
    }


    //---
    if(mIncrementButton){
        //mIncrementButton->SetDisabled(AreEqual(mValue, mValueMax));
        mIncrementButton->setDisabled(mValue+deltaValueActual>=mValueMax-0.0001);
    }

    if(mDecrementButton){
        mDecrementButton->setDisabled(AreEqual(mValue, mValueMin));
    }


    //----
    if(AreEqual(mValue, valueStored)==false){
        mValueChanged = true;

        mSigValue.setValue(mValue);

        if(increment || decrement){
            mValueChangedViaStepButtons = true ;
        }

        if(mCallbackObj){
            if(mValueChanged){
                mCallbackObj->OnValueChanged(this);
            }
            if(mValueChangedViaStepButtons){
                mCallbackObj->OnValueChangedViaButton(this);
            }
        }
    }

}


void GuiSlider::SetSlidingButtonPositionFromValues()
{
    if(mSlidingButton==nullptr) return;

    mValue = ClampValue(mValue, mValueMin, mValueMax);
    float f = (mValue-mValueMin)/(mValueMax-mValueMin);

    Vec2f buttonPos = mSlidingPosMin + (mSlidingPosMax - mSlidingPosMin) * f;
    Vec2f snappedPos = GetSnappedPosition(buttonPos);

    mSlidingButton->GetSprite()->setPosition(snappedPos);

    //if(snappedPos.Equals(buttonPos)==false){
    if(IsEqual(snappedPos, buttonPos)==false){
        SetValueFromSlidingButtonPosition();
    }
}


void GuiSlider::SetValueFromSlidingButtonPosition()
{

    if(mSlidingButton==nullptr) return;


    Vec2f buttonPos = mSlidingButton->GetSprite()->position();

    float f = DistanceTwoPoints(buttonPos,mSlidingPosMin)/DistanceTwoPoints(mSlidingPosMax,mSlidingPosMin);
    f = ClampValue(f, 0.0f, 1.0f);

    mValue = mValueMin + (mValueMax - mValueMin) * f;

    //----
    SetValueLabelText();
}


void GuiSlider::SetValueLabelText()
{
    if(mValueLabel==nullptr) return;

    //std::string valueText = std::to_string(value);

    std::ostringstream out;
    out.precision(mValueLabelPrecision);
    out << std::fixed << mValue;
    std::string valueText = out.str();
    //DbgOutput("SetValueText value: " + std::to_string(value));
    print("SetValueText value: " + std::to_string(GetIntValue()));


    if(mValueLabelSuffix != ""){
        valueText += " " + mValueLabelSuffix;
    }

    if(valueText != mValueLabel->text()){
        mValueLabel->setText(valueText);
    }

}


void GuiSlider::SetSnapDivision(int _nSnapDivision)
{

    mSnapPositions.clear();

    mNumSnapDivisions = _nSnapDivision;

    if(mNumSnapDivisions>1){
        float length = DistanceTwoPoints(mSlidingPosMin, mSlidingPosMax);
        float step = length/mNumSnapDivisions;

        float dist = 0;
        while(dist<=length+step/2.0){
            Vec2f p = MakeVec2fAtDistanceFromP1(mSlidingPosMin, mSlidingPosMax, dist);
            mSnapPositions.push_back(p);
            dist += step;
        }
    }

}


Vec2f GuiSlider::GetSnappedPosition(Vec2f _pos)
{

    if(mSnapPositions.empty()) return _pos;


    Vec2f snappedPos = _pos;

    float distMin = 100000.0;

    for(Vec2f &p : mSnapPositions){
        float dist = DistanceTwoPoints(_pos, p);
        if(dist<distMin){
            snappedPos = p;
            distMin = dist;
        }
    }

    return snappedPos;
}


/*
void GuiSlider::onSetBySignalSetter(Signal *_signal)
{


    if(_signal==&mSigValue){
        float value = mSigValue.value_currentOrNextIfSet();
        SetValue(value);

    }else if(_signal==&mSigDisabled){
        bool value = mSigDisabled.value_currentOrNextIfSet();
        setDisabled(value);

    }if(_signal==&mSigVisible){
        bool value = mSigVisible.value_currentOrNextIfSet();
        setVisible(value);
    }

}
*/


bool GuiSlider::onSignalSet(Signal *_signal)
{

    if(GuiWidget::onSignalSet(_signal)==true){
        return true;
    }

    if(_signal==&mSigValue){
        float value = mSigValue.value();
        SetValue(value);
        return true;
    }

    return false;

}


void GuiSlider::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    GuiWidget::obtainSignal_signalQuery(_signalQuery, _psp, _setErrorMessage);

    if(_signalQuery.mSignal){
        return;
    }

    //---
    if(_psp.signalFullName()=="VALUE"){
        _psp.obtainValue(_signalQuery, &mSigValue);
    }

    if(_setErrorMessage && _signalQuery.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void GuiSlider::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    GuiWidget::obtainSignal_signalSetter(_signalSetter, _psp, _setErrorMessage);

    if(_signalSetter.mSignal){
        return;
    }


    if(_psp.signalFullName()=="VALUE"){
        _psp.obtainValue(_signalSetter, &mSigValue);
    }

    if(_setErrorMessage && _signalSetter.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }


}

/*
//========================================================================================



void SetSliderValueSignalCallback::onSetBySignalSetter(FloatSignal *_signal, float _value)
{

    slider->SetValue(_value);

}

*/

//========================================================================================


GuiWidgetCfg* SliderFactory::createWidgetCfg(const std::string &_name)
{
    return new SliderCfg(this, _name);
}


GuiWidget* SliderFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    if(_rootSprite->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
                             + _rootSprite->sourceSprite()->name() +"' error !");
        dbgSystem.addMessage("The sprite must be a composite sprite!'");
        return nullptr;
    }

    return new GuiSlider(static_cast<SliderCfg*>(_cfg), static_cast<CompositeSprite*>(_rootSprite));
}



}


