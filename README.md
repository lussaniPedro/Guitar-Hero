# 🎸 Guitar Hero (Terminal Edition)

A mini **Guitar Hero** game fully developed in **C**, playable directly in the **Windows Terminal**, featuring **ANSI colors**, an interactive NPC, player rankings, automatic saves, and a difficulty system!

---

## 📜 Description

This project is a **rhythmic game** inspired by the classic *Guitar Hero*:
- Notes fall down the screen,
- You must press the correct key at the right time,
- Combos and scores are calculated,
- Player rankings and records are saved to a file,
- Includes a **tutorial**, an **NPC assistant**, and a **developer mode** to manage players.

---

## ⚙️ Features

✅ 3 difficulty levels (`Cry baby`, `Normal`, `Rock 'n' Roll`)  
✅ Player rankings and records saved in `GuitarHero.txt`  
✅ **Combo** system with record explosion 🎇  
✅ Interactive tutorial with an option to skip (`SPACE`)  
✅ ASCII animations for NPC 👾  
✅ **Color** system for keys (green, red, yellow, blue, orange)  
✅ Customizable keys via the settings menu  
✅ Configurable guitar arm size  
✅ Developer mode for managing players

---

## 🎮 Controls

- Use **UP/DOWN/LEFT/RIGHT** to navigate through the menus.
- **ENTER** to select.
- **ESC** to exit menus.
- Default keys `A S J K L` to play notes — **can be changed in the Settings menu**.
- During the tutorial: **SPACE** to skip.
- During the game: **ESC** ends the song.

---

## 🗂️ Project Structure

```plaintext
.
├── output
│   ├── GuitarHero.exe  (compiled)
│   └── GuitarHero.txt  (player save data)
├── src
│   ├── GuitarHero.c (main source code)
│   ├── gh_input.c (lib implementation)
│   └── gh_input.h (lib declaration)
└── README.md