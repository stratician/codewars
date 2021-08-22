// Helpers.cpp

#include "stdafx.h"
#include "Helpers.h"



// [CSockStream] //

CSockStream::CSockStream()
{
	m_pBuf = NULL;

}

CSockStream::~CSockStream()
{

}

void CSockStream::SetSource(char *pBuf, int siz)
{
	m_pBuf = pBuf;
	m_siz = siz;

	m_pPtr = m_pBuf;
	m_pEnd = &m_pBuf[siz];
}

int CSockStream::Parse(char *c)
{
	if(!m_pBuf) return 0;

	if(m_pPtr!=m_pEnd) 
	{
	*c = *m_pPtr++;
	m_siz--;
	return 1;
	}
	return 0;
}

// [CHTTPHeaderSet] //

CHTTPHeaderSet::CHTTPHeaderSet()
{
	int n;
	m_n = 16;
	m_tag = new char *[m_n];
	m_value = new char *[m_n];
	m_idx = 0;
}

CHTTPHeaderSet::~CHTTPHeaderSet()
{
	ClearHeaders();
	delete m_tag;
	delete m_value;
}

void CHTTPHeaderSet::ClearHeaders(void)
{
	while(m_idx)
	{
	m_idx--;
	delete m_tag[m_idx];
	delete m_value[m_idx];
	}
	m_contentLength = -1;
	m_chunked = FALSE;
}

void CHTTPHeaderSet::AddHeader(char *str)
{
	int n;
	int side;
	int len;
	side = 0;
	m_tag[m_idx] = NULL;
	m_value[m_idx] = NULL;
	for(n=0;n<1024;n++)
	{
	if(side==0)
	{
	if(str[n]==':' || !str[n])
		{
		m_tag[m_idx] = new char[n+1];
		memcpy(m_tag[m_idx], str, n);
		m_tag[m_idx][n] = 0;
		if(!str[n]) break; // no header - it's a GET command or something.
		side++;
		}
	} 
	else 
	{
	if(str[n]>32) {
				  len = strlen(&str[n]);
				  m_value[m_idx] = new char[len+1];
				  memcpy(m_value[m_idx], &str[n], len+1);
				  break;
				  }
	}

	} // end for

	if(!m_tag[m_idx]) {
						m_tag[m_idx] = new char[1];
						m_tag[m_idx][0] = 0;
						}
	if(!m_value[m_idx]) {
						m_value[m_idx] = new char[1];
						m_value[m_idx][0] = 0;
						}
	AnalyzeHeader(m_idx);
	m_idx++;
}

void CHTTPHeaderSet::ShowHeaders(void)
{
	int n;
	char buffer[1024];
	for(n=0;n<m_idx;n++)
	{
	sprintf(buffer,"[%s]\r\n[%s]",m_tag[n],m_value[n]);	
	AfxMessageBox(buffer);
	}
}

void CHTTPHeaderSet::AnalyzeHeader(int idx)
{
	char result[64];
	if(!strcmpi("Content-Length", m_tag[idx]))
		{
		m_contentLength = -1;
		sscanf(m_value[idx],"%d",&m_contentLength);
		}	
	else if(!strcmpi("Transfer-Encoding", m_tag[idx]))
		{
		if(!strcmpi("chunked", m_value[idx])) m_chunked = TRUE;
		}
	if(idx==0)
	{
	result[0] = 0;
	sscanf(m_tag[0],"%s", result);

	sscanf(&m_tag[0][strlen(result)],"%d", &m_code);
	}
}

// [CUrl] //

CUrl::CUrl(char *_str)
{
	int s,d;
	int v1,v2;

	m_fname = new char[1];
	m_fname[0] = 0;
	m_n = 16;
	m_arg = new char *[m_n];
	m_value = new char *[m_n];
	m_idx = 0;



	m_str = new char[strlen(_str)+1];
	// first convert string back into pure format (i.e. %?? stuff into normal)
	s = 0;
	for(d=0;d<256;d++)
	{
	if(!_str[s]) break;

	if(_str[s]!='%')
		{
		m_str[d] = _str[s];
		}
	else {
	v2 = _str[s+1];
	v1 = _str[s+2];
	if(v1>='0' && v1<='9') v1-='0';
	else {
		 if(v1>='A' && v1<='F') v1-='A'-10;
		 else if(v1>='a' && v1<='f') v1-='a'-10;
		 // else invalid
		 }
	if(v2>='0' && v2<='9') v2-='0';
	else {
		 if(v2>='A' && v2<='F') v2-='A'-10;
		 else if(v2>='a' && v2<='f') v2-='a'-10;
		 // else invalid
		 }
	m_str[d] = v2*16+v1;
	s+=2;
	}

	s++;
	}
	m_str[d] = 0;
	// now we have the string in a nice form. - lets treat it as a webpage request.
	CutUpString();
}

CUrl::~CUrl()
{
	delete m_str;

	while(m_idx)
	{
	m_idx--;
	delete m_arg[m_idx];
	delete m_value[m_idx];
	}
	delete m_arg;
	delete m_value;
	if(m_fname) delete m_fname;
}

#define CURL_MODE_NULL		0
#define CURL_MODE_FILE		1
#define CURL_MODE_ARG		2
#define CURL_MODE_VALUE		3

void CUrl::CutUpString(void)
{
	int n;
	int len;
	int mode;
	int ptr;
	char value[256];
	ptr = 0;

	len = strlen(m_str);
	mode = CURL_MODE_NULL;
	for(n=0;n<len+1;n++)
	{
	if(mode==CURL_MODE_NULL)
		{
		if(m_str[n]=='/') mode = CURL_MODE_FILE,ptr=0;
		}
	else if(mode==CURL_MODE_FILE)
		{
		value[ptr] = m_str[n];
		if(m_str[n]=='?' || !m_str[n])
			{
			value[ptr] = 0;
			SetFile(value);
			if(!m_str[n]) break;
			mode = CURL_MODE_ARG,ptr = 0;
			}
		else ptr++;
		}
	else if(mode==CURL_MODE_ARG)
		{
		value[ptr] = m_str[n];
		if(m_str[n]=='=' || !m_str[n])
			{
			value[ptr] = 0;
			SetArg(value);
			if(!m_str[n]) break;
			mode = CURL_MODE_VALUE,ptr = 0;
			}
		else ptr++;
		}
	else if(mode==CURL_MODE_VALUE)
		{
		value[ptr] = m_str[n];
		if(m_str[n]=='&' || !m_str[n])
			{
			value[ptr] = 0;
			SetValue(value);
			if(!m_str[n]) break;
			mode = CURL_MODE_ARG,ptr = 0;
			}
		else ptr++;
		}	
	}
}

void CUrl::SetFile(char *str)
{
	delete m_fname;
	m_fname = new char[strlen(str)+1];
	strcpy(m_fname,str);
}

void CUrl::SetArg(char *str)
{
	m_arg[m_idx] = new char[strlen(str)+1];
	strcpy(m_arg[m_idx], str);
	m_value[m_idx] = new char[1]; // Blank value field default.
	m_value[m_idx][0] = 0;
	m_idx++;
}

void CUrl::SetValue(char *str)
{
	delete m_value[m_idx-1];
	m_value[m_idx-1] = new char[strlen(str)+1];
	strcpy(m_value[m_idx-1], str);
}

void CUrl::ShowValues(void)
{
	int n;
	char buffer[256];
	sprintf(buffer,"Filename:%s", m_fname);
	AfxMessageBox(buffer);
	for(n=0;n<m_idx;n++)
	{
	sprintf(buffer,"[%d)%s=%s]",n ,m_arg[n], m_value[n]);
	AfxMessageBox(buffer);
	}
}

char *CUrl::Get(char *name)
{
	int n;
	for(n=0;n<m_idx;n++)
	if(!strcmp(name, m_arg[n]))
	{
	return m_value[n];
	}

	return NULL;
}