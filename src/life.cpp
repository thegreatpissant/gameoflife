// life.cpp
#include "./life.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

namespace life {
GameBoard genBoard(int height, int width) {
  GameBoard gameBoard;
  gameBoard.board = Board(height * width, 0);
  gameBoard.height = height;
  gameBoard.width = width;
  gameBoard.aliveList = AliveList();
  return gameBoard;
}

char getCellState(const GameBoard &gameBoard, int height, const int width, const int x,
                  const int y) {
  return gameBoard.board[y * width + x] & 0x01;
}

void setCellState(GameBoard &gameBoard, int height, const int width, const int x,
                  const int y, char state) {
  const int start_x = std::max(0, x - 1);
  const int start_y = std::max(0, y - 1);
  const int end_x = std::min(width, x + 2);
  const int end_y = std::min(height, y + 2);
  int count = 0;
  for (int i = start_x; i < end_x; i++) {
    for (int j = start_y; j < end_y; j++) {
      if (i == x && j == y) {
        gameBoard.board[j * width + i] = gameBoard.board[j * width + i] | state;
      } else {
        gameBoard.board[j * width + i] =
            (((gameBoard.board[j * width + i] >> 1) + state) << 1) |
            (gameBoard.board[j * width + i] & 0x01);
      }
    }
  }
}

int neighborCount(const GameBoard &gameBoard, const int height, const int width,
                      const int x, const int y) {
  return gameBoard.board[y * width + x] >> 1;
}

void iterateBoard(GameBoard &gameBoard, int height, int width) {
  /*
      Any live cell with fewer than two live neighbours dies, as if by
     underpopulation. Any live cell with two or three live neighbours lives on
     to the next generation. Any live cell with more than three live neighbours
     dies, as if by overpopulation. Any dead cell with exactly three live
     neighbours becomes a live cell, as if by reproduction.
  */

  GameBoard newGameBoard = genBoard(gameBoard.height, gameBoard.width);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int count = neighborCount(gameBoard, height, width, j, i);
      int cellState = getCellState(gameBoard, height, width, j, i);
      if (cellState == life::ALIVE) {
        if (count < 2 || count > 3) {
          setCellState(newGameBoard, height, width, j, i, life::DEAD);
        } else {
          setCellState(newGameBoard, height, width, j, i, life::ALIVE);
          newGameBoard.aliveList.push_back(std::make_pair(j, i));
        }
      } else {
        if (count == 3) {
          setCellState(newGameBoard, height, width, j, i, life::ALIVE);
          newGameBoard.aliveList.push_back(std::make_pair(j, i));
        }
      }
    }
  }
  gameBoard.board = std::move(newGameBoard.board);
  gameBoard.aliveList = std::move(newGameBoard.aliveList);
}

void printBoard(GameBoard gameBoard, int height, int width) {
  std::cout << "  ";
  for (int i = 0; i < width; i++) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < height; i++) {
    std::cout << i << " ";
    for (int j = 0; j < width; j++) {
      std::cout << gameBoard.board[i * width + j] << " ";
    }
    std::cout << std::endl;
  }
}
} // namespace life
