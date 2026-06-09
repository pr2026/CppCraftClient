![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Qt](https://img.shields.io/badge/Qt-6.0%2B-green.svg)
![Docker](https://img.shields.io/badge/Docker-Sandbox-blue.svg)
![SQLite](https://img.shields.io/badge/Database-SQLite-lightgrey.svg)
![Drogon](https://img.shields.io/badge/framework-Drogon-orange)

# CppCraft — десктопное приложение для обучения программированию на C++.

Это репозиторий для клиентской части.
Проект был разработан студентами 1 курса ПМИ НИУ ВШЭ СПб. Цель проекта - разработать учебную платформу для изучения C++ с возможностью автоматической проверки решений.

## Возможности:
### Для студента:
* Регистрация
* Авторизация
* Редактор кода с подсветкой синтаксиса на C++
* Отправка решений на проверку 
* Получение результата по решениям
* Просмотр статистики по решенным заданиям

### Для преподавателя:
* Регистрация 
* Авторизация
* Возможность добавлять, редактировать и удалять задачи
* Возможность добавлять тесты к задачам
* Просмотр статистики по всем задачам

---
## Технологический стек
* **Язык:** `C++17`
* `Qt 6 (Widgets)` + `Qt Network` - графический интерфейс
* `QScintilla` - редактор кода с подсветкой синтаксиса
* `Cmake` - система сборки
---

## Требования
* компилятор `C++17`
* `Qt 6` (Core, Widgets, Network)
* `QScintilla` 
* `Cmake` (3.16+)


##  Компоненты системы
- `LoginPage` — регистрация и аутентификация
- `TaskPage` — страница студента со списком задач и редактором кода
- `TeacherPage` — страница преподавателя для управления задачами
- `NetworkManager` — коммуникация с сервером

---

## Структура проекта
```bash
CppCraft_front/
├── src/                   # Исходные файлы
├── include/               # Заголовочные файлы
├── ui/                    # Файлы форм Qt Designer
└── CMakeLists.txt
```

## Команда 
1. Балакшина Ярославна [@YaraBalak](https://github.com/YaraBalak) - сервер
2. Рязанская Вероника [@veronica2102](https://github.com/veronica2102) - раннер и базы данных
3. Ярыгина Софья [@sonya-sonya](https://github.com/sonya-sonya) - клиент
4. Виноградов Илья [@domosedy](https://github.com/domosedy) - ментор
