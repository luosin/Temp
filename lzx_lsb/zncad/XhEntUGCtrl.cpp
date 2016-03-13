// XhEntUGCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ZnSxt.h"
#include "XhEntUGCtrl.h"

// CXhEntUGCtrl

IMPLEMENT_DYNAMIC(CXhEntUGCtrl, CUGCtrl)
CXhEntUGCtrl::CXhEntUGCtrl()
: m_pEntDATA(NULL)
, m_strCellPreText(_T(""))
{
	m_pDb = acdbHostApplicationServices()->workingDatabase() ;
	for (int i=0; i<30; i++)
	{
		m_iItemIsNull[i] = 0 ;
	}
}

CXhEntUGCtrl::~CXhEntUGCtrl()
{
	UGXPThemes::CleanUp();
	if (m_pEntDATA!=NULL)
	{
		delete m_pEntDATA ;
		m_pEntDATA = NULL ;
	}
}


BEGIN_MESSAGE_MAP(CXhEntUGCtrl, CUGCtrl)
END_MESSAGE_MAP()



// CXhEntUGCtrl 消息处理程序

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void CXhEntUGCtrl::OnSetup()
{
	//setup the fonts
	m_defFont.CreateFont( 14, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );
	m_boldFont.CreateFont( 14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_smFont.CreateFont( 10, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );	
	m_medFont.CreateFont( 16, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_medBoldFont.CreateFont( 16, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_lrgFont.CreateFont( 20, 0, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Times New Roman" ) );	
	SetDefFont( &m_defFont );

	int nCols = 1, nRows = 12;

	SetDefColWidth( 110 );
	SetSH_Width(120) ;
	SetTH_Height(5) ;
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

// 	const char *szClassName = typeid(*m_pEntDATA).name() ;
// 	CString strClassName ;
// 	CBlkUtility::ConvMultiToWide(szClassName, strClassName) ;
// 	if (strClassName== _T("class CXhjDATA"))
	CString strClassName  = m_pEntDATA->GetClassType() ;
 	if (strClassName== _T("CXhjDATA"))
	{
		nCols = 1 ;
		nRows = 12 ;		
		CXhjDATA *pDATA = dynamic_cast<CXhjDATA*>(m_pEntDATA) ;

		SetNumberRows( nRows );
		SetNumberCols( nCols );

		for (int i=0; i<nRows; i++)
		{	
			//QuickSetText(-1, i, pDATA->strItem[i]) ;
			GetCellIndirect( -1, i, &cell );
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetText(pDATA->strXhjItem[i]) ;
			SetCell(-1, i, &cell) ;			

			GetCellIndirect( 0, i, &cell );
			cell.ClearAll() ;
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetBackColor(RGB(240,240,240));
			switch (i)
			{
			case 0:
				{
					cell.SetText(pDATA->m_strName) ;
				}
				break;
			case 1:
				{
					cell.SetNumber(pDATA->m_nLoc);
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 2:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsTrain?_T("是"):_T("否")) ;
				}
				break;
			case 3:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					pDATA->m_strBoxTypeOption.Replace(_T(";"), _T("\n")) ;					
					cell.SetLabelText(pDATA->m_strBoxTypeOption) ;					
					cell.SetText(pDATA->m_strBoxType) ;
				}
				break;
			case 4:
				{
					cell.SetNumber(pDATA->m_nCoreNumI);
				}
				break;
			case 5:
				{
					cell.SetNumber(pDATA->m_nCoreNumII);
				}
				break;
			case 6:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("上\n下\n左\n右\n")) ;					
					cell.SetText(pDATA->m_strBoxPos) ;
				}
				break;
			case 7:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bHasDDUnit?_T("是"):_T("否")) ;
				}
				break;
			case 8:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsAlu?_T("是"):_T("否")) ;
				}
				break;
			case 9:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bHasPhone?_T("是"):_T("否")) ;
				}
				break;
			case 10:
				{
					cell.SetNumber(pDATA->m_nDSNum);
				}
				break;
			case 11:
				{
					cell.SetNumber(pDATA->m_nSpeedLimit);
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			}
			SetCell(0, i, &cell) ;			
		}
	}
	//else if (strClassName== _T("class CGdDATA"))
	else if (strClassName == _T("CGdDATA"))
	{
		nCols = 1 ;
		nRows = 13 ;
		CGdDATA *pDATA = dynamic_cast<CGdDATA*>(m_pEntDATA) ;

		SetNumberRows( nRows );
		SetNumberCols( nCols );

		for (int i=0; i<nRows; i++)
		{	
			GetCellIndirect( -1, i, &cell );
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetText(pDATA->strGdItem[i]) ;
			SetCell(-1, i, &cell) ;			

			GetCellIndirect( 0, i, &cell );
			cell.ClearAll() ;
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetBackColor(RGB(240,240,240));
			switch (i)
			{
			case 0:
				{
					cell.SetText(pDATA->m_strName) ;
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 1:
				{
					cell.SetText(pDATA->m_strCircuitType) ;
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 2:
				{
					cell.SetText(pDATA->m_str25HzDev) ;
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 3:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsDH?_T("是"):_T("否")) ;
				}
				break;
			case 4:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsDMH?_T("是"):_T("否")) ;
				}
				break;
			case 5:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIs4Line?_T("是"):_T("否")) ;
				}
				break;
			case 6:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsShareXB?_T("是"):_T("否")) ;
				}
				break;
			case 7:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIs1FnJ?_T("是"):_T("否")) ;
				}
				break;
			case 8:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsMainSec?_T("是"):_T("否")) ;
				}
				break;
			case 9:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("无\n送电\n受电\n")) ;	
					CString strTmp  = _T("无") ;
					if (pDATA->m_strLeftFOrS==_T("F"))
					{
						strTmp = _T("送电") ;
					}
					else if (pDATA->m_strLeftFOrS==_T("S"))
					{
						strTmp = _T("受电") ;
					}
					cell.SetText(strTmp) ;
				}
				break;
			case 10:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("无\n送电\n受电\n")) ;	
					CString strTmp  = _T("无") ;
					if (pDATA->m_strRightFOrS==_T("F"))
					{
						strTmp = _T("送电") ;
					}
					else if (pDATA->m_strRightFOrS==_T("S"))
					{
						strTmp = _T("受电") ;
					}
					cell.SetText(strTmp) ;
				}
				break;
			case 11:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetParam(CELLTYPE_IS_EDITABLE);
					cell.SetLabelText(_T("1700-1\n1700-2\n2300-1\n2300-2\n2000-1\n2000-2\n2600-1\n2600-2\n")) ;	
					cell.SetText(pDATA->m_strFreqLeft) ;
				}
				break;
			case 12:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;	
					cell.SetParam(CELLTYPE_IS_EDITABLE);
					cell.SetLabelText(_T("1700-1\n1700-2\n2300-1\n2300-2\n2000-1\n2000-2\n2600-1\n2600-2\n")) ;	
					cell.SetText(pDATA->m_strFreqRight) ;
				}
				break;
			}
			if (m_iItemIsNull[i]==1)
			{
				cell.SetText(_T("*多种*")) ;
			}
			SetCell(0, i, &cell) ;			
		}
	}
// 	else if (strClassName== _T("class CDcDATA"))
	else if (strClassName == _T("CDcDATA"))
	{
		nCols = 1 ;
		nRows = 19 ;
		CDcDATA *pDATA = dynamic_cast<CDcDATA*>(m_pEntDATA) ;

		SetNumberRows( nRows );
		SetNumberCols( nCols );

		for (int i=0; i<nRows; i++)
		{	
			GetCellIndirect( -1, i, &cell );
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetText(pDATA->strDcItem[i]) ;
			SetCell(-1, i, &cell) ;			

			GetCellIndirect( 0, i, &cell );			
			cell.ClearAll() ;
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			cell.SetBackColor(RGB(240,240,240));
			switch (i)
			{
			case 0:
				{
					cell.SetText(pDATA->m_strName) ;//道岔名称取自块属性
				}
				break;
			case 1:
				{
					cell.SetNumber(pDATA->m_nLoc);
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 2:
				{
					cell.SetNumber(pDATA->m_nJG_QyNum);
					int iDragNum = CDcDATA::GetDrag(pDATA->m_strJG_QyType) ;
					if (iDragNum==-1)
					{
						cell.SetParam(CELLTYPE_IS_EDITABLE);
					}
				}
				break;
			case 3:
				{
					cell.ClearAll() ;
					cell.SetParam(DROPLIST_JGDRAG_LOAD);
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("ZD6-D\nZD6-EJ\nZD6-EJJ\nZYJ7\nZYJ7+SH6\nZYJ7+SH6+SH6\nS700K\n")) ;		
					cell.SetText(pDATA->m_strJG_QyType) ;
				}
				break;
			case 4:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bJG_HasMT?_T("是"):_T("否")) ;
				}
				break;
			case 5:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("上\n下\n")) ;					
					cell.SetText(pDATA->m_strJG_QyPos) ;
				}
				break;
			case 6:
				{					
					cell.SetNumber(pDATA->m_nXG_QyNum);
					int iDragNum = CDcDATA::GetDrag(pDATA->m_strXG_QyType) ;
					if (iDragNum==-1)
					{
						cell.SetParam(CELLTYPE_IS_EDITABLE);
					}
				}
				break;
			case 7:
				{
					cell.ClearAll() ;
					cell.SetParam(DROPLIST_XGDRAG_LOAD);
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("无\nZYJ7\nZYJ7+SH6\nZYJ7+SH6+SH6\nS700K\n")) ;		
					cell.SetText(pDATA->m_strXG_QyType) ;
				}
				break;
			case 8:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bXG_HasMT?_T("是"):_T("否")) ;
				}
				break;
			case 9:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("上\n下\n")) ;	
					cell.SetText(pDATA->m_strXG_QyPos) ;
				}
				break;
			case 10:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("HZ24\nHZ12\n")) ;	
					cell.SetText(pDATA->m_strBoxType) ;
				}
				break;
			case 11:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsSingleAct?_T("是"):_T("否")) ;
				}
				break;
			case 12:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bIsComplex?_T("是"):_T("否")) ;
				}
				break;
			case 13:
				{		
					cell.SetText(pDATA->m_strRelationDc) ;
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 14:
				{
					cell.SetNumber(pDATA->m_nActIndex);
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 15:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bHasPhone?_T("是"):_T("否")) ;
				}
				break;
			case 16:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("否\n是\n")) ;					
					cell.SetText(pDATA->m_bHasDCQKBJ?_T("是"):_T("否")) ;
				}
				break;
			case 17:
				{
					cell.SetNumber(pDATA->m_nSpeedLimit);
					cell.SetParam(CELLTYPE_IS_EDITABLE);
				}
				break;
			case 18:
				{
					cell.ClearAll() ;
					cell.SetCellType(UGCT_DROPLIST) ;
					cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON) ;		
					cell.SetLabelText(_T("弯股切割\n直股切割\n")) ;					
					cell.SetText(pDATA->m_iCutJYJPos?_T("直股切割"):_T("弯股切割")) ;
				}
				break;
			}
			SetCell(0, i, &cell) ;			
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
void CXhEntUGCtrl::OnGetCell(int col,long row,CUGCell *cell)
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
void CXhEntUGCtrl::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
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

int CXhEntUGCtrl::OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param){

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

int CXhEntUGCtrl::OnDropList(long ID,int col,long row,long msg,long param){

	CUGCell cell;
	GetCell(col,row,&cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();

	if(nParam==DROPLIST_JGDRAG_LOAD||nParam==DROPLIST_XGDRAG_LOAD)
	{
		if(msg == UGCT_DROPLISTPOSTSELECT)
		{
			int nCol_QyNum = 0, nRow_QyNum = (nParam==DROPLIST_JGDRAG_LOAD?2:6) ;
			CString strQyType = cell.GetText();
			int iDragNum = CDcDATA::GetDrag(strQyType) ;
			if (iDragNum>=0)
			{
				CString strDragNum ;
				strDragNum.Format(_T("%d"), iDragNum) ;
				QuickSetText(nCol_QyNum, nRow_QyNum, strDragNum) ;
				SetCellParam(nCol_QyNum, nRow_QyNum, CELLTYPE_ISNOT_EDITABLE) ;
			}
			else if (iDragNum==-1)
			{
				SetCellParam(nCol_QyNum, nRow_QyNum,CELLTYPE_IS_EDITABLE) ;
			}
			this->m_CUGGrid->Update() ;
		}
	}

	return TRUE;
}

int CXhEntUGCtrl::SetCellParam(int nCol, int nRow, int nParam)
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
void CXhEntUGCtrl::SetupSection(long nRow, LPCTSTR lpcstrTitle)
{
	CUGCell cell;

	cell.SetBackColor(RGB(240,240,240));
	cell.SetTextColor(RGB(0,0,128));
	cell.SetFont(&m_boldFont);
	cell.SetAlignment(UG_ALIGNVCENTER);
	QuickSetRange(0,nRow,GetNumberCols() - 1,nRow,&cell);

	QuickSetText(-1,nRow,lpcstrTitle);
}

double CXhEntUGCtrl::QuickGetNumber(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber();
}

int CXhEntUGCtrl::QuickGetNumber(int nCol, int nRow, int& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
int CXhEntUGCtrl::QuickGetNumber(int nCol, int nRow, double& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
BOOL CXhEntUGCtrl::QuickGetBool(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetBool();
}

// -----------------------------------------------------------------------------
int CXhEntUGCtrl::OnEditFinish(int  col, long  row, CWnd * edit, LPCTSTR  string, BOOL  cancelFlag)
{
	int retCode =CUGCtrl::OnEditFinish (col, row, edit, string, cancelFlag) ;
	if (m_pEntDATA->GetClassType()==_T("CDcDATA"))
	{
		if (col==0&&row==13)
		{
			CString strDc = QuickGetText(0, 0) ;
			CZnSxt sxt ;
			sxt.SetAcDbDB(m_pDb) ;
			if (strDc!=_T("")&&_tcscmp(string, _T(""))!=0 )
			{
				QuickSetText(0, 11, _T("否")) ;
				if (!sxt.UpdataConDc(strDc, string))
				{
					return 0 ;
				}
				acutPrintf(_T("\n编辑了道岔%s的多动关联道岔，更新其关联道岔%s的属性"), strDc, string) ;
			}
			else if (strDc!=_T("")&&_tcscmp(string, _T(""))==0 )
			{
				QuickSetText(0, 11, _T("是")) ;
				if (!sxt.UpdataConDc(strDc, m_strCellPreText, 1))
				{
					return 0 ;
				}
				acutPrintf(_T("\n删除了道岔%s的多动关联道岔，更新其关联道岔%s的属性"), strDc, m_strCellPreText) ;

			}
			this->m_CUGGrid->Update() ;

		}
	}
	return (retCode) ;
}

// -----------------------------------------------------------------------------
int CXhEntUGCtrl::OnEditStart(int  col, long  row, CWnd ** edit)
{
	m_strCellPreText = QuickGetText(col, row) ;

	int retCode =CUGCtrl::OnEditStart (col, row, edit) ;
	return (retCode) ;
}
