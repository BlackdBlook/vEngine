@echo off

del .\file_modification_times.json

cd .\Shader

python Script/ShaderIc.py %cd%
