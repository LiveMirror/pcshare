// MyImageList.cpp: implementation of the CMyImageList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PcShare.h"
#include "MyImageList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyImageList::CMyImageList()
{

}

CMyImageList::~CMyImageList()
{

}

void CMyImageList::Add(UINT nBitmapId)
{
	CBitmap m_Bitmap, *pOldBitmap;
	m_Bitmap.LoadBitmap(nBitmapId);
	HDC hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	CDC m_MemDC;
	CDC* pDC = CDC::FromHandle(hScrDC);
	m_MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = m_MemDC.SelectObject(&m_Bitmap);
	COLORREF l = m_MemDC.GetPixel(0, 0);
	m_MemDC.SelectObject(pOldBitmap);
	CImageList::Add(&m_Bitmap, l);
	DeleteDC(hScrDC);
}

