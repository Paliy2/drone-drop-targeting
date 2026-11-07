# Діагностика тренувальних випадків

Усі запуски — у контейнері заняття 17 (`miltech-debug`, Debian 12, g++ 12, Valgrind 3.19).
Обидві збірки без жодного попередження під `-Wall -Wextra`.

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build-debug
cmake -S . -B build-asan  -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON && cmake --build build-asan
./build-asan/caseNN
valgrind --leak-check=full --track-origins=yes ./build-debug/caseNN
```

| Випадок | Клас дефекту | Інструмент | ASan/UBSan | Valgrind |
|---|---|---|---|---|
| 01 telemetry | вихід за межі купи (читання) | **AddressSanitizer** | бачить | бачить |
| 02 budget | переповнення знакового `int` | **UndefinedBehaviorSanitizer** | бачить | не бачить |
| 03 wind | читання неініціалізованого значення | **Valgrind Memcheck** | не бачить | бачить |

## Випадок 1 — `case-01-telemetry`

**Інструмент:** AddressSanitizer (Valgrind теж ловить, бо пам'ять на купі).

```
==207==ERROR: AddressSanitizer: heap-buffer-overflow on address 0xffffa2600b70
READ of size 8 at 0xffffa2600b70 thread T0
    #0 in SampleBuffer::push(double) case-01-telemetry/sample_buffer.cpp:15
    #1 in main case-01-telemetry/main.cpp:17
allocated by thread T0 here:
    #0 in operator new[](unsigned long)
    #1 in SampleBuffer::SampleBuffer(unsigned long) case-01-telemetry/sample_buffer.cpp:4
```

Valgrind: `Invalid read of size 8 ... Address ... is 0 bytes after a block of size 32 alloc'd`.

**Причина.** Коли буфер заповнений (`size_ == capacity_ == 4`), зсув іде циклом
`for (i = 0; i < size_; ++i) data_[i] = data_[i + 1];`. На останній ітерації `i = 3`
читається `data_[4]` — перший байт за 32-байтною областю з `new double[4]`. Зсуву
потрібно лише `size_ - 1` кроків.

**Наслідки.** Сміттєве значення з-за меж масиву записується в `data_[3]` і одразу
перезаписується новим зразком, тому вивід правильний. Але це UB: читання може
потрапити на чужі дані або на незамаплену сторінку (аварія), а якщо порядок
`--size_` / запису колись зміниться — сміття потрапить у середнє.

**Виправлення.** Умова циклу `i + 1 < size_` — зсуваються лише наявні сусіди. Вивід не змінився.

## Випадок 2 — `case-02-budget`

**Інструмент:** UndefinedBehaviorSanitizer (Valgrind мовчить — пам'ять не порушено;
ASan теж — це не доступ до пам'яті).

```
case-02-budget/budget.cpp:6:15: runtime error: signed integer overflow:
1800000000 + 450000000 cannot be represented in type 'int'
```

**Причина.** `total_weight` накопичує `v * WEIGHT_PER_VALUE` в `int`. Сума цінностей
900 × 5 000 000 = 4 500 000 000, а `INT_MAX` = 2 147 483 647. Уже на п'ятому доданку
(1 800 000 000 + 450 000 000) значення не вміщується — переповнення знакового цілого є UB.

**Наслідки.** Програма друкує правдоподібні, але хибні числа: `сумарний показник: 205032704`
(це 4 500 000 000 mod 2³²) і `середній на ціль: 15771746`. Оскільки це UB, компілятор
з оптимізаціями може взагалі перетворити цикл непередбачувано.

**Виправлення.** `total_weight` / `average_weight` повертають `long long`, накопичувач —
`long long`, множення робиться в `long long` (`static_cast<long long>(v) * WEIGHT_PER_VALUE`).
Тепер вивід правильний: `4500000000` і `346153846`.

## Випадок 3 — `case-03-wind`

**Інструмент:** Valgrind Memcheck з `--track-origins=yes`. ASan + UBSan мовчать
(у збірці з ними програма навіть надрукувала `-0.0` замість `0.0`).

```
==216== Conditional jump or move depends on uninitialised value(s)
==216==    at 0x4B6D830: __printf_fp_l (printf_fp.c:396)
==216==    ...
==216==    by 0x10971F: main (main.cpp:17)
==216==  Uninitialised value was created by a heap allocation
==216==    at 0x4885828: operator new(unsigned long)
==216==    by 0x109D13: load_profile(...) (wind.cpp:6)
==216== ERROR SUMMARY: 176 errors from 98 contexts
```

**Причина.** `WindProfile` — агрегат без ініціалізаторів, `new WindProfile` (default-initialization)
залишає поля невизначеними. `load_profile` записує лише `x` і `y`; `gust_mps` ніхто не
записує, а `main` його друкує, і `release_mode` порівнює `gust_mps > 12.0`.

**Наслідки.** Рішення про скид залежить від сміття в купі: на цій машині там випадково
нуль → «скид дозволено», в іншій збірці може бути 30.0 → «затримати скид», або навпаки —
дозвіл при реальному поривчастому вітрі. Санітайзери не бачать: ASan перевіряє лише, що
адреса належить живій області пам'яті, а не те, чи значення в ній було записане.

**Виправлення.** Ініціалізатори за замовчуванням у `WindProfile` (`x = 0.0`, `y = 0.0`,
`gust_mps = 0.0`): у метеозведенні поривчастості немає, отже штиль. Вивід не змінився.

## Перевірка після виправлень

```
case01 sanitizer_msgs=0  valgrind: ERROR SUMMARY: 0 errors from 0 contexts
case02 sanitizer_msgs=0  valgrind: ERROR SUMMARY: 0 errors from 0 contexts
case03 sanitizer_msgs=0  valgrind: ERROR SUMMARY: 0 errors from 0 contexts
```

Вивід ASan-збірки побайтово збігається з виводом Debug-збірки для кожного випадку.
