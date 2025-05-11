// life.hpp
#include <vector>

namespace life {

    const int LIFE_BOARD_WIDTH = 100;
    const int LIFE_BOARD_HEIGHT = 100;
    
    using Board = std::vector<int>;
    
    Board genBoard(int height=LIFE_BOARD_HEIGHT, int width=LIFE_BOARD_WIDTH);
    int neighborCount(Board, int height, int width, int x, int y);
    int getCellState(Board board, int height, int width, int x, int y);
    void setCellState(Board &board, int height, int width, int x, int y, int state);
    void itterateBoard(Board &board, int height, int width);
    void printBoard(Board board, int height, int width);
}