
cd .

if "%1"=="" ("D:\MAT-LAB\bin\win64\gmake"  DEPRULES=1 -j5  -f BLINK.mk all) else ("D:\MAT-LAB\bin\win64\gmake"  DEPRULES=1 -j5  -f BLINK.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1