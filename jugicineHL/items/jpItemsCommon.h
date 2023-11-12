#ifndef PLAYER__ITEMS_COMMON_H
#define PLAYER__ITEMS_COMMON_H

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <array>
#include <string>


#include "jpGlobal.h"


namespace pugi{
class xml_node;
}


namespace jugimap{
	

class Action;
class AppConfigurationLoader_xml;
class PlayedScene;
class BaseObject;
class TextSegment;
class Sprite;
class GameItem;
class GuiSlotDataComponent;
class GuiSlot;
class ItemsGroup;
class GuiSlotData;

class GuiTable;



class GameItem : public BaseObject
{

public:
    friend class AppConfigurationLoader_xml;
    friend class ItemsGroup;


    enum class Properties : int
    {
        NOT_SELECTABLE = 1,
        NOT_DELETABLE = 2

    };


    GameItem(const std::string &_name, ItemsGroup *_parentItemsGroup);
    GameItem(GameItem &_src);
    virtual ~GameItem();


    bool initConnections(PlayedScene* _scene);

    const std::string &name(){ return mName; }

    ItemsGroup* parentItemsGroup(){ return mParentItemsGroup; }
    void _setName(const std::string &_name){ mName = _name; }

    GuiSlotData *slotData(){ return  mGuiSlotData; }

    Action* doOnSelected(){ return mDoOnSelected; }


    int propertyFlags(){ return mPropertyFlags; }
    void _setPropertyFlags(int _propertyFlags){ mPropertyFlags = _propertyFlags; }
    void _createSlotData();

    bool isSelectable(){ return mSelectable; }
    void setSelectable(bool _selectable){ mSelectable = _selectable; }


protected:
    std::string mName;                                  // optional
    ItemsGroup *mParentItemsGroup = nullptr;            // LINK

    //---
    Action* mDoOnSelected = nullptr;                    // OWNED
    GuiSlotData* mGuiSlotData = nullptr;                 // OWNED

    //GuiSlot* mSlot = nullptr;

    int mPropertyFlags = 0;
    //---
    bool mSelectable = true;


};




//-------------------------------------------------------------------------------------


class ItemsGroup
{
public:


    ItemsGroup(const std::string &_name, BaseObject* _parent ): mName(_name), mParent(_parent){}
    ~ItemsGroup();

    bool initConnections(PlayedScene* _scene);


    const std::string &name(){ return mName; }
    BaseObject* parent(){ return mParent; }
    std::vector<GameItem*> &items(){ return  mItems; }
    GameItem* getItem(const std::string &_name);

    //---
    bool deleteItem(const std::string &_name);
    GameItem* addNameItem(const std::string &_name, bool _insertInFront=false);
    GameItem* addItem(GameItem *_item, bool _insertInFront=false);

    //---
    void deleteAllItems();

    //---
    void _setTable(GuiTable *_table){ mGuiTable = _table; }
    GuiTable *table(){ return mGuiTable; }

    int numItemsMin(){ return mNumItemsMin; }
    void _setNumItemsMin(int _numItemsMin){ mNumItemsMin = _numItemsMin; }
    int numItemsMax(){ return mNumItemsMax; }
    void _setNumItemsMax(int _numItemsMax){ mNumItemsMax = _numItemsMax; }


private:
    std::string mName;
    BaseObject* mParent = nullptr;                  // LINK
    std::vector<GameItem*>mItems;                   // OWNED
    GuiTable* mGuiTable = nullptr;                  // LINK  a table where this items group is displayed

    //---
    int mNumItemsMin = 0;
    int mNumItemsMax = 999999;


};


//-------------------------------------------------------------------------------------


class ItemsLibrary
{
public:

    ~ItemsLibrary();


    bool initConnections(PlayedScene* _scene);
    void addItemsGroup(ItemsGroup* _itemsGroup){ mItemsGroups.push_back(_itemsGroup); }

    std::vector<ItemsGroup*>& itemsGroups(){ return mItemsGroups; }
    ItemsGroup* getItemsGroup(const std::string & _name);


private:
    std::vector<ItemsGroup*>mItemsGroups;                   // OWNED
};





}




#endif
