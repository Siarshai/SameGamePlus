#include "boom_block.h"


template <typename L, typename R>
boost::bimap<L, R> make_bimap(std::initializer_list<typename boost::bimap<L, R>::value_type> list) {
    return boost::bimap<L, R>(list.begin(), list.end());
}

const BlockParseBimap& get_block_symbols() {
    static const BlockParseBimap block_symbols = make_bimap<char, BoomBlock::Type>({
            {'R', BoomBlock::Type::RED},
            {'B', BoomBlock::Type::BLUE},
            {'G', BoomBlock::Type::GREEN},
            {'Y', BoomBlock::Type::YELLOW},
    });
    return block_symbols;
}


BoomBlock::BoomBlock()
    : type_(Type::RED), alive_(false), column_(0), row_(0), block_size_(0), boom_block_qml(nullptr) {}

BoomBlock::BoomBlock(BoomBlock::Type type, bool alive, int column, int row)
        : type_(type), alive_(alive), column_(column), row_(row), block_size_(0), boom_block_qml(nullptr) {}

BoomBlock::~BoomBlock() {
    set_alive(false);
}

BoomBlock::BoomBlock(BoomBlock&& other) noexcept :
    type_(other.type_),
    alive_(other.alive_),
    column_(other.column_),
    row_(other.row_),
    block_size_(other.block_size_),
    boom_block_qml(other.boom_block_qml) {}

BoomBlock& BoomBlock::operator =(BoomBlock&& other) noexcept {
    type_ = other.type_;
    alive_ = other.alive_;
    column_ = other.column_;
    row_ = other.row_;
    block_size_ = other.block_size_;
    boom_block_qml = other.boom_block_qml;
    return *this;
}

void BoomBlock::capture(BoomBlock& other) {
    bool alive = other.is_alive();
    Type type = other.get_type();

    set_alive(false);
    boom_block_qml = other.boom_block_qml;
    other.boom_block_qml = nullptr;
    other.set_alive(false);

    set_alive(alive);
    set_type(type);
    set_column(column_);
    set_row(row_);
}

void BoomBlock::set_qml_representation(QObject* qml_representation, int block_size) {
    boom_block_qml = qml_representation;
    set_blocksize(block_size);
    set_type(type_);
    set_alive(alive_);
    set_column(column_);
    set_row(row_);
    // Not enabled by default for initial appearing in desired x position
    if (boom_block_qml)
        boom_block_qml->setProperty("x_spring_animation_enabled", true);
}

void BoomBlock::set_type(Type type) {
    type_ = type;
    if (boom_block_qml)
        boom_block_qml->setProperty("type", static_cast<int>(type));
}

BoomBlock::Type BoomBlock::get_type() const {
    return type_;
}

void BoomBlock::set_alive(bool alive) {
    alive_ = alive;
    if (boom_block_qml) {
        boom_block_qml->setProperty("dying", !alive);  // boom_block_qml is deleted from QML side
        auto clickable_block_area = boom_block_qml->findChild<QObject*>("clickable_block_area");
        if (!alive) {
            // For better responsiveness not waiting for object destruction.
            // New blocks in that place begin to catch clicks immediately.
            clickable_block_area->setProperty("enabled", false);
            boom_block_qml = nullptr;
        }
    }
}

bool BoomBlock::is_alive() const {
    return alive_;
}

void BoomBlock::set_column(int column) {
    column_ = column;
    if (boom_block_qml) {
        boom_block_qml->setProperty("column", column_);
        boom_block_qml->setProperty("x", column_*block_size_);
    }
}

void BoomBlock::set_row(int row) {
    row_ = row;
    if (boom_block_qml) {
        boom_block_qml->setProperty("row", row_);
        boom_block_qml->setProperty("y", row_ * block_size_);
    }
}

// implied to be the same for every block
void BoomBlock::set_blocksize(int block_size) {
    block_size_ = block_size;
    if (boom_block_qml) {
        boom_block_qml->setProperty("width", block_size_);
        boom_block_qml->setProperty("height", block_size_);
        boom_block_qml->setProperty("x", column_ * block_size_);
        boom_block_qml->setProperty("y", row_ * block_size_);
    }
}


std::vector<BoomBlock> from_strings(const std::vector<std::string>& str_board) {
    if (str_board.empty())
        throw std::runtime_error("Can not initialize board: empty list");
    const auto first_string_size = str_board.at(0).size();
    if (first_string_size == 0)
        throw std::runtime_error("Can not initialize board: empty first string");
    if (std::any_of(str_board.cbegin(), str_board.cend(),
                    [first_string_size](const std::string& s) { return first_string_size != s.size(); })) {
        throw std::runtime_error("Can not initialize board: strings of different size");
    }
    std::vector<BoomBlock> board;
    const auto& block_symbols = get_block_symbols();
    for (size_t row = 0; row < str_board.size(); ++row) {
        for (size_t column = 0; column < str_board.at(row).size(); ++column) {
            char block_repr = str_board.at(row)[column];
            if (block_repr == 'X') {
                board.emplace_back(BoomBlock::Type::RED, false, column, row);
                continue;
            }
            const auto it = block_symbols.left.find(block_repr);
            if (it != block_symbols.left.end()) {
                board.emplace_back(it->second, true, column, row);
                continue;
            }
            std::stringstream ss;
            ss << "Can not initialize board: unrecognized symbol " << block_repr;
            throw std::runtime_error(ss.str());
        }
    }
    return board;
}