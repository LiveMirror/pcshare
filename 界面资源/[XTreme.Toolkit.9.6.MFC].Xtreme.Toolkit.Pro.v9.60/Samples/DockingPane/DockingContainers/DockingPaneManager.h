// DockingPaneManager.h: interface for the CDockingPaneManager class.
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

#if !defined(AFX_DOCKINGPANEMANAGER_H__34C1E8DA_AC92_4721_BC15_A904662E0A5A__INCLUDED_)
#define AFX_DOCKINGPANEMANAGER_H__34C1E8DA_AC92_4721_BC15_A904662E0A5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDockingPaneTabbedContainer : public CXTPDockingPaneTabbedContainer
{
public:
	CDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout);

	void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);
	BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);
};

class CDockingPaneMiniWnd : public CXTPDockingPaneMiniWnd
{
public:
	CDockingPaneMiniWnd(CXTPDockingPaneLayout* pLayout);
	void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);

	void OnNcRButtonDown(UINT nHitTest, CPoint point);

	DECLARE_MESSAGE_MAP()
};



class CDockingPanePaintManager : public CXTPDockingPaneOffice2003Theme
{
public:
	CDockingPanePaintManager();
protected:

	void RoundRect(CDC* pDC, CRect rc, COLORREF clr);
	void DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrText);
	void DrawSplitter(CDC& dc, CXTPDockingPaneSplitterWnd* pSplitter);

	void DrawCaptionButtonFrame(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF& clrText);
};


class CDockingPaneManager : public CXTPDockingPaneManager
{
public:
	CDockingPaneManager();
	virtual ~CDockingPaneManager();

protected:
	virtual CXTPDockingPaneBase* OnCreatePane(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout);

};

#endif // !defined(AFX_DOCKINGPANEMANAGER_H__34C1E8DA_AC92_4721_BC15_A904662E0A5A__INCLUDED_)
