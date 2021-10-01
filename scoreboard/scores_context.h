#ifndef SAME_GAME_PLUS_SCORES_CONTEXT_H
#define SAME_GAME_PLUS_SCORES_CONTEXT_H


#include <QtCore/QArgument>
#include <QtCore/QVariant>
#include <QtCore/QAbstractTableModel>
#include <memory>

#include "iscoreboard.h"


class ScoresTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    ScoresTableModel(QVector<ScoreRecord>&& scores, int scores_num);

    [[nodiscard]] int rowCount(const QModelIndex& model_index) const override;
    [[nodiscard]] int columnCount(const QModelIndex& model_index) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

private:
    const int scores_num_;
    QVector<ScoreRecord> scores_;
};

Q_DECLARE_METATYPE(QAbstractTableModel*)
Q_DECLARE_METATYPE(ScoresTableModel*)


class ScoresApplicationData : public QObject {
    Q_OBJECT
public:
    ScoresApplicationData(int scores_num);
    Q_INVOKABLE QAbstractTableModel* getModel(QVariant name) const;
    void add_scores_retriever(const QString& name, const std::shared_ptr<IScoresTableRetriever>& retriever);
private:
    const int scores_num_;
    std::map<QString, std::shared_ptr<IScoresTableRetriever>> scores_retrievers_;
};


#endif //SAME_GAME_PLUS_SCORES_CONTEXT_H
