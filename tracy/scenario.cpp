#include <QApplication>
#include <QPoint>
#include <tracy/Tracy.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "player.h"
#include "tileStorage.h"

constexpr int SCORING_REPETITIONS = 200;
constexpr int WARMUP_REPETITIONS = 5;
constexpr int STORAGE_REPETITIONS = 50;
constexpr int SERIALIZATION_REPETITIONS = 200;

// The first three positions are the actual starting positions used by Player.
// Each additional position touches an earlier tile, keeping the board connected.
const QVector<QPoint> POSITIONS = {
    {11, 9}, {12, 9}, {12, 10}, {11, 10}, {11, 11}, {12, 11},
    {13, 11}, {13, 10}, {13, 9}, {13, 8}, {12, 8}, {11, 8},
    {10, 8}, {10, 9}, {10, 10}, {10, 11}, {11, 12}, {12, 12},
    {13, 12}, {14, 11}, {14, 10}, {14, 9}
};

std::unique_ptr<Player> makeBoard(int count, const QVector<Tile> &starting,
                                const QVector<Tile> &ordinary)
{
    ZoneScopedN("Board preparation");
    auto player = std::make_unique<Player>(QString("Tracy scenario"));
    player->setPlayerId(0);
    auto &tiles = player->hexagonGrid()->getTiles();

    // Initialize every field read or copied by the production scoring code.
    int id = 0;
    for (auto &row : tiles)
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

    for (int i = 0; i < count; ++i)
    {
        const Tile &source = i < 3 ? starting.at(i) : ordinary.at(i - 3);
        const auto animals = source.animals();
        const auto habitats = source.habitats();
        if (animals.isEmpty() || habitats.isEmpty())
            throw std::runtime_error("Plocica nema ocekivane podatke.");

        // Starting tiles have no tokens. Other tiles use an allowed animal.
        const QString token = i < 3 ? QString() : animals.at((i - 3) % animals.size());
        const QPoint position = POSITIONS.at(i);
        tiles[position.x()][position.y()]->setTileData(
            source.tileNumber(), true, false, token, animals, habitats, source.rotation());
    }
    return player;
}

void warmUp(Player &player)
{
    ZoneScopedN("Warm-up (exclude from comparison)");
    for (int i = 0; i < WARMUP_REPETITIONS; ++i)
        player.calculateScore();
}

std::int64_t repeatScoring(Player &player)
{
    std::int64_t checksum = 0;
    for (int i = 0; i < SCORING_REPETITIONS; ++i)
    {
        player.calculateScore();

        // Consume results without introducing a correctness oracle for scoring.
        for (const auto &entry : player.animalPoints())
            checksum += entry.second;
        for (const auto &entry : player.habitatScores())
            checksum += entry.second;
    }
    return checksum;
}

int main(int argc, char **argv)
{
    // QPixmap needs QApplication even though the workload has no visible window.
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication application(argc, argv);
    tracy::SetThreadName("Cascadia scenario");

    try
    {
        const QString path = ":/resources/storage.json";
        TileStorage all(path, "allTiles");
        TileStorage starting(path, "startingTiles");
        if (starting.startingTiles().isEmpty() || starting.startingTiles().first().size() != 3
            || all.allTiles().size() < 19)
            throw std::runtime_error("Nedostaju plocice za zadate scenarije.");

        std::cout << "Tracy scenario: mreza 30 x 30; po " << SCORING_REPETITIONS
                  << " merenih obracuna za svaku tablu.\n";

        const auto &firstStartingSet = starting.startingTiles().first();
        auto early = makeBoard(3, firstStartingSet, all.allTiles());
        auto middle = makeBoard(12, firstStartingSet, all.allTiles());
        auto late = makeBoard(22, firstStartingSet, all.allTiles());
        std::int64_t checksum = 0;

        warmUp(*early);
        {
            ZoneScopedN("Early board - 3 tiles");
            checksum += repeatScoring(*early);
        }
        std::cout << "Zavrseno bodovanje table sa 3 plocice.\n";

        warmUp(*middle);
        {
            ZoneScopedN("Middle board - 12 tiles");
            checksum += repeatScoring(*middle);
        }
        std::cout << "Zavrseno bodovanje table sa 12 plocica.\n";

        warmUp(*late);
        {
            ZoneScopedN("Late board - 22 tiles");
            checksum += repeatScoring(*late);
        }
        std::cout << "Zavrseno bodovanje table sa 22 plocice.\n";

        {
            ZoneScopedN("Selectable neighbours - 22 tiles");
            for (int i = 0; i < SCORING_REPETITIONS; ++i)
                late->hexagonGrid()->drawSelectableTiles();
        }

        {
            ZoneScopedN("JSON loading - 50 repetitions");
            for (int i = 0; i < STORAGE_REPETITIONS; ++i)
            {
                // Fresh objects prevent the parsers from appending to old data.
                TileStorage tiles(path, "allTiles");
                TileStorage starts(path, "startingTiles");
                checksum += tiles.allTiles().size() + starts.startingTiles().size();
            }
        }

        {
            ZoneScopedN("Tile serialization - 200 passes");
            for (int i = 0; i < SERIALIZATION_REPETITIONS; ++i)
                for (const Tile &tile : all.allTiles())
                {
                    const QVariant data = tile.toVariant();
                    Tile restored;
                    restored.fromVariant(data);
                    checksum += restored.tileNumber();
                }
        }

        std::cout << "Kontrolni zbir: " << checksum << '\n';
        std::cout << "Svi scenariji su zavrseni; sledi cuvanje Tracy snimka.\n";
    }
    catch (const std::exception &error)
    {
        std::cerr << "Scenario nije zavrsen: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
