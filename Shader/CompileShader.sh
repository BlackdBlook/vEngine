@echo off

cd /d %~dp0

python Script/ShaderIc.py . ./compiler/hlslc ./compiler/glslc
