CONFIG -= qt

TARGET = player

#DEFINES -= UNICODE

CONFIG(release, debug|release) {
    #release build
    DEFINES += NDEBUG

} else {
    #debug build
    DEFINES += _DEBUG
}


win32 {

    DEFINES += _WINDOWS

    LIBS += -lopengl32
    LIBS += -luser32
    LIBS += -lkernel32
    LIBS += -lgdi32
    LIBS += -lwinspool
    LIBS += -lcomdlg32
    LIBS += -ladvapi32
    LIBS += -lshell32
    LIBS += -lole32
    LIBS += -loleaut32
    LIBS += -luuid
    LIBS += -lodbc32
    LIBS += -lodbccp32


    CONFIG(release, debug|release) {

        #release build

        LIBS += "c:\Development\nCine\nCine-build\Release\ncine_main.lib"
        LIBS += "c:\Development\nCine\nCine-build\Release\ncine.lib"
        #LIBS += "c:\Development\box2d\build\bin\Release\box2d.lib"

        #INCLUDEPATH += "c:\Development\nCine\ncline\nCine-2019.11.r286-0058f02-Win64-VS2017_fix\lib"
        #DEPENDPATH += "c:/Development/cocos2d/build/Release.win32"

        #QMAKE_CXXFLAGS_RELEASE -= -MD
        #QMAKE_CXXFLAGS_RELEASE += -MT


        #QMAKE_CXXFLAGS_RELEASE -= -MT
        #QMAKE_CXXFLAGS_RELEASE += -MD

    } else {

        #debug build
        LIBS += "c:\Development\nCine\nCine-build\Debug\ncine_main_d.lib"
        LIBS += "c:\Development\nCine\nCine-build\Debug\ncine_d.lib"
        #LIBS += -L$$PWD/box2d/../../box2d/build/bin/debug/ -lbox2d
        #LIBS += "c:/Development/box2d/build/bin/Debug/box2d.lib"
        #DEPENDPATH += c:/Development/box2d/build/bin/Debug/box2d.lib
        #LIBS += -L$$PWD/../../../box2d/build/bin/debug/ -lbox2d

    }

    #QMAKE_CXXFLAGS -= -MDd
    #QMAKE_CXXFLAGS += -MD
}

INCLUDEPATH += "c:\Development\nCine\nCine\include"
INCLUDEPATH += "c:\Development\nCine\nCine-external\include\GLFW"
INCLUDEPATH += "c:\Development\nCine\nCine-build\generated\include"
INCLUDEPATH += "c:\Development\nCine\nCine-build\imgui-src\include_only"

#INCLUDEPATH += "c:\Development\box2d\include"


#INCLUDEPATH += "c:/Development/nCine/nCine-2020.05-Win64-VS2017/include"



#-----------------------------------------------------------------------------------------

FRAMEWORK_PATH = c:/Development/JugiMapProject/JugiCine

INCLUDEPATH += $$FRAMEWORK_PATH
INCLUDEPATH += $$FRAMEWORK_PATH/jugimap
INCLUDEPATH += $$FRAMEWORK_PATH/jugimap/map
INCLUDEPATH += $$FRAMEWORK_PATH/jugimap/source
INCLUDEPATH += $$FRAMEWORK_PATH/jugimap/animation
INCLUDEPATH += $$FRAMEWORK_PATH/jugimap/gui
INCLUDEPATH += $$FRAMEWORK_PATH/3rdParty
INCLUDEPATH += "$$FRAMEWORK_PATH/3rdParty/box2d"
#INCLUDEPATH += "$$FRAMEWORK_PATH/3rdParty/box2d/src"

INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/actions
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/widgets
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/items
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/components
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/gfxObjects
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/systems
INCLUDEPATH += $$FRAMEWORK_PATH/jugicineHL/entitySystem


# BOX2D
#-----------------------------------------------------------------------------------------

HEADERS += \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_api.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_block_allocator.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_body.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_broad_phase.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_chain_shape.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_circle_shape.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_collision.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_common.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_contact_manager.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_distance.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_distance_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_draw.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_dynamic_tree.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_edge_shape.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_fixture.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_friction_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_gear_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_growable_stack.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_math.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_motor_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_mouse_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_polygon_shape.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_prismatic_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_pulley_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_revolute_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_rope.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_settings.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_shape.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_stack_allocator.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_time_of_impact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_timer.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_time_step.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_types.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_weld_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_wheel_joint.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_world.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/b2_world_callbacks.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/box2d.h


HEADERS += \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_chain_circle_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_chain_polygon_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_circle_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_contact_solver.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_edge_circle_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_edge_polygon_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_island.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_polygon_circle_contact.h \
    $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_polygon_contact.h

SOURCES += \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_broad_phase.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_chain_shape.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_circle_shape.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_collide_circle.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_collide_edge.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_collide_polygon.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_collision.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_distance.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_dynamic_tree.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_edge_shape.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_polygon_shape.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/collision/b2_time_of_impact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/common/b2_block_allocator.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/common/b2_draw.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/common/b2_math.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/common/b2_settings.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/common/b2_stack_allocator.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/common/b2_timer.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_body.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_chain_circle_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_chain_polygon_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_circle_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_contact_manager.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_contact_solver.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_distance_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_edge_circle_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_edge_polygon_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_fixture.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_friction_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_gear_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_island.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_motor_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_mouse_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_polygon_circle_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_polygon_contact.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_prismatic_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_pulley_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_revolute_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_weld_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_wheel_joint.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_world.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/dynamics/b2_world_callbacks.cpp \
        $$FRAMEWORK_PATH/3rdParty/box2d/src/rope/b2_rope.cpp


# JUGICINE
#-----------------------------------------------------------------------------------------


HEADERS += \
    $$FRAMEWORK_PATH/3rdParty/utf8cpp/utf8.h \
    $$FRAMEWORK_PATH/3rdParty/utf8cpp/utf8/core.h \
    $$FRAMEWORK_PATH/3rdParty/utf8cpp/utf8/cpp11.h \
    $$FRAMEWORK_PATH/3rdParty/utf8cpp/utf8/checked.h \
    $$FRAMEWORK_PATH/3rdParty/utf8cpp/utf8/unchecked.h \
    $$FRAMEWORK_PATH/3rdParty/pugixml/pugiconfig.hpp \
    $$FRAMEWORK_PATH/3rdParty/pugixml/pugixml.hpp \
    $$FRAMEWORK_PATH/jugimap/map/jmLayerElement.h \
    $$FRAMEWORK_PATH/jugimap/map/jmLayer.h \
    $$FRAMEWORK_PATH/jugimap/map/jmSpriteLayer.h \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorLayer.h \
    $$FRAMEWORK_PATH/jugimap/map/jmMap.h \
    $$FRAMEWORK_PATH/jugimap/map/jmSprite.h \
    $$FRAMEWORK_PATH/jugimap/map/jmStandardSprite.h \
    $$FRAMEWORK_PATH/jugimap/map/jmTextSprite.h \
    $$FRAMEWORK_PATH/jugimap/map/jmCompositeSprite.h \
    $$FRAMEWORK_PATH/jugimap/map/jmSpriteModifiers.h \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorShape.h \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorShapeUtilities.h \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorShapeDrawing.h \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceContainers.h \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceSprite.h \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceFile.h \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceItem.h \
    $$FRAMEWORK_PATH/jugimap/source/jmColor.h \
    $$FRAMEWORK_PATH/jugimap/source/jmFont.h \
    $$FRAMEWORK_PATH/jugimap/source/jmSystemSourceObjects.h \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceObjectsLoader_xml.h \
    $$FRAMEWORK_PATH/jugimap/animation/jmAnimationCommon.h \
    $$FRAMEWORK_PATH/jugimap/animation/jmTimelineAnimation.h \
    $$FRAMEWORK_PATH/jugimap/animation/jmTimelineAnimationInstance.h \
    $$FRAMEWORK_PATH/jugimap/animation/jmFrameAnimation.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiCommon.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiBar.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiButton.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiSlider.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiTextField.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiTextInput.h \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiText.h \
    $$FRAMEWORK_PATH/jugimap/jmApp.h \
    $$FRAMEWORK_PATH/jugimap/jmScene.h \
    $$FRAMEWORK_PATH/jugimap/jmSceneLayout.h \
    $$FRAMEWORK_PATH/jugimap/jmCamera.h \
    $$FRAMEWORK_PATH/jugimap/jmCollision.h \
    $$FRAMEWORK_PATH/jugimap/jmCommon.h \
    $$FRAMEWORK_PATH/jugimap/jmCommonFunctions.h \
    $$FRAMEWORK_PATH/jugimap/jmSystem.h \
    $$FRAMEWORK_PATH/jugimap/jmGlobal.h \
    $$FRAMEWORK_PATH/jugimap/jmScene.h \
    $$FRAMEWORK_PATH/jugimap/jmUtilities.h \
    $$FRAMEWORK_PATH/jugimap/jmStreams.h \
    $$FRAMEWORK_PATH/jugimap/jmSignal.h \
    $$FRAMEWORK_PATH/jugimap/jmInput.h \
    $$FRAMEWORK_PATH/jugimap/jugimap.h \
    $$FRAMEWORK_PATH/jugimap/jmMapLoader_bin.h \
    $$FRAMEWORK_PATH/jugimap/jmParameter.h \
    $$FRAMEWORK_PATH/jugimap/jmSaveSignatures.h \
    $$FRAMEWORK_PATH/jugimap/jmTimer.h \
    $$FRAMEWORK_PATH/jugimap/jmTween.h \
    $$FRAMEWORK_PATH/jugimap/jmUsingDeclarations.h


SOURCES += \
    $$FRAMEWORK_PATH/3rdParty/pugixml/pugixml.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmLayerElement.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmLayer.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmSpriteLayer.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorLayer.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmMap.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmSprite.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmStandardSprite.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmTextSprite.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmCompositeSprite.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmSpriteModifiers.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorShape.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorShapeUtilities.cpp \
    $$FRAMEWORK_PATH/jugimap/map/jmVectorShapeDrawing.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceContainers.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceSprite.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceFile.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmColor.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmFont.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceItem.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmSystemSourceObjects.cpp \
    $$FRAMEWORK_PATH/jugimap/source/jmSourceObjectsLoader_xml.cpp \
    $$FRAMEWORK_PATH/jugimap/animation/jmAnimationCommon.cpp \
    $$FRAMEWORK_PATH/jugimap/animation/jmTimelineAnimation.cpp \
    $$FRAMEWORK_PATH/jugimap/animation/jmTimelineAnimationInstance.cpp \
    $$FRAMEWORK_PATH/jugimap/animation/jmFrameAnimation.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiCommon.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiBar.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiButton.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiSlider.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiTextField.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiTextInput.cpp \
    $$FRAMEWORK_PATH/jugimap/gui/jmGuiText.cpp \
    $$FRAMEWORK_PATH/jugimap/jmApp.cpp\
    $$FRAMEWORK_PATH/jugimap/jmScene.cpp\
    $$FRAMEWORK_PATH/jugimap/jmSceneLayout.cpp\
    $$FRAMEWORK_PATH/jugimap/jmCamera.cpp \
    $$FRAMEWORK_PATH/jugimap/jmCollision.cpp \
    $$FRAMEWORK_PATH/jugimap/jmCommon.cpp \
    $$FRAMEWORK_PATH/jugimap/jmCommonFunctions.cpp \
    $$FRAMEWORK_PATH/jugimap/jmGlobal.cpp \
    $$FRAMEWORK_PATH/jugimap/jmSystem.cpp \
    $$FRAMEWORK_PATH/jugimap/jmUtilities.cpp \
    $$FRAMEWORK_PATH/jugimap/jmStreams.cpp \
    $$FRAMEWORK_PATH/jugimap/jmSignal.cpp \
    $$FRAMEWORK_PATH/jugimap/jmInput.cpp \
    $$FRAMEWORK_PATH/jugimap/jmMapLoader_bin.cpp \
    $$FRAMEWORK_PATH/jugimap/jmParameter.cpp \
    $$FRAMEWORK_PATH/jugimap/jmTimer.cpp \
    $$FRAMEWORK_PATH/jugimap/jmTween.cpp


# JUGICINE HL
#-----------------------------------------------------------------------------------------

HEADERS += \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpActionsCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpCommands_Logic.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTransition.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTransitionOperations.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpCommands_Gui.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTriggers_Logic.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTriggers_Gui.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTriggers_Input.h \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpConditions.h \
    $$FRAMEWORK_PATH/jugicineHL/widgets/jpItemsTable.h \
    $$FRAMEWORK_PATH/jugicineHL/items/jpItemsCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/items/jpItemSlotData.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpComponentsCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpUserSelector.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpInputCustomizer.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpNewUserPanel.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpLanguageSelector.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpConfirmPanel.h \
    $$FRAMEWORK_PATH/jugicineHL/components/jpMessagePanel.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntity.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityGroups.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntitySystem.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityGlobal.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityUtilities.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpSourceEntity.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Callbacks.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Body.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Fixture.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2World.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2DebugDraw.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Utilities.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehavior.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehCfg.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehConditions.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehCommands.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpMovementBase.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpMovementCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpSpeedGenerators.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpGroundMovement.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpJumpMovement.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpPathMovement.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpPathPtPMovement.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpAnimatedBehavior.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpAngularVelocity.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/task/jpTaskBase.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/task/jpTaskCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/task/jpTransporter.h \
    $$FRAMEWORK_PATH/jugicineHL/gfxObjects/jpGfxObjectsCommon.h \
    $$FRAMEWORK_PATH/jugicineHL/gfxObjects/jpAnimationObjects.h \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpSettings.h \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpUser.h \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpInput.h \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpSystemVarious.h \
    $$FRAMEWORK_PATH/jugicineHL/jpGlobal.h \
    $$FRAMEWORK_PATH/jugicineHL/jpObjectFactory.h \
    $$FRAMEWORK_PATH/jugicineHL/jpVariables.h \
    $$FRAMEWORK_PATH/jugicineHL/jpQueries.h \
    $$FRAMEWORK_PATH/jugicineHL/jpGameObjectsLoader_xml.h \
    $$FRAMEWORK_PATH/jugicineHL/jpUtilities.h \
    $$FRAMEWORK_PATH/jugicineHL/jpPlayedApp.h \
    $$FRAMEWORK_PATH/jugicineHL/jpPlayedScene.h


SOURCES += \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpActionsCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpCommands_Logic.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpCommands_Gui.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTransition.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTransitionOperations.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTriggers_Logic.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTriggers_Gui.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpTriggers_Input.cpp \
    $$FRAMEWORK_PATH/jugicineHL/actions/jpConditions.cpp \
    $$FRAMEWORK_PATH/jugicineHL/widgets/jpItemsTable.cpp \
    $$FRAMEWORK_PATH/jugicineHL/items/jpItemsCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/items/jpItemSlotData.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpComponentsCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpUserSelector.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpInputCustomizer.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpNewUserPanel.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpLanguageSelector.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpConfirmPanel.cpp \
    $$FRAMEWORK_PATH/jugicineHL/components/jpMessagePanel.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntity.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityGroups.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntitySystem.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityGlobal.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpEntityUtilities.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpSourceEntity.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Callbacks.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Body.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Fixture.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2World.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2DebugDraw.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/jpB2Utilities.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehavior.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehCfg.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehConditions.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/scripting/jpBehCommands.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpMovementBase.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpMovementCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpSpeedGenerators.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpGroundMovement.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpJumpMovement.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpPathMovement.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpPathPtPMovement.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpAnimatedBehavior.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/movements/jpAngularVelocity.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/task/jpTaskBase.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/task/jpTaskCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/entitySystem/task/jpTransporter.cpp \
    $$FRAMEWORK_PATH/jugicineHL/gfxObjects/jpGfxObjectsCommon.cpp \
    $$FRAMEWORK_PATH/jugicineHL/gfxObjects/jpAnimationObjects.cpp \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpSettings.cpp \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpUser.cpp \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpInput.cpp \
    $$FRAMEWORK_PATH/jugicineHL/systems/jpSystemVarious.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpGlobal.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpObjectFactory.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpVariables.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpQueries.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpGameObjectsLoader_xml.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpUtilities.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpPlayedScene.cpp \
    $$FRAMEWORK_PATH/jugicineHL/jpPlayedApp.cpp


# PLAYER
#-----------------------------------------------------------------------------------------

HEADERS += \
    jpPlayer.h \
    main.h

SOURCES += \
    jpPlayer.cpp \
    main.cpp
	


