// XTColorRef.cpp: implementation of the CXTColorRef class.
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
#include <math.h>
#include "XTPColorManager.h"

#include "XTDefines.h"
#include "XTColorRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CXTColorRef CXTColorRef::BLACK  (0,   0,   0);
const CXTColorRef CXTColorRef::WHITE  (255, 255, 255);
const CXTColorRef CXTColorRef::GRAY_25(64,  64,  64);
const CXTColorRef CXTColorRef::GRAY_50(128, 128, 128);
const CXTColorRef CXTColorRef::GRAY_75(192, 192, 192);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CXTColorRef::toHSL(double& h, double& s, double& l) const
{
	double r = (double)getRValue()/255;
	double g = (double)getGValue()/255;
	double b = (double)getBValue()/255;

	double maxcolor = __max(r, __max(g, b));
	double mincolor = __min(r, __min(g, b));

	l = (maxcolor + mincolor)/2;

	if (maxcolor == mincolor)
	{
		h = 0;
		s = 0;
	}
	else
	{
		if (l < 0.5)
			s = (maxcolor-mincolor)/(maxcolor+mincolor);
		else
			s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

		if (r == maxcolor)
			h = (g-b)/(maxcolor-mincolor);
		else if (g == maxcolor)
			h = 2.0+(b-r)/(maxcolor-mincolor);
		else
			h = 4.0+(r-g)/(maxcolor-mincolor);

		h /= 6.0;
		if (h < 0.0) h += 1;
/*      h *= 60;
		if(h < 0.0) h += 360;*/
	}
}

void CXTColorRef::toHSB(double& h, double& s, double& b) const
{
	int red   = getRValue();
	int green = getGValue();
	int blue  = getBValue();

	int cmax = __max(red, __max(green, blue));
	int cmin = __min(red, __min(green, blue));

	b = ((double) cmax) / 255.0;
	if (cmax != 0)
		s = ((double) (cmax - cmin)) / ((double) cmax);
	else
		s = 0;
	if (s == 0)
		h = 0;
	else
	{
		double redc =   ((double) (cmax - red))   / ((double) (cmax - cmin));
		double greenc = ((double) (cmax - green)) / ((double) (cmax - cmin));
		double bluec =  ((double) (cmax - blue))  / ((double) (cmax - cmin));
		if (red == cmax)
			h = bluec - greenc;
		else if (green == cmax)
			h = 2.0 + redc - bluec;
		else
			h = 4.0 + greenc - redc;
		h /= 6.0;
		if (h < 0)
			h = h + 1.0;
	}
}

double CXTColorRef::HuetoRGB(double temp1, double temp2, double temp3)
{
	if( temp3 < 0 )
		temp3 = temp3+1.0;
	if( temp3 > 1 )
		temp3 = temp3-1.0;

	if( 6.0*temp3 < 1 )
		return (temp1+(temp2-temp1)*temp3*6.0);

	else if( 2.0*temp3 < 1 )
		return temp2;

	else if( 3.0*temp3 < 2.0 )
		return (temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0);

	return temp1;
}

void CXTColorRef::setHSL(double h, double s, double l)
{
	double r,g,b;
	double temp1, temp2;

	if (s == 0) {
		r = g = b = l;
	}
	else
	{
		if(l < 0.5)
			temp2 = l*(1.0+s);
		else
			temp2 = l+s-l*s;

		temp1 = 2.0*l-temp2;
//      h /= 360;

		r = HuetoRGB(temp1, temp2, h+1.0/3.0);
		g = HuetoRGB(temp1, temp2, h);
		b = HuetoRGB(temp1, temp2, h-1.0/3.0);
	}

	*this = RGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}

void CXTColorRef::setHSB(double hue, double saturation, double brightness)
{
	int r = 0, g = 0, b = 0;
	if (saturation == 0)
	{
		r = g = b = (int) (brightness * 255.0 + 0.5);
	}
	else
	{
		double h = (hue - floor(hue)) * 6.0;
		double f = h - floor(h);
		double p = brightness * (1.0 - saturation);
		double q = brightness * (1.0 - saturation * f);
		double t = brightness * (1.0 - (saturation * (1.0 - f)));
		switch ((int) h)
		{
			case 0:
				r = (int) (brightness * 255.0 + 0.5);
				g = (int) (t * 255.0 + 0.5);
				b = (int) (p * 255.0 + 0.5);
				break;
			case 1:
				r = (int) (q * 255.0 + 0.5);
				g = (int) (brightness * 255.0 + 0.5);
				b = (int) (p * 255.0 + 0.5);
				break;
			case 2:
				r = (int) (p * 255.0 + 0.5);
				g = (int) (brightness * 255.0 + 0.5);
				b = (int) (t * 255.0 + 0.5);
				break;
			case 3:
				r = (int) (p * 255.0 + 0.5);
				g = (int) (q * 255.0 + 0.5);
				b = (int) (brightness * 255.0 + 0.5);
				break;
			case 4:
				r = (int) (t * 255.0 + 0.5);
				g = (int) (p * 255.0 + 0.5);
				b = (int) (brightness * 255.0 + 0.5);
				break;
			case 5:
				r = (int) (brightness * 255.0 + 0.5);
				g = (int) (p * 255.0 + 0.5);
				b = (int) (q * 255.0 + 0.5);
				break;
		}
	}
	*this = RGB(r, g, b);
}

CXTColorRef CXTColorRef::fromHSB(double h, double s, double b)
{
	CXTColorRef cr;
	cr.setHSB(h, s, b);
	return cr;
}

CXTColorRef CXTColorRef::fromHSL(double h, double s, double l)
{
	CXTColorRef cr;
	cr.setHSL(h, s, l);
	return cr;
}

CXTColorRef& CXTColorRef::blend(const CXTColorRef& clr, int opacity)
{
	*this = RGB(BlendColor(clr.getRValue(), getRValue(), opacity),
				BlendColor(clr.getGValue(), getGValue(), opacity),
				BlendColor(clr.getBValue(), getBValue(), opacity));
	return *this;
}

CXTColorRef& CXTColorRef::add(const CXTColorRef& clr)
{
	*this = RGB(addColorValue(clr.getRValue(), getRValue()),
				addColorValue(clr.getGValue(), getGValue()),
				addColorValue(clr.getBValue(), getBValue()));
	return *this;
}

CXTColorRef& CXTColorRef::subtract(const CXTColorRef& clr)
{
	*this = RGB(addColorValue(-clr.getRValue(), getRValue()),
				addColorValue(-clr.getGValue(), getGValue()),
				addColorValue(-clr.getBValue(), getBValue()));
	return *this;
}

COLORREF CXTColorRef::SafeRGB(int r, int g, int b)
{
	return RGB(fixColorValue(r), fixColorValue(g), fixColorValue(b));
}

CXTColorRef CXTColorRef::operator + (int val) const
{
	val = fixColorValue(val);
	return operator + (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator - (int val) const
{
	val = fixColorValue(val);
	return operator - (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator * (int val) const
{
	val = fixColorValue(val);
	return operator * (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator / (int val) const
{
	val = fixColorValue(val);
	return operator / (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator + (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp += cr);
}

CXTColorRef CXTColorRef::operator - (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp -= cr);
}

CXTColorRef CXTColorRef::operator * (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp *= cr);
}

CXTColorRef CXTColorRef::operator / (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp -= cr);
}

CXTColorRef& CXTColorRef::operator += (int val)
{
	val = fixColorValue(val);
	return operator += (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator -= (int val)
{
	val = fixColorValue(val);
	return operator -= (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator *= (int val)
{
	val = fixColorValue(val);
	return operator *= (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator /= (int val)
{
	val = fixColorValue(val);
	return operator /= (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator += (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() + GetRValue(cr),
		getGValue() + GetGValue(cr),
		getBValue() + GetBValue(cr));
	return *this;
}

CXTColorRef& CXTColorRef::operator -= (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() - GetRValue(cr),
		getGValue() - GetGValue(cr),
		getBValue() - GetBValue(cr));
	return *this;
}

CXTColorRef& CXTColorRef::operator *= (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() * GetRValue(cr),
		getGValue() * GetGValue(cr),
		getBValue() * GetBValue(cr));
	return *this;
}

CXTColorRef& CXTColorRef::operator /= (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() / GetRValue(cr),
		getGValue() / GetGValue(cr),
		getBValue() / GetBValue(cr));
	return *this;
}

////  XP Colors Routings

COLORREF CXTColorRef::GetColor(int nIndex)
{
	return XTPColorManager()->GetColor(nIndex);
}

void CXTColorRef::RefreshColors()
{
	XTPColorManager()->RefreshColors();
}
