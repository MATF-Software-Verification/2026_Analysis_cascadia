#include <QtTest>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRegularExpression>
#include <QSignalSpy>

#include <memory>

#include "hexagon.h"

class AccessibleHexagon : public Hexagon
{
public:
    using Hexagon::Hexagon;
    using Hexagon::paint;
    using Hexagon::mousePressEvent;
};

class TestHexagonBranches : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<AccessibleHexagon> fixtureHexagon;

private slots:
    void init();
    void cleanup();
    void defaultCellCanBeCopied();
    void emptyPolygonHasZeroCenter();
    void defaultCellAcceptsInitializedData();
    void imageLayouts_data();
    void imageLayouts();
    void placedTokenReplacesAnimalOptions();
    void missingHabitatProducesTransparentImage();
    void missingAnimalKeepsHabitatBackground();
    void drawingStopsWhenPositionsRunOut();
    void paintUsesBackgroundOrPixmap_data();
    void paintUsesBackgroundOrPixmap();
    void mousePressStates_data();
    void mousePressStates();
    void marketViewMarksCellSelectable();
};

void TestHexagonBranches::init()
{
    fixtureHexagon = std::make_unique<AccessibleHexagon>(true, 20.0f, 0, 0);
}

void TestHexagonBranches::cleanup()
{
    fixtureHexagon.reset();
}

void TestHexagonBranches::defaultCellCanBeCopied()
{
    const Hexagon original;
    const Hexagon copy(original);
    QVERIFY(original.getTileData() == nullptr);
    QVERIFY(copy.getTileData() == nullptr);
    QVERIFY(copy.polygon().isEmpty());
}

void TestHexagonBranches::emptyPolygonHasZeroCenter()
{
    const Hexagon hexagon;
    QTest::ignoreMessage(QtWarningMsg, "Polygon is empty!");
    QCOMPARE(hexagon.getPolygonCenter(), QPointF(0, 0));
}

void TestHexagonBranches::defaultCellAcceptsInitializedData()
{
    TileData input(2, 3);
    input.setId(17);
    input.setIndex(1);
    input.setPlacedTile(false);
    input.setIsValid(true);
    input.setPlacedToken("");
    input.setAnimals({});
    input.setHabitats({});
    input.setRotation(0);
    Hexagon hexagon;
    hexagon.setTileData(input);
    QVERIFY(hexagon.getTileData());
    QCOMPARE(hexagon.getTileData()->toVariant().toMap(), input.toVariant().toMap());
    QCOMPARE(hexagon.getTileData()->getIndex(), 1);
    input.setId(99);
    QCOMPARE(hexagon.getTileData()->getId(), 17);
}

void TestHexagonBranches::imageLayouts_data()
{
    QTest::addColumn<QStringList>("animals");
    QTest::addColumn<QStringList>("habitats");
    QTest::addColumn<int>("rotation");
    QTest::newRow("one-option") << QStringList{"bear"} << QStringList{"forest"} << 0;
    QTest::newRow("two-options") << QStringList({"bear", "elk"}) << QStringList{"forest"} << 60;
    QTest::newRow("three-options-split-habitat") << QStringList({"bear", "elk", "fox"})
                                               << QStringList({"forest", "lake"}) << 120;
}

void TestHexagonBranches::imageLayouts()
{
    QFETCH(QStringList, animals);
    QFETCH(QStringList, habitats);
    QFETCH(int, rotation);
    for (const QString &animal : animals)
        QVERIFY(!QPixmap(":/resources/tokens/" + animal + ".png").isNull());
    QVERIFY(!QPixmap(":/resources/tiles/" + habitats.join("+") + ".png").isNull());
    Hexagon &hexagon = *fixtureHexagon;
    const QImage background = hexagon.generateImage({}, habitats, rotation, "").toImage();
    const QImage actual = hexagon.generateImage(animals, habitats, rotation, "").toImage();
    QCOMPARE(actual.size(), QSize(88, 100));
    QVERIFY(!actual.isNull());
    QVERIFY(actual != background);
    hexagon.setTileData(17, true, false, "", animals, habitats, rotation);
    QCOMPARE(hexagon.getTileData()->getAnimals(), animals);
    QCOMPARE(hexagon.getTileData()->getHabitats(), habitats);
}

void TestHexagonBranches::placedTokenReplacesAnimalOptions()
{
    QVERIFY(!QPixmap(":/resources/placedTokens/bear.png").isNull());
    Hexagon &hexagon = *fixtureHexagon;
    const QImage first = hexagon.generateImage({"elk"}, {"forest"}, 0, "bear").toImage();
    const QImage second = hexagon.generateImage({"fox", "salmon"}, {"forest"}, 0, "bear").toImage();
    const QImage background = hexagon.generateImage({}, {"forest"}, 0, "").toImage();
    QCOMPARE(first, second);
    QVERIFY(first != background);
}

void TestHexagonBranches::missingHabitatProducesTransparentImage()
{
    Hexagon &hexagon = *fixtureHexagon;
    QTest::ignoreMessage(QtDebugMsg, QRegularExpression("Failed to load habitat image:.*"));
    const QImage image = hexagon.generateImage({}, {"__missing_habitat__"}, 0, "").toImage();
    QVERIFY(!image.isNull());
    for (int y = 0; y < image.height(); ++y)
        for (int x = 0; x < image.width(); ++x)
            QCOMPARE(image.pixelColor(x, y).alpha(), 0);
}

void TestHexagonBranches::missingAnimalKeepsHabitatBackground()
{
    QVERIFY(!QPixmap(":/resources/tiles/forest.png").isNull());
    Hexagon &hexagon = *fixtureHexagon;
    const QImage background = hexagon.generateImage({}, {"forest"}, 0, "").toImage();
    QTest::ignoreMessage(QtDebugMsg, QRegularExpression("Failed to load animal image:.*"));
    const QImage actual = hexagon.generateImage({"__missing_animal__"}, {"forest"}, 0, "").toImage();
    QCOMPARE(actual, background);
}

void TestHexagonBranches::drawingStopsWhenPositionsRunOut()
{
    Hexagon &hexagon = *fixtureHexagon;
    QImage actual(50, 50, QImage::Format_ARGB32_Premultiplied);
    actual.fill(Qt::transparent);
    const QImage before = actual.copy();
    QPainter painter(&actual);
    hexagon.drawAnimalImages(painter, {}, {"bear"}, 10, false);
    painter.end();
    QCOMPARE(actual, before);
}

void TestHexagonBranches::paintUsesBackgroundOrPixmap_data()
{
    QTest::addColumn<bool>("hasPixmap");
    QTest::addColumn<QColor>("expectedCenter");
    QTest::newRow("background-only") << false << QColor(Qt::cyan);
    QTest::newRow("with-pixmap") << true << QColor(Qt::red);
}

void TestHexagonBranches::paintUsesBackgroundOrPixmap()
{
    QFETCH(bool, hasPixmap);
    QFETCH(QColor, expectedCenter);
    AccessibleHexagon &hexagon = *fixtureHexagon;
    hexagon.setBrush(QBrush(Qt::cyan));
    if (hasPixmap)
    {
        QPixmap pixmap(40, 40);
        pixmap.fill(Qt::red);
        hexagon.setCurrentPixmap(pixmap);
    }
    QImage image(64, 64, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.translate(32, 32);
    hexagon.paint(&painter, nullptr, nullptr);
    painter.end();
    QCOMPARE(image.pixelColor(32, 32), expectedCenter);
    QCOMPARE(image.pixelColor(0, 0).alpha(), 0);
}

void TestHexagonBranches::mousePressStates_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<bool>("hasPixmap");
    QTest::newRow("not-selectable") << false << false;
    QTest::newRow("selectable-empty") << true << false;
    QTest::newRow("selectable-image") << true << true;
}

void TestHexagonBranches::mousePressStates()
{
    QFETCH(bool, valid);
    QFETCH(bool, hasPixmap);
    AccessibleHexagon &hexagon = *fixtureHexagon;
    hexagon.getTileData()->setIsValid(valid);
    if (hasPixmap)
    {
        QPixmap image(20, 20);
        image.fill(Qt::red);
        hexagon.setCurrentPixmap(image);
    }
    QSignalSpy spy(&hexagon, &Hexagon::hexagonClicked);
    QVERIFY(spy.isValid());
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    event.setButton(Qt::LeftButton);
    event.setButtons(Qt::LeftButton);
    hexagon.mousePressEvent(&event);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<Hexagon *>(spy.at(0).at(0)), static_cast<Hexagon *>(&hexagon));
    QCOMPARE(hexagon.getTileData()->getIsValid(), valid);
}

void TestHexagonBranches::marketViewMarksCellSelectable()
{
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 600, 600);
    Hexagon hexagon(false, 20.0f, 0, 0);
    hexagon.drawHexagonView(2, &scene);
    QVERIFY(scene.items().contains(&hexagon));
    QCOMPARE(hexagon.getTileData()->getIndex(), 2);
    QVERIFY(hexagon.getTileData()->getIsValid());
    QCOMPARE(hexagon.brush().color(), QColor(138, 222, 237));
    const QPointF center = hexagon.getPolygonCenter();
    QVERIFY(qAbs(center.x() - 420.0) < 0.001);
    QVERIFY(qAbs(center.y() - 380.0) < 0.001);
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestHexagonBranches test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_hexagon_branches.moc"