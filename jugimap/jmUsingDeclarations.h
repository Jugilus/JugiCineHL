#ifndef JMUSINGDECLARATIONS_H
#define JMUSINGDECLARATIONS_H

#include <vector>
#include <string>

//#ifdef JUGI_EDITOR

//#include "../../jugi/eFramework/eVectorShape.h"

//#else

//#include "jmVectorShape.h"

//#endif



#ifdef JUGI_EDITOR

namespace jugi{

class TShapeEngine;
class TSpriteSetupEngine_MultiFrameSprites;
class TSpriteSetupEngine_ImagesCollisionShapes;
class TSpriteSetupEngine_ImagesHandlePoint;
class TComposedSpritesManager;
class TSpriteSheetSubimagesAppEngine;

bool Load_LegacyLSDfile(const std::string &);

struct TTiledImporter;
struct TTiledExporter;


namespace gui {

class TAnimationKeysWidget;
class TimelineAnimationEditorGUI;
class TimelineAnimationEditor_AnimationWidgets;
class TimelineAnimationEditor_AniTrackWidgets;
class TimelineAnimationEditor_KeyWidgets;
class TimelineAnimationEditor;

class FrameAnimationEditor;
class FrameAnimationEditorGUI;
class FrameAnimationEditor_AnimationWidgets;

class TAnimationFramesWidget;
struct TSourceObjectFrameAnimationsStored;
struct TSourceObjectMultikeyAnimationsStored;

class TDockContent_TextLabel;

class TEditFontDialog;
class TFontsLibraryDialog;
class TTextsLibraryDialog;

class TEditColorDialog;
class ColorsLibraryDialog;

class TAdjustTilesetDialog;
class TSpriteSheetSubimagesDialog;


class TPropertiesDock;
class TSourceSpritesLoad;
struct TSourceImageFileLoader;

class TTilesetDisplayWidget;

class TSceneLayoutDialog;

class TApplicationDialog;

class TNewMapDialog;

}
}

namespace oldStuff{

struct TObjectData;
void _MakeSets_StandardSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);
void _MakeSets_ComposedSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &ObjectsList, const std::string &gfxIngameDir);
void MakeGfxSets(oldStuff::TObjectData *);

}


#endif



namespace jugimap{


class MapLoader_bin;
class SourceObjectsLoader_xml;
class AppConfigurationLoader_xml;
class GameObjectsBuilder;
class VectorShape;

//class VectorShape;
class SourceSet;
class SourceFile;
class SourceSprite;
class FrameAnimation;
class TimelineAnimation;
class StandardSprite;
class CompositeSprite;
class SkeletonSprite;
//class SourceGraphics;
class SpriteLayer;
class VectorLayer;
class MapLoader_bin;
class SourceImage;

class FrameAnimation;
class TimelineAnimation;
class CompositeSprite;

class DummySourceObjects;
class SystemSourceObjects;


#ifdef JUGI_EDITOR

class MapWriter_bin;
class SourceObjectsWriter_xml;
class GameObjectsWriter_xml;
class ProjectMaintenance;
void MakeSourceSetImagesLayer(int);
SpriteLayer *MakeSourceSpriteLayer(SourceSprite *_SourceObject);

#endif



/*
#ifdef JUGI_EDITOR

using VectorShape = TEShape;
using RectangleShape = TERectangleShape;
using PolyLineShape = TEPolylineShape;
using BezierShape = TEBezierShape;
using EllipseShape = TEEllipseShape;
using SinglePointShape = TESinglePointShape;

#else

using VectorShape = FrmVectorShape;
using RectangleShape = FrmRectangleShape;
using PolyLineShape = FrmPolyLineShape;
using BezierShape = FrmBezierShape;
using EllipseShape = FrmEllipseShape;
using SinglePointShape = FrmSinglePointShape;

#endif

*/
}



#endif // JMUSINGDECLARATIONS_H
