# -*- coding: utf-8 -*-
import os
from datetime import datetime
import sys
import sqlite3
#import pandas as pd  # Optional, but often useful for data manipulation
#from collections import OrderedDict

DEBUG_MODE = False

#SQLITE_DB_PATH = 'plugins/RunAutoForm/'
SQLITE_DB_PATH = 'data/'
SQLITE_DB_NAME = 'cp.db'

# duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.加工面积 AS FROM '加工面积.csv';"
# duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.摇动形状 AS FROM '摇动形状.csv';"
# duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.材料形状表 AS FROM '材料形状表.csv';"
# duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.目标余量粗糙度 AS FROM '目标余量粗糙度.csv';"
# duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.粗糙度 AS FROM '粗糙度.csv';"
# duckdb -c "ATTACH 'cp.db' (TYPE SQLITE); CREATE TABLE cp.库 AS FROM '库.csv';"

def convert_to_sqlite(csv_file_path):
    import duckdb
    table_name = os.path.splitext(os.path.basename(csv_file_path))[0]


    duckdb_conn = duckdb.connect(database=':memory:')  # Use in-memory database
    duckdb_conn.execute(f"CREATE TABLE {table_name} AS SELECT * FROM read_csv_auto('{csv_file_path}', header=true, encoding='UTF-8')")


    sqlite_db_path = SQLITE_DB_NAME
    sqlite_conn = sqlite3.connect(sqlite_db_path)
    sqlite_cursor = sqlite_conn.cursor()

    schema_query = duckdb_conn.execute(f"SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '{table_name}'").fetchall()
    create_table_sql = f"CREATE TABLE {table_name} ("
    create_table_sql += ", ".join([f"{col[0]} {col[1]}" for col in schema_query])
    create_table_sql += ");"

    # Create the table in SQLite
    sqlite_cursor.execute(create_table_sql)

    # Fetch all rows from DuckDB and insert into SQLite
    rows = duckdb_conn.execute(f"SELECT * FROM {table_name}").fetchall()
    insert_sql = f"INSERT INTO {table_name} VALUES ({', '.join(['?'] * len(schema_query))})"
    sqlite_cursor.executemany(insert_sql, rows)

    # Commit changes and close connections
    sqlite_conn.commit()
    sqlite_conn.close()
    duckdb_conn.close()

    print(f"Data has been successfully exported to {sqlite_db_path}")



def get_data(csv_file_path, where = '', order_by = '', count = 1):
    if True:
        table_name = os.path.splitext(os.path.basename(csv_file_path))[0]
        return get_data_by_sqlite(table_name, where, order_by, count)
    else:
        return get_data_by_csv(csv_file_path, where, order_by, count)



def get_data_by_sqlite(table_name, where = '', order_by = '', count = 1):
    if not where:
        where = '1=1'

    try:
        sql_file_path = f"{SQLITE_DB_PATH}{SQLITE_DB_NAME}"
        if not os.path.exists(sql_file_path):
            sql_file_path = f"{SQLITE_DB_NAME}"
        con = sqlite3.connect(sql_file_path)
        sql = f"SELECT * FROM {table_name}"
        if where:
            sql += f" WHERE {where}"
        if order_by:
            sql += f" ORDER BY {order_by}"
        sql += f" LIMIT {count}"
        if DEBUG_MODE:
            print(f'sql={sql}')
        cur = con.cursor()
        res = cur.execute(sql)
        result = res.fetchone()  # Fetch the first row as a tuple

        cur.execute(f"PRAGMA table_info({table_name})")
        column_names = [column[1] for column in cur.fetchall()]

        con.close()

        if DEBUG_MODE:
            print(f'sql result={result}')
        if result:
            # Get column names (important for dictionary creation)
            # con_temp=duckdb.connect(database=':memory:')
            # con_temp.execute(f"SELECT * FROM read_csv_auto('{csv_file_path}', header=true, encoding='UTF-8') LIMIT 0")
            # column_names=[description[0] for description in con_temp.description]
            # con_temp.close()

            # Create a dictionary for easy access by column name
            first_row_dict = dict(zip(column_names, result))
            return first_row_dict
        else:
            my_dict = {}
            for key in column_names:
                my_dict[key] = None
            return my_dict

    except Exception as e:
        print(f"An error occurred: {e}")
        return None


def get_data_by_csv(csv_file_path, where = '', order_by = '', count = 1):
    """
    Retrieves data from a CSV file using DuckDB.

    Args:
        csv_file_path: The path to the CSV file.

    Returns:
        A dictionary representing the first row, or None if the file is empty or an error occurs.
        Alternatively, if using pandas, it returns a pandas Series.
    """

    if not where:
        where = '1=1'

    try:
        # Method 1: Using DuckDB's CSV reading capabilities directly (Recommended)
        con = duckdb.connect(database=':memory:')  # In-memory database
        sql = f"SELECT * FROM read_csv_auto('{csv_file_path}', header=true, encoding='UTF-8')"
        if where:
            sql += f" WHERE {where}"
        if order_by:
            sql += f" ORDER BY {order_by}"
        sql += f" LIMIT {count}"
        #print(sql)
        con.execute(sql) #read_csv_auto automatically detects separators
        result = con.fetchone()  # Fetch the first row as a tuple
        column_names=[description[0] for description in con.description]
        con.close()

        if result:
            # Get column names (important for dictionary creation)
            # con_temp=duckdb.connect(database=':memory:')
            # con_temp.execute(f"SELECT * FROM read_csv_auto('{csv_file_path}', header=true, encoding='UTF-8') LIMIT 0")
            # column_names=[description[0] for description in con_temp.description]
            # con_temp.close()

            # Create a dictionary for easy access by column name
            first_row_dict = dict(zip(column_names, result))
            return first_row_dict
        else:
            my_dict = {}
            for key in column_names:
                my_dict[key] = None
            return my_dict

    except Exception as e:
        print(f"An error occurred: {e}")
        return None

    # Method 2: Using pandas to read the CSV and then DuckDB (if you need pandas anyway)
    # import pandas as pd
    # try:
    #     df = pd.read_csv(csv_file_path, nrows=1) #Read only the first row
    #     con = duckdb.connect(database=':memory:', read_only=True)
    #     con.register("my_table", df) #Register the pandas dataframe as a table
    #     result = con.execute("SELECT * FROM my_table").fetchone()
    #     con.close()

    #     if result:
    #         return pd.Series(result, index=df.columns) #Return a pandas series
    #     else:
    #         return None

    # except Exception as e:
    #     print(f"An error occurred: {e}")
    #     return None


def get_ccode(r, inJgmj, inYdxz):
    #print(r)
    s = f"{r['CNo']} = "
    columns = 'ON   OFF MA  IP  SV  UP  DN  LN  LP  STEP    PL  V   HP  PP  C   S   L   AL  JS  JM  OC  LD'.split()
    G6 = float(get_data("加工面积.csv", f"加工面积='{inJgmj}'")['Y'])
    for c in columns:
        v = r[c]
        if c == 'SV':
            N2 = r['V']
            if N2 == 1:
                v = 69
            elif N2 == 2:
                v = 91
            elif N2 == 3:
                v = 112
            else:
                v = 147
        elif c == 'DN':
            v = int(round(r['DN'] / 20 * G6))
        elif c == 'LP':
            v = get_data("摇动形状.csv", f"摇动形状='{inYdxz}'")['AB']
        elif c == 'S':
            M2 = r['PL']
            v = int(round(50 + (29/3-1/12*G6) if M2=="+" else (29/3-1/12*G6)/2))
        elif c == 'JS':
            AU2 = r['JS']
            print(AU2)
            v = int(round(AU2*20/G6+100 if G6>=30 else AU2*20/G6))
        elif c == 'JM':
            v = 10 if G6 >= 80 else (10 if r['CNo'] == 'C802' else 0)
            v += 1
        s += f"{v} "
    return s

#from PyQt5.QtWidgets import QApplication, QMessageBox

#def show_message_box():
#    msg_box = QMessageBox()
#    msg_box.setIcon(QMessageBox.Information)
#    msg_box.setText("This is a PyQt message box")
#    msg_box.setWindowTitle("Message")
#    msg_box.setStandardButtons(QMessageBox.Ok)
#    msg_box.exec_()


#app = QApplication(sys.argv)
#show_message_box()
if 'window' in locals():
    inClzh = window.getData("inClzh")  # 材料组合
    inDjxz = window.getData("inDjxz")  # 电极形状
    inJgz = window.getData("inJgz")  # 加工轴
    inJgsd = float(window.getData("inJgsd"))   # 加工深度
    inJgmj = window.getData("inJgmj")  # 加工面积
    inCcd = window.getData("inCcd")  # 粗糙度
    inYdxz = window.getData("inYdxz")  # 摇动形状
    inYdms = window.getData("inYdms")  # 摇动模式
    inJgcz = window.getData("inJgcz")  # 加工侧重
    inDbhhw = float(window.getData("inDbhhw"))  # 单边火花位
    DEBUG_MODE = window.getData("DEBUG_MODE") == 'True'
else:
    window = None
    
    inClzh = '铜-钢'
    #inClzh = '石墨-钢'
    inDjxz = '通用'
    inJgz = 'Z'
    inJgsd = -0.500
    inJgmj = '□1○1'  #'□18○20' 
    inCcd = 'VDI0-Ra0.1'
    inYdxz = '圆'
    inYdms = '标准'
    inJgcz = '标准'
    inDbhhw = 0.35  # 0.20

c_lines = []
to_gaps = []
try:
    B12 = inDbhhw
    AB4 = float(get_data("目标余量粗糙度.csv")['余量']) # 0.25
    H2 = get_data("材料形状表.csv", f"材料='{inClzh}' AND 形状='{inDjxz}'")['R']
    # 第一行
    #print(AB4, H2)
    if B12 < AB4:
        c_lines.append('')
        pass
    else:
        index1 = get_data("库.csv", f"CNo LIKE '{H2}%' AND 余量Gap <= '{B12}'", f"余量Gap DESC")
        c_lines.append(get_ccode(index1, inJgmj, inYdxz))

    last_cwd = 10000
    while True:
        xx = get_data("目标余量粗糙度.csv", f"粗糙度 < '{last_cwd}'", '粗糙度 DESC')
        AC4 = xx['粗糙度'] # 10.5
        if AC4 is None:
            break
        index2 = get_data("库.csv", f"CNo LIKE '{H2}%' AND 粗糙度Ra <= '{AC4}'", f"粗糙度Ra DESC")
        last_cwd = AC4
        to_gaps.append(index2['余量Gap'])

        G8 = get_data("粗糙度.csv", f"粗糙度='{inCcd}'")['V']
        #print(B12, G8, index2['余量Gap'], index2['粗糙度Ra'])
        if B12 < index2['余量Gap'] or G8 > index2['粗糙度Ra']:
            c_lines.append('')
            continue
        #print(index2['CNo'])
        c_lines.append(get_ccode(index2, inJgmj, inYdxz))
        #break
    s = '\n'.join(c_lines)
    #print(s)
except Exception as ex:
    print(ex)
    s = ''
    #s = str(ex)
    raise 
#s = 'C011  =  000 40  0000  0070  50  07  10 000 1111 0000  +  01 000  11  0  47  25  10 180  09 0000  2 ;\n'
#s += 'C012  =  001 41  0000  0070  50  07  10 000 1111 0000  +  01 000  11  0  47  25  10 180  09 0000  2 ;\n'

s2 = ''
if c_lines:
    B5 = inJgsd
    s2 += f'H100 =   {B5}\n'
    B12 = inDbhhw
    s2 += f'H101 =   {B12 * 1000}\n'
    B10 = inYdms
    s2 += 'H102 =   {0}\n'.format('000' if B10 == '无' else ('010' if B10 == '自由' else ('020' if B10 == '锁定' else '000')))

    for i in range(len(c_lines)):
        if i == 0:
            if c_lines[i]:
                s2 += 'C000 STEPH101-{0} LNH102 G01 ZH100+{1} M04;\n'.format(0, 0)
            else:
                pass
        else:
            if c_lines[i]:
                s2 += 'C000 STEPH101-{0} LNH102 G01 ZH100+{1} M04;\n'.format(int(round(to_gaps[i-1]/4*3*1000)), to_gaps[i-1])
            else:
                pass

s = s + '\n' + s2
print(s)

if window:
    window.setData("result", s)
