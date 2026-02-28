如何使用tool生成自动参数数据：
1.  把 自动参数模型7.xlsm 另存为 自动参数模型7.xlsx，擦除宏功能
2.  运行 convert_xls_to_csv.exe，检查是否生成了 "库.csv"
3.  运行 convert_db.bat，检查是否生成了 "cp.db"
4.  把"cp.db"覆盖到 程序目录下 /data