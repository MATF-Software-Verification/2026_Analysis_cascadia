# Project Analysis Report — Cascadia++

**Author**: Julijana Jevtic, index 1131/2025

**Project**: [Cascadia++](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2024-2025/cascadia/-/tree/main?ref_type=heads)

**Branch**: `main`

**Commit pinned in `cascadia++/`**: `c32895403185b6ae4a968d4517d8a634adfe80ee`

This report is the deep-dive companion to [README.md](README.md). It explains why the project was chosen, how the analysis workflow was structured, which techniques were applied, and what the results suggest about the reliability of the implementation.

## 1. Project selection and tool-selection rationale

Cascadia++ is a suitable subject for a mixed verification study because its core logic is concrete and deterministic, yet it still includes enough state transitions and combinatorial behavior to make symbolic exploration and fuzzing meaningful. The project is not a toy example with a single main function; it has a board model, turn logic, player state, and grid-level rules that can be exercised repeatedly under different inputs.


This project is a good choice also because it is written in C++ (my preference).
Before selecting it, I verified that it actually builds and runs: I configured it with CMake, the project compiled successfully, and a quick test launch showed the application starts without immediate runtime errors.

This is exactly the kind of project that demonstrates the broader idea behind software verification: correctness and reliability are not proven by one single tool, but by a well-structured combination of complementary techniques, all applied to the same project in a reproducible manner.

From a verification perspective, this makes it a good candidate for a layered workflow:

- unit tests validate the expected behavior of isolated logic objects;
- symbolic execution explores broad classes of states and constraints;
- model checking checks bounded state transitions against assertions;
- profiling identifies computational hotspots in the game loop and board updates;
- fuzzing stresses parsing, data handling, and edge-case behavior;
- static analysis catches suspicious patterns before execution.

The main idea is not merely to find one bug, but to show how several complementary techniques can work together on the same project.

## 2. Environment

The project was analyzed in a Linux environment using the standard toolchain needed for C++ verification work.

- Operating system: Linux (Ubuntu/Debian-compatible environment)
- Build system: CMake, with Qt-dependent project configuration for the upstream application
- Compiler: modern C++ toolchain compatible with the upstream source
- Analysis tools:
  - Valgrind
- Runtime environment: Python 3 for the test runner and orchestration scripts

## 3. Unit tests + coverage


## Conclusion
