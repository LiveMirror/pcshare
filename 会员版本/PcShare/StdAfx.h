// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__76EF6D44_F157_4F38_8990_28B603203284__INCLUDED_)
#define AFX_STDAFX_H__76EF6D44_F157_4F38_8990_28B603203284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。

#define WINVER 0x0500		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_WINNT 0x0500		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_IE 0x0500	//为 IE 5.0 及更新版本改变为适当的值。

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>
#include <winsock2.h>
#include <afxtempl.h>
#include <afxcview.h>
#include <afxmt.h>
#include <Wininet.h>
#include <process.h>
#include <Winsvc.h>
#include <shlwapi.h>
#include <psapi.h>
#include <gdiplus.h>
#include <afxrich.h>
#include <Vfw.h>

#define _XTP_STATICLINK	 0
#include <XTToolkitPro.h>

#include "MyMarco.h"
#include "MyServerTran.h"
#include "MyBaseWnd.h"
#include "MyImageList.h"

void MyMainThread(LPVOID lPvoid);
void MyChildThread(LPVOID lPvoid);
void CheckValidThread(LPVOID lPvoid);
void StartChildThread(LPVOID lPvoid);

void BcdToAsc(BYTE *BcdStr, BYTE *AscStr, int Bcdlstrlen);
void AscToBcd(BYTE *AscStr, BYTE *BcdStr, int Asclstrlen);
BOOL GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue);
BOOL MakeFilePath(TCHAR *pathName);
BOOL IsMulitProcess();

void GetKeyFilePath(TCHAR* pFileName);
void GetMySaveFile(TCHAR* pFileName);
void GetTmpFilePath(TCHAR* FileName);
void GetBinFilePath(TCHAR* FileName);
void GetSysFileName(TCHAR* pFileName);
LPCTSTR GetIniFileName();
LPCTSTR GetIpList();

BOOL GetLoginInfo(TCHAR* pUrl, TCHAR* pIpStr);
int LocalityFromIP2(BYTE* IP, TCHAR* pFileName, TCHAR* pResult);

void GetUserInfo(LPCTSTR pId, TCHAR* pName, LPCTSTR pType);
void SetUserInfo(LPCTSTR pId, LPCTSTR pName, LPCTSTR pType);

LPBYTE GetMySource(LPCTSTR  pName, LPCTSTR pType, DWORD& len);
BOOL GetMySource(TCHAR* pFileName, BYTE** pFileData, DWORD& nFileLen);
void EncryptByte(LPVOID pData, DWORD nLen);
void EncryptByte(LPVOID pData, DWORD nLen, BYTE* pKey);
void GetWndText(MyServerTran* pItem, TCHAR* pText);
HCURSOR GetWaitCursor();
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void GetMd5Verify(BYTE* pSrcData, DWORD dSrcDataLen, BYTE* OutData);
HWND GetMainListWnd();
BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
LPBYTE GetMyExeSource(LPCTSTR  pName, LPCTSTR pType, DWORD& len);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__76EF6D44_F157_4F38_8990_28B603203284__INCLUDED_)
