#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>
#include <fstream>
#include <string>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmInput.h"
#include "jmStreams.h"
#include "jmFont.h"
#include "jmUtilities.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceSprite.h"
#include "jmMap.h"
#include "jmSpriteLayer.h"
#include "jmGuiBar.h"
#include "jmGuiText.h"
#include "jmGuiTextInput.h"
#include "jpObjectFactory.h"
#include "jpActionsCommon.h"
#include "jpUtilities.h"
#include "jpItemsCommon.h"
#include "jpItemSlotData.h"


namespace jugimap{





GuiSlotData::GuiSlotData(GuiSlotData &_src)
{

    for(GuiSlotDataComponent* c : _src.mComponents){

        GuiSlotDataComponent *cCopy = nullptr;

        if(c->mType == GuiSlotDataComponent::Type::SPRITE){
            cCopy = new GuiSlotDataSprite(*(static_cast<GuiSlotDataSprite*>(c)));

        }else if(c->mType == GuiSlotDataComponent::Type::TEXT_SPRITE){
            cCopy = new GuiSlotDataTextSprite(*(static_cast<GuiSlotDataTextSprite*>(c)));

        }else if(c->mType == GuiSlotDataComponent::Type::TEXT_SEGMENT){
            cCopy = new GuiSlotDataTextField(*(static_cast<GuiSlotDataTextField*>(c)));

        }

        if(cCopy){
            cCopy->mParentSlotData = this;
            mComponents.push_back(cCopy);
        }

    }

    // mParentItem is set by item copy constructor
    // mSlot is alink to table slot and should be nullptr

}


bool GuiSlotData::createSlotDataComponents(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("create items gui slot data for item '" + mParentItem->name() + "'");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        // childNodeName is the type of the item component



        if(nodeName=="sprite"){

            std::string spriteMode = n.attribute("mode").as_string("");

            GuiSlotDataSprite* ic = nullptr;

            if(spriteMode=="modify"){
                ic = new GuiSlotDataSprite(this, GuiSlotDataSprite::Mode::MODIFY_SLOT_SPRITE);

            }else if(spriteMode=="replace"){
                ic = new GuiSlotDataSprite(this, GuiSlotDataSprite::Mode::REPLACE_SLOT_SPRITE);
            }

            ic->mReplacementSourceSpriteName = n.attribute("sourceSprite").as_string("");

            mComponents.push_back(ic);

        }else if(nodeName=="textSprite"){

            GuiSlotDataTextSprite* ic = new GuiSlotDataTextSprite(this);
            ic->mSlotTextSpriteName = n.attribute("slotTextSprite").as_string("");
            ic->mTextSegmentPath = n.attribute("textSegment").as_string("");
            //ic->mTextSourcePath =  n.attribute("textSource").as_string("");

            mComponents.push_back(ic);

        }else if(nodeName=="textField"){

            GuiSlotDataTextField* ic = new GuiSlotDataTextField(this);

            ic->mSlotTextFieldName = n.attribute("slotTextField").as_string("");
            ic->mTextSegmentPath = n.attribute("textSegment").as_string("");

            mComponents.push_back(ic);

        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool GuiSlotData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init slot data for item '" + mParentItem->name() + "'");


    for(GuiSlotDataComponent * ic : mComponents){

        if(ic->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return  true;
}


GuiSlotDataComponent* GuiSlotData::getComponent(const std::string &_name)
{

    for(GuiSlotDataComponent *c : mComponents){
        if(c->mType==GuiSlotDataComponent::Type::SPRITE){
            if(static_cast<GuiSlotDataSprite*>(c)->mSlotSpriteName == _name){
                return c;
            }

        }else if(c->mType==GuiSlotDataComponent::Type::TEXT_SPRITE){
            if(static_cast<GuiSlotDataTextSprite*>(c)->mSlotTextSpriteName == _name){
                return c;
            }

        }else if(c->mType==GuiSlotDataComponent::Type::TEXT_SEGMENT){
            if(static_cast<GuiSlotDataTextField*>(c)->mSlotTextFieldName == _name){
                return c;
            }
        }
    }

    return nullptr;
}


//---------------------------------------------------------------------------------


GuiSlotDataComponent::GuiSlotDataComponent(GuiSlotDataComponent &_src)
{

    mType = _src.mType;
    //mParentSlotData = _src.mParentSlotData;

}



std::string GuiSlotDataComponent::GetTypeString(Type _type)
{

    if(_type == Type::SPRITE){
        return "sprite";

    }else if(_type == Type::TEXT_SPRITE){
        return "textSprite";

    }if(_type == Type::TEXT_SEGMENT){
        return "textSegment";

    }

    return "";

}


//---------------------------------------------------------------------------------


std::vector<SourceSprite*> GuiSlotDataSprite::collectedSourceSpritesForItemSprites;


GuiSlotDataSprite::GuiSlotDataSprite(GuiSlotData *_parentItem, GuiSlotDataSprite::Mode _mode) : GuiSlotDataComponent(_parentItem)
{
    mType = Type::SPRITE;
    mMode = _mode;
}


GuiSlotDataSprite::GuiSlotDataSprite( GuiSlotDataSprite &_src) : GuiSlotDataComponent(_src)
{

    mReplacementSourceSpriteName = _src.mReplacementSourceSpriteName;
    if(_src.mReplacementSprite){
       mReplacementSprite = _src.mReplacementSprite->makePassiveCopy();
    }
    mOwned = _src.mOwned;
    mItemSpriteName = _src.mItemSpriteName;

}


GuiSlotDataSprite::~GuiSlotDataSprite()
{

    if(mReplacementSprite && mOwned){
        delete mReplacementSprite;
    }

}


bool GuiSlotDataSprite::initConnections(PlayedScene *_scene)
{
    dbgSystem.addMessage("init '" + GetTypeString(mType) + "'");

    if(mMode==Mode::REPLACE_SLOT_SPRITE){
        SourceSprite* sourceSprite = ObtainSourceSpriteWithConstantParameter(collectedSourceSpritesForItemSprites, "itemSprite", mItemSpriteName);
        if(sourceSprite==nullptr){
            return false;
        }

        mReplacementSprite = Sprite::makePassiveSprite(sourceSprite);
    }




    dbgSystem.removeLastMessage();

    return true;
}


//---------------------------------------------------------------------------------



GuiSlotDataTextSprite::GuiSlotDataTextSprite(GuiSlotDataTextSprite &_src) : GuiSlotDataComponent(_src)
{

    mSlotTextSpriteName = _src.mSlotTextSpriteName;
    mText = _src.mText;
    //---
    mTextSegmentPath = _src.mTextSegmentPath;
    //mTextSourcePath = _src.mTextSourcePath;
}


bool GuiSlotDataTextSprite::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + GetTypeString(mType) + "'");

    /*
    if(mTextSourcePath.empty()==false){     // used in dynamic items !

        if(mTextSourcePath=="itemName"){
            mText = mParentSlotData->parentItem()->name();

        }else{
            dbgSystem.addMessage("Wrong text source '" + mTextSourcePath +"'");
            return false;
        }


    }else{
    */

        TextSegment * textSegment = ObtainTextSegment(mTextSegmentPath);
        if(textSegment==nullptr){
            return false;
        }

        mText = textSegment->plainText();

    //}


    dbgSystem.removeLastMessage();
    return true;
}




//---------------------------------------------------------------------------------



GuiSlotDataTextField::GuiSlotDataTextField( GuiSlotDataTextField &_src) : GuiSlotDataComponent(_src)
{

    mSlotTextFieldName = _src.mSlotTextFieldName;
    mTextSegment = _src.mTextSegment;
    //---
    mTextSegmentPath = _src.mTextSegmentPath;
}



bool GuiSlotDataTextField::initConnections(PlayedScene *_scene)
{
    dbgSystem.addMessage("init '" + GetTypeString(mType) + "'");

    //---
    mTextSegment = ObtainTextSegment(mTextSegmentPath);
    if(mTextSegment==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;
}






}




