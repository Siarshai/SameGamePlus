#include <QtCore/QDirIterator>
#include <QtCore/QTextStream>
#include "level_loader.h"


LevelLoader::LevelLoader(const QDir& path_to_levels) {
    levels.clear();
    if (!path_to_levels.exists())
        throw std::runtime_error("Levels directory " + path_to_levels.path().toStdString() + " does not exist");
    QDirIterator it(path_to_levels.path(), QStringList() << "*.txt", QDir::Files);
    while (it.hasNext()) {
        GameLevelRecord record;
        record.path_to_level = it.next();
        QFile file(record.path_to_level);
        if (file.open(QFile::ReadOnly)) {
            QTextStream instream(&file);
            instream >> record.name >> record.width >> record.height;
            if (instream.status() == QTextStream::Ok)
                levels.push_back(record);
            else
                throw std::runtime_error("Malformed level file " + record.path_to_level.toStdString());
        } else {
            throw std::runtime_error("Could not open level file " + record.path_to_level.toStdString());
        }
    }
}

std::vector<BoomBlock> LevelLoader::load_level(const QString& name) const {
    auto it = std::find_if(levels.cbegin(), levels.cend(),
            [name](const auto& record) { return record.name == name; });
    if (it == levels.cend())
        throw std::runtime_error("Can not load level: no such level: " + name.toStdString());
    const GameLevelRecord& record = *it;
    std::vector<std::string> strings_representation;
    QFile file(record.path_to_level);
    if (file.open(QFile::ReadOnly)) {
        QTextStream instream(&file);
        instream.readLine();  // skip header
        while (!instream.atEnd()) {
            std::string line = instream.readLine().toStdString();
            if (instream.status() != QTextStream::Ok)
                throw std::runtime_error("Unexpected stream malfunction: " + record.name.toStdString());
            if (line.empty())
                continue;
            strings_representation.push_back(line);
        }
    } else {
        throw std::runtime_error("Unexpected could not open file: " + record.name.toStdString());
    }
    return from_strings(strings_representation);
}

std::vector<BoomBlock> LevelLoader::randomly_generated_level(int max_column, int max_row, int colors) const {
    static const std::vector<BoomBlock::Type> type_array = {
            BoomBlock::Type::RED,
            BoomBlock::Type::BLUE,
            BoomBlock::Type::GREEN,
            BoomBlock::Type::YELLOW
    };
    std::vector<BoomBlock> board;
    colors = colors <= 3 ? 3 : 4;  // only three or four colors are supported right now
    for (int row = 0; row < max_row; ++row)
        for (int column = 0; column < max_column; ++column)
            board.emplace_back(type_array[rand() % colors], true, column, row);
    return board;
}

QVector<QString> LevelLoader::get_level_names() const {
    QVector<QString> result;
    result << "Random";
    for (const auto& record : levels)
        result << record.name;
    return result;
}