@echo off
for /L %%i in (0, 1, 9) do (
    echo Running iteration %%i
    classic_GA.exe 1024 1000 -5 10 10 1024 >> run%%i.txt
)