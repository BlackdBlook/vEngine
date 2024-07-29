@echo off

rm ./file_modification_times.json

python Script/ShaderIc.py . ./compiler/hlslc ./compiler/glslc
