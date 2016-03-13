#pragma once

class CBtTools
{
public:
	CBtTools(void);
	~CBtTools(void);
	// 重新计算轨道电路补偿电容
	int ReComputeCapability(const CString & strDir);
	BOOL GetCapInfo(const CString & strFreq, double dGuiDaoLen, double dLenPlus, CString & strCapability);
};
