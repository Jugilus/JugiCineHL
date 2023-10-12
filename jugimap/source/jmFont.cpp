#include <ncine/Texture.h>
#include "utf8cpp/utf8.h"

#include "jmSystem.h"
#include "jmFont.h"



namespace jugimap {



//std::string Font::pathPrefix;
std::string Font::textForHeight = "ABCXYZgqj";



/*

Font::Font(const std::string &_relativeFilePath, int _loadFontSize, FontKind _kind)
    : relativeFilePath(_relativeFilePath), loadFontSize(_loadFontSize), kind(_kind)
{

    ncFont = new ncine::Font(GetRelativeFilePath().c_str());
    //if(ncFont==nullptr){
    //    assert(true);
    //}

    ncTextNode = new ncine::TextNode(nullptr, ncFont);
    ncTextNode->setDrawEnabled(false);
    pixelHeightCommon = GetPixelHeight(textForHeight);

}

*/


/*

Font::Font(const std::string &_name, const std::string &_relativeFilePath, FontKind _kind)
    :mName(_name),
     relativeFilePath(_relativeFilePath),
     kind(_kind)
{
}

*/

Font::~Font()
{

    if(mNcineTextNode){
        delete mNcineTextNode;      // because it has no parent
    }
    if(mNcineFont){
        delete mNcineFont;
    }

}


bool Font::loadExternalData()
{

    std::string filePath = CurrentProject.gameRootDir + "/" + mRelativeFilePath;

    //if(ncFont){
    //    delete ncFont;
    //    ncFont = nullptr;
    //}
    //if(ncTextNode){
    //    delete ncTextNode;
    //    ncTextNode = nullptr;
    //}

    //---
    mNcineFont = new ncine::Font(filePath.c_str());
    if(mNcineFont->texture()->dataSize()==0 || mNcineFont->numGlyphs()==0){
        delete mNcineFont;
        mNcineFont = nullptr;

        dbgSystem.addMessage("Error loading font '" + filePath + "'!");
        return false;
    }

    mNcineFont->texture()->setMagFiltering(ncine::Texture::Filtering::NEAREST);
    mNcineFont->texture()->setMinFiltering(ncine::Texture::Filtering::LINEAR);


    mNcineTextNode = new ncine::TextNode(nullptr, mNcineFont);
    mNcineTextNode->setDrawEnabled(false);
    //pixelHeightCommon = GetPixelHeight(textForHeight);
    //int lineHeight = ncFont->lineHeight();
    mPixelHeightCommon = mNcineFont->lineHeight();


    return true;
}



int Font::GetPixelWidth(const std::string &s)
{

    Vec2f size = ncine::TextNode::calculateBoundaries(*mNcineFont, true, s.c_str());
    return size.x;

    //mText = s;
    //mNcineTextNode->setString(mText.c_str());
    //float width = mNcineTextNode->width();

    //return width;
}


int Font::GetPixelHeight(const std::string &s)
{

    return mNcineFont->lineHeight();

    //mText = s;
    //mNcineTextNode->setString(mText.c_str());
    //float height = mNcineTextNode->height();

    //return height;
}


Vec2f Font::GetPixelSize(const std::string &s)
{

    Vec2f size = ncine::TextNode::calculateBoundaries(*mNcineFont, true, s.c_str());
    return size;

    //mText = s;
    //mNcineTextNode->setString(mText.c_str());
    //float width = mNcineTextNode->width();
    //float height = mNcineTextNode->height();

    //return Vec2f(width, height);
}



//==============================================================================================





FontsLibrary::~FontsLibrary()
{
    deleteContent();
}


void FontsLibrary::deleteContent()
{
    for(Font* f : mFonts){
        delete f;
    }
    mFonts.clear();
}


bool FontsLibrary::loadExternalData()
{
    dbgSystem.addMessage("Loading fonts library external data...");

    for(Font* f : mFonts){
        if(f->loadExternalData()==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;
}


Font* FontsLibrary::getFont(const std::string &_name)
{

    for(Font* f : mFonts){
        if(f->name()==_name){
            return f;
        }
    }
    //if(mFonts.empty()==false){
    //    return mFonts.front();
    //}

    return systemFontStyle;

    //return nullptr;
}



/*
Font* FontsLibrary::FindStyle(const std::string &_nameID)
{

    if(systemFontStyle && systemFontStyle->name==_nameID){
        return systemFontStyle;
    }

    for(Font *ts : mFonts){
        if(ts->name == _nameID){
            return ts;
        }
    }

    return nullptr;
}
*/

/*
Font* FontsLibrary::GetSystemStyle()
{

    if(systemFontStyle==nullptr){
        systemFontStyle = new Font();
        systemFontStyle->name = "systemFont";

#ifdef JUGI_EDITOR
        //systemFontStyle->familyName = jugi::glob::FontMain.Font.family().toStdString();
        //systemFontStyle->size = jugi::glob::FontMain.Font.pointSize();
        //systemFontStyle->italic = jugi::glob::FontMain.Font.italic();
        //systemFontStyle->bold = jugi::glob::FontMain.Font.bold();
#endif
    }

    return systemFontStyle;
}
*/


int FontsLibrary::indexOfStyle(const std::string &name)
{

    for(int i=0; i<mFonts.size(); i++){
        if(mFonts[i]->name()==name){
            return i;
        }
    }
    return -1;
}







FontsLibrary fontsLibrary;



}



