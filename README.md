st130061@student.spbu.ru Ермаков Кирилл Андреевич 24Б82-мм

# ASCII85 encoder/decoder

CLI‑утилита для кодирования/декодирования в ASCII85, тестирование (GoogleTest и Python) и документация Doxygen.

## Структура
- `include/ascii85.h` — хэдер
- `src/ascii85.cpp` — реализация
- `src/main.cpp` — CLI (stdin → stdout)
- `tests/` — C++ и Python тесты
- `Makefile` — сборка, тесты, запуск
- `Doxyfile` и `docs/` — Doxygen

## Сборка
```bash
make all
```

## Запуск CLI
- Поточное кодирование (по умолчанию):
```bash
make run
```
- Поточное декодирование:
```bash
make run ARGS='-d'
```
- Буферная работа:
```bash
make run ARGS='-b'     # кодирование
make run ARGS='-db'    # декодирование
```

## Тесты
- GoogleTest:
```bash
make test-gtest
```
- Python:
```bash
make test-python
```
- Всё сразу:
```bash
make test
```

## Документация
```bash
make docs-create #Создание
make ocs-open    #Открытие
```
HTML: `docs/html/index.html`.

## Очистка
```bash
make clean
```
