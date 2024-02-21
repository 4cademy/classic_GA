@echo off
for /L %%i in (0, 1, 9) do (
    echo Running iteration %%i
    classic_GA.exe 1024 1000 -32.768 32.768 0 1024 64
)
python merge.py ./