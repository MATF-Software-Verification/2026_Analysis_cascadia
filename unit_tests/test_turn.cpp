#include <QtTest>
#include <memory>
#include "turn.h"

static QVariantMap tileInput(int id)
{
    return {{"id", id}, {"row", 2}, {"col", 3}, {"placedTile", true},
            {"isValid", false}, {"placedToken", "bear"}, {"rotation", 120},
            {"animals", QVariantList{QString("bear")}},
            {"habitats", QVariantList{QString("forest")}}};
}

static QVariantMap turnInput()
{
    return {{"placedTileData", tileInput(1)}, {"placedTokenTileData", tileInput(2)},
            {"newTile", tileInput(3)}, {"selectedTileIndex", 1}, {"playerId", 7},
            {"placedToken", "bear"}, {"newToken", "fox"},
            {"selectedTokenIndex", 2}, {"pinecone", 3}};
}

class TestTurn : public QObject
{
    Q_OBJECT
private:
    TileData placedTile;
    TileData tokenTile;
    TileData replacementTile;
    std::unique_ptr<Turn> turn;

private slots:
    void init();
    void cleanup();
    void constructorPreservesPlacedToken();
    void scalarValues_data();
    void scalarValues();
    void pointerSettersReplaceBorrowedTiles();
    void nullPointersAreRejected();
    void serializationContainsFullState();
    void deserializationReadsExplicitInput();
    void roundTripPreservesState();
    void copyPreservesStateAndSharesBorrowedTiles();
    void destructionKeepsBorrowedTilesAlive();
};

void TestTurn::init()
{
    placedTile.fromVariant(tileInput(1));
    tokenTile.fromVariant(tileInput(2));
    replacementTile.fromVariant(tileInput(3));
    placedTile.setIndex(0);
    tokenTile.setIndex(0);
    replacementTile.setIndex(0);

    turn = std::make_unique<Turn>();
    turn->setPlacedTileData(&placedTile);
    turn->setPlacedTokenTileData(&tokenTile);
    turn->setNewTile(&replacementTile);
    turn->setSelectedTileIndex(1);
    turn->setPlayerId(7);
    turn->setPlacedToken("bear");
    turn->setNewToken("fox");
    turn->setSelectedTokenIndex(2);
    turn->setPinecone(3);
}

void TestTurn::cleanup()
{
    turn.reset();
}

void TestTurn::constructorPreservesPlacedToken()
{
    TileData placedTile(0, 0);
    TileData tokenTile(0, 1);
    TileData newTile(0, 2);

    const Turn turn(
        &placedTile, &tokenTile, "bear",
        7, &newTile, "fox", 1, 2, 3
    );

    QCOMPARE(turn.getPlacedToken(), QString("bear"));
}

void TestTurn::scalarValues_data()
{
    QTest::addColumn<int>("index");
    QTest::addColumn<int>("pinecone");
    QTest::addColumn<QString>("token");
    QTest::newRow("zero-empty") << 0 << 0 << QString();
    QTest::newRow("nonzero") << 3 << 5 << QString("salmon");
}

void TestTurn::scalarValues()
{
    QFETCH(int, index);
    QFETCH(int, pinecone);
    QFETCH(QString, token);
    turn->setSelectedTileIndex(index);
    turn->setSelectedTokenIndex(index);
    turn->setPlayerId(index);
    turn->setPinecone(pinecone);
    turn->setPlacedToken(token);
    turn->setNewToken(token);
    QCOMPARE(turn->getSelectedTileIndex(), index);
    QCOMPARE(turn->getSelectedTokenIndex(), index);
    QCOMPARE(turn->getPlayerId(), index);
    QCOMPARE(turn->getPinecone(), pinecone);
    QCOMPARE(turn->getPlacedToken(), token);
    QCOMPARE(turn->getNewToken(), token);
}

void TestTurn::pointerSettersReplaceBorrowedTiles()
{
    turn->setPlacedTileData(&replacementTile);
    turn->setPlacedTokenTileData(&placedTile);
    turn->setNewTile(&tokenTile);
    QCOMPARE(turn->getPlacedTileData(), &replacementTile);
    QCOMPARE(turn->getPlacedTokenTileData(), &placedTile);
    QCOMPARE(turn->getNewTile(), &tokenTile);
}

void TestTurn::nullPointersAreRejected()
{
    QTest::ignoreMessage(QtDebugMsg, "Tried to setPlacedTileData with nullptr!");
    turn->setPlacedTileData(nullptr);
    QCOMPARE(turn->getPlacedTileData(), &placedTile);
    QTest::ignoreMessage(QtDebugMsg, "Tried to setPlacedTokenTileData with nullptr!");
    turn->setPlacedTokenTileData(nullptr);
    QCOMPARE(turn->getPlacedTokenTileData(), &tokenTile);
}

void TestTurn::serializationContainsFullState()
{
    QCOMPARE(turn->toVariant().toMap(), turnInput());
}

void TestTurn::deserializationReadsExplicitInput()
{
    Turn restored;
    restored.fromVariant(turnInput());
    QVERIFY(restored.getPlacedTileData());
    QVERIFY(restored.getPlacedTokenTileData());
    QVERIFY(restored.getNewTile());
    QCOMPARE(restored.getPlacedTileData()->getId(), 1);
    QCOMPARE(restored.getPlacedTokenTileData()->getId(), 2);
    QCOMPARE(restored.getNewTile()->getId(), 3);
    QCOMPARE(restored.getPlacedTileData()->getRotation(), 120);
    QCOMPARE(restored.getPlacedTokenTileData()->getPlacedToken(), QString("bear"));
    QCOMPARE(restored.getNewTile()->getHabitats(), QVector<QString>{"forest"});
    QCOMPARE(restored.getSelectedTileIndex(), 1);
    QCOMPARE(restored.getSelectedTokenIndex(), 2);
    QCOMPARE(restored.getPlayerId(), 7);
    QCOMPARE(restored.getPlacedToken(), QString("bear"));
    QCOMPARE(restored.getNewToken(), QString("fox"));
    QCOMPARE(restored.getPinecone(), 3);
}

void TestTurn::roundTripPreservesState()
{
    Turn restored;
    restored.fromVariant(turn->toVariant());
    QVERIFY(restored.getPlacedTileData() != turn->getPlacedTileData());
    QVERIFY(restored.getPlacedTokenTileData() != turn->getPlacedTokenTileData());
    QVERIFY(restored.getNewTile() != turn->getNewTile());
    QCOMPARE(restored.toVariant().toMap(), turnInput());
    restored.getPlacedTileData()->setId(99);
    QCOMPARE(placedTile.getId(), 1);
}

void TestTurn::copyPreservesStateAndSharesBorrowedTiles()
{
    {
        Turn copy(*turn);
        QCOMPARE(copy.toVariant().toMap(), turnInput());
        QCOMPARE(copy.getPlacedTileData(), &placedTile);
        QCOMPARE(copy.getPlacedTokenTileData(), &tokenTile);
        QCOMPARE(copy.getNewTile(), &replacementTile);
        copy.setPlayerId(9);
        QCOMPARE(turn->getPlayerId(), 7);
        copy.getPlacedTileData()->setId(42);
        QCOMPARE(turn->getPlacedTileData()->getId(), 42);
    }
    QCOMPARE(placedTile.getId(), 42);
}

void TestTurn::destructionKeepsBorrowedTilesAlive()
{
    turn.reset();
    QCOMPARE(placedTile.getId(), 1);
    QCOMPARE(tokenTile.getId(), 2);
    QCOMPARE(replacementTile.getId(), 3);
}

QTEST_APPLESS_MAIN(TestTurn)

#include "test_turn.moc"
