#ifndef PLAYER__ITEM_SLOT_DATA_H
#define PLAYER__ITEM_SLOT_DATA_H

#include <vector>
#include <string>



namespace pugi{
class xml_node;
}



namespace jugimap{
	

class AppConfigurationLoader_xml;
class PlayedScene;
class BaseObject;
class TextSegment;
class Sprite;
class SourceSprite;
class GameItem;
class GuiSlotDataComponent;
class GuiSlot;
class ItemsGroup;
class GuiSlotData;




//-------------------------------------------------------------------------------------



class GuiSlotData
{
public:

    GuiSlotData(GameItem* _parentItem) : mParentItem(_parentItem){}
    GuiSlotData(GuiSlotData &_src);

    bool createSlotDataComponents(const pugi::xml_node &_node);
    bool initConnections(PlayedScene *_scene);


    GuiSlot* slot(){ return mSlot; }
    void setSlot(GuiSlot * _slot){ mSlot = _slot; }

    GameItem* parentItem(){ return mParentItem; }
    std::vector<GuiSlotDataComponent*>& components(){ return mComponents; }

    void _setParentItem(GameItem* _parentItem){ mParentItem = _parentItem; }

    GuiSlotDataComponent* getComponent(const std::string &_name);



private:
    GameItem *mParentItem = nullptr;                // LINK
    std::vector<GuiSlotDataComponent*> mComponents;     // OWNED
    //---
    GuiSlot* mSlot = nullptr;                       // LINK  slot where this item is stored, or nullptr if it's not stored in any slot

};


class GuiSlotDataComponent
{
public:

    enum class Type{
        NOT_DEFINED,
        SPRITE,
        TEXT_SPRITE,
        TEXT_SEGMENT
    };
    static std::string GetTypeString(Type _type);



    GuiSlotDataComponent(GuiSlotData *_parentItem) : mParentSlotData(_parentItem){}
    GuiSlotDataComponent(GuiSlotDataComponent &_src);
    //GuiSlotComponent(GuiSlotComponent &_src);


    virtual ~GuiSlotDataComponent(){}

    virtual bool initConnections(PlayedScene * _scene) = 0;

    Type mType = Type::NOT_DEFINED;
    GuiSlotData *mParentSlotData = nullptr;

};



class GuiSlotDataSprite : public GuiSlotDataComponent
{
public:

    enum class Mode
    {
        MODIFY_SLOT_SPRITE,
        REPLACE_SLOT_SPRITE

    };

    static std::vector<SourceSprite*>collectedSourceSpritesForItemSprites;


    GuiSlotDataSprite(GuiSlotData *_parentItem, GuiSlotDataSprite::Mode _mode);
    ~GuiSlotDataSprite();

    bool initConnections(PlayedScene *_scene) override;

    GuiSlotDataSprite(GuiSlotDataSprite &_src);



    Mode mMode = Mode::MODIFY_SLOT_SPRITE;


    std::string mSlotSpriteName;


    //--- mode = REPLACE_SLOT_SPRITE
    std::string mReplacementSourceSpriteName;
    Sprite *mReplacementSprite = nullptr;                  // OWNDED if sprite is not stored in a layer, in that case it is owned by the layer!
    bool mOwned = true;


    //--- mode = MODIFY_SLOT_SPRITE
    bool mVisible = true;
    int mFrame = 0;

    //---
    std::string mItemSpriteName;                // obtain source sprite for 'mSprite' via 'itemSprite' constant parameter
};



class GuiSlotDataTextSprite : public GuiSlotDataComponent
{
public:

    GuiSlotDataTextSprite(GuiSlotData *_parentItem) : GuiSlotDataComponent(_parentItem) { mType = Type::TEXT_SPRITE; }


    bool initConnections(PlayedScene *_scene) override;


    GuiSlotDataTextSprite(GuiSlotDataTextSprite &_src);


    std::string mSlotTextSpriteName;
    std::string mText;

    //---
    std::string mTextSegmentPath;
    //std::string mTextSourcePath;

};



class GuiSlotDataTextField : public GuiSlotDataComponent
{
public:


    GuiSlotDataTextField(GuiSlotData *_parentItem) : GuiSlotDataComponent(_parentItem) { mType = Type::TEXT_SEGMENT; }
    GuiSlotDataTextField(GuiSlotDataTextField &_src);

    bool initConnections(PlayedScene *_scene) override;




    std::string mSlotTextFieldName;
    TextSegment *mTextSegment = nullptr;        // LINK

    //---
    std::string mTextSegmentPath;


};





}




#endif
