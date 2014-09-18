// DialogResourceSymbols.cpp : implementation file
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
#include "commandbarsdesigner.h"
#include "EmbeddedFrame.h"

#include "DialogResourceSymbols.h"
#include "DialogNewSymbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CDialogResourceSymbolsList::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	if (lpCompareItemStruct->itemData1 == NULL || lpCompareItemStruct->itemData2 == NULL)
		return 0;

	CResourceInfo* pInfo1 = (CResourceInfo*)lpCompareItemStruct->itemData1;
	CResourceInfo* pInfo2 = (CResourceInfo*)lpCompareItemStruct->itemData2;

	return pInfo1->m_strCaption.Compare(pInfo2->m_strCaption);
}

void CDialogResourceSymbolsList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rcItem = lpDrawItemStruct->rcItem;
	CResourceInfo* pInfo = (CResourceInfo*)lpDrawItemStruct->itemData;

	if (!pInfo)
		return;

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	// If this item is selected, set the background color
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		dc.FillSolidRect(rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);


	CString str = pInfo->m_strCaption;


	CRect rcCaption(rcItem);
	rcCaption.right = m_nCaptionWidth;
	dc.DrawText(str, rcCaption, DT_SINGLELINE|DT_VCENTER);


	CString strID;
	strID.Format(_T("%i"), pInfo->m_nID);

	CRect rcID(rcItem);
	rcID.left = m_nCaptionWidth;
	dc.DrawText(strID, rcID, DT_SINGLELINE|DT_VCENTER);

	if (pInfo->m_bInUse)
	{
		CRect rcUsed(rcItem);
		rcUsed.left = m_nUsedWidth;
		dc.DrawText(_T("Yes"), rcUsed, DT_SINGLELINE|DT_VCENTER);
	}


	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();

}


/////////////////////////////////////////////////////////////////////////////
// CDialogResourceSymbols dialog


CDialogResourceSymbols::CDialogResourceSymbols(CEmbeddedFrame* pFrame, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogResourceSymbols::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogResourceSymbols)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFrame = pFrame;
}


void CDialogResourceSymbols::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogResourceSymbols)
	DDX_Control(pDX, IDC_LIST_USEDBY, m_wndListUsage);
	DDX_Control(pDX, IDC_LIST_IDS, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogResourceSymbols, CDialog)
	//{{AFX_MSG_MAP(CDialogResourceSymbols)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_IDS, OnSelchangeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogResourceSymbols message handlers

void CDialogResourceSymbols::RefreshMainList()
{
	m_wndList.ResetContent();

	CMapResources* pResources = m_pFrame->ResourceManager()->GetResources();

	POSITION pos = pResources->GetStartPosition();
	while (pos)
	{
		CResourceInfo* pInfo;
		CString strCaption;
		pResources->GetNextAssoc(pos, strCaption, (CObject*&)pInfo);
		pInfo->m_strCaption = strCaption;
		pInfo->m_bInUse = FillUsageList(pInfo->m_nID, TRUE);

		m_wndList.AddString((LPCTSTR)pInfo);
	}

	OnSelchangeList();
}

BOOL CDialogResourceSymbols::OnInitDialog()
{
	CDialog::OnInitDialog();

	CXTPWindowRect rcName(GetDlgItem(IDC_STATIC_NAME));
	CXTPWindowRect rcID(GetDlgItem(IDC_STATIC_VALUE));
	CXTPWindowRect rcUsed(GetDlgItem(IDC_STATIC_USED));

	m_wndList.m_nCaptionWidth = rcID.left - rcName.left;
	m_wndList.m_nUsedWidth = rcUsed.left - rcName.left;

	RefreshMainList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogResourceSymbols::OnButtonImport()
{
	CFileDialog fd(FALSE, _T(".bas"), _T("Resource.bas"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Visual Basic Module (*.bas)|*.bas|Visual C++ Defenition (*.h)|*.h|All Files (*.*)|*.*||"));
	if (fd.DoModal() == IDOK)
	{
		CStdioFile file(fd.GetFileName(), CFile::modeCreate|CFile::modeWrite);

		if (fd.GetFileName().Right(2).CompareNoCase(_T(".h"))== 0)
		{
			for (int i = 0; i < m_wndList.GetCount(); i++)
			{
				CResourceInfo* pInfo = (CResourceInfo*)m_wndList.GetItemDataPtr(i);

				CString strLine;
				strLine.Format(_T("#define %s\t\t\t%i\n"), pInfo->m_strCaption, pInfo->m_nID);
				file.WriteString(strLine);

			}

		} else
		{

			file.WriteString(_T("Attribute VB_Name = \"Resource\"\n"));

			for (int i = 0; i < m_wndList.GetCount(); i++)
			{
				CResourceInfo* pInfo = (CResourceInfo*)m_wndList.GetItemDataPtr(i);

				CString strLine;
				strLine.Format(_T("Public Const %s = %i\n"), pInfo->m_strCaption, pInfo->m_nID);

				file.WriteString(strLine);

			}
		}
	}
}

void CDialogResourceSymbols::OnButtonNew()
{
	CDialogNewSymbol ns;
	ns.m_pResourceManager = m_pFrame->ResourceManager();

	if (ns.DoModal() == IDOK)
	{
		CMapResources* pMapResources = m_pFrame->ResourceManager()->GetResources();

		CResourceInfo* pInfo;
		CString strCaption;

		if (!pMapResources->Lookup(ns.m_strName, (CObject*&)pInfo) && m_pFrame->ResourceManager()->Find(ns.m_nValue) == NULL)
		{
			pMapResources->SetAt(ns.m_strName, new CResourceInfo(ns.m_nValue));

			if (ns.m_nValue == m_pFrame->ResourceManager()->m_nNextID)
				m_pFrame->ResourceManager()->m_nNextID++;

			RefreshMainList();
		}
	}
}

void CDialogResourceSymbols::OnButtonDelete()
{
	int nSel = m_wndList.GetCurSel();
	if (nSel != LB_ERR)
	{
		CResourceInfo* pInfo = (CResourceInfo*)m_wndList.GetItemDataPtr(nSel);

		if (!pInfo)
			return;

		m_pFrame->ResourceManager()->m_mapResources.RemoveKey(pInfo->m_strCaption);
		delete pInfo;

		RefreshMainList();
	}

}

BOOL CheckCommandBar(UINT nID, CXTPCommandBar* pCommandBar);

BOOL CheckControl(UINT nID, CXTPControl* pControl)
{
	if (pControl->GetID() == (int)nID)
		return TRUE;

	if (pControl->GetIconId() == (int)nID)
		return TRUE;

	CXTPCommandBar* pChildBar = pControl->GetCommandBar();
	if (pChildBar)
	{
		BOOL bFound = CheckCommandBar(nID, pChildBar);
		if (bFound)
			return TRUE;
	}
	return FALSE;
}

BOOL CheckCommandBar(UINT nID, CXTPCommandBar* pCommandBar)
{
	for (int i = 0; i < pCommandBar->GetControls()->GetCount(); i++)
	{
		CXTPControl* pControl = pCommandBar->GetControl(i);
		if (CheckControl(nID, pControl))
			return TRUE;
	}


	if (pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPPopupBar)))
	{
		CXTPPopupBar* pPopupBar = (CXTPPopupBar*)pCommandBar;
		CString strCaption; int nWidth; UINT nIDTearOff;
		if (pPopupBar->IsTearOffPopup(strCaption, nIDTearOff, nWidth) && (nID == nIDTearOff))
		{
			return TRUE;
		}
	}


	return FALSE;
}




BOOL CheckCommandBars(UINT nID, CXTPCommandBars* pCommandBars, CListBox* m_pList)
{
	BOOL bResult = FALSE;

	for (int i = 0; i < pCommandBars->GetCount(); i++)
	{
		CXTPCommandBar* pCommandBar = pCommandBars->GetAt(i);

		if (pCommandBar->GetBarID() == nID || CheckCommandBar(nID, pCommandBar))
		{
			if (!m_pList)
				return TRUE;

			bResult = TRUE;

			CString str;
			if (pCommandBar->GetType() == xtpBarTypeMenuBar)
				str = _T("Menu bar");
			else
				str = _T("ToolBar ") +  pCommandBar->GetTitle();

			m_pList->AddString(str);
		}

	}

	return bResult;
}
BOOL CheckControls(UINT nID, CXTPControls* pControls, CListBox* m_pList)
{
	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		if (CheckControl(nID, pControl))
		{
			if (!m_pList)
				return TRUE;

			m_pList->AddString(_T("Controls pane"));

			return TRUE;
		}

	}

	return FALSE;
}

BOOL CheckIcon(UINT nID, CXTPCommandBars* pCommandBars, CListBox* m_pList)
{

	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pIcons =
		pCommandBars->GetImageManager()->GetImages();

	UINT nIDCommand;
	CXTPImageManagerIconSet* pIconSet;

	POSITION pos = pIcons->GetStartPosition();
	while (pos)
	{
		pIcons->GetNextAssoc(pos, nIDCommand, pIconSet);

		if (pIconSet && nID == nIDCommand)
		{
			if (!m_pList)
				return TRUE;
			m_pList->AddString(_T("Icon"));

			return TRUE;

		}
	}

	return FALSE;
}

BOOL CheckAccels(UINT nID, HACCEL& hAccelTable, CListBox* m_pList)
{

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	if (nAccelSize <= 0)
		return FALSE;

	BOOL bResult = FALSE;

	ACCEL* lpAccel = new ACCEL [nAccelSize];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	for (int i = 0; i < nAccelSize; i ++)
	{
		if (lpAccel [i].cmd == nID)
		{
			if (m_pList)
				m_pList->AddString(_T("Accelerator"));

			bResult = TRUE;
			break;
		}
	}

	delete[] lpAccel;
	return bResult;
}

BOOL CDialogResourceSymbols::FillUsageList(UINT nID, BOOL bCheckOnly)
{
	CListBox* pListBox = bCheckOnly? NULL: &m_wndListUsage;

	if (CheckCommandBars(nID, m_pFrame->GetCommandBars(), pListBox) && bCheckOnly)
		return TRUE;

	if (CheckControls(nID, m_pFrame->m_pControls, pListBox) && bCheckOnly)
		return TRUE;

	if (CheckIcon(nID, m_pFrame->GetCommandBars(), pListBox) && bCheckOnly)
		return TRUE;

	if (CheckAccels(nID, m_pFrame->m_hAccelTable, pListBox) && bCheckOnly)
		return TRUE;

	return FALSE;
}

void CDialogResourceSymbols::OnSelchangeList()
{
	m_wndListUsage.ResetContent();

	int nSel = m_wndList.GetCurSel();

	if (nSel != LB_ERR)
	{
		CResourceInfo* pInfo = (CResourceInfo*)m_wndList.GetItemDataPtr(nSel);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(!pInfo->m_bInUse);

		FillUsageList(pInfo->m_nID, FALSE);



	}
	else
	{
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}

}
