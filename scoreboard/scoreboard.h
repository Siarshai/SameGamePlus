#ifndef SAME_GAME_PLUS_SCOREBOARD_H
#define SAME_GAME_PLUS_SCOREBOARD_H

#include <QVector>
#include <QtCore/QString>
#include <QtCore/QArgument>
#include <QtCore/QMetaType>
#include <utility>

#include "iscoreboard.h"


class ScoreBoard : public IScoreBoard {
public:
    ScoreBoard(const QString& scoreboard_filepath, int scores_num_threshold);
    [[nodiscard]] QVector<ScoreRecord> get_scores() const override;
    void add_new_score(const QString& name, quint32 score) override;
    [[nodiscard]] quint32 get_lowest_score() const override;

private:
    void write_scores(const QVector<ScoreRecord>& scores) const;

    const QString scoreboard_filepath_;
    const int scores_num_threshold_;
};


#endif //SAME_GAME_PLUS_SCOREBOARD_H
