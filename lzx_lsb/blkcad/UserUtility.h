// UserUtility.h: interface for the CUserUtility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERUTILITY_H__4169C3FC_666C_4019_A203_0CBBE9644AA8__INCLUDED_)
#define AFX_USERUTILITY_H__4169C3FC_666C_4019_A203_0CBBE9644AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUserUtility  
{
public:
	CUserUtility();
	virtual ~CUserUtility();

	// 批量设置扩展数据（从数据库读取）
	static int SetXdataSS(void);

};

#endif // !defined(AFX_USERUTILITY_H__4169C3FC_666C_4019_A203_0CBBE9644AA8__INCLUDED_)
