// clntsock.h : interface of the CClientSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __CLNTSOCK_H__
#define __CLNTSOCK_H__

class CMsg;
class CGameAppDoc;

class CClientSocket : public CSocket
{
	DECLARE_DYNAMIC(CClientSocket);
private:
	CClientSocket(const CClientSocket& rSrc);         // no implementation
	void operator=(const CClientSocket& rSrc);  // no implementation

// Construction
public:
	CClientSocket(CGameAppDoc* m_pDoc);

// Attributes
public:
	int m_nMsgCount;//INT_PTR //uintptr_t
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;
	CGameAppDoc* m_pDoc;
	BOOL IsAborted() { return m_pArchiveOut == NULL; }

// Operations
public:
	void Init();
	void Abort();
	void SendMsg(CMsg* pMsg);
	void ReceiveMsg(CMsg* pMsg);

// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);

// Implementation
public:
	virtual ~CClientSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // __CLNTSOCK_H__
