#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmSpriteLayer.h"
#include "jmTextSprite.h"
#include "jmCommonFunctions.h"
#include "jmSceneLayout.h"
#include "jmMap.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"

#include "jpQueries.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpObjectParser.h"
#include "jpGfxObjectsCommon.h"



namespace jugimap{




GfxObject::GfxObject()
{

}


GfxObject::GfxObject(GfxObjectCfg *_cfg, BaseObject *_parentObject) : mCfg(_cfg), mParentObject(_parentObject)
{

}


GfxObject::GfxObject(const GfxObject& _src)
{

    mCfg = _src.mCfg;                                           // LINK
    mParentObject = _src.mParentObject;                            // LINK

    /*
    mData.resize(_src.mData.size());
    for(unsigned int i=0; i<mData.size(); i++){
        mData[i].first = _src.mData[i].first;
        mData[i].second = ItemData::copy(_src.mData[i].second);
    }
    */
}


GfxObject::~GfxObject()
{
    /*
    for(auto &p : mData){
        delete p.second;
    }
    */

}


/*
ItemData* GfxObject::findData(const std::string &_name, bool _setErrorMessage)
{

    for(auto &p : mData){
        if(p.second->name()==_name){
            return p.second;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Data '" + _name + "' not found!");
    }

    return nullptr;

}
*/



void GfxObject::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{
}



void GfxObject::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{
}



//===========================================================================================


bool AniSpriteObjectCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading aniSpriteObject '" + mName +"' ...");


    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        /*
        if(nodeName=="int"){

            IntItemData *sid = new IntItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(data.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="float"){

            FloatItemData *sid = new FloatItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(data.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="bool"){

            BoolItemData *sid = new BoolItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(data.addObject(sid)==nullptr){
                return false;
            }


        }else
        */
        if(nodeName=="new"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sourceSprite"){
                    sourceSprite = a.as_string();

                }else if(attributeName=="animation"){
                    animation = a.as_string();

                }else if(attributeName=="layer"){
                    layer = a.as_string();

                }else if(attributeName=="map"){
                    map = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        /*
        }else if(nodeName=="dataConnections"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="text"){
                    std::string sValue = a.as_string();
                    dataConnections.push_back(DataConnectionCfg(DataConnectionType::TEXT, sValue));

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }
        */

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}




//====================================================================================================


AniSpriteObject::AniSpriteObject(GfxObjectCfg *_cfg, BaseObject *_parentObject) : GfxObject(_cfg, _parentObject)
{

}


AniSpriteObject::AniSpriteObject(const AniSpriteObject &_src) : GfxObject(_src)
{

    mSprite = _src.mSprite->makePassiveCopy();
    mSpriteOwned = true;

    mSpriteLayer = _src.mSpriteLayer;
    mAnimationInstance = ObtainAnimationInstance(mSprite, _src.mAnimationInstance->GetAnimation()->GetName());
    assert(mAnimationInstance);

}


AniSpriteObject::~AniSpriteObject()
{
    if(mSpriteOwned){
        delete mSprite;
    }
    delete mAnimationInstance;
}



bool AniSpriteObject::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing aniSpriteObject '" + name() +"' ...");


    AniSpriteObjectCfg *cfg = static_cast<AniSpriteObjectCfg*>(mCfg);


    SourceSprite *sourceSprite = FindSourceSpriteWithName(cfg->sourceSprite);
    if(sourceSprite==nullptr){
        return false;
    }

    mSprite = Sprite::makePassiveSprite(sourceSprite);
    mSpriteOwned = true;

    //----
    for(SceneMap *sm : _scene->sceneMaps()){
        if(sm->info.name == cfg->map){
            Layer * layer = FindLayerWithName(sm->map(), cfg->layer, LayerKind::SPRITE_LAYER);
            if(layer){
                mSpriteLayer = static_cast<SpriteLayer*>(layer);
                break;
            }
        }
    }

    if(mSpriteLayer==nullptr){
        dbgSystem.addMessage("Sprite layer '" + cfg->map +"' not found!");
        return false;
    }


    //----
    mAnimationInstance = ObtainAnimationInstance(mSprite, cfg->animation);
    if(mAnimationInstance==nullptr){
        return false;
    }


    //----
    /*
    for(DataConnectionCfg &dcf : cfg->dataConnections){
        if(dcf.type==DataConnectionType::TEXT){

            if(mSprite->type()!=SpriteType::TEXT){
                dbgSystem.addMessage("Data connection is of type 'text' but the sprite is not a text sprite!");
                return false;
            }

            ItemData *d =  cfg->data.getObject(dcf.data);
            if(d==nullptr){
                return false;
            }
            ItemData *dCopy = ItemData::copy(d);
            mData.push_back({DataConnectionType::TEXT, dCopy});
        }
    }
    */


    //----
    dbgSystem.removeLastMessage();
    return true;
}


void AniSpriteObject::start(Vec2f _pos)
{
    /*
    for(auto &p : mData){
        if(p.first==DataConnectionType::TEXT){

            std::string text = p.second->valueString();

            TextSprite *ts = static_cast<TextSprite*>(mSprite);
            ts->setText(text);
        }
    }
    */

    //----
    mSprite->_setParentLayer(mSpriteLayer);
    mSprite->setPosition(_pos);
    mSprite->initEngineObjects();

    mSpriteLayer->addSprite(mSprite);
    mSpriteOwned = false;

    //----
    mAnimationPlayer.Stop();

    int aniPlayerFlags = 0;
    if(mAnimationPlayer.GetAnimationInstance() != mAnimationInstance){
        aniPlayerFlags = mAnimationPlayer.Play(mAnimationInstance);
    }

    if(aniPlayerFlags & AnimationPlayerFlags::ANIMATED_PROPERTIES_CHANGED){
        mAnimationInstance->UpdateAnimatedSprites(true);
    }

}


void AniSpriteObject::update()
{

    int aniPlayerFlags = mAnimationPlayer.Update();

    if(aniPlayerFlags & AnimationPlayerFlags::ANIMATED_PROPERTIES_CHANGED){
        mAnimationInstance->UpdateAnimatedSprites(true);
    }

    if(mAnimationPlayer.GetState()==AnimationPlayerState::STALLED){
        end();
    }

}


bool AniSpriteObject::isAnimationStalled()
{

    if(mAnimationPlayer.GetState()==AnimationPlayerState::STALLED){
        return true;
    }

    return false;
}


bool AniSpriteObject::isAnimationStopped()
{

    if(mAnimationPlayer.GetState()==AnimationPlayerState::IDLE){
        return true;
    }

    return false;
}


void AniSpriteObject::end()
{

    mAnimationPlayer.Stop();

    mSpriteLayer->takeSprite(mSprite);        // remove sprite from layer
    mSpriteOwned = true;

}


//============================================================================================



GSpritesObject::~GSpritesObject()
{

}


GSpritesObject::GSpritesObject(const pugi::xml_node &_node)
{

    //mBaseType = BaseObjectType::GFX_OBJECT__SPRITES;

    mName = _node.attribute("name").as_string("");
    mSpritesObjectType = GetGfxObjectTypeFromString(_node.attribute("objectType").as_string(""));
    mObjectName = _node.attribute("objectName").as_string("");
    mSrcMapName = _node.attribute("objectSourceMap").as_string("");

}



GSpritesObject::GSpritesObject(std::vector<void*>_objects, GSpritesObjectType _objType)
{
    mObjects = _objects;
    mSpritesObjectType = _objType;
}



bool GSpritesObject::initConnections(PlayedScene *_scene)
{

    //mParentPlayerScene = _scene;

    dbgSystem.addMessage("init sprites object '" + mName + "'");


    if(mSpritesObjectType==GSpritesObjectType::NOT_DEFINED){
        dbgSystem.addMessage("Wrong object type!");
        return false;
    }

    if(collectObjects(mObjects, _scene, mSpritesObjectType, mObjectName, mSrcMapName)==false){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;

}



void GSpritesObject::setVisible(bool _visible)
{
     _setVisible(_visible);
     mSigVisible.reset(_visible);
}



void GSpritesObject::_setVisible(bool _visible)
{

    if(mSpritesObjectType==GSpritesObjectType::SPRITE){

        for(void* obj : mObjects){
            Sprite * sprite = static_cast<Sprite*>(obj);
            sprite->setVisible(_visible);
        }

    }else if(mSpritesObjectType==GSpritesObjectType::SPRITE_LAYER){

        for(void* obj : mObjects){
            SpriteLayer * spriteLayer = static_cast<SpriteLayer*>(obj);
            spriteLayer->setVisible(_visible);
        }

    }else if(mSpritesObjectType==GSpritesObjectType::MAP){

        for(void* obj : mObjects){
            Map * map = static_cast<Map*>(obj);
            map->setVisible(_visible);
        }

    }

}



bool GSpritesObject::onSignalSet(Signal *_signal)
{

    if(_signal==&mSigVisible){
        bool value = mSigVisible.value();
        _setVisible(value);
        return true;
    }

    return false;
}



void GSpritesObject::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalQuery, &mSigVisible);
    }

    if(_setErrorMessage && _signalQuery.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}



void GSpritesObject::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalSetter, &mSigVisible);
    }

    if(_setErrorMessage && _signalSetter.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }
}



//===========================================================================================


bool GTextSpriteCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading textSpriteObject '" + mName +"' ...");

    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="new"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sourceSprite"){
                    sourceSprite = a.as_string();

                }else if(attributeName=="layer"){
                    layer = a.as_string();

                }else if(attributeName=="map"){
                    map = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="existing"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="textSprite"){
                    textSprite = a.as_string();

                }else if(attributeName=="map"){
                    map = a.as_string();

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



/*
GTextSprite::GTextSprite(const pugi::xml_node &_node)
{

    //mBaseType = BaseObjectType::GFX_OBJECT__TEXT_SPRITE;

    mName = _node.attribute("name").as_string("");
    mTextSpriteName = _node.attribute("textSprite").as_string("");
    mSourceMapName = _node.attribute("sourceMap").as_string("");

}
*/

/*
bool GTextSprite::build(PlayedScene *_scene, GfxObjectCfg *_cfg, BaseObject *_parent)
{

    mCfg = static_cast<GTextSpriteCfg*>(_cfg);
    mParentObject = _parent;

    return true;
}

*/

bool GTextSprite::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("Initializing textSpriteObject '" + name() +"' ...");


    GTextSpriteCfg *cfg = static_cast<GTextSpriteCfg*>(mCfg);



    /*
    std::vector<Sprite*>sprites;
    for(SceneMap *sm : _scene->sceneMaps()){
        if(sm->map()->type()==MapType::SCREEN){
            CollectSpritesWithName(sm->map(), sprites, cfg->textSprite);
            if(sprites.empty()==false){
                break;
            }
        }
    }
    */

    std::vector<Sprite*>sprites;
    for(SceneMap *sm : _scene->sceneMaps()){
        if(sm->info.name==cfg->map){
            CollectSpritesWithName(sm->map(), sprites, cfg->textSprite);
            if(sprites.empty()==false){
                break;
            }
        }
    }

    if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){
        mTextSprite = static_cast<TextSprite*>(sprites.front());

        //if(app->usersDatabase()->activeUser()){
        //    mTextSprite->setText(app->usersDatabase()->activeUser()->name());
        //}

    }else{
        dbgSystem.addMessage("Text sprite with name '" + cfg->textSprite + "' not found!");
    }


    //---
    mSigVisible.setCallback(this);
    mSigText.setCallback(this);


    dbgSystem.removeLastMessage();
    return true;
}



//void GTextSprite::setVisible(bool _visible)
//{
//    mTextSprite->setVisible(_visible);
//}

/*
void GTextSprite::onSetBySignalSetter(Signal *_signal)
{

    if(_signal==&mSigText){
        const std::string & value = mSigText.value_currentOrNextIfSet();
        mTextSprite->setText(value);

    }else if(_signal==&mSigTextVisible){
        bool value = mSigTextVisible.value_currentOrNextIfSet();
        mTextSprite->setVisible(value);

    }



}
*/


bool GTextSprite::onSignalSet(Signal *_signal)
{

    if(_signal==&mSigText){
        const std::string & value = mSigText.value();
        mTextSprite->setText(value);
        return true;

    }else if(_signal==&mSigVisible){
        bool value = mSigVisible.value();
        mTextSprite->setVisible(value);
        return true;
    }

    return false;
}


void GTextSprite::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="TEXT"){
        _psp.obtainValue(_signalQuery, &mSigText);

    }else if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalQuery, &mSigVisible);
    }

    if(_setErrorMessage && _signalQuery.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void GTextSprite::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="TEXT"){
        _psp.obtainValue(_signalSetter, &mSigText);

    }else if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalSetter, &mSigVisible);
    }

    if(_setErrorMessage && _signalSetter.signal()==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


//===========================================================================================


void createParabolicPath(std::vector<PathPoint>&_pathPoints, float &_pathLength, float _distance, float _height, float _maxLength)
{

    // formula  y = a(x-cx)^2 + cy
    // at x=0 -> y=0 -> a = -(cy/(cx^2))

    float cx = _distance/2.0f;
    float cy = _height;
    float a = -(cy/(cx*cx));


    float xStep = _distance/10.0f;
    float dAngleMaxDeg = 10;
    float dAngleMinDeg = 5;

    Vec2f pPrev, p, pNext;
    float angle_pPrev_p = 0.0f;

    pPrev.x = 0.0f;
    pPrev.y = a*(pPrev.x-cx)*(pPrev.x-cx) + cy;
    _pathPoints.push_back(PathPoint(pPrev));


    //--- find second point

    float fStep = 1.0f;
    while(true){
        Vec2f pTest1;
        pTest1.x = pPrev.x + xStep/fStep;
        pTest1.y = a*(pTest1.x-cx)*(pTest1.x-cx) + cy;

        float dist = DistanceTwoPoints(pPrev, pTest1);
        if(dist<5){
            _pathPoints.push_back(PathPoint(pTest1));
            p = pTest1;
            break;
        }

        //---
        float angle1Deg = AngleBetweenTwoPoints(pPrev, pTest1);
        //if(angle1Deg<0.0f){
        //    angle1Deg += 360.0f;
        //}

        Vec2f pTest2;
        pTest2.x = pTest1.x + xStep/fStep;
        pTest2.y = a*(pTest2.x-cx)*(pTest2.x-cx) + cy;

        float angle2Deg = AngleBetweenTwoPoints( pTest1, pTest2);
        //if(angle2Deg<0.0f){
        //    angle1Deg += 360.0f;
        //}

        if(std::abs(angle1Deg - angle2Deg) <= dAngleMaxDeg){
            _pathPoints.push_back(PathPoint(pTest1));
            p = pTest1;
            break;

        }else{
            fStep++;
        }
    }

    _pathLength = DistanceTwoPoints(pPrev, p);
    angle_pPrev_p = AngleBetweenTwoPoints(pPrev, p);
    //if(angle_pPrev_p<0.0f){
    //    angle_pPrev_p += 360.0f;
    //}


    //---
    while(true){

        float fStep = 1.0f;
        while(true){

            pNext.x = p.x + xStep/fStep;
            pNext.y = a*(pNext.x-cx)*(pNext.x-cx) + cy;

            float angle_p_pNext = AngleBetweenTwoPoints(p, pNext);
            //if(angle_p_pNext<0.0f){
            //    angle_p_pNext += 360.0f;
            //}
            float dist = DistanceTwoPoints(p, pNext);

            float dAngle = std::abs(angle_p_pNext - angle_pPrev_p);
            if(dist<10 || (dAngle<=dAngleMaxDeg && dAngle>=dAngleMinDeg)){
                _pathPoints.push_back(PathPoint(pNext));
                _pathLength += dist;
                pPrev = p;
                p = pNext;
                angle_pPrev_p = angle_p_pNext;
                break;

            }else{

                if(dAngle>dAngleMaxDeg){
                    fStep++;

                }else{  // dAngle<dAngleMinDeg

                    fStep *= 0.5;
                }


            }
        }

        //if(std::abs(angle_pPrev_p - 270.0f) < 2.0f){
        //if(std::abs(angle_pPrev_p + 90.0f) < 2.0f){
        //    break;
        //}
        if(_pathLength>_maxLength){
            break;
        }


    }



}


bool GORelativeMoverCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading relativeMover '" + mName +"' ...");


    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="kind"){
                    std::string sValue = a.as_string();
                    if(sValue=="LINEAR"){
                        kind = MoverKind::LINEAR_MOVER;

                    }else if(sValue=="FOLLOWER"){
                        kind = MoverKind::FOLLOWER;

                    }else if(sValue=="PARABOLA"){
                        kind = MoverKind::PARABOLA;

                    }else if(sValue=="PATH"){
                        kind = MoverKind::PATH;

                    }else{
                        dbgSystem.addMessage("Unknown mover kind '" + sValue + "' !");
                        return false;
                    }

                }else if(attributeName=="speedP"){
                    std::string sValue = a.as_string();
                    if(StdString::floatNumber(sValue, speed)==false){
                        return false;
                    }

                //}else if(attributeName=="distanceP"){
                //    std::string sValue = a.as_string();
                //    if(StdString::integerNumber(sValue, distanceP)==false){
                //        return false;
                //    }

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="onEnd"){
            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="tags"){
                    std::string sValue = a.as_string();
                    tags = StdString::splitString(sValue, ",");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="fieldSpeed"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="angle"){
                    std::string sValue = a.as_string();
                    if(StdString::floatNumber(sValue, fieldSpeedAngleDeg)==false){
                        return false;
                    }

                }else if(attributeName=="speedP"){
                    std::string sValue = a.as_string();
                    if(StdString::floatNumber(sValue, fieldSpeed)==false){
                        return false;
                    }
                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="parentDirections"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                directions.emplace_back();
                DirectionCfg &dirCfg = directions.back();

                if(childNodeName=="right"){
                    dirCfg.parentDirection = Direction::RIGHT;

                }else if(childNodeName=="left"){
                    dirCfg.parentDirection = Direction::LEFT;

                }else if(childNodeName=="up"){
                    dirCfg.parentDirection = Direction::UP;

                }else if(childNodeName=="down"){
                    dirCfg.parentDirection = Direction::DOWN;

                }else{

                    dbgSystem.addMessage("Unknown node '" + childNodeName + "' !");
                    return false;
                }

                for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                    std::string attributeName =std::string(a.name());

                    if(attributeName=="angle"){
                        std::string sValue = a.as_string();
                        if(StdString::floatNumber(sValue, dirCfg.movingDirectionAngleDeg)==false){
                            return false;
                        }
                    }else if(attributeName=="animation"){
                        dirCfg.animation = a.as_string();

                    }else if(attributeName=="path"){
                        dirCfg.pathName = a.as_string();

                    }else if(attributeName=="distanceP"){
                        std::string sValue = a.as_string();
                        if(StdString::floatNumber(sValue, dirCfg.distanceP)==false){
                            return false;
                        }

                    }else if(attributeName=="heightP"){
                        std::string sValue = a.as_string();
                        if(StdString::floatNumber(sValue, dirCfg.heightP)==false){
                            return false;
                        }
                    }else{

                        dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                        return false;
                    }
                }

                if(kind==MoverKind::PARABOLA){
                    if(dirCfg.distanceP==0.0f || dirCfg.heightP==0.0f){
                        dbgSystem.addMessage("Missing dimensions for parabolic mover shape!");
                        return false;

                    }else{

                        createParabolicPath(dirCfg.pathPoints, dirCfg.pathLength, dirCfg.distanceP, dirCfg.heightP, dirCfg.pathLengthMax);

                    }
                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


//============================================================================================



void PathMover::start(Vec2f _startPos, std::vector<PathPoint> *_pathPoints, float _pathLength, float _speed)
{

    mCurrentPos = _startPos;
    mPathPoints = _pathPoints;
    mPathLength = _pathLength;
    mSpeed = _speed;

    mVelocity.set(0.0f, 0.0f);
    mPassedDistance = 0.0f;
    mStartIndex = 0;
    mCurrentIndex = 0;
    mEndIndex = mPathPoints->size()-1;

    mCurrentPosP = mPathPoints->front();
    mCurrentDistanceP = 0.0f;
    mDistance = mPathLength;

}


Vec2f PathMover::updateVelocity(float _timeStep)
{

    mPassedDistance += mSpeed * _timeStep;

    bool unitVelocityObtained = obtainUnitVelocity(mPassedDistance);
    if(unitVelocityObtained){
        mVelocity = mSpeed * mUnitVelocity;
        mCurrentPos += mVelocity * _timeStep;
    }

    //return mVelocity;
    return mUnitVelocity;
}


bool PathMover::isFinished()
{
    return mPassedDistance>=mDistance;

}


bool PathMover::obtainUnitVelocity(float passedDistanceP)
{

    //std::vector<PathPoint> & pathPoints = mPath->pathPoints();


    float dist = mCurrentDistanceP;
    int nextIndex = mCurrentIndex;

    while(true){

        nextIndex += 1;

        PathPoint p1 = (*mPathPoints)[mCurrentIndex];
        PathPoint p2 = (*mPathPoints)[nextIndex];
        float d = DistanceTwoPoints(p1, p2);
        dist += d;

        if(nextIndex == mEndIndex){
            break;
        }
        if(passedDistanceP < dist){
            break;

        }else{
            mCurrentIndex = nextIndex;
            mCurrentDistanceP += d;
        }
    }


    //---
    if(mCurrentIndex!=nextIndex){
        PathPoint p1 = (*mPathPoints)[mCurrentIndex];
        PathPoint p2 = (*mPathPoints)[nextIndex];
        //float d = DistanceTwoPoints(p1, p2);
        //mUnitVelocity.Set((p1.x-p2.x)/d, (p1.y-p2.y)/d);

        Vec2f newCurrentPosP = MakeVec2fAtDistanceFromP1(p2, p1, dist - passedDistanceP);

        float d = DistanceTwoPoints(newCurrentPosP, mCurrentPosP);
        mUnitVelocity.set((newCurrentPosP.x-mCurrentPosP.x)/d, (newCurrentPosP.y-mCurrentPosP.y)/d);

        mCurrentPosP = newCurrentPosP;

    }else{
        return false;
    }

    return true;

}


//============================================================================================


/*
void FollowMover::start(Vec2f _startPos, float _startAngle, Vec2f _endPos, float _speed)
{

    mUnitVelocity = normalVectorFromAngleDeg(_startAngle);
    mStartUnitVelocity = mUnitVelocity;

    mStartPos = _startPos;
    mCurrentPos = mStartPos;
    //mEndPos = _endPos;
    mSpeed = _speed;

    mTargetPos = _endPos;
    mTargetDistance = DistanceTwoPoints(mStartPos, mTargetPos);

    mFollowingDynamicPos = false;
}
*/


void FollowMover::start(Vec2f _startPos, Vec2f _startUnitVelocity, Vec2f _endPos)
{

    //mStartPos = _startPos;
    mCurrentPos = _startPos;
   // mStartUnitVelocity = _startUnitVelocity;
    mAngle_current = std::atan2(_startUnitVelocity.y, _startUnitVelocity.x) * 180.0/mathPI;
    if(mAngle_current<0.0f){
        mAngle_current += 360.0f;
    }

    mTargetPos = _endPos;
    //mTargetDistance = DistanceTwoPoints(mStartPos, mTargetPos);
    mRotationSpeed = mRotationSpeedDef;
    //fSpeed = 1.0;
    mDistCurrentTarget = DistanceTwoPoints(mCurrentPos, mTargetPos);

    mDistNear = mDistCurrentTarget/2.0;

}


Vec2f FollowMover::updateVelocity(float _timeStep, Vec2f _currentPos, Vec2f _targetPos, float _speed)
{

    /*
    if(mDistCurrentTarget <= mDistNear){
        mRotationSpeed = mRotationSpeedDef + mRotationSpeedDef*(1.0f - mDistCurrentTarget/mDistNear);
        _fSpeed = 1.0 - 0.25f * (1.0f - mDistCurrentTarget/mDistNear);
    }else{
        mRotationSpeed = mRotationSpeedDef;
        _fSpeed = 1.0f;
    }
    */

    mTargetPos = _targetPos;
    mAngle_target = AngleBetweenTwoPoints(_currentPos, mTargetPos);
    if(mAngle_target<0.0f){
        mAngle_target += 360.0f;
    }

    //float distCurrentTarget = DistanceTwoPoints(_currentPos, mTargetPos);

    float sign = (mAngle_target > mAngle_current)? 1.0f : -1.0 ;
    float deltaAngle = std::abs(mAngle_target - mAngle_current);

    if(deltaAngle>180.0f){
         deltaAngle = 360.0f - deltaAngle;
         sign *= -1.0f;
    }

    mAngle_current += sign * mRotationSpeed * _timeStep;
    if(mAngle_current<0.0f){
        while(mAngle_current<0.0f){
            mAngle_current += 360.0f;
        }
    }else if(mAngle_current>360.0f){
        while(mAngle_current>360.0f){
            mAngle_current -= 360.0f;
        }
    }

    float _angle_target = mAngle_target;
    float _angle_current = mAngle_current;

    if(mDistCurrentTarget < mDistNear){
        if(_angle_target - _angle_current > 180.0f){
            _angle_target -= 360.0f;

        }else if(_angle_current - _angle_target > 180.0f){
            _angle_current -= 360.0f;
        }

        float p =  mDistCurrentTarget/mDistNear;
        _angle_current = p*_angle_current + (1.0f-p)*_angle_target;
    }


    mUnitVelocity = normalVectorFromAngleDeg(_angle_current);

    mVelocity = _speed * mUnitVelocity;
    mCurrentPos += mVelocity * _timeStep;

    mDistCurrentTarget = DistanceTwoPoints(mCurrentPos, mTargetPos);


    return mUnitVelocity;

}


bool FollowMover::isTargetReached(float threeshold)
{


    print("dist="+std::to_string(mDistCurrentTarget));

    return mDistCurrentTarget<threeshold;

}


//============================================================================================


GORelativeMover::GORelativeMover(GfxObjectCfg *_cfg, BaseObject *_parentObject, Sprite *_sprite) : GfxObject(_cfg, _parentObject)
{
    mSprite = _sprite;
}


GORelativeMover::GORelativeMover(const GORelativeMover &_src) : GfxObject(_src)
{

    //mSprite = _src.mSprite->makePassiveCopy();
    //mSpriteOwned = true;

    //mSpriteLayer = _src.mSpriteLayer;
    //mAnimationInstance = ObtainAnimationInstance(mSprite, _src.mAnimationInstance->GetAnimation()->GetName());
    //assert(mAnimationInstance);

}


GORelativeMover::~GORelativeMover()
{
    //if(mSpriteOwned){
    //    delete mSprite;
    //}

    for(DirectionMover &dm : mDirectionMovers){
        if(dm.mAnimationInstance){
            delete dm.mAnimationInstance;
        }
    }

}


bool GORelativeMover::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing relativeMover '" + name() +"' ...");


    GORelativeMoverCfg *cfg = static_cast<GORelativeMoverCfg*>(mCfg);


    mDirectionMovers.resize(cfg->directions.size());
    for(unsigned int i=0; i<mDirectionMovers.size(); i++){
        GORelativeMoverCfg::DirectionCfg &dmCfg = cfg->directions[i];
        DirectionMover &dm = mDirectionMovers[i];
        dm.mCfg = &dmCfg;
        if(dmCfg.animation.empty()==false){
            dm.mAnimationInstance = ObtainAnimationInstance(mSprite, dmCfg.animation);
            if(dm.mAnimationInstance==nullptr){
                return false;
            }
        }
        if(dmCfg.pathName.empty()==false){
            dmCfg.mPath = FindVectorShapeWithName(_scene, dmCfg.pathName);
            if(dmCfg.mPath==nullptr){
                dbgSystem.addMessage("Vector shape '" + dmCfg.pathName + "' not found!");
                return false;
            }
        }
    }

    //----
    if(cfg->kind==MoverKind::PATH || cfg->kind==MoverKind::PARABOLA){
        mPathMover.reset(new PathMover());

        if(StdVector::contains(cfg->tags, std::string{"FOLLOW_SPAWNER"})){
            mFollowMover.reset(new FollowMover());
        }

    }else if(cfg->kind==MoverKind::FOLLOWER){
        mFollowMover.reset(new FollowMover());

    }


    //----
    dbgSystem.removeLastMessage();
    return true;
}


AnimationInstance * GORelativeMover::currentAnimationInstance()
{

    if(mCurrentDirectionMover){
        return mCurrentDirectionMover->mAnimationInstance;
    }

    return nullptr;
}


void GORelativeMover::start(Vec2f _pos, Direction _parentDirection, BaseObject *_followedObject)
{

    GORelativeMoverCfg *cfg = static_cast<GORelativeMoverCfg*>(mCfg);


    //----
    mCurrentDirectionMover = nullptr;

    for(DirectionMover &dm : mDirectionMovers){
        if(dm.mCfg->parentDirection==_parentDirection){
            mCurrentDirectionMover = &dm;
            break;
        }
    }
    if(mCurrentDirectionMover==nullptr){
        return;
    }

    mKind = cfg->kind;
    mSpeed = cfg->speed;
    if(mFollowMover.get()){
        mFollowedObject = _followedObject;
    }

    //----
    if(mKind==MoverKind::LINEAR_MOVER){
        mUnitVelocity = normalVectorFromAngleDeg(mCurrentDirectionMover->mCfg->movingDirectionAngleDeg);

    }else if(mKind==MoverKind::PATH){
        mPathMover->start(_pos, &mCurrentDirectionMover->mCfg->mPath->pathPoints(), mCurrentDirectionMover->mCfg->mPath->pathLength(), mSpeed);

    }else if(mKind==MoverKind::PARABOLA){
        mPathMover->start(_pos, &mCurrentDirectionMover->mCfg->pathPoints, mCurrentDirectionMover->mCfg->pathLength, mSpeed);

    }else if(mKind==MoverKind::FOLLOWER){
                Vec2f startUnitVelocity = normalVectorFromAngleDeg(mCurrentDirectionMover->mCfg->movingDirectionAngleDeg);
        Vec2f targetPos = mFollowedObject->position();
        mFollowMover->start(_pos, startUnitVelocity, targetPos);
    }

    mState = MoverState::MOVING;

}


Vec2f GORelativeMover::update(float _timeStep, Vec2f _currentPos)
{

    if(mCurrentDirectionMover==nullptr){
        return Vec2f{0.0f, 0.0f};
    }
    GORelativeMoverCfg *cfg = static_cast<GORelativeMoverCfg*>(mCfg);


    if(mState==MoverState::IDLE){
        mVelocity.set(0.0f, 0.0f);


    }else if(mState==MoverState::MOVING){

        //if(mKind==MoverKind::LINEAR_MOVER){
        //    mVelocity = cfg->speed * mUnitVelocity;
        //
        //}else

        if(mKind==MoverKind::FOLLOWER){

            mUnitVelocity = mFollowMover->updateVelocity(_timeStep, _currentPos, mFollowedObject->position(), mSpeed);
            if(mFollowMover->isTargetReached()){
                mSpeed = 0.0f;
                mState = MoverState::IDLE;
            }

        }else if(mKind==MoverKind::PATH || mKind==MoverKind::PARABOLA){

            mUnitVelocity = mPathMover->updateVelocity(_timeStep);

            if(mPathMover->isFinished()){
                if(mFollowMover.get()){
                    mFollowMover->start(_currentPos, mPathMover->unitVelocity(), mFollowedObject->position());
                    mKind = MoverKind::FOLLOWER;

                }else{
                    mSpeed = 0.0f;
                    mState = MoverState::IDLE;
                }
            }
        }

        mVelocity = mSpeed * mUnitVelocity;
    }

    return mVelocity;
}




void GORelativeMover::finish()
{

    mCurrentDirectionMover = nullptr;
    mState = MoverState::IDLE;

}




//===========================================================================================


bool GfxObjectsGroup::build(PlayedScene *_scene)
{

    dbgSystem.addMessage("Build gfx objects ...");


    for(GfxObject* o : mGfxObjects){
        if(o->initConnections(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();

    return true;
}


bool GfxObjectsGroup::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("Initialize gfx objects ...");


    for(GfxObject* o : mGfxObjects){
        if(o->initConnections(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();

    return true;
}



GfxObject* GfxObjectsGroup::getGfxObject(const std::string &_name)
{


    for(GfxObject* o : mGfxObjects){
        if(o->name()==_name){
            return o;
        }
    }

    dbgSystem.addMessage("Gfx object '" + _name +"' not found!");

    return nullptr;

}


//============================================================================================





}
