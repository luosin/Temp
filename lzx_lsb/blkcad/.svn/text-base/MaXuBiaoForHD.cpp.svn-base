// MaXuBiaoForJH.cpp: implementation of the CMaXuBiaoForHD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MaXuBiaoForHD.h"

#include "DBUtility.h"
#include "EditFreqDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString g_strMaXu_1[] = {"占用","HU","U","LU","L","L2","L3","L4","L5"} ;
CString g_strLinZhanState_1 [] = {"L5","L4","L3","L2","L","LU","U","HU","占用"} ;

CString g_strMaXu_CeXiang_1[] = {"UU","U2","LU","L","L2","L3","L4","L5"} ;
CString g_strMaXu_CeXiang2_1[] = {"UUS","U2S","LU","L","L2","L3","L4","L5"} ;
CString g_strMaXu_YinDao_1[] = {"HB","U","LU","L","L2","L3","L4","L5"} ;
CString g_strFreq_Down_1[] = {"1700-1", "2300-1", "1700-2", "2300-2"} ;
CString g_strFreq_Up_1[] = {"2000-1", "2600-1", "2000-2", "2600-2"} ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaXuBiaoForHD::CMaXuBiaoForHD()
{	
	m_iDwgIndex = 0 ;
	dTableStart_X = 0 ; //表格起始x
	dTableStart_Y = 0 ; //表格起始y	
	dTableCur_X = dTableStart_X ; //当前表格X
	dTableCur_Y	= dTableStart_Y ; //当前表格Y
    dTableCellHeight = 7 ; //表格单元格高度
	dTableCellWidth = 18 ;	//表格单元格宽度
	dBetweenTable_Y = 260 ;//表格间距（上下）360
	dDescribeCellWidth = 70.0000 ; //描述表格宽度
	textStyleId = NULL ;

	//当前进路的进出站及反向进出站信号机的id和名称,股道及类型
	m_strChuZhanXHJ_F = _T("") ;
	m_strChuZhanXHJ = _T("") ;
	m_strJinZhanXHJ_F = _T("") ;
	m_strJinZhanXHJ = _T("") ;
	m_iChuZhanXHJ_F_id = -1 ;
	m_iChuZhanXHJ_id = -1 ;
	m_iJinZhanXHJ_F_id = -1 ;
	m_iJinZhanXHJ_id = -1 ;	
	m_iGuDao_Type = 0 ;
	m_strGuDao = _T("") ;	
	
	//当前进路的方向：
	//0:从左往右
	//1:从右往左
	m_iCurJinLuDirect = 0 ;

	m_iZpGuiDaoSum_Rigth = 0 ;
	m_iZpGuiDaoSum_Left = 0 ;

	m_iLinZhanStateSum = 9 ;

	m_dFrameLeft_x = -105 ;

	m_strDwgSaveDir = _T("c:\\") ;
	m_strFileName = _T("erro") ;
	m_strStationName = _T("XXX") ;

	m_dSiYiTuHeight = 80 ;
	m_dBetweenTableAndBottom = 40 ; //old:47

	//0:温福；1:武广
	m_iVersionFor = 0 ; 

	m_dWidthGuiDao = 0.7;
	m_dWidthBianMa = 0.7;
	m_dHeightGuiDao = 4.0;
	m_dHeightBianMa = 4.0;
	
}

CMaXuBiaoForHD::~CMaXuBiaoForHD()
{
	this->ReleaseJLArray() ;
}

//将信号平面图读入数据库,轨道名称等采用文本，通过关联，xdata记录信息，此函数在早期使用，现废弃
void CMaXuBiaoForHD::ReadDwgToDb(LPUNKNOWN m_pConnection, AcDbDatabase * pCurDb)
{
	CString strSql ;

	_ConnectionPtr ptrCon; //与数据库的连接
	ptrCon = ( _ConnectionPtr ) m_pConnection ;

	CString strTable[2] = {_T("guidao"),_T("signal")} ;
	for(int iTb=0; iTb<2; iTb++)
	{
		strSql.Format(_T("delete * from %s"),strTable[iTb]) ;							
		ExecSqlString(strSql) ;
		//重置id
		strSql.Format(_T("alter table [%s] alter column id counter(1,1)"),strTable[iTb]) ;
		ExecSqlString(strSql) ;
	}
	
	AcDbBlockTable * pBlkTbl = NULL ;
	pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	
	AcDbEntity * pEnt = NULL ;
	CString strAppNamePre[3] = {_T("PRE1"),_T("PRE2"), _T("PRE3") } ;
	CString strAppNameNext[3] = {_T("NEXT1"),_T("NEXT2"), _T("NEXT3") } ;
	//CString strAppName
	
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbText * pText = NULL ;
			
			//处理文本 AcDbMText ;
			if (pEnt->isKindOf(AcDbText::desc()))			
			{
				CString strType ;
				CString strText ;	
				if (g_cSystem.GetXdata(pEnt,_T("TYPE"), strType))
				{
					if (strType==_T("GUIDAO"))
					{
						CString strPre[3] = {_T(""), _T(""), _T("")} ;
						CString strNext[3] = {_T(""), _T(""), _T("")} ;
						CString strXHJ , strXHJ2 ;
						int iGuidaoType = 0 ;
						int iUpOrDown = 0 ;
						int iTopOrBottom = 0 ;
						int iStartEnd = 0 ;
						CString strFreq = _T("") ;
						CString strTmp ;
						int iTmp = -1 ;

						for(int i=0; i<3; i++)
						{
							g_cSystem.GetXdata(pEnt,strAppNamePre[i], strPre[i]) ;
						}
						for(int i2=0; i2<3; i2++)
						{
							g_cSystem.GetXdata(pEnt,strAppNameNext[i2], strNext[i2]) ;
						}
						if (g_cSystem.GetXdata(pEnt,_T("XINHAOJI"),strTmp))
						{
							strXHJ = strTmp ;
						}
						if (g_cSystem.GetXdata(pEnt,_T("XINHAOJI2"),strTmp))
						{
							strXHJ2 = strTmp ;
						}
						if (g_cSystem.GetXdata(pEnt,_T("GUIDAOTYPE"),iTmp)) 
						{
							iGuidaoType = iTmp ;
						}
						if (g_cSystem.GetXdata(pEnt,_T("UPORDOWN"),iTmp)) 
						{
							iUpOrDown = iTmp ;
						}
						if (g_cSystem.GetXdata(pEnt,_T("TOPORBOTTOM"),iTmp)) 
						{
							iTopOrBottom = iTmp ;
						}
						
						if (g_cSystem.GetXdata(pEnt,_T("GUIDAOSTARTEND"),iTmp)) 
						{
							iStartEnd = iTmp ;
						}
						if (g_cSystem.GetXdata(pEnt,_T("FREQUENCE"),strTmp)) 
						{
							strFreq = strTmp ;
						}
						
						pText = (AcDbText *)pEnt ;
						strText = pText->textString() ;
						
						try
						{
							strSql.Format(_T("insert into guidao(name,pre1,pre2,pre3,next1,next2,next3,signal,signal2, type, upordown,startend,toporbottom,frequence) \
								values('%s','%s','%s','%s','%s','%s','%s','%s', '%s', %d,%d,%d,%d,'%s')"),\
								strText,strPre[0],strPre[1],strPre[2],strNext[0],strNext[1],strNext[2],strXHJ,strXHJ2,iGuidaoType, iUpOrDown,iStartEnd,iTopOrBottom,strFreq) ;
							ExecSqlString(strSql) ;
						}
						catch (_com_error& e)
						{
							CString sBuff = CBlkUtility::GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox(_T("unknown error in ReadDwgToDb")) ;
							
						}						
					}
					else if (strType==_T("XINHAOJI")) 
					{										
						pText = (AcDbText *)pEnt ;
						strText = pText->textString() ;

						CString strGuiDao(_T("")) ;
						int iXHJType = 0 ;
						int iXHJUpDown = 0 ;
						int iXHJDirect = 0 ;
						g_cSystem.GetXdata(pEnt,_T("GUIDAO"), strGuiDao) ;
						g_cSystem.GetXdata(pEnt,_T("XHJTYPE"), iXHJType) ;
						g_cSystem.GetXdata(pEnt,_T("UPORDOWN"), iXHJUpDown) ;
						g_cSystem.GetXdata(pEnt,_T("FANXIANG"),iXHJDirect) ;
						try
						{
							strSql.Format(_T("insert into signal(name,type, upordown,direct,guidao) \
								values('%s',%d,%d,%d,'%s')"),strText,iXHJType,iXHJUpDown,iXHJDirect,strGuiDao) ;
							ExecSqlString(strSql) ;				
						
						}
						catch (_com_error& e)
						{
							CString sBuff = CBlkUtility::GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox(_T("unknown error in ReadDwgToDb")) ;
							
						}			
						
					}
				}
				
			}
			pEnt->close() ;
		}
		
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

}
void CMaXuBiaoForHD::FindJinLu(CString strStart, CString strEnd, CStringArray &strArray)
{
	CStringArray strArray1 ;
	strArray1.Copy(strArray) ;
	
	strArray1.Add(strStart) ;
	if (strStart!=strEnd)
	{
		try
		{
			CADORecordset myset ;
			CString strSql ;
			strSql.Format(_T("select * from guidao where name='%s'"), strStart) ;
			myset.Open(g_PtrCon, strSql) ;
			
			if (!myset.IsBOF())
			{
				myset.MoveFirst() ;
			}
			while (!myset.IsEOF())
			{
				CString strNext1, strNext2, strNext3 ;
				myset.GetFieldValue(_T("next1"), strNext1) ;
				myset.GetFieldValue(_T("next2"), strNext2) ;
				myset.GetFieldValue(_T("next3"), strNext3) ;

				if (strNext1!=_T(""))
				{
					this->FindJinLu(strNext1, strEnd, strArray1) ;
				}
				if (strNext2!=_T("")) 
				{
					this->FindJinLu(strNext2, strEnd, strArray1) ;
				}
				if (strNext3!=_T("")) 
				{
					this->FindJinLu(strNext3, strEnd, strArray1) ;
				}
				
				myset.MoveNext() ;
			}
			myset.Close() ;			
		}
		catch (_com_error& e)
        {
			CString sBuff = CBlkUtility::GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in FindJinLu")) ;			
		}       
	}
	else//到达末端
	{
		if (strArray1.GetSize()>2)
		{
			CStringArray * strArrayTmp = new CStringArray ;
			strArrayTmp->Copy(strArray1) ;
			this->arrayJinLu.Add(strArrayTmp) ;
		}
	}
}

//对某一进路进行编码
//////////////////////////////////////////////////////////////////////////
BOOL CMaXuBiaoForHD::JinLuBianMa(CStringArray *strArrayJinLu)
{
	BOOL bRet = FALSE ;
	CString strSql ;
	CString strTempletFileName, strDwgFileName ;
	strTempletFileName.Format(_T("%s\\support\\maxubiao\\samp2.dwg"), m_strSysBasePath) ;	 
	//m_strDwgSaveDir.Format("E:\\file\\maxubiao") ;


	CString strMsg = _T("") ;
	
	strSql.Format(_T("delete * from jinlu")) ;
	ExecSqlString(strSql) ;
		
	//将此进路中所有轨道数据写入jinlu表
	for(int i=0; i<strArrayJinLu->GetSize(); i++)
	{
		//读入一个轨道数据到jinlu表，并填充其信息
		this->ReadOneGuiDaoToDb(strArrayJinLu->GetAt(i)) ;	
		strMsg.Format(_T("%s-%s"), strMsg, strArrayJinLu->GetAt(i)) ;
	}	
		 	
	//判断是哪种情况的进路，然后根据各个情况写码序表
	try
	{
		g_pCurDb = new AcDbDatabase ;
		g_cSystem.m_pDb = g_pCurDb ;
	
		//this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt.shx", 2, 0.7) ;	

		CADORecordset cSet ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen()) 
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon,strSql) ;

		int iFirstGuiDaoUpDown = -1 ;
		int iLastGuiDaoUpDown = -1 ;
		int iLastGuiDao_XHJType = -1 ;
		int iLastGuiDao_XHJUpDown = 0 ;
		int iLastGuiDao_XHJDirect = -1 ;
		BOOL bIsFirstGuiDaoLQ = FALSE ;
		BOOL bIsLastGuiDaoLQ = FALSE ;
		
		int iFirstGuiDao_XHJType = -1 ;
		int iFirstGuiDao_XHJUpDown = 0 ;
		int iFirstGuiDao_XHJDirect = -1 ;
		
		int iGuDaoType = 0 ;
		CString strFirstXHJ = _T("") ;
		CString strLastXHJ = _T("") ;

		//判断有无股道，确定是否中继站
		BOOL bFindGuDao = FALSE ;
		//判断在进站分界时：第一个股道的防护信号机类型，X(1) or XN(2)
		int _iFirstGuiDao_XHJ = -1 ;
		//判断判断在进站分界时：最后一个股道的防护信号机类型，S(1) or SN(2)
		int _iLastGuiDao_XHJ = -1 ;	
		
		if (cSet.GetRecordCount()>0)
		{
			//第一个轨道信息
			if (!cSet.IsBOF()) 
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("guidao_updown"),iFirstGuiDaoUpDown) ;
			cSet.GetFieldValue(_T("signal_type"),iFirstGuiDao_XHJType) ;
			cSet.GetFieldValue(_T("signal_updown"),iFirstGuiDao_XHJUpDown) ;
			cSet.GetFieldValue(_T("signal_direct"), iFirstGuiDao_XHJDirect) ;
			cSet.GetFieldValue(_T("signal"),strFirstXHJ) ;	
			//最后一个轨道信息
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("guidao_updown"),iLastGuiDaoUpDown) ;
			cSet.GetFieldValue(_T("signal_type"),iLastGuiDao_XHJType) ;
			cSet.GetFieldValue(_T("signal_updown"),iLastGuiDao_XHJUpDown) ;
			cSet.GetFieldValue(_T("signal_direct"), iLastGuiDao_XHJDirect) ;
			cSet.GetFieldValue(_T("signal"),strLastXHJ) ;

		}		
		else
		{
			AfxMessageBox(_T("进路数据为空")) ;
			return FALSE ;
		}
		
		//经过的股道信息：下行正线，上行正线，侧线
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		bFindGuDao = cSet.Find(_T("guidao_type>0")) ;
		if (bFindGuDao) 
		{
			cSet.GetFieldValue(_T("guidao_type"),iGuDaoType) ;
		}
		
		//判断在进站分界时：第一个股道的防护信号机类型，X(1) or XN(2)
		if (strFirstXHJ!=_T(""))
		{
			if (iFirstGuiDao_XHJType==1) 
			{
				if (iFirstGuiDao_XHJUpDown==1) 
				{
					if (iFirstGuiDao_XHJDirect==0)
					{
						_iFirstGuiDao_XHJ = 1 ;
					}
					else if(iFirstGuiDao_XHJDirect==1)
					{
						_iFirstGuiDao_XHJ = 2 ;
					}
				}
			}
		}
		//
		else
		{
			bIsFirstGuiDaoLQ = TRUE ;
		}
		//判断在进站分界时：最后一个股道的防护信号机类型，S(1) or SN(2)		
		if (strLastXHJ!=_T(""))
		{
			if (iLastGuiDao_XHJType==1) 
			{
				if (iLastGuiDao_XHJUpDown==2) 
				{
					if (iLastGuiDao_XHJDirect==0)
					{
						_iLastGuiDao_XHJ = 1 ;
					}
					else if(iLastGuiDao_XHJDirect==1)
					{
						_iLastGuiDao_XHJ = 2 ;
					}
				}
			}
		}
		else
		{
			bIsLastGuiDaoLQ = TRUE ;
		}

		this->GetJinLuGuiDaoFreq(*strArrayJinLu, m_strArrayCurJinLuFre) ;
		
		//左侧下行（第一个轨道是下行轨道或者防护信号机是X）		
		if (iFirstGuiDaoUpDown==1 || _iFirstGuiDao_XHJ==1)
		{
			//右侧下行（最后一个轨道是下行轨道，或者是其防护信号机是SN或者是x1lq）
			if (iLastGuiDaoUpDown==1 || _iLastGuiDao_XHJ==2 || bIsLastGuiDaoLQ == TRUE) 
			{
				//没有找到股道，是中继站,下行
				if (bFindGuDao==FALSE)
				{
					
					if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
					{		
						this->dTableCur_X = 0 ;
						this->JinLuBianMa_ZJZ_X() ;
						this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
						this->JinLuBianMa_ZJZ_X_F() ;
						this->dTableCur_Y = this->dTableStart_Y ;
						
					}
					strDwgFileName.Format(_T("%s\\%d_ZJZ_X.dwg"),m_strDwgSaveDir, this->m_iDwgIndex  ) ;

					g_pCurDb->saveAs(strDwgFileName) ;
					
					this->m_iDwgIndex++ ;
					
				}				
				//经过的股道是下行正线股道
				else if (iGuDaoType==1) 
				{					
					//中间没有分歧路线（链路中所有的轨道的下一个节点都是next1）
					//if (this->IsDirectJinLu())
					//{	
						
						if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
						{	
							this->dTableCur_X = 0 ;							
							this->JinLuBianMa_X_Z_X() ; //ok
							this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;							
							this->JinLuBianMa_X_Z_X_F() ;//上一种情况反向
							this->dTableCur_Y = this->dTableStart_Y ;

						}
						strDwgFileName.Format(_T("%s\\%d_%s.dwg"), m_strDwgSaveDir, this->m_iDwgIndex, m_strFileName) ;

						g_pCurDb->saveAs(strDwgFileName) ;

						this->m_iDwgIndex++ ;

						strMsg.Format(_T("%s\nX_Z_X"), strMsg) ;
					//}
					//中间有分歧路线，比如走八字					
					/*else
					{	

						this->JinLuBianMa_X_Z_X() ;
						this->JinLuBianMa_X_Z_X_F() ;
						strMsg.Format(_T("%s\nX_Z_X2"), strMsg) ;
					}
					*/
				}
				//经过的股道不是下行正线股道				
				else 
				{					
					
					if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
					{	
						this->dTableCur_X = 0 ;						
						this->JinLuBianMa_X_C_X() ; //ok
						this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
						this->JinLuBianMa_X_C_X_F() ;//上一种情况反向
						this->dTableCur_Y = this->dTableStart_Y ;						
					}
					strDwgFileName.Format(_T("%s\\%d_%s.dwg"), m_strDwgSaveDir,this->m_iDwgIndex, m_strFileName) ;

					g_pCurDb->saveAs(strDwgFileName) ;
					this->m_iDwgIndex++ ;
					
					strMsg.Format(_T("%s\nX_c_X"), strMsg) ;
				}
			}
			//右侧上行（最后一个轨道是上行轨道，或者是其防护信号机是S）
			else if (iLastGuiDaoUpDown==2 || _iLastGuiDao_XHJ==1 ) 
			{				
				
				if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
				{
					this->dTableCur_X = 0 ;
					this->JinLuBianMa_X_ZC_S() ; //ok
					this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
					this->JinLuBianMa_X_ZC_S_F() ;//上一种情况反向
					this->dTableCur_Y = this->dTableStart_Y ;					
				}
				strDwgFileName.Format(_T("%s\\%d_%s.dwg"), m_strDwgSaveDir, this->m_iDwgIndex, m_strFileName) ;

				g_pCurDb->saveAs(strDwgFileName) ;

				this->m_iDwgIndex++ ;				
				
				strMsg.Format(_T("%s\nX_Zc_s"), strMsg) ;
			}
		}
		//左侧上行（第一个轨道是上行轨道或者防护信号机是XN 或者s1lq）		
		else if (iFirstGuiDaoUpDown==2 || _iFirstGuiDao_XHJ==2 || bIsFirstGuiDaoLQ == TRUE)
		{
			//右侧上行（最后一个轨道是上行轨道或者其防护信号机是S）
			if (iLastGuiDaoUpDown==2 || _iLastGuiDao_XHJ==1)
			{
				//没有找到股道，是中继站,下行
				if (bFindGuDao==FALSE)
				{	
					
					if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
					{	
						this->dTableCur_X = 0 ;
						this->JinLuBianMa_ZJZ_S() ;
						this->dTableCur_X = dDescribeCellWidth - dTableCellWidth ;
						this->JinLuBianMa_ZJZ_S_F() ;
						this->dTableCur_Y = this->dTableStart_Y ;							
					}
					strDwgFileName.Format(_T("%s\\%d_ZJZ_S.dwg"), m_strDwgSaveDir, this->m_iDwgIndex) ;

					g_pCurDb->saveAs(strDwgFileName) ;
					
					this->m_iDwgIndex++ ;						
					
				}				
				//经过正线股道（经过的股道是上行正线股道）
				else if (iGuDaoType==2)
				{
					//中间没有分歧路线（链路中所有的轨道的下一个节点都是next1）
					//if (this->IsDirectJinLu())
					//{						
						
						if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
						{	
							this->dTableCur_X = 0 ;
							this->JinLuBianMa_S_Z_S() ; //ok
							this->dTableCur_X = dDescribeCellWidth - dTableCellWidth ;
							this->JinLuBianMa_S_Z_S_F() ;//上一种情况反向
							this->dTableCur_Y = this->dTableStart_Y ;							
						}
						strDwgFileName.Format(_T("%s\\%d_%s.dwg"), m_strDwgSaveDir, this->m_iDwgIndex, m_strFileName) ;

						g_pCurDb->saveAs(strDwgFileName) ;

						this->m_iDwgIndex++ ;						
						
						strMsg.Format(_T("%s\ns_Z_s"), strMsg) ;
					//}
					//中间走过八字路线（链路中不是所有的轨道的下一个节点都是next1）
				/*	else 
					{
						JinLuBianMa_S_Z_S() ;
						this->JinLuBianMa_S_Z_S_F() ;
						strMsg.Format(_T("%s\ns_Z_s2"), strMsg) ;
					} */
				}
				//经过的股道不是下行正线股道				
				else 
				{
					
					if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
					{						
						this->dTableCur_X = 0 ;
						this->JinLuBianMa_S_C_S() ; //ok
						this->dTableCur_X = dDescribeCellWidth - dTableCellWidth ;
						this->JinLuBianMa_S_C_S_F() ;//上一种情况反向
						this->dTableCur_Y = this->dTableStart_Y ;						
					}
					strDwgFileName.Format(_T("%s\\%d_%s.dwg"), m_strDwgSaveDir, this->m_iDwgIndex, m_strFileName) ;
					g_pCurDb->saveAs(strDwgFileName) ;
					this->m_iDwgIndex++ ;					
					strMsg.Format(_T("%s\ns_c_s"), strMsg) ;
				}
			}
			//右侧下行（最后一个轨道是下行轨道，或者是其防护信号机是SN）
			else if (iLastGuiDaoUpDown==1 || _iLastGuiDao_XHJ==2)
			{	
				
				if (Acad::eOk== g_pCurDb->readDwgFile(strTempletFileName))
				{
					this->dTableCur_X = 0 ;
					this->JinLuBianMa_S_ZC_X() ; //ok
					this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
					this->JinLuBianMa_S_ZC_X_F() ;//上一种情况反向		
					
					this->dTableCur_Y = this->dTableStart_Y ;					
				}
				strDwgFileName.Format(_T("%s\\%d_%s.dwg"), m_strDwgSaveDir, this->m_iDwgIndex, m_strFileName) ;

				g_pCurDb->saveAs(strDwgFileName) ;

				this->m_iDwgIndex++ ;				
				
				strMsg.Format(_T("%s\ns_Zc_X"), strMsg) ;
			}
		}

		cSet.Close() ;

		delete g_pCurDb ;
		g_pCurDb = NULL ;

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa")) ;
	} 			
	
	//AfxMessageBox(strMsg) ;
	
	return bRet ;

}

void CMaXuBiaoForHD::ReadOneGuiDaoToDb(CString strGuiDao)
{
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		
		CString strpre1, strpre2, strpre3, strnext1, strnext2, strnext3 ;
		
		CString strSignal, strSignal2 ;				
		int iGuiDaoType = 0 ;				
		int iGuiDaoUpDown = 0 ;	
		int iGuiDaoTopBottom = 0 ;
		int iXHJType = 0, iXHJType2 = 0 ;
		int iXHJUpDown = 0, iXHJUpDown2 = 0 ;
		int iXHJDirect = 0 ;				
		
		strSql.Format(_T("select * from guidao where name='%s'"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon,strSql) ;
		int iRecNum = cSet.GetRecordCount() ;
		if (iRecNum>0)
		{
			if (iRecNum>1)
			{
				CString strMsg ;
				strMsg.Format(_T("轨道%s有不止1条记录"),strGuiDao) ;
				AfxMessageBox(strMsg) ;
			}
			cSet.MoveFirst() ;
			cSet.GetFieldValue(_T("pre1"), strpre1) ;
			cSet.GetFieldValue(_T("pre2"), strpre2) ;
			cSet.GetFieldValue(_T("pre3"), strpre3) ;
			cSet.GetFieldValue(_T("next1"), strnext1) ;
			cSet.GetFieldValue(_T("next2"), strnext2) ;
			cSet.GetFieldValue(_T("next3"), strnext3) ;
			cSet.GetFieldValue(_T("signal"), strSignal) ;
			cSet.GetFieldValue(_T("signal2"), strSignal2) ;
			cSet.GetFieldValue(_T("type"),iGuiDaoType) ;
			cSet.GetFieldValue(_T("upordown"),iGuiDaoUpDown) ;
			cSet.GetFieldValue(_T("toporbottom"), iGuiDaoTopBottom) ;
			
			if (strSignal!=_T(""))
			{
				CADORecordset cSet2 ;
				strSql.Format(_T("select * from signal where name='%s'"),strSignal ) ;
				if (cSet2.IsOpen())
				{
					cSet2.Close() ;
				}
				cSet2.Open(g_PtrCon, strSql) ;
				if (cSet2.GetRecordCount()>0) 
				{
					if (cSet2.GetRecordCount()>1)
					{
						CString strMsg ;
						strMsg.Format(_T("信号机%s有不止1条记录"),strSignal) ;
						AfxMessageBox(strMsg) ;							
					}
					cSet2.MoveFirst() ;
					cSet2.GetFieldValue(_T("type"), iXHJType) ;
					cSet2.GetFieldValue(_T("upordown"), iXHJUpDown) ;
					cSet2.GetFieldValue(_T("direct"), iXHJDirect) ;
					
				}
				cSet2.Close() ;
			}
			if (strSignal2!=_T(""))
			{
				CADORecordset cSet3 ;
				strSql.Format(_T("select * from signal where name='%s'"),strSignal2 ) ;
				if (cSet3.IsOpen())
				{
					cSet3.Close() ;
				}
				cSet3.Open(g_PtrCon, strSql) ;
				if (cSet3.GetRecordCount()>0) 
				{
					if (cSet3.GetRecordCount()>1)
					{
						CString strMsg ;
						strMsg.Format(_T("信号机%s有不止1条记录"),strSignal2) ;
						AfxMessageBox(strMsg) ;							
					}
					cSet3.MoveFirst() ;
					cSet3.GetFieldValue(_T("type"), iXHJType2) ;
					cSet3.GetFieldValue(_T("upordown"), iXHJUpDown2) ;
					
				}
				cSet3.Close() ;
			}
			
			strSql.Format(_T("insert into jinlu(guidao,pre1,pre2,pre3,next1,next2,next3,\
							signal,signal2,guidao_topbottom,guidao_type,guidao_updown,signal_type,signal_updown, signal_type2,signal_updown2,signal_direct) \
							values('%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%d)"),\
							strGuiDao,strpre1,strpre2,strpre3,strnext1,strnext2,strnext3,strSignal,strSignal2,\
							iGuiDaoTopBottom,iGuiDaoType,iGuiDaoUpDown,iXHJType,iXHJUpDown,iXHJType2,iXHJUpDown2,iXHJDirect ) ;
			ExecSqlString(strSql) ;
			
		}
		cSet.Close() ;	
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error ReadOneGuiDaoToDb ")) ;
	} 			
				
}

BOOL CMaXuBiaoForHD::IsDirectJinLu()
{
	BOOL bRet = TRUE ;
	try	
	{
		CADORecordset cSet ;
		CString strSql ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF()) 
		{
			CString strNext1 ;
			cSet.GetFieldValue(_T("next1"), strNext1) ;
			cSet.MoveNext() ;
			if (!cSet.IsEOF())
			{
				CString strGuiDao ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (strNext1!=strGuiDao)
				{
					bRet = FALSE ;
					break ;
				}
			}
		}
		cSet.Close() ;

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in IsDirectJinLu")) ;
	} 	
	
	return bRet ;

}

void CMaXuBiaoForHD::JinLuBianMa_X_Z_X()
{	
	this->m_iCurJinLuDirect = 0 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机
	BOOL bPassXN = FALSE ; //是否越过进站信号机
	int iCodeTypeTmp = 0 ; //编码的种类 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;
		
		//找出下进站,下出站
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;
		this->m_strFileName.Format(_T("%s_%s_%s"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F) ;

		//end 找出下进站,下出站		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//临站是_T("L5,L4,...HU,占用")的9种情况
		//////////////////////////////////////////////////////////////////////////		
		//CString strStartCodeTmp[9] = {_T("L5","L4","L3","L2","L","LU","U","HU","占用")} ;
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s通过)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ, this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}			
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;

			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;

			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;

				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;
					}					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoId<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
				}
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp) ;
				
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
							
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
		
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		//strSql.Format(_T("select * from jinlu where guidao_updown=1")) ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述		
			if (iGuiDaoId>this->m_iChuZhanXHJ_id)
			{
				strDescribeTmp.Format(_T("%s占用(%s开放%s通过)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao) ;
			}
			else
			{
				strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
				
			}
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;
					}					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
				}
				this->GetGuiDaoBianMa2(strGuiDaoName,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp) ;

				if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
				{
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, _T("占用")) ;
				}

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////				

		//X开放直向，XI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		//情况描述:X开放直向，XI关闭
		strDescribeTmp.Format(_T("%s开放%s,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}

		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN = TRUE ;
				}
			}
			
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	
		
		if(m_iVersionFor == 1)
		{
			//情况描述:X开放直向至XI，XI开放引导
			
			strDescribeTmp.Format(_T("%s开放直向至%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN = TRUE ;
					}
				}
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
				cSet.MovePrevious() ;
				
				// 			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				// 			this->GetGuiDaoBianMa(strGuiDao, strArrayBianMaTmp, strRetBianMa, 2) ;
				// 			cSet.MovePrevious() ;			
			}
			CStringArray * pstrArrayTmpXIYinDao = new CStringArray() ;
			pstrArrayTmpXIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpXIYinDao) ;
		}
		
		cSet.Close() ;	

		//end X开放直向，XI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
		//X开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		//侧向		
		strDescribeTmp.Format(_T("%s开放侧向，%s关闭"), this->m_strJinZhanXHJ,this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pstrArrayTmpX_C = new CStringArray() ;
		pstrArrayTmpX_C->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpX_C) ;	

		//关闭
		strDescribeTmp.Format(_T("%s进站关闭"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pstrArrayTmpX_CLOSE = new CStringArray() ;
		pstrArrayTmpX_CLOSE->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpX_CLOSE) ;	

		//X开放引导
		strDescribeTmp.Format(_T("%s开放引导"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 2) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pstrArrayTmpX_Y = new CStringArray() ;
		pstrArrayTmpX_Y->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpX_Y) ;	
		
		cSet.Close() ;	
		
		//end X开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_z_x"),sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error x_z_x")) ;
	} 

	//绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2) ;
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}
}


void CMaXuBiaoForHD::DoALL()
{
	//this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt.shx", 2, 0.7) ;		
	
	//g_cSystem.m_pDb = acdbHostApplicationServices()->workingDatabase() ;

	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径
	this->ReleaseJLArray() ;
	this->m_iDwgIndex = 0 ;
		
	try
	{
		//查询数据库，找出起始和末端轨道
		CStringArray strArrayFirstGuiDao, strArrayLastGuiDao ;

		CADORecordset cSet ;
		CString strSql ;
		//前方没有轨道即认为是开始轨道，不用再手工指定起始轨道
		strSql.Format(_T("select * from guidao where pre1='' or pre1='start' ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF()) 
		{
			CString strFirstGuiDao ;
			cSet.GetFieldValue(_T("name"), strFirstGuiDao) ;

			ads_printf(_T("\n起始轨道：%s\n"),strFirstGuiDao) ;			
			strArrayFirstGuiDao.Add(strFirstGuiDao) ;
			cSet.MoveNext() ;
		}
		cSet.Close() ;
		
		//找出末端轨道
		strSql.Format(_T("select * from guidao where next1='' or next1='end' ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF()) 
		{
			CString strLastGuiDao ;
			cSet.GetFieldValue(_T("name"), strLastGuiDao) ;
			ads_printf(_T("\n末端轨道：%s\n"),strLastGuiDao) ;				
			strArrayLastGuiDao.Add(strLastGuiDao) ;
			cSet.MoveNext() ;
		}
		cSet.Close() ;

		int iFirstGuiDaoSum = strArrayFirstGuiDao.GetSize() ;
		int iLastGuiDaoSum = strArrayLastGuiDao.GetSize() ;
		ads_printf(_T("\n开始搜索进路：......\n")) ;
		//acedSetStatusBarProgressMeter(_T("搜索进路"), 0, iFirstGuiDaoSum*iLastGuiDaoSum-1) ;
		
		for(int iFirstGuiDaoNum=0; iFirstGuiDaoNum<iFirstGuiDaoSum; iFirstGuiDaoNum++)
		{
			for(int iLastGuiDaoNum=0; iLastGuiDaoNum<iLastGuiDaoSum; iLastGuiDaoNum++)
			{
				//acedSetStatusBarProgressMeterPos(iFirstGuiDaoNum*iLastGuiDaoSum+iLastGuiDaoNum) ;				
				CStringArray strArrayTmp ;
				CString strPassGuiDao = _T("") ;
				//test
				//if (iFirstGuiDaoNum == 1 && iLastGuiDaoNum == 0)
				//{
					this->FindJinLu(strArrayFirstGuiDao.GetAt(iFirstGuiDaoNum),strArrayLastGuiDao.GetAt(iLastGuiDaoNum), strPassGuiDao, strArrayTmp) ;
					//int iii = 0 ;
				//}
				//end test
			}
		}
		//acedRestoreStatusBar() ;		
		ads_printf(_T("\n搜索进路结束：......\n")) ;		

		int iJinLuNum = this->arrayJinLu.GetSize() ;
		ads_printf(_T("\n进路数量：%d\n"), iJinLuNum) ;
		ads_printf(_T("\n开始进路编码：......\n")) ;
		//acedSetStatusBarProgressMeter(_T("进路编码"), 0, iJinLuNum-1) ;
		
		this->GetFrontAndBackStation(this->m_strStationName, m_strFrontStation, m_strBackStation) ;

		CString strBarMsg ;
		strBarMsg.Format(_T("%d条进路编码"), iJinLuNum) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iJinLuNum-1) ;	
		for(int iJinLuIndex=0; iJinLuIndex<iJinLuNum; iJinLuIndex++)
		{
			acedSetStatusBarProgressMeterPos(iJinLuIndex) ;
			CStringArray * pArrayJinLu = arrayJinLu.GetAt(iJinLuIndex) ;
			ads_printf(_T("\nBegin进路编码-%d:"),iJinLuIndex) ;
//    			if (iJinLuIndex==35)
 			{
				this->JinLuBianMa(pArrayJinLu) ;
											
 			}
			ads_printf(_T("\nEnd进路编码-%d:"),iJinLuIndex) ;
				
		}		
		acedRestoreStatusBar() ;
		ads_printf(_T("\n进路编码结束：......\n")) ;	
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in doall")) ;
	}          

}
void CMaXuBiaoForHD::JinLuBianMa_X_C_X()
{
	this->m_iCurJinLuDirect = 0 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;


	CStringArray strArrayGuiDao ;	//各轨道名称
	CString strRetBianMa = _T("") ; // GetGuiDaoBianMa(...)的参数，用来返回得到的编码
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;	
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机（在其左侧为越过）
	BOOL bPassX = FALSE ; //是否越过下进站信号机（在其左侧为越过）
	int iCodeTypeTmp = 0 ; //编码的种类
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum,1) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出下进站,下出站
		//////////////////////////////////////////////////////////////////////////
		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;
		this->m_strFileName.Format(_T("%s_%s_%s"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F) ;

		//end 找出下进站,下出站		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////			

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////

		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s通过)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassXI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoId<this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}

				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,iCodeTypeTmp) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
								
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;
			
			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s通过)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao) ;			
			strArrayDescribe.Add(strDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassX = FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassXI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}			

				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
				
				if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
				{
					strRetBianMa = _T("占用") ;
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
				}

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						
			
			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;
		
		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	
		
		//X开放直向，XI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
		//情况描述:X开放直向，XI关闭
		strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassX)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassX = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	
		
		if (m_iVersionFor == 1)
		{	
			//情况描述:X开放直向至XI，XI开放引导
			
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassX)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassX = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp) ;
				cSet.MovePrevious() ;			
			}
			CStringArray * pstrArrayTmpXIYinDao = new CStringArray() ;
			pstrArrayTmpXIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpXIYinDao) ;
		}
		
		cSet.Close() ;	
		
		//end X开放直向，XI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_c_x"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_C_X")) ;			
	} 

	//绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
		
	}

}

void CMaXuBiaoForHD::JinLuBianMa_S_Z_S()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;	
	BOOL bBianMaChange = 0 ;

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;

	BOOL bPassXI = FALSE ; //是否越过下出站信号机
	BOOL bPassXN = FALSE ; //是否越过进站信号机
	int iCodeTypeTmp = 0 ; //编码的种类 
					
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;
	
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出上进站,上出站
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s←%s←%s"), this->m_strJinZhanXHJ_F, this->m_strChuZhanXHJ,this->m_strJinZhanXHJ) ;
		this->m_strFileName.Format(_T("%s_%s_%s"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F) ;

		//end 找出上进站,上出站		
		//////////////////////////////////////////////////////////////////////////	

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s通过)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ, this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;
			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;
					}
					this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp,2) ;
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}				
					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoId>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
					this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}				
				}				
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, 0, 2) ;
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////		
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述	
			if (iGuiDaoId<this->m_iChuZhanXHJ_id)
			{
				strDescribeTmp.Format(_T("%s占用(%s开放%s通过)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao) ;
			}
			else
			{
				strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
			}
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;
					}
					this->GetGuiDaoBianMa2(strGuiDaoName,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp,2) ;

					if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, _T("占用")) ;
					}
					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
					this->GetGuiDaoBianMa2(strGuiDaoName,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				}
				//this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 2) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		//S开放直向，SII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		

		//情况描述:S开放直向，SII关闭
		strDescribeTmp.Format(_T("%s开放%s,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Rigth>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
			
			//this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 2) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpSII = new CStringArray() ;
		pstrArrayTmpSII->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpSII) ;	
		
		if(m_iVersionFor == 1)
		{
			//情况描述:S开放直向至SII，SII开放引导
			strDescribeTmp.Format(_T("%s开放直向至%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(strGuiDao,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				
				cSet.MoveNext() ;
				
				// 			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				// 			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa, bBianMaChange, 2, 2) ;
				// 			cSet.MoveNext() ;			
			}
			CStringArray * pstrArrayTmpSIIYinDao = new CStringArray() ;
			pstrArrayTmpSIIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end S开放直向，SII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
		//S开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		//侧向		
		strDescribeTmp.Format(_T("%s开放侧向,%s关闭"), this->m_strJinZhanXHJ, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 1, 2) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pstrArrayTmpS_C = new CStringArray() ;
		pstrArrayTmpS_C->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpS_C) ;	

		//关闭
		strDescribeTmp.Format(_T("%s进站关闭"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 2) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pstrArrayTmpS_CLOSE = new CStringArray() ;
		pstrArrayTmpS_CLOSE->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpS_CLOSE) ;	

		//S开放引导
		strDescribeTmp.Format(_T("%s开放引导"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 2, 2) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pstrArrayTmpS_Y = new CStringArray() ;
		pstrArrayTmpS_Y->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpS_Y) ;	
		
		
		cSet.Close() ;	
		
		//end S开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s")) ;
	} 

    //绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}	

}

BOOL CMaXuBiaoForHD::GetJinZhanOrChuZhan(CString &strJinZhan, int &iJinZhanId, CString &strChuZhan, int &iChuZhanId, CString &strGuDao,int &iGuDaoType, CString &strTowardsJinZhanXHJ, int iUpDown, int iDirect)
{
	BOOL bRet = TRUE ;
	//找出进站,出站
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//找出进站		
		int iUpDown_Sql = iUpDown ;
		if (iUpDown==1 && iDirect==1)
		{
			iUpDown_Sql = 2 ;
		}
		if (iUpDown==2 && iDirect==1)
		{
			iUpDown_Sql = 1 ;
		}
		
		strSql.Format(_T("select * from jinlu where signal_type=1 and signal_updown=%d and signal_direct=%d"), iUpDown_Sql, iDirect ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		cSet.GetFieldValue(_T("signal"), strJinZhan) ;
		cSet.GetFieldValue(_T("id"), iJinZhanId) ;
		cSet.Close() ;	
		
		//找出出站		
		strSql.Format(_T("select * from jinlu where signal_type=2 and signal_updown=%d"), iUpDown_Sql) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("signal"), strChuZhan) ;
		}
		else
		{
			strSql.Format(_T("select * from jinlu where signal_type2=2 and signal_updown2=%d"), iUpDown_Sql) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			cSet.Open(g_PtrCon, strSql) ;
			if (cSet.GetRecordCount()>0)
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				cSet.GetFieldValue(_T("signal2"), strChuZhan) ;
			}
			
		}
		cSet.GetFieldValue(_T("guidao_type"), iGuDaoType) ;		
		cSet.GetFieldValue(_T("guidao"), strGuDao) ;
		cSet.GetFieldValue(_T("id"), iChuZhanId) ;
		cSet.Close() ;
		
		//有几个guidao_type>0，即股道有没有分隔为G1、G2,如果有分隔，则股道名称需要去掉后面的数字
		strSql.Format(_T("select * from jinlu where guidao_type>0")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>1)
		{
			strGuDao = strGuDao.Left(strGuDao.GetLength()-1) ;
		}
		cSet.Close() ;


		//找出对面进站
		int iTmp = -1 ;
		iTmp = (iUpDown+iDirect)%2 ;
		if (iTmp==0)
		{
			strSql.Format(_T("select * from jinlu where signal_type=1 and id<%d"), iChuZhanId) ;
		}
		else if (iTmp>0)
		{
			strSql.Format(_T("select * from jinlu where signal_type=1 and id>%d"), iChuZhanId) ;
		}
		else
		{
			AfxMessageBox(_T("can't find 对面进站xhj")) ;
		}

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		cSet.GetFieldValue(_T("signal"), strTowardsJinZhanXHJ) ;
		cSet.Close() ;	
		


	}
	catch (_com_error& e)
	{
		bRet = FALSE ;
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-GetJinZhanOrChuZhan"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		bRet = FALSE ;
		AfxMessageBox(_T("unknown error in GetJinZhanOrChuZhan")) ;
	} 	

	return bRet ;
}

//暂时没用
void CMaXuBiaoForHD::DrawBianMaTableForJH(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &strArrayGuiDao, CStringArray &strArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect, BOOL bIsFanXiangChuZhan)
{	
	double dLineWidthThin = 0.0000 ;
	double dLineWidthThick = 0.4000 ;
	double dTextHeight = 3.50000 ;

// 	this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt2.shx", dTextHeight, 0.7) ;	
// 	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName("zhffont_guidao", "txt.shx", "hztxt2.shx", dTextHeight, 0.5) ;

	this->textStyleId = g_cSystem.CreateTextStyleByFileName(_T("zhffont"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightBianMa, this->m_dWidthBianMa) ;	
	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName(_T("zhffont_guidao"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightGuiDao, this->m_dWidthGuiDao) ;

	CStringArray strArrayLinZhanState ;
	int iLinZhanStateNum = 9 ;

	for (int i=0; i<iLinZhanStateNum; i++)
	{
		strArrayLinZhanState.Add(g_strLinZhanState_1[i]) ;
	}

	if (bIsFanXiangChuZhan==1)
	{
		iLinZhanStateNum ++ ;
		strArrayLinZhanState.InsertAt(0, _T("L5")) ;
	}

	
	//武广模式
	if (m_iVersionFor==1)
	{
		iLinZhanStateNum ++ ;
		strArrayLinZhanState.InsertAt(0, _T("")) ;
		//取得进路轨道频率
		CStringArray strArrayTmp ;
		strArrayTmp.Add(_T("")) ;
		//this->GetJinLuGuiDaoFreq(strArrayGuiDao, strArrayFreq) ;
		CStringArray * pstrArrayTmp = new CStringArray() ;
		pstrArrayTmp->Copy(strArrayTmp) ;
		arrayBianMa.InsertAt(0, pstrArrayTmp) ;
		strArrayDescribe.InsertAt(0, _T("")) ;		
	}
		
	int iRow = arrayBianMa.GetSize() ;	
	int iCol = strArrayGuiDao.GetSize() ;
	double dVerLen = iRow*(this->dTableCellHeight) ;
	double dHorLen = iCol*(this->dTableCellWidth) ;

	ads_point base_pt = {0,0,0} ;
	base_pt[0] = 2 ;
	base_pt[1] = this->dTableCur_Y + 10 ;
	AcGePoint3d align_3d(0,0,0) ;
	g_cSystem.ConvertPtAdsTo3D(base_pt, align_3d) ;
	//g_cSystem.WriteText(base_pt,align_3d, this->m_strCaseType,0,3,AcDb::kTextBase,AcDb::kTextLeft,this->textStyleId,5,0.7,0) ;
	g_cSystem.AddText(this->m_strCaseType,align_3d,align_3d, AcDb::kTextLeft, AcDb::kTextBase, 5, 0.7, 0, 0, this->textStyleId) ;
	
	//画列线	
	BOOL bBianMaChange = 0 ;
	for(int iG=0; iG<iCol+1; iG++)
	{
		AcGePoint3d start_3d(0, 0, 0) ;
		AcGePoint3d end_3d(0, 0, 0) ;
		start_3d.x = iG*(this->dTableCellWidth) + this->dTableCur_X;
		start_3d.y = dTableCur_Y + this->dTableCellHeight ;
		end_3d.x = start_3d.x ;
		end_3d.y = -dVerLen + this->dTableCur_Y ;
		//g_cSystem.AddAcDbLine(start_3d, end_3d) ;		
		
		if (iG==0)
		{	
			bBianMaChange = arrayBianMaChange.GetAt(iG) ;
			
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d,dLineWidthThick, 0) ;
			if (iWriteDirect==0)
			{
				AcGePoint3d des_start_3d(0, 0, 0) ;
				AcGePoint3d des_end_3d(0, 0, 0) ;
				des_start_3d.x = this->dTableCur_X - dDescribeCellWidth ;
				des_start_3d.y = start_3d.y ;
				des_end_3d.x = des_start_3d.x ;
				des_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(des_start_3d, des_end_3d, dLineWidthThick, 0) ;
			}
			else if (iWriteDirect==1) 
			{
				AcGePoint3d LinZhan_start_3d(0, 0, 0) ;
				AcGePoint3d LinZhan_end_3d(0, 0, 0) ;
				LinZhan_start_3d.x = this->dTableCur_X - this->dTableCellWidth ;
				LinZhan_start_3d.y = start_3d.y ;
				LinZhan_end_3d.x = LinZhan_start_3d.x ;
				LinZhan_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(LinZhan_start_3d, LinZhan_end_3d, dLineWidthThick, 0) ;	
				
				ads_point base_pt_linzhan = {0,0,0} ;
				AcGePoint3d alignP_3d_linzhan(0, 0, 0) ;				
				base_pt_linzhan[0] =  LinZhan_start_3d.x + dTableCellWidth/2 ;
				base_pt_linzhan[1] =  dTableCur_Y + dTableCellHeight/2 ;
				g_cSystem.AddMText(base_pt_linzhan, _T("(邻站)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
		}
		else if (iG==iCol)
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d,dLineWidthThick, 0) ;
			if (iWriteDirect==0)
			{
				AcGePoint3d des_start_3d(0, 0, 0) ;
				AcGePoint3d des_end_3d(0, 0, 0) ;
				des_start_3d.x = start_3d.x + this->dTableCellWidth ;
				des_start_3d.y = start_3d.y ;
				des_end_3d.x = des_start_3d.x ;
				des_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(des_start_3d, des_end_3d, dLineWidthThick, 0) ;
			}
			else
			{
				AcGePoint3d LinZhan_start_3d(0, 0, 0) ;
				AcGePoint3d LinZhan_end_3d(0, 0, 0) ;
				LinZhan_start_3d.x = start_3d.x + dDescribeCellWidth ;
				LinZhan_start_3d.y = start_3d.y ;
				LinZhan_end_3d.x = LinZhan_start_3d.x ;
				LinZhan_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(LinZhan_start_3d, LinZhan_end_3d, dLineWidthThick, 0) ;				
				
			}
			
		}
		else
		{
			if (bBianMaChange!=arrayBianMaChange.GetAt(iG))
			{
				g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, dLineWidthThick, 0) ;
				bBianMaChange = arrayBianMaChange.GetAt(iG) ;
			}
			else
			{
				g_cSystem.AddAcDb2dPolyline(start_3d, end_3d,dLineWidthThick, 0) ;				
			}
		}

		AcGePoint3d start_3d_Mid(0, 0, 0) ;
		AcGePoint3d end_3d_Mid(0, 0, 0) ;
		start_3d_Mid.x = start_3d.x + this->dTableCellWidth/2 ;
		start_3d_Mid.y = dTableCur_Y  ;
		//武广要留一行画频率
		if (m_iVersionFor==1)
		{
			start_3d_Mid.y = start_3d_Mid.y - dTableCellHeight ;
		}
		end_3d_Mid.x = start_3d_Mid.x ;
		end_3d_Mid.y= end_3d.y ;
		
		ads_point base_pt = {0,0,0} ;
		
		base_pt[0] =  start_3d.x + dTableCellWidth/2  ;
		base_pt[1] =  dTableCur_Y + dTableCellHeight/2 ;
		

		//最后一列是封闭线，不用在画中线和写轨道了
		if (iG==iCol)
		{
			if (iWriteDirect==0)
			{
				g_cSystem.AddMText(base_pt, _T("(邻站)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			break ;
		}			

		g_cSystem.AddAcDb2dPolyline(start_3d_Mid, end_3d_Mid, dLineWidthThin, 0) ;

		g_cSystem.AddMText(base_pt, strArrayGuiDao.GetAt(iG), this->textStyleId_GuiDao, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		
		//武广，add频率
		if(m_iVersionFor == 1)
		{		
			base_pt[1] = base_pt[1] - dTableCellHeight ;
			g_cSystem.AddMText(base_pt, m_strArrayCurJinLuFre.GetAt(iG), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		}
	}

	//画行线
	for(int iM=0; iM<iRow+1; iM++)
	{
		AcGePoint3d start_3d(0, 0, 0) ;
		AcGePoint3d end_3d(0, 0, 0) ;

		start_3d.y = -iM*(this->dTableCellHeight) + this->dTableCur_Y ;
		end_3d.y = start_3d.y ;

		ads_point base_pt_linzhan = {0,0,0} ;
		AcGePoint3d alignP_3d_linzhan(0, 0, 0) ;
		
		if (iWriteDirect==0)
		{
			start_3d.x = this->dTableCur_X - dDescribeCellWidth;
			end_3d.x= dHorLen + this->dTableCur_X + this->dTableCellWidth ;

			if (iM<iLinZhanStateNum)
			{
				base_pt_linzhan[0] = end_3d.x -this->dTableCellWidth + dTableCellWidth/2 ;
				base_pt_linzhan[1] = start_3d.y - this->dTableCellHeight + dTableCellHeight/2 ;
				
				g_cSystem.AddMText(base_pt_linzhan, strArrayLinZhanState[iM] , this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
	
			}
		}
		else if (iWriteDirect==1)
		{
			start_3d.x = this->dTableCur_X - this->dTableCellWidth ;
			end_3d.x= dHorLen + this->dTableCur_X + dDescribeCellWidth ;
			
			if (iM<iLinZhanStateNum)
			{
				base_pt_linzhan[0] = start_3d.x + dTableCellWidth/2 ;
				base_pt_linzhan[1] = start_3d.y - this->dTableCellHeight + dTableCellHeight/2 ;
				
				g_cSystem.AddMText(base_pt_linzhan, strArrayLinZhanState[iM], this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			
		}

		//顶上的横线（在轨道名上方）只画一次
		if (iM==0)
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, 0, 0) ;
			
			AcGePoint3d top_start_3d(0, 0, 0) ;
			AcGePoint3d top_end_3d(0, 0, 0) ;
			top_start_3d.x = start_3d.x ;
			top_start_3d.y = start_3d.y + this->dTableCellHeight ;
			top_end_3d.x = end_3d.x ;
			top_end_3d.y = top_start_3d.y ;
			g_cSystem.AddAcDb2dPolyline(top_start_3d, top_end_3d, 0.4, 0) ;
		}
		else if (iM==iRow)
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, 0.4, 0) ;			
		}
		else 
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, 0, 0) ;
		}
		//最后一行线是封底线，以下操作不再进行
		if (iM==iRow)
		{
			break ;
		}
		
		ads_point base_pt = {0,0,0} ;
		AcGePoint3d alignP_3d(0, 0, 0) ;
		if (iWriteDirect==0)
		{
			base_pt[0] =  dTableCur_X - dDescribeCellWidth/2  ;	
		}
		else if (iWriteDirect==1)
		{
			base_pt[0] = dHorLen + this->dTableCur_X + dDescribeCellWidth/2 ;
		}
		base_pt[1] = start_3d.y - dTableCellHeight/2 ;
		g_cSystem.AddMText(base_pt, strArrayDescribe.GetAt(iM), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dDescribeCellWidth, 0) ;				
		
	}
	
	//分车从左往右走和从右往左走两种情况；
	int iBianMaMax = arrayBianMa.GetSize()-1 ;
	double dTextWidth = dTableCellWidth/2-1 ;
	if (iWriteDirect==0)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pstrArrayTmp->GetSize()-1 ;
			ads_point base_pt = {0, 0, 0} ;	
			base_pt[1] = -(iarray+1)*(this->dTableCellHeight) + dTableCellHeight/2 + this->dTableCur_Y ;
			if(iarray==0 && m_iVersionFor==1)
			{
				dTextWidth = dTableCellWidth - 1 ;
			}
			for(int ib=iNumMax; ib>=0; ib--)
			{	
				base_pt[0] = (iNumMax-ib)*(this->dTableCellWidth) + dTableCellWidth/4 + this->dTableCur_X ;
				if(iarray==0 && m_iVersionFor==1)
				{
					base_pt[0] = base_pt[0] + dTableCellWidth/4;
				}
				g_cSystem.AddMText(base_pt, pstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTextWidth, 0) ;				
				
			}
		}
	}
	else if (iWriteDirect==1)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pstrArrayTmp->GetSize() ;
			int iStartCol = iCol - iNumMax ;
			ads_point base_pt = {0, 0, 0} ;
			base_pt[1] = -(iarray+1)*(this->dTableCellHeight) + dTableCellHeight/2 + this->dTableCur_Y;	
			if(iarray==0 && m_iVersionFor==1)
			{
				dTextWidth = dTableCellWidth - 1 ;
			}
			for(int ib=0; ib<iNumMax; ib++)
			{
				base_pt[0] = (iStartCol+ib)*(this->dTableCellWidth) + dTableCellWidth/4 + this->dTableCur_X ;
				if(iarray==0 && m_iVersionFor==1)
				{
					base_pt[0] = base_pt[0] + dTableCellWidth/4;
				}
				g_cSystem.AddMText(base_pt, pstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth/2-1, 0) ;				
				
			}
		}
		
	}

	//begin 画图框
	//////////////////////////////////////////////////////////////////////////
	AcDbBlockReference * pRef = NULL ;
	CString strBlockName ;
	CStringArray strarrayAtt ;
	//double dSiYiTuHeight = 80 ;
	//double dBetweenTableAndBottom = 40 ; //old:47
	AcGePoint3d pt_3d_topleft(0,0,0),pt_3d_bottomleft(0,0,0),pt_3d_bottomright(0,0,0) ;
	
	//只在画上面一张表格时设置一次边框x坐标
	if (dTableCur_Y==0)
	{
		m_dFrameLeft_x =  iWriteDirect==0?-105:-53 ;
	}
	pt_3d_topleft.x = m_dFrameLeft_x ;
	pt_3d_topleft.y = dTableCur_Y + m_dSiYiTuHeight + dTableCellHeight ;
	pt_3d_bottomleft.x = pt_3d_topleft.x ;
	pt_3d_bottomleft.y = dTableCur_Y - dVerLen - m_dBetweenTableAndBottom ;
	pt_3d_bottomright.x = m_dFrameLeft_x+25+20+dDescribeCellWidth+dTableCellWidth+dHorLen ;// iWriteDirect==0?dHorLen+28:dHorLen+80 ;
	pt_3d_bottomright.y = pt_3d_bottomleft.y + 5 ;
	strBlockName = _T("maxubiao_TopLeft") ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_topleft ))
	{
		pRef->close() ;
	}
	strBlockName = _T("maxubiao_BottomLeft") ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_bottomleft ))
	{
		pRef->close() ;
	}
	strBlockName = _T("frame_MaXuBiao_BottomRightForWG") ;
	strarrayAtt.RemoveAll() ;
	//m_strStationName = _T("测试站") ;
	//strarrayAtt.Add(m_strStationName) ;
	//strarrayAtt.Add(m_strCaseType) ;
	//if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_bottomright, &strarrayAtt, FALSE))
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_bottomright))
	{
		pRef->close() ;
	}
	ads_point text_pt = {0, 0, 0} ;
	text_pt[0] = pt_3d_bottomright.x - 90 ;
	text_pt[1] = pt_3d_bottomright.y + 20 ;
	//AcCmColor colr ;
	//colr.setColorIndex(0) ;
	//g_cSystem.WriteText(text_pt, m_strStationName, 0, AcDb::kTextMid, 4, 0.9, colr) ;
	//AcDbObjectId objId ;
	//g_cSystem.WriteText(text_pt,m_strStationName, objId, 4, 0.8, AcDb::kTextCenter, AcDb::kTextVertMid, 0, this->textStyleId) ;
	g_cSystem.AddText(m_strStationName, text_pt, text_pt, AcDb::kTextCenter, AcDb::kTextVertMid, 4, 0.8, 0, 0,this->textStyleId) ;


	//text_pt[0] = pt_3d_bottomright.x - 80 ;
	text_pt[1] = pt_3d_bottomright.y + 12 ;
	//AcDbObjectId objId2 ;
	//g_cSystem.WriteText(text_pt,m_strCaseType, objId2, 4, 0.8, AcDb::kTextCenter, AcDb::kTextVertMid, 0, this->textStyleId) ;
	g_cSystem.AddText(m_strCaseType, text_pt, text_pt, AcDb::kTextCenter, AcDb::kTextVertMid, 4, 0.8, 0, 0,this->textStyleId) ;

	//g_cSystem.WriteText(text_pt, m_strCaseType, 0, AcDb::kTextMid, 3, 0.9, colr) ;
	
	AcGePoint3d pt_3d_1(pt_3d_topleft.x,pt_3d_topleft.y+5,0) ;
	AcGePoint3d pt_3d_2(pt_3d_bottomright.x+5,pt_3d_topleft.y+5,0) ;
	AcGePoint3d pt_3d_3(pt_3d_bottomright.x+5,pt_3d_bottomright.y-5,0) ;
	AcGePoint3d pt_3d_4(pt_3d_topleft.x,pt_3d_bottomright.y-5,0) ;
	AcGePoint3dArray pt_3d_array_outside, pt_3d_array_inside ;
	pt_3d_array_outside.append(pt_3d_1) ;
	pt_3d_array_outside.append(pt_3d_2) ;
	pt_3d_array_outside.append(pt_3d_3) ;
	pt_3d_array_outside.append(pt_3d_4) ;
	//暂时不需要外框，先屏蔽
	g_cSystem.AddAcDb2dPolyline(pt_3d_array_outside,0, 0) ;

	pt_3d_1.x +=25 ;
	pt_3d_1.y -=5 ; 
	pt_3d_2.x -=5 ;
	pt_3d_2.y -=5 ;
	pt_3d_3.x -=5 ;
	pt_3d_3.y +=5 ;
	pt_3d_4.x +=25 ;
	pt_3d_4.y +=5 ;
	pt_3d_array_inside.append(pt_3d_1) ;
	pt_3d_array_inside.append(pt_3d_2) ;
	pt_3d_array_inside.append(pt_3d_3) ;
	pt_3d_array_inside.append(pt_3d_4) ;
	g_cSystem.AddAcDb2dPolyline(pt_3d_array_inside,0.4, 0) ;
	//end 画图框
	//////////////////////////////////////////////////////////////////////////

	//begin 画示意图
	//////////////////////////////////////////////////////////////////////////
	AcGePoint3d pt_3d_mid(0,0,0) ;
	pt_3d_mid.x = pt_3d_1.x + (pt_3d_2.x-pt_3d_1.x)/2 ;
	pt_3d_mid.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
	strBlockName.Format(_T("%s\\syt"), this->m_strDwgSaveDir) ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_mid))
	{
		pRef->close() ;
	}
	//end 画示意图
	//////////////////////////////////////////////////////////////////////////

	//增量当前的Y坐标
	this->dTableCur_Y = this->dTableCur_Y - dVerLen - this->dBetweenTable_Y ;
	
}


void CMaXuBiaoForHD::JinLuBianMa_S_C_S()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;	
	BOOL bBianMaChange = 0 ;				

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;		
	
	BOOL bPassSI = FALSE ; //是否越过出站信号机（在其right侧为越过）
	BOOL bPassS = FALSE ; //是否越过进站信号机（在其right侧为越过）
	int iCodeTypeTmp = 0 ; //编码的种类 
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;

		//找出上进站,上出站
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s←%s←%s"), this->m_strJinZhanXHJ_F, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ) ;
		this->m_strFileName.Format(_T("%s_%s_%s"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F) ;
		//end 找出上进站,上出站
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s通过)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}	
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassSI = FALSE ;
			bPassS = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				if (!bPassSI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassSI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassS)
					{
						if (iGuiDaoId>this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassS = TRUE ;	
						}						
					}
					
				}

				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 2) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,bBianMaChange,iCodeTypeTmp, 2) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s通过)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码

			strRetBianMa = _T("") ;
			bPassSI = FALSE ;
			bPassS = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				if (!bPassSI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassSI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassS)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassS = TRUE ;	
						}							
					}
					
				}	

				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				if (iGuiDaoIdTmp==iGuiDaoId && iGuiDaoIdTmp==m_iChuZhanXHJ_id)
				{
					strRetBianMa = _T("占用") ;
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
				}

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		//S开放c向，SII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		

		//情况描述:S开放c向，SII关闭
		strDescribeTmp.Format(_T("%s开放%s,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao,this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		bPassS = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassS)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassS = TRUE ;
				}
			}

			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpSII = new CStringArray() ;
		pstrArrayTmpSII->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpSII) ;	
		
		if (m_iVersionFor == 1)
		{			
			//情况描述:S开放至SII，SII开放引导		
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;
			bPassS = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassS)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{	
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassS = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				cSet.MoveNext() ;			
			}
			CStringArray * pstrArrayTmpSIIYinDao = new CStringArray() ;
			pstrArrayTmpSIIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end S开放c向，SII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-in s-c-s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in s-c-s")) ;
	} 

    //绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}

}

void CMaXuBiaoForHD::GetJinLuGuiDao(CStringArray &strArrayGuiDao)
{
	CString strGuiDao ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen()) 
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		//将进路中所有轨道存入strArrayGuiDao		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			strArrayGuiDao.Add(strGuiDao) ;
			cSet.MoveNext() ;
		}
		cSet.Close() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetJinLuGuiDao")) ;
	} 
	
}

void CMaXuBiaoForHD::JinLuBianMa_X_ZC_S()
{
	this->m_iCurJinLuDirect = 0 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	CStringArray strArrayGuiDao ;	//各轨道名称
	CString strRetBianMa = _T("") ; // GetGuiDaoBianMa(...)的参数，用来返回得到的编码
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;	
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机（在其左侧为越过）
	BOOL bPassX = FALSE ; //是否越过下进站信号机（在其左侧为越过）
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //编码的种类 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum,1) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出进站,出站
		//////////////////////////////////////////////////////////////////////////
		/*
		CString this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F ;
		int this->m_iJinZhanXHJ_id = 0 ;
		int this->m_iChuZhanXHJ_id = 0 ;
		int this->m_iGuDao_Type = 0 ;		
		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ, this->m_iJinZhanXHJ_id, this->m_strChuZhanXHJ, this->m_iChuZhanXHJ_id, this->m_strGuDao, this->m_iGuDao_Type, this->m_strJinZhanXHJ_F) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;
		*/
		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;
		this->m_strFileName.Format(_T("%s_%s_%s"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F) ;

		//end 找出进站,出站		
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		
		//出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		strDescribeTmp.Format(_T("%s开放%s通过,S口区间空闲"), this->m_strChuZhanXHJ, this->m_strGuDao) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		strArrayBianMaTmp.RemoveAll() ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}			

		bPassXI = FALSE ;
		bPassX = FALSE ;
		
		iCodeTypeTmp = 0 ;		
		bBianMaChange = 0 ;
		strRetBianMa = _T("L5") ;		
		
		while (!cSet.IsBOF())
		{
			CString strGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
			
			if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					
					bPassXI = TRUE ;						
				}
				this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
				
			}
			else
			{
				if (!bPassX)
				{
					if (iGuiDaoId<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;	
					}
				}
				this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 1) ;
				
			}			
			
			cSet.MovePrevious() ;
		}
		
		cSet.Close() ;
		
		CStringArray * pstrArrayTmp_L5 = new CStringArray() ;
		pstrArrayTmp_L5->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmp_L5) ;
		
		//end 出站开放，全L5

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s,%s关闭)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao,this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
	
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;

				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 2, 1) ;
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
// 					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
// 					{
// 						arrayBianMaChange.Add(bBianMaChange) ;
// 					}
					
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoId<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 1) ;
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 1) ;
					
				}
				
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,iCodeTypeTmp) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是_T("L5,L4,...HU,占用")的9种情况
		//////////////////////////////////////////////////////////////////////////
		
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	
		int iLQ_ID = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		if (cSet.GetRecordCount()>0)
		{
			cSet.GetFieldValue(_T("id"), iLQ_ID) ;
		}
		cSet.Close() ;
		
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;
			
			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s,%s关闭)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao,this->m_strChuZhanXHJ) ;			
			strArrayDescribe.Add(strDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = iGuiDaoId<this->m_iChuZhanXHJ_id?TRUE:FALSE ;
			bPassX = iGuiDaoId<this->m_iJinZhanXHJ_id?TRUE:FALSE ;
			bPassJinZhan_F = iGuiDaoId<=this->m_iJinZhanXHJ_F_id?TRUE:FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2,1) ;

					if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
					{
						strRetBianMa = _T("占用") ;
						strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
					}

				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0 ) 
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;							
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
						if (iGuiDaoId==iGuiDaoIdTmp)//股道占用
						{
							strRetBianMa = _T("") ;
						}
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2,1) ;
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp,1) ;
					
				}			

				//this->GetGuiDaoBianMa(strGuiDaoName, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp) ;
				
				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						
			
			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;
		
		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		// 进站开放，出站关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
		//情况描述:进站开放，出站关闭
		strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassX)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassX = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	

		if (m_iVersionFor == 1)
		{
			//begin yd
			
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassX)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
				cSet.MovePrevious() ;			
			}
			//cSet.Close() ;	
			CStringArray * pstrArrayTmpyd = new CStringArray() ;
			pstrArrayTmpyd->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpyd) ;	
			
			//end yd
		}
		
		cSet.Close() ;	
		
		//end 进站开放，出站关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_zc_s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_zC_s")) ;			
	} 
	
	//绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2, 0, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}		
	}	
}

void CMaXuBiaoForHD::JinLuBianMa_S_ZC_X()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	CStringArray strArrayGuiDao ;	//各轨道名称
	CString strRetBianMa = _T("") ; // GetGuiDaoBianMa(...)的参数，用来返回得到的编码
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;	
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机
	BOOL bPassXN = FALSE ; //是否越过进站信号机
	int iCodeTypeTmp = 0 ; //编码的种类 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;	

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出进站,出站
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;
		this->m_strFileName.Format(_T("%s_%s_%s"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strJinZhanXHJ_F) ;

		//end 找出进站,出站		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s通过)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp) ;
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp) ;
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}
					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoId<this->m_iJinZhanXHJ_id)
						{
							//if (this->m_iGuDao_Type!=2)
							if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 2, 1) ;					
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}
					
				}
				
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,iCodeTypeTmp) ;
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
				
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////		
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;
			
			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s通过)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao) ;			
			strArrayDescribe.Add(strDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码

			strRetBianMa = _T("") ;
			bPassXI = iGuiDaoId<this->m_iChuZhanXHJ_id?TRUE:FALSE ;
			bPassXN = iGuiDaoId<this->m_iJinZhanXHJ_id?TRUE:FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						if (this->m_iGuDao_Type !=1 || this->m_iZpGuiDaoSum_Rigth>0) 
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;							
						}
						bPassXI = TRUE ;						
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1) ;
					if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("占用") ;
						strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
					}
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id )
						{
							if (this->m_iGuDao_Type !=2 || this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
					
				}
				//this->GetGuiDaoBianMa(strGuiDaoName, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp) ;				
				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						
			
			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;
		
		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////

		// 进站开放，出站关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
		//情况描述:进站开放，出站关闭
		strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;

		strRetBianMa = _T("占用") ;		
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;						
					}
					bPassXN = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	

		if (m_iVersionFor == 1)
		{
			//begin 引导
			//////////////////////////////////////////////////////////////////////////
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			
			strRetBianMa = _T("") ;		
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;						
						}
						bPassXN = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSet.MovePrevious() ;			
			}
			//cSet.Close() ;	
			CStringArray * pstrArrayTmpyd = new CStringArray() ;
			pstrArrayTmpyd->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpyd) ;
			
			//end 引导
		}
		
		cSet.Close() ;	
		
		//end 进站开放，出站关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_zc_x"), sBuff) ;		
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_zC_X")) ;			
	} 
	
	//绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
		
	}

}

void CMaXuBiaoForHD::JinLuBianMa_X_Z_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机
	BOOL bPassXN = FALSE ; //是否越过进站信号机
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //编码的种类 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;		

	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;
		
		//找出下进站,下出站
		//////////////////////////////////////////////////////////////////////////		

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s←%s←%s"), this->m_strJinZhanXHJ_F, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ) ;
	
		//end 找出下进站,下出站		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		strDescribeTmp.Format(_T("%s开放%s通过,X口区间空闲"), this->m_strChuZhanXHJ, this->m_strGuDao) ;
		//strDescribeTmp.Format(_T("%s开放直向通过,X口区间空闲"), this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		strArrayBianMaTmp.RemoveAll() ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}	
		
		bPassXI = FALSE ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;
		strRetBianMa = _T("L5") ;
		bBianMaChange = 0 ;

		while (!cSet.IsEOF())
		{
			CString strGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
			
			if (!bPassJinZhan_F)
			{
				if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
					}
					bPassJinZhan_F = TRUE ;
				}
			}
			else if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXI = TRUE ;
				}
			}
			else
			{
				if (!bPassXN)
				{
					if (iGuiDaoId>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				
			}			
			this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,bBianMaChange,iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;
		}			
		cSet.Close() ;
		
		CStringArray * pstrArrayTmp_L5 = new CStringArray() ;
		pstrArrayTmp_L5->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmp_L5) ;
		
		//end 出站开放，全L5

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		//CString strStartCodeTmp[9] = {"L5","L4","L3","L2","L","LU","U","HU","占用"} ;
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s,%s关闭)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{

			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
	
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;

			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			
			bBianMaChange = 0 ;			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
					{
// 						if (this->m_iZpGuiDaoSum_Left>0)
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
					}
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoId>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN	= TRUE ;
						}
					}
					
				}

				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, 0, 1, 1) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,bBianMaChange,iCodeTypeTmp, 1, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
		
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		int iSN1LQ_id = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d and signal<>''"),this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("id"), iSN1LQ_id) ;
		}
		cSet.Close() ;

		//左侧是在进站分界，倒置没有反向离去区段
		if (iSN1LQ_id==0)
		{
			strSql.Format(_T("select * from jinlu ")) ;			
		}
		else
		{
			strSql.Format(_T("select * from jinlu")) ;
		}
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述		
			if (iGuiDaoId<this->m_iChuZhanXHJ_id)
			{
				strDescribeTmp.Format(_T("%s占用(%s开放%s,%s关闭)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			}
			else
			{
				strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
				
			}
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
// 						if (iGuiDaoIdTmp==iGuiDaoId)
// 						{
// 							strRetBianMa = _T("") ;
// 						}
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id) 
					{
// 						if (this->m_iZpGuiDaoSum_Left>0)
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
					}
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN	= TRUE ;
						}
					}
					
				}				

				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;

				if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
				{
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, _T("占用")) ;
				}

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	

		//SN开放直向，SI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		//情况描述:SN开放直向，SI关闭
		strDescribeTmp.Format(_T("%s开放%s,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;

		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;

			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Rigth>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN	= TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	
		
		if (m_iVersionFor == 1)
		{
			//begin yd
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;		
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
				cSet.MoveNext() ;			
			}
			//cSet.Close() ;	
			CStringArray * pstrArrayTmpyd = new CStringArray() ;
			pstrArrayTmpyd->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpyd) ;
			//end yd
		}
		cSet.Close() ;	

		//end SN开放直向，SI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
		//SN开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		//侧向		
		strDescribeTmp.Format(_T("%s开放侧向,%s关闭"), this->m_strJinZhanXHJ, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 1, 1, 1) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pstrArrayTmpX_C = new CStringArray() ;
		pstrArrayTmpX_C->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpX_C) ;	

		//关闭
		strDescribeTmp.Format(_T("%s进站关闭"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 1, 1) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pstrArrayTmpX_CLOSE = new CStringArray() ;
		pstrArrayTmpX_CLOSE->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpX_CLOSE) ;	

		//X开放引导
		strDescribeTmp.Format(_T("%s开放引导"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 2, 1, 1) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pstrArrayTmpX_Y = new CStringArray() ;
		pstrArrayTmpX_Y->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpX_Y) ;	
		
		
		cSet.Close() ;	
		
		//end X开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_z_x_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error x_z_x_f")) ;
	} 

	//绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe,arrayBianMaChange, 1, 1) ;
		
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}	

}

void CMaXuBiaoForHD::JinLuBianMa_X_C_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	CStringArray strArrayGuiDao ;	//各轨道名称
	CString strRetBianMa = _T("") ; // GetGuiDaoBianMa(...)的参数，用来返回得到的编码
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;	
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机（在其左侧为越过）
	BOOL bPassX = FALSE ; //是否越过下进站信号机（在其左侧为越过）
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //编码的种类 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出下进站,下出站
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s←%s←%s"),this->m_strJinZhanXHJ_F, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ ) ;

		//end 找出下进站,下出站		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		//出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		strDescribeTmp.Format(_T("%s开放%s通过,X口区间空闲"), this->m_strChuZhanXHJ, this->m_strGuDao) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		strArrayBianMaTmp.RemoveAll() ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		
		bPassXI = FALSE ;
		bPassX = FALSE ;

		iCodeTypeTmp = 0 ;
		strRetBianMa = _T("L5") ;
		bBianMaChange = 0 ;
		
		while (!cSet.IsEOF())
		{
			CString strGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
			
			if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassXI = TRUE ;						
				}					
			}
			else
			{
				if (!bPassX)
				{
					if (iGuiDaoId>this->m_iJinZhanXHJ_id)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassX = TRUE ;	
					}						
				}
				
			}
			
			this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange,iCodeTypeTmp, 1, 1) ;
			
			cSet.MoveNext() ;
		}			
		cSet.Close() ;
		
		CStringArray * pstrArrayTmp_L5 = new CStringArray() ;
		pstrArrayTmp_L5->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmp_L5) ;
		
		//end 出站开放，全L5

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s,%s关闭)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			

			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoId>this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}

				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,iCodeTypeTmp, 1, 1) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange,iCodeTypeTmp, 1, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
		
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		int iLq_id = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsEOF()) 
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;
		
		//左侧以进站为分界，所以没有反向离去区段
		if (iLq_id==0)
		{
			strSql.Format(_T("select * from jinlu")) ;			
		}
		else
		{
			strSql.Format(_T("select * from jinlu")) ;
		}
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;
			
			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s,%s关闭)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;			
			strArrayDescribe.Add(strDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
// 						if (iGuiDaoIdTmp==iGuiDaoId)
// 						{
// 							strRetBianMa = _T("") ;
// 						}
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}			

				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
				
				if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
				{
					strRetBianMa = _T("占用") ;
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
				}

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						
			
			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;		
		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	
		
		//sn开放直向，sI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
		//情况描述:sn开放直向，sI关闭
		strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassX)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassX = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	
			
		if(m_iVersionFor == 1)
		{
			//情况描述:SN开放直向至SI，SI开放引导
			
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassX)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassX = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;
				cSet.MoveNext() ;			
			}
			
			CStringArray * pstrArrayTmpXIYinDao = new CStringArray() ;
			pstrArrayTmpXIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpXIYinDao) ;
		}
		
		cSet.Close() ;	
		
		//end SN开放直向，SI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_c_x_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_C_X_F")) ;			
	} 

	//绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange, 1, 1) ;	
	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}		
	}
}

void CMaXuBiaoForHD::JinLuBianMa_X_ZC_S_F()
{
	this->m_iCurJinLuDirect = 1 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	CStringArray strArrayGuiDao ;	//各轨道名称
	CString strRetBianMa = _T("") ; // GetGuiDaoBianMa(...)的参数，用来返回得到的编码
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;	
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机（在其左侧为越过）
	BOOL bPassX = FALSE ; //是否越过下进站信号机（在其左侧为越过）
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //编码的种类 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出进站,出站
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s←%s←%s"), this->m_strJinZhanXHJ_F, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ) ;
		
		//end 找出进站,出站		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		strDescribeTmp.Format(_T("%s开放%s通过,X口区间空闲"), this->m_strChuZhanXHJ, this->m_strGuDao) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		strArrayBianMaTmp.RemoveAll() ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}			

		bPassXI = FALSE ;
		bPassX = FALSE ;

		iCodeTypeTmp = 0 ;		
		bBianMaChange = 0 ;
		strRetBianMa = _T("L5") ;
		
		while (!cSet.IsEOF())
		{
			CString strGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
			
			if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					
					bPassXI = TRUE ;						
				}
				this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;					

			}
			else
			{
				if (!bPassX)
				{
					if (iGuiDaoId>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;	
					}
				}
				this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
			}
			
			cSet.MoveNext() ;
		}

		cSet.Close() ;
		
		CStringArray * pstrArrayTmp_L5 = new CStringArray() ;
		pstrArrayTmp_L5->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmp_L5) ;
		
		//end 出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		
		//临站是_T("L5,L4,...HU,占用")的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s,%s关闭)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;					

				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 1, 1) ;
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;					
// 					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
// 					{
// 						arrayBianMaChange.Add(bBianMaChange) ;
// 					}
					
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoId>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 2) ;
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
// 					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
// 					{
// 						arrayBianMaChange.Add(bBianMaChange) ;
// 					}				
					
				}
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,iCodeTypeTmp) ;
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是_T("L5,L4,...HU,占用")的9种情况
		//////////////////////////////////////////////////////////////////////////
		
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	
		int iLQ_ID = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		if (cSet.GetRecordCount()>0)
		{
			cSet.GetFieldValue(_T("id"), iLQ_ID) ;
		}
		cSet.Close() ;

		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;
			
			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s,%s关闭)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;			
			strArrayDescribe.Add(strDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;

			bPassXI = iGuiDaoId>this->m_iChuZhanXHJ_id?TRUE:FALSE ;
			bPassX = iGuiDaoId>this->m_iJinZhanXHJ_id?TRUE:FALSE ;
			bPassJinZhan_F = iGuiDaoId>=this->m_iJinZhanXHJ_F_id?TRUE:FALSE ;

			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1,1) ;
					if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
					{
						strRetBianMa = _T("占用") ;
						strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0) 
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;							
// 						}
						strRetBianMa = _T("占用") ;
 						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
						if (iGuiDaoId==iGuiDaoIdTmp)//股道占用
						{
							strRetBianMa = _T("") ;
						}
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1,1) ;
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp,2) ;
					
				}			

				//this->GetGuiDaoBianMa(strGuiDaoName, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp) ;
				
				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						
			
			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;
		
		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		//s开放，sI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		//情况描述:s开放，sI关闭
		strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;

			if (!bPassX)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassX = TRUE ;	
				}						
			}
			
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;
		
		if(m_iVersionFor == 1)
		{
			//begin yd
			strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			cSet.Open(g_PtrCon, strSql) ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				
				if (!bPassX)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;	
					}						
				}
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
				cSet.MoveNext() ;			
			}
			//cSet.Close() ;	
			CStringArray * pstrArrayTmpyd = new CStringArray() ;
			pstrArrayTmpyd->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpyd) ;
			
			//end yd
		}


		cSet.Close() ;	
		
		//end S开放，SI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_zc_s_F"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_zC_s_F")) ;			
	} 
	
	//绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange, 1, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}		
	}
}

void CMaXuBiaoForHD::JinLuBianMa_S_Z_S_F()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;	

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;

	BOOL bPassXI = FALSE ; //是否越过下出站信号机
	BOOL bPassXN = FALSE ; //是否越过进站信号机
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //编码的种类 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		

		//找出进站,出站
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;

		//end 找出进站,出站		
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		strDescribeTmp.Format(_T("%s开放%s通过,S口区间空闲"), this->m_strChuZhanXHJ, this->m_strGuDao) ;
		//strDescribeTmp.Format(_T("%s开放直向通过,S口区间空闲"), this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		strArrayBianMaTmp.RemoveAll() ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		bPassXI = FALSE ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;		
		strRetBianMa = _T("L5") ;		

		while (!cSet.IsBOF())
		{
			CString strGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
			
			if (!bPassJinZhan_F)
			{
				if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
				{
					if(this->m_iZpGuiDaoSum_Rigth>0)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
					}

					bPassJinZhan_F = TRUE ;
				}
			}
			else if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
				{
					if (this->m_iZpGuiDaoSum_Rigth>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXI = TRUE ;
				}
			}
			else if (!bPassXN)
			{
				if (iGuiDaoId<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN	= TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			
			cSet.MovePrevious() ;
		}			
		cSet.Close() ;
		
		CStringArray * pstrArrayTmp_L5 = new CStringArray() ;
		pstrArrayTmp_L5->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmp_L5) ;
		
		//end 出站开放，全L5

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s,%s关闭)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}	
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			bBianMaChange = 0 ;			
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
					{
// 						if (this->m_iZpGuiDaoSum_Rigth>0)
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
					}
				}
				else if (!bPassXN)
				{
					if (iGuiDaoId<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					//straaa.Format(_T("%s\n%s-%d"), straaa, strRetBianMa, bBianMaChange) ;
					arrayBianMaChange.Add(bBianMaChange) ;
				}			
				
				cSet.MovePrevious() ;
			}

			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		int iLq_id = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;

		//右侧是在进站分界，倒置没有反向离去区段
		if (iLq_id==0)
		{
			strSql.Format(_T("select * from jinlu ")) ;			
		}
		else
		{
			strSql.Format(_T("select * from jinlu")) ;
		}
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述	
			if (iGuiDaoId>this->m_iChuZhanXHJ_id)
			{
				strDescribeTmp.Format(_T("%s占用(%s开放%s,%s关闭)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			}
			else
			{
				strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
			}
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码

			strRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
// 						if (iGuiDaoIdTmp==iGuiDaoId)
// 						{
// 							strRetBianMa = _T("") ;
// 						}
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id) 
					{
// 						if (this->m_iZpGuiDaoSum_Rigth>0)
// 						{
// 							strRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
					}
					//this->GetGuiDaoBianMa2(strGuiDaoName,strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Left>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN	= TRUE ;
						}
					}
					//this->GetGuiDaoBianMa2(strGuiDaoName,strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
					
				}
				
				this->GetGuiDaoBianMa2(strGuiDaoName,strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				
				if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
				{
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, _T("占用")) ;
				}

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		//XN开放直向，XII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		

		//情况描述:XN开放直向，XII关闭
		strDescribeTmp.Format(_T("%s开放%s,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}

		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;			
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN	= TRUE ;
				}
			}
			
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpSII = new CStringArray() ;
		pstrArrayTmpSII->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpSII) ;	
		
		if (m_iVersionFor == 1)
		{
			//情况描述:XN开放直向至XII，XII开放引导	
			strDescribeTmp.Format(_T("%s开放直向至%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;			
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSet.MovePrevious() ;
				
				// 			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				// 			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa, bBianMaChange, 2, 2, 1) ;
				// 			cSet.MovePrevious() ;			
			}
			CStringArray * pstrArrayTmpSIIYinDao = new CStringArray() ;
			pstrArrayTmpSIIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end XN开放直向，XII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
		//XN开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		//侧向		
		strDescribeTmp.Format(_T("%s开放侧向,%s关闭"), this->m_strJinZhanXHJ, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 1, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pstrArrayTmpS_C = new CStringArray() ;
		pstrArrayTmpS_C->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpS_C) ;	

		//关闭
		strDescribeTmp.Format(_T("%s进站关闭"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pstrArrayTmpS_CLOSE = new CStringArray() ;
		pstrArrayTmpS_CLOSE->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpS_CLOSE) ;	

		//XN开放引导
		strDescribeTmp.Format(_T("%s开放引导"), this->m_strJinZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 2, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pstrArrayTmpS_Y = new CStringArray() ;
		pstrArrayTmpS_Y->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpS_Y) ;	
		
		cSet.Close() ;	
		
		//end XN开放侧向、关闭、引导
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s_f")) ;
	} 

    //绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2, 0, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}

	}

}

void CMaXuBiaoForHD::JinLuBianMa_S_C_S_F()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;		
	
	BOOL bPassSI = FALSE ; //是否越过出站信号机（在其right侧为越过）
	BOOL bPassS = FALSE ; //是否越过进站信号机（在其right侧为越过）
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //编码的种类 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;

		//找出上进站,上出站
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s→%s→%s"),this->m_strJinZhanXHJ, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ_F) ;

		//end 找出上进站,上出站
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////		


		//出站开放，全L5
		//////////////////////////////////////////////////////////////////////////
		strDescribeTmp.Format(_T("%s开放%s通过,S口区间空闲"), this->m_strChuZhanXHJ, this->m_strGuDao) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		strArrayBianMaTmp.RemoveAll() ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		bPassSI = FALSE ;
		bPassS = FALSE ;
		iCodeTypeTmp = 0 ;
		strRetBianMa = _T("L5") ;		

		while (!cSet.IsBOF())
		{
			CString strGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;

			if (!bPassSI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassSI = TRUE ;						
				}					
			}
			else if (!bPassS)
			{
				if (iGuiDaoId<this->m_iJinZhanXHJ_id)
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassS = TRUE ;	
				}				
				
			}
			
			this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;

			cSet.MovePrevious() ;
		}			
		cSet.Close() ;
		
		CStringArray * pstrArrayTmp_L5 = new CStringArray() ;
		pstrArrayTmp_L5->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmp_L5) ;

		//end 出站开放，全L5


		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s,%s关闭)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}	
						
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassSI = FALSE ;
			bPassS = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						strRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassSI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassSI = TRUE ;						
					}					
				}
				else if (!bPassS)
				{
					if (iGuiDaoId<this->m_iJinZhanXHJ_id)
					{
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassS = TRUE ;	
					}				
					
				}

				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 2, 1) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		int iLq_id = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d and signal<>''"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;

		strSql.Format(_T("select * from jinlu ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s关闭)"), strGuiDaoName, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassSI = FALSE ;
			bPassS = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
					{
						strRetBianMa = _T("JC") ;
// 						if (iGuiDaoIdTmp==iGuiDaoId)
// 						{
// 							strRetBianMa = _T("") ;
// 						}
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;						
					}		
				}
				else if (!bPassSI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("占用") ;
						iCodeTypeTmp = 0 ;
						bPassSI = TRUE ;						
					}					
				}
				else
				{
					if (!bPassS)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassS = TRUE ;	
						}							
					}
					
				}	

				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange,iCodeTypeTmp, 2,1) ;

				if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
				{
					strRetBianMa = _T("占用") ;
					strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
				}

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
		//XN开放c向，XII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		

		//情况描述:XN开放c向，XII关闭
		strDescribeTmp.Format(_T("%s开放%s,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao,this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}

		strArrayBianMaTmp.RemoveAll() ;

		strRetBianMa = _T("占用") ;
		bPassS = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassS)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					strRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassS = TRUE ;
				}
			}

			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpSII = new CStringArray() ;
		pstrArrayTmpSII->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpSII) ;	
		
		if(m_iVersionFor == 1)
		{		
			//情况描述:XN开放至XII，XII开放引导
			
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			
			strRetBianMa = _T("") ;
			bPassS = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassS)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{	
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassS = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSet.MovePrevious() ;			
			}
			CStringArray * pstrArrayTmpSIIYinDao = new CStringArray() ;
			pstrArrayTmpSIIYinDao->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end XN开放c向，XII关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-in s-c-s_F"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in s-c-s_F")) ;
	} 

    //绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2, 0, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}

}

void CMaXuBiaoForHD::JinLuBianMa_S_ZC_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	CStringArray strArrayGuiDao ;	//各轨道名称
	CString strRetBianMa = _T("") ; // GetGuiDaoBianMa(...)的参数，用来返回得到的编码
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;	
	
	BOOL bPassXI = FALSE ; //是否越过下出站信号机
	BOOL bPassXN = FALSE ; //是否越过进站信号机
	int iCodeTypeTmp = 0 ; //编码的种类 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		
	
		//找出进站,出站
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_strJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_strChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_strJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_strChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_strGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_strCaseType.Format(_T("%s←%s←%s"),this->m_strJinZhanXHJ_F, this->m_strChuZhanXHJ, this->m_strJinZhanXHJ) ;

		//end 找出进站,出站		
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_strArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_strArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_strArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_strArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s (%s开放%s通过)"), g_strLinZhanState_1[iDesIndex], this->m_strJinZhanXHJ,this->m_strGuDao) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strRetBianMa = g_strLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;
			
			//CString straaa ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;

				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 2) ;
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;					
					
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}
					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoId>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp, 1, 1) ;
					
					this->GetGuiDaoBianMa2(strGuiDaoTmp, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;					
					
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}		
					
				}
				
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa,iCodeTypeTmp) ;
					
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
	
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;
			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
			
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	
		int iLq_id = 0 ;
		strSql.Format(_T("select * from jinlu where guidao_updown=2 and id<%d and signal<>''"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;
				
		//if id=0, record is null
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}		
		
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;
			
			CString strGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//情况描述			
			strDescribeTmp.Format(_T("%s占用(%s开放%s通过)"), strGuiDaoName, this->m_strJinZhanXHJ, this->m_strGuDao) ;			
			strArrayDescribe.Add(strDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			bPassXI = iGuiDaoId>this->m_iChuZhanXHJ_id?TRUE:FALSE ;
			bPassXN = iGuiDaoId>this->m_iJinZhanXHJ_id?TRUE:FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;

				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id  )
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0) 
						{
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;							
						}
						bPassXI = TRUE ;						
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;

					if (iGuiDaoId==iGuiDaoIdTmp && iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						strRetBianMa = _T("占用") ;
						strArrayBianMaTmp.SetAt(strArrayBianMaTmp.GetSize()-1, strRetBianMa) ;
					}

				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
							{
								strRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
					
				}
				//this->GetGuiDaoBianMa(strGuiDaoName, strArrayBianMaTmp, strRetBianMa, iCodeTypeTmp) ;				
				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						
			
			cSet.MoveNext() ;
		}
		//end while
	
		cSet.Close() ;
		
		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////

		//sn开放，sI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////		
		
		//情况描述:sn开放，sI关闭
		strDescribeTmp.Format(_T("%s开放%s接车,%s关闭"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
		strArrayDescribe.Add(strDescribeTmp) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		strArrayBianMaTmp.RemoveAll() ;
		strRetBianMa = _T("占用") ;		
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
					{						
						strRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pstrArrayTmpXI = new CStringArray() ;
		pstrArrayTmpXI->Copy(strArrayBianMaTmp) ;
		arrayBianMa.Add(pstrArrayTmpXI) ;	

		if (m_iVersionFor == 1)
		{		
			//begin yd
			strDescribeTmp.Format(_T("%s开放%s,%s开放引导"), this->m_strJinZhanXHJ, this->m_strGuDao, this->m_strChuZhanXHJ) ;
			strArrayDescribe.Add(strDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			strArrayBianMaTmp.RemoveAll() ;
			strRetBianMa = _T("") ;		
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
						{						
							strRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(strGuiDao, strArrayBianMaTmp, strRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
				cSet.MoveNext() ;			
			}
			//cSet.Close() ;	
			CStringArray * pstrArrayTmpyd = new CStringArray() ;
			pstrArrayTmpyd->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmpyd) ;	
			
			//end yd
		}

		cSet.Close() ;	
		
		//end SN开放，SI关闭,或者引导
		//////////////////////////////////////////////////////////////////////////
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_zc_x_F"), sBuff) ;		
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_zC_X_F")) ;			
	} 
	
	//绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}		
	}
}

void CMaXuBiaoForHD::JinLuBianMa_ZJZ_X()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	this->m_iLinZhanStateSum = 9 ;	

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;
		this->m_strCaseType.Format(_T("1.下行正向")) ;
		
		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		CString strStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("占用")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s"), strStartCodeTmp[iDesIndex]) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}			
			
			strRetBianMa = strStartCodeTmp[iStartCodeNum] ;
			
			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange) ;
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}			
				
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		strSql.Format(_T("select * from jinlu where guidao_updown=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述		
			strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
				
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			while (!cSetTmp.IsBOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange) ;

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-zjz_x"),sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error zjz_x")) ;
	} 

	//绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2) ;
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}
}

void CMaXuBiaoForHD::JinLuBianMa_ZJZ_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	this->m_iLinZhanStateSum = 9 ;	

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;
		this->m_strCaseType.Format(_T("2.下行反向")) ;

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		CString strStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("占用")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s"), strStartCodeTmp[iDesIndex]) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			
			strRetBianMa = strStartCodeTmp[iStartCodeNum] ;
			
			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, 0, 1, 1) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, 0, 1, 1) ;				
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}				
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////
		
		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////

		//左侧是在进站分界，倒置没有反向离去区段
		strSql.Format(_T("select * from jinlu where guidao_updown=1")) ;
		
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述		
			strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			while (!cSetTmp.IsEOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 1, 1) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-zjz_x_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error zjz_x_f")) ;
	} 

	//绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe,arrayBianMaChange, 1) ;
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}
	}	

}

void CMaXuBiaoForHD::JinLuBianMa_ZJZ_S()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	this->m_iLinZhanStateSum = 9 ;
	

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;	
		this->m_strCaseType.Format(_T("1.上行正向")) ;

		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		CString strStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("占用")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s"), strStartCodeTmp[iDesIndex]) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}			
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			
			strRetBianMa = strStartCodeTmp[iStartCodeNum] ;
			
			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, 0, 2) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, 0, 2) ;
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////		
		strSql.Format(_T("select * from jinlu where guidao_updown=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsEOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//情况描述	
			strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			while (!cSetTmp.IsEOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 2) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s")) ;
	} 

    //绘制码序表
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange, 1) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}

	}
}

void CMaXuBiaoForHD::JinLuBianMa_ZJZ_S_F()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	this->m_iLinZhanStateSum = 9 ;
	

	//各轨道名称
	CStringArray strArrayGuiDao ;
	CString strRetBianMa = _T("") ;
	CString strDescribeTmp = _T("") ;
	CStringArray strArrayDescribe ; //情况描述array	
	CStringArray strArrayBianMaTmp ;//保存一种情况下的一行编码
	CString strGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//将进路中所有轨道存入strArrayGuiDao	
		this->GetJinLuGuiDao(strArrayGuiDao) ;		
		this->m_strCaseType.Format(_T("2.上行反向")) ;
		
		//临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		
		CString strStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("占用")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			strDescribeTmp.Format(_T("(邻站) %s"), strStartCodeTmp[iDesIndex]) ;
			strArrayDescribe.Add(strDescribeTmp) ;
		}			
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			strArrayBianMaTmp.RemoveAll() ;
			strSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, strSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}			
			
			strRetBianMa = strStartCodeTmp[iStartCodeNum] ;

			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString strGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), strGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, 0, 2, 1) ;
				this->GetGuiDaoBianMa2(strGuiDaoTmp,strArrayBianMaTmp, strRetBianMa, bBianMaChange, 0, 2, 1) ;
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}				
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pstrArrayTmp = new CStringArray() ;
			pstrArrayTmp->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp) ;			
		}	
		//end for
		//end 临站是"L5,L4,...HU,占用"的9种情况
		//////////////////////////////////////////////////////////////////////////		

		//各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////
		strSql.Format(_T("select * from jinlu where guidao_updown=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//遍历每个区间轨道
		while (!cSet.IsBOF()) 
		{
			//清除记录编码的array，准备下一条编码序列；
			strArrayBianMaTmp.RemoveAll() ;

			CString strGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), strGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			strDescribeTmp.Format(_T("%s占用"), strGuiDaoName) ;
			strArrayDescribe.Add(strDescribeTmp) ;

			CADORecordset cSetTmp ;
			strSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//一行编码
			strRetBianMa = _T("") ;
			while (!cSetTmp.IsBOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), strGuiDaoName) ;
				this->GetGuiDaoBianMa2(strGuiDaoName, strArrayBianMaTmp, strRetBianMa,bBianMaChange, 0, 2, 1) ;

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pstrArrayTmp1 = new CStringArray() ;
			pstrArrayTmp1->Copy(strArrayBianMaTmp) ;
			arrayBianMa.Add(pstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end 各个区间轨道占用情况
		//////////////////////////////////////////////////////////////////////////	
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s_f")) ;
	} 

    //绘制码序表
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}	
	this->DrawBianMaTable(arrayBianMa, strArrayGuiDao, strArrayDescribe, arrayBianMaChange2) ;	
	//end 绘制码序表
	//////////////////////////////////////////////////////////////////////////
	
	if (arrayBianMa.GetSize()>0)
	{
		for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
		{
			delete arrayBianMa[iBianMa] ;
			arrayBianMa[iBianMa] = NULL ;
		}

	}
}

//对于8字进路，本函数可能会走入死循环，增加strPassGuiDao变量，判断下一轨道是否为前一轨道，但是这样不能完全避开死循环（例如：下行一个区段有2个道岔可入，上行分别入2个区段）
//为避免死循环，每次遍历到一个区段，检查当前走过的的进路中是否已经走过该区段，若走过，就废弃当前进路.
void CMaXuBiaoForHD::FindJinLu(CString strStart, CString strEnd, CString strPassGuiDao, CStringArray &strArray)
{	
// 	CString strMsg ;
// 	CBlkUtility::ConvertArrayToString(strArray, _T("-"),  strMsg) ;
// 	acutPrintf(_T("\nstart:%s, end:%s, strarray:%s"), strStart, strEnd, strMsg) ;
	CStringArray strArray1 ;
	strArray1.Copy(strArray) ;

	strArray1.Add(strStart) ;
	if (strStart!=strEnd)
	{
		try
		{
			CADORecordset myset ;
			CString strSql ;
			strSql.Format(_T("select * from guidao where name='%s'"), strStart) ;
			myset.Open(g_PtrCon, strSql) ;
			
			if (!myset.IsBOF())
			{
				myset.MoveFirst() ;
			}
			while (!myset.IsEOF())
			{
				CString strNext1, strNext2, strNext3 ;
				myset.GetFieldValue(_T("next1"), strNext1) ;
				myset.GetFieldValue(_T("next2"), strNext2) ;
				myset.GetFieldValue(_T("next3"), strNext3) ;
				//CString strMsg ;
				//strMsg.Format(_T("%s-%s-%s"),strStart,strNext1,strNext2) ;
				//AfxMessageBox(strMsg) ;
				if (strNext1!=_T("") && CBlkUtility::FindStrInArray(strArray1, strNext1)==-1)
				{
				
					//m_Test_pt[1] = m_Test_pt[1] + 3 ;
					//g_cSystem.WriteText(m_Test_pt, strNext1, 0, 9, 2) ;
					this->FindJinLu(strNext1, strEnd, strStart,strArray1) ;
					
				}
				if (strNext2!=_T("") && CBlkUtility::FindStrInArray(strArray1, strNext2)==-1) 
				{
					//m_Test_pt[0] = m_Test_pt[0] + 3 ;
					//m_Test_pt[1] = m_Test_pt[1] + 3 ;
					//g_cSystem.WriteText(m_Test_pt, strNext2, 0, 9, 2) ;
					
					this->FindJinLu(strNext2, strEnd, strStart, strArray1) ;
				}
				if (strNext3!=_T("") && CBlkUtility::FindStrInArray(strArray1, strNext3)==-1) 
				{
					//m_Test_pt[0] = m_Test_pt[0] + 3 ;
					//m_Test_pt[1] = m_Test_pt[1] + 3 ;
					//g_cSystem.WriteText(m_Test_pt, strNext3, 0, 9, 2) ;
					
					this->FindJinLu(strNext3, strEnd, strStart, strArray1) ;
				}
				
				myset.MoveNext() ;
			}
			myset.Close() ;			
		}
		catch (_com_error& e)
        {
			CString sBuff = CBlkUtility::GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in FindJinLu")) ;			
		}       
	}
	else//到达末端
	{
		if (strArray1.GetSize()>2)
		{
			CStringArray * strArrayTmp = new CStringArray ;
			strArrayTmp->Copy(strArray1) ;
			this->arrayJinLu.Add(strArrayTmp) ;
// 			CString strMsg =_T(_T("")) ;
// 			for(int ii=0; ii<strArray1.GetSize(); ii++)
// 			{
// 				strMsg.Format(_T("%s-%s"), strMsg, strArray1.GetAt(ii)) ;
// 				//ads_printf(_T("%s\n"), strArray1.GetAt(ii)) ;
// 			}
// 			AfxMessageBox(strMsg) ;
// 			//ads_printf(_T("\n\n")) ;
			
		}
	}
}

//取得一个区段的编码
BOOL CMaXuBiaoForHD::GetGuiDaoBianMa2(CString strGuiDao, CStringArray &strArrayBianMa, CString &strRetBianMa, BOOL &bBianMaChange, int iCodeType, int iUpDown, int iDirect)
{
	BOOL bRet = TRUE ;

	//前方占用，后方闭塞分区必须HU码
	if (strRetBianMa==_T(""))
	{
		int iBianMaCount = strArrayBianMa.GetCount() ;
		if (iBianMaCount>0)
		{
			CString strLastBianMa = strArrayBianMa.GetAt(iBianMaCount-1) ;
			if(strLastBianMa==_T("占用")||strLastBianMa==_T("JC"))
			{
				strRetBianMa = _T("HU") ;
				strArrayBianMa.Add(strRetBianMa) ;
				bBianMaChange = !bBianMaChange ;
				return bRet ;
			}
		}
	}
	if (strRetBianMa==_T("占用")||strRetBianMa==_T("HU")||strRetBianMa==_T("U"))//因为加了上方代码，强制将码转为HU(iCodeType=0,而其实此时iCodeType=1)
	{
		iCodeType = 0 ;
	}


	CString strRetBianMaTmp ;
	strRetBianMaTmp.Format(_T("%s"), strRetBianMa) ;
	CStringList strList;
	switch(iCodeType) {
	case 0:
		{			
			for(int i=0; i<9; i++)
			{
				strList.AddTail(g_strMaXu_1[i]) ;
			}
		}		
		break;
	case 1:
		{	
			if (m_iVersionFor == 1)
			{
				for(int i=0; i<8; i++)
				{
					strList.AddTail(g_strMaXu_CeXiang2_1[i]) ;
				}
				
			}
			else
			{
				for(int i=0; i<8; i++)
				{
					strList.AddTail(g_strMaXu_CeXiang_1[i]) ;
				}
			}
		}		
		break;
	case 2:
		{			
			for(int i=0; i<8; i++)
			{
				strList.AddTail(g_strMaXu_YinDao_1[i]) ;
			}
		}		
		break;		
	default:
		break;
	}
	
	int iGuiDao_id = 0 ;
	try
	{
		CADORecordset cSet_1 ;
		CString strSql_1 ;
		strSql_1.Format(_T("select id from jinlu where guidao='%s'"), strGuiDao) ;
		if (cSet_1.IsOpen())
		{
			cSet_1.Close() ;
		}
		cSet_1.Open(g_PtrCon,strSql_1) ;
		if (cSet_1.GetRecordCount()>0)
		{
			if (!cSet_1.IsBOF())
			{
				cSet_1.MoveFirst() ;
			}
			cSet_1.GetFieldValue(_T("id"), iGuiDao_id) ;
		}
		else
		{
			AfxMessageBox(_T("coun't find guidao_id -- GetGuiDaoBianMa")) ;
		}

		cSet_1.Close() ;
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGuiDaoBianMa2-find guidao_id")) ;
	} 
	//进路从左往右
	if (this->m_iCurJinLuDirect==0)
	{
		//此轨道位于从进站到出站
		if (iGuiDao_id>=this->m_iJinZhanXHJ_id && iGuiDao_id<this->m_iChuZhanXHJ_F_id)
		{
			if (this->m_iZpGuiDaoSum_Left>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Left; iZpGuiDaoNum++)
				{
					CString strGuiDao_Zp_Tmp ;
					strGuiDao_Zp_Tmp = this->m_strArrayGuiDao_ZP_Left.GetAt(iZpGuiDaoNum) ;
					if (strGuiDao_Zp_Tmp==strGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//如果是转频轨道，则为ZP/JC，否则为JC
				if (bIsZp) 
				{
					strArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					strArrayBianMa.Add(_T("JC")) ;
				}
				strRetBianMa = _T("JC") ;
				/*if (strRetBianMa!=strRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}
		}
		//此轨道位于从出站到反向进站
		else if (iGuiDao_id>this->m_iChuZhanXHJ_id && iGuiDao_id<=this->m_iJinZhanXHJ_F_id ) 
		{
			if (this->m_iZpGuiDaoSum_Rigth>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Rigth; iZpGuiDaoNum++)
				{
					CString strGuiDao_Zp_Tmp ;
					strGuiDao_Zp_Tmp = this->m_strArrayGuiDao_ZP_Right.GetAt(iZpGuiDaoNum) ;
					if (strGuiDao_Zp_Tmp==strGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//如果是转频轨道，则为ZP/JC，否则为JC
				if (bIsZp) 
				{
					strArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					strArrayBianMa.Add(_T("JC")) ;
				}
				strRetBianMa = _T("JC") ;
				/*if (strRetBianMa!=strRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}
			
		}
		
	}
	//进路从右往左	
	else if (this->m_iCurJinLuDirect==1)
	{
		if (iGuiDao_id>=this->m_iJinZhanXHJ_F_id && iGuiDao_id<this->m_iChuZhanXHJ_id)
		{
			if (this->m_iZpGuiDaoSum_Left>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Left; iZpGuiDaoNum++)
				{
					CString strGuiDao_Zp_Tmp ;
					strGuiDao_Zp_Tmp = this->m_strArrayGuiDao_ZP_Left.GetAt(iZpGuiDaoNum) ;
					if (strGuiDao_Zp_Tmp==strGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//如果是转频轨道，则为ZP/JC，否则为JC
				if (bIsZp) 
				{
					strArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					strArrayBianMa.Add(_T("JC")) ;
				}
				strRetBianMa = _T("JC") ;
				/*if (strRetBianMa!=strRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}
		}
		else if (iGuiDao_id>this->m_iChuZhanXHJ_F_id && iGuiDao_id<=this->m_iJinZhanXHJ_id ) 
		{
			if (this->m_iZpGuiDaoSum_Rigth>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Rigth; iZpGuiDaoNum++)
				{
					CString strGuiDao_Zp_Tmp ;
					strGuiDao_Zp_Tmp = this->m_strArrayGuiDao_ZP_Right.GetAt(iZpGuiDaoNum) ;
					if (strGuiDao_Zp_Tmp==strGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//如果是转频轨道，则为ZP/JC，否则为JC
				if (bIsZp) 
				{
					strArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					strArrayBianMa.Add(_T("JC")) ;
				}
				strRetBianMa = _T("JC") ;
				/*if (strRetBianMa!=strRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}			
		}		

	}


	//下行
	if (iUpDown==1) 
	{
		//正向（下行）
		if (iDirect==0)
		{
			if (strRetBianMa == _T(""))
			{
				strRetBianMa = strList.GetHead() ;
				bBianMaChange = !bBianMaChange ;				
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = strList.Find(strRetBianMa) ;
				posEnd = strList.Find(_T("L5")) ;
				
				try
				{
					CString strSql ;
					CADORecordset cSet ;
					
					strSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, strSql) ;
					strSql.Format(_T("guidao='%s'"),strGuiDao) ;
					cSet.Find(strSql) ;		
					
					POSITION pos ;
					pos = strList.Find(strRetBianMa) ;
					
					if (!cSet.IsEOF()) 
					{
						cSet.MoveNext() ;
					}
					
					//不是最后一个轨道		
					if (!cSet.IsEOF())
					{
						//CString strGuiDao ;
						CString strGuiDao_XHJ, strGuiDao_XHJ2 ;

						int iGuiDao_XHJ_UpDown = 0 ; 
						int iGuiDao_XHJ_Type = 0 ;
						int iGuiDao_Type = 0 ;
						
						CString strPreGuiDao ;
						CString strPreGuiDao_XHJ ;
						int iPreGuiDao_XHJ_UpDown = 0 ;	
						int iPreGuiDao_XHJ_Type = 0 ;
						int iPreGuiDao_XHJ_Direct = 0 ;
						int iPreGuiDao_Type = 0 ;
						
						cSet.GetFieldValue(_T("guidao"), strPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), strPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;
						cSet.GetFieldValue(_T("guidao_type"), iPreGuiDao_Type) ;
						
						cSet.MovePrevious() ;
						
						//cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
						cSet.GetFieldValue(_T("signal"), strGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal2"), strGuiDao_XHJ2) ;
						cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;	
						cSet.GetFieldValue(_T("guidao_type"), iGuiDao_Type) ;
											
						//不同闭塞分区，需要变更发码
						//此时前方为右方
						//1.前方信号机不为空+前方信号机为下行+前方信号机不是出站(+前方信号机不是反向)（股道分割时的通过或进站前轨道）
						//2.本轨信号机不为空+本轨信号机为下行+本轨信号机为出站（股道分割时的G2，可能不分割时的股道也刚好符合）
						//3.本轨信号机不为空+本轨道为股道+本信号机2也不为空（股道不分割时）						
						if ((strPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==1 && iPreGuiDao_XHJ_Type!=2  && !(iPreGuiDao_XHJ_Type==1&&iPreGuiDao_XHJ_Direct==1)/*&& iPreGuiDao_XHJ_Direct!=1*/) 
							||(strGuiDao_XHJ!=_T("") && iGuiDao_XHJ_UpDown==1 && iGuiDao_XHJ_Type==2 )
							||(strGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && strGuiDao_XHJ2!=_T("")) )
						{
							//bBianMaChange = !bBianMaChange ;
							if (posEnd != pos)
							{
								if (strRetBianMa == _T("JC"))
								{
									strRetBianMa = _T("HU") ;
								}
								else
								{
									strList.GetNext(pos) ;
									strRetBianMa = strList.GetAt(pos) ;									
								}
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
						}
						else
						{
							if (strRetBianMa==_T("占用"))
							{
								strRetBianMa = _T("JC") ;
							}
						}
					}
					//最后一个轨道的情况
					else
					{
						cSet.MovePrevious() ;
						int iLastGuiDaoXHJ = 0 ;
						cSet.GetFieldValue(_T("signal_type"), iLastGuiDaoXHJ) ;
						//signal_type=1表示是进站信号机
						if (iLastGuiDaoXHJ!=1)
						{
							if (pos!=posEnd)
							{
								strList.GetNext(pos) ;
								strRetBianMa = strList.GetAt(pos) ;
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
						}
					}
					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					CString sBuff = CBlkUtility::GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error GetGuiDaoBianMa2 正向（下行）")) ;
				} 
			}
			
			strArrayBianMa.Add(strRetBianMa) ;
		}
		//反向（下行）
		else if (iDirect==1)
		{
			if (strRetBianMa == _T(""))
			{
				strRetBianMa = strList.GetHead() ;
				bBianMaChange = !bBianMaChange ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = strList.Find(strRetBianMa) ;
				posEnd = strList.Find(_T("L5")) ;
				
				try
				{
					CString strSql ;
					CADORecordset cSet ;
					
					strSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, strSql) ;
					strSql.Format(_T("guidao='%s'"),strGuiDao) ;
					cSet.Find(strSql) ;		
					
					POSITION pos ;
					pos = strList.Find(strRetBianMa) ;

					CString strGuiDao_XHJ, strGuiDao_XHJ2 ;
					int iGuiDao_XHJ_UpDown = 0 ; 
					int iGuiDao_XHJ_Type = 0 ;
					int iGuiDao_XHJ_Direct = 0 ;
					int iGuiDao_Type = 0 ;
										
					CString strPreGuiDao ;
					CString strPreGuiDao_XHJ ;
					int iPreGuiDao_UpDown = 0 ;
					int iPreGuiDao_XHJ_UpDown = 0 ;	
					int iPreGuiDao_XHJ_Type = 0 ;
					int iPreGuiDao_XHJ_Direct = 0 ;

					cSet.GetFieldValue(_T("signal"), strGuiDao_XHJ) ;
					cSet.GetFieldValue(_T("signal2"), strGuiDao_XHJ2) ;
					cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
					cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;
					cSet.GetFieldValue(_T("signal_direct"), iGuiDao_XHJ_Direct) ;
				
					cSet.MovePrevious() ;

					BOOL bIsFirstGd = TRUE ;
					
					//不是first一个轨道		
					if (!cSet.IsBOF())
					{
						bIsFirstGd = FALSE ;
						cSet.GetFieldValue(_T("guidao"), strPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), strPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("guidao_updown"), iPreGuiDao_UpDown) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;
					}
					
					//不同闭塞分区，需要变更发码					
					//此时前方变成了左方
					//根据“1LQ、2LQ区段在反向码序表中合并为一个区段”改进第二条
					//1.前方信号机不为空+前方信号机为上行+前方信号机是进站+前方信号机是反向（LQ区段）
					//2.本轨信号机不为空+本轨信号机为区间通过+前方轨道不是站内或者LQ（每遇到一个有区间通过信号机的区段就要变更发码）//20110608:1LQ有分隔的情况，增加&& strGuiDao!=_T("X1LQAG")
					//3.本轨信号机不为空+本信号机为出站+本信号机为上行（股道分割时的G1）
					//4.本轨信号机不为空+本轨道为股道+本信号机2也不为空（股道不分割时）
					//20100831:解决临站和本站第一个轨道编码一样的情况
					//5.
					if ((strPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==2 && iPreGuiDao_XHJ_Type==1 && iPreGuiDao_XHJ_Direct==1 ) 
						||(strGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==0 && (iPreGuiDao_UpDown!=0&&strPreGuiDao!=_T("X1LQAG")) ) 
						|| (strGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==2 && iGuiDao_XHJ_UpDown==2)
						|| (strGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && strGuiDao_XHJ2!=_T(""))
						||(strGuiDao_XHJ!=_T("") && iGuiDao_Type==0 && bIsFirstGd)) 
					{
						//bBianMaChange = !bBianMaChange ;
						if (posEnd != pos)
						{
							if (strRetBianMa == _T("JC"))
							{
								strRetBianMa = _T("HU") ;
							}
							else
							{
								strList.GetNext(pos) ;
								strRetBianMa = strList.GetAt(pos) ;
							}
						}
						//bianmachange
						bBianMaChange = !bBianMaChange ;
						
					}
					else
					{					
						if (strRetBianMa==_T("占用"))
						{
							strRetBianMa = _T("JC") ;
						}
					}
					
					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					CString sBuff = CBlkUtility::GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error GetGuiDaoBianMa2 反向（下行）")) ;
				} 
			}
			
			strArrayBianMa.Add(strRetBianMa) ;
		}

		
	}
	//上行
	else if (iUpDown==2)
	{
		//正向（上行）
		if (iDirect==0)
		{
			if (strRetBianMa == _T(""))
			{
				strRetBianMa = strList.GetHead() ;
				bBianMaChange = !bBianMaChange ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = strList.Find(strRetBianMa) ;
				posEnd = strList.Find(_T("L5")) ;
				
				try
				{
					CString strSql ;
					CADORecordset cSet ;
					
					strSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, strSql) ;
					strSql.Format(_T("guidao='%s'"),strGuiDao) ;
					cSet.Find(strSql) ;		
					
					POSITION pos ;
					pos = strList.Find(strRetBianMa) ;
					
					cSet.MovePrevious() ;
					
					//不是第一个轨道		
					if (!cSet.IsBOF())
					{
						//CString strGuiDao ;
						CString strGuiDao_XHJ, strGuiDao_XHJ2 ;
						int iGuiDao_XHJ_UpDown = 0 ; 
						int iGuiDao_XHJ_Type = 0 ;
						int iGuiDao_Type = 0 ;
						
						CString strPreGuiDao ;
						CString strPreGuiDao_XHJ ;
						int iPreGuiDao_XHJ_UpDown = 0 ;	
						int iPreGuiDao_XHJ_Type = 0 ;
						int iPreGuiDao_XHJ_Direct = 0 ;
						
						cSet.GetFieldValue(_T("guidao"), strPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), strPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;
						
						cSet.MoveNext() ;
						
						//cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
						cSet.GetFieldValue(_T("signal"), strGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal2"), strGuiDao_XHJ2) ;
						cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;						
						
						//不同闭塞分区，需要变更发码
						//此时前方是左方
						//1.前方信号机不为空+前方信号机为上行+前方信号机不是出站(+前方信号机不是反向)（股道分割时的通过或进站前轨道）
						//2.本轨信号机不为空+本轨信号机为上行+本轨信号机为出站（股道分割时的G2，可能不分割时的股道也刚好符合）
						//3.本轨信号机不为空+本轨道为股道+本信号机2也不为空（股道不分割时）
						if ((strPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==2 && iPreGuiDao_XHJ_Type!=2 && !(iPreGuiDao_XHJ_Type==1&&iPreGuiDao_XHJ_Direct==1)/*&& iPreGuiDao_XHJ_Direct!=1*/ ) 
							||(strGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==2 && iGuiDao_XHJ_UpDown==2)
							||(strGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && strGuiDao_XHJ2!=_T("") )) 
						{
							//bBianMaChange = !bBianMaChange ;
							if (posEnd != pos)
							{
								if (strRetBianMa == _T("JC"))
								{
									strRetBianMa = _T("HU") ;
								}
								else
								{
									strList.GetNext(pos) ;
									strRetBianMa = strList.GetAt(pos) ;
								}
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
							
						}
						else
						{
							if (strRetBianMa==_T("占用"))
							{
								strRetBianMa = _T("JC") ;
							}
						}
					}
					//第一个轨道的情况
					else
					{
						cSet.MoveNext() ;
						int iFirstGuiDaoXHJ = 0 ;
						cSet.GetFieldValue(_T("signal_type"), iFirstGuiDaoXHJ) ;
						if (iFirstGuiDaoXHJ!=1)
						{
							if (pos!=posEnd)
							{
								strList.GetNext(pos) ;
								strRetBianMa = strList.GetAt(pos) ;
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
							
						}
							

					}
					
					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					CString sBuff = CBlkUtility::GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error GetGuiDaoBianMa2 正向（上行）")) ;
				} 
			}
			
			strArrayBianMa.Add(strRetBianMa) ;
			
		}
		//反向（上行）
		else if (iDirect==1)
		{
			if (strRetBianMa == _T(""))
			{
				strRetBianMa = strList.GetHead() ;
				bBianMaChange = !bBianMaChange ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = strList.Find(strRetBianMa) ;
				posEnd = strList.Find(_T("L5")) ;
				
				try
				{
					CString strSql ;
					CADORecordset cSet ;
					
					strSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, strSql) ;
					strSql.Format(_T("guidao='%s'"),strGuiDao) ;
					cSet.Find(strSql) ;		
					
					POSITION pos ;
					pos = strList.Find(strRetBianMa) ;		
					
					CString strGuiDao_XHJ, strGuiDao_XHJ2 ;
					int iGuiDao_XHJ_UpDown = 0 ; 
					int iGuiDao_XHJ_Type = 0 ;
					int iGuiDao_XHJ_Direct = 0 ;
					int iGuiDao_Type = 0 ;

					CString strPreGuiDao ;
					CString strPreGuiDao_XHJ ;
					int iPreGuiDao_UpDown = 0 ;
					int iPreGuiDao_XHJ_UpDown = 0 ;	
					int iPreGuiDao_XHJ_Type = 0 ;
					int iPreGuiDao_XHJ_Direct = 0 ;

					//cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
					cSet.GetFieldValue(_T("signal"), strGuiDao_XHJ) ;	
					cSet.GetFieldValue(_T("signal2"), strGuiDao_XHJ2) ;
					cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
					cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;
					cSet.GetFieldValue(_T("signal_direct"), iGuiDao_XHJ_Direct) ;

					cSet.MoveNext() ;

					BOOL bIsLastGd = TRUE ;
					
					//不是最后一个轨道		
					if (!cSet.IsEOF())
					{
						bIsLastGd = FALSE ;
						cSet.GetFieldValue(_T("guidao"), strPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), strPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("guidao_updown"), iPreGuiDao_UpDown) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;					
					}
					
					//不同闭塞分区，需要变更发码
					//此时前方是右方
					//根据“1LQ、2LQ区段在反向码序表中合并为一个区段”改进第二条
					//1.前方信号机不为空+前方信号机为下行+前方信号机是进站+前方信号机是反向（LQ区段）
					//2.本轨信号机不为空+本轨信号机为区间通过+ 前方轨道不是站内或者LQ（每遇到一个有区间通过信号机的区段就要变更发码） //20110608:1LQ有分隔的情况，增加&& strGuiDao!=_T("S1LQAG")
					//3.本轨信号机不为空+本信号机为出站+本信号机为下行（股道分割时的G1）
					//4.本轨信号机不为空+本轨道为股道+本信号机2也不为空（股道不分割时）
					//20100831:解决临站和本站第一个轨道编码一样的情况
					//5.
					if ((strPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==1 && iPreGuiDao_XHJ_Type==1  && iPreGuiDao_XHJ_Direct==1) 
						||(strGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==0 && (iPreGuiDao_UpDown!=0 && strPreGuiDao!=_T("S1LQAG"))) 
						||(strGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==2 && iGuiDao_XHJ_UpDown==1 )
						||(strGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && strGuiDao_XHJ2!=_T(""))
						||(strGuiDao_XHJ!=_T("") && iGuiDao_Type==0 && bIsLastGd)) 
					{
						//bBianMaChange = !bBianMaChange ;						
						if (posEnd != pos)
						{
							if (strRetBianMa == _T("JC"))
							{
								strRetBianMa = _T("HU") ;
							}
							else
							{
								strList.GetNext(pos) ;
								strRetBianMa = strList.GetAt(pos) ;
							}
						}
						//bianmachange
						bBianMaChange = !bBianMaChange ;
						
					}
					else
					{
						if (strRetBianMa==_T("占用"))
						{
							//需要增加对股道2的判断
							if (iGuiDao_XHJ_Type==2)
							{
								strList.GetNext(pos) ;
								strRetBianMa = strList.GetAt(pos) ;
								bBianMaChange = !bBianMaChange ;
							}
							else
							{
								strRetBianMa = _T("JC") ;
							}
							
						}
					}

					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					bRet = FALSE ;
					CString sBuff = CBlkUtility::GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					bRet = FALSE ;
					AfxMessageBox(_T("unknown error GetGuiDaoBianMa2 反向（上行）")) ;
				} 
			}
			
			strArrayBianMa.Add(strRetBianMa) ;			
			
		}
	}
	/*if (strRetBianMa!=strRetBianMaTmp)
	{
		bBianMaChange = !bBianMaChange ;
	}
	*/
	return bRet ;

}

//取得进站、出站、反向进站、反向出站、股道、股道类型
BOOL CMaXuBiaoForHD::GetJinZhanOrChuZhan(CString &strJinZhanXHJ, int &iJinZhanXHJ_Id, CString &strChuZhanXHJ, int &iChuZhanXHJ_Id, CString &strJinZhanXHJ_F, int &iJinZhanXHJ_F_Id, CString &strChuZhanXHJ_F, int &iChuZhanXHJ_F_Id, CString &strGuDao, int &iGuDaoType, int iDirect)
{
	BOOL bRet = TRUE ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		//find进站
		strSql.Format(_T("select * from jinlu where signal_type=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;			
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>=2) 
		{
			//进站为X or XN
			if (iDirect==0) 
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_Id) ;
					cSet.GetFieldValue(_T("signal"), strJinZhanXHJ) ;
					cSet.MoveLast() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_F_Id) ;
					cSet.GetFieldValue(_T("signal"), strJinZhanXHJ_F) ;
				}
			}
			//进站为S or SN
			else
			{
				if (!cSet.IsEOF())
				{
					cSet.MoveLast() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_Id) ;
					cSet.GetFieldValue(_T("signal"), strJinZhanXHJ) ;
					cSet.MoveFirst() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_F_Id) ;
					cSet.GetFieldValue(_T("signal"), strJinZhanXHJ_F) ;
				}
			}
		}
		else
		{
			AfxMessageBox(_T("进路找不到2个进站-GetJinZhanOrChuZhan")) ;
		}
		cSet.Close() ;

		//find 出站
		//strSql.Format(_T("select * from jinlu where signal_type=2 and signal_updown=1")) ;
		strSql.Format(_T("select * from jinlu where signal_type=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;			
		}
		cSet.Open(g_PtrCon, strSql) ;
		int iRecordCount = cSet.GetRecordCount() ; 
		if (iRecordCount>0)
		{	
			//得到股道类型、名称
			cSet.GetFieldValue(_T("guidao_type"), iGuDaoType) ;			
			cSet.GetFieldValue(_T("guidao"), strGuDao) ;

			if (iRecordCount==2) 
			{
				//出站为XI 
				if (iDirect==0) 
				{
					if (!cSet.IsEOF())
					{
						cSet.MoveLast() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_Id) ;
						cSet.GetFieldValue(_T("signal"), strChuZhanXHJ) ;
						cSet.MoveFirst() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_F_Id) ;
						cSet.GetFieldValue(_T("signal"), strChuZhanXHJ_F) ;
					}
				}
				//出站为SI
				else
				{
					if (!cSet.IsBOF())
					{
						cSet.MoveFirst() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_Id) ;
						cSet.GetFieldValue(_T("signal"), strChuZhanXHJ) ;
						cSet.MoveLast() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_F_Id) ;
						cSet.GetFieldValue(_T("signal"), strChuZhanXHJ_F) ;
					}
				}
				//带分割的股道名称如4G1、4G2要去掉末尾数字
				strGuDao = strGuDao.Left(strGuDao.GetLength()-1) ;		
				
			}
			else if (iRecordCount==1)
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				cSet.GetFieldValue(_T("id"), iChuZhanXHJ_Id) ;
				iChuZhanXHJ_F_Id = iChuZhanXHJ_Id ;
				
				int iChuZhanXHJ_Updown_Tmp = 0 ;
				cSet.GetFieldValue(_T("signal_updown"), iChuZhanXHJ_Updown_Tmp) ;
				
				//signal是下行出站，signal2是上行出站
				//出站为XI
				if ((iChuZhanXHJ_Updown_Tmp==1 && iDirect==0)||(iChuZhanXHJ_Updown_Tmp==2 && iDirect==1))
				{
					
					cSet.GetFieldValue(_T("signal"), strChuZhanXHJ) ;
					cSet.GetFieldValue(_T("signal2"), strChuZhanXHJ_F) ;			
					
				}
				//出站为SI
				else if((iChuZhanXHJ_Updown_Tmp==1 && iDirect==1)||(iChuZhanXHJ_Updown_Tmp==2 && iDirect==0))
				{
					cSet.GetFieldValue(_T("signal2"), strChuZhanXHJ) ;
					cSet.GetFieldValue(_T("signal"), strChuZhanXHJ_F) ;
				} 
			}//end else if(iRecordCount==1)
		} //end if (iRecordCount>0)
		else
		{
			AfxMessageBox(_T("进路找不到出站-GetJinZhanOrChuZhan")) ;
		}
		cSet.Close() ;

		
	}
	catch (_com_error& e)
	{
		bRet = FALSE ;
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-GetJinZhanOrChuZhan"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		bRet = FALSE ;
		AfxMessageBox(_T("unknown error in GetJinZhanOrChuZhan")) ;
	} 	
	
	return bRet ;
	
}

//找出某一段内需要转频的轨道
//iDirect:
//0:从左向右
//1:从右向左
int CMaXuBiaoForHD::FindZP(CStringArray &strArrayGuiDao_ZP, int iStart_id, int iEnd_id, int iDirect)
{
	int iZpGuiDaoSum = 0 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		int iGuiDao_TopOrDown = 0 ;
		CString strGuiDao(_T("")) ;
		
		strSql.Format(_T("select * from jinlu where id>=%d and id<=%d"), iStart_id, iEnd_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (iDirect==0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
				cSet.GetFieldValue(_T("guidao_topbottom"), iGuiDao_TopOrDown) ;
			}
			while (!cSet.IsEOF())
			{
				int iGuiDao_TopOrDown_Tmp = 0 ;
				cSet.GetFieldValue(_T("guidao_topbottom"), iGuiDao_TopOrDown_Tmp) ;
				if (iGuiDao_TopOrDown_Tmp!=iGuiDao_TopOrDown)
				{
					cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
					strArrayGuiDao_ZP.Add(strGuiDao) ;
					iZpGuiDaoSum ++ ;
					iGuiDao_TopOrDown = iGuiDao_TopOrDown_Tmp ;
				}

				cSet.MoveNext() ;
			}
		}
		else if (iDirect==1)
		{
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
				cSet.GetFieldValue(_T("guidao_topbottom"), iGuiDao_TopOrDown) ;
			}
			while (!cSet.IsBOF())
			{
				int iGuiDao_TopOrDown_Tmp = 0 ;
				cSet.GetFieldValue(_T("guidao_topbottom"), iGuiDao_TopOrDown_Tmp) ;
				if (iGuiDao_TopOrDown_Tmp!=iGuiDao_TopOrDown)
				{
					cSet.GetFieldValue(_T("guidao"), strGuiDao) ;
					strArrayGuiDao_ZP.Add(strGuiDao) ;
					iZpGuiDaoSum ++ ;
					iGuiDao_TopOrDown = iGuiDao_TopOrDown_Tmp ;
				}
				
				cSet.MovePrevious() ;
			}
			
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-FindZP"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FindZP")) ;
	} 	

	return iZpGuiDaoSum ;
}

void CMaXuBiaoForHD::GetLinZhanStateSum(int &iRet, int iFirstOrLast)
{
	iRet = 9 ;
	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, strSql) ;
		
		if (iFirstOrLast ==0 )
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
		}
		else
		{
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
		}
		int iGuiDaoXHJType = -1 ;
		cSet.GetFieldValue(_T("signal_type"),iGuiDaoXHJType) ;
		if (iGuiDaoXHJType==1)
		{
			iRet = 8 ;
		}
		cSet.Close() ;
				
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-GetLinZhanStateSum"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetLinZhanStateSum")) ;
	} 	
	

}

void CMaXuBiaoForHD::SetGuiDaoFreq(CString &strGuiDao, const CString &strEndGuiDao, int &iIndex, int iUpOrDown/*=0*/)
{
	CString strGuiDaoTmp = _T("") ;

	AcDbBlockTable * pBlkTbl = NULL ;
	this->m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	
	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{			
			//处理 AcDbBlockReference ;
			if(pEnt->isKindOf(AcDbBlockReference::desc()))			
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;

				CString strTmp ;
				g_cSystem.GetAttValue(pRef, _T("GUIDAO"), strTmp) ; 
								
				if (strTmp == strGuiDao )
				{
					CString strFreqTmp ;
					//down
					if (iUpOrDown==0)
					{
						strFreqTmp = g_strFreq_Down_1[iIndex%4] ;
					}
					else
					{
						strFreqTmp = g_strFreq_Up_1[iIndex%4] ;
					}

					g_cSystem.SetAttValue(pRef, _T("FREQUENCE"), strFreqTmp) ;					

					iIndex ++ ;	
					g_cSystem.GetAttValue(pRef, _T("NEXT1"), strGuiDaoTmp) ;
					
					pEnt->close() ;
					pEnt = NULL ;
					break ;
				}


			}
			pEnt->close() ;
			pEnt = NULL ;
		}
		
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	if (strGuiDao == strEndGuiDao)
	{
		return ;
	}
	if (strGuiDaoTmp!=_T(""))
	{
		this->SetGuiDaoFreq(strGuiDaoTmp, strEndGuiDao, iIndex, iUpOrDown) ;
	}
}

void CMaXuBiaoForHD::SetOneGuiDaoFreq(CString strGuiDao, CString strFreq)
{
	AcDbBlockTable * pBlkTbl = NULL ;
	this->m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	
	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{			
			//处理 AcDbBlockReference ;
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;

				CString strTmp ;				
				g_cSystem.GetAttValue(pRef, _T("GUIDAO"), strTmp) ;
				
				if (strTmp == strGuiDao )
				{
					g_cSystem.SetAttValue(pRef, _T("FREQUENCE"), strFreq) ;
				}				
				
			}
			pEnt->close() ;
		}
		
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;


}

void CMaXuBiaoForHD::GetJinLuGuiDaoFreq(const CStringArray &strArrayGuiDao, CStringArray &strArrayFreq)
{
	strArrayFreq.RemoveAll() ;
	int iGuiDaoNum = strArrayGuiDao.GetSize() ;
	for (int i=0; i<iGuiDaoNum; i++)
	{
		CString strGuiDaoTmp, strFreqTmp ;
		strGuiDaoTmp = strArrayGuiDao.GetAt(i) ;
		try
		{
			CADORecordset cSet ;
			CString strSql ;
			
			strSql.Format(_T("select frequence from guidao where name='%s'"), strGuiDaoTmp ) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			cSet.Open(g_PtrCon, strSql) ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("frequence"), strFreqTmp) ;
			strArrayFreq.Add(strFreqTmp) ;
			cSet.Close() ;		
			
		}
		catch (_com_error& e)
		{
			//bRet = FALSE ;
			CString sBuff = CBlkUtility::GetErrorDescription(e) ;
			sBuff.Format(_T("%s-GetJinLuGuiDaoFreq"), sBuff) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			//bRet = FALSE ;
			AfxMessageBox(_T("unknown error in GetJinLuGuiDaoFreq")) ;
		} 
		
	}

}

//将信号平面图读入数据库
void CMaXuBiaoForHD::ReadDwgToDb2(LPUNKNOWN m_pConnection, AcDbDatabase * pCurDb)
{
	CString strSql ;

	_ConnectionPtr ptrCon; //与数据库的连接
	ptrCon = ( _ConnectionPtr ) m_pConnection ;

	CUserSystem mysys ;

	CString strTable[2] = {_T("guidao"),_T("signal")} ;
	for(int iTb=0; iTb<2; iTb++)
	{
		CDBUtility::ClearTable(strTable[iTb]) ;
	}
	
	AcDbBlockTable * pBlkTbl = NULL ;
	pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	
	AcDbEntity * pEnt = NULL ;
	CString strAppNamePre[3] = {_T("PRE1"),_T("PRE2"), _T("PRE3") } ;
	CString strAppNameNext[3] = {_T("NEXT1"),_T("NEXT2"), _T("NEXT3") } ;
	//CString strAppName
	
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//处理图块 AcDbBlockReference ;
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;

				//得到块名
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;		

				strBlockName.MakeLower() ;

				//确认是要读取的轨道类型
				if (strBlockName.Mid(0,15)==_T("maxubiao_guidao"))
				{

					CString strPre[3] = {_T(""), _T(""), _T("")} ;
					CString strNext[3] = {_T(""), _T(""), _T("")} ;
					CString strGuiDao, strXHJ , strXHJ2 ;
					int iGuidaoType = 0 ;
					int iUpOrDown = 0 ;
					int iTopOrBottom = 0 ;
					int iStartEnd = 0 ;
					CString strFreq = _T("") ;
					CString strTmp ;
					int iTmp = -1 ;

					int iXHJType = 0 ;
					int iXHJUpDown = 0 ;
					int iXHJDirect = 0 ;

					int iXHJType2 = 0 ;
					int iXHJUpDown2 = 0 ;
					int iXHJDirect2 = 0 ;
					
					//以"_"为标签分解类似maxubiao_guidao_gd_c_all的块名
					CStringArray strArrayBlokcName ;
					CBlkUtility::DivideString(strBlockName, _T("_"), strArrayBlokcName) ;

					CString strBlockNamePart3 = strArrayBlokcName.GetAt(2) ;
					CString strBlockNamePart4 = strArrayBlokcName.GetAt(3) ;
					CString strBlockNamePart5 = strArrayBlokcName.GetAt(4) ;
					//通过
					if (strBlockNamePart3 == _T("tg"))
					{
						iGuidaoType = 0 ;
						//下行
						if (strBlockNamePart4==_T("x"))
						{
							iUpOrDown = 1 ;
						}
						else//上行
						{
							iUpOrDown = 2 ;
						}
						//无分隔就表示有信号机
						if (strBlockNamePart5==_T("wfg"))
						{
							g_cSystem.GetAttValue(pRef,_T("XINHAOJI"),strXHJ) ;
							iXHJType = 0 ;
							iXHJUpDown = iUpOrDown ;
							iXHJDirect = 0 ;
						}

					} //end 通过
					//进站
					else if (strBlockNamePart3 == _T("jz"))
					{
						iGuidaoType = 0 ;
						//下行
						if (strBlockNamePart4==_T("x"))
						{
							iXHJUpDown = 1 ;
						}
						else//上行
						{
							iXHJUpDown = 2 ;
						}

						g_cSystem.GetAttValue(pRef,_T("XINHAOJI"),strXHJ) ;
						iXHJType = 1 ;
						//反向进站
						if (strBlockNamePart5==_T("f"))
						{							
							iXHJDirect = 1 ;
						}
					}// end 进站
					//股道
					else if (strBlockNamePart3 == _T("gd"))
					{
						if (strBlockNamePart4 == _T("x"))
						{
							iGuidaoType = 1 ;
						}
						else if (strBlockNamePart4 == _T("s"))
						{
							iGuidaoType = 2 ;
						}
						else if (strBlockNamePart4 == _T("c"))
						{
							iGuidaoType = 3 ;
						}

						if (strBlockNamePart5 == _T("left"))
						{
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI"), strXHJ) ;
							iXHJType = 2 ;
							iXHJUpDown = 2 ;
						}
						else if (strBlockNamePart5 == _T("right"))
						{
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI"), strXHJ) ;
							iXHJType = 2 ;
							iXHJUpDown = 1 ;
						}
						else if (strBlockNamePart5 == _T("all"))
						{
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI_S"), strXHJ) ;
							iXHJType = 2 ;
							iXHJUpDown = 2 ;
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI_X"), strXHJ2) ;
							iXHJType2 = 2 ;
							iXHJUpDown2 = 1 ;
						}

					}
					//end 股道
					else if (strBlockNamePart3 == _T("lq"))
					{
						iGuidaoType = -1 ;
					}

					// 其他站内和离去只要读取前后轨道就可以了

					g_cSystem.GetAttValue(pRef, _T("GUIDAO"), strGuiDao) ;
					g_cSystem.GetAttValue(pRef, _T("FREQUENCE"), strFreq) ;

					for(int i=0; i<3; i++)
					{
						g_cSystem.GetAttValue(pRef, strAppNamePre[i], strPre[i]) ;
					}
					for(int i2=0; i2<3; i2++)
					{
						g_cSystem.GetAttValue(pRef,strAppNameNext[i2], strNext[i2]) ;
					}

					try
					{
						CString strPlus1 = _T("") ;
						long iPlus2 = 0 ;
						CString strAOrBG = _T("") ;
						
						if(CBlkUtility::IsStrMatch(_T("^\\d+[A,B]*G$"), strGuiDao) && iGuidaoType==0) //区间轨道
						{
							CString strPart1 ;
							CBlkUtility::RegReplace(_T("^(\\d+)([A,B]*)G$"), _T("$1"), strGuiDao, strPart1) ;	
							CBlkUtility::RegReplace(_T("^(\\d+)([A,B]*)G$"), _T("$2"), strGuiDao, strAOrBG) ;	
							if (iUpOrDown==1)
							{
								strPlus1.Format(_T("1-%s-%d"), strPart1, strAOrBG==_T("A")?2:1) ;
							}
							else
							{
								strPlus1.Format(_T("2-%s-%d"), strPart1, strAOrBG==_T("A")?1:2) ;
							}							
						}
						else if (CBlkUtility::IsStrMatch(_T("^\\w+DG$"), strGuiDao)) //3DG
						{
							CString strPart1 ;
							if (CBlkUtility::IsStrMatch(_T("^\\d+DG$"), strGuiDao))
							{
								CBlkUtility::RegReplace(_T("^(\\d+)DG$"), _T("$1"), strGuiDao, strPart1) ;
								int iPart1 = _tstoi(strPart1) ;
								iPlus2 = (iPart1%2==0?3:1)*10000+iPart1 ;
								//strPlus1.Format(_T("%d-%s"), iPart1%2==0?3:1, strPart1) ;
							}
							else if (CBlkUtility::IsStrMatch(_T("^\\d+-\\d+DG$"), strGuiDao))
							{
								CBlkUtility::RegReplace(_T("^(\\d+)-(\\d+)DG$"), _T("$1"), strGuiDao, strPart1) ;								
								int iPart1 = _tstoi(strPart1) ;
								iPlus2 = (iPart1%2==0?3:1)*10000+iPart1 ;
								//strPlus1.Format(_T("%d-%s"), iPart1%2==0?3:1, strPart1) ;
							}
						}
						else if (CBlkUtility::IsStrMatch(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-3]?$"), strGuiDao)) //IG 3G
						{
							CString strPart1 ;
							CString strPart1New = _T("") ;
							CBlkUtility::RegReplace(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-3]?$"), _T("$1"), strGuiDao, strPart1) ;
							BOOL bConvertOk = this->ConvertSpecialChar(strPart1, strPart1New) ;
							int iPart1 = _tstoi(bConvertOk==TRUE?strPart1New:strPart1) ;
							iPlus2 = 20000+iPart1 ;
							//strPlus1.Format(_T("2-%s"), bConvertOk==TRUE?strPart1New:strPart1) ;
						}

					strSql.Format(_T("insert into guidao(name,pre1,pre2,pre3,next1,next2,next3,signal,signal2, type, upordown,startend,toporbottom,frequence, plus1, plus2,a_or_b) \
						values('%s','%s','%s','%s','%s','%s','%s','%s', '%s', %d,%d,%d,%d,'%s','%s', %d, '%s')"),\
						strGuiDao,strPre[0],strPre[1],strPre[2],strNext[0],strNext[1],strNext[2],strXHJ,strXHJ2,iGuidaoType, iUpOrDown,iStartEnd,iTopOrBottom,strFreq, strPlus1, iPlus2, strAOrBG) ;
					ExecSqlString(strSql) ;
					}
					catch (_com_error& e)
					{
						CString sBuff = CBlkUtility::GetErrorDescription(e) ;
						AfxMessageBox(sBuff) ;	
					}
					catch (...)
					{
						AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;
						
					}
					
					if (!strXHJ.IsEmpty())
					{
						try
						{
						strSql.Format(_T("insert into signal(name,type, upordown,direct,guidao) \
							values('%s',%d,%d,%d,'%s')"),strXHJ,iXHJType,iXHJUpDown,iXHJDirect,strGuiDao) ;
						ExecSqlString(strSql) ;				
						
						}
						catch (_com_error& e)
						{
							CString sBuff = CBlkUtility::GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;							
						}	
					}
					if (!strXHJ2.IsEmpty())
					{
						try
						{
						strSql.Format(_T("insert into signal(name,type, upordown,direct,guidao) \
							values('%s',%d,%d,%d,'%s')"),strXHJ2,iXHJType2,iXHJUpDown2,iXHJDirect2,strGuiDao) ;
						ExecSqlString(strSql) ;				
						
						}
						catch (_com_error& e)
						{
							CString sBuff = CBlkUtility::GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;
							
						}	
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
		CString strSql ;
		CADORecordset cSetFind2 ;

		for (int i=0; i<2; i++)
		{
			//分界点坐标
			long iLeftDivideRealLoc = 0 ;
			long iRightDivideRealLoc = 0 ;
			this->FindDivide(this->m_strStationName, iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
			
			//begin 将区间的轨道区段从compute_xhj_table读入
			if (i==0)
			{
				//strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<%d and up_or_down=%d and gd_name<>'' order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
				strSql.Format(_T("select * from compute_xhj_table where ((loc_real>=%d and loc_real<=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=0 and up_or_down=0)) or (loc_real<%d and loc_real>=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=1 and up_or_down=0))) and up_or_down=0 and right(name,1)<>'链' order by loc_real asc"),iLeftDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc,iRightDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc) ;
			}
			else
			{
				//strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and right(name,1)<>'链' and type<>11 order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
				strSql.Format(_T("select * from compute_xhj_table where ((loc_real>=%d and loc_real<=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=1 and up_or_down=1)) or (loc_real<=%d and loc_real>=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=0 and up_or_down=1))) and up_or_down=1 and right(name,1)<>'链' order by loc_real asc"),iLeftDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc,iRightDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc) ;
			}
			if (cSetFind2.IsOpen())
			{
				cSetFind2.Close() ;
			}
			if (cSetFind2.Open(g_PtrCon, strSql))
			{
				if (!cSetFind2.IsBOF())
				{
					cSetFind2.MoveFirst() ;
				}
				int iRsCount = cSetFind2.GetRecordCount() ;
				int iIndex = 0 ;
				while (!cSetFind2.IsEOF())
				{
					CString strXHName = _T("") ;
					CString strXHNameBackup = _T("") ;
					CString strGdName = _T("") ;

					cSetFind2.GetFieldValue(_T("name"), strXHName) ;
					strXHNameBackup = strXHName ;
					cSetFind2.GetFieldValue(_T("gd_name"), strGdName) ;

					//strGdName.Insert(100, _T("G")) ;
					
					CString strPre1GdName = _T("") ;
					CString strNext1GdName = _T("") ;
					CString strFreq = _T("") ;
					int iXhjType = 0 ;
					int iUpOrDown = 0 ;	
					int iIsReverse = 0 ;
					cSetFind2.GetFieldValue(_T("type"), iXhjType) ;
					cSetFind2.GetFieldValue(_T("is_reverse"), iIsReverse) ;

					int iGuidaoType = 0 ;
					
					if (i==0)//下行信号机后面的频率就是本轨道的频率
					{
						cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
					}
					else //上行信号点防护的轨道的频率,是其前一个信号点后面写的 频率
					{
						if (iIndex==0)//上行第一个信号点（分界点）不处理，select时保留它主要为了取其后的频率
						{
							cSetFind2.MoveNext() ;
							iIndex++ ;
							continue ;
						}
						else
						{
							cSetFind2.MovePrevious() ;
							cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
							cSetFind2.MoveNext() ;				
						}						
					}
					
					if (strXHName.Left(1)!=_T("F") && strXHName.Left(1)!=_T("X")&& strXHName.Left(1)!=_T("S"))
					{								
						strSql.Format(_T("insert into signal (name, type, upordown, direct, guidao) values('%s', 0, %d, 0, '%s')"), strXHName, i+1, strGdName) ;
						ExecSqlString(strSql) ;
					}
					else
					{
						strXHName = _T("") ;
					}
					if (iIndex==0 || (i==1&&iIndex==1))
					{
						strPre1GdName = _T("") ;
						cSetFind2.MoveNext() ;
						cSetFind2.GetFieldValue(_T("gd_name"), strNext1GdName) ;
						cSetFind2.MovePrevious() ;
						//strNext1GdName.Insert(100, _T("G")) ;
					}
					else if (iIndex==(iRsCount-1))
					{
						strNext1GdName = _T("") ;
						cSetFind2.MovePrevious() ;
						cSetFind2.GetFieldValue(_T("gd_name"), strPre1GdName) ;
						cSetFind2.MoveNext() ;
						//strPre1GdName.Insert(100, _T("G")) ;
					}
					else
					{
						cSetFind2.MovePrevious() ;
						int iPreXhjType = -1 ;
						int iIsPreXhjReverse = -1 ;
						cSetFind2.GetFieldValue(_T("type"), iPreXhjType) ;
						cSetFind2.GetFieldValue(_T("is_reverse"), iIsPreXhjReverse) ;

						cSetFind2.GetFieldValue(_T("gd_name"), strPre1GdName) ;
						cSetFind2.MoveNext() ;
						cSetFind2.MoveNext() ;
						cSetFind2.GetFieldValue(_T("gd_name"), strNext1GdName) ;
						cSetFind2.MovePrevious() ;

						if (iPreXhjType==3&&iIsPreXhjReverse==0&&i==1) //如果前一个是S进站，则此轨道已经在处理进站口S时添加过了
						{
							//更新S进站右方第一个轨道的Next1
							strSql.Format(_T("update guidao set next1='%s' where name='%s'"),strNext1GdName, strGdName) ;
							ExecSqlString(strSql) ;
							cSetFind2.MoveNext() ;
							iIndex++ ;
							continue ;
						}
						//strPre1GdName.Insert(100, _T("G")) ;
						//strNext1GdName.Insert(100, _T("G")) ;
					}
					
					//begin 在4个进站口的处理
					//X进站
					if (iXhjType==3&&iIsReverse==0&&i==0)
					{
						CString strQjGuidao = _T("") ;
						cSetFind2.MovePrevious() ;
						if (!cSetFind2.IsBOF())
						{
							cSetFind2.GetFieldValue(_T("gd_name"), strQjGuidao) ;

							CADORecordset cSetFindGuiDao ;
							CString strZnGuidao = _T(""), strZnPre1 = _T(""), strZnPre2 = _T("") ;
							strSql.Format(_T("select * from guidao where signal='%s'"), strXHNameBackup) ;
							if (cSetFindGuiDao.IsOpen())
							{
								cSetFindGuiDao.Close() ;
							}
							if (cSetFindGuiDao.Open(g_PtrCon, strSql))
							{
								if (cSetFindGuiDao.GetRecordCount()>0)
								{
									cSetFindGuiDao.MoveFirst() ;

									cSetFindGuiDao.GetFieldValue(_T("name"), strZnGuidao) ;									
									cSetFindGuiDao.GetFieldValue(_T("pre1"), strZnPre1) ;
									cSetFindGuiDao.GetFieldValue(_T("pre2"), strZnPre2) ;
									
									cSetFindGuiDao.Edit() ;
									cSetFindGuiDao.SetFieldValue(_T("pre3"), strZnPre2) ;
									cSetFindGuiDao.SetFieldValue(_T("pre2"), strZnPre1) ;
									cSetFindGuiDao.SetFieldValue(_T("pre1"), strQjGuidao) ;
									cSetFindGuiDao.Update() ;									
								}
								cSetFindGuiDao.Close() ;
							}
							if (strZnGuidao!=_T(""))
							{
								strSql.Format(_T("select * from guidao where name='%s'"), strQjGuidao) ;
								if (cSetFindGuiDao.IsOpen())
								{
									cSetFindGuiDao.Close() ;
								}
								if (cSetFindGuiDao.Open(g_PtrCon, strSql))
								{
									if (cSetFindGuiDao.GetRecordCount()>0)
									{
										cSetFindGuiDao.MoveFirst() ;
										
										cSetFindGuiDao.Edit() ;
										cSetFindGuiDao.SetFieldValue(_T("next1"), strZnGuidao) ;
										cSetFindGuiDao.Update() ;									
									}
									cSetFindGuiDao.Close() ;
								}					
							}
						}
						cSetFind2.MoveNext() ;
						iIndex++ ;
						cSetFind2.MoveNext() ;
						continue ;
					}
					//SN进站
					else if (iXhjType==3&&iIsReverse==1&&i==0)
					{
						CADORecordset cSetFindGuiDao ;
						CString strZnGuidao = _T(""), strZnNext1 = _T(""), strZnNext2 = _T("") ;
						strSql.Format(_T("select * from guidao where signal='%s'"), strXHNameBackup) ;
						if (cSetFindGuiDao.IsOpen())
						{
							cSetFindGuiDao.Close() ;
						}
						if (cSetFindGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetFindGuiDao.GetRecordCount()>0)
							{
								cSetFindGuiDao.MoveFirst() ;
								
								cSetFindGuiDao.GetFieldValue(_T("name"), strZnGuidao) ;									
								cSetFindGuiDao.GetFieldValue(_T("next1"), strZnNext1) ;
								cSetFindGuiDao.GetFieldValue(_T("next2"), strZnNext2) ;
								
								cSetFindGuiDao.Edit() ;
								cSetFindGuiDao.SetFieldValue(_T("next3"), strZnNext2) ;
								cSetFindGuiDao.SetFieldValue(_T("next2"), strZnNext1) ;
								cSetFindGuiDao.SetFieldValue(_T("next1"), strGdName) ;
								cSetFindGuiDao.Update() ;									
							}
							cSetFindGuiDao.Close() ;
						}
						if (strZnGuidao!=_T(""))
						{
							strPre1GdName = strZnGuidao ;
						}

						iGuidaoType = -1 ;
					}
					//S进站
					else if (iXhjType==3&&iIsReverse==0&&i==1)
					{
						CString strQjGuidao = _T("") ;
						CString strQjXhj = _T("") ;
						CString strFreq_FirstQjGd = _T("") ;
						cSetFind2.GetFieldValue(_T("set_freq"), strFreq_FirstQjGd) ;
						cSetFind2.MoveNext() ;
						if (!cSetFind2.IsEOF())
						{
							cSetFind2.GetFieldValue(_T("gd_name"), strQjGuidao) ;
							cSetFind2.GetFieldValue(_T("name"), strQjXhj) ;
							if (strQjXhj.Left(1)==_T("F"))
							{
								strQjXhj = _T("") ;
							}
							
							CADORecordset cSetFindGuiDao ;
							CString strZnGuidao = _T(""), strZnNext1 = _T(""), strZnNext2 = _T("") ;
							strSql.Format(_T("select * from guidao where signal='%s'"), strXHNameBackup) ;
							if (cSetFindGuiDao.IsOpen())
							{
								cSetFindGuiDao.Close() ;
							}
							if (cSetFindGuiDao.Open(g_PtrCon, strSql))
							{
								if (cSetFindGuiDao.GetRecordCount()>0)
								{
									cSetFindGuiDao.MoveFirst() ;
									
									cSetFindGuiDao.GetFieldValue(_T("name"), strZnGuidao) ;									
									cSetFindGuiDao.GetFieldValue(_T("next1"), strZnNext1) ;
									cSetFindGuiDao.GetFieldValue(_T("next2"), strZnNext2) ;
									
									cSetFindGuiDao.Edit() ;
									cSetFindGuiDao.SetFieldValue(_T("next3"), strZnNext2) ;
									cSetFindGuiDao.SetFieldValue(_T("next2"), strZnNext1) ;
									cSetFindGuiDao.SetFieldValue(_T("next1"), strQjGuidao) ;
									cSetFindGuiDao.Update() ;									
								}
								cSetFindGuiDao.Close() ;
							}
							if (strZnGuidao!=_T(""))
							{
								strSql.Format(_T("insert into guidao (name, pre1, next1, signal, type, upordown, frequence) values('%s', '%s', '%s', '%s',%d, %d, '%s')"), strQjGuidao, strZnGuidao, _T(""), strQjXhj, 0, i+1, strFreq_FirstQjGd) ;
								ExecSqlString(strSql) ;

// 								strSql.Format(_T("select * from guidao where name='%s'"), strQjGuidao) ;
// 								if (cSetFindGuiDao.IsOpen())
// 								{
// 									cSetFindGuiDao.Close() ;
// 								}
// 								if (cSetFindGuiDao.Open(g_PtrCon, strSql))
// 								{
// 									if (cSetFindGuiDao.GetRecordCount()>0)
// 									{
// 										cSetFindGuiDao.MoveFirst() ;
// 										
// 										cSetFindGuiDao.Edit() ;
// 										cSetFindGuiDao.SetFieldValue(_T("pre1"), strZnGuidao) ;
// 										cSetFindGuiDao.Update() ;									
// 									}
// 									cSetFindGuiDao.Close() ;
// 								}					
							}
						}
						//cSetFind2.MovePrevious() ;
						iIndex++ ;
						//cSetFind2.MoveNext() ;
						continue ;
					}
					//XN进站
					else if (iXhjType==3&&iIsReverse==1&&i==1)
					{
						CADORecordset cSetFindGuiDao ;
						CString strZnGuidao = _T(""), strZnPre1 = _T(""), strZnPre2 = _T("") ;
						strSql.Format(_T("select * from guidao where signal='%s'"), strXHNameBackup) ;
						if (cSetFindGuiDao.IsOpen())
						{
							cSetFindGuiDao.Close() ;
						}
						if (cSetFindGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetFindGuiDao.GetRecordCount()>0)
							{
								cSetFindGuiDao.MoveFirst() ;
								
								cSetFindGuiDao.GetFieldValue(_T("name"), strZnGuidao) ;									
								cSetFindGuiDao.GetFieldValue(_T("pre1"), strZnPre1) ;
								cSetFindGuiDao.GetFieldValue(_T("pre2"), strZnPre2) ;
								
								cSetFindGuiDao.Edit() ;
								cSetFindGuiDao.SetFieldValue(_T("pre3"), strZnPre2) ;
								cSetFindGuiDao.SetFieldValue(_T("pre2"), strZnPre1) ;
								cSetFindGuiDao.SetFieldValue(_T("pre1"), strGdName) ;
								cSetFindGuiDao.Update() ;									
							}
							cSetFindGuiDao.Close() ;
						}
						if (strZnGuidao!=_T(""))
						{
							strNext1GdName = strZnGuidao ;
						}
						
						iGuidaoType = -1 ;
					}

					//end 在4个进站口的处理
					
					strSql.Format(_T("insert into guidao (name, pre1, next1, signal, type, upordown, frequence) values('%s', '%s', '%s', '%s',%d, %d, '%s')"), strGdName, strPre1GdName, strNext1GdName, strXHName, iGuidaoType, i+1, strFreq) ;
					ExecSqlString(strSql) ;
					
					iIndex++ ;
					cSetFind2.MoveNext() ;
				}
				cSetFind2.Close() ;
			}
			//end 将区间的轨道区段从compute_xhj_table读入			
		}

		//begin 将正线站内轨道的频率从compute_xhj_table读入
		strSql.Format(_T("select * from import_gdsection_info where belong_station='%s'"), this->m_strStationName) ;
		if (cSetFind2.IsOpen())
		{
			cSetFind2.Close() ;
		}
		if (cSetFind2.Open(g_PtrCon, strSql))
		{
			if (!cSetFind2.IsBOF())
			{
				cSetFind2.MoveFirst() ;
			}
			while (!cSetFind2.IsEOF())
			{
				CString strFreq = _T(""), strGdName = _T("") ;
				
				cSetFind2.GetFieldValue(_T("freq"), strFreq) ;
				cSetFind2.GetFieldValue(_T("gd_name"), strGdName) ;			
				
				strSql.Format(_T("update guidao set frequence='%s' where name='%s'"), strFreq, strGdName) ;
				ExecSqlString(strSql) ;
				
				cSetFind2.MoveNext() ;					
			}
			cSetFind2.Close() ;
		}
		//end 将正线站内轨道的频率从compute_xhj_table读入


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;
		
	}

}


void CMaXuBiaoForHD::ExportLKQCTable(CString strSaveFileName)
{	
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	CString strTempletFile ;
	strTempletFile.Format(_T("%s\\support\\lkdata\\列控中心采集驱动配置表.xls"), m_strSysBasePath) ;

	int iQdCurIndex = 12 ;
	int iCjCurIndex = 20 ;

	CUserSystem mysys ;

	BOOL bIsZJZ = TRUE ;
	try
	{
		//找出是否有非通过的轨道，则表明不是中继站
		CString strSqlFindNotTG ;
		CADORecordset cSetFindNotTG ;
		strSqlFindNotTG.Format(_T("SELECT * FROM guidao where type<>0")) ;
		if (cSetFindNotTG.IsOpen())
		{
			cSetFindNotTG.Close() ;
		}
		if (cSetFindNotTG.Open(g_PtrCon, strSqlFindNotTG))
		{
			if (cSetFindNotTG.GetRecordCount()>0)
			{
				bIsZJZ = FALSE ;
			}
			cSetFindNotTG.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ExportLKQCTable")) ;		
	}

	if (bIsZJZ)
	{
		strTempletFile.Format(_T("%s\\support\\lkdata\\列控中心采集驱动配置表-中继站.xls"), m_strSysBasePath) ;
		iQdCurIndex = 10 ;
		iCjCurIndex = 10 ;
	}

	_ApplicationPtr m_pExcel = NULL ;
	//WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	_WorksheetPtr pSheet2 = NULL ;


	CString strMsg ;
	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( _T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return ;
		}

		//pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBook = m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strTempletFile ) ;
		
		//驱动
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;


		CString strWriteText = _T("") ;

		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)_T("B2")) ;
		strWriteText.Format(_T("%s列控中心驱动配置表"), this->m_strStationName) ;
		pRange->PutFormulaR1C1((_variant_t)strWriteText) ;	
				
		CADORecordset cSetFind ;
		CString strSql ;
		strSql.Format(_T("SELECT * FROM guidao where type>-1 and upordown=0 order by plus2")) ;
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
			
			while (!cSetFind.IsEOF())
			{
				CString strGdName = _T("") ;
				cSetFind.GetFieldValue(_T("name"), strGdName) ;

				int iCol = 0, iRow = 0 ;
				CString strColName = _T("") ;
				iCol = (iQdCurIndex/32)*2+3 ;
				strColName = CBlkUtility::GetExcelFieldName(iCol) ;
				iRow = iQdCurIndex%32+5 ;
				CString strCellName = _T("") ;
				strCellName.Format(_T("%s%d"), strColName, iRow) ;
				strWriteText = _T("站内方向继电器") ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

				strColName = CBlkUtility::GetExcelFieldName(iCol+1) ;
				strCellName.Format(_T("%s%d"), strColName, iRow) ;
				strWriteText.Format(_T("%s_FJ"), strGdName) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				pRange->PutFormulaR1C1((_variant_t)strWriteText) ;
			
				iQdCurIndex++ ;
				cSetFind.MoveNext() ;
			}
			cSetFind.Close() ;
		}

		//采集
		vt_index.iVal = 2 ;		
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)_T("B2")) ;
		strWriteText.Format(_T("%s列控中心采集配置表"), this->m_strStationName) ;
		pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

		CString strFJGJ[3] = {_T("FJ"),_T("GJ"),_T("GJ")} ;
		CString strType[3] = {_T("站内方向继电器"),_T("站内轨道继电器"),_T("区间轨道继电器")} ;
		CString strSqlTmp[3] = {
			_T("SELECT * FROM guidao where type>-1 and upordown=0 order by plus2"),
			_T("SELECT * FROM guidao where type>-1 and upordown=0 order by plus2"),
			_T("SELECT * FROM guidao where type=-1 or upordown>0 order by plus1")
		} ;
		for (int i=0; i<3; i++)
		{
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSqlTmp[i]))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}				
				while (!cSetFind.IsEOF())
				{
					CString strGdName = _T("") ;
					cSetFind.GetFieldValue(_T("name"), strGdName) ;					
						
					int iCol = 0, iRow = 0 ;
					CString strColName = _T("") ;

					if (iCjCurIndex==32)
					{
						int iii = 0 ;
					}
					iCol = (iCjCurIndex/32)*2+3 ;
					strColName = CBlkUtility::GetExcelFieldName(iCol) ;
					iRow = iCjCurIndex%32+5 ;
					CString strCellName = _T("") ;
					strCellName.Format(_T("%s%d"), strColName, iRow) ;
					strWriteText = strType[i] ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

					//pRange->GetEntireColumn()->AutoFit() ;
					
					strColName = CBlkUtility::GetExcelFieldName(iCol+1) ;
					strCellName.Format(_T("%s%d"), strColName, iRow) ;
					strWriteText.Format(_T("%s_%s"), strGdName, strFJGJ[i]) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

					//pRange->GetEntireColumn()->AutoFit() ;
					
					iCjCurIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
		}


		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;
	
		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;

		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
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


}

BOOL CMaXuBiaoForHD::ConvertSpecialChar(const CString strSrc, CString &strRet)
{
	BOOL bRet = FALSE ;
	CString strTmp[10] = {_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX"),_T("X")} ;
	for (int i=0; i<10; i++)
	{
		if (strSrc==strTmp[i])
		{
			if(i==9)
			{
				strRet.Format(_T("10")) ;
			}
			else
			{
				strRet.Format(_T("%d"), i+1) ;
			}
			bRet = TRUE ;
			break ;
		}
		
	}
	
	return bRet ;
}

//京沪中继站码序表，从xhj表读取数据生成
void CMaXuBiaoForHD::MaXuBiaoForJH()
{
	CString strSavePath = this->m_strDwgSaveDir ;
	//CUserSystem mysys ;
	try
	{
		CString strSql = _T("") ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from compute_xhj_table where type=11 order by loc_real asc")) ;
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
			int iSectionSum = cSet.GetRecordCount()-1 ;
			CString strBarMsg ;
			strBarMsg.Format(_T("京沪中继站码序表")) ;
			acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum) ;	
			int iStationIndex = 0 ;
			while (!cSet.IsEOF())
			{
				acedSetStatusBarProgressMeterPos(iStationIndex) ;

				int iId = 0 ;
				cSet.GetFieldValue(_T("id"), iId) ;
// 				if (iId==2446)
// 				{
// 					int iii = 0 ;
// 				}
// 				else 
// 				{
// 					cSet.MoveNext() ;
// 					continue ;
// 				}
// 				if (iStationIndex>1)
// 				{
// 					break ;
// 				}
				long iStationRealLoc = 0 ;				
				cSet.GetFieldValue(_T("loc_real"), iStationRealLoc) ;
				CString strStationName = _T("") ;
				cSet.GetFieldValue(_T("name"), strStationName) ;
				this->m_strStationName = strStationName ;

				CDBUtility::ClearTable(_T("signal")) ;
				CDBUtility::ClearTable(_T("guidao")) ;
				CString strSaveSubPath = _T("") ;
				strSaveSubPath.Format(_T("%s\\%s"), strSavePath, strStationName) ;
				::CreateDirectory( strSaveSubPath , NULL ) ;				
				this->m_strDwgSaveDir = strSaveSubPath ;
				
				//下行和上行
				for (int i=0; i<2; i++)
				{
					CADORecordset cSetFind2 ; 
					//分界点坐标
					long iLeftDivideRealLoc = 0 ;
					long iRightDivideRealLoc = 0 ;
					this->FindDivide(strStationName, iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
// 					for (int iDivideIndex=0; iDivideIndex<2; iDivideIndex++)
// 					{
// 						if (iDivideIndex==0)
// 						{
// 							strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down =%d and is_divide=1 order by loc_real desc"), iStationRealLoc, i) ;
// 						}
// 						else
// 						{
// 							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down =%d and is_divide=1 order by loc_real asc"), iStationRealLoc, i) ;
// 						}
// 						if (cSetFind2.IsOpen())
// 						{
// 							cSetFind2.Close() ;
// 						}
// 						if (cSetFind2.Open(g_PtrCon, strSql))
// 						{
// 							if (cSetFind2.GetRecordCount()>0)
// 							{
// 								cSetFind2.MoveFirst() ;
// 								if (iDivideIndex==0)
// 								{
// 									cSetFind2.GetFieldValue(_T("loc_real"), iLeftDivideRealLoc) ;
// 								}
// 								else
// 								{
// 									cSetFind2.GetFieldValue(_T("loc_real"), iRightDivideRealLoc) ;
// 								}
// 							}
// 							cSetFind2.Close() ;
// 						}
// 					}

					if (i==0)
					{
						strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<%d and up_or_down=%d and gd_name<>'' order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
					}
					else
					{
						strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and right(name,1)<>'链' and type<>11 order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
					}
					if (cSetFind2.IsOpen())
					{
						cSetFind2.Close() ;
					}
					if (cSetFind2.Open(g_PtrCon, strSql))
					{
						if (!cSetFind2.IsBOF())
						{
							cSetFind2.MoveFirst() ;
						}
						int iRsCount = cSetFind2.GetRecordCount() ;
						int iIndex = 0 ;
						while (!cSetFind2.IsEOF())
						{
							CString strXHName = _T("") ;
							CString strGdName = _T("") ;
							cSetFind2.GetFieldValue(_T("name"), strXHName) ;
							cSetFind2.GetFieldValue(_T("gd_name"), strGdName) ;
							//strGdName.Insert(100, _T("G")) ;
						
							CString strPre1GdName = _T("") ;
							CString strNext1GdName = _T("") ;
							CString strFreq = _T("") ;
							int iType = 0 ;
							int iUpOrDown = 0 ;

// 							if (strGdName=="00246AG")
// 							{
// 								int iii = 0 ;
// 							}

							
							if (i==0)//下行信号机后面的频率就是本轨道的频率
							{
								cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
							}
							else //上行信号点防护的轨道的频率,是其前一个信号点后面写的 频率
							{
								if (iIndex==0)//上行第一个信号点（分界点）不处理，select时保留它主要为了取其后的频率
								{
									cSetFind2.MoveNext() ;
									iIndex++ ;
									continue ;
								}
								else
								{
									cSetFind2.MovePrevious() ;
									cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
									cSetFind2.MoveNext() ;				
								}

							}

							if (strXHName.Left(1)!=_T("F"))
							{								
								strSql.Format(_T("insert into signal (name, type, upordown, direct, guidao) values('%s', 0, %d, 0, '%s')"), strXHName, i+1, strGdName) ;
								ExecSqlString(strSql) ;
							}
							else
							{
								strXHName = _T("") ;
							}
							if (iIndex==0 || (i==1&&iIndex==1))
							{
								strPre1GdName = _T("") ;
								cSetFind2.MoveNext() ;
								cSetFind2.GetFieldValue(_T("gd_name"), strNext1GdName) ;
								cSetFind2.MovePrevious() ;
								//strNext1GdName.Insert(100, _T("G")) ;
							}
							else if (iIndex==(iRsCount-1))
							{
								strNext1GdName = _T("") ;
								cSetFind2.MovePrevious() ;
								cSetFind2.GetFieldValue(_T("gd_name"), strPre1GdName) ;
								cSetFind2.MoveNext() ;
								//strPre1GdName.Insert(100, _T("G")) ;
							}
							else
							{
								cSetFind2.MovePrevious() ;
								cSetFind2.GetFieldValue(_T("gd_name"), strPre1GdName) ;
								cSetFind2.MoveNext() ;
								cSetFind2.MoveNext() ;
								cSetFind2.GetFieldValue(_T("gd_name"), strNext1GdName) ;
								cSetFind2.MovePrevious() ;
								//strPre1GdName.Insert(100, _T("G")) ;
								//strNext1GdName.Insert(100, _T("G")) ;
							}

							strSql.Format(_T("insert into guidao (name, pre1, next1, signal, type, upordown, frequence) values('%s', '%s', '%s', '%s',0, %d, '%s')"), strGdName, strPre1GdName, strNext1GdName, strXHName, i+1, strFreq) ;
							ExecSqlString(strSql) ;

							iIndex++ ;
							cSetFind2.MoveNext() ;
						}
						cSetFind2.Close() ;
					}

				}
				
				this->DoALL() ;
				
				cSet.MoveNext() ;
	
				iStationIndex++ ;
			}
			cSet.Close() ;
			acedRestoreStatusBar() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in MaXuBiaoForJH")) ;		
	}
}

void CMaXuBiaoForHD::ReleaseJLArray()
{
	if (arrayJinLu.GetSize()>0)
	{
		for(int i=0; i<arrayJinLu.GetSize(); i++)
		{
			if (arrayJinLu[i]!=NULL)
			{
				delete arrayJinLu[i] ;
				arrayJinLu[i] = NULL ;
			}
		}
		arrayJinLu.RemoveAll() ;
	}
}

void CMaXuBiaoForHD::DrawBianMaTable(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &strArrayGuiDao, CStringArray &strArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect, BOOL bIsFanXiangChuZhan)
{
	double dLineWidthThin = 0.0000 ;
	double dLineWidthThick = 0.4000 ;
	double dTextHeight = 3.50000 ;

// 	this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt2.shx", dTextHeight, 0.7) ;	
// 	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName("zhffont_guidao", "txt.shx", "hztxt2.shx", dTextHeight, 0.5) ;

	this->textStyleId = g_cSystem.CreateTextStyleByFileName(_T("zhffont"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightBianMa, this->m_dWidthBianMa) ;	
	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName(_T("zhffont_guidao"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightGuiDao, this->m_dWidthGuiDao) ;
	g_cSystem.ModifyFontStyle(_T("standard"), _T("hztxt2.shx")) ;
	
	CStringArray strArrayLinZhanState ;
	int iLinZhanStateNum = 9 ;

	for (int i=0; i<iLinZhanStateNum; i++)
	{
		strArrayLinZhanState.Add(g_strLinZhanState_1[i]) ;
	}

	if (bIsFanXiangChuZhan==1)
	{
		iLinZhanStateNum ++ ;
		strArrayLinZhanState.InsertAt(0, _T("L5")) ;
	}

	
	//武广模式
	if (m_iVersionFor==1)
	{
		iLinZhanStateNum ++ ;
		strArrayLinZhanState.InsertAt(0, _T("")) ;
		//取得进路轨道频率
		CStringArray strArrayTmp ;
		strArrayTmp.Add(_T("")) ;
		//this->GetJinLuGuiDaoFreq(strArrayGuiDao, strArrayFreq) ;
		CStringArray * pstrArrayTmp = new CStringArray() ;
		pstrArrayTmp->Copy(strArrayTmp) ;
		arrayBianMa.InsertAt(0, pstrArrayTmp) ;
		strArrayDescribe.InsertAt(0, _T("")) ;		
	}
		
	int iRow = arrayBianMa.GetSize() ;	
	int iCol = strArrayGuiDao.GetSize() ;
	double dVerLen = iRow*(this->dTableCellHeight) ;
	double dHorLen = iCol*(this->dTableCellWidth) ;

	ads_point base_pt = {0,0,0} ;
	base_pt[0] = 2 ;
	base_pt[1] = this->dTableCur_Y + 10 ;
	AcGePoint3d align_3d(0,0,0) ;
	g_cSystem.ConvertPtAdsTo3D(base_pt, align_3d) ;
	//g_cSystem.WriteText(base_pt,align_3d, this->m_strCaseType,0,3,AcDb::kTextBase,AcDb::kTextLeft,this->textStyleId,5,0.7,0) ;
	g_cSystem.AddText(this->m_strCaseType,align_3d,align_3d, AcDb::kTextLeft, AcDb::kTextBase, 5, 0.7, 0, 0, this->textStyleId) ;
	
	//画列线	
	BOOL bBianMaChange = 0 ;
	for(int iG=0; iG<iCol+1; iG++)
	{
		AcGePoint3d start_3d(0, 0, 0) ;
		AcGePoint3d end_3d(0, 0, 0) ;
		start_3d.x = iG*(this->dTableCellWidth) + this->dTableCur_X;
		start_3d.y = dTableCur_Y + this->dTableCellHeight ;
		end_3d.x = start_3d.x ;
		end_3d.y = -dVerLen + this->dTableCur_Y ;
		//g_cSystem.AddAcDbLine(start_3d, end_3d) ;		
		
		if (iG==0)
		{	
			bBianMaChange = arrayBianMaChange.GetAt(iG) ;
			
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d,dLineWidthThick, 0) ;
			if (iWriteDirect==0)
			{
				AcGePoint3d des_start_3d(0, 0, 0) ;
				AcGePoint3d des_end_3d(0, 0, 0) ;
				des_start_3d.x = this->dTableCur_X - dDescribeCellWidth ;
				des_start_3d.y = start_3d.y ;
				des_end_3d.x = des_start_3d.x ;
				des_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(des_start_3d, des_end_3d, dLineWidthThick, 0) ;
			}
			else if (iWriteDirect==1) 
			{
				AcGePoint3d LinZhan_start_3d(0, 0, 0) ;
				AcGePoint3d LinZhan_end_3d(0, 0, 0) ;
				LinZhan_start_3d.x = this->dTableCur_X - this->dTableCellWidth ;
				LinZhan_start_3d.y = start_3d.y ;
				LinZhan_end_3d.x = LinZhan_start_3d.x ;
				LinZhan_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(LinZhan_start_3d, LinZhan_end_3d, dLineWidthThick, 0) ;	
				
				ads_point base_pt_linzhan = {0,0,0} ;
				AcGePoint3d alignP_3d_linzhan(0, 0, 0) ;				
				base_pt_linzhan[0] =  LinZhan_start_3d.x + dTableCellWidth/2 ;
				base_pt_linzhan[1] =  dTableCur_Y + dTableCellHeight/2 ;
				g_cSystem.AddMText(base_pt_linzhan, _T("(邻站)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
		}
		else if (iG==iCol)
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d,dLineWidthThick, 0) ;
			if (iWriteDirect==0)
			{
				AcGePoint3d des_start_3d(0, 0, 0) ;
				AcGePoint3d des_end_3d(0, 0, 0) ;
				des_start_3d.x = start_3d.x + this->dTableCellWidth ;
				des_start_3d.y = start_3d.y ;
				des_end_3d.x = des_start_3d.x ;
				des_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(des_start_3d, des_end_3d, dLineWidthThick, 0) ;
			}
			else
			{
				AcGePoint3d LinZhan_start_3d(0, 0, 0) ;
				AcGePoint3d LinZhan_end_3d(0, 0, 0) ;
				LinZhan_start_3d.x = start_3d.x + dDescribeCellWidth ;
				LinZhan_start_3d.y = start_3d.y ;
				LinZhan_end_3d.x = LinZhan_start_3d.x ;
				LinZhan_end_3d.y = end_3d.y ;
				g_cSystem.AddAcDb2dPolyline(LinZhan_start_3d, LinZhan_end_3d, dLineWidthThick, 0) ;				
				
			}
			
		}
		else
		{
			if (bBianMaChange!=arrayBianMaChange.GetAt(iG))
			{
				g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, dLineWidthThick, 0, 3) ;
				bBianMaChange = arrayBianMaChange.GetAt(iG) ;
			}
			else
			{
				g_cSystem.AddAcDb2dPolyline(start_3d, end_3d,dLineWidthThin, 0) ;				
			}
		}

		AcGePoint3d start_3d_Mid(0, 0, 0) ;
		AcGePoint3d end_3d_Mid(0, 0, 0) ;
		start_3d_Mid.x = start_3d.x + this->dTableCellWidth/2 ;
		start_3d_Mid.y = dTableCur_Y  ;
		//武广要留一行画频率
		if (m_iVersionFor==1)
		{
			start_3d_Mid.y = start_3d_Mid.y - dTableCellHeight ;
		}
		end_3d_Mid.x = start_3d_Mid.x ;
		end_3d_Mid.y= end_3d.y ;
		
		ads_point base_pt = {0,0,0} ;
		
		base_pt[0] =  start_3d.x + dTableCellWidth/2  ;
		base_pt[1] =  dTableCur_Y + dTableCellHeight/2 ;
		

		//最后一列是封闭线，不用在画中线和写轨道了
		if (iG==iCol)
		{
			if (iWriteDirect==0)
			{
				g_cSystem.AddMText(base_pt, _T("(邻站)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			break ;
		}			

		g_cSystem.AddAcDb2dPolyline(start_3d_Mid, end_3d_Mid, dLineWidthThin, 0) ;

		g_cSystem.AddMText(base_pt, strArrayGuiDao.GetAt(iG), this->textStyleId_GuiDao, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		
		//武广，add频率
		if(m_iVersionFor == 1)
		{		
			base_pt[1] = base_pt[1] - dTableCellHeight ;
			g_cSystem.AddMText(base_pt, m_strArrayCurJinLuFre.GetAt(iG), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		}
	}

	//画行线
	for(int iM=0; iM<iRow+1; iM++)
	{
		AcGePoint3d start_3d(0, 0, 0) ;
		AcGePoint3d end_3d(0, 0, 0) ;

		start_3d.y = -iM*(this->dTableCellHeight) + this->dTableCur_Y ;
		end_3d.y = start_3d.y ;

		ads_point base_pt_linzhan = {0,0,0} ;
		AcGePoint3d alignP_3d_linzhan(0, 0, 0) ;
		
		if (iWriteDirect==0)
		{
			start_3d.x = this->dTableCur_X - dDescribeCellWidth;
			end_3d.x= dHorLen + this->dTableCur_X + this->dTableCellWidth ;

			if (iM<iLinZhanStateNum)
			{
				base_pt_linzhan[0] = end_3d.x -this->dTableCellWidth + dTableCellWidth/2 ;
				base_pt_linzhan[1] = start_3d.y - this->dTableCellHeight + dTableCellHeight/2 ;
				
				g_cSystem.AddMText(base_pt_linzhan, strArrayLinZhanState[iM] , this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
	
			}
		}
		else if (iWriteDirect==1)
		{
			start_3d.x = this->dTableCur_X - this->dTableCellWidth ;
			end_3d.x= dHorLen + this->dTableCur_X + dDescribeCellWidth ;
			
			if (iM<iLinZhanStateNum)
			{
				base_pt_linzhan[0] = start_3d.x + dTableCellWidth/2 ;
				base_pt_linzhan[1] = start_3d.y - this->dTableCellHeight + dTableCellHeight/2 ;
				
				g_cSystem.AddMText(base_pt_linzhan, strArrayLinZhanState[iM], this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			
		}

		//顶上的横线（在轨道名上方）只画一次
		if (iM==0)
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, 0, 0) ;
			
			AcGePoint3d top_start_3d(0, 0, 0) ;
			AcGePoint3d top_end_3d(0, 0, 0) ;
			top_start_3d.x = start_3d.x ;
			top_start_3d.y = start_3d.y + this->dTableCellHeight ;
			top_end_3d.x = end_3d.x ;
			top_end_3d.y = top_start_3d.y ;
			g_cSystem.AddAcDb2dPolyline(top_start_3d, top_end_3d, 0.4, 0) ;
		}
		else if (iM==iRow)
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, 0.4, 0) ;			
		}
		else 
		{
			g_cSystem.AddAcDb2dPolyline(start_3d, end_3d, 0, 0) ;
		}
		//最后一行线是封底线，以下操作不再进行
		if (iM==iRow)
		{
			break ;
		}
		
		ads_point base_pt = {0,0,0} ;
		AcGePoint3d alignP_3d(0, 0, 0) ;
		if (iWriteDirect==0)
		{
			base_pt[0] =  dTableCur_X - dDescribeCellWidth/2  ;	
		}
		else if (iWriteDirect==1)
		{
			base_pt[0] = dHorLen + this->dTableCur_X + dDescribeCellWidth/2 ;
		}
		base_pt[1] = start_3d.y - dTableCellHeight/2 ;
		g_cSystem.AddMText(base_pt, strArrayDescribe.GetAt(iM), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dDescribeCellWidth, 0) ;				
		
	}
	
	//分车从左往右走和从右往左走两种情况；
	int iBianMaMax = arrayBianMa.GetSize()-1 ;
	double dTextWidth = dTableCellWidth/2-1 ;
	if (iWriteDirect==0)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pstrArrayTmp->GetSize()-1 ;
			ads_point base_pt = {0, 0, 0} ;	
			base_pt[1] = -(iarray+1)*(this->dTableCellHeight) + dTableCellHeight/2 + this->dTableCur_Y ;
			if(iarray==0 && m_iVersionFor==1)
			{
				dTextWidth = dTableCellWidth - 1 ;
			}
			for(int ib=iNumMax; ib>=0; ib--)
			{	
				base_pt[0] = (iNumMax-ib)*(this->dTableCellWidth) + dTableCellWidth/4 + this->dTableCur_X ;
				if(iarray==0 && m_iVersionFor==1)
				{
					base_pt[0] = base_pt[0] + dTableCellWidth/4;
				}
				g_cSystem.AddMText(base_pt, pstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTextWidth, 0) ;				
				
			}
		}
	}
	else if (iWriteDirect==1)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pstrArrayTmp->GetSize() ;
			int iStartCol = iCol - iNumMax ;
			ads_point base_pt = {0, 0, 0} ;
			base_pt[1] = -(iarray+1)*(this->dTableCellHeight) + dTableCellHeight/2 + this->dTableCur_Y;	
			if(iarray==0 && m_iVersionFor==1)
			{
				dTextWidth = dTableCellWidth - 1 ;
			}
			for(int ib=0; ib<iNumMax; ib++)
			{
				base_pt[0] = (iStartCol+ib)*(this->dTableCellWidth) + dTableCellWidth/4 + this->dTableCur_X ;
				if(iarray==0 && m_iVersionFor==1)
				{
					base_pt[0] = base_pt[0] + dTableCellWidth/4;
				}
				g_cSystem.AddMText(base_pt, pstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth/2-1, 0) ;				
				
			}
		}
		
	}

	//begin 画图框 for 三院
	//////////////////////////////////////////////////////////////////////////
	/*
	AcDbBlockReference * pRef = NULL ;
	CString strBlockName ;
	CStringArray strArrayAtt ;
	AcGePoint3d pt_3d_topleft(0,0,0),pt_3d_bottomleft(0,0,0), pt_3d_topright(0,0,0), pt_3d_bottomright(0,0,0), pt_3d_describe(0,0,0) ;
	
	m_dSiYiTuHeight = 50 ;
	//只在画上面一张表格时设置一次边框x坐标
	if (dTableCur_Y==0)
	{
		m_dFrameLeft_x =  iWriteDirect==0?-105:-53 ;
	}
	pt_3d_topleft.x = m_dFrameLeft_x ;
	pt_3d_topleft.y = dTableCur_Y + m_dSiYiTuHeight + dTableCellHeight ;
	pt_3d_bottomleft.x = pt_3d_topleft.x ;
	pt_3d_bottomleft.y = pt_3d_topleft.y-287-5 ;
	double dContenWidth = 25+20+dDescribeCellWidth+dTableCellWidth+dHorLen ;
	if (dContenWidth<420)
	{
		dContenWidth = 420 ;
	}
	pt_3d_bottomright.x = m_dFrameLeft_x+dContenWidth ;// iWriteDirect==0?dHorLen+28:dHorLen+80 ;
	pt_3d_bottomright.y = pt_3d_bottomleft.y + 5 ;

	strBlockName = _T("maxubiao_bottomleft_3") ;
	strArrayAtt.RemoveAll() ;
	strArrayAtt.Add(_T("")) ;
	strArrayAtt.Add(_T("")) ;	
	strArrayAtt.Add(CBlkUtility::GetDateStr()) ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_bottomleft, &strArrayAtt, FALSE))
	{
		pRef->close() ;
	}

	//插入 5级审核标签	
	//三院图标
	strBlockName = _T("FRAME_SIGN_3_3") ;
	strArrayAtt.RemoveAll() ;
	strArrayAtt.Add(_T("哈尔滨至大连铁路客运专线")) ;
	strArrayAtt.Add(m_strDrawNum) ;
	strArrayAtt.Add(_T("大连至沈阳段")) ;
	strArrayAtt.Add(_T("施工图")) ;
	strArrayAtt.Add(m_strStationName) ;
	strArrayAtt.Add(_T("码序表")) ;
	strArrayAtt.Add(_T("")) ;
	strArrayAtt.Add(_T("")) ;
	strArrayAtt.Add(CBlkUtility::GetDateStr()) ;

	AcGePoint3d pt_3d_frame_insert(0,0,0) ; //frame插入点
	pt_3d_frame_insert = pt_3d_bottomright ;

	if (iRow>26)
	{
		strBlockName = _T("FRAME_SIGN_3_3_1") ;//三院图标		
		pt_3d_bottomright.x = pt_3d_bottomright.x+35 ;	
		pt_3d_frame_insert.x = pt_3d_bottomright.x ;
		pt_3d_frame_insert.y = pt_3d_topleft.y ;

	}

	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_frame_insert, &strArrayAtt, FALSE))
	{
		pRef->close() ;
	}
	ads_point text_pt = {0, 0, 0} ;
	text_pt[0] = pt_3d_bottomright.x - 90 ;
	text_pt[1] = pt_3d_bottomright.y + 20 ;
	
	AcGePoint3d pt_3d_1(pt_3d_topleft.x,pt_3d_topleft.y+5,0) ;
	AcGePoint3d pt_3d_2(pt_3d_bottomright.x+5,pt_3d_topleft.y+5,0) ;
	AcGePoint3d pt_3d_3(pt_3d_bottomright.x+5,pt_3d_bottomright.y-5,0) ;
	AcGePoint3d pt_3d_4(pt_3d_topleft.x,pt_3d_bottomright.y-5,0) ;
	AcGePoint3dArray pt_3d_array_outside, pt_3d_array_inside ;
	pt_3d_array_outside.append(pt_3d_1) ;
	pt_3d_array_outside.append(pt_3d_2) ;
	pt_3d_array_outside.append(pt_3d_3) ;
	pt_3d_array_outside.append(pt_3d_4) ;
//	//暂时不需要外框，先屏蔽
	g_cSystem.AddAcDb2dPolyline(pt_3d_array_outside,0, 0) ;

	pt_3d_1.x +=25 ;
	pt_3d_1.y -=5 ; 
	pt_3d_2.x -=5 ;
	pt_3d_2.y -=5 ;
	pt_3d_3.x -=5 ;
	pt_3d_3.y +=5 ;
	pt_3d_4.x +=25 ;
	pt_3d_4.y +=5 ;
	pt_3d_array_inside.append(pt_3d_1) ;
	pt_3d_array_inside.append(pt_3d_2) ;
	pt_3d_array_inside.append(pt_3d_3) ;
	pt_3d_array_inside.append(pt_3d_4) ;
	g_cSystem.AddAcDb2dPolyline(pt_3d_array_inside,1, 0) ;
	*/
	//end 画图框 for 三院
	//////////////////////////////////////////////////////////////////////////


//begin 画图框 for 本院
//////////////////////////////////////////////////////////////////////////
AcDbBlockReference * pRef = NULL ;
CString strBlockName ;
CStringArray strArrayAtt ;
AcGePoint3d pt_3d_topleft(0,0,0),pt_3d_bottomleft(0,0,0),pt_3d_bottomright(0,0,0), pt_3d_describe(0,0,0) ;

m_dSiYiTuHeight = 50 ;
//只在画上面一张表格时设置一次边框x坐标
if (dTableCur_Y==0)
{
	m_dFrameLeft_x =  iWriteDirect==0?-105:-53 ;
}
pt_3d_topleft.x = m_dFrameLeft_x ;
pt_3d_topleft.y = dTableCur_Y + m_dSiYiTuHeight + dTableCellHeight ;
pt_3d_bottomleft.x = pt_3d_topleft.x ;
pt_3d_bottomleft.y = pt_3d_topleft.y-287-5 ;
double dContenWidth = 25+20+dDescribeCellWidth+dTableCellWidth+dHorLen ;
if (dContenWidth<420)
{
	dContenWidth = 420 ;
}
pt_3d_bottomright.x = m_dFrameLeft_x+dContenWidth ;// iWriteDirect==0?dHorLen+28:dHorLen+80 ;
pt_3d_bottomright.y = pt_3d_bottomleft.y + 5 ;
strBlockName = _T("maxubiao_topleft") ;
if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_topleft ))
{
	pRef->close() ;
}
strBlockName = _T("maxubiao_bottomleft") ;
if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_bottomleft ))
{
	pRef->close() ;
}

//插入 5级审核标签	
//三院图标
strBlockName = _T("FRAME_SIGN_5_3") ;
strArrayAtt.RemoveAll() ;
//m_strStationName = _T("测试站") ;
CString	strStationNameFrame = m_strStationName ;
if (strStationNameFrame.Find(_T("站"))==-1 && strStationNameFrame.Find(_T("线路所"))==-1)
{
	strStationNameFrame.Insert(555, _T("站")) ;
}
strArrayAtt.Add(strStationNameFrame) ;
CString strDwgName = m_strCaseType ;
strDwgName.TrimLeft(_T("1.")) ;
strDwgName.TrimLeft(_T("2.")) ;
strDwgName.Insert(555, _T("码序表")) ;
strArrayAtt.Add(strDwgName) ;
strArrayAtt.Add(_T("G02-158-1-")) ;
CString strDate =_T("") ;
strDate = CBlkUtility::GetDateStr() ;
strArrayAtt.Add(strDate) ;

AcGePoint3d pt_3d_frame_insert(0,0,0) ; //frame插入点
pt_3d_frame_insert = pt_3d_bottomright ;

if (iRow>26)
{
	strBlockName = _T("FRAME_SIGN_5_3_1") ;	
	pt_3d_bottomright.x = pt_3d_bottomright.x+35 ;	
	pt_3d_frame_insert.x = pt_3d_bottomright.x ;
	pt_3d_frame_insert.y = pt_3d_topleft.y ;

}

if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_frame_insert, &strArrayAtt, FALSE))
{
	pRef->close() ;
}
ads_point text_pt = {0, 0, 0} ;
text_pt[0] = pt_3d_bottomright.x - 90 ;
text_pt[1] = pt_3d_bottomright.y + 20 ;

AcGePoint3d pt_3d_1(pt_3d_topleft.x,pt_3d_topleft.y+5,0) ;
AcGePoint3d pt_3d_2(pt_3d_bottomright.x+5,pt_3d_topleft.y+5,0) ;
AcGePoint3d pt_3d_3(pt_3d_bottomright.x+5,pt_3d_bottomright.y-5,0) ;
AcGePoint3d pt_3d_4(pt_3d_topleft.x,pt_3d_bottomright.y-5,0) ;
AcGePoint3dArray pt_3d_array_outside, pt_3d_array_inside ;
pt_3d_array_outside.append(pt_3d_1) ;
pt_3d_array_outside.append(pt_3d_2) ;
pt_3d_array_outside.append(pt_3d_3) ;
pt_3d_array_outside.append(pt_3d_4) ;
//	//暂时不需要外框，先屏蔽
g_cSystem.AddAcDb2dPolyline(pt_3d_array_outside,0, 0) ;

pt_3d_1.x +=25 ;
pt_3d_1.y -=5 ; 
pt_3d_2.x -=5 ;
pt_3d_2.y -=5 ;
pt_3d_3.x -=5 ;
pt_3d_3.y +=5 ;
pt_3d_4.x +=25 ;
pt_3d_4.y +=5 ;
pt_3d_array_inside.append(pt_3d_1) ;
pt_3d_array_inside.append(pt_3d_2) ;
pt_3d_array_inside.append(pt_3d_3) ;
pt_3d_array_inside.append(pt_3d_4) ;
g_cSystem.AddAcDb2dPolyline(pt_3d_array_inside,1, 0) ;
//end 画图框 for 本院
//////////////////////////////////////////////////////////////////////////

	
	//begin 画示意图
	//////////////////////////////////////////////////////////////////////////
	AcGePoint3d pt_3d_mid(0,0,0) ;
	pt_3d_mid.x = pt_3d_1.x + (pt_3d_2.x-pt_3d_1.x)/2 ;
	pt_3d_mid.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
	strBlockName.Format(_T("%s\\syt.dwg"), this->m_strDwgSaveDir) ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_mid, FALSE, 1))
	{
		pRef->close() ;
	}

	//end 画示意图
	//////////////////////////////////////////////////////////////////////////

	//增量当前的Y坐标
	this->dTableCur_Y = this->dTableCur_Y - dVerLen - this->dBetweenTable_Y ;
}

void CMaXuBiaoForHD::GetFrontAndBackStation(const CString strCurStation, CString &strFrontStation, CString &strBackStation)
{
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;		
	
		long iCurStationRealLoc = 0 ;
		strSql.Format(_T("select * from compute_xhj_table where name='%s' order by loc_real asc"), strCurStation) ;	
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			if (cSetFind.GetRecordCount()>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(_T("loc_real"), iCurStationRealLoc) ;
			}
			cSetFind.Close() ;
		}

		for (int i=0; i<2; i++)
		{
			if (i==0)
			{
				strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and type in(1,11) order by loc_real desc"), iCurStationRealLoc) ;
			}
			else
			{
				strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and type in(1,11) order by loc_real asc"), iCurStationRealLoc) ;
			}
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;
					if (i==0)
					{
						cSetFind.GetFieldValue(_T("name"), strFrontStation) ;
					}
					else
					{
						cSetFind.GetFieldValue(_T("name"), strBackStation) ;
					}
				}
				cSetFind.Close() ;
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
		AfxMessageBox(_T("unknown error in GetFrontAndBackStation")) ;
		
	}

}

void CMaXuBiaoForHD::FindDivide(const CString strStation, long &iLeftDivideRealLoc, long &iRightDivideRealLoc, int iUpOrDown)
{
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		
		long iStationRealLoc = 0 ;
		strSql.Format(_T("select * from compute_xhj_table where name='%s' order by loc_real asc "), strStation) ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			if (cSetFind.GetRecordCount()>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(_T("loc_real"), iStationRealLoc) ;
			}
			cSetFind.Close() ;
		}
	
		//分界点坐标
		for (int iDivideIndex=0; iDivideIndex<2; iDivideIndex++)
		{
			if (iDivideIndex==0)
			{
				strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down =%d and is_divide=1 order by loc_real desc"), iStationRealLoc, iUpOrDown) ;
			}
			else
			{
				strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down =%d and is_divide=1 order by loc_real asc"), iStationRealLoc, iUpOrDown) ;
			}
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;
					if (iDivideIndex==0)
					{
						cSetFind.GetFieldValue(_T("loc_real"), iLeftDivideRealLoc) ;
					}
					else
					{
						cSetFind.GetFieldValue(_T("loc_real"), iRightDivideRealLoc) ;
					}
				}
				cSetFind.Close() ;
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
		AfxMessageBox(_T("unknown error in FindDivide")) ;
		
	}

}

// 关联前后轨道
void CMaXuBiaoForHD::ConGuiDao(void)
{
	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;

	ads_name sum_name  , temp_name1, temp_name2  ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("INSERT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf( _T("\n关联前后轨道\n ") ) ;

	if( RTNORM == ads_ssget( NULL , NULL , NULL , &eb , sum_name ) )
	{
		long lLength = 0 ;
		ads_sslength(sum_name, &lLength) ;
		for(int i = 0; i<lLength; i++)
		{
			if (i==lLength-1)
			{
				continue ;
			}

			ads_ssname(sum_name, i, temp_name1) ;
			ads_ssname(sum_name, i+1, temp_name2) ;
			AcDbEntity * pEnt_1 = NULL ;
			AcDbEntity * pEnt_2 = NULL ;
			CString strGuiDaoName1, strGuiDaoName2 ;

			if (g_cSystem.OpenAcDbEntity(temp_name1, pEnt_1))
			{
				AcDbBlockReference * pRef1 = (AcDbBlockReference *)pEnt_1 ;
				//得到前一个轨道名称
				if(g_cSystem.GetAttValue(pRef1, _T("GUIDAO"), strGuiDaoName1))
				{
					if (g_cSystem.OpenAcDbEntity(temp_name2, pEnt_2))
					{
						AcDbBlockReference * pRef2 = (AcDbBlockReference *)pEnt_2 ;

						if (g_cSystem.GetAttValue(pRef2, _T("GUIDAO"), strGuiDaoName2))
						{
							CString strNext, strPre, strMsg ;
							CString strFillTag ; //前一个轨道添加哪个next
							if (g_cSystem.GetAttValue(pRef1, _T("NEXT1"), strNext))
							{
								if (strNext.IsEmpty())
								{
									g_cSystem.SetAttValue(pRef1, _T("NEXT1"), strGuiDaoName2) ;
									strFillTag = _T("NEXT1") ;
								}
								else
								{
									if(g_cSystem.GetAttValue(pRef1, _T("NEXT2"), strNext))
									{
										if (strNext.IsEmpty())
										{
											g_cSystem.SetAttValue(pRef1, _T("NEXT2"), strGuiDaoName2) ;
											strFillTag = _T("NEXT2") ;
										}
										else
										{
											if (g_cSystem.GetAttValue(pRef1, _T("NEXT3"), strNext)) 
											{
												if (strNext.IsEmpty())
												{
													g_cSystem.SetAttValue(pRef1, _T("NEXT3"), strGuiDaoName2) ;
													strFillTag = _T("NEXT3") ;
												}
												else
												{
													strMsg.Format(_T("%s的三个next轨道都已经指定,点YES覆盖NEXT3，点NO取消本次关联行为"), strGuiDaoName1) ;
													UINT nID = AfxMessageBox(strMsg,MB_YESNO );
													if( nID == IDYES)
													{
														g_cSystem.SetAttValue(pRef1, _T("NEXT3"), strGuiDaoName2) ;
														strFillTag = _T("NEXT3") ;
													}												

												}
											}

										}
									}

								}
							}
							else //第一次关联，先把next1...3属性加上
							{
								g_cSystem.AddAttValue(pRef1, _T("NEXT1"), strGuiDaoName2) ;
								g_cSystem.AddAttValue(pRef1, _T("NEXT2"), _T("")) ;
								g_cSystem.AddAttValue(pRef1, _T("NEXT3"), _T("")) ;
								strFillTag = "NEXT1" ;
							}
							//填充下一个轨道的pre属性
							if (!strFillTag.IsEmpty())
							{
								if (g_cSystem.GetAttValue(pRef2, _T("PRE1"), strPre))
								{
									if (strPre.IsEmpty())
									{
										g_cSystem.SetAttValue(pRef2, _T("PRE1"), strGuiDaoName1) ;
									}
									else
									{
										if (g_cSystem.GetAttValue(pRef2, _T("PRE2"), strPre))
										{
											if (strPre.IsEmpty())
											{
												g_cSystem.SetAttValue(pRef2, _T("PRE2"), strGuiDaoName1) ;
											}
											else
											{
												if (g_cSystem.GetAttValue(pRef2, _T("PRE3"), strPre))
												{
													if (strPre.IsEmpty())
													{
														g_cSystem.SetAttValue(pRef2, _T("PRE3"), strGuiDaoName1) ;
													}
													else
													{	
														strMsg.Format(_T("%s的三个pre轨道都已经指定,点YES覆盖NEXT3，点NO取消本次关联行为"),strGuiDaoName2) ;
														UINT nID = AfxMessageBox(strMsg,MB_YESNO );
														if( nID == IDYES)
														{
															g_cSystem.SetAttValue(pRef2, _T("PRE3"), strGuiDaoName1) ;

														}
														else if (nID == IDNO) 
														{
															//取消对前一个轨道关联设置
															for (int i=1; i<4; i++)
															{
																CString  strTagTmp ;
																strTagTmp.Format(_T("NEXT%d"), i) ;
																if (strFillTag==strTagTmp)
																{
																	g_cSystem.SetAttValue(pRef1, strFillTag, strNext) ;
																	break ;
																}
															}															

														}

													}
												}
											}
										}
									}
								}
								else ////第一次关联，先把pre1...3属性加上
								{
									g_cSystem.AddAttValue(pRef2, _T("PRE1"), strGuiDaoName1) ;
									g_cSystem.AddAttValue(pRef2, _T("PRE2"), _T("")) ;
									g_cSystem.AddAttValue(pRef2, _T("PRE3"), _T("")) ;
								}


							}

						}

						pEnt_2->close() ;
						pEnt_2 = NULL ;

					}
				}

				pEnt_1->close() ;
				pEnt_1 = NULL ;

			}


		}

		ads_ssfree(sum_name) ;
	}
}

// 删除轨道关联
void CMaXuBiaoForHD::DelGuiDaoCon(void)
{
	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;

	CString strTag[] = {_T("PRE1"), _T("PRE2"), _T("PRE3"), _T("NEXT1"), _T("NEXT2"), _T("NEXT3")} ;

	ads_name sum_name  , temp_name ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("INSERT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf( _T("\n选择要取消关联的轨道\n ") ) ;

	if( RTNORM == ads_ssget( NULL , NULL , NULL , &eb , sum_name ) )
	{
		long lLength = 0 ;
		ads_sslength(sum_name, &lLength) ;
		for(int i = 0; i<lLength; i++)
		{
			ads_ssname(sum_name, i, temp_name) ;
			AcDbEntity * pEnt = NULL ;			

			if (g_cSystem.OpenAcDbEntity(temp_name, pEnt))
			{			
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;

				for (int i=0; i<6; i++)
				{
					g_cSystem.SetAttValue(pRef, strTag[i], _T("")) ;
				}

				pEnt->close() ;
				pEnt = NULL ;				
			}		
		}

		ads_ssfree(sum_name) ;
	}
}

// 检查轨道关联
void CMaXuBiaoForHD::CheckGuiDaoCon(void)
{
	CUserSystem mysys ;
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	mysys.m_pDb = pCurDb ;

	Acad::ErrorStatus es;
	AcDbBlockTable * pBlkTbl = NULL ;
	AcDbBlockTableRecord * pBlkTblRec = NULL ;

	es = pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	assert(es==Acad::eOk) ;

	es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	assert(es==Acad::eOk) ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	//g_cSystem.DeleteLayer("检查轨道") ;
	CMaXuBiaoForHD::UnCheckGuiDaoCon() ;

	AcDbObjectId lay_Id ;
	lay_Id = mysys.Set_Layer( _T("") , _T("检查轨道") ) ;

	AcDbEntity * pEnt = NULL ;	

	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//处理图块 AcDbBlockReference ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;				

				//得到块名

				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;				

				strBlockName.MakeLower() ;

				//确认是要读取的轨道类型
				if (strBlockName.Mid(0,15)==_T("maxubiao_guidao"))
				{
					CString strAppNameNext[3] = {_T("NEXT1"), _T("NEXT2"), _T("NEXT3")} ;
					CString strNext ;

					AcDbAttribute *pAtt = NULL ;
					AcGePoint3d pt_3d_curAtt ;
					if (mysys.FindAtt(pRef, _T("GUIDAO"), pAtt))
					{
						pt_3d_curAtt = pAtt->position() ; 
						pAtt->close() ;
					}					

					for(int i=0; i<3; i++)
					{
						if(mysys.GetAttValue(pRef,strAppNameNext[i], strNext) )
						{
							if (!strNext.IsEmpty()) 
							{
								AcGePoint3d pt_3d_next, pt_3d_attribute ;
								if (GetGuiDaoBlockPosition(_T("GUIDAO"), strNext, pt_3d_next, pt_3d_attribute)) 
								{
									AcGePoint3d pt_3d_start(pt_3d_curAtt), pt_3d_end(pt_3d_attribute) ;
									pt_3d_start.x++ ;
									pt_3d_start.y++ ;
									pt_3d_end.x-- ;	
									pt_3d_end.y++ ;
									AcDbObjectId lineid, circleid;
									lineid = mysys.AddAcDbLine(pt_3d_start, pt_3d_end, 1) ;
									circleid = mysys.AddAcDbCircle(pt_3d_end, 1, 1) ;

									AcDbEntity *pEnt = NULL ;
									acdbOpenObject(pEnt, lineid, AcDb::kForWrite) ;
									mysys.SetXdata(pEnt, _T("TYPE"), _T("CHECK")) ;
									pEnt->close() ;
									pEnt = NULL ;

									acdbOpenObject(pEnt, circleid, AcDb::kForWrite) ;
									mysys.SetXdata(pEnt, _T("TYPE"), _T("CHECK")) ;
									pEnt->close() ;
									pEnt = NULL ;

								}
							}
						}
					}
				}

			}
			pEnt->close() ;
		}

	}
	mysys.Set_Layer(_T(""), _T("0")) ;

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

// 取消显示轨道关联
void CMaXuBiaoForHD::UnCheckGuiDaoCon(void)
{
	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;

	//g_cSystem.SetLayerStatus("检查轨道", true) ;

	AcDbBlockTable * pBlkTbl = NULL ;
	g_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;

	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{	

			//处理 	AcDbLine, AcDbCircle 
			if (pEnt->isKindOf(AcDbLine::desc())|| pEnt->isKindOf(AcDbCircle::desc()))
			{
				CString strValue ;
				g_cSystem.GetXdata(pEnt, _T("TYPE"), strValue) ;
				if (strValue == _T("CHECK") )
				{
					pEnt->erase() ;
				}			
			} 
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

}

// 设置轨道频率
void CMaXuBiaoForHD::SetFreq(void)
{
	CAcModuleResourceOverride myResources ;	

	CStringArray strArrayGuiDao, strArrayFreq ;

	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;

	ads_name sum_name  , temp_name  ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("INSERT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf( _T("\n设置频率，请选择轨道: " )) ;

	if( RTNORM == ads_ssget( NULL , NULL , NULL , &eb , sum_name ) )
	{		
		long lLength = 0 ;
		ads_sslength(sum_name, &lLength) ;
		for(int i = 0; i<lLength; i++)
		{
			ads_ssname (sum_name, i, temp_name) ;
			AcDbEntity * pEnt = NULL ;
			if (g_cSystem.OpenAcDbEntity(temp_name, pEnt))
			{
				//if (strcmp(pEnt->isA()->name(), _T("AcDbBlockReference"))==0)
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					CString strGuiDao, strFreq ;
					AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
					BOOL b1 = g_cSystem.GetAttValue(pRef, _T("GUIDAO"), strGuiDao) ;
					BOOL b2 = g_cSystem.GetAttValue(pRef, _T("FREQUENCE"), strFreq) ;
					if (b1)
					{
						strArrayGuiDao.Add(strGuiDao) ;
						strArrayFreq.Add(strFreq) ;
					}

				}				
				pEnt->close() ;
				pEnt = NULL ;
			}

		}

		ads_ssfree(sum_name) ;	

		CEditFreqDlg dlg ;
		dlg.m_strArrayGuiDao.Copy(strArrayGuiDao) ;
		dlg.m_strArrayFreq.Copy(strArrayFreq) ;

		if (IDOK==dlg.DoModal())
		{			
			this->m_pDb = g_pCurDb ;
			int iGuiDaoNum = dlg.m_strArrayGuiDao.GetSize() ;
			for (int i=0; i<iGuiDaoNum; i++)
			{
				CString strGuiDao, strFreq ;
				strGuiDao = dlg.m_strArrayGuiDao.GetAt(i) ;
				strFreq = dlg.m_strArrayFreq.GetAt(i) ;
				this->SetOneGuiDaoFreq(strGuiDao, strFreq) ;

			}
		}

	}
}

// 设置起始轨道频率（根据起始频率和频率布置规则自动计算其他频率）
void CMaXuBiaoForHD::SetStartFreq(void)
{
	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;
	this->m_pDb = g_pCurDb ;

	ads_name start_name, end_name ;
	CString strStartGuiDao, strEndGuiDao ;
	TCHAR strFreq[100] ;
	strFreq[0] = _T('\0') ;
	int iUpOrDown = 0 ;
	BOOL bGetGuiDao = FALSE ;
	ads_printf(_T("设置起始轨道频率:")) ;
	int iRet1 = g_cSystem.SelectAcDbEnt(_T("\n选择一个起始轨道:"), _T("AcDbBlockReference"), start_name) ;
	if(iRet1)
	{
		AcDbEntity * pEnt = NULL ;		
		if (g_cSystem.OpenAcDbEntity(start_name, pEnt) )
		{
			pEnt->highlight() ;
			int iRet2 = g_cSystem.SelectAcDbEnt(_T("\n选择一个结束轨道:"), _T("AcDbBlockReference"), end_name) ;
			if ( iRet2)
			{
				AcDbEntity * pEnt2 = NULL ;		
				if (g_cSystem.OpenAcDbEntity(end_name, pEnt2) )
				{
					pEnt2->highlight() ;				

					AcDbBlockReference * pRef = (AcDbBlockReference *) pEnt ;
					BOOL b1 = g_cSystem.GetAttValue(pRef, _T("GUIDAO"), strStartGuiDao) ;					
					AcDbBlockReference * pRef2 = (AcDbBlockReference *) pEnt2 ;
					BOOL b2 = g_cSystem.GetAttValue(pRef2, _T("GUIDAO"), strEndGuiDao) ;	

					if (b1&&b2)
					{
						bGetGuiDao = TRUE ;
					}
					pEnt2->unhighlight() ;
					pEnt2->close() ;
					pEnt2 = NULL ;
				}

			}
			else 
			{
				pEnt->unhighlight() ;
				pEnt->close() ;
				pEnt = NULL ;
				return ;
			}

			pEnt->unhighlight() ;
			pEnt->close() ;
			pEnt = NULL ;
		}
	}
	else 
	{
		return ;
	}

	if (bGetGuiDao == FALSE)
	{
		return ;
	}

	int iInputRet = ads_getstring(1, _T("\n输入起始频率:"), strFreq) ;
	if (RTNORM != iInputRet)
	{
		return ;
	}
	int iInputRet2 = ads_getint(_T("\n输入上(1)/下(0)行:"), &iUpOrDown) ;
	if (RTNORM != iInputRet2)
	{
		return ;
	}

	int iIndex = -1 ;
	for (int iFreqIndex=0; iFreqIndex<4; iFreqIndex++)
	{
		if (iUpOrDown==0)
		{	
			if (g_strFreq_Down_1[iFreqIndex] == strFreq)
			{
				iIndex = iFreqIndex ;
			}
		}
		else
		{
			if (g_strFreq_Up_1[iFreqIndex] == strFreq)
			{
				iIndex = iFreqIndex ;
			}
		}

	}
	if (iIndex!=-1)
	{		
		this->SetGuiDaoFreq(strStartGuiDao, strEndGuiDao, iIndex, iUpOrDown) ;		
	}
}
