#pragma once

class CBtTools
{
public:
	CBtTools(void);
	~CBtTools(void);
	// ���¼�������·��������
	int ReComputeCapability(const CString & strDir);
	BOOL GetCapInfo(const CString & strFreq, double dGuiDaoLen, double dLenPlus, CString & strCapability);
};
