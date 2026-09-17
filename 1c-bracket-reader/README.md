# 1C Bracket Format Reader

Приложение на C++17 и Qt для чтения и просмотра файлов скобочного формата 1С.

## Требования

- Компилятор с поддержкой C++17 (g++ 8+, clang 7+, MSVC 2019+)
- CMake 3.16+
- Qt5 или Qt6 (библиотека Widgets)

## Установка зависимостей

### Linux (Debian/Ubuntu)

```bash
# Для Qt5
sudo apt-get update
sudo apt-get install qtbase5-dev cmake g++ make

# Для Qt6
sudo apt-get install qt6-base-dev cmake g++ make
```

### Linux (Fedora/RHEL)

```bash
# Для Qt5
sudo dnf install qt5-qtbase-devel cmake gcc-c++ make

# Для Qt6
sudo dnf install qt6-qtbase-devel cmake gcc-c++ make
```

### macOS

```bash
brew install qt cmake
```

### Windows

1. Установите Visual Studio 2019 или новее с компонентом "Desktop development with C++"
2. Установите Qt через официальный установщик: https://www.qt.io/download
   - Выберите Qt 5.15.x или Qt 6.x
   - Отметьте компоненты: MSVC 2019 64-bit, Qt Widgets

## Сборка

### Локальная сборка

#### Linux/macOS

```bash
cd 1c-bracket-reader
mkdir build && cd build
cmake ..
make -j$(nproc)  # или make -j$(sysctl -n hw.ncpu) для macOS
```

#### Windows (Command Prompt)

```cmd
cd 1c-bracket-reader
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### Windows (PowerShell с Qt)

```powershell
# Инициализация среды Qt (путь может отличаться)
& "C:\Qt\6.5.0\msvc2019_64\bin\qtenv2.bat"

cd 1c-bracket-reader
mkdir build
cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Статическая сборка

Для создания исполняемого файла без внешних зависимостей используйте опцию `BUILD_STATIC`:

```bash
mkdir build && cd build
cmake -DBUILD_STATIC=ON ..
cmake --build . --config Release
```

**Примечание:** Для статической сборки требуется Qt, собранный со статическими библиотеками.

## GitHub Actions

Проект настроен для автоматической сборки на GitHub Actions при:
- Пуше в ветки `main` или `master`
- Создании pull request
- Создании релиза

Сборка производится для трёх платформ:
- Ubuntu Linux
- Windows (MSVC)
- macOS

Артефакты сборки доступны в разделе Actions или в релизах.

## Использование

1. Запустите приложение
2. В меню выберите "Файл" → "Открыть..." или нажмите Ctrl+O
3. Выберите файл в скобочном формате 1С
4. Файл будет отображен в виде дерева
5. При клике на узел дерева в статусбаре отображается полный путь до этого узла

## Структура проекта

```
1c-bracket-reader/
├── .github/workflows/    # GitHub Actions workflow
├── CMakeLists.txt        # Конфигурация сборки
├── README.md             # Этот файл
├── test_data.txt         # Тестовый файл
└── src/
    ├── main.cpp            # Точка входа
    ├── mainwindow.h/cpp    # Главное окно приложения
    ├── bracketparser.h/cpp # Парсер скобочного формата
    ├── treemodel.h/cpp     # Модель дерева для Qt
    └── treenode.h/cpp      # Узел дерева
```

## Особенности парсера

Парсер поддерживает:
- Чтение вложенных структур в фигурных скобках `{...}`
- Строковые значения в кавычках `"..."`
- Числовые значения
- UUID идентификаторы
- Разделители (запятые)
- Пропуск пробельных символов

## Пример

Входной файл (скобочный формат 1С):
```
{1,
{57,uuid-1,uuid-2,
{0,"Значение"},"Текст"}
}
```

Будет представлен в виде дерева:
```
Root
├── 1
├── {5 items}
│   ├── 57
│   ├── uuid-1
│   ├── uuid-2
│   └── {2 items}
│       ├── 0
│       └── Значение
└── Текст
```

## Лицензия

MIT License
