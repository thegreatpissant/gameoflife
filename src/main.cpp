#define SDL_MAIN_USE_CALLBACKS 1

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "life.hpp"
#include "Stats.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define BOARD_WIDTH 250
#define BOARD_HEIGHT 250
life::Board board;

typedef struct {
	SDL_Window *window{};
	SDL_Renderer *renderer{};
	life::Stats appStats{};
	Uint64 lastTime = 0;
	struct Mouse {
		bool down = false;
		int x = 0;
		int y = 0;
		int clickX = 0;
		int clickY = 0;
	} ;
	Mouse mouse;
	bool simulationPaused = false;
	//int boardWidth = BOARD_WIDTH;
	//int boardHeight = BOARD_HEIGHT;
	int boardWidth = WINDOW_WIDTH;
	int boardHeight = WINDOW_HEIGHT;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
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

	board = life::genBoard(appState->boardHeight, appState->boardWidth);
	life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->boardWidth / 2 + 0, appState->boardHeight / 2 + 0, life::ALIVE);
	life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->boardWidth / 2 + 1, appState->boardHeight / 2 + 0, life::ALIVE);
	life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->boardWidth / 2 + 0, appState->boardHeight / 2 + 1, life::ALIVE);
	life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->boardWidth / 2 + 1, appState->boardHeight / 2 + 1, life::ALIVE);
	life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->boardWidth / 2 + 1, appState->boardHeight / 2 + 2, life::ALIVE);
	life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->boardWidth / 2 + 2, appState->boardHeight / 2 + 2, life::ALIVE);
	appState->lastTime = SDL_GetTicks();

	SDL_Log("App initialized");
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	auto *appState = static_cast<AppState *>(appstate);
	auto &appStats = appState->appStats;

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
		if (event->key.scancode == SDL_SCANCODE_Q)
		{
			return SDL_APP_SUCCESS;
		}
	}
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		appState->mouse.down = true;
		appState->mouse.clickX = event->button.x;
		appState->mouse.clickY = event->button.y;
		appState->simulationPaused = true;
	}
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		appState->mouse.down = false;
		appState->simulationPaused = false;
	}
	if (event->type = SDL_EVENT_MOUSE_MOTION) {
		appState->mouse.x = event->button.x;
		appState->mouse.y = event->button.y;
	}

	if(appState->mouse.down) {
		std::cout << "Mouse Button Down:" << std::endl;
		std::cout << "clickX: " << appState->mouse.clickX << "clickY:" << appState->mouse.clickY << std::endl;
		std::cout << "x: " << appState->mouse.x<< "y:" << appState->mouse.y << std::endl;
		life::setCellState(board, appState->boardHeight, appState->boardWidth, appState->mouse.x, appState->mouse.y, life::ALIVE);

	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	auto *appState = static_cast<AppState*>(appstate);
	auto *appStats = &appState->appStats;
	int cellScale = 1;

	// Progress the board forward one step.
	appStats->start(life::ITERATE, SDL_GetTicks());
	if (!appState->simulationPaused) {
		life::iterateBoard(board, appState->boardHeight, appState->boardWidth);
	}
	appStats->stop(life::ITERATE, SDL_GetTicks());

	// Render the board.
	appStats->start(life::RENDER, SDL_GetTicks());
	SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(appState->renderer);
	SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	int cellWidth = WINDOW_WIDTH / appState->boardWidth;
	int cellHeight = WINDOW_HEIGHT / appState->boardHeight;
	for (int i = 0; i < appState->boardHeight; i++) {
		for (int j = 0; j < appState->boardWidth; j++) {
			if ( life::getCellState(board, appState->boardHeight, appState->boardWidth, j, i) == life::ALIVE) {
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
		auto *appState = static_cast<AppState*>(appstate);
		SDL_DestroyRenderer(appState->renderer);
		SDL_DestroyWindow(appState->window);
		delete appState;
	}
}
