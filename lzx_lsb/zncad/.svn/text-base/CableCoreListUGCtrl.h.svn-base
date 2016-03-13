#pragma once
#include "ugctrl.h"
#include "CableCore.h"

class CCableCoreListUGCtrl : public CUGCtrl
{
	DECLARE_DYNAMIC(CCableCoreListUGCtrl)

public:
	CCableCoreListUGCtrl();
	virtual ~CCableCoreListUGCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
		//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	//GetCellIndirect notification
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	//cell type notifications
	virtual int OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	void SetupSection(long nRow, LPCTSTR lpcstrTitle);

	//fonts
	CFont	m_defFont;
	CFont	m_boldFont;
	CFont	m_medFont;
	CFont	m_medBoldFont;
	CFont	m_lrgFont;
	CFont	m_smFont;
	double QuickGetNumber(int nCol, int nRow);
	int	QuickGetNumber(int nCol,int nRow, int& number);
	//int	QuickGetNumber(int nCol, int nRow, long& number);
	//int	QuickGetNumber(int nCol, int nRow, float& number);
	int	QuickGetNumber(int nCol, int nRow, double& number);
	BOOL QuickGetBool(int nCol, int nRow);
	int OnDropList(long ID,int col,long row,long msg,long param);

	int SetCellParam(int nCol, int nRow, int nParam) ;

	// -----------------------------------------------------------------------------
	virtual int OnEditFinish(int  col, long  row, CWnd * edit, LPCTSTR  string, BOOL  cancelFlag);
	// -----------------------------------------------------------------------------
	virtual int OnEditStart(int  col, long  row, CWnd ** edit);

	std::vector<CCableCore> m_vecCableCore ;

};


