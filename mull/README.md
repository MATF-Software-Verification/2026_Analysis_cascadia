# Mull — mutaciono testiranje

Mull pravi male izmene u kodu (mutante), pokreće testove i proverava da li ih testovi otkrivaju.
Na primer, zameni `==` sa `!=` ili `+` sa `-`. Ako testovi prijave grešku, mutant je otkriven.
Ako prođu, mutant je preživeo i treba pregledati šta izmena znači.

## Pokretanje

Potrebni su Clang 18, LLVM 18, Mull za LLVM 18, CMake i Qt 6 razvojni paketi.
Za Ubuntu instalacija prema Mull dokumentaciji je:

```bash
sudo apt update
sudo apt install -y curl ca-certificates gnupg cmake build-essential qt6-base-dev clang-18 llvm-18 libclang-rt-18-dev
curl -1sLf 'https://dl.cloudsmith.io/public/mull-project/mull-stable/setup.deb.sh' -o /tmp/mull-setup.deb.sh
sudo -E bash /tmp/mull-setup.deb.sh
sudo apt update
sudo apt install -y mull-18
```

Pokreni analizu iz korena projekta:

```bash
bash mull/mull.sh
```

## Šta testiramo

Podrazumevano koristimo četiri cela skupa postojećih Qt Test testova:

| Izvršni fajl | Šta proverava |
|---|---|
| `cascadia_unit_tests` | Stanje i serijalizaciju klase TileData. |
| `player_data_tests` | Podatke igrača i njihovu serijalizaciju. |
| `tile_storage_tests` | Učitavanje pločica iz JSON-a i neispravne ulaze. |
| `hexagon_grid_tests` | Dimenzije mreže, susede i izbor dozvoljenih polja. |

Mull menja samo produkcione fajlove dozvoljene u `mull.yml`.

Važne opcije skripte koju koristimo:

| Opcija | Zašto je koristimo |
|---|---|
| `-O0 -g` | Olakšava povezivanje mutacija sa izvornim kodom. |
| `-fpass-plugin=...` | Uključuje Mull plugin pri kompajliranju. |
| `-grecord-command-line` | Čuva informacije o naredbi kompajliranja potrebne Mull-u. |
| `-fprofile-instr-generate -fcoverage-mapping` | Dodaje LLVM pokrivenost; Mull može da izostavi mutante na nepokrivenim linijama. |
| `--allow-surviving` | Omogućava završetak analize i kada postoje preživeli mutanti. Oni se i dalje prijavljuju. |
| `--timeout 3000` | Ograničava jedno izvršavanje testa, uključujući slučaj kada mutacija napravi beskonačnu petlju. |
| `cxx_all` | Uključuje standardni skup C/C++ mutacionih operatora, kao u regex primeru. |

Postojeći CMake ima `ENABLE_COVERAGE` namenjen GCC/gcov pokrivenosti. Zato je ovde isključen, dok LLVM pokrivenost uključujemo navedenim Clang zastavicama.
`--clean-first` ponovo gradi izabrane ciljeve da bi se uvažile i izmene `mull.yml`.
Za kompajliranje i izvršavanje mutanata koristimo po dva paralelna posla radi manjeg opterećenja računara.

## Rezultati

**Najviše pažnje treba posvetiti `PlayerData`, gde je preživelo 9 od 15 mutanata.** Rezultati pokazuju koliko su izabrani testovi osetljivi na razmatrane izmene koda.

| Skup testova | Ukupno mutanata | Killed | Survived | Timeout | Mull mutation score |
|---|---:|---:|---:|---:|---:|
| `cascadia_unit_tests` | 19 | 12 | 7 | 0 | 63% |
| `player_data_tests` | 15 | 6 | 9 | 0 | 40% |
| `tile_storage_tests` | 7 | 5 | 0 | 2 | 71% |
| `hexagon_grid_tests` | 81 | 31 | 13 | 37 | 38% |

Polje mutationScore odgovara udelu Killed u ukupnom broju razmatranih mutanata, prikazanom kao ceo procenat. Timeout-i nisu uračunati kao Killed.


## Zaključci po skupu

- **TileData:** 12 od 19 mutanata ima status Killed, a 7 je preživelo. Pregled pojedinačnih izmena je potreban pre zaključka da svaki preživeli mutant predstavlja nedostatak testova.
- **PlayerData:** 6 od 15 mutanata ima status Killed, a 9 je preživelo. Osam preživelih potiče iz `tileData.cpp`, a samo jedan iz `playerData.cpp`; rezultat zato opisuje skup testova sa zavisnostima, a ne isključivo klasu PlayerData.
- **TileStorage:** potvrđeno je da nema preživelih mutanata, ali samo 5 od 7 ima status Killed. Preostala dva su Timeout, pa se ne može tvrditi da su testovi svojim proverama otkrili svih sedam mutacija.
- **HexagonGrid:** 31 od 81 mutanta ima status Killed. Veliki broj timeout-a (37) zahteva oprez pri oceni testova. Među 13 preživelih postoje i izmene koje ne menjaju ponašanje, kao i izmena vizuelnog prikaza koju testovi ne proveravaju.

## Primer 1: preživeli mutant u player_data_tests

**Lokacija:** `game/entities/tileData.cpp:156`, funkcija `TileData::setIndex`.

**Operator:** `cxx_assign_const`.

```cpp
// Original
index = newIndex;

// Mutant
index = 42;
```

Status u `player_data_tests.json` je **Survived**. Pomoćna funkcija `makeTile` poziva `setIndex(0)`, ali provere u ovom skupu usmerene su na podatke igrača i serijalizovane podatke pločica. Funkcija `compareTile` ne proverava indeks.

Isti mutant u `cascadia_unit_tests.json` ima status **Killed**. U testu `settersAndGettersPreserveValues` postavlja se indeks 4 i proverava `QCOMPARE(tile.getIndex(), 4)`.

**Zaključak:** mutant nije otkriven u jednom skupu, ali jeste u drugom. Ovo nije dokaz da projekat u celini nema test za `setIndex`, niti razlog da se automatski dodaje ista provera u svaki skup testova.

## Primer 2: preživela promena izgleda dozvoljenog polja

**Lokacija:** `game/board/hexagonGrid.cpp:97`, funkcija `HexagonGrid::setValidNeighbours`.

**Operator:** `cxx_remove_void_call`.

Mull uklanja poziv:

```cpp
hex->setBrush(QBrush(QColor(219, 224, 157)));
```

Status je **Survived**. Poziv `tile->setIsValid(true)` ostaje, pa polje zadržava oznaku da je dozvoljeno, ali izostaje postavljanje njegove boje.

Postojeći test `selectableNeighbours` proverava logičko stanje polja preko `getPlacedTile()` i `getIsValid()`, a ne boju četkice.

**Zaključak:** ovaj mutant ukazuje na deo vizuelnog ponašanja koji dati testovi ne proveravaju. Moguća dopuna je provera `brush().color()` za dozvoljeno polje nakon poziva `setValidNeighbours`, ako je boja deo zahtevanog ponašanja.

## Primer 3: otkrivena pogrešna rotacija pri učitavanju

**Lokacija:** `game/entities/tileStorage.cpp:64`, funkcija za učitavanje običnih pločica.

**Operator:** `cxx_init_const`.

```cpp
// Original
int rotation = tileObj.value("rotation").toInt();

// Mutant
int rotation = 42;
```

Status je **Killed**. Mutant zanemaruje rotaciju zadatu u JSON-u i uvek koristi 42.

U postojećem testu `loadsAllTiles` ulaz sadrži rotacije 120, 300 i 0, a proveravaju se iste vrednosti u učitanim pločicama. Takva provera može da otkrije ovu promenu. JSON izveštaj ne navodi koji je pojedinačni test prijavio neuspeh, pa se taj podatak ne tvrdi na osnovu samog statusa.

**Zaključak:** testovi proveravaju konkretnu vrednost učitanog atributa, što omogućava otkrivanje pogrešne inicijalizacije rotacije.

## Dodatni primer: ekvivalentan mutant

**Lokacija:** `game/board/hexagonGrid.cpp:81` (isto važi i za odgovarajući izbor na liniji 113).

**Operator:** `cxx_eq_to_ne`; status **Survived**.

```cpp
// Original
const int *rowOffset = (row % 2 == 0) ? rowOffsetEvenRow : rowOffsetOddRow;

// Mutant
const int *rowOffset = (row % 2 != 0) ? rowOffsetEvenRow : rowOffsetOddRow;
```

Oba niza rednih pomeraja imaju isti sadržaj:

```cpp
{0, -1, -1, 0, 1, 1}
```

Promena uslova bira drugi niz sa istim elementima, pa ne menja izračunate susede. To je ekvivalentna izmena u ovom kodu, a ne nedostatak testova. Ovo objašnjenje se odnosi na `rowOffset`; nizovi `colOffset` imaju različit sadržaj.

## Ograničenja tumačenja

Rezultati važe za izabrane test izvršne fajlove, mutacione operatore, putanje i filter pokrivenosti. Brojeve iz različitih skupova ne treba sabirati u jedinstven mutation score projekta: isti mutant može biti prisutan u više izveštaja, a kao u primeru `setIndex`, može imati i različit ishod.

Preživljavanje izmene nije samo po sebi dokaz greške u testu. Posebno treba razlikovati neprovereno ponašanje, ekvivalentne izmene i mutante koje otkriva drugi skup testova. Za timeout-e je potrebna dodatna provera pre pripisivanja konkretnog uzroka.
