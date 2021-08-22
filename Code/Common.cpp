///////////////////////////////////////////////////////////////////////////////////////////////////
// (Common.cpp)
//

#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"
#include "Common.h"

float g_ballRadius;
float g_ballRadiusFactor;

void TextOutEllipsis(CDC *pDC, int x, int y, const CString& str, int cx)
{
	int n,m;
	CString tmpStr;
	CString newStr;
	CSize size;

	// check for -ve width and truncate?
	if(cx < 0 ) cx = 0;
		
	// check if we can display more of the string?
	for(n=1;n<str.GetLength();n++)
	{
		tmpStr = str.Left(n);
		tmpStr += _T("... ");	

		size = pDC->GetTextExtent(tmpStr);
		if(size.cx > cx) break;
		newStr = tmpStr;
	}
	
	// check if we have the whole string? if so, just copy the original string param.
	if(n == str.GetLength())
		newStr = str;
  
	// get firt char of string (handles those funny clipping problems :) )
	if(n == 1)
	{
		// make sure we have a string (i.e. not a single character)
		if(str.GetLength() > 1)
		for(m=4;m>=0;m--)
		{
			if(m==-1)
			{
				newStr = _T("");
				break;
			}
			
			newStr = tmpStr.Left(m);
			size = pDC->GetTextExtent(newStr);
			if(size.cx < cx) break;
		}
	}

	pDC->TextOut(x, y, newStr, newStr.GetLength());
}

void ErrorMessage(CWnd *pParent, char *errorMessage) // Note: use this function as a replacement to MessageBox(...
{
	if(errorMessage)
	{
		char buffer[MAX_CHARS];
		sprintf(buffer,"*** !!Error!! %s ***", errorMessage);
		((CGameAppView *)pParent)->Message(buffer, 50); // important error message
	}
}

/*
int wwwsearchi(char *buf, int len, char *str ,int offset) // non case sensi.
{
	int n;
	int i;
	char s;
	char b;
	i = 0;
	for(n=offset;n<len;n++)
	{
	s = str[i];
	if(s>='a' && s<='z') s-=32;
	b = buf[n];
	if(b>='a' && b<='z') b-=32;



	if(b == s) // character match
		{
		i++;
		if(str[i]==0) return n+1;
		} else {
			   n-=i;
			   i=0;
			   }
	}
	return -1;
}

int wwwsearchiencapsulate(char *contents, char *buffer, int len, char *str_s ,char *str_e,int offset) // non case sensi.
{
	int start,end;
	start = offset = wwwsearchi(buffer,len, str_s, offset);
	end = offset = wwwsearchi(buffer,len, str_e, offset);
	end-=strlen(str_e);
	// 0-0: 
	memcpy(contents,&buffer[start],end-start);
	contents[end-start] = 0;
	return offset;
}

char *wwwsearchiencapsulateauto(int *rt_len, char *buffer, int len, char *str_s ,char *str_e,int *offset) // non case sensi.
{
	int start,end;
	start = *offset = wwwsearchi(buffer,len, str_s, *offset);
	if(*offset==-1) return NULL;
	end = *offset = wwwsearchi(buffer,len, str_e, *offset);
	if(*offset==-1) return NULL;
	end-=strlen(str_e);

	*rt_len = end-start;
	return &buffer[start];
}

char *makestring(char *buffer, int len)
{
	char *str;
	str = new char[len+1];
	memcpy(str, buffer, len);
	str[len] = 0;
	return str;
}
*/

// [CExaminer] //

extern char *wwwsearchiencapsulateauto(int *rt_len, char *buffer, int len, char *str_s ,char *str_e,int *offset); // non case sensi.

CExaminer::CExaminer(char *buf, int len)
{
	m_buf = buf;
	m_len = len;
	m_offset = 0;
	m_tmpstr = NULL;
}

CExaminer::~CExaminer()
{
	if(m_tmpstr) delete m_tmpstr;
}

void CExaminer::Restart(void)
{
	m_offset = 0;
}

CExaminer *CExaminer::Examine(char *start, char *end)
{
	char *entry;
	int entry_len;
	entry = wwwsearchiencapsulateauto(&entry_len, m_buf, m_len, start, end,&m_offset); // non case sensi.
	if(!entry) return NULL;

	return new CExaminer(entry, entry_len);
}

CExaminer *CExaminer::ExamineEntire(char *start, char *end)
{ // same as Examine, but does Restart every time (good when you don't know what order your args will be in)
	char *entry;
	int entry_len;
	m_offset = 0;
	entry = wwwsearchiencapsulateauto(&entry_len, m_buf, m_len, start, end,&m_offset); // non case sensi.
	if(!entry) return NULL;

	return new CExaminer(entry, entry_len);
}

char *CExaminer::GetString(void)
{
	if(m_tmpstr) delete m_tmpstr;
	m_tmpstr = new char[m_len+1];
	memcpy(m_tmpstr, m_buf, m_len);
	m_tmpstr[m_len] = 0;
	return m_tmpstr;
}

int CExaminer::GetLength(void)
{
	return m_len;
}

int CExaminer::GetInt(void)
{
	int v;
	char *tmpstr;
	tmpstr = new char[m_len+1];
	memcpy(tmpstr, m_buf, m_len);
	tmpstr[m_len] = 0;

	sscanf(tmpstr,"%d",&v);
	delete tmpstr;

	return v;
}
