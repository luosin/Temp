#pragma once
#include "entdata.h"

class CGdDATA :public CEntDATA
{
public:
	CGdDATA(void);
	virtual ~CGdDATA(void);
	// 电路类型
	CString m_strCircuitType;
	// 25Hz轨道电路设备
	CString m_str25HzDev;
	// 是否电化
	BOOL m_bIsDH;
	// 是否电码化
	BOOL m_bIsDMH;
	// 是否4线制
	BOOL m_bIs4Line;
	// 是否合用变压器箱
	BOOL m_bIsShareXB;
	// 是否1送多受
	BOOL m_bIs1FnJ;
	// 是否主区段
	BOOL m_bIsMainSec;
	// 左侧是送电F/受电S
	CString m_strLeftFOrS;
	// 右侧是送电F/受电S
	CString m_strRightFOrS;
	// 载频左侧（上行方向）
	CString m_strFreqLeft;
	// 载频右侧（下行方向）
	CString m_strFreqRight;

	static const CString strGdItem[13];
};
