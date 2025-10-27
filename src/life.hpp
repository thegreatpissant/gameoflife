// life.hpp
#include <vector>

namespace life {
constexpr int LIFE_BOARD_WIDTH = 100;
constexpr int LIFE_BOARD_HEIGHT = 100;

constexpr char DEAD = 0;
constexpr char ALIVE = 1;

using Board = std::vector<char>;
using AliveList = std::vector<std::pair<int, int>>;

struct GameBoard {
    Board board;
    int height;
    int width;
    AliveList aliveList;
};

GameBoard genBoard(int height=LIFE_BOARD_HEIGHT, int width=LIFE_BOARD_WIDTH);
int neighborCount(const GameBoard &, int x, int y);
char getCellState(const GameBoard &board, int x, int y);
void setCellState(GameBoard &board, int x, int y,
                  char state);
void iterateBoard(GameBoard &board);
void printBoard(GameBoard board);
} // namespace life
