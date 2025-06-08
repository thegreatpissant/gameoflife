#include "gtest/gtest.h"
#include "life.hpp" // Your existing life game header
#include <iostream> // Keep for printBoard if desired for debugging

// Test fixture for LifeGame tests, if needed for common setup/teardown
// For these tests, a fixture isn't strictly necessary yet, so we'll use TEST directly.

TEST(LifeGameTests, CreateBoard) {
    life::Board board = life::genBoard();
    // Assuming LIFE_BOARD_HEIGHT and LIFE_BOARD_WIDTH are constants defined in life.hpp
    ASSERT_EQ(board.size(), life::LIFE_BOARD_HEIGHT * life::LIFE_BOARD_WIDTH);
}

TEST(LifeGameTests, NoNeighborCount) {
    life::Board board = life::genBoard();
    int count = life::neighborCount(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0, 0);
    EXPECT_EQ(count, 0);
}

TEST(LifeGameTests, SingleNeighborCount) {
    life::Board board = life::genBoard();
    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0, 0, 1);
    int count = life::neighborCount(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 1, 1);
    EXPECT_EQ(count, 1);
}

TEST(LifeGameTests, MultiNeighborCount) {
    constexpr int boardWidth = 3, boardHeight = 3;
    // It's good practice to generate the board with the dimensions you intend to test
    life::Board board = life::genBoard(boardHeight, boardWidth); 
    life::setCellState(board, boardHeight, boardWidth, 0, 0, 1 );
    life::setCellState(board, boardHeight, boardWidth, 1, 0, 1 );
    life::setCellState(board, boardHeight, boardWidth, 0, 1, 1 );
    // The original test counts neighbors for (0,0). The neighbors are (1,0) and (0,1).
    const int count = life::neighborCount(board, boardHeight, boardWidth, 0, 0);
    EXPECT_EQ(count, 2);
}

TEST(LifeGameTests, GetCellState) {
    life::Board board = life::genBoard();
    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0, 0, 1);
    int state = life::getCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0, 0);
    EXPECT_EQ(state, 1);

    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 1, 0, 1);
    state = life::getCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 1, 0);
    EXPECT_EQ(state, 1);

    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 2, 0, 1);
    state = life::getCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 2, 0);
    EXPECT_EQ(state, 1);
}

TEST(LifeGameTests, SetCellState) {
    life::Board board = life::genBoard();

    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0, 0, 1);
    // Assuming getCellState is reliable, or directly check board if its structure is known
    // For a std::vector<int> board[y * width + x]
    EXPECT_EQ(board[0 * life::LIFE_BOARD_WIDTH + 0], 1);

    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 1, 0, 1);
    EXPECT_EQ(board[0 * life::LIFE_BOARD_WIDTH + 1], 1); // (row 0, col 1)

    life::setCellState(board, life::LIFE_BOARD_HEIGHT, life::LIFE_BOARD_WIDTH, 0, 2, 1);
    // Original test: int index = life::LIFE_BOARD_WIDTH * 2 + 0; (row 2, col 0)
    // This should be (row y=2, col x=0)
    EXPECT_EQ(board[2 * life::LIFE_BOARD_WIDTH + 0], 1);
}

TEST(LifeGameTests, IterateBoard) {
    constexpr int boardWidth = 3, boardHeight = 3;
    life::Board board = life::genBoard(boardHeight, boardWidth);

    // Initial state (a small "blinker" precursor that will form a line)
    // . X .      (0,1)
    // X X X      (1,0), (1,1), (1,2)
    // . X .      (2,1)
    // This was the setup in the original test:
    // life::setCellState(board, boardHeight, boardWidth, 1, 0, 1); // (1,0)
    // life::setCellState(board, boardHeight, boardWidth, 0, 1, 1); // (0,1)
    // life::setCellState(board, boardHeight, boardWidth, 1, 1, 1); // (1,1)
    // life::setCellState(board, boardHeight, boardWidth, 1, 2, 1); // (1,2)
    // life::setCellState(board, boardHeight, boardWidth, 2, 2, 1); // (2,2) - This seems like a typo in original, let's use a known pattern.
                                                                // The original gen2 was {1,1,0, 1,0,0, 1,1,1}
                                                                // Let's match the original test's intended outcome.
    
    // Original setup that leads to gen2 = {1,1,0, 1,0,0, 1,1,1}
    // This setup is a bit unusual for standard Life patterns, but we'll replicate it.
    // Board state before iteration:
    // ? X ?  (0,1) is 1
    // X X X  (1,0) is 1, (1,1) is 1, (1,2) is 1
    // ? ? X  (2,2) is 1
    // All others 0.
    // Let's represent this initial board directly for clarity:
    // Row 0: 0 1 0
    // Row 1: 1 1 0
    // Row 2: 0 0 1
    board = {0,1,0, 1,1,0, 0,1,1};


    // For debugging, you can still print:
    // std::cout << "Board before iteration:" << std::endl;
    // life::printBoard(board, boardHeight, boardWidth);

    life::iterateBoard(board, boardHeight, boardWidth);

    // std::cout << "Board after iteration:" << std::endl;
    // life::printBoard(board, boardHeight, boardWidth);

    const life::Board expected_gen2 = {1,1,0, 1,0,0, 1,1,1}; // As per original test's assertion
    
    // Gtest can compare vectors directly
    EXPECT_EQ(board, expected_gen2);
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}