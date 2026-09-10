#include <QtTest>
#include <QApplication>

#include <memory>

#include "player.h"

static void initializeBoard(Player &player)
{
    int id = 0;
    for (const auto &row : player.hexagonGrid()->getTiles())
        for (Hexagon *hexagon : row)
        {
            TileData *tile = hexagon->getTileData();
            tile->setId(id++);
            tile->setIndex(0);
            tile->setPlacedTile(false);
            tile->setIsValid(false);
            tile->setPlacedToken("");
            tile->setAnimals({});
            tile->setHabitats({});
            tile->setRotation(0);
        }
}

static QVector<TileData> startingTiles(int firstId)
{
    QVector<TileData> result;
    const QList<QPoint> positions{{11, 9}, {12, 9}, {12, 10}};
    for (int i = 0; i < 3; ++i)
    {
        TileData tile(positions[i].x(), positions[i].y());
        tile.setId(firstId + i);
        tile.setIndex(i);
        tile.setPlacedTile(true);
        tile.setIsValid(false);
        tile.setPlacedToken("");
        tile.setAnimals({"bear"});
        tile.setHabitats({"forest"});
        tile.setRotation(i * 60);
        result.append(tile);
    }
    return result;
}

class TestPlayer : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<Player> fixturePlayer;

private slots:
    void init();
    void cleanup();
    void namedConstructorSetsInitialState();
    void constructorPlacesThreeStartingTiles();
    void replacingStartingTilesReplacesValues();
    void turnAndPineconeStateCanBeChanged();
    void copyingMakesIndependentGrid();
    void calculateScorePublishesResults();
};

void TestPlayer::init()
{
    fixturePlayer = std::make_unique<Player>(QString("Player"));
    fixturePlayer->setPlayerId(0);
    initializeBoard(*fixturePlayer);
}

void TestPlayer::cleanup()
{
    fixturePlayer.reset();
}

void TestPlayer::namedConstructorSetsInitialState()
{
    const Player player("Julijana");
    QCOMPARE(player.playerName(), QString("Julijana"));
    QCOMPARE(player.pineconeNumber(), 0);
    QCOMPARE(player.pineconeButtonActivated(), false);
    QCOMPARE(player.myTurn(), false);
    QVERIFY(player.hexagonGrid());
    QCOMPARE(player.hexagonGrid()->getRows(), 30);
    QCOMPARE(player.hexagonGrid()->getCols(), 30);
}

void TestPlayer::constructorPlacesThreeStartingTiles()
{
    auto input = startingTiles(20);
    Player player(7, "Julijana", input);
    QCOMPARE(player.playerId(), 7);
    QCOMPARE(player.getPlayerId(), 7);
    QCOMPARE(player.playerName(), QString("Julijana"));
    const auto actual = player.getCentralTiles();
    QCOMPARE(actual.size(), 3);
    const QList<QPoint> positions{{11, 9}, {12, 9}, {12, 10}};
    for (int i = 0; i < 3; ++i)
    {
        QCOMPARE(actual[i].toVariant().toMap(), input[i].toVariant().toMap());
        QCOMPARE(actual[i].getIndex(), i);
        const auto *stored = player.hexagonGrid()->getTiles()[positions[i].x()][positions[i].y()]->getTileData();
        QCOMPARE(stored->getId(), 20 + i);
        QVERIFY(stored != &input[i]);
    }
    input[0].setId(99);
    QCOMPARE(player.getCentralTiles()[0].getId(), 20);
}

void TestPlayer::replacingStartingTilesReplacesValues()
{
    auto first = startingTiles(10);
    auto second = startingTiles(40);
    Player player(1, "Player", first);
    player.setCentralTiles(second);
    const auto actual = player.getCentralTiles();
    QCOMPARE(actual.size(), 3);
    for (int i = 0; i < 3; ++i)
        QCOMPARE(actual[i].toVariant().toMap(), second[i].toVariant().toMap());
}

void TestPlayer::turnAndPineconeStateCanBeChanged()
{
    Player &player = *fixturePlayer;
    player.setPlayerId(4);
    QCOMPARE(player.playerId(), 4);
    QCOMPARE(player.getPlayerId(), 4);
    player.setMyTurn(true);
    player.setPineconeNumber(3);
    player.setPineconeButtonActivated(true);
    QCOMPARE(player.myTurn(), true);
    QCOMPARE(player.pineconeNumber(), 3);
    QCOMPARE(player.pineconeButtonActivated(), true);
    player.setMyTurn(false);
    player.setPineconeNumber(0);
    player.setPineconeButtonActivated(false);
    QCOMPARE(player.myTurn(), false);
    QCOMPARE(player.pineconeNumber(), 0);
    QCOMPARE(player.pineconeButtonActivated(), false);
}

void TestPlayer::copyingMakesIndependentGrid()
{
    std::unique_ptr<Player> copy;
    {
        Player original("Player");
        original.setPlayerId(2);
        initializeBoard(original);
        original.setPineconeNumber(3);
        original.setPineconeButtonActivated(true);
        original.setMyTurn(true);
        original.hexagonGrid()->getTiles()[2][3]->getTileData()->setId(71);
        copy = std::make_unique<Player>(original);
        QCOMPARE(copy->playerName(), QString("Player"));
        QCOMPARE(copy->pineconeNumber(), 3);
        QCOMPARE(copy->pineconeButtonActivated(), true);
        QCOMPARE(copy->myTurn(), true);
        QVERIFY(copy->hexagonGrid() != original.hexagonGrid());
        QVERIFY(copy->hexagonGrid()->getTiles()[2][3] != original.hexagonGrid()->getTiles()[2][3]);
        QCOMPARE(copy->hexagonGrid()->getTiles()[2][3]->getTileData()->getId(), 71);
        original.hexagonGrid()->getTiles()[2][3]->getTileData()->setId(99);
        QCOMPARE(copy->hexagonGrid()->getTiles()[2][3]->getTileData()->getId(), 71);
    }
    QCOMPARE(copy->hexagonGrid()->getTiles()[2][3]->getTileData()->getId(), 71);
}

void TestPlayer::calculateScorePublishesResults()
{
    Player &player = *fixturePlayer;
    for (int col : {2, 3})
    {
        TileData *tile = player.hexagonGrid()->getTiles()[2][col]->getTileData();
        tile->setPlacedTile(true);
        tile->setHabitats({"forest"});
        tile->setAnimals({"bear"});
        tile->setPlacedToken("bear");
    }
    player.calculateScore();
    const auto animals = player.animalPoints();
    const auto habitats = player.habitatScores();
    QVERIFY(animals.count("bear") == 1);
    QVERIFY(habitats.count("forest") == 1);
    QCOMPARE(animals.at("bear"), 4);
    QCOMPARE(habitats.at("forest"), 2);
    for (const QString &name : {QString("elk"), QString("fox"), QString("hawk"), QString("salmon")})
    {
        QVERIFY(animals.count(name) == 1);
        QCOMPARE(animals.at(name), 0);
    }
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestPlayer test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_player.moc"