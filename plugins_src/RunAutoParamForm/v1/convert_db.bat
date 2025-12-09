@echo off
chcp 65001 >nul
rem echo Unicode text: 你好世界
del /Q cp.db
duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.加工面积 AS FROM '加工面积.csv';"
duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.摇动形状 AS FROM '摇动形状.csv';"
duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.材料形状表 AS FROM '材料形状表.csv';"
duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.目标余量粗糙度 AS FROM '目标余量粗糙度.csv';"
duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.粗糙度 AS FROM '粗糙度.csv';"
duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.库 AS FROM '库.csv';"