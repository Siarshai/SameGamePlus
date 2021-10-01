#include "scores_context.h"
#include "scoreboard_factory.h"


ScoresTableModel::ScoresTableModel(QVector<ScoreRecord>&& scores, int scores_num)
        : scores_num_(scores_num), scores_(scores) {}

int ScoresTableModel::rowCount(const QModelIndex& model_index) const {
    return scores_num_;
}

int ScoresTableModel::columnCount(const QModelIndex& model_index) const {
    return 2;
}

QVariant ScoresTableModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    int column = index.column();
    switch (role) {
        case Qt::DisplayRole:
            if (row < scores_.size()) {
                if (column == 0)
                    return scores_.at(row).name;
                else
                    return scores_.at(row).score;
            } else {
                if (column == 0)
                    return "Empty";
                else
                    return "0";
            }
        case Qt::TextAlignmentRole:
            if (column == 0)
                return Qt::AlignRight;
            else
                return Qt::AlignLeft;
        default:
            break;
    }
    return QVariant();
}

QHash<int, QByteArray> ScoresTableModel::roleNames() const {
    return {
            {Qt::DisplayRole, "modelData"},
            {Qt::TextAlignmentRole, "modelAlignment"}
    };
}


ScoresApplicationData::ScoresApplicationData(int scores_num)
        : scores_num_(scores_num) {}

QAbstractTableModel* ScoresApplicationData::getModel(QVariant name) const {
    if (name.type() != QVariant::String)
        throw std::runtime_error("GameBackend::select_level expected (str)");
    auto qname = name.toString();
    return new ScoresTableModel(scores_retrievers_.at(qname)->get_scores(), scores_num_);
}

void ScoresApplicationData::add_scores_retriever(
        const QString& name, const std::shared_ptr<IScoresTableRetriever>& retriever) {
    scores_retrievers_[name] = retriever;
}
