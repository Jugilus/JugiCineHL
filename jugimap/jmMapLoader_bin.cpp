#include <memory>


//#ifdef JUGI_EDITOR

#include "jmSystem.h"
#include "jmMap.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorShape.h"
#include "jmSystemSourceObjects.h"



//#endif



#include "jmStreams.h"
#include "jmParameter.h"
#include "jmSaveSignatures.h"
#include "jmSourceContainers.h"
#include "jmLayer.h"

#ifdef JUGI_EDITOR
#include "eFramework/map/eVectorShape.h"
#endif

#include "jmMapLoader_bin.h"



namespace jugimap {




bool MapLoader_bin::loadMapFile(const std::string &filePath, Map* map)
{

    reset();

    std::unique_ptr<BinaryStreamReader>Stream(NewBinaryStreamReader(filePath));
    //BinaryStreamReader* Stream = NewBinaryStreamReader(filePath);


    if(Stream->IsOpen()==false){
        loadMessage = "Error loading file: " + filePath + "  Stream not open!";
        return false;
    }


    if(Stream->Size()<4){
        loadMessage = "Wrong file: " + filePath;
        return false;
    }

    //--- format signature
    int signature = Stream->ReadInt();
    if(signature!=SaveSignature::FORMAT){
        loadMessage = "Error loading file: " + filePath + "  Wrong format!";
        return false;
    }

    loadedMap = map;


    int version = Stream->ReadInt();

   //--- data
    int posStart = Stream->Pos();
    int sizeCounter = Stream->Size()-posStart;

    while(sizeCounter>0){
        int signature = Stream->ReadInt();
        int sizeChunk = Stream->ReadInt();
        if(sizeChunk>sizeCounter){
            loadMessage = "Error loading file: " + filePath + "  ChunkSize error for signature: " + std::to_string(signature);
            return false;
        }
        int posChunk = Stream->Pos();


        // HEADER
        //---------------------------------------------------------------------------
        if(signature==SaveSignature::HEADER_V3){

            if(loadHeader_V3(*Stream, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  LoadHeader_V3 error!";
                return false;
            }

        }else if(signature==SaveSignature::HEADER_V4){

            if(loadHeader_V4(*Stream, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  LoadHeader_V4 error!";
                return false;
            }

        }else if(signature==SaveSignature::HEADER_V5){

            if(loadHeader_V5(*Stream, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  LoadHeader_V5 error!";
                return false;
            }

       // MAP
       //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::MAP){

            if(loadMap(*Stream, *map, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  loadMap error!";
                return false;
            }


        }else{
            print("Load LoadMapFile unknown signature: " + std::to_string(signature));
            Stream->Seek(posChunk+sizeChunk);

        }

        //---
        int dSize = Stream->Pos()-posChunk;
        if(dSize!=sizeChunk){
            loadMessage = "Error loading file: " + filePath + " Wrong chunkSize for signature: " + std::to_string(signature);
            return false;
        }
        sizeCounter-=(dSize+8);   //8 za signature in size
    }


    DbgPrintNodesTree(map);

    return true;
}


bool MapLoader_bin::loadHeader(const std::string &filePath)
{

    reset();

    std::unique_ptr<BinaryStreamReader>Stream(NewBinaryStreamReader(filePath));

    if(Stream->IsOpen()==false){
        loadMessage = "Error loading file: " + filePath + "  Stream not open!";
        return false;
    }

    if(Stream->Size()<4){
        loadMessage = "Wrong file: " + filePath;
        return false;
    }

    //--- format signature
    int signature = Stream->ReadInt();
    if(signature!=SaveSignature::FORMAT){
        loadMessage = "Error loading file: " + filePath + "  Wrong format!";
        return false;
    }

    loadedMap = nullptr;

    int version = Stream->ReadInt();

   //--- data
    int posStart = Stream->Pos();
    int sizeCounter = Stream->Size()-posStart;

    while(sizeCounter>0){
        int signature = Stream->ReadInt();
        int sizeChunk = Stream->ReadInt();
        if(sizeChunk>sizeCounter){
            loadMessage = "Error loading file: " + filePath + "  ChunkSize error for signature: " + std::to_string(signature);
            return false;
        }
        int posChunk = Stream->Pos();


        // HEADER
        //---------------------------------------------------------------------------
        if(signature==SaveSignature::HEADER_V3){

            if(loadHeader_V3(*Stream, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  LoadHeader_V3 error!";
                return false;
            }
            return true;


        }else if(signature==SaveSignature::HEADER_V4){

            if(loadHeader_V4(*Stream, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  LoadHeader_V4 error!";
                return false;
            }
            return true;

        }else if(signature==SaveSignature::HEADER_V5){

            if(loadHeader_V5(*Stream, sizeChunk)==false){
                loadMessage = "Error loading file: " + filePath + "  LoadHeader_V5 error!";
                return false;
            }
            return true;

        }else{
            print("Load LoadMapFile unknown signature: " + std::to_string(signature));
            Stream->Seek(posChunk+sizeChunk);

        }

        //---
        int dSize = Stream->Pos()-posChunk;
        if(dSize!=sizeChunk){
            loadMessage = "Error loading file: " + filePath + " Wrong chunkSize for signature: " + std::to_string(signature);
            return false;
        }
        sizeCounter-=(dSize+8);   //8 za signature in size
    }

    return true;
}



bool MapLoader_bin::isMapFile(const std::string &filePath)
{

    std::unique_ptr<BinaryStreamReader>Stream(NewBinaryStreamReader(filePath));

    if(Stream->IsOpen()==false){
        loadMessage = "Error loading file: " + filePath + "  Stream not open!";
        return false;
    }

    if(Stream->Size()<4){
        loadMessage = "Wrong file: " + filePath;
        return false;
    }

    //--- format signature
    int signature = Stream->ReadInt();
    if(signature!=SaveSignature::FORMAT){
        loadMessage = "Error loading file: " + filePath + "  Wrong format!";
        return false;
    }


    return true;
}





void MapLoader_bin::reset()
{

    projectRootDirectory = "";
    yOrientation = Yorientation::DOWN;
    tileWidth = -1;
    tileHeight = -1;
    numTilesX = -1;
    numTilesY = -1;
    usedSourceGroupsNames.clear();
    loadMessage = "";

    loadedMap = nullptr;
    loadedSpriteLayer = nullptr;
    loadedVectorLayer = nullptr;

}


bool MapLoader_bin::loadHeader_V3(BinaryStreamReader&Stream, int size)
{

    int posStart = Stream.Pos();

    //---
    projectRootDirectory = Stream.ReadString();
    int tileWidth = Stream.ReadInt();
    int tileHeight = Stream.ReadInt();


    if(loadedMap){
        loadedMap->mTileWidth = tileWidth;
        loadedMap->mTileHeight = tileHeight;
    }

    //---
    Stream.Seek(posStart+size);

    return true;
}


bool MapLoader_bin::loadHeader_V4(BinaryStreamReader&Stream, int size)
{

    int posStart = Stream.Pos();

    //---
    projectRootDirectory = Stream.ReadString();
    yOrientation = GetYorientationFromInt(Stream.ReadInt());

    //---
    Stream.Seek(posStart+size);

    return true;
}


bool MapLoader_bin::loadHeader_V5(BinaryStreamReader&Stream, int size)
{

    int posStart = Stream.Pos();

    //---
    projectRootDirectory = Stream.ReadString();
    yOrientation = GetYorientationFromInt(Stream.ReadInt());

    //---
    tileWidth = Stream.ReadInt();
    tileHeight = Stream.ReadInt();
    numTilesX = Stream.ReadInt();
    numTilesY = Stream.ReadInt();

    //---
    int nUsedSourceGroups = Stream.ReadInt();
    for(int i=0; i<nUsedSourceGroups; i++){
        std::string sourceGroupName =Stream.ReadString();
        usedSourceGroupsNames.push_back(sourceGroupName);
    }


    //---
    Stream.Seek(posStart+size);

    return true;
}







bool MapLoader_bin::loadMap(BinaryStreamReader &Stream, Map &map, int size, int flags)
{


    int posStart = Stream.Pos();
    int sizeCounter = size;

    while(sizeCounter>0){
        int signature = Stream.ReadInt();
        int sizeChunk = Stream.ReadInt();
        int posChunk = Stream.Pos();
        //SetStatusTextLeft("Loading... "+Int(100*Float(posChunk)/StreamSize(Stream))+" %");

        print("TEMap::Load signature: " + std::to_string(signature));


        // VARIABLES
        //---------------------------------------------------------------------------
         if(signature==SaveSignature::SOURCE_DATA_SAVE_HELPERS){


             //saveHelper.load(Stream);
            loadSourceDataSaveHelper(Stream);


        // VARIABLES
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::VARIABLES){
            map.mNumTilesX = Stream.ReadInt();
            map.mNumTilesY = Stream.ReadInt();

            //---
            for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
                Stream.ReadInt();
            }

           //------NEW LOADER VERSIONS additional parameters if needed
           //kompatibilnost z predhodnimi verzijami, ki shranijo manj parametrov
            if(Stream.Pos()<posChunk+sizeChunk){   //sizeChunk se ni prebran do konca - beremo dodatne parametre v novem loaderju
           //	Stream.ReadInt()	'test
           //	Stream.ReadByte()	'test
            }
           //.
           //.
           //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
            if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
            }

        }else if(signature==SaveSignature::VARIABLES_V2){
            std::string dirExportMap = Stream.ReadString();
            std::string dirExportSourceGraphics = Stream.ReadString();
            std::string dirExportImage = Stream.ReadString();
#ifdef JUGI_EDITOR
            //map.mDirExportMap = dirExportMap;
            //map.mDirExportSourceGraphics = dirExportSourceGraphics;
            map.mDirExportImage = dirExportImage;
#endif
            map.mNumTilesX = Stream.ReadInt();
            map.mNumTilesY = Stream.ReadInt();

            //---
            for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
                Stream.ReadInt();
            }

           //------NEW LOADER VERSIONS additional parameters if needed
           //kompatibilnost z predhodnimi verzijami, ki shranijo manj parametrov
            if(Stream.Pos()<posChunk+sizeChunk){   //sizeChunk se ni prebran do konca - beremo dodatne parametre v novem loaderju
           //	Stream.ReadInt()	'test
           //	Stream.ReadByte()	'test
            }
           //.
           //.
           //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
            if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
            }

        }else if(signature==SaveSignature::VARIABLES_V3){

             std::string dirExportImage = Stream.ReadString();
 #ifdef JUGI_EDITOR
             map.mDirExportImage = dirExportImage;
 #endif
             map.mTileWidth = Stream.ReadInt();
             map.mTileHeight = Stream.ReadInt();
             //CurrentProject.tileWidth = map.tileWidth;
             //CurrentProject.tileHeight =  map.tileHeight;
             map.mNumTilesX = Stream.ReadInt();
             map.mNumTilesY = Stream.ReadInt();

             //---
             for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
                 Stream.ReadInt();
             }

            //------NEW LOADER VERSIONS additional parameters if needed
            //kompatibilnost z predhodnimi verzijami, ki shranijo manj parametrov
             if(Stream.Pos()<posChunk+sizeChunk){   //sizeChunk se ni prebran do konca - beremo dodatne parametre v novem loaderju
            //	Stream.ReadInt()	'test
            //	Stream.ReadByte()	'test
             }
            //.
            //.
            //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
             if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                 Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
             }


        // LAYERS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::CHILD_ITEMS){

            int nChildItems = Stream.ReadInt();
            for(int i=1; i<=nChildItems; i++){

                int childSignature = Stream.ReadInt();
                int sizeChildChunk = Stream.ReadInt();
                int posChildChunk = Stream.Pos();

                if(childSignature==SaveSignature::LAYER){
                    SpriteLayer* L = new SpriteLayer(loadedMap);
                    map.addChildLayer(L);
                    if(loadSpriteLayer(Stream, *L, sizeChildChunk)==false){
                        //map.ChildItems->removeOne(L);
                        map.removeChildLayer(L);
                        delete L;
                        loadedSpriteLayer = nullptr;
                        return false;
                    }

                }else if(childSignature==SaveSignature::VECTOR_LAYER){

                    VectorLayer* L = new VectorLayer(loadedMap);
                    map.addChildLayer(L);
                    //if(L->LoadNEW(Stream, sizeChildChunk)==false){
                    if(loadVectorLayer(Stream, *L, sizeChildChunk)==false){
                        //map.ChildItems->removeOne(L);
                        map.removeChildLayer(L);
                        delete L;
                        loadedVectorLayer = nullptr;
                        return false;
                    }

                }else if(childSignature==SaveSignature::LAYERS_GROUP){

                    SpriteLayersGroup* LG = new SpriteLayersGroup();
                    map.addChildLayer(LG);
                    //if(LG->LoadNEW(Stream, sizeChildChunk)==false){
                    if(loadLayersGroup(Stream, *LG, sizeChildChunk)==false){
                        //map.ChildItems->removeOne(LG);
                        map.removeChildLayer(LG);
                        delete LG;
                        return false;
                    }

                }else if(childSignature==SaveSignature::VECTOR_LAYERS_GROUP){

                    VectorLayersGroup* VLG = new VectorLayersGroup();
                    map.addChildLayer(VLG);
                    //if(VLG->LoadNEW(Stream, sizeChildChunk)==false){
                    if(loadVectorLayersGroup(Stream, *VLG, sizeChildChunk)==false){
                        //map.ChildItems->removeOne(VLG);
                        map.removeChildLayer(VLG);
                        delete VLG;
                        return false;
                    }


                }else{
                    print("Load board child unknown signature: " + std::to_string(childSignature));
                    if(sizeChildChunk>sizeCounter){
                        print("Load board child error! Chunk size error for unknown signature: " + std::to_string(childSignature));
                        return false;
                    }
                    Stream.Seek(posChildChunk+sizeChildChunk);
                }
            }

        // PARAMETERS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::PARAMETERS){

            int nParameters = Stream.ReadInt();
            for(int i=1; i<=nParameters; i++){
                map.mParameters.push_back(CParameter());
                CParameter &PV = map.mParameters.back();
               // PV.Load_UTF8(Stream);
                loadParameter(Stream, PV);
            }

        }else{

             print("Load map unknown signature: " + std::to_string(signature));
             if(sizeChunk>sizeCounter){
                 print("Load map error! ChunkSize error for unknown signature: " + std::to_string(signature));
                 return false;
             }
             Stream.Seek(posChunk+sizeChunk);
        }

        //---
        int dSize = Stream.Pos()-posChunk;
        if(dSize!=sizeChunk){
            print("Load map error! Wrong chunkSize for signature: " + std::to_string(signature));
            return false;
        }
        sizeCounter -= (dSize+8);   //8 za signature in size
    }

    if(Stream.Pos()-posStart!=size){
        print("Load map error! Wrong size: " + std::to_string(Stream.Pos()-posStart) + "'(" + std::to_string(size) +" saved)'");
        return false;
    }

    return true;

}


bool MapLoader_bin::loadLayersGroup(BinaryStreamReader &Stream, SpriteLayersGroup &lg, int size, int flags)
{

    int posStart = Stream.Pos();
    int sizeCounter = size;

    while(sizeCounter>0){
        int signature = Stream.ReadInt();
        int sizeChunk = Stream.ReadInt();
        int posChunk = Stream.Pos();

        print("TELayersGroup::Load signature: " + std::to_string(signature));

       // VARIABLES
       //---------------------------------------------------------------------------
        //if(signature==glob::saveSignatureVariables){
        if(signature==SaveSignature::VARIABLES){
            //lg.LoadVariablesNEW(Stream);
            loadLayersGroupVariablesNEW(Stream, lg);

           //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
            if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
            }

        // CHILD ITEMS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::CHILD_ITEMS){
            int nChildItems = Stream.ReadInt();
            for(int i=1; i<=nChildItems; i++){

                int childSignature = Stream.ReadInt();
                int sizeChildChunk = Stream.ReadInt();
                int posChildChunk = Stream.Pos();

                if(childSignature == SaveSignature::LAYER){
                    SpriteLayer* L = new SpriteLayer(loadedMap);
                    lg.addChildLayer(L);
                    //if(L->LoadNEW(Stream, sizeChildChunk, cons::NO_FLAG)==false){
                    if(loadSpriteLayer(Stream, *L, sizeChildChunk)==false){
                        //lg.ChildItems->removeOne(L);
                        lg.removeChildLayer(L);
                        delete L;
                        loadedSpriteLayer = nullptr;
                        return false;
                    }


                }else{
                    print("Load Layers Group child unknown signature: " + std::to_string(childSignature));
                    if(sizeChildChunk>sizeCounter){
                        print("Load Layers Group child error! Chunk size error for unknown signature: " + std::to_string(childSignature));
                        return false;
                    }
                    Stream.Seek(posChildChunk+sizeChildChunk);
                }
            }

        }else{
            print("Load Layers Group unknown signature: " + std::to_string(signature));
            if(sizeChunk>sizeCounter){
                print("Load Layers Group error! ChunkSize error for unknown signature: " + std::to_string(signature));
                return false;
            }
            Stream.Seek(posChunk+sizeChunk);
        }

       //---
        int dSize = Stream.Pos()-posChunk;
        if(dSize!=sizeChunk){
           //error reading stream
            print("Load Layers Group error! Wrong chunkSize for signature: " + std::to_string(signature));
            return false;
        }
        sizeCounter-=(dSize+8);   //8 za signature in size
    }

    if(Stream.Pos()-posStart!=size){
        print("Load Layers Group error! Wrong size: " + std::to_string(Stream.Pos()-posStart) + "'(" + std::to_string(size) +" saved)'");
        return false;
    }

    return true;
}


void MapLoader_bin::loadLayersGroupVariablesNEW(BinaryStreamReader &Stream, SpriteLayersGroup &lg)
{

    //lg.nameID = Stream.ReadUTF8encodedByteArray();
    lg._setName(Stream.ReadString());
    lg.locked = Stream.ReadByte();
    lg.hidden = Stream.ReadByte();
    lg.node()->setDrawEnabled(!lg.hidden);

    //---
    for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
        Stream.ReadInt();
    }

}


bool MapLoader_bin::loadSpriteLayer(BinaryStreamReader &Stream, SpriteLayer &spriteLayer, int size, int flags)
{

    loadedSpriteLayer = &spriteLayer;


    if(loadedMap){
        spriteLayer.mMap = loadedMap;
        //spriteLayer.mZOrder = zOrderCounter;
        //zOrderCounter += settings.GetZOrderStep();
    }


    int posStart = Stream.Pos();
    int sizeCounter = size;

    while(sizeCounter>0){
        int signature = Stream.ReadInt();
        int sizeChunk = Stream.ReadInt();
        int posChunk = Stream.Pos();

       // VARIABLES
       //---------------------------------------------------------------------------
        if(signature==SaveSignature::VARIABLES){
            //LoadVariablesNEW(Stream);
            loadLayerVariablesNEW(Stream, spriteLayer);

           //------NEW LOADER VERSIONS additional parameters if needed
           //kompatibilnost z predhodnimi verzijami, ki shranijo manj parametrov
            if(Stream.Pos()<posChunk+sizeChunk){   //sizeChunk se ni prebran do konca - beremo dodatne parametre v novem loaderju
           //	Stream.ReadInt()	'test
           //	Stream.ReadByte()	'test
            }
           //.
           //.

           //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
            if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
            }

            //---
            //(Setup(nameID, MAKE_TILES_GRID);


       // OBJECTS
       //---------------------------------------------------------------------------

        /*
        }else if(signature==SaveSignature::OBJECTS){

            int nObjects = Stream.ReadInt();
            for(int i=0; i<nObjects; i++){
                TEObject *o = new TEObject();
                o->idMapCopy = TEObject::idMapCopyCounter++;
                //if(o->Load(Stream, -1, flags | cons::saveLAYER_OBJECT)==false){
                if(o->Load(Stream, -1, flags)==false){
                    delete o;
                    return false;
                }else{
                    layer.Objects.push_back(o);
                    o->iter = layer.Objects.end(); o->iter--;
                }
            }
        */

        // OBJECTS_V2
        //---------------------------------------------------------------------------
         }else if(signature==SaveSignature::OBJECTS_V2){

             int nObjects = Stream.ReadInt();
             for(int i=0; i<nObjects; i++){
                 int childSignature = Stream.ReadInt();
                 int sizeChildChunk = Stream.ReadInt();
                 int posChildChunk = Stream.Pos();

                 if(childSignature==SaveSignature::OBJECT){


                     /*
                     ESprite *o = new ESprite();
                     o->idMapCopy = ESprite::idMapCopyCounter++;
                     if(loadSpriteNEW(Stream, *o, sizeChildChunk, flags)==false){
                     //if(o->Load_v2NEW(Stream, sizeChildChunk, flags)==false){
                         delete o;
                         return false;
                     }else{
                         layer.mSprites.push_back(o);
                         o->iter = layer.mSprites.end(); o->iter--;
                     }
                     */

                     Sprite * sprite = _loadSpriteNEW(Stream, sizeChildChunk);
                     if(sprite){
                         spriteLayer.addSprite(sprite);
                     }

                 }else{
                     print("Load board child unknown signature: " + std::to_string(childSignature));
                     if(sizeChildChunk>sizeCounter){
                         print("Load Layer object error! Chunk size error for unknown signature: " + std::to_string(childSignature));
                         return false;
                     }
                     Stream.Seek(posChildChunk+sizeChildChunk);
                 }
             }


        // PARAMETERS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::PARAMETERS){

            int nParameters = Stream.ReadInt();
            for(int i=1; i<=nParameters; i++){
                spriteLayer.mParameters.push_back(CParameter());
                CParameter &PV = spriteLayer.mParameters.back();
                //PV.Load_UTF8(Stream);
                loadParameter(Stream, PV);
            }


        // PARALLAX LAYER PARAMETERS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::PARALLAX_LAYER_PARAMETERS){

            //layer.parallaxAndScreenLayerParameters.LoadNEW(Stream, -1);
            LoadParallaxAndScreenLayerParametersNEW(Stream, spriteLayer.mPP);


        }else{
           //error reading stream
           //loadMessage="Load Bitmap layer error! Wrong signature:"+signature
           //Return -1
            print("Load Bitmap layer unknown signature: " + std::to_string(signature));
            if(sizeChunk>sizeCounter){
                print("Load layer error! ChunkSize error for unknown signature: " + std::to_string(signature));
                return false;
            }
            Stream.Seek(posChunk+sizeChunk);
        }

        //---
        int dSize = Stream.Pos()-posChunk;
        if(dSize!=sizeChunk){
           //error reading stream
            print("Load layer error! Wrong chunkSize for signature: " + std::to_string(signature));
            return false;
        }
        sizeCounter -= (dSize+8);   //8 za signature in size
    }

    if(Stream.Pos()-posStart!=size){
       //error reading stream
        print("Load Layer error! Wrong size: " + std::to_string(Stream.Pos()-posStart) + "'(" + std::to_string(size) +" saved)'");
        return false;
    }

    loadedSpriteLayer = nullptr;
    return true;
}


void MapLoader_bin::loadLayerVariablesNEW(BinaryStreamReader &Stream, SpriteLayer &layer)
{

    //layer.nameID = Stream.ReadUTF8encodedByteArray();
    //layer.nameID = QString::fromStdString(Stream.ReadString());
    //layer.locked = Stream.ReadByte();
    //layer.hidden = Stream.ReadByte();
    layer._setName(Stream.ReadString());
    layer.locked = Stream.ReadByte();
    layer.hidden = Stream.ReadByte();
    layer.node()->setDrawEnabled(!layer.hidden);
    layer.mContent = GetSpriteLayerContentFromInt(Stream.ReadInt());
    bool includedInMultiLayerBrush = Stream.ReadByte();
#ifdef JUGI_EDITOR
    layer.mIncludedInMultiLayerBrush = includedInMultiLayerBrush;
#endif


    //layer.mBrushLayer->mContent = layer.mContent;

    /*
    layer.mWorldBoundingBox.min.x = stream.ReadFloat();
    layer.mWorldBoundingBox.min.y = stream.ReadFloat();
    layer.mWorldBoundingBox.max.x = stream.ReadFloat();
    layer.mWorldBoundingBox.max.y = stream.ReadFloat();

    if(settings.IsYCoordinateUp()){
        float yMin=0,yMax=0;
        if(loadedComposedSprite){                           // a child of loadedComposedSprite
            yMin = loadedComposedSprite->mSize.y - layer->mWorldBoundingBox.max.y;
            yMax = loadedComposedSprite->mSize.y - layer->mWorldBoundingBox.min.y;
        }else if(map){
            yMin = map->mNTiles.y * map->mTileSize.y - layer->mWorldBoundingBox.max.y;
            yMax = map->mNTiles.y * map->mTileSize.y - layer->mWorldBoundingBox.min.y;
        }else{
            assert(false);
        }
        layer->mWorldBoundingBox.min.y = yMin;
        layer->mWorldBoundingBox.max.y = yMax;
    }
    */



    layer.mWorldBoundingBox.min.x = Stream.ReadInt();
    layer.mWorldBoundingBox.min.y = Stream.ReadInt();
    layer.mWorldBoundingBox.max.x = Stream.ReadInt();
    layer.mWorldBoundingBox.max.y = Stream.ReadInt();

    //if(settings.IsYCoordinateUp()){
    if(yOrientation != settings.yOrientation()){
        float yMin = loadedMap->mNumTilesY * loadedMap->mTileHeight - layer.mWorldBoundingBox.max.y;
        float yMax = loadedMap->mNumTilesY * loadedMap->mTileHeight - layer.mWorldBoundingBox.min.y;
        layer.mWorldBoundingBox.min.y = yMin;
        layer.mWorldBoundingBox.max.y = yMax;
    }

    //---
    for(int i=0; i<6; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
        Stream.ReadInt();
    }

}


bool MapLoader_bin::LoadParallaxAndScreenLayerParametersNEW(BinaryStreamReader &Stream, ParallaxAndScreenLayerParameters &p)
{

    Layer * l = loadedSpriteLayer;
    if(l==nullptr){
        l = loadedVectorLayer;
    }
    assert(l);

    p.attachTolayer = Stream.ReadString();

    p.type = Stream.ReadInt();
    //int layerType = stream.ReadInt();
    if(p.type== ParallaxAndScreenLayerParameters::typePARALLAX_LAYER){
        l->mLayerType = LayerType::PARALLAX;

    }else if(p.type== ParallaxAndScreenLayerParameters::typePARALLAX_STRETCHING_SINGLE_SPRITE){
        l->mLayerType = LayerType::PARALLAX_STRETCHING_SINGLE_SPRITE;

    }else if(p.type== ParallaxAndScreenLayerParameters::typeSCREEN_LAYER){
        l->mLayerType = LayerType::SCREEN;

    }else if(p.type== ParallaxAndScreenLayerParameters::typeSCREEN_STRETCHING_SINGLE_SPRITE){
        l->mLayerType = LayerType::SCREEN_STRETCHING_SINGLE_SPRITE;
    }

    p.parallaxFactor.x = Stream.ReadFloat();
    p.parallaxFactor.y = Stream.ReadFloat();

    p.alignPosition.x = Stream.ReadInt();
    p.alignPosition.y = Stream.ReadInt();
    p.alignOffset.x = Stream.ReadInt();
    p.alignOffset.y = Stream.ReadInt();
    if(yOrientation != settings.yOrientation()){
        p.alignPosition.y = 100 - p.alignPosition.y;
        p.alignOffset.y = - p.alignOffset.y;      // offset is distance in pixels
    }
    p.alignOffset_ObtainFromMap.x = Stream.ReadByte();
    p.alignOffset_ObtainFromMap.y = Stream.ReadByte();

    p.tilingCount.x = Stream.ReadInt();
    p.tilingCount.y = Stream.ReadInt();
    p.tilingCountAutoSpread.x = Stream.ReadByte();
    p.tilingCountAutoSpread.y = Stream.ReadByte();
    p.tilingSpacing.x = Stream.ReadInt();
    p.tilingSpacing.y = Stream.ReadInt();
    p.tilingSpacingDelta.x = Stream.ReadInt();
    p.tilingSpacingDelta.y = Stream.ReadInt();

    p.stretch = Stream.ReadInt();

    //---
    for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
        Stream.ReadInt();
    }


    ParallaxAndScreenLayerParameters::typeLastSet = p.type;

    return true;

}


Sprite *MapLoader_bin::_loadSpriteNEW(BinaryStreamReader&Stream, int size, int flags)
{

    Sprite* sprite = nullptr;

    int posStart = Stream.Pos();
    int sizeCounter = size;

    //----
    int saveFormatVersion = Stream.ReadInt();

    if(saveFormatVersion==1){
        sprite = _loadSprite_formatVersion_1(Stream);

    }else if(saveFormatVersion==2){
        sprite = _loadSprite_v2(Stream);

    }else{
        assert(true);
    }

    //----
    if(Stream.Pos()-posStart!=size){
       //error reading stream
        print("Load Sprite error! Wrong size: " + std::to_string(Stream.Pos()-posStart) + "'(" + std::to_string(size) +" saved)'");
        return nullptr;
    }

    return sprite;

}



Sprite *MapLoader_bin::_loadSprite_formatVersion_1(BinaryStreamReader &Stream)
{

    Sprite* sprite = nullptr;

    //----
    //sourceSetSaveID = Stream.ReadInt();
    //saveID = Stream.ReadInt();

    //sprite.sourceGroupSaveID = Stream.ReadInt();
    //sprite.sourceSetSaveID = Stream.ReadInt();
    //sprite.sourceObjectSaveID = Stream.ReadInt();


    int sourceGroupSaveID = Stream.ReadInt();
    int sourceSetSaveID = Stream.ReadInt();
    int sourceObjectSaveID = Stream.ReadInt();

    SourceDataSaveHelper &dbgSaveHelper = saveHelper;
    SourceLibrary & dbgSourceLibrary = sourceLibrary;

    SourceSprite* sourceSprite = sourceLibrary.getSourceObjectViaSaveHelperIDs(sourceGroupSaveID, sourceSetSaveID, sourceObjectSaveID);

    if(sourceSprite==nullptr){      // should not be possible
        sourceSprite = dummySourceObjects.SSmissingSourceSprite;
        //assert(false);
    }

    if(sourceSprite->name()=="Platform 1"){
        DummyFunction();
    }

//
//    sprite = new Sprite();
//#else
    sprite = Sprite::makeSpriteByKind(sourceSprite->mKind);

#ifdef JUGI_EDITOR
    sprite->idMapCopy = Sprite::idMapCopyCounter++;
#endif

    sprite->mParentLayer = loadedSpriteLayer;
    sprite->mSourceSprite = sourceSprite;
    //s->map = map;
    //sprite->mParentComposedSprite = sl->mParentComposedSprite;

    //TESourceSet *SourceSet = SourceSets.GetSetByName(saveHelper.sourceSetName(sorSetSaveID));
    //if(SourceSet){
    //    SourceObject = SourceSet->GetObjectByName(saveHelper.sourceSpriteName(sorSaveID));
    //}

    int xt = Stream.ReadInt();
    int yt = Stream.ReadInt();
    int xInCell = Stream.ReadInt();
    int yInCell = Stream.ReadInt();

#ifdef JUGI_EDITOR
    sprite->xt = xt;
    sprite->yt = yt;
    sprite->xInCell = xInCell;
    sprite->yInCell = yInCell;

    //sprite->setPositionFromEditorInGridPosition();

    //if(settings.IsYCoordinateUp()){
    if(yOrientation != settings.yOrientation()){
        sprite->yt = loadedMap->mNumTilesY - sprite->yt - 1 ;

        if(loadedSpriteLayer->mContent==SpriteLayerContent::FREE_SPRITES){
            sprite->yInCell = loadedMap->mTileHeight - sprite->yInCell - 1;
            sprite->yt += sprite->yInCell/loadedMap->mTileHeight;
            sprite->yInCell = sprite->yInCell % loadedMap->mTileHeight;

        }else{
            sprite->yInCell = 0;
        }
        //sprite->mPosition.y = loadedMap->mNumTilesY * loadedMap->tileHeight - sprite->mPosition.y;
        //sprite->yt = int(sprite->mPosition.y) / loadedMap->mNumTilesY;
        //sprite->yInCell = int(sprite->mPosition.y) % loadedMap->mNumTilesY;
    }

    sprite->setPositionFromGridPosition();


#else

    //EMap* map = (sourceSprite->sourceComposedSprite()==nullptr)? loadedMap : sourceSprite->sourceComposedSprite()->map();
    //Vec2i tileSize(map->tileWidth, map->tileHeight);
    Vec2i tileSize(loadedMap->tileWidth(), loadedMap->tileHeight());
    sprite->setPositionFromGridPosition(Vec2i(xt, yt), Vec2i(xInCell,yInCell), tileSize);

    if(yOrientation != settings.yOrientation()){
        sprite->mPosition.y = loadedMap->mNumTilesY * tileSize.y - sprite->mPosition.y;
    }

#endif


    //----
    const int flagFLIP_X =          1 << 0;
    const int flagFLIP_Y =          1 << 1;
    const int flagSCALE_UNIFORM =   1 << 2;
    const int flagSCALE_X =         1 << 3;
    const int flagSCALE_Y =         1 << 4;
    const int flagROTATION =        1 << 5;
    const int flagALPHA =           1 << 6;
    const int flagOVERLAY_COLOR =   1 << 7;

    const int flagID =              1 << 10;
    const int flagNAME_ID =         1 << 11;
    const int flagDATA_FLAG =       1 << 12;
    const int flagSOURCE_OBJECT_REPLACEMENT = 1 << 13;
    const int flagCHILD_SOURCE_OBJECT_REPLACEMENTS = 1 << 14;

    const int flagPARAMETERS = 1 << 20;


    const int saved = Stream.ReadInt();

    if(saved & flagFLIP_X) sprite->mFlip.x = true;
    if(saved & flagFLIP_Y) sprite->mFlip.y = true;
    if(saved & flagSCALE_X) sprite->mScale.x = Stream.ReadFloat();
    if(saved & flagSCALE_Y) sprite->mScale.y = Stream.ReadFloat();
    if(saved & flagSCALE_UNIFORM){
        sprite->mScale.y = sprite->mScale.x;
#ifdef JUGI_EDITOR
        sprite->mUniformScale = true;
#endif
    }
    if(saved & flagROTATION) sprite->mRotation = Stream.ReadFloat();
    if(saved & flagALPHA) sprite->mAlpha = Stream.ReadFloat();

    if(saved & flagOVERLAY_COLOR){
        //sprite->mExtraProperties.kind = EShaderBasedProperties::kindOVERLAY_COLOR;
        //sprite->mExtraProperties.colorRGBA = ColorRGBA(Stream.ReadInt());
        //sprite->mExtraProperties.blend = GetColorOverlayBlendFromInt(Stream.ReadInt());
        int colorRGBA = Stream.ReadInt();
        int blend = Stream.ReadInt();

    }

    if(saved & flagID) sprite->mId = Stream.ReadInt();
    if(saved & flagNAME_ID) sprite->mName = Stream.ReadString();
    if(saved & flagDATA_FLAG) sprite->mDataFlags = Stream.ReadInt();

    if(saved & flagSOURCE_OBJECT_REPLACEMENT){
        //sprite.sourceObjectReplacement_sourceGroupSaveID = Stream.ReadInt();
        //sprite.sourceObjectReplacement_sourceSetSaveID = Stream.ReadInt();
        //sprite.sourceObjectReplacement_sourceObjectSaveID = Stream.ReadInt();
        sourceGroupSaveID = Stream.ReadInt();
        sourceSetSaveID = Stream.ReadInt();
        sourceObjectSaveID = Stream.ReadInt();
        //sprite->mSourceSpriteReplacement = sourceLibrary.getSourceObjectViaSaveHelperIDs(sourceGroupSaveID, sourceSetSaveID, sourceObjectSaveID);

        //sorSetSaveID = Stream.ReadInt();
        //sorSaveID = Stream.ReadInt();
        //TESourceSet *SourceSet = SourceSets.GetSetByName(saveHelper.sourceSetName(sorSetSaveID));
        //if(SourceSet){
        //    sourceObjectReplacement = SourceSet->GetObjectByName(saveHelper.sourceSpriteName(sorSaveID));
        //}
    }

    //if(sprite.sourceObjectReplacement){
    //    SetBreakPointHack();
    //}

    if(saved & flagCHILD_SOURCE_OBJECT_REPLACEMENTS){

        int nChilds = Stream.ReadInt();
        for(int i=0; i<nChilds; i++){
            //sprite->mChildrenSourceSpriteReplacements.push_back(ChildSourceObjectReplacement());
            //sprite.childSourceObjectReplacements.back().sourceObjectReplacement_sourceGroupSaveID = Stream.ReadInt();
            //sprite.childSourceObjectReplacements.back().sourceObjectReplacement_sourceSetSaveID = Stream.ReadInt();
            //sprite.childSourceObjectReplacements.back().sourceObjectReplacement_sourceObjectSaveID = Stream.ReadInt();
            //sprite->mChildrenSourceSpriteReplacements.back().childName = Stream.ReadString();

            sourceGroupSaveID = Stream.ReadInt();
            sourceSetSaveID = Stream.ReadInt();
            sourceObjectSaveID = Stream.ReadInt();
            std::string childName = Stream.ReadString();

            if(sprite->mSourceSprite && sprite->mSourceSprite->sourceCompositeSprite() && childName.empty()==false){       // safety, should always be true

                SourceSprite *soReplacement = sourceLibrary.getSourceObjectViaSaveHelperIDs(sourceGroupSaveID, sourceSetSaveID, sourceObjectSaveID);

                if(soReplacement){
                    //sprite->mChildrenSourceSpriteReplacements.push_back(ChildSourceObjectReplacement(childName, soReplacement, nullptr));
                }
            }
        }
    }


    //if(settings.IsYCoordinateUp()){
    if(yOrientation != settings.yOrientation()){
        //sprite->mPosition.y = loadedMap->mNumTilesY * tileSize.y - sprite->mPosition.y;
        sprite->mRotation = -sprite->mRotation;
    }


    if(sourceSprite->name()=="ram"){
        DummyFunction();
    }
    if(saved & flagPARAMETERS){
        int nParameters = Stream.ReadInt();
        for(int i=0; i<nParameters; i++){
            sprite->mParameters.push_back(CParameter());
            CParameter &PV = sprite->mParameters.back();
            //PV.Load_UTF8(Stream, TQParameterValue::SAVE_PARAMETERS_VALUE_ONLY);
            //PV.Load_UTF8(Stream);
            loadParameter(Stream, PV);
        }

    }


    //---
    if(dynamic_cast<CompositeSprite*>(sprite)){
        CompositeSprite *s = static_cast<CompositeSprite*>(sprite);


        if(loadedMap){
            if(sourceSprite->mName=="csBlueDiamond"){
                DummyFunction();
            }
        }

        //s->mHandle = sourceSprite->mSourceComposedSprite->mHandle;
        s->mHandle = sourceSprite->mSourceCompositeSprite->mHandleForMapCompositeSprites;
        //if(configuration.IsYCoordinateUp()){
        //    s->handle.y = ss->SourceComposedSprite->size.y - s->handle.y;
        //}



        if(loadedMap){
            DummyFunction();
        }
        //int zOrder = loadedSpriteLayer->mZOrder;
        //CompositeSprite::copyLayers(sourceSprite->mSourceComposedSprite, s, zOrder);
        CompositeSprite::copyLayers(sourceSprite->mSourceCompositeSprite, s);

        /*
        if(loadedMap){
            if(settings.GetZOrderStep()>0){
                zOrderCounter = std::max(zOrderCounter, zOrder);     // !
            }else{
                zOrderCounter = std::min(zOrderCounter, zOrder);
            }
        }
        */
    }




    return sprite;

}


Sprite *MapLoader_bin::_loadSprite_v2(BinaryStreamReader &Stream)
{

    Sprite* sprite = nullptr;

    //---
    int sourceGroupSaveID = Stream.ReadInt();
    int sourceSetSaveID = Stream.ReadInt();
    int sourceObjectSaveID = Stream.ReadInt();

    SourceDataSaveHelper &dbgSaveHelper = saveHelper;
    SourceLibrary & dbgSourceLibrary = sourceLibrary;

    SourceSprite* sourceSprite = nullptr;

    if(sourceGroupSaveID>=0 && sourceSetSaveID>=0 && sourceObjectSaveID>=0){
        sourceSprite = sourceLibrary.getSourceObjectViaSaveHelperIDs(sourceGroupSaveID, sourceSetSaveID, sourceObjectSaveID);
    }

    if(sourceSprite==nullptr){
        sourceSprite = dummySourceObjects.SSmissingSourceSprite;
        //assert(false);
    }

    if(sourceSprite->name()=="Platform 1"){
        DummyFunction();
    }

//
//    sprite = new Sprite();
//#else
    sprite = Sprite::makeSpriteByKind(sourceSprite->mKind);

#ifdef JUGI_EDITOR
    sprite->idMapCopy = Sprite::idMapCopyCounter++;
#endif

    sprite->mParentLayer = loadedSpriteLayer;
    sprite->mSourceSprite = sourceSprite;
    //s->map = map;
    //sprite->mParentComposedSprite = sl->mParentComposedSprite;

    //TESourceSet *SourceSet = SourceSets.GetSetByName(saveHelper.sourceSetName(sorSetSaveID));
    //if(SourceSet){
    //    SourceObject = SourceSet->GetObjectByName(saveHelper.sourceSpriteName(sorSaveID));
    //}

    int xt = Stream.ReadInt();
    int yt = Stream.ReadInt();
    int xInCell = Stream.ReadInt();
    int yInCell = Stream.ReadInt();

#ifdef JUGI_EDITOR
    sprite->xt = xt;
    sprite->yt = yt;
    sprite->xInCell = xInCell;
    sprite->yInCell = yInCell;

    //sprite->setPositionFromEditorInGridPosition();

    //if(settings.IsYCoordinateUp()){
    if(yOrientation != settings.yOrientation()){
        sprite->yt = loadedMap->mNumTilesY - sprite->yt - 1 ;

        if(loadedSpriteLayer->mContent==SpriteLayerContent::FREE_SPRITES){
            sprite->yInCell = loadedMap->mTileHeight - sprite->yInCell - 1;
            sprite->yt += sprite->yInCell/loadedMap->mTileHeight;
            sprite->yInCell = sprite->yInCell % loadedMap->mTileHeight;

        }else{
            sprite->yInCell = 0;
        }
        //sprite->mPosition.y = loadedMap->mNumTilesY * loadedMap->tileHeight - sprite->mPosition.y;
        //sprite->yt = int(sprite->mPosition.y) / loadedMap->mNumTilesY;
        //sprite->yInCell = int(sprite->mPosition.y) % loadedMap->mNumTilesY;
    }

    sprite->setPositionFromGridPosition();


#else

    //EMap* map = (sourceSprite->sourceComposedSprite()==nullptr)? loadedMap : sourceSprite->sourceComposedSprite()->map();
    //Vec2i tileSize(map->tileWidth, map->tileHeight);
    //Vec2i tileSize(loadedMap->tileWidth, loadedMap->tileHeight);
    //sprite->setPositionFromEditorGridPosition(Vec2i(xt, yt), Vec2i(xInCell,yInCell), tileSize);

    //if(settings.IsYCoordinateUp()){
    //    sprite->mPosition.y = loadedMap->mNumTilesY * tileSize.y - sprite->mPosition.y;
    //}


    Vec2i tileSize(loadedMap->tileWidth(), loadedMap->tileHeight());
    sprite->setPositionFromGridPosition(Vec2i(xt, yt), Vec2i(xInCell,yInCell), tileSize);

    if(yOrientation != settings.yOrientation()){
        sprite->mPosition.y = loadedMap->mNumTilesY * tileSize.y - sprite->mPosition.y;
    }

#endif


    //----
    const int flagFLIP_X =          1 << 0;
    const int flagFLIP_Y =          1 << 1;
    const int flagSCALE_UNIFORM =   1 << 2;
    const int flagSCALE_X =         1 << 3;
    const int flagSCALE_Y =         1 << 4;
    const int flagROTATION =        1 << 5;
    const int flagALPHA =           1 << 6;
    const int flagOVERLAY_COLOR =   1 << 7;

    const int flagID =              1 << 10;
    const int flagNAME_ID =         1 << 11;
    const int flagDATA_FLAG =       1 << 12;

    const int flagTEXT_DATA = 1 << 15;

    const int flagPARAMETERS = 1 << 20;


    const int saved = Stream.ReadInt();

    if(saved & flagFLIP_X) sprite->mFlip.x = true;
    if(saved & flagFLIP_Y) sprite->mFlip.y = true;
    if(saved & flagSCALE_X) sprite->mScale.x = Stream.ReadFloat();
    if(saved & flagSCALE_Y) sprite->mScale.y = Stream.ReadFloat();
    if(saved & flagSCALE_UNIFORM){
        sprite->mScale.y = sprite->mScale.x;
#ifdef JUGI_EDITOR
        sprite->mUniformScale = true;
#endif
    }
    if(saved & flagROTATION) sprite->mRotation = Stream.ReadFloat();
    if(saved & flagALPHA) sprite->mAlpha = Stream.ReadFloat();

    if(saved & flagOVERLAY_COLOR){
        //sprite->mExtraProperties.kind = EShaderBasedProperties::kindOVERLAY_COLOR;
        //sprite->mExtraProperties.colorRGBA = ColorRGBA(Stream.ReadInt());
        //sprite->mExtraProperties.blend = GetColorOverlayBlendFromInt(Stream.ReadInt());
        int colorRGBA = Stream.ReadInt();
        int blend = Stream.ReadInt();

    }

    if(saved & flagID) sprite->mId = Stream.ReadInt();
    if(saved & flagNAME_ID) sprite->mName = Stream.ReadString();
    if(saved & flagDATA_FLAG) sprite->mDataFlags = Stream.ReadInt();


    //if(settings.IsYCoordinateUp()){
    if(yOrientation != settings.yOrientation()){
        //sprite->mPosition.y = loadedMap->mNumTilesY * tileSize.y - sprite->mPosition.y;
        sprite->mRotation = -sprite->mRotation;
    }

    if(saved & flagTEXT_DATA){

        // the sprite can be also standard sprite with SSmissingSourceSprite dummy so we must check out its type before casting

        std::string text = Stream.ReadString();
        std::string BookName = Stream.ReadString();
        int segmentId = Stream.ReadInt();

        if(sprite->type()==SpriteType::TEXT){
            TextSprite* ts = static_cast<TextSprite*>(sprite);
            ts->mText = text;
            ts->mBookName = BookName;
            ts->mBookSegmentId = segmentId;
        }
    }


    if(saved & flagPARAMETERS){
        int nParameters = Stream.ReadInt();
        for(int i=0; i<nParameters; i++){
            sprite->mParameters.push_back(CParameter());
            CParameter &PV = sprite->mParameters.back();
            //PV.Load_UTF8(Stream, TQParameterValue::SAVE_PARAMETERS_VALUE_ONLY);
            //PV.Load_UTF8(Stream);
            loadParameter(Stream, PV);
        }

    }


    //---
    if(dynamic_cast<CompositeSprite*>(sprite)){
        CompositeSprite *s = static_cast<CompositeSprite*>(sprite);

        s->mHandle = sourceSprite->mSourceCompositeSprite->mHandleForMapCompositeSprites;
        //if(configuration.IsYCoordinateUp()){
        //    s->handle.y = ss->SourceComposedSprite->size.y - s->handle.y;
        //}

        if(loadedMap){
            if(sourceSprite->mName=="csBlueDiamond"){
                DummyFunction();
            }
        }


        if(loadedMap){
            DummyFunction();
        }
        //int zOrder = loadedSpriteLayer->mZOrder;
        //CompositeSprite::copyLayers(sourceSprite->mSourceComposedSprite, s, zOrder);
        CompositeSprite::copyLayers(sourceSprite->mSourceCompositeSprite, s);

        /*
        if(loadedMap){
            if(settings.GetZOrderStep()>0){
                zOrderCounter = std::max(zOrderCounter, zOrder);     // !
            }else{
                zOrderCounter = std::min(zOrderCounter, zOrder);
            }
        }
        */
    }




    return sprite;

}




bool MapLoader_bin::loadVectorLayersGroup(BinaryStreamReader &Stream, VectorLayersGroup &vlg, int size, int flags)
{

    int posStart = Stream.Pos();
    int sizeCounter = size;

    while(sizeCounter>0){
        int signature = Stream.ReadInt();
        int sizeChunk = Stream.ReadInt();
        int posChunk = Stream.Pos();

        print("TEVectorLayersGroup::Load signature: " + std::to_string(signature));


       // VARIABLES
       //---------------------------------------------------------------------------
        if(signature==SaveSignature::VARIABLES){
            loadVectorLayersGroupVariablesNEW(Stream, vlg);

           //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
            if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
            }

        // CHILD ITEMS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::CHILD_ITEMS){
            int nChildItems = Stream.ReadInt();
            for(int i=1; i<=nChildItems; i++){

                int childSignature = Stream.ReadInt();
                int sizeChildChunk = Stream.ReadInt();
                int posChildChunk = Stream.Pos();

                if(childSignature == SaveSignature::VECTOR_LAYER){
                    VectorLayer* L = new VectorLayer(loadedMap);
                    vlg.addChildLayer(L);
                    //if(L->LoadNEW(Stream, sizeChildChunk, cons::NO_FLAG)==false){
                    if(loadVectorLayer(Stream, *L, sizeChildChunk)==false){
                        //vlg.ChildItems->removeOne(L);
                        vlg.removeChildLayer(L);
                        delete L;
                        loadedVectorLayer = nullptr;
                        return false;
                    }


                }else{
                    print("Load Vector Layers Group child unknown signature: " + std::to_string(childSignature));
                    if(sizeChildChunk>sizeCounter){
                        print("Load Vector Layers Group child error! Chunk size error for unknown signature: " + std::to_string(childSignature));
                        return false;
                    }
                    Stream.Seek(posChildChunk+sizeChildChunk);
                }
            }

        }else{
            print("Load Vector Layers Group unknown signature: " + std::to_string(signature));
            if(sizeChunk>sizeCounter){
                print("Load Vector Layers Group error! ChunkSize error for unknown signature: " + std::to_string(signature));
                return false;
            }
            Stream.Seek(posChunk+sizeChunk);
        }

       //---
        int dSize = Stream.Pos()-posChunk;
        if(dSize!=sizeChunk){
           //error reading stream
            print("Load Vector Layers Group error! Wrong chunkSize for signature: " + std::to_string(signature));
            return false;
        }
        sizeCounter-=(dSize+8);   //8 za signature in size
    }

    if(Stream.Pos()-posStart!=size){
        print("Load Vector Layers Group error! Wrong size: " + std::to_string(Stream.Pos()-posStart) + "'(" + std::to_string(size) +" saved)'");
        return false;
    }

    return true;
}


void MapLoader_bin::loadVectorLayersGroupVariablesNEW(BinaryStreamReader &Stream, VectorLayersGroup &vlg)
{

    //vlg.nameID = QString::fromStdString(Stream.ReadString());
    //vlg.locked = Stream.ReadByte();
    //vlg.hidden = Stream.ReadByte();

    vlg._setName(Stream.ReadString());
    vlg.locked = Stream.ReadByte();
    vlg.hidden = Stream.ReadByte();
    vlg.node()->setDrawEnabled(!vlg.hidden);

    //---
    for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
        Stream.ReadInt();
    }
}


bool MapLoader_bin::loadVectorLayer(BinaryStreamReader &Stream, VectorLayer &vectorLayer, int size, int flags)
{

    loadedVectorLayer = &vectorLayer;

    if(loadedMap){
        vectorLayer.mMap = loadedMap;
    }

    int posStart = Stream.Pos();
    int sizeCounter = size;

    while(sizeCounter>0){
        int signature = Stream.ReadInt();
        int sizeChunk = Stream.ReadInt();
        int posChunk = Stream.Pos();

       // VARIABLES
       //---------------------------------------------------------------------------
        if(signature==SaveSignature::VARIABLES){
            //LoadVariablesNEW(Stream);
            loadVectorLayerVariablesNEW(Stream, vectorLayer);

           //------NEW LOADER VERSIONS additional parameters if needed
           //kompatibilnost z predhodnimi verzijami, ki shranijo manj parametrov
            if(Stream.Pos()<posChunk+sizeChunk){   //sizeChunk se ni prebran do konca - beremo dodatne parametre v novem loaderju
           //	Stream.ReadInt()	'test
           //	Stream.ReadByte()	'test
            }
           //.
           //.

           //---kompatibilnost z bodocimi verzijami, ki bodo mogoce shranile dodatne parametre
            if(Stream.Pos()<posChunk+sizeChunk){   //ce loader prebere vse parametre trenutne verzije in sizeChunk se vedno ni dosezen pomeni da, je file shranjen z bodoco verzijo -> skipamo preostale podatke
                Stream.Seek(posChunk+sizeChunk);   //dodatne parametre spustimo
            }

            //---
            //(Setup(nameID, MAKE_TILES_GRID);

        // SHAPES
        //---------------------------------------------------------------------------
         }else if(signature==SaveSignature::VECTOR_SHAPES){

            assert(false);

            /*
             int nShapes = Stream.ReadInt();
             for(int i=0; i<nShapes; i++){

                 ShapeKind shapeKind = GetShapeKindFromInt(Stream.ReadInt());
                 TEShape *Shape = TEShape::Create(shapeKind);

                 if(Shape==nullptr){        // za primer če v bodoče dodamo nove shape kinds
                     TEShape *DummyShape = new TEPolylineShape();
                     DummyShape->Load(Stream, -1, 0);
                     delete DummyShape;
                 }

                 if(Shape){
                     if(Shape->Load(Stream, -1, 0)==false){
                         delete Shape;
                         return false;
                     }else{
                         Shape->shapeMapId = TEShape::shapeMapIdCounter++;
                         vectorLayer.Shapes.push_back(Shape);
                         //o->iter = Objects.end(); o->iter--;
                     }
                 }
             }
             */

        }else if(signature==SaveSignature::VECTOR_SHAPES_V2){

            int nShapes = Stream.ReadInt();
            for(int i=0; i<nShapes; i++){

                ShapeKind shapeKind = GetShapeKindFromInt(Stream.ReadInt());
                //TEShape *Shape = TEShape::Create(shapeKind);
                //EShape *Shape = objectFactory.NewVectorShape(shapeKind);

#ifdef JUGI_EDITOR
                VectorShape* Shape = VectorShape::Create(shapeKind);
#else
                VectorShape* Shape = new VectorShape(shapeKind);
#endif


                if(Shape==nullptr){        // za primer če v bodoče dodamo nove shape kinds
#ifdef JUGI_EDITOR
                    VectorShape *DummyShape = new EPolylineShape();
#else
                    VectorShape* DummyShape = new VectorShape(ShapeKind::POLYLINE);
#endif
                    //DummyShape->Load_V2NEW(Stream, -1, 0);
                    loadShape_V2NEW(Stream, *DummyShape, -1);
                    delete DummyShape;
                }


                if(Shape){
                    //if(Shape->Load_V2NEW(Stream, -1, 0)==false){
                    if(loadShape_V2NEW(Stream, *Shape, -1)==false){
                        delete Shape;
                        return false;
                    }else{


                        //Shape->shapeMapId = TEShape::shapeMapIdCounter++;
                        //vectorLayer.Shapes.push_back(Shape);
                        vectorLayer.AddLoadedShape(Shape);
                        Shape->_setParentLayer(&vectorLayer);
                    }
                }
            }

        // PARAMETERS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::PARAMETERS){

            int nParameters = Stream.ReadInt();
            for(int i=1; i<=nParameters; i++){
                vectorLayer.mParameters.push_back(CParameter());
                CParameter &PV = vectorLayer.mParameters.back();
                //PV.Load_UTF8(Stream);
                loadParameter(Stream, PV);
            }


        // PARALLAX LAYER PARAMETERS
        //---------------------------------------------------------------------------
        }else if(signature==SaveSignature::PARALLAX_LAYER_PARAMETERS){

            LoadParallaxAndScreenLayerParametersNEW(Stream, vectorLayer.mPP);
            //parallaxAndScreenLayerParameters.LoadNEW(Stream, -1);

        }else{
           //error reading stream
           //loadMessage="Load Bitmap layer error! Wrong signature:"+signature
           //Return -1
            print("Load Bitmap layer unknown signature: " + std::to_string(signature));
            if(sizeChunk>sizeCounter){
                print("Load VectorLayer error! ChunkSize error for unknown signature: " + std::to_string(signature));
                return false;
            }
            Stream.Seek(posChunk+sizeChunk);
        }

        //---
        int dSize = Stream.Pos()-posChunk;
        if(dSize!=sizeChunk){
           //error reading stream
            print("Load VectorLayer error! Wrong chunkSize for signature: " + std::to_string(signature));
            return false;
        }
        sizeCounter -= (dSize+8);   //8 za signature in size
    }

    if(Stream.Pos()-posStart!=size){
       //error reading stream
        print("Load VectorLayer error! Wrong size: " + std::to_string(Stream.Pos()-posStart) + "'(" + std::to_string(size) +" saved)'");
        return false;
    }

    loadedVectorLayer = nullptr;

    return true;
}


void MapLoader_bin::loadVectorLayerVariablesNEW(BinaryStreamReader &Stream, VectorLayer &vectorLayer)
{

    //vectorLayer.nameID = Stream.ReadUTF8encodedByteArray();
    vectorLayer._setName(Stream.ReadString());
    vectorLayer.locked = Stream.ReadByte();
    vectorLayer.hidden = Stream.ReadByte();
    vectorLayer.node()->setDrawEnabled(!vectorLayer.hidden);


    //---
    for(int i=0; i<10; i++){   //rezervirano za morebitne dodatne parametre v prihodnje
        Stream.ReadInt();
    }
}


bool MapLoader_bin::loadShape_V2NEW(BinaryStreamReader &Stream, VectorShape &shape, int _size, int _flags)
{

    //kind = Stream.ReadInt();      // kind nalozimo iz layer class-a da vemo kateri shape narediti!


    shape.mClosed = Stream.ReadInt();
    shape.mName = Stream.ReadString();
    shape.mId = Stream.ReadInt();
    shape.mDataFlags = Stream.ReadInt();
    shape.mExtra = Stream.ReadInt();

    for(int i=0; i<8; i++){
        Stream.ReadInt();
    }

    //--- CPoints
    int nCPoints = Stream.ReadInt();
    for(int i=0; i<nCPoints; i++){
        ShapePoint CP;
        CP.x = Stream.ReadInt();
        CP.y = Stream.ReadInt();
        CP.BPprev.x = Stream.ReadDouble();
        CP.BPprev.y = Stream.ReadDouble();
        CP.BPnext.x = Stream.ReadDouble();
        CP.BPnext.y = Stream.ReadDouble();

        //if(settings.IsYCoordinateUp()){
        if(yOrientation != settings.yOrientation()){
            CP.y = loadedMap->mNumTilesY * loadedMap->mTileHeight - CP.y;
            CP.BPprev.y = loadedMap->mNumTilesY * loadedMap->mTileHeight - CP.BPprev.y;
            CP.BPnext.y = loadedMap->mNumTilesY * loadedMap->mTileHeight - CP.BPnext.y;
        }

        shape.mCPoints.push_back(CP);
    }
   shape.updatePathPoints();


    //--- parameters
    if((_flags & cons::SKIP_SAVING_PARAMETERS)==0){
        int nParameters = Stream.ReadInt();
        for(int i=0; i<nParameters; i++){
            shape.mParameters.push_back(CParameter());
            CParameter &PV = shape.mParameters.back();
            //PV.Load_UTF8(Stream);
            loadParameter(Stream, PV);
        }
    }

    return true;
}


bool MapLoader_bin::loadParameter(BinaryStreamReader &Stream, CParameter &pv, int _flags)
{

    if(_flags & CParameter::SAVE_PARAMETERS_VALUE_ONLY){

        pv.active = Stream.ReadByte();
        pv.type = Stream.ReadByte();

        if(pv.type==CParameter::typeINTEGER){
            pv.value = std::to_string(Stream.ReadInt());

        }else if(pv.type==CParameter::typeFLOAT){
            pv.value = std::to_string(Stream.ReadFloat());

        }else if(pv.type==CParameter::typeBOOLEAN){
            /*
            int b = Stream.ReadByte();
            if(b==1) value = "true";
            else if(b==0) value = "false";
            else return false;
            */

        }else if(pv.type==CParameter::typeSTRING){
            pv.value = Stream.ReadString();

        }else if(pv.type==CParameter::typeVALUES_SET){
            pv.value = Stream.ReadString();

        }else{
            return false;
        }

    }else{

        pv.active = Stream.ReadByte();
        pv.type = Stream.ReadByte();
        pv.flags = Stream.ReadByte();

        pv.name = Stream.ReadString();
        pv.value = Stream.ReadString();

        pv.valueMin = Stream.ReadString();
        pv.valueMax = Stream.ReadString();
        std::string valSet = Stream.ReadString();
        if(valSet.empty()==false){
            //valuesSet = valSet.split(",");
            pv.valuesSet = StdString::splitString(valSet, ",");
        }

        if(pv.type==CParameter::typeBOOLEAN){
            if(pv.value=="1" || pv.value=="true"){
                 pv.value = "true";

            }else{
                pv.value = "false";
            }
        }
    }

    return true;
}


void MapLoader_bin::loadSourceDataSaveHelper(BinaryStreamReader& Stream)
{

    saveHelper.groupNames.clear();

    int ngroupNames = Stream.ReadInt();
    for(int i=0; i<ngroupNames; i++){
        std::string name = Stream.ReadString();
        saveHelper.groupNames.push_back(name);
    }

    //---
    saveHelper.setNames.clear();

    int nSetNames = Stream.ReadInt();
    for(int i=0; i<nSetNames; i++){
        std::string name = Stream.ReadString();
        saveHelper.setNames.push_back(name);
    }


    //---
    saveHelper.sourceSpriteNames.clear();

    int nSourceSpritesNames = Stream.ReadInt();
    for(int i=0; i<nSourceSpritesNames; i++){
        std::string name = Stream.ReadString();
        saveHelper.sourceSpriteNames.push_back(name);
    }


}








MapLoader_bin mapLoader_bin;













}
