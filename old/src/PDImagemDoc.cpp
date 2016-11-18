// pdimadoc.cpp : implementation of the CPDImagemDoc class
//

#include "stdafx.h"
#include "pdimagem.h"
#include <limits.h>
#include "PDImagemDoc.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern CObList ObjectList;
extern POSITION pos;
extern BOOL Inserir;
/////////////////////////////////////////////////////////////////////////////
// CPDImagemDoc

IMPLEMENT_DYNCREATE(CPDImagemDoc, CDocument)

BEGIN_MESSAGE_MAP(CPDImagemDoc, CDocument)
    //{{AFX_MSG_MAP(CPDImagemDoc)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CPDImagemDoc construction/destruction

CPDImagemDoc::CPDImagemDoc()
{   
	if (Inserir)
		ObjectList.AddTail(this); 

	Imagem.Region	= FALSE;
	Inserir			= TRUE;
	m_sizeDOC		= CSize(1,1);
}

CPDImagemDoc::~CPDImagemDoc()
{
	if( ( pos = ObjectList.Find(this) ) != NULL ) 
		ObjectList.RemoveAt(pos);
}

/*
BOOL CPDImagemDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;
    m_sizeDOC = CSize(1,1);

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}
*/

BOOL CPDImagemDoc::OnOpenDocument(const char* pszPathName)
{
    CFile file;
    CFileException fe;
    if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
    {
        ReportSaveLoadException(pszPathName, &fe,
            FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        return FALSE;
    }

    DeleteContents();
    BeginWaitCursor();

    // replace calls to Serialize with ReadDIBFile function
    TRY
    {
        Imagem.hDIB = Imagem.ReadDIBFile(file);
    }
    CATCH (CFileException, eLoad)
    {
        file.Abort(); // will not throw an exception
        EndWaitCursor();
        ReportSaveLoadException(pszPathName, eLoad,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        Imagem.hDIB = NULL;
        return FALSE;
    }
    END_CATCH

    EndWaitCursor();
    
    Imagem.InitDIBData();
    if (Imagem.hDIB == NULL)
    {
        // may not be DIB format
        MessageBox(NULL, "Couldn't load DIB", NULL,
                         MB_ICONINFORMATION | MB_OK);
        return FALSE;
    }
    SetPathName(pszPathName);
    SetModifiedFlag(FALSE);     // start off with unmodified
    return TRUE;
}


BOOL CPDImagemDoc::OnSaveDocument(const char* pszPathName)
{
    CFile file;
    CFileException fe;

    if (!file.Open(pszPathName, CFile::modeCreate |
      CFile::modeReadWrite | CFile::shareExclusive, &fe))
    {
        ReportSaveLoadException(pszPathName, &fe,
            TRUE, AFX_IDP_INVALID_FILENAME);
        return FALSE;
    }

    // replace calls to Serialize with SaveDIB function
    BOOL bSuccess = FALSE;
    TRY
    {
        BeginWaitCursor();
        bSuccess = Imagem.SaveDIB(file);
        file.Close();
    }
    CATCH (CException, eSave)
    {
        file.Abort(); // will not throw an exception
        EndWaitCursor();
        ReportSaveLoadException(pszPathName, eSave,
            TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
        return FALSE;
    }
    END_CATCH

    EndWaitCursor();
    SetModifiedFlag(FALSE);     // back to unmodified

    if (!bSuccess)
    {
        // may be other-style DIB (load supported but not save)
        //  or other problem in SaveDIB
        MessageBox(NULL, "Couldn't save DIB", NULL,
                         MB_ICONINFORMATION | MB_OK);
    }

    return bSuccess;
}




/////////////////////////////////////////////////////////////////////////////
// CPDImagemDoc diagnostics

#ifdef _DEBUG
void CPDImagemDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CPDImagemDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}

#endif //_DEBUG

