# Data Setting EEPROM

This document describes how Archery Game stores persistent data in EEPROM, including game settings and score data.

## I. Overview

EEPROM is used for data that must remain valid after power-off. In Archery Game, EEPROM stores:

| Data block | Purpose | Source file |
|---|---|---|
| Game setting | Stores sound mode, number of arrows, arrow speed, and initial meteoroid speed. | `application/sources/app/app_eeprom.h` |
| Score data | Stores current score and the top 3 highest scores. | `application/sources/app/app_eeprom.h` |

Each EEPROM record is protected by a `Magic number` and a `Checksum`. This prevents the firmware from using invalid, corrupted, or unrelated EEPROM data.

## II. EEPROM Address Map

| Name | Address | Description |
|---|---:|---|
| `EEPROM_START_ADDR` | `0x0000` | Start address of the EEPROM area. |
| `EEPROM_END_ADDR` | `0x1000` | End address of the EEPROM area. |
| `EEPROM_SCORE_START_ADDR` | `0x0010` | Start address for score data. |
| `EEPROM_SCORE_PLAY_ADDR` | `0x00FA` | Reserved score-related address. |
| `EEPROM_SETTING_START_ADDR` | `0x0100` | Start address for game setting data. |

The Archery Game EEPROM Magic number is:

```cpp
#define AR_GAME_EEPROM_MAGIC_NUMBER ((uint32_t)0x123123BB)
```

## III. Record Format

Each EEPROM record uses the following layout:

```text
+----------------------+----------------------+----------------------+
| Magic number         | Data                 | Checksum             |
| 4 bytes              | payload struct       | 1 byte               |
+----------------------+----------------------+----------------------+
```

| Field | Description |
|---|---|
| `Magic number` | Identifies data that belongs to Archery Game. |
| `Data` | The payload struct that needs to be stored. |
| `Checksum` | 8-bit additive checksum from `Magic number` through the end of `Data`. |

## IV. Stored Data Structures

### 4.1 Game Setting

| Field | Meaning |
|---|---|
| `silent` | Sound on/off setting. |
| `num_arrow` | Number of available arrows. |
| `arrow_speed` | Arrow movement speed. |
| `meteoroid_speed` | Initial meteoroid movement speed. |

### 4.2 Score Data

| Field | Meaning |
|---|---|
| `score_now` | Score of the latest game session. |
| `score_1st` | Highest score. |
| `score_2nd` | Second highest score. |
| `score_3rd` | Third highest score. |

## V. Read And Write Mechanism

### 5.1 Read Data From EEPROM

Step 1: Read the full EEPROM record from the configured address.

```cpp
eeprom_read(address, (uint8_t *)&eeprom_data, sizeof(eeprom_data));
```

Step 2: Check the EEPROM driver result.

If the driver does not return `EEPROM_DRIVER_OK`, the data is treated as invalid.

Step 3: Check the `Magic number`.

The record is valid for Archery Game only when its Magic number matches:

```cpp
AR_GAME_EEPROM_MAGIC_NUMBER
```

Step 4: Recalculate the checksum.

The checksum is calculated from the `Magic number` through the end of the payload data.

Step 5: Decide which data to use.

| Condition | Result |
|---|---|
| Driver result is OK, Magic number is correct, and checksum matches | Use EEPROM payload data. |
| Any check fails | Use default data. |

### 5.2 Write Data To EEPROM

Step 1: Copy the latest application data into the EEPROM payload.

For score data, the payload is `ar_game_score_t`. For setting data, the payload is `ar_game_setting_t`.

Step 2: Write the Archery Game Magic number into the record.

```cpp
eeprom_data.magic_number = AR_GAME_EEPROM_MAGIC_NUMBER;
```

Step 3: Calculate the checksum for the record.

The checksum covers the `Magic number` and payload data.

Step 4: Store the checksum at the end of the EEPROM record.

```cpp
eeprom_data.check_sum = calculated_checksum;
```

Step 5: Write the complete record to EEPROM.

| Write target | EEPROM address | Data wrapper |
|---|---:|---|
| Score data | `EEPROM_SCORE_START_ADDR` | `Magic number` + `ar_game_score_t` + `Checksum` |
| Game setting | `EEPROM_SETTING_START_ADDR` | `Magic number` + `ar_game_setting_t` + `Checksum` |

## VI. Related APIs

```cpp
extern bool ar_game_score_read(ar_game_score_t* data);
extern bool ar_game_score_write(ar_game_score_t* data);

extern bool ar_game_setting_read(ar_game_setting_t* data);
extern bool ar_game_setting_write(ar_game_setting_t* data);
```

## VII. Code References

| Area | File |
|---|---|
| EEPROM address and data type definitions | `application/sources/app/app_eeprom.h` |
| EEPROM read/write implementation | `application/sources/app/app_eeprom.cpp` |
| EEPROM driver wrapper | `application/sources/driver/eeprom/eeprom.cpp` |
| Game setting screen usage | `application/sources/app/screens/scr_game_setting.cpp` |
| Score save flow | `application/sources/app/screens/scr_archery_game.cpp` |
