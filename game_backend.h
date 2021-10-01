#ifndef CLION_SANDBOX_GAME_BACKEND_H
#define CLION_SANDBOX_GAME_BACKEND_H


#include <QtCore/QObject>
#include <QVariant>
#include <scoreboard/iscoreboard.h>

#include "level_loader/ilevel_loader.h"
#include "game_board.h"


class GameBackend: public QObject {
    Q_OBJECT
public:
    GameBackend(std::unique_ptr<ILevelLoader> level_loader);
    void add_scores_keeper(const QString& name, const std::shared_ptr<IScoresToBackend>& keeper);
    void initialize_game_board(std::unique_ptr<IBoomBlockFactory> block_factory);

public slots:
    void on_start_new_game();
    void on_block_click(const QVariant& var_column, const QVariant& var_row);
    void reset();
    void add_to_scores(const QVariant& name);

signals:
    void score_changed(QVariant new_score);
    void game_over();
    void new_game_started(QVariant max_column_int, QVariant max_row_int,
            QVariant show_input_score_threshold);

private:
    std::unique_ptr<GameBoard> game_board;
    std::unique_ptr<ILevelLoader> level_loader_;
    int score_;
    std::map<QString, std::shared_ptr<IScoresToBackend>> scoreboards_;

    static const int no_pieces_left_score_bonus = 500;
};


#endif //CLION_SANDBOX_GAME_BACKEND_H
