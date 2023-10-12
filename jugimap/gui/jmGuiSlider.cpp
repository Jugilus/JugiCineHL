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
#include "jmGuiSlider.h"




namespace jugimap {





GuiSlider::GuiSlider(CompositeSprite * _sprite)
{

    kind = GuiWidgetKind::SLIDER;
    mRootSprite = _sprite;

}


GuiSlider::~GuiSlider()
{

    if(mSlidingButton) delete mSlidingButton;
    if(mIncrementButton) delete mIncrementButton;
    if(mDecrementButton) delete mDecrementButton;

}



bool GuiSlider::init()
{

    name = mRootSprite->parameters().value("wName");

    //---
    dbgSystem.addMessage("init slider '" + name + "'");

    //---
    tag = mRootSprite->parameters().intValue("wTag");
    designKind = mRootSprite->sourceSprite()->constantParameters().value("wDesign");


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
        mSlidingButton->SetName("slidingButton");
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
        mIncrementButton->SetName("incrementButton");
    }


    //--- decrement button
    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wDecrementButton");
    if(sprites.empty()==false){
        mDecrementButton = new GuiButton(sprites.front());
        if(mDecrementButton->init()==false){
            return false;
        }
        mDecrementButton->SetName("decrementButton");
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
        highlightMarkerSprite = sprites.front();
        highlightMarkerSprite->setVisible(false);
    }


    mInitialized = true;

    dbgSystem.removeLastMessage();

    return true;

}


void GuiSlider::SetToInitialState()
{

    if(mSlidingButton) mSlidingButton->SetToInitialState();
    if(mIncrementButton) mIncrementButton->SetToInitialState();
    if(mDecrementButton) mDecrementButton->SetToInitialState();

    SetSnapDivision(mNumSnapDivisions);

    mValue = ClampValue(mValue, mValueMin, mValueMax);

    SetSlidingButtonPositionFromValues();
    SetValueLabelText();

}


void GuiSlider::SetDisabled(bool _disabled)
{

    if(disabled==_disabled) return;

    GuiWidget::SetDisabled(_disabled);

    //--- reset interaction flags
    if(_disabled){
        mValueChangedViaStepButtons = false;
    }

    //---
    if(mIncrementButton){
        mIncrementButton->SetDisabled(_disabled);
    }
    if(mDecrementButton){
        mDecrementButton->SetDisabled(_disabled);
    }
    if(mSlidingButton){
        mSlidingButton->SetDisabled(_disabled);

    }
}


void GuiSlider::SetVisible(bool _visible)
{

    if(visible==_visible) return;

    GuiWidget::SetVisible(_visible);

    //--- reset interaction flags
    if(_visible==false){
        mValueChangedViaStepButtons = false;
    }

    mRootSprite->setVisible(visible);
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


void GuiSlider::Update()
{

    assert(mInitialized);

    if(disabled) return;
    if(visible==false) return;

    float valueStored = mValue;
    valueChanged = false;
    mValueChangedViaStepButtons = false;


    GuiCursorDeviceInput &id = guiCursorDeviceInput;


    if(mSlidingButton){

        mSlidingButton->Update();

        if(mState==GuiSliderState::IDLE){

            if(mSlidingButton->IsCursorDown()){
                Vec2f buttonPos = mSlidingButton->GetSprite()->position();
                Vec2f localCursorPos = mRootSprite->positionFromAbsPosition(guiCursorDeviceInput.GetCursorInMapPosition());

                Vec2f projMousePos;
                distToLine(localCursorPos, mSlidingPosMin, mSlidingPosMax, projMousePos);
                mStartMovingOffset = mSlidingButton->GetSprite()->position() - projMousePos;
                mState = GuiSliderState::MOVING;

                //DbgOutput("GuiSlider START MOVING startMovingOffset x:" + std::to_string(startMovingOffset.x) +"  y:" + std::to_string(startMovingOffset.y));

            }

        }else if(mState==GuiSliderState::MOVING){

            if(mSlidingButton->IsCursorDown()){

                //Vec2f localMousePos = sprite->ConvertFromWorldPos(mousePos);
                Vec2f localCursorPos = mRootSprite->positionFromAbsPosition(guiCursorDeviceInput.GetCursorInMapPosition());

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
        mIncrementButton->Update();
        if(mIncrementButton->IsPressed()){
            increment = true;
        }

        //---
        if(GuiWidget::GetInteracted() == mIncrementButton){
            GuiWidget::_SetInteractedWidget(this);
        }
    }

    if(increment==false){
        if(guiKeyboardAndJoystickInput.GetHighlightedWidget()==this){
            if(guiKeyboardAndJoystickInput.GetHighlightedChangeValue()==1){
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
        mDecrementButton->Update();
        if(mDecrementButton->IsPressed()){
            decrement = true;
        }

        //---
        if(GuiWidget::GetInteracted() == mDecrementButton){
            GuiWidget::_SetInteractedWidget(this);
        }
    }

    if(decrement==false){
        if(guiKeyboardAndJoystickInput.GetHighlightedWidget()==this){
            if(guiKeyboardAndJoystickInput.GetHighlightedChangeValue()==-1){
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
        mIncrementButton->SetDisabled(mValue+deltaValueActual>=mValueMax-0.0001);
    }

    if(mDecrementButton){
        mDecrementButton->SetDisabled(AreEqual(mValue, mValueMin));
    }


    //----
    if(AreEqual(mValue, valueStored)==false){
        valueChanged = true;

        if(increment || decrement){
            mValueChangedViaStepButtons = true ;
        }

        if(callbackObj){
            if(valueChanged){
                callbackObj->OnValueChanged(this);
            }
            if(mValueChangedViaStepButtons){
                callbackObj->OnValueChangedViaButton(this);
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





}


