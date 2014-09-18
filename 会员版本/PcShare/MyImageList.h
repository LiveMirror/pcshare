// MyImageList.h: interface for the CMyImageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYIMAGELIST_H__E3425BC7_08F7_4B6E_8B56_6E3B53743F96__INCLUDED_)
#define AFX_MYIMAGELIST_H__E3425BC7_08F7_4B6E_8B56_6E3B53743F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyImageList : public CImageList  
{
public:
	CMyImageList();
	virtual ~CMyImageList();
	void Add(UINT nBitmapId);
};

#endif // !defined(AFX_MYIMAGELIST_H__E3425BC7_08F7_4B6E_8B56_6E3B53743F96__INCLUDED_)
