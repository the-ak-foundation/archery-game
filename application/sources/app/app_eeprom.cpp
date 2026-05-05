#include "app_eeprom.h"

#include "eeprom.h"

typedef struct {
	uint32_t magic_number;
	ar_game_score_t data;
	uint8_t check_sum;
} ar_game_score_eeprom_t;

typedef struct {
	uint32_t magic_number;
	ar_game_setting_t data;
	uint8_t check_sum;
} ar_game_setting_eeprom_t;

#define AR_GAME_SCORE_CHECKSUM_SIZE	  (sizeof(uint32_t) + sizeof(ar_game_score_t))
#define AR_GAME_SETTING_CHECKSUM_SIZE (sizeof(uint32_t) + sizeof(ar_game_setting_t))

static uint8_t ar_game_eeprom_checksum(uint8_t *data, uint32_t size) {
	uint8_t check_sum = 0;

	for (uint32_t i = 0; i < size; i++) {
		check_sum += data[i];
	}

	return check_sum;
}

static void ar_game_eeprom_update_checksum(uint32_t *magic_number, uint8_t *check_sum, uint32_t check_sum_size) {
	*magic_number = AR_GAME_EEPROM_MAGIC_NUMBER;
	*check_sum	  = ar_game_eeprom_checksum((uint8_t *)magic_number, check_sum_size);
}

static bool ar_game_eeprom_is_valid(uint32_t *magic_number, uint8_t check_sum, uint32_t check_sum_size) {
	return (*magic_number == AR_GAME_EEPROM_MAGIC_NUMBER) && (check_sum == ar_game_eeprom_checksum((uint8_t *)magic_number, check_sum_size));
}

void ar_game_score_set_default(ar_game_score_t *data) {
	data->score_now = 0;
	data->score_1st = 0;
	data->score_2nd = 0;
	data->score_3rd = 0;
}

bool ar_game_score_read(ar_game_score_t *data) {
	ar_game_score_eeprom_t eeprom_data;

	uint8_t ret =eeprom_read(EEPROM_SCORE_START_ADDR, (uint8_t *)&eeprom_data, sizeof(eeprom_data));

	if (ret == EEPROM_DRIVER_OK && ar_game_eeprom_is_valid(&eeprom_data.magic_number, eeprom_data.check_sum, AR_GAME_SCORE_CHECKSUM_SIZE)) {
		*data = eeprom_data.data;
    return true;
	}

  ar_game_score_set_default(data);
  return false; 
}

bool ar_game_score_write(ar_game_score_t *data) {
	ar_game_score_eeprom_t eeprom_data;

	eeprom_data.data = *data;
	ar_game_eeprom_update_checksum(&eeprom_data.magic_number, &eeprom_data.check_sum, AR_GAME_SCORE_CHECKSUM_SIZE);

	return eeprom_write(EEPROM_SCORE_START_ADDR, (uint8_t *)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}

void ar_game_setting_set_default(ar_game_setting_t *data) {
	data->silent		  = 0;
	data->num_arrow		  = 5;
	data->arrow_speed	  = 5;
	data->meteoroid_speed = 1;
}

bool ar_game_setting_read(ar_game_setting_t *data) {
	ar_game_setting_eeprom_t eeprom_data;

	uint8_t ret = eeprom_read(EEPROM_SETTING_START_ADDR, (uint8_t *)&eeprom_data, sizeof(eeprom_data));

	if (ret == EEPROM_DRIVER_OK && ar_game_eeprom_is_valid(&eeprom_data.magic_number, eeprom_data.check_sum, AR_GAME_SETTING_CHECKSUM_SIZE)) {
		*data = eeprom_data.data;
		return true;
	}

  ar_game_setting_set_default(data);
  return false;
}

bool ar_game_setting_write(ar_game_setting_t *data) {
	ar_game_setting_eeprom_t eeprom_data;

	eeprom_data.data = *data;
	ar_game_eeprom_update_checksum(&eeprom_data.magic_number, &eeprom_data.check_sum, AR_GAME_SETTING_CHECKSUM_SIZE);

	return eeprom_write(EEPROM_SETTING_START_ADDR, (uint8_t *)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}
