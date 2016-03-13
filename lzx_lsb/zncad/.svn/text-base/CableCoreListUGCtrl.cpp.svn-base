// CableCoreListUGCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "cablecorelistugctrl.h"


IMPLEMENT_DYNAMIC(CCableCoreListUGCtrl, CUGCtrl)
CCableCoreListUGCtrl::CCableCoreListUGCtrl()
{
}

CCableCoreListUGCtrl::~CCableCoreListUGCtrl()
{
	UGXPThemes::CleanUp();
}


BEGIN_MESSAGE_MAP(CCableCoreListUGCtrl, CUGCtrl)
END_MESSAGE_MAP()



// CCableCoreListUGCtrl 消息处理程序

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void CCableCoreListUGCtrl::OnSetup()
{
	//setup the fonts
	m_defFont.CreateFont( 14, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );
	m_boldFont.CreateFont( 14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_smFont.CreateFont( 10, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );	
	m_medFont.CreateFont( 16, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_medBoldFont.CreateFont( 16, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_lrgFont.CreateFont( 20, 0, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Times New Roman" ) );	
	SetDefFont( &m_defFont );

	

	int nCols = 3 ;
	int nRows = (m_vecCableCore.size()>25?m_vecCableCore.size():25) ;

	SetTH_NumberRows( 1 );
	SetTH_Height(20) ;
	SetDefColWidth( 97 );
	SetSH_Width(50) ;
	
	//EnableMenu( TRUE );
	SetMultiSelectMode( TRUE );
	SetDoubleBufferMode( TRUE );

	// 	SetUserSizingMode( 2 );
	// 	SetVScrollMode( UG_SCROLLTRACKING );
	// 	SetHScrollMode( UG_SCROLLTRACKING );

	CUGCell cell;
	// setup the default grid look
	GetGridDefault( &cell );
	cell.SetFont( GetFont( AddFont( _T("Arial"), -12, 400 )));
	//cell.SetBackColor(RGB(238,237,232)) ;
	cell.SetAlignment(UG_ALIGNLEFT|UG_ALIGNVCENTER) ;
	SetGridDefault( &cell );	

	SetNumberRows( nRows );
	SetNumberCols( nCols );
	CString strCellText ;
	CString strHeads[] = {_T("序号"), _T("芯线"), _T("设备"), _T("频率")} ;

	for (int i=0; i<4; i++) //tophead
	{
		GetCellIndirect( i-1, -1, &cell );
		cell.SetAlignment( UG_ALIGNCENTER|UG_ALIGNVCENTER );		
		cell.SetText(strHeads[i]);
		SetCell(i-1, -1, &cell) ;	
	}

	vector<CCableCore>::iterator it_allcore = m_vecCableCore.begin() ;
	for (int i=0; i<nRows; i++)
	{			
		GetCellIndirect( -1, i, &cell ); //left
		cell.SetAlignment( UG_ALIGNCENTER|UG_ALIGNVCENTER );
		strCellText.Format(_T("%d"), i+1) ;
		cell.SetText(strCellText);
		SetCell(-1, i, &cell) ;	
		
		for (int j=0; j<3; j++)
		{
			GetCellIndirect( j, i, &cell );
			cell.ClearAll() ;
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetBackColor(RGB(240,240,240));
			strCellText = _T("") ;
			if (it_allcore!=m_vecCableCore.end())
			{
				if (j==0)
				{
					strCellText = it_allcore->m_strCoreNameShow ;
				}
				else if (j==1)
				{
					strCellText = it_allcore->m_strDevName ;
				}
				else if (j==2)
				{
					strCellText = it_allcore->m_strFreq ;
				}				
			}
			cell.SetText(strCellText) ;
			SetCell(j, i, &cell) ;	
		}	
		if (it_allcore!=m_vecCableCore.end())
		{
			++it_allcore ;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		object has been populated with all of the information
//		that will be used to draw the cell. This information
//		can now be changed before it is used for drawing.
//	Warning:
//		This notification is called for each cell that needs to be painted
//		Placing complicated calculations here will slowdown the refresh speed.
//	Params:
//		col, row	- coordinates of cell currently drawing
//		cell		- pointer to the cell object that is being drawn
//	Return:
//		<none>
void CCableCoreListUGCtrl::OnGetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);

	// 	if ( col >= 0 && row == -1 )
	// 	{	// set default values to the top heading
	// 		cell->SetNumberDecimals( 0 );
	// 		cell->SetNumber( col );
	// 	}
	// 	else if ( row >= 0 && col == -1 )
	// 	{	// set default values to the side heading (row numbers)
	// 		cell->SetNumberDecimals( 0 );
	// 		cell->SetNumber( row );
	// 	}
	// 	else if ( col >= 0 && row >= 0 )
	// 	{
	// 	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void CCableCoreListUGCtrl::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	CUGCell cell;
	GetCell(col,row,&cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();
	//acutPrintf(_T("\n%s-%d"), cell.GetText(),nParam) ;
	CString strlabel;
	if(cell.GetLabelText() != NULL)
		strlabel = cell.GetLabelText();

	// 	if(processed)
	// 	{
	// 		if(cell.GetCellType() == m_nSpinIndex)
	// 			return ;
	// 	}

// 	if(nParam == CELLTYPE_IS_EDITABLE || strlabel == "CELLTYPE_IS_EDITABLE")
// 	{
// 		StartEdit();
// 	}
}

int CCableCoreListUGCtrl::OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param){

	if(ID == UGCT_DROPLIST)
	{
		return OnDropList(ID, col, row, msg, param);
	}
	// 	if(ID == UGCT_CHECKBOX){
	// 		return OnCheckbox(ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nRadioIndex){
	// 	}
	// 	if(ID == m_nDateTimeIndex){
	// 		return OnDateTime(ID, col, row, msg, param);		
	// 	}
	// 	if(ID == m_nButtonIndex){
	// 		return OnPushButton( ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nEllipsisIndex){
	// 		return OnEllipsisButton( ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nSpinIndex){
	// 		return OnSpinButton( ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nSliderIndex){
	// 		return OnSlider( ID, col, row, msg, param);
	// 	}

	return TRUE;
}

int CCableCoreListUGCtrl::OnDropList(long ID,int col,long row,long msg,long param)
{
	return TRUE;
}

int CCableCoreListUGCtrl::SetCellParam(int nCol, int nRow, int nParam)
{
	int iRet = UG_SUCCESS ;
	// 	CUGCell cell;
	// 	GetCell(nCol, nRow, &cell);
	// 	return cell.SetParam(nParam) ;

	m_cell.ClearAll();
	GetCell(nCol, nRow, &m_cell);
	iRet = m_cell.SetParam(nParam) ;
	if (iRet==UG_SUCCESS)
	{
		iRet = SetCell(nCol, nRow, &m_cell);
	}

	return iRet ;
}
void CCableCoreListUGCtrl::SetupSection(long nRow, LPCTSTR lpcstrTitle)
{
	CUGCell cell;

	cell.SetBackColor(RGB(240,240,240));
	cell.SetTextColor(RGB(0,0,128));
	cell.SetFont(&m_boldFont);
	cell.SetAlignment(UG_ALIGNVCENTER);
	QuickSetRange(0,nRow,GetNumberCols() - 1,nRow,&cell);

	QuickSetText(-1,nRow,lpcstrTitle);
}

double CCableCoreListUGCtrl::QuickGetNumber(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber();
}

int CCableCoreListUGCtrl::QuickGetNumber(int nCol, int nRow, int& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
int CCableCoreListUGCtrl::QuickGetNumber(int nCol, int nRow, double& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
BOOL CCableCoreListUGCtrl::QuickGetBool(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetBool();
}

// -----------------------------------------------------------------------------
int CCableCoreListUGCtrl::OnEditFinish(int  col, long  row, CWnd * edit, LPCTSTR  string, BOOL  cancelFlag)
{
	int retCode =CUGCtrl::OnEditFinish (col, row, edit, string, cancelFlag) ;
	return (retCode) ;
}

// -----------------------------------------------------------------------------
int CCableCoreListUGCtrl::OnEditStart(int  col, long  row, CWnd ** edit)
{
	int retCode =CUGCtrl::OnEditStart (col, row, edit) ;
	return (retCode) ;
}
