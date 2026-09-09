#include <QtTest>

#include "tileData.h"
#include "test_fixtures.h"

class TestTileData : public QObject
{
    Q_OBJECT

  private slots:
    void parameterizedConstructorInitializesState();
    void settersAndGettersPreserveValues();
    void toVariantContainsObjectState();
    void serializationRoundTripPreservesState();
    void fromVariantReadsExplicitMap();
    void serializationPreservesLists_data(); // helper
    void serializationPreservesLists();
    void fromVariantReplacesPreviousLists();
    void toVariantContainsRequiredKeys();
};

void TestTileData::parameterizedConstructorInitializesState()
{
    const TileData tile(2, 5);

    QCOMPARE(tile.getRow(), 2);
    QCOMPARE(tile.getCol(), 5);
    QCOMPARE(tile.getPlacedTile(), false);
    QCOMPARE(tile.getPlacedToken(), QString());
    QCOMPARE(tile.getRotation(), 0);
}

void TestTileData::settersAndGettersPreserveValues()
{
    TileData tile(1, 3);
    const QVector<QString> animals = {"bear", "elk"};
    const QVector<QString> habitats = {"forest", "mountain"};

    tile.setId(17);
    tile.setPlacedTile(true);
    tile.setIsValid(true);
    tile.setPlacedToken("bear");
    tile.setAnimals(animals);
    tile.setHabitats(habitats);
    tile.setRotation(120);
    tile.setIndex(4);

    QCOMPARE(tile.getId(), 17);
    QCOMPARE(tile.getRow(), 1);
    QCOMPARE(tile.getCol(), 3);
    QCOMPARE(tile.getPlacedTile(), true);
    QCOMPARE(tile.getIsValid(), true);
    QCOMPARE(tile.getPlacedToken(), QString("bear"));
    QCOMPARE(tile.getAnimals(), animals);
    QCOMPARE(tile.getHabitats(), habitats);
    QCOMPARE(tile.getRotation(), 120);
    QCOMPARE(tile.getIndex(), 4);
}

void TestTileData::toVariantContainsObjectState()
{
    TileData tile(4, 6);
    tile.setId(9);
    tile.setPlacedTile(true);
    tile.setIsValid(false);
    tile.setPlacedToken("hawk");
    tile.setAnimals({"hawk", "fox"});
    tile.setHabitats({"lake"});
    tile.setRotation(60);

    const QVariantMap map = tile.toVariant().toMap();

    QCOMPARE(map.value("id").toInt(), 9);
    QCOMPARE(map.value("row").toInt(), 4);
    QCOMPARE(map.value("col").toInt(), 6);
    QCOMPARE(map.value("placedTile").toBool(), true);
    QCOMPARE(map.value("isValid").toBool(), false);
    QCOMPARE(map.value("placedToken").toString(), QString("hawk"));
    QCOMPARE(map.value("animals").toStringList(), QStringList({"hawk", "fox"}));
    QCOMPARE(map.value("habitats").toStringList(), QStringList({"lake"}));
    QCOMPARE(map.value("rotation").toInt(), 60);
}

void TestTileData::serializationRoundTripPreservesState()
{
    TileData original(7, 8);
    original.setId(42);
    original.setPlacedTile(true);
    original.setIsValid(true);
    original.setPlacedToken("salmon");
    original.setAnimals({"salmon", "bear"});
    original.setHabitats({"river", "forest"});
    original.setRotation(180);

    TileData restored;
    restored.fromVariant(original.toVariant());

    QCOMPARE(restored.getId(), original.getId());
    QCOMPARE(restored.getRow(), original.getRow());
    QCOMPARE(restored.getCol(), original.getCol());
    QCOMPARE(restored.getPlacedTile(), original.getPlacedTile());
    QCOMPARE(restored.getIsValid(), original.getIsValid());
    QCOMPARE(restored.getPlacedToken(), original.getPlacedToken());
    QCOMPARE(restored.getAnimals(), original.getAnimals());
    QCOMPARE(restored.getHabitats(), original.getHabitats());
    QCOMPARE(restored.getRotation(), original.getRotation());
}

void TestTileData::fromVariantReadsExplicitMap()
{
    TileData tile;
    tile.fromVariant(tileMap());
    compareTile(tile, tileMap());
}

// Prepare data for next test: serializationPreservesLists
void TestTileData::serializationPreservesLists_data()
{
    QTest::addColumn<QStringList>("animals");
    QTest::addColumn<QStringList>("habitats");
    QTest::newRow("empty") << QStringList{} << QStringList{};
    QTest::newRow("single") << QStringList{"bear"} << QStringList{"forest"};
    QTest::newRow("multiple") << QStringList{"bear", "elk", "fox"}
                             << QStringList{"forest", "mountain"};
}

void TestTileData::serializationPreservesLists()
{
    QFETCH(QStringList, animals);
    QFETCH(QStringList, habitats);
    TileData original = makeTile();
    original.setAnimals(animals);
    original.setHabitats(habitats);
    const QVariantMap encoded = original.toVariant().toMap();
    QCOMPARE(encoded.value("animals").toStringList(), animals); 
    QCOMPARE(encoded.value("habitats").toStringList(), habitats);
    TileData restored;
    restored.fromVariant(encoded); 
    QCOMPARE(restored.getAnimals(), animals);
    QCOMPARE(restored.getHabitats(), habitats);
}

void TestTileData::fromVariantReplacesPreviousLists()
{
    TileData tile = makeTile();
    QVariantMap input = tileMap(90);
    input["animals"] = QVariantList{QString("hawk")};
    input["habitats"] = QVariantList{QString("lake")};
    tile.fromVariant(input);
    compareTile(tile, input);
    input["animals"] = QVariantList{};
    input["habitats"] = QVariantList{};
    tile.fromVariant(input);
    QVERIFY(tile.getAnimals().isEmpty());
    QVERIFY(tile.getHabitats().isEmpty());
}

void TestTileData::toVariantContainsRequiredKeys()
{ 
    const TileData tile = makeTile();
    const QVariantMap encoded = tile.toVariant().toMap();
    const QStringList keys{"id", "row", "col", "placedTile", "isValid",
                           "placedToken", "rotation", "animals", "habitats"};
    for (const QString &key : keys)
        QVERIFY2(encoded.contains(key), qPrintable("Missing key: " + key));
}

QTEST_APPLESS_MAIN(TestTileData)

#include "test_tile_data.moc"
