#define SDL_MAIN_USE_CALLBACKS 1

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "life.hpp"
#include "stats.hpp"

static SDL_Window *window;
static SDL_Renderer *renderer;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

life::Board board;

static Uint64 last_time = 0;
static SDL_FPoint points[life::LIFE_BOARD_HEIGHT * life::LIFE_BOARD_WIDTH];

// global stats object
life::stats gameStats;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	SDL_Log("width:%d, height:%d", WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Could not initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("MySDLApp3", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		SDL_Log("Could not create window: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	board = life::genBoard(WINDOW_HEIGHT, WINDOW_WIDTH);
    life::setCellState(board, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_WIDTH / 2 + 0, WINDOW_HEIGHT / 2 + 0, 1);
    life::setCellState(board, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_WIDTH / 2 + 1, WINDOW_HEIGHT / 2 + 0, 1);
    life::setCellState(board, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_WIDTH / 2 + 0, WINDOW_HEIGHT / 2 + 1, 1);
    life::setCellState(board, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_WIDTH / 2 + 1, WINDOW_HEIGHT / 2 + 1, 1);
    life::setCellState(board, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_WIDTH / 2 + 1, WINDOW_HEIGHT / 2 + 2, 1);
    life::setCellState(board, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_WIDTH / 2 + 2, WINDOW_HEIGHT / 2 + 2, 1);
	last_time = SDL_GetTicks();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	if (event->type == SDL_EVENT_QUIT) 
	{
		return SDL_APP_SUCCESS;
	}
	//  print on s key
	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		//  if "S" is pressed execute stats
		if (event->key.scancode == SDL_SCANCODE_S)
		{
			gameStats.print();
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	// Progress the board forward one step.
	gameStats.start(life::ITERATE, SDL_GetTicks());
	life::iterateBoard(board, WINDOW_HEIGHT, WINDOW_WIDTH);
	gameStats.stop(life::ITERATE, SDL_GetTicks());

	// Render the board.
	gameStats.start(life::RENDER, SDL_GetTicks());
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < WINDOW_HEIGHT; i++) {
		for (int j = 0; j < WINDOW_WIDTH; j++) {
			if ( board[i * WINDOW_WIDTH + j] == 1) {
				SDL_Log("Render point");
				if (!SDL_RenderPoint(renderer, static_cast<float>(j), static_cast<float>(i))) {
					SDL_Log("Could not render point: %s", SDL_GetError());
					return SDL_APP_FAILURE;
				}
			}	
		}
	}
	gameStats.stop(life::RENDER, SDL_GetTicks());

	gameStats.start(life::PRESENT, SDL_GetTicks());
	SDL_RenderPresent(renderer);
	gameStats.stop(life::PRESENT, SDL_GetTicks());

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}