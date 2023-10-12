#ifndef JMMAPLOADER_BIN_H
#define JMMAPLOADER_BIN_H

#include <string>
#include "jmGlobal.h"



namespace jugimap{


class BinaryStreamReader;




//#ifdef JUGI_EDITOR


class Map;
class SpriteLayersGroup;
class SpriteLayer;
class ParallaxAndScreenLayerParameters;
class Sprite;
class VectorLayersGroup;
class VectorLayer;
class VectorShape;
class CParameter;



class MapLoader_bin
{
public:


    bool loadMapFile(const std::string &filePath, Map* map);
    bool loadHeader(const std::string &filePath);
    bool isMapFile(const std::string &filePath);


    //--- header
    std::string projectRootDirectory;
    Yorientation yOrientation = Yorientation::NOT_DEFINED;
    int tileWidth = -1;
    int tileHeight = -1;
    int numTilesX = -1;
    int numTilesY = -1;
    std::vector<std::string>usedSourceGroupsNames;


    //---
    std::string loadMessage;

private:
    bool loadMap(BinaryStreamReader&Stream, Map &map, int size, int flags=0);
    bool loadHeader_V3(BinaryStreamReader&Stream, int size);
    bool loadHeader_V4(BinaryStreamReader&Stream, int size);
    bool loadHeader_V5(BinaryStreamReader&Stream, int size);
    bool loadLayersGroup(BinaryStreamReader&Stream, SpriteLayersGroup &lg,  int size, int flags=0);
    void loadLayersGroupVariablesNEW(BinaryStreamReader& Stream, SpriteLayersGroup &lg);
    bool loadSpriteLayer(BinaryStreamReader&Stream, SpriteLayer &spriteLayer, int size, int flags=0);
    void loadLayerVariablesNEW(BinaryStreamReader& Stream, SpriteLayer &layer);
    bool LoadParallaxAndScreenLayerParametersNEW(BinaryStreamReader& Stream, ParallaxAndScreenLayerParameters &p);
    //bool loadSpriteNEW(BinaryStreamReader&Stream, ESprite &sprite, int size, int flags=0);
    //void loadSprite_formatVersion_1(BinaryStreamReader& Stream, ESprite &sprite);
    Sprite *_loadSpriteNEW(BinaryStreamReader&Stream, int size, int flags=0);
    Sprite *_loadSprite_formatVersion_1(BinaryStreamReader&Stream);
    Sprite *_loadSprite_v2(BinaryStreamReader&Stream);
    bool loadVectorLayersGroup(BinaryStreamReader&Stream, VectorLayersGroup &vlg, int size, int flags=0);
    void loadVectorLayersGroupVariablesNEW(BinaryStreamReader& Stream, VectorLayersGroup &vlg);
    bool loadVectorLayer(BinaryStreamReader&Stream, VectorLayer &vectorLayer, int size, int flags=0);
    void loadVectorLayerVariablesNEW(BinaryStreamReader& Stream, VectorLayer &vectorLayer);
    bool loadShape_V2NEW(BinaryStreamReader& Stream, VectorShape &shape, int _size, int _flags=0);

    bool loadParameter(BinaryStreamReader& Stream, CParameter &pv, int _flags=0);

    void loadSourceDataSaveHelper(BinaryStreamReader& Stream);

    void reset();


    Map* loadedMap = nullptr;
    SpriteLayer* loadedSpriteLayer = nullptr;
    VectorLayer* loadedVectorLayer = nullptr;



    //int zOrderCounterStart = 1000;
    //int zOrderCounter = 1000;         // starting depth value (the lowest layer)

static const int jmTILE_LAYER = 0;
static const int jmSPRITE_LAYER = 1;
static const int jmVECTOR_LAYER = 2;

};


extern MapLoader_bin mapLoader_bin;



//#else





//#endif




}







#endif // JMMAPLOADER_BIN_H
