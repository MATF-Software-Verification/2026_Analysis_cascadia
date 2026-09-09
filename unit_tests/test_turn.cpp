#include <QtTest>
#include "turn.h"
#include "test_fixtures.h"

class TestTurn : public QObject
{
    Q_OBJECT
private slots:
    void constructorPreservesPlacedToken()
    {
        TileData placedTile = makeTile(1);
        TileData tokenTile = makeTile(2);
        TileData newTile = makeTile(3);
        const Turn turn(&placedTile, &tokenTile, "bear", 7, &newTile, "fox", 1, 2, 3);
        QCOMPARE(turn.getPlacedToken(), QString("bear"));
    }
};

QTEST_APPLESS_MAIN(TestTurn)

#include "test_turn.moc"

