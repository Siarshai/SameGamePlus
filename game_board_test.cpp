#include "game_board.h"

#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"
#include "gmock/gmock.h"


using namespace testing;


namespace {
    __attribute__((unused))
    void debug_print_board(const GameBoard& gb, const std::vector<BoomBlock>& board, int max_column, int max_row) {
        const auto& block_symbols = get_block_symbols();
        std::stringstream ss;
        for (int row = 0; row < max_row; ++row) {
            for (int column = 0; column < max_column; ++column) {
                if (!board.at(gb.index(column, row)).is_alive())
                    ss << 'X';
                else
                    ss << block_symbols.right.at(board.at(gb.index(column, row)).get_type());
            }
            ss << '\n';
        }
        std::cout << ss.str();
    }
}


class MockBoomBlockFactory : public IBoomBlockFactory {
public:
    MOCK_METHOD0(createQMLBoomBlock, QObject*());
};


class GameBoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        game_board = std::make_unique<GameBoard>(
                std::make_unique<testing::NiceMock<MockBoomBlockFactory>>(), 5, 4);
    }

    std::unique_ptr<GameBoard> game_board;
};


TEST_F(GameBoardTest, BlocksAreDeadByDefault) {
    ASSERT_TRUE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, FillDeadBlocksThrows1) {
    EXPECT_THROW(game_board->flood_fill(0, 0), std::runtime_error);
}

TEST_F(GameBoardTest, FillDeadBlocksThrows2) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "XXXXX",
            "XXXXX",
            "RRRXX",
    });
    game_board->populate_board(std::move(board));
    EXPECT_THROW(game_board->flood_fill(0, 0), std::runtime_error);
}

TEST_F(GameBoardTest, FillOutsideBoardThrows) {
    EXPECT_THROW(game_board->flood_fill(100500, 0), std::runtime_error);
    EXPECT_THROW(game_board->flood_fill(0, 100500), std::runtime_error);
}

TEST_F(GameBoardTest, FillTest1OriginLastBlocks) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "XXXXX",
            "XXXXX",
            "RRRXX",
    });
    game_board->populate_board(std::move(board));
    ASSERT_FALSE(game_board->no_pieces_left());
    int blocks_deleted = game_board->flood_fill(0, game_board->max_row - 1);
    ASSERT_EQ(blocks_deleted, 3);
    ASSERT_TRUE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, FillTest2NotOriginIgnoringWrongColor) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "XXXXX",
            "XBBBB",
            "RRRRR",
    });
    game_board->populate_board(std::move(board));
    ASSERT_FALSE(game_board->no_pieces_left());
    int blocks_deleted = game_board->flood_fill(1, game_board->max_row - 2);
    ASSERT_EQ(blocks_deleted, 4);
    ASSERT_FALSE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, FillTest3FillSpreadsAllSides) {
    std::vector<BoomBlock> board = from_strings({
            "YYYYY",
            "YGGGY",
            "YGGGY",
            "YYYYY",
    });
    game_board->populate_board(std::move(board));
    ASSERT_FALSE(game_board->no_pieces_left());
    ASSERT_TRUE(game_board->are_floodable_blocks_left());
    int blocks_deleted = game_board->flood_fill(1, game_board->max_row - 2);
    ASSERT_EQ(blocks_deleted, 6);
    ASSERT_FALSE(game_board->no_pieces_left());
    ASSERT_TRUE(game_board->are_floodable_blocks_left());
    blocks_deleted = game_board->flood_fill(0, 0);
    ASSERT_EQ(blocks_deleted, 14);
    ASSERT_TRUE(game_board->no_pieces_left());
    ASSERT_FALSE(game_board->are_floodable_blocks_left());
}

TEST_F(GameBoardTest, SingularBlocksAreNotDeleted) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "XXXXX",
            "GBBXX",
            "RYYXX",
    });
    game_board->populate_board(std::move(board));
    int blocks_deleted = game_board->flood_fill(0, game_board->max_row - 1);
    ASSERT_EQ(blocks_deleted, 0);
    ASSERT_FALSE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, BoardShufflesOnSetup) {
    std::vector<BoomBlock> board = from_strings({
            "XXXRX",
            "XXRXX",
            "XRXXX",
            "XXXXR",
    });
    game_board->populate_board(std::move(board));
    int blocks_deleted = game_board->flood_fill(0, game_board->max_row - 1);
    ASSERT_EQ(blocks_deleted, 4);
}

TEST_F(GameBoardTest, TestAreFloodableBlocksLeft1) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "XXXXX",
            "BXXXX",
            "RXXXX",
    });
    game_board->populate_board(std::move(board));
    ASSERT_FALSE(game_board->are_floodable_blocks_left());
    ASSERT_FALSE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, TestAreFloodableBlocksLeft2) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "BXXXX",
            "BXXXX",
            "RXXXX",
    });
    game_board->populate_board(std::move(board));
    ASSERT_TRUE(game_board->are_floodable_blocks_left());
    ASSERT_FALSE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, TestAreFloodableBlocksLeft3) {
    std::vector<BoomBlock> board = from_strings({
            "GRXXX",
            "BGXXX",
            "RBXXX",
            "BRXXX",
    });
    game_board->populate_board(std::move(board));
    ASSERT_FALSE(game_board->are_floodable_blocks_left());
    ASSERT_FALSE(game_board->no_pieces_left());
}

TEST_F(GameBoardTest, ComplexTestShuffleCheck) {
    std::vector<BoomBlock> board = from_strings({
            "XXXXX",
            "XXXXX",
            "GGRRG",
            "BGRBB",
    });
    game_board->populate_board(std::move(board));
    int blocks_deleted = game_board->flood_fill(2, 2);
    ASSERT_EQ(blocks_deleted, 3);
    blocks_deleted = game_board->flood_fill(2, 2);
    ASSERT_EQ(blocks_deleted, 4);
    blocks_deleted = game_board->flood_fill(0, 3);
    ASSERT_EQ(blocks_deleted, 3);
    ASSERT_FALSE(game_board->are_floodable_blocks_left());
    ASSERT_TRUE(game_board->no_pieces_left());
}


