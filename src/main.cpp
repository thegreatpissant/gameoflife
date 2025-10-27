#define SDL_MAIN_USE_CALLBACKS 1

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "Stats.hpp"
#include "life.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define INITIAL_BOARD_WIDTH 500
#define INITIAL_BOARD_HEIGHT 500
life::GameBoard gameBoard;

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
  };

  Mouse mouse;
  bool simulationPaused = false;
  int boardWidth = INITIAL_BOARD_WIDTH;
  int boardHeight = INITIAL_BOARD_HEIGHT;
  bool vsyncState = true;
} AppState;

bool setVSync(AppState *appState) {
  if (appState->vsyncState && !SDL_SetRenderVSync(appState->renderer, 1)) {
    SDL_Log("Could not enable VSync: %s", SDL_GetError());
    return false;
  }
  if (!appState->vsyncState && !SDL_SetRenderVSync(appState->renderer, 0)) {
    SDL_Log("Could not disable VSync: %s", SDL_GetError());
    return false;
  }
  return true;
}

void zapBoard(AppState *appState){
  // Init the board state with random cells
  for (int y = 0; y < appState->boardHeight; y++) {
    for (int x = 0; x < appState->boardWidth; x++) {
      if (rand() % 2) {
        life::setCellState(gameBoard, appState->boardHeight, appState->boardWidth,
                           x, y, life::ALIVE);
      }
    }
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Could not initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  AppState *appState = nullptr;
  try {
    appState = new AppState();
  } catch (const std::bad_alloc &e) {
    SDL_Log("Could not allocate app state: %s", e.what());
    return SDL_APP_FAILURE;
  }
  *appstate = appState;

  if (!SDL_CreateWindowAndRenderer("MySDLApp3", WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                                   &appState->window, &appState->renderer)) {
    SDL_Log("Could not create window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  setVSync(appState);

  gameBoard = life::genBoard(appState->boardHeight, appState->boardWidth);
  appState->lastTime = SDL_GetTicks();

  SDL_Log("App initialized");
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  auto *appState = static_cast<AppState *>(appstate);
  auto &appStats = appState->appStats;

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  //  print on s key
  if (event->type == SDL_EVENT_KEY_DOWN) {
    //  if "S" is pressed execute stats
    if (event->key.scancode == SDL_SCANCODE_S) {
      appStats.print();
    }
    if (event->key.scancode == SDL_SCANCODE_Q ||
        event->key.scancode == SDL_SCANCODE_ESCAPE) {
      return SDL_APP_SUCCESS;
    }
    if (event->key.scancode == SDL_SCANCODE_V) {
      appState->vsyncState = !appState->vsyncState;
      if (!setVSync(appState)) {
        appState->vsyncState = !appState->vsyncState;
      }
      std::cout << "VSync: " << (appState->vsyncState ? "on" : "off")
                << std::endl;
    }
    if (event->key.scancode == SDL_SCANCODE_Z) {
      zapBoard(appState);
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

  int cellWidth = WINDOW_WIDTH / appState->boardWidth;
  int cellHeight = WINDOW_HEIGHT / appState->boardHeight;

  if (appState->mouse.down) {
    std::cout << "Mouse Button Down:" << std::endl;
    std::cout << "clickX: " << appState->mouse.clickX
              << "clickY:" << appState->mouse.clickY << std::endl;
    std::cout << "x: " << appState->mouse.x << "y:" << appState->mouse.y
              << std::endl;
    life::setCellState(gameBoard, appState->boardHeight, appState->boardWidth,
                       appState->mouse.x / cellWidth,
                       appState->mouse.y / cellHeight, life::ALIVE);
  }
  return SDL_APP_CONTINUE;
}

/*
 window = x
 board = y
 window / board = # boards per window
 window |----------|
 board  |-----|
  = 2 draws of window items for each board item
 b 0,0 = w 0,0
 b 1,0 = w 2,0
 b 1,1 = w 2,2
 */
bool renderCell(SDL_Renderer *renderer, float cellSize, int x, int y) {
  for (float xD = static_cast<float>(x); xD < x + cellSize; xD++) {
    for (float yD = static_cast<float>(y); yD < y + cellSize; yD++) {
      if (!SDL_RenderPoint(renderer, xD, yD)) {
        SDL_Log("Could not render point: %s", SDL_GetError());
        return false;
      }
    }
  }
  return true;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  static Uint64 ticks = 0;
  static Uint64 lastFrameRate = 0;
  auto *appState = static_cast<AppState *>(appstate);
  auto *appStats = &appState->appStats;

  /* Progress the board forward one step. */
  appStats->start(life::ITERATE, SDL_GetTicks());
  if (!appState->simulationPaused) {
    life::iterateBoard(gameBoard, appState->boardHeight, appState->boardWidth);
  }
  appStats->stop(life::ITERATE, SDL_GetTicks());

  // Render the board.
  appStats->start(life::RENDER, SDL_GetTicks());
  SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(appState->renderer);
  SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  int cellWidth = WINDOW_WIDTH / appState->boardWidth;
  int cellHeight = WINDOW_HEIGHT / appState->boardHeight;
  for (int y = 0; y < appState->boardHeight; y++) {
    for (int x = 0; x < appState->boardWidth; x++) {
      if (life::getCellState(gameBoard, appState->boardHeight, appState->boardWidth,
                             x, y) == life::ALIVE) {
        if (!renderCell(appState->renderer, cellWidth, x * cellWidth,
                        y * cellWidth)) {
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
  Uint64 postframe = SDL_GetTicksNS();
  Uint64 rate = SDL_NS_PER_SECOND / (postframe - ticks);
  appStats->set(life::FRAMERATE, rate);
  ticks = postframe;
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (appstate != nullptr) {
    auto *appState = static_cast<AppState *>(appstate);
    SDL_DestroyRenderer(appState->renderer);
    SDL_DestroyWindow(appState->window);
    delete appState;
  }
}
