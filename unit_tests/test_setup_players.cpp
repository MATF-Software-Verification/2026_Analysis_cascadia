#include <QtTest>
#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSet>

#include <memory>
#include <utility>
#include <vector>

#include "setupPlayers.h"

class TestSetupPlayers : public QObject
{
    Q_OBJECT

private:
    std::vector<std::unique_ptr<Player>> ownedPlayers;
    QVector<Player *> players;
    QJsonArray resourceGroups;

    QVariantMap expectedTile(const QJsonObject &input) const;

private slots:
    void init();
    void cleanup();
    void playerOrderPreservesPlayersAndAssignsIds_data();
    void playerOrderPreservesPlayersAndAssignsIds();
    void startingSetsAreDistinctAndComeFromStorage_data();
    void startingSetsAreDistinctAndComeFromStorage();
    void constructorPlacesThreeTilesPerPlayer();
    void generateTileDataPreservesFieldsAndCoordinates();
    void generatingAgainReplacesStartingTiles();
    void destructionKeepsBorrowedPlayersAlive();
};

void TestSetupPlayers::init()
{
    QFile file(":/resources/storage.json");
    QVERIFY2(file.open(QIODevice::ReadOnly), "Link resource.qrc into this test target.");
    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    QCOMPARE(error.error, QJsonParseError::NoError);
    QVERIFY(document.isObject());
    resourceGroups = document.object().value("startingTiles").toArray();
    QCOMPARE(resourceGroups.size(), 5);
    for (const auto &group : resourceGroups)
        QCOMPARE(group.toArray().size(), 3);

    for (int i = 0; i < 4; ++i)
    {
        auto player = std::make_unique<Player>(QString("Player %1").arg(i));
        player->setPlayerId(-1);
        player->setMyTurn(false);
        int id = 0;
        for (const auto &row : player->hexagonGrid()->getTiles())
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
        players.append(player.get());
        ownedPlayers.push_back(std::move(player));
    }
}

void TestSetupPlayers::cleanup()
{
    players.clear();
    ownedPlayers.clear();
    resourceGroups = QJsonArray{};
}

QVariantMap TestSetupPlayers::expectedTile(const QJsonObject &input) const
{
    return {{"m_tileNumber", input.value("tileNum").toString().toInt()},
            {"m_habitats", input.value("habitats").toArray().toVariantList()},
            {"m_animals", input.value("wildlife").toArray().toVariantList()},
            {"m_rotation", input.value("rotation").toInt()}};
}

void TestSetupPlayers::playerOrderPreservesPlayersAndAssignsIds_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("solo") << 1;
    QTest::newRow("two") << 2;
    QTest::newRow("three") << 3;
    QTest::newRow("four") << 4;
}

void TestSetupPlayers::playerOrderPreservesPlayersAndAssignsIds()
{
    QFETCH(int, count);
    auto participants = players.mid(0, count);
    const auto before = participants;
    SetupPlayers setup(participants);
    const auto order = setup.playerOrder();
    QCOMPARE(order.size(), count);
    QVERIFY(participants == before);
    QSet<Player *> seen;
    for (int i = 0; i < count; ++i)
    {
        QVERIFY(order[i]);
        QVERIFY(before.contains(order[i]));
        QVERIFY(!seen.contains(order[i]));
        seen.insert(order[i]);
        QCOMPARE(order[i]->playerId(), i);
        QCOMPARE(order[i]->myTurn(), i == 0);
    }
    for (int i = 0; i < count; ++i)
        QCOMPARE(before[i]->playerName(), QString("Player %1").arg(i));
}

void TestSetupPlayers::startingSetsAreDistinctAndComeFromStorage_data()
{
    playerOrderPreservesPlayersAndAssignsIds_data();
}

void TestSetupPlayers::startingSetsAreDistinctAndComeFromStorage()
{
    QFETCH(int, count);
    auto participants = players.mid(0, count);
    SetupPlayers setup(participants);
    const auto selected = setup.setupStartingTiles();
    QCOMPARE(selected.size(), std::size_t(count));
    QSet<int> usedGroups;
    for (int i = 0; i < count; ++i)
    {
        QVERIFY(selected.count(i) == 1);
        const auto &tiles = selected.at(i);
        QCOMPARE(tiles.size(), 3);
        int matchedGroup = -1;
        for (int group = 0; group < resourceGroups.size(); ++group)
        {
            const auto entries = resourceGroups[group].toArray();
            if (tiles[0].tileNumber() == entries[0].toObject().value("tileNum").toString().toInt())
                matchedGroup = group;
        }
        QVERIFY(matchedGroup >= 0);
        QVERIFY(!usedGroups.contains(matchedGroup));
        usedGroups.insert(matchedGroup);
        const auto entries = resourceGroups[matchedGroup].toArray();
        for (int j = 0; j < 3; ++j)
            QCOMPARE(tiles[j].toVariant().toMap(), expectedTile(entries[j].toObject()));
    }
}

void TestSetupPlayers::constructorPlacesThreeTilesPerPlayer()
{
    SetupPlayers setup(players);
    const QList<QPoint> positions{{11, 9}, {12, 9}, {12, 10}};
    QSet<int> allStartingIds;
    for (Player *player : setup.playerOrder())
    {
        const auto board = player->hexagonGrid()->getTiles();
        int placed = 0;
        for (const auto &row : board)
            for (const Hexagon *hexagon : row)
                if (hexagon->getTileData()->getPlacedTile())
                    ++placed;
        QCOMPARE(placed, 3);
        for (const QPoint &position : positions)
        {
            const TileData *tile = board[position.x()][position.y()]->getTileData();
            QVERIFY(tile->getPlacedTile());
            QVERIFY(!tile->getIsValid());
            QVERIFY(tile->getPlacedToken().isEmpty());
            QVERIFY(!tile->getHabitats().isEmpty());
            QVERIFY(!tile->getAnimals().isEmpty());
            QCOMPARE(tile->getRow(), position.x());
            QCOMPARE(tile->getCol(), position.y());
            QVERIFY(tile->getId() >= 1 && tile->getId() <= 15);
            QVERIFY(!allStartingIds.contains(tile->getId()));
            allStartingIds.insert(tile->getId());
        }
    }
    QCOMPARE(allStartingIds.size(), 12);
}

void TestSetupPlayers::generateTileDataPreservesFieldsAndCoordinates()
{
    SetupPlayers setup(players);
    QVector<Tile> input{
        Tile(501, {"forest"}, {"bear"}, 0),
        Tile(502, {"forest", "lake"}, {"fox", "salmon"}, 60),
        Tile(503, {"mountain"}, {"elk", "hawk"}, 300)
    };
    setup.generateTileData(players[0], input);
    const QList<QPoint> positions{{11, 9}, {12, 9}, {12, 10}};
    const auto board = players[0]->hexagonGrid()->getTiles();
    for (int i = 0; i < 3; ++i)
    {
        const TileData *tile = board[positions[i].x()][positions[i].y()]->getTileData();
        QCOMPARE(tile->getId(), input[i].tileNumber());
        QCOMPARE(tile->getHabitats(), input[i].habitats());
        QCOMPARE(tile->getAnimals(), input[i].animals());
        QCOMPARE(tile->getRotation(), input[i].rotation());
        QCOMPARE(tile->getRow(), positions[i].x());
        QCOMPARE(tile->getCol(), positions[i].y());
        QVERIFY(tile->getPlacedTile());
        QVERIFY(!tile->getIsValid());
        QVERIFY(tile->getPlacedToken().isEmpty());
    }
    input[1].setRotation(180);
    QCOMPARE(board[12][9]->getTileData()->getRotation(), 60);
}

void TestSetupPlayers::generatingAgainReplacesStartingTiles()
{
    SetupPlayers setup(players);
    QVector<Tile> first{
        Tile(501, {"forest"}, {"bear"}, 0),
        Tile(502, {"forest"}, {"bear"}, 60),
        Tile(503, {"forest"}, {"bear"}, 120)
    };
    QVector<Tile> second{
        Tile(601, {"mountain"}, {"hawk"}, 0),
        Tile(602, {"mountain"}, {"hawk"}, 180),
        Tile(603, {"mountain"}, {"hawk"}, 240)
    };
    setup.generateTileData(players[0], first);
    setup.generateTileData(players[0], second);
    const QList<QPoint> positions{{11, 9}, {12, 9}, {12, 10}};
    const auto board = players[0]->hexagonGrid()->getTiles();
    for (int i = 0; i < 3; ++i)
    {
        const TileData *tile = board[positions[i].x()][positions[i].y()]->getTileData();
        QCOMPARE(tile->getId(), second[i].tileNumber());
        QCOMPARE(tile->getHabitats(), second[i].habitats());
        QCOMPARE(tile->getAnimals(), second[i].animals());
        QCOMPARE(tile->getRotation(), second[i].rotation());
    }
}

void TestSetupPlayers::destructionKeepsBorrowedPlayersAlive()
{
    {
        SetupPlayers setup(players);
        QCOMPARE(setup.playerOrder().size(), 4);
    }
    for (int i = 0; i < players.size(); ++i)
    {
        QCOMPARE(players[i]->playerName(), QString("Player %1").arg(i));
        QVERIFY(players[i]->hexagonGrid());
        QCOMPARE(players[i]->hexagonGrid()->getRows(), 30);
    }
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestSetupPlayers test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_setup_players.moc"
