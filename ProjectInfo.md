### Project

I am developing a 2D game development tool currently called *JugiCine HL framework*. It is a continuation of the *JugiMap framework* which was created as a game framework for game maps created in *JugiMap editor*. The framework is not a standalone tool but uses *nCine* as a game engine. 

The word HL is an acronym for ‘high level’, which indicates the new direction of the project. The goal is to provide functionalities which would greatly simplify and speedup the development of games. 

Below are described the key elements of the framework.

### Game systems

Game systems are comprehensive solutions for concrete game tasks and behaviors.   While hard coded these systems are configurable so that the users can adjust their behavior. 

The framework includes various common systems like *GUI system*, *Input system*, *Users system* etc. In addition to that there is also a major dedicated system for development of 2d arcade games. This system is called *Entity system* and consists of various sub-systems and components for managing entities (game world objects). For example the movement of entities is handled by sub-systems called *movement engines*. Different movement engines are provided for different kinds of movements (ground movement, jumping, pathMovement, …).

Most of the game systems are modules which means that they are self-contained and independent from other game systems. *Entity system* is a module and the *movement engines* are submodules. 

Modules can be easily added or removed from the framework. They allow developers to customize or expand the framework features without affecting other parts of the framework.

Here is one example of how the developers can take advantage of the modular game systems. Let's say you need ground movement where the ground slope affects the speed (Sonic The Hedgehog style). The default *groundMovement engine*, although highly customizable, does not provide this option (it would just get too bloated). But the developers can code a new movement engine. In practice that would involve duplicating the default *groundMovement* submodule, renaming some classes and changing some code. They can then use the new and the original engine or remove the original from compilation.

### Core

The core part of the framework consists of various objects for various purposes. Below are mentioned some of them.

Objects which make modular code possible are *signals*. A game system uses input signals for the system controls and output signals for giving information about the system.

An essential part of the framework are *logic controllers* which regulate the logic flow of the running game. There are several types of logic controllers: *logic states*, *actions*, *conditions* and *commands*.

*Logic state* is a container for running game systems. It can have assigned actions and child logic states. A structure of parent - children logic states function as a state machine - one child state is active and the logic flow ‘passes’ through these active logic states.

The *actions* are used for managing game systems and logic flow. An action can have assigned *conditions* and *commands*. The commands get executed if conditions are true.

Logic controllers are (obviously) configurable and these configurations serve essentially as the logic scripts of the framework.

### Configuration data

Configuration data (parameters) for game systems and other components of the framework are written in XML format. Every configured object has its XML node and a defined structure of subnodes and attributes for storing its configuration data. The editing process mostly involves copy-pasting these blocks of XML nodes and filing (or changing) the attributes.

The configuration data is stored in configuration files but a lot of freedom is left to the users how to organize them. You can put all configuration data of a scene into one file or you may put them in more files for easier management. The configuration data parser reads all xml files within a dedicated directory (and subdirectories) and deduces the type of data from the node names.
 
Another important aspect of configuration data is error handling. A good XML editor is recommended to catch errors in XML syntax. That is just the first step. The application setup process includes extensive checking of configuration data. If there is an error in the data you will get an error message together with the calling setup stack to see where the error occurred. Any unrecognized XML node or attribute gives an error message. Any error in the value or the XML attribute (unknown constant, unrecognized referenced object, error in the value syntax..)  will also give an error message.

It is possible I will add a configuration data editor to JugMap Editor in future but it is not a pressing issue. I think that manual editing works well and is quite quick. 

 
### Scripting

Scripting is probably a necessary part of any ‘serious’ game engine. There may be applications able to produce very simple games (or games of a similar style) without scripting but anything more complex requires dealing with game logic. 

The scripting in the framework is minimalistic and part of configuration data. As mentioned before, the objects which manage the logic flow are logic controllers and their configuration data work as scripts.

Below are some examples of the script snippets from the platformer demo tests.

#### Example 1

```xml
<update>
    <state name="sceneLogic" systems="entitySystem, guiSystem" />
</update>
```

This represents the main update loop of the scene. It is very simple as there is only one state with no child states or actions. This state just runs Entity system and GUI system (the table and the slots for the items in the top bar are GUI widgets).

#### Example 2
 ```xml
<behaviorController>
    <state name="groundMovement" movementEngine="heroRunning">
        <a onSignals="INPUT:COMMAND/jump" setState="jumping" />
        <a onSignals="ENTITY:THIS/BLOCKED_DIRECTION:DOWN/NOT_ACTIVE" setState="jumping" />
    </state>
    <state name="jumping" movementEngine="heroJumping">
        <a onSignals="ENTITY:THIS/ENGINE:heroJumping:STATE=LANDED" setState="groundMovement" />
    </state>
</behaviorController>
```

The behavior controller is a controller (technically just a root state) which can be assigned to an entity. This simplified example manages walking and jumping of the controlled character. Walking and running are two states, each with an assigned appropriate movement engine. Both states have actions with conditions and commands for changing the state. At this point I will not describe conditions or commands (but you can probably guess what is going here).

#### Example 3
 ```xml
<a onSignals="ENTITY:THIS/CONTACT:CAT:pickableItem/START"  >
    <a onSignals="ENTITY:CONTACTED:iSmallCoin/CONTACT:ANY/START" setItem="ENTITY:THIS/coins ADD ENTITY:CONTACTED:iSmallCoin" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iSmallCoin" />
    <a onSignals="ENTITY:CONTACTED:iStarA/CONTACT:ANY/START" setData="ITEM/ENTITY:THIS/score ADD ITEM/ENTITY:CONTACTED:iStarA/score" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iStarA" />
    <a onSignals="ENTITY:CONTACTED:iGreenKey/CONTACT:ANY/START" setItem="ENTITY:THIS/itemsInventory ADD ENTITY:CONTACTED:iGreenKey" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iGreenKey" />
    <a onSignals="ENTITY:CONTACTED:iShurikenPack/CONTACT:ANY/START" setItem="ENTITY:THIS/itemsInventory ADD ENTITY:CONTACTED:iShurikenPack" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iShurikenPack" />
    <a onSignals="ENTITY:CONTACTED:iBoomerang/CONTACT:ANY/START" setItem="ENTITY:THIS/itemsInventory ADD ENTITY:CONTACTED:iBoomerang" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iBoomerang" />
    <a onSignals="ENTITY:CONTACTED:iAmigaBall/CONTACT:ANY/START" setItem="ENTITY:THIS/itemsInventory ADD ENTITY:CONTACTED:iAmigaBall" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iAmigaBall" />
    <a onSignals="ENTITY:CONTACTED:iHealthBottle/CONTACT:ANY/START" setItem="ENTITY:THIS/itemsInventory ADD ENTITY:CONTACTED:iHealthBottle" setSignal="ENTITY_MANAGER/REMOVE = ENTITY:CONTACTED:iHealthBottle" />
</a>
```


 ```xml
<a onSignals="ENTITY:THIS/CUSTOM_ACT/START = throwingItem" >
    <a onItem="ENTITY:THIS/itemsInventory SELECTED gamepad" setItem="ENTITY:THIS/itemsInventory REMOVE gamepad" setSignal="ENTITY:THIS/SPAWN = SOURCE_ENTITY:iGamepad" />
    <a onItem="ENTITY:THIS/itemsInventory SELECTED shurikenProxy" setItem="ENTITY:THIS/itemsInventory REMOVE shurikenOne" setSignal="ENTITY:THIS/SPAWN = SOURCE_ENTITY:shurikenUsed" />
    <a onItem="ENTITY:THIS/itemsInventory SELECTED boomerang" setSignal="ENTITY:THIS/SPAWN = SOURCE_ENTITY:wBoomerang" />
</a>   
```

These two wide blocks of actions manage picking and throwing of the items. As you can see the actions can also have child actions. 

Currently there are conditions *onSignal*, *onItem*, *onData* and commands *setSignal*, *setItem*, *setData* and *setState* which do 99% of the job. The goal of the framework scripting is to have as few conditions and commands as possible.

One may ask how the developer knows what signals, items and data are available for the conditions and commands mentioned above. The signals are used for accessing game systems so every game system contains all required signals for its operation. These signals will be described in documentation (preferably with some practical examples). Items and data are the custom data of the game and are specified in other configuration data by the developer.

### The Player

The player is an application which runs the framework game application. Two player versions are planned, one for the development and one for the released game. Currently the player is very basic, more features will be added later.


### Performance

At this point I am focusing on functionality and I haven’t done any benchmarks yet. The configurable nature of game systems does bring some overhead but in general I am not particularly concerned about performance.
If there is one system I do consider potentially problematic performance wise it is the animation system which is coded in a typical OOP fashion (virtual functions, a lot of redirections). I do have some ideas how to improve it if that will be needed in future. 

### Other

The main goal of the project is to make a game development tool which would allow quick development of fairly complex games (I am talking here only about programming, the game design and assets are another story). This development system should be simple enough that even users who are not programmers could learn in a week or two how to use it. Programmers on the other hand would be also able to customize the game systems if they need different functionality.

The fact that Entity system is a module shows what level of customization is possible. The name ‘Entity system’ may not be the best. Something like ‘Arcade game system’ would be probably better in order to describe the kind of games it could be used for. You could then have different systems (or add-on systems) for different types of games ( point and click adventures, dungeon crawlers, 2d racers, card games, match 3 games, visual novels…).

High level game systems do have their issues. They obviously can not provide every feature or behavior one can envision for their game. Customization is also not trivial per se and would, in general, require experienced programmers with a good understanding of the framework internals.


