
#include "jmCommonFunctions.h"
#include "gui/jmGuiText.h"

#include "jmSpriteModifiers.h"


namespace jugimap {



/*

SpriteModifiers::~SpriteModifiers()
{
    deleteContent();
}


void SpriteModifiers::deleteContent()
{
    for(SpriteModifier* sm : mModifiers){
        delete sm;
    }
}



SM_ChildTextSprite_ChangeText* SpriteModifiers::add_ChildTextSprite_ChangeText(const std::string &pValue)
{

    std::vector<std::string>subparameters = splitString(pValue, ",");

    if(subparameters.size()==2 || subparameters.size()==3){
        return nullptr;
    }

    const std::string & childSpriteTag = subparameters[0];


    // check if this modifier is already stored
    for(SpriteModifier * sm : mModifiers){
        if(sm->type()==SpriteModifierType::CHILD_TEXT_SPRITE__CHANGE_TEXT){
            SM_ChildTextSprite_ChangeText *smCTS = static_cast<SM_ChildTextSprite_ChangeText*>(sm);
            if(smCTS->childSpriteTag == childSpriteTag){    // Sprite child has already set ChangeText modifier!
                return nullptr;
            }
        }
    }


    //----
    SM_ChildTextSprite_ChangeText *sm = new SM_ChildTextSprite_ChangeText(pValue);




    return sm;

}
*/



//============================================================================================


std::string SM_ChangeChildText::name = "changeChildText";


SM_ChangeChildText::SM_ChangeChildText(const std::string &pValue)
{

    std::vector<std::string>subparameters = StdString::splitString(pValue, ",");

    if(!(subparameters.size()==2 || subparameters.size()==3)){
        return;
    }

    //----
    childSpriteTag = subparameters[0];
    StdString::removeStringWhiteSpacesOnStartAndEnd(childSpriteTag);

    if(subparameters.size()==2){
        text = subparameters[1];
        StdString::removeStringWhiteSpacesOnStartAndEnd(text);

    }else if(subparameters.size()==3){
        bookName = subparameters[1];
        StdString::removeStringWhiteSpacesOnStartAndEnd(bookName);
        std::string segment = subparameters[2];
        StdString::removeStringWhiteSpacesOnStartAndEnd(segment);
        textSegmentID =  StdString::stringToInt(segment, 0);
    }

}



std::string SM_ChangeChildText::obtainText()
{

    if(text.empty()==false){
        return text;
    }

    if(bookName.empty()==false){
        TextSegment *ts = textLibrary.GetTextSegmentFromBook(bookName, textSegmentID);
        if(ts){
            std::string t = ts->plainText();
            if(t.empty()==false){
                return t;
            }
        }
    }

    return "";

}


//=========================================================================================


std::string SM_ChangeChildParameter::name = "changeChildParameter";


SM_ChangeChildParameter::SM_ChangeChildParameter(const std::string &pValue)
{

    std::vector<std::string>subparameters = StdString::splitString(pValue, ",");

    if(subparameters.size()!=3){
        return;
    }

    //----
    childSpriteTag = subparameters[0];
    parameterName = subparameters[1];
    parameterValue = subparameters[2];

}


}
