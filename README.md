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

## 4. Tools used

This analysis is organized around the following techniques:

| # | Tool / technique | Category | Directory |
|---|---|---|---|
| 1 | Unit tests + coverage | Testing | [`unit_tests/`](unit_tests/) |
| 2 | Valgrind Memcheck | Memory analysis | [`valgrind/memcheck/`](valgrind/memcheck/) |

## Conclusion
