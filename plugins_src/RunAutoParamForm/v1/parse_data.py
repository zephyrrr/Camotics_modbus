import struct

def analyze_mixed_format_data(data_file, sample_format_sequences, sample_value_sequences):
    """
    分析包含混合格式数据的二进制文件。

    参数：
        data_file (str): 数据文件的路径。
        sample_format_sequences (list): 已知格式序列的列表（例如，["<i", "<f", "<h"]）。
        sample_value_sequences (list): 已知数据值序列的列表，与格式序列对应。
    """
    with open(data_file, "rb") as f:
        data = f.read()
    #print(data[0:10])
    print('data size: ', len(data))
    results = []

    n = 600
    for format_sequence, value_sequence in zip([sample_format_sequences], [sample_value_sequences]):
        byte_sizes = [struct.calcsize(fmt) for fmt in format_sequence]
        total_byte_size = sum(byte_sizes)
 
        try:
            extracted_values = []
            for i in range(0, len(data), n):  # 1392
                chunk = data[i:i + total_byte_size]
                if len(chunk) != total_byte_size:
                    break
                current_index = 0
                temp_values = []
                for fmt, byte_size in zip(format_sequence, byte_sizes):
                    sub_chunk = chunk[current_index:current_index + byte_size]
                    
                    temp_values.append(struct.unpack(fmt, sub_chunk)[0])
                    current_index += byte_size

                extracted_values.append(temp_values)
                #print(f'first record: {temp_values}')
                #break
            #print(len(extracted_values))

            # 检查提取出的数据是否包含指定的序列
            for i in range(len(extracted_values)):
                found = True
                
                try:
                    # index = extracted_values[i][0:].index(value_sequence[0])
                    # if index == -1:
                    #     found = False
                    # else:
                    index = 0
                    for j in range(len(value_sequence)):
                        if abs(extracted_values[i][index + j] - value_sequence[j]) > 0.00001:
                            found = False
                            break
                        #print(extracted_values[i], value_sequence)
                except ValueError:
                    found = False
                    break

                #start_index = index + len(value_sequence) + 1

                if found:
                    results.append((format_sequence, extracted_values[i], i))

        except struct.error:
            pass

    if results:
        for format_sequence, ev, start_index in results:
            print(f"可能的数据格式序列：{format_sequence}")
            print("提取出的数据：", ev)
            print(start_index)
            print(data[start_index*n:start_index*n+n])
            print(data[start_index*n+total_byte_size:start_index*n+total_byte_size+50])
            print("-" * 20)
    else:
        print("无法确定数据格式序列。")

# # 示例用法：
# # 1. 创建一个示例数据文件（替换为你的文件）
# with open("my_struct_data_ordered.bin", "wb") as f:
#     # 示例：写入小端序4字节整数，按顺序排列
#     data_to_write = [10, 255, -5, 123456, 10, 255, -5]
#     for value in data_to_write:
#         f.write(struct.pack("<i", value)) #小端序整数

# 2. 提供数据例子（替换为你的数据），按顺序排列
sample_value_sequences = [600, 110, 0, 15.0, 60, 10]
sample_format_sequences = ["<i", "<i", "<i", "<d", "<i", "<i"]

# "<i", "<I", "<h", "<H", "<b", "<B", "<f", "<d"
sample_value_sequences = [9, 120, 40, 0, 4.0 * 10, 50, 10, 32, 3, 1, 42, 0, 40, 11, 0, ord('+'), 11, 8, 2, 2, \
    0.1440, 0.5000, 24, ord('T')]
sample_format_sequences = ["<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<h", "<b", "<h", "<h", "<h", "<h", \
    "<f", "<f", "<b", "<b"]
sample_value_sequences = [3, 170, 55, 0, 3.0 * 10, 70, 10, 20, 5, 1, 42, 0, 40, 10, 0, ord('+'), 11, 15, 2, 0, ]  #  
length = min(len(sample_format_sequences), len(sample_value_sequences))
#  0.0860, 0.5000, 24
# 1392

# 3. 分析数据文件
#analyze_mixed_format_data("autoEx.ap", sample_format_sequences, sample_value_sequences)
analyze_mixed_format_data("auto.ap", sample_format_sequences[:length], sample_value_sequences[:length])