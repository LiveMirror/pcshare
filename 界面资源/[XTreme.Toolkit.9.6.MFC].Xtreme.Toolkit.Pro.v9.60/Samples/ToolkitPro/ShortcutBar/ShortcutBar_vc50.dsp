# Microsoft Developer Studio Project File - Name="ShortcutBar" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ShortcutBar - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ShortcutBar.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ShortcutBar.mak" CFG="ShortcutBar - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ShortcutBar - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ShortcutBar - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ShortcutBar - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "ShortcutBar - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ShortcutBar - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release/vc50"
# PROP BASE Intermediate_Dir "Release/vc50"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release/vc50"
# PROP Intermediate_Dir "Release/vc50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/vc50/ShortcutBar.exe" /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "ShortcutBar - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug/vc50"
# PROP BASE Intermediate_Dir "Debug/vc50"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug/vc50"
# PROP Intermediate_Dir "Debug/vc50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../../bin/vc50/ShortcutBarD.exe" /pdbtype:sept /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "ShortcutBar - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ShortcutBar___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "ShortcutBar___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release/vc50"
# PROP Intermediate_Dir "Unicode_Release/vc50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/vc50/ShortcutBar.exe" /libpath:"../../../lib/vc50"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../bin/vc50/ShortcutBarU.exe" /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "ShortcutBar - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ShortcutBar___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ShortcutBar___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug/vc50"
# PROP Intermediate_Dir "Unicode_Debug/vc50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../../bin/vc50/ShortcutBarD.exe" /pdbtype:sept /libpath:"../../../lib/vc50"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../bin/vc50/ShortcutBarUD.exe" /pdbtype:sept /libpath:"../../../lib/vc50"

!ENDIF 

# Begin Target

# Name "ShortcutBar - Win32 Release"
# Name "ShortcutBar - Win32 Debug"
# Name "ShortcutBar - Win32 Unicode Release"
# Name "ShortcutBar - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutBar.rc
# End Source File
# Begin Source File

SOURCE=.\ShortcutBarDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutBarView.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneCalendar.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneContacts.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneFolders.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneMail.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutBar.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutBarDoc.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutBarView.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneCalendar.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneContacts.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneFolders.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneMail.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CjSample.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon10.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon11.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon12.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon13.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon14.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon16.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon17.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon9.ico
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBar.ico
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBar.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBarDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Shortcuts.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shortcuts_big_alpha.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shortcuts_small_alpha.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
