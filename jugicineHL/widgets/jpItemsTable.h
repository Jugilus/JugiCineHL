#ifndef PLAYER__WIDGETS_ITEMS_TABLE_H
#define PLAYER__WIDGETS_ITEMS_TABLE_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <string>


#include "jmGuiCommon.h"



namespace jugimap{
	

class VariableManager;
class TriggerVariable;
class GuiButton;
class GuiSlider;
class CompositeSprite;
class Sprite;
class TextSprite;
class GuiTextField;
class GameItem;
class ItemsGroup;
class GuiMultiSlot;
class GuiSlot;




class GuiTable : public GuiWidget
{
public:



    enum class Layout
    {
        NOT_DEFINED,
        VERTICAL,
        HORIZONTAL
    };

    enum class ItemInsertDirection
    {
        NOT_DEFINED,
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        TOP_TO_BOTTOM,
        BOTTOM_TO_TOP
    };

    enum class Kind
    {
        FIXED_SLOTS,
        SCROLLING_SLOTS

    };


    enum class Flags
    {
        NONE,
        SELECT_FIRST_ITEM

    };



    GuiTable(CompositeSprite * _compositeSprite);
    ~GuiTable();

    bool init() override;

    void SetToInitialState() override;
    void Update() override;

    void updateSlots();
    void updateSlotsItems_FixedSlots();
    void updateSlotsItems_ScrollingSlots();
    void updateSlotsItems_NoSlider();

    void updateSlotContentForItem(GameItem* _item);

    void setItemsGroup(ItemsGroup * _itemsGroup);
    ItemsGroup* itemsGroup(){ return mItemsGroup; }

    void removeItem(GameItem *_item);
    void removeAllItems();

    GameItem* selectedItem(){ return mSelectedItem; }
    void setSelectedItem(GameItem *_item);

    void ScrollToTheStartOfItems();
    void ScrollToTheEndOfItems();

    //VariableManager *variables(){ return mVariables; }
    //TriggerVariableManager *triggers(){ return mTriggers; }

private:

    CompositeSprite *mRootSprite = nullptr;           // LINK
    ItemsGroup* mItemsGroup = nullptr;              // LINK
    GameItem* mSelectedItem = nullptr;

    Layout mLayout = Layout::NOT_DEFINED;
    ItemInsertDirection mMainInsertDirection = ItemInsertDirection::NOT_DEFINED;            // inserting multislots into table
    ItemInsertDirection mSecondaryInsertDirection = ItemInsertDirection::NOT_DEFINED;       // inserting slots inside multislot

    Kind mKind = Kind::FIXED_SLOTS;

    std::vector<GuiMultiSlot*>mMultiSlots;                      // OWNED
    GuiSlider *mSlider = nullptr;                     // LINK

    int mNumSlotsInMultiSlot = 0;

    int mNumSlotsHorizontal = 1;
    int mNumSlotsVertical = 1;
    int mSlotsSpacingHorizontal = 0;                       // spacing between multislots
    int mSlotsSpacingVertical = 0;                         // spacing between slots in multislots

    //---
    bool mAutoSelectSingleSlotItem = true;                 // if the table has one slot only, autoselect visible item
    //VariableManager *mVariables;
    //TriggerVariableManager *mTriggers;                  // OWNED
    //TriggerVariable *mTrigger_siDeletableChanged = nullptr;       // LINK for quick access


    void updateSliderValueStep();
    void clearSelectedItem();

};






//-------------------------------------------------------------------------------------


class GuiMultiSlot
{
public:

friend class GuiTable;
    /*
    enum class  SlotsLayout
    {
        HORIZONTAL,
        VERTICAL
    };

    enum class ItemInsertDirection
    {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        TOP_TO_BOTTOM,
        BOTTOM_TO_TOP
    };
    */


    //GuiMultiSlot(GuiTable *_parentTable, int _numSlots, SlotsLayout _slotsLayout, ItemInsertDirection _itemInsertDirection);
    GuiMultiSlot(GuiTable *_parentTable, int _numSlots);
    ~GuiMultiSlot();

    GuiWidget *parentWidget(){ return mParentWidget; }
    std::vector<GuiSlot*>& slots(){ return mSlots; }


private:
    GuiWidget *mParentWidget = nullptr;
    std::vector<GuiSlot*>mSlots;

    Vec2f position;

    //GuiTable::Layout mSlotsLayout = GuiTable::Layout::HORIZONTAL;
    //GuiTable::ItemInsertDirection mItemInsertDirection = GuiTable::ItemInsertDirection::LEFT_TO_RIGHT;

};




class GuiSlot
{
public:


    GuiSlot(GuiMultiSlot* _parentMultislot, CompositeSprite* _rootSprite, GuiButton *_selectingButton);
    bool init();

    ~GuiSlot();


    GuiMultiSlot* parentMultislot(){ return mParentMultislot; }
    CompositeSprite *rootSprite(){ return mRootSprite;}
    GuiButton* selectingButton(){ return mSelectingButton; }


    GameItem* item(){ return mGameItem; }
    void removeItem();
    void addItem(GameItem* _item);
    void updateComponents();

    void _setMarkedForRemoveItem(bool _marked){ mMarkedForRemoveItem = _marked; }
    bool _markedForRemoveItem(){ return mMarkedForRemoveItem; }

private:
    GuiMultiSlot *mParentMultislot = nullptr;                                 // LINK
    CompositeSprite * mRootSprite = nullptr;                            // LINK
    GuiButton* mSelectingButton = nullptr;                              // OWNED

    //---
    std::vector<std::pair<std::string, Sprite*>>mIcSprites;             // LINKS (cache for quicker access)
    std::vector<std::pair<std::string, TextSprite*>>mIcTextSprites;       // LINKS (cache for quicker access)
    std::vector<std::pair<std::string, GuiTextField*>>mIcTextFields;     // OWNED

    //---
    GameItem* mGameItem = nullptr;                                       // LINK to currently stored item or nulltpr if none

    bool mMarkedForRemoveItem = false;

};



struct SelectingSlotCallback : public GuiWidgetCallback
{

    SelectingSlotCallback(GuiSlot* _slot) : mSlot(_slot) {}


    void OnPressed(jugimap::GuiWidget *_widget) override;


    GuiSlot *mSlot = nullptr;

};





}




#endif
