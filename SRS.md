# Software Requirements Specification (SRS)
**Project:** Inventory Development Project
**Version:** 2.0
**Prepared by:** Krystian Klepadło
**Organization:** Academic Project
**Date:** May 1, 2026

## REVISION HISTORY

| Name | Date | Reason For Changes | Version |
| :--- | :--- | :--- | :--- |
| Krystian Klepadło | April 26, 2026 | Initial creation of the SRS document. | 1.0 |
| Krystian Klepadło | May 1, 2026 | Expanded use cases, added comprehensive UI image placeholders, detailed control schemes, map legends, and data persistence file structures. | 2.0 |

---

## 1. INTRODUCTION

### 1.1 Purpose
This document specifies the software requirements for the "Inventory Development Project," a 2D terminal-based RPG game. It covers the software's core gameplay engine, inventory management, and data persistence systems.

### 1.2 Document Conventions
Every requirement statement in this document is assigned a priority (High, Medium, or Low). Priorities for higher-level requirements are assumed to be inherited by detailed requirements.

### 1.3 Intended Audience and Reading Suggestions
This SRS is intended for project managers, developers, and testers. Readers should begin with the Product Scope and proceed through the System Features.

### 1.4 Product Scope
The software is a standalone, terminal-based RPG designed for entertainment. Its purpose is to provide a procedural world, a deep item-management system, and a scaling combat engine. Goals include strategic gameplay and player progression through ASCII visuals.

### 1.5 References
None.

---

## 2. OVERALL DESCRIPTION

### 2.1 Product Perspective
This product is a new, self-contained console application. It does not belong to a larger system. It interacts with the local operating system's terminal for display and the local file system for data storage.

### 2.2 Product Functions
The application provides a comprehensive RPG experience natively in the terminal. The core functions include:
* **World Exploration:** Navigating a 15x15 coordinate grid with visual terrain representation.
* **Tactical Combat:** A turn-based battle system against scaling enemies and Bosses.
* **Advanced Inventory Management:** A dynamic 2D array system where players can equip gear, consume potions, sort items, drop, or sell items for gold.
* **The Anvil (Item Upgrading):** An interactive menu allowing players to merge two identical items.
* **Store / Economy:** A shop system accessible in town to purchase randomly generated equipment.
* **Data Persistence:** Saving the active game state and cross-session meta-progression variables.

### 2.3 User Classes and Characteristics
* **Regular Player:** Casual users interacting via standard keyboard inputs. 
* **Testers/Admins:** Advanced users verifying game logic via save file modification.

### 2.4 Operating Environment
The game operates in a standard terminal environment (Windows CMD, Linux, macOS). For Windows, the application requires the MinGW runtime libraries to be present.

### 2.5 Design and Implementation Constraints
The system is built using C++ (11/17 standards) and is restricted to character-based ASCII graphics. Memory is managed manually for inventory pointers.

### 2.6 User Documentation
A tutorial video explaining how to use the software and navigate the menus will be delivered alongside the executable.

### 2.7 Assumptions and Dependencies
It is assumed the user has a terminal window set to at least 80x24 characters and that the OS supports standard ASCII rendering.

---

## 3. EXTERNAL INTERFACE REQUIREMENTS

### 3.1 User Interfaces, Controls, and Visuals
The interface is entirely console-based. The terminal window is continuously cleared and redrawn to simulate animation and state changes.

#### 3.1.1 The World Map Interface
The main screen consists of a top status bar (showing Player Level, HP, Gold, Fame, and EXP) and the 15x15 map grid below it.
**Map Legend (Symbols):**
* `@` : The Player Character.
* `T` : The Town (Safe zone, contains interactive features like Shop and Anvil).
* `^` : Mountains / Trees (Passable visual decorations that do not block movement).
* `"` : Grass / Plains (Passable visual decorations that do not block movement).

<img width="686" height="546" alt="town" src="https://github.com/user-attachments/assets/cea86c67-aaf9-4c21-8112-e8ab4a06b12d" />

#### 3.1.2 The Inventory Interface
When opened (`I`), a 5x5 grid replaces the map. A dynamic hover-bar appears below the grid, updating item statistics in real-time based on the cursor's position.

<img width="636" height="322" alt="image" src="https://github.com/user-attachments/assets/5ae6e47e-aa14-4dde-a45b-9e4b2ebc3964" />

When the player presses `Z` inside the inventory, the **Sorting Menu** appears, offering options to organize items by value, name, or compactly.

<img width="363" height="33" alt="image" src="https://github.com/user-attachments/assets/a0d5ad85-05dc-47dd-a91a-a6c2cc272e56" />

#### 3.1.3 The Combat Interface
Triggered randomly during movement. Replaces the map with a battle log, showing the Player's HP versus the Enemy's HP and offering combat commands (`A` Attack, `S` Special, `B` Block, `R` Run).

<img width="375" height="176" alt="image" src="https://github.com/user-attachments/assets/31e88a04-974f-4be7-82e9-b6b9d8b3cad9" />

#### 3.1.4 The Anvil Interface
Accessed in the Town. The screen prompts the player to enter the Row and Column of two identical items they wish to merge.

<img width="620" height="193" alt="image" src="https://github.com/user-attachments/assets/bb74d897-8f03-4d4f-8608-3d178680018d" />


#### 3.1.5 The Store Interface
Accessed in the Town. Displays a generated list of items and potions with their respective Gold prices. 

<img width="358" height="214" alt="image" src="https://github.com/user-attachments/assets/aef06d1b-7896-4c58-b77c-3b63962a14e8" />

#### 3.1.6 Game Over & Meta-Progression Interface
When Player HP reaches 0, the game displays a "You Died" screen, summarizing the run and calculating the total `Fame` points earned for the next playthrough.

<img width="208" height="130" alt="image" src="https://github.com/user-attachments/assets/dcd8f050-08e7-482f-a9ef-d2855a3dac58" />

### 3.2 Hardware Interfaces
Standard keyboard for input and monitor for console output.

### 3.3 Software Interfaces & File Organization
Data persistence is handled via two distinct `.dat` text files.

#### 3.3.1 savegame.dat (Session Data)
Stores the active character's current state:
* **Block 1 (Player Stats):** Level, Current HP, Max HP, Gold, EXP, and current X/Y coordinates.
* **Block 2 (Inventory State):** Dimensions of the inventory and a serialized list of all items currently held.
* **Block 3 (Equipment):** Identifiers for the currently equipped Weapon and Armor.

#### 3.3.2 meta.dat (Meta-Progression Data)
Survives player death and soft resets. Primarily stores the `Fame` integer variable to retain benefits for future runs.

### 3.4 Communications Interfaces
None.

---

## 4. SYSTEM USE CASES

### 4.1 Map Exploration & Interaction
* **Objective:** Navigate the procedural world and trigger points of interest.
* **Basic Flow:** Player uses WASD. System updates `@` coordinates and checks for Town interactions or random encounters (25% chance).

### 4.2 Combat Encounter
* **Objective:** Defeat enemies to gain EXP and Gold.
* **Basic Flow:** Player selects a combat action. System calculates damage using equipped gear. Enemy counter-attacks. Loop continues until victory or death.

### 4.3 Inventory Management
* **Objective:** Optimize gear and manage collected loot.
* **Basic Flow:** Player opens inventory, hovers over items, and presses `E` to equip, `M` to move, or `$` to sell.

### 4.4 Item Upgrade (The Anvil)
* **Objective:** Merge two items to create a stronger version.
* **Basic Flow:** Player inputs grid coordinates for two identical items. System validates and generates a Level N+1 item.

### 4.5 The Store / Economy
* **Objective:** Purchase new equipment using accumulated Gold.
* **Basic Flow:** System generates random stock. Player selects an item. System deducts Gold and places item in inventory.

### 4.6 Player Death & Meta-Progression
* **Objective:** Handle the game-over state gracefully.
* **Basic Flow:** Player dies. System calculates `Fame`, updates `meta.dat`, and deletes `savegame.dat`.

---

## 5. OTHER NONFUNCTIONAL REQUIREMENTS

### 5.1 Performance Requirements
Input response time must be under 16ms to avoid terminal flickering.

### 5.2 Safety Requirements
Input validation must prevent out-of-bounds memory access during coordinate entry.

### 5.3 Software Quality Attributes
* **Maintainability:** High (OOP modular design).
* **Portability:** High (Standard C++).

---

## 6. SYSTEM REQUIREMENTS CHART

| ID | Priority | Type | Description |
| :--- | :--- | :--- | :--- |
| REQ-01 | High | F | System shall allow 2D movement using WASD keys. |
| REQ-02 | High | F | System shall calculate combat damage based on gear levels. |
| REQ-03 | Medium | F | System shall save progress to a local file. |
| REQ-04 | High | NF | Interface shall render via ASCII characters. |

---

## APPENDIX A: GLOSSARY
* **ASCII:** American Standard Code for Information Interchange.
* **Meta-Progression:** Mechanics carrying over after death.
