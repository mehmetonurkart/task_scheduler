::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Name	    :	Doxygen.bat																		    ::
:: Purpose  :	Create project's doxygen documentation												::
:: Author   :	mehmtonrr																			::
:: Date	    :	13 Haz 2023																			::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@ECHO off
SETLOCAL
:START

ECHO *****************************************************
ECHO    			DOXYGEN
ECHO *****************************************************
ECHO.
ECHO 1. DOXYGEN_OUTPUT
ECHO 2. DOXYGEN_SETTINGS
ECHO.

set YEAR=%date:~10,4%
set MONTH=%date:~7,2%
set DAY=%date:~4,2%

SET PROJECTPATH=.
SET DOXYGENPATH=.\..\..\External_Library\doxygen_setup\bin
SET OUTPUTPATH=.\Documentation\Doxygen

SET /P CHOICE=Choose the do you will do : 
if not '%CHOICE%'=='' SET choice=%CHOICE:~0,1%
if '%CHOICE%'=='1' GOTO DOXYGEN_OUTPUT
if '%CHOICE%'=='2' GOTO DOXYGEN_SETTINGS
ECHO "%CHOICE%" is not valid, try again
ECHO.
GOTO START

:DOXYGEN_OUTPUT
cd %PROJECTPATH%
%DOXYGENPATH%\doxygen.exe Doxyfile
cd %OUTPUTPATH%
GOTO END

:DOXYGEN_SETTINGS
cd %PROJECTPATH%
%DOXYGENPATH%\doxywizard.exe Doxyfile
GOTO END

:END
EXIT 0
ENDLOCAL

