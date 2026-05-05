#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Setting game */
/*****************************************************************************/
ar_game_setting_t settingdata;
static uint8_t setting_location_chosse;

/*****************************************************************************/
/* View - Setting game */
/*****************************************************************************/
static void view_scr_game_setting();

view_dynamic_t dyn_view_item_game_setting = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_setting
};

view_screen_t scr_game_setting = {
	&dyn_view_item_game_setting,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_setting() {
	// Screen
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	// Icon
	view_render.drawBitmap(	0, \
							setting_location_chosse - \
							AR_GAME_SETTING_CHOSSE_ICON_AXIS_Y, \
							chosse_icon, \
							AR_GAME_SETTING_CHOSSE_ICON_SIZE_W, \
							AR_GAME_SETTING_CHOSSE_ICON_SIZE_H, \
							WHITE);
	if (settingdata.silent == 0) {
		view_render.drawBitmap(	109, 
								AR_GAME_SETTING_FRAMES_AXIS_Y_1 + \
								AR_GAME_SETTING_FRAMES_STEP*3-12, \
								speaker_1, \
								7, \
								7, \
								WHITE);
	}
	else {
		view_render.drawBitmap(	109, \
								AR_GAME_SETTING_FRAMES_AXIS_Y_1 + \
								AR_GAME_SETTING_FRAMES_STEP*3-12, \
								speaker_2, \
								7, \
								7, \
								WHITE);
	}
	// Frames
	view_render.drawRoundRect(	AR_GAME_SETTING_FRAMES_AXIS_X, \
								AR_GAME_SETTING_FRAMES_AXIS_Y_1, \
								AR_GAME_SETTING_FRAMES_SIZE_W, \
								AR_GAME_SETTING_FRAMES_SIZE_H, \
								AR_GAME_SETTING_FRAMES_SIZE_R, \
								WHITE);
	view_render.drawRoundRect(	AR_GAME_SETTING_FRAMES_AXIS_X, \
								AR_GAME_SETTING_FRAMES_AXIS_Y_1 + \
								AR_GAME_SETTING_FRAMES_STEP, \
								AR_GAME_SETTING_FRAMES_SIZE_W, \
								AR_GAME_SETTING_FRAMES_SIZE_H, \
								AR_GAME_SETTING_FRAMES_SIZE_R, \
								WHITE);
	view_render.drawRoundRect(	AR_GAME_SETTING_FRAMES_AXIS_X, \
								AR_GAME_SETTING_FRAMES_AXIS_Y_1 + \
								AR_GAME_SETTING_FRAMES_STEP*2, \
								AR_GAME_SETTING_FRAMES_SIZE_W, \
								AR_GAME_SETTING_FRAMES_SIZE_H, \
								AR_GAME_SETTING_FRAMES_SIZE_R, \
								WHITE);
	view_render.drawRoundRect(	AR_GAME_SETTING_FRAMES_AXIS_X, \
								AR_GAME_SETTING_FRAMES_AXIS_Y_1 + \
								AR_GAME_SETTING_FRAMES_STEP*3, \
								AR_GAME_SETTING_FRAMES_SIZE_W, \
								AR_GAME_SETTING_FRAMES_SIZE_H, \
								AR_GAME_SETTING_FRAMES_SIZE_R, \
								WHITE);
	// Count Arrow
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 5);
	view_render.print(" Arrows       ( ) ");
	view_render.setCursor(AR_GAME_SETTING_NUMBER_AXIS_X, 5);
	view_render.print(settingdata.num_arrow);    
	// Mine speed
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 20);
	view_render.print(" Meteoroid sp ( ) ");	
	view_render.setCursor(AR_GAME_SETTING_NUMBER_AXIS_X, 20);
	view_render.print(settingdata.meteoroid_speed);
	// Silent
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 35);
	view_render.print(" Silent           ");
	// EXIT
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X + 32, 50);
	view_render.print(" EXIT ") ;
	view_render.update();
}

/*****************************************************************************/
/* Handle - Setting game */
/*****************************************************************************/
void scr_game_setting_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		// Clear view
		view_render.clear();
		// Chosse item arrdess 1
		setting_location_chosse = SETTING_ITEM_ARRDESS_1;
		// Read setting data
		ar_game_setting_read(&settingdata);
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		// Change setting data
		switch (setting_location_chosse) {
		case SETTING_ITEM_ARRDESS_1: {
			// Change arrow number
			settingdata.num_arrow++;
			if (settingdata.num_arrow > MAX_NUM_ARROW) {
				settingdata.num_arrow = 1;
			}
		}
			break;

		case SETTING_ITEM_ARRDESS_2: {
			settingdata.meteoroid_speed++;
			if (settingdata.meteoroid_speed > 5) { 
				settingdata.meteoroid_speed = 1;
			}
		}
			break;

		case SETTING_ITEM_ARRDESS_3: {
			// Change meteoroid speed
			settingdata.silent = !settingdata.silent;
			BUZZER_Sleep(settingdata.silent);
		}
			break;

		case SETTING_ITEM_ARRDESS_4: {
			// Save change and exit
			settingdata.arrow_speed = 5;
			ar_game_setting_write(&settingdata);
			SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
			BUZZER_PlayTones(tones_startup);
		}
			break;

		default:
			break;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;
	
	case AC_DISPLAY_BUTTON_UP_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_LONG_PRESSED\n");
		// Change data max
		settingdata.num_arrow = MAX_NUM_ARROW;
		settingdata.meteoroid_speed = 5;
		settingdata.silent = 0;
	}
		BUZZER_Sleep(settingdata.silent);
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		// Move up
		setting_location_chosse -= STEP_SETTING_CHOSSE;
		if (setting_location_chosse == SETTING_ITEM_ARRDESS_0) { 
			setting_location_chosse = SETTING_ITEM_ARRDESS_4;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED\n");
		// Change data min
		settingdata.num_arrow = 1;
		settingdata.meteoroid_speed = 1;
		settingdata.silent = 1;
	}
		BUZZER_Sleep(settingdata.silent);
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		// Move down
		setting_location_chosse += STEP_SETTING_CHOSSE;
		if (setting_location_chosse > SETTING_ITEM_ARRDESS_4) { 
			setting_location_chosse = SETTING_ITEM_ARRDESS_1;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}