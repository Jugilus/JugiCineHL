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
#include "jmText.h"

#include "gui/widgets/jpGuiBar.h"
#include "gui/widgets/jpGuiTextInput.h"
#include "gui/widgets/jpGuiTable.h"

#include "jpObjectFactory.h"
#include "jpLogicState.h"
#include "jpUtilities.h"
#include "jpItemSlotData.h"
#include "jpItemsCommon.h"


namespace jugimap{





ItemsGroup::~ItemsGroup()
{

    for(GameItem* i : mItems){
        delete i;
    }

    //if(mDynamicItem){
    //    delete mDynamicItem;
    //}

}


bool ItemsGroup::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init items in group '" + mName + "'");

    for(GameItem* i : mItems){
       if(i->initConnections(_scene)==false){
           return false;
       }
    }

    dbgSystem.removeLastMessage();
    return true;
}


GameItem* ItemsGroup::getItem(const std::string &_name)
{
    for(GameItem* i : mItems){
       if(i->name()==_name){
           return i;
       }
    }

    return nullptr;

}



bool ItemsGroup::deleteItem(const std::string &_name)
{

    bool deleted = false;

    for(int i = int(mItems.size())-1; i>=0; i-- ){
        GameItem *item = mItems[i];
        if(item->name()==_name){
            mItems.erase(mItems.begin()+i);

            if(mGuiTable){
                mGuiTable->removeItem(item);
            }

            //---
            delete item;
            deleted = true;
            break;
        }
    }

    if(deleted && mGuiTable){
        mGuiTable->updateSlots();
    }

    return deleted;

}


GameItem *ItemsGroup::addNameItem(const std::string &_name, bool _insertInFront)
{

    GameItem *item = new GameItem(_name, this);
    item->mGuiSlotData = new GuiSlotData(item);

    GuiSlotDataTextSprite* ic = new GuiSlotDataTextSprite(item->mGuiSlotData);
    ic->mText = _name;
    ic->mSlotTextSpriteName = "itemName";

    item->mGuiSlotData->components().push_back(ic);


    //---
    return addItem(item, _insertInFront);
}


GameItem* ItemsGroup::addItem(GameItem *_item, bool _insertInFront)
{

    if(_insertInFront){
        mItems.insert(mItems.begin(), _item);
    }else{
        mItems.push_back(_item);
    }

    if(mGuiTable){
        mGuiTable->updateSlots();
    }

    return _item;
}


void ItemsGroup::deleteAllItems()
{

    if(mGuiTable){
        mGuiTable->removeAllItems();
    }

    for(GameItem *gm : mItems){
        delete gm;
    }
    mItems.clear();


    if(mGuiTable){
        mGuiTable->updateSlots();
    }

}



//---------------------------------------------------------------------------------


ItemsLibrary::~ItemsLibrary()
{

    for(ItemsGroup *ig : mItemsGroups){
        delete  ig;
    }

}


bool ItemsLibrary::initConnections(PlayedScene *_scene)
{

    //--- collect all source sprites intended for items, so we can retrieve them faster
    GuiSlotDataSprite::collectedSourceSpritesForItemSprites.clear();
    CollectSourceSpritesWithConstParameter(GuiSlotDataSprite::collectedSourceSpritesForItemSprites, "itemSprite");


    //---
    for(ItemsGroup *ig : mItemsGroups){
        if(ig->initConnections(_scene)==false){
            return false;
        }
    }

    return true;
}


ItemsGroup* ItemsLibrary::getItemsGroup(const std::string & _name)
{

    for(ItemsGroup *ig : mItemsGroups){
        if(ig->name()==_name){
            return ig;
        }
    }

    return nullptr;
}


//---------------------------------------------------------------------------------


GameItem::GameItem(const std::string &_name, ItemsGroup *_parentItemsGroup) : mName(_name), mParentItemsGroup(_parentItemsGroup)
{

    mBaseType = BaseObjectType::ITEM;


}


GameItem::GameItem(GameItem &_src)
{

    mName = _src.name();
    mParentItemsGroup = _src.mParentItemsGroup;

    if(_src.mDoOnSelected){

    }

    if(_src.mGuiSlotData){
        mGuiSlotData = new GuiSlotData(*_src.mGuiSlotData);
        mGuiSlotData->_setParentItem(this);
    }
}



GameItem::~GameItem()
{

    if(mGuiSlotData){
        delete mGuiSlotData;
    }
    if(mDoOnSelected){
        delete mDoOnSelected;
    }

}


bool GameItem::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init item '" + mName + "'");


    if(mGuiSlotData){
        if(mGuiSlotData->initConnections(_scene)==false){
            return false;
        }
    }

    if(mDoOnSelected){
        if(mDoOnSelected->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


void GameItem::_createSlotData()
{

    mGuiSlotData = new GuiSlotData(this);

}


//---------------------------------------------------------------------------------


/*

GameItem* DynamicItem::createNameItem(const std::string &_name)
{

    GameItem *i = new GameItem(_name, mParentItemsGroup);

}

*/



/*
//---------------------------------------------------------------------------------


GuiSlotData::GuiSlotData(GuiSlotData &_src)
{

    for(GuiSlotComponent* c : _src.mComponents){

        GuiSlotComponent *cCopy = nullptr;

        if(c->mType == GuiSlotComponent::Type::SPRITE){
            cCopy = new GuiSlotSprite(*(static_cast<GuiSlotSprite*>(c)));

        }else if(c->mType == GuiSlotComponent::Type::TEXT_SPRITE){
            cCopy = new GuiSlotTextSprite(*(static_cast<GuiSlotTextSprite*>(c)));

        }else if(c->mType == GuiSlotComponent::Type::TEXT_SEGMENT){
            cCopy = new GuiSlotTextField(*(static_cast<GuiSlotTextField*>(c)));

        }

        if(cCopy){
            cCopy->mParentSlotData = this;
            mComponents.push_back(cCopy);
        }

    }

    // mParentItem is set by item copy constructor
    // mSlot is alink to table slot and should be nullptr

}


bool GuiSlotData::createSlotComponents(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("create items gui slot data for item '" + mParentItem->name() + "'");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        // childNodeName is the type of the item component

        if(nodeName=="sprite"){

            GuiSlotSprite* ic = new GuiSlotSprite(this);
            ic->mSourceSpriteName = n.attribute("sourceSprite").as_string("");

            mComponents.push_back(ic);

        }else if(nodeName=="textSprite"){

            GuiSlotTextSprite* ic = new GuiSlotTextSprite(this);
            ic->mSlotTextSpriteName = n.attribute("slotTextSprite").as_string("");
            ic->mTextSegmentPath = n.attribute("textSegment").as_string("");
            ic->mTextSourcePath =  n.attribute("textSource").as_string("");

            mComponents.push_back(ic);

        }else if(nodeName=="textField"){

            GuiSlotTextField* ic = new GuiSlotTextField(this);

            ic->mSlotTextFieldName = n.attribute("slotTextField").as_string("");
            ic->mTextSegmentPath = n.attribute("textSegment").as_string("");

            mComponents.push_back(ic);

        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool GuiSlotData::initConnections(PlayerScene *_scene)
{

    dbgSystem.addMessage("init slot data for item '" + mParentItem->name() + "'");


    for(GuiSlotComponent * ic : mComponents){

        if(ic->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return  true;
}



//---------------------------------------------------------------------------------


GuiSlotComponent::GuiSlotComponent(GuiSlotComponent &_src)
{

    mType = _src.mType;
    //mParentSlotData = _src.mParentSlotData;

}



std::string GuiSlotComponent::GetTypeString(Type _type)
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


std::vector<SourceSprite*> GuiSlotSprite::collectedSourceSpritesForItemSprites;



GuiSlotSprite::GuiSlotSprite( GuiSlotSprite &_src) : GuiSlotComponent(_src)
{

    mSourceSpriteName = _src.mSourceSpriteName;
    if(_src.mSprite){
       mSprite = _src.mSprite->makePassiveCopy();
    }
    mOwned = _src.mOwned;
    mItemSpriteName = _src.mItemSpriteName;

}


GuiSlotSprite::~GuiSlotSprite()
{

    if(mOwned){
        delete mSprite;
    }

}


bool GuiSlotSprite::initConnections(PlayerScene *_scene)
{
    dbgSystem.addMessage("init '" + GetTypeString(mType) + "'");


    SourceSprite* sourceSprite = ObtainSourceSpriteWithConstantParameter(collectedSourceSpritesForItemSprites, "itemSprite", mItemSpriteName);
    if(sourceSprite==nullptr){
        return false;
    }

    mSprite = Sprite::makePassiveSprite(sourceSprite);


    dbgSystem.removeLastMessage();

    return true;
}


//---------------------------------------------------------------------------------



GuiSlotTextSprite::GuiSlotTextSprite(GuiSlotTextSprite &_src) : GuiSlotComponent(_src)
{

    mSlotTextSpriteName = _src.mSlotTextSpriteName;
    mText = _src.mText;
    //---
    mTextSegmentPath = _src.mTextSegmentPath;
    mTextSourcePath = _src.mTextSourcePath;
}


bool GuiSlotTextSprite::initConnections(PlayerScene *_scene)
{

    dbgSystem.addMessage("init '" + GetTypeString(mType) + "'");


    if(mTextSourcePath.empty()==false){     // used in dynamic items !

        if(mTextSourcePath=="itemName"){
            mText = mParentSlotData->parentItem()->name();

        }else{
            dbgSystem.addMessage("Wrong text source '" + mTextSourcePath +"'");
            return false;
        }


    }else{

        TextSegment * textSegment = ObtainTextSegment(mTextSegmentPath);
        if(textSegment==nullptr){
            return false;
        }

        mText = textSegment->plainText();

    }


    dbgSystem.removeLastMessage();
    return true;
}




//---------------------------------------------------------------------------------



GuiSlotTextField::GuiSlotTextField( GuiSlotTextField &_src) : GuiSlotComponent(_src)
{

    mSlotTextFieldName = _src.mSlotTextFieldName;
    mTextSegment = _src.mTextSegment;
    //---
    mTextSegmentPath = _src.mTextSegmentPath;
}



bool GuiSlotTextField::initConnections(PlayerScene *_scene)
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

*/




}




