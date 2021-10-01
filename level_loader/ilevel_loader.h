#ifndef SAME_GAME_PLUS_ILEVEL_LOADER_H
#define SAME_GAME_PLUS_ILEVEL_LOADER_H

#include "common/boom_block.h"


class ILevelLoader {
public:
    virtual ~ILevelLoader() = default;
    [[nodiscard]] virtual std::vector<BoomBlock> load_level(const QString& name) const = 0;
    [[nodiscard]] virtual std::vector<BoomBlock> randomly_generated_level(
            int max_column, int max_row, int colors) const = 0;
};

#endif //SAME_GAME_PLUS_ILEVEL_LOADER_H