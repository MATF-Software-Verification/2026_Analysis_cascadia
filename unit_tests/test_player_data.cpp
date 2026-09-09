#include <QtTest>
#include "playerData.h"
#include "test_fixtures.h"

class TestPlayerData : public QObject
{
    Q_OBJECT
private slots:
    void constructorPreservesPlayerData();
    void serializationPreservesTiles_data(); // helper
    void serializationPreservesTiles();
    void fromVariantReadsExplicitMap();
    void fromVariantReplacesExistingTiles();
};

void TestPlayerData::constructorPreservesPlayerData()
{
    const PlayerData player(7, "Julijana", {makeTile(10), makeTile(20)});
    QCOMPARE(player.getPlayerId(), 7);
    QCOMPARE(player.getPlayerName(), QString("Julijana"));
    QCOMPARE(player.getCentralTiles().size(), 2);
    compareTile(player.getCentralTiles().at(0), tileMap(10));
    compareTile(player.getCentralTiles().at(1), tileMap(20));
}

// Prepare data for next test: serializationPreservesTiles
void TestPlayerData::serializationPreservesTiles_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("empty") << 0;
    QTest::newRow("single") << 1;
    QTest::newRow("multiple") << 3;
}

void TestPlayerData::serializationPreservesTiles()
{
    QFETCH(int, count);
    QVector<TileData> tiles;
    for (int i = 0; i < count; ++i) tiles.append(makeTile(10 + i));
    const PlayerData original(7, QString::fromUtf8("Julijana Jevtić"), tiles);
    const QVariantMap encoded = original.toVariant().toMap();
    QVERIFY(encoded.contains("playerId"));
    QVERIFY(encoded.contains("playerName"));
    QVERIFY(encoded.contains("centralTiles"));
    QCOMPARE(encoded.value("playerId").toInt(), 7);
    QCOMPARE(encoded.value("playerName").toString(), original.getPlayerName());
    const QVariantList encodedTiles = encoded.value("centralTiles").toList();
    QCOMPARE(encodedTiles.size(), count);
    for (int i = 0; i < count; ++i) QCOMPARE(encodedTiles.at(i).toMap(), tileMap(10 + i));
    PlayerData restored;
    restored.fromVariant(encoded);
    QCOMPARE(restored.getPlayerId(), 7);
    QCOMPARE(restored.getPlayerName(), original.getPlayerName());
    QCOMPARE(restored.getCentralTiles().size(), count);
    for (int i = 0; i < count; ++i) compareTile(restored.getCentralTiles().at(i), tileMap(10 + i));
}

void TestPlayerData::fromVariantReadsExplicitMap()
{
    const QVariantMap input{{"playerId", 12}, {"playerName", "Ana"},
                            {"centralTiles", QVariantList{tileMap(31), tileMap(32)}}};
    PlayerData player;
    player.fromVariant(input);
    QCOMPARE(player.getPlayerId(), 12);
    QCOMPARE(player.getPlayerName(), QString("Ana"));
    QCOMPARE(player.getCentralTiles().size(), 2);
    compareTile(player.getCentralTiles().at(0), tileMap(31));
    compareTile(player.getCentralTiles().at(1), tileMap(32));
}

void TestPlayerData::fromVariantReplacesExistingTiles()
{
    PlayerData player(1, "Old", {makeTile(1), makeTile(2)});
    QVariantMap input{{"playerId", 2}, {"playerName", "Jelena"},
                      {"centralTiles", QVariantList{tileMap(99)}}};
    player.fromVariant(input);
    QCOMPARE(player.getPlayerId(), 2);
    QCOMPARE(player.getPlayerName(), QString("Jelena"));
    QCOMPARE(player.getCentralTiles().size(), 1);
    compareTile(player.getCentralTiles().at(0), tileMap(99));
    input["centralTiles"] = QVariantList{};
    player.fromVariant(input);
    QVERIFY(player.getCentralTiles().isEmpty());
}

QTEST_APPLESS_MAIN(TestPlayerData)

#include "test_player_data.moc"
