import os
import sys

glsl_shader_type_suffix=dict()

glsl_shader_type_suffix['vs']='-DVERT'
glsl_shader_type_suffix['fs']='-DFRAG'
glsl_shader_type_suffix['gs']='-DGERO'
glsl_shader_type_suffix['cs']='-DCOMP'

glsl_shader_output_suffix=dict()

glsl_shader_output_suffix['vs']='.vert.spv'
glsl_shader_output_suffix['fs']='.frag.spv'
glsl_shader_output_suffix['gs']='.gero.spv'
glsl_shader_output_suffix['cs']='.comp.spv'

glsl_fshader_stage=dict()

glsl_fshader_stage['vs']='-fshader-stage=vert'
glsl_fshader_stage['fs']='-fshader-stage=frag'
glsl_fshader_stage['gs']='-fshader-stage=gero'
glsl_fshader_stage['cs']='-fshader-stage=comp'

def combine_output_path(file_name : str, shader_output_suffix : str)->str:
    shader_output_suffix = os.path.join('ShaderCache', file_name + shader_output_suffix)
    # print(shader_output_suffix)
    return shader_output_suffix

def gen_compile_command(file_path : str)->list:
    compile_commands = []
    

    file_name = os.path.basename(file_path)
    name, ext = os.path.splitext(file_name)
    
    if ext !='.glsl' or '_' not in name:
        # 对传统glsl写法做兼容
        shader_output = os.path.join('ShaderCache', file_name.lower() + '.spv')
        shader_output = os.path.abspath(shader_output)
        print(shader_output)
        return [['glslc.exe', file_path, '-o', shader_output]]
    
    suffixes = name.split('_')

    for suffix in suffixes[1:]:
        if suffix in glsl_shader_type_suffix:
            command = gen_signel_compile_command(file_path, suffixes[0], suffix)
            compile_commands.append(command)
        else:
            print("unknow suffix", file_path)

    return compile_commands

def gen_signel_compile_command(file_path, file_name, suffix)->list:
    compile_command = []
    compile_command.append('glslc.exe')

    compile_command.append(glsl_fshader_stage[suffix])

    compile_command.append(file_path)

    compile_command.append(glsl_shader_type_suffix[suffix])


    compile_command.append('-o')

    output_path = combine_output_path(file_name, glsl_shader_output_suffix[suffix])

    compile_command.append(output_path)

    return compile_command
    

if __name__ == "__main__":
    commands = gen_compile_command('../DrawCube/DrawCube_vs.glsl')
    for cmd in commands:
        print(cmd)

    commands = gen_compile_command('../DrawCube/DrawCube.vert')
    for cmd in commands:
        print(cmd)

    commands = gen_compile_command('../DrawCube/DrawCube.glsl')
    for cmd in commands:
        print(cmd)