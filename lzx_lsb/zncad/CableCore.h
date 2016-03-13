#pragma once

class CCableCore
{
public:
	CCableCore(void);
	~CCableCore(void);
	CCableCore(const CString& strDevName, const CString& strCoreName);
	CCableCore(const CString& strDevName, const CString& strCoreName, const CString& strCoreNameShow, CString strFreq=_T(""), int iIsInside=0, int iIsShare=0, int iIsAdd=0);
	
	CString m_strDevName;
	CString m_strCoreName;
	CString m_strCoreNameShow;	
	CString m_strFreq;

	int m_iIsInside;
	int m_iIsShare;
	int m_iIsAdd;	

	int IsInside(void);
	int IsShare(void);
	int IsAdd(void);
	bool IsSameCore(const CCableCore& cableCore);
};
