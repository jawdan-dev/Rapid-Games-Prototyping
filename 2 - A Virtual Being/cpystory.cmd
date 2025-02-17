@echo off

:loop

xcopy source\virtualGame\assets\story.txt binaries\desktop\assets\story.txt /D /Y

timeout /t 1

goto loop