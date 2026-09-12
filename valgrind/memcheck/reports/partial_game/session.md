# Session: partial_game

## Automatic metadata

- Started at: 2026-09-12T23:25:50+02:00
- Valgrind: valgrind-3.26.0
- Build: Debug, C++20, -O0 -g
- Working directory: /home/julijana/Desktop/2026_Analysis_cascadia/cascadia++/cascadia
- Qt platform override: automatic
- QT_ENABLE_REGEXP_JIT: not set
- Game commit: c32895403185b6ae4a968d4517d8a634adfe80ee
- Finished at: 2026-09-12T23:28:54+02:00
- Valgrind/game exit code: 99

## Akcije

1. Pokrenuta je aplikacija.
2. Pokrenuta je partija za jednog igrača i potvrđen unos korisničkog imena.
3. Izvršeno je postavljanje pločice i tokena, nakon čega je obrađen završetak poteza, više puta.
4. Kliknuto je dugme `Back`, čime je otvoren dijalog za potvrdu povratka u glavni meni.

## Zaključci

Pregled koda pokazuje **propust u oslobađanju objekata partije za jednog igrača**.

U funkciji `Controller::startSinglePlayerGame()` promenljive `board` i `game` deklarisane su kao lokalne. One su odvojene od istoimenih članova klase `Controller`, koji ostaju `nullptr`. Destruktor proverava upravo te članove, pa ne oslobađa objekte napravljene u ovoj funkciji.

Ovi objekti nemaju ni Qt roditelja koji bi ih automatski obrisao. Konstruktor `Board` prosleđeni prozor čuva kao pokazivač, ali ga ne postavlja kao `QObject` roditelja. `GameGui` takođe ne briše objekat `Game`.

Slično tome, napravljeni `Player` nije dodat u člansku kolekciju `Controller::m_playerOrder`, koju destruktor oslobađa. Kolekcije običnih pokazivača u drugim objektima ne brišu igrača automatski.

**U logu su ovi objekti klasifikovani kao `still reachable`: ostali su alocirani i dostupni preko pokazivača.** Sama kategorija ne dokazuje grešku, ali pregled koda ovde otkriva nedostajuće oslobađanje.

## Kako se sesija završila

Click Exit dugmeta.

## Pronađeni problemi

**1. Neoslobođeni objekti partije u Cascadia++ kodu**

| Objekat  | Zapis | Veličina samog objekta | Memcheck kategorija |
| -------- | ----: | ---------------------: | ------------------- |
| `Player` |   736 |              152 bajta | `still reachable`   |
| `Game`   |   940 |            328 bajtova | `still reachable`   |
| `Board`  |   958 |            368 bajtova | `still reachable`   |

Ove veličine ne uključuju sve dodatne alokacije njihovih tabli, pločica i drugih podataka.

**2. Curenje pri obradi tastature**

Zapis 1165 prijavljuje **159.451 bajt**, odnosno 24 direktno i 159.427 indirektno izgubljenih bajtova. Alokacija potiče iz `xkb_compose_state_new`, preko Qt Wayland-a, tokom unosa imena. Tačan uzrok gubitka nije utvrđen.

**3. Curenje pri inicijalizaciji multimedije**

Zapis 636 prijavljuje **110 bajtova**, odnosno 56 direktno i 54 indirektno izgubljena bajta, kroz `libdrm`, FFmpeg i Qt Multimedia. To odgovara obrascu iz prethodne sesije.

**4. Tri neispravna čitanja memorije**

Zapisi 1–3 prijavljuju `Invalid read of size 16`. Tragovi alokacije vode kroz Qt i Wayland dodatak `libadwaita`, dok su mesta samih čitanja prikazana kao `???`. Nije potvrđen uzrok u Cascadia++ kodu.

**5. Moguće curenje i preostale alokacije**

Zapis 1026 prijavljuje **720 bajtova `possibly lost`** pri učitavanju biblioteka i upravljanju podacima niti.
