# Microsoft Developer Studio Project File - Name="PcShare" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PcShare - Win32 PcShare Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PcShare.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PcShare.mak" CFG="PcShare - Win32 PcShare Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PcShare - Win32 PcShare Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "PcShare - Win32 PcShare Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PcShare - Win32 PcShare Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PcShare___Win32_PcShare_Unicode_Debug"
# PROP BASE Intermediate_Dir "PcShare___Win32_PcShare_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PcShare___Win32_PcShare_Unicode_Debug"
# PROP Intermediate_Dir "PcShare___Win32_PcShare_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Wininet.lib Msimg32.lib Shlwapi.lib Ws2_32.lib Vfw32.lib psapi.lib gdiplus.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../bin/PcShare.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Wininet.lib Msimg32.lib Shlwapi.lib Ws2_32.lib Vfw32.lib psapi.lib gdiplus.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../bin/PcShare.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PcShare - Win32 PcShare Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PcShare___Win32_PcShare_Unicode_Release"
# PROP BASE Intermediate_Dir "PcShare___Win32_PcShare_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PcShare___Win32_PcShare_Unicode_Release"
# PROP Intermediate_Dir "PcShare___Win32_PcShare_Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Wininet.lib Msimg32.lib gdiplus.lib Shlwapi.lib Ws2_32.lib Vfw32.lib delayimp.lib psapi.lib /nologo /subsystem:windows /machine:I386 /out:"../bin/PcShare.exe" /OPT:NOWIN98
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Wininet.lib Msimg32.lib gdiplus.lib Shlwapi.lib Ws2_32.lib Vfw32.lib delayimp.lib psapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../bin/PcShare.exe" /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PcShare - Win32 PcShare Unicode Debug"
# Name "PcShare - Win32 PcShare Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Lzw.cpp
# End Source File
# Begin Source File

SOURCE=.\Lzw.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyBaseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBaseWnd.h
# End Source File
# Begin Source File

SOURCE=.\MyBottomView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBottomView.h
# End Source File
# Begin Source File

SOURCE=.\MyClientManaView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyClientManaView.h
# End Source File
# Begin Source File

SOURCE=.\MyClientTitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyClientTitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyCreateClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCreateClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyGlobalFuc.cpp
# End Source File
# Begin Source File

SOURCE=.\MyImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\MyImageList.h
# End Source File
# Begin Source File

SOURCE=.\MyLinkCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyLinkCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MyLogsView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyLogsView.h
# End Source File
# Begin Source File

SOURCE=.\MyMainShowWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMainShowWnd.h
# End Source File
# Begin Source File

SOURCE=.\MyOpenUrlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyOpenUrlDlg.h
# End Source File
# Begin Source File

SOURCE=.\MySelectTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MySelectTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MySendLinkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MySendLinkDlg.h
# End Source File
# Begin Source File

SOURCE=.\MySendMessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MySendMessDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyServerTran.cpp
# End Source File
# Begin Source File

SOURCE=.\MyServerTran.h
# End Source File
# Begin Source File

SOURCE=.\MySetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MySetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\MySocket.cpp
# End Source File
# Begin Source File

SOURCE=.\MySocket.h
# End Source File
# Begin Source File

SOURCE=.\MyStartSocksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyStartSocksDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyTopView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTopView.h
# End Source File
# Begin Source File

SOURCE=.\MyToServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyToServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyUpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyUpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyUpdateIpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyUpdateIpDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyUpLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyUpLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyWaitClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyWaitClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyWaitTransDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyWaitTransDlg.h
# End Source File
# Begin Source File

SOURCE=.\PcShare.cpp
# End Source File
# Begin Source File

SOURCE=.\PcShare.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MD5.H
# End Source File
# Begin Source File

SOURCE=.\MyMarco.h
# End Source File
# Begin Source File

SOURCE=.\PcShare.rc
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\111.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_m.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\camera.ico
# End Source File
# Begin Source File

SOURCE=.\res\CDDRIVE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\close_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close_dn_in.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close_up_in.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CloseMSN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLSDFOLD.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Cursor.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_l.cur
# End Source File
# Begin Source File

SOURCE=.\res\delete_16x.ico
# End Source File
# Begin Source File

SOURCE=.\res\DeleteFolderHS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DirOpen.ico
# End Source File
# Begin Source File

SOURCE=.\res\DRIVE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\eventlog.ico
# End Source File
# Begin Source File

SOURCE=.\res\eventlogFailureAudit.ico
# End Source File
# Begin Source File

SOURCE=.\res\eventlogSuccessAudit.ico
# End Source File
# Begin Source File

SOURCE=.\res\EXPLORER.ICO
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\folderopen.ico
# End Source File
# Begin Source File

SOURCE=.\res\GenericPicDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\GoToParentFolderHS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\hdrdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hdrup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ICO_10.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_com.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_dis.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_fil.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_key.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_mul.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ser.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_vid.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\IMG_0842.bmp
# End Source File
# Begin Source File

SOURCE=".\res\IP更新.ico"
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max_dn_gr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max_up_gr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Micro_Extras_014.ico
# End Source File
# Begin Source File

SOURCE=.\res\min_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min_dn_gr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min_up_gr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\netfol.ico
# End Source File
# Begin Source File

SOURCE=.\res\NETHOOD.ICO
# End Source File
# Begin Source File

SOURCE=".\res\Network Neighbourhood.ico"
# End Source File
# Begin Source File

SOURCE=.\res\noatun.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NoFix.ico
# End Source File
# Begin Source File

SOURCE=.\res\open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\opend.bmp
# End Source File
# Begin Source File

SOURCE=.\res\openiphone_005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PcShare.ico
# End Source File
# Begin Source File

SOURCE=.\res\PcShare.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Play.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Ps_Event.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Ps_Evnet.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ps_Update.ico
# End Source File
# Begin Source File

SOURCE=.\res\question.ico
# End Source File
# Begin Source File

SOURCE=.\res\qwqwqwq.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RefreshDocViewHS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RemoteDisk.ico
# End Source File
# Begin Source File

SOURCE=.\res\restor_dn_gr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\restor_up_gr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\restore_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\restore_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Save.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SaveAsWebPageHS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Screen.ico
# End Source File
# Begin Source File

SOURCE=.\res\SearchFolderHS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\status_anim.gif
# End Source File
# Begin Source File

SOURCE=.\res\Stop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SysConTrol.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\user.ico
# End Source File
# Begin Source File

SOURCE=.\res\video.ico
# End Source File
# Begin Source File

SOURCE=.\res\VSProject_genericproject.ico
# End Source File
# Begin Source File

SOURCE=.\res\W95MBX01.ICO
# End Source File
# Begin Source File

SOURCE=.\res\W95MBX04.ICO
# End Source File
# Begin Source File

SOURCE=.\res\WAIT07.CUR
# End Source File
# Begin Source File

SOURCE=".\res\帮助.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\帮助.ico"
# End Source File
# Begin Source File

SOURCE=".\res\帮助图标.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\保存视频.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\参数设置.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\参数设置.ico"
# End Source File
# Begin Source File

SOURCE=".\res\参数设置2.ico"
# End Source File
# Begin Source File

SOURCE=".\res\超级终端.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\超级终端.ico"
# End Source File
# Begin Source File

SOURCE=".\res\窗口.ico"
# End Source File
# Begin Source File

SOURCE=".\res\窗口管理.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\服务管理.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\服务管理.ico"
# End Source File
# Begin Source File

SOURCE=".\res\勾.ico"
# End Source File
# Begin Source File

SOURCE=".\res\关.ico"
# End Source File
# Begin Source File

SOURCE=".\res\关于.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\关于我们.ico"
# End Source File
# Begin Source File

SOURCE=".\res\键盘记录.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\键盘记录.ico"
# End Source File
# Begin Source File

SOURCE=".\res\键盘控制.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\进程管理.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\进程管理.ico"
# End Source File
# Begin Source File

SOURCE=".\res\进程管理7.ico"
# End Source File
# Begin Source File

SOURCE=".\res\进程控制.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\开.ico"
# End Source File
# Begin Source File

SOURCE=".\res\开始录像.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\开始录音.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\平滑处理.ico"
# End Source File
# Begin Source File

SOURCE=".\res\屏幕监控.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\屏幕监控.ico"
# End Source File
# Begin Source File

SOURCE=".\res\屏幕控制.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\屏幕控制1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\生成客户.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\生成客户.ico"
# End Source File
# Begin Source File

SOURCE=".\res\视频监控.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\视频监控.ico"
# End Source File
# Begin Source File

SOURCE=".\res\视屏.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\视屏.ico"
# End Source File
# Begin Source File

SOURCE=".\res\刷新IP.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\刷新IP.ico"
# End Source File
# Begin Source File

SOURCE=".\res\锁.ico"
# End Source File
# Begin Source File

SOURCE=".\res\停止录像.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\停止录音.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\未命名.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文件管理.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\文件管理.ico"
# End Source File
# Begin Source File

SOURCE=".\res\文件下载.ico"
# End Source File
# Begin Source File

SOURCE=".\res\无标题21.ico"
# End Source File
# Begin Source File

SOURCE=".\res\下载管理.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\下载管理.ico"
# End Source File
# Begin Source File

SOURCE=".\res\相关.ico"
# End Source File
# Begin Source File

SOURCE=".\res\小电脑.ico"
# End Source File
# Begin Source File

SOURCE=".\res\寻求帮助.ico"
# End Source File
# Begin Source File

SOURCE=".\res\用户小电脑.ico"
# End Source File
# Begin Source File

SOURCE=".\res\有摄像头小电脑.ico"
# End Source File
# Begin Source File

SOURCE=".\res\远程屏幕.ico"
# End Source File
# Begin Source File

SOURCE=".\res\远程终端.ico"
# End Source File
# Begin Source File

SOURCE=".\res\钥匙.ico"
# End Source File
# Begin Source File

SOURCE=".\res\主图标.ico"
# End Source File
# Begin Source File

SOURCE=".\res\注册表.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\注册表.ico"
# End Source File
# End Group
# Begin Group "TLNT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyTlntEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTlntEditView.h
# End Source File
# Begin Source File

SOURCE=.\MyTlntWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTlntWnd.h
# End Source File
# End Group
# Begin Group "FILE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyAttribDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyAttribDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyFileDlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDlDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyFileFindView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileFindView.h
# End Source File
# Begin Source File

SOURCE=.\MyFileListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFilelistview.h
# End Source File
# Begin Source File

SOURCE=.\MyFileTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileTreeView.h
# End Source File
# Begin Source File

SOURCE=.\MyFileWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileWnd.h
# End Source File
# End Group
# Begin Group "REGT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\myregteditvalue.cpp
# End Source File
# Begin Source File

SOURCE=.\myregteditvalue.h
# End Source File
# Begin Source File

SOURCE=.\MyRegtListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRegtListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MyRegtTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRegtTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MyRegtWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRegtWnd.h
# End Source File
# End Group
# Begin Group "MULT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyMultView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMultView.h
# End Source File
# Begin Source File

SOURCE=.\MyMultWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMultWnd.h
# End Source File
# End Group
# Begin Group "SERV"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyServDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyServDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyServListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyServListView.h
# End Source File
# Begin Source File

SOURCE=.\MyServWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyServWnd.h
# End Source File
# End Group
# Begin Group "CWND"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyCwndListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCwndListView.h
# End Source File
# Begin Source File

SOURCE=.\MyCwndWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCwndWnd.h
# End Source File
# End Group
# Begin Group "PROC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyProcListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyProcListView.h
# End Source File
# Begin Source File

SOURCE=.\MyProcWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyProcWnd.h
# End Source File
# End Group
# Begin Group "FRAM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyFramWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFramWnd.h
# End Source File
# End Group
# Begin Group "DOWN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyFileDownList.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDownList.h
# End Source File
# Begin Source File

SOURCE=.\MyFileDownView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDownView.h
# End Source File
# Begin Source File

SOURCE=.\MyFileDownWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDownWnd.h
# End Source File
# Begin Source File

SOURCE=.\MyFileLogsView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileLogsView.h
# End Source File
# End Group
# Begin Group "KEYM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyKeyMEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyKeyMEditView.h
# End Source File
# Begin Source File

SOURCE=.\MyKeyMWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MyKeyMWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\NetServer.exe
# End Source File
# Begin Source File

SOURCE="E:\备份代码\TOOL\QQWry.dat"
# End Source File
# Begin Source File

SOURCE=.\String.ini
# End Source File
# Begin Source File

SOURCE=.\res\StringDll.ini
# End Source File
# Begin Source File

SOURCE=.\res\StringPcMain.ini
# End Source File
# Begin Source File

SOURCE=.\res\StringPcMake.ini
# End Source File
# Begin Source File

SOURCE=.\res\Trans.avi
# End Source File
# Begin Source File

SOURCE=.\res\working.ani
# End Source File
# End Target
# End Project
