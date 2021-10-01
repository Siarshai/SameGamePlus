#include "scoreboard.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>

#include "scoreboard_factory.h"
#include "scores_sorting.h"


ScoreBoard::ScoreBoard(const QString& scoreboard_filepath, int scores_num_threshold)
        : scoreboard_filepath_(scoreboard_filepath)
        , scores_num_threshold_(scores_num_threshold) {
    QFile file(scoreboard_filepath_);
    file.open(QFile::ReadWrite);  // create file if does not exist
}


QVector<ScoreRecord> ScoreBoard::get_scores() const {
    QFile file(scoreboard_filepath_);
    QFileInfo fi(file);
    QVector<ScoreRecord> scores;
    if (file.open(QFile::ReadOnly)) {
        QTextStream instream(&file);
        while (!instream.atEnd() && scores.size() < scores_num_threshold_) {
            ScoreRecord record;
            instream >> record.name >> record.score;
            if (instream.status() == QTextStream::Ok)
                scores.push_back(record);
            else
                throw std::runtime_error("Scores file is corrupted " + scoreboard_filepath_.toStdString());
            instream.skipWhiteSpace();
        }
    } else {
        throw std::runtime_error("Could not open scores file to read: " +
                scoreboard_filepath_.toStdString() + " : " + file.errorString().toStdString());
    }
    return scores;
}


void ScoreBoard::add_new_score(const QString& name, quint32 score) {
    QVector<ScoreRecord> scores = get_scores();
    // Strategy pattern should be used here if there's more strategies for adding scores
    // DiscIO / Logic decomposition should be used if there's more business logic
    add_to_scores_inplace(scores, name, score, scores_num_threshold_);
    write_scores(scores);
}


void ScoreBoard::write_scores(const QVector<ScoreRecord>& scores) const {
    QFile file(scoreboard_filepath_);
    if (file.open(QFile::WriteOnly)) {
        QTextStream outstream(&file);
        for (const auto& record : scores)
            outstream << record.name << " " << record.score << "\n";
        if (outstream.status() != QTextStream::Ok)
            throw std::runtime_error("Could not write scores file " +
                    scoreboard_filepath_.toStdString());
    } else {
        throw std::runtime_error("Could not open scores file to write: "
                + scoreboard_filepath_.toStdString() + " : " + file.errorString().toStdString());
    }
}

quint32 ScoreBoard::get_lowest_score() const {
    QVector<ScoreRecord> scores = get_scores();
    if (scores.size() < scores_num_threshold_) {
        return 0;
    } else {
        auto it = std::min_element(scores.cbegin(), scores.cend(),
                [](const auto& r1, const auto& r2) { return r1.score < r2.score; });
        return it->score;
    }
}


std::shared_ptr<IScoreBoard> get_scoreboard(const QString& scoreboard_filepath, int scores_num) {
    return std::make_unique<ScoreBoard>(scoreboard_filepath, scores_num);
}