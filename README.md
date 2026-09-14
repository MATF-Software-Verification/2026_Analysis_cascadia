# Software Verification Analysis of Cascadia++

A software-verification project focused on the board-game implementation [Cascadia++](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2024-2025/cascadia/-/tree/main?ref_type=heads). The goal is to combine automated testing, symbolic execution, model checking, profiling, static analysis, and fuzzing in a single, reproducible workflow.

Course: *Verifikacija softvera*, Matematički fakultet, Univerzitet u Beogradu

## 1. Author info

- Julijana Jevtić
- Index: 1131/2025

## 2. Analyzed project description

- **Branch analyzed**: `main`
- **Commit pinned in the submodule**: `c32895403185b6ae4a968d4517d8a634adfe80ee`
- **Source**: added as a git submodule at [`cascadia++/`](cascadia++)

## 3. Project structure

Project contains a directory per technique, a pinned submodule, reproducible scripts, and a dedicated report that explains the methodology and findings.

## 4. Tools and dependencies

The project uses the following tools and techniques:

| Tool                  | Purpose                 |
| --------------------- | ----------------------- |
| **Qt Test**           | Unit testing            |
| **LCOV / genhtml**    | Code coverage           |
| **Valgrind Memcheck** | Dynamic memory analysis |
| **Cppcheck**          | Static analysis         |
| **Mull**              | Mutation testing        |
| **Google FuzzTest**   | Fuzz testing            |
| **Tracy Profiler**    | Performance profiling   |

### Installation

On Ubuntu, install the common dependencies with:

```bash
sudo apt update

sudo apt install -y \
    build-essential \
    cmake \
    git \
    curl \
    patch \
    pkg-config \
    qt6-base-dev \
    qt6-multimedia-dev \
    lcov \
    valgrind \
    cppcheck \
    clang \
    libclang-rt-dev \
    libglfw3-dev \
    libfreetype6-dev \
    libdbus-1-dev \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good
```

Mull uses a separate package repository. The analysis was performed with Mull for LLVM 18:

```bash
curl -1sLf 'https://dl.cloudsmith.io/public/mull-project/mull-stable/setup.deb.sh' | sudo -E bash
sudo apt install -y mull-18
```

Initialize the analyzed Cascadia++ project:

```bash
git submodule update --init --recursive
```

Google FuzzTest does not require a separate manual installation. It is downloaded automatically by CMake during the first fuzzing build.

Similarly, Tracy is downloaded and configured automatically when its analysis script is run for the first time.

> The first FuzzTest and Tracy builds therefore require an Internet connection.


## Conclusion

The analysis of *Cascadia* combines several complementary verification techniques. Unit tests and coverage evaluate expected behavior and exercised code, Mull assesses test effectiveness, Cppcheck detects potential issues statically, Valgrind checks memory-related problems, FuzzTest explores a wider range of inputs, and Tracy focuses on performance.

Together, these tools provide a broader and more reliable view of software quality than any single technique alone.

