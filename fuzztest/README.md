# Fuzz testiranje - Google FuzzTest

Proširen skup sadrži **15 fuzz testova za 7 klasa**. Svaki test predstavlja
svojstvo koje FuzzTest proverava nad mnogim generisanim ulazima.

## Postavljanje i pokretanje

Potrebni su Clang i AddressSanitizer runtime:

```bash
sudo apt update
sudo apt install clang libclang-rt-dev qt6-base-dev
```

Iz korena repozitorijuma:

```bash
bash fuzztest/fuzzer.sh
```

Prva kompilacija preuzima FuzzTest i njegove zavisnosti i zahteva internet.
Konfiguraciju pravimo ovako:

```bash
cp fuzztest/fuzztest.cfg.example fuzztest/fuzztest.cfg
```

## Konfiguracija

```bash
JOBS=2
FUZZ_DURATION=60
FUZZ_TARGETS=""
```

`JOBS` je broj paralelnih poslova **kompilacije**. Testovi se izvršavaju redom.
`FUZZ_DURATION` je trajanje fuzzinga **po testu**, u sekundama. Prazan
`FUZZ_TARGETS` bira svih 15 testova: oko 15 minuta planiranog fuzzinga, uz
kompilaciju. Test koji nađe problem završava ranije.

Za konkretan podskup potrebno je upisati puna imena razdvojena razmacima:

```bash
FUZZ_TARGETS="TileFuzz.RotationsUndoEachOther ScoringFuzz.FoxCountsDistinctAnimals"
```

Po potrebi se u isti `.cfg` mogu dodati `CC=clang-18`, `CXX=clang++-18`
ili `BUILD_TYPE=Debug`. Podrazumevani kompajleri su `clang` i `clang++`.

## Šta se testira

| Fuzz test | Generisani ulazi i očekivano svojstvo |
|---|---|
| `TileFuzz.SerializationPreservesState` | ID, dozvoljena rotacija i liste životinja/staništa; serijalizacija čuva sva ta polja. |
| `TileFuzz.RotationsUndoEachOther` | Početni ugao i do 40 smerova rotacije; obrnuti niz i šest punih koraka vraćaju početno stanje. |
| `TileDataFuzz.TileDataPreservesState` | ID, koordinate, ugao i liste; očuvanje serijalizovanih podataka polja. |
| `TurnFuzz.TurnPreservesState` | Igrač, indeks, šišarke i ugao; očuvanje poteza i nezavisnost deserializovanih podataka. |
| `TileStorageFuzz.LoadsGeneratedJson` | Ispravan JSON sa 0-12 pločica, oba formata; provera broja i sadržaja učitanih pločica. |
| `GridFuzz.NeighboursAreValid` | Dimenzije 0-8; susedi su tačni prema geometriji šestougaone mreže, jedinstveni, uzajamni i u granicama. |
| `GridFuzz.SelectableCellsMatchEmptyNeighbours` | Do 20 postavljenih polja u mreži 8×8; dostupna su upravo prazna susedna polja, i pri ponovnom pozivu. |
| `PlayerFuzz.CopyHasIndependentGrid` | Polje mreže, ID pločice i šišarke; menjanje i uništavanje kopije ne menja original. |
| `ScoringFuzz.BearGroupsFollowScoringTable` | Razdvojene grupe od 1-3 medveda; boduju se samo parovi, prema očekivanoj tabeli. |
| `ScoringFuzz.HawkGroupsCountOnlyIsolatedBirds` | Do devet razdvojenih grupa od 1-2 jastreba; boduju se izolovani, uz ograničenje za osam. |
| `ScoringFuzz.FoxCountsDistinctAnimals` | Do šest suseda jedne lisice; računaju se različite vrste, bez dupliranja iste vrste. |
| `ScoringFuzz.ElkStraightLinesFollowTable` | Prave horizontalne linije od 0-4 jelena na različitim mestima; očekivano 0, 2, 5, 9 ili 13 bodova. |
| `ScoringFuzz.SeparateSalmonRunsAddScores` | Do tri razdvojena niza od 1-8 lososa; sabiranje rezultata uz ograničenje po nizu. |
| `ScoringFuzz.LargestHabitatRegionWins` | Odvojene oblasti različitih veličina i svih pet staništa; računa se najveća oblast. |
| `ScoringFuzz.HabitatConnectionDependsOnRotation` | Dve susedne pločice i svih šest rotacija; spajanje staništa zavisi od dodirnih stranica. |


## Ponavljanje nalaza

Rezultati se čuvaju u `fuzztest/reports/run_<datum_vreme>/`.

Skripta posle pada jednog testa nastavlja sa ostalima. Na kraju vraća `1` ako
je bilo neuspeha. Kod `0` označava da u toj sesiji nije prijavljen problem;
nenulti kod traži pregled loga. Uzrok može biti narušeno svojstvo, ASan prijava,
prekoračenje ograničenja ili neuspešno pokretanje. Broj neuspešnih testova nije
broj različitih defekata.

Primer ponavljanja sačuvanog ulaza, uz odgovarajuće ime testa i apsolutnu putanju:

```bash
QT_QPA_PLATFORM=offscreen \
FUZZTEST_REPLAY="/puna/putanja/do/sacuvanog_ulaza" \
./build_fuzz/fuzz_cascadia --gtest_filter=TileFuzz.SerializationPreservesState
```

## Rezultati 

Za postavku konfiguracije `FUZZ_DURATION=60` **nađen je samo jedan problem**.

**Pri serijalizaciji i ponovnom učitavanju pločice gubi se rotacija.** 

| Podatak            | Vrednost                                                              |
| ------------------ | --------------------------------------------------------------------- |
| Test               | `TileFuzz.SerializationPreservesState`                                |
| Ulaz               | ID `2`, rotacija `120`, životinje `elk, fox`, staništa `forest, lake` |
| Očekivana rotacija | `120°`                                                                |
| Dobijena rotacija  | `0°`                                                                  |

**Uzrok se potvržuje u kodu:**

* `Tile::toVariant()` upisuje vrednost `m_rotation`.
* `Tile::fromVariant()` učitava broj pločice, staništa i životinje, ali **ne učitava rotaciju**.
* Novi objekat `Tile restored` počinje sa rotacijom `0`, koja zato ostaje nepromenjena.

`fuzz_cascadia.cc:75` označava mesto gde je test primetio problem; uzrok je u implementaciji `Tile::fromVariant()`.

**`SIGABRT` na kraju je posledica prekida koji FuzzTest pokreće nakon neuspešne provere.** 

Ovaj nalaz potvrđuje **istu grešku koju su ranije otkrili jedinični testovi rotacije**. 