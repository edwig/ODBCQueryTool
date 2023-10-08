/*
    Copyright (C) 2004 - 2021 Edwig Huisman

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "pch.h"
#include "Codeer.h"

unsigned long CCodeer::m_polynomen[20] =
// These are 20 random numbers
{
    0x1acb5af2,
    0x22564650,
    0x76e02ee6,
    0x0c227d34,
    0x05a037a7,
    0x652161b3,
    0x2def5096,
    0x2afe0b5c,
    0x377037dd,
    0x240c64c9,
    0x69a54103,
    0x178712d3,
    0x265815be,
    0x5b1d5c02,
    0x033a7908,
    0x59ac2447,
    0x26cb4074,
    0x774e50ee,
    0x6c7977c3,
    0x769328ce};

CCodeer::CCodeer(long p_schuifregister /* = 0 */)
{
  Reset(p_schuifregister);
}

void CCodeer::Reset(long p_schuifregister /* = 0 */)
// Reset. Nodig tussen niet gecorreleerde codeer/decodeer acties.
// Werking wordt niet uitgelegd.
{
	
  m_poltel=0;
  m_schuifregister = 0x00FFFF00 ^ p_schuifregister;
  CodeerChar('\0');    // voorkom hiermee bijvoorbeeld dat het 1e 0 char ff oplevert
                       // wat een aanwijzig kan zijn.
}

char CCodeer::CodeerChar(char p_kar)
// Codeer een karakter. Werking wordt niet uitgelegd.
{
	int n;
  char retval = (char)(p_kar ^ ((m_schuifregister >> 8 ) & 0xFF));

	m_schuifregister |= p_kar;       

	for(n=0;n<8;n++)
	{
	  m_schuifregister=m_schuifregister << 1;
		if(m_schuifregister & 0x01000000L)
    {
      m_schuifregister^=m_polynomen[m_poltel++];      
      if(m_poltel>=20)
        m_poltel=0;
    }
  }
 
  return retval;
}

char CCodeer::DeCodeerChar(char p_kar)
// De-codeer een karakter. Werking wordt niet uitgelegd.
{
	int n;
  char retval = (char)(p_kar ^ ((m_schuifregister >> 8 ) & 0xFF));

	m_schuifregister |= retval;       

	for(n=0;n<8;n++)
	{
	  m_schuifregister=m_schuifregister << 1;
		if(m_schuifregister & 0x01000000L)
    {
      m_schuifregister^=m_polynomen[m_poltel++];      
      if(m_poltel>=20)
        m_poltel=0;
    }
  }
 
  return retval;
}

CString CCodeer::CodeerString(CString p_str,int p_stap /* = 10*/)
// Codeer een string naar binaire data.
// Stap: moet in elk geval gelijk zijn bij decoderen.
{
  CString retval;
  int len = p_str.GetLength();
  TCHAR *buf=retval.GetBufferSetLength(len);
  for(int i=0;i<p_stap;i++)
  {
    for(int n=i;n<len;n+=p_stap)
    {
      buf[n]=CodeerChar((char) p_str[n]);
    }
  }
  retval.ReleaseBuffer(len); 
  return retval;
}

CString CCodeer::DeCodeerString(CString p_str,int p_stap /* = 10*/)
// Decodeer een binair gecodeerde string.
// Stap: moet in elk geval gelijk zijn bij coderen.
{
  CString retval;
  int len = p_str.GetLength();
  TCHAR *buf=retval.GetBufferSetLength(len);
  for(int i=0;i<p_stap;i++)
  {
    for(int n=i;n<len;n+=p_stap)
    {
      buf[n]=DeCodeerChar((uchar) p_str[n]);
    }
  }   
  retval.ReleaseBuffer(len); 
  return retval;
}

CString CCodeer::CodeerStringHex(CString p_str,int p_stap /* = 10*/)
// Codeer een string naar een hex-string (zonder spaties).
// Stap: moet in elk geval gelijk zijn bij decoderen.
{
  CString retval;
  int len = p_str.GetLength();
  CString buf;
  for(int i=0;i<p_stap;i++)
  {
    for(int n=i;n<len;n+=p_stap)
    {
      buf.Format(_T("%02x"),(unsigned char)CodeerChar((char)p_str[n]));
      retval += buf;
    }
  }
  return retval;
}

CString CCodeer::DeCodeerStringHex(CString p_str,int p_stap /* = 10*/)
// Decodeer een hex-string.
// Stap: moet in elk geval gelijk zijn bij decoderen.
{
	CString retval;
	int len = p_str.GetLength() / 2;
	TCHAR *bufs=retval.GetBufferSetLength(len);
	const TCHAR *bufl=(const TCHAR *)p_str;
	int code;
	for(int i=0;i<p_stap;i++)
	{
		for(int n=i;n<len;n+=p_stap)
		{
			_stscanf_s(bufl,_T("%2x"),&code);
			bufs[n]=DeCodeerChar((uchar)code);
			bufl+=2;
		}
	}   
	retval.ReleaseBuffer(len); 
	return retval;
}

CString CCodeer::CodeerString(CString p_str,CString p_mask) 
// Codeer een string naar binaire data.
// p_mask: om extra moeilijk te maken, moet in elk geval gelijk zijn bij decoderen.
{
  int stap = p_mask.GetLength();
  CodeerString(p_mask);
  CodeerChar((char)p_str.GetLength());
  return CodeerString(p_str,stap);
}

CString CCodeer::CodeerStringHex(CString p_str,CString p_mask) 
// Codeer een string naar een hex-string (zonder spaties).
// p_mask: om extra moeilijk te maken, moet in elk geval gelijk zijn bij decoderen.
{
  int stap = p_mask.GetLength();
  CodeerString(p_mask);
  CodeerChar((char)p_str.GetLength());
  return CodeerStringHex(p_str,stap);
}

CString CCodeer::DeCodeerString(CString p_str,CString p_mask) 
// Decodeer een binair gecodeerde string.
// p_mask: om extra moeilijk te maken, moet in elk geval gelijk zijn bij coderen.
{
  int stap = p_mask.GetLength();
  CodeerString(p_mask);
  CodeerChar((char)p_str.GetLength());
  return DeCodeerString(p_str,stap);
}

CString CCodeer::DeCodeerStringHex(CString p_str,CString p_mask) 
// Decodeer een hex-string.
// p_mask: om extra moeilijk te maken, moet in elk geval gelijk zijn bij coderen.
{
  int stap = p_mask.GetLength();
  CodeerString(p_mask);
  CodeerChar((char)(p_str.GetLength()/2));    // /2 want lengte is verdubbeld.
  return DeCodeerStringHex(p_str,stap);
}
