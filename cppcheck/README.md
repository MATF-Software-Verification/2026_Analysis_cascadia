# Cppcheck analiza

Za statičku analizu izvornog koda aplikacije Cascadia korišćen je alat
Cppcheck. Analiza je sprovedena na osnovu
`compile_commands.json` datoteke, a rezultati su sačuvani u XML formatu i
zatim pretvoreni u HTML izveštaj pomoću alata `cppcheck-htmlreport`.

## Preduslovi

Na Ubuntu/Debian sistemu potrebno je dodatno instalirati `cppcheck` alat:

```bash
sudo apt install cppcheck
```

## Pokretanje analize

Pre pokretanja analize potrebno je kopirati konfiguracionu datoteku:

```bash
cp cppcheck.cfg.example cppcheck.cfg
```

Analiza je pokrenuta sledećim opcijama:

```bash
cppcheck \
    -j "$JOBS" \
    --cppcheck-build-dir="$BUILD_DIR/cppcheck_cache" \
    --project="$BUILD_DIR/compile_commands.json" \
    --file-filter="$SOURCE_DIR/*" \
    --enable=all \
    --inconclusive \
    --library=qt \
    --suppress=missingIncludeSystem \
    --language=c++ \
    --std=c++20 \
    --platform=unix64 \
    --xml \
    --xml-version=2 \
    2>"$XML_FILE"
```

Skripta `cppcheck.sh` pre analize automatski konfiguriše projekat i generiše
`compile_commands.json` pomoću CMake-a:

```bash
cmake -S ../cascadia++/cascadia -B ../build_cppcheck \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_STANDARD=20 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

To je JSON datoteka koju CMake generiše kada je uključena
opcija `CMAKE_EXPORT_COMPILE_COMMANDS=ON`.
 Cppcheck koristi te zapise da analizira izvorni kod u istom kontekstu u kojem
se projekat kompajlira. Na taj način pravilno koristi include putanje, makroe,
Qt zaglavlja i opcije kompajlera. 

Opcija `--enable=all` uključuje sve dostupne kategorije provera, izuzev
`unusedFunction`, koja nije dostupna kada se analiza izvršava uz opciju `-j`.
Opcija `--inconclusive` uključuje i nalaze za koje Cppcheck nema potpunu
sigurnost. Opcija `--suppress=missingIncludeSystem` koristi se za potiskivanje
upozorenja koja nastaju zbog nedostupnih sistemskih zaglavlja i mogu predstavljati
lažno pozitivne rezultate.

## Rezultati

Rezultati jednog konkretnog pokretanja dostupni su u direktorijumu
[`reports/`](reports/), u HTML i XML formatu. Sažetak nalaza prikazan je u
tabeli:

| Kategorija | Broj nalaza | Udeo u ukupnom broju |
|:--|--:|--:|
| `style` | 159 | 56,6% |
| `warning` | 75 | 26,7% |
| `performance` | 41 | 14,6% |
| `information` | 6 | 2,1% |
| `error` | 0 | 0,0% |
| **Ukupno** | **281** | **100,0%** |

![Statistika Cppcheck nalaza](../images/statistics_cppcheck.png)

*Slika 1: Raspodela nalaza prema kategorijama.*

Vrste defekata prema učestalosti prikazane su na sledećoj slici:

![Vrste defekata prema učestalosti](../images/defect_types_cppcheck.png)

*Slika 2: Raspodela defekata prema tipu.*
