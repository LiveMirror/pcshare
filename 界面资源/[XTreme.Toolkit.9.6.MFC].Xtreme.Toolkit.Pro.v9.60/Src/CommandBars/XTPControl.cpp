// XTPControl.cpp : implementation of the CXTPControl class.
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
#include "XTPResource.h"
#include "XTPResourceManager.h"

#include "XTPControl.h"
#include "XTPControls.h"
#include "XTPMouseManager.h"
#include "XTPCommandBar.h"
#include "XTPPaintManager.h"
#include "XTPImageManager.h"
#include "XTPToolbar.h"
#include "XTPCommandBars.h"
#include "XTPPopupBar.h"
#include "XTPCommandBarsResource.h"
#include "XTPCustomizeTools.h"
#include "XTPSoundManager.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPControl::CXTPControl()
{
	EnableAutomation();


	m_nId = 0;
	m_nIconId = 0;
	m_nCustomIconId = 0;
	m_nTag = 0;
	m_nIndex = 0;
	m_dwFlags = 0;
	m_rcControl.SetRectEmpty();
	m_rcRaw.SetRectEmpty();

	m_pParent = 0;
	m_pControls = 0;
	m_bExpanded = FALSE;

	m_bChecked = FALSE;
	m_bEnabled = TRUE;
	m_bBeginGroup = FALSE;
	m_bEnabled = TRUE;
	m_bTemporary = FALSE;
	m_bSelected = FALSE;
	m_bPressed = FALSE;

	m_bWrap = FALSE;

	m_dwHideFlags = xtpNoHide;
	m_controlType = xtpControlError;
	m_bDoubleGripper = FALSE;

	m_bDefaultItem = FALSE;
	m_buttonStyle = xtpButtonAutomatic;
	m_buttonCustomStyle = xtpButtonAutomatic;

	m_bCloseSubMenuOnClick = TRUE;
}

CXTPControl::~CXTPControl()
{
}

CSize CXTPControl::GetSize(CDC* pDC)
{
	return GetPaintManager()->DrawControl(pDC, this, FALSE);
}

void CXTPControl::Draw(CDC* pDC)
{
	GetPaintManager()->DrawControl(pDC, this);
}

void CXTPControl::DelayRedrawParent()
{
	if (m_pParent && IsWindow(m_pParent->GetSafeHwnd()))
		((CXTPCommandBar*)m_pParent)->DelayRedraw();
}

void CXTPControl::DelayLayoutParent()
{
	if (m_pParent)
		((CXTPCommandBar*)m_pParent)->DelayLayout();
}
void CXTPControl::RedrawParent()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
		((CWnd*)m_pParent)->Invalidate(FALSE);
}

void CXTPControl::ScreenToClient(CPoint* point)
{
	ASSERT(m_pParent);
	if (m_pParent)
		((CWnd*)m_pParent)->ScreenToClient(point);
}


BOOL CXTPControl::OnSetSelected(int bSelected)
{
	if (bSelected == m_bSelected)
		return FALSE;

	if (bSelected == TRUE_KEYBOARD)
		m_pParent->SetPopuped(-1);

	m_bSelected = bSelected;

	m_pParent->OnControlSelected(bSelected, this);

	DelayRedrawParent();

	return TRUE;
}

void CXTPControl::OnMouseHover()
{
	if (m_pParent->SetPopuped(-1))
	{
		if (m_pParent->IsTrackingMode() == TRUE_POPUP && m_pParent->GetPosition() != xtpBarPopup)
			m_pParent->SetTrackingMode(FALSE);
	}
}

void CXTPControl::SetPrompt(const CString& strPrompt)
{
	if (strPrompt.IsEmpty())
		return;

	if (strPrompt.Find('\n') != -1)
	{
		AfxExtractSubString(m_strTooltipText, strPrompt, 1);
		AfxExtractSubString(m_strDescriptionText, strPrompt, 0);
	}
	else
	{
		m_strDescriptionText = m_strTooltipText = strPrompt;
	}
	m_strCaption = m_strTooltipText;
}

void CXTPControl::SetID(int nId)
{
	if (m_nId == nId)
		return;

	CString strPrompt;
	if (m_strCaption.IsEmpty() && XTPResourceManager()->LoadString(&strPrompt, nId))
	{
		SetPrompt(strPrompt);
	}

	m_nId = nId;
}

void CXTPControl::SetCaption(UINT nIDCaption)
{
	CString strCaption;
	if (strCaption.LoadString(nIDCaption))
	{
		SetCaption(strCaption);
	}
}

void CXTPControl::SetCaption(CString strCaption)
{
	int nShortCutPos = strCaption.Find ('\t');
	if ( nShortCutPos != -1 )
	{
		m_strShortcutTextAuto = strCaption.Mid(nShortCutPos + 1);
		strCaption = strCaption.Left (nShortCutPos);

	}

	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;
		DelayLayoutParent();
	}
}

//Click Helper
void CXTPControl::ClickToolBarButton(CRect rcActiveRect )
{
	if (rcActiveRect.IsRectEmpty())
		rcActiveRect = m_rcControl;

	m_pParent->SetSelected(m_nIndex);
	m_bPressed = TRUE;
	RedrawParent();

	m_pParent->SetCapture();
	CPoint pt(0, 0);
	BOOL bClick = FALSE;
	while ( TRUE )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		//if  (!IsWindow(m_pParent->GetSafeHwnd()))
		//  return;

		if (msg.message == WM_LBUTTONUP)
		{
			bClick = m_bSelected && ((!pt.x && !pt.y) || rcActiveRect.PtInRect(pt));
			break;
		}
		if (msg.message == WM_MOUSEMOVE)
		{
			pt = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));
			if (OnSetSelected(m_rcControl.PtInRect(pt)))
				RedrawParent();
			continue;
		}

		DispatchMessage (&msg);

		if ( CWnd::GetCapture() != m_pParent )
			break;
	}

	ReleaseCapture();
	if (m_bSelected)
	{
		m_pParent->SetSelected(-1);
		m_bSelected = FALSE;
	}
	m_bPressed = FALSE;
	DelayRedrawParent();



	if (bClick)
	{
		OnExecute();
	}
}

void CXTPControl::OnExecute()
{
	XTPSoundManager()->PlaySystemSound(xtpSoundMenuCommand);

	m_bPressed = TRUE_KEYBOARD;

	CXTPCommandBar* pCommandBar = m_pParent->GetRootParent();
	if (pCommandBar->m_pReturnCmd)
	{
		XTPMouseManager()->SendTrackLost();

		*pCommandBar->m_pReturnCmd = m_nId;
		return;
	}

	NMXTPCONTROL tagNMCONTROL;
	tagNMCONTROL.hdr.code = CBN_XTP_EXECUTE;
	tagNMCONTROL.hdr.idFrom = m_nId;
	tagNMCONTROL.hdr.hwndFrom = 0;
	tagNMCONTROL.pControl = this;

	CWnd* pOwner = m_pParent->GetOwnerSite();
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();

	if (pCommandBars)
	{
		if (m_nId) pCommandBars->SetCommandUsed(m_nId);

		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pParent);
		if (pPopupBar && pPopupBar->GetControlPopup() && ((CXTPControl*)pPopupBar->GetControlPopup())->GetID())
		{
			pCommandBars->SetCommandUsed(((CXTPControl*)pPopupBar->GetControlPopup())->GetID());
		}
	}
	BOOL bCloseSubMenuOnClick = m_bCloseSubMenuOnClick;

	if (bCloseSubMenuOnClick)
	{
		XTPMouseManager()->SendTrackLost();
	}

	if (m_nId == 0)
		return;

	if (pOwner->SendMessage(WM_XTP_COMMAND, m_nId, (LPARAM)&tagNMCONTROL) == 0)
	{
		pOwner->SendMessage(WM_COMMAND, m_nId);
	}

	if (!bCloseSubMenuOnClick)
	{
		m_pParent->OnIdleUpdateCmdUI(0, 0);
		RedrawParent();
	}
}

BOOL CXTPControl::IsCursorOver()
{
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);
	return m_rcControl.PtInRect(pt);
}

void CXTPControl::Copy(CXTPControl* pControl, BOOL /*bRecursive*/)
{
	m_nId = pControl->m_nId;
	m_nTag = pControl->m_nTag;
	m_dwFlags = pControl->m_dwFlags;
	m_controlType = pControl->m_controlType;
	m_strCaption = pControl->m_strCaption;
	m_strShortcutText = pControl->m_strShortcutText;
	m_strShortcutTextAuto = pControl->m_strShortcutTextAuto;
	m_strTooltipText = pControl->m_strTooltipText;
	m_strDescriptionText = pControl->m_strDescriptionText;
	m_strParameter = pControl->m_strParameter;
	m_nCustomIconId = pControl->m_nCustomIconId;
	m_nIconId = pControl->m_nIconId;
	m_bTemporary = pControl->m_bTemporary;
	m_strCustomCaption = pControl->m_strCustomCaption;
	m_strCategory = pControl->m_strCategory;
	m_dwHideFlags = pControl->m_dwHideFlags;
	m_bDefaultItem = pControl->m_bDefaultItem;
	m_bEnabled = pControl->m_bEnabled;
	m_bChecked = pControl->m_bChecked;
	m_buttonCustomStyle = pControl->m_buttonCustomStyle;
	m_buttonStyle = pControl->m_buttonStyle;


	m_bBeginGroup = pControl->m_bBeginGroup;

	m_bCloseSubMenuOnClick = pControl->m_bCloseSubMenuOnClick;

	m_mapDocTemplatesAssigned.Copy(pControl->m_mapDocTemplatesAssigned);
	m_mapDocTemplatesExcluded.Copy(pControl->m_mapDocTemplatesExcluded);
}

BOOL CXTPControl::Compare(CXTPControl* pOther)
{
	if (GetStyle() != pOther->GetStyle())
		return FALSE;

	if (GetType() != pOther->GetType())
		return FALSE;

	if (GetID() != pOther->GetID())
		return FALSE;

	if (GetBeginGroup() != pOther->GetBeginGroup())
		return FALSE;

	if (GetIconId() != pOther->GetIconId())
		return FALSE;

	if (m_strCaption != pOther->m_strCaption)
		return FALSE;

	if (m_strTooltipText != pOther->m_strTooltipText)
		return FALSE;

	if (m_strDescriptionText != pOther->m_strDescriptionText)
		return FALSE;

	if (m_strCustomCaption != pOther->m_strCustomCaption)
		return FALSE;

	if (m_buttonCustomStyle != pOther->m_buttonCustomStyle)
		return FALSE;

	if (m_strParameter != pOther->m_strParameter)
		return FALSE;

	if ((m_dwHideFlags & xtpHideCustomize) != (pOther->m_dwHideFlags & xtpHideCustomize))
		return FALSE;

	return TRUE;
}

IMPLEMENT_XTP_CONTROL( CXTPControl, CCmdTarget)


BOOL CXTPControl::IsCustomizeMode()
{
	return m_pParent? m_pParent->IsCustomizeMode(): FALSE;
}

void CXTPControl::OnInvertTracker(CDC* pDC, CRect rect)
{
	ASSERT(!rect.IsRectEmpty());

	pDC->InvertRect(CRect(rect.left, rect.top, rect.right, rect.top + 2));
	pDC->InvertRect(CRect(rect.left, rect.bottom - 2, rect.right, rect.bottom));
	pDC->InvertRect(CRect(rect.left, rect.top + 2, rect.left + 2, rect.bottom - 2));
	pDC->InvertRect(CRect(rect.right - 2, rect.top + 2, rect.right, rect.bottom - 2));

}

void CXTPControl::OnCustomizeMouseMove(CPoint point)
{
	if ( IsCustomizeResizeAllow() && m_rcControl.PtInRect(point) && ((point.x - m_rcControl.left <= 2) || (m_rcControl.right - point.x <= 2)))
	{
		::SetCursor(AfxGetApp()->LoadCursor(XTP_IDC_VRESIZE));
	}
}

BOOL CXTPControl::CustomizeStartResize(CPoint point)
{
	if (m_rcControl.PtInRect(point) && ((point.x - m_rcControl.left <= 2) || (m_rcControl.right - point.x <= 2)))
	{
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);
		ASSERT(pCommandBars->m_pDragSelected == this);
		pCommandBars->m_pDragSelected = NULL;
		m_pParent->Invalidate(FALSE);
		m_pParent->UpdateWindow();

		CRect rectTracker = m_rcControl;
		m_pParent->ClientToScreen(rectTracker);

		m_pParent->SetCapture();
		::SetCursor(AfxGetApp()->LoadCursor(XTP_IDC_VRESIZE));

		CDC* pDC = 0;

		CWnd* pWnd = CWnd::GetDesktopWindow();
		if (pWnd->LockWindowUpdate())
			pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
		else
			pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE);

		OnInvertTracker(pDC, rectTracker);
		int nMinWidth = GetCustomizeMinWidth();
		BOOL bLeftAnchor = (point.x - m_rcControl.left <= 2);

		LONG& lTrackerAnchor = bLeftAnchor? rectTracker.left: rectTracker.right;
		int nOffset = bLeftAnchor? m_rcControl.left - point.x: m_rcControl.right - point.x;

		BOOL bAccept = FALSE;
		while (CWnd::GetCapture() == m_pParent)
		{
			MSG msg;
			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(msg.lParam);
				m_pParent->ClientToScreen(&point);
				point.x += nOffset;

				point.x = bLeftAnchor? min(point.x, rectTracker.right - nMinWidth):
					max(point.x, rectTracker.left + nMinWidth);

				if (lTrackerAnchor != point.x)
				{
					OnInvertTracker(pDC, rectTracker);
					lTrackerAnchor = point.x;
					OnInvertTracker(pDC, rectTracker);
				}
			}
			else if (msg.message == WM_KEYDOWN &&  msg.wParam == VK_ESCAPE) break;
			else if (msg.message ==WM_LBUTTONUP)
			{
				bAccept = TRUE;
				break;
			}
			else  ::DispatchMessage(&msg);
		}

		OnInvertTracker(pDC, rectTracker);

		if (CWnd::GetCapture() == m_pParent) ReleaseCapture();

		pWnd->UnlockWindowUpdate();
		if (pDC != NULL)
		{
			pWnd->ReleaseDC(pDC);
			pDC = NULL;
		}

		pCommandBars->m_pDragSelected = this;

		if (bAccept && rectTracker.Width() != m_rcControl.Width())
		{
			SetWidth(rectTracker.Width());
			m_pParent->OnRecalcLayout();
		}
		m_pParent->Redraw();

		return TRUE;

	}

	return FALSE;
}

void CXTPControl::CustomizeStartDrag(CPoint pt)
{
	ASSERT(m_pParent);
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
	ASSERT(pCommandBars);

	if ((pCommandBars->m_pDragSelected == this) && IsCustomizeResizeAllow() &&
		CustomizeStartResize(pt))
	{
		return;
	}

	pCommandBars->SetDragControl(this);

	CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
	DROPEFFECT dropEffect = pDropSource->DoDragDrop(this);

	if (dropEffect == DROPEFFECT_NONE || dropEffect == DROPEFFECT_MOVE)
	{
		CXTPCommandBar* pParent = m_pParent;
		pParent->SetPopuped(-1);
		pParent->SetSelected(-1);

		if (pCommandBars->GetDragControl() == this)
			pCommandBars->SetDragControl(NULL);


		if (GetBeginGroup() && m_nIndex < m_pControls->GetCount() - 1)
			m_pControls->GetAt(m_nIndex + 1)->SetBeginGroup(TRUE);

		if (m_nIndex == 0 && m_pControls->GetCount() > 1)
			m_pControls->GetAt(1)->SetBeginGroup(FALSE);

		m_pControls->Remove(this);

		pParent->OnRecalcLayout();
	}
	pCommandBars->SetDragControl(pCommandBars->GetDragControl());
}

void CXTPControl::OnCustomizeDragOver(CXTPControl* /*pDataObject*/, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	m_pParent->SetPopuped(-1);
	m_pParent->SetSelected(-1);
}
BOOL CXTPControl::IsCustomizeDragOverAvail(CXTPCommandBar* /*pCommandBar*/, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	return TRUE;
}

BOOL CXTPControl::IsCustomizeResizeAllow()
{
	return FALSE;
}

CXTPImageManager* CXTPControl::GetImageManager()
{
	if (m_pParent)
		return m_pParent->GetImageManager();

	if (m_pControls)
	{
		CXTPCommandBars* pCommandBars = m_pControls->GetCommandBars();
		if (pCommandBars)
			return pCommandBars->GetImageManager();
	}
	return XTPImageManager();
}
CXTPPaintManager* CXTPControl::GetPaintManager()
{
	if (m_pParent)
		return m_pParent->GetPaintManager();

	if (m_pControls)
	{
		CXTPCommandBars* pCommandBars = m_pControls->GetCommandBars();
		if (pCommandBars)
			return pCommandBars->GetPaintManager();
	}
	return XTPPaintManager();

}
CXTPImageManagerIcon* CXTPControl::GetImage(int nWidth)
{
	if (GetIconId() != 0)
		return GetImageManager()->GetImage(GetIconId(), nWidth);
	return NULL;

}

void CXTPControl::SetExpanded(BOOL bExpanded)
{
	m_bExpanded = bExpanded;
	if (!bExpanded) SetHideFlags(GetHideFlags() & ~xtpHideExpand);
}

BOOL CXTPControl::IsItemDefault()
{
	return m_bDefaultItem;
}

void CXTPControl::SetItemDefault(BOOL bDefault)
{
	if (bDefault != m_bDefaultItem)
	{
		m_bDefaultItem = bDefault;
		DelayLayoutParent();
	}
}

void CXTPControl::SetFlags(DWORD dwFlags)
{
	m_dwFlags = dwFlags;
}

DWORD CXTPControl::GetFlags()
{
	return m_dwFlags;
}

void CXTPControl::CDocTemplateMap::Copy(CDocTemplateMap& map)
{
	RemoveAll();

	UINT nIDResource;
	BOOL bValue;
	POSITION pos = map.GetStartPosition();
	while (pos)
	{
		map.GetNextAssoc(pos, nIDResource, bValue);
		SetAt(nIDResource, bValue);
	}
}

CXTPControl* CXTPControl::FromUI(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pCommandBar = DYNAMIC_DOWNCAST(CXTPCommandBar, pCmdUI->m_pOther);
	if (!pCommandBar)
		return NULL;

	CXTPControls* pControls = pCommandBar->GetControls();
	if ((int)pCmdUI->m_nIndex < pControls->GetCount())
		return pControls->GetAt(pCmdUI->m_nIndex);

	return NULL;

}

BOOL CXTPControl::OnLButtonDblClk(CPoint point)
{
	OnClick(FALSE, point);
	return TRUE;
}

void CXTPControl::Reset()
{
	if (m_nCustomIconId != 0 || !m_strCustomCaption.IsEmpty())
	{
		m_nCustomIconId = 0;
		m_strCustomCaption = _T("");
		GetParent()->OnRecalcLayout();
	}
}

void CXTPControl::SetCustomIcon(HICON hIcon)
{
	CXTPImageManager* pImageManager = GetImageManager();
	if (pImageManager)
	{
		m_nCustomIconId = pImageManager->AddCustomIcon(hIcon);
	}
}


