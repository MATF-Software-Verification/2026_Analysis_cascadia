#include "hexagonGrid.h"
#include "tile.h"
#include "tileData.h"

#include <QCoreApplication>

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace
{
using Clock = std::chrono::steady_clock;

const std::vector<std::string> targets = {
    "TileFuzz.SerializationPreservesState",
    "TileFuzz.RotationIsCanonical",
    "GridFuzz.NeighboursAreValid",
};

int integerArgument(const std::string &argument, const std::string &prefix, int fallback)
{
    if (argument.rfind(prefix, 0) != 0)
        return fallback;

    const std::string value = argument.substr(prefix.size());
    try
    {
        return std::stoi(value);
    }
    catch (...)
    {
        return fallback;
    }
}

bool serializationProperty(std::mt19937 &random)
{
    std::uniform_int_distribution<int> integer(-1000, 1000);
    TileData original(integer(random), integer(random));
    original.setId(integer(random));
    original.setPlacedTile(integer(random) % 2 == 0);
    original.setIsValid(integer(random) % 2 == 0);
    original.setPlacedToken(QString::number(integer(random)));
    original.setRotation(integer(random));
    original.setAnimals({QString::number(integer(random)), QString::number(integer(random))});
    original.setHabitats({QString::number(integer(random))});

    TileData restored;
    restored.fromVariant(original.toVariant());
    return restored.getId() == original.getId() && restored.getRow() == original.getRow() &&
           restored.getCol() == original.getCol() &&
           restored.getPlacedTile() == original.getPlacedTile() &&
           restored.getIsValid() == original.getIsValid() &&
           restored.getPlacedToken() == original.getPlacedToken() &&
           restored.getRotation() == original.getRotation() &&
           restored.getAnimals() == original.getAnimals() &&
           restored.getHabitats() == original.getHabitats();
}

bool rotationProperty(std::mt19937 &random)
{
    std::uniform_int_distribution<int> integer(0, 5);
    Tile tile(1, {"forest"}, {"bear"}, integer(random) * 60);
    const int initialRotation = tile.rotation();
    for (int i = 0; i < 6; ++i)
        tile.rotateClockwise();
    return tile.rotation() == initialRotation;
}

bool neighbourProperty(std::mt19937 &random)
{
    std::uniform_int_distribution<int> dimension(1, 12);
    const int rows = dimension(random);
    const int cols = dimension(random);
    HexagonGrid grid(rows, cols, 20.0f);

    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < cols; ++col)
        {
            const auto neighbours = grid.getNeighbours(row, col);
            if (neighbours.size() > 6)
                return false;
            for (const auto &[neighbourRow, neighbourCol] : neighbours)
            {
                if (neighbourRow < 0 || neighbourRow >= rows || neighbourCol < 0 ||
                    neighbourCol >= cols || (neighbourRow == row && neighbourCol == col) ||
                    !grid.getNeighbours(neighbourRow, neighbourCol)
                         .contains(std::make_pair(row, col)))
                    return false;
            }
        }
    return true;
}

bool runTarget(const std::string &target, int seconds)
{
    std::mt19937 random(std::random_device{}());
    const auto deadline = Clock::now() + std::chrono::seconds(seconds);
    do
    {
        bool passed = false;
        if (target == targets[0])
            passed = serializationProperty(random);
        else if (target == targets[1])
            passed = rotationProperty(random);
        else if (target == targets[2])
            passed = neighbourProperty(random);
        else
        {
            std::cerr << "Unknown fuzz target: " << target << '\n';
            return false;
        }

        if (!passed)
        {
            std::cerr << "Property failed: " << target << '\n';
            return false;
        }
    } while (Clock::now() < deadline);
    return true;
}
} // namespace

int main(int argc, char **argv)
{
    QCoreApplication application(argc, argv);

    for (int index = 1; index < argc; ++index)
        if (std::string(argv[index]) == "--list_fuzz_tests")
        {
            for (const auto &target : targets)
                std::cout << "[*] Fuzz test: " << target << '\n';
            return 0;
        }

    std::string selectedTarget;
    int duration = 60;
    for (int index = 1; index < argc; ++index)
    {
        const std::string argument = argv[index];
        if (argument.rfind("--fuzz=", 0) == 0)
            selectedTarget = argument.substr(7);
        else if (argument.rfind("--fuzz_for=", 0) == 0)
            duration = integerArgument(argument, "--fuzz_for=", duration);
    }

    if (selectedTarget.empty())
    {
        std::cerr << "Usage: fuzz_cascadia --fuzz=<target> --fuzz_for=<seconds>s\n";
        return 2;
    }
    return runTarget(selectedTarget, std::max(1, duration)) ? 0 : 1;
}