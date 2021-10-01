#include "scores_sorting.h"


void add_to_scores_inplace(QVector<ScoreRecord>& scores, const QString& name,
                           quint32 score, int scores_num_threshold) {
    if (scores.empty()) {
        scores << ScoreRecord(name, score);
    } else {
        // just in case somebody messed up order in text file
        std::sort(scores.begin(), scores.end(),
                  [](const auto& r1, const auto& r2) { return r1.score > r2.score; });
        if (scores.size() > scores_num_threshold)
            scores.resize(scores_num_threshold);

        auto it = std::find_if(scores.begin(), scores.end(),
                               [score](const auto& r) { return r.score < score; });
        // do not bother to insert 11th element
        if (scores.size() != scores_num_threshold || it != scores.end()) {
            scores.insert(it, ScoreRecord(name, score));
            if (scores.size() > scores_num_threshold)
                scores.resize(scores_num_threshold);
        }
    }
}