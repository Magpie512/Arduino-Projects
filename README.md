# Trigger Tactics 🕹️
*A pipboy-esque tactical interface for simulated combat interactions (Five-way rock-paper-scissors).*

---

## Overview

**Trigger Tactics** is a physical interface game inspired by **Fallout's Pip-Boy**, designed to simulate strategic combat phases using tactile inputs and feedback mechanisms. Players engage in turn-based actions with limited-use abilities, health counters, and a dynamic interaction grid.

---

## System Components & I/O

| Component | Description | Code Pin | Type | Status/Note |
| :--- | :--- | :--- | :--- | :--- |
| **Cycle Button** (CYC) | Scrolls through move options (A0) | **A0** | Analog/Digital Input | `INPUT_PULLUP` |
| **Confirm Button** (SEL) | Selects current option/Confirms move (A1) | **A1** | Analog/Digital Input | `INPUT_PULLUP` |
| **Display Switch** | Toggles the LCD display power (ON/OFF) | **7** | Digital Input | `INPUT_PULLUP` |
| **Buzzer** | Provides audio feedback (Tunes/Damage SFX) | **10** | Digital Output | `tone()` function |
| **LCD Screen (16x2)** | Displays choices, HP, and turn results | **12, 11, 5, 4, 3, 6** | Digital Output | LiquidCrystal pins |
| **RGB LEDs** | Visual feedback (Pazazz!) | **13, 8, 9** | Digital Output | `rLed, gLed, bLed` |

---

## Action Commands

| Action | Type | Effect | Caveat / Limit |
| :--- | :--- | :--- | :--- |
| **Blast** | Attack | Deals 1 damage to opponent | Only **2 uses** per game. |
| **Guard** | Defense | Blocks Blast, no effect otherwise. | Vulnerable to **Trick** (-2 HP). |
| **Trick** | Manipulate | Bypasses **Guard** to deal 2 damage. | If matched against **Blast**, user takes **-2 HP**. |
| **Patch** | Heal | Heals 1 HP. | **3 uses** max. Interrupted by **Blast** or **Trick**. |
| **Jam** | Disrupt | Stuns opponent if timed with **Blast** or **Patch**. | If matched against **Trick**, user is stunned next turn. |

---

## Interaction Grid

| User vs Enemy | **Blast** | **Guard** | **Trick** | **Patch** | **Jam** |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Blast** | Both take 1 DMG | Enemy takes 0 DMG | Enemy takes 2 DMG | Enemy takes 1 DMG | User is stunned |
| **Guard** | Enemy takes 0 DMG | No effect | User takes 2 DMG | Enemy Heals 1 HP | No effect |
| **Trick** | User takes 2 DMG | Enemy takes 2 DMG | Both take 1 DMG | Enemy takes 1 DMG | User is stunned |
| **Patch** | User takes 1 DMG | User Heals 1 HP | Patch wasted (No effect) | Both Heal 1 HP | User is stunned |
| **Jam** | Enemy stunned | No effect | User stunned | Enemy stunned | No effect |

---

## Wiring Pinout (Arduino)

This table shows the hardware connections based on the `//Pin Delares` section of the code.

| Component | Component Pin | Arduino Pin | Notes |
| :--- | :--- | :--- | :--- |
| **Cycle/Back Button** | Digital Out | **A0** | Used for Cycle (forward) and Back (return) |
| **Confirm/Select Button**| Digital Out | **A1** | Used for Select (confirm) and Reset |
| **Piezo Buzzer** | Signal | **10** | `buzzerPin` |
| **Display ON/OFF Switch**| Digital Out | **7** | `displaySwitch` |
| **Red LED** | Anode (+) | **13** | `rLed` (Usually built-in LED too) |
| **Green LED** | Anode (+) | **8** | `gLed` |
| **Blue LED** | Anode (+) | **9** | `bLed` |
| **LCD RS** | Register Select | **12** | |
| **LCD Enable** | Enable | **11** | |
| **LCD D4** | Data 4 | **5** | |
| **LCD D5** | Data 5 | **4** | |
| **LCD D6** | Data 6 | **3** | |
| **LCD D7** | Data 7 | **6** | |

---

## Development Notes 📝

### Key Implementation Details
* The **"Dial"** mentioned in the original components list is implemented functionally by the **Cycle Button (A0)** in the code.
* **Health:** Both Player and Enemy start with **5 HP**.
* **Limited Uses:** **Blast** (2 uses), **Patch** (3 uses).
* **Stunned State:** A player/enemy skips their move on the next turn when stunned. When the enemy is stunned, the player's move resolves without an enemy counter-move.

### Troubleshooting & Learnings
* **2025-10-17:** Resolved bug where the On/Off switch logic was incomplete, now correctly checks for both states.
* **2025-10-17:** Corrected wiring issue for button assignment (A1 and A2) to properly utilize **A0** (`cycleBtn`) and **A1** (`confirmBtn`).
* **RGB LED:** Intended for adding visual flair to turn resolution.
* **Contrast Potentiometer:** Not critical, may be removed for simplification.
* **Music:** All tunes are placeholder/simple functions (AI/Stock) due to the developer's musical inexperience.
* **Patch vs Trick Correction:** Patch is stopped by Trick, and the use is consumed ("Patch wasted").

---

## IDEAS FOR FLAVOR

* Use Pixel art custom characters on the LCD for HP indicators or move icons.
* Utilize **RGB LEDs** for status notifications (e.g., Red flash on damage, Green flash on heal).
* Add more complex background **Music Tunes**.
