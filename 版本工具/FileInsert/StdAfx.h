// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__39F4E167_B641_4CD0_A65C_5857B7135007__INCLUDED_)
#define AFX_STDAFX_H__39F4E167_B641_4CD0_A65C_5857B7135007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

typedef struct _SYSFILEINFO_
{
	ULONGLONG nVerifyFlag;
	UINT nSrcFileLen;
	UINT nAddFileLen;
}SYSFILEINFO, *LPSYSFILEINFO;

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	BYTE	bWidth;               // Width of the image
	BYTE	bHeight;              // Height of the image (times 2)
	BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;            // Reserved
	WORD	wPlanes;              // Color Planes
	WORD	wBitCount;            // Bits per pixel
	DWORD	dwBytesInRes;         // how many bytes in this resource?
	WORD	nID;                  // the ID
} MEMICONDIRENTRY, *LPMEMICONDIRENTRY;

typedef struct 
{
	WORD			idReserved;   // Reserved
	WORD			idType;       // resource type (1 for icons)
	WORD			idCount;      // how many images?
	MEMICONDIRENTRY	idEntries[1]; // the entries for each image
} MEMICONDIR, *LPMEMICONDIR;
#pragma pack( pop )

typedef struct
{
    char    	szFileName[256];
    HINSTANCE	hInstance;
 } EXEDLLICONINFO, *LPEXEDLLICONINFO;

typedef enum tagWin32SysType{
   WindowsNT4,
   Windows2000,
   WindowsXP,
   Windows2003,
   Vista,
   Windows7
}Type;

#define _EXEC_FILE_			0x05
#define _RESTORE_FILE_		0x06

BOOL GetFileSaveInfo(BYTE** pSrcFileData, DWORD* nSrcFileLen, BYTE** pAddFileData, DWORD* nAddFileLen);
BOOL UpdateFileResource(LPCTSTR pFileName, LPCSTR lpType, LPCSTR  lpName,LPVOID pData, DWORD nLen);

typedef
BOOL
(WINAPI*
MyShellExecuteExW) (LPSHELLEXECUTEINFOW lpExecInfo);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__39F4E167_B641_4CD0_A65C_5857B7135007__INCLUDED_)
