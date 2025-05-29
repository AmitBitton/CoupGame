# Coup C++ Game

## Overview
This project implements the board game **Coup** in C++, combining core game logic with a **graphical user interface (GUI)** using the **SFML** library.
The game supports **2 to 6 players**, each randomly assigned a **role** (Governor, Spy, Baron, General, Judge, or Merchant), with unique special abilities.
Each player takes turns performing actions such as: `gather`, `tax`, `bribe`, `arrest`, `sanction` and `coup`.
Certain actions (like tax, bribe, and coup) can be blocked by specific roles.
The game includes logic for handling these blocks by passing temporary turns to relevant players (Detailed explanation below).
The last active player remaining wins the game.

### This project includes:
- Full implementation of game logic (`Game` and `Player` classes).
- Specialized role classes implementing unique abilities and block mechanics.
- A graphical interface using **SFML** for interactive gameplay.
- A **console demo** in `main.cpp` that simulates a full game run.
- A complete **test suite** using the **Doctest** framework to validate logic and edge cases.


---

##  Project Structure
**Game.h, Game.cpp**  
Defines and implements the core game logic, managing players, turns, actions, and game flow.

**Player.h, Player.cpp**  
Base class for all players, handling common attributes and actions.

**Governor.h/cpp, Spy.h/cpp, Baron.h/cpp, General.h/cpp, Judge.h/cpp, Merchant.h/cpp**  
Role-specific classes, each implementing unique abilities for their respective character in the game.

**GUI/GameDisplay.h, GameDisplay.cpp**  
Main GUI display – renders player info, action buttons, and background using SFML.

**GUI/GuiButton.h, GuiButton.cpp**  
Modular SFML button implementation used across all screens.

**GUI/Screens.h, Screens.cpp**  
Handles different GUI screens: start screen, add players, assign roles, and the main game loop.

**GUI/GuiMain.cpp**  
Entry point for running the GUI version of the game.

**main.cpp**  
Simulation of a full game, demonstrating core mechanics without a graphical interface.

**Test.cpp**  
Contains unit tests for game mechanics and edge cases, implemented using the `doctest` framework.

**doctest.h**  
The testing framework header used in `Test.cpp`.

**Demo.cpp**  
Optional console-based demonstration - We were provided with this file in an assignment.

**Makefile**  
Build automation with targets for compiling, running, testing, valgrind checks, and cleaning up.

**Font/OpenSans-Regular.ttf**  
Font used in SFML to render readable text in the GUI.

**Background/game_background.png, menu_background.png, win_background.png**  
SFML background images for gameplay, menu, and win screen respectively.

### Core Game Logic

#### `Game`
- Manages the overall flow of the game: player turns, actions, and special phases like blocking.
- Keeps track of players, their status, and the enforcement of game rules.
- 

#### `Player`
- Abstract base class representing any player.
- Contains shared logic for actions such as `gather`, `tax`, `bribe`, `arrest`, `coup`, and `sanction`.
- Tracks coin count, status (active/inactive), last action, and extra turns.

---

##  Role Classes

#### `Governor`
- Inherits from `Player`.
- Can perform `tax` to gain 3 coins.
- Can `undo` another player's `tax` action.

#### `Spy`
- Inherits from `Player`.
- Can block a player's ability to arrest during their next turn, without spending coins or consuming a turn.
- Can view the coin count of another player on his turn.
- Automatically clears the block after the affected player's next turn.

#### `Baron`
- Inherits from `Player`.
- Can "invest" 3 coins and receive 6 coins in return.
- When sanctioned, automatically gains 1 coin as compensation.

#### `General`
- Inherits from `Player`.
- Uses `undo` to block a coup and return the targeted player to the game (cost 5 coins).
- Gains coins when being arrested.

#### `Judge`
- Inherits from `Player`.
- Can `undo` a bribe action, causing the target player to lose the 4 coins paid.
- When `sanctioned`, the attacking player must pay an additional coin.

#### `Merchant`
- Inherits from `Player`.
- Receives a **bonus coin** at the start of a turn if they have 3 or more coins.
- Pays 2 coins when arrested (instead of giving a coin to the attacker).


### Turn Passing During Special Blocking Phases

Some roles, such as `Governor`, `Judge`, and `General`, have the ability to **block** specific actions after they are initiated by another player.
When such an action is performed (`tax`, `bribe`, or `coup`), and relevant blocking roles are present in the game, 
the turn is **temporarily passed** to those players, allowing them to decide whether to block the action or skip.

- After a `tax` action, the turn is passed to all active `Governor`s in order, allowing them to block tax or skip.
- After a `bribe` action, the turn is passed to all active `Judge`s in order, allowing them to block bribe or skip .
- After a `coup` action, the turn is passed to all active `General`s in order, allowing them to block the coup or skip.

Once all relevant blocking opportunities are exhausted or a block is applied, the game returns the turn to the appropriate player and resumes the standard turn order.

This mechanism ensures that reactions to powerful actions are handled immediately, and blocking decisions are made before the action fully resolves.

---

## GUI (SFML-Based)

### `GuiMain.cpp`
- Initializes and manages the main GUI flow by loading all game screens in sequence.


### `GameDisplay.{h,cpp}`
- Renders the main game screen: buttons, player info, background.
- Updates dynamically according to the current game state.

### `GuiButton.{h,cpp}`
- Reusable GUI button class for SFML.
- Supports text, position, color, and action binding.

### `Screens.{h,cpp}`
- Manages different screens (start menu, add players, assign roles, main game loop).
- Handles transitions and user input.

---

##  Other Files

### `main.cpp`
- Simulates a full game round with six player roles.
- Runs predefined actions and prints the game state after each move.

### `Test.cpp`
- Includes a comprehensive test suite built using the doctest framework.
- The tests check that all game mechanics work correctly, including handling both regular and edge cases.

---

## Assets

### `Background/`
Contains background images used in the game screens:
- `menu_background.png` – background for the **main menu** screen.
- `game_background.png` – background for the **main game** screen.
- `win_background.png` – background for the **winner** screen.

These images are loaded dynamically using **SFML**, and their presence is required for the game to render correctly.

### `Font/`
Contains the font used by the GUI:
- `OpenSans-Regular.ttf` .

---

## Running the Project

###  Compile and Run the GUI Version
This compiles and runs the **SFML GUI** version of the game.
```
make run
```

#### GUI Flow (Step-by-Step)
1. **Opening Screen**  
   Launch the game using the command:
```
make run
```
The game will open on the main screen.  
Click **"Start New Game"** to begin.

2. **Add Players Screen**  
   Type the name of each player (between **2 to 6 players**) in the input field.  
   After entering a name, click **"Add Player"** to add them.  
   Once all players are added, click **"Start Game"** to move to the next phase.

3. **Assign Roles Screen**  
   Each player will receive a role randomly.  
   Players must click **"Take Card"** one after another to draw their role.  
   The screen automatically updates to the next player until all have drawn.

4. **Main Game Loop Screen**  
   Players take turns performing actions using the GUI buttons:
- **Gather**, **Tax**, **Bribe**, **Arrest**, **Sanction**, **Coup**  
  If the action is blockable (e.g., `Tax`, `Bribe`, `Coup`), the game automatically passes the turn temporarily to relevant roles (e.g., `Governor`, `Judge`, `General`) who can choose to block or skip.
  special buttons for each role will appear in the player's corresponding turn in the game.
- 
- 5. **End Game**  
   When only one player remains active, the game ends and a **winner screen** is shown to declare the winner.

---
### Run the Unit Tests

This compiles and runs all unit tests using the Doctest framework:
```
make test
```
---
### Compile and Run the Main Demo
This compiles and runs the demo from `main.cpp`
```
make Main

./Main
```
---
### Run Memory Check with Valgrind
Check for memory leaks in both the demo and unit tests:
```
make valgrind
```
---
### Clean Build Files
Remove all compiled files
```
make clean
```
---

##  Notes

- Supports **2 to 6 players**, each with randomly assigned roles.
- Players can have **duplicate roles** – multiple players may receive the same role.
- The game logic throws meaningful **exceptions** when illegal actions are attempted.
- Includes a **modular GUI** built with SFML, separated into clear screens.
- Fonts and backgrounds are loaded from the `Font/` and `Background/` directories.
- Includes a **Makefile** with commands for compilation, testing, and memory checks.
- If a player starts their turn with **10 or more coins**, they are **must to perform a `coup`**.

---

# Author
+ Written by: Amit Bitton
+ Mail: amiteste.bitton@msmail.ariel.ac.il
+ Systems programming 2 , EX3, May 2025