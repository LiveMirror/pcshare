// srvritem.h : interface of the CWordPadSrvrItem class
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

class CWordPadDoc;
class CWordPadView;

class CEmbeddedItem : public COleServerItem
{
	DECLARE_DYNAMIC(CEmbeddedItem)

// Constructors
public:
	CEmbeddedItem(CWordPadDoc* pContainerDoc, int nBeg = 0, int nEnd = -1);

// Attributes
	int m_nBeg;
	int m_nEnd;
	LPDATAOBJECT m_lpRichDataObj;
	CWordPadDoc* GetDocument() const
		{ return (CWordPadDoc*) COleServerItem::GetDocument(); }
	CWordPadView* GetView() const;

// Implementation
public:
	BOOL OnDrawEx(CDC* pDC, CSize& rSize, BOOL bOutput);
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};


/////////////////////////////////////////////////////////////////////////////
