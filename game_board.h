#ifndef CLION_SANDBOX_GAME_BOARD_H
#define CLION_SANDBOX_GAME_BOARD_H

#include <bitset>
#include <vector>
#include <memory>
#include "common/boom_block.h"


class IBoomBlockFactory {
public:
    virtual ~IBoomBlockFactory() = default;
    virtual QObject* createQMLBoomBlock() = 0;
};


class GameBoard {
public:
    GameBoard(std::unique_ptr<IBoomBlockFactory> block_factory, size_t max_column, size_t max_row);
    ~GameBoard();

    [[nodiscard]] size_t index(size_t column, size_t row) const;

    void populate_board(std::vector<BoomBlock>&& board);
    int flood_fill(size_t column, size_t row);
    void clear_board();

    // must guarantee that board is shuffled before this call
    [[nodiscard]] bool are_floodable_blocks_left() const;
    [[nodiscard]] bool no_pieces_left() const;

    const size_t max_column;
    const size_t max_row;
    const size_t max_index;

private:
    void shuffle_board();
    void set_board(std::vector<BoomBlock>&& board, bool need_shuffle);
    bool are_floodable_pieces_left_recursive(size_t column, size_t row,
            BoomBlock::Type neighbour_type) const;
    void flood_fill_recursive(size_t column, size_t row, BoomBlock::Type initial_flood_type,
                              std::vector<bool>& walked_blocks, size_t& blocks_deleted);
    void shuffle_board_left();
    void shuffle_board_down();

    std::unique_ptr<IBoomBlockFactory> block_factory_;
    std::vector<BoomBlock> board_real;
};


#endif //CLION_SANDBOX_GAME_BOARD_H
