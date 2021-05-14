
// QueryToolView.cpp : implementation of the CQueryToolView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "QueryTool.h"
#endif

#include "QueryToolDoc.h"
#include "QueryToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQueryToolView

IMPLEMENT_DYNCREATE(CQueryToolView, CView)

BEGIN_MESSAGE_MAP(CQueryToolView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CQueryToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CQueryToolView construction/destruction

CQueryToolView::CQueryToolView() noexcept
{
	// TODO: add construction code here

}

CQueryToolView::~CQueryToolView()
{
}

BOOL CQueryToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CQueryToolView drawing

void CQueryToolView::OnDraw(CDC* /*pDC*/)
{
	CQueryToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CQueryToolView printing


void CQueryToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CQueryToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CQueryToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CQueryToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CQueryToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CQueryToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CQueryToolView diagnostics

#ifdef _DEBUG
void CQueryToolView::AssertValid() const
{
	CView::AssertValid();
}

void CQueryToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CQueryToolDoc* CQueryToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CQueryToolDoc)));
	return (CQueryToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CQueryToolView message handlers
