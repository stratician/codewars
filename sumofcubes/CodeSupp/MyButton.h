#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__



class MyButton :
	public CButton
{

	
public:
	MyButton();


public:

	int m_accessLevel;
	int m_timer;

public:
	virtual ~MyButton();

public:
	void process(void);

protected:
	//afx_msg void OnMouseHover();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseActivate(UINT nFlags);
	
	
protected:
	DECLARE_MESSAGE_MAP()

};

#endif // __MYBUTTON_H__