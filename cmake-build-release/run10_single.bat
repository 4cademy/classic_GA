@echo off
for /L %%i in (1, 1, 10) do (
    echo Running iteration %%i
    classic_GA.exe 1024 1 1000 -5 10 10 2048 1
)
python merge.py ./

