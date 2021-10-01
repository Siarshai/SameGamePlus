#ifndef CLION_SANDBOX_BOOM_BLOCK_H
#define CLION_SANDBOX_BOOM_BLOCK_H


#include <memory>
#include <iostream>

#include <boost/bimap.hpp>

#include <QtCore/QObject>
#include <QVariant>


class BoomBlock: public QObject {
   Q_OBJECT
public:
    enum class Type {
        RED = 0,
        BLUE = 1,
        GREEN = 2,
        YELLOW = 3
    };
    Q_ENUM(Type)

    BoomBlock();
    BoomBlock(BoomBlock::Type type, bool alive, int column, int row);
    ~BoomBlock() override;

    BoomBlock(const BoomBlock& other) = delete;
    BoomBlock& operator =(const BoomBlock& other) = delete;

    BoomBlock(BoomBlock&& other) noexcept;
    BoomBlock& operator =(BoomBlock&& other) noexcept;

    void capture(BoomBlock& other);
    void set_qml_representation(QObject* qml_representation, int block_size);

    void set_alive(bool alive);

    [[nodiscard]] bool is_alive() const;
    [[nodiscard]] Type get_type() const;

private:
    // implied to be the same for every block
    void set_column(int column);
    void set_row(int row);
    void set_type(Type type);
    void set_blocksize(int block_size);

    Type type_;
    bool alive_;
    int column_;
    int row_;
    int block_size_;
    QObject* boom_block_qml;
};

using BlockParseBimap = boost::bimaps::bimap<char, BoomBlock::Type>;
const BlockParseBimap& get_block_symbols();

std::vector<BoomBlock> from_strings(const std::vector<std::string>& str_board);


#endif //CLION_SANDBOX_BOOM_BLOCK_H
