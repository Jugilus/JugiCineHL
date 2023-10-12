#ifndef JMSHAPELAYER_H
#define JMSHAPELAYER_H

#include "jmLayer.h"




namespace jugimap {


class VectorLayersGroup : public LayerElement
{
public:


    VectorLayersGroup();
    VectorLayersGroup(const std::string& _nameID);


#ifndef JUGI_EDITOR
    void OnVectorLayersGroupConstruct();
    void rebuildQuickAccessContainers() override;

#else
    void OnVectorLayersGroupConstruct();
    VectorLayersGroup(const VectorLayersGroup& _SrcShapesGroup);
    ~VectorLayersGroup(){}

    void SetLayersMapPointers();

    void Draw(jugi::base::TGraphics& Graphics, int flags=0) override;
    void MoveContent(int dxt, int dyt) override;
    void rebuildQuickAccessContainers() override;
    //void UpdateBoundingBox(int flags=0) override;

#endif

};







///\ingroup MapElements
///\brief The VectorLayer class defines the vector layer from JugiMap Editor.
///
/// A vector layer stores VectorShape objects and is their owner.
class VectorLayer : public Layer
{
public:
    friend class VectorLayerNode;
    friend class MapLoader_bin;
    friend class MapWriter_bin;
    friend class CompositeSprite;
    friend class Map;



    virtual void updatePropertiesViaExtraParameters() override;
    virtual void updateCameraDependentObjects() override;
    virtual void updateBoundingBox(int flags=0) override;


    /// \brief Returns a reference to the vector of stored shapes in this vector layer.
    std::vector<VectorShape*>& shapes() {return mShapes;}

    /// \brief Adds a new vector shape to this vector layer.
    ///
    /// **Important:** This vector layer will take over the ownership of the added vector shape.
    void AddVectorShape(VectorShape* _vs){mShapes.push_back(_vs);}



    /// Constructor
    VectorLayer(Map *_map);
    VectorLayer(const std::string & _name, Map *_map);
    ~VectorLayer();

    void deleteContent() override;
    void deleteVectorShapes();

    void changeEllipsesToBezierCurves(int _dataFlag);


#ifndef JUGI_EDITOR

    void OnVectorLayerConstruct();

    /// Destructor.
    //virtual ~VectorLayer() override;

    void AddLoadedShape(VectorShape* _shape);

#else

    friend class jugi::TShapeEngine;
    friend class jugi::TSpriteSetupEngine_ImagesCollisionShapes;
    friend class jugi::TSpriteSetupEngine_ImagesHandlePoint;


    void OnVectorLayerConstruct();
    //EVectorLayer(const std::string _name);
    VectorLayer(const VectorLayer &_SrcLayer);


    void StoreShapesForUndo();
    void DeleteShapesForUndo();

    void MoveContent(int dxt, int dyt) override;
    //void UpdateBoundingBox(int flags = 0);
    void Draw(jugi::base::TGraphics& Graphics, int flags=0) override;
    void DrawBoundingBox(jugi::base::TGraphics& Graphics) override;


    //void updatePropertiesViaExtraParameters() override;

    void AddLoadedShape(VectorShape* _shape);


    jugimap::BoundingBoxInt mTilesBoundingBox;
    //jugimap::BoundingBoxInt mBoundingBox;
    std::list<VectorShape*> *mStoredShapesForUndo = nullptr;   // pomozna lista za undo

#endif


protected:

    std::vector<VectorShape*>mShapes;             //OWNED

private:



};





}




#endif // JMSHAPELAYER_H
