#include <stdio.h>
#include <SDL.h>
#include "./constants.h"
#include "./helper.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
bool served = FALSE;

typedef struct Ball {
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	// float width;
	// float height;
	int size;
} Ball;

typedef struct Player {
	int score;
	float yPosition;
} Player;

Ball ball;
Player p1, p2;

int initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		// SDL_WINDOW_BORDERLESS
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return FALSE;
	}

	// renderer = SDL_CreateRenderer(window, -1, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return FALSE;
	}
	return TRUE;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
	case SDL_QUIT: 
		game_is_running = FALSE;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			game_is_running = FALSE;
			break;
		case SDLK_SPACE:
			served = TRUE;
			break;
		}
	}
}

void setup() {
	srand((unsigned int)time(NULL));

	// Pode ser uma função Make ou CreateBall
	ball.x = WINDOW_WIDTH / 2 - ball.size / 2;
	ball.y = WINDOW_HEIGHT / 2 - ball.size / 2;
	ball.xSpeed = BALL_SPEED * (coin_flip() ? 1 : -1);
	ball.ySpeed = BALL_SPEED * (coin_flip() ? 1 : -1);
	ball.size = 10;
	
	// player 1
	p1.score = 0;
	p1.yPosition = WINDOW_HEIGHT / 2;

	// player 2
	p2.score = 0;
	p2.yPosition = WINDOW_HEIGHT / 2;
}

void update() {
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	// Update ball
	if (served) {
		ball.x += delta_time * ball.xSpeed;
		ball.y += delta_time * ball.ySpeed;
	}

	// Collision with walls
	if (ball.x < 0) {
		// ball.xSpeed = fabs(ball.xSpeed);
		p2.score += 1;
		served = FALSE;
		ball.x = WINDOW_WIDTH/2;
		ball.y = WINDOW_HEIGHT/2;	

		char *fmt = "P1 %d  |  P2 %d";
		int len = snprintf(NULL, 0, fmt, p1.score, p2.score);
		char buf[len+1];
		snprintf(buf, len + 1, fmt, p1.score, p2.score); 
		SDL_SetWindowTitle(window, buf);

	}
	if (ball.x > WINDOW_WIDTH - ball.size) {
		// ball.xSpeed = -fabs(ball.xSpeed);
		p1.score += 1;
		served = FALSE;
		ball.x = WINDOW_WIDTH/2;
		ball.y = WINDOW_HEIGHT/2;

		char *fmt = "P1 %d  |  P2 %d";
		int len = snprintf(NULL, 0, fmt, p1.score, p2.score);
		char buf[len+1];
		snprintf(buf, len + 1, fmt, p1.score, p2.score); 
		SDL_SetWindowTitle(window, buf);
	}

	if (ball.y < 0) {
		ball.ySpeed = fabs(ball.ySpeed);
	}
	if (ball.y >= WINDOW_HEIGHT - ball.size) {
		ball.ySpeed = -fabs(ball.ySpeed);
	}

	// Goal
	// if (ball.x < ball.size / 2) {
	// 	p2.score += 1;
	// 	served = FALSE;
	// 	ball.x = WINDOW_WIDTH/2;
	// 	ball.y = WINDOW_HEIGHT/2;
	// }

	// if (ball.x >= WINDOW_WIDTH - ball.size / 2) {
	// 	printf("Here");
	// 	p1.score += 1;
	// 	served = FALSE;
	// 	ball.x = WINDOW_WIDTH/2;
	// 	ball.y = WINDOW_HEIGHT/2;
	// }


	const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
	
	// Player 1
	// if (keyboard_state[SDL_SCANCODE_W] && p1.yPosition - PLAYER_HEIGHT / 2 > 0) {
	// 	p1.yPosition -= PLAYER_SPEED * delta_time;
	// }
	// if (keyboard_state[SDL_SCANCODE_S] && p1.yPosition + PLAYER_HEIGHT / 2 < WINDOW_HEIGHT) {
	// 	p1.yPosition += PLAYER_SPEED * delta_time;
	// }

	// Alternativa
	if (keyboard_state[SDL_SCANCODE_W]) {
		p1.yPosition -= PLAYER_SPEED * delta_time;
	}
	if (keyboard_state[SDL_SCANCODE_S]) {
		p1.yPosition += PLAYER_SPEED * delta_time;
	}
	if (p1.yPosition < PLAYER_HEIGHT/2) {
		p1.yPosition = PLAYER_HEIGHT/2;
	}
	if (p1.yPosition > WINDOW_HEIGHT - PLAYER_HEIGHT/2) {
		p1.yPosition = WINDOW_HEIGHT - PLAYER_HEIGHT/2;
	}


	// Player 2
	// if (keyboard_state[SDL_SCANCODE_UP] && p2.yPosition - PLAYER_HEIGHT / 2 > 0) {
	// 	p2.yPosition -= PLAYER_SPEED * delta_time;
	// }
	// if (keyboard_state[SDL_SCANCODE_DOWN] && p2.yPosition + PLAYER_HEIGHT / 2 < WINDOW_HEIGHT) {
	// 	p2.yPosition += PLAYER_SPEED * delta_time;
	// }

	// Alternativa
	if (keyboard_state[SDL_SCANCODE_UP]) {
		p2.yPosition -= PLAYER_SPEED * delta_time;
	}
	if (keyboard_state[SDL_SCANCODE_DOWN]) {
		p2.yPosition += PLAYER_SPEED * delta_time;
	}
	if (p2.yPosition < PLAYER_HEIGHT/2) {
		p2.yPosition = PLAYER_HEIGHT/2;
	}
	if (p2.yPosition > WINDOW_HEIGHT - PLAYER_HEIGHT/2) {
		p2.yPosition = WINDOW_HEIGHT - PLAYER_HEIGHT/2;
	}

	// Collision
	// if (ball.x < PLAYER_MARGIN && 
	// 	ball.y < p1.yPosition + PLAYER_HEIGHT/2 && 
	// 	ball.y > p1.yPosition - PLAYER_HEIGHT/2) {
	// 	ball.xSpeed *= -1;
	// 	ball.ySpeed *= -1 ;
	// }

	// if (ball.x > WINDOW_WIDTH - PLAYER_MARGIN - ball.size && 
	// 	ball.y < p2.yPosition + PLAYER_HEIGHT/2 && 
	// 	ball.y > p2.yPosition - PLAYER_HEIGHT/2) {
	// 	ball.xSpeed *= -1;
	// 	ball.ySpeed *= -1 ;
	// }

	// Alternative collision - Não gostei que estes rects foram declarados novamente
	SDL_Rect ball_rect = {
		(int)ball.x,
		(int)ball.y,
		(int)ball.size,
		(int)ball.size,
	};

	SDL_Rect p1_rect = {
		(int)PLAYER_MARGIN,
		(int)p1.yPosition - PLAYER_HEIGHT / 2,
		(int)PLAYER_WIDTH,
		(int)PLAYER_HEIGHT,
	};

	SDL_Rect p2_rect = {
		(int)WINDOW_WIDTH - PLAYER_MARGIN - PLAYER_WIDTH,
		(int)p2.yPosition - PLAYER_HEIGHT / 2,
		(int)PLAYER_WIDTH,
		(int)PLAYER_HEIGHT,
	};

	if (SDL_HasIntersection(&ball_rect, &p1_rect)) {
		ball.xSpeed = fabs(ball.xSpeed);
	}

	if (SDL_HasIntersection(&ball_rect, &p2_rect)) {
		ball.xSpeed = -fabs(ball.xSpeed);
	}

                             

	last_frame_time = SDL_GetTicks();

}

void render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Isso pode virar uma função RenderBall
	SDL_Rect ball_rect = {
		(int)ball.x,
		(int)ball.y,
		(int)ball.size,
		(int)ball.size,
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_Rect p1_rect = {
		(int)PLAYER_MARGIN,
		(int)p1.yPosition - PLAYER_HEIGHT / 2,
		(int)PLAYER_WIDTH,
		(int)PLAYER_HEIGHT,
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &p1_rect);

	SDL_Rect p2_rect = {
		(int)WINDOW_WIDTH - PLAYER_MARGIN - PLAYER_WIDTH,
		(int)p2.yPosition - PLAYER_HEIGHT / 2,
		(int)PLAYER_WIDTH,
		(int)PLAYER_HEIGHT,
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &p2_rect);

	SDL_RenderPresent(renderer);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	game_is_running = initialize_window();

	setup();

	while (game_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}


