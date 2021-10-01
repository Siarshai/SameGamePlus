#ifndef SAME_GAME_PLUS_SCORES_SORTING_H
#define SAME_GAME_PLUS_SCORES_SORTING_H

#include "iscoreboard.h"

void add_to_scores_inplace(QVector<ScoreRecord>& scores, const QString& name,
                           quint32 score, int scores_num_threshold);

#endif //SAME_GAME_PLUS_SCORES_SORTING_H
