#include "life.hpp" // Your existing life game header
#include "gtest/gtest.h"
#include <iostream> // Keep for printBoard if desired for debugging

// Test fixture for LifeGame tests, if needed for common setup/teardown
// For these tests, a fixture isn't strictly necessary yet, so we'll use TEST
// directly.

TEST(LifeGameTests, CreateBoard) {
  life::GameBoard gameBoard = life::genBoard(life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH);
  // Assuming LIFE_BOARD_HEIGHT and LIFE_BOARD_WIDTH are constants defined in
  // life.hpp
  ASSERT_EQ(gameBoard.board.size(), life::LIFE_BOARD_HEIGHT * life::LIFE_BOARD_WIDTH);
}

TEST(LifeGameTests, NoNeighborCount) {
  life::GameBoard gameBoard = life::genBoard();
  int count = life::neighborCount(gameBoard, 0, 0);
  EXPECT_EQ(0, count);
}

TEST(LifeGameTests, SingleNeighborCount) {
life::GameBoard gameBoard = life::genBoard();
life::setCellState(gameBoard, 0, 0,
                   life::ALIVE);
int count = life::neighborCount(gameBoard,1, 1);
EXPECT_EQ(1, count);
}

TEST(LifeGameTests, MultiNeighborCount) {
  constexpr int boardWidth = 3, boardHeight = 3;
  // It's good practice to generate the board with the dimensions you intend to
  // test
  life::GameBoard gameBoard = life::genBoard(boardHeight, boardWidth);
  life::setCellState(gameBoard, 0, 0, life::ALIVE);
  life::setCellState(gameBoard, 1, 0, life::ALIVE);
  life::setCellState(gameBoard, 0, 1, life::ALIVE);
  // The original test counts neighbors for (0,0). The neighbors are (1,0) and
  // (0,1).
  const int count = life::neighborCount(gameBoard, 0, 0);
  EXPECT_EQ(2, count);
}

TEST(LifeGameTests, GetCellState) {
  life::GameBoard gameBoard = life::genBoard();
  life::setCellState(gameBoard,  0,
                     0, life::ALIVE);
  int state = life::getCellState(gameBoard, 0, 0);
  EXPECT_EQ(state, life::ALIVE);

  life::setCellState(gameBoard, 1,
                     0, life::ALIVE);
  state = life::getCellState(gameBoard, 0, 0);
  EXPECT_EQ(state, life::ALIVE);

  state = life::getCellState(gameBoard, 1, 0);
  EXPECT_EQ(state, life::ALIVE);

  life::setCellState(gameBoard, 2,
                     0, life::ALIVE);
  state = life::getCellState(gameBoard, 2, 0);
  EXPECT_EQ(state, life::ALIVE);
}

TEST(LifeGameTests, SetCellState) {
  life::GameBoard gameBoard = life::genBoard();

  life::setCellState(gameBoard, 0,
                     0, life::ALIVE);
  // Assuming getCellState is reliable, or directly check board if its structure
  // is known For a std::vector<int> board[y * width + x]
  EXPECT_EQ(life::getCellState(gameBoard,0, 0),
            1);

  life::setCellState(gameBoard, 1,
                     0, life::ALIVE);
  EXPECT_EQ(life::getCellState(gameBoard, 1, 0),
            1); // (row 0, col 1)

  life::setCellState(gameBoard, 0,
                     2, life::ALIVE);
  // Original test: int index = life::LIFE_BOARD_WIDTH * 2 + 0; (row 2, col 0)
  // This should be (row y=2, col x=0)
  EXPECT_EQ(life::getCellState(gameBoard, 0, 2),
            1); // (row 2, col 0)
}

TEST(LifeGameTests, IterateBoard) {
  constexpr int boardWidth = 3, boardHeight = 3;

  // Initial state (a small "blinker" precursor that will form a line)
  // . X .      (0,1)
  // X X X      (1,0), (1,1), (1,2)
  // . X .      (2,1)
  // This was the setup in the original test:
  // life::setCellState(board, boardHeight, boardWidth, 1, 0, 1); // (1,0)
  // life::setCellState(board, boardHeight, boardWidth, 0, 1, 1); // (0,1)
  // life::setCellState(board, boardHeight, boardWidth, 1, 1, 1); // (1,1)
  // life::setCellState(board, boardHeight, boardWidth, 1, 2, 1); // (1,2)
  // life::setCellState(board, boardHeight, boardWidth, 2, 2, 1); // (2,2) -
  // This seems like a typo in original, let's use a known pattern. The original
  // gen2 was {1,1,0, 1,0,0, 1,1,1} Let's match the original test's intended
  // outcome.

  // Original setup that leads to gen2 = {1,1,0, 1,0,0, 1,1,1}
  // This setup is a bit unusual for standard Life patterns, but we'll replicate
  // it. Board state before iteration: ? X ?  (0,1) is 1 X X X  (1,0) is 1,
  // (1,1) is 1, (1,2) is 1 ? ? X  (2,2) is 1 All others 0. Let's represent this
  // initial board directly for clarity: Row 0: 0 1 0 Row 1: 1 1 0 Row 2: 0 0 1
  life::GameBoard gameBoard = life::genBoard(boardHeight, boardWidth);
  life::Board board_template = {0, 1, 0, 1, 1, 0, 0, 1, 1};
  {
    auto cell = board_template.begin();
    for (int y = 0; y < boardHeight; y++) {
      for (int x = 0; x < boardWidth; x++) {
        life::setCellState(gameBoard, x, y, *cell);
        cell++;
      }
    }
  }

  // For debugging, you can still print:
  // std::cout << "Board before iteration:" << std::endl;
  // life::printBoard(board, boardHeight, boardWidth);

  life::iterateBoard(gameBoard);

  // std::cout << "Board after iteration:" << std::endl;
  // life::printBoard(board, boardHeight, boardWidth);

  const life::Board expected_gen2_template = {
      1, 1, 0, 1, 0, 0, 1, 1, 1}; // As per original test's assertion
  life::GameBoard expected_gen2 = life::genBoard(boardHeight, boardWidth);
  {
    auto cell = expected_gen2_template.begin();
    for (int y = 0; y < boardHeight; y++) {
      for (int x = 0; x < boardWidth; x++) {
        life::setCellState(expected_gen2, x, y, *cell);
        cell++;
      }
    }
  }

  // Gtest can compare vectors directly
  EXPECT_EQ(gameBoard.board, expected_gen2.board);
}

// Main function to run all tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
