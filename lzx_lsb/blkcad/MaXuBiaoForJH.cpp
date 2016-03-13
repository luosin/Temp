// MaXuBiaoForJH.cpp: implementation of the CMaXuBiaoForJH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MaXuBiaoForJH.h"

#include "UserUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString g_cstrMaXu_1[] = {"ռ��","HU","U","LU","L","L2","L3","L4","L5"} ;
CString g_cstrLinZhanState_1 [] = {"L5","L4","L3","L2","L","LU","U","HU","ռ��"} ;

CString g_cstrMaXu_CeXiang_1[] = {"UU","U2","LU","L","L2","L3","L4","L5"} ;
CString g_cstrMaXu_CeXiang2_1[] = {"UUS","U2S","LU","L","L2","L3","L4","L5"} ;
CString g_cstrMaXu_YinDao_1[] = {"HB","U","LU","L","L2","L3","L4","L5"} ;
CString g_strFreq_Down_1[] = {"1700-1", "2300-1", "1700-2", "2300-2"} ;
CString g_strFreq_Up_1[] = {"2000-1", "2600-1", "2000-2", "2600-2"} ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaXuBiaoForJH::CMaXuBiaoForJH()
{	
	m_iDwgIndex = 0 ;
	dTableStart_X = 0 ; //�����ʼx
	dTableStart_Y = 0 ; //�����ʼy	
	dTableCur_X = dTableStart_X ; //��ǰ���X
	dTableCur_Y	= dTableStart_Y ; //��ǰ���Y
    dTableCellHeight = 7 ; //���Ԫ��߶�
	dTableCellWidth = 18 ;	//���Ԫ����
	dBetweenTable_Y = 260 ;//����ࣨ���£�360
	dDescribeCellWidth = 70.0000 ; //���������
	textStyleId = NULL ;

	//��ǰ��·�Ľ���վ���������վ�źŻ���id������,�ɵ�������
	m_cstrChuZhanXHJ_F = _T("") ;
	m_cstrChuZhanXHJ = _T("") ;
	m_cstrJinZhanXHJ_F = _T("") ;
	m_cstrJinZhanXHJ = _T("") ;
	m_iChuZhanXHJ_F_id = -1 ;
	m_iChuZhanXHJ_id = -1 ;
	m_iJinZhanXHJ_F_id = -1 ;
	m_iJinZhanXHJ_id = -1 ;	
	m_iGuDao_Type = 0 ;
	m_cstrGuDao = _T("") ;	
	
	//��ǰ��·�ķ���
	//0:��������
	//1:��������
	m_iCurJinLuDirect = 0 ;

	m_iZpGuiDaoSum_Rigth = 0 ;
	m_iZpGuiDaoSum_Left = 0 ;

	m_iLinZhanStateSum = 9 ;

	m_dFrameLeft_x = -105 ;

	cstrDwgSaveDir = _T("c:\\") ;
	m_cstrFileName = _T("erro") ;
	m_strStationName = _T("XXX") ;

	m_dSiYiTuHeight = 80 ;
	m_dBetweenTableAndBottom = 40 ; //old:47

	//0:�¸���1:���
	m_iVersionFor = 0 ; 

	m_dWidthGuiDao = 0.7;
	m_dWidthBianMa = 0.7;
	m_dHeightGuiDao = 4.0;
	m_dHeightBianMa = 4.0;
	
}

CMaXuBiaoForJH::~CMaXuBiaoForJH()
{
	this->ReleaseJLArray() ;
}

//���ź�ƽ��ͼ�������ݿ�,������ƵȲ����ı���ͨ��������xdata��¼��Ϣ���˺���������ʹ�ã��ַ���
void CMaXuBiaoForJH::ReadDwgToDb(LPUNKNOWN m_pConnection, AcDbDatabase * pCurDb)
{
	CString cstrSql ;

	_ConnectionPtr ptrCon; //�����ݿ������
	ptrCon = ( _ConnectionPtr ) m_pConnection ;

	CString cstrTable[2] = {_T("guidao"),_T("signal")} ;
	for(int iTb=0; iTb<2; iTb++)
	{
		cstrSql.Format(_T("delete * from %s"),cstrTable[iTb]) ;							
		ExecSqlString(cstrSql) ;
#ifdef VER_FOR_2002
		//����id
		cstrSql.Format(_T("alter table [%s] alter column id counter(1,1)"),cstrTable[iTb]) ;
		ExecSqlString(cstrSql) ;
#endif
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
	CString cstrAppNamePre[3] = {_T("PRE1"),_T("PRE2"), _T("PRE3") } ;
	CString cstrAppNameNext[3] = {_T("NEXT1"),_T("NEXT2"), _T("NEXT3") } ;
	//CString cstrAppName
	
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbText * pText = NULL ;
			
			//�����ı� AcDbMText ;
			if (pEnt->isKindOf(AcDbText::desc()))			
			{
				CString cstrType ;
				CString cstrText ;	
				if (g_cSystem.Get_Xdata(pEnt,_T("TYPE"), cstrType))
				{
					if (cstrType==_T("GUIDAO"))
					{
						CString cstrPre[3] = {_T(""), _T(""), _T("")} ;
						CString cstrNext[3] = {_T(""), _T(""), _T("")} ;
						CString cstrXHJ , cstrXHJ2 ;
						int iGuidaoType = 0 ;
						int iUpOrDown = 0 ;
						int iTopOrBottom = 0 ;
						int iStartEnd = 0 ;
						CString strFreq = _T("") ;
						CString cstrTmp ;
						int iTmp = -1 ;

						for(int i=0; i<3; i++)
						{
							g_cSystem.Get_Xdata(pEnt,cstrAppNamePre[i], cstrPre[i]) ;
						}
						for(int i2=0; i2<3; i2++)
						{
							g_cSystem.Get_Xdata(pEnt,cstrAppNameNext[i2], cstrNext[i2]) ;
						}
						if (g_cSystem.Get_Xdata(pEnt,_T("XINHAOJI"),cstrTmp))
						{
							cstrXHJ = cstrTmp ;
						}
						if (g_cSystem.Get_Xdata(pEnt,_T("XINHAOJI2"),cstrTmp))
						{
							cstrXHJ2 = cstrTmp ;
						}
						if (g_cSystem.Get_Xdata(pEnt,_T("GUIDAOTYPE"),iTmp)) 
						{
							iGuidaoType = iTmp ;
						}
						if (g_cSystem.Get_Xdata(pEnt,_T("UPORDOWN"),iTmp)) 
						{
							iUpOrDown = iTmp ;
						}
						if (g_cSystem.Get_Xdata(pEnt,_T("TOPORBOTTOM"),iTmp)) 
						{
							iTopOrBottom = iTmp ;
						}
						
						if (g_cSystem.Get_Xdata(pEnt,_T("GUIDAOSTARTEND"),iTmp)) 
						{
							iStartEnd = iTmp ;
						}
						if (g_cSystem.Get_Xdata(pEnt,_T("FREQUENCE"),cstrTmp)) 
						{
							strFreq = cstrTmp ;
						}
						
						pText = (AcDbText *)pEnt ;
						cstrText = pText->textString() ;
						
						try
						{
							cstrSql.Format(_T("insert into guidao(name,pre1,pre2,pre3,next1,next2,next3,signal,signal2, type, upordown,startend,toporbottom,frequence) \
								values('%s','%s','%s','%s','%s','%s','%s','%s', '%s', %d,%d,%d,%d,'%s')"),\
								cstrText,cstrPre[0],cstrPre[1],cstrPre[2],cstrNext[0],cstrNext[1],cstrNext[2],cstrXHJ,cstrXHJ2,iGuidaoType, iUpOrDown,iStartEnd,iTopOrBottom,strFreq) ;
							ExecSqlString(cstrSql) ;
						}
						catch (_com_error& e)
						{
							CString sBuff = GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox(_T("unknown error in ReadDwgToDb")) ;
							
						}						
					}
					else if (cstrType==_T("XINHAOJI")) 
					{										
						pText = (AcDbText *)pEnt ;
						cstrText = pText->textString() ;

						CString cstrGuiDao(_T("")) ;
						int iXHJType = 0 ;
						int iXHJUpDown = 0 ;
						int iXHJDirect = 0 ;
						g_cSystem.Get_Xdata(pEnt,_T("GUIDAO"), cstrGuiDao) ;
						g_cSystem.Get_Xdata(pEnt,_T("XHJTYPE"), iXHJType) ;
						g_cSystem.Get_Xdata(pEnt,_T("UPORDOWN"), iXHJUpDown) ;
						g_cSystem.Get_Xdata(pEnt,_T("FANXIANG"),iXHJDirect) ;
						try
						{
							cstrSql.Format(_T("insert into signal(name,type, upordown,direct,guidao) \
								values('%s',%d,%d,%d,'%s')"),cstrText,iXHJType,iXHJUpDown,iXHJDirect,cstrGuiDao) ;
							ExecSqlString(cstrSql) ;				
						
						}
						catch (_com_error& e)
						{
							CString sBuff = GetErrorDescription(e) ;
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
void CMaXuBiaoForJH::FindJinLu(CString cstrStart, CString cstrEnd, CStringArray &cstrArray)
{
	CStringArray cstrArray1 ;
	cstrArray1.Copy(cstrArray) ;
	
	cstrArray1.Add(cstrStart) ;
	if (cstrStart!=cstrEnd)
	{
		try
		{
			CADORecordset myset ;
			CString cstrSql ;
			cstrSql.Format(_T("select * from guidao where name='%s'"), cstrStart) ;
			myset.Open(g_PtrCon, cstrSql) ;
			
			if (!myset.IsBOF())
			{
				myset.MoveFirst() ;
			}
			while (!myset.IsEOF())
			{
				CString cstrNext1, cstrNext2, cstrNext3 ;
				myset.GetFieldValue(_T("next1"), cstrNext1) ;
				myset.GetFieldValue(_T("next2"), cstrNext2) ;
				myset.GetFieldValue(_T("next3"), cstrNext3) ;

				if (cstrNext1!=_T(""))
				{
					this->FindJinLu(cstrNext1, cstrEnd, cstrArray1) ;
				}
				if (cstrNext2!=_T("")) 
				{
					this->FindJinLu(cstrNext2, cstrEnd, cstrArray1) ;
				}
				if (cstrNext3!=_T("")) 
				{
					this->FindJinLu(cstrNext3, cstrEnd, cstrArray1) ;
				}
				
				myset.MoveNext() ;
			}
			myset.Close() ;			
		}
		catch (_com_error& e)
        {
			CString sBuff = GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in FindJinLu")) ;			
		}       
	}
	else//����ĩ��
	{
		if (cstrArray1.GetSize()>2)
		{
			CStringArray * cstrArrayTmp = new CStringArray ;
			cstrArrayTmp->Copy(cstrArray1) ;
			this->arrayJinLu.Add(cstrArrayTmp) ;
		}
	}
}

//��ĳһ��·���б���
//////////////////////////////////////////////////////////////////////////
BOOL CMaXuBiaoForJH::JinLuBianMa(CStringArray *cstrArrayJinLu)
{
	BOOL bRet = FALSE ;
	CString cstrSql ;
	CString cstrTempletFileName, cstrDwgFileName ;
	cstrTempletFileName.Format(_T("%s\\support\\maxubiao\\samp2.dwg"), m_strSysBasePath) ;	 
	//cstrDwgSaveDir.Format("E:\\file\\maxubiao") ;


	CString cstrMsg = _T("") ;
	
	cstrSql.Format(_T("delete * from jinlu")) ;
	ExecSqlString(cstrSql) ;
		
	//���˽�·�����й������д��jinlu��
	for(int i=0; i<cstrArrayJinLu->GetSize(); i++)
	{
		//����һ��������ݵ�jinlu�����������Ϣ
		this->ReadOneGuiDaoToDb(cstrArrayJinLu->GetAt(i)) ;	
		cstrMsg.Format(_T("%s-%s"), cstrMsg, cstrArrayJinLu->GetAt(i)) ;
	}	
		 	
	//�ж�����������Ľ�·��Ȼ����ݸ������д�����
	try
	{
		g_pCurDb = new AcDbDatabase ;
		g_cSystem.pDb = g_pCurDb ;
	
		//this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt.shx", 2, 0.7) ;	

		CADORecordset cSet ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen()) 
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon,cstrSql) ;

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
		CString cstrFirstXHJ = _T("") ;
		CString cstrLastXHJ = _T("") ;

		//�ж����޹ɵ���ȷ���Ƿ��м�վ
		BOOL bFindGuDao = FALSE ;
		//�ж��ڽ�վ�ֽ�ʱ����һ���ɵ��ķ����źŻ����ͣ�X(1) or XN(2)
		int _iFirstGuiDao_XHJ = -1 ;
		//�ж��ж��ڽ�վ�ֽ�ʱ�����һ���ɵ��ķ����źŻ����ͣ�S(1) or SN(2)
		int _iLastGuiDao_XHJ = -1 ;	
		
		if (cSet.GetRecordCount()>0)
		{
			//��һ�������Ϣ
			if (!cSet.IsBOF()) 
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("guidao_updown"),iFirstGuiDaoUpDown) ;
			cSet.GetFieldValue(_T("signal_type"),iFirstGuiDao_XHJType) ;
			cSet.GetFieldValue(_T("signal_updown"),iFirstGuiDao_XHJUpDown) ;
			cSet.GetFieldValue(_T("signal_direct"), iFirstGuiDao_XHJDirect) ;
			cSet.GetFieldValue(_T("signal"),cstrFirstXHJ) ;	
			//���һ�������Ϣ
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("guidao_updown"),iLastGuiDaoUpDown) ;
			cSet.GetFieldValue(_T("signal_type"),iLastGuiDao_XHJType) ;
			cSet.GetFieldValue(_T("signal_updown"),iLastGuiDao_XHJUpDown) ;
			cSet.GetFieldValue(_T("signal_direct"), iLastGuiDao_XHJDirect) ;
			cSet.GetFieldValue(_T("signal"),cstrLastXHJ) ;

		}		
		else
		{
			AfxMessageBox(_T("��·����Ϊ��")) ;
			return FALSE ;
		}
		
		//�����Ĺɵ���Ϣ���������ߣ��������ߣ�����
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		bFindGuDao = cSet.Find(_T("guidao_type>0")) ;
		if (bFindGuDao) 
		{
			cSet.GetFieldValue(_T("guidao_type"),iGuDaoType) ;
		}
		
		//�ж��ڽ�վ�ֽ�ʱ����һ���ɵ��ķ����źŻ����ͣ�X(1) or XN(2)
		if (cstrFirstXHJ!=_T(""))
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
		//�ж��ڽ�վ�ֽ�ʱ�����һ���ɵ��ķ����źŻ����ͣ�S(1) or SN(2)		
		if (cstrLastXHJ!=_T(""))
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

		this->GetJinLuGuiDaoFreq(*cstrArrayJinLu, m_strArrayCurJinLuFre) ;
		
		//������У���һ����������й�����߷����źŻ���X��		
		if (iFirstGuiDaoUpDown==1 || _iFirstGuiDao_XHJ==1)
		{
			//�Ҳ����У����һ����������й����������������źŻ���SN������x1lq��
			if (iLastGuiDaoUpDown==1 || _iLastGuiDao_XHJ==2 || bIsLastGuiDaoLQ == TRUE) 
			{
				//û���ҵ��ɵ������м�վ,����
				if (bFindGuDao==FALSE)
				{
					
					if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
					{		
						this->dTableCur_X = 0 ;
						this->JinLuBianMa_ZJZ_X() ;
						this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
						this->JinLuBianMa_ZJZ_X_F() ;
						this->dTableCur_Y = this->dTableStart_Y ;
						
					}
					cstrDwgFileName.Format(_T("%s\\%d_ZJZ_X.dwg"),cstrDwgSaveDir, this->m_iDwgIndex  ) ;

					g_pCurDb->saveAs(cstrDwgFileName) ;
					
					this->m_iDwgIndex++ ;
					
				}				
				//�����Ĺɵ����������߹ɵ�
				else if (iGuDaoType==1) 
				{					
					//�м�û�з���·�ߣ���·�����еĹ������һ���ڵ㶼��next1��
					//if (this->IsDirectJinLu())
					//{	
						
						if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
						{	
							this->dTableCur_X = 0 ;							
							this->JinLuBianMa_X_Z_X() ; //ok
							this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;							
							this->JinLuBianMa_X_Z_X_F() ;//��һ���������
							this->dTableCur_Y = this->dTableStart_Y ;

						}
						cstrDwgFileName.Format(_T("%s\\%d_%s.dwg"), cstrDwgSaveDir, this->m_iDwgIndex, m_cstrFileName) ;

						g_pCurDb->saveAs(cstrDwgFileName) ;

						this->m_iDwgIndex++ ;

						cstrMsg.Format(_T("%s\nX_Z_X"), cstrMsg) ;
					//}
					//�м��з���·�ߣ������߰���					
					/*else
					{	

						this->JinLuBianMa_X_Z_X() ;
						this->JinLuBianMa_X_Z_X_F() ;
						cstrMsg.Format(_T("%s\nX_Z_X2"), cstrMsg) ;
					}
					*/
				}
				//�����Ĺɵ������������߹ɵ�				
				else 
				{					
					
					if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
					{	
						this->dTableCur_X = 0 ;						
						this->JinLuBianMa_X_C_X() ; //ok
						this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
						this->JinLuBianMa_X_C_X_F() ;//��һ���������
						this->dTableCur_Y = this->dTableStart_Y ;						
					}
					cstrDwgFileName.Format(_T("%s\\%d_%s.dwg"), cstrDwgSaveDir,this->m_iDwgIndex, m_cstrFileName) ;

					g_pCurDb->saveAs(cstrDwgFileName) ;
					this->m_iDwgIndex++ ;
					
					cstrMsg.Format(_T("%s\nX_c_X"), cstrMsg) ;
				}
			}
			//�Ҳ����У����һ����������й����������������źŻ���S��
			else if (iLastGuiDaoUpDown==2 || _iLastGuiDao_XHJ==1 ) 
			{				
				
				if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
				{
					this->dTableCur_X = 0 ;
					this->JinLuBianMa_X_ZC_S() ; //ok
					this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
					this->JinLuBianMa_X_ZC_S_F() ;//��һ���������
					this->dTableCur_Y = this->dTableStart_Y ;					
				}
				cstrDwgFileName.Format(_T("%s\\%d_%s.dwg"), cstrDwgSaveDir, this->m_iDwgIndex, m_cstrFileName) ;

				g_pCurDb->saveAs(cstrDwgFileName) ;

				this->m_iDwgIndex++ ;				
				
				cstrMsg.Format(_T("%s\nX_Zc_s"), cstrMsg) ;
			}
		}
		//������У���һ����������й�����߷����źŻ���XN ����s1lq��		
		else if (iFirstGuiDaoUpDown==2 || _iFirstGuiDao_XHJ==2 || bIsFirstGuiDaoLQ == TRUE)
		{
			//�Ҳ����У����һ����������й������������źŻ���S��
			if (iLastGuiDaoUpDown==2 || _iLastGuiDao_XHJ==1)
			{
				//û���ҵ��ɵ������м�վ,����
				if (bFindGuDao==FALSE)
				{	
					
					if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
					{	
						this->dTableCur_X = 0 ;
						this->JinLuBianMa_ZJZ_S() ;
						this->dTableCur_X = dDescribeCellWidth - dTableCellWidth ;
						this->JinLuBianMa_ZJZ_S_F() ;
						this->dTableCur_Y = this->dTableStart_Y ;							
					}
					cstrDwgFileName.Format(_T("%s\\%d_ZJZ_S.dwg"), cstrDwgSaveDir, this->m_iDwgIndex) ;

					g_pCurDb->saveAs(cstrDwgFileName) ;
					
					this->m_iDwgIndex++ ;						
					
				}				
				//�������߹ɵ��������Ĺɵ����������߹ɵ���
				else if (iGuDaoType==2)
				{
					//�м�û�з���·�ߣ���·�����еĹ������һ���ڵ㶼��next1��
					//if (this->IsDirectJinLu())
					//{						
						
						if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
						{	
							this->dTableCur_X = 0 ;
							this->JinLuBianMa_S_Z_S() ; //ok
							this->dTableCur_X = dDescribeCellWidth - dTableCellWidth ;
							this->JinLuBianMa_S_Z_S_F() ;//��һ���������
							this->dTableCur_Y = this->dTableStart_Y ;							
						}
						cstrDwgFileName.Format(_T("%s\\%d_%s.dwg"), cstrDwgSaveDir, this->m_iDwgIndex, m_cstrFileName) ;

						g_pCurDb->saveAs(cstrDwgFileName) ;

						this->m_iDwgIndex++ ;						
						
						cstrMsg.Format(_T("%s\ns_Z_s"), cstrMsg) ;
					//}
					//�м��߹�����·�ߣ���·�в������еĹ������һ���ڵ㶼��next1��
				/*	else 
					{
						JinLuBianMa_S_Z_S() ;
						this->JinLuBianMa_S_Z_S_F() ;
						cstrMsg.Format(_T("%s\ns_Z_s2"), cstrMsg) ;
					} */
				}
				//�����Ĺɵ������������߹ɵ�				
				else 
				{
					
					if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
					{						
						this->dTableCur_X = 0 ;
						this->JinLuBianMa_S_C_S() ; //ok
						this->dTableCur_X = dDescribeCellWidth - dTableCellWidth ;
						this->JinLuBianMa_S_C_S_F() ;//��һ���������
						this->dTableCur_Y = this->dTableStart_Y ;						
					}
					cstrDwgFileName.Format(_T("%s\\%d_%s.dwg"), cstrDwgSaveDir, this->m_iDwgIndex, m_cstrFileName) ;
					g_pCurDb->saveAs(cstrDwgFileName) ;
					this->m_iDwgIndex++ ;					
					cstrMsg.Format(_T("%s\ns_c_s"), cstrMsg) ;
				}
			}
			//�Ҳ����У����һ����������й����������������źŻ���SN��
			else if (iLastGuiDaoUpDown==1 || _iLastGuiDao_XHJ==2)
			{	
				
				if (Acad::eOk== g_pCurDb->readDwgFile(cstrTempletFileName))
				{
					this->dTableCur_X = 0 ;
					this->JinLuBianMa_S_ZC_X() ; //ok
					this->dTableCur_X = dTableCellWidth - dDescribeCellWidth ;
					this->JinLuBianMa_S_ZC_X_F() ;//��һ���������		
					
					this->dTableCur_Y = this->dTableStart_Y ;					
				}
				cstrDwgFileName.Format(_T("%s\\%d_%s.dwg"), cstrDwgSaveDir, this->m_iDwgIndex, m_cstrFileName) ;

				g_pCurDb->saveAs(cstrDwgFileName) ;

				this->m_iDwgIndex++ ;				
				
				cstrMsg.Format(_T("%s\ns_Zc_X"), cstrMsg) ;
			}
		}

		cSet.Close() ;

		delete g_pCurDb ;
		g_pCurDb = NULL ;

	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa")) ;
	} 			
	
	//AfxMessageBox(cstrMsg) ;
	
	return bRet ;

}

void CMaXuBiaoForJH::ReadOneGuiDaoToDb(CString cstrGuiDao)
{
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		
		CString cstrpre1, cstrpre2, cstrpre3, cstrnext1, cstrnext2, cstrnext3 ;
		
		CString cstrSignal, cstrSignal2 ;				
		int iGuiDaoType = 0 ;				
		int iGuiDaoUpDown = 0 ;	
		int iGuiDaoTopBottom = 0 ;
		int iXHJType = 0, iXHJType2 = 0 ;
		int iXHJUpDown = 0, iXHJUpDown2 = 0 ;
		int iXHJDirect = 0 ;				
		
		cstrSql.Format(_T("select * from guidao where name='%s'"), cstrGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon,cstrSql) ;
		int iRecNum = cSet.GetRecordCount() ;
		if (iRecNum>0)
		{
			if (iRecNum>1)
			{
				CString cstrMsg ;
				cstrMsg.Format(_T("���%s�в�ֹ1����¼"),cstrGuiDao) ;
				AfxMessageBox(cstrMsg) ;
			}
			cSet.MoveFirst() ;
			cSet.GetFieldValue(_T("pre1"), cstrpre1) ;
			cSet.GetFieldValue(_T("pre2"), cstrpre2) ;
			cSet.GetFieldValue(_T("pre3"), cstrpre3) ;
			cSet.GetFieldValue(_T("next1"), cstrnext1) ;
			cSet.GetFieldValue(_T("next2"), cstrnext2) ;
			cSet.GetFieldValue(_T("next3"), cstrnext3) ;
			cSet.GetFieldValue(_T("signal"), cstrSignal) ;
			cSet.GetFieldValue(_T("signal2"), cstrSignal2) ;
			cSet.GetFieldValue(_T("type"),iGuiDaoType) ;
			cSet.GetFieldValue(_T("upordown"),iGuiDaoUpDown) ;
			cSet.GetFieldValue(_T("toporbottom"), iGuiDaoTopBottom) ;
			
			if (cstrSignal!=_T(""))
			{
				CADORecordset cSet2 ;
				cstrSql.Format(_T("select * from signal where name='%s'"),cstrSignal ) ;
				if (cSet2.IsOpen())
				{
					cSet2.Close() ;
				}
				cSet2.Open(g_PtrCon, cstrSql) ;
				if (cSet2.GetRecordCount()>0) 
				{
					if (cSet2.GetRecordCount()>1)
					{
						CString cstrMsg ;
						cstrMsg.Format(_T("�źŻ�%s�в�ֹ1����¼"),cstrSignal) ;
						AfxMessageBox(cstrMsg) ;							
					}
					cSet2.MoveFirst() ;
					cSet2.GetFieldValue(_T("type"), iXHJType) ;
					cSet2.GetFieldValue(_T("upordown"), iXHJUpDown) ;
					cSet2.GetFieldValue(_T("direct"), iXHJDirect) ;
					
				}
				cSet2.Close() ;
			}
			if (cstrSignal2!=_T(""))
			{
				CADORecordset cSet3 ;
				cstrSql.Format(_T("select * from signal where name='%s'"),cstrSignal2 ) ;
				if (cSet3.IsOpen())
				{
					cSet3.Close() ;
				}
				cSet3.Open(g_PtrCon, cstrSql) ;
				if (cSet3.GetRecordCount()>0) 
				{
					if (cSet3.GetRecordCount()>1)
					{
						CString cstrMsg ;
						cstrMsg.Format(_T("�źŻ�%s�в�ֹ1����¼"),cstrSignal2) ;
						AfxMessageBox(cstrMsg) ;							
					}
					cSet3.MoveFirst() ;
					cSet3.GetFieldValue(_T("type"), iXHJType2) ;
					cSet3.GetFieldValue(_T("upordown"), iXHJUpDown2) ;
					
				}
				cSet3.Close() ;
			}
			
			cstrSql.Format(_T("insert into jinlu(guidao,pre1,pre2,pre3,next1,next2,next3,\
							signal,signal2,guidao_topbottom,guidao_type,guidao_updown,signal_type,signal_updown, signal_type2,signal_updown2,signal_direct) \
							values('%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%d)"),\
							cstrGuiDao,cstrpre1,cstrpre2,cstrpre3,cstrnext1,cstrnext2,cstrnext3,cstrSignal,cstrSignal2,\
							iGuiDaoTopBottom,iGuiDaoType,iGuiDaoUpDown,iXHJType,iXHJUpDown,iXHJType2,iXHJUpDown2,iXHJDirect ) ;
			ExecSqlString(cstrSql) ;
			
		}
		cSet.Close() ;	
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error ReadOneGuiDaoToDb ")) ;
	} 			
				
}

BOOL CMaXuBiaoForJH::IsDirectJinLu()
{
	BOOL bRet = TRUE ;
	try	
	{
		CADORecordset cSet ;
		CString cstrSql ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF()) 
		{
			CString cstrNext1 ;
			cSet.GetFieldValue(_T("next1"), cstrNext1) ;
			cSet.MoveNext() ;
			if (!cSet.IsEOF())
			{
				CString cstrGuiDao ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (cstrNext1!=cstrGuiDao)
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
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in IsDirectJinLu")) ;
	} 	
	
	return bRet ;

}

void CMaXuBiaoForJH::JinLuBianMa_X_Z_X()
{	
	this->m_iCurJinLuDirect = 0 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ�
	BOOL bPassXN = FALSE ; //�Ƿ�Խ����վ�źŻ�
	int iCodeTypeTmp = 0 ; //��������� 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;
		
		//�ҳ��½�վ,�³�վ
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;
		this->m_cstrFileName.Format(_T("%s_%s_%s"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ��½�վ,�³�վ		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//��վ��_T("L5,L4,...HU,ռ��")��9�����
		//////////////////////////////////////////////////////////////////////////		
		//CString cstrStartCodeTmp[9] = {_T("L5","L4","L3","L2","L","LU","U","HU","ռ��")} ;
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%sͨ��)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}			
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;

			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;

			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;

				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp) ;
				
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
							
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
		
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������		
			if (iGuiDaoId>this->m_iChuZhanXHJ_id)
			{
				cstrDescribeTmp.Format(_T("%sռ��(%s����%sͨ��)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;
			}
			else
			{
				cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
				
			}
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoName,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp) ;

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////				

		//X����ֱ��XI�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		//�������:X����ֱ��XI�ر�
		cstrDescribeTmp.Format(_T("%s����%s,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}

		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN = TRUE ;
				}
			}
			
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	
		
		if(m_iVersionFor == 1)
		{
			//�������:X����ֱ����XI��XI��������
			
			cstrDescribeTmp.Format(_T("%s����ֱ����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN = TRUE ;
					}
				}
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
				cSet.MovePrevious() ;
				
				// 			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				// 			this->GetGuiDaoBianMa(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, 2) ;
				// 			cSet.MovePrevious() ;			
			}
			CStringArray * pcstrArrayTmpXIYinDao = new CStringArray() ;
			pcstrArrayTmpXIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpXIYinDao) ;
		}
		
		cSet.Close() ;	

		//end X����ֱ��XI�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
		//X���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		//����		
		cstrDescribeTmp.Format(_T("%s���Ų���%s�ر�"), this->m_cstrJinZhanXHJ,this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pcstrArrayTmpX_C = new CStringArray() ;
		pcstrArrayTmpX_C->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpX_C) ;	

		//�ر�
		cstrDescribeTmp.Format(_T("%s��վ�ر�"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pcstrArrayTmpX_CLOSE = new CStringArray() ;
		pcstrArrayTmpX_CLOSE->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpX_CLOSE) ;	

		//X��������
		cstrDescribeTmp.Format(_T("%s��������"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 2) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pcstrArrayTmpX_Y = new CStringArray() ;
		pcstrArrayTmpX_Y->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpX_Y) ;	
		
		cSet.Close() ;	
		
		//end X���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_z_x"),sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error x_z_x")) ;
	} 

	//���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2) ;
	//end ���������
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


void CMaXuBiaoForJH::DoALL()
{
	//this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt.shx", 2, 0.7) ;		
	
	//g_cSystem.pDb = acdbHostApplicationServices()->workingDatabase() ;

	g_cSystem.GetBasePath(m_strSysBasePath) ;//ϵͳ·��
	this->ReleaseJLArray() ;
	this->m_iDwgIndex = 0 ;
		
	try
	{
		//��ѯ���ݿ⣬�ҳ���ʼ��ĩ�˹��
		CStringArray cstrArrayFirstGuiDao, cstrArrayLastGuiDao ;

		CADORecordset cSet ;
		CString cstrSql ;
		//ǰ��û�й������Ϊ�ǿ�ʼ������������ֹ�ָ����ʼ���
		cstrSql.Format(_T("select * from guidao where pre1='' or pre1='start' ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF()) 
		{
			CString cstrFirstGuiDao ;
			cSet.GetFieldValue(_T("name"), cstrFirstGuiDao) ;

			ads_printf(_T("\n��ʼ�����%s\n"),cstrFirstGuiDao) ;			
			cstrArrayFirstGuiDao.Add(cstrFirstGuiDao) ;
			cSet.MoveNext() ;
		}
		cSet.Close() ;
		
		//�ҳ�ĩ�˹��
		cstrSql.Format(_T("select * from guidao where next1='' or next1='end' ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF()) 
		{
			CString cstrLastGuiDao ;
			cSet.GetFieldValue(_T("name"), cstrLastGuiDao) ;
			ads_printf(_T("\nĩ�˹����%s\n"),cstrLastGuiDao) ;				
			cstrArrayLastGuiDao.Add(cstrLastGuiDao) ;
			cSet.MoveNext() ;
		}
		cSet.Close() ;

		int iFirstGuiDaoSum = cstrArrayFirstGuiDao.GetSize() ;
		int iLastGuiDaoSum = cstrArrayLastGuiDao.GetSize() ;
		ads_printf(_T("\n��ʼ������·��......\n")) ;
		//acedSetStatusBarProgressMeter(_T("������·"), 0, iFirstGuiDaoSum*iLastGuiDaoSum-1) ;
		
		for(int iFirstGuiDaoNum=0; iFirstGuiDaoNum<iFirstGuiDaoSum; iFirstGuiDaoNum++)
		{
			for(int iLastGuiDaoNum=0; iLastGuiDaoNum<iLastGuiDaoSum; iLastGuiDaoNum++)
			{
				//acedSetStatusBarProgressMeterPos(iFirstGuiDaoNum*iLastGuiDaoSum+iLastGuiDaoNum) ;				
				CStringArray cstrArrayTmp ;
				CString cstrPassGuiDao = _T("") ;
				//test
				//if (iFirstGuiDaoNum == 1 && iLastGuiDaoNum == 0)
				//{
					this->FindJinLu(cstrArrayFirstGuiDao.GetAt(iFirstGuiDaoNum),cstrArrayLastGuiDao.GetAt(iLastGuiDaoNum), cstrPassGuiDao, cstrArrayTmp) ;
					//int iii = 0 ;
				//}
				//end test
			}
		}
		//acedRestoreStatusBar() ;		
		ads_printf(_T("\n������·������......\n")) ;		

		int iJinLuNum = this->arrayJinLu.GetSize() ;
		ads_printf(_T("\n��·������%d\n"), iJinLuNum) ;
		ads_printf(_T("\n��ʼ��·���룺......\n")) ;
		//acedSetStatusBarProgressMeter(_T("��·����"), 0, iJinLuNum-1) ;
		
		this->GetFrontAndBackStation(this->m_strStationName, m_strFrontStation, m_strBackStation) ;

		CString strBarMsg ;
		strBarMsg.Format(_T("%d����·����"), iJinLuNum) ;
#ifdef VER_FOR_2002
		acedSetStatusBarProgressMeter(strBarMsg, 0, iJinLuNum-1) ;	
#endif
		for(int iJinLuIndex=0; iJinLuIndex<iJinLuNum; iJinLuIndex++)
		{
#ifdef VER_FOR_2002
			acedSetStatusBarProgressMeterPos(iJinLuIndex) ;
#endif
			CStringArray * pArrayJinLu = arrayJinLu.GetAt(iJinLuIndex) ;
			ads_printf(_T("\nBegin��·����-%d:"),iJinLuIndex) ;
			//if (iJinLuIndex==0)
			//{
				this->JinLuBianMa(pArrayJinLu) ;
											
			//}
			ads_printf(_T("\nEnd��·����-%d:"),iJinLuIndex) ;
				
		}		
#ifdef VER_FOR_2002
		acedRestoreStatusBar() ;
#endif		
		ads_printf(_T("\n��·���������......\n")) ;		

	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in doall")) ;
	}          

}
void CMaXuBiaoForJH::JinLuBianMa_X_C_X()
{
	this->m_iCurJinLuDirect = 0 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;


	CStringArray cstrArrayGuiDao ;	//���������
	CString cstrRetBianMa = _T("") ; // GetGuiDaoBianMa(...)�Ĳ������������صõ��ı���
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;	
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ����������ΪԽ����
	BOOL bPassX = FALSE ; //�Ƿ�Խ���½�վ�źŻ����������ΪԽ����
	int iCodeTypeTmp = 0 ; //���������
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum,1) ;

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ��½�վ,�³�վ
		//////////////////////////////////////////////////////////////////////////
		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;
		this->m_cstrFileName.Format(_T("%s_%s_%s"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ��½�վ,�³�վ		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////			

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////

		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%sͨ��)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}

				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,iCodeTypeTmp) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
								
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id>%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;
			
			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%sͨ��)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;			
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassX = FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}			

				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
				
				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						
			
			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;
		
		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////	
		
		//X����ֱ��XI�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
		//�������:X����ֱ��XI�ر�
		cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassX)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					cstrRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassX = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	
		
		if (m_iVersionFor == 1)
		{	
			//�������:X����ֱ����XI��XI��������
			
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassX)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassX = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp) ;
				cSet.MovePrevious() ;			
			}
			CStringArray * pcstrArrayTmpXIYinDao = new CStringArray() ;
			pcstrArrayTmpXIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpXIYinDao) ;
		}
		
		cSet.Close() ;	
		
		//end X����ֱ��XI�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_c_x"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_C_X")) ;			
	} 

	//���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_S_Z_S()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;	
	BOOL bBianMaChange = 0 ;

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;

	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ�
	BOOL bPassXN = FALSE ; //�Ƿ�Խ����վ�źŻ�
	int iCodeTypeTmp = 0 ; //��������� 
					
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;
	
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ��Ͻ�վ,�ϳ�վ
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"), this->m_cstrJinZhanXHJ_F, this->m_cstrChuZhanXHJ,this->m_cstrJinZhanXHJ) ;
		this->m_cstrFileName.Format(_T("%s_%s_%s"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ��Ͻ�վ,�ϳ�վ		
		//////////////////////////////////////////////////////////////////////////	

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%sͨ��)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;
			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp,2) ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}				
				}				
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, 0, 2) ;
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////		
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������	
			if (iGuiDaoId<this->m_iChuZhanXHJ_id)
			{
				cstrDescribeTmp.Format(_T("%sռ��(%s����%sͨ��)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;
			}
			else
			{
				cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
			}
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id) 
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp,2) ;
					
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Rigth>0)
							{
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;
						}
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				}
				//this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 2) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		//S����ֱ��SII�ر�,��������
		//////////////////////////////////////////////////////////////////////////		

		//�������:S����ֱ��SII�ر�
		cstrDescribeTmp.Format(_T("%s����%s,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Rigth>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
			
			//this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 2) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpSII = new CStringArray() ;
		pcstrArrayTmpSII->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpSII) ;	
		
		if(m_iVersionFor == 1)
		{
			//�������:S����ֱ����SII��SII��������
			cstrDescribeTmp.Format(_T("%s����ֱ����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDao,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				
				cSet.MoveNext() ;
				
				// 			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				// 			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, 2, 2) ;
				// 			cSet.MoveNext() ;			
			}
			CStringArray * pcstrArrayTmpSIIYinDao = new CStringArray() ;
			pcstrArrayTmpSIIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end S����ֱ��SII�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
		//S���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		//����		
		cstrDescribeTmp.Format(_T("%s���Ų���,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 1, 2) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pcstrArrayTmpS_C = new CStringArray() ;
		pcstrArrayTmpS_C->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpS_C) ;	

		//�ر�
		cstrDescribeTmp.Format(_T("%s��վ�ر�"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 2) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pcstrArrayTmpS_CLOSE = new CStringArray() ;
		pcstrArrayTmpS_CLOSE->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpS_CLOSE) ;	

		//S��������
		cstrDescribeTmp.Format(_T("%s��������"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 2, 2) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pcstrArrayTmpS_Y = new CStringArray() ;
		pcstrArrayTmpS_Y->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpS_Y) ;	
		
		
		cSet.Close() ;	
		
		//end S���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s")) ;
	} 

    //���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange, 1) ;	
	//end ���������
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

BOOL CMaXuBiaoForJH::GetJinZhanOrChuZhan(CString &cstrJinZhan, int &iJinZhanId, CString &cstrChuZhan, int &iChuZhanId, CString &cstrGuDao,int &iGuDaoType, CString &cstrTowardsJinZhanXHJ, int iUpDown, int iDirect)
{
	BOOL bRet = TRUE ;
	//�ҳ���վ,��վ
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//�ҳ���վ		
		int iUpDown_Sql = iUpDown ;
		if (iUpDown==1 && iDirect==1)
		{
			iUpDown_Sql = 2 ;
		}
		if (iUpDown==2 && iDirect==1)
		{
			iUpDown_Sql = 1 ;
		}
		
		cstrSql.Format(_T("select * from jinlu where signal_type=1 and signal_updown=%d and signal_direct=%d"), iUpDown_Sql, iDirect ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		cSet.GetFieldValue(_T("signal"), cstrJinZhan) ;
		cSet.GetFieldValue(_T("id"), iJinZhanId) ;
		cSet.Close() ;	
		
		//�ҳ���վ		
		cstrSql.Format(_T("select * from jinlu where signal_type=2 and signal_updown=%d"), iUpDown_Sql) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("signal"), cstrChuZhan) ;
		}
		else
		{
			cstrSql.Format(_T("select * from jinlu where signal_type2=2 and signal_updown2=%d"), iUpDown_Sql) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			cSet.Open(g_PtrCon, cstrSql) ;
			if (cSet.GetRecordCount()>0)
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				cSet.GetFieldValue(_T("signal2"), cstrChuZhan) ;
			}
			
		}
		cSet.GetFieldValue(_T("guidao_type"), iGuDaoType) ;		
		cSet.GetFieldValue(_T("guidao"), cstrGuDao) ;
		cSet.GetFieldValue(_T("id"), iChuZhanId) ;
		cSet.Close() ;
		
		//�м���guidao_type>0�����ɵ���û�зָ�ΪG1��G2,����зָ�����ɵ�������Ҫȥ�����������
		cstrSql.Format(_T("select * from jinlu where guidao_type>0")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>1)
		{
			cstrGuDao = cstrGuDao.Left(cstrGuDao.GetLength()-1) ;
		}
		cSet.Close() ;


		//�ҳ������վ
		int iTmp = -1 ;
		iTmp = (iUpDown+iDirect)%2 ;
		if (iTmp==0)
		{
			cstrSql.Format(_T("select * from jinlu where signal_type=1 and id<%d"), iChuZhanId) ;
		}
		else if (iTmp>0)
		{
			cstrSql.Format(_T("select * from jinlu where signal_type=1 and id>%d"), iChuZhanId) ;
		}
		else
		{
			AfxMessageBox(_T("can't find �����վxhj")) ;
		}

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		cSet.GetFieldValue(_T("signal"), cstrTowardsJinZhanXHJ) ;
		cSet.Close() ;	
		


	}
	catch (_com_error& e)
	{
		bRet = FALSE ;
		CString sBuff = GetErrorDescription(e) ;
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

//��ʱû��
void CMaXuBiaoForJH::DrawBianMaTableForJH(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &cstrArrayGuiDao, CStringArray &cstrArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect, BOOL bIsFanXiangChuZhan)
{	
	double dLineWidthThin = 0.0000 ;
	double dLineWidthThick = 0.4000 ;
	double dTextHeight = 3.50000 ;

// 	this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt2.shx", dTextHeight, 0.7) ;	
// 	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName("zhffont_guidao", "txt.shx", "hztxt2.shx", dTextHeight, 0.5) ;

	this->textStyleId = g_cSystem.CreateTextStyleByFileName(_T("zhffont"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightBianMa, this->m_dWidthBianMa) ;	
	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName(_T("zhffont_guidao"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightGuiDao, this->m_dWidthGuiDao) ;

	CStringArray cstrArrayLinZhanState ;
	int iLinZhanStateNum = 9 ;

	for (int i=0; i<iLinZhanStateNum; i++)
	{
		cstrArrayLinZhanState.Add(g_cstrLinZhanState_1[i]) ;
	}

	if (bIsFanXiangChuZhan==1)
	{
		iLinZhanStateNum ++ ;
		cstrArrayLinZhanState.InsertAt(0, _T("L5")) ;
	}

	
	//���ģʽ
	if (m_iVersionFor==1)
	{
		iLinZhanStateNum ++ ;
		cstrArrayLinZhanState.InsertAt(0, _T("")) ;
		//ȡ�ý�·���Ƶ��
		CStringArray strArrayTmp ;
		strArrayTmp.Add(_T("")) ;
		//this->GetJinLuGuiDaoFreq(cstrArrayGuiDao, strArrayFreq) ;
		CStringArray * pcstrArrayTmp = new CStringArray() ;
		pcstrArrayTmp->Copy(strArrayTmp) ;
		arrayBianMa.InsertAt(0, pcstrArrayTmp) ;
		cstrArrayDescribe.InsertAt(0, _T("")) ;		
	}
		
	int iRow = arrayBianMa.GetSize() ;	
	int iCol = cstrArrayGuiDao.GetSize() ;
	double dVerLen = iRow*(this->dTableCellHeight) ;
	double dHorLen = iCol*(this->dTableCellWidth) ;

	ads_point base_pt = {0,0,0} ;
	base_pt[0] = 2 ;
	base_pt[1] = this->dTableCur_Y + 10 ;
	AcGePoint3d align_3d(0,0,0) ;
	g_cSystem.ConvertPtAdsTo3D(base_pt, align_3d) ;
	g_cSystem.WriteText(base_pt,align_3d, this->m_cstrCaseType,0,3,AcDb::kTextBase,AcDb::kTextLeft,this->textStyleId,5,0.7,0) ;
	
	//������	
	BOOL bBianMaChange = 0 ;
	for(int iG=0; iG<iCol+1; iG++)
	{
		AcGePoint3d start_3d(0, 0, 0) ;
		AcGePoint3d end_3d(0, 0, 0) ;
		start_3d.x = iG*(this->dTableCellWidth) + this->dTableCur_X;
		start_3d.y = dTableCur_Y + this->dTableCellHeight ;
		end_3d.x = start_3d.x ;
		end_3d.y = -dVerLen + this->dTableCur_Y ;
		//g_cSystem.AddAcDb2dLine(start_3d, end_3d) ;		
		
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
				g_cSystem.WriteMText(base_pt_linzhan, _T("(��վ)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
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
		//���Ҫ��һ�л�Ƶ��
		if (m_iVersionFor==1)
		{
			start_3d_Mid.y = start_3d_Mid.y - dTableCellHeight ;
		}
		end_3d_Mid.x = start_3d_Mid.x ;
		end_3d_Mid.y= end_3d.y ;
		
		ads_point base_pt = {0,0,0} ;
		
		base_pt[0] =  start_3d.x + dTableCellWidth/2  ;
		base_pt[1] =  dTableCur_Y + dTableCellHeight/2 ;
		

		//���һ���Ƿ���ߣ������ڻ����ߺ�д�����
		if (iG==iCol)
		{
			if (iWriteDirect==0)
			{
				g_cSystem.WriteMText(base_pt, _T("(��վ)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			break ;
		}			

		g_cSystem.AddAcDb2dPolyline(start_3d_Mid, end_3d_Mid, dLineWidthThin, 0) ;

		g_cSystem.WriteMText(base_pt, cstrArrayGuiDao.GetAt(iG), this->textStyleId_GuiDao, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		
		//��㣬addƵ��
		if(m_iVersionFor == 1)
		{		
			base_pt[1] = base_pt[1] - dTableCellHeight ;
			g_cSystem.WriteMText(base_pt, m_strArrayCurJinLuFre.GetAt(iG), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		}
	}

	//������
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
				
				g_cSystem.WriteMText(base_pt_linzhan, cstrArrayLinZhanState[iM] , this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
	
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
				
				g_cSystem.WriteMText(base_pt_linzhan, cstrArrayLinZhanState[iM], this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			
		}

		//���ϵĺ��ߣ��ڹ�����Ϸ���ֻ��һ��
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
		//���һ�����Ƿ���ߣ����²������ٽ���
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
		g_cSystem.WriteMText(base_pt, cstrArrayDescribe.GetAt(iM), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dDescribeCellWidth, 0) ;				
		
	}
	
	//�ֳ����������ߺʹ������������������
	int iBianMaMax = arrayBianMa.GetSize()-1 ;
	double dTextWidth = dTableCellWidth/2-1 ;
	if (iWriteDirect==0)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pcstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pcstrArrayTmp->GetSize()-1 ;
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
				g_cSystem.WriteMText(base_pt, pcstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTextWidth, 0) ;				
				
			}
		}
	}
	else if (iWriteDirect==1)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pcstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pcstrArrayTmp->GetSize() ;
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
				g_cSystem.WriteMText(base_pt, pcstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth/2-1, 0) ;				
				
			}
		}
		
	}

	//begin ��ͼ��
	//////////////////////////////////////////////////////////////////////////
	AcDbBlockReference * pRef = NULL ;
	CString strBlockName ;
	CStringArray strarrayAtt ;
	//double dSiYiTuHeight = 80 ;
	//double dBetweenTableAndBottom = 40 ; //old:47
	AcGePoint3d pt_3d_topleft(0,0,0),pt_3d_bottomleft(0,0,0),pt_3d_bottomright(0,0,0) ;
	
	//ֻ�ڻ�����һ�ű��ʱ����һ�α߿�x����
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
	//m_strStationName = _T("����վ") ;
	//strarrayAtt.Add(m_strStationName) ;
	//strarrayAtt.Add(m_cstrCaseType) ;
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
	AcDbObjectId objId ;
	g_cSystem.WriteText(text_pt,m_strStationName, objId, 4, 0.8, AcDb::kTextCenter, AcDb::kTextVertMid, 0, this->textStyleId) ;


	//text_pt[0] = pt_3d_bottomright.x - 80 ;
	text_pt[1] = pt_3d_bottomright.y + 12 ;
	AcDbObjectId objId2 ;
	g_cSystem.WriteText(text_pt,m_cstrCaseType, objId2, 4, 0.8, AcDb::kTextCenter, AcDb::kTextVertMid, 0, this->textStyleId) ;

	//g_cSystem.WriteText(text_pt, m_cstrCaseType, 0, AcDb::kTextMid, 3, 0.9, colr) ;
	
	AcGePoint3d pt_3d_1(pt_3d_topleft.x,pt_3d_topleft.y+5,0) ;
	AcGePoint3d pt_3d_2(pt_3d_bottomright.x+5,pt_3d_topleft.y+5,0) ;
	AcGePoint3d pt_3d_3(pt_3d_bottomright.x+5,pt_3d_bottomright.y-5,0) ;
	AcGePoint3d pt_3d_4(pt_3d_topleft.x,pt_3d_bottomright.y-5,0) ;
	AcGePoint3dArray pt_3d_array_outside, pt_3d_array_inside ;
	pt_3d_array_outside.append(pt_3d_1) ;
	pt_3d_array_outside.append(pt_3d_2) ;
	pt_3d_array_outside.append(pt_3d_3) ;
	pt_3d_array_outside.append(pt_3d_4) ;
	//��ʱ����Ҫ���������
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
	//end ��ͼ��
	//////////////////////////////////////////////////////////////////////////

	//begin ��ʾ��ͼ
	//////////////////////////////////////////////////////////////////////////
	AcGePoint3d pt_3d_mid(0,0,0) ;
	pt_3d_mid.x = pt_3d_1.x + (pt_3d_2.x-pt_3d_1.x)/2 ;
	pt_3d_mid.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
	strBlockName.Format(_T("%s\\syt"), this->cstrDwgSaveDir) ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_mid))
	{
		pRef->close() ;
	}
	//end ��ʾ��ͼ
	//////////////////////////////////////////////////////////////////////////

	//������ǰ��Y����
	this->dTableCur_Y = this->dTableCur_Y - dVerLen - this->dBetweenTable_Y ;
	
}


void CMaXuBiaoForJH::JinLuBianMa_S_C_S()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;	
	BOOL bBianMaChange = 0 ;				

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;		
	
	BOOL bPassSI = FALSE ; //�Ƿ�Խ����վ�źŻ�������right��ΪԽ����
	BOOL bPassS = FALSE ; //�Ƿ�Խ����վ�źŻ�������right��ΪԽ����
	int iCodeTypeTmp = 0 ; //��������� 
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;

		//�ҳ��Ͻ�վ,�ϳ�վ
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"), this->m_cstrJinZhanXHJ_F, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ) ;
		this->m_cstrFileName.Format(_T("%s_%s_%s"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F) ;
		//end �ҳ��Ͻ�վ,�ϳ�վ
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%sͨ��)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}	
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassSI = FALSE ;
			bPassS = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				if (!bPassSI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassS = TRUE ;	
						}						
					}
					
				}

				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 2) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange,iCodeTypeTmp, 2) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id<%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%sͨ��)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���

			cstrRetBianMa = _T("") ;
			bPassSI = FALSE ;
			bPassS = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				if (!bPassSI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassS = TRUE ;	
						}							
					}
					
				}	

				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		//S����c��SII�ر�,��������
		//////////////////////////////////////////////////////////////////////////		

		//�������:S����c��SII�ر�
		cstrDescribeTmp.Format(_T("%s����%s,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao,this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		bPassS = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassS)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					cstrRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassS = TRUE ;
				}
			}

			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpSII = new CStringArray() ;
		pcstrArrayTmpSII->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpSII) ;	
		
		if (m_iVersionFor == 1)
		{			
			//�������:S������SII��SII��������		
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;
			bPassS = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassS)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{	
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassS = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
				cSet.MoveNext() ;			
			}
			CStringArray * pcstrArrayTmpSIIYinDao = new CStringArray() ;
			pcstrArrayTmpSIIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end S����c��SII�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-in s-c-s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in s-c-s")) ;
	} 

    //���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::GetJinLuGuiDao(CStringArray &cstrArrayGuiDao)
{
	CString cstrGuiDao ;
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen()) 
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		//����·�����й������cstrArrayGuiDao		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			cstrArrayGuiDao.Add(cstrGuiDao) ;
			cSet.MoveNext() ;
		}
		cSet.Close() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetJinLuGuiDao")) ;
	} 
	
}

void CMaXuBiaoForJH::JinLuBianMa_X_ZC_S()
{
	this->m_iCurJinLuDirect = 0 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	CStringArray cstrArrayGuiDao ;	//���������
	CString cstrRetBianMa = _T("") ; // GetGuiDaoBianMa(...)�Ĳ������������صõ��ı���
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;	
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ����������ΪԽ����
	BOOL bPassX = FALSE ; //�Ƿ�Խ���½�վ�źŻ����������ΪԽ����
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //��������� 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum,1) ;

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ���վ,��վ
		//////////////////////////////////////////////////////////////////////////
		/*
		CString this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F ;
		int this->m_iJinZhanXHJ_id = 0 ;
		int this->m_iChuZhanXHJ_id = 0 ;
		int this->m_iGuDao_Type = 0 ;		
		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ, this->m_iJinZhanXHJ_id, this->m_cstrChuZhanXHJ, this->m_iChuZhanXHJ_id, this->m_cstrGuDao, this->m_iGuDao_Type, this->m_cstrJinZhanXHJ_F) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;
		*/
		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;
		this->m_cstrFileName.Format(_T("%s_%s_%s"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ���վ,��վ		
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		
		//��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		cstrDescribeTmp.Format(_T("%s����%sͨ��,S���������"), this->m_cstrChuZhanXHJ, this->m_cstrGuDao) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}			

		bPassXI = FALSE ;
		bPassX = FALSE ;
		
		iCodeTypeTmp = 0 ;		
		bBianMaChange = 0 ;
		cstrRetBianMa = _T("L5") ;		
		
		while (!cSet.IsBOF())
		{
			CString cstrGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
			
			if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					
					bPassXI = TRUE ;						
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
				
			}
			else
			{
				if (!bPassX)
				{
					if (iGuiDaoId<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;	
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 1) ;
				
			}			
			
			cSet.MovePrevious() ;
		}
		
		cSet.Close() ;
		
		CStringArray * pcstrArrayTmp_L5 = new CStringArray() ;
		pcstrArrayTmp_L5->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmp_L5) ;
		
		//end ��վ���ţ�ȫL5

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%s,%s�ر�)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao,this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
	
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;

				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
// 						{
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						cstrRetBianMa = _T("ռ��") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 2, 1) ;
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 1) ;
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 1) ;
					
				}
				
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,iCodeTypeTmp) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��_T("L5,L4,...HU,ռ��")��9�����
		//////////////////////////////////////////////////////////////////////////
		
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////	
		int iLQ_ID = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		if (cSet.GetRecordCount()>0)
		{
			cSet.GetFieldValue(_T("id"), iLQ_ID) ;
		}
		cSet.Close() ;
		
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d"), iLQ_ID) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;
			
			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%s,%s�ر�)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao,this->m_cstrChuZhanXHJ) ;			
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2,1) ;

				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0 ) 
// 						{
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;							
// 						}
						cstrRetBianMa = _T("ռ��") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2,1) ;
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
							{
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp,1) ;
					
				}			

				//this->GetGuiDaoBianMa(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp) ;
				
				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						
			
			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;
		
		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		// ��վ���ţ���վ�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
		//�������:��վ���ţ���վ�ر�
		cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassX)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassX = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	

		if (m_iVersionFor == 1)
		{
			//begin yd
			
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassX)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
				cSet.MovePrevious() ;			
			}
			//cSet.Close() ;	
			CStringArray * pcstrArrayTmpyd = new CStringArray() ;
			pcstrArrayTmpyd->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpyd) ;	
			
			//end yd
		}
		
		cSet.Close() ;	
		
		//end ��վ���ţ���վ�ر�,��������
		//////////////////////////////////////////////////////////////////////////
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_zc_s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_zC_s")) ;			
	} 
	
	//���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2, 0, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_S_ZC_X()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	CStringArray cstrArrayGuiDao ;	//���������
	CString cstrRetBianMa = _T("") ; // GetGuiDaoBianMa(...)�Ĳ������������صõ��ı���
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;	
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ�
	BOOL bPassXN = FALSE ; //�Ƿ�Խ����վ�źŻ�
	int iCodeTypeTmp = 0 ; //��������� 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;	

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ���վ,��վ
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;
		this->m_cstrFileName.Format(_T("%s_%s_%s"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ���վ,��վ		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%sͨ��)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				
				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp) ;
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp) ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 2, 1) ;					
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}
					
				}
				
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,iCodeTypeTmp) ;
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
				
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////		
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id>%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;
			
			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%sͨ��)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;			
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���

			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						if (this->m_iGuDao_Type !=1 || this->m_iZpGuiDaoSum_Rigth>0) 
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;							
						}
						bPassXI = TRUE ;						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1) ;
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id )
						{
							if (this->m_iGuDao_Type !=2 || this->m_iZpGuiDaoSum_Left>0)
							{
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
					
				}
				//this->GetGuiDaoBianMa(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp) ;				
				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						
			
			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;
		
		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////

		// ��վ���ţ���վ�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
		//�������:��վ���ţ���վ�ر�
		cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;

		cstrRetBianMa = _T("ռ��") ;		
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;						
					}
					bPassXN = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	

		if (m_iVersionFor == 1)
		{
			//begin ����
			//////////////////////////////////////////////////////////////////////////
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			
			cstrRetBianMa = _T("") ;		
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;						
						}
						bPassXN = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSet.MovePrevious() ;			
			}
			//cSet.Close() ;	
			CStringArray * pcstrArrayTmpyd = new CStringArray() ;
			pcstrArrayTmpyd->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpyd) ;
			
			//end ����
		}
		
		cSet.Close() ;	
		
		//end ��վ���ţ���վ�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_zc_x"), sBuff) ;		
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_zC_X")) ;			
	} 
	
	//���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_X_Z_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ�
	BOOL bPassXN = FALSE ; //�Ƿ�Խ����վ�źŻ�
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //��������� 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;		

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;
		
		//�ҳ��½�վ,�³�վ
		//////////////////////////////////////////////////////////////////////////		

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"), this->m_cstrJinZhanXHJ_F, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ) ;
	
		//end �ҳ��½�վ,�³�վ		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		cstrDescribeTmp.Format(_T("%s����%sͨ��,X���������"), this->m_cstrChuZhanXHJ, this->m_cstrGuDao) ;
		//cstrDescribeTmp.Format(_T("%s����ֱ��ͨ��,X���������"), this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}	
		
		bPassXI = FALSE ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;
		cstrRetBianMa = _T("L5") ;
		bBianMaChange = 0 ;

		while (!cSet.IsEOF())
		{
			CString cstrGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
			
			if (!bPassJinZhan_F)
			{
				if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						cstrRetBianMa = _T("JC") ;
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
						cstrRetBianMa = _T("") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				
			}			
			this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange,iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;
		}			
		cSet.Close() ;
		
		CStringArray * pcstrArrayTmp_L5 = new CStringArray() ;
		pcstrArrayTmp_L5->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmp_L5) ;
		
		//end ��վ���ţ�ȫL5

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		//CString cstrStartCodeTmp[9] = {"L5","L4","L3","L2","L","LU","U","HU","ռ��"} ;
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%s,%s�ر�)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{

			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
	
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;

			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			
			bBianMaChange = 0 ;			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
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
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						cstrRetBianMa = _T("ռ��") ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN	= TRUE ;
						}
					}
					
				}

				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, 0, 1, 1) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange,iCodeTypeTmp, 1, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
		
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		int iSN1LQ_id = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d and signal<>''"),this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("id"), iSN1LQ_id) ;
		}
		cSet.Close() ;

		//������ڽ�վ�ֽ磬����û�з�����ȥ����
		if (iSN1LQ_id==0)
		{
			cstrSql.Format(_T("select * from jinlu where id>%d"), this->m_iJinZhanXHJ_id) ;			
		}
		else
		{
			cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d or id>%d"), iSN1LQ_id, this->m_iJinZhanXHJ_id) ;
		}
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������		
			if (iGuiDaoId<this->m_iChuZhanXHJ_id)
			{
				cstrDescribeTmp.Format(_T("%sռ��(%s����%s,%s�ر�)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			}
			else
			{
				cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
				
			}
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
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
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						cstrRetBianMa = _T("ռ��") ;
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN	= TRUE ;
						}
					}
					
				}				

				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////	

		//SN����ֱ��SI�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		//�������:SN����ֱ��SI�ر�
		cstrDescribeTmp.Format(_T("%s����%s,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;

		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;

			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Rigth>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN	= TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	
		
		if (m_iVersionFor == 1)
		{
			//begin yd
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;		
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
				cSet.MoveNext() ;			
			}
			//cSet.Close() ;	
			CStringArray * pcstrArrayTmpyd = new CStringArray() ;
			pcstrArrayTmpyd->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpyd) ;
			//end yd
		}
		cSet.Close() ;	

		//end SN����ֱ��SI�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
		//SN���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		//����		
		cstrDescribeTmp.Format(_T("%s���Ų���,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 1, 1, 1) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pcstrArrayTmpX_C = new CStringArray() ;
		pcstrArrayTmpX_C->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpX_C) ;	

		//�ر�
		cstrDescribeTmp.Format(_T("%s��վ�ر�"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 1, 1) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pcstrArrayTmpX_CLOSE = new CStringArray() ;
		pcstrArrayTmpX_CLOSE->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpX_CLOSE) ;	

		//X��������
		cstrDescribeTmp.Format(_T("%s��������"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsEOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 2, 1, 1) ;
			cSet.MoveNext() ;			
		}
		CStringArray * pcstrArrayTmpX_Y = new CStringArray() ;
		pcstrArrayTmpX_Y->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpX_Y) ;	
		
		
		cSet.Close() ;	
		
		//end X���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_z_x_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error x_z_x_f")) ;
	} 

	//���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe,arrayBianMaChange, 1, 1) ;
		
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_X_C_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	CStringArray cstrArrayGuiDao ;	//���������
	CString cstrRetBianMa = _T("") ; // GetGuiDaoBianMa(...)�Ĳ������������صõ��ı���
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;	
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ����������ΪԽ����
	BOOL bPassX = FALSE ; //�Ƿ�Խ���½�վ�źŻ����������ΪԽ����
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //��������� 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ��½�վ,�³�վ
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ_F, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ ) ;

		//end �ҳ��½�վ,�³�վ		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		//��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		cstrDescribeTmp.Format(_T("%s����%sͨ��,X���������"), this->m_cstrChuZhanXHJ, this->m_cstrGuDao) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		
		bPassXI = FALSE ;
		bPassX = FALSE ;

		iCodeTypeTmp = 0 ;
		cstrRetBianMa = _T("L5") ;
		bBianMaChange = 0 ;
		
		while (!cSet.IsEOF())
		{
			CString cstrGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
			
			if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					cstrRetBianMa = _T("") ;
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
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassX = TRUE ;	
					}						
				}
				
			}
			
			this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange,iCodeTypeTmp, 1, 1) ;
			
			cSet.MoveNext() ;
		}			
		cSet.Close() ;
		
		CStringArray * pcstrArrayTmp_L5 = new CStringArray() ;
		pcstrArrayTmp_L5->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmp_L5) ;
		
		//end ��վ���ţ�ȫL5

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%s,%s�ر�)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			

			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("ռ��") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}

				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,iCodeTypeTmp, 1, 1) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange,iCodeTypeTmp, 1, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
		
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		int iLq_id = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsEOF()) 
			{
				cSet.MoveLast() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;
		
		//����Խ�վΪ�ֽ磬����û�з�����ȥ����
		if (iLq_id==0)
		{
			cstrSql.Format(_T("select * from jinlu where id<%d"), iLq_id) ;			
		}
		else
		{
			cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d"), iLq_id) ;
		}
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;
			
			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%s,%s�ر�)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;			
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("ռ��") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassX = TRUE ;	
						}						
					}
					
				}			

				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
				
				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						
			
			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;		
		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////	
		
		//sn����ֱ��sI�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
		//�������:sn����ֱ��sI�ر�
		cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassX)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					cstrRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassX = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	
			
		if(m_iVersionFor == 1)
		{
			//�������:SN����ֱ����SI��SI��������
			
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassX)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassX = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;
				cSet.MoveNext() ;			
			}
			
			CStringArray * pcstrArrayTmpXIYinDao = new CStringArray() ;
			pcstrArrayTmpXIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpXIYinDao) ;
		}
		
		cSet.Close() ;	
		
		//end SN����ֱ��SI�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_c_x_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_C_X_F")) ;			
	} 

	//���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange, 1, 1) ;	
	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_X_ZC_S_F()
{
	this->m_iCurJinLuDirect = 1 ;
	
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	CStringArray cstrArrayGuiDao ;	//���������
	CString cstrRetBianMa = _T("") ; // GetGuiDaoBianMa(...)�Ĳ������������صõ��ı���
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;	
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ����������ΪԽ����
	BOOL bPassX = FALSE ; //�Ƿ�Խ���½�վ�źŻ����������ΪԽ����
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //��������� 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ���վ,��վ
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"), this->m_cstrJinZhanXHJ_F, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ) ;
		
		//end �ҳ���վ,��վ		
		//////////////////////////////////////////////////////////////////////////

		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		cstrDescribeTmp.Format(_T("%s����%sͨ��,X���������"), this->m_cstrChuZhanXHJ, this->m_cstrGuDao) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}			

		bPassXI = FALSE ;
		bPassX = FALSE ;

		iCodeTypeTmp = 0 ;		
		bBianMaChange = 0 ;
		cstrRetBianMa = _T("L5") ;
		
		while (!cSet.IsEOF())
		{
			CString cstrGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
			
			if (!bPassXI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					
					bPassXI = TRUE ;						
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;					

			}
			else
			{
				if (!bPassX)
				{
					if (iGuiDaoId>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;	
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
			}
			
			cSet.MoveNext() ;
		}

		cSet.Close() ;
		
		CStringArray * pcstrArrayTmp_L5 = new CStringArray() ;
		pcstrArrayTmp_L5->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmp_L5) ;
		
		//end ��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		
		//��վ��_T("L5,L4,...HU,ռ��")��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%s,%s�ر�)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;					

				}
				else if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0)
// 						{
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						cstrRetBianMa = _T("ռ��") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 1, 1) ;
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 1, 1) ;					
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 2) ;
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2) ;
// 					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
// 					{
// 						arrayBianMaChange.Add(bBianMaChange) ;
// 					}				
					
				}
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,iCodeTypeTmp) ;
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��_T("L5,L4,...HU,ռ��")��9�����
		//////////////////////////////////////////////////////////////////////////
		
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////	
		int iLQ_ID = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		if (cSet.GetRecordCount()>0)
		{
			cSet.GetFieldValue(_T("id"), iLQ_ID) ;
		}
		cSet.Close() ;

		cstrSql.Format(_T("select * from jinlu where guidao_updown=1 and id<%d"), iLQ_ID) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;
			
			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%s,%s�ر�)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;			
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassX = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1,1) ;
				}
				else if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
// 						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Left>0) 
// 						{
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;							
// 						}
						cstrRetBianMa = _T("ռ��") ;
 						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1,1) ;
				}
				else
				{
					if (!bPassX)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
							{
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassX = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp,2) ;
					
				}			

				//this->GetGuiDaoBianMa(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp) ;
				
				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						
			
			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;
		
		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		//s���ţ�sI�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		//�������:s���ţ�sI�ر�
		cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		bPassX = FALSE ;
		iCodeTypeTmp = 0 ;
		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;

			if (!bPassX)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassX = TRUE ;	
				}						
			}
			
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;
		
		if(m_iVersionFor == 1)
		{
			//begin yd
			cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			cSet.Open(g_PtrCon, cstrSql) ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;		
			bPassX = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				
				if (!bPassX)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Rigth>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassX = TRUE ;	
					}						
				}
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
				cSet.MoveNext() ;			
			}
			//cSet.Close() ;	
			CStringArray * pcstrArrayTmpyd = new CStringArray() ;
			pcstrArrayTmpyd->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpyd) ;
			
			//end yd
		}


		cSet.Close() ;	
		
		//end S���ţ�SI�ر�,��������
		//////////////////////////////////////////////////////////////////////////

	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-x_zc_s_F"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_X_zC_s_F")) ;			
	} 
	
	//���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange, 1, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_S_Z_S_F()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;	

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;

	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ�
	BOOL bPassXN = FALSE ; //�Ƿ�Խ����վ�źŻ�
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //��������� 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		

		//�ҳ���վ,��վ
		//////////////////////////////////////////////////////////////////////////	

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ���վ,��վ		
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		
		
		
		//��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		cstrDescribeTmp.Format(_T("%s����%sͨ��,S���������"), this->m_cstrChuZhanXHJ, this->m_cstrGuDao) ;
		//cstrDescribeTmp.Format(_T("%s����ֱ��ͨ��,S���������"), this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		bPassXI = FALSE ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;		
		cstrRetBianMa = _T("L5") ;		

		while (!cSet.IsBOF())
		{
			CString cstrGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
			
			if (!bPassJinZhan_F)
			{
				if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
				{
					if(this->m_iZpGuiDaoSum_Rigth>0)
					{
						cstrRetBianMa = _T("JC") ;
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
						cstrRetBianMa = _T("") ;
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
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN	= TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			
			cSet.MovePrevious() ;
		}			
		cSet.Close() ;
		
		CStringArray * pcstrArrayTmp_L5 = new CStringArray() ;
		pcstrArrayTmp_L5->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmp_L5) ;
		
		//end ��վ���ţ�ȫL5

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%s,%s�ر�)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}	
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			bBianMaChange = 0 ;			
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
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
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						cstrRetBianMa = _T("ռ��") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					//cstraaa.Format(_T("%s\n%s-%d"), cstraaa, cstrRetBianMa, bBianMaChange) ;
					arrayBianMaChange.Add(bBianMaChange) ;
				}			
				
				cSet.MovePrevious() ;
			}

			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		int iLq_id = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d and signal<>'' "), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;

		//�Ҳ����ڽ�վ�ֽ磬����û�з�����ȥ����
		if (iLq_id==0)
		{
			cstrSql.Format(_T("select * from jinlu where id<%d"), this->m_iJinZhanXHJ_id) ;			
		}
		else
		{
			cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d or id<%d"), iLq_id, this->m_iJinZhanXHJ_id) ;
		}
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������	
			if (iGuiDaoId>this->m_iChuZhanXHJ_id)
			{
				cstrDescribeTmp.Format(_T("%sռ��(%s����%s,%s�ر�)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			}
			else
			{
				cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
			}
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���

			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
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
// 							cstrRetBianMa = _T("") ;
// 							iCodeTypeTmp = 1 ;
// 						}
						cstrRetBianMa = _T("ռ��") ;
						iCodeTypeTmp = 0 ;
						bPassXI = TRUE ;
					}
					//this->GetGuiDaoBianMa2(cstrGuiDaoName,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
						{
							if (this->m_iZpGuiDaoSum_Left>0)
							{
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN	= TRUE ;
						}
					}
					//this->GetGuiDaoBianMa2(cstrGuiDaoName,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
					
				}
				
				this->GetGuiDaoBianMa2(cstrGuiDaoName,cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		//XN����ֱ��XII�ر�,��������
		//////////////////////////////////////////////////////////////////////////		

		//�������:XN����ֱ��XII�ر�
		cstrDescribeTmp.Format(_T("%s����%s,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}

		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp2 = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;			
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
				{
					if (this->m_iZpGuiDaoSum_Left>0)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN	= TRUE ;
				}
			}
			
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpSII = new CStringArray() ;
		pcstrArrayTmpSII->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpSII) ;	
		
		if (m_iVersionFor == 1)
		{
			//�������:XN����ֱ����XII��XII��������	
			cstrDescribeTmp.Format(_T("%s����ֱ����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp2 = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp2) ;			
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp2<this->m_iJinZhanXHJ_id)
					{
						if (this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN	= TRUE ;
					}
				}
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSet.MovePrevious() ;
				
				// 			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				// 			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, 2, 2, 1) ;
				// 			cSet.MovePrevious() ;			
			}
			CStringArray * pcstrArrayTmpSIIYinDao = new CStringArray() ;
			pcstrArrayTmpSIIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end XN����ֱ��XII�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
		//XN���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		//����		
		cstrDescribeTmp.Format(_T("%s���Ų���,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<%d"), this->m_iJinZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 1, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pcstrArrayTmpS_C = new CStringArray() ;
		pcstrArrayTmpS_C->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpS_C) ;	

		//�ر�
		cstrDescribeTmp.Format(_T("%s��վ�ر�"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pcstrArrayTmpS_CLOSE = new CStringArray() ;
		pcstrArrayTmpS_CLOSE->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpS_CLOSE) ;	

		//XN��������
		cstrDescribeTmp.Format(_T("%s��������"), this->m_cstrJinZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("") ;
		
		while (!cSet.IsBOF())
		{
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 2, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		CStringArray * pcstrArrayTmpS_Y = new CStringArray() ;
		pcstrArrayTmpS_Y->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpS_Y) ;	
		
		cSet.Close() ;	
		
		//end XN���Ų��򡢹رա�����
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s_f")) ;
	} 

    //���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2, 0, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_S_C_S_F()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;		
	
	BOOL bPassSI = FALSE ; //�Ƿ�Խ����վ�źŻ�������right��ΪԽ����
	BOOL bPassS = FALSE ; //�Ƿ�Խ����վ�źŻ�������right��ΪԽ����
	BOOL bPassJinZhan_F = FALSE ;
	int iCodeTypeTmp = 0 ; //��������� 
	
	this->GetLinZhanStateSum(this->m_iLinZhanStateSum, 1) ;
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;

		//�ҳ��Ͻ�վ,�ϳ�վ
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ_F) ;

		//end �ҳ��Ͻ�վ,�ϳ�վ
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_id, this->m_iChuZhanXHJ_F_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_id+1, this->m_iJinZhanXHJ_F_id, this->m_iCurJinLuDirect) ;		

		//end find zp
		//////////////////////////////////////////////////////////////////////////		


		//��վ���ţ�ȫL5
		//////////////////////////////////////////////////////////////////////////
		cstrDescribeTmp.Format(_T("%s����%sͨ��,S���������"), this->m_cstrChuZhanXHJ, this->m_cstrGuDao) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}
		
		bPassSI = FALSE ;
		bPassS = FALSE ;
		iCodeTypeTmp = 0 ;
		cstrRetBianMa = _T("L5") ;		

		while (!cSet.IsBOF())
		{
			CString cstrGuiDaoTmp ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;

			if (!bPassSI)
			{
				if (iGuiDaoId==this->m_iChuZhanXHJ_id )
				{
					cstrRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassSI = TRUE ;						
				}					
			}
			else if (!bPassS)
			{
				if (iGuiDaoId<this->m_iJinZhanXHJ_id)
				{
					cstrRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassS = TRUE ;	
				}				
				
			}
			
			this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;

			cSet.MovePrevious() ;
		}			
		cSet.Close() ;
		
		CStringArray * pcstrArrayTmp_L5 = new CStringArray() ;
		pcstrArrayTmp_L5->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmp_L5) ;

		//end ��վ���ţ�ȫL5


		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%s,%s�ر�)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}	
						
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassSI = FALSE ;
			bPassS = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;
			bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;

				if (!bPassJinZhan_F)
				{
					if (iGuiDaoId==this->m_iJinZhanXHJ_F_id)
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;
					}
				}
				else if (!bPassSI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("ռ��") ;
						iCodeTypeTmp = 0 ;
						bPassSI = TRUE ;						
					}					
				}
				else if (!bPassS)
				{
					if (iGuiDaoId<this->m_iJinZhanXHJ_id)
					{
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassS = TRUE ;	
					}				
					
				}

				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 2, 1) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
				if (iStartCodeNum==this->m_iLinZhanStateSum-1)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		int iLq_id = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id>%d and signal<>''"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>0)
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			cSet.GetFieldValue(_T("id"), iLq_id) ;
		}
		cSet.Close() ;

		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id<%d or id>%d"), this->m_iJinZhanXHJ_id, iLq_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s�ر�)"), cstrGuiDaoName, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassSI = FALSE ;
			bPassS = FALSE ;
			bPassJinZhan_F = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				if (!bPassJinZhan_F)
				{
					if (iGuiDaoIdTmp==this->m_iJinZhanXHJ_F_id )
					{
						cstrRetBianMa = _T("JC") ;
						iCodeTypeTmp = 0 ;
						bPassJinZhan_F = TRUE ;						
					}		
				}
				else if (!bPassSI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id )
					{
						cstrRetBianMa = _T("ռ��") ;
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
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
							bPassS = TRUE ;	
						}							
					}
					
				}	

				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange,iCodeTypeTmp, 2,1) ;

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
		//XN����c��XII�ر�,��������
		//////////////////////////////////////////////////////////////////////////		

		//�������:XN����c��XII�ر�
		cstrDescribeTmp.Format(_T("%s����%s,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao,this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id<=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF())
		{
			cSet.MoveLast() ;
		}

		cstrArrayBianMaTmp.RemoveAll() ;

		cstrRetBianMa = _T("ռ��") ;
		bPassS = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsBOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassS)
			{
				if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
				{
					cstrRetBianMa = _T("") ;
					iCodeTypeTmp = 1 ;
					bPassS = TRUE ;
				}
			}

			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2, 1) ;
			cSet.MovePrevious() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpSII = new CStringArray() ;
		pcstrArrayTmpSII->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpSII) ;	
		
		if(m_iVersionFor == 1)
		{		
			//�������:XN������XII��XII��������
			
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			
			cstrRetBianMa = _T("") ;
			bPassS = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsBOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassS)
				{
					if (iGuiDaoIdTmp<this->m_iJinZhanXHJ_id)
					{	
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
						bPassS = TRUE ;
					}
				}			
				
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, iCodeTypeTmp, 2, 1) ;
				cSet.MovePrevious() ;			
			}
			CStringArray * pcstrArrayTmpSIIYinDao = new CStringArray() ;
			pcstrArrayTmpSIIYinDao->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpSIIYinDao) ;
		}
		
		cSet.Close() ;	

		//end XN����c��XII�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-in s-c-s_F"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in s-c-s_F")) ;
	} 

    //���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2, 0, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_S_ZC_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	
	CStringArray cstrArrayGuiDao ;	//���������
	CString cstrRetBianMa = _T("") ; // GetGuiDaoBianMa(...)�Ĳ������������صõ��ı���
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;	
	
	BOOL bPassXI = FALSE ; //�Ƿ�Խ���³�վ�źŻ�
	BOOL bPassXN = FALSE ; //�Ƿ�Խ����վ�źŻ�
	int iCodeTypeTmp = 0 ; //��������� 

	this->GetLinZhanStateSum(this->m_iLinZhanStateSum) ;

	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		
	
		//�ҳ���վ,��վ
		//////////////////////////////////////////////////////////////////////////

		this->GetJinZhanOrChuZhan(this->m_cstrJinZhanXHJ,this->m_iJinZhanXHJ_id,this->m_cstrChuZhanXHJ,this->m_iChuZhanXHJ_id,this->m_cstrJinZhanXHJ_F,this->m_iJinZhanXHJ_F_id,this->m_cstrChuZhanXHJ_F,this->m_iChuZhanXHJ_F_id,this->m_cstrGuDao,this->m_iGuDao_Type, this->m_iCurJinLuDirect) ;
		this->m_cstrCaseType.Format(_T("%s��%s��%s"),this->m_cstrJinZhanXHJ_F, this->m_cstrChuZhanXHJ, this->m_cstrJinZhanXHJ) ;

		//end �ҳ���վ,��վ		
		//////////////////////////////////////////////////////////////////////////
		
		//find zp
		//////////////////////////////////////////////////////////////////////////
		this->m_iZpGuiDaoSum_Left = 0 ;		
		this->m_iZpGuiDaoSum_Rigth = 0 ;	
		this->m_cstrArrayGuiDao_ZP_Left.RemoveAll() ;
		this->m_cstrArrayGuiDao_ZP_Right.RemoveAll() ;
		
		this->m_iZpGuiDaoSum_Left = this->FindZP(this->m_cstrArrayGuiDao_ZP_Left, this->m_iJinZhanXHJ_F_id, this->m_iChuZhanXHJ_id-1, this->m_iCurJinLuDirect) ;
		this->m_iZpGuiDaoSum_Rigth = this->FindZP(this->m_cstrArrayGuiDao_ZP_Right,this->m_iChuZhanXHJ_F_id+1, this->m_iJinZhanXHJ_id, this->m_iCurJinLuDirect) ;		
		
		//end find zp
		//////////////////////////////////////////////////////////////////////////		

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		for(int iDesIndex=0; iDesIndex<this->m_iLinZhanStateSum; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s (%s����%sͨ��)"), g_cstrLinZhanState_1[iDesIndex], this->m_cstrJinZhanXHJ,this->m_cstrGuDao) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<this->m_iLinZhanStateSum; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrRetBianMa = g_cstrLinZhanState_1[iStartCodeNum] ;
			
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;			
			bBianMaChange = 0 ;
			
			//CString cstraaa ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				int iGuiDaoId = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;

				if (!bPassXI)
				{
					if (iGuiDaoId==this->m_iChuZhanXHJ_id )
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0)
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXI = TRUE ;						
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 2) ;
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;					
					
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
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}
					}
					//this->GetGuiDaoBianMa(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp, 1, 1) ;
					
					this->GetGuiDaoBianMa2(cstrGuiDaoTmp, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;					
					
					if (iStartCodeNum==this->m_iLinZhanStateSum-1)
					{
						arrayBianMaChange.Add(bBianMaChange) ;
					}		
					
				}
				
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa,iCodeTypeTmp) ;
					
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
	
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;
			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
			
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////	
		int iLq_id = 0 ;
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id<%d and signal<>''"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
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
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2 and id<=%d"), iLq_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}		
		
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;
			
			CString cstrGuiDaoName = _T("") ;
			int iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
			
			//�������			
			cstrDescribeTmp.Format(_T("%sռ��(%s����%sͨ��)"), cstrGuiDaoName, this->m_cstrJinZhanXHJ, this->m_cstrGuDao) ;			
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			bPassXI = FALSE ;
			bPassXN = FALSE ;
			iCodeTypeTmp = 0 ;

			while (!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;

				if (!bPassXI)
				{
					if (iGuiDaoIdTmp==this->m_iChuZhanXHJ_id  )
					{
						if (this->m_iGuDao_Type!=2 || this->m_iZpGuiDaoSum_Left>0) 
						{
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;							
						}
						bPassXI = TRUE ;						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 2) ;
				}
				else
				{
					if (!bPassXN)
					{
						if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
						{
							if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
							{
								cstrRetBianMa = _T("") ;
								iCodeTypeTmp = 1 ;
							}
							bPassXN = TRUE ;	
						}						
					}
					this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
					
				}
				//this->GetGuiDaoBianMa(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa, iCodeTypeTmp) ;				
				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						
			
			cSet.MoveNext() ;
		}
		//end while
	
		cSet.Close() ;
		
		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////

		//sn���ţ�sI�ر�,��������
		//////////////////////////////////////////////////////////////////////////		
		
		//�������:sn���ţ�sI�ر�
		cstrDescribeTmp.Format(_T("%s����%s�ӳ�,%s�ر�"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
		cstrArrayDescribe.Add(cstrDescribeTmp) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d"), this->m_iChuZhanXHJ_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		
		cstrArrayBianMaTmp.RemoveAll() ;
		cstrRetBianMa = _T("ռ��") ;		
		bPassXN = FALSE ;
		iCodeTypeTmp = 0 ;

		while (!cSet.IsEOF())
		{
			int iGuiDaoIdTmp = 0 ;
			cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
			if (!bPassXN)
			{
				if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
				{
					if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
					{						
						cstrRetBianMa = _T("") ;
						iCodeTypeTmp = 1 ;
					}
					bPassXN = TRUE ;
				}
			}
			this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
			cSet.MoveNext() ;			
		}
		//cSet.Close() ;	
		CStringArray * pcstrArrayTmpXI = new CStringArray() ;
		pcstrArrayTmpXI->Copy(cstrArrayBianMaTmp) ;
		arrayBianMa.Add(pcstrArrayTmpXI) ;	

		if (m_iVersionFor == 1)
		{		
			//begin yd
			cstrDescribeTmp.Format(_T("%s����%s,%s��������"), this->m_cstrJinZhanXHJ, this->m_cstrGuDao, this->m_cstrChuZhanXHJ) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrRetBianMa = _T("") ;		
			bPassXN = FALSE ;
			iCodeTypeTmp = 2 ;
			
			while (!cSet.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
				if (!bPassXN)
				{
					if (iGuiDaoIdTmp>this->m_iJinZhanXHJ_id)
					{
						if (this->m_iGuDao_Type!=1 || this->m_iZpGuiDaoSum_Rigth>0)
						{						
							cstrRetBianMa = _T("") ;
							iCodeTypeTmp = 1 ;
						}
						bPassXN = TRUE ;
					}
				}
				this->GetGuiDaoBianMa2(cstrGuiDao, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, iCodeTypeTmp, 1, 1) ;
				cSet.MoveNext() ;			
			}
			//cSet.Close() ;	
			CStringArray * pcstrArrayTmpyd = new CStringArray() ;
			pcstrArrayTmpyd->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmpyd) ;	
			
			//end yd
		}

		cSet.Close() ;	
		
		//end SN���ţ�SI�ر�,��������
		//////////////////////////////////////////////////////////////////////////
		
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_zc_x_F"), sBuff) ;		
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_zC_X_F")) ;			
	} 
	
	//���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_ZJZ_X()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	this->m_iLinZhanStateSum = 9 ;	

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;
		this->m_cstrCaseType.Format(_T("1.��������")) ;
		
		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		CString cstrStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("ռ��")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s"), cstrStartCodeTmp[iDesIndex]) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}			
			
			cstrRetBianMa = cstrStartCodeTmp[iStartCodeNum] ;
			
			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange) ;
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}			
				
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������		
			cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
				
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			while (!cSetTmp.IsBOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange) ;

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-zjz_x"),sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error zjz_x")) ;
	} 

	//���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}
	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2) ;
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_ZJZ_X_F()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	this->m_iLinZhanStateSum = 9 ;	

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;
		this->m_cstrCaseType.Format(_T("2.���з���")) ;

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		CString cstrStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("ռ��")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s"), cstrStartCodeTmp[iDesIndex]) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			
			cstrRetBianMa = cstrStartCodeTmp[iStartCodeNum] ;
			
			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, 0, 1, 1) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, 0, 1, 1) ;				
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}				
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////
		
		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////

		//������ڽ�վ�ֽ磬����û�з�����ȥ����
		cstrSql.Format(_T("select * from jinlu where guidao_updown=1")) ;
		
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������		
			cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			while (!cSetTmp.IsEOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 1, 1) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-zjz_x_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error zjz_x_f")) ;
	} 

	//���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe,arrayBianMaChange, 1) ;
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_ZJZ_S()
{
	this->m_iCurJinLuDirect = 1 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;

	this->m_iLinZhanStateSum = 9 ;
	

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;	
		this->m_cstrCaseType.Format(_T("1.��������")) ;

		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		CString cstrStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("ռ��")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s"), cstrStartCodeTmp[iDesIndex]) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}			
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			
			cstrRetBianMa = cstrStartCodeTmp[iStartCodeNum] ;
			
			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsEOF())
			{
				CString cstrGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, 0, 2) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, 0, 2) ;
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}
				cSet.MoveNext() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////		
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsBOF()) 
		{
			cSet.MoveFirst() ;
		}
		//����ÿ��������
		while (!cSet.IsEOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			//�������	
			cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id>=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			while (!cSetTmp.IsEOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 2) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MoveNext() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s")) ;
	} 

    //���������
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange, 1) ;	
	//end ���������
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

void CMaXuBiaoForJH::JinLuBianMa_ZJZ_S_F()
{
	this->m_iCurJinLuDirect = 0 ;
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CArray<BOOL, BOOL> arrayBianMaChange ;
	BOOL bBianMaChange = 0 ;
	this->m_iLinZhanStateSum = 9 ;
	

	//���������
	CStringArray cstrArrayGuiDao ;
	CString cstrRetBianMa = _T("") ;
	CString cstrDescribeTmp = _T("") ;
	CStringArray cstrArrayDescribe ; //�������array	
	CStringArray cstrArrayBianMaTmp ;//����һ������µ�һ�б���
	CString cstrGuiDao ;				
				
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		//����·�����й������cstrArrayGuiDao	
		this->GetJinLuGuiDao(cstrArrayGuiDao) ;		
		this->m_cstrCaseType.Format(_T("2.���з���")) ;
		
		//��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		
		CString cstrStartCodeTmp[9] = {_T("L5"),_T("L4"),_T("L3"),_T("L2"),_T("L"),_T("LU"),_T("U"),_T("HU"),_T("ռ��")} ;
		for(int iDesIndex=0; iDesIndex<9; iDesIndex++)
		{
			cstrDescribeTmp.Format(_T("(��վ) %s"), cstrStartCodeTmp[iDesIndex]) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;
		}			
		
		for(int iStartCodeNum=0; iStartCodeNum<9; iStartCodeNum++)
		{
			cstrArrayBianMaTmp.RemoveAll() ;
			cstrSql.Format(_T("select * from jinlu")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}	
			cSet.Open(g_PtrCon, cstrSql) ;
			
			if (!cSet.IsEOF())
			{
				cSet.MoveLast() ;
			}			
			
			cstrRetBianMa = cstrStartCodeTmp[iStartCodeNum] ;

			BOOL bBianMaChange = 0 ;
			
			while (!cSet.IsBOF())
			{
				CString cstrGuiDaoTmp ;
				cSet.GetFieldValue(_T("guidao"), cstrGuiDaoTmp) ;
				//this->GetGuiDaoBianMa(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, 0, 2, 1) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoTmp,cstrArrayBianMaTmp, cstrRetBianMa, bBianMaChange, 0, 2, 1) ;
				if (iStartCodeNum==8)
				{
					arrayBianMaChange.Add(bBianMaChange) ;
				}				
				cSet.MovePrevious() ;
			}			
			cSet.Close() ;
			
			CStringArray * pcstrArrayTmp = new CStringArray() ;
			pcstrArrayTmp->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp) ;			
		}	
		//end for
		//end ��վ��"L5,L4,...HU,ռ��"��9�����
		//////////////////////////////////////////////////////////////////////////		

		//����������ռ�����
		//////////////////////////////////////////////////////////////////////////
		cstrSql.Format(_T("select * from jinlu where guidao_updown=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (!cSet.IsEOF()) 
		{
			cSet.MoveLast() ;
		}
		//����ÿ��������
		while (!cSet.IsBOF()) 
		{
			//�����¼�����array��׼����һ���������У�
			cstrArrayBianMaTmp.RemoveAll() ;

			CString cstrGuiDaoName = _T("") ;
			long iGuiDaoId = 0 ;
			cSet.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
			cSet.GetFieldValue(_T("id"), iGuiDaoId) ;

			cstrDescribeTmp.Format(_T("%sռ��"), cstrGuiDaoName) ;
			cstrArrayDescribe.Add(cstrDescribeTmp) ;

			CADORecordset cSetTmp ;
			cstrSql.Format(_T("select * from jinlu where id<=%d"), iGuiDaoId) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			cSetTmp.Open(g_PtrCon, cstrSql) ;
			if (!cSetTmp.IsEOF())
			{
				cSetTmp.MoveLast() ;
			}
			//һ�б���
			cstrRetBianMa = _T("") ;
			while (!cSetTmp.IsBOF())
			{
				cSetTmp.GetFieldValue(_T("guidao"), cstrGuiDaoName) ;
				this->GetGuiDaoBianMa2(cstrGuiDaoName, cstrArrayBianMaTmp, cstrRetBianMa,bBianMaChange, 0, 2, 1) ;

				cSetTmp.MovePrevious() ;
			}
			cSetTmp.Close() ;
			
			CStringArray * pcstrArrayTmp1 = new CStringArray() ;
			pcstrArrayTmp1->Copy(cstrArrayBianMaTmp) ;
			arrayBianMa.Add(pcstrArrayTmp1) ;						

			cSet.MovePrevious() ;
		}
		//end while
		cSet.Close() ;

		//end ����������ռ�����
		//////////////////////////////////////////////////////////////////////////	
		
	}	
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-s_z_s_f"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in JinLuBianMa_s_z_s_f")) ;
	} 

    //���������
	CArray<BOOL, BOOL> arrayBianMaChange2 ;
	int iBianMaChangeSum = arrayBianMaChange.GetSize() ;
	
	for(int iBianMaChangeIndex=iBianMaChangeSum-1; iBianMaChangeIndex>=0; iBianMaChangeIndex--)
	{
		arrayBianMaChange2.Add(arrayBianMaChange.GetAt(iBianMaChangeIndex)) ;
	}	
	this->DrawBianMaTable(arrayBianMa, cstrArrayGuiDao, cstrArrayDescribe, arrayBianMaChange2) ;	
	//end ���������
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

void CMaXuBiaoForJH::FindJinLu(CString cstrStart, CString cstrEnd, CString cstrPassGuiDao, CStringArray &cstrArray)
{
	//ads_printf(_T("%s-%s\n"),cstrStart, cstrEnd) ;
	CStringArray cstrArray1 ;
	cstrArray1.Copy(cstrArray) ;

	cstrArray1.Add(cstrStart) ;
	if (cstrStart!=cstrEnd)
	{
		try
		{
			CADORecordset myset ;
			CString cstrSql ;
			cstrSql.Format(_T("select * from guidao where name='%s'"), cstrStart) ;
			myset.Open(g_PtrCon, cstrSql) ;
			
			if (!myset.IsBOF())
			{
				myset.MoveFirst() ;
			}
			while (!myset.IsEOF())
			{
				CString cstrNext1, cstrNext2, cstrNext3 ;
				myset.GetFieldValue(_T("next1"), cstrNext1) ;
				myset.GetFieldValue(_T("next2"), cstrNext2) ;
				myset.GetFieldValue(_T("next3"), cstrNext3) ;
				//CString cstrMsg ;
				//cstrMsg.Format(_T("%s-%s-%s"),cstrStart,cstrNext1,cstrNext2) ;
				//AfxMessageBox(cstrMsg) ;
				if (cstrNext1!=_T("") && cstrNext1!=cstrPassGuiDao)
				{
					//m_Test_pt[1] = m_Test_pt[1] + 3 ;
					//g_cSystem.WriteText(m_Test_pt, cstrNext1, 0, 9, 2) ;
					this->FindJinLu(cstrNext1, cstrEnd, cstrStart,cstrArray1) ;
					
				}
				if (cstrNext2!=_T("") && cstrNext2!=cstrPassGuiDao) 
				{
					//m_Test_pt[0] = m_Test_pt[0] + 3 ;
					//m_Test_pt[1] = m_Test_pt[1] + 3 ;
					//g_cSystem.WriteText(m_Test_pt, cstrNext2, 0, 9, 2) ;
					
					this->FindJinLu(cstrNext2, cstrEnd, cstrStart, cstrArray1) ;
				}
				if (cstrNext3!=_T("") && cstrNext3!=cstrPassGuiDao) 
				{
					//m_Test_pt[0] = m_Test_pt[0] + 3 ;
					//m_Test_pt[1] = m_Test_pt[1] + 3 ;
					//g_cSystem.WriteText(m_Test_pt, cstrNext3, 0, 9, 2) ;
					
					this->FindJinLu(cstrNext3, cstrEnd, cstrStart, cstrArray1) ;
				}
				
				myset.MoveNext() ;
			}
			myset.Close() ;			
		}
		catch (_com_error& e)
        {
			CString sBuff = GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in FindJinLu")) ;			
		}       
	}
	else//����ĩ��
	{
		if (cstrArray1.GetSize()>2)
		{
			CStringArray * cstrArrayTmp = new CStringArray ;
			cstrArrayTmp->Copy(cstrArray1) ;
			this->arrayJinLu.Add(cstrArrayTmp) ;
// 			CString strMsg =_T(_T("")) ;
// 			for(int ii=0; ii<cstrArray1.GetSize(); ii++)
// 			{
// 				strMsg.Format(_T("%s-%s"), strMsg, cstrArray1.GetAt(ii)) ;
// 				//ads_printf(_T("%s\n"), cstrArray1.GetAt(ii)) ;
// 			}
// 			AfxMessageBox(strMsg) ;
// 			//ads_printf(_T("\n\n")) ;
			
		}
	}
}

//ȡ��һ�����εı���
BOOL CMaXuBiaoForJH::GetGuiDaoBianMa2(CString cstrGuiDao, CStringArray &cstrArrayBianMa, CString &cstrRetBianMa, BOOL &bBianMaChange, int iCodeType, int iUpDown, int iDirect)
{
	CString cstrRetBianMaTmp ;
	cstrRetBianMaTmp.Format(_T("%s"), cstrRetBianMa) ;
	BOOL bRet = TRUE ;
	CStringList cstrList;
	switch(iCodeType) {
	case 0:
		{			
			for(int i=0; i<9; i++)
			{
				cstrList.AddTail(g_cstrMaXu_1[i]) ;
			}
		}		
		break;
	case 1:
		{	
			if (m_iVersionFor == 1)
			{
				for(int i=0; i<8; i++)
				{
					cstrList.AddTail(g_cstrMaXu_CeXiang2_1[i]) ;
				}
				
			}
			else
			{
				for(int i=0; i<8; i++)
				{
					cstrList.AddTail(g_cstrMaXu_CeXiang_1[i]) ;
				}
			}
		}		
		break;
	case 2:
		{			
			for(int i=0; i<8; i++)
			{
				cstrList.AddTail(g_cstrMaXu_YinDao_1[i]) ;
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
		CString cstrSql_1 ;
		cstrSql_1.Format(_T("select id from jinlu where guidao='%s'"), cstrGuiDao) ;
		if (cSet_1.IsOpen())
		{
			cSet_1.Close() ;
		}
		cSet_1.Open(g_PtrCon,cstrSql_1) ;
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
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGuiDaoBianMa2-find guidao_id")) ;
	} 
	//��·��������
	if (this->m_iCurJinLuDirect==0)
	{
		//�˹��λ�ڴӽ�վ����վ
		if (iGuiDao_id>=this->m_iJinZhanXHJ_id && iGuiDao_id<this->m_iChuZhanXHJ_F_id)
		{
			if (this->m_iZpGuiDaoSum_Left>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Left; iZpGuiDaoNum++)
				{
					CString cstrGuiDao_Zp_Tmp ;
					cstrGuiDao_Zp_Tmp = this->m_cstrArrayGuiDao_ZP_Left.GetAt(iZpGuiDaoNum) ;
					if (cstrGuiDao_Zp_Tmp==cstrGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//�����תƵ�������ΪZP/JC������ΪJC
				if (bIsZp) 
				{
					cstrArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					cstrArrayBianMa.Add(_T("JC")) ;
				}
				cstrRetBianMa = _T("JC") ;
				/*if (cstrRetBianMa!=cstrRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}
		}
		//�˹��λ�ڴӳ�վ�������վ
		else if (iGuiDao_id>this->m_iChuZhanXHJ_id && iGuiDao_id<=this->m_iJinZhanXHJ_F_id ) 
		{
			if (this->m_iZpGuiDaoSum_Rigth>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Rigth; iZpGuiDaoNum++)
				{
					CString cstrGuiDao_Zp_Tmp ;
					cstrGuiDao_Zp_Tmp = this->m_cstrArrayGuiDao_ZP_Right.GetAt(iZpGuiDaoNum) ;
					if (cstrGuiDao_Zp_Tmp==cstrGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//�����תƵ�������ΪZP/JC������ΪJC
				if (bIsZp) 
				{
					cstrArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					cstrArrayBianMa.Add(_T("JC")) ;
				}
				cstrRetBianMa = _T("JC") ;
				/*if (cstrRetBianMa!=cstrRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}
			
		}
		
	}
	//��·��������	
	else if (this->m_iCurJinLuDirect==1)
	{
		if (iGuiDao_id>=this->m_iJinZhanXHJ_F_id && iGuiDao_id<this->m_iChuZhanXHJ_id)
		{
			if (this->m_iZpGuiDaoSum_Left>0)
			{
				BOOL bIsZp = FALSE ;
				for(int iZpGuiDaoNum=0; iZpGuiDaoNum<m_iZpGuiDaoSum_Left; iZpGuiDaoNum++)
				{
					CString cstrGuiDao_Zp_Tmp ;
					cstrGuiDao_Zp_Tmp = this->m_cstrArrayGuiDao_ZP_Left.GetAt(iZpGuiDaoNum) ;
					if (cstrGuiDao_Zp_Tmp==cstrGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//�����תƵ�������ΪZP/JC������ΪJC
				if (bIsZp) 
				{
					cstrArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					cstrArrayBianMa.Add(_T("JC")) ;
				}
				cstrRetBianMa = _T("JC") ;
				/*if (cstrRetBianMa!=cstrRetBianMaTmp)
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
					CString cstrGuiDao_Zp_Tmp ;
					cstrGuiDao_Zp_Tmp = this->m_cstrArrayGuiDao_ZP_Right.GetAt(iZpGuiDaoNum) ;
					if (cstrGuiDao_Zp_Tmp==cstrGuiDao) 
					{
						bIsZp = TRUE ;
						break ;
					}
				}
				//�����תƵ�������ΪZP/JC������ΪJC
				if (bIsZp) 
				{
					cstrArrayBianMa.Add(_T("JC/ZP")) ;
				}
				else
				{
					cstrArrayBianMa.Add(_T("JC")) ;
				}
				cstrRetBianMa = _T("JC") ;
				/*if (cstrRetBianMa!=cstrRetBianMaTmp)
				{
					bBianMaChange = !bBianMaChange ;
				}*/
				return bRet ;
			}			
		}		

	}


	//����
	if (iUpDown==1) 
	{
		//�������У�
		if (iDirect==0)
		{
			if (cstrRetBianMa == _T(""))
			{
				cstrRetBianMa = cstrList.GetHead() ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = cstrList.Find(cstrRetBianMa) ;
				posEnd = cstrList.Find(_T("L5")) ;
				
				try
				{
					CString cstrSql ;
					CADORecordset cSet ;
					
					cstrSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, cstrSql) ;
					cstrSql.Format(_T("guidao='%s'"),cstrGuiDao) ;
					cSet.Find(cstrSql) ;		
					
					POSITION pos ;
					pos = cstrList.Find(cstrRetBianMa) ;
					
					if (!cSet.IsEOF()) 
					{
						cSet.MoveNext() ;
					}
					
					//�������һ�����		
					if (!cSet.IsEOF())
					{
						//CString cstrGuiDao ;
						CString cstrGuiDao_XHJ, cstrGuiDao_XHJ2 ;

						int iGuiDao_XHJ_UpDown = 0 ; 
						int iGuiDao_XHJ_Type = 0 ;
						int iGuiDao_Type = 0 ;
						
						CString cstrPreGuiDao ;
						CString cstrPreGuiDao_XHJ ;
						int iPreGuiDao_XHJ_UpDown = 0 ;	
						int iPreGuiDao_XHJ_Type = 0 ;
						int iPreGuiDao_XHJ_Direct = 0 ;
						int iPreGuiDao_Type = 0 ;
						
						cSet.GetFieldValue(_T("guidao"), cstrPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), cstrPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;
						cSet.GetFieldValue(_T("guidao_type"), iPreGuiDao_Type) ;
						
						cSet.MovePrevious() ;
						
						//cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
						cSet.GetFieldValue(_T("signal"), cstrGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal2"), cstrGuiDao_XHJ2) ;
						cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;	
						cSet.GetFieldValue(_T("guidao_type"), iGuiDao_Type) ;
											
						//��ͬ������������Ҫ�������
						//��ʱǰ��Ϊ�ҷ�
						//1.ǰ���źŻ���Ϊ��+ǰ���źŻ�Ϊ����+ǰ���źŻ����ǳ�վ(+ǰ���źŻ����Ƿ���)���ɵ��ָ�ʱ��ͨ�����վǰ�����
						//2.�����źŻ���Ϊ��+�����źŻ�Ϊ����+�����źŻ�Ϊ��վ���ɵ��ָ�ʱ��G2�����ܲ��ָ�ʱ�Ĺɵ�Ҳ�պ÷��ϣ�
						//3.�����źŻ���Ϊ��+�����Ϊ�ɵ�+���źŻ�2Ҳ��Ϊ�գ��ɵ����ָ�ʱ��						
						if ((cstrPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==1 && iPreGuiDao_XHJ_Type!=2  && !(iPreGuiDao_XHJ_Type==1&&iPreGuiDao_XHJ_Direct==1)/*&& iPreGuiDao_XHJ_Direct!=1*/) 
							||(cstrGuiDao_XHJ!=_T("") && iGuiDao_XHJ_UpDown==1 && iGuiDao_XHJ_Type==2 )
							||(cstrGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && cstrGuiDao_XHJ2!=_T("")) )
						{
							//bBianMaChange = !bBianMaChange ;
							if (posEnd != pos)
							{
								if (cstrRetBianMa == _T("JC"))
								{
									cstrRetBianMa = _T("HU") ;
								}
								else
								{
									cstrList.GetNext(pos) ;
									cstrRetBianMa = cstrList.GetAt(pos) ;									
								}
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
						}
						else
						{
							if (cstrRetBianMa==_T("ռ��"))
							{
								cstrRetBianMa = _T("JC") ;
							}
						}
					}
					//���һ����������
					else
					{
						cSet.MovePrevious() ;
						int iLastGuiDaoXHJ = 0 ;
						cSet.GetFieldValue(_T("signal_type"), iLastGuiDaoXHJ) ;
						//signal_type=1��ʾ�ǽ�վ�źŻ�
						if (iLastGuiDaoXHJ!=1)
						{
							if (pos!=posEnd)
							{
								cstrList.GetNext(pos) ;
								cstrRetBianMa = cstrList.GetAt(pos) ;
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
						}
					}
					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					CString sBuff = GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error")) ;
				} 
			}
			
			cstrArrayBianMa.Add(cstrRetBianMa) ;
		}
		//�������У�
		else if (iDirect==1)
		{
			if (cstrRetBianMa == _T(""))
			{
				cstrRetBianMa = cstrList.GetHead() ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = cstrList.Find(cstrRetBianMa) ;
				posEnd = cstrList.Find(_T("L5")) ;
				
				try
				{
					CString cstrSql ;
					CADORecordset cSet ;
					
					cstrSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, cstrSql) ;
					cstrSql.Format(_T("guidao='%s'"),cstrGuiDao) ;
					cSet.Find(cstrSql) ;		
					
					POSITION pos ;
					pos = cstrList.Find(cstrRetBianMa) ;

					CString cstrGuiDao_XHJ, cstrGuiDao_XHJ2 ;
					int iGuiDao_XHJ_UpDown = 0 ; 
					int iGuiDao_XHJ_Type = 0 ;
					int iGuiDao_XHJ_Direct = 0 ;
					int iGuiDao_Type = 0 ;
										
					CString cstrPreGuiDao ;
					CString cstrPreGuiDao_XHJ ;
					int iPreGuiDao_UpDown = 0 ;
					int iPreGuiDao_XHJ_UpDown = 0 ;	
					int iPreGuiDao_XHJ_Type = 0 ;
					int iPreGuiDao_XHJ_Direct = 0 ;

					cSet.GetFieldValue(_T("signal"), cstrGuiDao_XHJ) ;
					cSet.GetFieldValue(_T("signal2"), cstrGuiDao_XHJ2) ;
					cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
					cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;
					cSet.GetFieldValue(_T("signal_direct"), iGuiDao_XHJ_Direct) ;
				
					cSet.MovePrevious() ;

					BOOL bIsFirstGd = TRUE ;
					
					//����firstһ�����		
					if (!cSet.IsBOF())
					{
						bIsFirstGd = FALSE ;
						cSet.GetFieldValue(_T("guidao"), cstrPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), cstrPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("guidao_updown"), iPreGuiDao_UpDown) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;
					}
					
					//��ͬ������������Ҫ�������					
					//��ʱǰ���������
					//���ݡ�1LQ��2LQ�����ڷ���������кϲ�Ϊһ�����Ρ��Ľ��ڶ���
					//1.ǰ���źŻ���Ϊ��+ǰ���źŻ�Ϊ����+ǰ���źŻ��ǽ�վ+ǰ���źŻ��Ƿ���LQ���Σ�
					//2.�����źŻ���Ϊ��+�����źŻ�Ϊ����ͨ��+ǰ���������վ�ڻ���LQ��ÿ����һ��������ͨ���źŻ������ξ�Ҫ������룩//20110608:1LQ�зָ������������&& cstrGuiDao!=_T("X1LQAG")
					//3.�����źŻ���Ϊ��+���źŻ�Ϊ��վ+���źŻ�Ϊ���У��ɵ��ָ�ʱ��G1��
					//4.�����źŻ���Ϊ��+�����Ϊ�ɵ�+���źŻ�2Ҳ��Ϊ�գ��ɵ����ָ�ʱ��
					//20100831:�����վ�ͱ�վ��һ���������һ�������
					//5.
					if ((cstrPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==2 && iPreGuiDao_XHJ_Type==1 && iPreGuiDao_XHJ_Direct==1 ) 
						||(cstrGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==0 && (iPreGuiDao_UpDown!=0&&cstrPreGuiDao!=_T("X1LQAG")) ) 
						|| (cstrGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==2 && iGuiDao_XHJ_UpDown==2)
						|| (cstrGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && cstrGuiDao_XHJ2!=_T(""))
						||(cstrGuiDao_XHJ!=_T("") && iGuiDao_Type==0 && bIsFirstGd)) 
					{
						//bBianMaChange = !bBianMaChange ;
						if (posEnd != pos)
						{
							if (cstrRetBianMa == _T("JC"))
							{
								cstrRetBianMa = _T("HU") ;
							}
							else
							{
								cstrList.GetNext(pos) ;
								cstrRetBianMa = cstrList.GetAt(pos) ;
							}
						}
						//bianmachange
						bBianMaChange = !bBianMaChange ;
						
					}
					else
					{					
						if (cstrRetBianMa==_T("ռ��"))
						{
							cstrRetBianMa = _T("JC") ;
						}
					}
					
					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					CString sBuff = GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error")) ;
				} 
			}
			
			cstrArrayBianMa.Add(cstrRetBianMa) ;
		}

		
	}
	//����
	else if (iUpDown==2)
	{
		//�������У�
		if (iDirect==0)
		{
			if (cstrRetBianMa == _T(""))
			{
				cstrRetBianMa = cstrList.GetHead() ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = cstrList.Find(cstrRetBianMa) ;
				posEnd = cstrList.Find(_T("L5")) ;
				
				try
				{
					CString cstrSql ;
					CADORecordset cSet ;
					
					cstrSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, cstrSql) ;
					cstrSql.Format(_T("guidao='%s'"),cstrGuiDao) ;
					cSet.Find(cstrSql) ;		
					
					POSITION pos ;
					pos = cstrList.Find(cstrRetBianMa) ;
					
					cSet.MovePrevious() ;
					
					//���ǵ�һ�����		
					if (!cSet.IsBOF())
					{
						//CString cstrGuiDao ;
						CString cstrGuiDao_XHJ, cstrGuiDao_XHJ2 ;
						int iGuiDao_XHJ_UpDown = 0 ; 
						int iGuiDao_XHJ_Type = 0 ;
						int iGuiDao_Type = 0 ;
						
						CString cstrPreGuiDao ;
						CString cstrPreGuiDao_XHJ ;
						int iPreGuiDao_XHJ_UpDown = 0 ;	
						int iPreGuiDao_XHJ_Type = 0 ;
						int iPreGuiDao_XHJ_Direct = 0 ;
						
						cSet.GetFieldValue(_T("guidao"), cstrPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), cstrPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;
						
						cSet.MoveNext() ;
						
						//cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
						cSet.GetFieldValue(_T("signal"), cstrGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("signal2"), cstrGuiDao_XHJ2) ;
						cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;						
						
						//��ͬ������������Ҫ�������
						//��ʱǰ������
						//1.ǰ���źŻ���Ϊ��+ǰ���źŻ�Ϊ����+ǰ���źŻ����ǳ�վ(+ǰ���źŻ����Ƿ���)���ɵ��ָ�ʱ��ͨ�����վǰ�����
						//2.�����źŻ���Ϊ��+�����źŻ�Ϊ����+�����źŻ�Ϊ��վ���ɵ��ָ�ʱ��G2�����ܲ��ָ�ʱ�Ĺɵ�Ҳ�պ÷��ϣ�
						//3.�����źŻ���Ϊ��+�����Ϊ�ɵ�+���źŻ�2Ҳ��Ϊ�գ��ɵ����ָ�ʱ��
						if ((cstrPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==2 && iPreGuiDao_XHJ_Type!=2 && !(iPreGuiDao_XHJ_Type==1&&iPreGuiDao_XHJ_Direct==1)/*&& iPreGuiDao_XHJ_Direct!=1*/ ) 
							||(cstrGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==2 && iGuiDao_XHJ_UpDown==2)
							||(cstrGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && cstrGuiDao_XHJ2!=_T("") )) 
						{
							//bBianMaChange = !bBianMaChange ;
							if (posEnd != pos)
							{
								if (cstrRetBianMa == _T("JC"))
								{
									cstrRetBianMa = _T("HU") ;
								}
								else
								{
									cstrList.GetNext(pos) ;
									cstrRetBianMa = cstrList.GetAt(pos) ;
								}
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
							
						}
						else
						{
							if (cstrRetBianMa==_T("ռ��"))
							{
								cstrRetBianMa = _T("JC") ;
							}
						}
					}
					//��һ����������
					else
					{
						cSet.MoveNext() ;
						int iFirstGuiDaoXHJ = 0 ;
						cSet.GetFieldValue(_T("signal_type"), iFirstGuiDaoXHJ) ;
						if (iFirstGuiDaoXHJ!=1)
						{
							if (pos!=posEnd)
							{
								cstrList.GetNext(pos) ;
								cstrRetBianMa = cstrList.GetAt(pos) ;
							}
							//bianmachange
							bBianMaChange = !bBianMaChange ;
							
						}
							

					}
					
					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					CString sBuff = GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error")) ;
				} 
			}
			
			cstrArrayBianMa.Add(cstrRetBianMa) ;
			
		}
		//�������У�
		else if (iDirect==1)
		{
			if (cstrRetBianMa == _T(""))
			{
				cstrRetBianMa = cstrList.GetHead() ;
			}
			else
			{
				POSITION  posBegin, posEnd ;
				posBegin = cstrList.Find(cstrRetBianMa) ;
				posEnd = cstrList.Find(_T("L5")) ;
				
				try
				{
					CString cstrSql ;
					CADORecordset cSet ;
					
					cstrSql.Format(_T("select * from jinlu")) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					cSet.Open(g_PtrCon, cstrSql) ;
					cstrSql.Format(_T("guidao='%s'"),cstrGuiDao) ;
					cSet.Find(cstrSql) ;		
					
					POSITION pos ;
					pos = cstrList.Find(cstrRetBianMa) ;		
					
					CString cstrGuiDao_XHJ, cstrGuiDao_XHJ2 ;
					int iGuiDao_XHJ_UpDown = 0 ; 
					int iGuiDao_XHJ_Type = 0 ;
					int iGuiDao_XHJ_Direct = 0 ;
					int iGuiDao_Type = 0 ;

					CString cstrPreGuiDao ;
					CString cstrPreGuiDao_XHJ ;
					int iPreGuiDao_UpDown = 0 ;
					int iPreGuiDao_XHJ_UpDown = 0 ;	
					int iPreGuiDao_XHJ_Type = 0 ;
					int iPreGuiDao_XHJ_Direct = 0 ;

					//cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
					cSet.GetFieldValue(_T("signal"), cstrGuiDao_XHJ) ;	
					cSet.GetFieldValue(_T("signal2"), cstrGuiDao_XHJ2) ;
					cSet.GetFieldValue(_T("signal_updown"), iGuiDao_XHJ_UpDown) ;
					cSet.GetFieldValue(_T("signal_type"), iGuiDao_XHJ_Type) ;
					cSet.GetFieldValue(_T("signal_direct"), iGuiDao_XHJ_Direct) ;

					cSet.MoveNext() ;

					BOOL bIsLastGd = TRUE ;
					
					//�������һ�����		
					if (!cSet.IsEOF())
					{
						bIsLastGd = FALSE ;
						cSet.GetFieldValue(_T("guidao"), cstrPreGuiDao) ;
						cSet.GetFieldValue(_T("signal"), cstrPreGuiDao_XHJ) ;
						cSet.GetFieldValue(_T("guidao_updown"), iPreGuiDao_UpDown) ;
						cSet.GetFieldValue(_T("signal_updown"), iPreGuiDao_XHJ_UpDown) ;
						cSet.GetFieldValue(_T("signal_type"), iPreGuiDao_XHJ_Type) ;
						cSet.GetFieldValue(_T("signal_direct"), iPreGuiDao_XHJ_Direct) ;					
					}
					
					//��ͬ������������Ҫ�������
					//��ʱǰ�����ҷ�
					//���ݡ�1LQ��2LQ�����ڷ���������кϲ�Ϊһ�����Ρ��Ľ��ڶ���
					//1.ǰ���źŻ���Ϊ��+ǰ���źŻ�Ϊ����+ǰ���źŻ��ǽ�վ+ǰ���źŻ��Ƿ���LQ���Σ�
					//2.�����źŻ���Ϊ��+�����źŻ�Ϊ����ͨ��+ ǰ���������վ�ڻ���LQ��ÿ����һ��������ͨ���źŻ������ξ�Ҫ������룩 //20110608:1LQ�зָ������������&& cstrGuiDao!=_T("S1LQAG")
					//3.�����źŻ���Ϊ��+���źŻ�Ϊ��վ+���źŻ�Ϊ���У��ɵ��ָ�ʱ��G1��
					//4.�����źŻ���Ϊ��+�����Ϊ�ɵ�+���źŻ�2Ҳ��Ϊ�գ��ɵ����ָ�ʱ��
					//20100831:�����վ�ͱ�վ��һ���������һ�������
					//5.
					if ((cstrPreGuiDao_XHJ!=_T("") && iPreGuiDao_XHJ_UpDown==1 && iPreGuiDao_XHJ_Type==1  && iPreGuiDao_XHJ_Direct==1) 
						||(cstrGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==0 && (iPreGuiDao_UpDown!=0 && cstrPreGuiDao!=_T("S1LQAG"))) 
						||(cstrGuiDao_XHJ!=_T("") && iGuiDao_XHJ_Type==2 && iGuiDao_XHJ_UpDown==1 )
						||(cstrGuiDao_XHJ!=_T("") && iGuiDao_Type>0 && cstrGuiDao_XHJ2!=_T(""))
						||(cstrGuiDao_XHJ!=_T("") && iGuiDao_Type==0 && bIsLastGd)) 
					{
						//bBianMaChange = !bBianMaChange ;						
						if (posEnd != pos)
						{
							if (cstrRetBianMa == _T("JC"))
							{
								cstrRetBianMa = _T("HU") ;
							}
							else
							{
								cstrList.GetNext(pos) ;
								cstrRetBianMa = cstrList.GetAt(pos) ;
							}
						}
						//bianmachange
						bBianMaChange = !bBianMaChange ;
						
					}
					else
					{
						if (cstrRetBianMa==_T("ռ��"))
						{
							//��Ҫ���ӶԹɵ�2���ж�
							if (iGuiDao_XHJ_Type==2)
							{
								cstrList.GetNext(pos) ;
								cstrRetBianMa = cstrList.GetAt(pos) ;								
							}
							else
							{
								cstrRetBianMa = _T("JC") ;
							}
							
						}
					}

					cSet.Close() ;
					
				}
				catch (_com_error& e)
				{
					bRet = FALSE ;
					CString sBuff = GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;  
				}
				catch (...)
				{
					bRet = FALSE ;
					AfxMessageBox(_T("unknown error")) ;
				} 
			}
			
			cstrArrayBianMa.Add(cstrRetBianMa) ;			
			
		}
	}
	/*if (cstrRetBianMa!=cstrRetBianMaTmp)
	{
		bBianMaChange = !bBianMaChange ;
	}
	*/
	return bRet ;

}

//ȡ�ý�վ����վ�������վ�������վ���ɵ����ɵ�����
BOOL CMaXuBiaoForJH::GetJinZhanOrChuZhan(CString &cstrJinZhanXHJ, int &iJinZhanXHJ_Id, CString &cstrChuZhanXHJ, int &iChuZhanXHJ_Id, CString &cstrJinZhanXHJ_F, int &iJinZhanXHJ_F_Id, CString &cstrChuZhanXHJ_F, int &iChuZhanXHJ_F_Id, CString &cstrGuDao, int &iGuDaoType, int iDirect)
{
	BOOL bRet = TRUE ;
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;
		//find��վ
		cstrSql.Format(_T("select * from jinlu where signal_type=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;			
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		if (cSet.GetRecordCount()>=2) 
		{
			//��վΪX or XN
			if (iDirect==0) 
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_Id) ;
					cSet.GetFieldValue(_T("signal"), cstrJinZhanXHJ) ;
					cSet.MoveLast() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_F_Id) ;
					cSet.GetFieldValue(_T("signal"), cstrJinZhanXHJ_F) ;
				}
			}
			//��վΪS or SN
			else
			{
				if (!cSet.IsEOF())
				{
					cSet.MoveLast() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_Id) ;
					cSet.GetFieldValue(_T("signal"), cstrJinZhanXHJ) ;
					cSet.MoveFirst() ;
					cSet.GetFieldValue(_T("id"), iJinZhanXHJ_F_Id) ;
					cSet.GetFieldValue(_T("signal"), cstrJinZhanXHJ_F) ;
				}
			}
		}
		else
		{
			AfxMessageBox(_T("��·�Ҳ���2����վ-GetJinZhanOrChuZhan")) ;
		}
		cSet.Close() ;

		//find ��վ
		//cstrSql.Format(_T("select * from jinlu where signal_type=2 and signal_updown=1")) ;
		cstrSql.Format(_T("select * from jinlu where signal_type=2")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;			
		}
		cSet.Open(g_PtrCon, cstrSql) ;
		int iRecordCount = cSet.GetRecordCount() ; 
		if (iRecordCount>0)
		{	
			//�õ��ɵ����͡�����
			cSet.GetFieldValue(_T("guidao_type"), iGuDaoType) ;			
			cSet.GetFieldValue(_T("guidao"), cstrGuDao) ;

			if (iRecordCount==2) 
			{
				//��վΪXI 
				if (iDirect==0) 
				{
					if (!cSet.IsEOF())
					{
						cSet.MoveLast() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_Id) ;
						cSet.GetFieldValue(_T("signal"), cstrChuZhanXHJ) ;
						cSet.MoveFirst() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_F_Id) ;
						cSet.GetFieldValue(_T("signal"), cstrChuZhanXHJ_F) ;
					}
				}
				//��վΪSI
				else
				{
					if (!cSet.IsBOF())
					{
						cSet.MoveFirst() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_Id) ;
						cSet.GetFieldValue(_T("signal"), cstrChuZhanXHJ) ;
						cSet.MoveLast() ;
						cSet.GetFieldValue(_T("id"), iChuZhanXHJ_F_Id) ;
						cSet.GetFieldValue(_T("signal"), cstrChuZhanXHJ_F) ;
					}
				}
				//���ָ�Ĺɵ�������4G1��4G2Ҫȥ��ĩβ����
				cstrGuDao = cstrGuDao.Left(cstrGuDao.GetLength()-1) ;		
				
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
				
				//signal�����г�վ��signal2�����г�վ
				//��վΪXI
				if ((iChuZhanXHJ_Updown_Tmp==1 && iDirect==0)||(iChuZhanXHJ_Updown_Tmp==2 && iDirect==1))
				{
					
					cSet.GetFieldValue(_T("signal"), cstrChuZhanXHJ) ;
					cSet.GetFieldValue(_T("signal2"), cstrChuZhanXHJ_F) ;			
					
				}
				//��վΪSI
				else if((iChuZhanXHJ_Updown_Tmp==1 && iDirect==1)||(iChuZhanXHJ_Updown_Tmp==2 && iDirect==0))
				{
					cSet.GetFieldValue(_T("signal2"), cstrChuZhanXHJ) ;
					cSet.GetFieldValue(_T("signal"), cstrChuZhanXHJ_F) ;
				} 
			}//end else if(iRecordCount==1)
		} //end if (iRecordCount>0)
		else
		{
			AfxMessageBox(_T("��·�Ҳ�����վ-GetJinZhanOrChuZhan")) ;
		}
		cSet.Close() ;

		
	}
	catch (_com_error& e)
	{
		bRet = FALSE ;
		CString sBuff = GetErrorDescription(e) ;
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

//�ҳ�ĳһ������ҪתƵ�Ĺ��
//iDirect:
//0:��������
//1:��������
int CMaXuBiaoForJH::FindZP(CStringArray &cstrArrayGuiDao_ZP, int iStart_id, int iEnd_id, int iDirect)
{
	int iZpGuiDaoSum = 0 ;
	try
	{
		CADORecordset cSet ;
		CString cstrSql ;

		int iGuiDao_TopOrDown = 0 ;
		CString cstrGuiDao(_T("")) ;
		
		cstrSql.Format(_T("select * from jinlu where id>=%d and id<=%d"), iStart_id, iEnd_id) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, cstrSql) ;
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
					cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
					cstrArrayGuiDao_ZP.Add(cstrGuiDao) ;
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
					cSet.GetFieldValue(_T("guidao"), cstrGuiDao) ;
					cstrArrayGuiDao_ZP.Add(cstrGuiDao) ;
					iZpGuiDaoSum ++ ;
					iGuiDao_TopOrDown = iGuiDao_TopOrDown_Tmp ;
				}
				
				cSet.MovePrevious() ;
			}
			
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-FindZP"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FindZP")) ;
	} 	

	return iZpGuiDaoSum ;
}

void CMaXuBiaoForJH::GetLinZhanStateSum(int &iRet, int iFirstOrLast)
{
	iRet = 9 ;
	try
	{
		CString cstrSql ;
		CADORecordset cSet ;
		cstrSql.Format(_T("select * from jinlu")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}	
		cSet.Open(g_PtrCon, cstrSql) ;
		
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
		CString sBuff = GetErrorDescription(e) ;
		sBuff.Format(_T("%s-GetLinZhanStateSum"), sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetLinZhanStateSum")) ;
	} 	
	

}

void CMaXuBiaoForJH::SetGuiDaoFreq(CString &strGuiDao, const CString &strEndGuiDao, int &iIndex, int iUpOrDown/*=0*/)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{			
			//���� AcDbBlockReference ;
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

					g_cSystem.EditValue(pRef, _T("FREQUENCE"), strFreqTmp) ;					

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

void CMaXuBiaoForJH::SetOneGuiDaoFreq(CString strGuiDao, CString strFreq)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{			
			//���� AcDbBlockReference ;
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;

				CString strTmp ;				
				g_cSystem.GetAttValue(pRef, _T("GUIDAO"), strTmp) ;
				
				if (strTmp == strGuiDao )
				{
					g_cSystem.EditValue(pRef, _T("FREQUENCE"), strFreq) ;
				}				
				
			}
			pEnt->close() ;
		}
		
	}

}

void CMaXuBiaoForJH::GetJinLuGuiDaoFreq(const CStringArray &strArrayGuiDao, CStringArray &strArrayFreq)
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
			CString cstrSql ;
			
			cstrSql.Format(_T("select frequence from guidao where name='%s'"), strGuiDaoTmp ) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			cSet.Open(g_PtrCon, cstrSql) ;
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
			CString sBuff = GetErrorDescription(e) ;
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

//���ź�ƽ��ͼ�������ݿ�
void CMaXuBiaoForJH::ReadDwgToDb2(LPUNKNOWN m_pConnection, AcDbDatabase * pCurDb)
{
	CString cstrSql ;

	_ConnectionPtr ptrCon; //�����ݿ������
	ptrCon = ( _ConnectionPtr ) m_pConnection ;

	CUserSystem mysys ;
	CUserUtility userutility ;

	CString cstrTable[2] = {_T("guidao"),_T("signal")} ;
	for(int iTb=0; iTb<2; iTb++)
	{
		userutility.ClearTable(cstrTable[iTb]) ;
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
	CString cstrAppNamePre[3] = {_T("PRE1"),_T("PRE2"), _T("PRE3") } ;
	CString cstrAppNameNext[3] = {_T("NEXT1"),_T("NEXT2"), _T("NEXT3") } ;
	//CString cstrAppName
	
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//����ͼ�� AcDbBlockReference ;
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;

				//�õ�����
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;

				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;		

				strBlockName.MakeLower() ;

				//ȷ����Ҫ��ȡ�Ĺ������
				if (strBlockName.Mid(0,15)==_T("maxubiao_guidao"))
				{

					CString cstrPre[3] = {_T(""), _T(""), _T("")} ;
					CString cstrNext[3] = {_T(""), _T(""), _T("")} ;
					CString cstrGuiDao, cstrXHJ , cstrXHJ2 ;
					int iGuidaoType = 0 ;
					int iUpOrDown = 0 ;
					int iTopOrBottom = 0 ;
					int iStartEnd = 0 ;
					CString strFreq = _T("") ;
					CString cstrTmp ;
					int iTmp = -1 ;

					int iXHJType = 0 ;
					int iXHJUpDown = 0 ;
					int iXHJDirect = 0 ;

					int iXHJType2 = 0 ;
					int iXHJUpDown2 = 0 ;
					int iXHJDirect2 = 0 ;
					
					//��"_"Ϊ��ǩ�ֽ�����maxubiao_guidao_gd_c_all�Ŀ���
					CStringArray strArrayBlokcName ;
					g_cSystem.DivideString(strBlockName, _T("_"), strArrayBlokcName) ;

					CString strBlockNamePart3 = strArrayBlokcName.GetAt(2) ;
					CString strBlockNamePart4 = strArrayBlokcName.GetAt(3) ;
					CString strBlockNamePart5 = strArrayBlokcName.GetAt(4) ;
					//ͨ��
					if (strBlockNamePart3 == _T("tg"))
					{
						iGuidaoType = 0 ;
						//����
						if (strBlockNamePart4==_T("x"))
						{
							iUpOrDown = 1 ;
						}
						else//����
						{
							iUpOrDown = 2 ;
						}
						//�޷ָ��ͱ�ʾ���źŻ�
						if (strBlockNamePart5==_T("wfg"))
						{
							g_cSystem.GetAttValue(pRef,_T("XINHAOJI"),cstrXHJ) ;
							iXHJType = 0 ;
							iXHJUpDown = iUpOrDown ;
							iXHJDirect = 0 ;
						}

					} //end ͨ��
					//��վ
					else if (strBlockNamePart3 == _T("jz"))
					{
						iGuidaoType = 0 ;
						//����
						if (strBlockNamePart4==_T("x"))
						{
							iXHJUpDown = 1 ;
						}
						else//����
						{
							iXHJUpDown = 2 ;
						}

						g_cSystem.GetAttValue(pRef,_T("XINHAOJI"),cstrXHJ) ;
						iXHJType = 1 ;
						//�����վ
						if (strBlockNamePart5==_T("f"))
						{							
							iXHJDirect = 1 ;
						}
					}// end ��վ
					//�ɵ�
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
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI"), cstrXHJ) ;
							iXHJType = 2 ;
							iXHJUpDown = 2 ;
						}
						else if (strBlockNamePart5 == _T("right"))
						{
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI"), cstrXHJ) ;
							iXHJType = 2 ;
							iXHJUpDown = 1 ;
						}
						else if (strBlockNamePart5 == _T("all"))
						{
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI_S"), cstrXHJ) ;
							iXHJType = 2 ;
							iXHJUpDown = 2 ;
							g_cSystem.GetAttValue(pRef, _T("XINHAOJI_X"), cstrXHJ2) ;
							iXHJType2 = 2 ;
							iXHJUpDown2 = 1 ;
						}

					}
					//end �ɵ�
					else if (strBlockNamePart3 == _T("lq"))
					{
						iGuidaoType = -1 ;
					}

					// ����վ�ں���ȥֻҪ��ȡǰ�����Ϳ�����

					g_cSystem.GetAttValue(pRef, _T("GUIDAO"), cstrGuiDao) ;
					g_cSystem.GetAttValue(pRef, _T("FREQUENCE"), strFreq) ;

					for(int i=0; i<3; i++)
					{
						g_cSystem.GetAttValue(pRef, cstrAppNamePre[i], cstrPre[i]) ;
					}
					for(int i2=0; i2<3; i2++)
					{
						g_cSystem.GetAttValue(pRef,cstrAppNameNext[i2], cstrNext[i2]) ;
					}

					try
					{
						CString strPlus1 = _T("") ;
						long iPlus2 = 0 ;
						CString strAOrBG = _T("") ;
						
						if(mysys.IsStrMatch(_T("^\\d+[A,B]*G$"), cstrGuiDao) && iGuidaoType==0) //������
						{
							CString strPart1 ;
							mysys.RegReplace(_T("^(\\d+)([A,B]*)G$"), _T("$1"), cstrGuiDao, strPart1) ;	
							mysys.RegReplace(_T("^(\\d+)([A,B]*)G$"), _T("$2"), cstrGuiDao, strAOrBG) ;	
							if (iUpOrDown==1)
							{
								strPlus1.Format(_T("1-%s-%d"), strPart1, strAOrBG==_T("A")?2:1) ;
							}
							else
							{
								strPlus1.Format(_T("2-%s-%d"), strPart1, strAOrBG==_T("A")?1:2) ;
							}							
						}
						else if (mysys.IsStrMatch(_T("^\\w+DG$"), cstrGuiDao)) //3DG
						{
							CString strPart1 ;
							if (mysys.IsStrMatch(_T("^\\d+DG$"), cstrGuiDao))
							{
								mysys.RegReplace(_T("^(\\d+)DG$"), _T("$1"), cstrGuiDao, strPart1) ;
								int iPart1 = _tstoi(strPart1) ;
								iPlus2 = (iPart1%2==0?3:1)*10000+iPart1 ;
								//strPlus1.Format(_T("%d-%s"), iPart1%2==0?3:1, strPart1) ;
							}
							else if (mysys.IsStrMatch(_T("^\\d+-\\d+DG$"), cstrGuiDao))
							{
								mysys.RegReplace(_T("^(\\d+)-(\\d+)DG$"), _T("$1"), cstrGuiDao, strPart1) ;								
								int iPart1 = _tstoi(strPart1) ;
								iPlus2 = (iPart1%2==0?3:1)*10000+iPart1 ;
								//strPlus1.Format(_T("%d-%s"), iPart1%2==0?3:1, strPart1) ;
							}
						}
						else if (mysys.IsStrMatch(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-3]?$"), cstrGuiDao)) //IG 3G
						{
							CString strPart1 ;
							CString strPart1New = _T("") ;
							mysys.RegReplace(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-3]?$"), _T("$1"), cstrGuiDao, strPart1) ;
							BOOL bConvertOk = this->ConvertSpecialChar(strPart1, strPart1New) ;
							int iPart1 = _tstoi(bConvertOk==TRUE?strPart1New:strPart1) ;
							iPlus2 = 20000+iPart1 ;
							//strPlus1.Format(_T("2-%s"), bConvertOk==TRUE?strPart1New:strPart1) ;
						}

					cstrSql.Format(_T("insert into guidao(name,pre1,pre2,pre3,next1,next2,next3,signal,signal2, type, upordown,startend,toporbottom,frequence, plus1, plus2,a_or_b) \
						values('%s','%s','%s','%s','%s','%s','%s','%s', '%s', %d,%d,%d,%d,'%s','%s', %d, '%s')"),\
						cstrGuiDao,cstrPre[0],cstrPre[1],cstrPre[2],cstrNext[0],cstrNext[1],cstrNext[2],cstrXHJ,cstrXHJ2,iGuidaoType, iUpOrDown,iStartEnd,iTopOrBottom,strFreq, strPlus1, iPlus2, strAOrBG) ;
					ExecSqlString(cstrSql) ;
					}
					catch (_com_error& e)
					{
						CString sBuff = GetErrorDescription(e) ;
						AfxMessageBox(sBuff) ;	
					}
					catch (...)
					{
						AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;
						
					}
					
					if (!cstrXHJ.IsEmpty())
					{
						try
						{
						cstrSql.Format(_T("insert into signal(name,type, upordown,direct,guidao) \
							values('%s',%d,%d,%d,'%s')"),cstrXHJ,iXHJType,iXHJUpDown,iXHJDirect,cstrGuiDao) ;
						ExecSqlString(cstrSql) ;				
						
						}
						catch (_com_error& e)
						{
							CString sBuff = GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;							
						}	
					}
					if (!cstrXHJ2.IsEmpty())
					{
						try
						{
						cstrSql.Format(_T("insert into signal(name,type, upordown,direct,guidao) \
							values('%s',%d,%d,%d,'%s')"),cstrXHJ2,iXHJType2,iXHJUpDown2,iXHJDirect2,cstrGuiDao) ;
						ExecSqlString(cstrSql) ;				
						
						}
						catch (_com_error& e)
						{
							CString sBuff = GetErrorDescription(e) ;
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
			//�ֽ������
			long iLeftDivideRealLoc = 0 ;
			long iRightDivideRealLoc = 0 ;
			this->FindDivide(this->m_strStationName, iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
			
			//begin ������Ĺ�����δ�compute_xhj_table����
			if (i==0)
			{
				//strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<%d and up_or_down=%d and gd_name<>'' order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
				strSql.Format(_T("select * from compute_xhj_table where ((loc_real>=%d and loc_real<=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=0 and up_or_down=0)) or (loc_real<%d and loc_real>=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=1 and up_or_down=0))) and up_or_down=0 and right(name,1)<>'��' order by loc_real asc"),iLeftDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc,iRightDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc) ;
			}
			else
			{
				//strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and right(name,1)<>'��' and type<>11 order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
				strSql.Format(_T("select * from compute_xhj_table where ((loc_real>=%d and loc_real<=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=1 and up_or_down=1)) or (loc_real<=%d and loc_real>=(select loc_real from compute_xhj_table where loc_real>=%d and loc_real<%d and type=3 and is_reverse=0 and up_or_down=1))) and up_or_down=1 and right(name,1)<>'��' order by loc_real asc"),iLeftDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc,iRightDivideRealLoc, iLeftDivideRealLoc,iRightDivideRealLoc) ;
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
					
					if (i==0)//�����źŻ������Ƶ�ʾ��Ǳ������Ƶ��
					{
						cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
					}
					else //�����źŵ�����Ĺ����Ƶ��,����ǰһ���źŵ����д�� Ƶ��
					{
						if (iIndex==0)//���е�һ���źŵ㣨�ֽ�㣩������selectʱ��������ҪΪ��ȡ����Ƶ��
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

						if (iPreXhjType==3&&iIsPreXhjReverse==0&&i==1) //���ǰһ����S��վ����˹���Ѿ��ڴ����վ��Sʱ��ӹ���
						{
							//����S��վ�ҷ���һ�������Next1
							strSql.Format(_T("update guidao set next1='%s' where name='%s'"),strNext1GdName, strGdName) ;
							ExecSqlString(strSql) ;
							cSetFind2.MoveNext() ;
							iIndex++ ;
							continue ;
						}
						//strPre1GdName.Insert(100, _T("G")) ;
						//strNext1GdName.Insert(100, _T("G")) ;
					}
					
					//begin ��4����վ�ڵĴ���
					//X��վ
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
					//SN��վ
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
					//S��վ
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
					//XN��վ
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

					//end ��4����վ�ڵĴ���
					
					strSql.Format(_T("insert into guidao (name, pre1, next1, signal, type, upordown, frequence) values('%s', '%s', '%s', '%s',%d, %d, '%s')"), strGdName, strPre1GdName, strNext1GdName, strXHName, iGuidaoType, i+1, strFreq) ;
					ExecSqlString(strSql) ;
					
					iIndex++ ;
					cSetFind2.MoveNext() ;
				}
				cSetFind2.Close() ;
			}
			//end ������Ĺ�����δ�compute_xhj_table����			
		}

		//begin ������վ�ڹ����Ƶ�ʴ�compute_xhj_table����
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
		//end ������վ�ڹ����Ƶ�ʴ�compute_xhj_table����


	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ReadDwgToDb2")) ;
		
	}

}


void CMaXuBiaoForJH::ExportLKQCTable(CString strSaveFileName)
{	
	g_cSystem.GetBasePath(m_strSysBasePath) ;//ϵͳ·��

	CString strTempletFile ;
	strTempletFile.Format(_T("%s\\support\\lkdata\\�п����Ĳɼ��������ñ�.xls"), m_strSysBasePath) ;

	int iQdCurIndex = 12 ;
	int iCjCurIndex = 20 ;

	CUserSystem mysys ;

	BOOL bIsZJZ = TRUE ;
	try
	{
		//�ҳ��Ƿ��з�ͨ���Ĺ��������������м�վ
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
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ExportLKQCTable")) ;		
	}

	if (bIsZJZ)
	{
		strTempletFile.Format(_T("%s\\support\\lkdata\\�п����Ĳɼ��������ñ�-�м�վ.xls"), m_strSysBasePath) ;
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
			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
			return ;
		}

		//pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBook = m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strTempletFile ) ;
		
		//����
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;


		CString strWriteText = _T("") ;

		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)_T("B2")) ;
		strWriteText.Format(_T("%s�п������������ñ�"), this->m_strStationName) ;
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
				strColName = mysys.GetExcelFieldName(iCol) ;
				iRow = iQdCurIndex%32+5 ;
				CString strCellName = _T("") ;
				strCellName.Format(_T("%s%d"), strColName, iRow) ;
				strWriteText = _T("վ�ڷ���̵���") ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

				strColName = mysys.GetExcelFieldName(iCol+1) ;
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

		//�ɼ�
		vt_index.iVal = 2 ;		
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)_T("B2")) ;
		strWriteText.Format(_T("%s�п����Ĳɼ����ñ�"), this->m_strStationName) ;
		pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

		CString strFJGJ[3] = {_T("FJ"),_T("GJ"),_T("GJ")} ;
		CString strType[3] = {_T("վ�ڷ���̵���"),_T("վ�ڹ���̵���"),_T("�������̵���")} ;
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
					strColName = mysys.GetExcelFieldName(iCol) ;
					iRow = iCjCurIndex%32+5 ;
					CString strCellName = _T("") ;
					strCellName.Format(_T("%s%d"), strColName, iRow) ;
					strWriteText = strType[i] ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					pRange->PutFormulaR1C1((_variant_t)strWriteText) ;

					//pRange->GetEntireColumn()->AutoFit() ;
					
					strColName = mysys.GetExcelFieldName(iCol+1) ;
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


		//����excel�ļ�
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
		CString sBuff = GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!"));
		
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}


}

BOOL CMaXuBiaoForJH::ConvertSpecialChar(const CString strSrc, CString &strRet)
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

void CMaXuBiaoForJH::MaXuBiaoForJH()
{
	CUserUtility userutility ;
	CString strSavePath = this->cstrDwgSaveDir ;
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
			strBarMsg.Format(_T("�����м�վ�����")) ;
#ifdef VER_FOR_2002
			acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum) ;	
#endif
			int iStationIndex = 0 ;
			while (!cSet.IsEOF())
			{
#ifdef VER_FOR_2002
				acedSetStatusBarProgressMeterPos(iStationIndex) ;
#endif

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

				userutility.ClearTable(_T("signal")) ;
				userutility.ClearTable(_T("guidao")) ;
				CString strSaveSubPath = _T("") ;
				strSaveSubPath.Format(_T("%s\\%s"), strSavePath, strStationName) ;
				::CreateDirectory( strSaveSubPath , NULL ) ;				
				this->cstrDwgSaveDir = strSaveSubPath ;
				
				//���к�����
				for (int i=0; i<2; i++)
				{
					CADORecordset cSetFind2 ; 
					//�ֽ������
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
						strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and right(name,1)<>'��' and type<>11 order by loc_real asc"), iLeftDivideRealLoc, iRightDivideRealLoc, i) ;
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

							
							if (i==0)//�����źŻ������Ƶ�ʾ��Ǳ������Ƶ��
							{
								cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
							}
							else //�����źŵ�����Ĺ����Ƶ��,����ǰһ���źŵ����д�� Ƶ��
							{
								if (iIndex==0)//���е�һ���źŵ㣨�ֽ�㣩������selectʱ��������ҪΪ��ȡ����Ƶ��
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
#ifdef VER_FOR_2002
			acedRestoreStatusBar() ;
#endif
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in MaXuBiaoForJH")) ;		
	}

}

void CMaXuBiaoForJH::ReleaseJLArray()
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

void CMaXuBiaoForJH::DrawBianMaTable(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &cstrArrayGuiDao, CStringArray &cstrArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect, BOOL bIsFanXiangChuZhan)
{
	double dLineWidthThin = 0.0000 ;
	double dLineWidthThick = 0.4000 ;
	double dTextHeight = 3.50000 ;

// 	this->textStyleId = g_cSystem.CreateTextStyleByFileName("zhffont", "txt.shx", "hztxt2.shx", dTextHeight, 0.7) ;	
// 	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName("zhffont_guidao", "txt.shx", "hztxt2.shx", dTextHeight, 0.5) ;

	this->textStyleId = g_cSystem.CreateTextStyleByFileName(_T("zhffont"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightBianMa, this->m_dWidthBianMa) ;	
	this->textStyleId_GuiDao = g_cSystem.CreateTextStyleByFileName(_T("zhffont_guidao"), _T("txt.shx"), _T("hztxt2.shx"), this->m_dHeightGuiDao, this->m_dWidthGuiDao) ;
	g_cSystem.ModifyFontStyle(_T("standard"), _T("hztxt2.shx")) ;
	
	CStringArray cstrArrayLinZhanState ;
	int iLinZhanStateNum = 9 ;

	for (int i=0; i<iLinZhanStateNum; i++)
	{
		cstrArrayLinZhanState.Add(g_cstrLinZhanState_1[i]) ;
	}

	if (bIsFanXiangChuZhan==1)
	{
		iLinZhanStateNum ++ ;
		cstrArrayLinZhanState.InsertAt(0, _T("L5")) ;
	}

	
	//���ģʽ
	if (m_iVersionFor==1)
	{
		iLinZhanStateNum ++ ;
		cstrArrayLinZhanState.InsertAt(0, _T("")) ;
		//ȡ�ý�·���Ƶ��
		CStringArray strArrayTmp ;
		strArrayTmp.Add(_T("")) ;
		//this->GetJinLuGuiDaoFreq(cstrArrayGuiDao, strArrayFreq) ;
		CStringArray * pcstrArrayTmp = new CStringArray() ;
		pcstrArrayTmp->Copy(strArrayTmp) ;
		arrayBianMa.InsertAt(0, pcstrArrayTmp) ;
		cstrArrayDescribe.InsertAt(0, _T("")) ;		
	}
		
	int iRow = arrayBianMa.GetSize() ;	
	int iCol = cstrArrayGuiDao.GetSize() ;
	double dVerLen = iRow*(this->dTableCellHeight) ;
	double dHorLen = iCol*(this->dTableCellWidth) ;

	ads_point base_pt = {0,0,0} ;
	base_pt[0] = 2 ;
	base_pt[1] = this->dTableCur_Y + 10 ;
	AcGePoint3d align_3d(0,0,0) ;
	g_cSystem.ConvertPtAdsTo3D(base_pt, align_3d) ;
	g_cSystem.WriteText(base_pt,align_3d, this->m_cstrCaseType,0,3,AcDb::kTextBase,AcDb::kTextLeft,this->textStyleId,5,0.7,0) ;
	
	//������	
	BOOL bBianMaChange = 0 ;
	for(int iG=0; iG<iCol+1; iG++)
	{
		AcGePoint3d start_3d(0, 0, 0) ;
		AcGePoint3d end_3d(0, 0, 0) ;
		start_3d.x = iG*(this->dTableCellWidth) + this->dTableCur_X;
		start_3d.y = dTableCur_Y + this->dTableCellHeight ;
		end_3d.x = start_3d.x ;
		end_3d.y = -dVerLen + this->dTableCur_Y ;
		//g_cSystem.AddAcDb2dLine(start_3d, end_3d) ;		
		
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
				g_cSystem.WriteMText(base_pt_linzhan, _T("(��վ)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
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
		//���Ҫ��һ�л�Ƶ��
		if (m_iVersionFor==1)
		{
			start_3d_Mid.y = start_3d_Mid.y - dTableCellHeight ;
		}
		end_3d_Mid.x = start_3d_Mid.x ;
		end_3d_Mid.y= end_3d.y ;
		
		ads_point base_pt = {0,0,0} ;
		
		base_pt[0] =  start_3d.x + dTableCellWidth/2  ;
		base_pt[1] =  dTableCur_Y + dTableCellHeight/2 ;
		

		//���һ���Ƿ���ߣ������ڻ����ߺ�д�����
		if (iG==iCol)
		{
			if (iWriteDirect==0)
			{
				g_cSystem.WriteMText(base_pt, _T("(��վ)"), this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			break ;
		}			

		g_cSystem.AddAcDb2dPolyline(start_3d_Mid, end_3d_Mid, dLineWidthThin, 0) ;

		g_cSystem.WriteMText(base_pt, cstrArrayGuiDao.GetAt(iG), this->textStyleId_GuiDao, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		
		//��㣬addƵ��
		if(m_iVersionFor == 1)
		{		
			base_pt[1] = base_pt[1] - dTableCellHeight ;
			g_cSystem.WriteMText(base_pt, m_strArrayCurJinLuFre.GetAt(iG), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
		}
	}

	//������
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
				
				g_cSystem.WriteMText(base_pt_linzhan, cstrArrayLinZhanState[iM] , this->textStyleId	, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth, 0) ;				
	
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
				
				g_cSystem.WriteMText(base_pt_linzhan, cstrArrayLinZhanState[iM], this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth-1, 0) ;				
				
			}
			
		}

		//���ϵĺ��ߣ��ڹ�����Ϸ���ֻ��һ��
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
		//���һ�����Ƿ���ߣ����²������ٽ���
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
		g_cSystem.WriteMText(base_pt, cstrArrayDescribe.GetAt(iM), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dDescribeCellWidth, 0) ;				
		
	}
	
	//�ֳ����������ߺʹ������������������
	int iBianMaMax = arrayBianMa.GetSize()-1 ;
	double dTextWidth = dTableCellWidth/2-1 ;
	if (iWriteDirect==0)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pcstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pcstrArrayTmp->GetSize()-1 ;
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
				g_cSystem.WriteMText(base_pt, pcstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTextWidth, 0) ;				
				
			}
		}
	}
	else if (iWriteDirect==1)
	{
		for(int iarray=iBianMaMax; iarray>=0; iarray--)
		{
			CStringArray *pcstrArrayTmp = arrayBianMa.GetAt(iarray) ;
			int iNumMax = pcstrArrayTmp->GetSize() ;
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
				g_cSystem.WriteMText(base_pt, pcstrArrayTmp->GetAt(ib), this->textStyleId, AcDbMText::kMiddleCenter, dTextHeight, dTableCellWidth/2-1, 0) ;				
				
			}
		}
		
	}

	//begin ��ͼ��
	//////////////////////////////////////////////////////////////////////////
	AcDbBlockReference * pRef = NULL ;
	CString strBlockName ;
	CStringArray strArrayAtt ;
	AcGePoint3d pt_3d_topleft(0,0,0),pt_3d_bottomleft(0,0,0),pt_3d_bottomright(0,0,0), pt_3d_describe(0,0,0) ;
	
	m_dSiYiTuHeight = 50 ;
	//ֻ�ڻ�����һ�ű��ʱ����һ�α߿�x����
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

	strBlockName = _T("frame_3") ;
	pt_3d_describe.x = m_dFrameLeft_x+35 ;
	pt_3d_describe.y = pt_3d_bottomleft.y+5 ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_describe ))
	{
		pRef->close() ;
	}

	//���� 5����˱�ǩ
	strBlockName = _T("frame_5") ;
	strArrayAtt.RemoveAll() ;
	//m_strStationName = _T("����վ") ;
	CString	strStationNameFrame = m_strStationName ;
	if (strStationNameFrame.Find(_T("վ"))==-1 && strStationNameFrame.Find(_T("��·��"))==-1)
	{
		strStationNameFrame.Insert(555, _T("վ")) ;
	}
	strArrayAtt.Add(strStationNameFrame) ;
	CString strDwgName = m_cstrCaseType ;
	strDwgName.TrimLeft(_T("1.")) ;
	strDwgName.TrimLeft(_T("2.")) ;
	strDwgName.Insert(555, _T("�����")) ;
	strArrayAtt.Add(strDwgName) ;
	strArrayAtt.Add(_T("G02-158-1-")) ;
	CString strDate =_T("") ;
	strDate = g_cSystem.GetDateStr() ;
	strArrayAtt.Add(strDate) ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_bottomright, &strArrayAtt, FALSE))
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
//	//��ʱ����Ҫ���������
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
	//end ��ͼ��
	//////////////////////////////////////////////////////////////////////////

	//begin ��ʾ��ͼ
	//////////////////////////////////////////////////////////////////////////
// 	AcGePoint3d pt_3d_mid(0,0,0) ;
// 	pt_3d_mid.x = pt_3d_1.x + (pt_3d_2.x-pt_3d_1.x)/2 ;
// 	pt_3d_mid.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
// 	strBlockName.Format(_T("%s\\syt"), this->cstrDwgSaveDir) ;
// 	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_mid))
// 	{
// 		pRef->close() ;
// 	}


	AcGePoint3d pt_3d_insert(0,0,0) ;
	ads_point base_pt_insert = {0,0,0} ;
	AcGePoint3d pt_3d_start(this->dTableCur_X-(m_iCurJinLuDirect==0?0:dTableCellWidth),pt_3d_1.y - m_dSiYiTuHeight/2,0) ;
	AcGePoint3d pt_3d_end(this->dTableCur_X+dHorLen+(m_iCurJinLuDirect==1?0:dTableCellWidth),pt_3d_1.y - m_dSiYiTuHeight/2,0) ;
	g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.4, 0) ;

	//CString strFrontStation = _T(""), strBackStation = _T("") ;
// 	strFrontStation.Insert(100, _T("(��վ)")) ;
// 	strBackStation.Insert(0, _T("(��վ)")) ;

	//begin ���з����ͷ
	AcGePoint3d pt_3d_arrow(this->dTableCur_X+iCol*dTableCellWidth/2,pt_3d_1.y-10,0) ;
	strBlockName.Format(_T("pmt_arrow_%s"), this->m_iCurJinLuDirect==0?_T("right"):_T("left")) ;
	if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_arrow ))
	{
		pRef->close() ;
	}
	//end ���з����ͷ

	//begin ������վ1�����
	//if (m_iCurJinLuDirect==0)

	int iLzUpOrDown = -1 ;
	int iLzType = 0 ;
	CString strLzName = _T("") ;
	CString strLzGd  = _T("") ;

	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		
		CString strCurLastGd  = _T("") ;
		if (m_iCurJinLuDirect==0)
		{
			strCurLastGd = cstrArrayGuiDao.GetAt(iCol-1) ;
		}
		else
		{
			strCurLastGd = cstrArrayGuiDao.GetAt(0) ;
		}
		
		
		long iCurRealLoc = 0 ;
		int iCurUpOrDown = -1 ;
		
		if (strCurLastGd.Mid(1,3)==_T("1LQ"))
		{
			if (strCurLastGd.Left(4)==_T("X1LQ"))
			{
				strSql.Format(_T("select * from compute_xhj_table where gd_name='%s' and loc_real<( select loc_real from compute_xhj_table where name='%s') order by loc_real desc"), strCurLastGd, this->m_strStationName);
			}
			else
			{
				strSql.Format(_T("select * from compute_xhj_table where gd_name='%s' and loc_real>( select loc_real from compute_xhj_table where name='%s') order by loc_real asc"), strCurLastGd, this->m_strStationName);
			}
		}
		else
		{
			strSql.Format(_T("select * from compute_xhj_table where gd_name='%s' order by loc_real asc"), strCurLastGd) ;
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
				cSetFind.GetFieldValue(_T("loc_real"), iCurRealLoc) ;
				cSetFind.GetFieldValue(_T("up_or_down"), iCurUpOrDown) ;
			}
			cSetFind.Close() ;
		}
		
		if (iCurRealLoc>0)
		{
			if (m_iCurJinLuDirect==0)
			{
				strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down=%d and right(name,1)<>'��' order by loc_real asc "), iCurRealLoc, iCurUpOrDown) ;
			}
			else
			{
				strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down=%d and right(name,1)<>'��' order by loc_real desc "), iCurRealLoc, iCurUpOrDown) ;
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
					cSetFind.GetFieldValue(_T("up_or_down"), iLzUpOrDown) ;
					cSetFind.GetFieldValue(_T("type"), iLzType) ;
					cSetFind.GetFieldValue(_T("name"), strLzName) ;
					cSetFind.GetFieldValue(_T("gd_name"), strLzGd) ;
				}
				cSetFind.Close() ;
			}
		}
		
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ������վ1�����")) ;
		
	}
	
	//end ������վ1�����
	

	for (int iGdIndex=0; iGdIndex<iCol; iGdIndex++)
	{
		CString strGdName = cstrArrayGuiDao.GetAt(iGdIndex) ;
		
		int iUpOrDown = -1 ;
		int iType = 0 ;
		CString strName = _T("") ;

		try
		{
			CString strSql ;
			CADORecordset cSetFind ;

			//CString strGdNameNoG = strGdName.Left(strGdName.GetLength()-1) ;
			if (strGdName.Mid(1,3)==_T("1LQ"))
			{
				if (strGdName.Left(4)==_T("X1LQ"))
				{
					strSql.Format(_T("select * from compute_xhj_table where gd_name='%s' and loc_real<( select loc_real from compute_xhj_table where name='%s') order by loc_real desc"), strGdName, this->m_strStationName);
				}
				else
				{
					strSql.Format(_T("select * from compute_xhj_table where gd_name='%s' and loc_real>( select loc_real from compute_xhj_table where name='%s') order by loc_real asc"), strGdName ,this->m_strStationName);
				}
			}
			else
			{
				strSql.Format(_T("select * from compute_xhj_table where gd_name='%s' order by loc_real asc"), strGdName) ;
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
					cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
					cSetFind.GetFieldValue(_T("type"), iType) ;
					cSetFind.GetFieldValue(_T("name"), strName) ;
				}
				cSetFind.Close() ;
			}
		}
		catch (_com_error& e)
		{
			CString sBuff = GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;	
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in drawmaxubiao")) ;
			
		}
		
		pt_3d_insert.x = this->dTableCur_X+iGdIndex*dTableCellWidth+dTableCellWidth/2 ;
		pt_3d_insert.y = pt_3d_1.y - m_dSiYiTuHeight/2 +(iUpOrDown==0?-2:2) ;
		base_pt_insert[0] = pt_3d_insert.x ;
		base_pt_insert[1] = pt_3d_insert.y ;		
		g_cSystem.WriteText(base_pt_insert,pt_3d_insert, strGdName,0,3,AcDb::kTextBase,AcDb::kTextMid,this->textStyleId,2,0.7,0) ;

		//�ֽ��ʾ
		if (iGdIndex==0||iGdIndex==(iCol-1))
		{
			pt_3d_insert.x = this->dTableCur_X+(iGdIndex>0?iCol*dTableCellWidth:0);
			pt_3d_insert.y = pt_3d_1.y-m_dSiYiTuHeight/2+(iUpOrDown==0?16:-14) ;
			strBlockName.Format(_T("pmt_divide_%d"), iUpOrDown) ;
			if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert ))
			{
				pRef->close() ;
			}

			pt_3d_insert.x = pt_3d_insert.x+(iGdIndex==0?-2:2) ;
			pt_3d_insert.y = pt_3d_insert.y+(iUpOrDown==0?2:-4) ;
			base_pt_insert[0] = pt_3d_insert.x ;
			base_pt_insert[1] = pt_3d_insert.y ;
			//
			CString	strLzStationName = iGdIndex==0?m_strFrontStation:m_strBackStation ;
			if (strLzStationName.Find(_T("վ"))==-1 && strLzStationName.Find(_T("��·��"))==-1)
			{
				strLzStationName.Insert(555, _T("վ")) ;
			}
			g_cSystem.WriteText(base_pt_insert, strLzStationName, 0, iGdIndex==0?AcDb::kTextRight:AcDb::kTextLeft, 2, 0.7) ;

		}
		if (iUpOrDown==0)
		{
			pt_3d_insert.x = this->dTableCur_X+iGdIndex*dTableCellWidth ;
		}
		else
		{
			pt_3d_insert.x = this->dTableCur_X+(iGdIndex+1)*dTableCellWidth ;
		}
		pt_3d_insert.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
		
		strBlockName.Format(_T("maxubiao_xhj_%s%s%s"), iType==0?(iUpOrDown==0?_T("x"):_T("s")):(iUpOrDown==0?_T("s"):_T("x")), iType==3?_T("f"):_T(""), iType==0?(strName.Left(1)==_T("F")?_T("_0"):_T("_1")):_T("") ) ;
		
		strArrayAtt.RemoveAll() ;
		//strName.TrimLeft(_T("F")) ;
		if (strName.Left(1)==_T("F"))
		{
			strName = _T("") ;
		}
		strArrayAtt.Add(strName) ;
		if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		//begin ��վ
		AcGePoint3d pt_3d_lz(0,0,0) ;
		ads_point pt_base_lz = {0,0,0} ;
		if (m_iCurJinLuDirect==0 && iGdIndex==(iCol-1))
		{
			// ����������
			if (iLzUpOrDown==0)
			{
				pt_3d_insert.x = this->dTableCur_X+(iGdIndex+1)*dTableCellWidth ;
				pt_3d_lz.x = pt_3d_insert.x+dTableCellWidth/2 ;
			}
			else
			{
				pt_3d_insert.x = this->dTableCur_X+(iGdIndex+2)*dTableCellWidth ;
				pt_3d_lz.x = pt_3d_insert.x-dTableCellWidth/2 ;
			}
			pt_3d_insert.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
			
			strBlockName.Format(_T("maxubiao_xhj_%s%s%s"), iLzType==0?(iLzUpOrDown==0?_T("x"):_T("s")):(iLzUpOrDown==0?_T("s"):_T("x")), iLzType==3?_T("f"):_T(""), iLzType==0?(strLzName.Left(1)==_T("F")?_T("_0"):_T("_1")):_T("") ) ;
			
			strArrayAtt.RemoveAll() ;
			strLzName.TrimLeft(_T("F")) ;
			strArrayAtt.Add(strLzName) ;
			//strArrayAtt.Add(strLzGd) ;
			if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}

			pt_3d_lz.y = pt_3d_1.y - m_dSiYiTuHeight/2 +(iUpOrDown==0?-2:2) ;
			pt_base_lz[0] = pt_3d_lz.x ;
			pt_base_lz[1] = pt_3d_lz.y ;

			g_cSystem.WriteText(pt_base_lz,pt_3d_lz, strLzGd,0,3,AcDb::kTextBase,AcDb::kTextMid,this->textStyleId,2,0.7,0) ;

		}
		else if (m_iCurJinLuDirect==1 && iGdIndex==0)
		{
			// ����������
			if (iLzUpOrDown==0)
			{
				pt_3d_insert.x = this->dTableCur_X -dTableCellWidth ;
				pt_3d_lz.x = pt_3d_insert.x+dTableCellWidth/2 ;
			}
			else
			{
				pt_3d_insert.x = this->dTableCur_X ;
				pt_3d_lz.x = pt_3d_insert.x-dTableCellWidth/2 ;
			}
			pt_3d_insert.y = pt_3d_1.y - m_dSiYiTuHeight/2 ;
			
			strBlockName.Format(_T("maxubiao_xhj_%s%s%s"), iLzType==0?(iLzUpOrDown==0?_T("x"):_T("s")):(iLzUpOrDown==0?_T("s"):_T("x")), iLzType==3?_T("f"):_T(""), iLzType==0?(strLzName.Left(1)==_T("F")?_T("_0"):_T("_1")):_T("") ) ;
			
			strArrayAtt.RemoveAll() ;
			strLzName.TrimLeft(_T("F")) ;
			strArrayAtt.Add(strLzName) ;
			if( g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}

			pt_3d_lz.y = pt_3d_1.y - m_dSiYiTuHeight/2 +(iUpOrDown==0?-2:2) ;
			pt_base_lz[0] = pt_3d_lz.x ;
			pt_base_lz[1] = pt_3d_lz.y ;
			
			g_cSystem.WriteText(pt_base_lz,pt_3d_lz, strLzGd,0,3,AcDb::kTextBase,AcDb::kTextMid,this->textStyleId,2,0.7,0) ;


		}
		//end ��վ

	}

	//end ��ʾ��ͼ
	//////////////////////////////////////////////////////////////////////////

	//������ǰ��Y����
	this->dTableCur_Y = this->dTableCur_Y - dVerLen - this->dBetweenTable_Y ;
}

void CMaXuBiaoForJH::GetFrontAndBackStation(const CString strCurStation, CString &strFrontStation, CString &strBackStation)
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
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetFrontAndBackStation")) ;
		
	}

}

void CMaXuBiaoForJH::FindDivide(const CString strStation, long &iLeftDivideRealLoc, long &iRightDivideRealLoc, int iUpOrDown)
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
	
		//�ֽ������
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
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FindDivide")) ;
		
	}

}
