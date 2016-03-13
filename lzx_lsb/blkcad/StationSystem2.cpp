#include "stdafx.h"
#include "resource.h"
#include "StationSystem.h"
#include "DButility.h"
#include "YthCircuitData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CStationSystem::ComputeOneTypeCircuit2(const CString strBlockName, const AcGePoint3d pt_3d_ref, AcDbDatabase *pCurDb, AcDbDatabase *&pSaveDb, int &iPxCircuitIndex)
{
	double dPxCircuitHeight = 297 ;
	
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	int iIdAddPlus = 1000 ;

	//��r14�²���Ŀ������ɴ�д����2002���ᣬ����ͳһ������תΪСд
	CString strBlockNameTmp = strBlockName ;
	strBlockNameTmp.MakeLower() ;

	//acutPrintf(_T("in ComputeOneTypeCircuit2: %s"), strBlockNameTmp) ;
	
	//18�ŵ�����������
	if (strBlockNameTmp==_T("dlt_dc_18_d_3j+2x_indoor_tb"))
	{
		double dCellHeight = 25 ;
		double dCellWidth = 18 ;
		int iRowSum = 9 ;
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
		
		for (int iCurRow=0; iCurRow<iRowSum; iCurRow++)
		{
			pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;
			//��Ԫ�����Ͻǡ����½�
			AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
			AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth/2, pt_3d_cur_base.y-dCellHeight, 0) ;
			
			CArray<AcDbText *, AcDbText *> arrayText ;
			mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
			mysys.SortEnt(arrayText, 2) ;
			CStringArray strArrayText ;
			mysys.GetHaveSortedText(arrayText, strArrayText) ;
			mysys.DestroyEntInArray(arrayText) ;
			
			//��������
			if (strArrayText.GetSize()>0)
			{
				CMapStringToString mapCircuitTypeToStr ;
				
				CString strSwitchName = strArrayText.GetAt(0) ;
				mapCircuitTypeToStr.SetAt(_T("27"), strSwitchName) ;
				
				pt_3d_topleft.x=pt_3d_cur_base.x+dCellWidth ;
				pt_3d_bottomright.x=pt_3d_topleft.x+dCellWidth ;
				mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
				mysys.SortEnt(arrayText, 2) ;
				strArrayText.RemoveAll() ;
				mysys.GetHaveSortedText(arrayText, strArrayText) ;
				mysys.DestroyEntInArray(arrayText) ;
				if (strArrayText.GetSize()>0)
				{
					CString strTDZZLocTmp = strArrayText.GetAt(0) ;
					if (CBlkUtility::IsStrMatch(_T("^(Z[1-9]\\d*-[1-9]\\d*)$"), strTDZZLocTmp))
					{
						mapCircuitTypeToStr.SetAt(_T("81"), strTDZZLocTmp) ;				
						
						CString strTypeNum[5] = {_T("105"),_T("106"),_T("107"),_T("108"),_T("109")} ; 
						
						//TDF
						pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*2;
						pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
						for (int i=0; i<5; i++)
						{						  
							pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight*i/5 ;
							pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight/5 ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
							mysys.SortEnt(arrayText, 2) ;
							strArrayText.RemoveAll() ;
							mysys.GetHaveSortedText(arrayText, strArrayText) ;
							mysys.DestroyEntInArray(arrayText) ;
							CString strTDFZLoc = _T("") ;
							if (strArrayText.GetSize()>0)
							{
								strTDFZLoc = strArrayText.GetAt(0) ;
							}
							mapCircuitTypeToStr.SetAt(strTypeNum[i], strTDFZLoc) ;				 
						}
						
						//BHZ
						pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*3;
						pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
						pt_3d_topleft.y = pt_3d_cur_base.y ;
						pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight ;
						mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
						mysys.SortEnt(arrayText, 2) ;
						strArrayText.RemoveAll() ;
						mysys.GetHaveSortedText(arrayText, strArrayText) ;
						mysys.DestroyEntInArray(arrayText) ;
						CString strBHZZLoc = _T("") ;
						if (strArrayText.GetSize()>0)
						{
							strBHZZLoc = strArrayText.GetAt(0) ;
						}
						mapCircuitTypeToStr.SetAt(_T("133"), strBHZZLoc) ;
						
						//GJ-Z GJ-Q
						CString strTypeNum2[2] = {_T("159"),_T("185")} ; 
						for (int i2=0; i2<2; i2++)
						{
							pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(4+i2);
							pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
							mysys.SortEnt(arrayText, 2) ;
							strArrayText.RemoveAll() ;
							mysys.GetHaveSortedText(arrayText, strArrayText) ;
							mysys.DestroyEntInArray(arrayText) ;
							CString strGJLoc = _T("") ;
							if (strArrayText.GetSize()>0)
							{
								strGJLoc = strArrayText.GetAt(0) ;
							}
							mapCircuitTypeToStr.SetAt(strTypeNum2[i2], strGJLoc) ;
						}	
						
						//���������õ�·ͼ
						CString strCircuitTemplete ;
						strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\DC_18_D_3J+2X_INDOOR.dwg"), m_strSysBasePath) ;
						AcGeMatrix3d xform;  
						AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
						xform.setToTranslation(vec); 					
						this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
						
						iPxCircuitIndex++ ;						
					}
					else
					{
						continue ;
					}
				}
			}
		}
	  
	}
	//��������
	else if (strBlockNameTmp==_T("dlt_dc_18_d_3j+2x_outdoor_tb1")||strBlockNameTmp==_T("dlt_dc_18_d_3j+2x_outdoor_tb2"))
	{
		double dCellHeight = 5 ;
		double dCellWidth = 18 ;
		int iRowNumInfact = 0 ;
		int iRowNum = 45 ;
		int iRowNum2 = 40 ;
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
		if (strBlockName==_T("dlt_dc_18_d_3J+2X_outdoor_tb1"))
		{
			iRowNumInfact = iRowNum ;
		}
		else if(strBlockName==_T("dlt_dc_18_d_3J+2X_outdoor_tb2"))
		{
			iRowNumInfact = iRowNum2;
		}
		for (int iCurRow=0; iCurRow<iRowNumInfact; iCurRow++)
		{
			pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;
			
			//��Ԫ�����Ͻǡ����½�
			AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
			AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
			
			CMapStringToString mapCircuitTypeToStr ;
			
			pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth ;
			pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;		  
			CArray<AcDbText *, AcDbText *> arrayText ;
			mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
			mysys.SortEnt(arrayText, 2) ;
			CStringArray strArrayText ;
			mysys.GetHaveSortedText(arrayText, strArrayText) ;
			mysys.DestroyEntInArray(arrayText) ;
			CString strTDFLoc = _T("") ;
			if (strArrayText.GetSize()>0)
			{
				strTDFLoc = strArrayText.GetAt(0) ;
				mapCircuitTypeToStr.SetAt(_T("79"), strTDFLoc) ;
				
				CString strTypeNum[5] = {_T("105"),_T("131"),_T("157"),_T("183"),_T("209")} ; 
				
				for (int iCol=0; iCol<5; iCol++)
				{
					pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(iCol+2) ;
					pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
					
					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					mysys.SortEnt(arrayText, 2) ;
					strArrayText.RemoveAll() ;
					mysys.GetHaveSortedText(arrayText, strArrayText) ;
					mysys.DestroyEntInArray(arrayText) ;
					CString strText = _T("") ;
					if (strArrayText.GetSize()>0)
					{
						strText = strArrayText.GetAt(0) ;
					}
					mapCircuitTypeToStr.SetAt(strTypeNum[iCol], strText) ;	  
				} 
				//���������õ�·ͼ
				CString strCircuitTemplete ;
				strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\DC_ALL-OUTDOOR.dwg"), m_strSysBasePath) ;
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				xform.setToTranslation(vec); 					
				this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				
				iPxCircuitIndex++ ;				
			}
		}
	}
	//18�ŵ�����������
	else if (strBlockNameTmp==_T("dlt_dc_18_d_3j+2x_qc_tb"))
	{
		double dCellHeight = 15 ;
		double dCellWidth = 19 ;
		int iRowSum = 14 ;
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;

		for (int iCurRow=0; iCurRow<iRowSum; iCurRow++)
		{
			pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;
			//��Ԫ�����Ͻǡ����½�
			AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
			AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
			
			CArray<AcDbText *, AcDbText *> arrayText ;
			mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
			mysys.SortEnt(arrayText, 2) ;
			CStringArray strArrayText ;
			mysys.GetHaveSortedText(arrayText, strArrayText) ;
			mysys.DestroyEntInArray(arrayText) ;
			
			//��������
			if (strArrayText.GetSize()>0)
			{
				CMapStringToString mapCircuitTypeToStr ;
				
				CString strSwitchName = strArrayText.GetAt(0) ;
				mapCircuitTypeToStr.SetAt(_T("28"), strSwitchName) ;
				
				CString strTypeZLoc[2] = {_T("53"), _T("54")} ;
				pt_3d_topleft.x+=dCellWidth ;
				pt_3d_bottomright.x+=dCellWidth ;
				for (int iZIndex=0; iZIndex<2; iZIndex++)
				{
					pt_3d_topleft.y = pt_3d_cur_base.y-iZIndex*dCellHeight/3 ;
					pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight/3 ;

					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					mysys.SortEnt(arrayText, 2) ;
					strArrayText.RemoveAll() ;
					mysys.GetHaveSortedText(arrayText, strArrayText) ;
					mysys.DestroyEntInArray(arrayText) ;
					CString strZLoc = _T("") ;
					if (strArrayText.GetSize()>0)
					{
						strZLoc = strArrayText.GetAt(0) ;
					}
					mapCircuitTypeToStr.SetAt(strTypeZLoc[iZIndex], strZLoc) ;
				}

				CString strTypeJKLoc[10] = {_T("81"), _T("107"), _T("133"), _T("159"), _T("185"), _T("211"), _T("237"), _T("263"), _T("289"), _T("315")} ;
				pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight*2/3 ;
				pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight/3 ;
				for (int iJkIndex=0; iJkIndex<10; iJkIndex++)
				{
					pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iJkIndex) ;
					pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;

					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					mysys.SortEnt(arrayText, 2) ;
					strArrayText.RemoveAll() ;
					mysys.GetHaveSortedText(arrayText, strArrayText) ;
					mysys.DestroyEntInArray(arrayText) ;
					CString strJKLoc = _T("") ;
					if (strArrayText.GetSize()>0)
					{
						strJKLoc = strArrayText.GetAt(0) ;
					}
					mapCircuitTypeToStr.SetAt(strTypeJKLoc[iJkIndex], strJKLoc) ;
				}			
				
				//���������õ�·ͼ
				CString strCircuitTemplete ;
				strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\DC_18_D_3J+2X_DRIVE.dwg"), m_strSysBasePath) ;
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				xform.setToTranslation(vec); 					
				this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				
				iPxCircuitIndex++ ;	
				
			}
		}  
	  
	}
	//18��˫����������
	else if (strBlockNameTmp==_T("dlt_dc_18_s_3j+2x_indoor_tb"))
	{
		double dCellHeight = 25 ;
		double dCellWidth = 18 ;
		int iRowSum = 9 ;
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
		
		for (int iCurRow=0; iCurRow<iRowSum; iCurRow++)
		{
			pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;

			CMapStringToString mapCircuitTypeToStr ;

			CString strTypeSwitchName[2] = {_T("29"), _T("211")} ;
			CString strTypeNum[2][5] = {
				{_T("105"),_T("106"),_T("107"),_T("108"),_T("109")},
				{_T("261"),_T("262"),_T("263"),_T("264"),_T("265")}
			}; 
			CString strTypeBHZ[2] = {_T("133"), _T("289")} ;
			CString strTypeNum2[2][2] = {
				{_T("159"),_T("185")},
				{_T("315"),_T("341")}
			} ; 

			CString strSwitchName = _T("") ;

			for (int iSwitchIndex=0; iSwitchIndex<2; iSwitchIndex++)
			{				
				//��Ԫ�����Ͻǡ����½�
				AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x+iSwitchIndex*dCellWidth*6, pt_3d_cur_base.y, 0) ;
				AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+iSwitchIndex*dCellWidth*6+dCellWidth/2, pt_3d_cur_base.y-dCellHeight, 0) ;
				
				CArray<AcDbText *, AcDbText *> arrayText ;
				mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
				mysys.SortEnt(arrayText, 2) ;
				CStringArray strArrayText ;
				mysys.GetHaveSortedText(arrayText, strArrayText) ;
				mysys.DestroyEntInArray(arrayText) ;
				
				//��������
				if (strArrayText.GetSize()>0)
				{					
					strSwitchName = strArrayText.GetAt(0) ;
					mapCircuitTypeToStr.SetAt(strTypeSwitchName[iSwitchIndex], strSwitchName) ;
					
					pt_3d_topleft.x=pt_3d_cur_base.x+iSwitchIndex*dCellWidth*6+dCellWidth ;
					pt_3d_bottomright.x=pt_3d_topleft.x+dCellWidth ;
					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					mysys.SortEnt(arrayText, 2) ;
					strArrayText.RemoveAll() ;
					mysys.GetHaveSortedText(arrayText, strArrayText) ;
					mysys.DestroyEntInArray(arrayText) ;
					if (strArrayText.GetSize()>0)
					{
						CString strTDZZLocTmp = strArrayText.GetAt(0) ;
						if (CBlkUtility::IsStrMatch(_T("^(Z[1-9]\\d*-[1-9]\\d*)$"), strTDZZLocTmp))
						{
							if (iSwitchIndex==0)
							{
								mapCircuitTypeToStr.SetAt(_T("81"), strTDZZLocTmp) ;
							}							
							//TDF
							pt_3d_topleft.x = pt_3d_cur_base.x+iSwitchIndex*dCellWidth*6+dCellWidth*2;
							pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
							for (int i=0; i<5; i++)
							{						  
								pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight*i/5 ;
								pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight/5 ;
								mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
								mysys.SortEnt(arrayText, 2) ;
								strArrayText.RemoveAll() ;
								mysys.GetHaveSortedText(arrayText, strArrayText) ;
								mysys.DestroyEntInArray(arrayText) ;
								CString strTDFZLoc = _T("") ;
								if (strArrayText.GetSize()>0)
								{
									strTDFZLoc = strArrayText.GetAt(0) ;
								}
								mapCircuitTypeToStr.SetAt(strTypeNum[iSwitchIndex][i], strTDFZLoc) ;				 
							}
							
							//BHZ
							pt_3d_topleft.x = pt_3d_cur_base.x+iSwitchIndex*dCellWidth*6+dCellWidth*3;
							pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
							pt_3d_topleft.y = pt_3d_cur_base.y ;
							pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
							mysys.SortEnt(arrayText, 2) ;
							strArrayText.RemoveAll() ;
							mysys.GetHaveSortedText(arrayText, strArrayText) ;
							mysys.DestroyEntInArray(arrayText) ;
							CString strBHZZLoc = _T("") ;
							if (strArrayText.GetSize()>0)
							{
								strBHZZLoc = strArrayText.GetAt(0) ;
							}
							mapCircuitTypeToStr.SetAt(strTypeBHZ[iSwitchIndex], strBHZZLoc) ;
							
							//GJ-Z GJ-Q
							for (int i2=0; i2<2; i2++)
							{
								pt_3d_topleft.x = pt_3d_cur_base.x+iSwitchIndex*dCellWidth*6+dCellWidth*(4+i2);
								pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
								mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
								mysys.SortEnt(arrayText, 2) ;
								strArrayText.RemoveAll() ;
								mysys.GetHaveSortedText(arrayText, strArrayText) ;
								mysys.DestroyEntInArray(arrayText) ;
								CString strGJLoc = _T("") ;
								if (strArrayText.GetSize()>0)
								{
									strGJLoc = strArrayText.GetAt(0) ;
								}
								mapCircuitTypeToStr.SetAt(strTypeNum2[iSwitchIndex][i2], strGJLoc) ;
							}	
							
						}
		
					}
				}
				
			}
			if (strSwitchName==_T(""))
			{
				continue ;
			}
			
			//���������õ�·ͼ
			CString strCircuitTemplete ;
			strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\DC_18_S_3J+2X_INDOOR.dwg"), m_strSysBasePath) ;
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight*2, 0); 
			xform.setToTranslation(vec); 					
			this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
			
			iPxCircuitIndex++ ;	
		}		
	}
	//18��˫����������
	else if (strBlockNameTmp==_T("dlt_dc_18_s_3j+2x_qc_tb"))
	{
		double dCellHeight = 20 ;
		double dCellWidth = 19 ;
		int iRowSum = 11 ;
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
		
		for (int iCurRow=0; iCurRow<iRowSum; iCurRow++)
		{
			pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;
			//��Ԫ�����Ͻǡ����½�
			AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
			AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
			
			CArray<AcDbText *, AcDbText *> arrayText ;
			mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
			mysys.SortEnt(arrayText, 2) ;
			CStringArray strArrayText ;
			mysys.GetHaveSortedText(arrayText, strArrayText) ;
			mysys.DestroyEntInArray(arrayText) ;
			
			//��������
			if (strArrayText.GetSize()>0)
			{
				CMapStringToString mapCircuitTypeToStr ;
				
				CString strSwitchName = strArrayText.GetAt(0) ;
				mapCircuitTypeToStr.SetAt(_T("28"), strSwitchName) ;
				
				CString strTypeZLoc[3] = {_T("53"), _T("54"), _T("55")} ;
				pt_3d_topleft.x+=dCellWidth ;
				pt_3d_bottomright.x+=dCellWidth ;
				for (int iZIndex=0; iZIndex<3; iZIndex++)
				{
					pt_3d_topleft.y = pt_3d_cur_base.y-iZIndex*dCellHeight/4 ;
					pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight/4 ;
					
					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					mysys.SortEnt(arrayText, 2) ;
					strArrayText.RemoveAll() ;
					mysys.GetHaveSortedText(arrayText, strArrayText) ;
					mysys.DestroyEntInArray(arrayText) ;
					CString strZLoc = _T("") ;
					if (strArrayText.GetSize()>0)
					{
						strZLoc = strArrayText.GetAt(0) ;
					}
					mapCircuitTypeToStr.SetAt(strTypeZLoc[iZIndex], strZLoc) ;
				}
				
				CString strTypeJKLoc[12] = {_T("82"), _T("108"), _T("134"), _T("160"), _T("186"), _T("212"), _T("238"), _T("264"), _T("290"), _T("316"), _T("342"), _T("368")} ;
				pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight*3/4 ;
				pt_3d_bottomright.y = pt_3d_topleft.y-dCellHeight/4 ;
				for (int iJkIndex=0; iJkIndex<12; iJkIndex++)
				{
					pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iJkIndex) ;
					pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
					
					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					mysys.SortEnt(arrayText, 2) ;
					strArrayText.RemoveAll() ;
					mysys.GetHaveSortedText(arrayText, strArrayText) ;
					mysys.DestroyEntInArray(arrayText) ;

					CString strJKLoc = _T("") ;
					if (strArrayText.GetSize()>0)
					{
						strJKLoc = strArrayText.GetAt(0) ;
					}
					mapCircuitTypeToStr.SetAt(strTypeJKLoc[iJkIndex], strJKLoc) ;
				}			
				
				//���������õ�·ͼ
				CString strCircuitTemplete ;
				strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\DC_18_S_3J+2X_DRIVE.dwg"),m_strSysBasePath) ;
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				xform.setToTranslation(vec); 					
				this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				
				iPxCircuitIndex++ ;	
				
			}
		}  
		
	}

	
	return TRUE ;	
	
}

// ԭReplaceSpecialChar�����ǽ�I��V���滻Ϊ%%161��%%165���ַ�֮
int CStationSystem::ReplaceSpecialChar2(void)
{
	int iProcSum = 0 ;

	try
	{
		CADORecordset cSetAll ;
		CString strSql ;

		CUserSystem mysys ;

		//�����ɼ�����
		CString strTbName[2] = {_T("interlock_outin_place"), _T("traincontrol_outin_place")} ;
		for (int i=0; i<2; i++)
		{
			iProcSum = 0 ;
			strSql.Format(_T("select * from %s"), strTbName[i]) ;
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if(cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}
				while (!cSetAll.IsEOF())
				{
					CString strDevName = _T("") ;

					CString strDevNameNew = _T("") ;
					cSetAll.GetFieldValue(_T("dev_name"), strDevName) ;
					
					this->ConvertSpecialChar(strDevName, strDevNameNew, 1) ;

					if (strDevNameNew!=strDevName&&strDevNameNew!=_T(""))
					{
						cSetAll.Edit() ;
						cSetAll.SetFieldValue(_T("dev_name"), strDevNameNew) ;
						cSetAll.Update() ;
						acutPrintf(_T("\n%s-%s"), strDevName, strDevNameNew) ;
						iProcSum++ ;
					}


					cSetAll.MoveNext() ;
				}
				cSetAll.Close() ;

				acutPrintf(_T("\n%sת���ַ�����:%d"), strTbName[i], iProcSum) ;
			}

		}

		//��Ƶ���ۺϹ񡢹���񡢹����ϲ���
		CString strTbName2[4] = {_T("z_gd_place"), _T("y_place"),_T("zh_place"),_T("g_place")} ;
		for(int i2=0; i2<4; i2++)
		{
			iProcSum = 0 ;
			strSql.Format(_T("select * from %s"), strTbName2[i2]) ;
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if(cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}

				while (!cSetAll.IsEOF())
				{
					CString strDevName = _T("") ;				
					CString strDevNameNew = _T("") ;
					cSetAll.GetFieldValue(_T("gd_name"), strDevName) ;
                    
					this->ConvertSpecialChar(strDevName, strDevNameNew, 1) ;

					if (strDevNameNew!=strDevName&&strDevNameNew!=_T(""))
					{
						cSetAll.Edit() ;
						cSetAll.SetFieldValue(_T("gd_name"), strDevNameNew) ;
						cSetAll.Update() ;
						acutPrintf(_T("\n%s-%s"), strDevName, strDevNameNew) ;
						iProcSum++ ;
					}

					cSetAll.MoveNext() ;
				}
				cSetAll.Close() ;

				acutPrintf(_T("\n%sת���ַ�����:%d"), strTbName2[i2], iProcSum) ;
			}
		}

		//��Ϲ�
		CString strTbName3[2] = {_T("z_place"), _T("fxp_place")} ;
		for(int i3=0; i3<2; i3++)
		{
			iProcSum = 0 ;
			if (i3==0)
			{
				strSql.Format(_T("select * from %s where type in('JZZ','JLZ','CZZ','DXZ','USUZ')"), strTbName3[i3]) ;
			}
			else
			{
				strSql.Format(_T("select * from %s where zuhe_type in(1,2,3,4)"), strTbName3[i3]) ;
			}
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if(cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}

				while (!cSetAll.IsEOF())
				{
					CString strDevName = _T("") ;				
					CString strDevNameNew = _T("") ;
					cSetAll.GetFieldValue(_T("dev_name"), strDevName) ;

					this->ConvertSpecialChar(strDevName, strDevNameNew, 1) ;

					if (strDevNameNew!=strDevName&&strDevNameNew!=_T(""))
					{
						cSetAll.Edit() ;
						cSetAll.SetFieldValue(_T("dev_name"), strDevNameNew) ;
						cSetAll.Update() ;
						acutPrintf(_T("\n%s-%s"), strDevName, strDevNameNew) ;
						iProcSum++ ;
					}

					cSetAll.MoveNext() ;
				}
				cSetAll.Close() ;

				acutPrintf(_T("\n%sת���ַ�����:%d"), strTbName3[i3], iProcSum) ;
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
		AfxMessageBox(_T("unknown error in ConvertSpecialChar")) ;
	} 

	return 0 ;
}

// ����վ����Ϲ���Ƶ���ۺϹ񡢹���񡢷�����
void CStationSystem::ReadBzt(void)
{


}
