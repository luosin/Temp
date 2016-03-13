#include "stdafx.h"
#include "resource.h"
#include "LKDataTable.h"
#include "DBUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//查询道岔 信息
int CLKDataTable::GetDcInfo(const CString strDcName, long &iDcLoc)
{
	int iRet = -1 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		
		strSql.Format(_T("select * from import_dc_info where name='%s' and belong_station='%s'"), strDcName, m_strCurStation) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSetFind.GetRecordCount() ;
			if (iRsCount>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(_T("loc_num"), iDcLoc) ;
				cSetFind.GetFieldValue(_T("up_or_down"), iRet) ;
			}
			cSetFind.Close(); 
		}
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetDcInfo")) ;

	}
	
	return iRet ;
}

//查询信号机（进出站信号机）for进路数据表
int CLKDataTable::GetXhjInfo(const CString strXhjName, int &iUpOrDown, long &iXhjLoc, int &iType, int &iIsReverse)
{
	int iRet = -1 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		
		strSql.Format(_T("select * from import_xhj_info where name='%s' and belong_station='%s'"), strXhjName, m_strCurStation) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			iRet = cSetFind.GetRecordCount() ;
			if (iRet>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
				cSetFind.GetFieldValue(_T("loc_num"), iXhjLoc) ;
				cSetFind.GetFieldValue(_T("type"), iType) ;
				cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
			}
			cSetFind.Close(); 
		}
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetXhjInfo")) ;

	}
	
	return iRet ;
	
}


//查询类似轨道1DG,轨道长度，频率，状态（1：道岔反位，0：道岔定位）
int CLKDataTable::GetGdInfo(const CString strGdName, const CString strDcList, int &iDcCase, long &iGdLen, CString &strFreq)
{
	int iRet = 0 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		int iFindMark = strGdName.Find(_T("-")) ;
		CString strReverseStr = _T("") ; //反位道岔字符串,查询import_gdsection_info时用
		if (strGdName.Right(2)==_T("DG"))
		{
			if (iFindMark==-1)//112DG形式
			{
				CString strDcName = strGdName.Left(strGdName.GetLength()-2) ;
				
				for (int i=0; i<3; i++)
				{
					CString strToFind = _T("") ;
					switch (i)
					{
					case 0:
						strToFind.Format(_T("(%s)"), strDcName) ;
						break;
					case 1:
						strToFind.Format(_T("(%s/"), strDcName) ;
						break;
					case 2:
						strToFind.Format(_T("/%s)"), strDcName) ;
						break;
					}
					if(strDcList.Find(strToFind)!=-1)
					{
						iDcCase = 1 ;
						strReverseStr = strDcName ;						
						break ;
					}
				}
			}
		} //if (strGd.Right(2)==_T("DG"))

		strSql.Format(_T("select * from import_gdsection_info where gd_name='%s' and reversedc_name='%s' and belong_station='%s'"), strGdName, strReverseStr, m_strCurStation) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			iRet = cSetFind.GetRecordCount() ;
			if (iRet>0)
			{
				cSetFind.MoveFirst() ;
				
				cSetFind.GetFieldValue(_T("freq"), strFreq) ;
				cSetFind.GetFieldValue(_T("section_len"), iGdLen) ;
			}
			cSetFind.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGdInfo")) ;

	}

	return iRet ;	
}

//查询类似111-113轨道信息
//查询轨道长度，频率，状态（10+2=12：反、定，10+1=11：定、反，10+0=13：定、定）
//iToward=-1，此参数不作用
//iToward=0,表示坐标从小到大，作用于111-113DG形式的轨道，分界成道岔111、113后，strDcName1为坐标较小的道岔，strDcName2反之
//iToward=1,表示坐标从大到小，作用于111-113DG形式的轨道，分界成道岔111、113后，strDcName1为坐标较大的道岔，strDcName2反之

int CLKDataTable::GetGdInfo(const CString strGdName, const CString strDcList, int &iDcCase, long &iGdLen, CString &strFreq, long &iDcLoc1, long &iDcLoc2, CString &strDcName1, CString &strDcName2, int iToward)
{
	int iRet = 0 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		int iFindMark = strGdName.Find(_T("-")) ;
		CString strReverseStr = _T("") ; //反位道岔字符串,查询import_gdsection_info时用
		CString strDcName1_Back, strDcName2_Back ;
		if (strGdName.Right(2)==_T("DG"))
		{
			if (iFindMark!=-1)//111-113DG形式
			{
				iDcCase = 10 ;
				//CString strDcName1, strDcName2 ;
				strDcName1 = strGdName.Left(iFindMark) ;
				strDcName2 = strGdName.Mid(iFindMark+1, strGdName.GetLength()-iFindMark-3) ; //字符总长-strDcName1-"-"-"DG"
				strDcName1_Back = strDcName1 ;
				strDcName2_Back = strDcName2 ;

				if (iToward!=-1)
				{
					long iDcLocTmp = 0 ;
					CString strDcNameTmp = _T("") ;
					this->GetDcInfo(strDcName1, iDcLoc1) ;
					this->GetDcInfo(strDcName2, iDcLoc2) ;

					if (iToward==0)
					{
						if (iDcLoc1>iDcLoc2)
						{
							iDcLocTmp = iDcLoc1 ;
							iDcLoc1 = iDcLoc2 ;
							iDcLoc2 = iDcLocTmp ;

							strDcNameTmp = strDcName1 ;
							strDcName1 = strDcName2 ;
							strDcName2 = strDcNameTmp ;
						}
					}
					else
					{
						if (iDcLoc1<iDcLoc2)
						{
							iDcLocTmp = iDcLoc2 ;
							iDcLoc2 = iDcLoc1 ;
							iDcLoc1 = iDcLocTmp ;
							
							strDcNameTmp = strDcName2 ;
							strDcName2 = strDcName1 ;
							strDcName1 = strDcNameTmp ;
						}						
					}
				}

				for (int i=0; i<2; i++)
				{
					for (int j=0; j<3; j++)
					{
						CString strToFind = _T("") ;
						switch (j)
						{
						case 0:
							strToFind.Format(_T("(%s)"), i==0?strDcName1:strDcName2) ;
							break;
						case 1:
							strToFind.Format(_T("(%s/"), i==0?strDcName1:strDcName2) ;
							break;
						case 2:
							strToFind.Format(_T("/%s)"), i==0?strDcName1:strDcName2) ;
							break;
						}
						if(strDcList.Find(strToFind)!=-1)
						{
							if (strReverseStr==_T(""))
							{
								strReverseStr = (i==0?strDcName1:strDcName2) ;
							}
							else
							{
								strReverseStr.Insert(555, _T(",")) ;
								strReverseStr.Insert(555, i==0?strDcName1:strDcName2) ;
							}
							iDcCase+=(i==0?2:1) ;
							//bFindN = TRUE ;
							break ;
						}
					}
				}//end for int i=0
			} //else //111-113DG形式
		} //if (strGd.Right(2)==_T("DG"))
		
		//前面可能颠倒了strName1、strName2的顺序
		if (iToward!=-1)
		{			
			if (strReverseStr.Find(_T(","))!=-1)
			{
				strReverseStr.Format(_T("%s,%s"), strDcName1_Back, strDcName2_Back) ;
			}
		}
		
		strSql.Format(_T("select * from import_gdsection_info where gd_name='%s' and reversedc_name='%s' and belong_station='%s'"), strGdName, strReverseStr, m_strCurStation) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			iRet = cSetFind.GetRecordCount() ;
			if (iRet>0)
			{
				cSetFind.MoveFirst() ;
				
				cSetFind.GetFieldValue(_T("freq"), strFreq) ;
				cSetFind.GetFieldValue(_T("section_len"), iGdLen) ;
			}
			cSetFind.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGdInfo")) ;

	}

	return iRet ;
	
}


//查询非DG的轨道信息
int CLKDataTable::GetGdInfo(const CString strGdName, long &iLen1, long &iLen2, CString &strFreq1, CString &strFreq2, int &iUpOrDown)
{
	int iRet = 0 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		strSql.Format(_T("select * from import_gdsection_info where gd_name='%s' and belong_station='%s'"), strGdName, m_strCurStation) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{	
			iRet = cSetFind.GetRecordCount() ;
			if (iRet>0)
			{
				cSetFind.MoveFirst() ;
				
				cSetFind.GetFieldValue(_T("freq"), strFreq1) ;
				cSetFind.GetFieldValue(_T("section_len"), iLen1) ;
				cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
			}
			cSetFind.Close() ;
		}

		//没有找到类似3G的长度，试试能否找到3G1 3G2
		if (iRet==0)
		{
			for (int i=0; i<2; i++)
			{
				CString strGdTmp ;
				strGdTmp.Format(_T("%s%d"), strGdName, i+1) ;
				strSql.Format(_T("select * from import_gdsection_info where gd_name='%s' and belong_station='%s'"), strGdTmp, m_strCurStation) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{					
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						
						int iGdLen = 0 ;
						CString strFreq = _T("") ;
						cSetFind.GetFieldValue(_T("freq"), strFreq) ;
						cSetFind.GetFieldValue(_T("section_len"), iGdLen) ;
						cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
						if (i==0)
						{
							iLen1 = iGdLen ;
							strFreq1 = strFreq ;
						}
						else
						{
							iLen2 = iGdLen ;
							strFreq2 = strFreq ;
						}
						iRet++ ;
						
					}
					cSetFind.Close() ;
				}				
			}
		}		

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGdInfo")) ;

	}
	
	return iRet ;
	
}

//计算出互为双动的道岔
void CLKDataTable::ComputeDc()
{
	CUserSystem mysys ;
	CDBUtility::ClearTable(_T("compute_dc")) ;
	
	try
	{
		CString strSql ;
		CADORecordset cSet ;
		
		strSql.Format(_T("select * from import_compute_lsb")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iJlCount = cSet.GetRecordCount() ;
			
			CString strBarMsg ;
			strBarMsg.Format(_T("处理道岔")) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iJlCount-1) ;	

			int iJlIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iJlIndex) ;

				
				CString strDcList = _T("") ;
				cSet.GetFieldValue(_T("dc_list"), strDcList) ;
				//begin dc_list_new
				CStringArray strArrayDcList ;
				CBlkUtility::DivideString(strDcList, _T(","), strArrayDcList) ;
				for(int iDcIndex=0; iDcIndex<strArrayDcList.GetSize(); iDcIndex++)
				{
					CString strDc = strArrayDcList.GetAt(iDcIndex) ;
					strDc.TrimLeft() ;
					strDc.TrimRight() ;					
					if (CBlkUtility::IsStrMatch(_T("^[\\(,\\{]?(\\d+)/(\\d+)[\\),\\}]?$"), strDc))
					{	
						CString strDc1 = _T(""), strDc2 = _T("") ;
						CBlkUtility::RegReplace(_T("^[\\(,\\{]?(\\d+)/(\\d+)[\\),\\}]?$"), _T("$1"), strDc, strDc1) ;
						CBlkUtility::RegReplace(_T("^[\\(,\\{]?(\\d+)/(\\d+)[\\),\\}]?$"), _T("$2"), strDc, strDc2) ;
						
						CADORecordset cSetFind ;
						strSql.Format(_T("select * from compute_dc where name in('%s','%s')"), strDc1, strDc2) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (cSetFind.GetRecordCount()==0)
							{
								strSql.Format(_T("insert into compute_dc(name,other_name) values('%s','%s')"), strDc1, strDc2) ;
								ExecSqlString(strSql) ;
								strSql.Format(_T("insert into compute_dc(name,other_name) values('%s','%s')"), strDc2, strDc1) ;
								ExecSqlString(strSql) ;
							}
							cSetFind.Close() ;
						}
					}
				}				
				//end dc_list_new
				
				cSet.MoveNext() ;
			}
			

			acedRestoreStatusBar() ;

			cSet.Close() ;
			
		}
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeDc")) ;

	}
	
}

//找到和本道岔互为双动的道岔
int CLKDataTable::GetOtherDc(const CString strDc, CString &strOtherDc)
{
	int iRet = 0 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		
		strSql.Format(_T("select * from compute_dc where name='%s'"), strDc) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{	
			iRet = cSetFind.GetRecordCount() ;
			if (iRet>0)
			{
				cSetFind.MoveFirst() ;
				
				cSetFind.GetFieldValue(_T("other_name"), strOtherDc) ;
			}
			cSetFind.Close() ;
		}	
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetOtherDc")) ;

	}
	
	return iRet ;
	
}

//计算一段长度的速度字符串
void CLKDataTable::GetSpeedStr(long iStartLoc, long iSumLen, CString &strRet, int iUpOrDown, int iPlus)
{	
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		iUpOrDown = iUpOrDown%10 ;
		
		if (iUpOrDown==-1)
		{
			if (strRet==_T(""))
			{
				strRet.Format(_T("80/%d"), iSumLen) ;

			}
			else
			{
				CString strTmp  = _T("") ;
				strRet.Insert(555, _T(",\n")) ;
				strTmp.Format(_T("80/%d"), iSumLen) ;
				strRet.Insert(555, strTmp) ;
			}
		}
		else
		{
			//坐标从小到大（左-》右）
			if (iPlus==1)
			{
				long iEndLoc = iStartLoc+iSumLen ;

				int iRsCount = 0 ;
				long iCurStartLoc = iStartLoc ;
				strSql.Format(_T("select * from import_speed where end_loc_num>%d and end_loc_num<%d and up_or_down=%d and is_reverse=%d order by end_loc_num asc"), iStartLoc, iEndLoc, iUpOrDown, iUpOrDown==0?0:1) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					iRsCount = cSetFind.GetRecordCount() ;
					if (iRsCount>0)
					{
						cSetFind.MoveFirst() ;

						while (!cSetFind.IsEOF())
						{
							long iEndLocNum = 0 ;
							int iSpeed = 0 ;
							cSetFind.GetFieldValue(_T("end_loc_num"), iEndLocNum) ;
							cSetFind.GetFieldValue(_T("speed"), iSpeed) ;
							long iSpeedLen = iEndLocNum - iCurStartLoc ;
							CString strTmp = _T("") ;
							if (strRet==_T(""))
							{
								strRet.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
							}
							else
							{
								strRet.Insert(555, _T(",\n")) ;
								strTmp.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
								strRet.Insert(555, strTmp) ;
							}
							iCurStartLoc = iEndLocNum ;

							cSetFind.MoveNext() ;
						}
					}
					cSetFind.Close() ;
				}
				
				strSql.Format(_T("select * from import_speed where end_loc_num>=%d and up_or_down=%d and is_reverse=%d order by end_loc_num asc"), iEndLoc, iUpOrDown, iUpOrDown==0?0:1) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{						
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;							
						
						//while (!cSetFind.IsEOF())
						//{
							long iEndLocNum = 0 ;
							int iSpeed = 0 ;
							cSetFind.GetFieldValue(_T("end_loc_num"), iEndLocNum) ;
							cSetFind.GetFieldValue(_T("speed"), iSpeed) ;
							long iSpeedLen = iEndLoc - iCurStartLoc ;
							CString strTmp = _T("") ;
							if (strRet==_T(""))
							{
								strRet.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
							}
							else
							{
								strRet.Insert(555, _T(",\n")) ;
								strTmp.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
								strRet.Insert(555, strTmp) ;
							}
							iCurStartLoc = iEndLoc ;
							
							//cSetFind.MoveNext() ;
						//}
					}
					cSetFind.Close() ;
				}
				
			}
			else if (iPlus==-1)//坐标从大到小（右-》左）
			{
				long iEndLoc = iStartLoc-iSumLen ;
				
				int iRsCount = 0 ;
				long iCurStartLoc = iStartLoc ;
				strSql.Format(_T("select * from import_speed where end_loc_num>%d and end_loc_num<%d and up_or_down=%d and is_reverse=%d order by end_loc_num desc"),  iEndLoc, iStartLoc, iUpOrDown, iUpOrDown==0?1:0) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					iRsCount = cSetFind.GetRecordCount() ;
					if (iRsCount>0)
					{
						cSetFind.MoveFirst() ;
						
						while (!cSetFind.IsEOF())
						{
							long iEndLocNum = 0 ;
							int iSpeed = 0 ;
							cSetFind.GetFieldValue(_T("end_loc_num"), iEndLocNum) ;
							cSetFind.GetFieldValue(_T("speed"), iSpeed) ;
							long iSpeedLen =  iCurStartLoc - iEndLocNum ;
							CString strTmp = _T("") ;
							if (strRet==_T(""))
							{
								strRet.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
							}
							else
							{
								strRet.Insert(555, _T(",\n")) ;
								strTmp.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
								strRet.Insert(555, strTmp) ;
							}
							iCurStartLoc = iEndLocNum ;
							
							cSetFind.MoveNext() ;
						}
					}
					cSetFind.Close() ;
				}
				
				strSql.Format(_T("select * from import_speed where end_loc_num<=%d and up_or_down=%d and is_reverse=%d order by end_loc_num desc"), iEndLoc, iUpOrDown, iUpOrDown==0?1:0) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{						
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;							
						
						//while (!cSetFind.IsEOF())
						//{
							long iEndLocNum = 0 ;
							int iSpeed = 0 ;
							cSetFind.GetFieldValue(_T("end_loc_num"), iEndLocNum) ;
							cSetFind.GetFieldValue(_T("speed"), iSpeed) ;
							long iSpeedLen =  iCurStartLoc - iEndLoc ;
							CString strTmp = _T("") ;
							if (strRet==_T(""))
							{
								strRet.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
							}
							else
							{
								strRet.Insert(555, _T(",\n")) ;
								strTmp.Format(_T("%d/%d"), iSpeed, iSpeedLen) ;
								strRet.Insert(555, strTmp) ;
							}
							iCurStartLoc = iEndLoc ;
							
							//cSetFind.MoveNext() ;
						//}
					}
					cSetFind.Close() ;
				}
			}
		}		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetSpeedStr")) ;

	}	
}


void CLKDataTable::FormatGroupSn(int iGroupSn, CString &strGroupSn)
{
	strGroupSn.Format(_T("%d%d%d"),iGroupSn/100 ,iGroupSn%100/10,iGroupSn%100%10 ) ;
}

int CLKDataTable::GetStationInfo(const CString strStationName, CString &strBigRegionSn, CString &strSmallRegionSn, CString &strStationSn)
{
	int iRet = 0 ;
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		
		strSql.Format(_T("select * from import_station_info where name='%s' order by id asc"), strStationName) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			iRet = cSetFind.GetRecordCount() ;
			if (iRet>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(_T("bigregion_sn"), strBigRegionSn) ;
				cSetFind.GetFieldValue(_T("smallregion_sn"), strSmallRegionSn) ;
				cSetFind.GetFieldValue(_T("station_sn"), strStationSn) ;
			}
			
			cSetFind.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetStationInfo")) ;

	}
	
	return iRet ;
}



int CLKDataTable::ImportStationInfo(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable("import_station_info") ;
	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ("Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}
		
		pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		
		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		CString strColName[4] = {_T("B"),_T("C"),_T("D"),_T("E")} ;
		
		int iCurRow = 3 ;
		BOOL bNext = TRUE ;
		while (bNext)
		{
			CString strStationName = _T(""), strBigRegionSn = _T(""), strSmallRegionSn = _T(""), strStationSn = _T("") ;
			
			BOOL bCurRowIsBlank = TRUE ;
			CString strBarMsg ;
			strBarMsg.Format(_T("读取车站信息表：第%d行"), iCurRow) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, 3) ;	

			
			BOOL bReadSomething = FALSE ;
			for (int iColIndex=0; iColIndex<4; iColIndex++)
			{

				acedSetStatusBarProgressMeterPos(iColIndex) ;

				CString strCellName = _T(""), strCellText = _T("") ;
				strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				strCellText = (CString)(pRange->GetText().bstrVal) ;
				strCellText.TrimLeft() ;
				strCellText.TrimRight() ;
				
				if (iColIndex==0)
				{
					if (strCellText==_T(""))
					{
						break ;
					}
					else
					{
						bReadSomething = TRUE ;
					}
				}				
				
				switch (iColIndex)
				{
				case 0:
					strStationName = strCellText ;
					break;
				case 1:
					strBigRegionSn = strCellText ;		
					break;
				case 2:
					strSmallRegionSn = strCellText ;
					break;
				case 3:
					strStationSn = strCellText ;
					break;
				default:				
					break;
				}
			}
			if (bReadSomething)
			{
				strExec.Format(_T("insert into import_station_info(name, bigregion_sn, smallregion_sn, station_sn)\
								  values('%s', '%s', '%s', '%s' )"), strStationName, strBigRegionSn, strSmallRegionSn, strStationSn) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
			}	
			
			if (bCurRowIsBlank)
			{
				bNext = FALSE ;
			}
			else
			{
				iCurRow++ ;
			}

			acedRestoreStatusBar() ;

		}			

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return iRet ;
	
}

//绘制坡度 速度 区间信号机 应答器
void CLKDataTable::DrawPdt()
{
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	//CString strMileSys = _T("DK") ;
	double dPlusForDrawX = 0.035 ;// 3.5/100 每100米的对应于cad图上3.5个单位长
	double dMileSysFirstLineY[2] ={60-22, -60+19+3} ;//0 ; //刻度尺的第一条横线Y坐标 
	double dMileSysSecondLineY[2] = {55-22, -55+19+3} ;// -5 ; //刻度尺的第二条横线Y坐标
	double dPdFirstHLineY[2] = {52.5-3+8, -42.5+3-8} ; //-7.5 ; //坡度第一条横线Y坐标
	double dPdSecondHLineY[2] = {42.5-3+8, -52.5+3-8} ;//-17.5 ; //坡度第二条横线Y坐标
	double dPlusForDrawSpeedY = 0.1 ;//0.15 ;// 6/40 每40公里的速度对应于cad图上6个单位长
	double dDrawSpeedStartY[2] = {45, -120+3} ;//-6 ; //绘制速度的起始Y坐标，即速度为0时对应的Y坐标
	double dDrawXhjY[2] = {6,-6} ; //-50 ;//绘制信号机基点Y坐标
	double dDrawStationY[2] = {28.25, -28.25+3} ;//62 ; //绘制车站基点的Y坐标：速度最大假如450，则450*0.15-6=61.5，车站绘制在速度上方
	double dBetweenUpOrDown = 135 ; //上下行之间距离
	
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		AcDbBlockReference *pRef = NULL ;
		CStringArray strArrayAtt ;

		//begin 取得线路起始里程
		long iStartLocShow = 0, iEndLocShow = 0 ; //起始点显示里程
		long iStartLocNum = 0, iEndLocNum = 0 ; //起始点里程
		long iStartRealLoc = 0, iEndRealLoc = 0 ; //起始点实际里程
		strSql.Format(_T("select * from compute_xhj_table where type in(1,11) order by loc_real asc")) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			if (!cSetFind.IsBOF())
			{
				cSetFind.MoveFirst() ;
			}	
			CString strLocTmp = _T("") ;
			cSetFind.GetFieldValue(_T("loc_show"), strLocTmp) ;
			this->GetNumOfLoc(strLocTmp, iStartLocShow) ;
			cSetFind.GetFieldValue(_T("loc_num"), iStartLocNum) ;
			cSetFind.GetFieldValue(_T("loc_real"), iStartRealLoc) ;
			cSetFind.MoveLast() ;
			cSetFind.GetFieldValue(_T("loc_show"), strLocTmp) ;
			this->GetNumOfLoc(strLocTmp, iEndLocShow) ;
			cSetFind.GetFieldValue(_T("loc_num"), iEndLocNum) ;
			cSetFind.GetFieldValue(_T("loc_real"), iEndRealLoc) ;

			cSetFind.Close() ;
		}
		//end 取得线路起始里程

		for (int iUpOrDownIndex=0; iUpOrDownIndex<2; iUpOrDownIndex++)
		{			
			//begin 绘制标尺
			//绘制标尺横线贯通直线
			mysys.Set_Layer(_T(""), _T("ruler")) ;
			AcGePoint3d pt_3d_start(iStartRealLoc*dPlusForDrawX, dMileSysFirstLineY[iUpOrDownIndex], 0) ;
			AcGePoint3d pt_3d_end(iEndRealLoc*dPlusForDrawX, dMileSysFirstLineY[iUpOrDownIndex], 0) ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_start.y = pt_3d_end.y = dMileSysSecondLineY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			mysys.Set_Layer(_T(""), _T("pd_line")) ;
			pt_3d_start.y = pt_3d_end.y = dPdFirstHLineY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_start.y = pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			

			mysys.Set_Layer(_T(""), _T("ruler")) ;
			//查询有多少个里程变换（长短链）
			CStringArray strArrayChainLocShow, strArrayChainLocNum, strArrayChainLocReal ; //保存长短链的几个里程

			int iChainRsCount = 0 ;
			strSql.Format(_T("select * from compute_xhj_table where right(name,1)='链' and up_or_down=%d order by loc_real asc"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				iChainRsCount = cSetFind.GetRecordCount() ;		
				while (!cSetFind.IsEOF())
				{
					long iLongLocNum = 0, iLongLocReal = 0 ;
				
					CString strLocTmp = _T("") ;
					
					cSetFind.GetFieldValue(_T("loc_show"), strLocTmp) ;
					//this->GetNumOfLoc(strLocTmp, iLongLocShow) ;
					strArrayChainLocShow.Add(strLocTmp) ;

					cSetFind.GetFieldValue(_T("loc_num"), iLongLocNum) ;
					cSetFind.GetFieldValue(_T("loc_real"), iLongLocReal) ;

					CString strLongLocNum = _T(""), strLongLocReal = _T("") ;
					
					strLocTmp.Format(_T("%d"), iLongLocNum) ;
					strArrayChainLocNum.Add(strLocTmp) ;
					strLocTmp.Format(_T("%d"), iLongLocReal) ;
					strArrayChainLocReal.Add(strLocTmp) ;					
					
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}

			long iMileSysStartLoc = iStartLocNum ; //每个里程体系的起始值
			long iMileSysStartLocReal = iStartRealLoc ; //每个里程体系的起始实际值
			if (iChainRsCount==0)//没有长短链
			{
				CString strTmp = _T("") ;
				strTmp.Format(_T("%d"), iEndRealLoc) ;
				strArrayChainLocShow.Add(strTmp) ;				
				strArrayChainLocNum.Add(strTmp) ;				
				strArrayChainLocReal.Add(strTmp) ;
				iChainRsCount = 1 ;
			}
			for (int iChainIndex=0; iChainIndex<iChainRsCount; iChainIndex++)
			{
				CString strChainLocShow = strArrayChainLocShow.GetAt(iChainIndex) ;
				CString strChainLocNum = strArrayChainLocNum.GetAt(iChainIndex) ;
				CString strChainLocReal = strArrayChainLocReal.GetAt(iChainIndex) ;

				long iChainLocShow = 0 ; // _tstol(strChainLocShow) ;
				if (strChainLocShow.Find(_T("+"))!=-1)
				{
					this->GetNumOfLoc(strChainLocShow, iChainLocShow) ;
				}
				else
				{
					iChainLocShow = _tstol(strChainLocShow) ;
				}
				
				long iChainLocNum = _tstol(strChainLocNum) ;
				long iChainLocReal = _tstol(strChainLocReal) ;

				double dLongX = iChainLocReal*dPlusForDrawX ;
				
				AcGePoint3d pt_3d_start(dLongX, dMileSysFirstLineY[iUpOrDownIndex], 0) ;
				AcGePoint3d pt_3d_end(dLongX, dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-3:3), 0) ;
				ads_point pt_base_text = {dLongX, dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?2:-2), 0} ;
				CString strText = _T("") ;

				if (iChainRsCount>1)
				{
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;
					CString strChianLocNum_DK ;
					this->GetLocShow(iChainLocNum, this->m_strMileSysName, strChianLocNum_DK) ;
					strText.Format(_T("%s=%s"), strChainLocShow, strChianLocNum_DK) ;
					//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.6) ;
					mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.6) ;
				}				
				
				long iCurStartKm = iMileSysStartLoc%1000>0?(iMileSysStartLoc/1000+1):(iMileSysStartLoc/1000) ;
				long iCurEndKm = iChainLocShow/1000 ;

				long iTmp1 = iMileSysStartLoc%1000 ;
				if (iTmp1>0) //例如起始里程（iMileSysStartLoc）为K2+123，则iCurStartKm为K3，那么从K2->K3之间的百米标就漏画了
				{
					double dKmX = (iMileSysStartLoc/1000*1000-iMileSysStartLoc+iMileSysStartLocReal)*dPlusForDrawX ;
					for (int iMileIndex=(iTmp1%100>0?(iMileSysStartLoc%1000/100+1):(iMileSysStartLoc%1000/100)); iMileIndex<10; iMileIndex++)
					{
						double dMx =dKmX+iMileIndex*3.5  ;
						pt_3d_start.x = pt_3d_end.x = dMx;
						pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
						pt_base_text[0] = dMx ;
						pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 				
						strText.Format(_T("%d"), iMileIndex) ;
						//AcCmColor clr ;
						//clr.setColorIndex(7) ;
						//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;	
						mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 7) ;
					}					
				}
				long iTmp2 = iChainLocShow%1000 ;
				if (iTmp2>0)  
				{
					double dKmX = (iChainLocShow/1000*1000-iMileSysStartLoc+iMileSysStartLocReal)*dPlusForDrawX ;
					for (int iMileIndex=1; iMileIndex<(iTmp2%100>0?(iChainLocShow%1000/100+1):(iChainLocShow%1000/100)); iMileIndex++)
					{
						double dMx =dKmX+iMileIndex*3.5  ;
						pt_3d_start.x = pt_3d_end.x = dMx;
						pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
						pt_base_text[0] = dMx ;
						pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 				
						strText.Format(_T("%d"), iMileIndex) ;
						//AcCmColor clr ;
						//clr.setColorIndex(7) ;
						//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;
						mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 7) ;

					}					
				}
				
				//绘制千米标
				for (long i=iCurStartKm; i<=iCurEndKm; i++)
				{
					double dKmX = (i*1000-iMileSysStartLoc+iMileSysStartLocReal)*dPlusForDrawX ;
					pt_3d_start.x = pt_3d_end.x = dKmX ;
					pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-1:1) ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
					pt_base_text[0] = dKmX ;
					pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 					
					strText.Format(_T("%s%d"), m_strMileSysName, i) ;
					//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.5) ;
					mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.5) ;
					
					//绘制百米标
					if (i<iCurEndKm)
					{
						for (int iMileIndex=1; iMileIndex<10; iMileIndex++)
						{
							double dMx =dKmX+iMileIndex*3.5  ;
							pt_3d_start.x = pt_3d_end.x = dMx;
							pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ;
							mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
							pt_base_text[0] = dMx ;
							pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 				
							strText.Format(_T("%d"), iMileIndex) ;
							//AcCmColor clr ;
							//clr.setColorIndex(7) ;
							//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;	
							mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 7) ;
						}
					}
				}
				
				
				if (iChainIndex==iChainRsCount-1) //最后一个长短链
				{
					iCurStartKm = iChainLocNum%1000>0?(iChainLocNum/1000+1):(iChainLocNum/1000) ;
					iCurEndKm = iEndLocNum/1000 ;

					for (long i=iCurStartKm; i<=iCurEndKm; i++)
					{
						double dKmX = (i*1000-iChainLocNum+iChainLocReal)*dPlusForDrawX ;
						pt_3d_start.x = pt_3d_end.x = dKmX ;
						pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-1:1) ; 
						
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
						pt_base_text[0] = dKmX ;
						pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 					
						strText.Format(_T("K%d"), i) ;
						//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.5) ;
						mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.5) ;
						
						//绘制百米标
						if (i<iCurEndKm)
						{
							for (int iMileIndex=1; iMileIndex<10; iMileIndex++)
							{
								double dMx =dKmX+iMileIndex*3.5  ;
								pt_3d_start.x = pt_3d_end.x = dMx;
								pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ; 
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
								pt_base_text[0] = dMx ;
								pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 					
								strText.Format(_T("%d"), iMileIndex) ;
								//AcCmColor clr ;
								//clr.setColorIndex(7) ;
								//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.5, clr) ;
								mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.5, 0, 7) ;
							}
						}
					}
				}
				
				iMileSysStartLoc = iChainLocNum ;
				iMileSysStartLocReal = iChainLocReal ;
			}

			//end 绘制标尺
		
			//begin 绘制坡度
			mysys.Set_Layer(_T(""), _T("pd")) ;
			//begin 找出坡度上下行起始loc
			double dPdStartLoc = 0 ;			
			strSql.Format(_T("select top 1 * from import_pd_info where up_or_down=%d order by id %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc") ) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					double dEndLoc = 0 ;
					cSetFind.GetFieldValue(_T("end_loc_num"), dEndLoc) ;
					if (iUpOrDownIndex==0)//down
					{
						double dFirstPdLen = 0 ;
						cSetFind.GetFieldValue(_T("len"), dFirstPdLen) ;
						dPdStartLoc = dEndLoc-dFirstPdLen ;
					}
					else if (iUpOrDownIndex==1)//up
					{
						double dPdSumLen = 0 ;
						CADORecordset cSetGetAllLen ;
						strSql.Format(_T("select sum(len) as pdsumlen from import_pd_info where up_or_down=1 ")) ;
						if (cSetGetAllLen.IsOpen())
						{
							cSetGetAllLen.Close() ;
						}
						if (cSetGetAllLen.Open(g_PtrCon, strSql))
						{
							cSetGetAllLen.GetFieldValue(_T("pdsumlen"), dPdSumLen) ;
							cSetGetAllLen.Close() ;
						}
						dPdStartLoc = dEndLoc+dPdSumLen ;
					}

				}

				cSetFind.Close() ;
			}			
			//end 找出坡度上下行起始loc

			strSql.Format(_T("select * from import_pd_info where up_or_down=%d order by id "), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				double dCurStartRealLoc = dPdStartLoc ; //iUpOrDownIndex==0?1086:1309693 ; //当前起始loc_real 1086:921622			
				while (!cSetFind.IsEOF())
				{
					CString strPd = _T("") ;
					CString strEndLoc = _T("") ;
					double dLen = 0 ;
					cSetFind.GetFieldValue(_T("pd"), strPd) ;
					cSetFind.GetFieldValue(_T("len"), dLen) ;
					cSetFind.GetFieldValue(_T("end_loc"), strEndLoc) ;
					
					ads_point pt_base_text = {0, 0, 0} ;
					//AcCmColor clr ;
					
					double dPd = 0 ;
					dPd = _tstof(strPd) ;
					pt_3d_start.x = dCurStartRealLoc*dPlusForDrawX ;
					pt_3d_end.x = (dCurStartRealLoc+(iUpOrDownIndex==0?dLen:-dLen))*dPlusForDrawX ;
					double dMidX = (dCurStartRealLoc+(iUpOrDownIndex==0?dLen/2:-dLen/2))*dPlusForDrawX ;
					double dMidY = dPdFirstHLineY[iUpOrDownIndex]-(dPdFirstHLineY[iUpOrDownIndex]-dPdSecondHLineY[iUpOrDownIndex])/2 ;
					
					if (dPd==0)
					{
						pt_3d_start.y = pt_3d_end.y = dMidY;
					}
					else if (dPd>0)
					{
						pt_3d_start.y = dPdSecondHLineY[iUpOrDownIndex] ;
						pt_3d_end.y = dPdFirstHLineY[iUpOrDownIndex] ;
					}
					else
					{
						pt_3d_start.y = dPdFirstHLineY[iUpOrDownIndex] ;
						pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex] ;
					}
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 2) ;//坡度线 黄色 2
					pt_3d_start.x = pt_3d_end.x ;
					pt_3d_start.y = dPdFirstHLineY[iUpOrDownIndex] ;
					pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex] ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ; //坡度界限 竖线
					
					pt_base_text[0] = dMidX ;
					pt_base_text[1] = dMidY+1.2 ;			
					//clr.setColorIndex(6) ; //AcMagenta 6 洋红
					//mysys.WriteText(pt_base_text, strPd, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;  //坡度值
					mysys.AddText(strPd, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 6) ; //AcMagenta 6 洋红
					pt_base_text[1] = dMidY-1.2 ;
					//clr.setColorIndex(3) ; //AcGreen 3 绿色
					CString strLen = _T("") ;
					strLen.Format(_T("%.3f"), dLen) ;
					strLen.TrimRight(_T("0")) ;
					strLen.TrimRight(_T(".")) ;
					//mysys.WriteText(pt_base_text, strLen, 0, AcDb::kTextMid, 1.5, 0.5, clr) ; //坡长
					mysys.AddText(strLen, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 3) ;//坡长 AcGreen 3 绿色
					
					pt_3d_start.x = pt_3d_end.x = (dCurStartRealLoc+(iUpOrDownIndex==0?dLen:-dLen))*dPlusForDrawX ;
					if (iUpOrDownIndex==0)
					{
						pt_3d_start.y = dPdSecondHLineY[iUpOrDownIndex]-2 ;
						pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex]-8 ;
					}
					else 
					{
						pt_3d_start.y = dPdFirstHLineY[iUpOrDownIndex]+2 ;
						pt_3d_end.y = dPdFirstHLineY[iUpOrDownIndex]+8 ;

					}
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 4) ;//坡度endloc线 acCyan 4 青 
					
					//clr.setColorIndex(4) ; //acCyan 4 青 
					pt_base_text[0] = pt_3d_end.x-0.5 ;
					pt_base_text[1] = pt_3d_start.y ;
					//mysys.WriteText(pt_base_text, strEndLoc, PI/2, iUpOrDownIndex==0?AcDb::kTextRight:AcDb::kTextLeft, 1.5, 0.5, clr) ; //坡度endloc acCyan 4 青
					mysys.AddText(strEndLoc, pt_base_text, pt_base_text, iUpOrDownIndex==0?AcDb::kTextRight:AcDb::kTextLeft, AcDb::kTextBase, 1.5, 0.5, PI/2, 4) ;//坡度endloc acCyan 4 青
					
					dCurStartRealLoc=dCurStartRealLoc+(iUpOrDownIndex==0?dLen:-dLen) ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
			//end 绘制坡度
		
			
			//begin 绘制速度 
			mysys.Set_Layer(_T(""), _T("speed")) ;
			//begin 找出速度上下行起始loc
			double dSpeedStartLoc = 0 ;			
			strSql.Format(_T("select top 1 * from import_speed where up_or_down=%d and is_reverse=0 order by id %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc") ) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					double dEndLoc = 0 ;
					cSetFind.GetFieldValue(_T("end_loc_num"), dEndLoc) ;
					if (iUpOrDownIndex==0)//down
					{
						double dFirstSpeedLen = 0 ;
						cSetFind.GetFieldValue(_T("len"), dFirstSpeedLen) ;
						dSpeedStartLoc = dEndLoc-dFirstSpeedLen ;
					}
					else if (iUpOrDownIndex==1)//up
					{
						double dSpeedSumLen = 0 ;
						CADORecordset cSetGetAllLen ;
						strSql.Format(_T("select sum(len) as speedsumlen from import_speed where up_or_down=1 and is_reverse=0 ")) ;
						if (cSetGetAllLen.IsOpen())
						{
							cSetGetAllLen.Close() ;
						}
						if (cSetGetAllLen.Open(g_PtrCon, strSql))
						{
							cSetGetAllLen.GetFieldValue(_T("speedsumlen"), dSpeedSumLen) ;
							cSetGetAllLen.Close() ;
						}
						dSpeedStartLoc = dEndLoc+dSpeedSumLen ;
					}

				}
				
				cSetFind.Close() ;
			}			
			//end 找出速度上下行起始loc

			strSql.Format(_T("select * from import_speed where up_or_down=%d and is_reverse=0 order by id asc"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				double dCurStartRealLoc = dSpeedStartLoc ;//iUpOrDownIndex==0?591974:864362 ; //当前起始loc_real
				int iPreSpeed = -1 ; //记录前一个速度值
				CString strPreEndLoc = _T("") ;
				while (!cSetFind.IsEOF())
				{
					CString strSpeed = _T(""), strLen = _T("") ;
					int iSpeed = 0 ;
					CString strEndLoc = _T("") ;
					long iLen = 0 ;
					cSetFind.GetFieldValue(_T("speed"), iSpeed) ;
					cSetFind.GetFieldValue(_T("len"), iLen) ;
					cSetFind.GetFieldValue(_T("end_loc"), strEndLoc) ;
					
					strSpeed.Format(_T("%d"), iSpeed) ;
					strLen.Format(_T("%d"), iLen) ;
					
					ads_point pt_base_text = {0, 0, 0} ;
					//AcCmColor clr ;
					
					pt_3d_start.x = dCurStartRealLoc*dPlusForDrawX ;
					pt_3d_end.x = (dCurStartRealLoc+(iUpOrDownIndex==0?iLen:-iLen))*dPlusForDrawX ;
					pt_3d_start.y = pt_3d_end.y = dDrawSpeedStartY[iUpOrDownIndex]+iSpeed*dPlusForDrawSpeedY ;
					//double dMidX = (dCurStartRealLoc+(dLen/2))*dPlusForDrawX ;
					//double dMidY = dPdFirstHLineY-(dPdFirstHLineY-dPdSecondHLineY)/2 ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;//速度线 红色 1 
					
					pt_base_text[0] = (dCurStartRealLoc+(iUpOrDownIndex==0?(iLen/2):(-iLen/2)))*dPlusForDrawX ;
					pt_base_text[1] = pt_3d_start.y+1.2 ;
					//clr.setColorIndex(6) ; //洋红
					//mysys.WriteText(pt_base_text, strSpeed, 0, AcDb::kTextMid, 1.5, 0.5, clr) ; //速度endloc acCyan 4 青
					mysys.AddText(strSpeed, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 6) ; //速度endloc 洋红
					pt_base_text[1] = pt_3d_start.y-1.2 ;
					//clr.setColorIndex(3) ; 
					CString strLenWrite = strLen ;
					strLenWrite.Insert(555, _T("m")) ;
					//mysys.WriteText(pt_base_text, strLenWrite, 0, AcDb::kTextMid, 1.5, 0.5, clr) ; //速度endloc acCyan 4 青
					mysys.AddText(strLenWrite, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 3) ;//绿
					
					if (iPreSpeed!=-1)//prespeed还是初始赋值-1时表示这是第一个速度段
					{
						pt_3d_end.x = pt_3d_start.x ;
						pt_3d_start.y = dDrawSpeedStartY[iUpOrDownIndex]+iPreSpeed*dPlusForDrawSpeedY ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;//前一个速度和当前速度的折线 红色 1 
						
						pt_3d_start.y = max(pt_3d_start.y, pt_3d_end.y) + 1 ;
						pt_3d_end.y = pt_3d_start.y+9 ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 2) ;//endloc 黄色 2
						
						//clr.setColorIndex(2) ;  
						pt_base_text[0] = pt_3d_end.x-0.5 ;
						pt_base_text[1] = pt_3d_start.y ;
						//mysys.WriteText(pt_base_text, strPreEndLoc, PI/2, AcDb::kTextLeft, 1.5, 0.5, clr) ; //速度endloc 黄2
						mysys.AddText(strPreEndLoc, pt_base_text, pt_base_text, AcDb::kTextLeft, AcDb::kTextBase, 1.5, 0.5, PI/2, 2) ;//黄2
						
					}
					
					iPreSpeed = iSpeed ;
					strPreEndLoc = strEndLoc ;
					if (iUpOrDownIndex==0)
					{
						dCurStartRealLoc+=iLen ;
					}
					else
					{
						dCurStartRealLoc-=iLen ;
					}
					
					cSetFind.MoveNext() ;
				}
				
				cSetFind.Close() ;
			}
			//end 绘制速度
			
		
			//begin 绘制信号点
			mysys.Set_Layer(_T(""), _T("signal")) ;
			double dCurX = 0 ;
			double dCurY = dDrawXhjY[iUpOrDownIndex] ;			
			
			pt_3d_start.x = iStartRealLoc*dPlusForDrawX ;
			pt_3d_end.x = iEndRealLoc*dPlusForDrawX ;
			pt_3d_start.y = pt_3d_end.y = dDrawXhjY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			
			pt_3d_start.y = pt_3d_end.y = dCurY ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			
			//strSql.Format(_T("select * from compute_xhj_table where not (right(name,1)='链' and up_or_down=1) and not (up_or_down=1 and type in(0,2,3)) order by loc_real asc")) ;
			strSql.Format(_T("select * from compute_xhj_table where (up_or_down=%d and type not in(2)) or type in (1,11) order by loc_real %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc")) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				int iXhIndex = 0 ;
				
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d个信号点"), iUpOrDownIndex==0?_T("下行"):_T("上行"),iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iXhIndex) ;


					mysys.Set_Layer(_T(""), _T("signal")) ;

					CString strName = _T("") ;
					CString strLocShow = _T("") ;
					CString strLocNum = _T("") ;
					CString strFreq = _T("") ;
					CString strGdName = _T("") ;
					CString strYdqType = _T("") ;
					long iLocNum = 0 ;
					long iLen = 0 ;
					long iXhRealLoc = 0 ;
					int iIsDivide = 0 ;
					int iType = 0 ;
					int iUpOrDown = 0 ;
					int iIsReverse = 0 ;
					cSetFind.GetFieldValue(_T("name"), strName) ;
					cSetFind.GetFieldValue(_T("loc_show"), strLocShow) ;
					cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
					cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
					cSetFind.GetFieldValue(_T("loc_real"), iXhRealLoc) ;
					cSetFind.GetFieldValue(_T("is_divide"), iIsDivide) ;
					cSetFind.GetFieldValue(_T("type"), iType) ;
					cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
					cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
					
					this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;
					
					if (iUpOrDownIndex==0)
					{
						cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;
						cSetFind.GetFieldValue(_T("len"), iLen) ;
					}
					cSetFind.MoveNext() ;
					iXhIndex++ ;

					if (cSetFind.IsEOF())
					{
						break ;
					}
					if (iUpOrDownIndex==1)
					{
						int iTypeTmp = -1 ;
						cSetFind.GetFieldValue(_T("type"), iTypeTmp) ;
						if (iTypeTmp==1||iTypeTmp==11||iTypeTmp==2) //假如下一个是车站或者中继站、出站，则读不到频率，得继续下一个
						{
							cSetFind.MoveNext() ;
							if (!cSetFind.IsEOF())
							{
								cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;
								cSetFind.GetFieldValue(_T("len"), iLen) ;
								cSetFind.MovePrevious() ;
							}
						}
						else
						{		
							cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;
							cSetFind.GetFieldValue(_T("len"), iLen) ;
						}
					}
					
					CString strLen ;
					strLen.Format(_T("%d"), iLen) ;
					
					dCurX = iXhRealLoc*dPlusForDrawX ;
					
					ads_point pt_insert = {0,0,0};
					pt_insert[0] = dCurX ;
					pt_insert[1] = dCurY ;
					AcGePoint3d pt_3d_insert(0,0,0) ;
					
					CString strBlockName = _T("") ;
					
					if (iType==1||iType==11)
					{
						mysys.Set_Layer(_T(""), _T("station")) ;
						pt_insert[1] = dDrawStationY[iUpOrDownIndex]+1 ;
						//AcCmColor clr ;
						//clr.setColorIndex(4) ;
						//mysys.WriteText(pt_insert, strName, 0, AcDb::kTextMid, 2, 0.7, clr) ;
						mysys.AddText(strName, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
						pt_insert[1] -= 6 ;
						//mysys.WriteText(pt_insert, strLocNum, 0, AcDb::kTextMid, 2, 0.7, clr) ;
						mysys.AddText(strLocNum, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
						pt_insert[1]+=3 ;
						AcGePoint3d pt_3d_start(dCurX-5, pt_insert[1], 0) ;
						AcGePoint3d pt_3d_end(dCurX+5, pt_insert[1], 0) ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
					}
					else if (iType==0||iType==2||iType==3||iType==4)
					{
						strArrayAtt.RemoveAll() ;
						if (strName.Right(2)==_T("链"))
						{
							strBlockName.Format(_T("pdt_chain_%s"), iUpOrDownIndex==0?_T("x"):_T("s")) ;
							strLocShow.Insert(555,_T("=")) ;
							strLocShow.Insert(555, strLocNum) ;
							CString strChainLen = _T("") ;
							strChainLen.Format(_T("%s=%d"), strName, iLen) ;
							strArrayAtt.Add(strLocShow) ;
							strArrayAtt.Add(strChainLen) ;
						}
						else
						{
							if (strName.Left(1)==_T("F"))
							{
								strBlockName.Format(_T("pdt_tg_%s_f"), iUpOrDownIndex==0?_T("x"):_T("s") ) ;
							}
							else if (iType==2)
							{
								strBlockName.Format(_T("pdt_cz_%s"),iUpOrDownIndex==0?_T("x"):_T("s")) ;
// 								if (iUpOrDown==1)
// 								{
// 									strBlockName.Format(_T("pdt_cz_%s"), iUpOrDownIndex==0?_T("s"):_T("x")) ;
// 									strName =(iUpOrDownIndex==0?_T("SI"):_T("XII")) ;
// 								}
							}
							else if (iType==3)
							{
								strBlockName.Format(_T("pdt_jz_%s_z"), iUpOrDownIndex==0?_T("x"):_T("s") ) ;
								if (iIsReverse==1)
								{
									strBlockName.Format(_T("pdt_jz_%s_f"),iUpOrDownIndex==0?_T("s"):_T("x") ) ;
								}
							}
							else if (iType==0)
							{
								strBlockName.Format(_T("pdt_tg_%s"),iUpOrDownIndex==0?_T("x"):_T("s") ) ;
							}
							else if (iType==4)
							{
								strBlockName.Format(_T("pdt_jyj_0")) ;
								if (iUpOrDownIndex==0)
								{
									if (strName.Left(1)==_T("S"))
									{
										strBlockName.Format(_T("pdt_cz_s")) ;
									}
									else if (strName.Left(1)==_T("X"))
									{
										strBlockName.Format(_T("pdt_cz_x")) ;
									}
									
								}
								else if (iUpOrDownIndex==1)
								{
									if (strName.Left(1)==_T("X"))
									{
										strBlockName.Format(_T("pdt_cz_x")) ;
									}
									else if (strName.Left(1)==_T("S"))
									{
										strBlockName.Format(_T("pdt_cz_s")) ;
									}
									
								}
							}
							strArrayAtt.Add(strName) ;
							strArrayAtt.Add(strLocNum) ;
						}
						
						//mysys.Set_Layer(_T(""), _T("xhj")) ;
						mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert ) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
						{
							pRef->close() ;
							pRef = NULL ;
						}					
						
						//绘制freq len gdname
						//if ((iType==0||iIsReverse==1)&&strName.Right(2)!=_T("链"))
						if (iType!=2&&strName.Right(2)!=_T("链"))
						{
							pt_3d_insert.x = dCurX+(iUpOrDownIndex==0?6.8:-6.8) ;
							
							strArrayAtt.RemoveAll();
							CString strLenTmp = strLen ;
							strLenTmp.Insert(555, _T("m")) ;
							strArrayAtt.Add(strLenTmp) ;
							strArrayAtt.Add(strFreq) ;						
							strArrayAtt.Add(strGdName) ;
							strBlockName.Format(_T("pdt_freq_%s"), iUpOrDownIndex==0?_T("x"):_T("s")) ;
							mysys.Set_Layer(_T(""), _T("freq")) ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
								pRef = NULL ;
							}								
						}							
						//是否是分界
						if (iIsDivide==1)
						{
							pt_3d_insert.x = dCurX ;
							pt_3d_insert.y= pt_3d_insert.y+ (iUpOrDownIndex==0?15:-15) ;
							strBlockName.Format(_T("PMT_DIVIDE_0") ) ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
							{
								if (iUpOrDownIndex!=0)
								{
									pRef->setRotation(PI) ;
								}
								pRef->close() ;
								pRef = NULL ;
							}	
						}					
					}	
					
					//iXhIndex++ ;
					//cSetFind.MoveNext() ;
				}			
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}
			//end 绘制信号点
			
			//begin 绘制应答器
			mysys.Set_Layer(_T(""), _T("ydq")) ;
			strSql.Format(_T("select * from export_ydq where loc_real>0 and up_or_down=%d and (right(name,2)='-1' or InStr(name, '-')=0) order by loc_real asc"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d组应答器"), iUpOrDownIndex==0?_T("下行"):_T("上行"), iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

				int iYdqGroupIndex = 0 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iYdqGroupIndex) ;

					CString strName = _T(""), strSn = _T(""), strLoc = _T(""), strUseage = _T("") ;
					long iLocReal = 0 ;
					cSetFind.GetFieldValue(_T("name"), strName) ;
					cSetFind.GetFieldValue(_T("sn"), strSn) ;
					cSetFind.GetFieldValue(_T("loc"), strLoc) ;
					cSetFind.GetFieldValue(_T("useage"), strUseage) ;
					cSetFind.GetFieldValue(_T("loc_real"), iLocReal) ;
					
					dCurX = iLocReal*dPlusForDrawX ;
					
					ads_point pt_insert = {0,0,0};
					pt_insert[0] = dCurX ;
					pt_insert[1] = dCurY ;
					AcGePoint3d pt_3d_insert(0,0,0) ;				
					
					CString strBlockName = _T("") ;
					CString strUseageTmp = strUseage ;
					
					if (strUseage==_T("FCZ"))
					{
						strUseageTmp = _T("CZ") ;
					}
					else if (strUseage==_T("FDW"))
					{
						strUseageTmp = _T("DW") ;
					}
					else if (strUseage.Left(2)==_T("ZJ"))
					{
						strUseageTmp = _T("ZJ") ;
					}
					strUseageTmp.MakeLower() ;
					strBlockName.Format(_T("pdt_ydq_%s_%s"), strUseageTmp, iUpOrDownIndex==0?_T("x"):_T("s") ) ;
					if (strUseage==_T("FJZ"))
					{
						strBlockName.Format(_T("pdt_ydq_%s_%s"), strUseageTmp, iUpOrDownIndex==0?_T("s"):_T("x") ) ;
					}
					strArrayAtt.RemoveAll() ;
					CString strNameTmp = strName ;
// 					if (strName==_T("B11221"))
// 					{
// 						int ii = 0 ;
// 					}
					if (strName.Find(_T("-"))!=-1)
					{
						strNameTmp = strName.Left(strName.Find(_T("-"))) ;						
					}					
					strArrayAtt.Add(strNameTmp) ;
					strArrayAtt.Add(strLoc) ;
					pt_3d_insert.y = dDrawXhjY[iUpOrDownIndex] ;
					pt_3d_insert.x = iLocReal*dPlusForDrawX ;

					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
					{
						pRef->close() ;
						pRef = NULL ;
					}
					
					//应答器编号和用途
					CString strSnUseage = _T("") ;
					CStringArray strArraySn ;
					CBlkUtility::DivideString(strSn, _T("-"), strArraySn) ;
					strSnUseage.Format(_T("%s-%s/%s"), strArraySn.GetAt(2), strArraySn.GetAt(3), strUseage) ;
					//AcCmColor clr ;
					//clr.setColorIndex(2) ; //yellow
					pt_insert[0] = pt_3d_insert.x ;
					pt_insert[1] = pt_3d_insert.y-(iUpOrDownIndex==0?5:-3) ;
					//mysys.WriteText(pt_insert, strSnUseage, 0, AcDb::kTextCenter, 1, 0.5, clr) ;
					mysys.AddText(strSnUseage, pt_insert, pt_insert, AcDb::kTextCenter, AcDb::kTextBase, 1, 0.5, 0, 2) ;					
					
					iYdqGroupIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}			
			//end 绘制应答器

			//begin 绘制分相
			mysys.Set_Layer(_T(""), _T("dfx")) ;
			strSql.Format(_T("select * from import_dfx_info where up_or_down=%d order by id %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc")) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d个电分相"), iUpOrDownIndex==0?_T("下行"):_T("上行"), iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

				int iDfxIndex = 0 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iDfxIndex) ;

					CString strDfxStartLoc = _T(""), strDfxEndLoc = _T(""), strDfxComment = _T(""), strDfxLen = _T("") ;
					long iDfxStartLoc = 0, iDfxEndLoc = 0 ;					
					double	dDfxLen = 0 ;
					cSetFind.GetFieldValue(_T("start_loc_num"), iDfxStartLoc) ;
					cSetFind.GetFieldValue(_T("end_loc_num"), iDfxEndLoc) ;
					cSetFind.GetFieldValue(_T("start_loc"), strDfxStartLoc) ;
					cSetFind.GetFieldValue(_T("end_loc"), strDfxEndLoc) ;
					cSetFind.GetFieldValue(_T("len"), dDfxLen) ;

					strDfxLen.Format(_T("%d米"), dDfxLen) ;

					long iDfxStartReal = iUpOrDownIndex==0?iDfxStartLoc:iDfxEndLoc ; //电分相起始（左边）real_loc
					//begin 找出比分相起始点小的最近的信号点loc					
					CADORecordset cSetFindNearLoc ;
					strSql.Format(_T("select * from compute_xhj_table where loc_num<=%d and type=0 and up_or_down=%d order by loc_real desc"), iUpOrDownIndex==0?iDfxStartLoc:iDfxEndLoc, iUpOrDownIndex) ;
					if (cSetFindNearLoc.IsOpen())
					{
						cSetFindNearLoc.Close() ;
					}
					if (cSetFindNearLoc.Open(g_PtrCon, strSql))
					{
						if (cSetFindNearLoc.GetRecordCount()>0)
						{
							cSetFindNearLoc.MoveFirst() ;

							long iNearLocNum = 0, iNearLocReal = 0 ;
							cSetFindNearLoc.GetFieldValue(_T("loc_num"), iNearLocNum) ;
							cSetFindNearLoc.GetFieldValue(_T("loc_real"), iNearLocReal) ;
							iDfxStartReal = iNearLocReal+((iUpOrDownIndex==0?iDfxStartLoc:iDfxEndLoc)-iNearLocNum) ;
						}
						
						cSetFindNearLoc.Close() ;
					}
					//end 找出比分相起始点小的最近的信号点loc
					
					double dDfxStartX = iDfxStartReal*dPlusForDrawX ;
					double dDfxEndX = (iDfxStartReal+dDfxLen)*dPlusForDrawX ;

					AcGePoint3dArray ptArray_3d ;
					AcGePoint3d pt_3d_pl(0,0,0), pt_3d_line_start(0,0,0), pt_3d_line_end(0,0,0) ;
					pt_3d_pl.x = dDfxStartX ;
					pt_3d_pl.y = dDrawXhjY[iUpOrDownIndex]+(iUpOrDownIndex==0?10:-10) ;
					ptArray_3d.append(pt_3d_pl) ;//pt1

					//起点loc
					pt_3d_line_start = pt_3d_pl ;
					pt_3d_line_start.y+=(iUpOrDownIndex==0?1:-1) ;
					pt_3d_line_end.x = pt_3d_line_start.x ;
					pt_3d_line_end.y = pt_3d_line_start.y+(iUpOrDownIndex==0?5:-5) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;//起始loc线
					pt_3d_line_start.x-=0.5 ;
					ads_point pt_base_loc_text = {pt_3d_line_start.x,pt_3d_line_start.y,0} ;					
					//mysys.WriteText(pt_base_loc_text, iUpOrDownIndex==0?strDfxStartLoc:strDfxEndLoc, 90*PI/180, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, 1, 0.5) ; //起始loc
					mysys.AddText(iUpOrDownIndex==0?strDfxStartLoc:strDfxEndLoc, pt_base_loc_text, pt_base_loc_text, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, AcDb::kTextBase, 1, 0.5, 90*PI/180) ;//起始loc

					pt_3d_pl.x = dDfxEndX ;
					ptArray_3d.append(pt_3d_pl) ; //pt2

					//终点loc
					pt_3d_line_start = pt_3d_pl ;
					pt_3d_line_start.y+=(iUpOrDownIndex==0?1:-1) ;
					pt_3d_line_end.x = pt_3d_line_start.x ;
					pt_3d_line_end.y = pt_3d_line_start.y+(iUpOrDownIndex==0?5:-5) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;//终点loc线
					pt_3d_line_start.x+=1.5 ;
					pt_base_loc_text[0] = pt_3d_line_start.x ;
					pt_base_loc_text[1] = pt_3d_line_start.y ;					
					//mysys.WriteText(pt_base_loc_text, iUpOrDownIndex==0?strDfxEndLoc:strDfxStartLoc, 90*PI/180, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, 1, 0.5) ; //终始loc
					mysys.AddText(iUpOrDownIndex==0?strDfxEndLoc:strDfxStartLoc, pt_base_loc_text, pt_base_loc_text, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, AcDb::kTextBase, 1, 0.5, 90*PI/180) ;//终始loc


					pt_3d_pl.y = dDrawXhjY[iUpOrDownIndex]+(iUpOrDownIndex==0?-5.5:5) ;
					ptArray_3d.append(pt_3d_pl) ;//pt3
					pt_3d_pl.x = dDfxStartX ;
					ptArray_3d.append(pt_3d_pl) ;//pt4


					mysys.AddAcDb2dPolyline(ptArray_3d, 0, 0) ;	
					
					
// 					CString str = _T("") ;
// 					CStringArray strArraySn ;
// 					CBlkUtility::DivideString(strSn, _T("-"), strArraySn) ;
// 					strSnUseage.Format(_T("%s-%s/%s"), strArraySn.GetAt(2), strArraySn.GetAt(3), strUseage) ;
// 					AcCmColor clr ;
// 					clr.setColorIndex(2) ; //yellow
// 					pt_insert[0] = pt_3d_insert.x ;
// 					pt_insert[1] = pt_3d_insert.y-(iUpOrDownIndex==0?5:-3) ;
// 					mysys.WriteText(pt_insert, strSnUseage, 0, AcDb::kTextCenter, 1, 0.5, clr) ;
					
					
					iDfxIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}
			//end 绘制分相


		}

		//插入左侧frame
		AcGePoint3d pt_3d_frame(iStartRealLoc*dPlusForDrawX,0,0) ;
		if (mysys.InsertBlock(pRef, _T("frame_pdt_left"), pt_3d_frame))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		double dFrameLineY[4] = {98,93,-93,-98} ;
		AcGePoint3d pt_3d_start(iStartRealLoc*dPlusForDrawX,0,0) ;
		AcGePoint3d pt_3d_end(iEndRealLoc*dPlusForDrawX,0,0) ;
		for (int iFrameLineIndex=0; iFrameLineIndex<4; iFrameLineIndex++)
		{
			pt_3d_start.y = pt_3d_end.y = dFrameLineY[iFrameLineIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end, iFrameLineIndex%3==0?2:1) ;//黄色 2 红色 1 
		}		


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawPdt")) ;
		
	}

}

int CLKDataTable::GetNameFromLoc(CString strXlsFileName, const CString strFromCol, int iFromStartRow, const CString strToCol, int iToStartRow, int iSheet, int iUpOrDown)
{
	int iRet = 0 ;
	_ApplicationPtr m_pExcel = NULL ;
	//WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return 0;
		}
		
		pWorkBook =m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iSheet ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		int iCurRow = iFromStartRow ;
		int iCurToRow = iToStartRow ;

		BOOL bNext = TRUE ;
		while (bNext)
		{
			long iLocNum = 0 ;
			CString strCellName = _T(""), strCellText = _T("") ;
			strCellName.Format(_T("%s%d"), strFromCol, iCurRow) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName) ;
			strCellText = (CString)(pRange->GetText().bstrVal) ;
			strCellText.TrimLeft() ;
			strCellText.TrimRight() ;			
			
			if (strCellText==_T(""))
			{
				bNext = FALSE ;
				continue ;
			}
			
			if (strCellText.Find(_T("+"))==-1)
			{
				iLocNum = _tstol(strCellText) ;
			}
			else if (this->GetNumOfLoc(strCellText, iLocNum)==FALSE)
			{
				acutPrintf(_T("\n第%d行里程%s不符合X123+456或者123456形式的格式"), iCurRow, strCellText) ;
				iCurRow++ ;
				iCurToRow++ ;
				continue ;
			}

			CString strFName = _T("") ;
			this->GetFName(iLocNum, strFName, iUpOrDown) ;
			strFName.TrimLeft(_T("F")) ;
		
			strCellName.Format(_T("%s%d"), strToCol, iCurToRow) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName) ;
			pRange->PutNumberFormat((_variant_t)_T("@")) ;
			pRange->PutFormulaR1C1((_variant_t)strFName) ;

			acutPrintf(_T("\n%d:处理第%d行"), iRet, iCurRow) ;

			iCurRow++ ;
			iCurToRow++ ;
			
			iRet++ ;

		}	
		
		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt = VT_ERROR;
		varOptional.scode = (long)DISP_E_PARAMNOTFOUND;		
		
		pWorkBook->SaveAs( (_variant_t) strXlsFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;
		
		//////////////////////////////////////////////////////////////////////////

		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return iRet ;
}

int CLKDataTable::ImportPdTable(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable("import_pd_info") ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ("Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}
		
		pWorkBooks = m_pExcel->GetWorkbooks() ;		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		pWorksheets  = m_pExcel->GetSheets() ;

		CString strBarMsg ;
		strBarMsg.Format(_T("读取线路坡度表")) ;

		acedSetStatusBarProgressMeter(strBarMsg, 0, 1) ;	

		for (int iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
		{

			acedSetStatusBarProgressMeterPos(iSheetIndex) ;

			VARIANT vt_index ;
			vt_index.vt = VT_I2 ;
			vt_index.iVal = iSheetIndex+1 ;			
			pSheet = pWorksheets ->GetItem( vt_index ) ;
			
			CString strColName[3] = {_T("B"),_T("C"),_T("D")} ;
			
			int iCurRow = 3 ;
			BOOL bNext = TRUE ;
			while (bNext)
			{
				CString strEndLoc = _T("");
				CString strPd = _T("") ;
				double dLen = 0 ;
				long iEndLocNum = 0;
				
				BOOL bCurRowIsBlank = TRUE ;
				
				BOOL bReadSomething = FALSE ;
				for (int iColIndex=0; iColIndex<3; iColIndex++)
				{

					CString strCellName = _T(""), strCellText = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					
					if (iColIndex==0)
					{
						if (strCellText==_T(""))
						{
							break ;
						}
						else
						{
							bReadSomething = TRUE ;
						}
					}				
					
					switch (iColIndex)
					{
					case 0:
						strPd = strCellText ;
						break;
					case 1:					
						dLen = _tstof(strCellText) ;
						break;
					case 2:
						strEndLoc = strCellText ;
						break;
			
					}
				}
				if (bReadSomething)
				{
					if (strEndLoc.Find(_T("+"))==-1)
					{
						iEndLocNum = _tstoi(strEndLoc) ;
					}
					else if (this->GetNumOfLoc(strEndLoc, iEndLocNum)==FALSE)
					{
						acutPrintf(_T("\n第%d个sheet%d行里程%s不符合格式"), iSheetIndex, iCurRow, strEndLoc) ;
					}
					
				strExec.Format(_T("insert into import_pd_info(pd, len, end_loc, end_loc_num, up_or_down)\
					values('%s',%.3f,'%s',%d ,%d)"), strPd, dLen, strEndLoc, iEndLocNum, iSheetIndex) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
				}	
				
				if (bCurRowIsBlank)
				{
					bNext = FALSE ;
				}
				else
				{
					iCurRow++ ;
				}

			}	

		}

		acedRestoreStatusBar() ;
		

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return iRet ;

}

BOOL CLKDataTable::GetNumOfLoc(const CString strLoc, double &dLoc)
{
	BOOL bRet = TRUE ;
	
	CUserSystem mysys ;
	
	//显示里程转换为数值
	if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]\\d+\\+\\d{3}\\.\\d+$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^\\w*[k,K](\\d+)\\+(\\d{3}\\.\\d+)$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^\\w*[k,K](\\d+)\\+(\\d{3}\\.\\d+)$"), _T("$2"), strLoc, strPart2) ;
		double dPart1 = 0, dPart2 = 0 ;
		dPart1 = _tstof(strPart1) ;
		dPart2 = _tstof(strPart2) ;
		dLoc = dPart1*1000+ dPart2;
	}
	else if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]-\\d+\\+-\\d{3}\\.\\d+$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^\\w*[k,K]-(\\d+)\\+-(\\d{3}\\.\\d+)$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^\\w*[k,K]-(\\d+)\\+-(\\d{3}\\.\\d+)$"), _T("$2"), strLoc, strPart2) ;
		dLoc = 0-(_tstof(strPart1)*1000+_tstof(strPart2)) ;
	}
	else if (CBlkUtility::IsStrMatch(_T("^(\\d+)\\.*(\\d*)$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^(\\d+)\\.*(\\d*)$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^(\\d+)\\.*(\\d*)$"), _T("$2"), strLoc, strPart2) ;
		dLoc = _tstof(strPart1)*1000+_tstof(strPart2) ;
	}
	else if (CBlkUtility::IsStrMatch(_T("^-(\\d+)\\.*(\\d*)$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^-(\\d+)\\.*(\\d*)$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^-(\\d+)\\.*(\\d*)$"), _T("$2"), strLoc, strPart2) ;
		dLoc = 0-(_tstof(strPart1)*1000+_tstof(strPart2)) ;
	}
	else
	{
		dLoc = 0 ;
		bRet = FALSE ;
	}
	
	return bRet ;

}

int CLKDataTable::ImportDFXTable(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable("import_dfx_info") ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ("Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}
		
		pWorkBooks = m_pExcel->GetWorkbooks() ;		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		pWorksheets  = m_pExcel->GetSheets() ;

		CString strBarMsg ;
		strBarMsg.Format(_T("读取线路电分相表")) ;

		acedSetStatusBarProgressMeter(strBarMsg, 0, 1) ;	

		for (int iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
		{

			acedSetStatusBarProgressMeterPos(iSheetIndex) ;

			VARIANT vt_index ;
			vt_index.vt = VT_I2 ;
			vt_index.iVal = iSheetIndex+1 ;			
			pSheet = pWorksheets ->GetItem( vt_index ) ;
			
			CString strColName[4] = {_T("B"),_T("C"),_T("D"),_T("E")} ;
			
			int iCurRow = 3 ;
			BOOL bNext = TRUE ;
			while (bNext)
			{
				CString strStartLoc = _T("");
				CString strEndLoc = _T("");
				CString strComment = _T("") ;
				double dLen = 0 ;
				long iStartLocNum = 0;
				long iEndLocNum = 0;
				
				BOOL bCurRowIsBlank = TRUE ;
				
				BOOL bReadSomething = FALSE ;
				for (int iColIndex=0; iColIndex<4; iColIndex++)
				{

					CString strCellName = _T(""), strCellText = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					
					if (iColIndex==0)
					{
						if (strCellText==_T(""))
						{
							break ;
						}
						else
						{
							bReadSomething = TRUE ;
						}
					}				
					
					switch (iColIndex)
					{
					case 0:
						strStartLoc = strCellText ;
						break;
					case 1:					
						strEndLoc = strCellText ;
						break;
					case 2:
						dLen = _tstof(strCellText) ;
						break;
					case 3:
						strComment = strCellText ;
					default:
						break;			
					}
				}
				if (bReadSomething)
				{
					if (strStartLoc.Find(_T("+"))==-1)
					{
						iStartLocNum = _tstoi(strStartLoc) ;
					}
					else if (this->GetNumOfLoc(strStartLoc, iStartLocNum)==FALSE)
					{
						acutPrintf(_T("\n第%d个sheet%d行起点里程%s不符合格式"), iSheetIndex, iCurRow, strStartLoc) ;
					}
					if (strEndLoc.Find(_T("+"))==-1)
					{
						iEndLocNum = _tstoi(strEndLoc) ;
					}
					else if (this->GetNumOfLoc(strEndLoc, iEndLocNum)==FALSE)
					{
						acutPrintf(_T("\n第%d个sheet%d行终点里程%s不符合格式"), iSheetIndex, iCurRow, strEndLoc) ;
					}
					
				strExec.Format(_T("insert into import_dfx_info(start_loc,start_loc_num, end_loc, end_loc_num, len, up_or_down, comment)\
					values('%s',%d,'%s',%d ,%.3f, %d, '%s')"), strStartLoc, iStartLocNum, strEndLoc, iEndLocNum, dLen, iSheetIndex, strComment) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
				}	
				
				if (bCurRowIsBlank)
				{
					bNext = FALSE ;
				}
				else
				{
					iCurRow++ ;
				}

			}	

		}

		acedRestoreStatusBar() ;
		

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return iRet ;

}

//按相对位置（不按比例）绘制 区间信号机 
void CLKDataTable::DrawPdt2()
{
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	//CString strMileSys = _T("DK") ;
	double dPlusForDrawX = 0.012 ; //0.035 ;// 3.5/100 每100米的对应于cad图上3.5个单位长
	double dMileSysFirstLineY[2] ={60-22, -60+19+3} ;//0 ; //刻度尺的第一条横线Y坐标 
	double dMileSysSecondLineY[2] = {55-22, -55+19+3} ;// -5 ; //刻度尺的第二条横线Y坐标
	double dPdFirstHLineY[2] = {52.5-3+8, -42.5+3-8} ; //-7.5 ; //坡度第一条横线Y坐标
	double dPdSecondHLineY[2] = {42.5-3+8, -52.5+3-8} ;//-17.5 ; //坡度第二条横线Y坐标
	double dPlusForDrawSpeedY = 0.1 ;//0.15 ;// 6/40 每40公里的速度对应于cad图上6个单位长
	double dDrawSpeedStartY[2] = {45, -120+3} ;//-6 ; //绘制速度的起始Y坐标，即速度为0时对应的Y坐标
	double dDrawXhjY[2] = {2.7,-2.7} ; //-50 ;//绘制信号机基点Y坐标
	double dDrawStationY[2] = {24.05, -28.25+3+4.2} ;//62 ; //绘制车站基点的Y坐标：速度最大假如450，则450*0.15-6=61.5，车站绘制在速度上方
	double dBetweenUpOrDown = 135 ; //上下行之间距离

	double dBetweenXhj = 10 ; //正常情况前后架信号机距离
	double dBetweenXhjPlus = 2 ; //上下行信号机错开距离
	
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		AcDbBlockReference *pRef = NULL ;
		CStringArray strArrayAtt ;

	
		//begin 绘制信号点
		mysys.Set_Layer(_T(""), _T("signal")) ;		
	
		strSql.Format(_T("select * from compute_xhj_table where type not in(2) order by loc_real asc")) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			if (!cSetFind.IsBOF())
			{
				cSetFind.MoveFirst() ;
			}
			int iRsCount = cSetFind.GetRecordCount() ;
			int iXhIndex = 0 ;
			double dCurDownX = 0, dCurUpX = 0 ; //当前下/上行绘制的x坐标
			double dPreX = 0 ; //上一个绘制的x坐标
			long iPreRealLoc = 0 ; //上一个绘制的信号点的realloc,用来和当前要绘制的比较，如果相等，就用dPreX当做当前的x
			
			CString strBarMsg ;
			strBarMsg.Format(_T("绘制%d个信号点"), iRsCount) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

			while (!cSetFind.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iXhIndex) ;
				
			
				CString strName = _T("") ;
				CString strLocShow = _T("") ;
				CString strLocNum = _T("") ;
				CString strFreq = _T("") ;
				CString strGdName = _T("") ;
				CString strYdqType = _T("") ;
				long iLocNum = 0 ;
				long iLen = 0 ;
				long iXhRealLoc = 0 ;
				int iIsDivide = 0 ;
				int iType = 0 ;
				int iUpOrDown = 0 ;
				int iIsReverse = 0 ;
				cSetFind.GetFieldValue(_T("name"), strName) ;
				cSetFind.GetFieldValue(_T("loc_show"), strLocShow) ;
				cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
				cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
				cSetFind.GetFieldValue(_T("loc_real"), iXhRealLoc) ;
				cSetFind.GetFieldValue(_T("is_divide"), iIsDivide) ;
				cSetFind.GetFieldValue(_T("type"), iType) ;
				cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
				cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
				
				this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;
				
				ads_point pt_insert = {0,0,0};
				AcGePoint3d pt_3d_insert(0,0,0) ;
				
				CString strBlockName = _T("") ;
				
				//车站或中继站
				if (iType==1||iType==11)
				{
					mysys.Set_Layer(_T(""), _T("station")) ;
					pt_insert[0] = dPreX+dBetweenXhj/2 ;
					pt_insert[1] = dDrawStationY[iUpOrDown]+1 ;
					//AcCmColor clr ;
					//clr.setColorIndex(4) ;
					//mysys.WriteText(pt_insert, strName, 0, AcDb::kTextMid, 2, 0.7, clr) ;
					mysys.AddText(strName, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
					pt_insert[1] -= 6 ;
					//mysys.WriteText(pt_insert, strLocNum, 0, AcDb::kTextMid, 2, 0.7, clr) ;
					mysys.AddText(strLocNum, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
					pt_insert[1]+=3 ;
					AcGePoint3d pt_3d_start(dPreX, pt_insert[1], 0) ;
					AcGePoint3d pt_3d_end(dPreX+dBetweenXhj, pt_insert[1], 0) ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
				else if (iType==0||iType==2||iType==3||iType==4)
				{
					//begin len,freq
					if (iUpOrDown==0) //下行，每行对应的就是本信号机防护的长度、频率
					{
						cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;
						cSetFind.GetFieldValue(_T("len"), iLen) ;
					}
					else
					{
						CADORecordset cSetFindTmp ;
						strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down=1 and right(name,1)<>'链' and type not in(1,2,11) order by loc_real desc"), iXhRealLoc) ;
						if (cSetFindTmp.IsOpen())
						{
							cSetFindTmp.Close() ;
						}
						if (cSetFindTmp.Open(g_PtrCon, strSql))
						{
							if (cSetFindTmp.GetRecordCount()>0)
							{
								cSetFindTmp.MoveFirst() ;
								cSetFindTmp.GetFieldValue(_T("set_freq"), strFreq) ;
								cSetFindTmp.GetFieldValue(_T("len"), iLen) ;
							}
							cSetFindTmp.Close() ;
						}
					}
					//end len,freq

					//////////////////////////////////////////////////////////////////////////
					strArrayAtt.RemoveAll() ;
					if (strName.Right(2)==_T("链"))
					{
						strBlockName.Format(_T("pdt_chain_%s"), iUpOrDown==0?_T("x"):_T("s")) ;
						strLocShow.Insert(555,_T("=")) ;
						strLocShow.Insert(555, strLocNum) ;
						CString strChainLen = _T("") ;
						strChainLen.Format(_T("%s=%d"), strName, iLen) ;
						strArrayAtt.Add(strLocShow) ;
						strArrayAtt.Add(strChainLen) ;
						pt_insert[0] = dPreX+dBetweenXhj/2 ;
					}
					else
					{
						if (strName.Left(1)==_T("F"))
						{
							strBlockName.Format(_T("pdt_tg_%s_f"), iUpOrDown==0?_T("x"):_T("s") ) ;
						}
						else if (iType==2)
						{
							strBlockName.Format(_T("pdt_cz_%s"),iUpOrDown==0?_T("x"):_T("s")) ;
							
						}
						else if (iType==3)
						{
							strBlockName.Format(_T("pdt_jz_%s_z"), iUpOrDown==0?_T("x"):_T("s") ) ;
							if (iIsReverse==1)
							{
								strBlockName.Format(_T("pdt_jz_%s_f"),iUpOrDown==0?_T("s"):_T("x") ) ;
							}
						}
						else if (iType==0)
						{
							strBlockName.Format(_T("pdt_tg_%s"),iUpOrDown==0?_T("x"):_T("s") ) ;
						}
						else if (iType==4)
						{
							strBlockName.Format(_T("pdt_jyj_0")) ;
							if (iUpOrDown==0)
							{
								if (strName.Left(1)==_T("S"))
								{
									strBlockName.Format(_T("pdt_cz_s_0")) ;
								}
								else if (strName.Left(1)==_T("X"))
								{
									strBlockName.Format(_T("pdt_cz_x_0")) ;
								}
								
							}
							else if (iUpOrDown==1)
							{
								if (strName.Left(1)==_T("X"))
								{
									strBlockName.Format(_T("pdt_cz_x_1")) ;
								}
								else if (strName.Left(1)==_T("S"))
								{
									strBlockName.Format(_T("pdt_cz_s_1")) ;
								}
								
							}
						}
						strArrayAtt.Add(strName) ;
						strArrayAtt.Add(strLocNum) ;



						//begin 下一个信号点的realloc
						long iXhRealLocNext = 0 ;
						cSetFind.MoveNext() ;
						if (!cSetFind.IsEOF())
						{
							cSetFind.GetFieldValue(_T("loc_real"), iXhRealLocNext) ;
						}
						cSetFind.MovePrevious() ;
						//end 下一个信号点的realloc

						if (iUpOrDown==0) //下行
						{
							if (iXhRealLoc==iPreRealLoc) //该loc和上次的一样，即并置
							{
								dCurDownX = dPreX ;
							}
							else
							{
								dCurDownX+=dBetweenXhj ;
								if (dCurDownX<=dPreX) //
								{
									dCurDownX=dPreX+dBetweenXhjPlus ;
								}								
							}
							if (iXhRealLoc==iXhRealLocNext) //本信号点和下一个并置时
							{
								dCurDownX=dPreX+dBetweenXhj ;
							}
							dPreX = dCurDownX ;
							pt_insert[0] = dCurDownX ;							
						}
						else //上行
						{
							if (iXhRealLoc==iPreRealLoc) //该loc和上次的一样，即并置
							{
								dCurUpX = dPreX ;
							}
							else
							{
								dCurUpX+=dBetweenXhj ;
								if (dCurUpX<=dPreX) //
								{
									dCurUpX = dPreX+dBetweenXhjPlus ; 
								}								
							}
							if (iXhRealLoc==iXhRealLocNext) //本信号点和下一个并置时
							{
								dCurUpX=dPreX+dBetweenXhj ;
							}
							dPreX = dCurUpX ;
							pt_insert[0] = dCurUpX ;						
						}

					}

					CString strLen ;
					strLen.Format(_T("%d"), iLen) ;
					
					//mysys.Set_Layer(_T(""), _T("xhj")) ;	
					pt_insert[1] = dDrawXhjY[iUpOrDown] ;
					mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert ) ;
					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
					{
						pRef->close() ;
						pRef = NULL ;
					}					
					
					//绘制freq len gdname
					//if ((iType==0||iIsReverse==1)&&strName.Right(2)!=_T("链"))
					if (iType!=2&&strName.Right(2)!=_T("链"))
					{
						pt_3d_insert.x = pt_insert[0]+(iUpOrDown==0?6.8:-6.8) ;
						
						strArrayAtt.RemoveAll();
						CString strLenTmp = strLen ;
						strLenTmp.Insert(555, _T("m")) ;
						strArrayAtt.Add(strLenTmp) ;
						strArrayAtt.Add(strFreq) ;						
						strArrayAtt.Add(strGdName) ;
						strBlockName.Format(_T("pdt_freq_%s"), iUpOrDown==0?_T("x"):_T("s")) ;
						mysys.Set_Layer(_T(""), _T("freq")) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
						{
							pRef->close() ;
							pRef = NULL ;
						}								
					}							
					//是否是分界
					if (iIsDivide==1)
					{
						pt_3d_insert.x = pt_insert[0] ;
						pt_3d_insert.y= pt_3d_insert.y+ (iUpOrDown==0?15:-15) ;
						strBlockName.Format(_T("PMT_DIVIDE_0") ) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
						{
							if (iUpOrDown!=0)
							{
								pRef->setRotation(PI) ;
							}
							pRef->close() ;
							pRef = NULL ;
						}	
					}					
				}

				iPreRealLoc = iXhRealLoc ;
				iXhIndex++ ;
				cSetFind.MoveNext() ;
			}	
			//绘制上下行轨道线
			AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
			pt_3d_start.y = pt_3d_end.y = dDrawXhjY[0] ;
			pt_3d_end.x = dCurDownX ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_start.y = pt_3d_end.y = dDrawXhjY[1] ;
			pt_3d_end.x = dCurUpX ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

			cSetFind.Close() ;

			acedRestoreStatusBar() ;

		}
		//end 绘制信号点		
			


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawPdt2")) ;		
	}

}

//导入列控数据表-线路数据表
int CLKDataTable::ImportXHJ_LKDATA(CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_xhj_lkdata")) ;
	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		pWorksheets  = m_pExcel->GetSheets( ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;

		CString strColName[10] = {_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K")} ;

		for (int iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
		{
			vt_index.iVal = iSheetIndex+1 ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;

			int iUpOrDown = (iSheetIndex==0?1:0) ;

			int iCurRow = 4 ;
			BOOL bNext = TRUE ;

			CString strPreStation  ; //前一条记录的车站，车站第一次出现的记录为分界
			
			while (bNext)
			{
				BOOL bCurRowIsBlank = TRUE ;
				CString strBarMsg ;
				strBarMsg.Format(_T("读取列控数据之信号数据：第%d行"), iCurRow) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, 9) ;	


				CString strStationName = _T(""),  strXhjName = _T(""), strLocShow = _T(""), strXhjType = _T(""), strJyjType = _T(""), strGdName = _T(""), strSetFreq = _T(""), strGdAtt = _T(""), strComment = _T("") ;
				long iLocShow = 0 ;
				long iLocReal = 0 ;
				int iLen = 0;

				int iIsDivide = 0 ;
				int iIsReverse = 0 ;
				int iType = 0 ;

				long iLocNum = 0 ;

				CString strLongShortStart = _T("") ;
				long iLongShortLen = 0 ;
				long iLongShortStart = 0 ;

				BOOL bReadSomething = FALSE ;
				for (int iColIndex=0; iColIndex<10; iColIndex++)
				{

					acedSetStatusBarProgressMeterPos(iColIndex) ;

					CString strCellName = _T(""), strCellText = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					
					if (iColIndex==0)
					{
						if (strCellText==_T(""))
						{
							break ;
						}
						else
						{
							bReadSomething = TRUE ;
							iRet++ ;
						}
					}					
					
					switch (iColIndex)
					{
					case 0:
						strStationName = strCellText ;
						break;
					case 1:
						{
							strXhjName = strCellText ;
						}
						break;
					case 2:
						strLocShow = strCellText ;
						break;
					case 3:
						strXhjType = strCellText ;
						break;
					case 4:
						strJyjType = strCellText ;
						break;
					case 5:
						strGdName = strCellText ;							
						break;
					case 6:
						strSetFreq = strCellText ;
						break;
					case 7:
						iLen = _tstol(strCellText) ;						
						break;
					case 8:
						strGdAtt = strCellText ;
						break;
					case 9:
						strComment = strCellText ;
						break;
					}
				}
				if (bReadSomething)
				{
					//显示里程转换为数值
					if (strLocShow.Find(_T("+"))==-1)
					{
						iLocShow = _tstol(strLocShow) ;
					}
					else if (this->GetNumOfLoc(strLocShow, iLocShow)==FALSE)
					{
						acutPrintf(_T("\n第%d行设置里程%s不符合格式"), iCurRow, strLocShow) ;
					}

					//modified in 20120612
					//20120613

					iLocReal = iLocShow ;
					iLocNum = iLocShow ;

					if (strPreStation!=strStationName)
					{
						iIsDivide = 1 ;
						strPreStation = strStationName ;
					}
					if (strXhjName==_T(""))//站内绝缘节
					{
						iType = 4 ;
					}
					else if (strXhjType==_T("进站信号机"))
					{
						iType = 3 ;
					}
					else if (strXhjType==_T("出站信号机"))
					{
						iType = 2 ;
					}
					else if (strXhjType==_T("通过信号机"))
					{
						iType = 0 ;
					}
					else if (strXhjType==_T("出站口"))
					{
						iType = 3 ;
						iIsReverse = 1 ;
					}
					else if (strXhjType==_T("没有信号机"))
					{
						if (strJyjType.Left(4)==_T("机械"))
						{
							iType = 4 ;
						}
						else
						{
							iType = 0 ;
						}
					}
										
					strExec.Format(_T("insert into import_xhj_lkdata(xhj_name, loc_show, loc_show_num, loc_num, loc_real, type, jyj_type, gd_name, freq, len, gd_attribute, up_or_down, belong_station,comment, is_divide, is_reverse, type_num)\
						values('%s','%s',%d, %d, %d, '%s','%s','%s','%s',%d, '%s',%d,'%s','%s', %d, %d, %d)"), strXhjName, strLocShow, iLocShow, iLocNum, iLocReal, strXhjType, strJyjType, strGdName, strSetFreq, iLen, strGdAtt, iUpOrDown, strStationName,strComment, iIsDivide, iIsReverse, iType) ;
					ExecSqlString(strExec) ;

					CString strReg = _T("^([k,K]\\d+\\+\\d+)\\D+(\\d+)m$") ; //举例："K100+200,短链100m",长短链不单独成行，写在备注栏
					if (CBlkUtility::IsStrMatch(strReg, strComment))
					{
						strXhjName = _T("长链") ;
						CString strLongShortLen = _T("") ;
						
						CBlkUtility::RegReplace(strReg, _T("$1"), strComment, strLongShortStart);
						CBlkUtility::RegReplace(strReg, _T("$2"), strComment, strLongShortLen);
						iLongShortLen = _tstol(strLongShortLen) ;
						long iLongShortLenTmp = iLongShortLen ;

						if (strComment.Find(_T("短链"))!=-1)
						{
							iLongShortLenTmp = 0-iLongShortLenTmp ;
							strXhjName = _T("短链") ;
						}
						this->GetNumOfLoc(strLongShortStart, iLongShortStart) ;

						CString strLsLocShow ;
						long iLsLocShow = 0, iLsLocNum = 0, iLsLocReal = 0 ;

						//if (iUpOrDown==0)
						//{
							strLsLocShow = strLongShortStart ;
							iLsLocShow = iLongShortStart ;
							iLsLocNum = iLsLocShow-iLongShortLenTmp ;
							iLsLocReal = iLsLocShow ;
						//}
						//else 
						//{
// 							iLocShow = iLongShortStart+iLongShortLenTmp ;
// 							this->GetLocShow(iLocShow, _T("K"), strLocShow) ;
// 							iLocNum = iLongShortStart ;	

							//modified in 20120612:上行计算断链不对（给哈大出平面图）

// 							iLsLocShow = iLongShortStart ;
// 							this->GetLocShow(iLsLocShow, _T("K"), strLsLocShow) ;
// 							iLsLocNum = iLongShortStart-iLongShortLenTmp ;	
// 							iLsLocReal = iLocReal-(iLocNum-iLsLocNum) ;

						//}
						//iLocReal = iLocShow ;

						strExec.Format(_T("insert into import_xhj_lkdata(xhj_name, loc_show, loc_show_num, loc_num, loc_real,  len, up_or_down, belong_station)\
							values('%s','%s',%d, %d, %d, %d,%d,'%s')"), strXhjName, strLsLocShow, iLsLocShow, iLsLocNum, iLsLocReal, iLongShortLen, iUpOrDown, strStationName) ;
						
						ExecSqlString(strExec) ;
					}



					bCurRowIsBlank = FALSE ;	
				}				

				if (bCurRowIsBlank)
				{
					bNext = FALSE ;
				}
				else
				{
					iCurRow++ ;
				}

				acedRestoreStatusBar() ;

			}
		}

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}

	this->UpDateAllRealLoc_LKDATA(_T("import_xhj_lkdata")) ;

	return iRet ;
	
}

//导入列控数据表-应答器位置表
int CLKDataTable::ImportYdq_LKDATA(CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_ydq_lkdata")) ;
	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		pWorksheets  = m_pExcel->GetSheets( ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;

		CString strColName[10] = {_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K")} ;

		for (int iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
		{
			vt_index.iVal = iSheetIndex+1 ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;

			int iUpOrDown = 1-iSheetIndex ;

			int iCurRow = 3 ;
			BOOL bNext = TRUE ;
			while (bNext)
			{
				BOOL bCurRowIsBlank = TRUE ;
				CString strBarMsg ;
				strBarMsg.Format(_T("读取列控数据之应答器数据：第%d行"), iCurRow) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, 9) ;	


				CString strStationName = _T(""),  strYdqName = _T(""), strYdqSn = _T(""), strLocShow = _T(""), strDevType = _T(""), strUseage = _T(""), strComment = _T(""), strTNum = _T(""), strLongShortStr = _T("");
				long iLocShow = 0 ;
				long iLocReal = 0 ;
				long iNID_BG = 0;				
				
				CString strLongShortStart = _T("") ;
				long iLongShortLen = 0 ;
				long iLongShortStart = 0 ;

				BOOL bReadSomething = FALSE ;
				for (int iColIndex=0; iColIndex<10; iColIndex++)
				{

					acedSetStatusBarProgressMeterPos(iColIndex) ;

					CString strCellName = _T(""), strCellText = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					
					if (iColIndex==0)
					{
						if (strCellText==_T(""))
						{
							break ;
						}
						else
						{
							bReadSomething = TRUE ;
							iRet++ ;
						}
					}
					
					
					switch (iColIndex)
					{
					case 0:
						strYdqName = strCellText ;
						break;
					case 1:
						{
							strYdqSn = strCellText ;
						}
						break;
					case 2:
						strLocShow = strCellText ;
						break;
					case 3:
						strDevType = strCellText ;
						break;
					case 4:
						strUseage = strCellText ;
						break;
					case 5:
						strComment = strCellText ;							
						break;
					case 6:
						strStationName = strCellText ;
						break;
					case 7:
						strTNum = strCellText ;						
						break;
					case 8:
						iNID_BG = _tstol(strCellText) ;
						break;
					case 9:
						strLongShortStr = strCellText ;
						break;
					}
				}
				if (bReadSomething)
				{
					//显示里程转换为数值
					if (strLocShow.Find(_T("+"))==-1)
					{
						iLocShow = _tstol(strLocShow) ;
					}
					else if (this->GetNumOfLoc(strLocShow, iLocShow)==FALSE)
					{
						acutPrintf(_T("\n第%d行设置里程%s不符合格式"), iCurRow, strLocShow) ;
					}

					iLocReal = iLocShow ;

					CString strReg = _T("^([k,K]\\d+\\+\\d+)\\D+(\\d+)m$") ; //举例："K100+200,短链100m",长短链不单独成行，写在备注栏
					if (CBlkUtility::IsStrMatch(strReg, strLongShortStr))
					{
						CString strLongShortLen = _T("") ;
						CBlkUtility::RegReplace(strReg, _T("$1"), strLongShortStr, strLongShortStart);
						CBlkUtility::RegReplace(strReg, _T("$2"), strLongShortStr, strLongShortLen);
						iLongShortLen = _tstol(strLongShortLen) ;
						if (strLongShortStr.Find(_T("短链"))!=-1)
						{
							iLongShortLen = 0-iLongShortLen ;
						}
						this->GetNumOfLoc(strLongShortStart, iLongShortStart) ;
					}
										
					strExec.Format(_T("insert into import_ydq_lkdata(ydq_name, ydq_sn, loc_show, loc_show_num, loc_real, dev_type, useage, belong_station, comment, up_or_down, t_number, nid_bg, longshort_str, longshort_len, longshort_start, longshort_start_num)\
						values('%s','%s','%s', %d, %d,'%s','%s','%s','%s', %d,'%s',%d,'%s',%d,'%s',%d)"), strYdqName, strYdqSn, strLocShow, iLocShow, iLocReal, strDevType, strUseage, strStationName, strComment, iUpOrDown, strTNum, iNID_BG, strLongShortStr, iLongShortLen, strLongShortStart, iLongShortStart ) ;
					ExecSqlString(strExec) ;
					bCurRowIsBlank = FALSE ;	
				}				

				if (bCurRowIsBlank)
				{
					bNext = FALSE ;
				}
				else
				{
					iCurRow++ ;
				}

				acedRestoreStatusBar() ;

			}
		}

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}

	this->UpDateAllRealLoc_LKDATA(_T("import_ydq_lkdata")) ;

	return iRet ;
	
}


void CLKDataTable::UpDateAllRealLoc_LKDATA(CString strDbTable)
{
	try
	{
		CString strSql = _T("") ;
		CADORecordset cSet ;
		for (int iUpOrDown=0; iUpOrDown<2; iUpOrDown++)
		{
			if (strDbTable==_T("import_ydq_lkdata"))
			{
				strSql.Format(_T("select * from %s where longshort_len<>0 and up_or_down=%d order by id %s"), strDbTable, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
			}
			else if (strDbTable==_T("import_xhj_lkdata"))
			{
				strSql.Format(_T("select * from %s where right(xhj_name,1)='链' and up_or_down=%d order by id %s"), strDbTable, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
			}
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				int iLongShortChainSum = cSet.GetRecordCount()-1 ;
				CString strBarMsg ;
				strBarMsg.Format(_T("处理%s%d个长短链"), iUpOrDown==0?_T("下行"):_T("上行"), iLongShortChainSum) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iLongShortChainSum) ;	

				int iLongShortChainIndex = 0 ;
				while (!cSet.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iLongShortChainIndex) ;

					
					long iId = 0 ;
					long iLen = 0 ;
					CString strName = _T("") ;
					
					cSet.GetFieldValue(_T("id"), iId) ;
					//cSet.GetFieldValue(_T("name"), strName) ;
					if (strDbTable==_T("import_ydq_lkdata"))
					{
						cSet.GetFieldValue(_T("longshort_len"), iLen) ;
					}
					else 
					{
						cSet.GetFieldValue(_T("len"), iLen) ;
						cSet.GetFieldValue(_T("xhj_name"), strName) ;
						if (strName==_T("短链"))
						{
							iLen = 0-iLen ;
						}
					}			

					CADORecordset cSetUpdate ;
					strSql.Format(_T("select * from %s where id>%d and up_or_down=%d order by id %s"), strDbTable, iId, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
					if (iUpOrDown>0)
					{
						strSql.Format(_T("select * from %s where id<%d and up_or_down=%d order by id %s"), strDbTable, iId, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;

						if (strDbTable==_T("import_ydq_lkdata"))
						{
							strSql.Format(_T("select * from %s where id<=%d and up_or_down=%d order by id %s"), strDbTable, iId, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
						}
					}
					if (cSetUpdate.IsOpen())
					{
						cSetUpdate.Close() ;
					}
					if (cSetUpdate.Open(g_PtrCon, strSql))
					{
						if (!cSetUpdate.IsBOF())
						{
							cSetUpdate.MoveFirst() ;
						}
						while (!cSetUpdate.IsEOF())
						{
							long iRealLoc = 0 ;
							cSetUpdate.GetFieldValue(_T("loc_real"), iRealLoc) ;
							long iNewRealLoc = 0 ;
							iNewRealLoc = iRealLoc+iLen ;
							cSetUpdate.Edit() ;
							cSetUpdate.SetFieldValue(_T("loc_real"), iNewRealLoc) ;
							cSetUpdate.Update() ;
							cSetUpdate.MoveNext() ;
						}
						cSetUpdate.Close() ;
					}
					
					
					//strSql.Format(_T("UPDATE import_xhj_table SET loc_real=loc_real%s where id>=%d"), strTmp, iId) ;
					//ExecSqlString(strSql) ;
					
					iLongShortChainIndex++ ;
					
					cSet.MoveNext() ;
					
				}
				cSet.Close() ;

				acedRestoreStatusBar() ;

			}
			
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in UpDateAllRealLoc_LKDATA")) ;
		
	}
	
}

//按比例绘制,由列控数据表调入数据
void CLKDataTable::DrawPdt_LKDATA()
{
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	//CString strMileSys = _T("DK") ;
	double dPlusForDrawX = 0.035 ;// 3.5/100 每100米的对应于cad图上3.5个单位长
	double dMileSysFirstLineY[2] ={60-22, -60+19+3} ;//0 ; //刻度尺的第一条横线Y坐标 
	double dMileSysSecondLineY[2] = {55-22, -55+19+3} ;// -5 ; //刻度尺的第二条横线Y坐标
	double dPdFirstHLineY[2] = {52.5-3+8, -42.5+3-8} ; //-7.5 ; //坡度第一条横线Y坐标
	double dPdSecondHLineY[2] = {42.5-3+8, -52.5+3-8} ;//-17.5 ; //坡度第二条横线Y坐标
	double dPlusForDrawSpeedY = 0.1 ;//0.15 ;// 6/40 每40公里的速度对应于cad图上6个单位长
	double dDrawSpeedStartY[2] = {45, -120+3} ;//-6 ; //绘制速度的起始Y坐标，即速度为0时对应的Y坐标
	double dDrawXhjY[2] = {6,-6} ; //-50 ;//绘制信号机基点Y坐标
	double dDrawStationY[2] = {28.25, -28.25+3} ;//62 ; //绘制车站基点的Y坐标：速度最大假如450，则450*0.15-6=61.5，车站绘制在速度上方
	double dBetweenUpOrDown = 135 ; //上下行之间距离
	
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		AcDbBlockReference *pRef = NULL ;
		CStringArray strArrayAtt ;

		//begin 取得线路起始里程
		//long iStartLocShow = 0, iEndLocShow = 0 ; //起始点显示里程
		long iStartLocNum = 0, iEndLocNum = 0 ; //起始点里程
		long iStartRealLoc = 0, iEndRealLoc = 0 ; //起始点实际里程
		strSql.Format(_T("select max(loc_show_num) as max_show, min(loc_show_num) as min_show,max(loc_real) as max_real, min(loc_real) as min_real from import_xhj_lkdata")) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			if (!cSetFind.IsBOF())
			{
				cSetFind.MoveFirst() ;
			}	
			CString strLocTmp = _T("") ;
			//cSetFind.GetFieldValue(_T("loc_show"), strLocTmp) ;
			//this->GetNumOfLoc(strLocTmp, iStartLocShow) ;
			cSetFind.GetFieldValue(_T("min_show"), iStartLocNum) ;
			cSetFind.GetFieldValue(_T("min_real"), iStartRealLoc) ;
			//cSetFind.MoveLast() ;
			//cSetFind.GetFieldValue(_T("loc_show"), strLocTmp) ;
			//this->GetNumOfLoc(strLocTmp, iEndLocShow) ;
			cSetFind.GetFieldValue(_T("max_show"), iEndLocNum) ;
			cSetFind.GetFieldValue(_T("max_real"), iEndRealLoc) ;

			cSetFind.Close() ;
		}
		//end 取得线路起始里程

		for (int iUpOrDownIndex=0; iUpOrDownIndex<2; iUpOrDownIndex++)
		{			
			//begin 绘制标尺
			//绘制标尺横线贯通直线
			mysys.Set_Layer(_T(""), _T("ruler")) ;
			AcGePoint3d pt_3d_start(iStartRealLoc*dPlusForDrawX, dMileSysFirstLineY[iUpOrDownIndex], 0) ;
			AcGePoint3d pt_3d_end(iEndRealLoc*dPlusForDrawX, dMileSysFirstLineY[iUpOrDownIndex], 0) ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_start.y = pt_3d_end.y = dMileSysSecondLineY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			mysys.Set_Layer(_T(""), _T("pd_line")) ;
			pt_3d_start.y = pt_3d_end.y = dPdFirstHLineY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_start.y = pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			

			mysys.Set_Layer(_T(""), _T("ruler")) ;
			//查询有多少个里程变换（长短链）
			CStringArray strArrayChainLocShow, strArrayChainLocNum, strArrayChainLocReal ; //保存长短链的几个里程

			int iChainRsCount = 0 ;
			strSql.Format(_T("select * from import_xhj_lkdata where right(xhj_name,1)='链' and up_or_down=%d order by loc_real asc"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				iChainRsCount = cSetFind.GetRecordCount() ;		
				while (!cSetFind.IsEOF())
				{


					long iLongLocNum = 0, iLongLocReal = 0 ;
				
					CString strLocTmp = _T("") ;
					
					cSetFind.GetFieldValue(_T("loc_show"), strLocTmp) ;
					//this->GetNumOfLoc(strLocTmp, iLongLocShow) ;
					strArrayChainLocShow.Add(strLocTmp) ;

					cSetFind.GetFieldValue(_T("loc_num"), iLongLocNum) ;
					cSetFind.GetFieldValue(_T("loc_real"), iLongLocReal) ;

					CString strLongLocNum = _T(""), strLongLocReal = _T("") ;
					
					strLocTmp.Format(_T("%d"), iLongLocNum) ;
					strArrayChainLocNum.Add(strLocTmp) ;
					strLocTmp.Format(_T("%d"), iLongLocReal) ;
					strArrayChainLocReal.Add(strLocTmp) ;					
					
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}

			long iMileSysStartLoc = iStartLocNum ; //每个里程体系的起始值
			long iMileSysStartLocReal = iStartRealLoc ; //每个里程体系的起始实际值
			if (iChainRsCount==0)//没有长短链
			{
				CString strTmp = _T("") ;
				strTmp.Format(_T("%d"), iEndRealLoc) ;
				strArrayChainLocShow.Add(strTmp) ;				
				strArrayChainLocNum.Add(strTmp) ;				
				strArrayChainLocReal.Add(strTmp) ;
				iChainRsCount = 1 ;
			}
			for (int iChainIndex=0; iChainIndex<iChainRsCount; iChainIndex++)
			{
				CString strChainLocShow = strArrayChainLocShow.GetAt(iChainIndex) ;
				CString strChainLocNum = strArrayChainLocNum.GetAt(iChainIndex) ;
				CString strChainLocReal = strArrayChainLocReal.GetAt(iChainIndex) ;

				long iChainLocShow = 0 ; // _tstol(strChainLocShow) ;
				if (strChainLocShow.Find(_T("+"))!=-1)
				{
					this->GetNumOfLoc(strChainLocShow, iChainLocShow) ;
				}
				else
				{
					iChainLocShow = _tstol(strChainLocShow) ;
				}
				
				long iChainLocNum = _tstol(strChainLocNum) ;
				long iChainLocReal = _tstol(strChainLocReal) ;

				double dLongX = iChainLocReal*dPlusForDrawX ;
				
				AcGePoint3d pt_3d_start(dLongX, dMileSysFirstLineY[iUpOrDownIndex], 0) ;
				AcGePoint3d pt_3d_end(dLongX, dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-3:3), 0) ;
				ads_point pt_base_text = {dLongX, dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?2:-2), 0} ;
				CString strText = _T("") ;

				if (iChainRsCount>1)
				{
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;
					CString strChianLocNum_DK ;
					this->GetLocShow(iChainLocNum, this->m_strMileSysName, strChianLocNum_DK) ;
					strText.Format(_T("%s=%s"), strChainLocShow, strChianLocNum_DK) ;
					//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.6) ;
					mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.6, 0, 0) ;
				}				
				
				long iCurStartKm = iMileSysStartLoc%1000>0?(iMileSysStartLoc/1000+1):(iMileSysStartLoc/1000) ;
				long iCurEndKm = iChainLocShow/1000 ;

				long iTmp1 = iMileSysStartLoc%1000 ;
				if (iTmp1>0) //例如起始里程（iMileSysStartLoc）为K2+123，则iCurStartKm为K3，那么从K2->K3之间的百米标就漏画了
				{
					double dKmX = (iMileSysStartLoc/1000*1000-iMileSysStartLoc+iMileSysStartLocReal)*dPlusForDrawX ;
					for (int iMileIndex=(iTmp1%100>0?(iMileSysStartLoc%1000/100+1):(iMileSysStartLoc%1000/100)); iMileIndex<10; iMileIndex++)
					{
						double dMx =dKmX+iMileIndex*3.5  ;
						pt_3d_start.x = pt_3d_end.x = dMx;
						pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
						pt_base_text[0] = dMx ;
						pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 				
						strText.Format(_T("%d"), iMileIndex) ;
						//AcCmColor clr ;
						//clr.setColorIndex(7) ;
						//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;	
						mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 7) ;
					}					
				}
				long iTmp2 = iChainLocShow%1000 ;
				if (iTmp2>0)  
				{
					double dKmX = (iChainLocShow/1000*1000-iMileSysStartLoc+iMileSysStartLocReal)*dPlusForDrawX ;
					for (int iMileIndex=1; iMileIndex<(iTmp2%100>0?(iChainLocShow%1000/100+1):(iChainLocShow%1000/100)); iMileIndex++)
					{
						double dMx =dKmX+iMileIndex*3.5  ;
						pt_3d_start.x = pt_3d_end.x = dMx;
						pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
						pt_base_text[0] = dMx ;
						pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 				
						strText.Format(_T("%d"), iMileIndex) ;
						//AcCmColor clr ;
						//clr.setColorIndex(7) ;
						//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;
						mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 7) ;
					}					
				}
				
				//绘制千米标
				for (long i=iCurStartKm; i<=iCurEndKm; i++)
				{
					double dKmX = (i*1000-iMileSysStartLoc+iMileSysStartLocReal)*dPlusForDrawX ;
					pt_3d_start.x = pt_3d_end.x = dKmX ;
					pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-1:1) ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
					pt_base_text[0] = dKmX ;
					pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 					
					strText.Format(_T("%s%d"), m_strMileSysName, i) ;
					//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.5) ;
					mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.5) ;
					
					//绘制百米标
					if (i<iCurEndKm)
					{
						for (int iMileIndex=1; iMileIndex<10; iMileIndex++)
						{
							double dMx =dKmX+iMileIndex*3.5  ;
							pt_3d_start.x = pt_3d_end.x = dMx;
							pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ;
							mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
							pt_base_text[0] = dMx ;
							pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 				
							strText.Format(_T("%d"), iMileIndex) ;
							//AcCmColor clr ;
							//clr.setColorIndex(7) ;
							//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;	
							mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 7) ;
						}
					}
				}
				
				
				if (iChainIndex==iChainRsCount-1) //最后一个长短链
				{
					iCurStartKm = iChainLocNum%1000>0?(iChainLocNum/1000+1):(iChainLocNum/1000) ;
					iCurEndKm = iEndLocNum/1000 ;

					for (long i=iCurStartKm; i<=iCurEndKm; i++)
					{
						double dKmX = (i*1000-iChainLocNum+iChainLocReal)*dPlusForDrawX ;
						pt_3d_start.x = pt_3d_end.x = dKmX ;
						pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-1:1) ; 
						
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
						pt_base_text[0] = dKmX ;
						pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 					
						strText.Format(_T("K%d"), i) ;
						//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.5) ;
						mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.5) ;
						
						//绘制百米标
						if (i<iCurEndKm)
						{
							for (int iMileIndex=1; iMileIndex<10; iMileIndex++)
							{
								double dMx =dKmX+iMileIndex*3.5  ;
								pt_3d_start.x = pt_3d_end.x = dMx;
								pt_3d_end.y = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-0.5:0.5) ; 
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ;
								pt_base_text[0] = dMx ;
								pt_base_text[1] = dMileSysFirstLineY[iUpOrDownIndex]+(iUpOrDownIndex==0?-2.5:2.5) ; 					
								strText.Format(_T("%d"), iMileIndex) ;
								//AcCmColor clr ;
								//clr.setColorIndex(7) ;
								//mysys.WriteText(pt_base_text, strText, 0, AcDb::kTextMid, 2, 0.5, clr) ;
								mysys.AddText(strText, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.5, 0, 7) ;
							}
						}
					}
				}
				
				iMileSysStartLoc = iChainLocNum ;
				iMileSysStartLocReal = iChainLocReal ;
			}

			//end 绘制标尺
		
			//begin 绘制坡度
			mysys.Set_Layer(_T(""), _T("pd")) ;
			//begin 找出坡度上下行起始loc
			double dPdStartLoc = 0 ;			
			strSql.Format(_T("select top 1 * from import_pd_info where up_or_down=%d order by id %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc") ) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;

					double dEndLoc = 0 ;
					cSetFind.GetFieldValue(_T("end_loc_num"), dEndLoc) ;
					if (iUpOrDownIndex==0)//down
					{
						double dFirstPdLen = 0 ;
						cSetFind.GetFieldValue(_T("len"), dFirstPdLen) ;
						dPdStartLoc = dEndLoc-dFirstPdLen ;
					}
					else if (iUpOrDownIndex==1)//up
					{
						double dPdSumLen = 0 ;
						CADORecordset cSetGetAllLen ;
						strSql.Format(_T("select sum(len) as pdsumlen from import_pd_info where up_or_down=1 ")) ;
						if (cSetGetAllLen.IsOpen())
						{
							cSetGetAllLen.Close() ;
						}
						if (cSetGetAllLen.Open(g_PtrCon, strSql))
						{
							cSetGetAllLen.GetFieldValue(_T("pdsumlen"), dPdSumLen) ;
							cSetGetAllLen.Close() ;
						}
						dPdStartLoc = dEndLoc+dPdSumLen ;
					}
				}

				cSetFind.Close() ;
			}			
			//end 找出坡度上下行起始loc

			strSql.Format(_T("select * from import_pd_info where up_or_down=%d order by id "), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				double dCurStartRealLoc = dPdStartLoc ; //iUpOrDownIndex==0?1086:1309693 ; //当前起始loc_real 1086:921622			
				while (!cSetFind.IsEOF())
				{
					CString strPd = _T("") ;
					CString strEndLoc = _T("") ;
					double dLen = 0 ;
					cSetFind.GetFieldValue(_T("pd"), strPd) ;
					cSetFind.GetFieldValue(_T("len"), dLen) ;
					cSetFind.GetFieldValue(_T("end_loc"), strEndLoc) ;
					
					ads_point pt_base_text = {0, 0, 0} ;
					//AcCmColor clr ;
					
					double dPd = 0 ;
					dPd = _tstof(strPd) ;
					pt_3d_start.x = dCurStartRealLoc*dPlusForDrawX ;
					pt_3d_end.x = (dCurStartRealLoc+(iUpOrDownIndex==0?dLen:-dLen))*dPlusForDrawX ;
					double dMidX = (dCurStartRealLoc+(iUpOrDownIndex==0?dLen/2:-dLen/2))*dPlusForDrawX ;
					double dMidY = dPdFirstHLineY[iUpOrDownIndex]-(dPdFirstHLineY[iUpOrDownIndex]-dPdSecondHLineY[iUpOrDownIndex])/2 ;
					
					if (dPd==0)
					{
						pt_3d_start.y = pt_3d_end.y = dMidY;
					}
					else if (dPd>0)
					{
						pt_3d_start.y = dPdSecondHLineY[iUpOrDownIndex] ;
						pt_3d_end.y = dPdFirstHLineY[iUpOrDownIndex] ;
					}
					else
					{
						pt_3d_start.y = dPdFirstHLineY[iUpOrDownIndex] ;
						pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex] ;
					}
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 2) ;//坡度线 黄色 2
					pt_3d_start.x = pt_3d_end.x ;
					pt_3d_start.y = dPdFirstHLineY[iUpOrDownIndex] ;
					pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex] ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 7) ; //坡度界限 竖线
					
					pt_base_text[0] = dMidX ;
					pt_base_text[1] = dMidY+1.2 ;			
					//clr.setColorIndex(6) ; //
					//mysys.WriteText(pt_base_text, strPd, 0, AcDb::kTextMid, 1.5, 0.5, clr) ;  
					mysys.AddText(strPd, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 6) ;//坡度值 AcMagenta 6 洋红
					pt_base_text[1] = dMidY-1.2 ;
					//clr.setColorIndex(3) ; //
					CString strLen = _T("") ;
					strLen.Format(_T("%.3f"), dLen) ;
					strLen.TrimRight(_T("0")) ;
					strLen.TrimRight(_T(".")) ;
					//mysys.WriteText(pt_base_text, strLen, 0, AcDb::kTextMid, 1.5, 0.5, clr) ; 
					mysys.AddText(strLen, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 3) ;//坡长AcGreen 3 绿色
					
					pt_3d_start.x = pt_3d_end.x = (dCurStartRealLoc+(iUpOrDownIndex==0?dLen:-dLen))*dPlusForDrawX ;
					if (iUpOrDownIndex==0)
					{
						pt_3d_start.y = dPdSecondHLineY[iUpOrDownIndex]-2 ;
						pt_3d_end.y = dPdSecondHLineY[iUpOrDownIndex]-8 ;
					}
					else 
					{
						pt_3d_start.y = dPdFirstHLineY[iUpOrDownIndex]+2 ;
						pt_3d_end.y = dPdFirstHLineY[iUpOrDownIndex]+8 ;

					}
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 4) ;//坡度endloc线 acCyan 4 青 
					
					//clr.setColorIndex(4) ; //acCyan 4 青 
					pt_base_text[0] = pt_3d_end.x-0.5 ;
					pt_base_text[1] = pt_3d_start.y ;
					//mysys.WriteText(pt_base_text, strEndLoc, PI/2, iUpOrDownIndex==0?AcDb::kTextRight:AcDb::kTextLeft, 1.5, 0.5, clr) ; //坡度endloc acCyan 4 青
					mysys.AddText(strEndLoc, pt_base_text, pt_base_text, iUpOrDownIndex==0?AcDb::kTextRight:AcDb::kTextLeft, AcDb::kTextBase, 1.5, 0.5, PI/2, 4) ;//坡度endloc acCyan 4 青
					
					dCurStartRealLoc=dCurStartRealLoc+(iUpOrDownIndex==0?dLen:-dLen) ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
			//end 绘制坡度
		
			
			//begin 绘制速度 
			mysys.Set_Layer(_T(""), _T("speed")) ;
			//begin 找出速度上下行起始loc
			double dSpeedStartLoc = 0 ;			
			strSql.Format(_T("select top 1 * from import_speed where up_or_down=%d and is_reverse=0 order by id %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc") ) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;
					double dEndLoc = 0 ;
					cSetFind.GetFieldValue(_T("end_loc_num"), dEndLoc) ;
					if (iUpOrDownIndex==0)//down
					{
						double dFirstSpeedLen = 0 ;
						cSetFind.GetFieldValue(_T("len"), dFirstSpeedLen) ;
						dSpeedStartLoc = dEndLoc-dFirstSpeedLen ;
					}
					else if (iUpOrDownIndex==1)//up
					{
						double dSpeedSumLen = 0 ;
						CADORecordset cSetGetAllLen ;
						strSql.Format(_T("select sum(len) as speedsumlen from import_speed where up_or_down=1 and is_reverse=0 ")) ;
						if (cSetGetAllLen.IsOpen())
						{
							cSetGetAllLen.Close() ;
						}
						if (cSetGetAllLen.Open(g_PtrCon, strSql))
						{
							cSetGetAllLen.GetFieldValue(_T("speedsumlen"), dSpeedSumLen) ;
							cSetGetAllLen.Close() ;
						}
						dSpeedStartLoc = dEndLoc+dSpeedSumLen ;
					}
				}
				
				cSetFind.Close() ;
			}			
			//end 找出速度上下行起始loc

			strSql.Format(_T("select * from import_speed where up_or_down=%d and is_reverse=0 order by id asc"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				double dCurStartRealLoc = dSpeedStartLoc ;//iUpOrDownIndex==0?591974:864362 ; //当前起始loc_real
				int iPreSpeed = -1 ; //记录前一个速度值
				CString strPreEndLoc = _T("") ;
				while (!cSetFind.IsEOF())
				{
					CString strSpeed = _T(""), strLen = _T("") ;
					int iSpeed = 0 ;
					CString strEndLoc = _T("") ;
					long iLen = 0 ;
					cSetFind.GetFieldValue(_T("speed"), iSpeed) ;
					cSetFind.GetFieldValue(_T("len"), iLen) ;
					cSetFind.GetFieldValue(_T("end_loc"), strEndLoc) ;
					
					strSpeed.Format(_T("%d"), iSpeed) ;
					strLen.Format(_T("%d"), iLen) ;
					
					ads_point pt_base_text = {0, 0, 0} ;
					//AcCmColor clr ;
					
					pt_3d_start.x = dCurStartRealLoc*dPlusForDrawX ;
					pt_3d_end.x = (dCurStartRealLoc+(iUpOrDownIndex==0?iLen:-iLen))*dPlusForDrawX ;
					pt_3d_start.y = pt_3d_end.y = dDrawSpeedStartY[iUpOrDownIndex]+iSpeed*dPlusForDrawSpeedY ;
					//double dMidX = (dCurStartRealLoc+(dLen/2))*dPlusForDrawX ;
					//double dMidY = dPdFirstHLineY-(dPdFirstHLineY-dPdSecondHLineY)/2 ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;//速度线 红色 1 
					
					pt_base_text[0] = (dCurStartRealLoc+(iUpOrDownIndex==0?(iLen/2):(-iLen/2)))*dPlusForDrawX ;
					pt_base_text[1] = pt_3d_start.y+1.2 ;
					//clr.setColorIndex(6) ; //
					//mysys.WriteText(pt_base_text, strSpeed, 0, AcDb::kTextMid, 1.5, 0.5, clr) ; //速度endloc acCyan 4 青
					mysys.AddText(strSpeed, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 6) ;//速度endloc 洋红
					pt_base_text[1] = pt_3d_start.y-1.2 ;
					//clr.setColorIndex(3) ; //绿
					CString strLenWrite = strLen ;
					strLenWrite.Insert(555, _T("m")) ;
					//mysys.WriteText(pt_base_text, strLenWrite, 0, AcDb::kTextMid, 1.5, 0.5, clr) ; //速度endloc acCyan 4 青
					mysys.AddText(strLenWrite, pt_base_text, pt_base_text, AcDb::kTextMid, AcDb::kTextBase, 1.5, 0.5, 0, 3) ;//速度endloc 绿
					
					if (iPreSpeed!=-1)//prespeed还是初始赋值-1时表示这是第一个速度段
					{
						pt_3d_end.x = pt_3d_start.x ;
						pt_3d_start.y = dDrawSpeedStartY[iUpOrDownIndex]+iPreSpeed*dPlusForDrawSpeedY ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;//前一个速度和当前速度的折线 红色 1 
						
						pt_3d_start.y = max(pt_3d_start.y, pt_3d_end.y) + 1 ;
						pt_3d_end.y = pt_3d_start.y+9 ;
						mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 2) ;//endloc 黄色 2
						
						//clr.setColorIndex(2) ; //黄2 
						pt_base_text[0] = pt_3d_end.x-0.5 ;
						pt_base_text[1] = pt_3d_start.y ;
						//mysys.WriteText(pt_base_text, strPreEndLoc, PI/2, AcDb::kTextLeft, 1.5, 0.5, clr) ; //速度endloc 黄2
						mysys.AddText(strPreEndLoc, pt_base_text, pt_base_text, AcDb::kTextLeft, AcDb::kTextBase, 1.5, 0.5, PI/2, 2) ;//速度endloc 黄2
						
					}
					
					iPreSpeed = iSpeed ;
					strPreEndLoc = strEndLoc ;
					if (iUpOrDownIndex==0)
					{
						dCurStartRealLoc+=iLen ;
					}
					else
					{
						dCurStartRealLoc-=iLen ;
					}
					
					cSetFind.MoveNext() ;
				}
				
				cSetFind.Close() ;
			}
			//end 绘制速度
			
		
			//begin 绘制信号点
			mysys.Set_Layer(_T(""), _T("signal")) ;
			double dCurX = 0 ;
			double dCurY = dDrawXhjY[iUpOrDownIndex] ;			
			
			pt_3d_start.x = iStartRealLoc*dPlusForDrawX ;
			pt_3d_end.x = iEndRealLoc*dPlusForDrawX ;
			pt_3d_start.y = pt_3d_end.y = dDrawXhjY[iUpOrDownIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			
			pt_3d_start.y = pt_3d_end.y = dCurY ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			
			//strSql.Format(_T("select * from compute_xhj_table where not (right(name,1)='链' and up_or_down=1) and not (up_or_down=1 and type in(0,2,3)) order by loc_real asc")) ;
			strSql.Format(_T("select * from import_xhj_lkdata where up_or_down=%d order by loc_real %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc")) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				int iXhIndex = 0 ;
				
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d个信号点"), iUpOrDownIndex==0?_T("下行"):_T("上行"),iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

// 				int iFindTCCNum = 0 ;
				CString strPreStation = _T("") ;
				CString strStationFront = _T(""), strStationBack = _T("") ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iXhIndex) ;


					mysys.Set_Layer(_T(""), _T("signal")) ;

					CString strName = _T("") ;
					CString strLocShow = _T("") ;
					CString strLocNum = _T("") ;
					CString strFreq = _T("") ;
					CString strGdName = _T("") ;
					//CString strYdqType = _T("") ;
					CString strXhjType = _T("") ;
					CString strJyjType = _T("") ;
					CString strGdAtt = _T("") ;
					CString strBelongStation = _T("") ;
					long iLocShow = 0 ;
					long iLocNum = 0 ;
					long iLen = 0 ;
					long iXhRealLoc = 0 ;
					int iIsDivide = 0 ;
					int iType = 0 ;
					int iUpOrDown = 0 ;
					int iIsReverse = 0 ;
					
					cSetFind.GetFieldValue(_T("xhj_name"), strName) ;
					cSetFind.GetFieldValue(_T("loc_show"), strLocShow) ;
					cSetFind.GetFieldValue(_T("loc_show_num"), iLocShow) ;
					cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
					//cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
					cSetFind.GetFieldValue(_T("loc_real"), iXhRealLoc) ;
					//cSetFind.GetFieldValue(_T("is_divide"), iIsDivide) ;
					cSetFind.GetFieldValue(_T("type"), strXhjType) ;
					cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
					cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
					cSetFind.GetFieldValue(_T("freq"), strFreq) ;
					cSetFind.GetFieldValue(_T("len"), iLen) ;
					cSetFind.GetFieldValue(_T("jyj_type"), strJyjType) ;
					cSetFind.GetFieldValue(_T("gd_attribute"), strGdAtt) ;
					cSetFind.GetFieldValue(_T("belong_station"), strBelongStation) ;

					//利用车站的变化来处理边界处
					if (strPreStation==_T(""))
					{
						strPreStation = strBelongStation ;
					}
					else if (strPreStation!=strBelongStation)
					{
						iIsDivide = 1 ;
						strStationFront = strPreStation ;
						strStationBack = strBelongStation ;
						strPreStation = strBelongStation ;
					}

					//利用标示的TCC边界来处理分界处
// 					if (strGdAtt==_T("TCC边界"))
// 					{
// 						iFindTCCNum++ ;
// 						if (iFindTCCNum==2)
// 						{
// 							iIsDivide = 1 ;
// 							strStationBack = strBelongStation ;
// 						}
// 						else if (iFindTCCNum==1)
// 						{
// 							strStationFront = strBelongStation ;
// 						}
// 					}
// 					else
// 					{
// 						iFindTCCNum = 0 ;
// 					}

					
					iLocShow = iLocShow+(iUpOrDown==0?-1:1) ;
					iLocNum = iLocNum+(iUpOrDown==0?-1:1) ;
					iXhRealLoc = iXhRealLoc+(iUpOrDown==0?-1:1) ;
					CBlkUtility::RegReplace(_T("^(.*\\D)\\d+\\+\\d+$"), _T("$1"), strLocShow, m_strMileSysName) ;
					this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;	
					
					CString strLen ;
					strLen.Format(_T("%d"), iLen) ;
					
					dCurX = iXhRealLoc*dPlusForDrawX ;
					
					ads_point pt_insert = {0,0,0};
					pt_insert[0] = dCurX ;
					pt_insert[1] = dCurY ;
					AcGePoint3d pt_3d_insert(0,0,0) ;
					
					CString strBlockName = _T("") ;
					if (strName==_T(""))//站内绝缘节
					{
						iType = 4 ;
					}
					else if (strXhjType==_T("进站信号机"))
					{
						iType = 3 ;
					}
					else if (strXhjType==_T("出站信号机"))
					{
						iType = 2 ;
					}
					else if (strXhjType==_T("通过信号机"))
					{
						iType = 0 ;
					}
					else if (strXhjType==_T("出站口"))
					{
						iType = 3 ;
						iIsReverse = 1 ;
					}
					else if (strXhjType==_T("没有信号机"))
					{
						if (strJyjType.Left(4)==_T("机械"))
						{
							iType = 4 ;
						}
						else
						{
							iType = 2 ;
						}
					}
					

					if (iType==0||iType==2||iType==3||iType==4)
					{
						strArrayAtt.RemoveAll() ;
						if (strName.Right(2)==_T("链"))
						{
							strBlockName.Format(_T("pdt_chain_%s"), iUpOrDownIndex==0?_T("x"):_T("s")) ;
							int iChainLocNum = iLocNum+(iUpOrDown==0?1:-1) ; //反向操作，还原，断链不需要1米的差
							CString strChianLocNum ;
							this->GetLocShow(iChainLocNum, m_strMileSysName, strChianLocNum) ;
							strLocShow.Insert(555,_T("=")) ;
							strLocShow.Insert(555, strChianLocNum) ;
							CString strChainLen = _T("") ;
							strChainLen.Format(_T("%s=%d"), strName, iLen) ;
							strArrayAtt.Add(strLocShow) ;
							strArrayAtt.Add(strChainLen) ;
						}
						else
						{
							if (strName.Left(1)==_T("F"))
							{
								strBlockName.Format(_T("pdt_tg_%s_f"), iUpOrDownIndex==0?_T("x"):_T("s") ) ;
							}
							else if (iType==2)
							{
								strBlockName.Format(_T("pdt_cz_%s"),iUpOrDownIndex==0?_T("x"):_T("s")) ;
// 								if (iUpOrDown==1)
// 								{
// 									strBlockName.Format(_T("pdt_cz_%s"), iUpOrDownIndex==0?_T("s"):_T("x")) ;
// 									strName =(iUpOrDownIndex==0?_T("SI"):_T("XII")) ;
// 								}
							}
							else if (iType==3)
							{
								strBlockName.Format(_T("pdt_jz_%s_z"), iUpOrDownIndex==0?_T("x"):_T("s") ) ;
								if (iIsReverse==1)
								{
									strBlockName.Format(_T("pdt_jz_%s_f"),iUpOrDownIndex==0?_T("s"):_T("x") ) ;
								}
							}
							else if (iType==0)
							{
								strBlockName.Format(_T("pdt_tg_%s"),iUpOrDownIndex==0?_T("x"):_T("s") ) ;
							}
							else if (iType==4)
							{
								strBlockName.Format(_T("pdt_jyj_0")) ;
								if (iUpOrDownIndex==0)
								{
									if (strName.Left(1)==_T("S"))
									{
										strBlockName.Format(_T("pdt_cz_s")) ;
									}
									else if (strName.Left(1)==_T("X"))
									{
										strBlockName.Format(_T("pdt_cz_x")) ;
									}
									
								}
								else if (iUpOrDownIndex==1)
								{
									if (strName.Left(1)==_T("X"))
									{
										strBlockName.Format(_T("pdt_cz_x")) ;
									}
									else if (strName.Left(1)==_T("S"))
									{
										strBlockName.Format(_T("pdt_cz_s")) ;
									}
									
								}
							}
							strArrayAtt.Add(strName) ;
							strArrayAtt.Add(strLocNum) ;
						}
						
						//mysys.Set_Layer(_T(""), _T("xhj")) ;
						mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert ) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
						{
							pRef->close() ;
							pRef = NULL ;
						}					
						
						//绘制freq len gdname
						//if ((iType==0||iIsReverse==1)&&strName.Right(2)!=_T("链"))
						if (strName.Right(2)!=_T("链"))
						{
							pt_3d_insert.x = dCurX+(iUpOrDownIndex==0?6.8:-6.8) ;
							
							strArrayAtt.RemoveAll();
							CString strLenTmp = strLen ;
							strLenTmp.Insert(555, _T("m")) ;
							strArrayAtt.Add(strLenTmp) ;
							strArrayAtt.Add(strFreq) ;						
							strArrayAtt.Add(strGdName) ;
							strBlockName.Format(_T("pdt_freq_%s"), iUpOrDownIndex==0?_T("x"):_T("s")) ;
							mysys.Set_Layer(_T(""), _T("freq")) ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
								pRef = NULL ;
							}								
						}							
						//是否是分界
						if (iIsDivide==1)
						{
							pt_3d_insert.x = dCurX ;
							pt_3d_insert.y= pt_3d_insert.y+ (iUpOrDownIndex==0?15:-15) ;
							strBlockName.Format(_T("PMT_DIVIDE_0") ) ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
							{
								if (iUpOrDownIndex!=0)
								{
									pRef->setRotation(PI) ;
								}
								pRef->close() ;
								pRef = NULL ;
							}

							pt_insert[0] = pt_3d_insert.x+(iUpOrDownIndex==0?-1.5:1.5) ;
							pt_insert[1] = pt_3d_insert.y+(iUpOrDownIndex==0?1:-3) ;
							//AcCmColor clr ;
							//clr.setColorIndex(4) ;
							//mysys.WriteText(pt_insert, strStationFront, 0, (iUpOrDownIndex==0?AcDb::kTextRight:AcDb::kTextLeft), 2, 0.7, clr) ;
							mysys.AddText(strStationFront, pt_insert, pt_insert, iUpOrDownIndex==0?AcDb::kTextRight:AcDb::kTextLeft, AcDb::kTextBase, 2, 0.7, 0, 4) ;
							pt_insert[0] = pt_3d_insert.x+(iUpOrDownIndex==0?1.5:-1.5) ;							
							//mysys.WriteText(pt_insert, strStationBack, 0, (iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight), 2, 0.7, clr) ;
							mysys.AddText(strStationBack, pt_insert, pt_insert, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, AcDb::kTextBase, 2, 0.7, 0, 4) ;
							
						}					
					}	
					
					iXhIndex++ ;
					cSetFind.MoveNext() ;
				}			
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}
			//end 绘制信号点

			//begin 绘制车站
			mysys.Set_Layer(_T(""), _T("station")) ;
			strSql.Format(_T("select * from compute_xhj_table where up_or_down=%d and type in(1,11)"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d个车站图标"), iUpOrDownIndex==0?_T("下行"):_T("上行"), iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

				int iYdqGroupIndex = 0 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iYdqGroupIndex) ;

					long iLocRealNum = 0, iLocNum = 0 ;
					CString strStationName = _T(""), strLocNum = _T("") ;
					cSetFind.GetFieldValue(_T("loc_real"), iLocRealNum) ;
					cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
					cSetFind.GetFieldValue(_T("name"), strStationName) ;
					
					CString strBigRegionSn, strSmallRegionSn, strStationSn ;
					this->GetStationInfo(strStationName,  strBigRegionSn, strSmallRegionSn, strStationSn) ;
					this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;
					ads_point pt_insert = {iLocRealNum*dPlusForDrawX, dDrawStationY[iUpOrDownIndex]+1,0} ;					
					//AcCmColor clr ;
					//clr.setColorIndex(4) ;
					CString strStationNameTmp ;
					strStationNameTmp.Format(_T("%s<编号=%s>"), strStationName, strStationSn) ;
					//mysys.WriteText(pt_insert, strStationNameTmp, 0, AcDb::kTextMid, 2, 0.7, clr) ;
					mysys.AddText(strStationNameTmp, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;

					pt_insert[1] -= 6 ;
					//mysys.WriteText(pt_insert, strLocNum, 0, AcDb::kTextMid, 2, 0.7, clr) ;
					mysys.AddText(strLocNum, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
					pt_insert[1]+=3 ;
					AcGePoint3d pt_3d_start(iLocRealNum*dPlusForDrawX-5, pt_insert[1], 0) ;
					AcGePoint3d pt_3d_end(iLocRealNum*dPlusForDrawX+5, pt_insert[1], 0) ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

					iYdqGroupIndex++ ;
					cSetFind.MoveNext() ;
				}

				cSetFind.Close() ;
			}


			//end 绘制车站

			
			
			//begin 绘制应答器
			mysys.Set_Layer(_T(""), _T("ydq")) ;
			strSql.Format(_T("select * from import_ydq_lkdata where up_or_down=%d and InStr(comment,'侧线')=0 and InStr(comment,'三四线')=0 and (right(ydq_name,2)='-1' or InStr(ydq_name, '-')=0)"), iUpOrDownIndex) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d组应答器"), iUpOrDownIndex==0?_T("下行"):_T("上行"), iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

				int iYdqGroupIndex = 0 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iYdqGroupIndex) ;


					CString strSn = _T(""), strYdqName = _T(""), strLocShow = _T(""), strUseage = _T("") ;
					long iLocRealNum = 0 ;
					cSetFind.GetFieldValue(_T("ydq_sn"), strSn) ;
					cSetFind.GetFieldValue(_T("ydq_name"), strYdqName) ;
					cSetFind.GetFieldValue(_T("loc_show"), strLocShow) ;
					cSetFind.GetFieldValue(_T("useage"), strUseage) ;
					cSetFind.GetFieldValue(_T("loc_real"), iLocRealNum) ;			

					CStringArray strArraySn ;
					CBlkUtility::DivideString(strSn, _T("-"), strArraySn) ;
					CString strGroupSn = _T(""), strGroupName = _T("") ;
					if (strArraySn.GetSize()==4)
					{
						strGroupSn = strSn ;
						strGroupName = strYdqName ;
					}
					else
					{
						strGroupSn = strSn.Left(strSn.GetLength()-2) ;
						strGroupName = strYdqName.Left(strYdqName.GetLength()-2) ;
					}

					dCurX = iLocRealNum*dPlusForDrawX ;

					AcGePoint3d pt_3d_insert(0,0,0) ;
					pt_3d_insert.y = dDrawXhjY[iUpOrDownIndex] ;
					pt_3d_insert.x = iLocRealNum*dPlusForDrawX + (iUpOrDownIndex==0?-1:1)*0.6 ;
					if (strUseage==_T("FCZ")||strUseage==_T("FJZ"))
					{
						pt_3d_insert.x +=(iUpOrDownIndex==0?1:-1)*1.1 ;
					}
					else
					{
						pt_3d_insert.x +=(iUpOrDownIndex==0?-1:1)*0.6 ;
					}

					strArrayAtt.RemoveAll() ;
					strArrayAtt.Add(strGroupName) ;
					strArrayAtt.Add(strLocShow) ;
					CString strYdqAttBlkName = _T("") ;
					strYdqAttBlkName.Format(_T("pdt_ydq_txt_%s"), iUpOrDownIndex==0?_T("x"):_T("s")) ;
					if (mysys.InsertBlock(pRef, strYdqAttBlkName, pt_3d_insert, &strArrayAtt,FALSE, 0))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					//应答器图形参数
					double dDist1 = 0.7601 ; //坐标文字块基点和第一个应答器基点距离
					double dDist2 = 1.3125 ; //应答器等腰三角形高
					double dDist3 = 2.1474 ; //圆心距三角定点距离
					double dRadius = 0.3801 ; //圆心
					double dTxtHeight = 0.6 ; //圆圈内文字
					double dTxtWidth = 0.7 ; //圆圈内文字

					CADORecordset cSetFindYdq ;
					strSql.Format(_T("select * from import_ydq_lkdata where up_or_down=%d and left(ydq_sn,%d)= '%s'"), iUpOrDownIndex, strGroupSn.GetLength(), strGroupSn) ;
					if (cSetFindYdq.IsOpen())
					{
						cSetFindYdq.Close() ;
					}
					if (cSetFindYdq.Open(g_PtrCon, strSql))
					{
						int iYdqNum = cSetFindYdq.GetRecordCount() ;
						if (!cSetFindYdq.IsBOF())
						{
							cSetFindYdq.MoveFirst() ;
						}
						int iYdqIndex = 0 ;
						while (!cSetFindYdq.IsEOF())
						{
							CString strDevType = _T("") ;
							cSetFindYdq.GetFieldValue(_T("dev_type"), strDevType) ;							
					
							pt_3d_insert.x = pt_3d_insert.x + (iUpOrDownIndex==0?1:-1)*dDist1 ;
							CString strYdqBlkName = _T("") ;
							strYdqBlkName.Format(_T("pdt_ydq_single_%d"),strDevType==_T("有源")?1:0) ;
							if (iYdqIndex==0)
							{
								AcGePoint3d pt_3d_linestart(pt_3d_insert.x, pt_3d_insert.y-dDist2,0) ;
								AcGePoint3d pt_3d_lineend(pt_3d_insert.x, pt_3d_insert.y-dDist2-0.25,0) ;
								mysys.AddAcDbLine(pt_3d_linestart, pt_3d_lineend) ;
							}

							if (mysys.InsertBlock(pRef, strYdqBlkName, pt_3d_insert))
							{
								pRef->close() ;
								pRef = NULL ;
							}

							
							CString strYdqIndex = _T("") ;
							strYdqIndex.Format(_T("%d"), iYdqIndex+1) ;
							AcGePoint3d pt_3d_cirCenter(pt_3d_insert.x,pt_3d_insert.y-dDist3,0) ;
							ads_point pt_ydqindex_insert = {pt_3d_cirCenter.x,pt_3d_cirCenter.y,0} ;
							mysys.AddAcDbCircle(pt_3d_cirCenter, dRadius) ;
							//mysys.WriteText(pt_ydqindex_insert, strYdqIndex, 0, AcDb::kTextMid, dTxtHeight, dTxtWidth) ;
							mysys.AddText(strYdqIndex, pt_ydqindex_insert, pt_ydqindex_insert, AcDb::kTextMid, AcDb::kTextBase, dTxtHeight, dTxtWidth, 0, 0) ;

							iYdqIndex++ ;

							cSetFindYdq.MoveNext() ;
						}

						cSetFindYdq.Close() ;

					}
					
					//应答器编号和用途
					CString strSnUseage = _T("") ;
					strSnUseage.Format(_T("%s-%s/%s"), strArraySn.GetAt(2), strArraySn.GetAt(3), strUseage) ;
					//AcCmColor clr ;
					//clr.setColorIndex(2) ; 
					ads_point pt_insert = {0,0,0};
					pt_insert[0] = iLocRealNum*dPlusForDrawX + (iUpOrDownIndex==0?-1:1)*0.6 ;
					pt_insert[1] = dDrawXhjY[iUpOrDownIndex]-(iUpOrDownIndex==0?5:-3) ;
					//mysys.WriteText(pt_insert, strSnUseage, 0, AcDb::kTextCenter, 1, 0.5, clr) ;
					mysys.AddText(strSnUseage, pt_insert, pt_insert, AcDb::kTextCenter, AcDb::kTextBase, 1, 0.5, 0, 2) ;//2 yellow
					
					
					iYdqGroupIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}			
			//end 绘制应答器

			
			//begin 绘制分相
			mysys.Set_Layer(_T(""), _T("dfx")) ;
			strSql.Format(_T("select * from import_dfx_info where up_or_down=%d order by id %s"), iUpOrDownIndex, iUpOrDownIndex==0?_T("asc"):_T("desc")) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iRsCount = cSetFind.GetRecordCount() ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%s:绘制%d个电分相"), iUpOrDownIndex==0?_T("下行"):_T("上行"), iRsCount) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

				int iDfxIndex = 0 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iDfxIndex) ;

					CString strDfxStartLoc = _T(""), strDfxEndLoc = _T(""), strDfxComment = _T(""), strDfxLen = _T("") ;
					long iDfxStartLoc = 0, iDfxEndLoc = 0 ;					
					double	dDfxLen = 0 ;
					cSetFind.GetFieldValue(_T("start_loc_num"), iDfxStartLoc) ;
					cSetFind.GetFieldValue(_T("end_loc_num"), iDfxEndLoc) ;
					cSetFind.GetFieldValue(_T("start_loc"), strDfxStartLoc) ;
					cSetFind.GetFieldValue(_T("end_loc"), strDfxEndLoc) ;
					cSetFind.GetFieldValue(_T("len"), dDfxLen) ;

					strDfxLen.Format(_T("%d米"), dDfxLen) ;

					long iDfxStartReal = iUpOrDownIndex==0?iDfxStartLoc:iDfxEndLoc ; //电分相起始（左边）real_loc
					//begin 找出比分相起始点小的最近的信号点loc					
					CADORecordset cSetFindNearLoc ;
					strSql.Format(_T("select * from import_xhj_lkdata where loc_num<=%d  and up_or_down=%d order by loc_real desc"), iUpOrDownIndex==0?iDfxStartLoc:iDfxEndLoc, iUpOrDownIndex) ;
					if (cSetFindNearLoc.IsOpen())
					{
						cSetFindNearLoc.Close() ;
					}
					if (cSetFindNearLoc.Open(g_PtrCon, strSql))
					{
						if (cSetFindNearLoc.GetRecordCount()>0)
						{
							cSetFindNearLoc.MoveFirst() ;

							long iNearLocNum = 0, iNearLocReal = 0 ;
							cSetFindNearLoc.GetFieldValue(_T("loc_num"), iNearLocNum) ;
							cSetFindNearLoc.GetFieldValue(_T("loc_real"), iNearLocReal) ;
							iDfxStartReal = iNearLocReal+((iUpOrDownIndex==0?iDfxStartLoc:iDfxEndLoc)-iNearLocNum) ;
						}
						
						cSetFindNearLoc.Close() ;
					}
					//end 找出比分相起始点小的最近的信号点loc
					
					double dDfxStartX = iDfxStartReal*dPlusForDrawX ;
					double dDfxEndX = (iDfxStartReal+dDfxLen)*dPlusForDrawX ;

					AcGePoint3dArray ptArray_3d ;
					AcGePoint3d pt_3d_pl(0,0,0), pt_3d_line_start(0,0,0), pt_3d_line_end(0,0,0) ;
					pt_3d_pl.x = dDfxStartX ;
					pt_3d_pl.y = dDrawXhjY[iUpOrDownIndex]+(iUpOrDownIndex==0?10:-10) ;
					ptArray_3d.append(pt_3d_pl) ;//pt1

					//起点loc
					pt_3d_line_start = pt_3d_pl ;
					pt_3d_line_start.y+=(iUpOrDownIndex==0?1:-1) ;
					pt_3d_line_end.x = pt_3d_line_start.x ;
					pt_3d_line_end.y = pt_3d_line_start.y+(iUpOrDownIndex==0?5:-5) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;//起始loc线
					pt_3d_line_start.x-=0.5 ;
					ads_point pt_base_loc_text = {pt_3d_line_start.x,pt_3d_line_start.y,0} ;					
					//mysys.WriteText(pt_base_loc_text, iUpOrDownIndex==0?strDfxStartLoc:strDfxEndLoc, 90*PI/180, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, 1, 0.5) ; //起始loc
					mysys.AddText(iUpOrDownIndex==0?strDfxStartLoc:strDfxEndLoc, pt_base_loc_text, pt_base_loc_text, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, AcDb::kTextBase, 1, 0.5, 90*PI/180, 0) ;//起始loc

					pt_3d_pl.x = dDfxEndX ;
					ptArray_3d.append(pt_3d_pl) ; //pt2

					//终点loc
					pt_3d_line_start = pt_3d_pl ;
					pt_3d_line_start.y+=(iUpOrDownIndex==0?1:-1) ;
					pt_3d_line_end.x = pt_3d_line_start.x ;
					pt_3d_line_end.y = pt_3d_line_start.y+(iUpOrDownIndex==0?5:-5) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;//终点loc线
					pt_3d_line_start.x+=1.5 ;
					pt_base_loc_text[0] = pt_3d_line_start.x ;
					pt_base_loc_text[1] = pt_3d_line_start.y ;					
					//mysys.WriteText(pt_base_loc_text, iUpOrDownIndex==0?strDfxEndLoc:strDfxStartLoc, 90*PI/180, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, 1, 0.5) ; //终始loc
					mysys.AddText(iUpOrDownIndex==0?strDfxEndLoc:strDfxStartLoc, pt_base_loc_text, pt_base_loc_text, iUpOrDownIndex==0?AcDb::kTextLeft:AcDb::kTextRight, AcDb::kTextBase, 1, 0.5, 90*PI/180, 0) ;//终始loc

					pt_3d_pl.y = dDrawXhjY[iUpOrDownIndex]+(iUpOrDownIndex==0?-5.5:5) ;
					ptArray_3d.append(pt_3d_pl) ;//pt3
					pt_3d_pl.x = dDfxStartX ;
					ptArray_3d.append(pt_3d_pl) ;//pt4


					mysys.AddAcDb2dPolyline(ptArray_3d, 0, 0) ;	
					

					
					
// 					CString str = _T("") ;
// 					CStringArray strArraySn ;
// 					CBlkUtility::DivideString(strSn, _T("-"), strArraySn) ;
// 					strSnUseage.Format(_T("%s-%s/%s"), strArraySn.GetAt(2), strArraySn.GetAt(3), strUseage) ;
// 					AcCmColor clr ;
// 					clr.setColorIndex(2) ; //yellow
// 					pt_insert[0] = pt_3d_insert.x ;
// 					pt_insert[1] = pt_3d_insert.y-(iUpOrDownIndex==0?5:-3) ;
// 					mysys.WriteText(pt_insert, strSnUseage, 0, AcDb::kTextCenter, 1, 0.5, clr) ;
					
					
					iDfxIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}
			//end 绘制分相


		}

		//插入左侧frame
		AcGePoint3d pt_3d_frame(iStartRealLoc*dPlusForDrawX,0,0) ;
		if (mysys.InsertBlock(pRef, _T("frame_pdt_left"), pt_3d_frame))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		double dFrameLineY[4] = {98,93,-93,-98} ;
		AcGePoint3d pt_3d_start(iStartRealLoc*dPlusForDrawX,0,0) ;
		AcGePoint3d pt_3d_end(iEndRealLoc*dPlusForDrawX,0,0) ;
		for (int iFrameLineIndex=0; iFrameLineIndex<4; iFrameLineIndex++)
		{
			pt_3d_start.y = pt_3d_end.y = dFrameLineY[iFrameLineIndex] ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end, iFrameLineIndex%3==0?2:1) ;//黄色 2 红色 1 
		}	
		
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt2.shx")) ;


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawPdt_LKDATA")) ;
		
	}
}

//计算进路信息表，结果回填import_compute_lsb表,增加3/4线
//一个轨道区段有3个道岔组成的时候未作处理
void CLKDataTable::ComputeJLInfo2()
{
	CUserSystem mysys ;
	CDBUtility::ClearTable(_T("xhj_gd")) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;	

		//先找出各进站信号机和第一个轨道，便于在发车进路时判断终端信号机
		strSql.Format(_T("select distinct startxhj_name,IIF(InStr(gd_list,',')>0,left(gd_list,InStr(gd_list,',')-1),gd_list) as first_gd from import_compute_lsb where InStr(to_or_from,'至')>0")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				CString strStartXhjName = _T("") ;
				CString strGd = _T("") ;
				cSet.GetFieldValue(_T("startxhj_name"), strStartXhjName) ;
				cSet.GetFieldValue(_T("first_gd"), strGd) ;

				strSql.Format(_T("insert into xhj_gd (xhj_name, gd_name) values('%s','%s')"), strStartXhjName, strGd) ;
				ExecSqlString(strSql) ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

		//begin 遍历所有进路，计算补充每条进路的信息
		strSql.Format(_T("select * from import_compute_lsb")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iJlCount = cSet.GetRecordCount() ;

			CString strBarMsg ;
			strBarMsg.Format(_T("处理%d条进路"), iJlCount) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iJlCount-1) ;	

			int iJlIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iJlIndex) ;

				CString strToOrFrom = _T("") ;
				CString strButton = _T("") ;
				CString strStartXhj = _T("") ;
				CString strDcList = _T("") ;
				CString strGdList = _T("") ;
				cSet.GetFieldValue(_T("to_or_from"), strToOrFrom) ;
				cSet.GetFieldValue(_T("jl_button"), strButton) ;
				cSet.GetFieldValue(_T("startxhj_name"), strStartXhj) ;
				cSet.GetFieldValue(_T("dc_list"), strDcList) ;
				cSet.GetFieldValue(_T("gd_list"), strGdList) ;

				int iJlNum = 0 ;
				cSet.GetFieldValue(_T("jl_index"), iJlNum) ;
				if (iJlNum==23)
				{
					int iii = 0 ;
				}

				//至或者由的 股道名称
				CString strToOrFromGd = strToOrFrom ;
				CString strGdNum ;
				if (strToOrFrom.Find(_T("股"))>=2)
				{
					strGdNum = strToOrFrom.Mid(2, strToOrFrom.Find(_T("股"))-2) ;
				}
				else if (strToOrFrom.Find(_T("G"))>=2)
				{
					strGdNum = strToOrFrom.Mid(2, strToOrFrom.Find(_T("G"))-2) ;
				}
				
				strToOrFromGd.Format(_T("%sG"), strGdNum) ;

				//begin 进路类型  终端信号机				
				
				//道岔都是定位
				BOOL bDcIsAllDw = TRUE ;
				if (strDcList.Find(_T("("))!=-1)
				{	
					CString strDcListTmp = strDcList ;
					CString strDcListTmpRs = strDcListTmp ;
					//复式交分道岔写法如108(106)/204
					CBlkUtility::RegReplace(_T("\\d+\\(\\d+\\)"),_T("AAA"), strDcListTmp, strDcListTmpRs ) ;
					if (strDcListTmpRs.Find(_T("("))!=-1)
					{
						bDcIsAllDw = FALSE ;
					}				
				}

				CString strJlType = _T("") ; //进路类型
				CString strStartXhj_Show = _T("-") ;//始端信号机显示
				CString strEndXhj = _T("") ;

				//起始信号机信息
				int iStartXhj_UpOrDown = -1 ;
				long iStartXhj_LocNum = 0 ;
				int iStartXhj_Type = 0 ;
				int iStartXhj_IsReverse = 0 ;
				if(this->GetXhjInfo(strStartXhj, iStartXhj_UpOrDown, iStartXhj_LocNum, iStartXhj_Type, iStartXhj_IsReverse)==0)
				{
					cSet.MoveNext() ; //20110722：下辛店特殊修改
					continue ;
				}

				//股道信息
				long iToOrFromGd_Len1 = 0 ;
				long iToOrFromGd_Len2 = 0 ;
				CString strToOrFromGd_Freq1 = _T("") ;
				CString strToOrFromGd_Freq2 = _T("") ;
				int iToOrFromGd_UpOrDown = -1 ;
				if(this->GetGdInfo(strToOrFromGd,  iToOrFromGd_Len1, iToOrFromGd_Len2, strToOrFromGd_Freq1, strToOrFromGd_Freq2, iToOrFromGd_UpOrDown)==0)
				{
					cSet.MoveNext() ; //20110722：下辛店特殊修改
					continue ;
				}

				//接车
				if (strToOrFrom.Left(2)==_T("至"))
				{
					if ( iStartXhj_UpOrDown==iToOrFromGd_UpOrDown && iStartXhj_IsReverse==0 && (iStartXhj_UpOrDown+iToOrFromGd_UpOrDown)>-1 && bDcIsAllDw )
					{
						strJlType = _T("正线接车") ;//1
					}
					else if ( iStartXhj_UpOrDown==iToOrFromGd_UpOrDown && iStartXhj_IsReverse==1 && (iStartXhj_UpOrDown+iToOrFromGd_UpOrDown)>-1 && bDcIsAllDw )
					{
						strJlType = _T("反向正线接车") ;//2
					}
					else if (iToOrFromGd_UpOrDown==-1)
					{
						strJlType = _T("侧向接车-1") ;//3
						if (iStartXhj_IsReverse)
						{
							strJlType = _T("反向侧向接车-1") ;//4
						}
					}
					else if (iToOrFromGd_UpOrDown==0&&strStartXhj.Left(1)==_T("X") || iToOrFromGd_UpOrDown==1&&strStartXhj.Left(1)==_T("S"))
					{
						strJlType = _T("侧向接车-2") ;//5
						if (iStartXhj_IsReverse)
						{
							strJlType = _T("反向侧向接车-2") ;//6
						}
					}
					else if (iToOrFromGd_UpOrDown==0&&strStartXhj.Left(1)==_T("S") || iToOrFromGd_UpOrDown==1&&strStartXhj.Left(1)==_T("X"))
					{
						strJlType = _T("侧向接车-3") ;//7
						if (iStartXhj_IsReverse)
						{
							strJlType = _T("反向侧向接车-3") ;//8
						}
					}

					if (strStartXhj.Left(1)==_T("X"))
					{
						strEndXhj.Format(_T("X%s"), strGdNum) ;
					}
					else if (strStartXhj.Left(1)==_T("S"))
					{
						strEndXhj.Format(_T("S%s"), strGdNum) ;
					}
				}
				//发车
				else if (strToOrFrom.Left(2)==_T("由"))
				{
					strJlType = _T("发车") ;

					strEndXhj = strButton.Mid(strButton.ReverseFind(_T(' '))+1) ;
					strEndXhj.TrimRight(_T("LA")) ;				

				}
				//end 进路类型 终端信号机

				//begin startxhj_show
				if (strJlType.Find(_T("侧向"))!=-1)
				{
					strStartXhj_Show = _T("UUS") ;
				}
				//end startxhj_show


				CADORecordset cSetFind ;

				//begin 应答器
				//begin 起始应答器编号
				CString strYdqSn = _T("-") ;
				long iStartYdqLoc = 0 ;
				
				strSql.Format(_T("select * from export_ydq where belong_station='%s' and dev_type='有源' and belong_xhj='%s' and right(useage,2) in ('JZ','CZ') order by id asc"), this->m_strCurStation, strStartXhj) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						CString strYdqSnTmp = _T("") ;
						cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
						cSetFind.GetFieldValue(_T("loc_num"), iStartYdqLoc) ;
						//strYdqSn = strYdqSnTmp.Left(strYdqSnTmp.ReverseFind(_T('-'))) ;
						strYdqSn = strYdqSnTmp ;
					}
					cSetFind.Close() ;
				}
				//end 起始应答器编号

				//接车才处理经过的应答器
				CString strPassYdqFinal = _T("-") ;

				if (strToOrFrom.Left(2)==_T("至") && strJlType.Find(_T("正线接车"))==-1)
				{
					CString strCzName1 = _T(""), strCzName2 = _T("") ;
					if (strStartXhj.Left(1)==_T("X"))
					{
						strCzName1.Format(_T("S%s"), strGdNum) ;
						strCzName2.Format(_T("X%s"), strGdNum) ;
					}
					else if (strStartXhj.Left(1)==_T("S"))
					{
						strCzName1.Format(_T("X%s"), strGdNum) ;
						strCzName2.Format(_T("S%s"), strGdNum) ;
					}

					//进站
					long iJzLoc = 0 ;
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and belong_xhj='%s' and right(useage,2)='JZ' order by id asc"), this->m_strCurStation, strStartXhj) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							cSetFind.GetFieldValue(_T("loc_num"), iJzLoc) ;
						}

						cSetFind.Close() ;
					}
					//出站1
					long iCzLoc1 = 0 ;
					CString strCzSn1 = _T("") ;
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and belong_xhj='%s' and right(useage,2)='CZ' order by id asc"), this->m_strCurStation, strCzName1) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							CString strYdqSnTmp = _T("") ;
							cSetFind.GetFieldValue(_T("loc_num"), iCzLoc1) ;
							cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
							strCzSn1 = strYdqSnTmp.Right(5).Left(3) ;
						}
						
						cSetFind.Close() ;
					}
					//定位
					CLongArray iArrayDwLoc ;
					CStringArray strArrayDwSn ;
					
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and left(belong_xhj,%d)='%sG' and right(useage,2)='DW' order by id asc"), this->m_strCurStation, strGdNum.GetLength()+1, strGdNum) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							cSetFind.MoveFirst() ;
							while (!cSetFind.IsEOF())
							{
								long iDwLoc = 0 ;
								CString strDwSn = _T("") ;
								CString strYdqSnTmp = _T("") ;
								cSetFind.GetFieldValue(_T("loc_num"), iDwLoc) ;
								cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
								strDwSn = strYdqSnTmp.Right(3) ;

								iArrayDwLoc.Add(iDwLoc) ;
								strArrayDwSn.Add(strDwSn) ;

								cSetFind.MoveNext() ;
							}

						}						
						cSetFind.Close() ;
					}
					//出站2
					long iCzLoc2 = 0 ;
					CString strCzSn2 = _T("") ;
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and belong_xhj='%s' and right(useage,2)='CZ' order by id asc"), this->m_strCurStation, strCzName2) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							CString strYdqSnTmp = _T("") ;
							cSetFind.GetFieldValue(_T("loc_num"), iCzLoc2) ;
							cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
							strCzSn2 = strYdqSnTmp.Right(5).Left(3) ;
						}						
						cSetFind.Close() ;
					}

					strPassYdqFinal.Format(_T("%s/%d"), strCzSn1, abs(iCzLoc1-iJzLoc)) ;
					
					int iDwSum = iArrayDwLoc.GetSize() ;
					CString strPassYdqDw = _T("") ;
					long iPreLoc = iCzLoc1 ;
					for (int iDwIndex=0; iDwIndex<iDwSum; iDwIndex++)
					{
						CString strPassYdqDwTmp = _T("") ;
						strPassYdqDwTmp.Format(_T(",\n%s/%d"), strArrayDwSn.GetAt(iDwIndex), abs(int(iArrayDwLoc.GetAt(iDwIndex)-iPreLoc))) ;
						strPassYdqDw.Insert(555, strPassYdqDwTmp) ;
						iPreLoc = iArrayDwLoc.GetAt(iDwIndex) ;
					}

					strPassYdqFinal.Insert(555, strPassYdqDw) ;
					if (iDwSum>0)
					{
						CString strPassYdqCz2 = _T("") ;
						strPassYdqCz2.Format(_T(",\n%s/%d"), strCzSn2, abs(int(iCzLoc2-iArrayDwLoc.GetAt(iDwSum-1)))) ;
						strPassYdqFinal.Insert(555, strPassYdqCz2) ;
					}

					//strPassYdqFinal.Format(_T("%s/%d,\n%s/%d,\n%s/%d"), strCzSn1, abs(iCzLoc1-iJzLoc), strDwSn, abs(iDwLoc-iCzLoc1), strCzSn2, abs(iCzLoc2-iDwLoc)) ;
				}
				//end 应答器


				//begin dc_list_new
				CString	 strDcListNew = _T("") ;
				CStringArray strArrayDcList ;
				CBlkUtility::DivideString(strDcList, _T(","), strArrayDcList) ; //20110722：以前是","分隔，下辛店特殊修改
				for(int iDcIndex=0; iDcIndex<strArrayDcList.GetSize(); iDcIndex++)
				{
					CString strDc = strArrayDcList.GetAt(iDcIndex) ;
					strDc.TrimLeft() ;
					strDc.TrimRight() ;					
					if (strDc.Find(_T("{"))==-1&&strDc.Find(_T("}"))==-1&&strDc.Find(_T("["))==-1&&strDc.Find(_T("]"))==-1&&strDc!=_T(""))
					{
						if (iDcIndex==0)
						{
							strDcListNew = strDc ;
						}
						else
						{
							strDcListNew.Insert(555,_T(",")) ;
							strDcListNew.Insert(555, strDc) ;
						}
					}
				}				
				//end dc_list_new

				//begin gd_list
				CString strGdListNew = _T("") ;
				CStringArray strArrayGdList ;
				CBlkUtility::DivideString(strGdList, _T(","), strArrayGdList) ; //20110722：以前是","分隔，下辛店特殊修改
				for(int iStrIndex=0; iStrIndex<strArrayGdList.GetSize(); iStrIndex++)
				{
					CString strGd = strArrayGdList.GetAt(iStrIndex) ;
					strGd.TrimLeft() ;
					strGd.TrimRight() ;					
					if (strGd.Find(_T("<"))==-1&&strGd!=_T(""))
					{
						if (iStrIndex==0)
						{
							strGdListNew = strGd ;
						}
						else
						{
							strGdListNew.Insert(555,_T(",")) ;
							strGdListNew.Insert(555, strGd) ;
						}
					}
				}
				//重新将去掉 <> 空白字符 后的轨道区段list分隔到数组内 
				strArrayGdList.RemoveAll() ;
				CBlkUtility::DivideString(strGdListNew, _T(","), strArrayGdList) ;

				long iLenSum = 0 ; //所有区段累计长度
				CString strGdListFinal = _T("") ; //轨道区段最终输出
				int iGdIndex = 0 ;
				for (iGdIndex=0; iGdIndex<strArrayGdList.GetSize(); iGdIndex++)
				{
					CString strCurGdSection = _T("") ;

					CString strGd = strArrayGdList.GetAt(iGdIndex) ;
					long iGdLen = 0 ;
					long iLen1 = 0, iLen2 = 0 ;
					CString strFreqCase1 = _T("0") ;
					CString strFreqCase2 = _T("0") ;
					CString strXhjCase = _T("没有信号机") ;

					int iDcCase = 0 ;
					int iRsCount = 0 ;//this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreqCase) ;

					
					int iFindMark = strGd.Find(_T("-")) ;
					
					CString strReverseStr = _T("") ; //反位道岔字符串,查询import_gdsection_info时用
					if (strGd.Right(2)==_T("DG"))
					{
						if (iFindMark==-1)//112DG形式
						{
							iRsCount = this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreqCase1) ;
						}
						else //111-113DG形式
						{
							long iDcLoc1 = 0, iDcLoc2 = 0 ;
							CString strDcName1, strDcName2 ;
							iRsCount = this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreqCase1, iDcLoc1, iDcLoc2, strDcName1, strDcName2) ;
						} //else //111-113DG形式
					} //if (strGd.Right(2)==_T("DG"))
					else
					{
						int iUpOrDown = 0 ;						
						iRsCount = this->GetGdInfo(strGd, iLen1, iLen2, strFreqCase1, strFreqCase2, iUpOrDown) ;
						iGdLen = iLen1+iLen2 ;
					}

					iLenSum +=iGdLen ;		

					if (iGdIndex==strArrayGdList.GetSize()-1)
					{
						if (strJlType==_T("发车"))
						{
							strXhjCase = _T("出站口") ;
						}						 
					}

					//分割成G1 G2的情况
					if (iRsCount>1)
					{
						if (strStartXhj.Left(1)==_T("X"))//从左往右先遇到G1，反之先遇到G2
						{
							for (int i=0; i<2; i++)
							{
								CString strGdTmp ;
								strGdTmp.Format(_T("%s%d"), strGd, i+1) ;		
								if (i==1)
								{
									strXhjCase = _T("出站信号机") ;
								}
								CString strFreqCase =(i==0?strFreqCase1:strFreqCase2) ;
								long iGdLenTmp = (i==0?iLen1:iLen2) ;
								
								if (strGdListFinal==_T(""))
								{
									strGdListFinal.Format(_T("%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
								}
								else
								{
									CString strTmp = _T("") ;
									strTmp.Format(_T(", \n%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
									strGdListFinal.Insert(10000, strTmp) ;
								}							
							}
						}
						else
						{
							for (int i=0; i<2; i++)
							{
								CString strGdTmp ;
								strGdTmp.Format(_T("%s%d"), strGd, 2-i) ;		
								if (i==1)
								{
									strXhjCase = _T("出站信号机") ;
								}
								CString strFreqCase =(i==0?strFreqCase2:strFreqCase1) ;
								long iGdLenTmp = (i==0?iLen2:iLen1) ;
								
								if (strGdListFinal==_T(""))
								{
									strGdListFinal.Format(_T("%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
								}
								else
								{
									CString strTmp = _T("") ;
									strTmp.Format(_T(", \n%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
									strGdListFinal.Insert(10000, strTmp) ;
								}							
							}
						}

					}
					else
					{
						//if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*G$"), strGd))
						if (CBlkUtility::IsStrMatch(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G$"), strGd))
						{
							strXhjCase = _T("出站信号机") ;
						}						 
						if (strGdListFinal==_T(""))
						{
							strGdListFinal.Format(_T("%d\\%s\\%s\\%s"), iGdLen, strFreqCase1, strXhjCase, strGd) ;
						}
						else
						{
							CString strTmp = _T("") ;
							strTmp.Format(_T(", \n%d\\%s\\%s\\%s"), iGdLen, strFreqCase1, strXhjCase, strGd) ;
							strGdListFinal.Insert(10000, strTmp) ;
						}
					}

				} //for (int iGdIndex=0; iGdIndex<strArrayGdList.GetSize(); iGdIndex++)

// 				//如果有轨道频率是0，则其它所有频率全部置0
// 				if (CBlkUtility::IsStrMatch(_T("\\d+\\\\0\\\\.+\\\\.+"), strGdListFinal))
// 				{
// 					CString strGdListFinalTmp = _T("") ;
// 					CString strReg = _T("(\\d+)\\\\(\\d{4}-[1-2]|0)\\\\(.+\\\\.+)") ;
// 					CBlkUtility::RegReplace(strReg, _T("$1\\\\0\\\\$3"), strGdListFinal, strGdListFinalTmp) ;
// 					strGdListFinal = strGdListFinalTmp ;
// 				}


				//begin 为出北京南改动
// 				int iFindLoc = strGdListFinal.Find(_T("\\0\\")) ;
// 				if (iFindLoc!=-1)
// 				{
// 					CString strGdListFinalTmp = _T("") ;
// 					CString strReg = _T("\\d{4}-[1-2]") ;
// 					CBlkUtility::RegReplace(strReg, _T("0"), strGdListFinal, strGdListFinalTmp) ;
// 					strGdListFinal = strGdListFinalTmp ;
// 				}
				if (strJlType.Find(_T("侧向接车"))!=-1)
				{
					CString strGdListFinalTmp = _T("") ;
					CString strReg = _T("(\\d+)\\\\\\d{4}-[1-2]\\\\(.+\\\\.*[^IVX\\d]G)") ;
					CString strSub = _T("$1\\\\0\\\\$2") ;
					CBlkUtility::RegReplace(strReg, strSub, strGdListFinal, strGdListFinalTmp) ;
					strGdListFinal = strGdListFinalTmp ;
				}
				//end 为出北京南改动

				//end gd_list

				//begin speed

				CString strSpeedFinal = _T("") ; //速度描述字符串
// 				if (iJlNum == 41)
// 				{
// 					int aa = 0 ;
// 				}

                int iOldInXOrS = (iStartXhj_UpOrDown==2||iStartXhj_UpOrDown==3)?-1:iStartXhj_UpOrDown ;
                long iCurSpeedStartLoc = iStartXhj_LocNum ;//当前速度的起始里程
                long iCurLoc = iStartXhj_LocNum ; //暂时已虹桥为例 X信号机 里程
                long iLenSumCurSpeed = 0 ; //当前段速度累计长度
                int iPlus = 1 ; //进路从左往右是里程增加iplus为1，反向为减iplus为-1
                //long iLenSumAllSpeed = 0 ;//所有速度总的累计长度
				CString strExpectDc = _T("") ; //前一个轨道跟本轨道哪个道岔链接（双动）
				
				if (strStartXhj.Left(1)==_T("X"))
				{
					iPlus = 1 ;
				}
				else if (strStartXhj.Left(1)==_T("S"))
				{
					iPlus = -1 ;
				}			
				
// 				if (iJlNum == 132)
// 				{
// 					int aaa = 0 ;
// 				}

                int iGdSum = strArrayGdList.GetSize() ;
                for (iGdIndex=0; iGdIndex<iGdSum; iGdIndex++)
                {
                    CString strGd = strArrayGdList.GetAt(iGdIndex) ;
                    long iGdLen = 0 ;
                    
                    //////////////////////////////////////////////////////////////////////////
                    if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*DG$"), strGd))//111DG形式
                    {
                        CString strDcName = strGd.Left(strGd.GetLength()-2) ;
                        
                        long iDcLoc = 0 ;//道岔的公里标                         
                        int iCurInXOrS = this->GetDcInfo(strDcName, iDcLoc) ;
                        int iDcCase = 0 ;
                        CString strFreq = _T("") ;
                        this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreq) ;
                        
                        if ((iOldInXOrS==0&&iCurInXOrS==0)||(iOldInXOrS==10&&iCurInXOrS==10)) //x->x or xp->xp
                        {
                            if (iDcCase==0) //道岔在定位
                            {
                                iLenSumCurSpeed+=iGdLen ;
                            }
                            else if (iDcCase==1)//反位
                            {
                                int iLenStraight = (iDcLoc-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
                                iLenSumCurSpeed+=iLenStraight ;
                                //处理前期积累的速度长度,一个阶段结束
                                //////////////////////////////////////////////////////////////////////////
								this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
                                //////////////////////////////////////////////////////////////////////////
                                
                                int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
                                iCurSpeedStartLoc = iDcLoc ;
                                iLenSumCurSpeed = iLenCurve ;
								this->GetOtherDc(strDcName, strExpectDc) ;
                            }
                            
                        }
						//x->s or xp->sp or x->xp or xp->x or sp->s or s->sp
                        else if ((iOldInXOrS==0&&iCurInXOrS==1)||(iOldInXOrS==10&&iCurInXOrS==11)||(iOldInXOrS==0&&iCurInXOrS==10)||(iOldInXOrS==10&&iCurInXOrS==0)||(iOldInXOrS==11&&iCurInXOrS==1)||(iOldInXOrS==1&&iCurInXOrS==11)) 
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        else if ((iOldInXOrS==0&&iCurInXOrS==-1)||(iOldInXOrS==10&&iCurInXOrS==-1))//x->c or xp->c
                        {
                            iLenSumCurSpeed+=iGdLen ;
                        }
                        else if ((iOldInXOrS==1&&iCurInXOrS==1)||(iOldInXOrS==11&&iCurInXOrS==11))//s->s or sp->sp
                        {
                            if (iDcCase==0) //道岔在定位
                            {
                                iLenSumCurSpeed+=iGdLen ;
                            }
                            else if (iDcCase==1)//反位
                            {
                                int iLenStraight = (iDcLoc-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
                                iLenSumCurSpeed+=iLenStraight ;
                                //处理前期积累的速度长度,一个阶段结束
                                //////////////////////////////////////////////////////////////////////////
								this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
                                //////////////////////////////////////////////////////////////////////////
                                
                                int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
                                iCurSpeedStartLoc = iDcLoc ;
                                iLenSumCurSpeed = iLenCurve ;

								this->GetOtherDc(strDcName, strExpectDc) ;
                            }
                        }   
                        else if ((iOldInXOrS==1&&iCurInXOrS==0)||(iOldInXOrS==11&&iCurInXOrS==10))//s->x or sp->xp
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
                            //////////////////////////////////////////////////////////////////////////
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        else if ((iOldInXOrS==1&&iCurInXOrS==-1)||(iOldInXOrS==11&&iCurInXOrS==-1))//s->c or sp->c
                        {
                            iLenSumCurSpeed+=iGdLen ;
                        }
                        else if (iOldInXOrS==-1&&iCurInXOrS==-1)//c->c
                        {
                            iLenSumCurSpeed+=iGdLen ;
							if (iDcCase==1)//反位
							{
								this->GetOtherDc(strDcName, strExpectDc) ;
							}
                        }   
                        else if ((iOldInXOrS==-1&&iCurInXOrS==0)||(iOldInXOrS==-1&&iCurInXOrS==10))//c->x or c->xp
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
                            //////////////////////////////////////////////////////////////////////////
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        else if ((iOldInXOrS==-1&&iCurInXOrS==1)||(iOldInXOrS==-1&&iCurInXOrS==11))//c->s c->sp
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        
                        iOldInXOrS = iCurInXOrS ; 
                     } 
					 else if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*-[1-9]\\d*DG$"), strGd))//111-113DG形式
					 {
						 CString strDcName1, strDcName2 ;
						 CBlkUtility::RegReplace(_T("^([1-9]\\d*)-([1-9]\\d*)DG$"), _T("$1"), strGd, strDcName1) ;
						 CBlkUtility::RegReplace(_T("^([1-9]\\d*)-([1-9]\\d*)DG$"), _T("$2"), strGd, strDcName2) ;
						 
						 long iDcLoc1 = 0, iDcLoc2 =0 ;//道岔的公里标                            
						 int iCurInXOrS = this->GetDcInfo(strDcName1, iDcLoc1) ;
						 this->GetDcInfo(strDcName2, iDcLoc2) ;          
						 
						 int iDcCase = 0 ;
						 CString strFreq = _T("") ;
						 //this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreq) ;
						 this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreq, iDcLoc1, iDcLoc2, strDcName1, strDcName2, iPlus==1?0:1) ;
						 
						 if ((iOldInXOrS==0&&iCurInXOrS==0)||(iOldInXOrS==10&&iCurInXOrS==10)) //x->x or xp->xp
						 {
							 if (iDcCase==10) //道岔全在定位
							 {
								 iLenSumCurSpeed+=iGdLen ;
							 }
							 else if (iDcCase==12)//1反位2定位
							 {
								 int iLenStraight = (iDcLoc1-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc1 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName1, strExpectDc) ;
							 }
							 else if (iDcCase==11)//1定位2反位
							 {
								 int iLenStraight = (iDcLoc2-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc2 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
							 
						 }
						 //x->s or xp->sp or x->xp or xp->x or sp->s or s->sp
						 else if ((iOldInXOrS==0&&iCurInXOrS==1)||(iOldInXOrS==10&&iCurInXOrS==11)||(iOldInXOrS==0&&iCurInXOrS==10)||(iOldInXOrS==10&&iCurInXOrS==0)||(iOldInXOrS==11&&iCurInXOrS==1)||(iOldInXOrS==1&&iCurInXOrS==11)) 
						 {
							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ;  
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								}
							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;

								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                         

							 }

						 }
						 else if ((iOldInXOrS==0&&iCurInXOrS==-1)||(iOldInXOrS==10&&iCurInXOrS==-1))//x->c or xp->c
						 {
							 iLenSumCurSpeed+=iGdLen ;
						 }
						 else if ((iOldInXOrS==1&&iCurInXOrS==1)||(iOldInXOrS==11&&iCurInXOrS==11))//s->s or sp->sp
						 {
							 if (iDcCase==10) //道岔全在定位
							 {
								 iLenSumCurSpeed+=iGdLen ;
							 }
							 else if (iDcCase==12)//1反位2定位
							 {
								 int iLenStraight = (iDcLoc1-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc1 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName1, strExpectDc) ;
							 }
							 else if (iDcCase==11)//1定位2反位
							 {
								 int iLenStraight = (iDcLoc2-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc2 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
							 
						 }   
						 else if ((iOldInXOrS==1&&iCurInXOrS==0)||(iOldInXOrS==11&&iCurInXOrS==10))//s->x or sp->xp
						 {
							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ; 
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								}

							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;
								 
								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                      
							 }
						 }
						 else if ((iOldInXOrS==1&&iCurInXOrS==-1)||(iOldInXOrS==11&&iCurInXOrS==-1))//s->c or sp->c
						 {
							 iLenSumCurSpeed+=iGdLen ;                               
						 }
						 else if (iOldInXOrS==-1&&iCurInXOrS==-1)//c->c
						 {
							 iLenSumCurSpeed+=iGdLen ; 
							 if (iDcCase==12)//1反位2定位
							 {
								 this->GetOtherDc(strDcName1, strExpectDc) ;
							 }
							 else if (iDcCase==11)//1定位2反位
							 {
								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
							 else if (iDcCase==13)//1、2均反位
							 {
								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
						 }   
						 else if ((iOldInXOrS==-1&&iCurInXOrS==0)||(iOldInXOrS==-1&&iCurInXOrS==10))//c->x or c->xp
						 {
							 //如果没有提供侧线道岔坐标，则给个默认的前后道岔值							
							 if (strExpectDc!=strDcName1&&strExpectDc!=strDcName2)
							 {								 
								 acutPrintf(_T("第%d条进路c->x时strExprctDc为空，注意检查!\n"), iJlNum) ;
								 
// 								 if (strToOrFrom.Left(2)==_T("至"))
// 								 {
// 									strExpectDc = strDcName1 ;		
// 								 }
// 								 else if (strToOrFrom.Left(2)==_T("由"))
// 								 {
// 									 strExpectDc = strDcName2 ;
// 								 }

								strExpectDc = strDcName1 ;

							 }
							 //////////////////////////////////////////////////////////////////////////

							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {	
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ;  
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								 }
							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;
								 
								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                         
								 
							 }

						 }
						 else if ((iOldInXOrS==-1&&iCurInXOrS==1)||(iOldInXOrS==-1&&iCurInXOrS==11))//c->s or c->sp
						 {
							 //如果没有提供侧线道岔坐标，则给个默认的前后道岔值
							 if (strExpectDc!=strDcName1&&strExpectDc!=strDcName2)
							 {
								 acutPrintf(_T("第%d条进路c->s时strExprctDc为空，注意检查!\n"), iJlNum) ;
// 								 if (strToOrFrom.Left(2)==_T("至"))
// 								 {
// 									 strExpectDc = strDcName1 ;		
// 								 }
// 								 else if (strToOrFrom.Left(2)==_T("由"))
// 								 {
// 									 strExpectDc = strDcName2 ;
// 								 }
								 strExpectDc = strDcName1 ;
							 }
							 //////////////////////////////////////////////////////////////////////////

							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {	
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ;  
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								 }
							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;
								 
								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                       
							 }
						 }
						 
						 iOldInXOrS = iCurInXOrS ; 
                     }
					 else if (CBlkUtility::IsStrMatch(_T("^(M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))G[1-2]?$|^[1-9]\\d*G[1-2]?$"), strGd))//3G IIG IG1形式
					 {
						 CString strFreq1 = _T("") ;
						 CString strFreq2 = _T("") ;
						 int iUpOrDown = -1 ;                
						 long iLen1 = 0, iLen2 = 0 ;
						 this->GetGdInfo(strGd,  iLen1, iLen2, strFreq1, strFreq2, iUpOrDown) ;

						 iGdLen = iLen1+iLen2 ;
						 
						 iLenSumCurSpeed+=iGdLen ;
						 
						 iOldInXOrS = iUpOrDown ; 
					 }
					 else
					 {
						 CString strFreq1 = _T("") ;
						 CString strFreq2 = _T("") ;
						 int iUpOrDown = -1 ;                    
						 long iLen1 = 0, iLen2 = 0 ;
						 this->GetGdInfo(strGd, iLen1, iLen2, strFreq1, strFreq2, iUpOrDown) ;
						 iGdLen = iLen1+iLen2 ;
						 
						 iLenSumCurSpeed+=iGdLen ;
						 
						 iOldInXOrS = iUpOrDown ;
					 }
					 
					 iCurLoc+=(iGdLen*iPlus) ;

					 if (iGdIndex==iGdSum-1)//最后一个轨道
					 {
						 //处理前期积累的速度长度,一个阶段结束
						 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, iOldInXOrS, iPlus) ;
					 }
				}

				//begin 合并速度

				//end 合并速度

				//end speed

				cSet.Edit() ;
				cSet.SetFieldValue(_T("jl_type"), strJlType) ;
				cSet.SetFieldValue(_T("startxhj_show_new"), strStartXhj_Show) ;
				cSet.SetFieldValue(_T("endxhj_name"), strEndXhj) ;
				cSet.SetFieldValue(_T("dc_list_new"), strDcListNew) ;
				cSet.SetFieldValue(_T("gd_section"), strGdListFinal) ;
				cSet.SetFieldValue(_T("gd_len_sum"), iLenSum) ;
				cSet.SetFieldValue(_T("line_speed"), strSpeedFinal) ;
				cSet.SetFieldValue(_T("ydq_sn"), strYdqSn) ;
				cSet.SetFieldValue(_T("pass_ydq"), strPassYdqFinal) ;
				

				cSet.Update() ;

				cSet.MoveNext() ;
				iJlIndex++ ;
			}


			acedRestoreStatusBar() ;

			cSet.Close() ;
			
		}
		//end 遍历所有进路，计算补充每条进路的信息

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeJLInfo2")) ;
		
	}

}
// 不按比例，按相对位置绘制，由列控数据表调入数据
int CLKDataTable::DrawPdt_LKDATA_N(void)
{

	AcDbDatabase *pCurDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	//CString strMileSys = _T("DK") ;
	double dPlusForDrawX = 0.012 ; //0.035 ;// 3.5/100 每100米的对应于cad图上3.5个单位长
	double dMileSysFirstLineY[2] ={60-22, -60+19+3} ;//0 ; //刻度尺的第一条横线Y坐标 
	double dMileSysSecondLineY[2] = {55-22, -55+19+3} ;// -5 ; //刻度尺的第二条横线Y坐标
	double dPdFirstHLineY[2] = {52.5-3+8, -42.5+3-8} ; //-7.5 ; //坡度第一条横线Y坐标
	double dPdSecondHLineY[2] = {42.5-3+8, -52.5+3-8} ;//-17.5 ; //坡度第二条横线Y坐标
	double dPlusForDrawSpeedY = 0.1 ;//0.15 ;// 6/40 每40公里的速度对应于cad图上6个单位长
	double dDrawSpeedStartY[2] = {45, -120+3} ;//-6 ; //绘制速度的起始Y坐标，即速度为0时对应的Y坐标
	double dDrawXhjY[2] = {2.7,-2.7} ; //-50 ;//绘制信号机基点Y坐标
	double dDrawStationY[2] = {24.05, -28.25+3+4.2} ;//62 ; //绘制车站基点的Y坐标：速度最大假如450，则450*0.15-6=61.5，车站绘制在速度上方
	double dBetweenUpOrDown = 135 ; //上下行之间距离

	double dBetweenXhj = 10 ; //正常情况前后架信号机距离
	double dBetweenXhjPlus = 2 ; //上下行信号机错开距离

	try
	{
		CString strSql ;
		CADORecordset cSetFind ;

		AcDbBlockReference *pRef = NULL ;
		CStringArray strArrayAtt ;


		//begin 绘制信号点
		mysys.Set_Layer(_T(""), _T("signal")) ;		

		//strSql.Format(_T("select * from compute_xhj_lkdata where type not in(2) order by loc_real asc")) ;
		strSql.Format(_T("select * from import_xhj_lkdata order by loc_real asc")) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			if (!cSetFind.IsBOF())
			{
				cSetFind.MoveFirst() ;
			}
			int iRsCount = cSetFind.GetRecordCount() ;
			int iXhIndex = 0 ;
			double dCurDownX = 0, dCurUpX = 0 ; //当前下/上行绘制的x坐标
			double dPreX = 0 ; //上一个绘制的x坐标
			long iPreRealLoc = 0 ; //上一个绘制的信号点的realloc,用来和当前要绘制的比较，如果相等，就用dPreX当做当前的x

			CString strBarMsg ;
			strBarMsg.Format(_T("绘制%d个信号点"), iRsCount) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	


			while (!cSetFind.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iXhIndex) ;
				

				int iId = 0 ;
				CString strName = _T("") ;
				CString strLocShow = _T("") ;
				CString strLocNum = _T("") ;
				CString strFreq = _T("") ;
				CString strGdName = _T("") ;
				//CString strYdqType = _T("") ;
				//CString strXhjType = _T("") ;
				//CString strJyjType = _T("") ;
				CString strGdAtt = _T("") ;
				CString strBelongStation = _T("") ;

				long iLocShow = 0 ;
				long iLocNum = 0 ;
				long iLen = 0 ;
				long iXhRealLoc = 0 ;
				int iIsDivide = 0 ;
				int iType = 0 ;
				int iUpOrDown = 0 ;
				int iIsReverse = 0 ;
				cSetFind.GetFieldValue(_T("id"), iId) ;
				cSetFind.GetFieldValue(_T("xhj_name"), strName) ;
				cSetFind.GetFieldValue(_T("loc_show"), strLocShow) ;
				cSetFind.GetFieldValue(_T("loc_show_num"), iLocShow) ;
				cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
				cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
				cSetFind.GetFieldValue(_T("loc_real"), iXhRealLoc) ;
				cSetFind.GetFieldValue(_T("is_divide"), iIsDivide) ;
				cSetFind.GetFieldValue(_T("type_num"), iType) ;
				cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
				cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;

				cSetFind.GetFieldValue(_T("freq"), strFreq) ;
				cSetFind.GetFieldValue(_T("len"), iLen) ;
				//cSetFind.GetFieldValue(_T("jyj_type"), strJyjType) ;
				cSetFind.GetFieldValue(_T("gd_attribute"), strGdAtt) ;
				cSetFind.GetFieldValue(_T("belong_station"), strBelongStation) ;

				this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;

				ads_point pt_insert = {0,0,0};
				AcGePoint3d pt_3d_insert(0,0,0) ;

				CString strBlockName = _T("") ;

				if (iType==1||iType==11)
				{
					mysys.Set_Layer(_T(""), _T("station")) ;
					pt_insert[0] = dPreX+dBetweenXhj/2 ;
					pt_insert[1] = dDrawStationY[iUpOrDown]+1 ;
					//AcCmColor clr ;
					//clr.setColorIndex(4) ;
					//mysys.WriteText(pt_insert, strName, 0, AcDb::kTextMid, 2, 0.7, clr) ;
					mysys.AddText(strName, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
					pt_insert[1] -= 6 ;
					//mysys.WriteText(pt_insert, strLocNum, 0, AcDb::kTextMid, 2, 0.7, clr) ;
					mysys.AddText(strLocNum, pt_insert, pt_insert, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 4) ;
					pt_insert[1]+=3 ;
					AcGePoint3d pt_3d_start(dPreX, pt_insert[1], 0) ;
					AcGePoint3d pt_3d_end(dPreX+dBetweenXhj, pt_insert[1], 0) ;
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
				else if (iType==0||iType==2||iType==3||iType==4)
				{

					//////////////////////////////////////////////////////////////////////////
					strArrayAtt.RemoveAll() ;
					if (strName.Right(2)==_T("链"))
					{
						strBlockName.Format(_T("pdt_chain_%s"), iUpOrDown==0?_T("x"):_T("s")) ;
						strLocShow.Insert(555,_T("=")) ;
						strLocShow.Insert(555, strLocNum) ;
						CString strChainLen = _T("") ;
						strChainLen.Format(_T("%s=%d"), strName, iLen) ;
						strArrayAtt.Add(strLocShow) ;
						strArrayAtt.Add(strChainLen) ;
						pt_insert[0] = dPreX+dBetweenXhj/2 ;
					}
					else
					{
						if (strName.Left(1)==_T("F"))
						{
							strBlockName.Format(_T("pdt_tg_%s_f"), iUpOrDown==0?_T("x"):_T("s") ) ;
						}
						else if (iType==2)
						{
							strBlockName.Format(_T("pdt_cz_%s"),iUpOrDown==0?_T("x"):_T("s")) ;

						}
						else if (iType==3)
						{
							strBlockName.Format(_T("pdt_jz_%s_z"), iUpOrDown==0?_T("x"):_T("s") ) ;
							if (iIsReverse==1)
							{
								strBlockName.Format(_T("pdt_jz_%s_f"),iUpOrDown==0?_T("s"):_T("x") ) ;
							}
						}
						else if (iType==0)
						{
							strBlockName.Format(_T("pdt_tg_%s"),iUpOrDown==0?_T("x"):_T("s") ) ;
						}
						else if (iType==4)
						{
							strBlockName.Format(_T("pdt_jyj_0")) ;
							if (iUpOrDown==0)
							{
								if (strName.Left(1)==_T("S"))
								{
									strBlockName.Format(_T("pdt_cz_s_0")) ;
								}
								else if (strName.Left(1)==_T("X"))
								{
									strBlockName.Format(_T("pdt_cz_x_0")) ;
								}

							}
							else if (iUpOrDown==1)
							{
								if (strName.Left(1)==_T("X"))
								{
									strBlockName.Format(_T("pdt_cz_x_1")) ;
								}
								else if (strName.Left(1)==_T("S"))
								{
									strBlockName.Format(_T("pdt_cz_s_1")) ;
								}

							}
						}
						strArrayAtt.Add(strName) ;
						strArrayAtt.Add(strLocNum) ;



						//begin 下一个信号点的realloc
						long iXhRealLocNext = 0 ;
						cSetFind.MoveNext() ;
						if (!cSetFind.IsEOF())
						{
							cSetFind.GetFieldValue(_T("loc_real"), iXhRealLocNext) ;
						}
						cSetFind.MovePrevious() ;
						//end 下一个信号点的realloc

						if (iUpOrDown==0) //下行
						{
							if (iXhRealLoc==iPreRealLoc) //该loc和上次的一样，即并置
							{
								dCurDownX = dPreX ;
							}
							else
							{
								dCurDownX+=dBetweenXhj ;
								if (dCurDownX<=dPreX) //
								{
									dCurDownX=dPreX+dBetweenXhjPlus ;
								}								
							}
							if (iXhRealLoc==iXhRealLocNext) //本信号点和下一个并置时
							{
								dCurDownX=dPreX+dBetweenXhj ;
							}
							dPreX = dCurDownX ;
							pt_insert[0] = dCurDownX ;							
						}
						else //上行
						{
							if (iXhRealLoc==iPreRealLoc) //该loc和上次的一样，即并置
							{
								dCurUpX = dPreX ;
							}
							else
							{
								dCurUpX+=dBetweenXhj ;
								if (dCurUpX<=dPreX) //
								{
									dCurUpX = dPreX+dBetweenXhjPlus ;
								}								
							}
							if (iXhRealLoc==iXhRealLocNext) //本信号点和下一个并置时
							{
								dCurUpX=dPreX+dBetweenXhj ;
							}
							dPreX = dCurUpX ;
							pt_insert[0] = dCurUpX ;						
						}

					}

					CString strLen ;
					strLen.Format(_T("%d"), iLen) ;

					//mysys.Set_Layer(_T(""), _T("xhj")) ;	
					pt_insert[1] = dDrawXhjY[iUpOrDown] ;
					mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert ) ;
					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
					{
						pRef->close() ;
						pRef = NULL ;
					}					

					//绘制freq len gdname
					//if ((iType==0||iIsReverse==1)&&strName.Right(2)!=_T("链"))
					if (strName.Right(2)!=_T("链"))
					{
						pt_3d_insert.x = pt_insert[0]+(iUpOrDown==0?6.8:-6.8) ;

						strArrayAtt.RemoveAll();
						CString strLenTmp = strLen ;
						strLenTmp.Insert(555, _T("m")) ;
						strArrayAtt.Add(strLenTmp) ;
						strArrayAtt.Add(strFreq) ;						
						strArrayAtt.Add(strGdName) ;
						strBlockName.Format(_T("pdt_freq_%s"), iUpOrDown==0?_T("x"):_T("s")) ;
						mysys.Set_Layer(_T(""), _T("freq")) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
						{
							pRef->close() ;
							pRef = NULL ;
						}								
					}							
					//是否是分界
					if (iIsDivide==1)
					{
						pt_3d_insert.x = pt_insert[0] ;
						pt_3d_insert.y= pt_3d_insert.y+ (iUpOrDown==0?15:-15) ;
						strBlockName.Format(_T("PMT_DIVIDE_0")) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
						{
							if (iUpOrDown!=0)
							{
								pRef->setRotation(PI) ;
							}
							pRef->close() ;
							pRef = NULL ;
						}

						CString strOtherStation ;
						CADORecordset cSetFindOtherStation ;
						strSql.Format(_T("select top 1 * from import_xhj_lkdata where id<%d order by id desc"), iId) ;
						if (cSetFindOtherStation.IsOpen())
						{
							cSetFindOtherStation.Close() ;
						}
						if (cSetFindOtherStation.Open(g_PtrCon, strSql))
						{
							if (cSetFindOtherStation.GetRecordCount()>0)
							{
								cSetFindOtherStation.MoveFirst() ;
								cSetFindOtherStation.GetFieldValue(_T("belong_station"), strOtherStation) ;
							}

							cSetFindOtherStation.Close() ;
						}

						CString strStationFront = iUpOrDown==0?strOtherStation:strBelongStation ;
						CString strStationBack = iUpOrDown==0?strBelongStation:strOtherStation;
						pt_insert[0] = pt_3d_insert.x-1.5 ;
						pt_insert[1] = pt_3d_insert.y+(iUpOrDown==0?1:-3) ;
						//AcCmColor clr ;
						//clr.setColorIndex(4) ;
						//mysys.WriteText(pt_insert, strStationFront, 0, AcDb::kTextRight, 2, 0.7, clr) ;
						mysys.AddText(strStationFront, pt_insert, pt_insert, AcDb::kTextRight, AcDb::kTextBase, 2, 0.7, 0, 4) ;
						pt_insert[0] = pt_3d_insert.x+1.5 ;							
						//mysys.WriteText(pt_insert, strStationBack, 0, AcDb::kTextLeft, 2, 0.7, clr) ;
						mysys.AddText(strStationBack, pt_insert, pt_insert, AcDb::kTextLeft, AcDb::kTextBase, 2, 0.7, 0, 4) ;

					}					
				}

				iPreRealLoc = iXhRealLoc ;
				iXhIndex++ ;
				cSetFind.MoveNext() ;
			}	
			//绘制上下行轨道线
			AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
			pt_3d_start.y = pt_3d_end.y = dDrawXhjY[0] ;
			pt_3d_end.x = dCurDownX ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_start.y = pt_3d_end.y = dDrawXhjY[1] ;
			pt_3d_end.x = dCurUpX ;
			mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

			cSetFind.Close() ;

			acedRestoreStatusBar() ;

		}
		//end 绘制信号点		



	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawPdt_LKDATA_N")) ;		
	}

	return 0;
}
