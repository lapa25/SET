# SET5

Репозиторий с реализацией генератора потока, хеширования и алгоритма HyperLogLog (+ packed версия),
а также с результатами экспериментов (CSV) и скриптом построения графиков.

---

## Структура репозитория

```

SET5/
├── CMakeLists.txt
├── README.md
│
├── experiment.cpp              # основной эксперимент (ввод параметров через консоль)
├── random_stream_gen.h
├── random_stream_gen.cpp       # генерация слоуваря
├── hash_func_gen.h
├── hash_func_gen.cpp           # генерация хеша
├── hyperloglog.h
├── hyperloglog.cpp             # базовый HyperLogLog (uint8_t регистры)
├── hyperloglog_packed.h
├── hyperloglog_packed.cpp      # усовершенствованный HyperLogLog (6 бит на регистр)
│
├──plot.py                      # построение графиков из CSV (ввод путей через консоль)
│
├── run_p10.csv                 # подробные результаты (база, p=10)
├── summary_p10.csv             # статистики (baseline, p=10)
├── run_p12.csv                 # по аналогии
├── summary_p12.csv             
├── run_p14.csv                
├── summary_p14.csv           
├── run_p12_packed.csv          # packed, p=12
└── summary_p12_packed.csv      # packed, p=12
│
└── report/
    ├── Отчет.docx
    │
    ├── p10/
    │ ├── analysis.txt
    │ ├── fig1_stream.png 
    │ └── fig2_mean_std.png
    │
    ├── p12/
    │ ├── analysis.txt
    │ ├── fig1_stream.png
    │ └── fig2_mean_std.png
    │
    ├── p14/
    │ ├── analysis.txt
    │ ├── fig1_stream.png
    │ └── fig2_mean_std.png
    │
    └── p12_packed/
    ├── analysis.txt
    ├── fig1_stream.png
    └── fig2_mean_std.png
````

---

## Сборка и запуск (C++)

### Сборка
```bash
mkdir -p build
cmake -S . -B build
cmake --build build
````

### Запуск

```bash
./build/SET5
```

Дальше программа попросит параметры (num_streams, stream_len, universe_size, steps, p, seed, hash_seed, packed, имена CSV).

---

## Графики (Python)

Скрипт читает `run.csv` и `summary.csv` и сохраняет графики + `analysis.txt`.

```bash
python3 scripts/plot_hll.py
```

Дальше он спросит пути к CSV и папку, куда сохранить результаты.

---

## Что находится в CSV

* `run_*.csv`:

    * значения по каждому потоку и шагу: истинное `true_F0` и оценка `estimate_N`
* `summary_*.csv`:

    * агрегированные статистики по шагам: среднее и стандартное отклонение оценок
