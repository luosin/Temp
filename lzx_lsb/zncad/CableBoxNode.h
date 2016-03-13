#pragma once
#include "CableCore.h"

class CCableBoxNode
{
public:
	CCableBoxNode(void);
	~CCableBoxNode(void);
	CCableBoxNode(const CString& strBoxType, const CString& strDevName, const CString& strDevType, const CString& strFreq);

	CString m_strBoxType;
	CString m_strDevName;
	CString m_strDevType;
	std::vector<CCableCore> m_vecCableCore ;
	// ��ѯһ��о���Ƿ���ڣ�ֻҪdevname��corename��ͬ����Ϊͬһо
	bool FindCableCore(const CCableCore& cableCore);
	CString m_strFreq;
};
