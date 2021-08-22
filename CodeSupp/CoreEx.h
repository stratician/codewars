#ifndef __COREEX_H__
#define __COREEX_H__

class CCoreEx {
protected:
CCoreEx *m_pParent; // pointer to Parent - for Retrieving Event Messages.
int m_id;
public:
CCoreEx();
void SetId(int id);
void PassParent(CCoreEx *pParent);
virtual void OnEvent(int id);				  
};

#endif // __COREEX_H__
