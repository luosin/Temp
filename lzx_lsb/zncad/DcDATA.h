#pragma once
#include "entdata.h"

class CDcDATA :public CEntDATA
{
public:
	CDcDATA(void);
	virtual ~CDcDATA(void);

	//����꣬���뷽ʽ��K000+000
	int m_nLoc;
	//���ǣ��������ȱʡֵΪ1��
	int m_nJG_QyNum ;
	//���ǣ����ʽ��ZD6-D��ZD6-EJ��ZD6-EJJ��ZYJ7��ZYJ7-SH6��S700K����ȱʡֵ��
	CString m_strJG_QyType ;
	//����Ƿ��������������ȱʡֵΪ��
	BOOL m_bJG_HasMT ;
	//���ǣ����λ�ڵ���ȱʡֵΪ�£�
	CString m_strJG_QyPos ;
	//о��ǣ��������ȱʡֵΪ0��
	int m_nXG_QyNum ;
	//о��ǣ����ʽ��ZYJ7��ZYJ7-SH6��S700K����ȱʡֵ����о��ǣ��������Ϊ0ʱ�д��
	CString m_strXG_QyType ;
	//о���Ƿ��������������ȱʡֵΪ��
	BOOL m_bXG_HasMT ;
	//о��ǣ����λ�ڵ���ȱʡֵΪ�£�
	CString m_strXG_QyPos ;
	//�������XB1/XB2/HZ12/HZ24��ȱʡֵΪHZ24��
	CString m_strBoxType ;
	//�Ƿ񵥶���ȱʡֵΪ�ǣ�
	BOOL m_bIsSingleAct ;
	//�Ƿ�ʽ���֣�ȱʡֵΪ�񣬵�ѡ��ǵ���ʱ�д�ѡ�
	BOOL m_bIsComplex ;
	//�ද��������
	CString m_strRelationDc ;
	//�ڼ��� ����ȱʡֵ�����ǵ������Ǹ�ʽ����ʱ�д�ѡ�
	int m_nActIndex ;
	//�Ƿ��е绰��ȱʡֵΪ�ǣ�
	BOOL m_bHasPhone ;
	//�Ƿ��е���ȱ�ڱ�����ȱʡֵΪ�ǣ�
	BOOL m_bHasDCQKBJ ;
	//����ֵ
	int m_nSpeedLimit ;
	//��Ե��ֱ����и0����ɣ�ȱʡ����1��ֱ��
	int m_iCutJYJPos ;

	static const CString strDcItem[19];

	// ����ǣ����ʽ�ó�ÿһ��ǣ��
	int GetDrag(CStringArray& strArrayDrag,  int iJgOrXg=0);
	static int GetDrag(const CString& strQyType);
	// ������
	int GetActionNum(void);
};
