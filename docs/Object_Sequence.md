# Object Sequence

This document describes the runtime sequence of each main object in Archery Game. Each object is handled by its own AK task and receives signals from the screen task, button callbacks, timers, or other object tasks.

## I. Object Summary

| Object | Task ID | Handler | Main responsibility |
|---|---|---|---|
| Archery | `AR_GAME_ARCHERY_ID` | `ar_game_archery_handle()` | Controls the player position and bow image state. |
| Arrow | `AR_GAME_ARROW_ID` | `ar_game_arrow_handle()` | Shoots arrows, moves active arrows, and restores available arrow count. |
| Meteoroid | `AR_GAME_METEOROID_ID` | `ar_game_meteoroid_handle()` | Spawns meteoroids, moves them, checks collision with arrows, and updates score. |
| Bang | `AR_GAME_BANG_ID` | `ar_game_bang_handle()` | Plays explosion animation after a meteoroid is hit. |
| Border | `AR_GAME_BORDER_ID` | `ar_game_border_handle()` | Checks level-up condition and game-over condition. |

## II. Archery Object Sequence

Archery owns the player position. Button signals update the internal `archery_y` value, and the periodic update signal copies it into the rendered `archery.y`.

```mermaid
sequenceDiagram
    autonumber
    participant Screen as Screen Task
    participant Button as Button Callback
    participant AK as AK Scheduler
    participant Archery as Archery Task
    participant Render as Screen Render

    Screen->>AK: AR_GAME_ARCHERY_SETUP
    AK->>Archery: Handle setup
    Archery->>Archery: Set x, y, visible, action_image

    Button->>AK: AR_GAME_ARCHERY_UP
    AK->>Archery: Handle move up
    Archery->>Archery: Decrease archery_y with min limit

    Button->>AK: AR_GAME_ARCHERY_DOWN
    AK->>Archery: Handle move down
    Archery->>Archery: Increase archery_y with max limit

    Screen->>AK: AR_GAME_ARCHERY_UPDATE
    AK->>Archery: Handle update
    Archery->>Archery: archery.y = archery_y
    Render-->>Archery: Read latest Archery state

    Screen->>AK: AR_GAME_ARCHERY_RESET
    AK->>Archery: Handle reset
    Archery->>Archery: Hide Archery object
```

## III. Arrow Object Sequence

Arrow receives shoot input from the MODE button. Each game tick moves visible arrows to the right. When an arrow exits the screen, it is hidden and the available arrow count is restored.

```mermaid
sequenceDiagram
    autonumber
    participant Button as Button Callback
    participant Screen as Screen Task
    participant AK as AK Scheduler
    participant Arrow as Arrow Task
    participant Archery as Archery State
    participant Buzzer as Buzzer

    Screen->>AK: AR_GAME_ARROW_SETUP
    AK->>Arrow: Handle setup
    Arrow->>Arrow: Clear all arrows

    Button->>AK: AR_GAME_ARROW_SHOOT
    AK->>Arrow: Handle shoot

    alt num_arrow == 0
        Arrow->>Buzzer: Play warning sound
    else arrow is available
        Arrow->>Arrow: Find hidden arrow slot
        Arrow->>Arrow: Show arrow at Archery position
        Arrow->>Arrow: Decrease num_arrow
        Arrow->>Archery: Set empty bow image when no arrow remains
        Arrow->>Buzzer: Play click sound
    end

    Screen->>AK: AR_GAME_ARROW_RUN
    AK->>Arrow: Handle run
    Arrow->>Arrow: Move visible arrows by arrow_speed

    alt arrow reaches screen edge
        Arrow->>Arrow: Hide arrow and reset x
        Arrow->>Archery: Restore available arrow image
    end

    Screen->>AK: AR_GAME_ARROW_RESET
    AK->>Arrow: Handle reset
    Arrow->>Arrow: Hide all arrows
```

## IV. Meteoroid Object Sequence

Meteoroid moves from right to left. On each tick, it updates position and animation frame. Collision checking compares active arrows with visible meteoroids.

```mermaid
sequenceDiagram
    autonumber
    participant Screen as Screen Task
    participant AK as AK Scheduler
    participant Meteoroid as Meteoroid Task
    participant Arrow as Arrow State
    participant Bang as Bang State
    participant Score as Score State
    participant Buzzer as Buzzer

    Screen->>AK: AR_GAME_METEOROID_SETUP
    AK->>Meteoroid: Handle setup
    Meteoroid->>Meteoroid: Create meteoroids by lane
    Meteoroid->>Meteoroid: Random x and action_image

    Screen->>AK: AR_GAME_METEOROID_RUN
    AK->>Meteoroid: Handle run
    Meteoroid->>Meteoroid: Move visible meteoroids left
    Meteoroid->>Meteoroid: Advance animation frame

    Screen->>AK: AR_GAME_METEOROID_DETONATOR
    AK->>Meteoroid: Handle collision check
    Meteoroid-->>Arrow: Check visible arrows

    alt arrow collides with meteoroid
        Meteoroid->>Arrow: Hide arrow and reset position
        Meteoroid->>Meteoroid: Hide meteoroid and randomize next spawn
        Meteoroid->>Bang: Show bang at collision position
        Meteoroid->>Score: Add 10 points
        Meteoroid->>Buzzer: Play bang sound
    end

    Screen->>AK: AR_GAME_METEOROID_RESET
    AK->>Meteoroid: Handle reset
    Meteoroid->>Meteoroid: Hide all meteoroids
```

## V. Bang Object Sequence

Bang is the explosion effect. It becomes visible when Meteoroid detects a collision. The Bang task only advances the effect frame and restores the meteoroid after the animation ends.

```mermaid
sequenceDiagram
    autonumber
    participant Screen as Screen Task
    participant AK as AK Scheduler
    participant Meteoroid as Meteoroid Task
    participant Bang as Bang Task
    participant Render as Screen Render

    Screen->>AK: AR_GAME_BANG_SETUP
    AK->>Bang: Handle setup
    Bang->>Bang: Hide all bang effects

    Meteoroid->>Bang: Set visible bang after collision
    Meteoroid->>Bang: Set bang x, y, action_image

    Screen->>AK: AR_GAME_BANG_UPDATE
    AK->>Bang: Handle update
    Bang->>Bang: Advance bang action_image
    Render-->>Bang: Read visible bang frame

    alt animation reaches end
        Bang->>Bang: Hide bang
        Bang->>Meteoroid: Show meteoroid again
    end

    Screen->>AK: AR_GAME_BANG_RESET
    AK->>Bang: Handle reset
    Bang->>Bang: Hide all bang effects
```

## VI. Border Object Sequence

Border protects the safe area. It checks level-up by score and sends `AR_GAME_RESET` to the screen task when a visible meteoroid reaches the border.

```mermaid
sequenceDiagram
    autonumber
    participant Screen as Screen Task
    participant AK as AK Scheduler
    participant Border as Border Task
    participant Meteoroid as Meteoroid State
    participant Setting as Game Setting

    Screen->>AK: AR_GAME_BORDER_SETUP
    AK->>Border: Handle setup
    Border->>Border: Set x, visible, action_image

    Screen->>AK: AR_GAME_LEVEL_UP
    AK->>Border: Handle level-up check

    alt score reaches next level score
        Border->>Setting: Increase meteoroid_speed
        Border->>Border: Move next level score threshold
    end

    Screen->>AK: AR_GAME_CHECK_GAME_OVER
    AK->>Border: Handle game-over check
    Border-->>Meteoroid: Check visible meteoroid x position

    alt meteoroid reaches border
        Border->>AK: AR_GAME_RESET
        AK->>Screen: Reset game flow
    end

    Screen->>AK: AR_GAME_BORDER_RESET
    AK->>Border: Handle reset
    Border->>Border: Hide Border object
```

## VII. Code References

| Object | Source file | Header file |
|---|---|---|
| Archery | `application/sources/app/game/archery_game/ar_game_archery.cpp` | `application/sources/app/game/archery_game/ar_game_archery.h` |
| Arrow | `application/sources/app/game/archery_game/ar_game_arrow.cpp` | `application/sources/app/game/archery_game/ar_game_arrow.h` |
| Meteoroid | `application/sources/app/game/archery_game/ar_game_meteoroid.cpp` | `application/sources/app/game/archery_game/ar_game_meteoroid.h` |
| Bang | `application/sources/app/game/archery_game/ar_game_bang.cpp` | `application/sources/app/game/archery_game/ar_game_bang.h` |
| Border | `application/sources/app/game/archery_game/ar_game_border.cpp` | `application/sources/app/game/archery_game/ar_game_border.h` |

