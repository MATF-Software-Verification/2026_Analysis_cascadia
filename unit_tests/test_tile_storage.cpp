#include <QtTest>
#include <QTemporaryDir>
#include <QTemporaryFile>

#include <memory>
#include <stdexcept>

#include "tileStorage.h"

class TestTileStorage : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<QTemporaryDir> directory;
    std::unique_ptr<QTemporaryFile> temporaryFile;

private slots:
    void init();
    void cleanup();
    void loadsAllTiles();
    void loadsStartingTiles();
    void emptyCollections_data();
    void emptyCollections();
    void missingFile_data();
    void missingFile();
    void invalidJson_data();
    void invalidJson();
};

void TestTileStorage::init()
{
    directory = std::make_unique<QTemporaryDir>();
    QVERIFY(directory->isValid());
    temporaryFile = std::make_unique<QTemporaryFile>(directory->filePath("tiles-XXXXXX.json"));
    QVERIFY(temporaryFile->open());
}

void TestTileStorage::cleanup()
{
    temporaryFile.reset();
    directory.reset();
}

void TestTileStorage::loadsAllTiles()
{
    const QByteArray json = R"json({"allTiles":[
        {"tileNum":"17","habitats":["forest","river"],"wildlife":["bear","salmon"],"rotation":120},
        {"tileNum":"3","habitats":["mountain"],"wildlife":["elk"],"rotation":300},
        {"tileNum":"8","habitats":[],"wildlife":[],"rotation":0}
    ]})json";
    QTemporaryFile &file = *temporaryFile;
    QCOMPARE(file.write(json), qint64(json.size()));
    QVERIFY(file.flush());
    file.close();

    const TileStorage storage(file.fileName(), "allTiles");
    const auto &tiles = storage.allTiles();
    QCOMPARE(tiles.size(), 3);
    QVERIFY(storage.startingTiles().isEmpty());
    QCOMPARE(tiles[0].tileNumber(), 17);
    QCOMPARE(tiles[0].habitats(), QVector<QString>({"forest", "river"}));
    QCOMPARE(tiles[0].animals(), QVector<QString>({"bear", "salmon"}));
    QCOMPARE(tiles[0].rotation(), 120);
    QCOMPARE(tiles[1].tileNumber(), 3);
    QCOMPARE(tiles[1].habitats(), QVector<QString>{"mountain"});
    QCOMPARE(tiles[1].animals(), QVector<QString>{"elk"});
    QCOMPARE(tiles[1].rotation(), 300);
    QCOMPARE(tiles[2].tileNumber(), 8);
    QVERIFY(tiles[2].habitats().isEmpty());
    QVERIFY(tiles[2].animals().isEmpty());
    QCOMPARE(tiles[2].rotation(), 0);
}

void TestTileStorage::loadsStartingTiles()
{
    const QByteArray json = R"json({"startingTiles":[
        [
            {"tileNum":"10","habitats":["forest","river"],"wildlife":["bear","salmon"],"rotation":60},
            {"tileNum":"11","habitats":["mountain"],"wildlife":["elk"],"rotation":180}
        ],
        [{"tileNum":"20","habitats":[],"wildlife":[],"rotation":300}],
        []
    ]})json";
    QTemporaryFile &file = *temporaryFile;
    QCOMPARE(file.write(json), qint64(json.size()));
    QVERIFY(file.flush());
    file.close();

    const TileStorage storage(file.fileName(), "startingTiles");
    const auto &groups = storage.startingTiles();
    QVERIFY(storage.allTiles().isEmpty());
    QCOMPARE(groups.size(), 3);
    QCOMPARE(groups[0].size(), 2);
    QCOMPARE(groups[1].size(), 1);
    QVERIFY(groups[2].isEmpty());
    QCOMPARE(groups[0][0].tileNumber(), 10);
    QCOMPARE(groups[0][0].habitats(), QVector<QString>({"forest", "river"}));
    QCOMPARE(groups[0][0].animals(), QVector<QString>({"bear", "salmon"}));
    QCOMPARE(groups[0][0].rotation(), 60);
    QCOMPARE(groups[0][1].tileNumber(), 11);
    QCOMPARE(groups[0][1].habitats(), QVector<QString>{"mountain"});
    QCOMPARE(groups[0][1].animals(), QVector<QString>{"elk"});
    QCOMPARE(groups[0][1].rotation(), 180);
    QCOMPARE(groups[1][0].tileNumber(), 20);
    QVERIFY(groups[1][0].habitats().isEmpty());
    QVERIFY(groups[1][0].animals().isEmpty());
    QCOMPARE(groups[1][0].rotation(), 300);
}

void TestTileStorage::emptyCollections_data()
{
    QTest::addColumn<QString>("parseType");
    QTest::newRow("allTiles") << QString("allTiles");
    QTest::newRow("startingTiles") << QString("startingTiles");
}

void TestTileStorage::emptyCollections()
{
    QFETCH(QString, parseType);
    const QByteArray json = R"json({"allTiles":[],"startingTiles":[]})json";
    QTemporaryFile &file = *temporaryFile;
    QCOMPARE(file.write(json), qint64(json.size()));
    QVERIFY(file.flush());
    file.close();
    const TileStorage storage(file.fileName(), parseType);
    QVERIFY(storage.allTiles().isEmpty());
    QVERIFY(storage.startingTiles().isEmpty());
}

void TestTileStorage::missingFile_data()
{
    QTest::addColumn<QString>("parseType");
    QTest::newRow("allTiles") << QString("allTiles");
    QTest::newRow("startingTiles") << QString("startingTiles");
}

void TestTileStorage::missingFile()
{
    QFETCH(QString, parseType);
    const QString path = directory->filePath("missing.json");
    QVERIFY_EXCEPTION_THROWN(TileStorage(path, parseType), std::runtime_error);
}

void TestTileStorage::invalidJson_data()
{
    QTest::addColumn<QString>("parseType");
    QTest::addColumn<QByteArray>("json");
    QTest::newRow("all-malformed") << QString("allTiles") << QByteArray("{broken");
    QTest::newRow("starting-malformed") << QString("startingTiles") << QByteArray("{broken");
    QTest::newRow("all-array-root") << QString("allTiles") << QByteArray("[]");
    QTest::newRow("starting-array-root") << QString("startingTiles") << QByteArray("[]");
    QTest::newRow("all-empty-file") << QString("allTiles") << QByteArray{};
    QTest::newRow("starting-empty-file") << QString("startingTiles") << QByteArray{};
}

void TestTileStorage::invalidJson()
{
    QFETCH(QString, parseType);
    QFETCH(QByteArray, json);
    QTemporaryFile &file = *temporaryFile;
    QCOMPARE(file.write(json), qint64(json.size()));
    QVERIFY(file.flush());
    file.close();
    QVERIFY_EXCEPTION_THROWN(TileStorage(file.fileName(), parseType), std::runtime_error);
}

int main(int argc, char **argv)
{
    TestTileStorage test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_tile_storage.moc"