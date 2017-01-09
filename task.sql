-- -*- coding: utf-8 -*-

-- Kod tworzący podstawowe tabele bazy danych procedur komputerowych i
-- systemu debian live. Domyślnie hasło root-a m...b...<n>. Pamiętać o
-- ustawieniu bind-address na 0.0.0.0 w /etc/mysql/my.cnf

-- Użycie po zalogowaniu do bazy jako root: source task.sql

CREATE DATABASE task2;

USE task2;

-- Użytkownik task wpuszczany zewsząd, hasło task

-- CREATE USER 'task'@'%' IDENTIFIED BY 'task';

-- Wszystkie przywileje na tabelach tej bazy, bo chcemy mieć możliwość
-- zmiany struktury tabeli danych, gdyby w trakcie realizacji projektu
-- doszły nowe zmienne.

GRANT ALL PRIVILEGES ON task2.* to 'task'@'%';
FLUSH PRIVILEGES;

-- Tabela sesji - informacje na temat rozpoczętych i zakończonych
-- sesji poszczególnych zadań

CREATE TABLE IF NOT EXISTS session (
session_id INT(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,     
project VARCHAR(100) COLLATE utf8_polish_ci NOT NULL,
cnd VARCHAR(50) COLLATE utf8_polish_ci, -- condition zarezerwowane
subject VARCHAR(100) COLLATE utf8_polish_ci NOT NULL,
age INT NOT NULL,
gender ENUM('M','K') COLLATE utf8_polish_ci NOT NULL,
stage ENUM('started','finished') COLLATE utf8_polish_ci NOT NULL,
timestamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
project_sha VARCHAR(60) COLLATE utf8_polish_ci,
lib_sha VARCHAR(60) COLLATE utf8_polish_ci,
tag VARCHAR(50)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

CREATE TABLE IF NOT EXISTS session_tables (
session_id INT(11) NOT NULL,
table_name VARCHAR(50) COLLATE utf8_polish_ci NOT NULL,
stage ENUM('started','finished') COLLATE utf8_polish_ci NOT NULL,
timestamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
KEY (session_id), -- pomijamy nazwę, automatycznie przydzieli session_id
FOREIGN KEY (session_id) REFERENCES session (session_id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

CREATE TABLE IF NOT EXISTS projects (
name VARCHAR(100) COLLATE utf8_polish_ci NOT NULL,
description VARCHAR(1000) COLLATE utf8_polish_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;