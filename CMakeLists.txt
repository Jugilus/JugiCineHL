set(NCPROJECT_NAME "JugiCineHL")
set(NCPROJECT_EXE_NAME "JugiCineHL")
set(NCPROJECT_VENDOR "Jugilus")
set(NCPROJECT_COPYRIGHT "Copyright ©2016-2022 ${NCPROJECT_VENDOR}")
set(NCPROJECT_DESCRIPTION "JugiCineHL")
set(NCPROJECT_HOMEPAGE "https://jugicine.github.io")
set(NCPROJECT_REVERSE_DNS "io.github.jugicine.playerTestbed")

set(NCPROJECT_INCLUDE_DIRS
3rdParty
3rdParty/box2d
jugimap
jugimap/map
jugimap/source
jugimap/animation
jugimap/gui
jugicineHL
jugicineHL/actions
jugicineHL/widgets
jugicineHL/items
jugicineHL/components
jugicineHL/gfxObjects
jugicineHL/systems
jugicineHL/movements
jugicineHL/ai
jugicineHL/scripting
jugicineHL/entitySystem
)

set(NCPROJECT_SOURCES
# 3rdParty
3rdParty/utf8cpp/utf8.h
3rdParty/utf8cpp/utf8/core.h
3rdParty/utf8cpp/utf8/cpp11.h
3rdParty/utf8cpp/utf8/checked.h
3rdParty/utf8cpp/utf8/unchecked.h
3rdParty/pugixml/pugiconfig.hpp
3rdParty/pugixml/pugixml.hpp

3rdParty/pugixml/pugixml.cpp

3rdParty/box2d/b2_api.h
3rdParty/box2d/b2_block_allocator.h
3rdParty/box2d/b2_body.h
3rdParty/box2d/b2_broad_phase.h
3rdParty/box2d/b2_chain_shape.h
3rdParty/box2d/b2_circle_shape.h
3rdParty/box2d/b2_collision.h
3rdParty/box2d/b2_common.h
3rdParty/box2d/b2_contact.h
3rdParty/box2d/b2_contact_manager.h
3rdParty/box2d/b2_distance.h
3rdParty/box2d/b2_distance_joint.h
3rdParty/box2d/b2_draw.h
3rdParty/box2d/b2_dynamic_tree.h
3rdParty/box2d/b2_edge_shape.h
3rdParty/box2d/b2_fixture.h
3rdParty/box2d/b2_friction_joint.h
3rdParty/box2d/b2_gear_joint.h
3rdParty/box2d/b2_growable_stack.h
3rdParty/box2d/b2_joint.h
3rdParty/box2d/b2_math.h
3rdParty/box2d/b2_motor_joint.h
3rdParty/box2d/b2_mouse_joint.h
3rdParty/box2d/b2_polygon_shape.h
3rdParty/box2d/b2_prismatic_joint.h
3rdParty/box2d/b2_pulley_joint.h
3rdParty/box2d/b2_revolute_joint.h
3rdParty/box2d/b2_rope.h
3rdParty/box2d/b2_settings.h
3rdParty/box2d/b2_shape.h
3rdParty/box2d/b2_stack_allocator.h
3rdParty/box2d/b2_time_of_impact.h
3rdParty/box2d/b2_timer.h
3rdParty/box2d/b2_time_step.h
3rdParty/box2d/b2_types.h
3rdParty/box2d/b2_weld_joint.h
3rdParty/box2d/b2_wheel_joint.h
3rdParty/box2d/b2_world.h
3rdParty/box2d/b2_world_callbacks.h
3rdParty/box2d/box2d.h

3rdParty/box2d/src/dynamics/b2_chain_circle_contact.h
3rdParty/box2d/src/dynamics/b2_chain_polygon_contact.h
3rdParty/box2d/src/dynamics/b2_circle_contact.h
3rdParty/box2d/src/dynamics/b2_contact_solver.h
3rdParty/box2d/src/dynamics/b2_edge_circle_contact.h
3rdParty/box2d/src/dynamics/b2_edge_polygon_contact.h
3rdParty/box2d/src/dynamics/b2_island.h
3rdParty/box2d/src/dynamics/b2_polygon_circle_contact.h
3rdParty/box2d/src/dynamics/b2_polygon_contact.h

3rdParty/box2d/src/collision/b2_broad_phase.cpp
3rdParty/box2d/src/collision/b2_chain_shape.cpp
3rdParty/box2d/src/collision/b2_circle_shape.cpp
3rdParty/box2d/src/collision/b2_collide_circle.cpp
3rdParty/box2d/src/collision/b2_collide_edge.cpp
3rdParty/box2d/src/collision/b2_collide_polygon.cpp
3rdParty/box2d/src/collision/b2_collision.cpp
3rdParty/box2d/src/collision/b2_distance.cpp
3rdParty/box2d/src/collision/b2_dynamic_tree.cpp
3rdParty/box2d/src/collision/b2_edge_shape.cpp
3rdParty/box2d/src/collision/b2_polygon_shape.cpp
3rdParty/box2d/src/collision/b2_time_of_impact.cpp
3rdParty/box2d/src/common/b2_block_allocator.cpp
3rdParty/box2d/src/common/b2_draw.cpp
3rdParty/box2d/src/common/b2_math.cpp
3rdParty/box2d/src/common/b2_settings.cpp
3rdParty/box2d/src/common/b2_stack_allocator.cpp
3rdParty/box2d/src/common/b2_timer.cpp
3rdParty/box2d/src/dynamics/b2_body.cpp
3rdParty/box2d/src/dynamics/b2_chain_circle_contact.cpp
3rdParty/box2d/src/dynamics/b2_chain_polygon_contact.cpp
3rdParty/box2d/src/dynamics/b2_circle_contact.cpp
3rdParty/box2d/src/dynamics/b2_contact.cpp
3rdParty/box2d/src/dynamics/b2_contact_manager.cpp
3rdParty/box2d/src/dynamics/b2_contact_solver.cpp
3rdParty/box2d/src/dynamics/b2_distance_joint.cpp
3rdParty/box2d/src/dynamics/b2_edge_circle_contact.cpp
3rdParty/box2d/src/dynamics/b2_edge_polygon_contact.cpp
3rdParty/box2d/src/dynamics/b2_fixture.cpp
3rdParty/box2d/src/dynamics/b2_friction_joint.cpp
3rdParty/box2d/src/dynamics/b2_gear_joint.cpp
3rdParty/box2d/src/dynamics/b2_island.cpp
3rdParty/box2d/src/dynamics/b2_joint.cpp
3rdParty/box2d/src/dynamics/b2_motor_joint.cpp
3rdParty/box2d/src/dynamics/b2_mouse_joint.cpp
3rdParty/box2d/src/dynamics/b2_polygon_circle_contact.cpp
3rdParty/box2d/src/dynamics/b2_polygon_contact.cpp
3rdParty/box2d/src/dynamics/b2_prismatic_joint.cpp
3rdParty/box2d/src/dynamics/b2_pulley_joint.cpp
3rdParty/box2d/src/dynamics/b2_revolute_joint.cpp
3rdParty/box2d/src/dynamics/b2_weld_joint.cpp
3rdParty/box2d/src/dynamics/b2_wheel_joint.cpp
3rdParty/box2d/src/dynamics/b2_world.cpp
3rdParty/box2d/src/dynamics/b2_world_callbacks.cpp
3rdParty/box2d/src/rope/b2_rope.cpp
		


# JUGIMAP
jugimap/map/jmLayerElement.h
jugimap/map/jmLayer.h
jugimap/map/jmSpriteLayer.h
jugimap/map/jmVectorLayer.h
jugimap/map/jmMap.h
jugimap/map/jmSprite.h
jugimap/map/jmStandardSprite.h
jugimap/map/jmTextSprite.h
jugimap/map/jmCompositeSprite.h
jugimap/map/jmSpriteModifiers.h
jugimap/map/jmVectorShape.h
jugimap/map/jmVectorShapeUtilities.h
jugimap/map/jmVectorShapeDrawing.h
jugimap/source/jmSourceContainers.h
jugimap/source/jmSourceSprite.h
jugimap/source/jmSourceFile.h
jugimap/source/jmSourceItem.h
jugimap/source/jmColor.h
jugimap/source/jmFont.h
jugimap/source/jmSystemSourceObjects.h
jugimap/source/jmSourceObjectsLoader_xml.h
jugimap/animation/jmAnimationCommon.h
jugimap/animation/jmTimelineAnimation.h
jugimap/animation/jmTimelineAnimationInstance.h
jugimap/animation/jmFrameAnimation.h
jugimap/gui/jmGuiCommon.h
jugimap/gui/jmGuiBar.h
jugimap/gui/jmGuiButton.h
jugimap/gui/jmGuiSlider.h
jugimap/gui/jmGuiTextField.h
jugimap/gui/jmGuiTextInput.h
jugimap/gui/jmGuiText.h
jugimap/jmApp.h
jugimap/jmScene.h
jugimap/jmSceneLayout.h
jugimap/jmCamera.h
jugimap/jmCollision.h
jugimap/jmCommon.h
jugimap/jmCommonFunctions.h
jugimap/jmSystem.h
jugimap/jmGlobal.h
jugimap/jmScene.h
jugimap/jmUtilities.h
jugimap/jmStreams.h
jugimap/jmInput.h
jugimap/jugimap.h
jugimap/jmMapLoader_bin.h
jugimap/jmParameter.h
jugimap/jmSaveSignatures.h
jugimap/jmTimer.h
jugimap/jmTween.h
jugimap/jmSignal.h
jugimap/jmUsingDeclarations.h

jugimap/map/jmLayerElement.cpp
jugimap/map/jmLayer.cpp
jugimap/map/jmSpriteLayer.cpp
jugimap/map/jmVectorLayer.cpp
jugimap/map/jmMap.cpp
jugimap/map/jmSprite.cpp
jugimap/map/jmStandardSprite.cpp
jugimap/map/jmTextSprite.cpp
jugimap/map/jmCompositeSprite.cpp
jugimap/map/jmSpriteModifiers.cpp
jugimap/map/jmVectorShape.cpp
jugimap/map/jmVectorShapeUtilities.cpp
jugimap/map/jmVectorShapeDrawing.cpp
jugimap/source/jmSourceContainers.cpp
jugimap/source/jmSourceSprite.cpp
jugimap/source/jmSourceFile.cpp
jugimap/source/jmColor.cpp
jugimap/source/jmFont.cpp
jugimap/source/jmSourceItem.cpp
jugimap/source/jmSystemSourceObjects.cpp
jugimap/source/jmSourceObjectsLoader_xml.cpp
jugimap/animation/jmAnimationCommon.cpp
jugimap/animation/jmTimelineAnimation.cpp
jugimap/animation/jmTimelineAnimationInstance.cpp
jugimap/animation/jmFrameAnimation.cpp
jugimap/gui/jmGuiCommon.cpp
jugimap/gui/jmGuiBar.cpp
jugimap/gui/jmGuiButton.cpp
jugimap/gui/jmGuiSlider.cpp
jugimap/gui/jmGuiTextField.cpp
jugimap/gui/jmGuiTextInput.cpp
jugimap/gui/jmGuiText.cpp
jugimap/jmApp.cpp
jugimap/jmScene.cpp
jugimap/jmSceneLayout.cpp
jugimap/jmCamera.cpp
jugimap/jmCollision.cpp
jugimap/jmCommon.cpp
jugimap/jmCommonFunctions.cpp
jugimap/jmGlobal.cpp
jugimap/jmSystem.cpp
jugimap/jmUtilities.cpp
jugimap/jmStreams.cpp
jugimap/jmInput.cpp
jugimap/jmMapLoader_bin.cpp
jugimap/jmParameter.cpp
jugimap/jmTimer.cpp
jugimap/jmTween.cpp
jugimap/jmSignal.cpp

# JUGICINE HL

jugicineHL/actions/jpActionsCommon.h
jugicineHL/actions/jpCommands_Logic.h
jugicineHL/actions/jpTransition.h
jugicineHL/actions/jpTransitionOperations.h
jugicineHL/actions/jpCommands_Gui.h
jugicineHL/actions/jpTriggers_Logic.h
jugicineHL/actions/jpTriggers_Gui.h
jugicineHL/actions/jpTriggers_Input.h
jugicineHL/actions/jpConditions.h
jugicineHL/widgets/jpItemsTable.h
jugicineHL/items/jpItemsCommon.h
jugicineHL/items/jpItemSlotData.h
jugicineHL/components/jpComponentsCommon.h
jugicineHL/components/jpUserSelector.h
jugicineHL/components/jpInputCustomizer.h
jugicineHL/components/jpNewUserPanel.h
jugicineHL/components/jpLanguageSelector.h
jugicineHL/components/jpConfirmPanel.h
jugicineHL/components/jpMessagePanel.h
jugicineHL/gfxObjects/jpGfxObjectsCommon.h
jugicineHL/gfxObjects/jpAnimationObjects.h
jugicineHL/entitySystem/jpEntity.h
jugicineHL/entitySystem/jpEntitySystem.h
jugicineHL/entitySystem/jpEntityCommon.h
jugicineHL/entitySystem/jpEntityGlobal.h
jugicineHL/entitySystem/jpEntityGroups.h
jugicineHL/entitySystem/jpEntityUtilities.h
jugicineHL/entitySystem/jpSourceEntity.h
jugicineHL/entitySystem/jpB2Callbacks.h
jugicineHL/entitySystem/jpB2Body.h
jugicineHL/entitySystem/jpB2Fixture.h
jugicineHL/entitySystem/jpB2World.h
jugicineHL/entitySystem/jpB2DebugDraw.h
jugicineHL/entitySystem/jpB2Utilities.h
jugicineHL/entitySystem/scripting/jpBehavior.h
jugicineHL/entitySystem/scripting/jpBehCfg.h
jugicineHL/entitySystem/scripting/jpBehConditions.h
jugicineHL/entitySystem/scripting/jpBehCommands.h
jugicineHL/entitySystem/movements/jpMovementBase.h
jugicineHL/entitySystem/movements/jpMovementCommon.h
jugicineHL/entitySystem/movements/jpSpeedGenerators.h
jugicineHL/entitySystem/movements/jpGroundMovement.h
jugicineHL/entitySystem/movements/jpJumpMovement.h
jugicineHL/entitySystem/movements/jpPathMovement.h
jugicineHL/entitySystem/movements/jpPathPtPMovement.h
jugicineHL/entitySystem/movements/jpAnimatedBehavior.h
jugicineHL/entitySystem/movements/jpAngularVelocity.h
jugicineHL/entitySystem/task/jpTaskBase.h
jugicineHL/entitySystem/task/jpTaskCommon.h
jugicineHL/entitySystem/task/jpTransporter.h
jugicineHL/systems/jpSettings.h
jugicineHL/systems/jpUser.h
jugicineHL/systems/jpInput.h
jugicineHL/systems/jpSystemVarious.h
jugicineHL/jpGlobal.h
jugicineHL/jpObjectFactory.h
jugicineHL/jpVariables.h
jugicineHL/jpGameObjectsLoader_xml.h
jugicineHL/jpUtilities.h
jugicineHL/jpQueries.h
jugicineHL/jpPlayedApp.h
jugicineHL/jpPlayedScene.h

jugicineHL/actions/jpActionsCommon.cpp
jugicineHL/actions/jpCommands_Logic.cpp
jugicineHL/actions/jpCommands_Gui.cpp
jugicineHL/actions/jpTransition.cpp
jugicineHL/actions/jpTransitionOperations.cpp
jugicineHL/actions/jpTriggers_Logic.cpp
jugicineHL/actions/jpTriggers_Gui.cpp
jugicineHL/actions/jpTriggers_Input.cpp
jugicineHL/actions/jpConditions.cpp
jugicineHL/widgets/jpItemsTable.cpp
jugicineHL/items/jpItemsCommon.cpp
jugicineHL/items/jpItemSlotData.cpp
jugicineHL/components/jpComponentsCommon.cpp
jugicineHL/components/jpUserSelector.cpp
jugicineHL/components/jpInputCustomizer.cpp
jugicineHL/components/jpNewUserPanel.cpp
jugicineHL/components/jpLanguageSelector.cpp
jugicineHL/components/jpConfirmPanel.cpp
jugicineHL/components/jpMessagePanel.cpp
jugicineHL/gfxObjects/jpGfxObjectsCommon.cpp
jugicineHL/gfxObjects/jpAnimationObjects.cpp
jugicineHL/entitySystem/jpEntity.cpp
jugicineHL/entitySystem/jpEntitySystem.cpp
jugicineHL/entitySystem/jpEntityCommon.cpp
jugicineHL/entitySystem/jpEntityGlobal.cpp
jugicineHL/entitySystem/jpEntityGroups.cpp
jugicineHL/entitySystem/jpEntityUtilities.cpp
jugicineHL/entitySystem/jpSourceEntity.cpp
jugicineHL/entitySystem/jpB2Callbacks.cpp
jugicineHL/entitySystem/jpB2Body.cpp
jugicineHL/entitySystem/jpB2Fixture.cpp
jugicineHL/entitySystem/jpB2World.cpp
jugicineHL/entitySystem/jpB2DebugDraw.cpp
jugicineHL/entitySystem/jpB2Utilities.cpp
jugicineHL/entitySystem/scripting/jpBehavior.cpp
jugicineHL/entitySystem/scripting/jpBehCfg.cpp
jugicineHL/entitySystem/scripting/jpBehConditions.cpp
jugicineHL/entitySystem/scripting/jpBehCommands.cpp
jugicineHL/entitySystem/movements/jpMovementBase.cpp
jugicineHL/entitySystem/movements/jpMovementCommon.cpp
jugicineHL/entitySystem/movements/jpSpeedGenerators.cpp
jugicineHL/entitySystem/movements/jpGroundMovement.cpp
jugicineHL/entitySystem/movements/jpJumpMovement.cpp
jugicineHL/entitySystem/movements/jpPathMovement.cpp
jugicineHL/entitySystem/movements/jpPathPtPMovement.cpp
jugicineHL/entitySystem/movements/jpAnimatedBehavior.cpp
jugicineHL/entitySystem/movements/jpAngularVelocity.cpp
jugicineHL/entitySystem/task/jpTaskBase.cpp
jugicineHL/entitySystem/task/jpTaskCommon.cpp
jugicineHL/entitySystem/task/jpTransporter.cpp
jugicineHL/systems/jpSettings.cpp
jugicineHL/systems/jpUser.cpp
jugicineHL/systems/jpInput.cpp
jugicineHL/systems/jpSystemVarious.cpp
jugicineHL/jpGlobal.cpp
jugicineHL/jpObjectFactory.cpp
jugicineHL/jpVariables.cpp
jugicineHL/jpGameObjectsLoader_xml.cpp
jugicineHL/jpUtilities.cpp
jugicineHL/jpQueries.cpp
jugicineHL/jpPlayedScene.cpp
jugicineHL/jpPlayedApp.cpp

# PLAYER
player/jpPlayer.h
player/main.h

player/jpPlayer.cpp
player/main.cpp
)



set(hardcodedPaths OFF CACHE BOOL "Hard-coded paths")


if(${hardcodedPaths} STREQUAL "ON")

    cmake_minimum_required(VERSION 3.13)
    project(${NCPROJECT_NAME})

    add_executable(${NCPROJECT_EXE_NAME} WIN32 ${NCPROJECT_SOURCES})

    target_compile_definitions(${NCPROJECT_EXE_NAME} PUBLIC HARDCODED_PROJECT_PATH)


    list(APPEND NCPROJECT_INCLUDE_DIRS
        c:/Development/nCine/nCine/include
        c:/Development/nCine/nCine-external/include/GLFW
        c:/Development/nCine/nCine-build/generated/include
        c:/Development/nCine/nCine-build/imgui-src/include_only
        )

    target_include_directories(${NCPROJECT_EXE_NAME} PRIVATE ${NCPROJECT_INCLUDE_DIRS})

    #if($<CONFIG:Debug>)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_link_directories(${NCPROJECT_EXE_NAME} PUBLIC "c:/Development/nCine/nCine-build/Debug")
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine_main_d)
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine_d)
        message("DEBUG linking")

    else()
        target_link_directories(${NCPROJECT_EXE_NAME} PUBLIC "c:/Development/nCine/nCine-build/Release")
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine_main)
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine)
        message("RELEASE linking")

    endif()


else()


    # Don't edit beyond this line
    if(CMAKE_SYSTEM_NAME STREQUAL "Android")
            return()
    endif()

    cmake_minimum_required(VERSION 3.13)
    project(${NCPROJECT_NAME})

    find_path(NCPROJECT_DIR cmake/project_main.cmake
            PATHS ${nCine_DIR} $ENV{NCINE_DIR}
            PATH_SUFFIXES project ../project ../../../project ../../../share/ncine/project
            NO_CMAKE_FIND_ROOT_PATH # For Emscripten
            DOC "Path to the nCine template project scripts")

    if(IS_DIRECTORY ${NCPROJECT_DIR})
            include(${NCPROJECT_DIR}/cmake/project_main.cmake)
    else()
            if(NOT DEFINED nCine_DIR AND "$ENV{NCINE_DIR}" STREQUAL "")
                    message(FATAL_ERROR "Set the nCine_DIR CMake variable or the NCINE_DIR environment variable to a valid nCine directory.")
            elseif(DEFINED nCine_DIR)
                    message(FATAL_ERROR "The nCine_DIR CMake variable \"${nCine_DIR}\" is not a valid nCine directory.")
            elseif(NOT "$ENV{NCINE_DIR}" STREQUAL "")
                    message(FATAL_ERROR "The NCINE_DIR environment variable \"$ENV{NCINE_DIR}\" is not a valid nCine directory.")
            endif()
    endif()

endif()
