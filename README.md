# Тестовое задание для стажера по направлению *Разработчик C/C++*
## https://github.com/Ksenia-rgb/infotecs-traineeship-developer-c-cxx
Задание основано на аналогичном проекте: https://github.com/Ksenia-rgb/infotecs-traineeship-developer-cxx
Основные отличия:
- Система сборки проекта: Makefile вместо cmake


`tecslog` - разработанная в ходе выполнения задания библиотека для записи сообщений в журнал с разными уровнями важности.

`demo-app` - приложение для проверки работы библиотеки
## Структура папок
- `tecslog` - папка, содержащая файлы библиотеки
  - `include` - заголовочные файлы библиотеки для подключения
  - `src` - файлы с реализацией библиотеки
- `demo-app` - папка, содержащая файлы приложения для проверки работы библиотеки
- `tests` - папка, содержащая файлы с Unit-тестами библиотеки

## Сборка проекта
Проект собирается с помощью Makefile.

**Поддерживаемые цели сборки make:**
- Сборка библиотеки (библиотека собирается как динамическая)
```
make tecslog
```
- Сборка демонстрационного приложения
```
make demo-app
```
- Сборка всех тестов
```
make tests
```

- Запуск демонстрационного приложения
```
make run
```
Для передачи дополнительных параметров используется переменная ARGS (при помощи GNU Make):
```
make run ARGS="logs/monday.log INFO"
```

- Запуск тестов для библиотеки (должны быть заранее собраны командой выше)
```
make test
```
- Запуск тестов для библиотеки с подробным выводом
```
make test-verbose
```

**Запуски:**
- Запуск демонстрационного приложения с передачей параметров:
```
./bin/demo-app ../logs/monday.log INFO
```


## Примеры использования библиотеки
### Доступные уровни важности сообщений
- INFO
- WARNING
- ERROR
### Инициализация библиотеки
Инициализация проводится с помощью команды `init`, имеющей параметры:
1. Имя файла журнала для записи логов. Если такой файл не может быть открыт для записи, будет установлен файл по умолчанию (`tecslog.log`)
2. Уровень важности сообщения по умолчанию. Если будет указан некорректный уровень (в случае указания строки), будет установлен минимальный уровень (INFO).

Инициализация может быть пропущена - будут установлены описанные значения по умолчанию.
### Базовое использование
```c++
#include <tecslog/tecslog.hpp>

int main() 
{
  tecslog::init("logs/monday.log", tecslog::Level::INFO);
  tecslog::info("Welcome to tecslog");
  tecslog::warning("Some warning message");
  tecslog::error("Some error message);
  
  tecslog::setLevel(tecslog::Level::DEBUG); // Set default log level to debug
  tecslog::info("This message should be displayed");

  tecslog::log(tecslog::Level::DEBUG, "Another debug displayed message");
  tecslog::log(tecslog::Level::ERROR, "Another error displayed message");
}
```

### Использование строковых представлений Level
```c++
#include <tecslog/tecslog.hpp>

int main() 
{
  tecslog::init("logs/monday.log", "INFO"); // Set correct str level

  tecslog::log("INFO", "Welcome to tecslog");
  tecslog::log("WARNING", Some warning message");
  tecslog::error("ERROR", "Some error message);
  
  std::error_code code = tecslog::setLevel("CRITICAL"); // Set incorrect default level and get error_code != 0
  if (code)
  {
    std::cerr << code.message() << '\n'
    return 1;
  }

  code = tecslog::log("WARN", "Incorrect warn message"); // Specify incorrect log level and get error_code != 0
  if (code)
  {
    std::cerr << code.message() << '\n'
    return 1;
  }

  tecslog::printPossibleLevels(std::cout); // Output: 0 INFO 1 WARNING 2 ERROR
}
```
### Использование без начальной инициализации
```c++
#include <tecslog/tecslog.hpp>

int main() 
{
  tecslog::info("Hello, everyone"); // Log in default file (tecslog.log) with default level specified as min level (INFO)
}
```

## Использование демонстанционного приложения
- Запуск демонстрационного приложения с передачей параметров:
```
./bin/demo-app ../logs/monday.log INFO
```
### Параметры приложения
1. Имя файла журнала для записи логов
2. Уровень важности сообщения по умолчанию

### Пользовательские команды
Демонстрационное приложение является консольным. Пользователю доступны такие команды:
- Установка нового уровня важности сообщения по умолчанию. В параметрах указывает *уровень*.

`default <level>`

- Установка уровня важности сообщения, который будет использован, если уровень не был указан в команде log. В параметрах указывается *уровень*.

`silence <level>`

- Запись лога в журнал. Параметры:
  - *Сообщение*. Если в сообщении есть пробелы, оно должно быть обозначено кавычками (message, "message", "long message")
  - *Уровень*. Опциональный параметр. Может быть опущен, если до этого была использована команда `silence`, в этом случае сообщение логируется с уровнем, указанным в последней команде `silence`.

`log <message> [level]`

- Вывод вспомогательной информации о командах.

`help`


Признаком конца пользовательского ввода является EOF (на Linux: Ctrl + D | на Windows Ctrl + Z затем Enter).

### Пример использования приложения
```c++
./bin/demo-app ../logs/monday.log WARNING
log "check message" WARNING
log "info message" INFO
default INFO
log "info message again" INFO
silence ERROR
log error
```
Файл `../logs/monday.log`:
```
11.08.2026 16:35:56 [WARNING] check message
11.08.2026 16:36:44 [INFO] info message again
11.08.2026 16:36:59 [ERROR] error
```
