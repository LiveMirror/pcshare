// DialogBitmapExport.cpp : implementation file
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
#include "resource.h"
#include "DialogBitmapExport.h"
#include "ResourceManager.h"


#define BFT_BITMAP 0x4d42   /* 'BM' */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPImageManagerIconHandle& CImage::GetHandle()
{
	XTPImageState state = imageState == -1? pParent->m_imageState: imageState;

	return state == xtpImageNormal? pIcon->GetIcon(): state == xtpImageHot? pIcon->GetHotIcon():
	state == xtpImageChecked? pIcon->GetCheckedIcon(): pIcon->GetDisabledIcon();
}


LRESULT CComboBoxMask::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_KILLFOCUS:
		{
			LRESULT lResult = CComboBox::WindowProc(message, wParam, lParam);
			Invalidate(FALSE);
			return lResult;
		}

	case WM_PAINT:
		{
			CPaintDC dc(this);

			// Get the client rect, and paint to a memory device context.
			// This will help reduce screen flicker.  Pass the memory
			// device context to the default window procedure to do
			// default painting.

			CRect r;
			GetClientRect(&r);
			CXTPBufferDC memDC(dc, r);
			memDC.FillSolidRect(r, GetSysColor(COLOR_WINDOW));

			CComboBox::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

			if (IsWindowEnabled())
			{
				r.DeflateRect(3, 3, 3 + ::GetSystemMetrics(SM_CXHTHUMB), 3);

				COLORREF clr = ((CDialogBitmapImagesHost*)GetParent())->m_clrMask;
				memDC.FillSolidRect(r, clr);
			}
			return TRUE;
		}
	}

	return CComboBox::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapExport dialog


CDialogBitmapExport::CDialogBitmapExport(CXTPImageManager* pImageManager, CResourceManager* pResourceManager)
	: CDialogBitmapImagesHost(CDialogBitmapExport::IDD, pResourceManager)
	, m_pImageManager(pImageManager)
{
	//{{AFX_DATA_INIT(CDialogBitmapExport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CDialogBitmapExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBitmapExport)
	DDX_Control(pDX, IDC_COMBO_MASK, m_comboMask);
	DDX_Control(pDX, IDC_COMBO_STATE, m_comboState);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_comboDevice);
	DDX_Control(pDX, IDC_COMBO_ID, m_comboId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBitmapExport, CDialog)
	//{{AFX_MSG_MAP(CDialogBitmapExport)
	ON_CBN_SELCHANGE(IDC_COMBO_MASK, OnSelchangeComboMask)
	ON_CBN_DBLCLK(IDC_COMBO_MASK, OnDblclkComboMask)
	ON_CBN_SELCHANGE(IDC_COMBO_STATE, OnStateChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, OnDeviceChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_ID, OnIdChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapExport message handlers

BOOL CDialogBitmapExport::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT(m_pImageManager);

	CXTPWindowRect rc(GetDlgItem(IDC_STATIC_IMAGES));
	ScreenToClient(&rc);

	m_wndImages.Create(AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW)), 0, WS_CHILD|WS_VISIBLE, rc, this, 0);


	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pImages = m_pImageManager->GetImages();

	UINT nID, nWidth;
	CXTPImageManagerIconSet* pIconSet;
	CXTPImageManagerIcon* pIcon;

	POSITION pos = pImages->GetStartPosition();
	while (pos)
	{
		pImages->GetNextAssoc(pos, nID, pIconSet);
		CXTPImageManagerIconSet::CIconSetMap* pIconSetMap = pIconSet->GetIcons();

		POSITION posIconSet  = pIconSetMap->GetStartPosition();
		while (posIconSet)
		{
			pIconSetMap->GetNextAssoc(posIconSet, nWidth, pIcon);
			if (pIcon->IsScaled())
				continue;

			CString strDevice;
			strDevice.Format(_T("%ix%i"), pIcon->GetWidth(), pIcon->GetHeight());

			if (m_comboDevice.FindStringExact(0, strDevice) == -1)
			{
				int nIndex = m_comboDevice.AddString(strDevice);
				m_comboDevice.SetItemData(nIndex, MAKELONG(pIcon->GetWidth(), pIcon->GetHeight()));
			}
		}
	}

	m_comboMask.AddString(_T("Select Color..."));
	m_comboDevice.SetCurSel(0);
	OnDeviceChanged();

	m_comboDevice.EnableWindow(m_comboDevice.GetCount() > 1);


	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogBitmapExport::OnUpdateIcons()
{
	m_arrImages.RemoveAll();
	m_comboId.ResetContent();
	int nIndex = m_comboDevice.GetCurSel();

	if (nIndex == CB_ERR)
		return;

	DWORD dwDevice = (DWORD)m_comboDevice.GetItemData(nIndex);
	CSize sz(dwDevice);

	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pImages = m_pImageManager->GetImages();

	UINT nID, nWidth;
	CXTPImageManagerIconSet* pIconSet;
	CXTPImageManagerIcon* pIcon;
	POSITION pos = pImages->GetStartPosition();
	while (pos)
	{
		pImages->GetNextAssoc(pos, nID, pIconSet);
		CXTPImageManagerIconSet::CIconSetMap* pIconSetMap = pIconSet->GetIcons();
		BOOL bFound = FALSE;

		POSITION posIconSet  = pIconSetMap->GetStartPosition();
		while (posIconSet)
		{
			pIconSetMap->GetNextAssoc(posIconSet, nWidth, pIcon);
			if (pIcon->IsScaled())
				continue;

			if (pIcon->GetWidth() != sz.cx || pIcon->GetHeight() != sz.cy)
				continue;

			CImage image;
			image.nID = pIcon->GetID();
			image.imageState = (XTPImageState)-1;
			image.pIcon = pIcon;
			image.pParent = this;

			int nIndex = 0;
			for (; nIndex < m_arrImages.GetSize(); nIndex ++) if (m_arrImages[nIndex].nID > image.nID)
				break;
			m_arrImages.InsertAt(nIndex, image);
			bFound = TRUE;
		}

		if (bFound)
		{
			CString strResource = m_pResourceManager->GetStringID(nID);
			int nIndex = m_comboId.AddString(strResource);
			m_comboId.SetItemData(nIndex, nID);
		}
	}

	for (int i = 0; i < m_arrImages.GetSize(); i++)
		m_arrImages[i].nIndex = i;

	m_pSelected = 0;
	m_szIcons = sz;
	m_wndImages.OnImageCountChanged();
}

void CDialogBitmapExport::OnImageChanged(CImage* pImage)
{
	m_pSelected = pImage;

	m_comboState.ResetContent();

	if (pImage != NULL)
	{
		m_comboState.AddString(_T("(Default)"));
	}

	m_comboState.AddString(_T("xtpImageNormal"));
	m_comboState.AddString(_T("xtpImageDisabled"));
	m_comboState.AddString(_T("xtpImageHot"));
	m_comboState.AddString(_T("xtpImageChecked"));

	if (pImage != NULL)
	{
		m_comboState.SetCurSel(m_pSelected->imageState + 1);
	}
	else
	{
		m_comboState.SetCurSel(m_imageState);
	}

	m_comboId.EnableWindow(pImage != NULL);
	m_wndImages.Invalidate(FALSE);

	if (pImage != NULL)
	{
		CString strResource = m_pResourceManager->GetStringID(pImage->nID);
		int nIndex = m_comboId.FindStringExact(0, strResource);
		m_comboId.SetCurSel(nIndex);
	}
	else
	{
		m_comboId.SetCurSel(-1);
	}
}



void CDialogBitmapExport::OnSelchangeComboMask()
{
	OnDblclkComboMask();

}

void CDialogBitmapExport::OnDblclkComboMask()
{
	CColorDialog cd(m_clrMask);

	if (cd.DoModal() == IDOK)
	{
		m_clrMask = cd.GetColor();
	}
	m_comboMask.SetCurSel(-1);

	m_comboMask.Invalidate(FALSE);
	m_wndImages.Invalidate(FALSE);
}

void CDialogBitmapExport::OnStateChanged()
{
	if (m_pSelected == 0)
	{
		m_imageState = (XTPImageState)m_comboState.GetCurSel();
	}
	else
	{
		m_pSelected->imageState = (XTPImageState)(m_comboState.GetCurSel() - 1);
	}
	m_wndImages.Invalidate(FALSE);
}

void CDialogBitmapExport::OnDeviceChanged()
{
	int nIndex = m_comboDevice.GetCurSel();

	if (nIndex == CB_ERR)
		return;

	m_pSelected = 0;
	OnUpdateIcons();
	OnImageChanged(NULL);
}

void CDialogBitmapExport::OnIdChanged()
{
	if (m_pSelected == 0)
		return;

	int nIndex = m_comboDevice.GetCurSel();

	if (nIndex == CB_ERR)
		return;

	DWORD dwDevice = (DWORD)m_comboDevice.GetItemData(nIndex);
	CSize sz(dwDevice);

	nIndex = m_comboId.GetCurSel();

	if (nIndex == CB_ERR)
		return;

	UINT nIDSelected = (UINT)m_comboId.GetItemData(nIndex);


	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pImages = m_pImageManager->GetImages();

	UINT nID, nWidth;
	CXTPImageManagerIconSet* pIconSet;
	CXTPImageManagerIcon* pIcon;
	POSITION pos = pImages->GetStartPosition();
	while (pos)
	{
		pImages->GetNextAssoc(pos, nID, pIconSet);
		CXTPImageManagerIconSet::CIconSetMap* pIconSetMap = pIconSet->GetIcons();

		if (pIconSet->GetID() != nIDSelected)
			continue;

		POSITION posIconSet  = pIconSetMap->GetStartPosition();
		while (posIconSet)
		{
			pIconSetMap->GetNextAssoc(posIconSet, nWidth, pIcon);
			if (pIcon->IsScaled())
				continue;

			if (pIcon->GetWidth() != sz.cx || pIcon->GetHeight() != sz.cy)
				continue;

			m_pSelected->nID = nIDSelected;
			m_pSelected->pIcon = pIcon;

			m_wndImages.Invalidate(FALSE);
			return;
		}
	}

}


BOOL CDialogBitmapExport::CreateBitmap(BITMAPINFO* pBitmapInfo, BYTE*& pDest, CBitmap& bmp)
{
	int nHeight = m_szIcons.cy;
	int nWidth = m_szIcons.cx * m_arrImages.GetSize();

	if (nWidth == 0)
		return FALSE;


	memset(pBitmapInfo, 0, sizeof(BITMAPINFOHEADER));

	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biHeight = nHeight;
	pBitmapInfo->bmiHeader.biWidth = nWidth;

	UINT nSize = pBitmapInfo->bmiHeader.biHeight * pBitmapInfo->bmiHeader.biWidth * 3;
	pBitmapInfo->bmiHeader.biBitCount = 24;
	pBitmapInfo->bmiHeader.biSizeImage = nSize;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biPlanes = 1;

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);


	HBITMAP hBmpResult = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	ASSERT(hBmpResult);
	if (!hBmpResult)
		return FALSE;

	HBITMAP hbmpOld = (HBITMAP)::SelectObject(dcSrc, hBmpResult);

	dcSrc.FillSolidRect(0, 0, nWidth, nHeight, m_clrMask);

	for (int i = 0; i < m_arrImages.GetSize(); i++)
	{
		CPoint pt(0 + i * m_szIcons.cx, 0);
		CXTPImageManagerIcon* pIcon = m_arrImages[i].pIcon;

		if (pIcon)
		{
			CXTPImageManagerIconHandle& pHandle = m_arrImages[i].GetHandle();
			pIcon->Draw(&dcSrc, pt, pHandle, m_szIcons);
		}
	}

	::SelectObject(dcSrc, hbmpOld);

	bmp.Attach(hBmpResult);

	return TRUE;

}


void CDialogBitmapExport::OnOK()
{
	CString strFilter = _T("Bitmap Files (*.bmp)|*.bmp|All files (*.*)|*.*||");

	CFileDialog fd(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
	if (fd.DoModal() != IDOK)
		return;


	BITMAPINFO  bmi;
	BYTE* pDest = NULL;
	CBitmap bmp;

	if (!CreateBitmap(&bmi, pDest, bmp))
		return;

	CFile fileResult(fd.GetPathName(), CFile::modeWrite|CFile::modeCreate);

	BITMAPFILEHEADER    hdr;

	hdr.bfType      = BFT_BITMAP;
	hdr.bfSize      = bmi.bmiHeader.biSizeImage + sizeof (BITMAPFILEHEADER);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + bmi.bmiHeader.biSize;

	fileResult.Write(&hdr, sizeof(BITMAPFILEHEADER));
	fileResult.Write(&bmi, sizeof(BITMAPINFOHEADER));
	fileResult.Write(pDest, bmi.bmiHeader.biSizeImage);

	fileResult.Close();


	CDialog::OnOK();
}
