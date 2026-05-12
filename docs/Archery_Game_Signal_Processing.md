# Archery Game Signal Processing

This document explains how the Archery Game processes button input, task messages, game-loop ticks, and object updates. The game uses the AK event-driven task architecture: each major game object owns a task, receives signals through AK messages, and updates its own state.

## I. Overview

The Archery Game is implemented using event-driven tasks.

Each game object owns:

- A dedicated task
- Its own signal handler
- Its own state data
- Its own update logic

The display task (`AC_TASK_DISPLAY_ID`) owns the screen manager and handles screen-level events. During gameplay, it also receives the periodic game tick and posts update messages for each game-object task.

Input events from hardware buttons are converted into software signals. These signals are posted into the AK message pool first, then the AK scheduler dispatches them to the destination task handler.

The main game loop is driven by a periodic timer signal:

```c
AR_GAME_TIME_TICK
```

The current game tick interval is:

```c
AR_GAME_TIME_TICK_INTERVAL = 100 ms
```

Main runtime flow:

1. Button callbacks or timers create software signals.
2. Signals are posted into the AK message pool.
3. The AK scheduler dispatches messages to destination task handlers.
4. Each task updates only the state it owns.
5. The screen render reads the latest object state and refreshes the display buffer.

### High Level Architecture

#### 1. Game Start

```mermaid
%%{
  init: {
    "theme": "base",
    "themeVariables": {
      "background": "#0D1117",
      "primaryColor": "#161B22",
      "primaryBorderColor": "#58A6FF",
      "primaryTextColor": "#C9D1D9",
      "lineColor": "#58A6FF",
      "actorBkg": "#21262D",
      "actorBorder": "#58A6FF",
      "actorTextColor": "#79C0FF",
      "signalColor": "#58A6FF",
      "signalTextColor": "#C9D1D9",
      "noteBkgColor": "#1F2937",
      "noteBorderColor": "#F59E0B",
      "noteTextColor": "#F9FAFB",
      "activationBkgColor": "#1D4ED8",
      "activationBorderColor": "#60A5FA"
    }
  }
}%%
sequenceDiagram
    autonumber

      participant Screen as Display Task<br/>(Archery Game Screen)
      participant Setting as Setting Storage<br/>(EEPROM)
      participant AK as AK OS Event-Driven<br/>(Message Pool and Scheduler)
      participant Archery as Archery Task
      participant Arrow as Arrow Task
      participant Meteoroid as Meteoroid Task
      participant Bang as Bang Task
      participant Border as Border Task
      participant Timer as Game Tick Timer

      Note over Screen,Timer: GAME START

      Note right of Screen: Read game settings
      Screen->>Setting:
      activate Screen
      Note right of Setting: Read EEPROM<br/>Game Setting

      Note over Screen,AK: Post setup messages to AK Message Pool

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_ARCHERY_SETUP
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_ARROW_SETUP
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_METEOROID_SETUP
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_BANG_SETUP
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_BORDER_SETUP
      deactivate AK

      Note right of Screen: Create periodic timer
      Screen->>Timer:
      Note right of Timer: Periodic Game Tick Timer<br/>(Trigger every 100 ms)

      Note right of Screen: State = GAME_PLAY
      Screen->>Screen:
      Note right of Screen: Screen Render<br/>(Refresh Display Buffer)
      deactivate Screen

      Note over AK,Border: AK task scheduler processes setup object messages

      Note right of AK: Handle signal (AR_GAME_ARCHERY_SETUP)
      AK->>Archery:
      activate Archery
      Note right of Archery: Initialize Archery object
      deactivate Archery

      Note right of AK: Handle signal (AR_GAME_ARROW_SETUP)
      AK->>Arrow:
      activate Arrow
      Note right of Arrow: Initialize Arrow object
      deactivate Arrow

      Note right of AK: Handle signal (AR_GAME_METEOROID_SETUP)
      AK->>Meteoroid:
      activate Meteoroid
      Note right of Meteoroid: Initialize Meteoroid object
      deactivate Meteoroid

      Note right of AK: Handle signal (AR_GAME_BANG_SETUP)
      AK->>Bang:
      activate Bang
      Note right of Bang: Initialize Bang object
      deactivate Bang

      Note right of AK: Handle signal (AR_GAME_BORDER_SETUP)
      AK->>Border:
      activate Border
      Note right of Border: Initialize Border object
      deactivate Border
```

#### 2. Game Playing

```mermaid
%%{
  init: {
    "theme": "base",
    "themeVariables": {
      "background": "#0D1117",
      "primaryColor": "#161B22",
      "primaryBorderColor": "#58A6FF",
      "primaryTextColor": "#C9D1D9",
      "lineColor": "#58A6FF",
      "actorBkg": "#21262D",
      "actorBorder": "#58A6FF",
      "actorTextColor": "#79C0FF",
      "signalColor": "#58A6FF",
      "signalTextColor": "#C9D1D9",
      "noteBkgColor": "#1F2937",
      "noteBorderColor": "#F59E0B",
      "noteTextColor": "#F9FAFB",
      "activationBkgColor": "#1D4ED8",
      "activationBorderColor": "#60A5FA"
    }
  }
}%%
sequenceDiagram
    autonumber

      participant Button as Button Callback<br/>(UP, DOWN and MODE)
      participant Screen as Display Task<br/>(Archery Game Screen)
      participant AK as AK OS Event-Driven<br/>(Message Pool and Scheduler)
      participant Archery as Archery Task
      participant Arrow as Arrow Task
      participant Meteoroid as Meteoroid Task
      participant Bang as Bang Task
      participant Border as Border Task
      participant Timer as Game Tick Timer

      Note over Button,Timer: GAME PLAYING

      Note over Button,Archery: Button callback state

      Note right of Button: MODE button released
      Button->>AK:
      activate AK
      Note right of AK: AR_GAME_ARROW_SHOOT
      deactivate AK

      Note right of Button: UP button released
      Button->>AK:
      activate AK
      Note right of AK: AR_GAME_ARCHERY_UP
      deactivate AK

      Note right of Button: DOWN button released
      Button->>AK:
      activate AK
      Note right of AK: AR_GAME_ARCHERY_DOWN
      deactivate AK

      Note over AK,Archery: AK task scheduler processes button messages

      Note right of AK: Handle signal (AR_GAME_ARROW_SHOOT)
      AK->>Arrow:
      activate Arrow
      Note right of Arrow: Create a new Arrow from Archery position
      deactivate Arrow

      Note right of AK: Handle signal (AR_GAME_ARCHERY_UP)
      AK->>Archery:
      activate Archery
      Note right of Archery: Move Archery upward
      deactivate Archery

      Note right of AK: Handle signal (AR_GAME_ARCHERY_DOWN)
      AK->>Archery:
      activate Archery
      Note right of Archery: Move Archery downward
      deactivate Archery

      Note over Screen,Timer: Trigger every 100 ms

      Note right of AK: AR_GAME_TIME_TICK
      Timer->>AK:
      activate AK
      deactivate AK

      Note over AK,Screen: AK task scheduler processes timer message

      Note right of Screen: Handle signal (AR_GAME_TIME_TICK)
      AK->>Screen:
      activate Screen

      Note over Screen,AK: Post update messages to AK Message Pool

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_ARCHERY_UPDATE
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_ARROW_RUN
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_METEOROID_RUN
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_METEOROID_DETONATOR
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_BANG_UPDATE
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_LEVEL_UP
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_CHECK_GAME_OVER
      deactivate AK

      Screen->>Screen:
      Note right of Screen: Screen Render<br/>(Refresh Display Buffer)
      deactivate Screen

      Note over AK,Border: AK task scheduler processes update messages

      Note right of AK: Handle signal (AR_GAME_ARCHERY_UPDATE)
      AK->>Archery:
      activate Archery
      Note right of Archery: Update Archery display position
      deactivate Archery

      Note right of AK: Handle signal (AR_GAME_ARROW_RUN)
      AK->>Arrow:
      activate Arrow
      Note right of Arrow: Move active Arrows
      deactivate Arrow

      Note right of AK: Handle signal (AR_GAME_METEOROID_RUN)
      AK->>Meteoroid:
      activate Meteoroid
      Note right of Meteoroid: Move and animate Meteoroids
      deactivate Meteoroid

      Note right of AK: Handle signal (AR_GAME_METEOROID_DETONATOR)
      AK->>Meteoroid:
      activate Meteoroid
      Note right of Meteoroid: Check Arrow and Meteoroid collision
      deactivate Meteoroid

      Note right of AK: Handle signal (AR_GAME_BANG_UPDATE)
      AK->>Bang:
      activate Bang
      Note right of Bang: Update Bang animation
      deactivate Bang

      Note right of AK: Handle signal (AR_GAME_LEVEL_UP)
      AK->>Border:
      activate Border
      Note right of Border: Increase level when score reaches threshold
      deactivate Border

      Note right of AK: Handle signal (AR_GAME_CHECK_GAME_OVER)
      AK->>Border:
      activate Border
      Note right of Border: Check Meteoroid reaches Border
      deactivate Border
```

#### 3. Game Reset

```mermaid
%%{
  init: {
    "theme": "base",
    "themeVariables": {
      "background": "#0D1117",
      "primaryColor": "#161B22",
      "primaryBorderColor": "#58A6FF",
      "primaryTextColor": "#C9D1D9",
      "lineColor": "#58A6FF",
      "actorBkg": "#21262D",
      "actorBorder": "#58A6FF",
      "actorTextColor": "#79C0FF",
      "signalColor": "#58A6FF",
      "signalTextColor": "#C9D1D9",
      "noteBkgColor": "#1F2937",
      "noteBorderColor": "#F59E0B",
      "noteTextColor": "#F9FAFB",
      "activationBkgColor": "#1D4ED8",
      "activationBorderColor": "#60A5FA"
    }
  }
}%%
sequenceDiagram
    autonumber

      participant Screen as Display Task<br/>(Archery Game Screen)
      participant AK as AK OS Event-Driven<br/>(Message Pool and Scheduler)
      participant Archery as Archery Task
      participant Arrow as Arrow Task
      participant Meteoroid as Meteoroid Task
      participant Bang as Bang Task
      participant Border as Border Task
      participant Timer as Game Tick Timer
      participant Score as Score Storage<br/>(EEPROM)
      participant GameOver as  Display Task<br/>(Game Over Screen)

      Note over Border,GameOver: GAME RESET

      Note right of Border: Meteoroid reaches Border
      Border->>AK:
      activate AK
      Note right of AK: AR_GAME_RESET
      deactivate AK

      Note over AK,Screen: AK task scheduler processes reset message

      Note right of AK: Handle signal (AR_GAME_RESET)
      AK->>Screen:
      activate Screen
      Note right of Screen: Stop AR_GAME_TIME_TICK timer
      Screen->>Timer:

      Note over Screen,AK: Post reset messages to AK Message Pool

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_ARCHERY_RESET
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_ARROW_RESET
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_METEOROID_RESET
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_BANG_RESET
      deactivate AK

      Note right of Screen: Task post message
      Screen->>AK:
      activate AK
      Note right of AK: AR_GAME_BORDER_RESET
      deactivate AK

      Note right of Screen: Save score and ranking
      Screen->>Score:
      Note right of Score: Read, rank, and write score data

      Note right of Screen: Start game-over timers
      Screen->>Timer:
      Note right of Timer: AR_GAME_EXIT_GAME<br/>(Trigger after 5000 ms)
      Screen->>Timer:
      Note right of Timer: AR_GAME_OVER_TEXT_ANIM_TICK<br/>(Trigger every 250 ms)

      Note right of Screen: State = GAME_OVER
      Screen->>Screen:
      Note right of Screen: Screen Render<br/>(Refresh Display Buffer)
      deactivate Screen

      Note over AK,Border: AK task scheduler processes reset object messages

      Note right of AK: Handle signal (AR_GAME_ARCHERY_RESET)
      AK->>Archery:
      activate Archery
      Note right of Archery: Hide Archery object
      deactivate Archery

      Note right of AK: Handle signal (AR_GAME_ARROW_RESET)
      AK->>Arrow:
      activate Arrow
      Note right of Arrow: Hide all Arrow objects
      deactivate Arrow

      Note right of AK: Handle signal (AR_GAME_METEOROID_RESET)
      AK->>Meteoroid:
      activate Meteoroid
      Note right of Meteoroid: Hide all Meteoroid objects
      deactivate Meteoroid

      Note right of AK: Handle signal (AR_GAME_BANG_RESET)
      AK->>Bang:
      activate Bang
      Note right of Bang: Hide all Bang objects
      deactivate Bang

      Note right of AK: Handle signal (AR_GAME_BORDER_RESET)
      AK->>Border:
      activate Border
      Note right of Border: Hide Border object
      deactivate Border

      Note over Timer,GameOver: Exit game-over animation
      NOTE right of Timer: Timer trigger after 5000ms
      Timer->>AK:
      activate AK
      Note right of AK: AR_GAME_EXIT_GAME
      deactivate AK

      Note right of AK: Handle signal (AR_GAME_EXIT_GAME)
      AK->>Screen:
      activate Screen
      Note right of Screen: Stop text animation timer
      Screen->>Timer:
      Note right of Screen: State = GAME_OFF
      Screen->>GameOver:
      deactivate Screen
      Note right of GameOver: Switch to Game Over screen
      activate GameOver
      GameOver->>GameOver:
      Note right of GameOver: Screen Render<br/>(Refresh Display Buffer)
      deactivate GameOver
```


## II. Code References

| Area | File |
|---|---|
| Task IDs and task handlers | `application/sources/app/task_list.h` |
| Task table registration | `application/sources/app/task_list.cpp` |
| Signal definitions | `application/sources/app/app.h` |
| Button callback logic | `application/sources/app/app_bsp.cpp` |
| Main game screen logic | `application/sources/app/screens/scr_archery_game.cpp` |
| Screen manager | `application/sources/common/screen_manager.cpp` |

## III. Task Ownership

| Task | Responsibility | Owns Data | Receives Signals |
|---|---|---|---|
| `AC_TASK_DISPLAY_ID` | Screen manager, render scheduling, and central game tick dispatch | Current screen state | Display signals and game tick signals |
| `AR_GAME_ARCHERY_ID` | Archer/player control | Archer position and display state | `SETUP`, `UPDATE`, `UP`, `DOWN`, `RESET` |
| `AR_GAME_ARROW_ID` | Arrow movement and shooting | Arrow state and active arrows | `SETUP`, `RUN`, `SHOOT`, `RESET` |
| `AR_GAME_METEOROID_ID` | Meteoroid movement and collision checks | Meteoroid state | `SETUP`, `RUN`, `DETONATOR`, `RESET` |
| `AR_GAME_BANG_ID` | Explosion animation | Effect frames and visibility | `SETUP`, `UPDATE`, `RESET` |
| `AR_GAME_BORDER_ID` | Border logic and game-over detection | Border state and level/game-over checks | `SETUP`, `LEVEL_UP`, `CHECK_GAME_OVER`, `RESET` |

## IV. Button Event Processing

Button callbacks serve two screen modes:

- Normal screen mode (`ar_game_state == GAME_OFF`): button signals are posted to `AC_TASK_DISPLAY_ID` for menu or screen navigation.
- Gameplay screen mode (`ar_game_state != GAME_OFF`): button signals are posted directly to game-object tasks, such as Arrow for shooting and Archery for movement.

In both modes, the callback only posts a signal to AK. The destination task handles that signal later when the AK scheduler dispatches it.

### Button Processing Rules

| Button | Condition | Posted Signal | Destination |
|---|---|---|---|
| MODE Released | `ar_game_state == GAME_OFF` | `AC_DISPLAY_BUTTON_MODE_RELEASED` | `AC_TASK_DISPLAY_ID` |
| MODE Released | `ar_game_state != GAME_OFF` | `AR_GAME_ARROW_SHOOT` | `AR_GAME_ARROW_ID` |
| UP Released | `ar_game_state == GAME_OFF` | `AC_DISPLAY_BUTTON_UP_RELEASED` | `AC_TASK_DISPLAY_ID` |
| UP Released | `ar_game_state != GAME_OFF` | `AR_GAME_ARCHERY_UP` | `AR_GAME_ARCHERY_ID` |
| DOWN Released | `ar_game_state == GAME_OFF` | `AC_DISPLAY_BUTTON_DOWN_RELEASED` | `AC_TASK_DISPLAY_ID` |
| DOWN Released | `ar_game_state != GAME_OFF` | `AR_GAME_ARCHERY_DOWN` | `AR_GAME_ARCHERY_ID` |

## V. Runtime Scheduling Notes

The system uses asynchronous task-based scheduling.

Important runtime characteristics:

- Signals are queued in AK before they are handled.
- A sender posts a message; it does not directly execute the destination task handler.
- Handlers are isolated by task ownership.
- Game logic is decoupled through signals.
- Timing depends on scheduler execution order.
- A signal may wait in the AK message pool before its handler executes.
- `AR_GAME_TIME_TICK` can appear between button pressed and released logs because the timer keeps running.
