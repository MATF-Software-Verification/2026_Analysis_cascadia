#pragma once
#include <QtTest>
#include "tileData.h"

inline QVariantMap tileMap(int id = 17)
{
    return {{"id", id}, {"row", 2}, {"col", 5}, {"placedTile", true},
            {"isValid", false}, {"placedToken", "bear"}, {"rotation", 120},
            {"animals", QVariantList{QString("bear"), QString("elk")}},
            {"habitats", QVariantList{QString("forest"), QString("mountain")}}};
}

inline TileData makeTile(int id = 17)
{
    TileData tile;
    tile.fromVariant(tileMap(id));
    tile.setIndex(0);
    return tile;
}


inline void compareTile(const TileData &tile, const QVariantMap &expected)
{
    QCOMPARE(tile.getId(), expected.value("id").toInt());
    QCOMPARE(tile.getRow(), expected.value("row").toInt());
    QCOMPARE(tile.getCol(), expected.value("col").toInt());
    QCOMPARE(tile.getPlacedTile(), expected.value("placedTile").toBool());
    QCOMPARE(tile.getIsValid(), expected.value("isValid").toBool());
    QCOMPARE(tile.getPlacedToken(), expected.value("placedToken").toString());
    QCOMPARE(tile.getRotation(), expected.value("rotation").toInt());
    QCOMPARE(tile.getAnimals(), expected.value("animals").toStringList());
    QCOMPARE(tile.getHabitats(), expected.value("habitats").toStringList());
}
