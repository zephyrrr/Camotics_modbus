# -*- coding: utf-8 -*-
import os
from datetime import datetime
import sys
import sqlite3
#import pandas as pd  # Optional, but often useful for data manipulation
#from collections import OrderedDict

DEBUG_MODE = True

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
        raise Exception('')
        return get_data_by_csv(csv_file_path, where, order_by, count)



def get_data_by_sqlite(table_name, where = '', order_by = '', count = 1):
    if not where:
        where = '1=1'

    all_rows = []
    try:
        sql_file_path = f"{SQLITE_DB_PATH}{SQLITE_DB_NAME}"
        if not os.path.exists(sql_file_path):
            sql_file_path = f"{SQLITE_DB_NAME}"
        if not os.path.exists(sql_file_path):
            print(f'{sql_file_path} not exist.')
            return None
        if DEBUG_MODE:
            print(f'sqlite file={sql_file_path}')
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
        while True:
            row = res.fetchone()  # Fetch the first row as a tuple
            if row is None:
                break
            all_rows.append(row)

        cur.execute(f"PRAGMA table_info({table_name})")
        column_names = [column[1] for column in cur.fetchall()]

        con.close()

        if DEBUG_MODE:
            print(f'sql result={all_rows}')
        if len(all_rows) > 0:
            # Get column names (important for dictionary creation)
            # con_temp=duckdb.connect(database=':memory:')
            # con_temp.execute(f"SELECT * FROM read_csv_auto('{csv_file_path}', header=true, encoding='UTF-8') LIMIT 0")
            # column_names=[description[0] for description in con_temp.description]
            # con_temp.close()

            # Create a dictionary for easy access by column name
            all_results = []
            for row in all_rows:
                row_dict = dict(zip(column_names, row))
                all_results.append(row_dict)
            return all_results
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


def get_ccode(r, other_cs):
    #print(r)
    s = f"C{int(r['C NO.']):03d} = "
    columns = 'ON   OFF MA  IP  SV  UP  DN  LN  LP  STEP    PL  V   HP  PP  C   S   L   AL  JS  JM  OC  LD'.split()
    #G6 = float(get_data("加工面积.csv", f"加工面积='{inJgmj}'")['Y'])
    for c in columns:
        if c in r:
            v = r[c]
        elif c in other_cs:
            v = other_cs[c]
        else:
            v = ''
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

def read_all_from_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        return file.read()


if __name__ == "__main__":
    #allClzh = read_all_from_file(os.path.join(SQLITE_DB_PATH, 'inClzh_data.json')).split('\n')
    #allDjxz = read_all_from_file(os.path.join(SQLITE_DB_PATH, 'inDjxz_data.json')).split('\n')
    #allJgcz = read_all_from_file(os.path.join(SQLITE_DB_PATH, 'inJgcz_data.json')).split('\n')


    if 'window' in locals() and window:
        inClzh = window.getData("inClzh")  # 材料组合
        inDjxz = window.getData("inDjxz")  # 电极形状
        inJgz = window.getData("inJgz")  # 加工轴
        inJgsd = float(window.getData("inJgsd"))   # 加工深度
        inJgmj = window.getData("inJgmj")  # 加工面积
        inCcd = window.getData("inCcd")  # 粗糙度
        inYdxz = window.getData("inYdxz")  # 摇动形状
        #inYdxzSpecial = window.getData("inYdxzSpecial")  # 摇动形状(分象限)
        inYdms = window.getData("inYdms")  # 摇动模式
        inJgcz = window.getData("inJgcz")  # 加工侧重
        inDbhhw = float(window.getData("inDbhhw"))  # 单边火花位
        #DEBUG_MODE = window.getData("DEBUG_MODE") == 'True'

        if DEBUG_MODE:
            print(inClzh, inDjxz, inJgz, inJgsd, inJgmj)
            print(inCcd, inYdxz, inYdms, inJgcz, inDbhhw)
    else:
        window = None
    
        inClzh = 1  #'铜-钢'
        inDjxz = 3  #'通用'
        inJgz = 'Z' # todo
        inJgsd = -3.200
        inJgmj = 68  #'□18○20' 
        inCcd = 0.5  #'VDI16-Ra0.63'
        inYdxz = '0000'  #'圆'
        inYdms = 0  #'思考'
        inJgcz = 1  #'标准'
        inDbhhw = 0.150  # 0.20
        DEBUG_MODE = True

        # 材料，，      形状，侧重，1（切入），单边
        # 材料、面积、形状、侧重、Ra（粗糙度）、切入=0
    ret_lines = []
    to_cmyls = []   # 侧面余量
    to_dbyls = []   # 底部余量
    to_cs = []
    to_rows = []

    try:
        other_cs = {}
        other_cs['LP'] = inYdxz
        #other_cs['LP'] = get_data("摇动形状.csv", f"摇动形状='{inYdxz}'")[0]['AB']
        #if str(other_cs['LP']) == '6666':
        #    other_cs['LP'] = inYdxzSpecial
        other_cs['LN'] = '000'
        other_cs['STEP'] = '0.000'

        #inClzhNo = allClzh.index(inClzh) + 1
        #inDjxzNo = allDjxz.index(inDjxz) + 1
        #inJgczNo = allJgcz.index(inJgcz) + 1
        r1 = get_data("库.csv", f"材料='{inClzh}' AND 形状='{inDjxz}' AND 重视='{inJgcz}' AND 切入=1 AND 侧面余量<={inDbhhw} ORDER BY 侧面余量 DESC", f"")
        if type(r1) is list:
            if len(r1) > 1:
                print('!!!duplicate result: ', r1)
            elif len(r1) == 0:
                raise Exception('No data found in 库.csv for the given parameters with 切入=1')
        ret_lines.append(get_ccode(r1[0], other_cs))
        to_cs.append(int(r1[0]['C NO.']))
        to_cmyls.append(float(r1[0]['侧面余量']))
        to_dbyls.append(float(r1[0]['底部余量']))
        to_rows.append(None)

        #inJgmjNo = get_data("加工面积.csv", f"加工面积='{inJgmj}'")[0]['Y']
        #inCcdNo = get_data("粗糙度.csv", f"粗糙度='{inCcd}'")[0]['V']
        r2 = get_data("库.csv", f"材料='{inClzh}' AND 面积='{inJgmj}' AND 形状='{inDjxz}' AND 重视='{inJgcz}' AND 切入=0 AND Ra='{inCcd}'", f"", 100)  # 行 ASC
        #print(f'r2 len={len(r2)}')

        for i in range(len(r2)):
            #print(r2[i]['侧面余量'])
            if float(r2[i]['侧面余量']) > inDbhhw:
                continue
            #print(get_ccode(r2[i], other_cs))
            ret_lines.append(get_ccode(r2[i], other_cs))
            to_cs.append(int(r2[i]['C NO.']))
            to_cmyls.append(float(r2[i]['侧面余量']))
            to_dbyls.append(float(r2[i]['底部余量']))
            to_rows.append(r2[i])

        #s = '\n'.join(c_lines)
        #print(s)
    except Exception as ex:
        print(ex)
        #s = ''
        #s = str(ex)
        raise 
    #s = 'C011  =  000 40  0000  0070  50  07  10 000 1111 0000  +  01 000  11  0  47  25  10 180  09 0000  2 ;\n'
    #s += 'C012  =  001 41  0000  0070  50  07  10 000 1111 0000  +  01 000  11  0  47  25  10 180  09 0000  2 ;\n'

    if len(ret_lines) > 0:
        B5 = inJgsd
        ret_lines.append(f'H100 =   {B5}')
        B12 = inDbhhw
        ret_lines.append(f'H101 =   {B12}')
        B10 = inYdms
        #ret_lines.append('H102 =   {0}'.format('000' if B10 == '无' else ('010' if B10 == '自由' else ('020' if B10 == '锁定' else '300'))))
        ret_lines.append('H102 =   {0}'.format('000' if B10 == 0 else ('010' if B10 == 1 else ('020' if B10 == 2 else '300'))))

        for i in range(len(to_cs)):
            if to_rows[i] and to_rows[i]['定时'] and to_rows[i]['定时时间']:
                n = int(to_rows[i]['定时时间'])
                if n > 0:
                    if to_rows[i]['定时'] == 'Z':
                        ret_lines.append(f"G85Z{n};")
                    elif to_rows[i]['定时'] == 'T':
                        ret_lines.append(f"G85T{n};")
            ret_lines.append(f'C{to_cs[i]:03d} STEPH101-{to_cmyls[i]:.4f} LNH102 G01 ZH100{to_dbyls[i]:+.4f} M04;')
            # STEP 是经过计算的，表格里显示的和excel里的不一致

    s = '\n'.join(ret_lines)
    if DEBUG_MODE:
        print(s)

    if window:
        window.setData("result", s)
