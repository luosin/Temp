#pragma once
#include "entdata.h"

class CXhjDATA :public CEntDATA
{
public:
	CXhjDATA(void);
	CXhjDATA(const CString& strBlkName);
	virtual ~CXhjDATA(void);	

	//�źŻ�����
	CString m_strBlkName;
	// ����꣬ȱʡΪ��
	int m_nLoc;
	// �Ƿ��г���ȱʡΪ��
	BOOL m_bIsTrain;
	//����������ͣ�XB1/XB2/HZ12/HZ24��ȱʡֵΪXB1��
	CString m_strBoxType ; 
	// �������Iʵ��ʹ��о��������ȱʡֵ����ע����Ϊ������ʱ����Ҫ�ֱ����루���I�����II��..������������˿�͵绰о����
	int m_nCoreNumI ; 
	int m_nCoreNumII ;
	//���λ���źŻ���0/��1/��2/��3��ȱʡֵΪ�źŻ��£�
	CString m_strBoxPos ; 
	//�Ƿ���õ�Ƶ�Ԫ��ȱʡֵΪ�ǣ�
	BOOL m_bHasDDUnit ;
	//�Ƿ�Ϊ���Ͻ������ȱʡֵΪ�ǣ�
	BOOL m_bIsAlu; 
	 //�Ƿ��е绰��ȱʡֵΪ�ǣ�
	BOOL m_bHasPhone ;
	//��˿����о�������޵�˿������2��3��ȱʡֵΪ2
	int m_nDSNum ; 
	 //����ֵ����ȱʡֵ����������ʱ�д�ѡ�
	int m_nSpeedLimit ;
	static const CString strXhjItem[12];
	BOOL Init(void);
	CString m_strBoxTypeOption;
};
