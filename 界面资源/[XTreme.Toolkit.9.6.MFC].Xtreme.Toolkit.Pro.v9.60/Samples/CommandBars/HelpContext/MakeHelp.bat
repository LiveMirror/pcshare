@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by HELPCONTEXT.HPJ. >"hlp\HelpContext.hm"
echo. >>"hlp\HelpContext.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\HelpContext.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\HelpContext.hm"
echo. >>"hlp\HelpContext.hm"
echo // Prompts (IDP_*) >>"hlp\HelpContext.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\HelpContext.hm"
echo. >>"hlp\HelpContext.hm"
echo // Resources (IDR_*) >>"hlp\HelpContext.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\HelpContext.hm"
echo. >>"hlp\HelpContext.hm"
echo // Dialogs (IDD_*) >>"hlp\HelpContext.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\HelpContext.hm"
echo. >>"hlp\HelpContext.hm"
echo // Frame Controls (IDW_*) >>"hlp\HelpContext.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\HelpContext.hm"
REM -- Make help for Project HELPCONTEXT


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\HelpContext.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\HelpContext.hlp" goto :Error
if not exist "hlp\HelpContext.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\HelpContext.hlp" Debug
if exist Debug\nul copy "hlp\HelpContext.cnt" Debug
if exist Release\nul copy "hlp\HelpContext.hlp" Release
if exist Release\nul copy "hlp\HelpContext.cnt" Release
echo.
goto :done

:Error
echo hlp\HelpContext.hpj(1) : error: Problem encountered creating help file

:done
echo.
