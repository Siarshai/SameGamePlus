#include "game_board.h"

#include <cstdlib>
#include <sstream>
#include <boost/bimap.hpp>

#include <QObject>
#include <QtQml/QQmlComponent>


GameBoard::GameBoard(std::unique_ptr<IBoomBlockFactory> block_factory, size_t max_column, size_t max_row) :
        max_column(max_column),
        max_row(max_row),
        max_index(max_column*max_row),
        block_factory_(std::move(block_factory)) {
    board_real.resize(max_index);
    if (max_row < 2 || max_column < 2) {
        std::stringstream ss;
        ss << "Invalid max_row and max_column: "
            << max_row << ", " << max_column;
        throw std::runtime_error(ss.str());
    }
    if (max_row > 50 || max_column > 50) {
        std::stringstream ss;
        ss << "max_row and max_column are suspiciously high (you sure you meant that?): "
            << max_row << ", " << max_column;
        throw std::runtime_error(ss.str());
    }
}

GameBoard::~GameBoard() {
    clear_board();
}

size_t GameBoard::index(size_t column, size_t row) const {
    return column + (row*max_column);
}

void GameBoard::populate_board(std::vector<BoomBlock>&& board) {
    if (board.size() != max_index)
        throw std::runtime_error("Game board is incompatible");
    set_board(std::move(board), true);
}

int GameBoard::flood_fill(size_t column, size_t row) {
    if (column > max_column || row > max_row) {
        std::stringstream ss;
        ss << "Tried to access element beyond max index, ";
        ss << " max с: " << max_column << "r: " << max_row;
        ss << " found c: " << column << " r: " << row;
        throw std::runtime_error(ss.str());
    }
    auto& initial_block = board_real.at(index(column, row));
    if (!initial_block.is_alive()) {
        std::stringstream ss;
        ss << "Tried to flood fill dead element in backend: ";
        ss << " c: " << column << " r: " << row;
        throw std::runtime_error(ss.str());
    }
    std::vector<bool> walked_blocks;
    walked_blocks.resize(max_index);
    size_t blocks_deleted = 0;
    walked_blocks[index(column, row)] = true;
    flood_fill_recursive(column + 1, row, initial_block.get_type(), walked_blocks, blocks_deleted);
    flood_fill_recursive(column, row + 1, initial_block.get_type(), walked_blocks, blocks_deleted);
    if (column > 0)
        flood_fill_recursive(column - 1, row, initial_block.get_type(), walked_blocks, blocks_deleted);
    if (row > 0)
        flood_fill_recursive(column, row - 1, initial_block.get_type(), walked_blocks, blocks_deleted);

    if (blocks_deleted > 0) {
        initial_block.set_alive(false);
        ++blocks_deleted;
        shuffle_board();
    }
    return blocks_deleted;
}

void GameBoard::clear_board() {
    for (auto& block : board_real)
        block.set_alive(false);
}

bool GameBoard::are_floodable_blocks_left() const {
    if (!board_real[index(0, max_row - 1)].is_alive())
        return false;
    auto current_type = board_real[index(0, max_row - 1)].get_type();
    return are_floodable_pieces_left_recursive(1, max_row - 1, current_type)
        || are_floodable_pieces_left_recursive(0, max_row - 2, current_type);
}

bool GameBoard::no_pieces_left() const {
    return !board_real[index(0, max_row - 1)].is_alive();
}

void GameBoard::shuffle_board() {
    shuffle_board_down();
    shuffle_board_left();
}

void GameBoard::set_board(std::vector<BoomBlock>&& board, bool need_shuffle) {
    clear_board();
    board_real = std::move(board);
    if (need_shuffle)
        shuffle_board();
    for (auto& block : board_real)
        block.set_qml_representation(block_factory_->createQMLBoomBlock(), 40);
}

bool GameBoard::are_floodable_pieces_left_recursive(size_t column, size_t row,
        BoomBlock::Type neighbour_type) const {
    if (column > max_column || column < 0 || row > max_row || row < 0)
        return false;
    if (!board_real[index(column, row)].is_alive())
        return false;
    auto current_type = board_real[index(column, row)].get_type();
    if (current_type == neighbour_type)
        return true;
    return are_floodable_pieces_left_recursive(column + 1, row, current_type)
           || are_floodable_pieces_left_recursive(column, row - 1, current_type);
}

void GameBoard::flood_fill_recursive(size_t column, size_t row, BoomBlock::Type initial_flood_type,
                                     std::vector<bool>& walked_blocks, size_t& blocks_deleted) {
    if (column > max_column - 1 || row > max_row - 1)
        return;
    if (walked_blocks[index(column, row)])
        return;
    auto& current_block = board_real.at(index(column, row));
    if(!current_block.is_alive())
        return;
    if (current_block.get_type() != initial_flood_type)
        return;
    walked_blocks[index(column, row)] = true;
    current_block.set_alive(false);
    ++blocks_deleted;
    flood_fill_recursive(column + 1, row, initial_flood_type, walked_blocks, blocks_deleted);
    flood_fill_recursive(column, row + 1, initial_flood_type, walked_blocks, blocks_deleted);
    if (column > 0)
        flood_fill_recursive(column - 1, row, initial_flood_type, walked_blocks, blocks_deleted);
    if (row > 0)
        flood_fill_recursive(column, row - 1, initial_flood_type, walked_blocks, blocks_deleted);
}

void GameBoard::shuffle_board_left() {
    for (size_t row = 0; row < max_row; ++row) {
        size_t fall_distance = 0;
        for (size_t column = 0; column < max_column; ++column) {
            if (!board_real[index(column, row)].is_alive()) {
                ++fall_distance;
            } else {
                if (fall_distance > 0)
                    board_real[index(column - fall_distance, row)].capture(board_real[index(column, row)]);
            }
        }
    }
}

void GameBoard::shuffle_board_down() {
    for (size_t column = 0; column < max_column; ++column) {
        size_t fall_distance = 0;
        size_t row = max_row;
        do {
            --row;
            if (!board_real[index(column, row)].is_alive()) {
                ++fall_distance;
            } else {
                if (fall_distance > 0)
                    board_real[index(column, row + fall_distance)].capture(board_real[index(column, row)]);
            }
        } while (row != 0);
    }
}
