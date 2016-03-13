#pragma once
#include "entdata.h"

class CGdDATA :public CEntDATA
{
public:
	CGdDATA(void);
	virtual ~CGdDATA(void);
	// ��·����
	CString m_strCircuitType;
	// 25Hz�����·�豸
	CString m_str25HzDev;
	// �Ƿ�绯
	BOOL m_bIsDH;
	// �Ƿ���뻯
	BOOL m_bIsDMH;
	// �Ƿ�4����
	BOOL m_bIs4Line;
	// �Ƿ���ñ�ѹ����
	BOOL m_bIsShareXB;
	// �Ƿ�1�Ͷ���
	BOOL m_bIs1FnJ;
	// �Ƿ�������
	BOOL m_bIsMainSec;
	// ������͵�F/�ܵ�S
	CString m_strLeftFOrS;
	// �Ҳ����͵�F/�ܵ�S
	CString m_strRightFOrS;
	// ��Ƶ��ࣨ���з���
	CString m_strFreqLeft;
	// ��Ƶ�Ҳࣨ���з���
	CString m_strFreqRight;

	static const CString strGdItem[13];
};
