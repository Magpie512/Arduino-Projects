# Trigger Tactics  
*A pipboy-esque tactical interface for simulated combat interactions.*
*( Five way rock paper scissors )*

---

## Overview

**Trigger Tactics** is a physical interface game inspired by Fallout's Pip-Boy, designed to simulate strategic combat phases using tactile inputs and feedback mechanisms. Players engage in turn-based actions with limited-use abilities, health counters, and a dynamic interaction grid.

---

## System Components

| Component         | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| **Dial**         | Potentiometer used to scroll/select options                                 |
| **Select Button**| Advances to the next phase                                                  |
| **Back Button**  | Returns to the previous phase                                               |
| **Power Button** | Powers the system on                                                        |
| **Damage Indicators** | Piezo buzzer, vibe motor, or LEDs simulate damage feedback ("owies")     |
| **LCD Screen**   | Displays current action choices                                             |
| **Segment Left** | Opponent HP counter (5 → 0)                                                 |
| **Segment Right**| Player HP counter (5 → 0)                                                   |

---

## Action Commands

| Action   | Type        | Effect                                                                 | Caveat                                                                 |
|----------|-------------|------------------------------------------------------------------------|------------------------------------------------------------------------|
| **Blast**| Attack      | Deals 1 damage to opponent                                              | Only 2 uses                                                            |
| **Guard**| Defense     | Blocks Blast                                                            | Vulnerable to Trick (-1 HP)                                           |
| **Trick**| Manipulate  | Bypasses Guard, deals 1 damage                                          | If matched against Blast: deals 0 DMG and takes -2 HP                 |
| **Patch**| Heal        | Heals 1 HP                                                              | 3 uses max; interrupted by Blast or Trick                             |
| **Jam**  | Disrupt     | Stuns opponent if timed with Blast or Patch                             | If used during Trick, user is stunned next turn                       |

---

## Interaction Grid

| User vs Enemy | Blast         | Guard         | Trick         | Patch         | Jam           |
|---------------|---------------|---------------|---------------|---------------|---------------|
| **Blast**     | Both take 1 DMG, user stunned | Enemy takes 0 DMG | Enemy takes 2 DMG | Enemy takes 1 DMG | User is stunned |
| **Guard**     | Enemy takes 0 DMG | No effect     | User takes 2 DMG | Heals 1 HP     | No effect     |
| **Trick**     | User takes 2 DMG | Enemy takes 2 DMG | Both take 1 DMG, both stunned | Patch wasted | User is stunned |
| **Patch**     | Takes 1 DMG    | Heals 1 HP     | Patch wasted   | Heals 1 HP     | User is stunned |
| **Jam**       | Enemy stunned  | No effect     | User stunned   | Enemy stunned  | No effect     |

---

## Notes

- **Patch vs Trick**: Patch is consumed even if no healing or damage occurs.
- **Jam vs Trick**: Risky—can backfire and stun the user.
- **Blast**: Limited to 2 uses per game.
- **Patch**: Limited to 3 uses per game.

---

## Wiring

---

## Problems
On/Off Works now 25-10-17
Problem was an ill written function that initially
only checked for if its off and never if its on

Auto Selecting Turns 25-10-17
The problem came from wiring being 
a1 for cycle
and a2 for select
not a code issure just me wiring it like a chimp

Going to add RGB LED to inherently add problems

Contrast Potentiometer 
Is either on at 100% or 
off at every other config
Admittedly not much of an issue just wanted to 
toy with it. might remove tbh

if patch doesnt heal user 2025-10-17

---

## IDEAS

USE PIXEL DESIGNS LIKE STAR FOR EXPLOSION

UTILIZE RGB LED

---

## NOTES

ALL TUNES ARE AI
I AM NOT MUSICALLY INCLINED

