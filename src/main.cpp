#define SDL_MAIN_USE_CALLBACKS 1

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "life.hpp"
#include "Stats.hpp"

#define WINDOW_WIDTH 100
#define WINDOW_HEIGHT 100

life::Board board;

typedef struct {
	SDL_Window *window{};
	SDL_Renderer *renderer{};
	life::Stats appStats{};
	Uint64 lastTime = 0;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	SDL_Log("width:%d, height:%d", WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Could not initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	AppState *appState = nullptr;
	try
	{
		appState = new AppState();
	} catch (const std::bad_alloc& e) {
		SDL_Log("Could not allocate app state: %s", e.what());
		return SDL_APP_FAILURE;
	}
	*appstate = appState;

	if (!SDL_CreateWindowAndRenderer("MySDLApp3", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &appState->window, &appState->renderer)) {
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
	appState->lastTime = SDL_GetTicks();

	SDL_Log("App initialized");
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	auto *app_state = static_cast<AppState *>(appstate);
	auto &appStats = app_state->appStats;

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
			appStats.print();
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	auto *appState = static_cast<AppState*>(appstate);
	auto *appStats = &appState->appStats;

	// Progress the board forward one step.
	appStats->start(life::ITERATE, SDL_GetTicks());
	life::iterateBoard(board, WINDOW_HEIGHT, WINDOW_WIDTH);
	appStats->stop(life::ITERATE, SDL_GetTicks());

	// Render the board.
	appStats->start(life::RENDER, SDL_GetTicks());
	SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(appState->renderer);
	SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < WINDOW_HEIGHT; i++) {
		for (int j = 0; j < WINDOW_WIDTH; j++) {
			if ( board[i * WINDOW_WIDTH + j] == 1) {
				SDL_Log("Render point");
				if (!SDL_RenderPoint(appState->renderer, static_cast<float>(j), static_cast<float>(i))) {
					SDL_Log("Could not render point: %s", SDL_GetError());
					return SDL_APP_FAILURE;
				}
			}	
		}
	}
	appStats->stop(life::RENDER, SDL_GetTicks());

	appStats->start(life::PRESENT, SDL_GetTicks());
	SDL_RenderPresent(appState->renderer);
	appStats->stop(life::PRESENT, SDL_GetTicks());

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	if (appstate != nullptr) {
		auto *app_state = static_cast<AppState*>(appstate);
		SDL_DestroyRenderer(app_state->renderer);
		SDL_DestroyWindow(app_state->window);
		delete app_state;
	}
}