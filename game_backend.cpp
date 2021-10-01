#include <QtCore/QSettings>
#include "game_backend.h"


static int cells_changed_to_score_diff(int cells_visited) {
    return (cells_visited - 1) * (cells_visited - 1);
}


GameBackend::GameBackend(std::unique_ptr<ILevelLoader> level_loader) :
        level_loader_(std::move(level_loader)), score_(0) {}

void GameBackend::initialize_game_board(std::unique_ptr<IBoomBlockFactory> block_factory) {
    game_board = std::make_unique<GameBoard>(std::move(block_factory), 10, 15);
}

void GameBackend::on_start_new_game() {
    QSettings settings;
    auto difficulty = settings.value("difficulty", "medium").toString();
    auto level_idx = settings.value("level_idx", 0).toInt();
    auto level_names = settings.value("level_names").value<QVector<QString>>();

    // just in case number of levels decreases
    const QString& selected_level = level_names.at(level_idx >= level_names.size() ? 0 : level_idx);
    std::vector<BoomBlock>&& board = selected_level == "Random"
            ? level_loader_->randomly_generated_level(10, 15, difficulty == "medium" ? 3 : 4)
            : level_loader_->load_level(selected_level);
    game_board->populate_board(std::move(board));

    if (selected_level == "Random" && !game_board->are_floodable_blocks_left()) {
        // in an unlikely event that NO blocks will be destructible at start let's redo initializing
        on_start_new_game();
    } else {
        // do not save score for custom levels
        auto show_input_score_threshold =
                selected_level == "Random" ?
                scoreboards_.at(difficulty)->get_lowest_score() :
                std::numeric_limits<int>::max();
        emit new_game_started(
                QVariant::fromValue(game_board->max_column),
                QVariant::fromValue(game_board->max_row),
                QVariant::fromValue(show_input_score_threshold));
        score_ = 0;
        emit score_changed(QVariant(score_));
    }
}

void GameBackend::reset() {
    game_board->clear_board();
}

void GameBackend::add_to_scores(const QVariant& name) {
    if (name.type() != QVariant::String)
        throw std::runtime_error("GameBackend::change_difficulty expected (str)");
    QSettings settings;
    auto difficulty = settings.value("difficulty", "medium").toString();
    scoreboards_.at(difficulty)->add_new_score(name.toString(), score_);
}

void GameBackend::on_block_click(const QVariant& var_column, const QVariant& var_row) {
    if (var_column.type() != QVariant::Int || var_row.type() != QVariant::Int)
        throw std::runtime_error("GameBackend::on_click expected (int, int)");
    size_t column, row;
    column = var_column.toUInt();
    row = var_row.toUInt();
    size_t cells_changed = game_board->flood_fill(column, row);
    if (cells_changed > 0) {
        score_ += cells_changed_to_score_diff(cells_changed);
        emit score_changed(score_);
        if (!game_board->are_floodable_blocks_left()) {
            if (game_board->no_pieces_left()) {
                score_ += no_pieces_left_score_bonus;
                emit score_changed(score_);
            }
            emit game_over();
        }
    }
}

void GameBackend::add_scores_keeper(const QString& name, const std::shared_ptr<IScoresToBackend>& keeper) {
    scoreboards_[name] = keeper;
}
