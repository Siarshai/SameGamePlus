#ifndef SAME_GAME_PLUS_LEVEL_LOADER_H
#define SAME_GAME_PLUS_LEVEL_LOADER_H

#include <fstream>
#include <filesystem>
#include <QtCore/QDir>
#include <QVector>

#include "ilevel_loader.h"


struct GameLevelRecord {
    QString path_to_level;
    QString name;
    int width;
    int height;
};

class LevelLoader : public ILevelLoader {
public:
    LevelLoader(const QDir& path_to_levels);
    [[nodiscard]] std::vector<BoomBlock> load_level(const QString& name) const override;
    [[nodiscard]] std::vector<BoomBlock> randomly_generated_level(
            int max_column, int max_row, int colors) const override;
    [[nodiscard]] QVector<QString> get_level_names() const;

private:
    std::vector<GameLevelRecord> levels;
};


#endif //SAME_GAME_PLUS_LEVEL_LOADER_H
