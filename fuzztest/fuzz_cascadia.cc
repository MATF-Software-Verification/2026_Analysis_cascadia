// Include FuzzTest before Qt headers to avoid Qt keyword macro conflicts.
#include "fuzztest/fuzztest.h"
#include "gtest/gtest.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryFile>
#include <QVariant>

#include <algorithm>
#include <cmath>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "hexagon.h"
#include "hexagonGrid.h"
#include "player.h"
#include "playerData.h"
#include "scoring.h"
#include "tile.h"
#include "tileData.h"
#include "tileStorage.h"
#include "token.h"
#include "turn.h"

namespace {

auto Rotations()
{
    return fuzztest::ElementOf<int>({0, 60, 120, 180, 240, 300});
}

auto Animals()
{
    return fuzztest::VectorOf(fuzztest::ElementOf<std::string>(
        {"bear", "elk", "fox", "hawk", "salmon"})).WithMaxSize(3);
}

auto Habitats()
{
    return fuzztest::VectorOf(fuzztest::ElementOf<std::string>(
        {"forest", "mountain", "lake", "swamp", "desert"})).WithMaxSize(2);
}

QVector<QString> ToQtStrings(const std::vector<std::string> &values)
{
    QVector<QString> result;
    for (const auto &value : values)
        result.append(QString::fromUtf8(value.data(), value.size()));
    return result;
}

QVariantMap TileMap(int id, int row, int col, int rotation)
{
    return {{"id", id}, {"row", row}, {"col", col},
            {"placedTile", true}, {"isValid", false},
            {"placedToken", "bear"}, {"rotation", rotation},
            {"animals", QVariantList{QString("bear")}},
            {"habitats", QVariantList{QString("forest")}}};
}

void SerializationPreservesState(int id, int rotation,
                                const std::vector<std::string> &animals,
                                const std::vector<std::string> &habitats)
{
    const Tile original(id, ToQtStrings(habitats), ToQtStrings(animals), rotation);
    Tile restored;
    restored.fromVariant(original.toVariant());
    EXPECT_EQ(restored.tileNumber(), id);
    EXPECT_EQ(restored.animals(), ToQtStrings(animals));
    EXPECT_EQ(restored.habitats(), ToQtStrings(habitats));
    EXPECT_EQ(restored.rotation(), rotation);
}
FUZZ_TEST(TileFuzz, SerializationPreservesState)
    .WithDomains(fuzztest::InRange(0, 1000), Rotations(), Animals(), Habitats())
    .WithSeeds({{1, 0, {"bear"}, {"forest"}},
                {2, 120, {"elk", "fox"}, {"forest", "lake"}}});

// Reversing a sequence of rotations must restore every serialized field.
void RotationsUndoEachOther(int rotation, const std::vector<bool> &clockwise)
{
    Tile tile(7, {"forest", "lake"}, {"bear", "elk"}, rotation);
    const QVariant before = tile.toVariant();
    for (bool right : clockwise)
    {
        if (right) tile.rotateClockwise();
        else tile.rotateCounterClockwise();
        EXPECT_GE(tile.rotation(), 0);
        EXPECT_LT(tile.rotation(), 360);
        EXPECT_EQ(tile.rotation() % 60, 0);
    }
    for (auto it = clockwise.rbegin(); it != clockwise.rend(); ++it)
    {
        if (*it) tile.rotateCounterClockwise();
        else tile.rotateClockwise();
    }
    EXPECT_EQ(tile.toVariant(), before);
    for (int i = 0; i < 6; ++i) tile.rotateClockwise();
    EXPECT_EQ(tile.toVariant(), before);
    for (int i = 0; i < 6; ++i) tile.rotateCounterClockwise();
    EXPECT_EQ(tile.toVariant(), before);
}
FUZZ_TEST(TileFuzz, RotationsUndoEachOther)
    .WithDomains(Rotations(), fuzztest::VectorOf(fuzztest::Arbitrary<bool>()).WithMaxSize(40))
    .WithSeeds({{0, {}}, {300, {true}}, {0, {false}}, {120, {true, false, false}}});

void TileDataPreservesState(int id, int row, int col, int rotation,
                           const std::vector<std::string> &animals,
                           const std::vector<std::string> &habitats)
{
    TileData original(row, col);
    original.setId(id);
    original.setIndex(0);
    original.setPlacedTile(true);
    original.setIsValid(false);
    original.setPlacedToken(animals.empty() ? QString() : QString::fromStdString(animals[0]));
    original.setAnimals(ToQtStrings(animals));
    original.setHabitats(ToQtStrings(habitats));
    original.setRotation(rotation);

    TileData restored;
    restored.fromVariant(original.toVariant());
    EXPECT_EQ(restored.getId(), id);
    EXPECT_EQ(restored.getRow(), row);
    EXPECT_EQ(restored.getCol(), col);
    EXPECT_TRUE(restored.getPlacedTile());
    EXPECT_FALSE(restored.getIsValid());
    EXPECT_EQ(restored.getPlacedToken(), original.getPlacedToken());
    EXPECT_EQ(restored.getAnimals(), ToQtStrings(animals));
    EXPECT_EQ(restored.getHabitats(), ToQtStrings(habitats));
    EXPECT_EQ(restored.getRotation(), rotation);
    // Index is not part of TileData's serialized format.
}
FUZZ_TEST(TileDataFuzz, TileDataPreservesState)
    .WithDomains(fuzztest::InRange(0, 1000), fuzztest::InRange(0, 29),
                 fuzztest::InRange(0, 29), Rotations(), Animals(), Habitats())
    .WithSeeds({{0, 0, 0, 0, {}, {}},
                {1000, 29, 29, 300, {"salmon"}, {"lake"}}});

void TurnPreservesState(int playerId, int index, int pinecones, int rotation)
{
    // Populate all borrowed pointers and scalar fields before serialization.
    TileData placed, tokenTile, replacement;
    placed.fromVariant(TileMap(1, 2, 3, rotation));
    tokenTile.fromVariant(TileMap(2, 2, 4, rotation));
    replacement.fromVariant(TileMap(3, 0, 0, rotation));
    placed.setIndex(0);
    tokenTile.setIndex(0);
    replacement.setIndex(0);
    Turn original;
    original.setPlacedTileData(&placed);
    original.setPlacedTokenTileData(&tokenTile);
    original.setNewTile(&replacement);
    original.setPlayerId(playerId);
    original.setSelectedTileIndex(index);
    original.setSelectedTokenIndex(index);
    original.setPinecone(pinecones);
    original.setPlacedToken("bear");
    original.setNewToken("fox");

    const QVariantMap expected{
        {"placedTileData", placed.toVariant()}, {"placedTokenTileData", tokenTile.toVariant()},
        {"newTile", replacement.toVariant()}, {"playerId", playerId},
        {"selectedTileIndex", index}, {"selectedTokenIndex", index},
        {"pinecone", pinecones}, {"placedToken", "bear"}, {"newToken", "fox"}};
    EXPECT_EQ(original.toVariant().toMap(), expected);
    Turn restored;
    restored.fromVariant(original.toVariant());
    ASSERT_NE(restored.getPlacedTileData(), nullptr);
    ASSERT_NE(restored.getPlacedTokenTileData(), nullptr);
    ASSERT_NE(restored.getNewTile(), nullptr);
    EXPECT_EQ(restored.toVariant().toMap(), expected);
    EXPECT_NE(restored.getPlacedTileData(), &placed);
    restored.getPlacedTileData()->setId(99);
    EXPECT_EQ(placed.getId(), 1);
}
FUZZ_TEST(TurnFuzz, TurnPreservesState)
    .WithDomains(fuzztest::InRange(0, 4), fuzztest::InRange(0, 3),
                 fuzztest::InRange(0, 20), Rotations())
    .WithSeeds({{0, 0, 0, 0}, {4, 3, 20, 300}});

void LoadsGeneratedJson(const std::vector<int> &ids, int rotation, bool starting)
{
    QJsonArray tiles;
    for (int id : ids)
        tiles.append(QJsonObject{{"tileNum", QString::number(id)},
                                 {"habitats", QJsonArray{"forest"}},
                                 {"wildlife", QJsonArray{"bear"}}, {"rotation", rotation}});
    const QString key = starting ? "startingTiles" : "allTiles";
    const QJsonObject root{{key, starting ? QJsonArray{tiles} : tiles}};
    const QByteArray bytes = QJsonDocument(root).toJson(QJsonDocument::Compact);
    QTemporaryFile file;
    ASSERT_TRUE(file.open());
    ASSERT_EQ(file.write(bytes), bytes.size());
    ASSERT_TRUE(file.flush());
    file.close();

    const TileStorage storage(file.fileName(), key);
    if (starting) ASSERT_EQ(storage.startingTiles().size(), 1);
    const auto &loaded = starting ? storage.startingTiles()[0] : storage.allTiles();
    ASSERT_EQ(loaded.size(), static_cast<qsizetype>(ids.size()));
    for (qsizetype i = 0; i < loaded.size(); ++i)
    {
        EXPECT_EQ(loaded[i].tileNumber(), ids[i]);
        EXPECT_EQ(loaded[i].rotation(), rotation);
        EXPECT_EQ(loaded[i].animals(), QVector<QString>{"bear"});
        EXPECT_EQ(loaded[i].habitats(), QVector<QString>{"forest"});
    }
}
FUZZ_TEST(TileStorageFuzz, LoadsGeneratedJson)
    .WithDomains(fuzztest::VectorOf(fuzztest::InRange(0, 1000)).WithMaxSize(12),
                 Rotations(), fuzztest::Arbitrary<bool>())
    .WithSeeds({{{}, 0, false}, {{1, 2, 3}, 120, true}, {{0, 1000}, 300, false}});

// Independent oracle: convert odd-row offset coordinates to axial coordinates.
bool AreAdjacent(int row, int col, int otherRow, int otherCol)
{
    const int q = col - (row - (row % 2)) / 2;
    const int otherQ = otherCol - (otherRow - (otherRow % 2)) / 2;
    const int dq = q - otherQ;
    const int dr = row - otherRow;
    return std::max({std::abs(dq), std::abs(dr), std::abs(dq + dr)}) == 1;
}

void NeighboursAreValid(int rows, int cols)
{
    HexagonGrid grid(rows, cols, 10.0f);
    EXPECT_EQ(grid.getRows(), rows);
    EXPECT_EQ(grid.getCols(), cols);
    ASSERT_EQ(grid.getTiles().size(), rows);
    for (int row = 0; row < rows; ++row)
    {
        ASSERT_EQ(grid.getTiles()[row].size(), cols);
        for (int col = 0; col < cols; ++col)
        {
            const auto neighbours = grid.getNeighbours(row, col);
            EXPECT_LE(neighbours.size(), 6);
            const std::set<std::pair<int, int>> unique(neighbours.begin(), neighbours.end());
            EXPECT_EQ(unique.size(), static_cast<std::size_t>(neighbours.size()));
            for (const auto &[nextRow, nextCol] : neighbours)
            {
                ASSERT_GE(nextRow, 0);
                ASSERT_LT(nextRow, rows);
                ASSERT_GE(nextCol, 0);
                ASSERT_LT(nextCol, cols);
                EXPECT_FALSE(nextRow == row && nextCol == col);
                const auto reverse = grid.getNeighbours(nextRow, nextCol);
                EXPECT_NE(std::find(reverse.begin(), reverse.end(), std::make_pair(row, col)),
                          reverse.end());
            }
            if (row > 0 && row < rows - 1 && col > 0 && col < cols - 1)
                EXPECT_EQ(neighbours.size(), 6);
            for (int r = 0; r < rows; ++r)
                for (int c = 0; c < cols; ++c)
                    EXPECT_EQ(neighbours.contains(std::make_pair(r, c)),
                              AreAdjacent(row, col, r, c));
        }
    }
}
FUZZ_TEST(GridFuzz, NeighboursAreValid)
    .WithDomains(fuzztest::InRange(0, 8), fuzztest::InRange(0, 8))
    .WithSeeds({{0, 0}, {0, 8}, {8, 0}, {1, 1}, {1, 8}, {8, 1}, {8, 8}});

// Empty cells are selectable exactly when they border at least one placed tile.
void SelectableCellsMatchEmptyNeighbours(const std::vector<int> &placedCells)
{
    HexagonGrid grid(8, 8, 10.0f);
    const std::set<int> occupied(placedCells.begin(), placedCells.end());
    for (int cell : occupied)
        grid.getTiles()[cell / 8][cell % 8]->getTileData()->setPlacedTile(true);
    for (int repeat = 0; repeat < 2; ++repeat)
    {
        grid.drawSelectableTiles();
        for (int row = 0; row < 8; ++row)
            for (int col = 0; col < 8; ++col)
            {
                const bool isOccupied = occupied.count(row * 8 + col) != 0;
                bool touchesPlaced = false;
                for (int cell : occupied)
                    touchesPlaced |= AreAdjacent(row, col, cell / 8, cell % 8);
                const auto *tile = grid.getTiles()[row][col]->getTileData();
                EXPECT_EQ(tile->getPlacedTile(), isOccupied);
                EXPECT_EQ(tile->getIsValid(), !isOccupied && touchesPlaced);
            }
    }
}
FUZZ_TEST(GridFuzz, SelectableCellsMatchEmptyNeighbours)
    .WithDomains(fuzztest::VectorOf(fuzztest::InRange(0, 63)).WithMaxSize(20))
    .WithSeeds({{{}}, {{0}}, {{0, 63}}, {{0, 0, 1}}, {{27, 28, 35, 36}}});

void InitializePlayer(Player &player, int rows, int cols)
{
    // Initialize every scalar and owned pointer before using the default player.
    player.setHexagonGrid(new HexagonGrid(rows, cols, 10.0f));
    player.setPlayerId(1);
    player.setPineconeNumber(0);
    player.setPineconeButtonActivated(false);
    player.setMyTurn(false);
    int id = 0;
    for (auto &row : player.hexagonGrid()->getTiles())
        for (Hexagon *hexagon : row)
        {
            TileData *tile = hexagon->getTileData();
            tile->setId(id++);
            tile->setIndex(0);
            tile->setPlacedTile(false);
            tile->setIsValid(false);
            tile->setPlacedToken("");
            tile->setAnimals({});
            tile->setHabitats({});
            tile->setRotation(0);
        }
}

void CopyHasIndependentGrid(int row, int col, int id, int pinecones)
{
    Player original;
    InitializePlayer(original, 6, 6);
    original.setPineconeNumber(pinecones);
    TileData *originalTile = original.hexagonGrid()->getTiles()[row][col]->getTileData();
    originalTile->setId(id);

    {
        Player copy(original);
        ASSERT_NE(copy.hexagonGrid(), original.hexagonGrid());
        TileData *copiedTile = copy.hexagonGrid()->getTiles()[row][col]->getTileData();
        ASSERT_NE(copiedTile, originalTile);
        EXPECT_EQ(copiedTile->getId(), id);
        EXPECT_EQ(copy.pineconeNumber(), pinecones);
        copiedTile->setId(id + 1);
        copy.setPineconeNumber(pinecones + 1);
        EXPECT_EQ(originalTile->getId(), id);
        EXPECT_EQ(original.pineconeNumber(), pinecones);
    }
    // Destroying the copy must leave the original grid alive.
    EXPECT_EQ(originalTile->getId(), id);
}
FUZZ_TEST(PlayerFuzz, CopyHasIndependentGrid)
    .WithDomains(fuzztest::InRange(0, 5), fuzztest::InRange(0, 5),
                 fuzztest::InRange(0, 1000), fuzztest::InRange(0, 20))
    .WithSeeds({{0, 0, 0, 0}, {5, 5, 1000, 20}});

// Write data directly: scoring tests do not need textures or a scene.
void Place(HexagonGrid *grid, int row, int col, const QString &animal,
           const QVector<QString> &habitats = {"forest"}, int rotation = 0)
{
    auto *tile = grid->getTiles()[row][col]->getTileData();
    tile->setPlacedTile(true);
    tile->setPlacedToken(animal);
    tile->setAnimals(animal.isEmpty() ? QVector<QString>{} : QVector<QString>{animal});
    tile->setHabitats(habitats);
    tile->setRotation(rotation);
}

void BearGroupsFollowScoringTable(const std::vector<int> &groupSizes,
                                int rowOffset, int colOffset)
{
    Player player;
    InitializePlayer(player, 14, 6);
    HexagonGrid *grid = player.hexagonGrid();

    // Separate groups by three rows so they cannot join into one component.
    for (std::size_t group = 0; group < groupSizes.size(); ++group)
        for (int i = 0; i < groupSizes[group]; ++i)
        {
            TileData *tile = grid->getTiles()[rowOffset + 3 * group][colOffset + i]->getTileData();
            tile->setPlacedTile(true);
            tile->setPlacedToken("bear");
            tile->setAnimals({"bear"});
            tile->setHabitats({"forest"});
        }

    // Rule used by the existing bear tests: only groups of exactly two count.
    const int scores[] = {0, 4, 11, 19, 27};
    const auto pairCount = std::count(groupSizes.begin(), groupSizes.end(), 2);
    Scoring scoring(player);
    EXPECT_EQ(scoring.calculateBearScore(grid), scores[pairCount]);
}
FUZZ_TEST(ScoringFuzz, BearGroupsFollowScoringTable)
    .WithDomains(fuzztest::VectorOf(fuzztest::InRange(1, 3)).WithMaxSize(4),
                 fuzztest::InRange(0, 2), fuzztest::InRange(0, 2))
    .WithSeeds({{{}, 0, 0}, {{1, 2, 3}, 1, 1}, {{2, 2, 2, 2}, 2, 2}});

// Pairs of neighbouring hawks contribute zero; isolated hawks follow card A.
void HawkGroupsCountOnlyIsolatedBirds(const std::vector<int> &groupSizes, int rowOffset)
{
    Player player;
    InitializePlayer(player, 27, 4);
    auto *grid = player.hexagonGrid();
    for (std::size_t group = 0; group < groupSizes.size(); ++group)
        for (int i = 0; i < groupSizes[group]; ++i)
            Place(grid, rowOffset + 3 * static_cast<int>(group), 1 + i, "hawk");
    const int table[] = {0, 2, 5, 8, 11, 14, 18, 22, 26};
    const int isolated = static_cast<int>(std::count(groupSizes.begin(), groupSizes.end(), 1));
    Scoring scoring(player);
    EXPECT_EQ(scoring.calculateHawkScore(grid), table[std::min(isolated, 8)]);
}
FUZZ_TEST(ScoringFuzz, HawkGroupsCountOnlyIsolatedBirds)
    .WithDomains(fuzztest::VectorOf(fuzztest::InRange(1, 2)).WithMaxSize(9),
                 fuzztest::InRange(0, 2))
    .WithSeeds({{{}, 0}, {{2}, 1}, {{1, 2, 1}, 0}, {{1, 1, 1, 1, 1, 1, 1, 1, 1}, 2}});

// Use one fox so the total is exactly its distinct neighbouring species count.
void FoxCountsDistinctAnimals(const std::vector<std::string> &animals, int row)
{
    Player player;
    InitializePlayer(player, 7, 7);
    auto *grid = player.hexagonGrid();
    Place(grid, row, 3, "fox");
    const auto neighbours = grid->getNeighbours(row, 3);
    ASSERT_EQ(neighbours.size(), 6);
    std::set<std::string> distinct;
    for (std::size_t i = 0; i < animals.size(); ++i)
    {
        const auto [r, c] = neighbours[static_cast<qsizetype>(i)];
        Place(grid, r, c, QString::fromStdString(animals[i]));
        if (!animals[i].empty()) distinct.insert(animals[i]);
    }
    Scoring scoring(player);
    EXPECT_EQ(scoring.calculateFoxScore(grid), static_cast<int>(distinct.size()));
}
FUZZ_TEST(ScoringFuzz, FoxCountsDistinctAnimals)
    .WithDomains(fuzztest::VectorOf(fuzztest::ElementOf<std::string>(
                     {"", "bear", "elk", "hawk", "salmon"})).WithMaxSize(6),
                 fuzztest::InRange(1, 5))
    .WithSeeds({{{}, 2}, {{"bear", "bear"}, 3},
                {{"bear", "elk", "hawk", "salmon", "", "bear"}, 2}});

void ElkStraightLinesFollowTable(int length, int row, int col)
{
    Player player;
    InitializePlayer(player, 7, 8);
    auto *grid = player.hexagonGrid();
    for (int i = 0; i < length; ++i) Place(grid, row, col + i, "elk");
    const int table[] = {0, 2, 5, 9, 13};
    Scoring scoring(player);
    EXPECT_EQ(scoring.calculateElkScore(grid), table[length]);
}
FUZZ_TEST(ScoringFuzz, ElkStraightLinesFollowTable)
    .WithDomains(fuzztest::InRange(0, 4), fuzztest::InRange(0, 6), fuzztest::InRange(0, 4))
    .WithSeeds({{0, 0, 0}, {1, 1, 1}, {4, 6, 4}});

void SeparateSalmonRunsAddScores(const std::vector<int> &lengths, int rowOffset, int col)
{
    Player player;
    InitializePlayer(player, 9, 11);
    auto *grid = player.hexagonGrid();
    const int table[] = {0, 2, 4, 7, 11, 15, 20, 26};
    int expected = 0;
    for (std::size_t run = 0; run < lengths.size(); ++run)
    {
        for (int i = 0; i < lengths[run]; ++i)
            Place(grid, rowOffset + 3 * static_cast<int>(run), col + i, "salmon");
        expected += table[std::min(lengths[run], 7)];
    }
    Scoring scoring(player);
    EXPECT_EQ(scoring.calculateSalmonScore(grid), expected);
}
FUZZ_TEST(ScoringFuzz, SeparateSalmonRunsAddScores)
    .WithDomains(fuzztest::VectorOf(fuzztest::InRange(1, 8)).WithMaxSize(3),
                 fuzztest::InRange(0, 2), fuzztest::InRange(0, 3))
    .WithSeeds({{{}, 0, 0}, {{1}, 0, 0}, {{3, 2}, 1, 1}, {{8}, 2, 3}});

// Each row is a separate region; only the largest region of one habitat scores.
void LargestHabitatRegionWins(const std::vector<int> &lengths,
                             const std::string &habitat, int rotation)
{
    Player player;
    InitializePlayer(player, 9, 8);
    auto *grid = player.hexagonGrid();
    const QString name = QString::fromStdString(habitat);
    for (std::size_t region = 0; region < lengths.size(); ++region)
        for (int col = 0; col < lengths[region]; ++col)
            Place(grid, 1 + 3 * static_cast<int>(region), col + 1, "", {name}, rotation);
    const int expected = lengths.empty() ? 0 : *std::max_element(lengths.begin(), lengths.end());
    Scoring scoring(player);
    const auto scores = scoring.playerHabitatScores();
    EXPECT_EQ(scores.at(name), expected);
    for (const auto &[otherName, score] : scores)
        if (otherName != name) EXPECT_EQ(score, 0);
}
FUZZ_TEST(ScoringFuzz, LargestHabitatRegionWins)
    .WithDomains(fuzztest::VectorOf(fuzztest::InRange(1, 6)).WithMaxSize(3),
                 fuzztest::ElementOf<std::string>({"forest", "lake", "mountain", "swamp", "desert"}),
                 Rotations())
    .WithSeeds({{{}, "forest", 0}, {{3, 2}, "forest", 120}, {{6, 6, 1}, "lake", 300}});

// At 120, 180 and 240 degrees the forest half faces the tile on the right.
void HabitatConnectionDependsOnRotation(int rotation, int row, int col)
{
    Player player;
    InitializePlayer(player, 7, 7);
    auto *grid = player.hexagonGrid();
    Place(grid, row, col, "", {"forest", "lake"}, rotation);
    Place(grid, row, col + 1, "", {"forest"});
    const bool connected = rotation == 120 || rotation == 180 || rotation == 240;
    Scoring scoring(player);
    const auto scores = scoring.playerHabitatScores();
    EXPECT_EQ(scores.at("forest"), connected ? 2 : 1);
    EXPECT_EQ(scores.at("lake"), 1);
}
FUZZ_TEST(ScoringFuzz, HabitatConnectionDependsOnRotation)
    .WithDomains(Rotations(), fuzztest::InRange(0, 6), fuzztest::InRange(0, 5))
    .WithSeeds({{0, 2, 2}, {180, 2, 2}, {300, 3, 3}});

} // namespace
