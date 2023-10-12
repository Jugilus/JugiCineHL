#ifndef JM_SYSTEM_SOURCE_OBJECTS_H
#define JM_SYSTEM_SOURCE_OBJECTS_H

#include <vector>
#include "string"

#include "jmCommonFunctions.h"


namespace jugimap {



class SourceFile;
class SourceImage;
class SourceSprite;
class Sprite;

class EmbeddedSourceDummyImage;

struct DummySourceObjects
{

    void create();
    void updateSourceImagesForDefinedSize();
    void deleteContent();


    SourceFile *SFdummy = nullptr;
    SourceImage *SImissingSourceSprite = nullptr;
    SourceImage *SImissingTexture = nullptr;
    SourceImage *SIemptyFrame = nullptr;
    SourceImage *SIcompositeSpriteSourceImage = nullptr;
    SourceImage *SIdummyChildForEmptyCompositeSprite = nullptr;
    //ESourceImage *SIComposedSpriteDummy = nullptr;
    SourceSprite *SSdummyChildForEmptyCompositeSprite = nullptr;

    SourceSprite* SStextSourceSprite = nullptr;
    SourceSprite* SSmissingSourceSprite = nullptr;

    //---
    int imageSize = 32;

    EmbeddedSourceDummyImage *dummyImage = nullptr;

    std::string buffName = "dummiesImage";
    Sprite *SdummyChildForEmptyCompositeSprite;

};


extern DummySourceObjects dummySourceObjects;


//-----------------------------------------------------------------------------


class EmbeddedSourceDummyImage
{
public:

    EmbeddedSourceDummyImage();
    ~EmbeddedSourceDummyImage();


    unsigned char* data = nullptr;
    int width = 0;
    int height = 0;

//protected:

    std::vector<int>colors;
    std::vector<std::string>pixelRows;

};


//-----------------------------------------------------------------------------


class EmbeddedDefaultFontImage
{
public:

    EmbeddedDefaultFontImage();
    ~EmbeddedDefaultFontImage();


    unsigned char* data = nullptr;
    int width = 0;
    int height = 0;

    std::vector<int>colors;
    std::vector<std::string>pixelRows;

};


class EmbeddedDefaultFontFile
{
public:

    EmbeddedDefaultFontFile();
    ~EmbeddedDefaultFontFile();


    unsigned char* data = nullptr;
    int dataSize = 0;
    std::string name = "embeddedDefaultFontFile";

    std::vector<std::string>textRows;

};


//-----------------------------------------------------------------------------


template<typename T>
void obtainImageByteArray( T &ei)
{

    ei.data = new unsigned char[ei.width * ei.height * 4];

    //return;

    unsigned char* p = ei.data;

    for(int y=0; y<ei.height; y++){
        std::vector<std::string>strColorIndexes = StdString::splitString(ei.pixelRows[y], ",");
        assert(strColorIndexes.size()==ei.width);
        for(int x=0; x<ei.width; x++){
            int colorIndex = std::stoi(strColorIndexes[x]);
            //int colorIndex = 0;
            assert(colorIndex < ei.colors.size());
            int rgba = ei.colors[colorIndex];

            int r = rgba & 0xFF;
            int g = (rgba >> 8) & 0xFF;
            int b = (rgba >> 16) & 0xFF;
            int a = (rgba >> 24) & 0xFF;

            p[0] = r;
            p[1] = g;
            p[2] = b;
            p[3] = a;
            p += 4;
        }
    }
}


//-----------------------------------------------------------------------------


class SystemSourceObjects
{
public:


    void create();
    void deleteContent();


private:

    bool mDeleted= false;

};


extern SystemSourceObjects systemSourceObjects;


}


#endif // JMSYSTEMSOURCEOBJECTS_H
