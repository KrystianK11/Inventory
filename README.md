# ⚔️ Terminal RPG: Inventory & Exploration

A fully-featured, console-based 2D RPG developed entirely in modern C++. This project focuses on deep system mechanics, offering players a procedural world to explore, a dynamic loot system, and a roguelite meta-progression loop.

## 🌟 Key Features

*   🗺️ **World Exploration:** Navigate a 15x15 coordinate grid map featuring passable terrain decorations (mountains, grass) and safe zones (Towns).
*   🎒 **Advanced Inventory Management:** A dynamic 5x5 grid system. Hover over items to see real-time stat updates, equip gear, consume potions, or organize your bag using the built-in Sorting Menu (sort by value, name, or compact).
*   ⚔️ **Tactical Combat:** Trigger random encounters while exploring. Engage in a turn-based battle system against enemies with scaling difficulty and stats based on your equipped gear.
*   ⚒️ **The Anvil (Upgrading System):** Visit the Town to access the Anvil. Merge two identical items together to level them up and increase their power.
*   💰 **Dynamic Economy:** Earn gold from battles and spend it at the Town Store, which generates random stock and potions every visit.
*   💾 **Persistent Progression:** 
    *   **Session Saves:** Your current run, stats, and inventory are securely saved and loaded via `savegame.dat`.
    *   **Meta-Progression:** When you die, the game calculates your run score and rewards you with `Fame` points saved in `meta.dat`, offering permanent benefits for future playthroughs.

## 🎮 Controls
*   **W, A, S, D** - Movement across the map
*   **I** - Open/Close Inventory
*   **1, 2, 3** - Combat Actions (Attack, Special, Block)
*   Inside Inventory: **E** (Equip/Consume), **M** (Move), **$** (Sell), **Z** (Sort)

## 📋 Project Tracking
The development process, feature roadmap, and task management are tracked via Trello. 
🔗 **[View Trello Board](https://trello.com/b/UCpV3ZoX/inventory)**
