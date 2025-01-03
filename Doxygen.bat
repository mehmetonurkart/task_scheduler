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

SET SVNPATH=C:\Program Files\TortoiseSVN\bin
SET PROJECTPATH=%~dp0
SET DOXYGENPATH=%~dp0..\External_Library\doxygen\bin
SET OUTPUTPATH=..\Documentation\Doxygen
SET SVN=..\External_Library\binSVN

FOR /F "tokens=* USEBACKQ" %%g IN (`%SVN%\svn info --show-item=revision`) do (SET "VERSION=%%g")

SET /P CHOICE=Choose the do you will do : 
if not '%CHOICE%'=='' SET choice=%CHOICE:~0,1%
if '%CHOICE%'=='1' GOTO DOXYGEN_OUTPUT
if '%CHOICE%'=='2' GOTO DOXYGEN_SETTINGS
ECHO "%CHOICE%" is not valid, try again
ECHO.
GOTO START

:DOXYGEN_OUTPUT
ECHO.
ECHO -----------------------------------------------------
ECHO.
ECHO 1. SVN_UPDATE
ECHO 2. LOCAL_CODE
ECHO.
SET /P CHOICE=Do you want SVN update ? : 
if not '%CHOICE%'=='' SET choice=%CHOICE:~0,1%
if '%CHOICE%'=='1' GOTO SVN_UPDATE
if '%CHOICE%'=='2' GOTO LOCAL_CODE
ECHO "%CHOICE%" is not valid, try again
ECHO.
GOTO END

:SVN_UPDATE
"%SVNPATH%\TortoiseProc.exe" /command:update /path:"%~dp0" /closeonend:2
cd %PROJECTPATH%
%DOXYGENPATH%\doxygen.exe Doxyfile
cd %OUTPUTPATH%
rename "ECT_BCM_A_SAMPLE" "ECT_BCM_A_SAMPLE_Revision_%VERSION%"
GOTO END

:LOCAL_CODE
cd %PROJECTPATH%
%DOXYGENPATH%\doxygen.exe Doxyfile
cd %OUTPUTPATH%
rename "ECT_BCM_A_SAMPLE" "ECT_BCM_A_SAMPLE_Revision_%VERSION%"
GOTO END

:DOXYGEN_SETTINGS
cd %PROJECTPATH%
%DOXYGENPATH%\doxywizard.exe Doxyfile
GOTO END



:END
EXIT 0
ENDLOCAL

