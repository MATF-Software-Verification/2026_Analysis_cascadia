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
- Kompajler: GNU g++ 15.2.0; C++17 za testove i C++20 za izgradnju igre korišćenu uz Memcheck
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

## 4. Valgrind Memcheck

Za dinamičku analizu upravljanja memorijom korišćen je **Valgrind Memcheck**. Alat proverava memorijske operacije tokom izvršavanja programa: neispravna čitanja i upise, upotrebu neinicijalizovanih vrednosti, neispravno oslobađanje i curenje memorije. Analiza je obuhvatila **12 testnih izvršivih programa** i **dve interaktivne sesije igre**. Time su provereni i izdvojeni objekti i deo toka njihove zajedničke upotrebe u aplikaciji.

### 4.1. Konfiguracija i pokretanje

Korišćen je Valgrind 3.26.0. Izgradnje za Memcheck odvojene su od izgradnje za pokrivenost: testovi se izgrađuju u `build_memcheck`, a igra u `build_memcheck_game`. Koristi se konfiguracija `Debug`, sa opcijama `-O0 -g`, bez instrumentacije sanitajzerima i za pokrivenost. Testovi koriste C++17, a analiziran build igre C++20.

Testni programi koji zahtevaju grafičke komponente izvršavaju se sa `QT_QPA_PLATFORM=offscreen`. Interaktivne sesije koriste grafičko okruženje računara; u njihovim metapodacima izbor Qt platforme je automatski.

Za testove se koristi skripta [run_tests_memcheck.sh](valgrind/memcheck/run_tests_memcheck.sh). Pre prvog pokretanja kopira se primer konfiguracije, a u nizu `TESTS` biraju se izvršivi programi:

```bash
cp valgrind/memcheck/memcheck.cfg.example valgrind/memcheck/memcheck.cfg
bash valgrind/memcheck/run_tests_memcheck.sh
```

Postojeću lokalnu konfiguraciju nije potrebno ponovo kopirati. Igra se pokreće skriptom [run_game_memcheck.sh](valgrind/memcheck/run_game_memcheck.sh), uz naziv scenarija:

```bash
bash valgrind/memcheck/run_game_memcheck.sh startup_exit
bash valgrind/memcheck/run_game_memcheck.sh partial_game
```

Pri ponavljanju analize treba sačuvati prethodne rezultate i izabrati novi naziv sesije, na primer `partial_game_repeat`.

Memcheck se pokreće direktno nad izvršivim programima, sa opcijama `--leak-check=full`, `--show-leak-kinds=all`, `--track-origins=yes` i `--num-callers=30`. Opcije `--errors-for-leak-kinds=definite,possible` i `--error-exitcode=99` omogućavaju da prijavljene memorijske greške, uključujući ove kategorije curenja, utiču na izlazni status.

Za svaki test čuvaju se `memcheck_<test>.log` i `<test>_output.log`, dok `summary.tsv` beleži izlazne kodove. Za svaku sesiju igre čuvaju se `memcheck.log`, `game_output.log` i `session.md`. 

### 4.2. Rezultati nad jediničnim testovima

U svih **12 analiziranih pokretanja** kategorije `definitely lost`, `indirectly lost` i `possibly lost` imaju po **0 bajtova**. U devet pokretanja Memcheck prijavljuje nula grešaka, a u tri po jednu grešku pri inicijalizaciji Qt-a.

| Testni program | Memcheck greške | Izgubljena memorija, bajtovi | `Still reachable`, bajtovi |
| --- | ---: | ---: | ---: |
| `cascadia_unit_tests` | 0 | 0 | 17.400 |
| `player_data_tests` | 0 | 0 | 17.400 |
| `turn_tests` | 0 | 0 | 17.400 |
| `tile_tests` | 0 | 0 | 17.400 |
| `tile_storage_tests` | 0 | 0 | 17.400 |
| `token_tests` | 0 | 0 | 24.547 |
| `scoring_tests` | 1 | 0 | 24.610 |
| `hexagon_branch_tests` | 1 | 0 | 24.914 |
| `hexagon_grid_tests` | 1 | 0 | 24.914 |
| `hexagon_tests` | 0 | 0 | 24.914 |
| `player_tests` | 0 | 0 | 24.914 |
| `scoring_branch_tests` | 0 | 0 | 24.914 |


Tri prijave Memcheck greške imaju isti obrazac `Invalid read of size 16` i putanju inicijalizacije Qt/IBus komponenti. U prikazanom primeru čitanje počinje na pomeraju 27 u bloku od 42 bajta: preostaje 15 bajtova, a pokušava se čitanje 16. To je prijava neispravnog čitanja, odvojena od klasifikacije curenja memorije.

Ovi tragovi ukazuju na mogući zajednički problem tokom inicijalizacije biblioteka. Tačan uzrok nije utvrđen, pa prijave nisu označene kao bezopasne niti kao tri nezavisna defekta igre. Tragovi preostale dostupne memorije uključuju GLib, libgomp, DBus i Fontconfig. Kategorija `still reachable` znači da je memorija i dalje dostupna preko pokazivača; sama po sebi ne dokazuje grešku.


### 4.3. Interaktivne sesije igre

| Sesija | Izvršene aktivnosti | Početak–kraj | Način završetka |
| --- | --- | --- | --- |
| `startup_exit` | Pokretanje aplikacije i prikaz glavnog menija | 22:49:07–22:50:00 | Klik na `Exit` |
| `partial_game` | Pokretanje partije za jednog igrača, unos imena, više postavljanja pločice i tokena sa završetkom poteza, otvaranje dijaloga preko dugmeta `Back` | 23:25:50–23:28:54 | Klik na `Exit` |

Obe sesije imaju izlazni kod **99**. Uz korišćenu opciju `--error-exitcode=99`, ovaj status označava da je Memcheck prijavio greške. Evidencija sesija navodi zatvaranje dugmetom `Exit`; kod 99 sam po sebi ne označava pad aplikacije.

#### 4.3.1. Pokretanje i zatvaranje aplikacije — startup_exit

Izdvojeno je 75 zapisa povezanih sa izvornim fajlovima igre. Zapis 45 prijavljuje **110 bajtova** izgubljene memorije: 56 direktno i 54 indirektno. Alokacije potiču iz `drmGetVersion`, pozvane preko FFmpeg-a i Qt Multimedia tokom stvaranja objekta `Settings`. 

Zapis 57 prijavljuje **720 bajtova `possibly lost`** pri učitavanju biblioteka i upravljanju podacima lokalnim za nit. Preostalih 70 zapisa pripada kategoriji `still reachable`.



#### 4.3.2. Deo partije za jednog igrača — partial_game

Ova sesija obuhvata i stvaranje objekata partije i njihovu upotrebu tokom poteza. U izdvojenim zapisima prijavljeni su sledeći nalazi:

| Nalaz | Zapis | Tumačenje |
| --- | ---: | --- |
| 159.451 bajt izgubljene memorije: 24 direktno i 159.427 indirektno | 1165 | Alokacija u `xkb_compose_state_new`, preko Qt Wayland-a, tokom unosa imena; tačan uzrok gubitka nije utvrđen |
| 110 bajtova izgubljene memorije: 56 direktno i 54 indirektno | 636 | Isti obrazac alokacija u putanji libdrm/FFmpeg/Qt Multimedia kao u sesiji `startup_exit` |
| Tri prijave `Invalid read of size 16` | 1–3 | Tragovi alokacije uključuju Qt i Wayland dodatak `libadwaita`; mesta samih čitanja prikazana su kao `???` |
| 720 bajtova `possibly lost` | 1026 | Učitavanje biblioteka i upravljanje podacima lokalnim za nit; curenje nije potvrđeno |

Prisustvo Cascadia++ funkcije u tragu poziva pokazuje iz kog toka aplikacije se došlo do biblioteke. Ono samo po sebi ne određuje ko je odgovoran za gubitak memorije ili neispravno čitanje. 

### 4.4. Propust u oslobađanju objekata partije

U logu sesije `partial_game` sledeći objekti ostali su alocirani i klasifikovani su kao `still reachable`:

| Objekat | Zapis | Veličina samog objekta | Mesto stvaranja |
| --- | ---: | ---: | --- |
| `Player` | 736 | 152 bajta | `controller.cpp:115` |
| `Game` | 940 | 328 bajtova | `controller.cpp:128` |
| `Board` | 958 | 368 bajtova | `controller.cpp:118` |



Pregled [controller.cpp](cascadia++/cascadia/controller/controller.cpp) pokazuje konkretan propust: u `Controller::startSinglePlayerGame()` deklarisane su **lokalne promenljive `board` i `game`**, odvojene od istoimenih članova klase. U ovom toku članski pokazivači ostaju `nullptr`, dok `Controller::~Controller()` proverava i briše upravo njih. Tako predviđeno oslobađanje ne obuhvata objekte napravljene za partiju.

Objekti nemaju ni Qt roditelja koji bi ih automatski obrisao. Konstruktor `Board` u [board.cpp](cascadia++/cascadia/game/board/board.cpp) prosleđeni prozor čuva kao `parentWidget`, ali ne inicijalizuje bazni `QObject` tim roditeljem. `GameGui` čuva pokazivač na `Game`, ali ga njegov destruktor ne briše.

Igrač napravljen u istoj funkciji nije dodat u člansku kolekciju `Controller::m_playerOrder`, koju destruktor oslobađa. Kopije kolekcija pokazivača u `SetupPlayers` i `Game` ne brišu automatski objekte tipa `Player`.

**Posledica:** u pregledanom toku ne postoji predviđeno oslobađanje ovih objekata partije. Njihovi pokazivači mogu ostati dostupni, pa Memcheck kategorija `still reachable` ne isključuje propust u upravljanju životnim vekom. Zaključak o ovom propustu zasniva se na kombinaciji loga i pregleda koda.

**Predlog ispravke:** jasno odrediti vlasnike objekata `Game`, `Board` i `Player`, sačuvati ih u odgovarajućim članovima kontrolera. Pri tome treba uskladiti redosled uništavanja i izbeći višestruko brisanje.

### 4.5. Ograničenja i evidencija

Osnovu rezultata čine dokument **Memcheck na unit testovima** i popunjene datoteke `session.md` za `startup_exit` i `partial_game`. Uz njih se čuvaju izvorni Memcheck logovi i izlaz testnih programa odnosno igre. Brojevi zapisa u odeljku o igri odnose se na izdvojene izveštaje `cascadia_findings.md`, a ne na redove u izvornom logu.

Skripta za izdvajanje zadržava cele zapise koji pominju prepoznate izvorne fajlove igre. Takav pregled olakšava analizu, ali nije potpuna klasifikacija odgovornosti. Navedene veličine iz pojedinačnih zapisa nisu ukupna `LEAK SUMMARY` za ceo proces, a broj izdvojenih zapisa nije broj potvrđenih defekata. Zapisi sa bibliotečkim alokacijama čuvaju se i kada njihov uzrok nije razjašnjen.

Memcheck posmatra samo izvršene putanje. Ove dve sesije ne obuhvataju završetak cele partije, više uzastopnih partija, sve korisničke akcije niti mrežnu igru. Rezultati jediničnih testova zato ne zamenjuju proveru celog toka stvaranja, upotrebe i oslobađanja objekata u aplikaciji.

## 5. Cppcheck analiza

Za statičku analizu izvornog koda aplikacije Cascadia korišćen je alat
Cppcheck. Analiza je sprovedena na osnovu
`compile_commands.json` datoteke, a rezultati su sačuvani u XML formatu i
zatim pretvoreni u HTML izveštaj pomoću alata `cppcheck-htmlreport`.

### 5.1. Preduslovi

Na Ubuntu/Debian sistemu potrebno je dodatno instalirati `cppcheck` alat:

```bash
sudo apt install cppcheck
```

### 5.2. Pokretanje analize

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

### 5.3. Rezultati

Rezultati jednog konkretnog pokretanja dostupni su u direktorijumu
[`reports/`](cppcheck/reports), u HTML i XML formatu. Sažetak nalaza prikazan je u
tabeli ne računajući `inconcl`:

| Kategorija | Broj nalaza | Udeo u ukupnom broju |
|:--|--:|--:|
| `style` | 159 | 56,6% |
| `warning` | 75 | 26,7% |
| `performance` | 41 | 14,6% |
| `information` | 6 | 2,1% |
| `error` | 0 | 0,0% |
| **Ukupno** | **281** | **100,0%** |

![Statistika Cppcheck nalaza](images/statistics_cppcheck.png)

*Slika 1: Raspodela nalaza prema kategorijama.*

Vrste defekata prema učestalosti prikazane su na sledećoj slici:

![Vrste defekata prema učestalosti](images/defect_types_cppcheck.png)

*Slika 2: Raspodela defekata prema tipu.*

### 5.4. Analiza rezultata

**U analiziranom pokretanju nije prijavljen nijedan nalaz kategorije `error`. Dostavljena tabela sadrži 78 upozorenja kategorije `warning`, od kojih su tri označena kao `inconclusive`.**


| Vrsta upozorenja | Broj | Jednostavno značenje |
| --- | ---: | --- |
| `uninitMemberVar` | 59 | Konstruktor ne postavlja početnu vrednost nekog polja. |
| `missingMemberCopy` | 3 | Konstruktor kopije izostavlja neko polje; potrebno je proveriti da li je to namerno. Sva tri nalaza su `inconclusive`. |
| `noOperatorEq` | 2 | Klasa upravlja resursima, ali nema sopstveni operator dodele koji uređuje njihovo kopiranje. |
| `nullPointerRedundantCheck` | 14 | Pokazivač se koristi pre provere da li je `nullptr`, pa je provera zakasnela ili suvišna. |
| **Ukupno** | **78** |  |

| Klasa | Neinicijalizovana polja | Izostavljeno kopiranje | Operator dodele | Provera pokazivača | Ukupno |
| --- | ---: | ---: | ---: | ---: | ---: |
| `PlayerData` | 1 | 0 | 0 | 0 | 1 |
| `Turn` | 9 | 1 | 1 | 0 | 11 |
| `ServerService` | 1 | 0 | 0 | 0 | 1 |
| `Board` | 22 | 0 | 0 | 0 | 22 |
| `Player` | 7 | 2 | 1 | 0 | 10 |
| `TileData` | 10 | 0 | 0 | 0 | 10 |
| `Token` | 3 | 0 | 0 | 0 | 3 |
| `GameGui` | 0 | 0 | 0 | 14 | 14 |
| `LobbyView` | 6 | 0 | 0 | 0 | 6 |
| **Ukupno** | **59** | **3** | **2** | **14** | **78** |


#### Neinicijalizovana polja — `uninitMemberVar` (59)

Primer je defaultni konstruktor `Player()`. Posebno je interesantan pokazivač `m_hexagonGrid`, koji konstruktor ne postavlja, a destruktor koristi u izrazu `delete m_hexagonGrid`. Zato pravljenje lokalnog objekta tim konstruktorom i njegovo uništavanje bez prethodnog postavljanja pokazivača nije bezbedno.

Ipak, u pozivima u `Controller` koriste se konstruktori sa parametrima, koji prave mrežu. Identifikator izostavljen u konstruktoru sa imenom igrača naknadno postavlja `SetupPlayers`. Nije potvrđeno da se problem podrazumevanog konstruktora ispoljava tokom normalne partije.

Drugačiji primer je konstruktor `Turn` sa parametrima: prima `newTile` i `playerId`, ali ih ne upisuje u odgovarajuća polja. To je konkretan propust u čuvanju prosleđenih podataka.


#### Izostavljeno kopiranje polja — `missingMemberCopy` (3)

Kod `Turn` se `ownsData` ne preuzima iz originala, već ostaje `false`. To može biti namerno: kopija pozajmljuje podatke i ne oslobađa ih. Slepo kopiranje vrednosti `true`, uz kopiranje istih pokazivača, omogućilo bi da dva objekta oslobađaju iste podatke. Pozajmljena kopija pritom sme da se koristi samo dok podaci postoje. 

Sva tri nalaza su označena kao `inconclusive`. Potrebno je razjasniti namenu kopije i vlasništvo.

#### Nedostajući operator dodele — `noOperatorEq` (2)

`Player` oslobađa svoj grid u destruktoru i ima konstruktor kopije koji pravi novi grid, ali nema sopstveni operator dodele. Zato `Player b(a)` i `b = a` nemaju isto ponašanje: automatska dodela kopira adresu mreže. Prethodni grid odredišta može ostati neoslobođena, a oba objekta mogu pokušati da oslobode istu grid.

#### Provera pokazivača posle upotrebe — `nullPointerRedundantCheck` (14)

U `GameGui::setupUi()` pokazivač `m_game` koristi se za pristup tabli, pa se tek zatim proverava kroz `if (... && m_game)`. Takva provera ne štiti prethodnu upotrebu.

Ako klasa zahteva postojeći objekat igre, provera je suvišna. Ako prihvata `nullptr`, provera mora prethoditi prvoj upotrebi, uključujući konstruktor koji odmah poziva metode za crtanje. U kodu `Controller` prvo pravi `Game`, pa ga prosleđuje `GameGui`.

### 5.5. Performanse

Cppcheck je prijavio **41 upozorenje** kategorije `performance`:

| Vrsta prijave           |   Broj | Značenje                                                                       |
| ----------------------- | -----: | ------------------------------------------------------------------------------ |
| `returnByReference`     |     29 | Predlaže vraćanje podataka kroz konstantnu referencu radi izbegavanja kopija.  |
| `passedByValue`         |     10 | Predlaže prosleđivanje parametara kroz konstantnu referencu.                   |
| `useInitializationList` |      2 | Predlaže direktnu inicijalizaciju polja umesto naknadne dodele u konstruktoru. |
| **Ukupno**              | **41** |                                                                                |

Najvažnije prijave odnose se na kopiranje mapa rezultata u klasama `Player` i `Scoring`, kao i prosleđivanje stringova i kolekcija po vrednosti. 

**Ovo su predlozi optimizacije, a ne potvrđeni problemi brzine izvršavanja.** Stvarni uticaj treba proveriti merenjem.

## 6. Mull — mutaciono testiranje

Mull pravi male izmene u kodu (mutante), pokreće testove i proverava da li ih testovi otkrivaju.
Na primer, zameni `==` sa `!=` ili `+` sa `-`. Ako testovi prijave grešku, mutant je otkriven.
Ako prođu, mutant je preživeo i treba pregledati šta izmena znači.

### 6.1. Pokretanje

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

### 6.2. Šta testiramo

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

### 6.3. Rezultati

**Najviše pažnje treba posvetiti `PlayerData`, gde je preživelo 9 od 15 mutanata.** Rezultati pokazuju koliko su izabrani testovi osetljivi na razmatrane izmene koda.

| Skup testova | Ukupno mutanata | Killed | Survived | Timeout | Mull mutation score |
|---|---:|---:|---:|---:|---:|
| `cascadia_unit_tests` | 19 | 12 | 7 | 0 | 63% |
| `player_data_tests` | 15 | 6 | 9 | 0 | 40% |
| `tile_storage_tests` | 7 | 5 | 0 | 2 | 71% |
| `hexagon_grid_tests` | 81 | 31 | 13 | 37 | 38% |

Polje mutationScore odgovara udelu Killed u ukupnom broju razmatranih mutanata, prikazanom kao ceo procenat. Timeout-i nisu uračunati kao Killed.


### 6.4. Zaključci po skupu

- **TileData:** 12 od 19 mutanata ima status Killed, a 7 je preživelo. Pregled pojedinačnih izmena je potreban pre zaključka da svaki preživeli mutant predstavlja nedostatak testova.
- **PlayerData:** 6 od 15 mutanata ima status Killed, a 9 je preživelo. Osam preživelih potiče iz `tileData.cpp`, a samo jedan iz `playerData.cpp`; rezultat zato opisuje skup testova sa zavisnostima, a ne isključivo klasu PlayerData.
- **TileStorage:** potvrđeno je da nema preživelih mutanata, ali samo 5 od 7 ima status Killed. Preostala dva su Timeout, pa se ne može tvrditi da su testovi svojim proverama otkrili svih sedam mutacija.
- **HexagonGrid:** 31 od 81 mutanta ima status Killed. Veliki broj timeout-a (37) zahteva oprez pri oceni testova. Među 13 preživelih postoje i izmene koje ne menjaju ponašanje, kao i izmena vizuelnog prikaza koju testovi ne proveravaju.

### 6.5. Primer 1: preživeli mutant u player_data_tests

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

### 6.6. Primer 2: preživela promena izgleda dozvoljenog polja

**Lokacija:** `game/board/hexagonGrid.cpp:97`, funkcija `HexagonGrid::setValidNeighbours`.

**Operator:** `cxx_remove_void_call`.

Mull uklanja poziv:

```cpp
hex->setBrush(QBrush(QColor(219, 224, 157)));
```

Status je **Survived**. Poziv `tile->setIsValid(true)` ostaje, pa polje zadržava oznaku da je dozvoljeno, ali izostaje postavljanje njegove boje.

Postojeći test `selectableNeighbours` proverava logičko stanje polja preko `getPlacedTile()` i `getIsValid()`, a ne boju četkice.

**Zaključak:** ovaj mutant ukazuje na deo vizuelnog ponašanja koji dati testovi ne proveravaju. Moguća dopuna je provera `brush().color()` za dozvoljeno polje nakon poziva `setValidNeighbours`, ako je boja deo zahtevanog ponašanja.

### 6.7. Primer 3: otkrivena pogrešna rotacija pri učitavanju

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

### 6.8. Dodatni primer: ekvivalentan mutant

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

### 6.9. Ograničenja tumačenja

Rezultati važe za izabrane test izvršne fajlove, mutacione operatore, putanje i filter pokrivenosti. Brojeve iz različitih skupova ne treba sabirati u jedinstven mutation score projekta: isti mutant može biti prisutan u više izveštaja, a kao u primeru `setIndex`, može imati i različit ishod.

Preživljavanje izmene nije samo po sebi dokaz greške u testu. Posebno treba razlikovati neprovereno ponašanje, ekvivalentne izmene i mutante koje otkriva drugi skup testova. Za timeout-e je potrebna dodatna provera pre pripisivanja konkretnog uzroka.

## 7. Fuzz testiranje - Google FuzzTest

Proširen skup sadrži **15 fuzz testova za 7 klasa**. Svaki test predstavlja
svojstvo koje FuzzTest proverava nad mnogim generisanim ulazima.

### 7.1. Postavljanje i pokretanje

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

### 7.2. Konfiguracija

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

### 7.3. Šta se testira

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


### 7.4. Ponavljanje nalaza

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

### 7.5. Rezultati

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

## 8. Profilisanje pomoću alata Tracy

Tracy koristimo da izmerimo koliko traju pojedine operacije u igri Cascadia++.
Na primer, možemo da uporedimo vreme bodovanja životinja, vreme bodovanja staništa
i vreme kopiranja mreže igrača.

U funkcije koje pratimo dodate su **zone**. Zona beleži početak i kraj jednog
izvršavanja funkcije. U Tracy te zone vidimo kao blokove na vremenskoj
liniji, a u statistici kao broj poziva i njihova trajanja.

### 8.1. Priprema

Za Ubuntu instaliraj potrebne pakete:

```bash
sudo apt update
sudo apt install build-essential cmake git patch pkg-config \
    qt6-base-dev qt6-multimedia-dev libglfw3-dev libfreetype6-dev libdbus-1-dev \
    gstreamer1.0-plugins-base gstreamer1.0-plugins-good libwayland-dev wayland-protocols
```

Tracy se preuzima automatski pri prvom pokretanju. Prvo kompajliranje zato traje
duže i zahteva internet. Skripta koristi dve paralelne kompilacije (`--parallel 2`).
Koristi se **Tracy 0.11.1**.

### 8.2. Pokretanje scenarija

Skripta redom:

1. Pravi posebnu kopiju izvora u `tracy/build/source` i u nju dodaje Tracy zone.
2. Kompajlira scenario i alat `tracy-capture`, koji prima podatke o izvršavanju.
3. Pokreće snimač, zatim scenario i čeka da se snimanje završi.
4. Čuva snimak i logove u novom direktorijumu unutar `tracy/reports`.

Izvorni fajlovi u podmodulu ostaju neizmenjeni. Za profilisanje se koristi
`RelWithDebInfo`.
Konfiguracija ne uključuje coverage ni sanitizere.


| Zona u snimku | Operacije |
|---|---|
| `Early board - 3 tiles` | 200 obračuna rezultata na tabli sa 3 početne pločice, bez tokena. |
| `Middle board - 12 tiles` | 200 obračuna na tabli sa 12 pločica i 9 tokena. |
| `Late board - 22 tiles` | 200 obračuna na tabli sa 22 pločice i 19 tokena. |
| `Selectable neighbours - 22 tiles` | 200 prolazaka koji označavaju slobodna susedna polja. |
| `JSON loading - 50 repetitions` | 50 učitavanja običnih i početnih pločica, svaki put u nove objekte. |
| `Tile serialization - 200 passes` | 200 prolazaka kroz 85 običnih pločica: pretvaranje u `QVariant` i učitavanje u novi objekat. |

Sve tri table koriste **isti grid 30 × 30**, kakvu pravi klasa `Player`.
Menja se broj postavljenih pločica, a ne dimenzija grida.
Raspored je unapred zadat i povezan. Podaci se uzimaju iz originalnog
`resources/storage.json`: prvi skup početnih pločica i prvih 19 običnih pločica.
Na obične pločice postavlja se po jedna životinja koju ta pločica dozvoljava.
Veće table proširuju manje istim redosledom.

Ovo su ručno formirana stanja za poređenje, a ne snimak nasumično odigrane partije.
Ona omogućavaju da svako pokretanje koristi iste podatke. Učitavanje JSON-a meri
obradu ugrađenog Qt resursa, pa iz njega ne izvodimo zaključke o brzini diska.

Pre svake grupe od 200 obračuna izvršava se još 5 obračuna za zagrevanje.
Priprema tabli i zagrevanje imaju zasebne zone i **ne uključuju se u poređenje**.
U log se ispisuje kontrolni zbir obrađenih rezultata. On pomaže da proverimo da su
dva pokretanja obradila iste podatke; nije dokaz ispravnosti bodovanja ili serijalizacije.

### 8.3. Snimanje same igre

```bash
bash tracy/tracy.sh game
```

Otvoriće se instrumentirana igra. Za jedan pregledan snimak:

1. Pokreni igru za jednog igrača.
2. Postavi nekoliko pločica i tokena, uz bar jednu rotaciju pločice.
3. Otvori prikaz rezultata da se izvrši bodovanje.
4. Zatvori igru i sačekaj poruku skripte da je snimak sačuvan.

Ručno igranje može da pokaže kada se
javlja sporija operacija, dok je automatski scenario pogodniji za ponavljanje
istog opterećenja. Automatski scenario ne simulira sve korisničke akcije igre.

**Ovo neće biti deo analize, ostavljeno je samo kao mogućnost.**

### 8.4. Rezultati

| Fajl | Sadržaj |
|---|---|
| `capture.tracy` | Snimak koji se otvara u Tracy pregledniku. |
| `program.log` | Poruke scenarija ili igre; kod scenarija i kontrolni zbir. |
| `capture.log` | Poruke snimača i eventualni problemi pri povezivanju. |

Svako pokretanje pravi novi direktorijum. 

### 8.5. Otvaranje snimka u Tracyju

Posle prvog uspešnog pokretanja jednom kompajliraj preglednik iz već preuzete
Tracy verzije:

```bash
cmake -S tracy/build/_deps/tracy-src/profiler -B tracy/build/viewer \
    -DCMAKE_BUILD_TYPE=Release -DLEGACY=ON -DNO_PARALLEL_STL=ON
cmake --build tracy/build/viewer --parallel 2
```

`LEGACY=ON` bira X11 prikaz, a `NO_PARALLEL_STL=ON` uklanja potrebu za dodatnom
TBB bibliotekom. Ove opcije se odnose na Tracy alate.

Zatim pokreni pregledač i kroz **Open** izabrati `capture.tracy`:

```bash
./tracy/build/viewer/tracy-profiler
```

### 8.6. Merenje i tumačenje

Patch ukupno dodaje 37 zona u 10 izvornih fajlova. Neke, poput funkcija klase
`Game`, pojavljuju se tek kada pokreneš igru i izvršiš odgovarajuće akcije.

- **Broj poziva** govori koliko puta je funkcija izvršena u izabranom intervalu.
- **Ukupno vreme (total)** je zbir trajanja njenih poziva.
- **Prosečno vreme** je ukupno vreme podeljeno brojem poziva.
- **Maksimalno vreme** pokazuje najduži zabeleženi poziv.
- **Self time** izuzima vreme u ugnježdenim instrumentiranim zonama.

Vremena roditeljske i unutrašnjih zona se preklapaju. Zato ih ne sabiramo kao
nezavisne troškove.
Tracy zone mere proteklo vreme, koje može da obuhvati i čekanje ili prekid rada
niti. Najduži poziv zato nije sam po sebi dokaz sporog algoritma.

![Early fuzz](images/3_fuzz.png)

![Middle fuzz](images/12_fuzz.png)

![Late fuzz](images/22_fuzz.png)


Prikazujemo vreme zone bez vremena ugnježdenih Tracy zona (self only). Zato približno 492 µs kod Player::calculateScore nije ukupno trajanje jednog obračuna.

Sa slika dobijamo sledeća ukupna self vremena za po 200 obračuna:

Zona	 | 3 pločice	| 12 pločica	| 22 pločice
|---|---|---|---|
HexagonGrid::copy	|  104,47 ms	| 	105,24 ms		| 105,18 ms
Player::calculateScore | 	98,37 ms		| 98,46 ms		| 98,91 ms
Scoring::habitatSides	| 1,70 ms	| 	27,06 ms		| 53,61 ms
Scoring::calculateHabitat	| 	1,11 ms	| 	21,36 ms		| 41,18 ms

Kopiranje predstavlja veliki, gotovo stalan trošak. U svakom scenariju imamo 600 kopiranja za 200 obračuna, odnosno tri kopiranja po obračunu.
Ona nastaju:
- pri inicijalizaciji člana Scoring::m_player;
- pri prosleđivanju igrača po vrednosti u calculatePlayerAnimalScore;
- pri prosleđivanju igrača po vrednosti u calculatePlayerHabitatsScore.

Kopiranje igrača pokreće duboko kopiranje celog grida. U sva tri scenarija grid ima 30 × 30 polja, pa se kopira svih 900 polja bez obzira na broj postavljenih pločica. To objašnjava približno jednako vreme kopiranja. 

Obrada staništa postaje znatno skuplja, trošak raste prvenstveno zato što se funkcija poziva mnogo više puta.
U kodu se obilazak staništa pokreće ponovo za svaku postavljenu pločicu, sa novom evidencijom posećenih polja. To omogućava ponovljeni obilazak iste povezane grupe i objašnjava zabeleženi rast.


![Early fuzz1](images/3_fuzz.png)

![Middle fuzz1](images/12_fuzz.png)

![Late fuzz1](images/22_fuzz.png)

**Sada upoređujemo i potvrđujemo naša oučavanja: prosečno trajanje raste sa 1,11 ms na 1,65 ms, odnosno približno 49,5% između početne i završne table.**

Uključeno je **With children**, pa vreme `Player::calculateScore` obuhvata i operacije koje se izvršavaju unutar njega.

| Scenario   | Broj obračuna | Ukupno vreme bodovanja | Prosečno po obračunu |
| ---------- | ------------: | ---------------------: | -------------------: |
| 3 pločice  |           200 |              221,19 ms |              1,11 ms |
| 12 pločica |           200 |              277,54 ms |              1,39 ms |
| 22 pločice |           200 |              330,63 ms |              1,65 ms |

**Kopiranje mreže ima veliki, gotovo stalan trošak.**

`HexagonGrid::copy` traje ukupno **104,47–105,24 ms** u svakom scenariju. Zabeleženo je po **600 poziva**, odnosno tri kopiranja po obračunu.
To odgovara približno **0,52 ms kopiranja po obračunu**. 

Potencijalna optimizacija je smanjenje nepotrebnih kopiranja igrača i mreže, uz proveru gde je bezbedno koristiti reference.

**Najveći deo povećanja vremena dolazi iz obračuna staništa.**

Za `Scoring::calculatePlayerHabitatsScore`, uključujući njegove unutrašnje pozive, imamo:

| Scenario   | Ukupno vreme obračuna staništa |
| ---------- | -----------------------------: |
| 3 pločice  |                        5,42 ms |
| 12 pločica |                       58,13 ms |
| 22 pločice |                      108,81 ms |

Između prve i poslednje table ukupno bodovanje raste za **109,44 ms**, a obrada staništa za **103,39 ms**. Dakle, ona objašnjava najveći deo zabeleženog povećanja.

Ovo je u skladu sa prethodnim nalazom iz koda: obilazak staništa pokreće se iznova za svaku postavljenu pločicu, pa se iste povezane grupe mogu obilaziti više puta.

## Zaključak

Analiza projekta *Cascadia* pokazala je da različite tehnike verifikacije daju različite, ali međusobno dopunjujuće informacije o kvalitetu softvera. 

Jedinični testovi proveravaju konkretno očekivano ponašanje programa, dok pokrivenost pokazuje koji delovi koda su njima obuhvaćeni. Mull dodatno procenjuje kvalitet testova proveravajući da li oni zaista mogu da otkriju promene u programu.
Cppcheck pronalazi potencijalne probleme direktno u izvornom kodu bez izvršavanja programa, dok Valgrind proverava probleme vezane za upravljanje memorijom tokom izvršavanja. FuzzTest automatski generiše veliki broj različitih ulaza i na taj način može da otkrije greške koje klasični testovi ne obuhvate. Tracy se, za razliku od ostalih alata, fokusira na performanse i omogućava pronalaženje delova programa koji troše najviše vremena.

Zajedno, ove tehnike daju znatno potpuniju procenu projekta nego bilo koja od njih pojedinačno, a rezultati pokazuju i da visoka pokrivenost koda sama po sebi nije dovoljna garancija ispravnosti.