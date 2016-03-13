#pragma once
#include "entdata.h"

class CXhjDATA :public CEntDATA
{
public:
	CXhjDATA(void);
	CXhjDATA(const CString& strBlkName);
	virtual ~CXhjDATA(void);	

	//信号机块名
	CString m_strBlkName;
	// 公里标，缺省为空
	int m_nLoc;
	// 是否列车，缺省为否
	BOOL m_bIsTrain;
	//采用箱盒类型，XB1/XB2/HZ12/HZ24（缺省值为XB1）
	CString m_strBoxType ; 
	// 进入箱盒I实际使用芯线数（无缺省值）；注：当为多个箱盒时，需要分别输入（箱盒I、箱盒II…..），不包含灯丝和电话芯线数
	int m_nCoreNumI ; 
	int m_nCoreNumII ;
	//箱盒位于信号机上0/下1/左2/右3（缺省值为信号机下）
	CString m_strBoxPos ; 
	//是否采用点灯单元（缺省值为是）
	BOOL m_bHasDDUnit ;
	//是否为铝合金机构（缺省值为是）
	BOOL m_bIsAlu; 
	 //是否有电话（缺省值为是）
	BOOL m_bHasPhone ;
	//灯丝报警芯线数，无灯丝报警、2、3，缺省值为2
	int m_nDSNum ; 
	 //限速值（无缺省值，当有限速时有此选项）
	int m_nSpeedLimit ;
	static const CString strXhjItem[12];
	BOOL Init(void);
	CString m_strBoxTypeOption;
};
