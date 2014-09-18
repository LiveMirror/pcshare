// AppearanceSetEclipse.h: interface for the CAppearanceSetEclipse class.
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

#if !defined(AFX_APPEARANCESETECLIPSE_H__598ECCD6_5508_4219_A699_A414FBC210BA__INCLUDED_)
#define AFX_APPEARANCESETECLIPSE_H__598ECCD6_5508_4219_A699_A414FBC210BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEclipseTabPaintManager : public CXTPTabPaintManager
{
public:
	class CColorSetEclipse : public CXTPTabPaintManager::CColorSetDefault
	{
		void RefreshMetrics()
		{
			CColorSetDefault::RefreshMetrics();
			m_clrFrameBorder =  RGB(132, 130, 132);

			if ((((CEclipseTabPaintManager*)m_pPaintManager)->m_bFillTab))
				m_clrSelectedText = GetXtremeColor(COLOR_CAPTIONTEXT);

			if ((((CEclipseTabPaintManager*)m_pPaintManager)->m_bFillTab))
				m_clrHeaderFace.SetStandardValue(XTPPaintManager()->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
			else
				m_clrHeaderFace.SetStandardValue(XTPPaintManager()->GetXtremeColor(XPCOLOR_3DFACE));
		}
	};


	class CAppearanceSetEclipse : public CXTPTabPaintManager::CAppearanceSetFlat
	{
	public:
		CAppearanceSetEclipse()
		{
			m_rcHeaderMargin.SetRect(-1, -1, 0, 0);
		}
		CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);
		void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		void RefreshMetrics()
		{
			CAppearanceSetFlat::RefreshMetrics();

			if ((((CEclipseTabPaintManager*)m_pPaintManager)->m_bFillTab))
				m_nButtonHeight = max(24, m_nButtonHeight);
			else
				m_nButtonHeight = max(22, m_nButtonHeight);
		}
	};

public:
	CEclipseTabPaintManager()
	{
		m_bBoldSelected = FALSE;
		m_bFrame = TRUE;
		m_bFillTab = TRUE;


		m_rcClientMargin.SetRect(0, 1, 0, 0);
		m_rcControlMargin.SetRect(4, 4, 4, 4);
		m_clientFrame = xtpTabFrameNone;

		SetAppearanceSet(new CAppearanceSetEclipse());
		SetColorSet(new CColorSetEclipse());
	}
	void DrawTabControl (CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);
	static void DrawFrame(CDC* pDC, CRect rcControl, BOOL bFillRect = FALSE);

	void SetFrame(BOOL bFrame)
	{
		if (!bFrame)
		{
			m_rcControlMargin.SetRectEmpty();
			m_bFrame = bFrame;
		}
	}

	BOOL m_bFrame;
	BOOL m_bFillTab;
};

template <class T>
class CEclipseDockingPanePaintManager: public T
{
public:
	CEclipseDockingPanePaintManager()
	{
		CEclipseTabPaintManager* pTabManager = (CEclipseTabPaintManager*)SetTabPaintManager(new CEclipseTabPaintManager());
		pTabManager->SetFrame(FALSE);
		pTabManager->m_bFillTab = FALSE;
		pTabManager->m_bShowIcons = FALSE;
		pTabManager->RefreshMetrics();
	}


	void AdjustCaptionRect(CXTPDockingPaneTabbedContainer* /*pPane*/, CRect& rc)
	{
		rc.DeflateRect(4, 4, 4, 4);
	}
	void DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* /*pPane*/, CRect rc)
	{
		CEclipseTabPaintManager::DrawFrame(&dc, rc, TRUE);
	}


	void DrawPane(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
	{
		dc.FillSolidRect(rc, XTPPaintManager()->GetXtremeColor(XPCOLOR_3DFACE));

		DrawCaption(dc, pPane, rc);

		AdjustCaptionRect(pPane, rc);

		if (pPane->IsTabsVisible())
		{
			m_pTabPaintManager->DrawTabControl(pPane, &dc, rc);
		}
	}
};

template <class T>
class CEclipseCommandBarsPaintManager: public T
{
public:

	BOOL IsPaneBar(CXTPCommandBar* pBar)
	{
		CXTPToolBar* pToolbar = DYNAMIC_DOWNCAST(CXTPToolBar, pBar);
		return (pToolbar && pToolbar->GetDockBar() == NULL && pToolbar->GetPosition() == xtpBarTop);
	}

	void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
	{
		if (IsPaneBar(pBar))
		{
			pDC->FillSolidRect(CXTPClientRect(pBar), XTPPaintManager()->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
		}
		else
		{
			T::FillCommandBarEntry(pDC, pBar);
		}
	}
	CRect GetCommandBarBorders(CXTPCommandBar* pBar)
	{
		if (IsPaneBar(pBar))
		{
			return CRect(1, 1, 1, 1);
		}
		else
		{
			return T::GetCommandBarBorders(pBar);
		}
	}


};

#endif // !defined(AFX_APPEARANCESETECLIPSE_H__598ECCD6_5508_4219_A699_A414FBC210BA__INCLUDED_)
