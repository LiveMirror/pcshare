// MyWorkMoudle.h: interface for the CMyWorkMoudle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYWORKMOUDLE_H__3AA99B43_6891_4A26_A057_DDF4DDF8D0ED__INCLUDED_)
#define AFX_MYWORKMOUDLE_H__3AA99B43_6891_4A26_A057_DDF4DDF8D0ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyWorkMoudle  
{
public:
	
	void DoWork();
	CMyWorkMoudle();
	virtual ~CMyWorkMoudle();

	DWORD nParentThreadId;

private:

	HMODULE	hCtrMd;
	char	m_ModFilePath[256];
	HMODULE GetModFile(char* pFilePath, UINT nCmd);
	void MakeModFileMd5(LPCTSTR pFileName, BYTE* sMd5Str);
};

#endif // !defined(AFX_MYWORKMOUDLE_H__3AA99B43_6891_4A26_A057_DDF4DDF8D0ED__INCLUDED_)
