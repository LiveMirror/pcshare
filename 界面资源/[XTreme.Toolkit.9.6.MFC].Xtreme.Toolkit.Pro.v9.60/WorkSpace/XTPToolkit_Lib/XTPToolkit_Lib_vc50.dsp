# Microsoft Developer Studio Project File - Name="XTPToolkit_Lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XTPToolkit_Lib - Win32 Dynamic Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XTPToolkit_Lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XTPToolkit_Lib.mak" CFG="XTPToolkit_Lib - Win32 Dynamic Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XTPToolkit_Lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Dynamic Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Dynamic Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Dynamic Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTPToolkit_Lib - Win32 Dynamic Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XTPToolkit_Lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release/vc50"
# PROP BASE Intermediate_Dir "Release/vc50"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release/vc50"
# PROP Intermediate_Dir "Release/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_XTP_STATICLINK" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibStatic.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug/vc50"
# PROP BASE Intermediate_Dir "Debug/vc50"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug/vc50"
# PROP Intermediate_Dir "Debug/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_XTP_STATICLINK" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibStaticd.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Dynamic Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XTPToolkit_Lib___Win32_Dynamic_Release"
# PROP BASE Intermediate_Dir "XTPToolkit_Lib___Win32_Dynamic_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Dynamic_Release/vc50"
# PROP Intermediate_Dir "Dynamic_Release/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "WIN32" /D "_AFXDLL" /D "NDEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibDynStatic.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Dynamic Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XTPToolkit_Lib___Win32_Dynamic_Debug"
# PROP BASE Intermediate_Dir "XTPToolkit_Lib___Win32_Dynamic_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dynamic_Debug/vc50"
# PROP Intermediate_Dir "Dynamic_Debug/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /MDd /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "WIN32" /D "_AFXDLL" /D "_DEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibDynStaticd.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XTPToolkit_Lib___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "XTPToolkit_Lib___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release/vc50"
# PROP Intermediate_Dir "Unicode_Release/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "NDEBUG" /D "WIN32" /D "_UNICODE" /D "_LIB" /D "_XTP_STATICLINK" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibStaticU.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XTPToolkit_Lib___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "XTPToolkit_Lib___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug/vc50"
# PROP Intermediate_Dir "Unicode_Debug/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "_DEBUG" /D "WIN32" /D "_UNICODE" /D "_LIB" /D "_XTP_STATICLINK" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibStaticUD.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Dynamic Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XTPToolkit_Lib___Win32_Dynamic_Unicode_Debug"
# PROP BASE Intermediate_Dir "XTPToolkit_Lib___Win32_Dynamic_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dynamic_Unicode_Debug/vc50"
# PROP Intermediate_Dir "Dynamic_Unicode_Debug/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /D "WIN32" /D "_AFXDLL" /D "_DEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /MDd /W4 /GX /Zi /Od /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "_AFXDLL" /D "_DEBUG" /D "WIN32" /D "_UNICODE" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibDynStaticUD.lib"

!ELSEIF  "$(CFG)" == "XTPToolkit_Lib - Win32 Dynamic Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XTPToolkit_Lib___Win32_Dynamic_Unicode_Release"
# PROP BASE Intermediate_Dir "XTPToolkit_Lib___Win32_Dynamic_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Dynamic_Unicode_Release/vc50"
# PROP Intermediate_Dir "Dynamic_Unicode_Release/vc50"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /D "WIN32" /D "_AFXDLL" /D "NDEBUG" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include" /I "../../include/CommandBars" /I "../../include/DockingPane" /I "../../include/PropertyGrid" /I "../../include/Controls" /I "../../include/Controls/Common.Include" /I "../../Src/Controls/Common.Src" /I "../../include/CommonPro.Include" /I "../../include/Controls/Common.Include/Theme" /I "../../include/Controls/ToolkitPro" /I "../../include/TabManager" /I "../../include/ReportControl" /D "_AFXDLL" /D "NDEBUG" /D "WIN32" /D "_UNICODE" /D "_LIB" /D "_XTP_DYN_STATICLIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc50/XTP9601LibDynStaticU.lib"

!ENDIF 

# Begin Target

# Name "XTPToolkit_Lib - Win32 Release"
# Name "XTPToolkit_Lib - Win32 Debug"
# Name "XTPToolkit_Lib - Win32 Dynamic Release"
# Name "XTPToolkit_Lib - Win32 Dynamic Debug"
# Name "XTPToolkit_Lib - Win32 Unicode Release"
# Name "XTPToolkit_Lib - Win32 Unicode Debug"
# Name "XTPToolkit_Lib - Win32 Dynamic Unicode Debug"
# Name "XTPToolkit_Lib - Win32 Dynamic Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "Theme"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTButtonTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTButtonThemeOffice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTCaptionTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTFlatControlsTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTFlatTabCtrlTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTHeaderTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTMenuListBoxTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTOutBarTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTSplitterTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTTabCtrlTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\Theme\XTThemeManager.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTAnimationMemDC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTBrowseDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTBrowseEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTCaption.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTCaptionPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTCheckListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorPageCustom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorPageStandard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorRef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTColorSelectorCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTComboBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTDirWatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTDropSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTEditListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTFlatComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTFlatHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTFlatTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTFontCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTFunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTGlobal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTHexEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTHtmlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTListCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTMDIWndTab.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTMemDC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTMemFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTMenuListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTOutBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTPagerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTRegistryManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizeFormView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizeGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizePoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizePropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizePropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTResizeRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTSearchOptionsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTShellListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTShellListCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTShellPidl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTShellSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTShellTreeBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTShellTreeCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTSortClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTabCtrlButtons.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTabCtrlButtons.h
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTabView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTipOfTheDay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTipWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTreeBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTTreeCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTVC50Helpers.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTVersionManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTWindowList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTWindowPos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTWndHook.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\Common.Src\XTWndShadow.cpp
# End Source File
# End Group
# Begin Group "ToolkitPro"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPPopupControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPPopupItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPPopupPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPShortcutBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPShortcutBarPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPShortcutBarPane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPTaskPanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPTaskPanelGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPTaskPanelGroupItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPTaskPanelItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPTaskPanelItems.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\Controls\ToolkitPro\XTPTaskPanelPaintManager.cpp
# End Source File
# End Group
# End Group
# Begin Group "CommandBars"

# PROP Default_Filter ""
# Begin Group "Themes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\CommandBars\Themes\XTPDefaultTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\Themes\XTPNativeXPTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\Themes\XTPOffice2003Theme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\Themes\XTPOfficeTheme.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCommandBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCommandBars.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlComboBoxExt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlCustom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlExt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControlPopupColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPControls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCustomizeCommandsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCustomizeKeyboardPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCustomizeOptionsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCustomizeSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCustomizeToolbarsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPCustomizeTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPDockBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPDockContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPDockState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPHookManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPImageEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPMouseManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPPopupBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPReBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPShadowsManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPShortcutManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPTabClientWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommandBars\XTPToolBar.cpp
# End Source File
# End Group
# Begin Group "DockingPane"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneAutoHidePanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneLayout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneMiniWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPanePaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneSplitterContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\DockingPane\XTPDockingPaneTabbedContainer.cpp
# End Source File
# End Group
# Begin Group "PropertyGrid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGrid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridInplaceButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridInplaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridInplaceList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItemBool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItemColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItemExt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItemFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItemNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridItemSize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\PropertyGrid\XTPPropertyGridView.cpp
# End Source File
# End Group
# Begin Group "TabManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\TabManager\XTPTabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TabManager\XTPTabManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TabManager\XTPTabPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TabManager\XTPTabPaintManagerAppearance.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\TabManager\XTPTabPaintManagerColors.cpp
# End Source File
# End Group
# Begin Group "CommonPro"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPColorManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPDrawHelpers.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPImageManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPManageState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPPropExchange.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\CommonPro.Src\XTPWinThemeWrapper.cpp
# End Source File
# End Group
# Begin Group "ReportControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportColumn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportColumns.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportDragDrop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportFilterEditControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportGroupRow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportHyperlink.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportInplaceControls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportRecordItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportRecordItemText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportRecords.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportRow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportRows.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportSubListControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportTip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\ReportControl\XTPReportView.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Controls.h"

# PROP Default_Filter ""
# Begin Group "Common.h"

# PROP Default_Filter ""
# Begin Group "Theme.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTButtonTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTButtonThemeOffice.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTCaptionTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTFlatControlsTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTFlatTabCtrlTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTHeaderTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTMenuListBoxTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTOutBarTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTSplitterTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTTabCtrlTheme.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\Theme\XTThemeManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTAnimationMemDC.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTBrowseDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTBrowseEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTCaption.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTCaptionPopupWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTCheckListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorPageCustom.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorPageStandard.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorPicker.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorPopup.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorRef.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTColorSelectorCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTComboBoxEx.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTDateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTDirWatcher.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTDropSource.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTEditListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTFlatComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTFlatHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTFlatTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTFontCombo.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTFunctions.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTGlobal.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTHexEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTHtmlView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTHyperLink.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTListBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTListCtrlView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTMaskEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTMDIWndTab.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTMemDC.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTMemFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTMenuListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTNotify.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTOutBarCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTPagerCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTRegistryManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResize.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizeDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizeFormView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizeGroupBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizePoint.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizePropertyPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizePropertySheet.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTResizeRect.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTSearchOptionsCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTShellListBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTShellListCtrlView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTShellPidl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTShellSettings.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTShellTreeBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTShellTreeCtrlView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTSortClass.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTabView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTipOfTheDay.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTipWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTrayIcon.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTreeBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTTreeCtrlView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTVC50Helpers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTVersionManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTWindowList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTWindowPos.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTWndHook.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\Common.Include\XTWndShadow.h
# End Source File
# End Group
# Begin Group "ToolkitPro.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPPopupControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPPopupItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPPopupPaintManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPShortcutBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPShortcutBarPaintManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPShortcutBarPane.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanel.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanelDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanelGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanelGroupItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanelItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanelItems.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Controls\ToolkitPro\XTPTaskPanelPaintManager.h
# End Source File
# End Group
# End Group
# Begin Group "CommandBars.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCommandBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCommandBars.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCommandBarsDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCommandBarsIncludes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCommandBarsResource.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlComboBoxExt.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlCustom.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlExt.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlPopup.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControlPopupColor.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPControls.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCustomizeCommandsPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCustomizeKeyboardPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCustomizeOptionsPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCustomizeSheet.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCustomizeToolbarsPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPCustomizeTools.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPDialogBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPDialogBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPDockBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPDockContext.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPDockState.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPHookManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPImageEditor.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPMenuBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPMouseManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPPaintManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPPopupBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPReBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPShadowsManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPShortcutManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPSoundManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPTabClientWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommandBars\XTPToolBar.h
# End Source File
# End Group
# Begin Group "DockingPane.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPane.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneAutoHidePanel.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneContext.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneIncludes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneLayout.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneMiniWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPanePaintManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneResource.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneSplitterContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DockingPane\XTPDockingPaneTabbedContainer.h
# End Source File
# End Group
# Begin Group "PropertyGrid.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridIncludes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridInplaceButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridInplaceEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridInplaceList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItemBool.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItemColor.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItemExt.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItemFont.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItemNumber.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridItemSize.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridPaintManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridResource.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PropertyGrid\XTPPropertyGridView.h
# End Source File
# End Group
# Begin Group "TabManager.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\TabManager\XTPTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TabManager\XTPTabManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TabManager\XTPTabManagerIncludes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TabManager\XTPTabPaintManager.h
# End Source File
# End Group
# Begin Group "CommonPro.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTMaskEditT.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPColorManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPDrawHelpers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPImageManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPIntel80Helpers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPMacros.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPManageState.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPPropExchange.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPPropExchange.inc
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPResourceManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPVC50Helpers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPVC80Helpers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CommonPro.Include\XTPWinThemeWrapper.h
# End Source File
# End Group
# Begin Group "ReportControl.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportColumn.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportColumns.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportControlIncludes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportControlResource.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportDragDrop.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportFilterEditControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportGroupRow.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportHyperlink.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportInplaceControls.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportPaintManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportRecordItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportRecordItemText.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportRecords.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportRow.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportRows.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportSubListControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportTip.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReportControl\XTPReportView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
