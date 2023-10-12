#ifdef JUGI_EDITOR
#include <QString>
#endif

#include "jmCommonFunctions.h"
#include "jmLayerElement.h"
#include "jmCompositeSprite.h"



namespace jugimap {





LayerNode::LayerNode(LayerElement *_baseLayer): layerElement(_baseLayer)
{
    type_ = ncine::Object::ObjectType::SCENENODE;
    setDeleteChildrenOnDestruction(false);
}


LayerNode::~LayerNode()
{
    DummyFunction();
}


void LayerNode::transform()
{
    SceneNode::transform();
}




void LayerNode::SyncChildrenWithLayerElement()
{

     children_.clear();

#ifdef JUGI_EDITOR

     if(layerElement->ChildItems && layerElement->ChildItems->empty()==false){
         for(jugi::TItem* i : *layerElement->ChildItems){
             LayerElement* le = static_cast<LayerElement*>(i);
             le->mNode->parent_ = this;
             children_.pushBack(le->mNode);
         }
     }

#else

     for(LayerElement* le : layerElement->children()){
         le->node()->parent_ = this;
         children_.pushBack(le->mNode);
     }

#endif

}






void LayerElement::TLayerElementInit()
{
#ifdef JUGI_EDITOR
    layerID = layerIDcounter++;
#endif
}


#ifndef JUGI_EDITOR
LayerElement::LayerElement()
#else
LayerElement::LayerElement() : TItem()
#endif
{
    TLayerElementInit();
}

//TLayerElement::TLayerElement(TLayerElement* _ParentItem) : TItem()
//{
//    ParentItem = _ParentItem;
//    TLayerElementInit();
//}


#ifndef JUGI_EDITOR
LayerElement::LayerElement(const std::string &_name)
#else
LayerElement::LayerElement(const std::string &_name) : TItem(QString::fromStdString(_name))
#endif
{
    TLayerElementInit();
    mName = _name;
}

#ifndef JUGI_EDITOR
LayerElement::LayerElement(const LayerElement &_src)
#else
LayerElement::LayerElement(const LayerElement& _src) : TItem(_src)
#endif
{
    TLayerElementInit();

    mKind = _src.mKind;
    mName = _src.mName;
    hidden = _src.hidden;
    locked = _src.locked;
    mParameters = _src.mParameters;

    // mNode is created by extended classes

#ifndef JUGI_EDITOR
    mParent = _src.mParent;
#else
    ParentItem = _src.ParentItem;
    if(changeNameOnCopy){
        if(nameID.endsWith("cut")){
            nameID.replace(nameID.size()-3,3,"copy");
        }else if(nameID.endsWith("copy")==false){
            nameID = QString("%1 copy").arg(nameID);
        }
    }
#endif

}




LayerElement::~LayerElement()
{
#ifndef JUGI_EDITOR
    for(LayerElement *le : mChildren){
        delete le;
    }
    mChildren.clear();

#else
    // children deleted by TItem
    if(dbgPrintOnDelete) qDebug()<<"~LayerElement() name:"<< QString::fromStdString(mName) << "  nameID:"<< nameID;

#endif


    delete mNode;
}


void LayerElement::_setName(const std::string &_name)
{
    mName = _name;

#ifdef JUGI_EDITOR
    nameID = QString::fromStdString(mName);
#endif

}


void LayerElement::addChildLayer(LayerElement *bl, int pos)
{

#ifdef JUGI_EDITOR
    AddChildItem(bl, pos);
#else
    if(pos<0 || pos>=mChildren.size()){
        mChildren.push_back(bl);
    }else{
        mChildren.insert(mChildren.begin()+pos, bl);
    }
    bl->mParent = this;
#endif

    rebuildNodes();
}


void LayerElement::removeChildLayer(LayerElement* bl)
{

    bool removed = false;

#ifdef JUGI_EDITOR
    assert(ChildItems);
    removed = ChildItems->removeOne(bl);  assert(removed);
#else
    removed = RemoveElementFromVector(mChildren, bl);  assert(removed);
#endif

    //
    rebuildNodes();
}


void LayerElement::deleteContent()
{

#ifdef JUGI_EDITOR
    if(ChildItems){
        DeleteChildItems();
    }

#else
    for(LayerElement *le : mChildren){
        delete le;
    }
    mChildren.clear();
#endif

    //
    rebuildNodes();
}



void LayerElement::rebuildNodes()
{
    mNode->SyncChildrenWithLayerElement();
    rebuildQuickAccessContainers();
}


void LayerElement::updateGeometry(int flags)
{

    unsigned long int lastFrameUpdated = mNode->getLastFrameUpdated();
    mNode->update(1);
    updateBoundingBox(flags);
    mNode->setLastFrameUpdated(lastFrameUpdated);
}



void DbgPrintNodesTree(LayerElement* le, std::string _spacing)
{

    std::string text = _spacing + le->name();
    print(text);
    _spacing += "   ";

    for(ncine::SceneNode *n : le->nodeChildren()){
        LayerNode* ln = dynamic_cast<LayerNode*>(n);
        if(ln){
            DbgPrintNodesTree(ln->layerElement, _spacing);
        }
    }

}




}
