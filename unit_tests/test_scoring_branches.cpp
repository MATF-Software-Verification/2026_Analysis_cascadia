#include <QtTest>
#include <QApplication>

#include <memory>
#include <set>
#include <tuple>
#include <vector>

#include "scoring.h"

class TestScoringBranches : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Scoring> scoring;
    HexagonGrid *grid = nullptr;

private slots:
    void init();
    void cleanup();
    void oppositeSides_data();
    void oppositeSides();
    void habitatRotations_data();
    void habitatRotations();
    void singleHabitatCoversEverySide();
    void neighboursWithDirections_data();
    void neighboursWithDirections();
    void tokenNeighbourFiltering();
    void standaloneElkLineSplitting_data();
    void standaloneElkLineSplitting();
    void salmonTriangleContainsEachCellOnce();
    void connectedHabitatCycleCountsEachTileOnce();
};

void TestScoringBranches::init()
{
    player = std::make_unique<Player>(QString("Branch fixture"));
    player->setPlayerId(0);
    grid = player->hexagonGrid();
    int id = 0;
    for (const auto &row : grid->getTiles())
        for (Hexagon *hexagon : row)
        {
            auto *tile = hexagon->getTileData();
            tile->setId(id++);
            tile->setIndex(0);
            tile->setPlacedTile(false);
            tile->setIsValid(false);
            tile->setPlacedToken("");
            tile->setAnimals({});
            tile->setHabitats({});
            tile->setRotation(0);
        }
    scoring = std::make_unique<Scoring>(*player);
}

void TestScoringBranches::cleanup()
{
    scoring.reset();
    player.reset();
    grid = nullptr;
}

void TestScoringBranches::oppositeSides_data()
{
    QTest::addColumn<QString>("side");
    QTest::addColumn<QString>("expected");
    QTest::newRow("top-left") << QString("Top-Left") << QString("Bottom-Right");
    QTest::newRow("top-right") << QString("Top-Right") << QString("Bottom-Left");
    QTest::newRow("right") << QString("Right") << QString("Left");
    QTest::newRow("bottom-right") << QString("Bottom-Right") << QString("Top-Left");
    QTest::newRow("bottom-left") << QString("Bottom-Left") << QString("Top-Right");
    QTest::newRow("left") << QString("Left") << QString("Right");
    QTest::newRow("unrecognized") << QString("not-a-side") << QString("Unknown");
}

void TestScoringBranches::oppositeSides()
{
    QFETCH(QString, side);
    QFETCH(QString, expected);
    QCOMPARE(scoring->getAdjacentSide(side), expected);
    if (expected != "Unknown")
        QCOMPARE(scoring->getAdjacentSide(expected), side);
}

void TestScoringBranches::habitatRotations_data()
{
    QTest::addColumn<int>("rotation");
    QTest::addColumn<QStringList>("forestSides");
    QTest::newRow("0") << 0 << QStringList({"Top-Left", "Bottom-Left", "Left"});
    QTest::newRow("60") << 60 << QStringList({"Top-Left", "Top-Right", "Left"});
    QTest::newRow("120") << 120 << QStringList({"Top-Left", "Top-Right", "Right"});
    QTest::newRow("180") << 180 << QStringList({"Top-Right", "Right", "Bottom-Right"});
    QTest::newRow("240") << 240 << QStringList({"Right", "Bottom-Right", "Bottom-Left"});
    QTest::newRow("300") << 300 << QStringList({"Bottom-Right", "Bottom-Left", "Left"});
}

void TestScoringBranches::habitatRotations()
{
    QFETCH(int, rotation);
    QFETCH(QStringList, forestSides);
    auto *tile = grid->getTiles()[2][2]->getTileData();
    tile->setHabitats({"forest", "lake"});
    tile->setRotation(rotation);
    const auto actual = scoring->habitatSides(grid->getTiles(), 2, 2);
    const QStringList sides{"Top-Left", "Top-Right", "Right", "Bottom-Right", "Bottom-Left", "Left"};
    QCOMPARE(actual.size(), std::size_t(6));
    for (const QString &side : sides)
    {
        QVERIFY(actual.count(side) == 1);
        QCOMPARE(actual.at(side), forestSides.contains(side) ? QString("forest") : QString("lake"));
    }
}

void TestScoringBranches::singleHabitatCoversEverySide()
{
    auto *tile = grid->getTiles()[2][2]->getTileData();
    tile->setHabitats({"mountain"});
    tile->setRotation(240);
    const auto actual = scoring->habitatSides(grid->getTiles(), 2, 2);
    QCOMPARE(actual.size(), std::size_t(6));
    for (const auto &entry : actual)
        QCOMPARE(entry.second, QString("mountain"));
}

void TestScoringBranches::neighboursWithDirections_data()
{
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("col");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("even-interior") << 2 << 2 << QStringList({
        "1,1:Top-Left", "1,2:Top-Right", "2,3:Right",
        "3,2:Bottom-Right", "3,1:Bottom-Left", "2,1:Left"});
    QTest::newRow("odd-interior") << 1 << 2 << QStringList({
        "0,2:Top-Left", "0,3:Top-Right", "1,3:Right",
        "2,3:Bottom-Right", "2,2:Bottom-Left", "1,1:Left"});
    QTest::newRow("top-left") << 0 << 0 << QStringList({"0,1:Right", "1,0:Bottom-Right"});
    QTest::newRow("bottom-right") << 29 << 29 << QStringList({"28,29:Top-Left", "29,28:Left"});
}

void TestScoringBranches::neighboursWithDirections()
{
    QFETCH(int, row);
    QFETCH(int, col);
    QFETCH(QStringList, expected);
    QStringList actual;
    for (const auto &cell : scoring->getNeighbours(grid->getTiles(), row, col))
        actual.append(QString("%1,%2:%3").arg(std::get<0>(cell)).arg(std::get<1>(cell)).arg(std::get<2>(cell)));
    actual.sort();
    expected.sort();
    QCOMPARE(actual, expected);
}

void TestScoringBranches::tokenNeighbourFiltering()
{
    const std::pair<int, int> origin{0, 0};
    QVERIFY(scoring->findNeighborsWithToken(grid, origin, "bear").isEmpty());
    grid->getTiles()[0][1]->getTileData()->setPlacedToken("bear");
    grid->getTiles()[1][0]->getTileData()->setPlacedToken("fox");
    grid->getTiles()[8][8]->getTileData()->setPlacedToken("bear");
    const auto actual = scoring->findNeighborsWithToken(grid, origin, "bear");
    QCOMPARE(actual.size(), 1);
    QCOMPARE(actual.first().first, 0);
    QCOMPARE(actual.first().second, 1);
    QVERIFY(scoring->findNeighborsWithToken(grid, origin, "hawk").isEmpty());
}

void TestScoringBranches::standaloneElkLineSplitting_data()
{
    QTest::addColumn<int>("length");
    QTest::addColumn<QList<int>>("expectedSizes");
    QTest::newRow("one") << 1 << QList<int>{1};
    QTest::newRow("four-boundary") << 4 << QList<int>{4};
    QTest::newRow("five-remainder") << 5 << QList<int>({4, 1});
    QTest::newRow("eight-exact") << 8 << QList<int>({4, 4});
    QTest::newRow("nine-remainder") << 9 << QList<int>({4, 4, 1});
}

void TestScoringBranches::standaloneElkLineSplitting()
{
    QFETCH(int, length);
    QFETCH(QList<int>, expectedSizes);
    std::vector<std::pair<int, int>> line;
    for (int col = 0; col < length; ++col)
        line.emplace_back(2, col);
    const auto original = line;
    std::vector<std::vector<std::pair<int, int>>> groups;
    std::unordered_set<std::pair<int, int>, Scoring::PairHash> used;
    scoring->processStandaloneElkLine(line, groups, used);
    QCOMPARE(groups.size(), std::size_t(expectedSizes.size()));
    QCOMPARE(used.size(), std::size_t(length));
    std::vector<std::pair<int, int>> flattened;
    for (std::size_t i = 0; i < groups.size(); ++i)
    {
        QCOMPARE(groups[i].size(), std::size_t(expectedSizes[int(i)]));
        flattened.insert(flattened.end(), groups[i].begin(), groups[i].end());
    }
    QVERIFY(flattened == original);
    QVERIFY(line == original);
    for (const auto &cell : original)
        QVERIFY(used.count(cell) == 1);
}

void TestScoringBranches::salmonTriangleContainsEachCellOnce()
{
    const std::vector<std::pair<int, int>> cells{{2,2}, {2,3}, {3,2}};
    for (const auto &cell : cells)
        grid->getTiles()[cell.first][cell.second]->getTileData()->setPlacedToken("salmon");
    const auto run = scoring->findSalmonRun(grid, cells.front());
    QCOMPARE(run.size(), std::size_t(3));
    const std::set<std::pair<int, int>> actual(run.begin(), run.end());
    const std::set<std::pair<int, int>> expected(cells.begin(), cells.end());
    QVERIFY(actual == expected);
    QCOMPARE(scoring->calculateSalmonScore(grid), 7);
}

void TestScoringBranches::connectedHabitatCycleCountsEachTileOnce()
{
    const std::vector<std::pair<int, int>> cells{{2,2}, {2,3}, {3,2}};
    for (const auto &cell : cells)
    {
        auto *tile = grid->getTiles()[cell.first][cell.second]->getTileData();
        tile->setPlacedTile(true);
        tile->setHabitats({"forest"});
    }
    const auto scores = scoring->calculatePlayerHabitatsScore(*player);
    QVERIFY(scores.count("forest") == 1);
    QCOMPARE(scores.at("forest"), 3);
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestScoringBranches test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_scoring_branches.moc"