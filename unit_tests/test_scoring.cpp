#include <QtTest>
#include <QApplication>

#include <memory>

#include "scoring.h"

class TestScoring : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Scoring> scoring;
    HexagonGrid *grid = nullptr;
    void place(int row, int col, const QString &animal,
               const QVector<QString> &habitats = {"forest"}, int rotation = 0);

private slots:
    void init();
    void cleanup();
    void emptyBoardScoresZero();
    void bearGroups_data();
    void bearGroups();
    void isolatedHawks_data();
    void isolatedHawks();
    void adjacentHawksDoNotScore();
    void foxCountsDistinctAnimals();
    void elkStraightLines_data();
    void elkStraightLines();
    void bentElkGroupIsNotOneStraightLine();
    void salmonRuns_data();
    void salmonRuns();
    void separateSalmonRunsAddTheirScores();
    void bentSalmonRunIsAllowed();
    void branchingSalmonGroupDoesNotScore();
    void largestHabitatRegionWins();
    void habitatConnectionDependsOnFacingSides_data();
    void habitatConnectionDependsOnFacingSides();
};

void TestScoring::init()
{
    player = std::make_unique<Player>(QString("Scoring fixture"));
    player->setPlayerId(0);
    grid = player->hexagonGrid();
    int id = 0;
    for (const auto &row : grid->getTiles())
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
    scoring = std::make_unique<Scoring>(*player);
}

void TestScoring::cleanup()
{
    scoring.reset();
    player.reset();
    grid = nullptr;
}

void TestScoring::place(int row, int col, const QString &animal,
                        const QVector<QString> &habitats, int rotation)
{
    TileData *tile = grid->getTiles()[row][col]->getTileData();
    tile->setPlacedTile(true);
    tile->setPlacedToken(animal);
    tile->setHabitats(habitats);
    tile->setAnimals(animal.isEmpty() ? QVector<QString>{} : QVector<QString>{animal});
    tile->setRotation(rotation);
}

void TestScoring::emptyBoardScoresZero()
{
    const auto animals = scoring->playerAnimalScore();
    const auto habitats = scoring->playerHabitatScores();
    QCOMPARE(animals.size(), std::size_t(5));
    QCOMPARE(habitats.size(), std::size_t(5));
    for (const QString &name : {QString("bear"), QString("elk"), QString("hawk"), QString("fox"), QString("salmon")})
    {
        QVERIFY(animals.count(name) == 1);
        QCOMPARE(animals.at(name), 0);
    }
    for (const QString &name : {QString("forest"), QString("lake"), QString("mountain"), QString("swamp"), QString("desert")})
    {
        QVERIFY(habitats.count(name) == 1);
        QCOMPARE(habitats.at(name), 0);
    }
}

void TestScoring::bearGroups_data()
{
    QTest::addColumn<QList<QPoint>>("positions");
    QTest::addColumn<int>("expected");
    QTest::newRow("single") << QList<QPoint>{{2,2}} << 0;
    QTest::newRow("one-pair") << QList<QPoint>{{2,2},{2,3}} << 4;
    QTest::newRow("three-connected") << QList<QPoint>{{2,2},{2,3},{2,4}} << 0;
    QTest::newRow("two-pairs") << QList<QPoint>{{2,2},{2,3},{6,2},{6,3}} << 11;
    QTest::newRow("three-pairs") << QList<QPoint>{{2,2},{2,3},{6,2},{6,3},{10,2},{10,3}} << 19;
    QTest::newRow("four-pairs") << QList<QPoint>{{2,2},{2,3},{6,2},{6,3},{10,2},{10,3},{14,2},{14,3}} << 27;
}

void TestScoring::bearGroups()
{
    QFETCH(QList<QPoint>, positions);
    QFETCH(int, expected);
    for (const QPoint &position : positions)
        place(position.x(), position.y(), "bear");
    QCOMPARE(scoring->calculateBearScore(grid), expected);
}

void TestScoring::isolatedHawks_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<int>("expected");
    QTest::newRow("one") << 1 << 2;
    QTest::newRow("two") << 2 << 5;
    QTest::newRow("eight") << 8 << 26;
    QTest::newRow("nine-cap") << 9 << 26;
}

void TestScoring::isolatedHawks()
{
    QFETCH(int, count);
    QFETCH(int, expected);
    for (int i = 0; i < count; ++i)
        place(2, 2 * i, "hawk");
    QCOMPARE(scoring->calculateHawkScore(grid), expected);
}

void TestScoring::adjacentHawksDoNotScore()
{
    place(2, 2, "hawk");
    place(2, 3, "hawk");
    QCOMPARE(scoring->calculateHawkScore(grid), 0);
    place(8, 8, "hawk");
    QCOMPARE(scoring->calculateHawkScore(grid), 2);
}

void TestScoring::foxCountsDistinctAnimals()
{
    place(2, 2, "fox");
    QCOMPARE(scoring->calculateFoxScore(grid), 0);
    place(2, 1, "bear");
    place(1, 1, "bear");
    QCOMPARE(scoring->calculateFoxScore(grid), 1);
    place(1, 2, "hawk");
    place(2, 3, "salmon");
    QCOMPARE(scoring->calculateFoxScore(grid), 3);
    place(3, 1, "elk");
    QCOMPARE(scoring->calculateFoxScore(grid), 4);
}

void TestScoring::elkStraightLines_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<int>("expected");
    QTest::newRow("one") << 1 << 2;
    QTest::newRow("two") << 2 << 5;
    QTest::newRow("three") << 3 << 9;
    QTest::newRow("four") << 4 << 13;
}

void TestScoring::elkStraightLines()
{
    QFETCH(int, count);
    QFETCH(int, expected);
    for (int col = 0; col < count; ++col)
        place(2, 2 + col, "elk");
    QCOMPARE(scoring->calculateElkScore(grid), expected);
}

void TestScoring::bentElkGroupIsNotOneStraightLine()
{
    place(2, 2, "elk");
    place(2, 3, "elk");
    place(3, 3, "elk");
    QCOMPARE(scoring->calculateElkScore(grid), 7);
}

void TestScoring::salmonRuns_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<int>("expected");
    QTest::newRow("one") << 1 << 2;
    QTest::newRow("two") << 2 << 4;
    QTest::newRow("three") << 3 << 7;
    QTest::newRow("four") << 4 << 11;
    QTest::newRow("five") << 5 << 15;
    QTest::newRow("six") << 6 << 20;
    QTest::newRow("seven") << 7 << 26;
    QTest::newRow("eight-cap") << 8 << 26;
}

void TestScoring::salmonRuns()
{
    QFETCH(int, count);
    QFETCH(int, expected);
    for (int col = 0; col < count; ++col)
        place(2, 2 + col, "salmon");
    QCOMPARE(scoring->calculateSalmonScore(grid), expected);
}

void TestScoring::separateSalmonRunsAddTheirScores()
{
    place(2, 2, "salmon");
    place(2, 3, "salmon");
    place(2, 4, "salmon");
    place(8, 2, "salmon");
    place(8, 3, "salmon");
    QCOMPARE(scoring->calculateSalmonScore(grid), 11);
}

void TestScoring::bentSalmonRunIsAllowed()
{
    place(2, 2, "salmon");
    place(2, 3, "salmon");
    place(3, 3, "salmon");
    QCOMPARE(scoring->calculateSalmonScore(grid), 7);
}

void TestScoring::branchingSalmonGroupDoesNotScore()
{
    place(4, 4, "salmon");
    place(4, 3, "salmon");
    place(3, 4, "salmon");
    place(5, 4, "salmon");
    QCOMPARE(scoring->calculateSalmonScore(grid), 0);
}

void TestScoring::largestHabitatRegionWins()
{
    place(2, 2, "");
    place(2, 3, "");
    place(2, 4, "");
    place(8, 2, "");
    place(8, 3, "");
    const auto result = scoring->calculatePlayerHabitatsScore(*player);
    QVERIFY(result.count("forest") == 1);
    QCOMPARE(result.at("forest"), 3);
}

void TestScoring::habitatConnectionDependsOnFacingSides_data()
{
    QTest::addColumn<int>("rotation");
    QTest::addColumn<int>("expectedForest");
    QTest::newRow("mismatched-edge") << 0 << 1;
    QTest::newRow("matching-after-half-turn") << 180 << 2;
}

void TestScoring::habitatConnectionDependsOnFacingSides()
{
    QFETCH(int, rotation);
    QFETCH(int, expectedForest);
    place(2, 2, "", {"forest", "lake"}, rotation);
    place(2, 3, "", {"forest"});
    const auto result = scoring->calculatePlayerHabitatsScore(*player);
    QVERIFY(result.count("forest") == 1);
    QVERIFY(result.count("lake") == 1);
    QCOMPARE(result.at("forest"), expectedForest);
    QCOMPARE(result.at("lake"), 1);
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestScoring test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_scoring.moc"