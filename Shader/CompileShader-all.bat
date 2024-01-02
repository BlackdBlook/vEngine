@echo off

echo %cd%
cd .\Shader
if not exist ShaderCache mkdir ShaderCache


for /R %%i in (*.frag, *.vert) do (
    echo Compiling: %%i
    glslc.exe %%i -o "ShaderCache/%%~ni%%~xi.spv"
)

echo Compilation complete.
