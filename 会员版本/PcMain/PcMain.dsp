# Microsoft Developer Studio Project File - Name="PcMain" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PcMain - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PcMain.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PcMain.mak" CFG="PcMain - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PcMain - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PcMain - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PcMain - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCMAIN_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCMAIN_EXPORTS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib ole32.lib Ws2_32.lib Shlwapi.lib psapi.lib gdiplus.lib delayimp.lib Mswsock.lib Imm32.lib vfw32.lib winmm.lib Shlwapi.lib strmbasd.lib wininet.lib Wtsapi32.lib Userenv.lib /nologo /dll /machine:I386 /out:"../bin/update/PcMain.dll" /OPT:NOWIN98 /DELAYLOAD:gdiplus.dll
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PcMain - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCMAIN_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCMAIN_EXPORTS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib gdi32.lib user32.lib advapi32.lib shell32.lib Ws2_32.lib psapi.lib gdiplus.lib delayimp.lib Mswsock.lib winmm.lib ole32.lib Shlwapi.lib strmbasd.lib vfw32.lib wininet.lib Wtsapi32.lib Userenv.lib /nologo /dll /debug /machine:I386 /out:"../bin/update/PcMain.dll" /pdbtype:sept /DELAYLOAD:gdiplus.dll
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PcMain - Win32 Release"
# Name "PcMain - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Lzw.cpp
# End Source File
# Begin Source File

SOURCE=.\MyAudioTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyAudioTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyClientMain.cpp
# End Source File
# Begin Source File

SOURCE=.\MyClientMain.h
# End Source File
# Begin Source File

SOURCE=.\MyClientTran.cpp
# End Source File
# Begin Source File

SOURCE=.\MyClientTran.h
# End Source File
# Begin Source File

SOURCE=.\MyDownTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDownTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyFrameTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFrameTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyKeyMTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyKeyMTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyMainFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMainFunc.h
# End Source File
# Begin Source File

SOURCE=.\MyMainTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMainTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyTlntTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTlntTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyUpTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyUpTrans.h
# End Source File
# Begin Source File

SOURCE=.\MyVideoTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MyVideoTrans.h
# End Source File
# Begin Source File

SOURCE=.\PcMain.cpp
# End Source File
# Begin Source File

SOURCE=.\PcMain.def
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IOCTLCMD.H
# End Source File
# Begin Source File

SOURCE=.\Lzw.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
