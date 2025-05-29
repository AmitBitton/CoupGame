# Coup C++ Game

This project is an implementation of the **Coup** game in C++, with added **role-based logic** and a **graphical interface using SFML**.

---

##  Project Structure

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
- When `sanctioned`, the attacking player must pay an additional coin

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
- simulation of a full game round with six player roles.
- Runs predefined actions and prints the game state after each move.

### `Test.cpp`
- Includes a comprehensive test suite built using the doctest framework.
- The tests check that all game mechanics work correctly, including handling both regular and edge cases.

---

## Running the Project

###  Compile and Run the GUI Version
This compiles and runs the **SFML GUI** version of the game:

`make run`
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
- 
---

# Author
+ Written by: Amit Bitton
+ Mail: amiteste.bitton@msmail.ariel.ac.il
+ Systems programming 2 , EX3, May 2025