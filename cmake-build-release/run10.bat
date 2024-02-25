@echo off
echo %DATE% %TIME%
setlocal enabledelayedexpansion
set /A no_subpops=1
for /L %%k in (1, 1, 7) do (
    set /A no_subpops=!no_subpops!*2
    echo Running with !no_subpops! subpopulations
    set /A sequences=1
    for /L %%j in (1, 1, 10) do (
        set /A sequences=!sequences!*2
        echo Running with !sequences! sequences
        for /L %%i in (1, 1, 10) do (
            echo Running iteration %%i
            classic_GA.exe 1024 !no_subpops! 1000 -5 10 9 2048 !sequences!
        )
        python merge.py ./
        set /A sequence_length=2048/!sequences!
        set dir=!no_subpops!_!sequence_length!
        mkdir !dir!
        echo Moving files to !dir!
        move *.csv !dir!
    )
)
endlocal
echo %DATE% %TIME%
