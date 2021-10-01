#ifndef SAME_GAME_PLUS_ISCOREBOARD_H
#define SAME_GAME_PLUS_ISCOREBOARD_H


#include <QtCore/QArgument>
#include <QtCore/QMetaType>
#include <QString>
#include <QVector>


struct ScoreRecord {
    Q_GADGET
public:
    ScoreRecord() = default;
    ScoreRecord(QString name, quint32 score)
            : name(std::move(name)), score(score) {}
    bool operator ==(const ScoreRecord& other) const {
        return name == other.name && score == other.score;
    };
    QString name;
    quint32 score;
};


class IScoresToBackend {
public:
    virtual ~IScoresToBackend() = default;
    virtual void add_new_score(const QString& name, quint32 score) = 0;
    virtual quint32 get_lowest_score() const = 0;
};

class IScoresTableRetriever {
public:
    virtual ~IScoresTableRetriever() = default;
    [[nodiscard]] virtual QVector<ScoreRecord> get_scores() const = 0;
};

class IScoreBoard : public IScoresToBackend, public IScoresTableRetriever {};


Q_DECLARE_METATYPE(ScoreRecord)
Q_DECLARE_METATYPE(QVector<ScoreRecord>)


#endif //SAME_GAME_PLUS_ISCOREBOARD_H
