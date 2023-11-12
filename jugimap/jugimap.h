#ifndef JUGIMAP_API_H
#define JUGIMAP_API_H


//#include "jmMapBinaryLoader.h"
//#include "jmObjectFactory.h"
#include "jmCommon.h"
#include "jmCommonFunctions.h"
#include "jmParameter.h"
#include "jmTween.h"
#include "jmTimer.h"
#include "jmUsingDeclarations.h"
#include "jmSaveSignatures.h"
#include "jmApp.h"
#include "jmScene.h"
#include "jmGlobal.h"
#include "jmSystem.h"
#include "map/jmCompositeSprite.h"
#include "map/jmLayer.h"
#include "map/jmLayerElement.h"
#include "map/jmMap.h"
#include "map/jmSprite.h"
#include "map/jmSpriteLayer.h"
#include "map/jmSpriteModifiers.h"
#include "map/jmStandardSprite.h"
#include "map/jmTextSprite.h"
#include "map/jmVectorLayer.h"
#include "map/jmVectorShape.h"
#include "map/jmVectorShapeDrawing.h"
#include "map/jmVectorShapeUtilities.h"
#include "source/jmSourceContainers.h"
#include "source/jmSourceFile.h"
#include "source/jmSourceItem.h"
#include "source/jmSourceSprite.h"
#include "source/jmColor.h"
#include "source/jmFont.h"
#include "source/jmSystemSourceObjects.h"
#include "source/jmSourceObjectsLoader_xml.h"
#include "animation/jmAnimationCommon.h"
#include "animation/jmFrameAnimation.h"
#include "animation/jmTimelineAnimation.h"
#include "animation/jmTimelineAnimationInstance.h"
//#include "gui/jmGuiText.h"
//#include "gui/jmGuiTextInput.h"
//#include "gui/jmGuiCommon.h"
//#include "gui/jmGuiBar.h"
#include "jmCamera.h"
#include "jmCollision.h"
#include "jmUtilities.h"
#include "jmStreams.h"
#include "jmInput.h"
#include "jmSignal.h"
#include "jmMapLoader_bin.h"


namespace jugimap
{









// DoxyGen

/// \defgroup MapLoaders

/// \defgroup MapElements

/// \defgroup Cameras

/// \defgroup Animation

/// \defgroup Globals

/// \defgroup SearchFunctions

/// \defgroup Common

/// \defgroup CommonFunctions

/// \defgroup Streams

/// \defgroup Gui

/// \defgroup Input

/// \defgroup EngineExtension_Cocos2d-x
///
/// ### Cocos2d-x nodes ownership
/// Jugimap extended classes which wrap Cocos2d-x node objects does **not** take ownership over them.
/// When deleting a jugimap scene first delete its Cocos2d scene node via
/// Cocos2d director. For deleting jugimap sprites or text objects use dedicated layer functions which also
/// remove related Cocos2d-x nodes.
///

/// \defgroup EngineExtension_SFML

/// \defgroup EngineExtension_AGK-tier2

/// \defgroup EngineExtension_nCine
///
/// ### nCine nodes ownership
/// Jugimap extended classes which wrap nCIne node objects does **not** take ownership over them. The exception are Scene objects.
/// For deleting jugimap sprites or text objects use dedicated layer functions which also
/// delete related nCine nodes.
///








}



#endif
