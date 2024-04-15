from enum import Enum
class ShaderType(Enum):
    Vertex = 1
    Fragment = 2
    Compute = 3
    Geometry = 4


hlsl_shader_config=dict()
hlsl_shader_config[ShaderType.Vertex] = 'vs_6_0'
hlsl_shader_config[ShaderType.Fragment] = 'ps_6_0'
hlsl_shader_config[ShaderType.Compute] = 'cs_6_0'
hlsl_shader_config[ShaderType.Geometry] = 'gs_6_0'

hlsl_entry_Point=dict()
hlsl_entry_Point[ShaderType.Vertex]='VS'
hlsl_entry_Point[ShaderType.Fragment]='PS'
hlsl_entry_Point[ShaderType.Compute]='CS'
hlsl_entry_Point[ShaderType.Geometry]='GS'

hlsl_shader_suffix=dict()
hlsl_shader_suffix['vs']=ShaderType.Vertex
hlsl_shader_suffix['ps']=ShaderType.Fragment
hlsl_shader_suffix['cs']=ShaderType.Compute
hlsl_shader_suffix['gs']=ShaderType.Geometry


def get_shader_name_and_types(file_name : str) :
    shader_types = []

    suffixes = file_name.split('_')

    for suffix in suffixes[1:]:
        if suffix in hlsl_shader_suffix:
            shader_types.append(hlsl_shader_suffix[suffix])
        else:
            print('未解析到的shader后缀 ', suffix)

    return suffixes[0], shader_types
