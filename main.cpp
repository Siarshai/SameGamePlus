#include <string>
#include <cstdlib>
#include <ctime>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>
#include <QtCore/QDir>
#include <QAbstractTableModel>
#include <QtCore/QSettings>

#include "game_backend.h"
#include "level_loader/level_loader.h"
#include "scoreboard/scores_context.h"
#include "scoreboard/scoreboard_factory.h"


__attribute__((unused))
void print_tree(const QObject* object, int depth) {
    qDebug().noquote() << QString::fromStdString(std::string(depth, '>'))
        << object->objectName() << object->metaObject()->className();
    for (const QObject* o: object->children()) {
        print_tree(o, depth+1);
    }
}


class RealBoomBlockFactory : public IBoomBlockFactory {
public:
    RealBoomBlockFactory(QQmlEngine* engine, QObject* parent, QObject* blocks_callback) :
            component_(engine, QUrl(QStringLiteral("qrc:/resources/BoomBlock.qml")), parent),
            parent_(parent),
            blocks_callback_(blocks_callback) {
        if (component_.isError()) {
            std::stringstream ss;
            ss << "BoomBlock.qml failed to initialize: ";
            for (const auto& e: component_.errors())
                ss << e.toString().toStdString();
            throw std::runtime_error(ss.str());
        }
    }

    QObject* createQMLBoomBlock() override {
        QObject* object = component_.create();
        // apparently setting parent for QQmlComponent is not enough :/
        object->setProperty("parent", QVariant::fromValue<QObject*>(parent_));
        // destroyed from state change
        QQmlEngine::setObjectOwnership(object, QQmlEngine::ObjectOwnership::JavaScriptOwnership);
        auto clickable_block_area = object->findChild<QObject*>("clickable_block_area");
        QObject::connect(clickable_block_area, SIGNAL(blockClick(QVariant, QVariant)),
                         blocks_callback_, SLOT(on_block_click(QVariant, QVariant)));
        return object;
    }

private:
    QQmlComponent component_;
    QObject* parent_;  // non-owning
    QObject* blocks_callback_;  // non-owning
};



int main(int argc, char** argv) {
    std::srand(std::time(nullptr));
    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));

    const int scores_table_size = 10;
    auto loader = std::make_unique<LevelLoader>(QDir(":/resources/levels/"));
    const auto& medium_scoreboard = get_scoreboard("medium_scoreboard.txt", scores_table_size);
    const auto& hard_scoreboard = get_scoreboard("hard_scoreboard.txt", scores_table_size);

    // for ScoresApplicationData::getModel() - surprisingly not registered by default
    qmlRegisterType<QAbstractTableModel>();
    ScoresApplicationData scores_data(scores_table_size);
    scores_data.add_scores_retriever("medium", medium_scoreboard);
    scores_data.add_scores_retriever("hard", hard_scoreboard);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Stub");
    QCoreApplication::setOrganizationDomain("stub.com");
    QCoreApplication::setApplicationName("SameGamePlus");

    QQuickView quickview;
    quickview.rootContext()->setContextProperty("scores_application_data", &scores_data);
    quickview.setSource(QUrl(QStringLiteral("qrc:/resources/main.qml")));
    quickview.setMinimumHeight(quickview.height());
    quickview.setMaximumHeight(quickview.height());
    quickview.setMinimumWidth(quickview.width());
    quickview.setMaximumWidth(quickview.width());

    auto main_exit_button = quickview.findChild<QObject*>("main_exit_button");
    QObject::connect(main_exit_button, SIGNAL(clicked()),
                     &app, SLOT(quit()));

    /*
     * NOTE: This is not how it's done! I should have exposed GameBacked through context here
     * but I wanted to see capabilities of Qt5
     * */
    auto main_start_button = quickview.findChild<QObject*>("main_start_button");
    auto back_to_menu_button = quickview.findChild<QObject*>("back_to_menu_button");
    auto start_new_game_button = quickview.findChild<QObject*>("restart_button");
    auto score_label = quickview.findChild<QObject*>("score_label");
    auto game_canvas = quickview.findChild<QObject*>("game_canvas");
    auto win_dialog_with_input = quickview.findChild<QObject*>("win_dialog_with_input");

    QSettings settings;
    settings.setValue("level_names", QVariant::fromValue(loader->get_level_names()));
    settings.sync();

    GameBackend game_backend(std::move(loader));
    game_backend.add_scores_keeper("medium", medium_scoreboard);
    game_backend.add_scores_keeper("hard", hard_scoreboard);
    game_backend.initialize_game_board(
            std::make_unique<RealBoomBlockFactory>(quickview.engine(), game_canvas, &game_backend));

    QObject::connect(back_to_menu_button, SIGNAL(clicked()),
                     &game_backend, SLOT(reset()));
    QObject::connect(main_start_button, SIGNAL(clicked()),
                     &game_backend, SLOT(on_start_new_game()));
    QObject::connect(start_new_game_button, SIGNAL(clicked()),
                     &game_backend, SLOT(on_start_new_game()));
    QObject::connect(win_dialog_with_input, SIGNAL(addToScores(QVariant)),
                     &game_backend, SLOT(add_to_scores(QVariant)));
    QObject::connect(&game_backend, SIGNAL(new_game_started(QVariant, QVariant, QVariant)),
                     game_canvas, SLOT(onNewGameStarted(QVariant, QVariant, QVariant)));
    QObject::connect(&game_backend, SIGNAL(game_over()),
                     game_canvas, SLOT(onGameOver()));
    QObject::connect(&game_backend, SIGNAL(score_changed(QVariant)),
                     score_label, SLOT(onScoreChanged(QVariant)));

    quickview.show();

    return QGuiApplication::exec();
}

