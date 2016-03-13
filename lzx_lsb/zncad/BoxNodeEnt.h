#pragma once
#include "boxnodebase.h"

class CBoxNodeEnt :	public CBoxNodeBase
{
public:
	CBoxNodeEnt(void);
	~CBoxNodeEnt(void);
	CBoxNodeEnt(const CString& strBoxType, const CString& strDevName, const CString& strDevType, int iEntType=-1, long nHandleBox=0, long nHandleDev=0, const CString& strBoxBlock=_T(""), const CString& strDevBlock=_T(""),  const CString& strCableString=_T(""), int iDistToXhl=0 ) ;

	long m_nHandleBox;
	long m_nHandleDev;
	CString m_strBoxBlock;
	CString m_strDevBlock;
	CString m_strCableString;
	int m_iDistToXhl;

	CString m_strFreq ;

	int m_iBoxSum ;
	int m_iBoxIndex ;

	CString m_strDragType;
	int m_iDragIndex ;
	CString m_strDcQyType;
	CString m_strActIndex ;
	long m_nIdPlus;
	//1����죬2��о��
	int m_iJxg;

	// ת�޻���о���ȣ����ź�¥��Զ�˵ģ�
	int m_iLenForPlus;
	//��к�next�����ź�¥������м������
	long m_nHandleCable ;
};
