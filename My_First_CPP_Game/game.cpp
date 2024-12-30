#include "utils.h"
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define pressed(b) (!input->buttons[b].is_down && input->buttons[b].changed)

void save_score(int player_1_score, int player_2_score);
void load_score(int& player_1_score, int& player_2_score);
void save_highscore(int highscore);
void load_highscore(int& highscore);
void update_highscore(int score_1, int score_2);


float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;
int loaded_player_1_score, loaded_player_2_score;
int high_score;

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

static void simulate_game(Input* input, float dt) {
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);

	if (current_gamemode == GM_GAMEPLAY) {
		float player_1_ddp = 0.f;
		float player_2_ddp = 0.f;
		load_highscore(high_score);

		if (enemy_is_ai) {
			if (is_down(BUTTON_UP)) player_1_ddp += 2000;
			if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
		}
		else {
		//if (ball_p_y > player_1_p) player_1_ddp += 1300;
		//if (ball_p_y < player_1_p) player_1_ddp -= 1300;
		player_1_ddp = (ball_p_y - player_1_p) * 100;
		if (player_1_ddp > 1300) player_1_ddp = 1300;
		if (player_1_ddp < -1300) player_1_ddp = -1300;
		}

		if (is_down(BUTTON_W)) player_2_ddp += 2000;
		if (is_down(BUTTON_S)) player_2_ddp -= 2000;

		//Simulate player movement
		{
			player_1_ddp -= player_1_dp * 10.f;

			player_1_p = player_1_p + player_1_dp * dt + player_1_ddp * dt * dt * .5f;
			player_1_dp = player_1_dp + player_1_ddp * dt;

			if (player_1_p + player_half_size_y > arena_half_size_y) {
				player_1_p = arena_half_size_y - player_half_size_y;
				player_1_dp *= -1;
			}
			else if (player_1_p - player_half_size_y < -arena_half_size_y) {
				player_1_p = -arena_half_size_y + player_half_size_y;
				player_1_dp *= -1;
			}

			player_2_ddp -= player_2_dp * 10.f;

			player_2_p = player_2_p + player_2_dp * dt + player_2_ddp * dt * dt * .5f;
			player_2_dp = player_2_dp + player_2_ddp * dt;

			if (player_2_p + player_half_size_y > arena_half_size_y) {
				player_2_p = arena_half_size_y - player_half_size_y;
				player_2_dp *= -0.5;
			}
			else if (player_2_p - player_half_size_y < -arena_half_size_y) {
				player_2_p = -arena_half_size_y + player_half_size_y;
				player_2_dp *= -0.5;
			}
		}

		//Simulate ball movement
		{
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;


			if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
				ball_p_x - ball_half_size < 80 + player_half_size_x &&
				ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
				ball_p_y - ball_half_size < player_1_p + player_half_size_y) {
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
			}
			else if (ball_p_x - ball_half_size < -80 + player_half_size_x &&
				ball_p_x + ball_half_size > -80 - player_half_size_x &&
				ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
				ball_p_y - ball_half_size < player_2_p + player_half_size_y) {
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
			}

			if (ball_p_y + ball_half_size > arena_half_size_y) {
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}

			if (ball_p_x + ball_half_size > arena_half_size_x) {
				ball_p_x = arena_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_p_x = -arena_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_2_score++;
			}
		}

		draw_number(player_1_score, -10, 40, 1.0f, 0xffffff);
		draw_number(player_2_score, 10, 40, 1.0f, 0xffffff);
		//Rendering
		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000);

		draw_text("BACK TO MENU WITH ESC", -85, 47.5, .25f, 0xffffff);
		draw_text("HIGHSCORE", 65, 47.5, .25f, 0xffffff);
		draw_number(high_score, 81.5, 46.8, .4f, 0xffffff);

		if (pressed(BUTTON_ESCAPE)) {
			load_score(loaded_player_1_score, loaded_player_2_score);
			load_highscore(high_score);
			if (player_1_score > loaded_player_1_score) {
				save_score(player_1_score, player_2_score);
			}
			else if (player_2_score > loaded_player_2_score) {
				save_score(player_1_score, player_2_score);
			}
			update_highscore(player_1_score, player_2_score);
			player_1_score = 0;
			player_2_score = 0;
			current_gamemode = GM_MENU;
		}
	}
 else {

	 if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
		 hot_button = !hot_button;
	 }

	 if (pressed(BUTTON_ENTER)) {
		 current_gamemode = GM_GAMEPLAY;
		 enemy_is_ai = hot_button ? 1 : 0;
	 }

	 if (hot_button == 0) {
		 draw_text("SINGLE PLAYER", -80, -10, 1, 0xff0000);
		 draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
	 }
	 else {
		 draw_text("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
		 draw_text("MULTIPLAYER", 20, -10, 1, 0xff0000);
	 }
	 draw_text("EXIT WITH ESC", -85, 47.5, .25, 0xffffff);
	 if (pressed(BUTTON_ESCAPE)) {
		 running = 0;
	 }
	 draw_text("PONG", -20, 40, 2, 0xffffff);
	 draw_text("BY STRAUT ANDREI EUGEN", -50, 22, .75, 0xffffff);
	}
}

