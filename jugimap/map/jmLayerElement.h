#ifndef JMLAYERNODEITEM_H
#define JMLAYERNODEITEM_H

#ifdef JUGI_EDITOR

#include "jugi/items/itemsBase.h"
#endif


#include <ncine/SceneNode.h>
#include "jmGlobal.h"
#include "jmParameter.h"



namespace jugimap {



class LayerElement;
class CompositeSprite;
class Scene;
class App;



class LayerNode : public ncine::SceneNode
{
public:


    LayerNode(LayerElement *_baseLayer);
    virtual ~LayerNode() override;

    nctl::Array<SceneNode *> &getChildren(){ return children_; }
    void transform() override;

    //void resetLastFrameUpdated(){ lastFrameUpdated_ = 0;}
    unsigned long int getLastFrameUpdated(){ return lastFrameUpdated_;}
    void setLastFrameUpdated(unsigned long int _lastFrameUpdated){ lastFrameUpdated_ = _lastFrameUpdated;}
    void resetLastFrameUpdated(){ lastFrameUpdated_ = 0;}




    LayerElement * layerElement = nullptr;               // LINK
    CompositeSprite * compositeSprite = nullptr;          // LINK
    Scene *scene = nullptr;                                // LINK
    App *app = nullptr;                                     // LINK


//#ifdef JUGI_EDITOR
    void SyncChildrenWithLayerElement();
//#endif



};



#ifdef JUGI_EDITOR
class LayerElement : public jugi::TItem
#else
class LayerElement
#endif

{
public:
    friend class LayerNode;
    friend class SourceObjectsLoader_xml;


    void TLayerElementInit();
    LayerElement();
    LayerElement(const std::string& _name);
    LayerElement(const LayerElement& _LayerElement);
    virtual ~LayerElement();


    /// Update the axis aligned bounding box of this layer element.
    virtual void updateBoundingBox(int flags=0){}

    virtual void updateGeometry(int flags=0);

    virtual void rebuildNodes();
    virtual void rebuildQuickAccessContainers(){}

    /// Returns the kind of this layer.
    LayerKind kind() {return mKind;}

    const std::string & name(){ return mName; }

    void addChildLayer(LayerElement* bl, int pos=-1);

    void removeChildLayer(LayerElement* bl);

    virtual void deleteContent();


    void _setName(const std::string &_name);

    LayerNode* node(){ return mNode; }

    nctl::Array<ncine::SceneNode *> &nodeChildren(){ return mNode->getChildren(); }

    CParameters& parameters(){ return mParameters; }

    bool isHidden(){ return hidden; }
    bool isVisible(){ return !hidden; }
    void setVisible(bool _visible){ hidden=!_visible; mNode->setDrawEnabled(!hidden); }

    virtual void setEngineNodesEnabled(bool _forceEnabled){}


    CParameters mParameters;
    bool hidden = false;
    bool locked = false;
    bool edited = false;

protected:
    LayerKind mKind = LayerKind::NOT_DEFINED;
    std::string mName;
    LayerNode *mNode = nullptr;                  // LINK to node in nodes structure


#ifndef JUGI_EDITOR

public:
    std::vector<LayerElement*>& children(){ return mChildren; }
    LayerElement *parent(){ return mParent; }

protected:
    std::vector<LayerElement*>mChildren;        // owned
    LayerElement *mParent = nullptr;            // LINK

#else

public:
    static bool changeNameOnCopy;
    static int layerIDcounter;

    bool IsParentHidden();
    virtual void MoveContent(int dx, int dy){}
    virtual void Draw(jugi::base::TGraphics&, int flags=0){}
    virtual void DrawBoundingBox(jugi::base::TGraphics& Graphics){}
    virtual void updatePropertiesViaExtraParameters(){}

    int layerID = 0;

#endif



};


void DbgPrintNodesTree(LayerElement* le, std::string _spacing="");






}










#endif // JMLAYERNODEITEM_H
