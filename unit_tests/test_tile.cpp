#include <QtTest>
#include "tile.h"

class TestTile : public QObject
{
    Q_OBJECT

private slots:
    void constructorsPreserveState();
    void rotations_data();
    void rotations();
    void rotationCycles_data(); // helper
    void rotationCycles();
    void serializationPreservesLists_data(); // helper
    void serializationPreservesLists();
    void fromVariantReplacesLists();
    void serializationPreservesRotation();
    void fromVariantReadsRotation();
    void typeDescribesTile();
};

void TestTile::constructorsPreserveState()
{
    const Tile empty;
    QCOMPARE(empty.tileNumber(), 0);
    QCOMPARE(empty.rotation(), 0);
    QVERIFY(empty.animals().isEmpty());
    QVERIFY(empty.habitats().isEmpty());

    const QVector<QString> habitats{"forest", "river"};
    const QVector<QString> animals{"bear", "salmon"};
    Tile tile(17, habitats, animals, 120);
    QCOMPARE(tile.tileNumber(), 17);
    QCOMPARE(tile.habitats(), habitats);
    QCOMPARE(tile.animals(), animals);
    QCOMPARE(tile.rotation(), 120);
    tile.setRotation(240);
    QCOMPARE(tile.rotation(), 240);
}

// Prepare data for next test: rotations
void TestTile::rotations_data()
{
    QTest::addColumn<int>("initial");
    QTest::addColumn<int>("clockwise");
    QTest::addColumn<int>("counterClockwise");
    QTest::newRow("0")   << 0   << 60  << 300;
    QTest::newRow("60")  << 60  << 120 << 0;
    QTest::newRow("120") << 120 << 180 << 60;
    QTest::newRow("180") << 180 << 240 << 120;
    QTest::newRow("240") << 240 << 300 << 180;
    QTest::newRow("300") << 300 << 0   << 240;
}

void TestTile::rotations()
{
    QFETCH(int, initial);
    QFETCH(int, clockwise);
    QFETCH(int, counterClockwise);
    Tile tile(1, {}, {}, initial);
    tile.rotateClockwise();
    QCOMPARE(tile.rotation(), clockwise);
    tile.setRotation(initial);
    tile.rotateCounterClockwise();
    QCOMPARE(tile.rotation(), counterClockwise);
}

// Prepare data for next test: rotationCycles
void TestTile::rotationCycles_data()
{
    QTest::addColumn<int>("initial");
    QTest::newRow("0") << 0;
    QTest::newRow("60") << 60;
    QTest::newRow("120") << 120;
    QTest::newRow("180") << 180;
    QTest::newRow("240") << 240;
    QTest::newRow("300") << 300;
}

void TestTile::rotationCycles()
{
    QFETCH(int, initial);
    Tile tile(1, {"forest"}, {"bear"}, initial);
    for (int i = 0; i < 6; ++i)
        tile.rotateClockwise();
    QCOMPARE(tile.rotation(), initial);
    for (int i = 0; i < 6; ++i)
        tile.rotateCounterClockwise();
    QCOMPARE(tile.rotation(), initial);
    tile.rotateClockwise();
    tile.rotateCounterClockwise();
    QCOMPARE(tile.rotation(), initial);
    tile.rotateCounterClockwise();
    tile.rotateClockwise();
    QCOMPARE(tile.rotation(), initial);
    QCOMPARE(tile.tileNumber(), 1);
    QCOMPARE(tile.habitats(), QVector<QString>{"forest"});
    QCOMPARE(tile.animals(), QVector<QString>{"bear"});
}

void TestTile::serializationPreservesLists_data()
{
    QTest::addColumn<QStringList>("habitats");
    QTest::addColumn<QStringList>("animals");
    QTest::newRow("empty") << QStringList{} << QStringList{};
    QTest::newRow("single") << QStringList{"forest"} << QStringList{"bear"};
    QTest::newRow("multiple") << QStringList({"forest", "river"})
                              << QStringList({"bear", "salmon", "fox"});
}

void TestTile::serializationPreservesLists()
{
    QFETCH(QStringList, habitats);
    QFETCH(QStringList, animals);
    const Tile original(17, habitats, animals, 0);
    const QVariantMap encoded = original.toVariant().toMap();
    const QStringList keys{"m_tileNumber", "m_habitats", "m_animals", "m_rotation"};
    for (const QString &key : keys)
        QVERIFY(encoded.contains(key));
    QCOMPARE(encoded.value("m_tileNumber").toInt(), 17);
    QCOMPARE(encoded.value("m_habitats").toStringList(), habitats);
    QCOMPARE(encoded.value("m_animals").toStringList(), animals);
    QCOMPARE(encoded.value("m_rotation").toInt(), 0);
    Tile restored;
    restored.fromVariant(encoded);
    QCOMPARE(restored.tileNumber(), 17);
    QCOMPARE(restored.habitats(), habitats);
    QCOMPARE(restored.animals(), animals);
}

void TestTile::fromVariantReplacesLists()
{
    Tile tile(1, {"forest", "river"}, {"bear", "salmon"}, 0);
    QVariantMap input{
        {"m_tileNumber", 9}, {"m_rotation", 0},
        {"m_habitats", QVariantList{QString("mountain")}},
        {"m_animals", QVariantList{QString("elk")}}
    };
    tile.fromVariant(input);
    QCOMPARE(tile.tileNumber(), 9);
    QCOMPARE(tile.habitats(), QVector<QString>{"mountain"});
    QCOMPARE(tile.animals(), QVector<QString>{"elk"});
    input["m_habitats"] = QVariantList{};
    input["m_animals"] = QVariantList{};
    tile.fromVariant(input);
    QVERIFY(tile.habitats().isEmpty());
    QVERIFY(tile.animals().isEmpty());
}

void TestTile::serializationPreservesRotation()
{
    const Tile original(17, {"forest"}, {"bear"}, 120);
    const QVariantMap encoded = original.toVariant().toMap();
    QVERIFY(encoded.contains("m_rotation"));
    QCOMPARE(encoded.value("m_rotation").toInt(), 120);
    Tile restored;
    restored.fromVariant(encoded);
    QCOMPARE(restored.rotation(), 120);
}

void TestTile::fromVariantReadsRotation()
{
    Tile tile(1, {}, {}, 240);
    const QVariantMap input{
        {"m_tileNumber", 8}, {"m_rotation", 60},
        {"m_habitats", QVariantList{}}, {"m_animals", QVariantList{}}
    };
    tile.fromVariant(input);
    QCOMPARE(tile.rotation(), 60);
}

void TestTile::typeDescribesTile()
{
    QCOMPARE(Tile().type(), QString("Tile 0: Habitats [], Animals [], Rotation: 0"));
    const Tile tile(17, {"forest", "river"}, {"bear", "salmon"}, 120);
    QCOMPARE(tile.type(), QString("Tile 17: Habitats [forest, river], Animals [bear, salmon], Rotation: 120"));
}

QTEST_APPLESS_MAIN(TestTile)

#include "test_tile.moc"
