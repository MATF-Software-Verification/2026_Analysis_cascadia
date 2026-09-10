# Izveštaj analize Cascadia++

**Autor**: Julijana Jevtić, indeks 1131/2025

**Projekat**: [Cascadia++](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2024-2025/cascadia/-/tree/main?ref_type=heads)

**Grana**: `main`

**Commit korišćen u `cascadia++/`**: `c32895403185b6ae4a968d4517d8a634adfe80ee`

## 1. Izbor projekta i obrazloženje izbora alata

Cascadia++ je pogodan projekat za analizu verifikacije zato što je njegova osnovna logika konkretna i deterministička, ali ipak uključuje dovoljno promena stanja i kombinatornog ponašanja da simboličko istraživanje i fuzzing budu smisleni. Projekat nije trivijalan primer sa jednom glavnom funkcijom; on ima model table, logiku poteza, stanje igrača i pravila na nivou mreže koja se mogu izvršavati više puta sa različitim ulazima.

Ovaj projekat je dobar izbor i zato što je napisan u jeziku C++ (moja preferenca).
Pre izbora proverila sam da se zaista izgrađuje i pokreće: konfigurisala sam ga pomoću CMake-a, projekat se uspešno kompajlirao, a probno pokretanje pokazalo je da se aplikacija pokreće bez grešaka tokom izvršavanja.

Iz perspektive verifikacije, to ga čini dobrim kandidatom za:

- jedinične testove koji proveravaju očekivano ponašanje izdvojenih logičkih objekata;
- simboličko izvršavanje koje istražuje klase stanja i ograničenja;
- proveru modela koja proverava promene stanja u odnosu na tvrdnje;
- profilisanje koje prepoznaje računski zahtevna mesta u petlji igre i ažuriranju table;
- fuzzing koji opterećuje parsiranje, rukovanje podacima i ponašanje u graničnim slučajevima;
- statičku analizu koja otkriva sumnjive obrasce pre izvršavanja.

Glavna ideja nije samo pronalaženje jedne greške, već pokazivanje kako nekoliko komplementarnih tehnika može zajedno da se primeni na istom projektu.

## 2. Okruženje

- Operativni sistem: Ubuntu 26.04, 64-bitno Linux okruženje
- Sistem za izgradnju: CMake 4.2.3
- Kompajler: GNU g++ 15.2.0, uz standard C++17
- Biblioteka i testni okvir: Qt 6.10.2 i Qt Test
- Alati za analizu i merenje: Valgrind 3.26.0, gcov i LCOV 2.0-1
- Pokretanje testova: CTest
- Okruženje za izvršavanje skripti: Python 3.14.4

## 3. Jedinični testovi

Za proveru izabranih delova aplikacije Cascadia++ pripremljeni su testovi u okviru **Qt Test**, dok su za merenje pokrivenosti korišćeni **GCC/gcov i LCOV**. Izgradnja je organizovana pomoću CMake-a, a izvršive datoteke sa testovima registrovane su u okviru CTest-a.

Cilj testiranja je provera očuvanja podataka, rotacije pločica, učitavanja JSON datoteka, susedstva na tabli, izračunavanja poena i pripreme početnog stanja igre.


### 3.1. Konfiguracija i izvršavanje

Za izgradnju testova potrebni su GCC, CMake i Qt 6, uključujući module `Core`, `Test` i `Widgets`. Praćenje pokrivenosti zahteva odgovarajući `gcov`, kao i alate `lcov` i `genhtml`. Projekat za testiranje zahteva CMake 3.16 ili noviji i C++17.

Podešavanja za praćenje pokrivenosti nalaze se u lokalnoj datoteci `unit_tests/coverage.cfg`, koja se pri prvom podešavanju dobija kopiranjem primera:

```bash
cp unit_tests/coverage.cfg.example unit_tests/coverage.cfg
```

Kopiranje nije potrebno ponavljati ako lokalna konfiguracija već postoji. U njoj se podešavaju GCC izvršiva datoteka (`CXX`), odgovarajući alat `GCOV_TOOL`, broj paralelnih poslova, uključivanje sanitajzera, rano filtriranje izvora i automatsko otvaranje HTML izveštaja. Za ovu konfiguraciju pokrivenosti podržan je GCC.

Iz korena repozitorijuma analiza se pokreće komandom:

```bash
bash unit_tests/coverage.sh
```

Skripta izgrađuje testove u direktorijumu `build_coverage`, resetuje prethodne brojače, pokreće CTest, prikuplja podatke o izvršavanju i generiše HTML izveštaj. U konfiguraciji koriste se opcije `-O0`, `-g` i `--coverage`. Instrumentacija sanitajzerima podešava se nezavisno opcijom `ENABLE_SANITIZERS`.

Testovi grafičkih klasa koriste `QApplication` i platformu `offscreen`, pa se izvršavaju bez prikazivanja prozora. Resursi potrebni za proveru slika uključuju se preko originalne datoteke `resource.qrc`.

Skripta čuva rezultate pojedinačnih pokretanja u direktorijumima `unit_tests/reports/run_*`, a poslednji uspešno generisan HTML izveštaj i odgovarajući log testova čuvaju na putanjama:

- `unit_tests/reports/html/index.html` — početna stranica izveštaja o pokrivenosti;
- `unit_tests/reports/tests.log` — izlaz testova, uključujući neuspešne provere.

Generisanje pokrivenosti nastavljam i kada CTest prijavi neuspešne testove, a izlazni status testiranja se čuva. Ako generisanje izveštaja ne uspe, prethodno objavljen HTML ne treba smatrati rezultatom novog pokretanja.

### 3.2. Organizacija i sadržaj testova

Pripremljeni skup obuhvata **12 proizvodnih klasa i 14 Qt Test izvršivih programa**, registrovanih kao zasebni CTest testovi. Dve dodatne datoteke proširuju provere alternativnim granama u klasama `Hexagon` i `Scoring`. 

| Datoteka sa testovima | Klasa | Glavni predmet provere |
|---|---|---|
| `test_tile_data.cpp` | `TileData` | Stanje pločice na tabli, geteri i seteri, serijalizacija, deserializacija i zamena prethodnih lista |
| `test_player_data.cpp` | `PlayerData` | Identitet i ime igrača, kolekcija centralnih pločica i očuvanje podataka pri serijalizaciji |
| `test_turn.cpp` | `Turn` | Konstruktor poteza, token, indeksi, podaci o pločicama, serijalizacija i provere pokazivača |
| `test_tile.cpp` | `Tile` | Sadržaj pločice, rotacije, prelazak između 0° i 300°, pun krug i očuvanje rotacije pri deserializaciji |
| `test_tile_storage.cpp` | `TileStorage` | Učitavanje pločica i početnih grupa iz JSON-a, redosled, prazne kolekcije i greške pri učitavanju |
| `test_hexagon_grid.cpp` | `HexagonGrid` | Susedi u parnim i neparnim redovima, ivice table, simetrija susedstva i označavanje dostupnih polja |
| `test_hexagon.cpp` | `Hexagon` | Geometrija grafičkog polja, kopiranje podataka, prikaz u sceni i signal klika |
| `test_token.cpp` | `Token` | Indeks i oblik tokena, serijalizovana polja, slike životinja, vlasništvo scene i signal klika |
| `test_player.cpp` | `Player` | Postavljanje početnih pločica, stanje igrača, nezavisnost kopije table i objavljivanje izračunatih poena |
| `test_scoring.cpp` | `Scoring` | Bodovanje životinja i povezanih staništa, uz regresione scenarije za nepravilne grupe |

### 3.3. Metodologija

Za poređenje stvarnih i očekivanih vrednosti koriste se `QCOMPARE` i `QVERIFY`, a za očekivane izuzetke `QVERIFY_EXCEPTION_THROWN`. Provere signala koriste `QSignalSpy`. Testovi sa više ulaza organizovani su pomoću funkcija sa sufiksom `_data`, redova `QTest::newRow` i preuzimanja podataka preko `QFETCH`.

Obuhvaćeni su tipični ulazi i granični slučajevi. Za učitavanje se koriste male privremene datoteke sa unapred poznatim sadržajem, nezavisno od veličine originalne kolekcije.

Kod serijalizacije proveravaju se i pojedinačna zapisana polja i povratno pretvaranje u objekat. Deserializacija se dodatno proverava pomoću ručno sastavljenih mapa, kako očekivani ulaz ne bi uvek zavisio od iste proizvodne funkcije za serijalizaciju.

Sve testne klase koriste isti obrazac: deklaracije testova nalaze se u `private slots`, a definicije ispod klase imaju oblik `TestClass::method()`. Qt Test automatski poziva `init()` pre svake testne funkcije, odnosno svakog reda podataka, i `cleanup()` nakon nje.

### 3.4. Rezultati pokrivenosti

![Unit Tests Coverage](images/unit_tests_coverage.png)

Iz posmatranih klasa ukupno su ostale nepokrivene **24 linije, 5 funkcija i 1012 grana**. Procenti se odnose na odabranih 12 implementacionih datoteka, a ne na celu aplikaciju, sva njihova zaglavlja ili sve zavisnosti.

Visoka pokrivenost linija pokazuje da je izvršen veliki deo izabranih implementacija. Niža pokrivenost grana pokazuje da nisu zabeleženi svi ishodi grananja. Jedna izvršena linija može sadržati složen uslov sa više mogućih ishoda. LCOV/gcov može evidentirati i grane povezane sa obradom izuzetaka koje generiše C++ kompajler; bez pregleda pojedinačnih zapisa ne može se utvrditi njihov udeo među nepokrivenim granama.

### 3.5. Uočeni problemi

Pokrivenost nije mera ispravnosti rezultata. Na primer, `Turn` ima 100% pokrivenosti linija i funkcija, ali test konstruktora ipak otkriva izgubljenu vrednost tokena. 

Uočavamo **pet neuspešnih testnih funkcija u tri Qt Test klase** (`turn_tests`, `tile_tests` i `scoring_tests`). Dve provere u klasi `TestTile` reprodukuju isti propust, pa ovi rezultati predstavljaju **četiri različita problema**.

#### 3.5.1. Konstruktor klase Turn ne čuva prosleđeni token

Test `constructorPreservesPlacedToken()` konstruiše `Turn` sa argumentom `"bear"` i odmah proverava `getPlacedToken()`. Dobijena je prazna niska `""`. Pregled `common/turn.cpp` pokazuje da parametar `placedToken` nije upotrebljen u listi inicijalizacije članova, pa član tipa `QString` ostaje prazan.

**Posledica:** objekat gubi token prosleđen konstruktoru. U istom konstruktoru nedostaju inicijalizacije `playerId` i `newTile`.

**Predlog ispravke:** inicijalizovati `placedToken`, `playerId` i `newTile` prosleđenim argumentima, uz redosled članova iz `turn.h`.

#### 3.5.2. Deserializacija klase Tile ne vraća rotaciju

Prvi test u `test_tile.cpp` serijalizuje pločicu rotiranu za 120° i učitava podatke u novu pločicu. Iako mapa sadrži rotaciju, odredište ostaje na 0°. Drugi test polazi od objekta rotiranog za 240° i učitava nezavisno pripremljenu mapu sa rotacijom 60°; objekat ostaje na 240°.
Vidimo da se pri deserializaciji zadržava prethodna rotacija odredišta.

**Posledica:** rotacija se ne očuva pri povratnom pretvaranju (`fromVariant()`) i ne ažurira pri učitavanju nove vrednosti.

**Predlog ispravke:** u `Tile::fromVariant()` učitati `m_rotation` iz mape i zadržati obe provere kao regresione testove istog propusta.


#### 3.5.3. Savijena grupa losova pogrešno se boduje kao pravolinijska

Test postavlja losove (`elk`) na polja `(2,2)`, `(2,3)` i `(3,3)`. Grupa je povezana, ali nije pravolinijska. Dozvoljena podela je par i jedan pojedinačni token, što daje **5 + 2 = 7** poena. Program vraća **9**.
Pregled `generateAllElkLines()` i `findElkLine()` pokazuje prikupljanje povezanih tokena bez odgovarajuće provere pravolinijskog rasporeda. Dobijena grupa veličine tri zatim se boduje sa devet poena.

**Posledica:** u ovom scenariju rezultat je netačan, uvećan je za dva poena.

**Predlog ispravke:** formirati dozvoljene pravolinijske grupe i izabrati najvredniju podelu bez višestrukog bodovanja istog tokena.

#### 3.5.4. Razgranata grupa lososa pogrešno se boduje kao niz

Test postavlja losose (`salmon`) na `(4,4)`, `(4,3)`, `(3,4)` i `(5,4)`. Središnji token ima tri susedna lososa. Očekuje se **0** poena, a dobijeno je **11**.
Dozvoljen niz ima najviše dva susedna lososa po tokenu i ne sme dodirivati druge losose van niza.

`findSalmonRun()` prikuplja povezanu grupu, ali ne proverava ograničenje broja suseda. `calculateSalmonScore()` zatim veličinu grupe, četiri, pretvara u 11 poena.

**Posledica:** nevažeća grupa dobija 11 poena. 
**Predlog ispravke:** proveriti validnost cele grupe pre dodele poena i odbaciti razgranate grupe.

### 3.6. Ograničenja i evidencija izvršavanja

Analiza ne obuhvata kompletnu mrežnu komunikaciju, sve tokove partije, sve interakcije korisničkog interfejsa niti sve varijante kartica bodovanja. Grafički testovi sa platformom `offscreen` proveravaju određene objekte, signale i prikaz, ali ne zamenjuju proveru celog korisničkog toka. Kod pripreme igre nisu iscrpno ispitane sve nevažeće kombinacije ulaza i svi mogući ishodi slučajnog mešanja.

## Zaključak
