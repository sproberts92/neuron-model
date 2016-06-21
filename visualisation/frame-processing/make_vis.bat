@echo off
CALL .\visualisation\frame-processing\venv\scripts\activate.bat
echo Starting processing...
CALL py .\visualisation\frame-processing\frame-processing.py
