![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Qt](https://img.shields.io/badge/Qt-6.0%2B-green.svg)
![Docker](https://img.shields.io/badge/Docker-Sandbox-blue.svg)
![SQLite](https://img.shields.io/badge/Database-SQLite-lightgrey.svg)
![Drogon](https://img.shields.io/badge/framework-Drogon-orange)

# CppCraft — десктопное приложение для обучения программированию на C++.

Проект был разработан студентами 1 курса ПМИ НИУ ВШЭ СПб. Цель проекта - разработать учебную платформу для изучения C++ с возможностью автоматической проверки решений.
## Текущие возможности (MVP):
* Регистрация пользователя (студент)
* Авторизация пользователя (студент)
* Возможность просмотра списка задач и условия конкретной задачи
* Отправка решений и получение ответа


---
## Технологический стек
* **Язык:** C++17
* **Клиент (GUI):** `Qt 6 (Widgets)` + `Qt Network`
* **Сервер:** [DrogonFramework](https://drogonframework.github.io/drogon-docs/#/ENG/ENG-02-Installation)
* **База данных:** `SQLite3`
* **Изоляция и выполнение:** `Docker`
* **Тестирование:** `doctest`
---

##  Архитектура системы
Проект включает в себя классическую многослойную архитектуру клиент-сервер:

1. **Client Layer (Qt GUI):** Представление данных и взаимодействие с пользователем.
2. **Controller Layer (HTTP API):** Обработка входящих HTTP запросов от клиента, валидация входных данных.
3. **Runner:** Модуль выполнения кода.
   
---


## Быстрый старт
