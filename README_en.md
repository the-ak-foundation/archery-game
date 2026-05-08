# Archery Game - Build on AK Embedded Base Kit

| [VN](README.md) | [EN] |

## I. Introduction

The Archery game is a game running on the AK Embedded Base Kit. It is built to help embedded programming enthusiasts learn and practice event-driven programming. During the development of the archery game, you will learn more about designing and applying UML, Tasks, Signals, Timers, Messages, State-machines,...

### 1.1 Hardware

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/AK_Embedded_Base_Kit_STM32L151.webp" alt="AK Embedded Base Kit - STM32L151" width="480"/></p>
<p align="center"><strong><em>Figure 1:</em></strong> AK Embedded Base Kit - STM32L151</p>

[AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu) is an evaluation kit for advanced embedded software learners.

The KIT integrates an **OLED 1.3" LCD, 3 buttons, and a Buzzer speaker**, with these features sufficient to learn the event-driven system through practical game design.

The KIT also integrates **RS485, NRF24L01+, and Flash up to 32MB**, suitable for prototyping real-world applications in embedded systems such as wired, wireless communication, data logger storage applications,...

### 1.2 Game Description and Objects
The following description of the **“Archery game”**, explains how to play and the game's processing mechanism. This document is used for reference in designing and developing the game later.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/menu_game.webp" alt="menu game" width="480"/></p>
<p align="center"><strong><em>Figure 2:</em></strong> Menu game</p>

The game starts with the **Menu game** screen with the following options: 
- **Archery Game:** Select to start the game.
- **Setting:** Select to set the game parameters.
- **Charts:** Select to view the top 3 highest scores.
- **Exit:** Exit the menu to the standby screen.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/objects_in_the_game.webp" alt="archery game play screen" width="600"/></p>
<p align="center"><strong><em>Figure 3:</em></strong> Game play screen and objects</p>

#### 1.2.1 Objects in the Game:
|Object|Object Name|Description|
|---|---|---|
|**Bow**|Archery|Move up/down to select the position to shoot the arrow|
|**Arrow**|Arrow|Shot from the bow, used to destroy meteoroids|
|**Explosion**|Bang|Effect that appears when meteoroid is destroyed|
|**Border**|Border|Safe zone to protect from meteoroids falling into|
|**Meteoroid**|Meteoroid|Object flying towards the bow with increasing speed, capable of destroying the border|

**(*)** In the rest of the document, the names of the objects will be used to refer to the objects.

#### 1.2.2 How to Play: 
- In this game, you will control the Archery, move **up/down** with the **[Up]/[Down]** buttons, to select the position to **shoot** the Arrow.
- When pressing the **[Mode]** button, the Arrow will be shot, aiming to destroy the incoming Meteoroids.
- The goal of the game is to get as many points as possible, the game will end when a Meteoroid touches the Border.

#### 1.2.3 Game Mechanics:
- **Scoring:** Points are calculated by the number of Meteoroids destroyed. Each destroyed Meteoroid corresponds to 10 points. The accumulated score will be displayed in the bottom right corner of the screen.
- **Difficulty:** Every time 200 points are accumulated, the Meteoroid's flying speed will increase by one level. The initial difficulty can be set in the **Setting** section.
- **Arrow Limit:** When shooting, the number of available Arrows will decrease corresponding to the number of flying Arrows, if the available Arrows decrease to "0", you cannot shoot and there will be a warning sound. The number of available Arrows will be restored when a Meteoroid is destroyed or the Arrow flies off the screen. The number of Arrows is displayed in the bottom left corner of the screen and can be changed in the **Setting**.

- **Animation:** To make the game more lively, objects will have additional animation when moving.
- **Game Over:** When a Meteoroid touches the Border, the game will end. Objects will be reset and the score will be saved. You will enter the “Game Over” screen with 3 options:
  - **Restart:** play again.
  - **Charts:** go to view the leaderboard.
  - **Home:** back to the game menu.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/game_over.webp" alt="archery game over screen" width="480"/></p>
<p align="center"><strong><em>Figure 4:</em></strong> Game_over screen</p>

## II. Design - ARCHERY GAME

### 2.1 Sequence Diagram

### 2.2 Details

#### 2.2.1 Object Attributes

#### 2.2.2 Task

#### 2.2.3 Message & Signal

## III. Detailed Code Guide for Objects

### 3.1 Archery

### 3.2 Arrow

### 3.3 Bang

### 3.4 Border

### 3.5 Meteoroid

## IV. Display and Sound in Archery Game

### 4.1 Graphics

#### 4.1.1 Designing Graphics for Objects

#### 4.1.2 Code

### 4.2 Sound

## V. EEPROM Management

### 5.1 Management Structure

### 5.2 Function