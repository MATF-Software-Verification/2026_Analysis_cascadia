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

```text
2026_Analysis_cascadia/
├── README.md
├── ProjectAnalysisReport.md
├── ProjectAnalysisReport.pdf
├── .github/
│   └── workflows/
│       ├── gate.yml
│       └── tickets.yml
├── .gitmodules
├── cascadia++/                      # pinned upstream project under analysis
│   └── ...
└── .gitignore
```

Project contains ne directory per technique, a pinned submodule, reproducible scripts, and a dedicated report that explains the methodology and findings.

## 4. Tools used

This analysis is organized around the following techniques:

| # | Tool / technique | Category | Directory |
|---|---|---|---|
| 1 | Unit tests + coverage | Testing | [`unit_tests/`](unit_tests/) |

### 4.1 Unit tests

## Conclusion
