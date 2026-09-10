#include <QtTest>
#include <QApplication>
#include <QPoint>
#include <set>
#include "hexagonGrid.h"

class TestHexagonGrid : public QObject
{
    Q_OBJECT
private slots:
    void defaultGridIsEmpty();
    void dimensions_data();
    void dimensions();
    void exactNeighbours_data();
    void exactNeighbours();
    void neighbourInvariants_data();
    void neighbourInvariants();
    void selectableNeighbours_data();
    void selectableNeighbours();
    void selectableCellsSurroundAllPlacedTiles();
};

void TestHexagonGrid::defaultGridIsEmpty()
{
    HexagonGrid grid;
    QCOMPARE(grid.getRows(), 0);
    QCOMPARE(grid.getCols(), 0);
    QVERIFY(grid.getTiles().isEmpty());
}

void TestHexagonGrid::dimensions_data()
{
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");
    QTest::newRow("single") << 1 << 1;
    QTest::newRow("rectangle") << 3 << 5;
    QTest::newRow("empty") << 0 << 0;
}

void TestHexagonGrid::dimensions()
{
    QFETCH(int, rows);
    QFETCH(int, cols);
    HexagonGrid grid(rows, cols, 20.0f);
    QCOMPARE(grid.getRows(), rows);
    QCOMPARE(grid.getCols(), cols);
    QCOMPARE(grid.getTiles().size(), rows);
    for (int row = 0; row < rows; ++row)
    {
        QCOMPARE(grid.getTiles()[row].size(), cols);
        for (int col = 0; col < cols; ++col)
        {
            QVERIFY(grid.getTiles()[row][col]);
            const TileData *tile = grid.getTiles()[row][col]->getTileData();
            QVERIFY(tile);
            QCOMPARE(tile->getRow(), row);
            QCOMPARE(tile->getCol(), col);
            QVERIFY(!tile->getPlacedTile());
            QVERIFY(!tile->getIsValid());
        }
    }
}

void TestHexagonGrid::exactNeighbours_data()
{
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("col");
    QTest::addColumn<QList<QPoint>>("expected");
    QTest::newRow("even-interior") << 2 << 2
        << QList<QPoint>{{2,1},{1,1},{1,2},{2,3},{3,1},{3,2}};
    QTest::newRow("odd-interior") << 1 << 2
        << QList<QPoint>{{1,1},{0,2},{0,3},{1,3},{2,2},{2,3}};
    QTest::newRow("top-left") << 0 << 0 << QList<QPoint>{{0,1},{1,0}};
    QTest::newRow("top-right") << 0 << 4 << QList<QPoint>{{0,3},{1,3},{1,4}};
    QTest::newRow("bottom-left-odd") << 3 << 0
        << QList<QPoint>{{2,0},{2,1},{3,1}};
    QTest::newRow("bottom-right-odd") << 3 << 4
        << QList<QPoint>{{3,3},{2,4}};
}

void TestHexagonGrid::exactNeighbours()
{
    QFETCH(int, row);
    QFETCH(int, col);
    QFETCH(QList<QPoint>, expected);
    HexagonGrid grid(4, 5, 20.0f);
    const auto actual = grid.getNeighbours(row, col);
    QCOMPARE(actual.size(), expected.size());
    for (const QPoint &point : expected)
        QVERIFY(actual.contains(std::make_pair(point.x(), point.y())));
}

void TestHexagonGrid::neighbourInvariants_data()
{
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");
    QTest::newRow("single") << 1 << 1;
    QTest::newRow("one-row") << 1 << 5;
    QTest::newRow("one-column") << 5 << 1;
    QTest::newRow("rectangle") << 4 << 5;
}

void TestHexagonGrid::neighbourInvariants()
{
    QFETCH(int, rows);
    QFETCH(int, cols);
    HexagonGrid grid(rows, cols, 20.0f);
    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < cols; ++col)
        {
            const auto neighbours = grid.getNeighbours(row, col);
            QVERIFY(neighbours.size() <= 6);
            std::set<std::pair<int, int>> unique;
            for (const auto &cell : neighbours)
            {
                QVERIFY(cell.first >= 0 && cell.first < rows);
                QVERIFY(cell.second >= 0 && cell.second < cols);
                QVERIFY(cell != std::make_pair(row, col));
                QVERIFY(unique.insert(cell).second);
                QVERIFY(grid.getNeighbours(cell.first, cell.second).contains(std::make_pair(row, col)));
            }
            if (rows == 1 && cols == 1)
                QVERIFY(neighbours.isEmpty());
        }
}

void TestHexagonGrid::selectableNeighbours_data()
{
    QTest::addColumn<int>("row");
    QTest::newRow("even") << 2;
    QTest::newRow("odd") << 1;
}

void TestHexagonGrid::selectableNeighbours()
{
    QFETCH(int, row);
    HexagonGrid grid(4, 5, 20.0f);
    grid.getTiles()[row][2]->getTileData()->setPlacedTile(true);
    grid.getTiles()[row][1]->getTileData()->setPlacedTile(true);
    const auto expected = grid.getNeighbours(row, 2);
    for (int repeat = 0; repeat < 2; ++repeat)
    {
        grid.setValidNeighbours(row, 2);
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 5; ++c)
            {
                const TileData *tile = grid.getTiles()[r][c]->getTileData();
                const bool occupied = r == row && (c == 1 || c == 2);
                QCOMPARE(tile->getPlacedTile(), occupied);
                const bool shouldBeValid = !occupied && expected.contains(std::make_pair(r,c));
                QCOMPARE(tile->getIsValid(), shouldBeValid);
            }
    }
}

void TestHexagonGrid::selectableCellsSurroundAllPlacedTiles()
{
    HexagonGrid grid(4, 5, 20.0f);
    grid.drawSelectableTiles();
    for (const auto &row : grid.getTiles())
        for (const Hexagon *hexagon : row)
            QVERIFY(!hexagon->getTileData()->getIsValid());

    grid.getTiles()[0][0]->getTileData()->setPlacedTile(true);
    grid.getTiles()[3][4]->getTileData()->setPlacedTile(true);
    const QList<QPoint> expected{{0,1},{1,0},{3,3},{2,4}};
    grid.drawSelectableTiles();
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 5; ++col)
            QCOMPARE(grid.getTiles()[row][col]->getTileData()->getIsValid(),
                     expected.contains(QPoint(row,col)));
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestHexagonGrid tests;
    return QTest::qExec(&tests, argc, argv);
}
#include "test_hexagon_grid.moc"