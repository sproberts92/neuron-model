rem @echo off
CALL .\visualisation\activity\venv\scripts\activate.bat
echo Starting processing...
CALL py .\visualisation\activity\vis_activity.py
pause