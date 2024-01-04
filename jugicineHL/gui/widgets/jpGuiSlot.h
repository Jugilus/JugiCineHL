#ifndef PLAYER__GUI__SLOT_H
#define PLAYER__GUI__SLOT_H

#include <vector>
#include <string>
#include "jpGuiCommon.h"



namespace pugi{
class xml_node;
}



namespace jugimap{
	

class Sprite;
class CompositeSprite;
class TextSprite;
class GuiButton;
class GuiTextField;
class GuiTableNew;
class PlayedScene;
class VectorShape;
class GuiSlotComponent;
class Item;

class ItemData;
class IntItemData;
class BoolItemData;
class SpriteLayer;
class SpriteData;
class TextData;
class TextSegmentData;

struct GuiSlotNewCfg;



class GuiSlotNew : public GuiWidget
{
public:


    GuiSlotNew(GuiSlotNewCfg* _cfg, CompositeSprite* _rootSprite);
    GuiSlotNew(GuiSlotNewCfg* _cfg, CompositeSprite* _rootSprite, GuiTableNew *_parentTable);
    ~GuiSlotNew() override;

    bool initConnections(GuiSystem * _guiSystem) override;

    void update();

    //GuiMultiSlotNew* parentMultislot(){ return mParentMultislot; }
    CompositeSprite *rootSprite(){ return mRootSprite;}
    GuiButton* selectingButton(){ return mSelectingButton; }
    GuiTableNew* parentTable(){ return mParentTable; }


    Item* item(){ return mItem; }
    void setItem(Item* _item) override;
    void removeItem();
    void updateComponents();

    void _setMarkedForRemoveItem(bool _marked){ mMarkedForRemoveItem = _marked; }
    bool _markedForRemoveItem(){ return mMarkedForRemoveItem; }


private:
    GuiSlotNewCfg *mCfg = nullptr;                        // LINK
    CompositeSprite * mRootSprite = nullptr;                            // LINK
    GuiButton* mSelectingButton = nullptr;                              // OWNED

    GuiTableNew *mParentTable = nullptr;                // set for slots which are members of a table (LINK)

    //---
    std::vector<GuiSlotComponent*>mComponents;

    //---
    Item* mItem = nullptr;                                       // LINK to currently stored item or nulltpr if none
    bool mMarkedForRemoveItem = false;

};


struct SelectingSlotNewCallback : public GuiWidgetCallback
{

    SelectingSlotNewCallback(GuiSlotNew* _slot) : mSlot(_slot) {}


    void OnPressedStarted(jugimap::GuiWidget *_widget) override;


    GuiSlotNew *mSlot = nullptr;

};


//===================================================================================



class GuiSlotNewFactory : public WidgetFactory
{
public:

    GuiSlotNewFactory(const std::string &_typeSignature, WidgetType _type) : WidgetFactory(_typeSignature, _type){}

    GuiWidgetCfg* createWidgetCfg(const std::string &_name) override;
    GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) override;


};





//===================================================================================



enum class GuiSlotComponentType
{
    UNKNOWN,
    SPRITE_POS_MARKER,
    TEXT_SPRITE,
    TEXT_FIELD
};


struct GuiSlotComponentCfg
{

    GuiSlotComponentType mType = GuiSlotComponentType::UNKNOWN;

};


class GuiSlotComponent : public SignalCallback
{
public:

    GuiSlotComponent(GuiSlotNew *_parentSlot, GuiSlotComponentType _type) : mParentSlot(_parentSlot), mType(_type){}
    //GuiSlotComponent(GuiSlotComponent &_src);

    virtual ~GuiSlotComponent(){}

    virtual bool initConnections(PlayedScene * _scene) = 0;

    virtual void updateOnAddedItem(Item *_item) = 0;
    virtual void update() = 0;
    virtual void clear() = 0;


    GuiSlotComponentType type(){ return mType; }
    GuiSlotNew * parentSlot(){ return mParentSlot; }

    bool onDataChanged(ItemData *_data) override;


protected:
    GuiSlotNew *mParentSlot = nullptr;
    GuiSlotComponentType mType = GuiSlotComponentType::UNKNOWN;

};


//=================================================================================


struct GuiSlotSpritePosMarkerCfg : public GuiSlotComponentCfg
{
    std::string pointShape;
    std::string layer;
    std::string itemSpriteData;

};


class GuiSlotSpritePosMarker : public GuiSlotComponent
{
public:

    GuiSlotSpritePosMarker(GuiSlotSpritePosMarkerCfg *_cfg, GuiSlotNew *_parentSlot);

    bool initConnections(PlayedScene *_scene) override;

    void updateOnAddedItem(Item *_item) override;
    void update() override;
    void clear() override;


private:
    GuiSlotSpritePosMarkerCfg *mCfg = nullptr;      // LINK
    VectorShape *mPointShape = nullptr;                  // LINK
    SpriteLayer *mSpriteLayer = nullptr;            // LINK

    //--- data of the item in the slot
    SpriteData *mItemSpriteData = nullptr;          // LINK


};


//=================================================================================


struct GuiSlotTextSpriteCfg : public GuiSlotComponentCfg
{
    std::string sprite;
    std::string itemData;
    std::string digitsWithZero;
};


class GuiSlotTextSprite : public GuiSlotComponent
{
public:

    GuiSlotTextSprite(GuiSlotTextSpriteCfg *_cfg, GuiSlotNew *_parentSlot);

    bool initConnections(PlayedScene *_scene) override;

    void updateOnAddedItem(Item *_item) override;
    void update() override;
    void clear() override;


private:
    GuiSlotTextSpriteCfg *mCfg = nullptr;               // LINK
    TextSprite *mTextSprite = nullptr;                  // LINK

    //--- data of the item in the slot (it can be of different type and it will be converted to string if needed)
    ItemData *mItemData = nullptr;          // LINK

};


//=================================================================================


struct GuiSlotTextFieldCfg : public GuiSlotComponentCfg
{
    std::string textField;
    std::string itemTextSegmentData;

};


class GuiSlotTextField : public GuiSlotComponent
{
public:

    GuiSlotTextField(GuiSlotTextFieldCfg *_cfg, GuiSlotNew *_parentSlot);

    bool initConnections(PlayedScene *_scene) override;

    void updateOnAddedItem(Item *_item) override;
    void update() override;
    void clear() override;


private:
    GuiSlotTextFieldCfg *mCfg = nullptr;               // LINK
    GuiTextField *mTextField = nullptr;                  // LINK

    //--- data of the item in the slot
    TextSegmentData *mItemTextSegmentData = nullptr;          // LINK
};


//=================================================================================


struct GuiSlotCounterCfg : public GuiSlotComponentCfg
{
    std::string textSprite;
    std::string itemData_int;
    //int minNumberOfDigits = 1;

    std::string digitsWithZero;

};




//=================================================================================




struct GuiSlotNewCfg : public GuiWidgetCfg
{

    GuiSlotNewCfg(WidgetFactory *_factory, const std::string &_name) : GuiWidgetCfg(_factory,_name){}

    bool initCfg(const pugi::xml_node &_node) override;


    std::string selectingButton;
    std::string selectingButtonSprite;
    std::vector<GuiSlotSpritePosMarkerCfg>slotSpritePosMarkerCfgs;
    std::vector<GuiSlotTextSpriteCfg>slotTextSpriteCfgs;
    std::vector<GuiSlotTextFieldCfg>slotTextFieldCfgs;
    std::vector<GuiSlotCounterCfg>slotCounterCfgs;

};




}




#endif
