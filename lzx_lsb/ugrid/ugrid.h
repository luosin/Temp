// ugrid.h : ugrid DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CugridApp
// �йش���ʵ�ֵ���Ϣ������� ugrid.cpp
//

class CugridApp : public CWinApp
{
public:
	CugridApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
