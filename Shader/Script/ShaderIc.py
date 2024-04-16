import os
import time
import json
import sys

# 文件修改时间记录文件
record_file = 'file_modification_times.json'

shader_file_type={'.frag','.vert','.hlsl'}


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
from hlsl_utils import *

def compile_shaders(path, shader_files):
    if not os.path.exists('ShaderCache'):
        os.mkdir('ShaderCache')
    for shader_file in shader_files:
        # 获取文件名和扩展名
        file_name = os.path.basename(shader_file)
        name, ext = os.path.splitext(file_name)
        if str.find(ext, 'hlsl') != -1:
            #hlsl代码
            base_name, shader_types = get_shader_name_and_types(name)
            if len(shader_types) == 0:
                print(file_name, 'unknow hlsl shader type')
                continue
            
            hlslc_path = os.path.join(sys.argv[1], 'hlslc.exe')
            print(hlslc_path)

            # 调用dxc.exe编译shader
            # dxc.exe -spirv -T vs_6_1 -E main .\input.vert -Fo .\output.vert.spv -fspv-extension=SPV_EXT_descriptor_indexin
            for shader_type in shader_types:
                output_file_name = f'{base_name}.{hlsl_entry_Point[shader_type]}.spv'.lower()
                output_file_path = os.path.join(path, 'ShaderCache', output_file_name)
                print(output_file_path)
                ret = subprocess.run([hlslc_path, '-spirv', '-T', hlsl_shader_config[shader_type], '-E', hlsl_entry_Point[shader_type], shader_file, '-Fo', output_file_path])
                print(ret)


        else:
            #glsl代码
            # 设置输出文件名
            output_file = os.path.join('ShaderCache', name + ext + '.spv').lower()

            # 调用glslc.exe编译shader
            subprocess.run(['./glslc.exe', shader_file, '-o', output_file])


def main(path):
    old_times = load_modification_times()
    new_times = get_modification_times(path)
    modified = print_modified_files(old_times, new_times)
    compile_shaders(path, modified)
    record_modification_times(new_times)

if __name__ == '__main__':
    main(sys.argv[1])
