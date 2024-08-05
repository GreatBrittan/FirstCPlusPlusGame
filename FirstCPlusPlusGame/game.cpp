#include <cmath>
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down &&  input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down &&  input->buttons[b].changed)



float player_pos_x = 0.f;
float player_pos_y = 0.f;

int direction = 0; // 0 is UP, 1, is LEFT, 2 is DOWN, 3 is RIGHT


float enemySize = 3.0f;
float playerSize = 1.0f;
float enemies_x[5];
float enemies_y[5];
float spawn_timer = 0.0f;
const float spawn_interval = 1.0f; // Time interval for spawning enemies in seconds
int total_enemies = 0;
const int max_enemies = 5;

const float attack_distance = 3.0f + enemySize + playerSize; // Define the attack distance
const float attack_line_length = 3.0f; // Length of the attack line

internal void
spawn_enemy() {
	if (total_enemies < max_enemies) {
		// Set the position of the new enemy
		enemies_x[total_enemies] = ((float(rand()) / RAND_MAX) * (render_state.width - 1)); // Set initial x position
		enemies_y[total_enemies] = ((float(rand()) / RAND_MAX) * (render_state.height - 1)); // Set initial y position
		total_enemies++;
	}
}

internal void
playerAttack() {
	int i = 0;
	float increment = attack_line_length / 10;

	// Upwards Attack
	if (direction == 0) {
		float attack_x_start = player_pos_x;
			float attack_x_end = player_pos_x;
		float attack_y_start = player_pos_y; 
		float attack_y_end = player_pos_y + attack_line_length;

		for (int j = 0; j < 11; j++) {
			draw_rect(attack_x_start, attack_y_start + (attack_line_length), increment, attack_line_length, 0xFF0000); // Red line
		}
		
		
	}
	// Left Attack
	if (direction == 1) {
		float attack_x_start = player_pos_x;
		float attack_x_end = player_pos_x - attack_line_length;
		float attack_y_start = player_pos_y;
		float attack_y_end = player_pos_y;

		for (int j = 0; j < 11; j++) {
			draw_rect(attack_x_start - (attack_line_length), attack_y_start, attack_line_length, increment, 0xFF0000); // Red line
		}


	}
	// Down Attack
	if (direction == 2) {
		float attack_x_start = player_pos_x;
		float attack_x_end = player_pos_x;
		float attack_y_start = player_pos_y;
		float attack_y_end = player_pos_y - attack_line_length;

		for (int j = 0; j < 11; j++) {
			draw_rect(attack_x_start, attack_y_start - (attack_line_length), increment, attack_line_length, 0xFF0000); // Red line
		}


	}
	// Right Attack
	if (direction == 3) {
		float attack_x_start = player_pos_x;
		float attack_x_end = player_pos_x + attack_line_length;
		float attack_y_start = player_pos_y;
		float attack_y_end = player_pos_y;

		for (int j = 0; j < 11; j++) {
			draw_rect(attack_x_start + (attack_line_length), attack_y_start , attack_line_length, increment, 0xFF0000); // Red line
		}

	}
	// Check for enemies within the attack distance
	while (i < total_enemies) {
		// Calculate the distance between the player and the enemy
		float dx = enemies_x[i] - player_pos_x;
		float dy = enemies_y[i] - player_pos_y;
		bool is_in_attack_range = false;

		// Check if the enemy is in the attack range based on direction
		switch (direction) {
		case 0:
			is_in_attack_range = (dy >= 0 && dy <= attack_distance  && abs(dx) <= attack_distance);
			break;
		case 2:
			is_in_attack_range = (dy <= 0 && dy >= -attack_distance && abs(dx) <= attack_distance);
			break;
		case 1:
			is_in_attack_range = (dx <= 0 && dx >= -attack_distance && abs(dy) <= attack_distance);
			break;
		case 3:
			is_in_attack_range = (dx >= 0 && dx <= attack_distance && abs(dy) <= attack_distance);
			break;
		}

		// If the enemy is in the attack range, remove it
		if (is_in_attack_range) {
			// Remove the enemy by shifting the remaining enemies
			for (int j = i; j < total_enemies - 1; j++) {
				enemies_x[j] = enemies_x[j + 1];
				enemies_y[j] = enemies_y[j + 1];
			}
			total_enemies--; // Decrease the total number of enemies
		}
		else {
			i++; // Move to the next enemy if the current one is not removed
		}
	}
}


internal void
simulate_game(Input* input, float dt) {
	
	clear_screen(0x7CFC00);

	float speed = 50.f; // units per second
	if (is_down(BUTTON_UP)) {
		player_pos_y += speed * dt;
		direction = 0;
	}
	if (is_down(BUTTON_DOWN)) {
		player_pos_y -= speed * dt;
		direction = 2;
	}
	if (is_down(BUTTON_RIGHT)) {
		player_pos_x += speed * dt;
		direction = 3;
	}
	if (is_down(BUTTON_LEFT)) {
		player_pos_x -= speed * dt;
		direction = 1;
	}
	if (is_down(BUTTON_SPACEBAR)) playerAttack();

	
	spawn_timer += 2*dt;
	if (spawn_timer >= spawn_interval && total_enemies < max_enemies) {
		spawn_enemy();
		// Reset the spawn timer
		spawn_timer = 0.0f;
	}

	if (total_enemies > 0) {
		for (int i = 0; i < total_enemies; i++) {
			enemies_x[i] = enemies_x[i] + clamp(-0.03f, (player_pos_x - enemies_x[i]), 0.03f);
			enemies_y[i] = enemies_y[i] + clamp(-0.03f, (player_pos_y - enemies_y[i]), 0.03f);
			draw_rect(enemies_x[i], enemies_y[i], enemySize, enemySize, 0x000000);
		}
	}
	draw_rect(player_pos_x, player_pos_y, playerSize, playerSize, 0x964B00);
	

	for (int i = 0; i < total_enemies; i++) {
		if (abs(enemies_x[i] - player_pos_x) < 3.5 && abs(enemies_y[i] - player_pos_y) < 3.5) {
			clear_screen(0xffffff);
			running = false;
		}
	}
}
