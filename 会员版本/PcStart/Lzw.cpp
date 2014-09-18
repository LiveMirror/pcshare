#include "stdafx.h"
#include "Lzw.h"

//===================================================================
// 目标为8位色
void  FCLzw::Decode_WriteString_to8 (WORD Code)
{
	if (Code < m_LZW_CLEAR)
	{
		m_pCurOut[m_dwCurrPixel++] = (BYTE) m_pStrBegin[Code].wSuffix ;
		if (m_dwCurrPixel == m_dwDibWidth) // 写完一行, 切换值至下一行
			this->Decode_SwitchToFollowLine () ;
	}
	else
	{
		this->Decode_WriteString_to8 (m_pStrBegin[Code].wPrefix) ;
		this->Decode_WriteString_to8 (m_pStrBegin[Code].wSuffix) ;
	}
}
//===================================================================
void  FCLzw::LZW_GIF_Decode (BYTE * InBuffer, BYTE * DibBuffer, DWORD dwDibWidth, DWORD dwDibHeight, bool bInterlace)
{
	m_pStrBegin = new LZW_STRING [LZW_MAX_TABLE_SIZE + 32] ;
	if ((m_pStrBegin == NULL) || (InBuffer == NULL) || (DibBuffer == NULL))
		 return ;
	//	初始化设置-----------------------------------------+
	m_pCurIn = InBuffer + 1 ;
	m_pCurOut = DibBuffer ;
	m_byInBit = 0 ; // 输入从0开始, 输出为8位色位图
	m_byMinCode = *InBuffer ; // 获得最小码长度
	m_LZW_CLEAR = 1 << m_byMinCode ; // clear
	m_LZW_END = m_LZW_CLEAR + 1 ; // end

	m_dwDibWidth = dwDibWidth ; // DIB宽
	m_dwCurrPixel = 0 ;
	m_dwPitch = 4 * ((dwDibWidth * 8 + 31) / 32) ;

	m_byInterval = bInterlace ? 0 : 0xFF ; // 设置交错索引
	if (m_byInterval == 0) // 交错存储
	{
		m_pOrigin = m_pCurOut ;
		m_dwDibHeight = dwDibHeight ; // DIB高
		m_dwCurrHeight = 0 ; // 从最顶行开始
	}
	//	初始化设置完成-------------------------------------+

	WORD		Code = 0, Old = 0 ;
	this->Decode_InitStringTable () ;
	while ((Code = this->Decode_GetNextCode ()) != m_LZW_END)
	{
		if (Code == m_LZW_CLEAR)
		{
			this->Decode_InitStringTable () ;
			while ((Code = this->Decode_GetNextCode ()) == m_LZW_CLEAR) ;
		}
		else
			if (this->Decode_IsInTable (Code))
				this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Code)) ;
			else
				this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Old)) ;

		this->Decode_WriteString_to8 (Code) ;
		Old = Code ;
	}
	delete[] m_pStrBegin ;
}

BOOL FCLzw::PcUnZip(BYTE* pSrcData, BYTE** pDesData, DWORD* nUnZipLen)
{
	*nUnZipLen = *((DWORD*) &pSrcData[12]);
	*pDesData = new BYTE[*nUnZipLen];
	ZeroMemory(*pDesData, *nUnZipLen);
	LZW_Decode (pSrcData + 26 + (*(WORD *) &pSrcData[24]), *pDesData);
	return TRUE;
}