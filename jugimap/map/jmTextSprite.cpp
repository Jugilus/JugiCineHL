#include <algorithm>
#include <assert.h>
#include "jmSystem.h"
#include "jmCommon.h"
#include "jmCommonFunctions.h"
#include "jmSourceSprite.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"
#include "jmFrameAnimation.h"
#include "jmAnimationCommon.h"
#include "jmCollision.h"
#include "jmMap.h"
#include "jmLayer.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmFont.h"
#include "jmColor.h"
#include "jmGuiText.h"
#include "jmTextSprite.h"



namespace jugimap {




TextSpriteNode::TextSpriteNode(TextSprite *_s): s(_s)
{
    type_ = ncine::Object::ObjectType::TEXTNODE;
    setDeleteChildrenOnDestruction(false);
    setRenderMode(ncine::Font::RenderMode::GLYPH_SPRITE);
}


TextSpriteNode::~TextSpriteNode()
{
    //delete s;

}


void TextSpriteNode::transform()
{

    if(s->text()=="Sorry, "){
        DummyFunction();
    }

    ncine::TextNode::transform();

    if (dirtyBits_.test(DirtyBitPositions::TransformationBit) != false){
        s->onTransform();
    }

    /*
    if(s->activeImage){
        s->mWorldBoundingBox = TransformRectf(Rectf(-s->activeImage->GetWidth()*0.5f, -s->activeImage->GetHeight()*0.5, s->activeImage->GetWidth()*0.5f, s->activeImage->GetHeight()*0.5f), worldMatrix());
        s->setBoundingBoxUpdateRequired(true);

        // solution bellow would be also OK!
        //updateAabb();
        //mWorldBoundingBox.set(Vec2f(ncine::Sprite::aabb().x, ncine::Sprite::aabb().y), Vec2f(ncine::Sprite::aabb().x+ncine::Sprite::aabb().w, ncine::Sprite::aabb().y+ncine::Sprite::aabb().h));

        if(s->collider){
            s->collider->UpdateShapes(worldMatrix());
        }
    }
    */
}


//void TextSpriteNode::setTransformationBitsDirty()
//{

//    dirtyBits_.set(DirtyBitPositions::TransformationBit);

//}



//---------------------------------------------------------------------------------------


TextSprite::TextSprite()
{

    mNode = new TextSpriteNode(this);
}


TextSprite::~TextSprite()
{
    delete mNode;
}


void TextSprite::setFont(Font *_font)
{
    mFont = _font;
    static_cast<TextSpriteNode*>(mNode)->setFont(mFont->GetNcFont());

}


void TextSprite::setColor(ColorRGBA _color)
{
    mColor = _color;
    mNode->setColor(_color.toNcineColor());
}


void TextSprite::setRelativeHandle(Vec2f _relHandle, bool adjustToPixelCoordinates)
{

    mRelativeHandle = _relHandle;

    float w = static_cast<TextSpriteNode*>(mNode)->width();
    float h = static_cast<TextSpriteNode*>(mNode)->height();

    w = w / mNode->scale().x;       // we must use unscaled size
    h = h / mNode->scale().y;


    if(AreEqual(w, 0.0f)==false && AreEqual(h, 0.0f)==false){

        if(adjustToPixelCoordinates){
            float hx = w * mRelativeHandle.x;
            if(hx>0.001){
                float hxRounded = std::roundf(hx);
                mRelativeHandle.x = hxRounded / w;
            }
            float hy = h * mRelativeHandle.y;
            if(hy>0.001){
                float hyRounded = std::roundf(hy);
                mRelativeHandle.y = hyRounded / h;
            }
        }

        mHandle.x = w*mRelativeHandle.x;
        mHandle.y = h*mRelativeHandle.y;
        //static_cast<TextSpriteNode*>(mNode)->setAnchorPoint(mRelativeHandle.x, mRelativeHandle.y);

        ncine::Vector2f anchorPoint(mHandle.x - w * 0.5f, mHandle.y - h*0.5f);
        mNode->setAbsAnchorPoint(anchorPoint);
    }

    //---- update bounding box
    mWorldBoundingBox = TransformRectf(BoundingBoxFloat(-w*0.5f, -h*0.5, w*0.5f, h*0.5f), mNode->worldMatrix());
    setParentBoundingBoxUpdateRequired(true);


    /*
    if(AreEqual(w, 0.0f)==false && AreEqual(h, 0.0f)==false){

        if(adjustToPixelCoordinates){
            float hx = w * (mRelativeHandle.x-0.5);
            if(hx>0.001){
                float hxRounded = std::roundf(hx);
                mRelativeHandle.x = hxRounded / w + 0.5;
            }
            float hy = h * (mRelativeHandle.y-0.5);
            if(hy>0.001){
                float hyRounded = std::roundf(hy);
                mRelativeHandle.y = hyRounded / h + 0.5;
            }
        }

        mHandle.x = w*mRelativeHandle.x;
        mHandle.y = h*mRelativeHandle.y;
        static_cast<TextSpriteNode*>(mNode)->setAnchorPoint(mRelativeHandle.x, mRelativeHandle.y);
    }
    */

}


/*
bool TextSprite::setFromSourceItemData()
{
    TextLabel *tl = sourceSprite()->sourceImages().front()->textLabel();
    if(tl==nullptr){
        return false;
    }

    setFont(tl->font);
    setColor(tl->sourceColor);


    return true;
}
*/


void TextSprite::initEngineObjects()
{


    if(isInitialized()) return;

    if(mFont){
        return;
    }



    //if(mSourceSpriteReplacement){
    //    mOriginalSourceSprite = mSourceSprite;
    //    mSourceSprite = mSourceSpriteReplacement;
    //}


    if(mBookName!=""){
        TextSegment *ts = textLibrary.GetTextSegmentFromBook(mBookName, mBookSegmentId);
        if(ts){
            mText = ts->plainText();
        }
    }


    //--- if text sprite is loaded from the editor then the string and the style are obtained from the graphic item !
    if(sourceSprite() && sourceSprite()->sourceImages().empty()==false){

        //mText = sourceSprite()->sourceImages().front()->label();
        SourceImage *si = sourceSprite()->sourceImages().front();
        TextLabel *tl = si->textLabel();
        assert(tl);

        if(mText=="OPTIONS"){
            DummyFunction();
        }
        //int fontId = tl->fontStyle->id;     // sourceSprite()->sourceImages().front()->fontStyleId();
        //if(fontId>=0 && fontId<fontLibrary.fonts().size()){
        //    mFont = fontLibrary.getFont(fontId);
        //}
        mFont = tl->font;

        //const std::string &fontName =  sourceSprite()->sourceImages().front()->fontName();
        //mFont = fontLibrary.getFont(fontName);


        //int colorId = sourceSprite()->sourceImages().front()->colorStyleId();
        //if(colorId>=0 && colorId<textColorsLibrary.size()){
        //    _color = textColorsLibrary[colorId];
        //}
        //mColor = textColorsLibrary.At(colorId);
        //mColor = sourceSprite()->sourceImages().front()->fontColorRGBA();
        mColor = tl->color->colorRGBA();

        //mText = tl->text;
        if(mText.empty()){
            mText = si->name();
        }


        //TextHandleVariant thv = sourceSprite()->sourceImages().front()->textHandleVariant();
        mRelativeHandle = GetRelativeHandleFromTextHandleVariant(tl->handle, settings.yOrientation());
    }


    if(mFont==nullptr){
        assert(fontsLibrary.fonts().size()>0);
        mFont = fontsLibrary.fonts().front();            // Make sure that you have added at least one font to fontLibrary !!!
    }

    if(mText.empty()){
        mText = "text";
    }




    //setFont(mFont);
    //setText(mText);
    //setRelativeHandle(mRelativeHandle);
    //setColor(mColor);
    updateAllNodeParameters();

    //--------------------------------------------------------




    //static_cast<TextSpriteNode*>(mNode)->setLayer(parentLayer()->zOrder());


    //float w = width();
    //float h = height();
    //SetHandle(Vec2f(GetRelativeHandle().x*w,  GetRelativeHandle().y*h));


    mInitialized = true;

    //---
    //setChangeFlags(Property::ALL);

}


void TextSprite::clearExternalDataLinks()
{

    // mFont uses global external data

}


void TextSprite::updateAllNodeParameters()
{

    Sprite::updateAllNodeParameters();

    setFont(mFont);
    setColor(mColor);
    setText(mText);
    setRelativeHandle(mRelativeHandle);

}


void TextSprite::setHandle(Vec2f _handle)
{

    mHandle = _handle;
    //static_cast<TextSpriteNode*>(mNode)->setAnchorPoint(mRelativeHandle.x, mRelativeHandle.y);
    //static_cast<TextSpriteNode*>(mNode)->setTransformationBitsDirty();      // needed in the editor


    float w = static_cast<TextSpriteNode*>(mNode)->width();
    float h = static_cast<TextSpriteNode*>(mNode)->height();

    w = w / mNode->scale().x;       // we must use unscaled size
    h = h / mNode->scale().y;

    ncine::Vector2f anchorPoint(mHandle.x - w * 0.5f, mHandle.y - h*0.5f);
    mNode->setAbsAnchorPoint(anchorPoint);

}


/*
void TextSprite::updateEngineObjects()
{

    return;
    if(changeFlags()==0) return;

    int flags = changeFlags();



    if(flags & Property::VISIBILITY){
        mNode->setDrawEnabled(isVisible());
    }

    if(flags & Property::TRANSFORMATION){

        Vec2f localPos = position();
        localPos += parentLayer()->GetParallaxOffset();
        Vec2f localScale = scale();
        float localRotation = rotation();
        Vec2i localFlip = flip();

        //if (flags & Property::HANDLE){
        //    static_cast<TextSpriteNode*>(mNode)->setAnchorPoint(relativeHandle().x, relativeHandle().y);
        //}

        if(dbgSprite && dbgChildSprite==nullptr){
            dbgChildSprite = this;      // next sprite of "segments. " - non drifting sprite
        }
        if(mText=="segments. "){            // drifting sprite
            if(dbgSprite==nullptr){
                dbgSprite = this;
            }
        }

        if(flags & Property::POSITION){
            mNode->setPosition(localPos.x, localPos.y);
        }
        if(flags & Property::SCALE){
            mNode->setScale((localFlip.x==0)? localScale.x : -localScale.x, (localFlip.y==0)? localScale.y : -localScale.y);
        }
        if(flags & Property::ROTATION){
            mNode->setRotation(localRotation);
        }
        if(flags & Property::FLIP){
            mNode->setScale((localFlip.x==0)? localScale.x : -localScale.x, (localFlip.y==0)? localScale.y : -localScale.y);
        }

        if(this==dbgSprite && dbgChildSprite){
            DummyFunction();
            DbgOutput(text()+ " anchor y:" + std::to_string(mNode->absAnchorPoint().y) + "  pos y:" + std::to_string(mNode->position().y)+ " abs y:" + std::to_string(mNode->absPosition().y));

        }
        if(this==dbgChildSprite){
            DbgOutput(text()+ " anchor y:" + std::to_string(mNode->absAnchorPoint().y) + "  pos y:" + std::to_string(mNode->position().y)+ " abs y:" + std::to_string(mNode->absPosition().y));
        }

    }


    if(flags & Property::APPEARANCE){

        //if(flags & Property::COLOR){
            //ncine::TextNode::setColor(GetColor().r, GetColor().g, GetColor().b, GetColor().a);
        //}
        if(flags & Property::ALPHA){
            mNode->setAlphaF(alpha());
        }
    }


    //---
    setChangeFlags(0);

}
*/


void TextSprite::onTransform()
{

    assert(isInitialized());

    //ncine::TextNode::transform();

    //---- update bounding box

#ifdef JUGI_EDITOR

    ncine::Matrix4x4f m = mNode->worldMatrix();
    float widthUnscaled = width() / mNode->scale().x;
    float heightUnscaled = height() / mNode->scale().y;
    //BoundingBoxFloat rect(-width()*0.5f, -height()*0.5, width()*0.5f, height()*0.5f);
    BoundingBoxFloat rect(-widthUnscaled*0.5f, -heightUnscaled*0.5, widthUnscaled*0.5f, heightUnscaled*0.5f);

    mBox[0] = jugimap::transform(Vec2f( rect.min.x, rect.min.y ), m);
    mBox[1] = jugimap::transform(Vec2f( rect.max.x, rect.min.y ), m);
    mBox[2] = jugimap::transform(Vec2f( rect.max.x, rect.max.y ), m);
    mBox[3] = jugimap::transform(Vec2f( rect.min.x, rect.max.y ), m);
    //----
    mWorldBoundingBox.setInitialSize();
    for(int i=0; i<mBox.size(); i++){
        if(mWorldBoundingBox.min.x > mBox[i].x) mWorldBoundingBox.min.x = mBox[i].x;
        if(mWorldBoundingBox.min.y > mBox[i].y) mWorldBoundingBox.min.y = mBox[i].y;
        if(mWorldBoundingBox.max.x < mBox[i].x) mWorldBoundingBox.max.x = mBox[i].x;
        if(mWorldBoundingBox.max.y < mBox[i].y) mWorldBoundingBox.max.y = mBox[i].y;
    }


#else

    mWorldBoundingBox = TransformRectf(BoundingBoxFloat(-width()*0.5f, -height()*0.5, width()*0.5f, height()*0.5f), mNode->worldMatrix());

#endif

    setParentBoundingBoxUpdateRequired(true);


}


bool TextSprite::pointInside(Vec2f _pos)
{

    //if(changeFlags() & Sprite::Property::TRANSFORMATION || changeFlags() & Sprite::Property::TEXT_STRING){
    //    updateEngineObjects();
    //}

    return boundingBox().Contains(_pos);
}


void TextSprite::setText(const std::string &_textString)
{

    mText = _textString;

    if(mText=="segments. "){
        DummyFunction();
    }

    if(mFont==nullptr) return;

    static_cast<TextSpriteNode*>(mNode)->setString(mText.c_str());
    float w = static_cast<TextSpriteNode*>(mNode)->width();
    float h = static_cast<TextSpriteNode*>(mNode)->height();

    w = w / mNode->scale().x;   // we must use unscaled size
    h = h / mNode->scale().y;

    //---  adjust relative handle to rounded value so that text get positioned on full pixel coordinates (to prevent drawing blury text)

    setRelativeHandle(mRelativeHandle);
    /*
    if(AreEqual(w, 0.0f)==false && AreEqual(h, 0.0f)==false){
        float hx = w * mRelativeHandle.x;
        if(hx>0.001){
            float hxRounded = std::roundf(hx);
            mRelativeHandle.x = hxRounded / w;
        }
        float hy = h * mRelativeHandle.y;
        if(hy>0.001){
            float hyRounded = std::roundf(hy);
            mRelativeHandle.y = hyRounded / h;
        }
    }
    */

    //--- fix for ncine reseting anchor point when assigning empty string
    if(mText.empty()==false && mEmptyString){
        static_cast<TextSpriteNode*>(mNode)->setAnchorPoint(relativeHandle().x, relativeHandle().y);
    }
    mEmptyString = mText.empty();

    //---- update handle
    //mHandle = {w*mRelativeHandle.x, h*mRelativeHandle.y};
    setHandle({w*mRelativeHandle.x, h*mRelativeHandle.y});
     //static_cast<TextSpriteNode*>(mNode)->setAnchorPoint(mRelativeHandle.x, mRelativeHandle.y);

    //---- update bounding box
    mWorldBoundingBox = TransformRectf(BoundingBoxFloat(-w*0.5f, -h*0.5, w*0.5f, h*0.5f), mNode->worldMatrix());
    setParentBoundingBoxUpdateRequired(true);

}


bool TextSprite::setTextFromBook(const std::string _bookName, int _bookSegmentId)
{

    mBookName = _bookName;
    mBookSegmentId = _bookSegmentId;

    TextSegment *ts = textLibrary.GetTextSegmentFromBook(mBookName, mBookSegmentId);
    if(ts==nullptr){
        return false;
    }

    setText(ts->plainText());

    return true;
}


void TextSprite::copyProperties(Sprite *_sprite, int copyFlags)
{

    Sprite::copyProperties(_sprite, copyFlags);

    TextSprite * spr = dynamic_cast<TextSprite*>(_sprite);
    if(spr==nullptr) return;


    if(copyFlags & Property::COLOR){
        mColor = spr->mColor;
    }

    if(copyFlags & Property::TEXT_STRING){
        mText = spr->mText;
        mBookName = spr->mBookName;
        mBookSegmentId = spr->mBookSegmentId;
    }

    //appendToChangeFlags(copyFlags);

}

/*
void TextSprite::setEngineNodesEnabled(bool _forceEnabled)
{

    if(_forceEnabled){
        node()->setEnabled(true);

    }else{
        node()->setEnabled(mVisible);
    }

}
*/

//==========================================================================================================


void SkeletonSprite::initEngineObjects()
{

    //sourceSprite()->init();


    SourceImage *gi = sourceSprite()->sourceImages().front();
    SourceFile *gf = gi->parentSourceFile();

    if(type()==SpriteType::SPINE){

        // spine atlas file: gf->GetSpineAtlasRelativeFilePath()
        // spine skeleton file: gf->GetRelativeFilePath()
        // spine skeleton skin name: gi->name()


    }else if(type()==SpriteType::SPRITER){

        // spriter model file: gf->GetRelativeFilePath()
        // spriter entity name: gi->name()

    }

}



ncine::TextNode * errorMessageLabel = nullptr;


}



