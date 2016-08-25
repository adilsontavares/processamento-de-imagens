// PDImagemDoc.h : interface of the CPDImagemDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PDIMADOC_H

#define PDIMADOC_H

#include "PDIBase.h"
#include "PDIMorfb.h"
 
class CPDImagemDoc : public CDocument
{
public: // create from serialization only
    CPDImagemDoc();
    DECLARE_DYNCREATE(CPDImagemDoc)

// Attributes
public:
    CSize m_sizeDOC;
    CPDIMorfoB Imagem; 
// Operations
public:

// Implementation
public:
    virtual ~CPDImagemDoc();
    virtual BOOL OnSaveDocument(const char* pszPathName);
    virtual BOOL OnOpenDocument(const char* pszPathName);
//    virtual BOOL    OnNewDocument();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CPDImagemDoc)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////
