# Session: startup_exit

## Automatic metadata

- Started at: 2026-09-12T22:49:07+02:00
- Valgrind: valgrind-3.26.0
- Build: Debug, C++20, -O0 -g
- Working directory: /home/julijana/Desktop/2026_Analysis_cascadia/cascadia++/cascadia
- Qt platform override: automatic
- QT_ENABLE_REGEXP_JIT: not set
- Game commit: c32895403185b6ae4a968d4517d8a634adfe80ee
- Finished at: 2026-09-12T22:50:00+02:00
- Valgrind/game exit code: 99

## Akcije

1. Pokrenuta je aplikacija.
2. Prikazao se meni.
3. Zatvorena je aplikacija.

## Zaključci

Memcheck je prijavio curenje memorije alocirane tokom inicijalizacije multimedijalnih biblioteka. Trag poziva prolazi kroz Cascadia++ kod, ali to samo po sebi ne znači da je greška nastala u tom kodu.

Kod predviđa da `Controller` obriše `MainWindow`, a `MainWindow` obriše `Settings`. Njegovi objekti `QMediaPlayer` i `QAudioOutput` imaju Qt roditelja, koji ih automatski oslobađa pri svom uništavanju. Zbog toga nije potvrđeno da je uzrok curenja zaboravljen `delete` u ovim klasama.

**Prijavljeno curenje je zabeleženo, ali njegov tačan uzrok je ostao neutvrđen.**

## Kako se sesija završila

Click Exit dugmeta.

## Pronađeni problemi

* **Curenje od ukupno 110 bajtova:** 56 direktno i 54 indirektno izgubljena bajta. Alokacije potiču iz funkcije `drmGetVersion`, pozvane preko FFmpeg-a i Qt Multimedia. Zapisi 1, 2 i 28 prikazuju indirektne gubitke koji su već uključeni u ukupnu veličinu zapisa 45.
* **Moguće curenje od 720 bajtova:** zapis 57 pripada kategoriji `possibly lost` i odnosi se na učitavanje biblioteka i podatke lokalne za nit. Ovaj nalaz nije potvrđeno curenje.
* **Preostalih 70 zapisa pripada kategoriji `still reachable`:** memorija je ostala dostupna preko pokazivača. Ova kategorija sama po sebi nije dokaz curenja.

Broj od **75 zapisa nije broj potvrđenih grešaka u igri**.
