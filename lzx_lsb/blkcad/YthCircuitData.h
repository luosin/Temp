// YthCircuitData.h: interface for the CYthCircuitData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTHCIRCUITDATA_H__CC8BC91D_971F_407A_866E_81E35AB265E0__INCLUDED_)
#define AFX_YTHCIRCUITDATA_H__CC8BC91D_971F_407A_866E_81E35AB265E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYthCircuitData  
{
public:
	CString m_strDate;
	CString m_strSN;
	CString m_strCircuitType;
	CString m_strStationName;
	CString m_strFsJsStartNode2;
	CString m_strFsJsStartNode1;
	CString m_strFJNodePre;
	CString m_strGJNodePre;
	CString m_strBackGName;
	CString m_strFrontGName;
	int m_iSideRNode;
	int m_iSideRName;
	int m_iSideFsJsNum;
	CString m_strSideYNum;
	CString m_strSideGName;
	int m_iCurRNode;
	int m_iCurRName;
	CString m_strCurZhNum;
	CString m_strCurZNum;
	int m_iCurFsJsNum;
	CString m_strCurFreq;
	CString m_strCurYNum;
	CString m_strCurGName;
	CYthCircuitData();
	virtual ~CYthCircuitData();

};

#endif // !defined(AFX_YTHCIRCUITDATA_H__CC8BC91D_971F_407A_866E_81E35AB265E0__INCLUDED_)
