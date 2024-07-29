@echo off

del .\file_modification_times.json

python Script/ShaderIc.py . ./hlslc.exe ./glslc.exe
