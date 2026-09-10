#include <QtTest>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointer>
#include <QSignalSpy>

#include <memory>

#include "token.h"

class ClickableToken : public Token
{
public:
    using Token::Token;
    using Token::mousePressEvent;
};

class TestToken : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<ClickableToken> fixtureToken;

private slots:
    void init();
    void cleanup();
    void constructorSetsIndexAndShape();
    void animalCanBeReplaced_data();
    void animalCanBeReplaced();
    void serializationContainsIndexAndRadius();
    void deserializationReadsExplicitMap();
    void serializedFieldsRoundTrip();
    void displayingAnimalUpdatesStateAndImage();
    void sceneOwnsDrawnToken();
    void mousePressEmitsTokenIdentity();
};

void TestToken::init()
{
    fixtureToken = std::make_unique<ClickableToken>(0);
}

void TestToken::cleanup()
{
    fixtureToken.reset();
}

void TestToken::constructorSetsIndexAndShape()
{
    const Token token(3);
    QCOMPARE(token.getIndex(), 3);
    QCOMPARE(token.rect(), QRectF(-25, -25, 50, 50));
    QCOMPARE(token.childItems().size(), 1);
}

void TestToken::animalCanBeReplaced_data()
{
    QTest::addColumn<QString>("animal");
    QTest::newRow("empty") << QString();
    QTest::newRow("bear") << QString("bear");
    QTest::newRow("salmon") << QString("salmon");
}

void TestToken::animalCanBeReplaced()
{
    QFETCH(QString, animal);
    Token &token = *fixtureToken;
    token.setAnimal("fox");
    token.setAnimal(animal);
    QCOMPARE(token.animal(), animal);
}

void TestToken::serializationContainsIndexAndRadius()
{
    const Token token(2);
    const QVariantMap map = token.toVariant().toMap();
    QVERIFY(map.contains("index"));
    QVERIFY(map.contains("radius"));
    QCOMPARE(map.value("index").toInt(), 2);
    QCOMPARE(map.value("radius").toDouble(), 25.0);
}

void TestToken::deserializationReadsExplicitMap()
{
    Token &token = *fixtureToken;
    const QVariantMap input{{"index", 4}, {"radius", 40.0}};
    token.fromVariant(input);
    QCOMPARE(token.getIndex(), 4);
    QCOMPARE(token.toVariant().toMap().value("radius").toDouble(), 40.0);
}

void TestToken::serializedFieldsRoundTrip()
{
    const Token original(3);
    Token &restored = *fixtureToken;
    restored.fromVariant(original.toVariant());
    QCOMPARE(restored.getIndex(), 3);
    QCOMPARE(restored.toVariant().toMap(), original.toVariant().toMap());
}

void TestToken::displayingAnimalUpdatesStateAndImage()
{
    QVERIFY(!QPixmap(":/resources/tokens/bear.png").isNull());
    Token token(1);
    token.displayTokenImage("bear");
    QCOMPARE(token.animal(), QString("bear"));
    QCOMPARE(token.childItems().size(), 1);
    auto *item = dynamic_cast<QGraphicsPixmapItem *>(token.childItems().first());
    QVERIFY(item);
    QVERIFY(!item->pixmap().isNull());
    QCOMPARE(item->pixmap().size(), QSize(50, 50));
    QCOMPARE(item->pos(), QPointF(-25, -25));
}

void TestToken::sceneOwnsDrawnToken()
{
    QPointer<Token> guarded;
    {
        QGraphicsScene scene;
        scene.setSceneRect(0, 0, 600, 600);
        auto *token = new Token(1);
        guarded = token;
        token->drawToken(&scene);
        QVERIFY(scene.items().contains(token));
        QCOMPARE(token->pos(), QPointF(320, 250));
    }
    QVERIFY(guarded.isNull());
}

void TestToken::mousePressEmitsTokenIdentity()
{
    ClickableToken &token = *fixtureToken;
    QSignalSpy spy(&token, &Token::tokenClicked);
    QVERIFY(spy.isValid());
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    event.setButton(Qt::LeftButton);
    event.setButtons(Qt::LeftButton);
    token.mousePressEvent(&event);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<Token *>(spy.at(0).at(0)), static_cast<Token *>(&token));
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication application(argc, argv);
    TestToken test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_token.moc"