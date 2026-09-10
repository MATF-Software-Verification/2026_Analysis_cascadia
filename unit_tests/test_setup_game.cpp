#include <QtTest>
#include <QFile>
#include <QSet>

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "setupGame.h"

class TestSetupGame : public QObject
{
    Q_OBJECT

private:
    QVector<QString> names;
    std::unique_ptr<SetupGame> setup;

private slots:
    void init();
    void cleanup();
    void constructorPreservesNamesAndSeparatesMarket();
    void tileCounts_data();
    void tileCounts();
    void invalidPlayerCounts_data();
    void invalidPlayerCounts();
    void tokenSupplyContainsTwentyOfEachAnimal();
    void drawingTilesRemovesExactlyFour_data();
    void drawingTilesRemovesExactlyFour();
    void drawingMixedTokensRemovesExactlyFour_data();
    void drawingMixedTokensRemovesExactlyFour();
    void identicalTokenDetection_data();
    void identicalTokenDetection();
    void shufflingPreservesTokens_data();
    void shufflingPreservesTokens();
    void shufflingPreservesCompleteTiles_data();
    void shufflingPreservesCompleteTiles();
};

void TestSetupGame::init()
{
    // Qt Test runs this before every test/data row.
    QVERIFY2(QFile::exists(":/resources/storage.json"), "Link resource.qrc into this test target.");
    names = {"Ana", "Julijana"};
    setup = std::make_unique<SetupGame>(2, names);
}

void TestSetupGame::cleanup()
{
    setup.reset();
    names.clear();
}

void TestSetupGame::constructorPreservesNamesAndSeparatesMarket()
{
    QCOMPARE(setup->numOfPlayers(), 2);
    QCOMPARE(setup->playerNames(), names);
    names[0] = "Changed";
    QCOMPARE(setup->playerNames()[0], QString("Ana"));

    // Two-player setup selects 43 tiles, with four moved into the market.
    QCOMPARE(setup->currentTiles().size(), 4);
    QCOMPARE(setup->allTiles().size(), 39);
    QSet<int> ids;
    const auto tiles = setup->currentTiles() + setup->allTiles();
    for (const Tile &tile : tiles)
    {
        QVERIFY(!ids.contains(tile.tileNumber()));
        ids.insert(tile.tileNumber());
    }
    QCOMPARE(ids.size(), 43);

    QCOMPARE(setup->currentTokens().size(), 4);
    QCOMPARE(setup->allTokens().size(), 96);
    const auto tokens = setup->currentTokens() + setup->allTokens();
    for (const QString &animal : {QString("bear"), QString("fox"), QString("salmon"),
                                  QString("hawk"), QString("elk")})
        QCOMPARE(tokens.count(animal), 20);
    QSet<QString> marketTypes;
    for (const QString &token : setup->currentTokens())
        marketTypes.insert(token);
    QVERIFY(marketTypes.size() > 1);
}

void TestSetupGame::tileCounts_data()
{
    QTest::addColumn<int>("players");
    QTest::addColumn<int>("expected");

    QTest::newRow("solo") << 1 << 43;
    QTest::newRow("two") << 2 << 43;
    QTest::newRow("three") << 3 << 63;
    QTest::newRow("four") << 4 << 83;
}

void TestSetupGame::tileCounts()
{
    QFETCH(int, players);
    QFETCH(int, expected);
    const auto tiles = setup->setupTiles(players);
    QCOMPARE(tiles.size(), expected);
    QSet<int> ids;
    for (const Tile &tile : tiles)
    {
        QVERIFY(tile.tileNumber() >= 16 && tile.tileNumber() <= 100);
        QVERIFY(!ids.contains(tile.tileNumber()));
        ids.insert(tile.tileNumber());
    }
}

void TestSetupGame::invalidPlayerCounts_data()
{
    QTest::addColumn<int>("players");
    QTest::newRow("negative") << -1;
    QTest::newRow("zero") << 0;
    QTest::newRow("above-maximum") << 5;
}

void TestSetupGame::invalidPlayerCounts()
{
    QFETCH(int, players);
    QVERIFY_EXCEPTION_THROWN(setup->setupTiles(players), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(SetupGame(players, names), std::invalid_argument);
}

void TestSetupGame::tokenSupplyContainsTwentyOfEachAnimal()
{
    const auto numbers = setup->tokenNumbers();
    const auto tokens = setup->setupTokens();
    QCOMPARE(numbers.size(), std::size_t(5));
    QCOMPARE(tokens.size(), 100);
    for (const QString &animal : {QString("bear"), QString("fox"), QString("salmon"),
                                  QString("hawk"), QString("elk")})
    {
        QVERIFY(numbers.count(animal) == 1);
        QCOMPARE(numbers.at(animal), 20);
        QCOMPARE(tokens.count(animal), 20);
    }
}

void TestSetupGame::drawingTilesRemovesExactlyFour_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("exactly-four") << 4;
    QTest::newRow("with-remainder") << 7;
}

void TestSetupGame::drawingTilesRemovesExactlyFour()
{
    QFETCH(int, count);
    QVector<Tile> tiles;
    for (int i = 0; i < count; ++i)
        tiles.append(Tile(100 + i, {"forest"}, {"bear"}, (i % 6) * 60));
    const auto before = tiles;
    const auto drawn = setup->setupCurrentTiles(tiles);
    QCOMPARE(drawn.size(), 4);
    QCOMPARE(tiles.size(), count - 4);
    for (int i = 0; i < 4; ++i)
        QCOMPARE(drawn[i].toVariant().toMap(), before[i].toVariant().toMap());
    for (int i = 0; i < count - 4; ++i)
        QCOMPARE(tiles[i].toVariant().toMap(), before[i + 4].toVariant().toMap());
}

void TestSetupGame::drawingMixedTokensRemovesExactlyFour_data()
{
    QTest::addColumn<QStringList>("input");
    QTest::newRow("exactly-four") << QStringList({"bear", "fox", "hawk", "elk"});
    QTest::newRow("with-remainder") << QStringList({"bear", "bear", "bear", "fox", "elk", "salmon"});
}

void TestSetupGame::drawingMixedTokensRemovesExactlyFour()
{
    QFETCH(QStringList, input);
    QVector<QString> tokens = input;
    const auto drawn = setup->setupCurrentTokens(tokens);
    QCOMPARE(drawn, input.mid(0, 4));
    QCOMPARE(tokens, input.mid(4));
}

void TestSetupGame::identicalTokenDetection_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expected");
    QTest::newRow("empty") << QStringList{} << false;
    QTest::newRow("single") << QStringList{"bear"} << true;
    QTest::newRow("all-equal") << QStringList({"bear", "bear", "bear", "bear"}) << true;
    QTest::newRow("early-difference") << QStringList({"bear", "fox", "bear", "bear"}) << false;
    QTest::newRow("late-difference") << QStringList({"bear", "bear", "bear", "fox"}) << false;
}

void TestSetupGame::identicalTokenDetection()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expected);
    const auto before = tokens;
    QCOMPARE(setup->checkIfAllTokensAreSame(tokens), expected);
    QCOMPARE(tokens, before);
}

void TestSetupGame::shufflingPreservesTokens_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::newRow("empty") << QStringList{};
    QTest::newRow("single") << QStringList{"bear"};
    QTest::newRow("duplicates") << QStringList({"bear", "bear", "fox", "salmon", "elk"});
}

void TestSetupGame::shufflingPreservesTokens()
{
    QFETCH(QStringList, tokens);
    auto expected = tokens;
    SetupGame::shuffleTokens(tokens);
    std::sort(tokens.begin(), tokens.end());
    std::sort(expected.begin(), expected.end());
    QCOMPARE(tokens, expected);
}

void TestSetupGame::shufflingPreservesCompleteTiles_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("empty") << 0;
    QTest::newRow("single") << 1;
    QTest::newRow("multiple") << 6;
}

void TestSetupGame::shufflingPreservesCompleteTiles()
{
    QFETCH(int, count);
    QVector<Tile> tiles;
    for (int i = 0; i < count; ++i)
        tiles.append(Tile(i, {"forest", "lake"}, {"bear", "salmon"}, i * 60));
    const auto expected = tiles;
    setup->shuffleTiles(tiles);
    std::sort(tiles.begin(), tiles.end(), [](const Tile &a, const Tile &b) {
        return a.tileNumber() < b.tileNumber();
    });
    QCOMPARE(tiles.size(), expected.size());
    for (int i = 0; i < count; ++i)
        QCOMPARE(tiles[i].toVariant().toMap(), expected[i].toVariant().toMap());
}

int main(int argc, char **argv)
{
    TestSetupGame test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_setup_game.moc"
