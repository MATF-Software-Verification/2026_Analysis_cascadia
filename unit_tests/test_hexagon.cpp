#include <QtTest>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>
#include <cmath>
#include "hexagon.h"

class ClickableHexagon : public Hexagon
{
public:
    using Hexagon::Hexagon;
    using Hexagon::mousePressEvent;
};

class TestHexagon : public QObject
{
    Q_OBJECT
private slots:
    void constructorCreatesSixVertices();
    void polygonCenterFollowsTranslation();
    void settingDataPreservesCoordinates();
    void settingDataMakesIndependentCopy();
    void copyConstructorCopiesTileIndependently();
    void emptyTileBrush_data();
    void emptyTileBrush();
    void drawingPlacesCellInScene_data();
    void drawingPlacesCellInScene();
    void mousePressEmitsCellIdentity();
};

void TestHexagon::constructorCreatesSixVertices()
{
    const Hexagon hexagon(true, 20.0f, 2, 3);
    QCOMPARE(hexagon.polygon().size(), 6);
    for (const QPointF &point : hexagon.polygon())
        QVERIFY(std::abs(std::hypot(point.x(), point.y()) - 20.0) < 0.001);
    QVERIFY(hexagon.getTileData());
    QCOMPARE(hexagon.getTileData()->getRow(), 2);
    QCOMPARE(hexagon.getTileData()->getCol(), 3);
    QVERIFY(!hexagon.getTileData()->getPlacedTile());
    QVERIFY(!hexagon.getTileData()->getIsValid());
}

void TestHexagon::polygonCenterFollowsTranslation()
{
    Hexagon hexagon(true, 20.0f, 0, 0);
    QVERIFY(std::abs(hexagon.getPolygonCenter().x()) < 0.001);
    QVERIFY(std::abs(hexagon.getPolygonCenter().y()) < 0.001);
    hexagon.setPolygon(hexagon.polygon().translated(40, 70));
    QVERIFY(std::abs(hexagon.getPolygonCenter().x() - 40.0) < 0.001);
    QVERIFY(std::abs(hexagon.getPolygonCenter().y() - 70.0) < 0.001);
}

void TestHexagon::settingDataPreservesCoordinates()
{
    Hexagon hexagon(true, 20.0f, 2, 3);
    hexagon.setTileData(17, true, false, "bear", {"bear", "elk"}, {"forest"}, 120);
    const TileData *tile = hexagon.getTileData();
    QCOMPARE(tile->getId(), 17);
    QCOMPARE(tile->getRow(), 2);
    QCOMPARE(tile->getCol(), 3);
    QCOMPARE(tile->getPlacedTile(), true);
    QCOMPARE(tile->getIsValid(), false);
    QCOMPARE(tile->getPlacedToken(), QString("bear"));
    QCOMPARE(tile->getAnimals(), QVector<QString>({"bear", "elk"}));
    QCOMPARE(tile->getHabitats(), QVector<QString>{"forest"});
    QCOMPARE(tile->getRotation(), 120);
}

void TestHexagon::settingDataMakesIndependentCopy()
{
    TileData original(4, 5);
    original.setId(18);
    original.setIndex(2);
    original.setPlacedTile(true);
    original.setIsValid(false);
    original.setPlacedToken("fox");
    original.setAnimals({"fox"});
    original.setHabitats({"forest"});
    original.setRotation(60);
    Hexagon hexagon(true, 20.0f, 0, 0);
    hexagon.setTileData(original);
    QVERIFY(hexagon.getTileData() != &original);
    QCOMPARE(hexagon.getTileData()->toVariant().toMap(), original.toVariant().toMap());
    QCOMPARE(hexagon.getTileData()->getIndex(), 2);
    original.setId(99);
    QCOMPARE(hexagon.getTileData()->getId(), 18);
}

void TestHexagon::copyConstructorCopiesTileIndependently()
{
    Hexagon original(true, 20.0f, 2, 3);
    original.setTileData(17, true, false, "bear", {"bear"}, {"forest"}, 120);
    original.getTileData()->setIndex(0);
    Hexagon copy(original);
    QVERIFY(copy.getTileData() != original.getTileData());
    QCOMPARE(copy.polygon(), original.polygon());
    QCOMPARE(copy.getTileData()->toVariant().toMap(), original.getTileData()->toVariant().toMap());
    copy.getTileData()->setRotation(240);
    QCOMPARE(original.getTileData()->getRotation(), 120);
}

void TestHexagon::emptyTileBrush_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QColor>("expected");
    QTest::newRow("selectable") << true << QColor(219, 224, 157);
    QTest::newRow("not-selectable") << false << QColor(Qt::cyan);
}

void TestHexagon::emptyTileBrush()
{
    QFETCH(bool, valid);
    QFETCH(QColor, expected);
    Hexagon hexagon(true, 20.0f, 0, 0);
    hexagon.setTileData(0, false, valid, "", {}, {}, 0);
    QCOMPARE(hexagon.brush().color(), expected);
    QCOMPARE(hexagon.getTileData()->getIsValid(), valid);
}

void TestHexagon::drawingPlacesCellInScene_data()
{
    QTest::addColumn<int>("row");
    QTest::addColumn<double>("xFactor");
    QTest::addColumn<double>("expectedY");
    QTest::newRow("even-row") << 2 << 3.0 << 60.0;
    QTest::newRow("odd-row") << 1 << 3.5 << 30.0;
}

void TestHexagon::drawingPlacesCellInScene()
{
    QFETCH(int, row);
    QFETCH(double, xFactor);
    QFETCH(double, expectedY);
    QGraphicsScene scene;
    Hexagon hexagon(true, 20.0f, row, 3);
    hexagon.drawHexagon(row, 3, &scene);
    QVERIFY(scene.items().contains(&hexagon));
    const QPointF center = hexagon.getPolygonCenter();
    QVERIFY(std::abs(center.x() - xFactor * std::sqrt(3.0) * 20.0) < 0.001);
    QVERIFY(std::abs(center.y() - expectedY) < 0.001);
}

void TestHexagon::mousePressEmitsCellIdentity()
{
    ClickableHexagon hexagon(true, 20.0f, 0, 0);
    QSignalSpy spy(&hexagon, &Hexagon::hexagonClicked);
    QVERIFY(spy.isValid());
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    event.setButton(Qt::LeftButton);
    event.setButtons(Qt::LeftButton);
    hexagon.mousePressEvent(&event);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<Hexagon *>(spy.at(0).at(0)), static_cast<Hexagon *>(&hexagon));
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestHexagon tests;
    return QTest::qExec(&tests, argc, argv);
}
#include "test_hexagon.moc"
