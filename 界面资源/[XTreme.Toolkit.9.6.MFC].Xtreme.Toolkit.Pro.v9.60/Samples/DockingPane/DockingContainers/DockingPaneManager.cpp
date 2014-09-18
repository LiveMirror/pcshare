// DockingPaneManager.cpp: implementation of the CDockingPaneManager class.
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

#include "stdafx.h"
#include "DockingContainers.h"
#include "DockingPaneManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CDockingPaneTabbedContainer

CDockingPaneTabbedContainer::CDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneTabbedContainer(pLayout)
{
	m_arrCaptionButtons.Add(new CXTPDockingPaneCaptionButton(IDS_ABOUT, this));
	EnableToolTips();
}

void CDockingPaneTabbedContainer::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	CXTPDockingPaneTabbedContainer::OnCaptionButtonClick(pButton);

	if (pButton->GetID() == IDS_ABOUT)
	{
		((CDockingContainersApp*)AfxGetApp())->OnAppAbout();
	}
}

BOOL CDockingPaneTabbedContainer::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton->GetID() == IDS_ABOUT)
		return DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, GetParentFrame()) == 0;

	return CXTPDockingPaneTabbedContainer::IsCaptionButtonVisible(pButton);
}



//////////////////////////////////////////////////////////////////////////
// CDockingPaneMiniWnd

CDockingPaneMiniWnd::CDockingPaneMiniWnd(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneMiniWnd(pLayout)
{
	m_arrCaptionButtons.Add(new CXTPDockingPaneCaptionButton(IDS_ABOUT, this));
}

void CDockingPaneMiniWnd::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	CXTPDockingPaneMiniWnd::OnCaptionButtonClick(pButton);

	if (pButton->GetID() == IDS_ABOUT)
	{
		((CDockingContainersApp*)AfxGetApp())->OnAppAbout();
	}
}

BEGIN_MESSAGE_MAP(CDockingPaneMiniWnd, CXTPDockingPaneMiniWnd)
	ON_WM_NCRBUTTONDOWN()
END_MESSAGE_MAP()

void CDockingPaneMiniWnd::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
#ifdef _XTP_INCLUDE_COMMANDBARS
	if (nHitTest == HTCAPTION)
	{
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(((CXTPMDIFrameWnd*)AfxGetMainWnd())->GetCommandBars());

		CXTPDockingPaneManager* pManager = GetDockingPaneManager();

		POSITION pos = pManager->GetPaneList().GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = pManager->GetPaneList().GetNext(pos);

			CXTPControl* pControl = pPopupBar->GetControls()->Add(xtpControlButton, pPane->GetID());
			pControl->SetCaption(pPane->GetTitle());
			pControl->SetFlags(xtpFlagManualUpdate);

		}

		UINT nID = CXTPCommandBars::TrackPopupMenu(pPopupBar, TPM_RETURNCMD, point.x, point.y, AfxGetMainWnd());

		if (nID > 0)
		{
			pManager->ShowPane(nID);
		}

		pPopupBar->InternalRelease();

	}
#endif

	CXTPDockingPaneMiniWnd::OnNcRButtonDown(nHitTest, point);
}


//////////////////////////////////////////////////////////////////////////
// CDockingPanePaintManager



CDockingPanePaintManager::CDockingPanePaintManager()
{
	m_nSplitterSize = 6;
}

void DrawAboutGlyph(CDC* pDC, CPoint pt, COLORREF clr)
{
	CXTPPenDC pen(*pDC, clr);

	POINT pts[] = {
		{ pt.x - 1, pt.y - 2 }, { pt.x , pt.y - 3 },    { pt.x + 2, pt.y - 3 },
		{ pt.x + 3, pt.y - 2 }, { pt.x + 3, pt.y -1 },  { pt.x + 1, pt.y + 1 },
		{ pt.x + 1, pt.y + 3 }};
	pDC->Polyline(&pts[0], sizeof(pts) / sizeof(POINT));
	pDC->SetPixel(pt.x + 1, pt.y + 4, clr);
}

void CDockingPanePaintManager::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrText)
{
	CXTPDockingPaneOffice2003Theme::DrawCaptionButton(pDC, pButton, clrText);

	if (pButton->GetID() == IDS_ABOUT)
	{
		DrawCaptionButtonFrame(pDC, pButton, clrText);

		CRect rc(pButton->GetRect());
		DrawAboutGlyph(pDC, CPoint(rc.CenterPoint().x - 2, rc.CenterPoint().y - 1), clrText);
		DrawAboutGlyph(pDC, CPoint(rc.CenterPoint().x - 1, rc.CenterPoint().y -1), clrText);


	}
}

void CDockingPanePaintManager::DrawSplitter(CDC& dc, CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc(pSplitter);

	dc.FillSolidRect(rc, GetXtremeColor(XPCOLOR_3DFACE));



	if (!pSplitter->IsHorizontal())
	{
		for (int y = 1; y < rc.Height() - 1; y += 4)
		{
			dc.FillSolidRect(3, y + 1, 2, 2, GetXtremeColor(COLOR_BTNHIGHLIGHT));
			dc.FillSolidRect(2, y, 2, 2, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	else
	{
		for (int x = 1; x < rc.Width() - 1; x += 4)
		{
			dc.FillSolidRect(x + 1, 3, 2, 2, GetXtremeColor(COLOR_BTNHIGHLIGHT));
			dc.FillSolidRect(x, 2, 2, 2, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}

}


void CDockingPanePaintManager::RoundRect(CDC* pDC, CRect rc, COLORREF clr)
{
	CXTPBrushDC brush(*pDC, clr);
	CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_MENUBAR_BORDER));
	pDC->RoundRect(rc.left, rc.top, rc.right, rc.bottom, 6, 6);
}


void CDockingPanePaintManager::DrawCaptionButtonFrame(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF& clrText)
{
	if (m_bLunaTheme)
	{
		CXTPDockingPaneOffice2003Theme::DrawCaptionButtonFrame(pDC, pButton, clrText);
	}
	else
	{
		if (pButton->IsSelected())
			RoundRect(pDC, pButton->GetRect(), m_clrNormalCaptionLight);
		if (pButton->IsPressed())
			RoundRect(pDC, pButton->GetRect(), m_clrNormalCaptionDark);
	}
}

//////////////////////////////////////////////////////////////////////
// CDockingPaneManager

CDockingPaneManager::CDockingPaneManager()
{


}

CDockingPaneManager::~CDockingPaneManager()
{

}


CXTPDockingPaneBase* CDockingPaneManager::OnCreatePane(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout)
{
	if (type == xtpPaneTypeTabbedContainer)
		return new CDockingPaneTabbedContainer(pLayout);

	if (type == xtpPaneTypeMiniWnd)
		return new CDockingPaneMiniWnd(pLayout);

	return CXTPDockingPaneManager::OnCreatePane(type, pLayout);

}
