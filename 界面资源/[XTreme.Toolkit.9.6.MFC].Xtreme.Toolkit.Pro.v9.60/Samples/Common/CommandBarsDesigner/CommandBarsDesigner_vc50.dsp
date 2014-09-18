# Microsoft Developer Studio Project File - Name="CommandBarsDesigner" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CommandBarsDesigner - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CommandBarsDesigner.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CommandBarsDesigner.mak" CFG="CommandBarsDesigner - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CommandBarsDesigner - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CommandBarsDesigner - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CommandBarsDesigner - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "CommandBarsDesigner - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "CommandBarsDesigner - Win32 Static Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CommandBarsDesigner - Win32 Release"

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
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/vc50/CommandBarsDesigner.exe" /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "CommandBarsDesigner - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../../bin/vc50/CommandBarsDesignerD.exe" /pdbtype:sept /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "CommandBarsDesigner - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CommandBarsDesigner___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "CommandBarsDesigner___Win32_Unicode_Release"
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
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/vc50/CommandBarsDesigner.exe" /libpath:"../../../lib/vc50"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../bin/vc50/CommandBarsDesignerU.exe" /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "CommandBarsDesigner - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CommandBarsDesigner___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CommandBarsDesigner___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug/vc50"
# PROP Intermediate_Dir "Unicode_Debug/vc50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../../bin/vc50/CommandBarsDesignerD.exe" /pdbtype:sept /libpath:"../../../lib/vc50"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../bin/vc50/CommandBarsDesignerUD.exe" /pdbtype:sept /libpath:"../../../lib/vc50"

!ELSEIF  "$(CFG)" == "CommandBarsDesigner - Win32 Static Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CommandBarsDesigner___Win32_Static_Release"
# PROP BASE Intermediate_Dir "CommandBarsDesigner___Win32_Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../include" /I "../../AboutDlg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_XTP_STATICLINK" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../include" /i "../../AboutDlg" /d "NDEBUG" /d "_AFXDLL" /d "_XTP_STATICLINK"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/vc50/CommandBarsDesigner.exe" /libpath:"../../../lib/vc50"
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/vc50/CommandBarsDesigner.exe" /libpath:"../../../lib/vc50"

!ENDIF 

# Begin Target

# Name "CommandBarsDesigner - Win32 Release"
# Name "CommandBarsDesigner - Win32 Debug"
# Name "CommandBarsDesigner - Win32 Unicode Release"
# Name "CommandBarsDesigner - Win32 Unicode Debug"
# Name "CommandBarsDesigner - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesigner.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesigner.rc
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerDesignerView.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerView.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogAccelProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBitmapExport.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBitmapImages.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBitmapImport.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogListEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogNewControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogNewIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogNewSymbol.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogNewToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogPropertyPages.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogResourceSymbols.cpp
# End Source File
# Begin Source File

SOURCE=.\EmbeddedFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\EmbeddedView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PaneAccelerators.cpp
# End Source File
# Begin Source File

SOURCE=.\PaneControls.cpp
# End Source File
# Begin Source File

SOURCE=.\PaneIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\PaneProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\PaneToolbars.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyItemFlags.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesigner.h
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerDesignerView.h
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerDoc.h
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerEditView.h
# End Source File
# Begin Source File

SOURCE=.\CommandBarsDesignerView.h
# End Source File
# Begin Source File

SOURCE=.\DialogAccelProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogBitmapExport.h
# End Source File
# Begin Source File

SOURCE=.\DialogBitmapImages.h
# End Source File
# Begin Source File

SOURCE=.\DialogBitmapImport.h
# End Source File
# Begin Source File

SOURCE=.\DialogListEditor.h
# End Source File
# Begin Source File

SOURCE=.\DialogNewControl.h
# End Source File
# Begin Source File

SOURCE=.\DialogNewIcon.h
# End Source File
# Begin Source File

SOURCE=.\DialogNewSymbol.h
# End Source File
# Begin Source File

SOURCE=.\DialogNewToolbar.h
# End Source File
# Begin Source File

SOURCE=.\DialogPropertyPages.h
# End Source File
# Begin Source File

SOURCE=.\DialogResourceSymbols.h
# End Source File
# Begin Source File

SOURCE=.\EmbeddedFrame.h
# End Source File
# Begin Source File

SOURCE=.\EmbeddedView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PaneAccelerators.h
# End Source File
# Begin Source File

SOURCE=.\PaneControls.h
# End Source File
# Begin Source File

SOURCE=.\PaneIcons.h
# End Source File
# Begin Source File

SOURCE=.\PaneProperties.h
# End Source File
# Begin Source File

SOURCE=.\PaneToolbars.h
# End Source File
# Begin Source File

SOURCE=.\PropertyItemFlags.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmapNoIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CjSample.ico
# End Source File
# Begin Source File

SOURCE=.\res\CommandBarsDesigner.ico
# End Source File
# Begin Source File

SOURCE=.\res\CommandBarsDesigner.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CommandBarsDesignerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Embedded.bmp
# End Source File
# Begin Source File

SOURCE=.\res\form.ico
# End Source File
# Begin Source File

SOURCE=.\res\IconEmpty.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconNoIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\pane_too.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneAccelerators.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneControls.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneImages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneProperties.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneToobarsTreeIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneToolbars.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PropertyIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
