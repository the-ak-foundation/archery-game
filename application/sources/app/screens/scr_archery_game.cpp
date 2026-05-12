#include "scr_archery_game.h"

#include <stdlib.h>

#include "screens.h"

/*****************************************************************************/
/* Variable Declaration - Archery game screen */
/*****************************************************************************/
#define AR_GAME_COMMAND_TEXT_MAX_LEN		(10)
#define AR_GAME_HIGHSCORE_SCORE				(300)
#define AR_GAME_ARRAY_SIZE(arr)				(sizeof(arr) / sizeof((arr)[0]))

uint8_t ar_game_state; 
ar_game_setting_t settingsetup;
static uint8_t ar_game_over_text_index = 0;

static const char* ar_game_over_text = " Too Bad!";
static const char* const ar_game_praise_text[] = {
    "Excellent",  // 9
    "Good Job!",  // 9
    "  Great!",   // 8
    " Amazing!",  // 9
    " Winner!"    // 8
};

static const char* const ar_game_blame_text[] = {
    " Too Bad!",  // 9
    " Try More",  // 9
    " Missed!",   // 8
    "  Oops!",    // 6
    " So Close"   // 9
};

static const char* ar_game_random_text(const char* const* text_list, uint8_t text_count) {
	return text_list[rand() % text_count];
}

static void ar_game_print_text_partial(const char* text, uint8_t max_chars) {
	if (text == NULL) return;
	for (uint8_t i = 0; i < max_chars && text[i] != '\0'; i++) {
		view_render.print(text[i]);
	}
}

/*****************************************************************************/
/* View - Archery game screen*/
/*****************************************************************************/
void ar_game_frame_display() {
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(2,55);
	view_render.print("Arrow:");
	view_render.print(settingsetup.num_arrow);
	view_render.setCursor(60,55);
	view_render.print(" Score:");
	view_render.print(ar_game_score);
	view_render.drawLine(0, LCD_HEIGHT, 	LCD_WIDTH, LCD_HEIGHT,		WHITE);
	view_render.drawLine(0, LCD_HEIGHT-10, 	LCD_WIDTH, LCD_HEIGHT-10,	WHITE);
	view_render.drawRect(0, 0, 128, 64, 1);
}

void ar_game_archery_display() {
	if (archery.visible == WHITE) {
		if (archery.action_image == AR_GAME_ARCHERY_ACTION_IMAGE_1) {
			view_render.drawBitmap(	archery.x, \
					archery.y - 10, \
					bitmap_archery_I, \
					SIZE_BITMAP_ARCHERY_X, \
					SIZE_BITMAP_ARCHERY_Y, \
					WHITE);
		}
		else if (archery.action_image == AR_GAME_ARCHERY_ACTION_IMAGE_2) {
			view_render.drawBitmap(	archery.x, \
					archery.y - 10, \
					bitmap_archery_II, \
					SIZE_BITMAP_ARCHERY_X, \
					SIZE_BITMAP_ARCHERY_Y, \
					WHITE);
		}
	}
}

void ar_game_arrow_display() {
	for (uint8_t i = 0; i < MAX_NUM_ARROW; i++) {
		if (arrow[i].visible == WHITE) {
			view_render.drawBitmap(	arrow[i].x, \
									arrow[i].y, \
									bitmap_arrow, \
									SIZE_BITMAP_ARROW_X, \
									SIZE_BITMAP_ARROW_Y, \
									WHITE);
		}
	}
}

void ar_game_meteoroid_display() {
	for (uint8_t i = 0; i < NUM_METEOROIDS; i++) {
		if (meteoroid[i].visible == WHITE) {
			if (meteoroid[i].action_image == AR_GAME_METEOROID_ACTION_IMAGE_1) {
				view_render.drawBitmap(	meteoroid[i].x, \
										meteoroid[i].y, \
										bitmap_meteoroid_I, \
										SIZE_BITMAP_METEOROIDS_X, \
										SIZE_BITMAP_METEOROIDS_Y, \
				 						WHITE);
			}
			else if (meteoroid[i].action_image == AR_GAME_METEOROID_ACTION_IMAGE_2) {
				view_render.drawBitmap(	meteoroid[i].x, \
										meteoroid[i].y, \
										bitmap_meteoroid_II, \
										SIZE_BITMAP_METEOROIDS_X, \
										SIZE_BITMAP_METEOROIDS_Y, \
										WHITE);
			}
			else if (meteoroid[i].action_image == AR_GAME_METEOROID_ACTION_IMAGE_3) {
				view_render.drawBitmap(	meteoroid[i].x, \
										meteoroid[i].y, \
										bitmap_meteoroid_III, \
										SIZE_BITMAP_METEOROIDS_X, \
										SIZE_BITMAP_METEOROIDS_Y, \
				 						WHITE);
			}
		}
	}
}

void ar_game_bang_display() {
	for (uint8_t i = 0; i < NUM_BANG; i++) {
		if (bang[i].visible == WHITE) {
			if (bang[i].action_image == AR_GAME_BANG_ACTION_IMAGE_1) {
				view_render.drawBitmap(	bang[i].x, \
										bang[i].y, \
										bitmap_bang_I, \
										SIZE_BITMAP_BANG_I_X, \
										SIZE_BITMAP_BANG_I_Y, \
										WHITE);
			}
			else if (bang[i].action_image == AR_GAME_BANG_ACTION_IMAGE_2) {
				view_render.drawBitmap(	bang[i].x, \
										bang[i].y, \
										bitmap_bang_II, \
										SIZE_BITMAP_BANG_I_X, \
										SIZE_BITMAP_BANG_I_Y, \
				 						WHITE);
			}
			else if (bang[i].action_image == AR_GAME_BANG_ACTION_IMAGE_3) {
				view_render.drawBitmap( bang[i].x + 2, \
										bang[i].y - 1, \
										bitmap_bang_III, \
										SIZE_BITMAP_BANG_II_X, \
										SIZE_BITMAP_BANG_II_Y, \
				 						WHITE);
			}
		}
	}
}

void ar_game_border_display() {
	if (border.visible == WHITE) {
		view_render.drawFastVLine(	border.x, \
									AXIS_Y_BORDER_ON, \
									AXIS_Y_BORDER_UNDER, \
									WHITE);
		for (uint8_t i = 0; i < NUM_METEOROIDS; i++) {
			view_render.fillCircle(	border.x, \
									meteoroid[i].y + 5, \
									1, \
									WHITE);
		}
	}
}

static void view_scr_archery_game();

view_dynamic_t dyn_view_item_archery_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_archery_game
};

view_screen_t scr_archery_game = {
	&dyn_view_item_archery_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_archery_game() {
	if (ar_game_state == GAME_PLAY) {
		ar_game_frame_display();
		ar_game_archery_display();
		ar_game_arrow_display();
		ar_game_meteoroid_display();
		ar_game_bang_display();
		ar_game_border_display();
	}
	else if (ar_game_state == GAME_OVER) {
		view_render.clear();
		view_render.drawBitmap(	0, \
			0, \
			bitmap_dolphin, \
			119, \
			62, \
			WHITE);
		
		view_render.setTextSize(1);
		view_render.setTextColor(WHITE);
		view_render.setCursor(74, 15);
		ar_game_print_text_partial(ar_game_over_text, ar_game_over_text_index);
	}
}

/*****************************************************************************/
/* Handle - Archery game screen */
/*****************************************************************************/
void rank_ranking() {
	if (gamescore.score_now > gamescore.score_1st) {
		gamescore.score_3rd = gamescore.score_2nd;
		gamescore.score_2nd = gamescore.score_1st;
		gamescore.score_1st = gamescore.score_now;
	}
	else if (gamescore.score_now > gamescore.score_2nd) {
		gamescore.score_3rd = gamescore.score_2nd;
		gamescore.score_2nd = gamescore.score_now;
	}
	else if (gamescore.score_now > gamescore.score_3rd) {
		gamescore.score_3rd = gamescore.score_now;
	}
}

void scr_archery_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		// Get setting data
		ar_game_setting_read(&settingsetup);

		// Setup game Object
		task_post_pure_msg(AR_GAME_ARCHERY_ID, 	 	AR_GAME_ARCHERY_SETUP);
		task_post_pure_msg(AR_GAME_ARROW_ID, 	 	AR_GAME_ARROW_SETUP);
		task_post_pure_msg(AR_GAME_METEOROID_ID, 	AR_GAME_METEOROID_SETUP);
		task_post_pure_msg(AR_GAME_BANG_ID, 	 	AR_GAME_BANG_SETUP);
		task_post_pure_msg(AR_GAME_BORDER_ID, 	 	AR_GAME_BORDER_SETUP);
		
		// Setup timer
		timer_set(	AC_TASK_DISPLAY_ID, \
					AR_GAME_TIME_TICK, \
					AR_GAME_TIME_TICK_INTERVAL, \
					TIMER_PERIODIC);
		// State update
		ar_game_state = GAME_PLAY;
	} break;

	case AR_GAME_TIME_TICK: {
		APP_DBG_SIG("AR_GAME_TIME_TICK\n");
		// Time tick
		task_post_pure_msg(AR_GAME_ARCHERY_ID, 		AR_GAME_ARCHERY_UPDATE);
		task_post_pure_msg(AR_GAME_ARROW_ID, 		AR_GAME_ARROW_RUN);
		task_post_pure_msg(AR_GAME_METEOROID_ID, 	AR_GAME_METEOROID_RUN);
		task_post_pure_msg(AR_GAME_METEOROID_ID, 	AR_GAME_METEOROID_DETONATOR);
		task_post_pure_msg(AR_GAME_BANG_ID, 		AR_GAME_BANG_UPDATE);
		task_post_pure_msg(AR_GAME_BORDER_ID, 		AR_GAME_LEVEL_UP);
		task_post_pure_msg(AR_GAME_BORDER_ID, 		AR_GAME_CHECK_GAME_OVER);
	} break;

	case AR_GAME_RESET: {
		APP_DBG_SIG("AR_GAME_RESET\n");
   
		// Stop timer tick
		timer_remove_attr(AC_TASK_DISPLAY_ID, AR_GAME_TIME_TICK);

		// Reset game Object
		task_post_pure_msg(AR_GAME_ARCHERY_ID, 		AR_GAME_ARCHERY_RESET);
		task_post_pure_msg(AR_GAME_ARROW_ID, 		AR_GAME_ARROW_RESET);
		task_post_pure_msg(AR_GAME_METEOROID_ID,	AR_GAME_METEOROID_RESET);
		task_post_pure_msg(AR_GAME_BANG_ID, 		AR_GAME_BANG_RESET);
		task_post_pure_msg(AR_GAME_BORDER_ID, 		AR_GAME_BORDER_RESET);

		// Reset text animation index
		ar_game_over_text_index = 0;

		// Timer Exit
		timer_set(	AC_TASK_DISPLAY_ID, \
					AR_GAME_EXIT_GAME, \
					AR_GAME_TIME_EXIT_INTERVAL, \
					TIMER_ONE_SHOT);

		// Save and reset Score
		ar_game_score_read(&gamescore);
		gamescore.score_now = ar_game_score;
		rank_ranking();
		ar_game_score_write(&gamescore);
		ar_game_score = 10;

		// Setup text animation timer
		timer_set(	AC_TASK_DISPLAY_ID, \
					AR_GAME_OVER_TEXT_ANIM_TICK, \
					AR_GAME_OVER_TEXT_ANIM_TICK_INTERVAL, \
					TIMER_PERIODIC);

		if (gamescore.score_now > AR_GAME_HIGHSCORE_SCORE) {
			ar_game_over_text = ar_game_random_text(ar_game_praise_text, \
													AR_GAME_ARRAY_SIZE(ar_game_praise_text));
			BUZZER_PlaySound(BUZZER_SOUND_HIGHSCORE);
		}
		else {
			ar_game_over_text = ar_game_random_text(ar_game_blame_text, \
													AR_GAME_ARRAY_SIZE(ar_game_blame_text));
			BUZZER_PlaySound(BUZZER_SOUND_LOWSCORE);
		}

		// State update
		ar_game_state = GAME_OVER;
	} break;

	case AR_GAME_OVER_TEXT_ANIM_TICK: {
		APP_DBG_SIG("AR_GAME_OVER_TEXT_ANIM_TICK\n");
		if (ar_game_over_text_index < AR_GAME_COMMAND_TEXT_MAX_LEN) {
			ar_game_over_text_index++;
		}
		else {
			timer_remove_attr(AC_TASK_DISPLAY_ID, AR_GAME_OVER_TEXT_ANIM_TICK);
		}
	} break;

	case AR_GAME_EXIT_GAME: {
		APP_DBG_SIG("AR_GAME_EXIT_GAME\n");

		// Stop text animation timer
		timer_remove_attr(AC_TASK_DISPLAY_ID, AR_GAME_OVER_TEXT_ANIM_TICK);

		// State update
		ar_game_state = GAME_OFF;
		// Change the screen
		SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
	} break;

	default:
		break;
	}
}
