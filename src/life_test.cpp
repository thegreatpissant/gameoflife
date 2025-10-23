#include "life.hpp"
#include <cassert>
#include <iostream>

void TEST_create_board() {
  std::cout << "running test: TEST_create_board" << std::endl;
  life::Board board = life::genBoard();
  assert(board.size() == life::LIFE_BOARD_HEIGHT * life::LIFE_BOARD_WIDTH);
  std::cout << "TEST_create_board passed" << std::endl;
}

void TEST_no_neighbor_count() {
  std::cout << "running test: TEST_neighbor_count" << std::endl;
  life::Board board = life::genBoard();
  int count = life::neighborCount(board, life::LIFE_BOARD_HEIGHT,
                                  life::LIFE_BOARD_WIDTH, 0, 0);
  std::cout << "Want: " << 0 << " Got: " << count << std::endl;
  assert(count == 0);
  std::cout << "TEST_neighbor_count passed" << std::endl;
}

void TEST_single_neighbor_count() {
  std::cout << "running test: TEST_single_neighbor_count" << std::endl;
  life::Board board = life::genBoard();
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0,
                     0, 1);
  int count = life::neighborCount(board, life::LIFE_BOARD_HEIGHT,
                                  life::LIFE_BOARD_WIDTH, 1, 1);
  std::cout << "Want: " << 1 << " Got: " << count << std::endl;
  assert(count == 1);
  std::cout << "TEST_single_neighbor_count passed" << std::endl;
}

void TEST_multi_neighbor_count() {
  std::cout << "running test: TEST_multi_neighbor_count" << std::endl;
  const int boardWidth = 3, boardHeight = 3;
  life::Board board = life::genBoard();
  life::setCellState(board, boardHeight, boardWidth, 0, 0, 1);
  life::setCellState(board, boardHeight, boardWidth, 1, 0, 1);
  life::setCellState(board, boardHeight, boardWidth, 0, 1, 1);
  int count = life::neighborCount(board, boardHeight, boardWidth, 0, 0);
  std::cout << "Want: " << 2 << " Got: " << count << std::endl;
  assert(count == 2);
  std::cout << "TEST_multi_neighbor_count passed" << std::endl;
}

void TEST_get_cell_state() {
  std::cout << "running test: TEST_get_cell_state" << std::endl;
  life::Board board = life::genBoard();
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0,
                     0, 1);
  int state = life::getCellState(board, life::LIFE_BOARD_HEIGHT,
                                 life::LIFE_BOARD_WIDTH, 0, 0);
  std::cout << "board[0] Want: " << 1 << " Got: " << state << std::endl;
  assert(state == 1);
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 1,
                     0, 1);
  state = life::getCellState(board, life::LIFE_BOARD_HEIGHT,
                             life::LIFE_BOARD_WIDTH, 1, 0);
  std::cout << "board[1] Want: " << 1 << " Got: " << state << std::endl;
  assert(state == 1);
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 2,
                     0, 1);
  state = life::getCellState(board, life::LIFE_BOARD_HEIGHT,
                             life::LIFE_BOARD_WIDTH, 2, 0);
  std::cout << "board[2] Want: " << 1 << " Got: " << state << std::endl;
  assert(state == 1);
}

void TEST_set_cell_state() {
  std::cout << "running test: TEST_set_cell_state" << std::endl;
  life::Board board = life::genBoard();
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0,
                     0, 1);
  std::cout << "board[0] Want: " << 1 << " Got: " << board[0] << std::endl;
  assert(board[0] == 1);
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 1,
                     0, 1);
  std::cout << "board[1] Want: " << 1 << " Got: " << board[1] << std::endl;
  assert(board[1] == 1);
  life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0,
                     2, 1);
  int index = life::LIFE_BOARD_WIDTH * 2 + 0;
  std::cout << "board[" << index << "] Want: " << 1 << " Got: " << board[index]
            << std::endl;
  assert(board[index] == 1);
  std::cout << "TEST_set_cell_state passed" << std::endl;
}

void TEST_itterate_board() {
  std::cout << "running test: TEST_itterate_board" << std::endl;
  const int boardWidth = 3, boardHeight = 3;
  life::Board board = life::genBoard(boardHeight, boardWidth);
  life::setCellState(board, boardHeight, boardWidth, 1, 0, 1);
  life::setCellState(board, boardHeight, boardWidth, 0, 1, 1);
  life::setCellState(board, boardHeight, boardWidth, 1, 1, 1);
  life::setCellState(board, boardHeight, boardWidth, 1, 2, 1);
  life::setCellState(board, boardHeight, boardWidth, 2, 2, 1);

  life::printBoard(board, boardHeight, boardWidth);
  life::iterateBoard(board, boardHeight, boardWidth);
  life::printBoard(board, boardHeight, boardWidth);

  life::Board gen2 = {1, 1, 0, 1, 0, 0, 1, 1, 1};
  assert(board == gen2);
  std::cout << "board:" << std::endl;
  life::printBoard(board, boardHeight, boardWidth);
  std::cout << "gen2:" << std::endl;
  life::printBoard(gen2, boardHeight, boardWidth);
  std::cout << "TEST_itterate_board passed" << std::endl;
}

int main() {
  TEST_create_board();
  TEST_no_neighbor_count();
  TEST_single_neighbor_count();
  TEST_set_cell_state();
  TEST_get_cell_state();
  TEST_multi_neighbor_count();
  TEST_itterate_board();
  return 0;
}