// MainFrm.cpp : implementation of the CMainFrame class
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
#include "CommonControls.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_XTP_CREATECONTROL()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DOC, OnUpdateButtonDoc)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create Status bar.
	// Important: All control bars including the Status Bar
	// must be created before the CommandBars....
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Create the AnimationBar
	CXTPToolBar* pAnimationBar = (CXTPToolBar*)pCommandBars->Add(_T("Animation Bar"), xtpBarTop);
	if (!pAnimationBar || !pAnimationBar->LoadToolBar(IDR_ANIMATIONBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Dock the AnimationBar to the right of the standard toolbar
	DockRightOf(pAnimationBar, pToolBar);

	// Create the SliderBar
	CXTPToolBar* pSliderBar = (CXTPToolBar*)pCommandBars->Add(_T("Slider Bar"), xtpBarTop);
	if (!pSliderBar || !pSliderBar->LoadToolBar(IDR_SLIDERBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Create the OptionsBar
	CXTPToolBar* pOptionsBar = (CXTPToolBar*)pCommandBars->Add(_T("Options Bar"), xtpBarTop);
	if (!pOptionsBar || !pOptionsBar->LoadToolBar(IDR_OPTIONSBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Dock the OptionsBar to the right of the SliderBar
	DockRightOf(pOptionsBar, pSliderBar);

	// Create the StaticBar
	CXTPToolBar* pStaticBar = (CXTPToolBar*)pCommandBars->Add(_T("Static Bar"), xtpBarTop);
	if (!pStaticBar || !pStaticBar->LoadToolBar(IDR_STATICBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Dock the StaticBar to the right of the OptionsBar
	DockRightOf(pStaticBar, pOptionsBar);

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	// This function is called by the Extreme Tool Kit application framework.
	// This function handles the creation of the controls that are placed
	// on the CommandBars.  The LPCREATECONTROLSTRUCT structure contains information
	// about the control to create.

#if _MSC_VER >= 1200 || defined (_XTP_INCLUDE_CONTROLS)

	// Create the Calendar control
	if (lpCreateControl->nID == IDC_CALENDAR)
	{
		// Create the calendar control.
		// Note: The control does not use the DTS_UPDOWN style.
		// DTP controls that do not use the DTS_UPDOWN style display
		// an arrow button. If the user clicks this button, a month
		// calendar control drops down. The user can select a specific
		// date by clicking an area of the calendar.
		if (!m_monthCal.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT,
			CRect(0, 0, 200, 22), this, IDC_CALENDAR))
		{
			return FALSE;
		}

		// Use m_monthCal to create a new CXTPControlCustom* control.
		CXTPControlCustom* pControlMonthCal = CXTPControlCustom::CreateControlCustom(&m_monthCal);

		// Set the xtpFlagManualUpdate flag.
		// Without this flag you must call the Enable member of
		// CCmdUI in the ON_UPDATE_COMMAND_UI handler.
		pControlMonthCal->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlMonthCal;
		return TRUE;
	}

	// Create the Date control
	if (lpCreateControl->nID == IDC_DATE)
	{
		// Create the date control.
		// Note: This control is created using the DTS_UPDOWN style.
		// The DTS_UPDOWN style creates a control with a spin control
		// positioned on the right side of the Date control.
		if (!m_date.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_UPDOWN,
			CRect(0, 0, 100, 22), this, IDC_DATE))
		{
			return FALSE;
		}

		// Create a new CXTPControlCustom* control from the m_date control.
		CXTPControlCustom* pControlDate = CXTPControlCustom::CreateControlCustom(&m_date);

		// Set the xtpFlagManualUpdate flag.
		// Without this flag you must call the Enable member
		// of CCmdUI in the ON_UPDATE_COMMAND_UI handler.
		pControlDate->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlDate;
		return TRUE;
	}

	// Create the Time control
	if (lpCreateControl->nID == IDC_TIME)
	{
		// Create the date control.
		// Note: This control is created using the DTS_UPDOWN style.
		// The DTS_UPDOWN style creates a control with a spin control
		// positioned on the right side of the Time control.
		if (!m_time.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_UPDOWN,
			CRect(0, 0, 90, 22), this, IDC_TIME))
		{
			return FALSE;
		}

		// Set the format of the Time control.
		m_time.SetFormat(_T("hh':'mm':'ss tt"));

		// Create a CXTPControlCustom* from the m_time control.
		CXTPControlCustom* pControlTime = CXTPControlCustom::CreateControlCustom(&m_time);

		// Set the xtpFlagManualUpdate flag.
		// Without this flag you must call the Enable member
		// of CCmdUI in the ON_UPDATE_COMMAND_UI handler.
		pControlTime->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlTime;
		return TRUE;
	}
#endif

	// Create the Animation control
	if (lpCreateControl->nID == IDC_ANIMATION)
	{
		// Create the Animation control.
		if (!m_wndAnimCtrl.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | ACS_AUTOPLAY,
			CRect(0, 0, 0, 0), this, IDC_ANIMATION))
		{
			return FALSE;
		}

		// Open the HeartBeat avi that is associated with the IDR_HEARTBEAT string resource.
		m_wndAnimCtrl.Open(IDR_HEARTBEAT);

		// Create a CXTPControlCustom* control from the m_wndAnimCtrl control.
		CXTPControlCustom* pControlAnimation = CXTPControlCustom::CreateControlCustom(&m_wndAnimCtrl);

		// Set the margins of the Animation control.
		pControlAnimation->SetBorders(3, 3, 3, 3);

		// Set the vertical draw options to xtpVerticalShow.
		// This will draw the animation control when the Animation
		// toolbar is docked vertically.
		pControlAnimation->SetVerticalPositionOption(xtpVerticalShow);

		lpCreateControl->pControl = pControlAnimation;
		return TRUE;
	}

	// Create the Slider control
	if (lpCreateControl->nID == IDC_SLIDER)
	{
		// Create the Slider control
		if (!m_wndSlider.Create(TBS_HORZ | WS_CHILD | WS_VISIBLE,
			CRect(0, 0, 200, 16), this, IDC_SLIDER))
		{
			return FALSE;
		}

		// Set the maximum range of the Slider control's scroll bar.
		m_wndSlider.SetRangeMax(0xFF);

		// Create a CXTPControlCustom* from the m_wndSlider control.
		CXTPControlCustom* pControlSlider = CXTPControlCustom::CreateControlCustom(&m_wndSlider);

		// Set the margins of the Slider control.
		pControlSlider->SetBorders(3, 3, 3, 3);

		// Set the vertical draw options to xtpVerticalButton.
		// This will show a button in place of the Slider control
		// when the Slider control toolbar is docked vertically.
		pControlSlider->SetVerticalPositionOption(xtpVerticalButton);

		lpCreateControl->pControl = pControlSlider;
		return TRUE;
	}

	// Create the Option radio button
	if (lpCreateControl->nID >= IDC_OPTION_HIDE && lpCreateControl->nID <= IDC_OPTION_BUTTON)
	{
		// Get the ID of the control currently being created
		UINT nID = lpCreateControl->nID;

		// Determine the index of the control
		CButton& wndOption = m_wndOptions[nID - IDC_OPTION_HIDE];

		// Create the Options radio button
		if (!wndOption.Create(NULL, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			CRect(0, 0, 100, 20), this, nID))
		{
			return FALSE;
		}

		// Use the radio button to create a CXTPControlCustom* control
		CXTPControlCustom* pControlOption = CXTPControlCustom::CreateControlCustom(&wndOption);

		// Set the ID of the custom control to the ID of the radio button
		pControlOption->SetID(nID);

		// Set the window text to the string resource associated with the button ID
		wndOption.SetWindowText(pControlOption->GetCaption());

		// Set the font of the radio button
		wndOption.SetFont(XTPPaintManager()->GetRegularFont());

		// Check the first radio button on the Options toolbar.
		wndOption.SetCheck(nID == IDC_OPTION_HIDE);

		// Set the vertical draw option to hide when the Option toolbar is drawn vertically.
		pControlOption->SetVerticalPositionOption(xtpVerticalHide);

		// Set the xtpFlagManualUpdate flag.
		// Without this flag you must call the Enable member of
		// CCmdUI in the ON_UPDATE_COMMAND_UI handler.
		pControlOption->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlOption;
		return TRUE;
	}

	// Create the Static control
	if (lpCreateControl->nID == ID_BUTTON_DOC)
	{
		CXTPControl* pControlStatic = new CControlStatic();

		lpCreateControl->pControl = pControlStatic;
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// This  member function is called when the Slider control is scrolled.

	// Determine if the WM_HSCROLL message is from the Slider control's scroll bar.
	if ((CSliderCtrl*)pScrollBar == &m_wndSlider)
	{
		CMDIChildWnd* pChild = MDIGetActive(); // Get the active child window.
		if (pChild)
		{
			// Send a WM_HSCROLL message to the active child window.
			pChild->GetActiveView()->SendMessage(WM_HSCROLL, 0, (LPARAM)m_wndSlider.GetSafeHwnd());
		}
	}

	// Let the base class handle the message.
	CMDIFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrame::OnUpdateButtonDoc(CCmdUI* pCmdUI)
{
	// This is the Command UI handler function for the Static control.

	// Use the DYNAMIC_DOWNCAST macro to cast the CWnd* pCmdUI->m_pOther to a CXTPCommandBar* object.
	CXTPCommandBar* pCommandBar = DYNAMIC_DOWNCAST(CXTPCommandBar, pCmdUI->m_pOther);
	if (!pCommandBar)
		return;

	// Use the DYNAMIC_DOWNCAST macro to cast the CXTPControl* object to a CControlStatic* object.
	CControlStatic* pControl = DYNAMIC_DOWNCAST(CControlStatic, pCommandBar->GetControl(pCmdUI->m_nIndex));
	if (!pControl)
		return;

	// nCount is used to count the number of open child windows.
	int nCount = 0;

	// Get a pointer to the first child window.
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient)->GetWindow(GW_CHILD);
	while (pWnd)
	{
		nCount ++;
		pWnd = pWnd->GetWindow(GW_HWNDNEXT); // Returns the next window on the window manager's list.
	}

	// Create the string caption for the Static control
	CString strCaption;
	strCaption.Format(_T("Opened Documents: %i"), nCount);

	// Set the caption on the Static control
	pControl->SetCaption(strCaption);

	// Enable the Static control if the number of child windows is greater than 0.
	pCmdUI->Enable(nCount > 0);
}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;

	// Instantiate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);

	// Add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);

	// Add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_COMMONCTYPE);

	// Initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_COMMONCTYPE);
	pPageCommands->InsertNewMenuCategory();

	// Display the customize dialog.
	dlg.DoModal();
}

///////////////////////////////////////////////////////////////////
// CControlStatic

IMPLEMENT_XTP_CONTROL(CControlStatic, CXTPControl)

CSize CControlStatic::GetSize(CDC* pDC)
{
	// This function is used to determine the size
	// of the bounding rectangle of the text.  The
	// function returns the size of the rectangle in
	// a CSize object.

	// Determine if the toolbar is docked horizontally or vertically.
	BOOL bVert = GetParent()->GetPosition() == xtpBarRight || GetParent()->GetPosition() == xtpBarLeft;

	// Instantiate an empty CRect object to hold the
	// size of the bounding rectangle of the text.
	CXTPEmptyRect rcText;

	// Function declaration for the DrawTextEx function:
	//  CSize DrawTextEx(
	//      CDC*    pDC,                A pointer to the current device context.
	//      CString str,                A CString object that contains the text to draw.
	//      CRect   rcText,             A CRect object that is used to hold the size of the bounding rectangle of the text.
	//      BOOL    bDraw,              If TRUE, then the text is drawn.  If FALSE, then the function returns the size of the bounding rectangle of the text.
	//      BOOL    bVert,              TRUE if the toolbar is docked in a vertical position.
	//      BOOL    bCentered,          TRUE if the text is drawn centered.
	//      BOOL    bTriangled,         If TRUE, then an arrow is drawn at the end of the text.
	//      BOOL    bEmbosssed = FALSE, TRUE to draw the text embossed.
	//  );

	// Do not draw the text, just get the size of the bounding rectangle of the text.
	CSize sz = GetPaintManager()->DrawTextEx(pDC, GetCaption(), &rcText, FALSE, bVert, FALSE, FALSE);

	// If the toolbar is docked in a vertical position,
	// then the minimum width of the bounding rectangle is 22.
	// The height of the bounding rectangle is the length of the text.
	if (bVert)
		return CSize(max(22, sz.cx), sz.cy);

	// If the toolbar is drawn in a horizontal position, then
	// the length of the bounding rectangle is the length of the text.
	// The height of the bounding rectangle is a minimum of 22.
	return CSize(sz.cx, max(22, sz.cy));
}

void CControlStatic::Draw(CDC* pDC)
{
	// Determine if the toolbar is docked horizontally or vertically.
	BOOL bVert = GetParent()->GetPosition() == xtpBarRight || GetParent()->GetPosition() == xtpBarLeft;

	// Get the bounding rectangle of the control.
	CRect rcText = GetRect();

	// Function declaration for DrawControlEntry:
	// DrawControlEntry(
	//      CDC*            pDC,         Pointer to a valid device context
	//      CRect           rc,          Rectangle to draw.
	//      BOOL            bEnabled,    TRUE if the control is enabled.
	//      BOOL            bSelected,   TRUE if the control is selected.
	//      BOOL            bPressed,    TRUE if the control is pushed.
	//      BOOL            bChecked,    TRUE if the control is checked.
	//      BOOL            bPopuped,    TRUE if the control is popuped.
	//      XTPBarPosition  barPosition, Parent's bar position.
	//  );
	// This method is called to fill the control's face.
	GetPaintManager()->DrawControlEntry(pDC, rcText, GetEnabled(), FALSE, FALSE, FALSE, FALSE, GetParent()->GetPosition());

	// Function declaration for the DrawTextEx function:
	//  CSize DrawTextEx(
	//      CDC*    pDC,                A pointer to the current device context.
	//      CString str,                A CString object that contains the text to draw.
	//      CRect   rcText,             A CRect object that is used to hold the size of the bounding rectangle of the text.
	//      BOOL    bDraw,              If TRUE, then the text is drawn.  If FALSE, then the function returns the size of the bounding rectangle of the text.
	//      BOOL    bVert,              TRUE if the toolbar is docked in a vertical position.
	//      BOOL    bCentered,          TRUE if the text is drawn centered.
	//      BOOL    bTriangled,         If TRUE, then an arrow is drawn at the end of the text.
	//      BOOL    bEmbosssed = FALSE, TRUE to draw the text embossed.
	//  );
	// GetEnabled() returns TRUE if the control is enabled; otherwise FALSE.
	GetPaintManager()->DrawTextEx(pDC, GetCaption(), &rcText, TRUE, bVert, TRUE, FALSE, !GetEnabled());
}

///////////////////////////////////////////////////////////////////
// CButtonOptions

BEGIN_MESSAGE_MAP(CButtonOptions, CButton)
	//{{AFX_MSG_MAP(CButtonOptions)
	ON_CONTROL_REFLECT(BN_CLICKED, OnOptionShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CButtonOptions::OnOptionShow()
{
	// This function sets the vertical draw behavior for the Option
	// buttons on the Options toolbar if the toolbar is docked vertically.
	// The three possible options are to hide the control, show the control,
	// or to draw the control as a button.  These options are defined as an
	// enum in the Extreme Tool Kit as follows:
	//
	//  enum XTPControlCustomVerticalOptions
	//  {
	//      xtpVerticalHide,  // To hide control if parent toolbar vertically docked.
	//      xtpVerticalShow,  // To show control if parent toolbar vertically docked.
	//      xtpVerticalButton // To show control as a button.
	//  };

	// Get a pointer to the CommandBar.
	CXTPCommandBar* pCommandBar = DYNAMIC_DOWNCAST(CXTPCommandBar, GetParent());
	if (pCommandBar)
	{
		// Get the ID of the currently selected Options button on the Options toolbar.
		int nID = ::GetDlgCtrlID(m_hWnd);

		// Loop through all of the controls on the Options CommnadBar.
		for (int i = 0; i < pCommandBar->GetControls()->GetCount(); i++)
		{
			// Use the DYNAMIC_DOWNCAST macro to cast the CButtonOptions control to a CXTPControlCustom control.
			CXTPControlCustom* pControl = DYNAMIC_DOWNCAST(CXTPControlCustom, pCommandBar->GetControl(i));
			if (pControl)
			{
				// Set the control's vertical draw style while the toolbar is docked vertically.
				switch(nID)
				{
				case IDC_OPTION_HIDE:  // Option "Hide Vertically" button is selected.
					pControl->SetVerticalPositionOption(xtpVerticalHide);
					break;
				case IDC_OPTION_SHOW: // Options "Show Vertically" button is selected.
					pControl->SetVerticalPositionOption(xtpVerticalShow);
					break;
				case IDC_OPTION_BUTTON: // Options "Show as Button" button is selected.
					pControl->SetVerticalPositionOption(xtpVerticalButton);
				};
			}
		}
	}
}
