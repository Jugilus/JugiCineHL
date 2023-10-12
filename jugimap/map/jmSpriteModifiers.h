#ifndef JM_SPRITE_MODIFIERS_H
#define JM_SPRITE_MODIFIERS_H

#include <string>
#include <vector>



namespace jugimap {


/*
struct CParameter;


enum class SpriteModifierType
{
    CHILD_TEXT_SPRITE__CHANGE_TEXT = 1
};




class SpriteModifier
{
public:

    virtual ~SpriteModifier(){}

    SpriteModifierType type(){ return mType; }


protected:

    SpriteModifierType mType;


};

*/


struct SM_ChangeChildText
{
public:
    static std::string name;


    SM_ChangeChildText(const std::string &pValue);


    bool isValid(){ return !childSpriteTag.empty(); }

    std::string obtainText();



    std::string childSpriteTag;
    std::string text;
    std::string bookName;
    int textSegmentID = 0;

};




struct SM_ChangeChildParameter
{
public:
    static std::string name;


    SM_ChangeChildParameter(const std::string &pValue);


    bool isValid(){ return !childSpriteTag.empty(); }


    std::string childSpriteTag;
    std::string parameterName;
    std::string parameterValue;

};




/*

//======================================================================================



class SpriteModifiers
{
public:

    ~SpriteModifiers();

    void deleteContent();

    std::vector<SpriteModifier*>& modifiers(){ return  mModifiers; }


    SM_ChildTextSprite_ChangeText* add_ChildTextSprite_ChangeText(const std::string &pValue);


protected:
    std::vector<SpriteModifier*>mModifiers;

};

*/



}



#endif // JMSPRITEMODIFIERS_H
