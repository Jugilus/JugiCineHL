#include <assert.h>
#include <string>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmInput.h"
#include "jmStreams.h"
#include "jmUtilities.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmMap.h"
#include "jmText.h"

#include "jpGuiCommon.h"
#include "jpGuiTextInput.h"


namespace jugimap{




bool TextInputCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                return false;
            }


        }else if(nodeName=="backgroundBox"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    backgroundBoxSprite = a.as_string();

                }else if(attributeName=="idleFrame"){
                    idleFrame = a.as_int();

                }else if(attributeName=="cursorOverFrame"){
                    cursorOverFrame = a.as_int();

                }else if(attributeName=="activeFrame"){
                    activeFrame = a.as_int();

                }else if(attributeName=="disabledFrame"){
                    disabledFrame = a.as_int();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="text"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="textSprite"){
                    textSprite = a.as_string();

                }else if(attributeName=="idleColor"){
                    idleTextColor = a.as_string();

                }else if(attributeName=="activeColor"){
                    activeTextColor = a.as_string();

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




//====================================================================================================================

/*
GuiTextInput::GuiTextInput(CompositeSprite *_rootSprite)
{
    mType = WidgetType::TEXT_INPUT;
    mRootSprite = _rootSprite;
}
*/

GuiTextInput::GuiTextInput(TextInputCfg *_cfg, CompositeSprite *_rootSprite)
{
    mType = WidgetType::TEXT_INPUT;
    mCfg = _cfg;
    mRootSprite = _rootSprite;
}


bool GuiTextInput::initConnections(GuiSystem *_guiSystem)
{

    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");


    mName = mRootSprite->parameters().value("wName");
    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }


    std::vector<Sprite*>sprites;

    //---- background box
    CollectSpritesWithName(mRootSprite->map(), sprites, mCfg->backgroundBoxSprite);

    if(sprites.empty()){
        dbgSystem.addMessage("Background box sprite '" + mCfg->backgroundBoxSprite +"' not found!");
        return false;
    }
    if(sprites.front()->type() != SpriteType::STANDARD){
        dbgSystem.addMessage("Background box sprite '" + mCfg->backgroundBoxSprite +"' is not a standard sprite!");
        return false;
    }

    mInputBoxSprite = static_cast<StandardSprite*>(sprites.front());

    int numFrames = mInputBoxSprite->sourceSprite()->sourceImages().size();


    //----
    if(mCfg->idleFrame < 0 || mCfg->idleFrame >= numFrames){
        dbgSystem.addMessage("Idle frame index '" + std::to_string(mCfg->idleFrame) + "' is out of available frames (" +  std::to_string(numFrames) + ")!");
        return false;
    }
    mIndexInputBoxSprite_idle = mCfg->idleFrame;

    //----
    if(mCfg->cursorOverFrame < 0 || mCfg->cursorOverFrame >= numFrames){
        dbgSystem.addMessage("Cursor over frame index '" + std::to_string(mCfg->cursorOverFrame) + "' is out of available frames (" +  std::to_string(numFrames) + ")!");
        return false;
    }
    mIndexInputBoxSprite_CO = mCfg->cursorOverFrame;

    //----
    if(mCfg->activeFrame < 0 || mCfg->activeFrame >= numFrames){
        dbgSystem.addMessage("Active frame index '" + std::to_string(mCfg->activeFrame) + "' is out of available frames (" +  std::to_string(numFrames) + ")!");
        return false;
    }
    mIndexInputBoxSprite_active = mCfg->activeFrame;

    //----
    if(mCfg->disabledFrame < 0 || mCfg->disabledFrame >= numFrames){
        dbgSystem.addMessage("Disabled frame index '" + std::to_string(mCfg->disabledFrame) + "' is out of available frames (" +  std::to_string(numFrames) + ")!");
        return false;
    }
    mIndexInputBoxSprite_disabled = mCfg->disabledFrame;


    //--- text sprite
    sprites.clear();

    CollectSpritesWithName(mRootSprite->map(), sprites, mCfg->textSprite);

    if(sprites.empty()){
        dbgSystem.addMessage("Text sprite '" + mCfg->textSprite +"' not found!");
        return false;
    }
    if(sprites.front()->type() != SpriteType::TEXT){
        dbgSystem.addMessage("Text sprite '" + mCfg->textSprite +"' is not a text sprite!");
        return false;
    }

    mTextSprite = static_cast<TextSprite*>(sprites.front());

    mTextColor_idle = mTextSprite->color();
    mTextColor_active = mTextColor_idle;

    if(mCfg->idleTextColor.empty()==false){
        mTextColor_idle = ColorRGBA::ParseFromHexString(mCfg->idleTextColor);
    }

    if(mCfg->activeTextColor.empty()==false){
        mTextColor_active = ColorRGBA::ParseFromHexString(mCfg->activeTextColor);
    }

    if(mTextSprite->color()!=mTextColor_idle){
        mTextSprite->setColor(mTextColor_idle);
    }
    mTextSprite->setVisible(false);


    //---- cursor
    mCursorSprite = static_cast<TextSprite*>(mTextSprite->makeActiveCopy());
    mCursorSprite->setText("_");

    if(mCursorSprite->color()!=mTextColor_idle){
        mCursorSprite->setColor(mTextColor_idle);
    }
    mCursorSprite->setVisible(false);



    //---
    makeCharsFileNameSafe();

    mInitialized = true;

    dbgSystem.removeLastMessage();

    return true;

}


/*
bool GuiTextInput::init()
{

    mName = mRootSprite->parameters().value("wName");

    dbgSystem.addMessage("init bar '" + mName + "'");

    //---
    mTag = mRootSprite->parameters().intValue("wTag");
    mDesignKind = mRootSprite->sourceSprite()->constantParameters().value("wDesign");

    std::vector<Sprite*>sprites;

    //--- box
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wTextInputBox");
    if(sprites.empty()==false){
        mInputBoxSprite = dynamic_cast<StandardSprite*>(sprites.front());
    }
    if(mInputBoxSprite==nullptr){
        dbgSystem.addMessage("Input box sprite not found (no standard sprite with parameter 'wTextInputBox') !");
        return false;
    }


    if(mInputBoxSprite->sourceSprite()->sourceImages().size()==1){
        mIndexInputBoxSprite_idle = 0;
        mIndexInputBoxSprite_CO = 0;
        mIndexInputBoxSprite_active = 0;
        mIndexInputBoxSprite_disabled = 0;

    }else if(mInputBoxSprite->sourceSprite()->sourceImages().size()==2){
         mIndexInputBoxSprite_idle = 0;
         mIndexInputBoxSprite_CO = 1;
         mIndexInputBoxSprite_active = 1;
         mIndexInputBoxSprite_disabled = 0;

    }else if(mInputBoxSprite->sourceSprite()->sourceImages().size()==3){
        mIndexInputBoxSprite_idle = 0;
        mIndexInputBoxSprite_CO = 1;
        mIndexInputBoxSprite_active = 2;
        mIndexInputBoxSprite_disabled = 0;

    }else if(mInputBoxSprite->sourceSprite()->sourceImages().size()==4){
        mIndexInputBoxSprite_idle = 0;
        mIndexInputBoxSprite_CO = 1;
        mIndexInputBoxSprite_active = 2;
        mIndexInputBoxSprite_disabled = 3;
   }


    //--- text sprite
    sprites.clear();

    CollectSpritesWithConstParameter(mRootSprite, sprites, "wText");
    if(sprites.empty()==false){
        mTextSprite = dynamic_cast<TextSprite*>(sprites.front());

        mTextColor_idle = mTextSprite->color();
        mTextColor_active = mTextColor_idle;

        //std::string sTextColor = textSprite->originalSourceSprite()->constantParameters().value("wTextColor");
        std::string sTextColor = mTextSprite->sourceSprite()->constantParameters().value("wTextColor");
        std::vector<std::string>values = StdString::splitString(sTextColor, ",");
        if(values.size()>=2){
            std::string color1 = values[0];
            StdString::removeStringWhiteSpacesOnStartAndEnd(color1);
            mTextColor_idle = ColorRGBA::ParseFromHexString(color1);

            std::string color2 = values[1];
            StdString::removeStringWhiteSpacesOnStartAndEnd(color2);
            mTextColor_active = ColorRGBA::ParseFromHexString(color2);
        }
    }
    if(mTextSprite==nullptr){
        dbgSystem.addMessage("Text sprite for input text not found (no text sprite with parameter 'wText') !");
        return false;
    }

    if(mTextSprite->color()!=mTextColor_idle){
        mTextSprite->setColor(mTextColor_idle);
    }
    mTextSprite->setVisible(false);


    //--- cursor sprite
    sprites.clear();
    CollectSpritesWithConstParameter(mRootSprite, sprites, "wCursor");
    if(sprites.empty()==false){
        mCursorSprite = dynamic_cast<TextSprite*>(sprites.front());
        if(mCursorSprite==nullptr){
            dbgSystem.addMessage("Text sprite for cursor marker not found (no text sprite with parameter 'wCursor') !");
            return false;
        }

        if(mCursorSprite->color()!=mTextColor_idle){
            mCursorSprite->setColor(mTextColor_idle);
        }
        mCursorSprite->setVisible(false);
    }

    //---
    makeCharsFileNameSafe();


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();

    return true;
}
*/



void GuiTextInput::start(const std::string &_text)
{

    mText = _text;
    start();

}


void GuiTextInput::start()
{

    mDefaultText = mText;

    mTextUnicodes.clear();
    GetStringUnicodes(mText, mTextUnicodes);

    updateTextOnChange();

    //mCursorFlashingStartMS = time.GetPassedNetTimeMS();
    mCursorFlashingTimer.start(mCursorFlashingDurationS, Timer::Mode::AUTO_RESTART);

    if(mTextSprite->isVisible()==false){
        mTextSprite->setVisible(true);
    }
    if(mTextSprite->color()!=mTextColor_active){
        mTextSprite->setColor(mTextColor_active);
    }

    if(mCursorSprite){
        mCursorSprite->setVisible(true);
        if(mCursorSprite->color()!=mTextColor_active){
            mCursorSprite->setColor(mTextColor_active);
        }
    }


    mActive = true;
    keyboard.reset();
}


void GuiTextInput::setText(const std::string &_text)
{

    mText = _text;
    mTextSprite->setText(mText);
    if(mTextSprite->isVisible()==false){
        mTextSprite->setVisible(true);
    }

    if(mActive){
        updateTextOnChange();
    }
}


void GuiTextInput::updateTextOnChange()
{


    if(mText.length()>mMaxLength){
        mText = mText.substr(0, mMaxLength);
    }

    if(mTextSprite->text()!=mText){
        mTextSprite->setText(mText);
    }


    // set position of cursor
    if(mCursorSprite){
        Vec2f cursorPos = mTextSprite->position();
        //cursorPos.x += textSprite->GetWidth();
        cursorPos.x += mTextSprite->width();

        //if(cursorPos.Equals(cursorSprite->GetPosition())==false){
        if(IsEqual(cursorPos, mCursorSprite->position())==false){
            mCursorSprite->setPosition(cursorPos);
        }
    }

}


//void GuiTextInput::setCursorFlashingSpeed(int _cursorFlashingMS)
//{
//    mCursorFlashingMS = _cursorFlashingMS;
//}

void GuiTextInput::setCursorFlashingDuration(float _durationS)
{
    mCursorFlashingDurationS = _durationS;
}


void GuiTextInput::setDisabled(bool _disabled)
{

    GuiWidget::setDisabled(_disabled);

    if(_disabled){
        finish();
        mInputBoxSprite->setActiveImageByFrameIndex(mIndexInputBoxSprite_disabled);
    }else{
        mInputBoxSprite->setActiveImageByFrameIndex(mIndexInputBoxSprite_idle);
    }

}


void GuiTextInput::setVisible(bool _visible)
{
    GuiWidget::setVisible(_visible);

    finish();
    if(mRootSprite->isVisible()!=_visible){
        mRootSprite->setVisible(_visible);
        if(_visible){
            if(mCursorSprite){
                mCursorSprite->setVisible(false);        // adjust cursorSprite
            }
        }
    }
}


void GuiTextInput::update()
{

    assert(mInitialized);

    if(mDisabled) return;
    if(mVisible==false) return;

    mValueChanged = false;


    if(mActive==false){

        mHovered = mInputBoxSprite->pointInside(guiCursorDeviceInput.getCursorInMapPosition());
        if(mHovered== false && guiKeyboardAndJoystickInput.highlightedWidget()==this){
            mHovered = true;
        }

        mPressed = false;
        if(mHovered){
            mPressed = guiCursorDeviceInput.isPressed();
            if(mPressed==false && guiKeyboardAndJoystickInput.highlightedWidget()==this){
                if(guiKeyboardAndJoystickInput.isHighlighedPressed()){
                    mPressed = true;
                }
            }
        }


        if(mHovered){
            //DbgOutput("textInput: " + GetName()+ " cursorOver");
            if(mInputBoxSprite->sourceSprite()->sourceImages().size()>1){
                mInputBoxSprite->setActiveImageByFrameIndex(mIndexInputBoxSprite_CO);
            }
            GuiWidget::_SetInteractedWidget(this);

        }else{
            mInputBoxSprite->setActiveImageByFrameIndex(mIndexInputBoxSprite_idle);
        }

        if(mPressed){
            start();
        }

        return;
    }



    if(keyboard.isKeyPressed(KeyCode::BACKSPACE)){
        //if(mText.length()>0){
        //    mText = mText.substr(0, mText.length()-1);
        //if(mText.length()-nBytesLastCharacter>=0){
            //mText = mText.substr(0, mText.length()-nBytesLastCharacter);

        if(mTextUnicodes.size()>0){
            std::string c = GetUnicodeCharacter(mTextUnicodes.back());
            mText = mText.substr(0, mText.length() - c.size());
            mTextUnicodes.pop_back();
            updateTextOnChange();
        }

    }else if(keyboard.isKeyPressed(KeyCode::RETURN)){
        if(mText != "" || mAllowEmptyText){
            finish();
            mValueChanged = true;
            GuiWidget::_SetInteractedWidget(this);
            if(callback()){
                callback()->OnValueChanged(this);
            }
            return;
        }

    }else if(keyboard.isKeyPressed(KeyCode::ESCAPE)){
        if(mAllowEscToAbort){
            mText = mDefaultText;
            finish();
            return;
        }
    }

    if(GuiWidget::GetInteracted()!=this &&
       (GuiWidget::GetInteracted_Pressed() || GuiWidget::GetInteracted_ValueChanged()))
    {
        // ? Can't hijack interacted widget. I could abort text editing but probably better to just leave it active.
    }


    //----
    if(mText.length() < mMaxLength){

        //char c = keyboard.GetCharPressed();
        std::string c = keyboard.getPressedChar();

        /*
        if(c!=0){
            DummyFunction();

        }


        if(c!=0 && mBannedChars.find(c) != std::string::npos){
            c = 0;
        }


        if(c!=0 && mAllowedAsciCodes.empty()==false){
            if(std::find(mAllowedAsciCodes.begin(), mAllowedAsciCodes.end(), c) == mAllowedAsciCodes.end()) {  // c was not found -> not allowed
                c = 0;
            }
        }

        if(c!=0){
            mText.push_back(c);
            updateTextOnChange();
        }
        */

        if(StdVector::contains(mBannedChars, c)){
            c = "";
        }

        if(c.empty()==false){
            mText += c;
            int unicode = GetCharacterUnicode(c);
            mTextUnicodes.push_back(unicode);

            updateTextOnChange();
        }

    }



    //---- cursor flashing
    //if(time.GetPassedNetTimeMS()- mCursorFlashingStartMS < mCursorFlashingMS){
    //    return;
    //}

    if(mCursorFlashingTimer.update()==Timer::State::INTERVAL_PASSED){
        if(mCursorSprite){
            mCursorSprite->setVisible(!mCursorSprite->isVisible());
        }
    }


   // mCursorFlashingStartMS = time.GetPassedNetTimeMS();


}


void GuiTextInput::makeCharsFileNameSafe()
{

    mBannedChars.empty();

    // it seems these cracters are forbidden for file names
    mBannedChars.emplace_back("/");
    mBannedChars.emplace_back("\\");
    mBannedChars.emplace_back("<");
    mBannedChars.emplace_back(">");
    mBannedChars.emplace_back(":");
    mBannedChars.emplace_back("\"");
    mBannedChars.emplace_back("|");
    mBannedChars.emplace_back("?");
    mBannedChars.emplace_back("*");

    // lets ban some more
    mBannedChars.emplace_back(",");
    mBannedChars.emplace_back(";");
    mBannedChars.emplace_back(".");

}



void GuiTextInput::finish()
{

    mActive = false;


    mInputBoxSprite->setActiveImageByFrameIndex(mIndexInputBoxSprite_idle);

    if(mTextSprite->text()!=mText){
        mTextSprite->setText(mText);
    }
    if(mTextSprite->color()!=mTextColor_idle){
        mTextSprite->setColor(mTextColor_idle);
    }

    if(mCursorSprite){
        mCursorSprite->setVisible(false);
    }

    //Game.BlockKeys = 0;   //important!
    //ccFlushAll();
}




//========================================================================================


GuiWidgetCfg* TextInputFactory::createWidgetCfg(const std::string &_name)
{
    return new TextInputCfg(this, _name);
}


GuiWidget* TextInputFactory::createWidget(GuiWidgetCfg *_cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    if(_rootSprite->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
                             + _rootSprite->sourceSprite()->name() +"' error !");
        dbgSystem.addMessage("The sprite must be a composite sprite!'");
        return nullptr;
    }

    return new GuiTextInput(static_cast<TextInputCfg*>(_cfg), static_cast<CompositeSprite*>(_rootSprite));
}

















}




