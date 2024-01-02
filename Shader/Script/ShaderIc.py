import os
import time
import json
import sys

# 文件修改时间记录文件
record_file = 'file_modification_times.json'

shader_file_type={'.frag','.vert'}

def is_shader_source_file(file_name : str):
    # 获取文件名和扩展名
    base_name = os.path.basename(file_name)
    name, ext = os.path.splitext(base_name)
    return shader_file_type.__contains__(ext)

def get_modification_times(path):
    modification_times = {}
    for filename in os.listdir(path):
        file_path = os.path.join(path, filename)
        if os.path.isdir(file_path):
            modification_times.update(get_modification_times(file_path))
        if os.path.isfile(file_path):
            if is_shader_source_file(file_path):
                if modification_times.__contains__(file_path):
                    print("Found duplicate file names")
                modification_times[file_path] = os.path.getmtime(file_path)
    return modification_times

def record_modification_times(modification_times):
    with open(record_file, 'w', encoding='utf-8') as f:
        json.dump(modification_times, f, indent=4, ensure_ascii=False)

def load_modification_times():
    try:
        with open(record_file, 'r', encoding='utf-8') as f:
            return json.load(f)
    except FileNotFoundError:
        return {}

def print_modified_files(old_times, new_times):
    modified = []
    for filename, new_time in new_times.items():
        old_time = old_times.get(filename)
        if old_time is None or old_time < new_time:
            print(f'文件 "{filename}" 已被修改')
            modified.append(filename)
    return modified

import os
import subprocess

def compile_shaders(shader_files):
    if not os.path.exists('ShaderCache'):
        os.mkdir('ShaderCache')
    for shader_file in shader_files:
        # 获取文件名和扩展名
        base_name = os.path.basename(shader_file)
        name, ext = os.path.splitext(base_name)

        # 设置输出文件名
        output_file = os.path.join('ShaderCache', name + ext + '.spv')

        # 调用glslc.exe编译shader
        subprocess.run(['./glslc.exe', shader_file, '-o', output_file])


def main(path):
    old_times = load_modification_times()
    new_times = get_modification_times(path)
    modified = print_modified_files(old_times, new_times)
    compile_shaders(modified)
    record_modification_times(new_times)

if __name__ == '__main__':
    main(sys.argv[1])
