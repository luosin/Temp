// XhEntUGCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DistToXhlUGCtrl.h"

// CDistToXhlUGCtrl

IMPLEMENT_DYNAMIC(CDistToXhlUGCtrl, CUGCtrl)
CDistToXhlUGCtrl::CDistToXhlUGCtrl()
: m_strCellPreText(_T(""))
{
	for (int i=0; i<30; i++)
	{
		m_iItemIsNull[i] = 0 ;
	}
}

CDistToXhlUGCtrl::~CDistToXhlUGCtrl()
{
	UGXPThemes::CleanUp();
}


BEGIN_MESSAGE_MAP(CDistToXhlUGCtrl, CUGCtrl)
END_MESSAGE_MAP()



// CDistToXhlUGCtrl 消息处理程序

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void CDistToXhlUGCtrl::OnSetup()
{
	//setup the fonts
	m_defFont.CreateFont( 14, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );
	m_boldFont.CreateFont( 14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_smFont.CreateFont( 10, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );	
	m_medFont.CreateFont( 16, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_medBoldFont.CreateFont( 16, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_lrgFont.CreateFont( 20, 0, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Times New Roman" ) );	
	SetDefFont( &m_defFont );


	int nCols = 5, nRows = 0 ;
	for (int i=0; i<m_vecToXhl.size(); i++)
	{
		nRows+=m_vecToXhl[i].size() ;
	}
	
	SetTH_NumberRows( 1 );
	SetDefColWidth( 80 );
	SetSH_Width(40) ;
	SetTH_Height(20) ;

	//EnableMenu( TRUE );
	SetMultiSelectMode( TRUE );
	SetDoubleBufferMode( TRUE );

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
	CString strHeads[] = {_T("序号"), _T("类型"), _T("名称"), _T("距离"), _T("区间"), _T("上/下栏")} ;
	int iColWidth[5] = {80, 80, 85, 50, 50} ;

	for (int i=0; i<6; i++) //tophead
	{
		GetCellIndirect( i-1, -1, &cell );
		cell.SetAlignment( UG_ALIGNCENTER|UG_ALIGNVCENTER );		
		cell.SetText(strHeads[i]);
		SetCell(i-1, -1, &cell) ;
		if (i>0)
		{
			SetColWidth(i-1, iColWidth[i-1]) ;
		}
	}

	int iIndexRow = 0 ;
	for (int iSec=0; iSec<m_vecToXhl.size(); iSec++)
	{
		for (int i=0; i<m_vecToXhl[iSec].size(); i++)
		{			
			GetCellIndirect( -1, iIndexRow, &cell ); //left
			cell.SetAlignment( UG_ALIGNCENTER|UG_ALIGNVCENTER );
			strCellText.Format(_T("%d"), iIndexRow+1) ;
			cell.SetText(strCellText);
			SetCell(-1, iIndexRow, &cell) ;	

			for (int j=0; j<5; j++)
			{
				GetCellIndirect( j, iIndexRow, &cell );
				cell.ClearAll() ;
				cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
				cell.SetBackColor(RGB(240,240,240));

				if (j==2)
				{
					CString strTmp = m_vecToXhl[iSec][i][2];
					int iLoc = strTmp.Find(_T("__")) ;
					if (iLoc==-1)
					{
						strCellText = strTmp ;
					}
					else
					{
						strCellText = strTmp.Left(iLoc) ;
					}
				}
				else if (j==3)
				{
					strCellText.Format(_T("%d"), iSec+1) ;
				}
				else if (j==4)
				{
					CString strTmp = m_vecToXhl[iSec][i][2];
					int iLoc = strTmp.Find(_T("__")) ;
					if (iLoc==-1)
					{
						strCellText = _T("") ;
					}
					else
					{
						strCellText = strTmp.Mid(iLoc+2) ;
					}
				}
				else
				{
					strCellText = m_vecToXhl[iSec][i][j];		
				}
				cell.SetText(strCellText) ;
				if (j==2)
				{
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				SetCell(j, iIndexRow, &cell) ;	
			}
			iIndexRow++ ;
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
void CDistToXhlUGCtrl::OnGetCell(int col,long row,CUGCell *cell)
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
void CDistToXhlUGCtrl::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
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

	if(nParam == CELLTYPE_IS_EDITABLE || strlabel == "CELLTYPE_IS_EDITABLE")
	{
		StartEdit();
	}
}

int CDistToXhlUGCtrl::OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param){

	if(ID == UGCT_DROPLIST)
	{
		//return OnDropList(ID, col, row, msg, param);
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

int CDistToXhlUGCtrl::SetCellParam(int nCol, int nRow, int nParam)
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
void CDistToXhlUGCtrl::SetupSection(long nRow, LPCTSTR lpcstrTitle)
{
	CUGCell cell;

	cell.SetBackColor(RGB(240,240,240));
	cell.SetTextColor(RGB(0,0,128));
	cell.SetFont(&m_boldFont);
	cell.SetAlignment(UG_ALIGNVCENTER);
	QuickSetRange(0,nRow,GetNumberCols() - 1,nRow,&cell);

	QuickSetText(-1,nRow,lpcstrTitle);
}

double CDistToXhlUGCtrl::QuickGetNumber(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber();
}

int CDistToXhlUGCtrl::QuickGetNumber(int nCol, int nRow, int& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
int CDistToXhlUGCtrl::QuickGetNumber(int nCol, int nRow, double& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
BOOL CDistToXhlUGCtrl::QuickGetBool(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetBool();
}

// -----------------------------------------------------------------------------
int CDistToXhlUGCtrl::OnEditFinish(int  col, long  row, CWnd * edit, LPCTSTR  string, BOOL  cancelFlag)
{
	int retCode =CUGCtrl::OnEditFinish (col, row, edit, string, cancelFlag) ;

	acutPrintf(_T("\n%s->%s"), m_strCellPreText, string) ;

	if (m_strCellPreText!=string)
	{
		COLORREF color = RGB(255, 0, 0) ;
		this->QuickSetTextColor(col, row, color) ;
	}	

	return (retCode) ;
}

// -----------------------------------------------------------------------------
int CDistToXhlUGCtrl::OnEditStart(int  col, long  row, CWnd ** edit)
{
	m_strCellPreText = QuickGetText(col, row) ;

	int retCode =CUGCtrl::OnEditStart (col, row, edit) ;
	return (retCode) ;
}

// 将数据从Grid读出
int CDistToXhlUGCtrl::OutData(vector<vector<vector<CString> > >& vecSection)
{
	long nNumRows = this->GetNumberRows() ;
	acutPrintf(_T("\nOutData.Numrows:%d"), nNumRows) ;
	CString strPreSec = _T("1") ;
	vector<vector<CString> > vecToXhl ;

	for (int i=0; i<nNumRows; i++)
	{
		vector<CString > vec ;
		CString strCurSec = _T("1") ;
		for (int j=0; j<4; j++)
		{
			CString strText = QuickGetText(j, i) ;
			if (j==3&&!strText.IsEmpty())
			{
				strCurSec = strText ;
			}
			if (j==2) //上下栏信息和距离栏记录在一起
			{
				CString strUpODownGrid = QuickGetText(j+2, i) ;
				strText.AppendFormat(_T("__%s"), strUpODownGrid.IsEmpty()?_T("上"): strUpODownGrid) ;
			}
			vec.push_back(strText) ;
		}

		if (strCurSec.Compare(strPreSec)!=0)
		{
			vecSection.push_back(vecToXhl) ;
			vecToXhl.clear() ;
			vecToXhl.push_back(vec) ;		
			strPreSec = strCurSec ;
		}
		else
		{
			vecToXhl.push_back(vec) ;
		}
		if (i==(nNumRows-1)) //最后一个区段
		{
			vecSection.push_back(vecToXhl) ;
		}
	}
	return 0;
}

// 从外界传入数据更新Grid
int CDistToXhlUGCtrl::InData(const vector<vector<vector<CString> > >& vecToXhl)
{
	m_vecToXhl = vecToXhl ;
	OnSetup() ;
	Invalidate() ;
	return 0;
}
