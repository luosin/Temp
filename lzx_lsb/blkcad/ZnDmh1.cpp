#include "stdafx.h"
#include "zndmh.h"

#include "DBUtility.h"

#include "ConNode.h"

extern BOOL bFirst ;

// 填充一个内隔离模块的扩展数据, iGCJIndex为0表示股道无分割时GCJ1，其他表示1GCJ~10GCJ
void CZnDmh::FillXdataNGL(int iGCJIndex, const CString & strGuiDao, const CString &strIsROrF, int iIsSendCode, CMapStringToString & mapStrToStrXdata, CMapStringToString &mapToAddXdata, const CString &strJinLuName)
{
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	//GUIDAO
	mapStrToStrXdata.SetAt(_T("GUIDAO"), strGuiDao) ;

	//读取strGuiDao在ZZH中的相关位置信息
	int iShelf_FL=0, iLayer_FL=0, iBox_FL=0, iShelf=0, iLayer=0, iBox=0 ;
	this->GetZzhInfo(strGuiDao, iShelf_FL, iLayer_FL, iBox_FL, iShelf, iLayer, iBox) ;

	CString strZhLoc, strFlZhLoc, strFLIndex, strZhIndex, strNflNode1, strNflNode2 ;
	strZhLoc.Format(_T("%s%d-%d"),m_strPreZZH, iShelf, iLayer) ;
	strFlZhLoc.Format(_T("%s%d-%d"),m_strPreZZH,  iShelf_FL, iLayer_FL) ;
	strFLIndex.Format(_T("%d"), iBox_FL) ;
	strZhIndex.Format(_T("%d"), iBox) ;
	this->GetZhNbNode(_T("MGFL-T"), _T("NFL"), _T("SIDENODE1"), iBox_FL, strNflNode1) ;
	this->GetZhNbNode(_T("MGFL-T"), _T("NFL"), _T("SIDENODE2"), iBox_FL, strNflNode2) ;

	mapStrToStrXdata.SetAt(_T("ZH_LOC"), strZhLoc) ;
	mapStrToStrXdata.SetAt(_T("FLZH_LOC"), strFlZhLoc) ;
	mapStrToStrXdata.SetAt(_T("FL_INDEX"), strFLIndex) ;
	mapStrToStrXdata.SetAt(_T("ZH_INDEX"), strZhIndex) ;
	mapStrToStrXdata.SetAt(_T("NFL_NODE1"), strNflNode1) ;
	mapStrToStrXdata.SetAt(_T("NFL_NODE2"), strNflNode2) ;

	//FXP
	if (strIsROrF==_T("R")) //受电端
	{
		CString strCircuitType, strGJFPoint, strGJFSide, strGSide1, strGSide2, strFxpLoc1, strFxpLoc2 ;
		this->GetLsInfoR(strGuiDao, strCircuitType, strGJFPoint, strGJFSide, strGSide1, strGSide2, strFxpLoc1, strFxpLoc2) ;

		if (m_iFxpFrom==1) //程序计算分线盘端子，从m_strFxpStart开始顺
		{
			//strFxpLoc1 = m_strFxpCurDz ;
			this->GetFxpDz(strFxpLoc1) ;
			this->GetFxpDz(strFxpLoc2) ;
		}
		mapStrToStrXdata.SetAt(_T("FXP_LOC1"), strFxpLoc1) ;
		mapStrToStrXdata.SetAt(_T("FXP_LOC2"), strFxpLoc2) ;

		CString strGLoc ;
		int iMarkLoc = strGSide1.Find(_T("-")) ;
		strGLoc = strGSide1.Left(iMarkLoc+2) ;
		mapStrToStrXdata.SetAt(_T("G_LOC"), strGLoc) ;

		CString strGNode1, strGNode2 ;
		strGNode1 = strGSide1.Mid(iMarkLoc+2) ;
		strGNode2 = strGSide2.Mid(iMarkLoc+2) ;
		mapStrToStrXdata.SetAt(_T("G_NODE1"), strGNode1) ;
		mapStrToStrXdata.SetAt(_T("G_NODE2"), strGNode2) ;

	}
	else
	{
		CString strPower1, strPower2, strFxpLoc1, strFxpLoc2 ;
		this->GetLsInfoF(strGuiDao, strPower1, strPower2, strFxpLoc1, strFxpLoc2) ;

		if (m_iFxpFrom==1) //程序计算分线盘端子，从m_strFxpStart开始顺
		{
			//strFxpLoc1 = m_strFxpCurDz ;
			this->GetFxpDz(strFxpLoc1) ;
			this->GetFxpDz(strFxpLoc2) ;
		}

		mapStrToStrXdata.SetAt(_T("FXP_LOC1"), strFxpLoc1) ;
		mapStrToStrXdata.SetAt(_T("FXP_LOC2"), strFxpLoc2) ;
	}

	if (m_iTwoOrFourLine==2)
	{
		CString strNodeNames1[4] = {_T("A2"),_T("A12"), _T("A5"), _T("A15")} ;
		CString strZhNb ;
		strZhNb.Format(_T("MGL-%s"), strIsROrF) ;
		for(int i=0; i<4; i++)
		{	
			CString strNodeTmp ;						
			this->GetZhNbNode(strZhNb, _T("NGL-T"), strNodeNames1[i], iBox, strNodeTmp) ;
			CString strTypeTmp ;
			strTypeTmp.Format(_T("NODE_%s"),strNodeNames1[i]) ;
			mapStrToStrXdata.SetAt(strTypeTmp, strNodeTmp) ;
		}
		if (iIsSendCode==1)
		{
			mapStrToStrXdata.SetAt(_T("R_INDEX"), strZhIndex) ;
			CString strRNum1, strRNum2 ;
			strRNum1.Format(_T("%d"), iBox*2-1) ;
			strRNum2.Format(_T("%d"), iBox*2) ;
			mapStrToStrXdata.SetAt(_T("R_NUM1"), strRNum1) ;
			mapStrToStrXdata.SetAt(_T("R_NUM2"), strRNum2) ;

			CString strNodeNames2[2] = {_T("A8"),_T("A18")} ;
			for(int i=0; i<2; i++)
			{	
				CString strNodeTmp ;						
				this->GetZhNbNode(strZhNb, _T("NGL-T"), strNodeNames2[i], iBox, strNodeTmp) ;
				CString strTypeTmp ;
				strTypeTmp.Format(_T("NODE_%s"),strNodeNames2[i]) ;
				mapStrToStrXdata.SetAt(strTypeTmp, strNodeTmp) ;
			}
		}
	}
	else if (m_iTwoOrFourLine==4)
	{
		int iGroup_R=0, iIndex_R=0 ;
		iGroup_R = (iBox-1)/6+1 ;
		iIndex_R = (iBox-1)%6+1 ;

		CString strName_R ;
		strName_R.Format(_T("%%%%%d-R%d"), 160+iGroup_R, iBox) ;
		mapStrToStrXdata.SetAt(_T("R_NAME"), strName_R) ;

		CString strNodeR1, strNodeR2, strNodeR3 ; //对应I-1， I-2， 过渡端子
		strNodeR1.Format(_T("0%d-%d"), iGroup_R>2?iGroup_R+1:iGroup_R, (iIndex_R-1)*3+1) ;
		strNodeR2.Format(_T("0%d-%d"), iGroup_R>2?iGroup_R+1:iGroup_R, (iIndex_R-1)*3+3) ;
		strNodeR3.Format(_T("0%d-%d"), iGroup_R>2?iGroup_R+1:iGroup_R, (iIndex_R-1)*3+2) ;

		mapStrToStrXdata.SetAt(_T("NODE_R_SIDE1"), strNodeR1) ;
		mapStrToStrXdata.SetAt(_T("NODE_A8"), strNodeR2) ;
		mapStrToStrXdata.SetAt(_T("NODE_A18"), strNodeR3) ;

	}

	if (iIsSendCode)
	{
		CString strAddXdata ;
		CString strFirstSnPre ;
		if (iGCJIndex<100) //正线
		{
			if (iGCJIndex==0)
			{
				strFirstSnPre.Format(_T("ZJC_GCJ1")) ;
			}
			else
			{
				strFirstSnPre.Format(_T("CD_%dGCJ"), iGCJIndex) ;
			}
			strAddXdata.Format(_T("MY_SN,NGL_NODE_A18_%d;FIRST_SN,%s_22"), iGCJIndex, strFirstSnPre) ;
			mapToAddXdata.SetAt(_T("NODE_A18"), strAddXdata) ;
			strAddXdata.Format(_T("MY_SN,NGL_NODE_A8_%d;FIRST_SN,%s_12"),iGCJIndex, strFirstSnPre) ;
			mapToAddXdata.SetAt(_T("NODE_A8"), strAddXdata) ;
		}
		else
		{
			int iGJFIndex = iGCJIndex%100 ; //传入iGCJIndex参数时:iGCJIndex+100(下行运行方向)/iGCJIndex+200(上行运行方向)
			int iUpOrDown = iGCJIndex/200 ; //0:下行,1:上行

			strAddXdata.Format(_T("MY_SN,%sNGL_NODE_A18_%d;FIRST_SN,%s%dGJF1_%d3"), strJinLuName, iGJFIndex, strJinLuName, iGJFIndex, iUpOrDown==0?2:4) ;
			mapToAddXdata.SetAt(_T("NODE_A18"), strAddXdata) ;
			strAddXdata.Format(_T("MY_SN,%sNGL_NODE_A8_%d;FIRST_SN,%s%dGJF1_%d3"), strJinLuName, iGJFIndex, strJinLuName, iGJFIndex, iUpOrDown==0?1:3) ;
			mapToAddXdata.SetAt(_T("NODE_A8"), strAddXdata) ;
		}
	}

}

// 填充GCJ_0000（strGuDao不为空时表示股道无分割）块的扩展数据
void CZnDmh::FillXdataGCJ_0000(int iJinLuType,const CString & strGuDao, const CString	&strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strGuDaoShow = strGuDao ;
	strGuDaoShow.TrimLeft(_T("*")) ;
	strGuDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuDaoShow) ;

	CString strKey, strAddXdata ;
	strKey.Format(_T("%s_GCJ%d_22"),  iJinLuType==0?_T("ZJC"):_T("FJC"), iJinLuType==0?1:2) ;
	strAddXdata.Format(_T("FIRST_SN,NGL_NODE_A18_0;FIRST_CON_SN,CON_%s_GCJ%d_22"), iJinLuType==0?_T("ZJC"):_T("FJC"), iJinLuType==0?1:2) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;
	strKey.Format(_T("%s_GCJ%d_12"),  iJinLuType==0?_T("ZJC"):_T("FJC"), iJinLuType==0?1:2) ;
	strAddXdata.Format(_T("FIRST_SN,NGL_NODE_A8_0;FIRST_CON_SN,CON_%s_GCJ%d_12"),iJinLuType==0?_T("ZJC"):_T("FJC"), iJinLuType==0?1:2) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;	

	strKey.Format(_T("%s_GCJ%d_11"),  iJinLuType==0?_T("ZJC"):_T("FJC"), iJinLuType==0?1:2) ;
	strAddXdata.Format(_T("FIRST_SN,TFGZ_NODE_III1")) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;
	strKey.Format(_T("%s_GCJ%d_21"),  iJinLuType==0?_T("ZJC"):_T("FJC"), iJinLuType==0?1:2) ;
	strAddXdata.Format(_T("FIRST_SN,TFGZ_NODE_III2")) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s' and dmh_in_zlayer_layerblock='dmh_box_z_%s'"), strStartEndXhj,iJinLuType==0?_T("zjc"):_T("fjc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int nId = 0 ;
				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("id"), nId) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

				strKey.Format(_T("%s_ZLOC"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;

			}
			else
			{
				strErrMsg.Format(_T("FillGCJ_0000下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataGCJ_0000")) ;
	}	
}

// 填充JMJ块的扩展数据,iGuiDaoFenGe=1表示无分割
void CZnDmh::FillXdataJMJ_0000(int iJinLuType, int iGuDaoFenGe, const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey, strAddXdata ;
	if (iGuDaoFenGe==1) //JMJ31端侧面01-3~4连到了TFGZ块，01-5~6连到了GCJ1，02-3~4连到1GCJ,02-5~6连到2GCJ
	{
		strKey.Format(_T("%s_JMJ_31"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		strAddXdata.Format(_T("FIRST_SN,TFGZ_NODE_II1;FIRST_CON_SN,CON_%s_JMJ_31"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
		strKey.Format(_T("%s_JMJ_41"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		strAddXdata.Format(_T("FIRST_SN,TFGZ_NODE_II2;FIRST_CON_SN,CON_%s_JMJ_41"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	

		strKey.Format(_T("%s_JMJ_32"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		strAddXdata.Format(_T("FIRST_SN,CD_1GCJ_11;FIRST_CON_SN,CON_%s_JMJ_32"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
		strKey.Format(_T("%s_JMJ_42"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		strAddXdata.Format(_T("FIRST_SN,CD_1GCJ_21;FIRST_CON_SN,CON_%s_JMJ_42"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
		strKey.Format(_T("%s_JMJ_52"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		strAddXdata.Format(_T("FIRST_SN,CD_2GCJ_11;FIRST_CON_SN,CON_%s_JMJ_52"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
		strKey.Format(_T("%s_JMJ_62"), iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		strAddXdata.Format(_T("FIRST_SN,CD_2GCJ_21;FIRST_CON_SN,CON_%s_JMJ_62"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
	}
	else //JMJ31端侧面01-3~4连到了nGCJ(n=iGuiDaoFenGe)块，01-5~6连到了(n-1)GCJ，02-3~4连到(n+2)GCJ,02-5~6连到(n+1)GCJ
	{
		int iGCJNodes[] = {13,23,13,23, 11,21,11,21} ;
		int iJMJNodes[] = {31,41,51,61, 32,42,52,62} ;
		int iGCJIndexs[] = {0,0,-1,-1,2,2,1,1} ;
		for (int i=0;i<8; i++)
		{
			strKey.Format(_T("%s_JMJ_%d"),iJinLuType==0?_T("ZJC"):_T("FJC"), iJMJNodes[i]) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_%d;FIRST_CON_SN,CON_%s_JMJ_%d"), iGuDaoFenGe+iGCJIndexs[i],iGCJNodes[i],iJinLuType==0?_T("ZJC"):_T("FJC"), iJMJNodes[i]) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s' and dmh_in_zlayer_layerblock='dmh_box_z_%s'"), strStartEndXhj,iJinLuType==0?_T("ZJC"):_T("FJC")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int nId = 0 ;
				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("id"), nId) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				strKey.Format(_T("%s_ZLOC"),iJinLuType==0?_T("ZJC"):_T("FJC")) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;

				strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname='JMJ'"), nId) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()==1)
					{
						cSet.MoveFirst() ;
						CString strJZXHJ ;
						cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strJZXHJ) ;
						mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZXHJ) ;
					}
					else
					{
						strErrMsg.Format(_T("FillZJCFJC_0000下面语句执行结果不为1:\n%s"), strSql) ;
						AfxMessageBox(strErrMsg) ;
					}

					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("FillZJCFJC_0000下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataZJCFJC_0000")) ;
	}	
}

//填充GCJ扩展数据，此处nGCJIndex是从1开始
void CZnDmh::FillXdataGCJ(int nGCJIndex, int nGCJJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, int iJinLuType)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	strAddXdata.Format(_T("%d"), nGCJIndex) ;
	mapToReplaceXdata.SetAt(_T("GCJINDEX"), strAddXdata) ;

	strAddXdata.Format(_T("%dGCJ"), nGCJIndex) ;
	mapStrToStrXdata.SetAt(_T("GCJNAME"), strAddXdata) ;

	CString strConStr, strParSn ;
	if (iJinLuType%2==0)
	{
		strParSn.Format(_T("PAR_SN,CD-%d_ZLOC"), nGCJIndex>5?2:1) ;
	}
	else 
	{
		strParSn.Format(_T("PAR_SN,%s_ZLOC"), iGuDaoFenGe==1?_T("2CD1"):_T("2CD")) ;
	}
	strKey.Format(_T("CD_%dGCJ_22"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,NGL_NODE_A18_%d;FIRST_CON_SN,CON_%dGCJ_22;%s"), strKey, nGCJIndex,nGCJIndex, strParSn) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;
	strKey.Format(_T("CD_%dGCJ_12"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,NGL_NODE_A8_%d;FIRST_CON_SN,CON_%dGCJ_12;%s"), strKey, nGCJIndex, nGCJIndex, strParSn) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;

	//连接电缆类型，增加MY_SN
	int iNodes[4] = {11, 12, 21, 22} ;
	for (int i=0; i<4; i++)
	{
		strKey.Format(_T("CON_%dGCJ_%d"), nGCJIndex, iNodes[i]) ;
		strAddXdata.Format(_T("MY_SN,%s"), strKey) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
	}

	if (iGuDaoFenGe==1) 
	{
		//CD_nGCJ_11
		strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_11"), nGCJIndex) ;
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,CD_%dGCJ_13"), nGCJIndex, nGCJIndex-2) ;
		if (nGCJIndex==1)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,TFGZ_NODE_II1"), nGCJIndex) ;
			if (iJinLuType==0||iJinLuType==3)
			{
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,%s_JMJ_32"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC")) ;
			}
		}
		else if (nGCJIndex==2)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,TFGZ_NODE_III1"), nGCJIndex) ;
			if (iJinLuType==0||iJinLuType==3)
			{
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,%s_JMJ_52"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC")) ;
			}
		}
		strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_11"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		//CD_nGCJ_21
		strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_21"), nGCJIndex) ;
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,CD_%dGCJ_23"), nGCJIndex, nGCJIndex-2) ;
		if (nGCJIndex==1)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,TFGZ_NODE_II2"), nGCJIndex) ;
			if (iJinLuType==0||iJinLuType==3)
			{
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,%s_JMJ_42"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC")) ;
			}
		}
		else if (nGCJIndex==2)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,TFGZ_NODE_III2"), nGCJIndex) ;
			if (iJinLuType==0||iJinLuType==3)
			{
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,%s_JMJ_62"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC")) ;
			}
		}
		strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_21"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		//CD_nGCJ_13
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_13;FIRST_SN,CD_%dGCJ_11;%s"), nGCJIndex, nGCJIndex+2, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_13"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		//CD_nGCJ_23
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_23;FIRST_SN,CD_%dGCJ_21;%s"), nGCJIndex, nGCJIndex+2, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_23"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

	}
	else //股道有分割
	{
		//CD_nGCJ_11
		strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_11"), nGCJIndex) ;
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,CD_%dGCJ_13"), nGCJIndex, nGCJIndex-2) ;
		if (nGCJIndex==1)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,TFGZ_NODE_II1"), nGCJIndex) ;
		}
		else if (nGCJIndex==2)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,TFGZ_NODE_III1"), nGCJIndex) ;
		}
		strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_11"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		//CD_nGCJ_21
		strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_21"), nGCJIndex) ;
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,CD_%dGCJ_23"), nGCJIndex, nGCJIndex-2) ;
		if (nGCJIndex==1)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,TFGZ_NODE_II2"), nGCJIndex) ;
		}
		else if (nGCJIndex==2)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,TFGZ_NODE_III2"), nGCJIndex) ;
		}
		strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_21"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		//CD_nGCJ_13		
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_13;FIRST_SN,CD_%dGCJ_11;%s"), nGCJIndex, nGCJIndex+2, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_13"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		//CD_nGCJ_23
		strAddXdata.Format(_T("MY_SN,CD_%dGCJ_23;FIRST_SN,CD_%dGCJ_21;%s"), nGCJIndex, nGCJIndex+2, strParSn) ;
		strKey.Format(_T("CD_%dGCJ_23"), nGCJIndex) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		if (iJinLuType==0||iJinLuType==3)
		{
			if (nGCJIndex==iGuDaoFenGe||nGCJIndex==(iGuDaoFenGe-1))
			{
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_13;FIRST_SN,%s_JMJ_%d;%s"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC"), nGCJIndex==iGuDaoFenGe?31:51, strParSn) ;
				strKey.Format(_T("CD_%dGCJ_13"), nGCJIndex) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;

				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_23;FIRST_SN,%s_JMJ_%d;%s"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC"), nGCJIndex==iGuDaoFenGe?41:61, strParSn) ;
				strKey.Format(_T("CD_%dGCJ_23"), nGCJIndex) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
			else if (nGCJIndex==(iGuDaoFenGe+1)||nGCJIndex==(iGuDaoFenGe+2))
			{
				strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_11"), nGCJIndex) ;
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_11;FIRST_SN,%s_JMJ_%d;%s;%s"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC"), nGCJIndex==(iGuDaoFenGe+1)?52:32, strConStr, strParSn) ;
				strKey.Format(_T("CD_%dGCJ_11"), nGCJIndex) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;

				strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_21"), nGCJIndex) ;
				strAddXdata.Format(_T("MY_SN,CD_%dGCJ_21;FIRST_SN,%s_JMJ_%d;%s;%s"), nGCJIndex, iJinLuType==0?_T("ZJC"):_T("FJC"), nGCJIndex==iGuDaoFenGe?62:42, strConStr, strParSn) ;
				strKey.Format(_T("CD_%dGCJ_21"), nGCJIndex) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
		}

	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		//strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(11,12,13,21,22,23)"), strZhName, nGCJJdqIndex) ;
		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 "), strZhName, nGCJJdqIndex) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;


					strKey.Format(_T("CD_%dGCJ_%d"), nGCJIndex, iJdqNode) ;

					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}

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
		AfxMessageBox(_T("unknown error in FillXdataGCJ")) ;
	}
}

// 填充GCJ所在的组合位置图块的扩展数据
void CZnDmh::FillXdataGCJZLoc(int iJinLuType, const CString & strLayerBlk, const CString & strZhShowName,const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iMode)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("CD_ZNAME"), strZhShowName) ;

	CString strKey ;
	CString strAddXdata ;

	CString strSql ;

	strKey = _T("GCJ_ZLOC") ;
	if (iJinLuType==0)
	{
		strAddXdata.Format(_T("MY_SN,CD-%d_ZLOC"), strLayerBlk==_T("dmh_box_z_1cda_2")?2:1) ;

		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_layerblock='%s' and dmh_in_zlayer_startendxhj1='%s→%s'"), strLayerBlk, strJZ, strCZ) ;
	}
	else if (iJinLuType==1)
	{
		strAddXdata.Format(_T("MY_SN,%s_ZLOC"), strZhShowName) ;

		strSql.Format(_T("select * from dmh_in_zlayer where (dmh_in_zlayer_layerblock='dmh_box_z_%s_1' or dmh_in_zlayer_layerblock='dmh_box_z_%s_2') and (dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj2='%s→%s')"), strZhShowName, strZhShowName, strFCZ, strJZ, strFCZ, strJZ) ;
	}
	else if (iJinLuType==2)
	{
		strAddXdata.Format(_T("MY_SN,CD-%d_ZLOC"), strLayerBlk==_T("dmh_box_z_1cda_2")?2:1) ;

		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_layerblock='%s' and dmh_in_zlayer_startendxhj1='%s→%s'"), strLayerBlk, strCZ, strFJZ) ;
	}
	else if (iJinLuType==3)
	{		
		strAddXdata.Format(_T("MY_SN,%s_ZLOC"), strZhShowName) ;

		strSql.Format(_T("select * from dmh_in_zlayer where (dmh_in_zlayer_layerblock='dmh_box_z_%s_1' or dmh_in_zlayer_layerblock='dmh_box_z_%s_2') and (dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj2='%s→%s')"), strZhShowName, strZhShowName, strFJZ, strFCZ, strFJZ, strFCZ) ;
	}
	if (iMode==1)
	{
		strAddXdata.Format(_T("MY_SN,ZJ_FENGE_GUDAO_ZLOC")) ;
	}
	mapToAddXdata.SetAt(strKey, strAddXdata) ;	

	try
	{
		CADORecordset cSet ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strGCJLoc ;
				strGCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GCJ_ZLOC"), strGCJLoc) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataGCJZLoc")) ;
	}	
}

// 填充TFGZ块的扩展数据
//iRunDirect在生成侧线股道时有用，其意义和正线时相反，iRunDirect=0表示上行，但是图纸布局是从右往左
void CZnDmh::FillXdataTFGZ(const CString & strJinLuName, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, int iJinLuType, int iRunDirect)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;

	CString strKey ;
	CString strAddXdata ;

	//正线
	if (iJinLuType<4)
	{
		CString strCurs[6] = {_T("TFGZ_NODE_I1"), _T("TFGZ_NODE_I2"), _T("TFGZ_NODE_II1"), _T("TFGZ_NODE_II2"), _T("TFGZ_NODE_III1"), _T("TFGZ_NODE_III2")} ;
		CString strCons[6];
		CString strCons1[6] = {_T("ZJC_FMJ_33"), _T("ZJC_FMJ_43"), _T("ZJC_JMJ_31"), _T("ZJC_JMJ_41"), _T("ZJC_GCJ1_11"), _T("ZJC_GCJ1_21")} ;
		CString strCons2[6] = {_T("ZJC_FMJ_33"), _T("ZJC_FMJ_43"), _T("CD_1GCJ_11"), _T("CD_1GCJ_21"), _T("CD_2GCJ_11"), _T("CD_2GCJ_21")} ;

		if (iJinLuType==0)
		{
			if (iGuDaoFenGe==1)
			{
				for (int i=0; i<6; i++)
				{
					strCons[i] = strCons1[i] ;
				}
			}
			else
			{
				for (int i=0; i<6; i++)
				{
					strCons[i] = strCons2[i] ;
				}
			}
		}
		else if (iJinLuType==1)
		{
			//20130709:here
			for (int i=0; i<6; i++)
			{				
				if(i==0)
				{
					strCons[i] = _T("ZJC_FMJ_32") ;
				}
				else if (i==1)
				{
					strCons[i] = _T("ZJC_FMJ_42") ;
				}
				else
				{
					strCons[i] = strCons2[i] ;
				}
			}
		}
		else if (iJinLuType==2)
		{
			for (int i=0; i<6; i++)
			{
				if(i==0)
				{
					strCons[i] = _T("FJC_FMJ_32") ;
				}
				else if (i==1)
				{
					strCons[i] = _T("FJC_FMJ_42") ;
				}
				else
				{
					strCons[i] = strCons2[i] ;
				}
			}
		}
		else if (iJinLuType==3)
		{
			if (iGuDaoFenGe==1)
			{
				for (int i=0; i<6; i++)
				{
					strCons[i] = strCons1[i] ;
					strCons[i].Replace(_T("ZJC_JMJ"), _T("FJC_JMJ")) ;
					strCons[i].Replace(_T("ZJC_GCJ1"), _T("FJC_GCJ2")) ;
					strCons[i].Replace(_T("ZJC_FMJ"), _T("FJC_FMJ")) ;
				}
			}
			else
			{
				for (int i=0; i<6; i++)
				{
					strCons[i] = strCons2[i] ;
				}
			}
		}

		for (int iNode=0; iNode<6; iNode++)
		{
			strKey = strCurs[iNode] ;
			strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,%s"), strCurs[iNode], strCons[iNode]) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;	
		}
	}
	else
	{
		//针对侧线
		CString strCurs[4] = {_T("TFGZ_NODE_I1"), _T("TFGZ_NODE_I2"), _T("TFGZ_NODE_II1"), _T("TFGZ_NODE_II2")} ;
		CString strCons[4] = {_T("FBJ_NODE_31"), _T("FBJ_NODE_41"), _T("1GJF1_31"), _T("1GJF1_41")} ;	
		if (iRunDirect==1)
		{
			strCons[2] = _T("1GJF1_11") ;
			strCons[3] = _T("1GJF1_21") ;
		}
		for (int iNode=0; iNode<4; iNode++)
		{
			strKey = strCurs[iNode] ;
			strAddXdata.Format(_T("MY_SN,%s%s;FIRST_SN,%s%s"), strJinLuName, strCurs[iNode], strJinLuName, strCons[iNode]) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;	
		}
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zzhcell where dmh_in_zzhcell_cellcontent='%s' and dmh_in_zzhcell_layerid in (select id from dmh_in_zzhlayer where dmh_in_zzhlayer_layerblock='dmh_box_zzh_tfgz')"), strJinLuName) ;
		
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_boxnum"), iBoxNum) ;
				CString strTFGZLoc ;
				strTFGZLoc.Format(_T("%s%d-%d"), m_strPreZZH, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("TFGZ_ZH_LOC"), strTFGZLoc) ;
				CString strZHIndex ;
				strZHIndex.Format(_T("(%d)"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("JINLU_ZH_INDEX"), strZHIndex) ;

				strSql.Format(_T("select * from dmh_def_nodes where dmh_nodes_loctype='ZPW.TFG' and dmh_nodes_loc=%d"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;
						while (!cSet.IsEOF())
						{
							CString strNodeName, strNode ;
							cSet.GetFieldValue(_T("dmh_nodes_nodename"), strNodeName) ;
							cSet.GetFieldValue(_T("dmh_nodes_node"), strNode) ;
							CString strType ;
							strType.Format(_T("TFGZ_NODE_%s"), strNodeName) ;
							mapStrToStrXdata.SetAt(strType, strNode) ;

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}
			}

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
		AfxMessageBox(_T("unknown error in FillXdataTFGZ")) ;
	}
}

// 填充ZFJC-FMJ 块的扩展数据
void CZnDmh::FillXdataZFJC_FMJ(int iLineNum, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	strAddXdata.Format(_T("A%d"), iLineNum+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM1"), strAddXdata) ;
	strAddXdata.Format(_T("B%d"), iLineNum+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM2"), strAddXdata) ;	

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	CString strCurs[4] = {_T("ZJC_FMJ_33"), _T("ZJC_FMJ_43"), _T("ZJC_FMJ_31"), _T("ZJC_FMJ_41")} ;
	CString strCons[4] = {_T("TFGZ_NODE_I1"), _T("TFGZ_NODE_I2"), _T("FBJ_NODE_31"), _T("FBJ_NODE_41")} ;
	for (int iNode=0; iNode<4; iNode++)
	{
		strKey = strCurs[iNode] ;
		strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,%s"), strCurs[iNode], strCons[iNode]) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_zjc'"), strJZ, strCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int nId = 0 ;
				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("id"), nId) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strZCJLoc ;
				strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZCJLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataZFJC_FMJ下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataZFJC_FMJ")) ;
	}	
}



// 填充FBJ块的扩展数据
void CZnDmh::FillXdataFBJ(int iJinLuType, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapToReplaceXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;

	CString strCurs[4] = {_T("FBJ_NODE_31"), _T("FBJ_NODE_41"), _T("FBJ_NODE_32"), _T("FBJ_NODE_42")} ;
	CString strCons[4] = {_T("ZJC_FMJ_31"), _T("ZJC_FMJ_41"), _T(""), _T("")} ;
	strCons[2].Format(_T("%sFS_S-1"),strJinLuName) ;
	strCons[3].Format(_T("%sFS_S-2"),strJinLuName) ;
	if (iJinLuType==2||iJinLuType==3)
	{
		strCons[0] = _T("FJC_FMJ_31") ;
		strCons[1] = _T("FJC_FMJ_41") ;		
	}
	else if (iJinLuType==4)
	{
		strCons[0].Format(_T("%sTFGZ_NODE_I1"), strJinLuName) ;
		strCons[1].Format(_T("%sTFGZ_NODE_I2"), strJinLuName) ;
	}

	for (int iNode=0; iNode<4; iNode++)
	{
		strKey = strCurs[iNode] ;

		strAddXdata.Format(_T("MY_SN,%s%s;FIRST_SN,%s;FIRST_CON_SN,CON_%s"), iJinLuType<4?_T(""):strJinLuName, strCurs[iNode], strCons[iNode], strCurs[iNode]) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zcell where (dmh_in_zcell_cellcontent like '%%%s' or dmh_in_zcell_cellcontent like '%s%%') and dmh_in_zcell_jdqname='FBJ'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strFBJLoc ;
				strFBJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FBJ_ZLOC"), strFBJLoc) ;
				mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strCellContent) ;

				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='FBJ' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("FBJ_NODE_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}

					}

					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("FillXdataFBJ下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataFBJ")) ;
	}
}

// 填充lici_jfmj块扩展数据
void CZnDmh::FillXdataLiCi_JFMJ(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;
	CString strGuDaoTmp = strGuDao ; //将类似*IG+的名称改成IG
	strGuDaoTmp.TrimLeft(_T("*")) ;
	strGuDaoTmp.TrimRight(_T("+")) ;	
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuDaoTmp) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//ZJC_ZLOC FJC_ZLOC
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj2='%s→%s'"), strJZ, strCZ, strFJZ, strFCZ, strFJZ, strFCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					CString strLayerBlk ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layerblock"), strLayerBlk) ;
					int iShelfNum = 0, iLayerNum = 0 ;					

					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;					

					if (strLayerBlk==_T("dmh_box_z_zjc"))
					{
						mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZLoc) ;
					}
					else if (strLayerBlk==_T("dmh_box_z_fjc"))
					{
						mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;
					}

					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}

		//ZBJH 3JGJ所在组合位置
		if (iJinLuType==1) //反发
		{
			strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='3JGJ' and dmh_in_zcell_cellcontent='%s'"), strJZ) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iRsCount = cSet.GetRecordCount() ;
				if (iRsCount==1)
				{
					cSet.MoveFirst() ;

					int iShelfNum = 0, iLayerNum = 0 ;					
					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;	
					mapStrToStrXdata.SetAt(_T("3JGJ_ZLOC"), strZLoc) ;
				}
				else if(iRsCount>1)
				{
					strErrMsg.Format(_T("FillXdataLiCi_JFMJ下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}
				cSet.Close() ;
			}

			//3JGJ 21端子
			strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname='%s3JGJ' and dmh_def_zhnb_jdqnode=21 and dmh_def_zhnb_contype=0 "), strJZ.Left(1)) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()>0)
				{
					cSet.MoveFirst() ;

					while (!cSet.IsEOF())
					{
						int iJdqNode = 0 ;
						CString strCon ;	
						//cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
						cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
						strKey.Format(_T("ZBJH_3JGJ_21")) ;
						mapStrToStrXdata.SetAt(strKey, strCon) ;

						cSet.MoveNext() ;
					}
				}
				cSet.Close() ;
			}
		}

		//ZBJH 1LQJ所在组合位置
		if (iJinLuType==2) //正发
		{	
			strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='1LQJ' and dmh_in_zcell_cellcontent='%s'"), strJZ) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iRsCount = cSet.GetRecordCount() ;
				if (iRsCount==1)
				{
					cSet.MoveFirst() ;

					int iShelfNum = 0, iLayerNum = 0 ;					
					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;	
					mapStrToStrXdata.SetAt(_T("1LQJ_ZLOC"), strZLoc) ;
				}
				else if(iRsCount>1)
				{
					strErrMsg.Format(_T("FillXdataLiCi_JFMJ下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}
				cSet.Close() ;
			}

			//1LQJ 21端子
			strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname='%s1LQJ' and dmh_def_zhnb_jdqnode=21 and dmh_def_zhnb_contype=0 "), strJZ.Left(1)) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()>0)
				{
					cSet.MoveFirst() ;

					while (!cSet.IsEOF())
					{
						int iJdqNode = 0 ;
						CString strCon ;	
						//cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
						cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
						strKey.Format(_T("ZBJH_1LQJ_21")) ;
						mapStrToStrXdata.SetAt(strKey, strCon) ;

						cSet.MoveNext() ;
					}
				}
				cSet.Close() ;
			}
		}


		CString strStartEndXhj ;

		//begin 股道无分割、股道有分割时ZJ FF ZF
		if (iGuDaoFenGe==1||(iGuDaoFenGe>1&&iJinLuType<3))
		{
			if (iJinLuType==0)
			{
				strStartEndXhj.Format(_T("%s→%s"), strJZ, strCZ) ;
			}
			else if (iJinLuType==1) //反发时取得是正接的GJF
			{
				strStartEndXhj.Format(_T("%s→%s"), strJZ, strCZ) ;
			}
			else if (iJinLuType==2) //正发
			{
				strStartEndXhj.Format(_T("%s→%s"), strCZ, strFJZ) ;			
			}
			else if (iJinLuType==3) //反接时取得是正发的GJF
			{
				strStartEndXhj.Format(_T("%s→%s"), strCZ, strFJZ) ;
			}

			strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s' or dmh_in_zlayer_startendxhj2='%s'"), strStartEndXhj, strStartEndXhj) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iRsCountLayer = cSet.GetRecordCount() ;
				if (iRsCountLayer>0)
				{
					cSet.MoveFirst() ;

					while (!cSet.IsEOF())
					{
						CString strLayerBlk ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layerblock"), strLayerBlk) ;
						int iShelfNum = 0, iLayerNum = 0 ;
						int nId = 0 ;
						cSet.GetFieldValue(_T("id"), nId) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
						CString strZCJLoc ;
						strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;


						if (strLayerBlk==_T("dmh_box_z_1cdb"))
						{
							mapStrToStrXdata.SetAt(_T("CD-1_ZLOC"), strZCJLoc) ;
							mapStrToStrXdata.SetAt(_T("CD-1_ZNAME"), _T("1CDB")) ;
						}
						else if (strLayerBlk==_T("dmh_box_z_1cda_2"))
						{
							mapStrToStrXdata.SetAt(_T("CD-2_ZLOC"), strZCJLoc) ;
							if (iGuDaoFenGe==1)
							{
								mapStrToStrXdata.SetAt(_T("CD-2_ZNAME"), _T("1CDA-2")) ;
							}
							else
							{
								mapStrToStrXdata.SetAt(_T("CD-2_ZNAME"), _T("1CDA")) ;
							}
						}
						else if (strLayerBlk==_T("dmh_box_z_1cda_1"))
						{
							mapStrToStrXdata.SetAt(_T("CD-1_ZLOC"), strZCJLoc) ;
							if (iJinLuType==0||iJinLuType==1)
							{
								if (iRsCountLayer==2)
								{
									mapStrToStrXdata.SetAt(_T("CD-1_ZNAME"), _T("1CDA")) ;
								}
								else if (iRsCountLayer==3)
								{
									mapStrToStrXdata.SetAt(_T("CD-1_ZNAME"), _T("1CDA-1")) ;
								}
							}
							else if (iJinLuType==2||iJinLuType==3)
							{
								if (iRsCountLayer==1)
								{
									mapStrToStrXdata.SetAt(_T("CD-1_ZNAME"), _T("1CDA")) ;
								}
								else if (iRsCountLayer==2)
								{
									mapStrToStrXdata.SetAt(_T("CD-1_ZNAME"), _T("1CDA-1")) ;
								}
							}			
						}

						CADORecordset cSetCell ;
						strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname like '%%GJF1' "), nId) ;
						if (cSetCell.IsOpen())
						{
							cSetCell.Close() ;
						}
						if (cSetCell.Open(g_PtrCon, strSql))
						{
							int iRsCountGuiDao = cSetCell.GetRecordCount() ;
							if (iRsCountGuiDao>0)
							{
								cSetCell.MoveFirst() ;
								while (!cSetCell.IsEOF())
								{
									CString strJdqName ;
									CString strCell ;
									cSetCell.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
									cSetCell.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCell) ;
									CString strTypeName ;
									int iGJF1Index = _tstoi(strJdqName.Left(strJdqName.GetLength()-4)) ;
									if (iGuDaoFenGe>1&&iJinLuType==1&&iGJF1Index<=iGuDaoFenGe) //分割股道的反发励磁部分，不取分割轨
									{
										cSetCell.MoveNext() ;
										continue;
									}
									strTypeName.Format(_T("GUIDAO%d"), iGJF1Index) ;
									strCell.TrimLeft(_T("*")) ;
									strCell.TrimRight(_T("+")) ;
									mapStrToStrXdata.SetAt(strTypeName, strCell) ;

									cSetCell.MoveNext() ;
								}
							}
							cSetCell.Close() ;
						}						

						cSet.MoveNext() ;
					}
				}

				cSet.Close() ;
			}
		}//end 股道无分割、股道有分割时ZJ FF ZF		
		else if (iGuDaoFenGe>1&&iJinLuType==3)
		{
			//先取正接中的分割轨
			strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_1cdb'"), strJZ, strCZ) ;

			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iRsCountLayer = cSet.GetRecordCount() ;
				if (iRsCountLayer==1)
				{
					cSet.MoveFirst() ;					

					int iShelfNum = 0, iLayerNum = 0 ;
					int nId = 0 ;
					cSet.GetFieldValue(_T("id"), nId) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZCJLoc ;
					strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					mapStrToStrXdata.SetAt(_T("ZJ_FENGE_GUDAO_ZLOC"), strZCJLoc) ;

					CADORecordset cSetCell ;
					strSql.Format(_T("select top %d * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname like '%%GJF1' "),iGuDaoFenGe, nId) ;
					if (cSetCell.IsOpen())
					{
						cSetCell.Close() ;
					}
					if (cSetCell.Open(g_PtrCon, strSql))
					{
						int iRsCountGuiDao = cSetCell.GetRecordCount() ;
						if (iRsCountGuiDao>0)
						{
							cSetCell.MoveFirst() ;
							while (!cSetCell.IsEOF())
							{
								CString strJdqName ;
								CString strCell ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCell) ;
								CString strTypeName ;
								int iGJF1Index = _tstoi(strJdqName.Left(strJdqName.GetLength()-4)) ;
								strTypeName.Format(_T("FENGEGUDAO%d"), iGJF1Index) ;
								strCell.TrimLeft(_T("*")) ;
								strCell.TrimRight(_T("+")) ;
								mapStrToStrXdata.SetAt(strTypeName, strCell) ;

								cSetCell.MoveNext() ;
							}
						}
						cSetCell.Close() ;
					}
				}
				else 
				{
					strErrMsg.Format(_T("FillXdataLiCi_JFMJ下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}
				cSet.Close() ;
			}

			//再取正发中的轨道
			strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s'"), strCZ, strFJZ) ;

			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iRsCountLayer = cSet.GetRecordCount() ;
				if (iRsCountLayer>0)
				{
					cSet.MoveFirst() ;	

					while (!cSet.IsEOF())
					{
						CString strLayerBlk ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layerblock"), strLayerBlk) ;
						int iShelfNum = 0, iLayerNum = 0 ;
						int nId = 0 ;
						cSet.GetFieldValue(_T("id"), nId) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
						CString strZCJLoc ;
						strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

						if (strLayerBlk==_T("dmh_box_z_1cdb"))
						{
							mapStrToStrXdata.SetAt(_T("CD-1_ZLOC"), strZCJLoc) ;
							mapStrToStrXdata.SetAt(_T("CD-1_ZNAME"), _T("1CDB")) ;
						}
						else if (strLayerBlk==_T("dmh_box_z_1cda_2"))
						{
							mapStrToStrXdata.SetAt(_T("CD-2_ZLOC"), strZCJLoc) ;
							mapStrToStrXdata.SetAt(_T("CD-2_ZNAME"), _T("1CDA")) ;
						}

						CADORecordset cSetCell ;
						strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname like '%%GJF1' "), nId) ;
						if (cSetCell.IsOpen())
						{
							cSetCell.Close() ;
						}
						if (cSetCell.Open(g_PtrCon, strSql))
						{
							int iRsCountGuiDao = cSetCell.GetRecordCount() ;
							if (iRsCountGuiDao>0)
							{
								cSetCell.MoveFirst() ;
								while (!cSetCell.IsEOF())
								{
									CString strJdqName ;
									CString strCell ;
									cSetCell.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
									cSetCell.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCell) ;
									CString strTypeName ;
									int iGJF1Index = _tstoi(strJdqName.Left(strJdqName.GetLength()-4)) ;
									strTypeName.Format(_T("GUIDAO%d"), iGJF1Index) ;
									strCell.TrimLeft(_T("*")) ;
									strCell.TrimRight(_T("+")) ;
									mapStrToStrXdata.SetAt(strTypeName, strCell) ;

									cSetCell.MoveNext() ;
								}
							}
							cSetCell.Close() ;
						}						

						cSet.MoveNext() ;
					}
				}
				cSet.Close() ;
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
		AfxMessageBox(_T("unknown error in FillXdataLiCi_JFMJ")) ;
	}
}

// 填充发送块的扩展数据
void CZnDmh::FillXdataFS(int iUpOrDwon, int iJinLuType, const CString &strJinLuName,CMapStringToString & mapReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapReplaceXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;

	CString strCurs[2] = {_T("FS_S-1"), _T("FS_S-2")} ;
	CString strCons[2] = {_T("FBJ_NODE_32"), _T("FBJ_NODE_42")} ;
	for (int iNode=0; iNode<2; iNode++)
	{
		strKey.Format(_T("%s%s"), strJinLuName, strCurs[iNode]) ;
		strAddXdata.Format(_T("FIRST_SN,%s%s"), strJinLuName,strCons[iNode]) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	
	}

	if (iJinLuType>1&&iJinLuType<4) //正发、反接
	{
		strKey.Format(_T("%sFS_NODE_Z24-2"), strJinLuName) ;
		strAddXdata.Format(_T("SECOND_SN,%sFS_NODE_1"), strJinLuName) ;
		mapToAddXdata.SetAt(strKey,strAddXdata) ;
	}
	else if (iJinLuType==4||iJinLuType==5) //侧线股道，调车接近轨，+24-2、1、2000节点相连
	{
		strKey.Format(_T("%sFS_NODE_1"), strJinLuName) ;
		strAddXdata.Format(_T("FIRST_SN,%sFS_NODE_Z24-2;SECOND_SN,%sFS_NODE_%d"), strJinLuName, strJinLuName, iUpOrDwon==0?1700:2000) ;
		mapToAddXdata.SetAt(strKey,strAddXdata) ;
		//侧线股道下行fs连接到编号用11、12......等2位数
		if (iJinLuType==4)
		{			
			for (int i=0; i<6; i++)
			{
				strKey.Format(_T("FS_TOCON_NUM%d"), i+1) ;
				strAddXdata.Format(_T("1%d"), i+1) ;
				mapStrToStrXdata.SetAt(strKey,strAddXdata) ;
			}
		}
	}
	else if (iJinLuType==6) //半自动接近轨，+24-2、1、2300节点相连
	{
		strKey.Format(_T("%sFS_NODE_1"), strJinLuName) ;
		strAddXdata.Format(_T("FIRST_SN,%sFS_NODE_Z24-2;SECOND_SN,%sFS_NODE_2300"), strJinLuName, strJinLuName) ;
		mapToAddXdata.SetAt(strKey,strAddXdata) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zcell where (dmh_in_zcell_cellcontent like '%%%s' or dmh_in_zcell_cellcontent like '%s%%') and dmh_in_zcell_jdqname='FBJ'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strFBJLoc ;
				strFBJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FBJ_ZLOC"), strFBJLoc) ;
				mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strCellContent) ;

				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='FBJ' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("FBJ_NODE_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}

					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("FillXdataFS下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}

		strSql.Format(_T("select * from dmh_in_zycell where dmh_in_zycell_jinluname like '%%%s' or dmh_in_zycell_jinluname like '%s%%'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShlefNum = 0, iBoxNum = 0 ;
				CString strJinLuNameAll, strFreq1, strFreq2 ;
				cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_freq1"), strFreq1) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_freq2"), strFreq2) ;

				strAddXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("ZY_LOC"), strAddXdata) ;

				strAddXdata.Format(_T("%dFS"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FS_INDEX"), strAddXdata) ;

				strAddXdata = strFreq1 ;
				if (!strFreq2.IsEmpty())
				{
					strAddXdata.AppendFormat(_T("/%s"), strFreq2) ;
				}
				mapStrToStrXdata.SetAt(_T("FREQ"), strAddXdata) ;

				strAddXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("DNUM"), strAddXdata) ;

				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
				mapStrToStrXdata.SetAt(_T("POWER_DNUM_KZ"), strAddXdata) ;
				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
				mapStrToStrXdata.SetAt(_T("POWER_DNUM_KF"), strAddXdata) ;
				strAddXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("POWER_RDNUM"), strAddXdata) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataFS下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}
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
		AfxMessageBox(_T("unknown error in FillXdataFS")) ;
	}
}

// 填充FuSi-GJF块的扩展数据
void CZnDmh::FillXdataFuSi_GJF(const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s' and dmh_in_zlayer_layerblock<>'dmh_box_z_zjc'"), strStartEndXhj) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					CString strLayerBlk ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layerblock"), strLayerBlk) ;
					int iShelfNum = 0, iLayerNum = 0 ;
					int nId = 0 ;
					cSet.GetFieldValue(_T("id"), nId) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;					

					if (strLayerBlk==_T("dmh_box_z_1cda_2"))
					{						
						mapStrToStrXdata.SetAt(_T("CD-2_ZLOC"), strZLoc) ;
						if (iGuDaoFenGe==1)
						{
							mapStrToStrXdata.SetAt(_T("CD-2_ZNAME"), _T("1CDA-2")) ;
						}
						else
						{
							mapStrToStrXdata.SetAt(_T("CD-2_ZNAME"), _T("1CDA")) ;
						}
					}

					CADORecordset cSetCell ;
					strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname like '%%GJF1' "), nId) ;
					if (cSetCell.IsOpen())
					{
						cSetCell.Close() ;
					}
					if (cSetCell.Open(g_PtrCon, strSql))
					{
						if (cSetCell.GetRecordCount()>0)
						{
							cSetCell.MoveFirst() ;
							while (!cSetCell.IsEOF())
							{
								CString strJdqName ;
								CString strCell ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCell) ;
								CString strTypeName ;
								strTypeName.Format(_T("GUIDAO%s"), strJdqName.Left(strJdqName.GetLength()-4)) ;

								strCell.TrimLeft(_T("*")) ;
								strCell.TrimRight(_T("+")) ;
								mapStrToStrXdata.SetAt(strTypeName, strCell) ;

								cSetCell.MoveNext() ;
							}

						}

						cSetCell.Close() ;
					}				

					cSet.MoveNext() ;
				}
			}

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
		AfxMessageBox(_T("unknown error in FillXdataFuSi_GJF")) ;
	}
}

// 填充块fusi-xjf的扩展数据
void CZnDmh::FillXdataFuSi_XJF(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;

	CString strGuDaoShow = strGuDao ;
	strGuDaoShow.TrimLeft(_T("*")) ;
	strGuDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuDaoShow) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj1='%s→%s'"), strJZ, strCZ, strFJZ, strFCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					CString strLayerBlk ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layerblock"), strLayerBlk) ;
					int iShelfNum = 0, iLayerNum = 0 ;
					int nId = 0 ;
					cSet.GetFieldValue(_T("id"), nId) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;					

					if (strLayerBlk==_T("dmh_box_z_zjc"))
					{
						mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZLoc) ;
					}
					else if (strLayerBlk==_T("dmh_box_z_fjc"))
					{
						mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;
					}

					cSet.MoveNext() ;
				}

			}

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
		AfxMessageBox(_T("unknown error in FillXdataFuSi_XJF")) ;
	}
}

// 填充GCJGJF-GJF块的扩展数据
//此处iGJFIndex是nGJF1前的n，而iGCJIndex是与该继电器关联的GCJ序号，分别对应于TYPE和MY_SN,例：*IIG2+,和1GCJ关联的GJF1，TYPE=1CDB_2GJF1_31，MY_SN=CD_1GJF1_31
void CZnDmh::FillXdataGCJGJF_GJF(int iJinLuType, int iGCJIndex, int iGJFIndex,  int iMaxGCJ,const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuiDao,int nGJFJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	//GUIDAO
	if (iGCJIndex==0)
	{
		strKey = _T("GUDAO") ;			
	}
	else
	{
		strKey = _T("GUIDAO") ;
	}
	CString strGuiDaoShow = strGuiDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(strKey, strGuiDaoShow) ;
	//GJFNAME
	strAddXdata.Format(_T("%dGJF1"), iGJFIndex) ;
	mapStrToStrXdata.SetAt(_T("GJFNAME"), strAddXdata) ;
	//JDQ_JD_GROUP
	CString strGJFJDQ_JDGROUP = _T("XXX") ; //正接、正发用第1组节点，反发、反接用第3组节点
	if (iJinLuType%2==0)
	{
		strGJFJDQ_JDGROUP = _T("1") ;
	}
	else
	{
		strGJFJDQ_JDGROUP = _T("3") ;
	}
	mapStrToStrXdata.SetAt(_T("JDQ_JD_GROUP"), strGJFJDQ_JDGROUP) ;    

	//GJFINDEX
	strAddXdata.Format(_T("%d"), iGJFIndex) ;
	mapToReplaceXdata.SetAt(_T("GJFINDEX"), strAddXdata) ;
	//GJFINDEX1
	strAddXdata.Format(_T("%d"), iGCJIndex) ;
	mapToReplaceXdata.SetAt(_T("GCJINDEX"), strAddXdata) ;
	//JDQJDGROUP
	mapToReplaceXdata.SetAt(_T("JDQJDGROUP"), strGJFJDQ_JDGROUP) ;
	//PARSN
	CString strParSn ;
	strParSn.Format(_T("CD-%d_ZLOC"), iGJFIndex>5?2:1) ;
	if (iJinLuType==3&&strGuiDao.Left(1)==_T("*")) //反接时，有分割的股道的GJF1不在正发组合里面，而在正接里面
	{
		strParSn = _T("ZJ_FENGE_GUDAO_ZLOC") ;
	}
	mapToReplaceXdata.SetAt(_T("PARSN"), strParSn) ;
	//ZHNAME	
	mapToReplaceXdata.SetAt(_T("ZHNAME"), strZhName) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		//strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(%s1,%s2,%s3)"), strZhName, nGJFJdqIndex, strGJFJDQ_JDGROUP, strGJFJDQ_JDGROUP, strGJFJDQ_JDGROUP) ;
		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, nGJFJdqIndex) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;

					strKey.Format(_T("%s_%dGJF1_%d"), strZhName, iGJFIndex, iJdqNode) ;
					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}

		if (iGCJIndex==0)
		{
			strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_zjc'"), strJZ, strCZ) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()==1)
				{
					cSet.MoveFirst() ;							
					int iShelfNum = 0, iLayerNum = 0 ;							
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZCJLoc ;
					strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZCJLoc) ;
				}
				else
				{
					strErrMsg.Format(_T("FillXdataGCJGJF_GJF下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}

				cSet.Close() ;
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
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_GJF")) ;
	}

	if (iJinLuType==0)
	{
		if (iGuDaoFenGe==1) 
		{
			switch (iGCJIndex)
			{
			case 0:
				{
					//无内容，反接时有
				}
				break;
			case 1:
				{
					strAddXdata.Format(_T("FIRST_SN,ZJC_JMJ_12")) ;
					mapToAddXdata.SetAt(_T("CD_1GJF1_11"), strAddXdata) ;
				}
				break;		
			case 5:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_6GJF1_11")) ;
					mapToAddXdata.SetAt(_T("CD_5GJF1_12"), strAddXdata) ;
				}
				break;
			case 6:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_5GJF1_12")) ;
					mapToAddXdata.SetAt(_T("CD_6GJF1_11"), strAddXdata) ;
				}
				break;
			default:
				break;
			}

			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGJF1_12"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_11")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}

		}
		else 
		{

			strKey.Format(_T("CD_%dGJF1_11"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_12"), iGCJIndex-1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			strKey.Format(_T("CD_%dGJF1_12"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_11"), iGCJIndex+1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			if (iGCJIndex==iGuDaoFenGe)
			{
				strKey.Format(_T("CD_%dGJF1_12"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZJC_JMJ_11")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
			else if(iGCJIndex==(iGuDaoFenGe+1))
			{
				strKey.Format(_T("CD_%dGJF1_11"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZJC_JMJ_12")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}

			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGJF1_12"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_11")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
		}

	}
	else if (iJinLuType==1)
	{
		//股道分割与否都一样
		strKey.Format(_T("CD_%dGJF1_31"), iGCJIndex) ;
		strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_32"), iGCJIndex-1) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		strKey.Format(_T("CD_%dGJF1_32"), iGCJIndex) ;
		strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_31"), iGCJIndex+1) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		strKey.Format(_T("CD_%dGJF1_33"), iGCJIndex) ;
		strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_1"), iGCJIndex-1) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;

		if (iGCJIndex==1)
		{
			// 			if (iGJFIndex!=5) //在反接反发中，各GJF1的31端子....?待定，暂时手改
			// 			{
			// 				strAddXdata.Format(_T("%s_%sGJF1_%s1"), strZhName, iGJFIndex, strGJFJDQ_JDGROUP) ;
			// 				mapStrToStrXdata.SetAt(strAddXdata, _T("04-11")) ;
			// 			}
			strKey.Format(_T("CD_%dGJF1_31"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,ZJC_FMJ_12")) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}
		//对应1GCJ,6GCJ的GJF1
		if (iGCJIndex%5==1)
		{
			strKey.Format(_T("CD_%dGJF1_33"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_3"), iGCJIndex) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}

		//此段代码须放到switch后面
		if (iGCJIndex==iMaxGCJ) //
		{				
			strKey.Format(_T("CD_%dGJF1_32"),iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,ZJC_GJF1_51")) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}
	}
	else if (iJinLuType==2)
	{
		switch (iGCJIndex)
		{
		case 1:
			{		 
				strAddXdata.Format(_T("FIRST_SN,FJC_FMJ_12")) ;
				mapToAddXdata.SetAt(_T("CD_1GJF1_11"), strAddXdata) ;
			}
			break;		
		case 5:
			{
				strAddXdata.Format(_T("FIRST_SN,CD_6GJF1_11")) ;
				mapToAddXdata.SetAt(_T("CD_5GJF1_12"), strAddXdata) ;
			}
			break;
		case 6:
			{
				strAddXdata.Format(_T("FIRST_SN,CD_5GJF1_12")) ;
				mapToAddXdata.SetAt(_T("CD_6GJF1_11"), strAddXdata) ;
			}
			break;
		default:
			break;
		}

		//此段代码须放到switch后面
		if (iGCJIndex==iMaxGCJ) //连到ZBJH
		{
			strKey.Format(_T("CD_%dGJF1_12"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,ZJC_GJF1_61")) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}	
	}
	else if (iJinLuType==3)
	{
		if (iGuDaoFenGe==1) 
		{
			strKey.Format(_T("CD_%dGJF1_31"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_32"), iGCJIndex-1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			strKey.Format(_T("CD_%dGJF1_32"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_31"), iGCJIndex+1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			strKey.Format(_T("CD_%dGJF1_33"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_1"), iGCJIndex-1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			if (iGCJIndex==0)
			{
				strAddXdata.Format(_T("FIRST_SN,FJC_JMJ_11")) ;
				mapToAddXdata.SetAt(_T("ZJC_GJF1_32"), strAddXdata) ;
				strAddXdata.Format(_T("FIRST_SN,FJC_GCJ2_3")) ;
				mapToAddXdata.SetAt(_T("ZJC_GJF1_33"), strAddXdata) ;
			}
			if (iGCJIndex==1)
			{
				strKey.Format(_T("CD_%dGJF1_31"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,FJC_FMJ_12")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
			//对应1GCJ,6GCJ的GJF1
			if (iGCJIndex%5==1)
			{
				strKey.Format(_T("CD_%dGJF1_33"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_3"), iGCJIndex) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}

			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGJF1_32"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_11")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
		}
		else 
		{

			strKey.Format(_T("CD_%dGJF1_31"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_32"), iGCJIndex-1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			strKey.Format(_T("CD_%dGJF1_32"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_31"), iGCJIndex+1) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			strKey.Format(_T("CD_%dGJF1_33"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_1"), iGCJIndex-1) ;
			if (iGCJIndex%5==1)
			{				
				strAddXdata.Format(_T("FIRST_SN,CD_%dGCJ_3"), iGCJIndex) ;				
			}
			mapToAddXdata.SetAt(strKey, strAddXdata) ;

			if (iGCJIndex==iGuDaoFenGe)
			{
				strKey.Format(_T("CD_%dGJF1_32"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,FJC_JMJ_11")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
			else if(iGCJIndex==(iGuDaoFenGe+1))
			{
				strKey.Format(_T("CD_%dGJF1_31"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,FJC_JMJ_12")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}

			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGJF1_32"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_11")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
		}

	}

}

// 填充GCJGJF-GCJ块的扩展数据
void CZnDmh::FillXdataGCJGJF_GCJ(int iJinLuType, int iGCJIndex,  int iMaxGCJ, const CString &strGuiDao, int nGCJJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, CString strJZ, CString strFCZ, CString strCZ, CString strFJZ)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	//PARSN
	CString strParSn ;
	if (iJinLuType==0||iJinLuType==2)
	{
		strParSn.Format(_T("CD-%d_ZLOC"), iGCJIndex>5?2:1) ;
	}
	else 
	{
		strParSn.Format(_T("%s_ZLOC"), iGuDaoFenGe==1?_T("2CD1"):_T("2CD")) ;
	}
	mapToReplaceXdata.SetAt(_T("PARSN"), strParSn) ;

	//GCJINDEX
	strAddXdata.Format(_T("%d"), iGCJIndex) ;
	mapToReplaceXdata.SetAt(_T("GCJINDEX"), strAddXdata) ;

	strAddXdata.Format(_T("%dGCJ"), iGCJIndex) ;
	mapStrToStrXdata.SetAt(_T("GCJNAME"), strAddXdata) ;

	if (iJinLuType==0)
	{
		if (iGuDaoFenGe==1) 
		{
			switch (iGCJIndex)
			{
			case 0:
				{	
					CString strGuiDaoShow = strGuiDao ;
					strGuiDaoShow.TrimLeft(_T("*")) ;
					strGuiDaoShow.TrimRight(_T("+")) ;
					mapStrToStrXdata.SetAt(_T("GUDAO"), strGuiDaoShow) ;
					strAddXdata.Format(_T("FIRST_SN,CD_1GCJ_3")) ;
					mapToAddXdata.SetAt(_T("ZJC_GCJ1_1"), strAddXdata) ;
				}
				break;
			case 1:
				{				 
					strAddXdata.Format(_T("FIRST_SN,ZJC_GCJ1_1")) ;
					mapToAddXdata.SetAt(_T("CD_1GCJ_3"), strAddXdata) ;
				}
				break;		
			case 5:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_6GCJ_3")) ;
					mapToAddXdata.SetAt(_T("CD_5GCJ_1"), strAddXdata) ;
				}
				break;
			case 6:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_5GCJ_1")) ;
					mapToAddXdata.SetAt(_T("CD_6GCJ_3"), strAddXdata) ;
				}
				break;
			default:
				break;
			}

			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_13")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
		}
		else 
		{
			switch (iGCJIndex)
			{					
			case 5:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_6GCJ_3")) ;
					mapToAddXdata.SetAt(_T("CD_5GCJ_1"), strAddXdata) ;
				}
				break;
			case 6:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_5GCJ_1")) ;
					mapToAddXdata.SetAt(_T("CD_6GCJ_3"), strAddXdata) ;
				}
				break;
			default:
				break;
			}
			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_13")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
		}
	}
	else if (iJinLuType==1)
	{
		//int iGJFIndex = iMaxGCJ-iGCJIndex+1 ; //和当前GCJ对应的GJF的序号

		//股道分割与否都是用2CD一个组合
		switch (iGCJIndex)
		{
		case 1:
			{				 
				strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex);
				mapToAddXdata.SetAt(_T("CD_1GCJ_3"), strAddXdata) ;
				strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
				mapToAddXdata.SetAt(_T("CD_1GCJ_1"), strAddXdata) ;
			}
			break;		
		case 5:
			{
				strAddXdata.Format(_T("FIRST_SN,CD_6GCJ_3")) ;
				mapToAddXdata.SetAt(_T("CD_5GCJ_1"), strAddXdata) ;
			}
			break;
		case 6:
			{
				strAddXdata.Format(_T("FIRST_SN,CD_5GCJ_1;SECOND_SN,CD_%dGJF1_33"), iGCJIndex) ;
				mapToAddXdata.SetAt(_T("CD_6GCJ_3"), strAddXdata) ;
				strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
				mapToAddXdata.SetAt(_T("CD_6GCJ_1"), strAddXdata) ;
			}
			break;
		case 2:
		case 3:
		case 4:
		case 7:
		case 8:
		case 9:
			{
				strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}
			break;
		default:
			break;
		}

		//此段代码须放到switch后面
		if (iGCJIndex==iMaxGCJ) //连到ZBJH
		{
			strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,ZJC_GJF1_53")) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}	

	}
	else if (iJinLuType==2)
	{
		switch (iGCJIndex)
		{					
		case 5:
			{
				strAddXdata.Format(_T("FIRST_SN,CD_6GCJ_3")) ;
				mapToAddXdata.SetAt(_T("CD_5GCJ_1"), strAddXdata) ;
			}
			break;
		case 6:
			{
				strAddXdata.Format(_T("FIRST_SN,CD_5GCJ_1")) ;
				mapToAddXdata.SetAt(_T("CD_6GCJ_3"), strAddXdata) ;
			}
			break;
		default:
			break;
		}
		//此段代码须放到switch后面
		if (iGCJIndex==iMaxGCJ) //连到ZJC-GJF1
		{
			strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
			strAddXdata.Format(_T("FIRST_SN,ZJC_GJF1_63")) ;
			mapToAddXdata.SetAt(strKey, strAddXdata) ;
		}
	}
	else if (iJinLuType==3)
	{		
		//int iGJFIndex = iMaxGCJ-iGCJIndex+1 ; //和当前GCJ对应的GJF的序号
		if (iGuDaoFenGe==1) 
		{
			switch (iGCJIndex)
			{
			case 0:
				{	
					CString strGuiDaoShow = strGuiDao ;
					strGuiDaoShow.TrimLeft(_T("*")) ;
					strGuiDaoShow.TrimRight(_T("+")) ;
					mapStrToStrXdata.SetAt(_T("GUDAO"), strGuiDaoShow) ;
					strAddXdata.Format(_T("FIRST_SN,CD_1GCJ_3")) ;
					mapToAddXdata.SetAt(_T("FJC_GCJ2_1"), strAddXdata) ;
					strAddXdata.Format(_T("FIRST_SN,ZJC_GJF1_33")) ;
					mapToAddXdata.SetAt(_T("FJC_GCJ2_3"), strAddXdata) ;
				}
				break;
			case 1:
				{				 
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33;SECOND_SN,FJC_GCJ2_1"), iGCJIndex);
					mapToAddXdata.SetAt(_T("CD_1GCJ_3"), strAddXdata) ;
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
					mapToAddXdata.SetAt(_T("CD_1GCJ_1"), strAddXdata) ;
				}
				break;		
			case 5:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_6GCJ_3")) ;
					mapToAddXdata.SetAt(_T("CD_5GCJ_1"), strAddXdata) ;
				}
				break;
			case 6:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_5GCJ_1;SECOND_SN,CD_%dGJF1_33"), iGCJIndex) ;
					mapToAddXdata.SetAt(_T("CD_6GCJ_3"), strAddXdata) ;
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
					mapToAddXdata.SetAt(_T("CD_6GCJ_1"), strAddXdata) ;
				}
				break;
			case 2:
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:
				{
					strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
					mapToAddXdata.SetAt(strKey, strAddXdata) ;
				}
				break;
			default:
				break;
			}
			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_13")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}

		}
		else //JMJ31端侧面01-3~4连到了nGCJ(n=iGuiDaoFenGe)块，01-5~6连到了(n-1)GCJ，02-3~4连到(n+2)GCJ,02-5~6连到(n+1)GCJ
		{
			switch (iGCJIndex)
			{
			case 1:
				{				 
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex);
					mapToAddXdata.SetAt(_T("CD_1GCJ_3"), strAddXdata) ;
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
					mapToAddXdata.SetAt(_T("CD_1GCJ_1"), strAddXdata) ;
				}
				break;		
			case 5:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_6GCJ_3")) ;
					mapToAddXdata.SetAt(_T("CD_5GCJ_1"), strAddXdata) ;
				}
				break;
			case 6:
				{
					strAddXdata.Format(_T("FIRST_SN,CD_5GCJ_1;SECOND_SN,CD_%dGJF1_33"), iGCJIndex) ;
					mapToAddXdata.SetAt(_T("CD_6GCJ_3"), strAddXdata) ;
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
					mapToAddXdata.SetAt(_T("CD_6GCJ_1"), strAddXdata) ;
				}
				break;
			case 2:
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:
				{
					strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
					strAddXdata.Format(_T("FIRST_SN,CD_%dGJF1_33"), iGCJIndex+1);
					mapToAddXdata.SetAt(strKey, strAddXdata) ;
				}
				break;
			default:
				break;
			}
			//此段代码须放到switch后面
			if (iGCJIndex==iMaxGCJ) //连到ZBJH
			{
				strKey.Format(_T("CD_%dGCJ_1"), iGCJIndex) ;
				strAddXdata.Format(_T("FIRST_SN,ZBJH_JGJ-LQJ_13")) ;
				mapToAddXdata.SetAt(strKey, strAddXdata) ;
			}

		}
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		//strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(1,3)"), strZhName, nGCJJdqIndex) ;
		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 "), strZhName, nGCJJdqIndex) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;

					strKey.Format(_T("CD_%dGCJ_%d"), iGCJIndex, iJdqNode) ;
					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}

		if (iJinLuType==3&&iGCJIndex==0)
		{
			strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_fjc'"), strFJZ, strFCZ) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()==1)
				{
					cSet.MoveFirst() ;							
					int iShelfNum = 0, iLayerNum = 0 ;							
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZCJLoc ;
					strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZCJLoc) ;
				}
				else
				{
					strErrMsg.Format(_T("FillXdataGCJGJF_GCJ下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}

				cSet.Close() ;
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
		AfxMessageBox(_T("unknown error in FillXdataGCJ")) ;
	}

}

// 填充GCJGJF-JFMJ_0000块扩展数据
void CZnDmh::FillXdataGCJGJF_JFMJ_0000(const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey, strXdata ;

	if (iGuDaoFenGe==1)
	{
		strKey = _T("ZJC_JMJ_12") ;
		strXdata = _T("FIRST_SN,CD-1_1GJF1_11") ;
		mapToAddXdata.SetAt(strKey, strXdata) ;
	}
	else
	{
		strKey = _T("ZJC_JMJ_12") ;
		strXdata.Format(_T("FIRST_SN,CD-1_%dGJF1_11"), iGuDaoFenGe+1);
		mapToAddXdata.SetAt(strKey, strXdata) ;

		strKey = _T("ZJC_JMJ_11") ;
		strXdata.Format(_T("FIRST_SN,CD-1_%dGJF1_12"), iGuDaoFenGe);
		mapToAddXdata.SetAt(strKey, strXdata) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s' and dmh_in_zlayer_layerblock='dmh_box_z_zjc'"), strStartEndXhj) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{					
					int iShelfNum = 0, iLayerNum = 0 ;
					int nId = 0 ;
					cSet.GetFieldValue(_T("id"), nId) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZCJLoc ;
					strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

					mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZCJLoc) ;

					CADORecordset cSetCell ;
					strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname in('JMJ')"), nId) ;
					if (cSetCell.IsOpen())
					{
						cSetCell.Close() ;
					}
					if (cSetCell.Open(g_PtrCon, strSql))
					{
						if (cSetCell.GetRecordCount()>0)
						{
							cSetCell.MoveFirst() ;
							while (!cSetCell.IsEOF())
							{
								CString strJdqName ;
								CString strCell ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCell) ;
								CString strTypeName ;
								if (strJdqName==_T("JMJ"))
								{
									strTypeName = _T("JZ_XHJ") ;
								}
								mapStrToStrXdata.SetAt(strTypeName, strCell) ;

								cSetCell.MoveNext() ;
							}

						}

						cSetCell.Close() ;
					}

					cSet.MoveNext() ;
				}

			}

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
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_JFMJ_0000")) ;
	}
}

// 填充GCJGJF-JFMJ_1000 块的扩展数据
void CZnDmh::FillXdataGCJGJF_JFMJ_1000(int iLineNum, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	strAddXdata.Format(_T("A%d"), iLineNum+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM1"), strAddXdata) ;
	strAddXdata.Format(_T("B%d"), iLineNum+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM2"), strAddXdata) ;	

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	CString strCurs[4] = {_T("ZJC_FMJ_33"), _T("ZJC_FMJ_43"), _T("ZJC_FMJ_31"), _T("ZJC_FMJ_41")} ;
	CString strCons[4] = {_T("TFGZ_NODE_I1"), _T("TFGZ_NODE_I2"), _T("FBJ_NODE_31"), _T("FBJ_NODE_41")} ;
	for (int iNode=0; iNode<4; iNode++)
	{
		strKey = strCurs[iNode] ;
		strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,%s"), strCurs[iNode], strCons[iNode]) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_zjc'"), strJZ, strCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int nId = 0 ;
				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("id"), nId) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strZCJLoc ;
				strZCJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZCJLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataGCJGJF_JFMJ_1000下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_JFMJ_1000")) ;
	}	
}

// 填充GCJGJF_JFMJ_2000块的扩展数据
void CZnDmh::FillXdataGCJGJF_JFMJ_2000(int iLineIndex, const CString &strJZ, const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;

	strAddXdata.Format(_T("C%d"), iLineIndex+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM1"), strAddXdata) ;
	strAddXdata.Format(_T("D%d"), iLineIndex+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM2"), strAddXdata) ;	

	CString strCurs[5] = {_T("FJC_FMJ_32"), _T("FJC_FMJ_42"), _T("FJC_FMJ_31"), _T("FJC_FMJ_41"), _T("FJC_FMJ_12")} ;
	CString strCons[5] = {_T("TFGZ_NODE_I1"), _T("TFGZ_NODE_I2"), _T("FBJ_NODE_31"), _T("FBJ_NODE_41"), _T("CD-1_1GJF1_11")} ;
	for (int iNode=0; iNode<5; iNode++)
	{
		strKey = strCurs[iNode] ;
		strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,%s"), strCurs[iNode], strCons[iNode]) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_fjc'"), strFJZ, strFCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0 ;				
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("GCJGJF_JFMJ_2000下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in GCJGJF_JFMJ_2000")) ;
	}	
}

// 填充GCJGJF_JFMJ_3000块扩展数据
void CZnDmh::FillXdataGCJGJF_JFMJ_3000(int iMaxGCJ, const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey, strXdata ;

	if (iGuDaoFenGe==1)
	{
		strKey = _T("FJC_JMJ_12") ;
		strXdata.Format(_T("FIRST_SN,CD_%dGJF1_31"), iMaxGCJ) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;

		strKey = _T("FJC_JMJ_11") ;
		strXdata.Format(_T("FIRST_SN,ZJC_GJF1_32")) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;
	}
	else
	{
		strKey = _T("FJC_JMJ_12") ;
		strXdata.Format(_T("FIRST_SN,CD_%dGJF1_31"), iMaxGCJ-iGuDaoFenGe);
		mapToAddXdata.SetAt(strKey, strXdata) ;

		strKey = _T("FJC_JMJ_11") ;
		strXdata.Format(_T("FIRST_SN,CD_%dGJF1_32"), iMaxGCJ-iGuDaoFenGe+1);
		mapToAddXdata.SetAt(strKey, strXdata) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s' and dmh_in_zlayer_layerblock='dmh_box_z_fjc'"), strStartEndXhj) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{					
					int iShelfNum = 0, iLayerNum = 0 ;
					int nId = 0 ;
					cSet.GetFieldValue(_T("id"), nId) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

					mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;

					CADORecordset cSetCell ;
					strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid=%d and dmh_in_zcell_jdqname in('JMJ')"), nId) ;
					if (cSetCell.IsOpen())
					{
						cSetCell.Close() ;
					}
					if (cSetCell.Open(g_PtrCon, strSql))
					{
						if (cSetCell.GetRecordCount()>0)
						{
							cSetCell.MoveFirst() ;
							while (!cSetCell.IsEOF())
							{
								CString strJdqName ;
								CString strCell ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
								cSetCell.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCell) ;
								CString strTypeName ;
								if (strJdqName==_T("JMJ"))
								{
									strTypeName = _T("FJZ_XHJ") ;
								}
								mapStrToStrXdata.SetAt(strTypeName, strCell) ;

								cSetCell.MoveNext() ;
							}
						}

						cSetCell.Close() ;
					}

					cSet.MoveNext() ;
				}
			}

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
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_JFMJ_3000")) ;
	}
}

// 填充zbjh-3jgj块的扩展数据
void CZnDmh::FillXdataZBJH_JGJ_1LQ(int iJinLuType, const CString &strJZXHJ, int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZXHJ) ;

	if (iJinLuType==0) //正接
	{
		mapStrToStrXdata.SetAt(_T("JGJ_LQJ"), _T("3JGJ")) ;

		strKey = _T("ZBJH_JGJ-LQJ_11");
		strXdata.Format(_T("FIRST_SN,CD_%dGJF1_12"), iToConGJFIndex) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;
		strKey = _T("ZBJH_JGJ-LQJ_13");
		strXdata.Format(_T("FIRST_SN,CD_%dGCJ_1"),  iToConGJFIndex) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;
	}
	else if (iJinLuType==3) //反接
	{
		mapStrToStrXdata.SetAt(_T("JGJ_LQJ"), _T("1LQJ")) ;

		strKey = _T("ZBJH_JGJ-LQJ_11");
		strXdata.Format(_T("FIRST_SN,CD_%dGJF1_32"), iToConGJFIndex) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;
		strKey = _T("ZBJH_JGJ-LQJ_13");
		strXdata.Format(_T("FIRST_SN,CD_%dGCJ_1"), iToConGJFIndex) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//3JGJ 1LQJ的11 13端子
		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname='%s%s' and dmh_def_zhnb_jdqnode in (11,13) and dmh_def_zhnb_contype=0 "), strJZXHJ.Left(1), iJinLuType==0?_T("3JGJ"):_T("1LQJ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
					strKey.Format(_T("ZBJH_JGJ-LQJ_%d"), iJdqNode) ;
					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='3JGJ' and dmh_in_zcell_cellcontent='%s'"), strJZXHJ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{					
					int iShelfNum = 0, iLayerNum = 0 ;	

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

					mapStrToStrXdata.SetAt(_T("ZBJH_ZLOC"), strZLoc) ;

					cSet.MoveNext() ;
				}
			}
			else if(iRsCount>1)
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataZBJH_JGJ_1LQ")) ;
	}
}

// 取得线路的相关信息，进站、出站......
void CZnDmh::GetLineInfo(const CString & strLineName, CString & strJZXHJ, CString & strFCZXHJ, CString & strCZXHJ, CString & strFJZXHJ)
{
	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select distinct dmh_import_type, dmh_import_xhj, dmh_import_cellcol from dmh_import where dmh_import_table=0 and dmh_import_line='%s' order by dmh_import_cellcol asc"), strLineName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				CString strJZ_ZJ, strCZ_ZJ ; //strType==正接时获得的进站、出站
				CString strFCZ_FF, strJZ_FF ; //strType==反发时获得的反出站、进站
				CString strCZ_ZF, strFJZ_ZF ; //strType==正发时获得的出站、反进站
				CString strFJZ_FJ, strFCZ_FJ  ; //strType==反接时获得的反进站、反出站

				while (!cSet.IsEOF())
				{
					CString strType, strStartEndXhj ;
					cSet.GetFieldValue(_T("dmh_import_type"), strType) ;
					cSet.GetFieldValue(_T("dmh_import_xhj"), strStartEndXhj) ;
					int iLocFindXhj = strStartEndXhj.Find(_T("→")) ;
					int iLenXhj = strStartEndXhj.GetLength() ;
					if (iLocFindXhj!=-1)
					{
						if (strType==_T("正接"))
						{
							//取JZ、CZ							
							strJZ_ZJ = strStartEndXhj.Left(iLocFindXhj) ;
							strCZ_ZJ = strStartEndXhj.Right(iLenXhj-iLocFindXhj-2) ;			
						}
						else if (strType==_T("反发"))
						{
							//取FCZ、JZ
							strFCZ_FF = strStartEndXhj.Left(iLocFindXhj) ;
							strJZ_FF = strStartEndXhj.Right(iLenXhj-iLocFindXhj-2) ;
						}
						else if (strType==_T("正发"))
						{
							//取CZ/FJZ
							strCZ_ZF = strStartEndXhj.Left(iLocFindXhj) ;
							strFJZ_ZF = strStartEndXhj.Right(iLenXhj-iLocFindXhj-2) ;
						}
						else if (strType==_T("反接"))
						{
							//取FJZ、FCZ						
							strFJZ_FJ = strStartEndXhj.Left(iLocFindXhj) ;
							strFCZ_FJ = strStartEndXhj.Right(iLenXhj-iLocFindXhj-2) ;
						}
						else if (strType==_T("接码"))
						{
							strJZXHJ = strStartEndXhj.Left(iLocFindXhj) ;
							strCZXHJ = strStartEndXhj.Right(iLenXhj-iLocFindXhj-2) ;
						}
					}
					else 
					{
						acutPrintf(_T("\n%s未找到→标示"), strStartEndXhj) ;
					}

					cSet.MoveNext() ;
				}

				//JZ
				if (!strJZ_ZJ.IsEmpty())
				{
					strJZXHJ = strJZ_ZJ ;
				}
				else 
				{
					strJZXHJ = strJZ_FF ;
				}
				//CZ
				if (!strCZ_ZF.IsEmpty())
				{
					strCZXHJ = strCZ_ZF ;
				}
				else 
				{
					strCZXHJ = strCZ_ZJ ;
				}
				//FCZ
				if (!strFCZ_FF.IsEmpty())
				{
					strFCZXHJ = strFCZ_FF ;
				}
				else 
				{
					strFCZXHJ = strFCZ_FJ ;
				}
				//FJ
				if (!strFJZ_FJ.IsEmpty())
				{
					strFJZXHJ = strFJZ_FJ ;
				}
				else 
				{
					strFJZXHJ = strFJZ_ZF ;
				}

			}

			cSet.Close() ;
		}

		if (strFCZXHJ.IsEmpty()&&!strCZXHJ.IsEmpty())
		{
			strFCZXHJ.Format(_T("%s%s"), strCZXHJ.Left(1)==_T("X")?_T("S"):_T("X"), strCZXHJ.Right(strCZXHJ.GetLength()-1)) ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetLineInfo")) ;
	}
}

// 填充+1FS块扩展数据
void CZnDmh::FillXdataFSPLUS(CMapStringToString & mapStrToStrXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;

	CString strXdata ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zycell where dmh_in_zycell_jinluname like '%%+1%%'")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShlefNum = 0, iBoxNum = 0 ;

				cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;			

				strXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_ZYLOC"), strXdata) ;

				strXdata.Format(_T("%dFS"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_INDEX"), strXdata) ;			

			}
			else
			{
				strErrMsg.Format(_T("FillXdataFSPLUS下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}
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
		AfxMessageBox(_T("unknown error in FillXdataFSPLUS")) ;
	}
}
// 填充（FJC-FMJ）块的扩展数据
void CZnDmh::FillXdataFJC_FMJ(int iLineIndex, const CString &strJZ, const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;

	strAddXdata.Format(_T("C%d"), iLineIndex+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM1"), strAddXdata) ;
	strAddXdata.Format(_T("D%d"), iLineIndex+1) ;
	mapStrToStrXdata.SetAt(_T("FMJ_TOCON_NUM2"), strAddXdata) ;	

	CString strCurs[5] = {_T("FJC_FMJ_32"), _T("FJC_FMJ_42"), _T("FJC_FMJ_31"), _T("FJC_FMJ_41"), _T("FJC_FMJ_12")} ;
	CString strCons[5] = {_T("TFGZ_NODE_I1"), _T("TFGZ_NODE_I2"), _T("FBJ_NODE_31"), _T("FBJ_NODE_41"), _T("CD-1_1GJF1_11")} ;
	for (int iNode=0; iNode<5; iNode++)
	{
		strKey = strCurs[iNode] ;
		strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,%s"), strCurs[iNode], strCons[iNode]) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;	
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_fjc'"), strFJZ, strFCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0 ;				
				cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataFJC_FMJ下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataFJC_FMJ")) ;
	}	
}

// 链接GCJ-GJF左端，类似ZBJH块
void CZnDmh::FillXdataZJC_GJF1(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ,const CString &strGuDao, int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strGuiDaoShow = strGuDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuiDaoShow) ;

	CString strJdqGroup, strNodeX1, strNodeX3 ;
	if (iJinLuType==1)
	{
		strJdqGroup = _T("5") ;
		strNodeX1 = _T("04-14") ;
		strNodeX3 = _T("04-15") ;
	}
	else if (iJinLuType==2)
	{
		strJdqGroup = _T("6") ;
		strNodeX1 = _T("04-16") ;
		strNodeX3 = _T("04-17") ;
	}
	mapStrToStrXdata.SetAt(_T("ZJC_GJF1_JDQGROUP"), strJdqGroup) ;
	mapStrToStrXdata.SetAt(_T("ZJC_GJF1_NODE_X1"), strNodeX1) ;
	mapStrToStrXdata.SetAt(_T("ZJC_GJF1_NODE_X3"), strNodeX3) ;	

	strKey = _T("ZJC_GJF1_NODE_X1");
	strXdata.Format(_T("MY_SN,ZJC_GJF1_%s1;FIRST_SN,CD_%dGJF1_12"), strJdqGroup, iToConGJFIndex) ;
	mapToAddXdata.SetAt(strKey, strXdata) ;
	strKey = _T("ZJC_GJF1_NODE_X3");
	strXdata.Format(_T("MY_SN,ZJC_GJF1_%s3;FIRST_SN,CD_%dGCJ_1"), strJdqGroup, iToConGJFIndex) ;
	mapToAddXdata.SetAt(strKey, strXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_startendxhj1='%s→%s' and dmh_in_zlayer_layerblock='dmh_box_z_zjc'"), strJZ, strCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iShelfNum = 0, iLayerNum = 0 ;

					cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZLoc) ;

					cSet.MoveNext() ;
				}

			}

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
		AfxMessageBox(_T("unknown error in FillXdataZJC_GJF1")) ;
	}
}

// 取得GCJ继电器在组合中的序号（1~10）
int CZnDmh::GetGCJGJFJdqInfo(int iJinLuType, const CString & strJZ, const CString & strFCZ, const CString & strCZ, const CString & strFJZ, const CString & strGuiDao, CString strZhNames[2], int iJDQIndexs[2], CString strJdqNames[2])
{	
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		CString strJdqTypes[2] = {_T("GCJ"), _T("GJF1")} ;
		CString strGuiDaoTmp = strGuiDao ;

		for (int i=0; i<2; i++)
		{
			CString strLayerId ;
			if (i==1&&iJinLuType%2==1)
			{
				if (strGuiDao.Right(1)==_T("+"))
				{
					strGuiDaoTmp.TrimRight(_T("+")) ;
				}
				else
				{
					strGuiDaoTmp.Append(_T("+")) ;
				}
			}
			if (iJinLuType==0)
			{
				strSql.Format(_T("SELECT * FROM dmh_in_zlayer WHERE dmh_in_zlayer_startendxhj1='%s→%s' and InStr(dmh_in_zlayer_layerblock,'cd')>0"), strJZ, strCZ) ;
			}
			else if (iJinLuType==1)
			{
				strSql.Format(_T("SELECT * FROM dmh_in_zlayer WHERE (dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj2='%s→%s') and InStr(dmh_in_zlayer_layerblock,'cd')>0"), strFCZ, strJZ, strFCZ, strJZ) ;
				if (i==1)
				{
					strSql.Format(_T("SELECT * FROM dmh_in_zlayer WHERE dmh_in_zlayer_startendxhj1='%s→%s' and InStr(dmh_in_zlayer_layerblock,'cd')>0"), strJZ, strCZ) ;
				}
			}
			else if (iJinLuType==2)
			{
				strSql.Format(_T("SELECT * FROM dmh_in_zlayer WHERE dmh_in_zlayer_startendxhj1='%s→%s' and InStr(dmh_in_zlayer_layerblock,'cd')>0"), strCZ, strFJZ) ;
			}
			else if (iJinLuType==3)
			{
				strSql.Format(_T("SELECT * FROM dmh_in_zlayer WHERE (dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj2='%s→%s') and InStr(dmh_in_zlayer_layerblock,'cd')>0"), strFJZ, strFCZ, strFJZ, strFCZ) ;
				if (i==1)
				{
					strSql.Format(_T("SELECT * FROM dmh_in_zlayer WHERE (dmh_in_zlayer_startendxhj1='%s→%s' or dmh_in_zlayer_startendxhj1='%s→%s') and InStr(dmh_in_zlayer_layerblock,'cd')>0"), strCZ, strFJZ, strJZ, strCZ) ;
				}
			}

			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iRsCount = cSet.GetRecordCount() ;
				if (iRsCount>0)
				{
					cSet.MoveFirst() ;
					while (!cSet.IsEOF())
					{
						int iId = 0 ;
						CString strLayerBlock ;	
						cSet.GetFieldValue(_T("id"), iId) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layerblock"), strLayerBlock) ;

						//////////////////////////////////////////////////////////////////////////
						strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and right(dmh_in_zcell_jdqname,%d)='%s' and dmh_in_zcell_layerid=%d"), strGuiDaoTmp,strJdqTypes[i].GetLength(), strJdqTypes[i],iId) ;
						CADORecordset cSet2 ;
						if (cSet2.IsOpen())
						{
							cSet2.Close() ;
						}
						if (cSet2.Open(g_PtrCon, strSql))
						{
							int iRsCount2 = cSet2.GetRecordCount() ;
							if (iRsCount2==1)
							{
								cSet2.MoveFirst() ;	
								int iBoxNum = 0 ;
								CString strJdqName ;
								cSet2.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
								cSet2.GetFieldValue(_T("dmh_in_zcell_jdqname"), strJdqName) ;
								if (strLayerBlock.Find(_T("1cda"))!=-1)
								{
									strZhNames[i]=_T("1CDA") ;
								}
								else if (strLayerBlock.Find(_T("1cdb"))!=-1)
								{
									strZhNames[i]=_T("1CDB") ;
								}
								else if (strLayerBlock.Find(_T("2cd"))!=-1)
								{
									if (strLayerBlock.Find(_T("2cd1"))!=-1)
									{
										strZhNames[i]=_T("2CD1") ;
									}
									else
									{
										strZhNames[i]=_T("2CD") ;
									}									
								}
								iJDQIndexs[i] = iBoxNum ;
								strJdqNames[i] = strJdqName ;
							}	

							cSet2.Close() ;
						}
						//////////////////////////////////////////////////////////////////////////

						cSet.MoveNext() ;
					}				
				}

				cSet.Close() ;
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
		AfxMessageBox(_T("unknown error in GetGCJGJFJdqInfo")) ;
	}

	return 0;
}

// 填充编码部分的扩展数据
void CZnDmh::FillXdataBianMa(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, int iUpOrDown)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	mapToReplaceXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;
	//XORS
	CString strXorS ;
	strXorS = strJZ.Left(1) ;
	if (iJinLuType==5) //调车接近时不能从信号机名判断出上下行方向，增加一个有默认值的参数放最后，原函数调用不用理会该参数
	{
		strXorS = (iUpOrDown==0?_T("X"):_T("S")) ;
	}
	mapToReplaceXdata.SetAt(_T("XORS"), strXorS) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//beging ZJC_ZLOC/FJC_ZLOC		
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='LXJF' and ((dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_zjc')) or (dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_fjc'))) "), strJZ, strFJZ) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					int iShelfNum = 0, iLayerNum = 0 ;
					CString strXhj ;

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strXhj) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;					

					if (strXhj==strJZ)
					{
						mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZLoc) ;
					}
					else if (strXhj==strFJZ)
					{
						mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;
					}

					cSet.MoveNext() ;
				}				

			}
			cSet.Close() ;
		}
		//end ZJC_ZLOC/FJC_ZLOC

		//begin ZBJH_ZLOC
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='2LQJ' and dmh_in_zcell_cellcontent='%s'"), strJZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{					
					int iShelfNum = 0, iLayerNum = 0 ;	

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

					mapStrToStrXdata.SetAt(_T("ZBJH_ZLOC"), strZLoc) ;

					cSet.MoveNext() ;
				}
			}
			else if(iRsCount>1)
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
		//end ZBJH_ZLOC

		//beging ZBJH 节点
		// 		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname in('%s2LQJ','%s3LQJ','%s4LQJ') and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(11,12,13, 31,32,33)"), strXorS, strXorS, strXorS) ;
		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname in('%s2LQJ','%s3LQJ','%s4LQJ') and dmh_def_zhnb_contype=0"), strXorS, strXorS, strXorS) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon, strJdqName ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqname"), strJdqName) ;

					strKey.Format(_T("ZBJH_%s_%d"), strJdqName, iJdqNode) ;

					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		//end ZBJH 节点

		//begin 侧线股道，XHFS_ZLOC
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_xhfs') "), iJinLuType==6?strJZ:strCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount>0)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;

				strAddXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("XHFSJDQINDEX"), strAddXdata) ;

				//begin XHFS 节点
				// 				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='XHFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(11,12,13)"), iBoxNum) ;
				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='XHFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;

				CADORecordset cSetTie ;
				if (cSetTie.IsOpen())
				{
					cSetTie.Close() ;
				}
				if (cSetTie.Open(g_PtrCon, strSql))
				{
					if (cSetTie.GetRecordCount()>0)
					{
						cSetTie.MoveFirst() ;
						while (!cSetTie.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;	
							cSetTie.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSetTie.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							//有的块里定义的type是XHFS_XJF_%d形式，有的是XHFS_%dXJF_%d形式
							strKey.Format(_T("XHFS_XJF_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;

							strKey.Format(_T("XHFS_%dXJF_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;

							cSetTie.MoveNext() ;
						}
					}
					cSetTie.Close() ;
				}
				//end XHFS 节点

				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("XHFS_ZLOC"), strZLoc) ;
			}
			cSet.Close() ;
		}
		//end 侧线股道，XHFS_ZLOC

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataBianMa")) ;
	}
}

// 填充侧线GJF1块的扩展数据
void CZnDmh::FillXdataGJF1_CeXian(const CString &strJinLuName, const CString &strGuiDao, int iGCJIndex,  CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strXdata ;

	int iGJFIndex = iGCJIndex%100 ; //传入iGCJIndex参数时:iGCJIndex+100(下行运行方向)/iGCJIndex+200(上行运行方向)
	int iUpOrDown = iGCJIndex/200 ; //0:下行,1:上行

	CString strGuiDaoShow = strGuiDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUIDAO"), strGuiDaoShow) ;

	int iJdqNodes[2][6] = {{11,12,13, 21,22,23},{31,32,33, 41,42,43}} ;
	//CString strFirstSns[6] = {_T("GJF1_12"), _T("GJF1_11")} ;
	for (int i=0; i<6; i++)
	{
		strKey.Format(_T("GJF1_%d"), iJdqNodes[iUpOrDown][i]) ;
		CString	strFirstSn ;
		switch (i)
		{
		case 0:
			{
				strFirstSn.Format(_T("%s%dGJF1_%d2"), strJinLuName,iGJFIndex-1, iUpOrDown==0?1:3) ;
				if (iGJFIndex==1)
				{
					strFirstSn.Format(_T("%sTFGZ_NODE_II1"), strJinLuName) ;
				}
			}
			break;
		case 1:
			{
				strFirstSn.Format(_T("%s%dGJF1_%d1"), strJinLuName,iGJFIndex+1, iUpOrDown==0?1:3) ;
			}
			break;
		case 2:
			{
				strFirstSn.Format(_T("%sNGL_NODE_A8_%d"), strJinLuName,iGJFIndex) ;
			}
			break;
		case 3:
			{
				strFirstSn.Format(_T("%s%dGJF1_%d2"), strJinLuName,iGJFIndex-1, iUpOrDown==0?2:4) ;
				if (iGJFIndex==1)
				{
					strFirstSn.Format(_T("%sTFGZ_NODE_II2"), strJinLuName) ;
				}
			}
			break;
		case 4:
			{
				strFirstSn.Format(_T("%s%dGJF1_%d1"), strJinLuName,iGJFIndex+1, iUpOrDown==0?2:4) ;
			}
			break;
		case 5:
			{
				strFirstSn.Format(_T("%sNGL_NODE_A18_%d"), strJinLuName,iGJFIndex) ;
			}
			break;
		}

		strXdata.Format(_T("MY_SN,%s%dGJF1_%d;FIRST_SN,%s"), strJinLuName, iGJFIndex, iJdqNodes[iUpOrDown][i], strFirstSn) ;
		mapToAddXdata.SetAt(strKey, strXdata) ;		
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		CString strGuiDaoTmp = strGuiDao ;
		strGuiDaoTmp.TrimRight(_T("+")) ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='GJF1' and dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_gdfs')"), strGuiDaoTmp) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				//cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GDFS_ZLOC"), strZLoc) ;

				strXdata.Format(_T("(%d)"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("JDQINDEX"), strXdata) ;

				// 				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='GDFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_jdqnode in(%s) and dmh_def_zhnb_contype=0"), iBoxNum, iUpOrDown==0?_T("11,12,13, 21,22,23"):_T("31,32,33, 41,42,43")) ;
				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='GDFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;

				CADORecordset cSetJdqNode ;
				if (cSetJdqNode.IsOpen())
				{
					cSetJdqNode.Close() ;
				}
				if (cSetJdqNode.Open(g_PtrCon, strSql))
				{
					if (cSetJdqNode.GetRecordCount()>0)
					{
						cSetJdqNode.MoveFirst() ;

						while (!cSetJdqNode.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("GJF1_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSetJdqNode.MoveNext() ;
						}
					}
					cSetJdqNode.Close() ;
				}
			}
			else
			{
				acutPrintf(_T("\n%s执行结果不为1:%d"), strSql, iRsCount) ;

			}

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
		AfxMessageBox(_T("unknown error in FillXdataGJF1_CeXian")) ;
	}
}

void CZnDmh::FillXdataFBJ_FS_EXAMPLE(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata)
{
	mapStrToStrXdata.RemoveAll() ;

	CString strJinLuName, strStartEndXhj ;
	if (iJinLuType==1)
	{
		strJinLuName = _T("正向进站") ;
		strStartEndXhj.Format(_T("%s→%s"), strJZ, strCZ) ;
	}
	else if (iJinLuType==3)
	{
		strJinLuName = _T("正向出站") ;
		strStartEndXhj.Format(_T("%s→%s"), strCZ, strFJZ) ;
	}
	mapStrToStrXdata.SetAt(_T("ZX_JINLU_TYPE"), strJinLuName) ;
	mapStrToStrXdata.SetAt(_T("ZX_STARTENDXHJ"), strStartEndXhj) ;	

}

//GCJ-GJF1左侧图块（半自动接近时）
void CZnDmh::FillXdataGDFS_GJF1(const CString &strJZ,  int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	strKey = _T("GDFS_GJF1_11");
	strXdata.Format(_T("MY_SN,ZBJH_JGJ-LQJ_11;FIRST_SN,CD_%dGJF1_12"), iToConGJFIndex) ;
	mapToAddXdata.SetAt(strKey, strXdata) ;
	strKey = _T("GDFS_GJF1_13");
	strXdata.Format(_T("MY_SN,ZBJH_JGJ-LQJ_13;FIRST_SN,CD_%dGCJ_1"), iToConGJFIndex) ;
	mapToAddXdata.SetAt(strKey, strXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent like '%s%%' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_gdfs')"), strJZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString strGuiDao ;

				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strGuiDao) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GDFS_ZLOC"), strZLoc) ;

				CString strGuiDaoShow = strGuiDao ;
				strGuiDaoShow.TrimLeft(_T("*")) ;
				strGuiDaoShow.TrimRight(_T("+")) ;
				mapStrToStrXdata.SetAt(_T("GUIDAO"), strGuiDaoShow) ;

				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='GDFS' and dmh_def_zhnb_jdqindex=%d"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("GDFS_GJF1_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}

					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataGDFS_GJF1")) ;
	}
}

// 填充N+1模块数据
void CZnDmh::FillXdataNAdd1(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ,  CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata,CMapStringToString & mapToReplaceXdata, int iUpOrDown)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	mapToReplaceXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;

	//XORS
	CString strXorS ;
	strXorS = strJZ.Left(1) ;
	if (iJinLuType==5) //调车接近时不能从信号机名判断出上下行方向，增加一个有默认值的参数放最后，原函数调用不用理会该参数
	{
		strXorS = (iUpOrDown==0?_T("X"):_T("S")) ;
	}
	mapToReplaceXdata.SetAt(_T("XORS"), strXorS) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in (select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_fbj')"), strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strFBJLoc ;
				strFBJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FBJ_ZLOC"), strFBJLoc) ;
				mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strCellContent) ;
				strXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX_FBJ"), strXdata) ;

				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='FBJ' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("FBJ_NODE_%d"), iJdqNode) ;
							if ((iJdqNode==11||iJdqNode==12)&&iJinLuType!=-1)
							{
								strTypeName.Format(_T("FBJ_%sNODE_%d"), strJinLuName, iJdqNode) ;
							}
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}

					cSet.Close() ;
				}

			}
			else
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}

		//beging ZJC_ZLOC/FJC_ZLOC		
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='LXJF' and ((dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_zjc')) or (dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_fjc'))) "), strJZ, strFJZ) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					int iShelfNum = 0, iLayerNum = 0 ;
					CString strXhj ;

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strXhj) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;					

					if (strXhj==strJZ)
					{
						mapStrToStrXdata.SetAt(_T("ZJC_ZLOC"), strZLoc) ;
					}
					else if (strXhj==strFJZ)
					{
						mapStrToStrXdata.SetAt(_T("FJC_ZLOC"), strZLoc) ;
					}

					cSet.MoveNext() ;
				}				

			}
			cSet.Close() ;
		}
		//end ZJC_ZLOC/FJC_ZLOC

		//begin ZBJH_ZLOC
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='2LQJ' and dmh_in_zcell_cellcontent='%s'"), strJZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{					
					int iShelfNum = 0, iLayerNum = 0 ;	

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

					mapStrToStrXdata.SetAt(_T("ZBJH_ZLOC"), strZLoc) ;

					cSet.MoveNext() ;
				}
			}
			else if(iRsCount>1)
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
		//end ZBJH_ZLOC

		//beging ZBJH 节点
		// 		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname in('%s2LQJ','%s3LQJ','%s4LQJ') and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(11,12,13, 31,32,33)"), strXorS, strXorS, strXorS) ;
		strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='ZBJH' and dmh_def_zhnb_jdqname in('%s2LQJ','%s3LQJ','%s4LQJ') and dmh_def_zhnb_contype=0"), strXorS, strXorS, strXorS) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon, strJdqName ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqname"), strJdqName) ;

					strKey.Format(_T("ZBJH_%s_%d"), strJdqName, iJdqNode) ;

					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		//end ZBJH 节点

		//begin 侧线股道，XHFS_ZLOC
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_xhfs') "), iJinLuType==6?strJZ:strCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount>0)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;

				strXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("XHFSJDQINDEX"), strXdata) ;

				//begin XHFS 节点
				// 				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='XHFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 and dmh_def_zhnb_jdqnode in(11,12,13)"), iBoxNum) ;
				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='XHFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 "), iBoxNum) ;

				CADORecordset cSetTie ;
				if (cSetTie.IsOpen())
				{
					cSetTie.Close() ;
				}
				if (cSetTie.Open(g_PtrCon, strSql))
				{
					if (cSetTie.GetRecordCount()>0)
					{
						cSetTie.MoveFirst() ;
						while (!cSetTie.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;	
							cSetTie.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSetTie.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							//有的块里定义的type是XHFS_XJF_%d形式，有的是XHFS_%dXJF_%d形式
							strKey.Format(_T("XHFS_XJF_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;

							strKey.Format(_T("XHFS_%dXJF_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;


							cSetTie.MoveNext() ;
						}
					}
					cSetTie.Close() ;
				}
				//end XHFS 节点

				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("XHFS_ZLOC"), strZLoc) ;
			}
			cSet.Close() ;
		}
		//end 侧线股道，XHFS_ZLOC

		//for FS
		if (iJinLuType==-1)
		{
			strSql.Format(_T("select * from dmh_in_zycell where instr(dmh_in_zycell_jinluname, '+1')>0")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()==1)
				{
					cSet.MoveFirst() ;
					int iShlefNum = 0, iBoxNum = 0 ;
					CString strJinLuNameAll, strFreq1, strFreq2 ;
					cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_freq1"), strFreq1) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_freq2"), strFreq2) ;

					strXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
					mapStrToStrXdata.SetAt(_T("ZY_LOC"), strXdata) ;

					strXdata.Format(_T("%dFS"), iBoxNum) ;
					mapStrToStrXdata.SetAt(_T("FS_INDEX"), strXdata) ;

					strXdata = strFreq1 ;
					if (!strFreq2.IsEmpty())
					{
						strXdata.AppendFormat(_T("/%s"), strFreq2) ;
					}
					mapStrToStrXdata.SetAt(_T("FREQ"), strXdata) ;

					strXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
					mapStrToStrXdata.SetAt(_T("DNUM"), strXdata) ;

					strXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
					mapStrToStrXdata.SetAt(_T("POWER_DNUM_KZ"), strXdata) ;
					strXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
					mapStrToStrXdata.SetAt(_T("POWER_DNUM_KF"), strXdata) ;
					strXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
					mapStrToStrXdata.SetAt(_T("POWER_RDNUM"), strXdata) ;
				}
				else
				{
					strErrMsg.Format(_T("FillXdataFS下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}
				cSet.Close() ;
			}

			//汇流排zloc
			if (m_bUseHLP==TRUE)
			{
				strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_hlp'")) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()==1)
					{
						cSet.MoveFirst() ;
						int iShlefNum = 0, iLayerNum = 0 ;

						cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShlefNum) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;

						strXdata.Format(_T("%s%d-%d"), m_strPreZ, iShlefNum, iLayerNum) ;
						mapStrToStrXdata.SetAt(_T("HLP_ZLOC"), strXdata) ;
					}
					cSet.Close() ;
				}

			}

		}
		// 		else if (iJinLuType==0) //for zj-ff编码
		// 		{
		// 
		// 		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataNAdd1")) ;
	}
}

// 将n+1上标示为tocon和tocon2的端子关联转为id
//tocon是串联一条线上一堆端子的都指定其tocon为其前端发送器端子，比如F2，即tocon为FS_NODE_F2
//tocon2是两个模块之间需要关联的端子（都在同一水平位置），比如左侧FBJ的12端，要连到右侧FBJ的11端
void CZnDmh::TransNAdd1SnToId(AcDbDatabase *& pDb)
{
	//先将各要关联的端子读入数据库，便于分类、排序
	CDBUtility::ClearTable(_T("dmh_tocon")) ;
	// 	CUserSystem mysys ; //在SetEntConnect函数中用到了g_cSystem,所以此处用mysys出错
	g_cSystem.m_pDb = pDb ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	CString strAppToCons[2] = {_T("TOCON"),  _T("TOCON2")} ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		// 		long nMaxId = g_cSystem.GetMaxId() ;
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				for (int i=0; i<2; i++)
				{
					CString strToCon ;
					if (g_cSystem.GetXdata(pEnt, strAppToCons[i], strToCon)==TRUE)
					{
						UINT nHandle = g_cSystem.GetHandle(pEnt) ;
						AcDbText *pText = AcDbText::cast(pEnt) ;
						CString strText = pText->textString() ;
						double dTextX = pText->position().x	;
						CString strMySn = _T("") ;
						if (i==1)
						{							
							g_cSystem.GetXdata(pEnt, _T("MY_SN"), strMySn) ;
						}
						CString strSql ;
						strSql.Format(_T("insert into dmh_tocon(dmh_tocon_tocon, dmh_tocon_handle, dmh_tocon_x, dmh_tocon_text, dmh_tocon_type, dmh_tocon_mysn) values('%s', %d, %.3f, '%s', %d, '%s')"), strToCon, nHandle, dTextX, strText, i, strMySn) ;
						ExecSqlString(strSql) ;					
					}
				}
			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	
	try
	{
		//取得当前图纸中最大ID
		long nMaxId = g_cSystem.GetMaxId() ;

		CString strSql ;
		CADORecordset cSet ;

		//先给各block排个序，FS/XJM/SJM.... 1/2/3
		strSql.Format(_T(" SELECT DISTINCT left(dmh_tocon_mysn, instr(dmh_tocon_mysn,'_')-1) as jinluname from dmh_tocon where dmh_tocon_type=1 and dmh_tocon_mysn<>'' ")) ;
		CStringArray strArrayJinLuName ;
		int iJinLuCount = CDBUtility::GetRecordValue(strSql, _T("jinluname"), strArrayJinLuName) ;
		CLongArray nArrayId ; 
		for (int iJinLuIndex=0; iJinLuIndex<iJinLuCount; iJinLuIndex++)
		{
			CString strJinLuName  = strArrayJinLuName.GetAt(iJinLuIndex) ;
			strSql.Format(_T("select top 1 * from dmh_tocon where left(dmh_tocon_mysn, len('%s'))='%s' "), strJinLuName, strJinLuName) ;
			long nId = 0 ;
			CDBUtility::GetRecordValue(strSql, _T("id"), nId) ;
			nArrayId.Add(nId) ;
		}
		CString strRetIds ; 
		CBlkUtility::ConvertArrayToString(nArrayId, _T(","),  strRetIds) ;
		strSql.Format(_T("	select * from dmh_tocon where id in(%s) order by dmh_tocon_x asc"), strRetIds) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				int iBlockIndex = 0 ;
				while (!cSet.IsEOF())
				{
					CString strMySn ;
					cSet.GetFieldValue(_T("dmh_tocon_mysn"), strMySn) ;
					int iMarkLoc = strMySn.Find(_T("_")) ;
					CString strJinLuName ;
					strJinLuName = strMySn.Left(iMarkLoc) ;
					strSql.Format(_T("update dmh_tocon set dmh_tocon_blockindex=%d where left(dmh_tocon_mysn,%d)='%s' "), iBlockIndex, iMarkLoc, strJinLuName) ;
					ExecSqlString(strSql) ;
					iBlockIndex++ ;
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
		
		//begin tocon
		strSql.Format(_T("SELECT DISTINCT dmh_tocon_tocon FROM dmh_tocon where dmh_tocon_type=0")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					CString strToCon ;
					cSet.GetFieldValue(_T("dmh_tocon_tocon"), strToCon) ;

					CADORecordset cSetOneToCon ;
					strSql.Format(_T("select * from dmh_tocon where dmh_tocon_tocon='%s' order by dmh_tocon_x asc"), strToCon) ;
					if (cSetOneToCon.IsOpen())
					{
						cSetOneToCon.Close() ;
					}
					if (cSetOneToCon.Open(g_PtrCon, strSql))
					{
						if (cSetOneToCon.GetRecordCount()>0)
						{				
							cSetOneToCon.MoveFirst() ;

							while (!cSetOneToCon.IsEOF())
							{
								int nHandle1, nHandle2 ;
								cSetOneToCon.GetFieldValue(_T("dmh_tocon_handle"), nHandle1) ;

								cSetOneToCon.MoveNext() ;
								if (!cSetOneToCon.IsEOF())
								{								
									cSetOneToCon.GetFieldValue(_T("dmh_tocon_handle"), nHandle2) ;
									AcDbEntity *pEnt1 = NULL, *pEnt2 = NULL ;
									g_cSystem.OpenAcDbEntity(nHandle1, pEnt1) ;
									g_cSystem.OpenAcDbEntity(nHandle2, pEnt2) ;

									//添加ID
									int nFirst_Id = 0 , nSecond_Id = 0 ;
									//本端子
									if(!g_cSystem.GetXdata(pEnt1 , _T("ID") , nFirst_Id))
									{
										nFirst_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt1 , _T("ID") , nFirst_Id) ;
									}
									//连接到的端子
									if(!g_cSystem.GetXdata(pEnt2 , _T("ID") , nSecond_Id))
									{
										nSecond_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt2 , _T("ID") , nSecond_Id) ;
									}
									//连接电缆类型（若有）
									long nCon_Cable_Id = 0 ;

									//关联端子
									if(SetEntConnect(pEnt1, nSecond_Id, nCon_Cable_Id,1))
									{
										SetEntConnect(pEnt2, nFirst_Id, nCon_Cable_Id,1) ;
									}

									if(pEnt1 != NULL)
									{
										pEnt1->close() ;
									}
									if(pEnt2 != NULL)
									{
										pEnt2->close() ;
									}
								}
								else
								{
									break;
								}								
							}
						}
						cSetOneToCon.Close() ;
					}


					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		//end tocon

		//begin tocon2
		strSql.Format(_T("SELECT DISTINCT right(dmh_tocon_tocon, len(dmh_tocon_tocon)-instr(dmh_tocon_tocon,'_')) as tocon from dmh_tocon where dmh_tocon_type=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					CString strToCon ;
					cSet.GetFieldValue(_T("tocon"), strToCon) ;

					CADORecordset cSetOneToCon ;
					strSql.Format(_T("select * from dmh_tocon where right(dmh_tocon_tocon, len('%s'))='%s' and dmh_tocon_type=1 order by dmh_tocon_x asc"), strToCon, strToCon) ;
					if (cSetOneToCon.IsOpen())
					{
						cSetOneToCon.Close() ;
					}
					if (cSetOneToCon.Open(g_PtrCon, strSql))
					{
						if (cSetOneToCon.GetRecordCount()>0)
						{				
							cSetOneToCon.MoveFirst() ;
							while (!cSetOneToCon.IsEOF())
							{
								CString strToConFullText1 ;
								cSetOneToCon.GetFieldValue(_T("dmh_tocon_tocon"), strToConFullText1) ;
								CString strToConLeftOrRight1 ;
								int iLocFind = strToConFullText1.Find(_T("_")) ;
								if (iLocFind!=-1)
								{
									strToConLeftOrRight1 = strToConFullText1.Left(iLocFind) ;
								}
								if (strToConLeftOrRight1==_T("LEFT"))
								{
									cSetOneToCon.MoveNext() ;
									continue;
								}
								int iBlockIndex1 = 0 ;
								cSetOneToCon.GetFieldValue(_T("dmh_tocon_blockindex"), iBlockIndex1) ;

								int nHandle1, nHandle2 ;
								cSetOneToCon.GetFieldValue(_T("dmh_tocon_handle"), nHandle1) ;

								cSetOneToCon.MoveNext() ;
								if (!cSetOneToCon.IsEOF())
								{	
									CString strToConFullText2 ;
									cSetOneToCon.GetFieldValue(_T("dmh_tocon_tocon"), strToConFullText2) ;
									CString strToConLeftOrRight2 ;
									int iLocFind2 = strToConFullText2.Find(_T("_")) ;
									if (iLocFind2!=-1)
									{
										strToConLeftOrRight2 = strToConFullText2.Left(iLocFind2) ;
									}

									int iBlockIndex2 = 0 ;
									cSetOneToCon.GetFieldValue(_T("dmh_tocon_blockindex"), iBlockIndex2) ;
									if(! (iBlockIndex2==(iBlockIndex1+1) && strToConLeftOrRight2==_T("LEFT")))
									{
										//cSetOneToCon.MoveNext() ;
										continue ;
									}

									cSetOneToCon.GetFieldValue(_T("dmh_tocon_handle"), nHandle2) ;
									AcDbEntity *pEnt1 = NULL, *pEnt2 = NULL ;
									g_cSystem.OpenAcDbEntity(nHandle1, pEnt1) ;
									g_cSystem.OpenAcDbEntity(nHandle2, pEnt2) ;

									//添加ID
									long nFirst_Id = 0 , nSecond_Id = 0 ;
									//本端子
									if(!g_cSystem.GetXdata(pEnt1 , _T("ID") , nFirst_Id))
									{
										nFirst_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt1 , _T("ID") , nFirst_Id) ;
									}
									//连接到的端子
									if(!g_cSystem.GetXdata(pEnt2 , _T("ID") , nSecond_Id))
									{
										nSecond_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt2 , _T("ID") , nSecond_Id) ;
									}
									//连接电缆类型（若有）
									long nCon_Cable_Id = 0 ;

									//关联端子
									if(SetEntConnect(pEnt1, nSecond_Id, nCon_Cable_Id,1))
									{
										SetEntConnect(pEnt2, nFirst_Id, nCon_Cable_Id,1) ;
									}

									if(pEnt1 != NULL)
									{
										pEnt1->close() ;
									}
									if(pEnt2 != NULL)
									{
										pEnt2->close() ;
									}

								}
								else
								{
									break;
								}								
							}
						}
						cSetOneToCon.Close() ;
					}


					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		//end tocon2
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in TransNAdd1SnToId")) ;
	}
	
}

BOOL CZnDmh::IsColInRange(const CString & strColIndex, const CString & strStrarCol, int iMergeCols)
{
	BOOL bRet = TRUE ;
	CString strEndCol = strStrarCol;
	for (int i=0; i<iMergeCols; i++)
	{
		IncreaseCol(strEndCol) ;
	}
	int iColIndexLen = strColIndex.GetLength() ;
	int iColCurLen = strEndCol.GetLength() ;
	if (iColIndexLen>iColCurLen)
	{
		bRet = FALSE ;
	}
	else if (iColIndexLen==iColCurLen)
	{
		bRet = strColIndex<strEndCol ;
	}

	return bRet;
}


//仅支持处理A~ZZ之间的列(足矣)
void CZnDmh::IncreaseCol(CString & strColIndex)
{
	int iColIndexLength = strColIndex.GetLength() ;
	CString strColIndexTmp ;

	TCHAR chLast = strColIndex.GetAt(iColIndexLength-1) ;
	if (chLast!=_T('Z'))
	{
		strColIndexTmp.Format(_T("%s%c"),strColIndex.Left(iColIndexLength-1), chLast+1) ;
	}
	else
	{
		if (iColIndexLength==1)
		{
			strColIndexTmp.Format(_T("AA")) ;
		}
		else
		{
			strColIndexTmp.Format(_T("%cA"), strColIndex.GetAt(0)+1) ;
		}
	}

	strColIndex = strColIndexTmp ;

}
//仅支持处理A~ZZ之间的列(足矣)
void CZnDmh::IncreaseCol(CString & strColIndex, int iIncrease)
{
	for (int i=0; i<iIncrease; i++)
	{
		IncreaseCol(strColIndex) ;
	}
}

// 给strFxpFor变量分配分线盘端子
int CZnDmh::GetFxpDz(CString &strFxpFor)
{
	if (m_strFxpCurDz==_T(""))
	{
		m_strFxpCurDz = _T("F1-101-1") ;
		AfxMessageBox(_T("起始端子为空，默认起始端子从F1-101-1开始计算")) ;
	}
	strFxpFor = m_strFxpCurDz ;

	//更新m_strFxpCurDz
	//F1-908-5,此处，F1所带数字从1~n，9处数字从1~10，但第10层不用，08处数字从1~13,5处数字从1~6，下面分别用i4~i1表示
	int i1 = 0, i2 = 0, i3 = 0, i4 = 0 ;
	CStringArray strArrayRet ;
	CBlkUtility::DivideString(m_strFxpCurDz, _T("-"), strArrayRet) ;
	if (strArrayRet.GetCount()==3)
	{
		CString str1, str2, str3 ;
		str1 = strArrayRet.GetAt(2) ;
		str2 = strArrayRet.GetAt(1) ;
		str3 = strArrayRet.GetAt(0) ;

		i1 = _tstoi(str1) ;
		i2 = _tstoi(str2.Right(2)) ;
		i3 = _tstoi(str2.Left(str2.GetLength()-2)) ;
		i4 = _tstoi(str3.Right(str3.GetLength()-1)) ;

		i1+=1 ;
		if (i1<7)
		{
			m_strFxpCurDz.Format(_T("F%d-%d%s%d-%d"), i4, i3, i2<10?_T("0"):_T(""), i2, i1) ;
		}
		else
		{
			i1 = 1 ;
			i2+=1 ;
			if (i2<14)
			{
				m_strFxpCurDz.Format(_T("F%d-%d%s%d-%d"), i4, i3, i2<10?_T("0"):_T(""), i2, i1) ;
			}
			else
			{
				i2 = 1 ;
				i3+=1 ;
				if (i3<10)
				{
					m_strFxpCurDz.Format(_T("F%d-%d%s%d-%d"), i4, i3, i2<10?_T("0"):_T(""), i2, i1) ;
				}
				else
				{
					i3 = 1 ;
					i4+=1 ;
					m_strFxpCurDz.Format(_T("F%d-%d%s%d-%d"), i4, i3, i2<10?_T("0"):_T(""), i2, i1) ;
				}
			}
		}
	}

	return 0;
}

// 填充零散电路数据-FBJ
void CZnDmh::FillXdataLingSan_FBJ(int iJinLuType, const CString &strJinLuName, CMapStringToString & mapStrToStrXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zcell where (dmh_in_zcell_cellcontent like '%%%s' or dmh_in_zcell_cellcontent like '%s%%') and dmh_in_zcell_jdqname='FBJ'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strFBJLoc ;
				strFBJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FBJ_ZLOC"), strFBJLoc) ;
				mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strCellContent) ;

				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='FBJ' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("FBJ_NODE_%d"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}

					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("FillXdataLingSan_FBJ下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataLingSan_FBJ")) ;
	}
}

// 填充零散电路数据-XHFS
void CZnDmh::FillXdataLingSan_XHFS(const CString &strCZ_X, const CString &strCZ_S, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata)
{
	CString strMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;

	CString strKey ;
	CString strXdata ;

	mapStrToStrXdata.SetAt(_T("CZ_XHJ_X"), strCZ_X) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ_S"), strCZ_S) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;		

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='LXJF' and dmh_in_zcell_cellcontent in('%s', '%s') and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_xhfs')"), strCZ_X, strCZ_S) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==2)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
					CString	strCellContent ;
					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;

					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					CString strFindVal ; //查找是否已有XHFS_ZLoc,若有，则和现在的对比，看是否一致（防止X3 S3 不在同一个组合位置）
					if(mapStrToStrXdata.Lookup(_T("XHFS_ZLOC"), strFindVal)==TRUE)
					{
						if (strFindVal!=strZLoc)
						{
							strMsg.Format(_T("注意:%s和%s不在同一层XHFS组合位置"), strCZ_X, strCZ_S) ;
							AfxMessageBox(strMsg) ;
						}
					}
					else
					{
						mapStrToStrXdata.SetAt(_T("XHFS_ZLOC"), strZLoc) ;
					}

					strKey.Format(_T("XHFSJDQINDEX_%s"), strCellContent==strCZ_X?_T("X"):_T("S")) ;
					strXdata.Format(_T("%d"), iBoxNum) ;
					mapToReplaceXdata.SetAt(strKey, strXdata) ;

					CADORecordset cSetJdqNode ;
					strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='XHFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
					if (cSetJdqNode.IsOpen())
					{
						cSetJdqNode.Close() ;
					}
					if (cSetJdqNode.Open(g_PtrCon, strSql))
					{
						if (cSetJdqNode.GetRecordCount()>0)
						{
							cSetJdqNode.MoveFirst() ;

							while (!cSetJdqNode.IsEOF())
							{
								int iJdqNode = 0 ;
								CString strCon ;
								cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
								cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

								CString strTypeName ;
								strTypeName.Format(_T("XHFS_%dXJF_%d"), iBoxNum, iJdqNode) ;
								mapStrToStrXdata.SetAt(strTypeName, strCon) ;

								cSetJdqNode.MoveNext() ;
							}
						}
						cSetJdqNode.Close() ;
					}

					cSet.MoveNext() ;
				}
			}
			else
			{
				acutPrintf(_T("\n%s执行结果不为2:%d"), strSql, iRsCount) ;
			}

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
		AfxMessageBox(_T("unknown error in FillXdataLingSan_XHFS")) ;
	}
}

// 填充零散电路-GDFS
//若股道分割了，则零散电路需要变化
void CZnDmh::FillXdataLingSan_GDFS(const CString &strGuiDao, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata)
{
	mapStrToStrXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;

	CString strKey ;
	CString strXdata ;

	CString strGuiDaoShow = strGuiDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuiDaoShow) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		CString strGuiDaoTmp = strGuiDao ;
		strGuiDaoTmp.TrimRight(_T("+")) ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='GJF1' and dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_gdfs')"), strGuiDaoTmp) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GDFS_ZLOC"), strZLoc) ;

				strXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX"), strXdata) ;

				CADORecordset cSetJdqNode ;
				strSql.Format(_T("select * from dmh_def_zhnb where dmh_def_zhnb_zhname='GDFS' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSetJdqNode.IsOpen())
				{
					cSetJdqNode.Close() ;
				}
				if (cSetJdqNode.Open(g_PtrCon, strSql))
				{
					if (cSetJdqNode.GetRecordCount()>0)
					{
						cSetJdqNode.MoveFirst() ;

						while (!cSetJdqNode.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("GDFS_%dGJF1_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSetJdqNode.MoveNext() ;
						}
					}
					cSetJdqNode.Close() ;
				}
			}
			else
			{
				acutPrintf(_T("\n%s执行结果不为1:%d"), strSql, iRsCount) ;

			}

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
		AfxMessageBox(_T("unknown error in FillXdataLingSan_GDFS")) ;
	}
}