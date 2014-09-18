// XTMemFile.cpp : implementation of the CXTMemFile class.
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

#include "StdAfx.h"
#include "XTMemFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTMemFile
/////////////////////////////////////////////////////////////////////////////
CXTMemFile::CXTMemFile( UINT nGrowBytes)
	: CMemFile ( nGrowBytes )
{
	// Modified default constructor
	m_File.m_hFile = NULL;
	m_bOpen = false;
	m_uiOpenFlags = 0;
}

CXTMemFile::CXTMemFile( BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes )
	: CMemFile ( lpBuffer, nBufferSize, nGrowBytes )
{
	// Modified default constructor
	m_File.m_hFile = NULL;
	m_bOpen = false;
	m_uiOpenFlags = 0;
}

CXTMemFile::CXTMemFile(LPCTSTR lpszFilename, UINT uiOpenFlags)
{
	// CFile compatible constructor
	m_File.m_hFile = NULL;
	m_bOpen = false;
	m_uiOpenFlags = uiOpenFlags;
	Open(lpszFilename,uiOpenFlags);
}

CXTMemFile::~CXTMemFile()
{
	if (m_bOpen)
		Close();
	m_File.m_hFile = NULL;
	m_uiOpenFlags = 0;
}

// Implementation

BOOL CXTMemFile::ReadString(CString& rString)
{
	// Readstring Beta Version

	rString.Empty();
	char ch;

	bool bFound = false;
	while (GetPosition() < GetLength())
	{
		Read(&ch,1);

		bFound = (ch == '\x0D');
		if (bFound || (ch == '\x0'))
			break;
		else
			rString = rString + ch;
	}
	if (bFound)
	{
		Read(&ch,1);
		if (ch != '\x0A')
			Seek(-1,current);
	}

	if (rString.GetLength() > 0)
		return true;
	return false;
}

void CXTMemFile::WriteString( LPCTSTR lpsz )
{
	// WriteString Beta Version

	if (_tcslen(lpsz) > 0)
	{
		UINT iCount;
		for (iCount = 0; iCount < _tcslen(lpsz); iCount++)
			if ((lpsz[iCount] != '\x0D') && (lpsz[iCount] != '\x00'))
				Write(&lpsz[iCount],1);
	}
	char ch = '\x0D';
	Write(&ch,1);
	ch = '\x0A';
	Write(&ch,1);

}

bool CXTMemFile::Duplicate(CFile *fDuplicate)
{
	// Standard Copy Routine

	BYTE *pbData = new BYTE[1024];
	DWORD dwPos = (DWORD)GetPosition();
	SeekToBegin();
	fDuplicate->SetLength(0);
	while (GetPosition() < GetLength())
	{
		DWORD dwRead = Read(pbData,1024);
		fDuplicate->Write(pbData,dwRead);
	}
	Seek(dwPos,fDuplicate->begin);
	delete pbData;
	return true;
}

bool CXTMemFile::Duplicate(CString strDup)
{
	// Duplicate With CString compatibility

	CFile file(strDup,CFile::modeCreate | CFile::modeWrite);
	bool bResult = Duplicate(&file);
	file.Close();
	return bResult;
}

bool CXTMemFile::Open(CString strFilename, UINT uiOpenFlags,CFileException *pError)
{
	// CFile compatible Open

	if (m_File.m_hFile != NULL)
		return false;
	if (m_File.Open(strFilename,uiOpenFlags,pError))
	{
		m_uiOpenFlags = uiOpenFlags;
		Load();
		m_bOpen = true;
		return true;
	}
	else
		return false;
}

bool CXTMemFile::Save()
{
	if (m_File.m_hFile == NULL)
		return false;
	CString str;

	// Check if the physical file has write access
	if (!(m_uiOpenFlags & modeWrite) && !(m_uiOpenFlags & modeReadWrite))
	{
		return false;
	}

	DWORD dwPos = (DWORD)GetPosition();
	DWORD dwLength = 1024;
	BYTE *pData = new BYTE[dwLength];
	if (pData == NULL)
		return false;
	SeekToBegin();
	m_File.SetLength(0);
	while (GetPosition() < GetLength())
	{
		DWORD dwRead = Read(pData,dwLength);
		m_File.Write(pData,dwRead);
	}
	Seek(dwPos,begin);
	delete pData;
	m_File.Flush();
	return true;
}

bool CXTMemFile::Load()
{
	if (m_File.m_hFile == NULL)
		return false;

	DWORD dwLength = 1024;
	BYTE *pData = new BYTE[dwLength];
	if (pData == NULL)
		return false;
	SeekToBegin();
	SetLength(0);
	m_File.SeekToBegin();
	while (m_File.GetPosition() < m_File.GetLength())
	{
		DWORD dwRead = m_File.Read(pData,dwLength);
		Write(pData,dwRead);
	}

	delete pData;
	return true;
}

void CXTMemFile::Close()
{
	if ((m_File.m_hFile != NULL) && m_bOpen)
	{
		m_bOpen = false;
		Save();
		m_File.Close();
		m_File.m_hFile = NULL;
	}
}

void CXTMemFile::Flush()
{
	CMemFile::Flush();
	Save();
}

bool CXTMemFile::Discard()
{
	return Load();
}

bool CXTMemFile::Import(CFile *fImp)
{
	if ((fImp == NULL) || (fImp->m_hFile == NULL))
		return false;

	DWORD dwLength = 1024;
	BYTE *pData = new BYTE[dwLength];
	if (pData == NULL)
		return false;
	SeekToBegin();
	SetLength(0);
	fImp->SeekToBegin();
	while (fImp->GetPosition() < fImp->GetLength())
	{
		DWORD dwRead = fImp->Read(pData,dwLength);
		Write(pData,dwRead);
	}

	delete pData;
	return true;
}

bool CXTMemFile::Append(CFile *fApp)
{
	if ((fApp == NULL) || (fApp->m_hFile == NULL))
		return false;

	DWORD dwLength = 1024;
	BYTE *pData = new BYTE[dwLength];
	if (pData == NULL)
		return false;
	SeekToEnd();
	fApp->SeekToBegin();
	while (fApp->GetPosition() < fApp->GetLength())
	{
		DWORD dwRead = fApp->Read(pData,dwLength);
		Write(pData,dwRead);
	}

	delete pData;
	return true;
}

DWORD CXTMemFile::Insert(CFile *fSrc, DWORD dwSourcePos, DWORD dwDestPos, DWORD dwBytes)
{
	if (dwBytes == 0)
		return 0;

	CXTMemFile fPreIns;
	CXTMemFile fIns;
	CXTMemFile fPostIns;
	CXTMemFile fComplete;

	Extract(&fPreIns,0,dwDestPos); // Extract Data BEFORE insertion point
	Extract(&fPostIns,dwDestPos, (DWORD)((DWORD)GetLength() - dwDestPos)); // Extract Data AFTER intertion point

	// Extract the "to be inserted" Data from the (fSrc) Source file
	{
		DWORD dwLength = 1024;
		DWORD dwCounter = 0;
		BYTE *pData = new BYTE[dwLength];
		if (pData == NULL)
			return 0;
		fSrc->Seek(dwSourcePos,begin);

		while (((fSrc->GetPosition() - dwSourcePos) < dwBytes) && (fSrc->GetPosition() < fSrc->GetLength()) )
		{
			DWORD dwRead = fSrc->Read(pData,dwLength);
			if (dwCounter + dwRead > dwSourcePos + dwBytes)
				dwRead = dwBytes - dwCounter;
			fIns.Write(pData,dwRead);
			dwCounter += dwRead;
		}
		delete pData;
	} // Finished Extraction


	fComplete += &fPreIns; // Append BEFORE Data
	fComplete += &fIns; // Append the "to be inserted" Data
	fComplete += &fPostIns;  // Append the AFTER Data

	*this = &fComplete; // Copy completed File

	return (DWORD)fIns.GetLength();
}

DWORD CXTMemFile::Insert(CString strSrc, DWORD dwSourcePos, DWORD dwDestPos, DWORD dwBytes)
{
	CFile file(strSrc,CFile::modeRead);
	DWORD dwResult = Insert(&file, dwSourcePos, dwDestPos, dwBytes);
	file.Close();
	return dwResult;
}

DWORD CXTMemFile::Extract(CFile *fDest, DWORD dwStartPos, DWORD dwBytes)
{
	if (dwBytes == 0)
		return 0;
	DWORD dwPos = (DWORD)GetPosition();
	DWORD dwLength = 1024;
	DWORD dwCounter = 0;
	BYTE *pData = new BYTE[dwLength];
	if (pData == NULL)
		return 0;
	Seek(dwStartPos,begin);

	while (((GetPosition() - dwStartPos) < dwBytes) && (GetPosition() < GetLength()) )
	{
		DWORD dwRead = Read(pData,dwLength);
		if (dwCounter + dwRead > dwStartPos + dwBytes)
			dwRead = dwBytes - dwCounter;
		fDest->Write(pData,dwRead);
		dwCounter += dwRead;
	}
	Seek(dwPos,begin);
	delete pData;
	fDest->Flush();
	return dwCounter;

}

DWORD CXTMemFile::Extract(CString strDest, DWORD dwStartPos, DWORD dwBytes)
{
	CFile file(strDest,CFile::modeCreate | CFile::modeWrite);
	DWORD dwResult = Extract(&file,dwStartPos,dwBytes);
	file.Close();
	return dwResult;
}

// Overloaded Operators

void CXTMemFile::operator =(CFile *fDup)
{
	Import(fDup);
}

void CXTMemFile::operator =(CString strDup)
{
	CFile file;
	if (file.Open(strDup,CFile::modeRead))
	{
		Import(&file);
		file.Close();
	}
}

void CXTMemFile::operator =(DWORD dwFilePos)
{
	Seek(dwFilePos,begin);
}

void CXTMemFile::operator +=(CFile *fApp)
{
	Append(fApp);
}

void CXTMemFile::operator +=(CString strApp)
{
	CFile file;
	if (file.Open(strApp,CFile::modeRead))
	{
		Append(&file);
		file.Close();
	}
}

// Indexing Operators

BYTE CXTMemFile::operator [](DWORD dwFilePos)
{
	if (dwFilePos > GetLength() -1)
		return 0;

	BYTE bTemp;
	Seek(dwFilePos,begin);
	Read(&bTemp,sizeof(BYTE));
	return bTemp;
}

LONG CXTMemFile::FindData(void* pData, DWORD dwDataLen, LONG lStartPos)
{
	BYTE* pBytes = (BYTE*)pData;


	Seek(lStartPos,begin);

	bool bFoundAll = false;
	LONG lPos = -1;

	while ((!bFoundAll) && (GetPosition() < GetLength()))
	{
		bool bFoundFirst = false;

		while ((!bFoundFirst) && (GetPosition() < GetLength()))
		{
			lPos = (LONG)GetPosition();
			BYTE read;
			Read(&read,1);
			if (read != pBytes[0])
				continue;

			bFoundFirst = true;
			break;
		}

		//Hier Die Restlichen Bytes (dwDataLen -1) vergleichen

		DWORD i = 1;
		bool bFound = true;

		while ((i < dwDataLen) && (GetPosition() < GetLength()) && bFoundFirst)
		{
			BYTE read;
			Read(&read,1);
			if (read == pBytes[i++])
				continue;

			bFound = false;
			Seek(lPos+1,begin);
			break;
		}

		bFoundAll = bFound && bFoundFirst;
	}

	lPos = bFoundAll ? lPos : -1;
//  while (bFound && (iPos < strSeek.GetLength())
//  {
//  }
	return lPos;
}
