# raidnite

Collection of gameplay engine libraries for a hobby project.

I was finding that my prototypes were taking a long time to test in Unreal due to the nature of the coupling to the game itself. It was easy to get "lost in the sauce" so to speak, so I decided to completely decouple gameplay from the Unreal Engine.

My gameplay engine powers all the game logic, with the Unreal Engine handling input, audio, graphics. Essentially, the unreal 'actors' are simply a visual representation of the gameplay and blindly do what they are told.

This project extracts the gameplay code into an standalone, testable library. It contains four projects: engine, testing, thirdparty and sandbox.

### Engine

Lower level systems to support the game. This includes low-level utilies and generic gameplay systems that the game project is expected to leverage and often override.

#### Feature Set

- [x] Logging
- [x] Asserts
- [x] Delegates
- [x] Filesystem Utility
- [x] Intrusive Lists and Pools
- [x] Time Utility
- [x] Entities and Components
- [x] JSON Parsing
- [x] Localization
- [x] Service Locator for generic subsystems
- [x] Game Events
- [x] Combat Log + Serialization
- [x] Attributes (Strength, Armor, etc).
- [x] Powers (Mana, Energy, etc.)
- [x] Modifiers for Powers and Attributes (Buffs, Debuffs)
- [x] Damage Events and Damage/Resist Calculations
- [x] Aggro Table
- [x] Command Line Arguments (Bools, Ints, Strings)
- [x] 2D Tile Map
- [x] Pathfinding
- [x] Party / Groups
- [ ] State Machine
- [ ] States
- [ ] Abilities
- [ ] Talents (Spell Modifiers)
- [ ] Factions
- [ ] Unit AI
- [ ] ... and more.


### Sandbox

IMGUI testing framework. This project contains real-time tools to test gameplay with a basic debug UI.

### Testing

GoogleTest unit testing framework. This project enables test-driven development for features in the engine project.

### ThirdParty

Support project containing MIT licensed third-party frameworks.
