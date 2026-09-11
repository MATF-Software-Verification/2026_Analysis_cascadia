# Memcheck na unit testovima

**Ni u jednom od dvanaest pokretanja memorija nije klasifikovana kao definitely, indirectly ili possibly lost.**

- U devet pokretanja prijavljeno je **0 Memcheck errors**.
- U tri pokretanja prijavljeno je po **1 invalid read while Qt was initializing**.
- Sva tri error trace-a prate istu putanju poziva bibliotečkih funkcija.
- Ovi logovi **ne potvrđuju greške u upravljanju memorijom u logici igre Cascadia++**.

Tri greške ipak beležimo. Utvrdili smo gde su prijavljene i kojim redosledom:

1. Testni program se pokreće.
2. Kreira objekat Qt aplikacije, `QApplication`.
3. Qt inicijalizuje svoju platformu za rad bez prikazivanja prozora i podršku za unos teksta.
4. Memcheck prijavljuje invalid read tokom te inicijalizacije.


## Rezultati po testnom programu

„Lost bytes“ u nastavku obuhvataju definitely, indirectly i possibly lost memory; svaka kategorija je zasebno jednaka nuli u svakom logu. 

| Executable / source log | Memcheck errors | Lost bytes | Still reachable bytes |
| --- | ---: | ---: | ---: |
| [cascadia_unit_tests](memcheck_cascadia_unit_tests.log) | 0 | 0 | 17,400 |
| [player_data_tests](memcheck_player_data_tests.log) | 0 | 0 | 17,400 |
| [turn_tests](memcheck_turn_tests.log) | 0 | 0 | 17,400 |
| [tile_tests](memcheck_tile_tests.log) | 0 | 0 | 17,400 |
| [tile_storage_tests](memcheck_tile_storage_tests.log) | 0 | 0 | 17,400 |
| [token_tests](memcheck_token_tests.log) | 0 | 0 | 24,547 |
| [scoring_tests](memcheck_scoring_tests.log) | 1 | 0 | 24,610 |
| [hexagon_branch_tests](memcheck_hexagon_branch_tests.log) | 1 | 0 | 24,914 |
| [hexagon_grid_tests](memcheck_hexagon_grid_tests.log) | 1 | 0 | 24,914 |
| [hexagon_tests](memcheck_hexagon_tests.log) | 0 | 0 | 24,914 |
| [player_tests](memcheck_player_tests.log) | 0 | 0 | 24,914 |
| [scoring_branch_tests](memcheck_scoring_branch_tests.log) | 0 | 0 | 24,914 |


## Primer greške, korak po korak

U [memcheck_hexagon_grid_tests.log](memcheck_hexagon_grid_tests.log) greška počinje na **line 7**:

```text
Invalid read of size 16
```

To znači: „Program je pokušao da pročita 16 bajtova, a deo tog čitanja bio je izvan memorije koju Memcheck smatra validnom.“

Kasnije u istoj dijagnostici vidimo:

```text
Address 0x99ef28b is 27 bytes inside a block of size 42 alloc'd
```

Jednostavnom računicom:

| Detalj | Vrednost |
| --- | ---: |
| Size of the allocated block | 42 bytes |
| Offset where the read starts | 27 bytes |
| Bytes remaining from that point | 42 − 27 = 15 bytes |
| Bytes requested by the read | 16 bytes |
| Amount past the end | 1 byte |

**Ovo je invalid read, a ne leak.** 

Sva tri testna programa u kojima Memcheck prijavljuje grešku ulaze u isti Qt/IBus initialization path i prijavljuju isti obrazac čitanja od 16 bajtova. To ukazuje na **mogući zajednički problem tokom inicijalizacije Qt/IBus komponenti pri pokretanju**. To ne potvrđuje postojanje tri nezavisne greške u igri.

Iz dostupnih logova nije jasno zašto drugi GUI testovi to nisu prijavili. Uzrok nije utvrđen, pa ne možemo reći da je problem bezopasan niti odrediti ispravku u kodu igre.

Tragovi still reachable allocations uključuju GLib, libgomp, DBus i Fontconfig. Pet testnih grupa zadržava po **17,400 bytes in 17 blocks**, sa podudarnim library allocation paths. To podržava tumačenje da je reč o stanju koje biblioteke zadržavaju tokom rada procesa.
