// life.cpp
#include "./life.hpp"
#include <algorithm>
#include <iostream>

using namespace std;
namespace life {
Board genBoard(const int height, const int width) {
    Board board = Board(height * width);
    for(auto beg = board.begin(); beg != board.end(); beg++) {
        *beg = 0;
    }
    return board;
}
int getCellState(const Board& board, int height, const int width, const int x, const int y) {
    return board[y * width + x];
}

void setCellState(Board &board, int height, const int width, const int x, const int y, int state) {
    board[y * width + x] = state;
}

int neighborCount(const Board& board, const int height, const int width, const int x, const int y){
    const int start_x = std::max(0, x - 1);
    const int start_y = std::max(0, y - 1);
    const int end_x = std::min(width, x + 2);
    const int end_y = std::min(height, y + 2);
    int count = 0;
    for (int i = start_x; i < end_x; i++) {
        for (int j = start_y; j < end_y; j++) {
            if (i == x && j == y) {
                continue;
            }
            if (board[j * width + i] == 1) {
                count++;
            }
        }
    }
    return count;
}

void iterateBoard(Board &board, int height, int width) {
/*
    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
    Board newBoard = genBoard(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int count = neighborCount(board, height, width, j, i);
            int cellState = getCellState(board, height, width, j, i);
            if (cellState == 1) {
                if (count < 2 || count > 3) {
                    setCellState(newBoard, height, width, j, i, 0);
                } else {
                    setCellState(newBoard, height, width, j, i, 1);
                }
            } else {
                if (count == 3) {
                    setCellState(newBoard, height, width, j, i, 1);
                }
            }
        }
    }
    board = std::move(newBoard);
}

void printBoard(Board board, int height, int width) {
    std::cout << "  ";
    for (int i = 0; i < width; i++ ) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < height; i++) {
        std::cout << i << " ";
        for (int j = 0; j < width; j++) {
            std::cout << board[i * width + j] << " ";
        }
        std::cout << std::endl;
    }
}
}