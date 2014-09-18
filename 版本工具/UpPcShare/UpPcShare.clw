; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUpPcShareDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "UpPcShare.h"

ClassCount=3
Class1=CUpPcShareApp
Class2=CUpPcShareDlg

ResourceCount=3
Resource2=IDD_UPPCSHARE_DIALOG
Resource1=IDR_MAINFRAME
Class3=CMyUnZipDlg
Resource3=IDD_DIALOG_UNZIP

[CLS:CUpPcShareApp]
Type=0
HeaderFile=UpPcShare.h
ImplementationFile=UpPcShare.cpp
Filter=N

[CLS:CUpPcShareDlg]
Type=0
HeaderFile=UpPcShareDlg.h
ImplementationFile=UpPcShareDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CUpPcShareDlg



[DLG:IDD_UPPCSHARE_DIALOG]
Type=1
Class=CUpPcShareDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_FILENAME,edit,1350633600
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON_GETFILE,button,1342242816
Control6=IDC_STATIC,static,1342308865
Control7=IDC_EDIT_USER_ID,edit,1350639745
Control8=IDC_CHECK_ISKEY,button,1342242819

[DLG:IDD_DIALOG_UNZIP]
Type=1
Class=CMyUnZipDlg
ControlCount=1
Control1=IDC_PROGRESS_FILE,msctls_progress32,1342177280

[CLS:CMyUnZipDlg]
Type=0
HeaderFile=MyUnZipDlg.h
ImplementationFile=MyUnZipDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMyUnZipDlg
VirtualFilter=dWC

