// life.hpp
#include <vector>

namespace life {
constexpr int LIFE_BOARD_WIDTH = 100;
constexpr int LIFE_BOARD_HEIGHT = 100;

constexpr char DEAD = 0;
constexpr char ALIVE = 1;

using Board = std::vector<char>;

Board genBoard(int height = LIFE_BOARD_HEIGHT, int width = LIFE_BOARD_WIDTH);
int neighborCount(const Board &, int height, int width, int x, int y);
char getCellState(const Board &board, int height, int width, int x, int y);
void setCellState(Board &board, int height, int width, int x, int y,
                  char state);
void iterateBoard(Board &board, int height, int width);
void printBoard(Board board, int height, int width);
} // namespace life
