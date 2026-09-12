#!/usr/bin/env python3
"""Extract complete Memcheck records that mention Cascadia source files."""

import argparse
from pathlib import Path
import re


def log_blocks(text):
    # Valgrind separates diagnostic records with blank lines, usually PID-prefixed.
    block = []
    start = 1
    for number, line in enumerate(text.splitlines(), 1):
        if not re.sub(r"^==\d+== ?", "", line).strip():
            if block:
                yield start, "\n".join(block)
                block = []
        else:
            if not block:
                start = number
            block.append(line)
    if block:
        yield start, "\n".join(block)


def description(block):
    # Translate the finding label; retain the original evidence below it.
    labels = {
        "Invalid read": "Neispravno čitanje memorije",
        "Invalid write": "Neispravan upis u memoriju",
        "Conditional jump": "Uslov zavisi od neinicijalizovane vrednosti",
        "Use of uninitialised": "Upotreba neinicijalizovane vrednosti",
        "Invalid free": "Neispravno oslobađanje memorije",
        "Mismatched": "Neusklađeno zauzimanje i oslobađanje memorije",
        "are definitely lost": "Memorija klasifikovana kao definitivno izgubljena",
        "are indirectly lost": "Memorija klasifikovana kao indirektno izgubljena",
        "are possibly lost": "Memorija klasifikovana kao moguće izgubljena",
        "are still reachable": "Memorija koja je i dalje dostupna preko pokazivača",
    }
    return next((label for key, label in labels.items() if key in block), "Zapis povezan sa igrom")


def main():
    parser = argparse.ArgumentParser(description="Izdvaja zapise koji pominju izvorne fajlove igre Cascadia++.")
    parser.add_argument("log", type=Path, help="putanja do Memcheck loga")
    parser.add_argument("--source-dir", type=Path, default=Path(__file__).resolve().parents[2] / "cascadia++/cascadia")
    parser.add_argument("--output", type=Path, help="izlazni Markdown fajl")
    args = parser.parse_args()
    if not args.source_dir.is_dir():
        parser.error("Nije pronađen izvorni kod igre. Postavi skriptu u valgrind/memcheck/ ili koristi --source-dir.")

    # Match actual production filenames. Ignore main() and generated Qt files.
    sources = {
        path.name for path in args.source_dir.rglob("*")
        if path.is_file() and path.suffix in {".cpp", ".cc", ".cxx", ".h", ".hpp"}
        and path.name not in {"main.cpp", "mocs_compilation.cpp"}
        and not path.name.startswith(("moc_", "qrc_", "ui_"))
        and not any(part.startswith("build") for part in path.relative_to(args.source_dir).parts)
    }
    frame = re.compile(r"\b(?:at|by) 0x[\da-fA-F]+:.*?\(([^()]+\.(?:cpp|cc|cxx|h|hpp)):(\d+)\)")
    findings = []
    for line, block in log_blocks(args.log.read_text(encoding="utf-8", errors="replace")):
        matches = sorted({f"{Path(name).name}:{row}" for name, row in frame.findall(block) if Path(name).name in sources})
        if matches:
            findings.append((line, block, matches))

    output = args.output or args.log.with_name("cascadia_findings.md")
    # Never overwrite the original log or a manually edited report.
    with output.open("x", encoding="utf-8") as report:
        report.write(f"# Memcheck zapisi povezani sa igrom Cascadia++\n\nIzvorni log: `{args.log.name}`\n\n")
        report.write(f"Broj izdvojenih zapisa: **{len(findings)}**. Ovo nije broj potvrđenih grešaka u igri.\n\n")
        report.write("Izdvojeni su celi zapisi koji u tragu poziva pominju prepoznate izvorne fajlove igre. "
                     "Podudaranje naziva fajla ne dokazuje uzrok greške. "
                     "`main.cpp` i generisani Qt fajlovi nisu kriterijum, jer se pojavljuju i u inicijalizaciji biblioteka. "
                     "Uključene su sve kategorije takvih zapisa, pa i `still reachable`. Ostali zapisi ostaju u izvornom logu.\n\n")
        if not findings:
            report.write("Nema zapisa sa prepoznatim izvornim fajlovima igre. To ne dokazuje odsustvo grešaka, posebno ako tragovi imaju nepoznate ili nedostajuće funkcije.\n")
        for index, (line, block, matches) in enumerate(findings, 1):
            report.write(f"## Zapis {index}: {description(block)}\n\nPočetni red u izvornom logu: **{line}**.\n\n")
            report.write("Fajlovi igre: " + ", ".join(f"`{match}`" for match in matches) + ".\n\n")
            report.write(f"Originalni zapis:\n\n```text\n{block}\n```\n\n")
    print(f"Sačuvan izveštaj: {output} ({len(findings)} zapisa). Izvorni log nije izmenjen.")


if __name__ == "__main__":
    try:
        main()
    except FileExistsError:
        raise SystemExit("Izlazni fajl već postoji. Sačuvaj postojeći nalaz i izaberi drugo ime pomoću --output.")
    except OSError as error:
        raise SystemExit(f"Nije moguće obraditi fajlove: {error}")
