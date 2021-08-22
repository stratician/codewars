#ifndef __HELPERS_H__
#define __HELPERS_H__

// Helpers.h : header file
//

// [CSockStream] //

class CSockStream {
public:
char *m_pBuf;
char *m_pPtr;
char *m_pEnd;
int m_siz;
public:
CSockStream();
~CSockStream();
void SetSource(char *pBuf, int siz);
int Parse(char *c);
};

// <-[CSockStream] //

// [CHTTPHeaderSet] //

class CHTTPHeaderSet {
public:
char **m_tag;
char **m_value;
int m_n;
int m_idx;
int m_contentLength;
BOOL m_chunked;
int m_code; // 200 = OK, 404 = Not Found
public:
CHTTPHeaderSet();
~CHTTPHeaderSet();
void ClearHeaders(void);
void AddHeader(char *str);
void ShowHeaders(void);
void AnalyzeHeader(int idx);
};

// <-[CHTTPHeaderSet] //

// [CUrl] //

class CUrl {
public:
char *m_str;
char *m_fname;
char **m_arg;
char **m_value;
int m_n;
int m_idx;
public:
CUrl(char *_str);
~CUrl();
void CutUpString(void);
void SetFile(char *str);
void SetArg(char *str);
void SetValue(char *str);
void ShowValues(void);
char *Get(char *name);
};

// <-[CUrl] //

#endif //__HELPERS_H__