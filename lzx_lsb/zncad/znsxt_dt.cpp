#include "znsxt.h"

//取得地铁各设备的公里标
int CZnSxt::GetDistToXhl_DT(const AcGePoint3d& pt_3d_toxhlheader1, const AcGePoint3d& pt_3d_toxhlheader2, vector<vector<vector<CString> > >& vecSection)
{
	struct resbuf* pRb ;

	//将距离栏的所有文本都设为中间对齐（不移动其位置）
	for (int n=0; n<2; n++)
	{
		ads_point pt1 = {n==0?pt_3d_toxhlheader1.x:pt_3d_toxhlheader2.x, n==0?pt_3d_toxhlheader1.y:pt_3d_toxhlheader2.y-35, 0} ;
		ads_point pt2 = {n==0?pt_3d_toxhlheader1.x+m_dMaxLenPmt:pt_3d_toxhlheader2.x+m_dMaxLenPmt, n==0?pt_3d_toxhlheader1.y+35:pt_3d_toxhlheader2.y, 0} ;
		pRb = acutBuildList(RTDXF0, _T("TEXT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
		CLongArray nArrayHandleDist;
		int nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandleDist) ;
		acutRelRb(pRb) ;
		for (int i=0; i<nLen; i++)
		{
			AcDbEntity* pEnt = NULL ;
			if (m_mysys.OpenAcDbEntity(nArrayHandleDist.GetAt(i), pEnt))
			{
				AcDbText* pText = AcDbText::cast(pEnt) ;
				m_mysys.ConvertTextMode(pText, AcDb::kTextMid) ;
				pEnt->close() ;
			}
		}
	}

	CMapStringToString mapBlkNameToType ;
	CString strBlkNames[11] = {_T("DT_AP_000"), _T("DT_AP_100"), _T("DT_AP_200"), _T("DT_DHX_000"), _T("DT_JZ_000"), _T("DT_JZ_100"), _T("DT_JZ_200"), _T("DT_RFM_000"), _T("DT_YDQ_000"), _T("DT_YDQ_100"), _T("DT_YDQ_200")} ;
	CString strTypes[11] = {_T("无线-波导管"),  _T("无线-天线"),  _T("无线-漏缆"),  _T("短环线"),  _T("计轴"),  _T("计轴-分界处"),  _T("计轴-超限"),  _T("人防门"),  _T("应答器-可变"),  _T("应答器-固定"),  _T("应答器-预告")} ;
	for (int i=0; i<11; i++)
	{
		mapBlkNameToType.SetAt(strBlkNames[i], strTypes[i]) ;
	}
	//搜索出所有车站标，以此将整条线分成多段，每区段的设备（信号机）不会重名？
	long nNumStation = 0 ;
	CLongArray nArrayHandleStation;
	ads_point pt1 = {pt_3d_toxhlheader2.x, pt_3d_toxhlheader2.y, 0} ;
	ads_point pt2 = {pt_3d_toxhlheader1.x+m_dMaxLenPmt, pt_3d_toxhlheader1.y, 0} ; 
	pRb = acutBuildList(RTDXF0, _T("INSERT"), 2, _T("PMT_STATION"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
	nNumStation = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandleStation) ;
	acutRelRb(pRb) ;

	//begin 各区段x
	m_mysys.SortEnt(nArrayHandleStation,1, 0) ;
	CDoubleArray dArrayX ;
	dArrayX.Add(pt_3d_toxhlheader1.x) ;
	for(int iIndexStation=0; iIndexStation<nNumStation; iIndexStation++)
	{
		AcGePoint3d pt_3d_insert ;
		m_mysys.GetEntPosition(nArrayHandleStation.GetAt(iIndexStation), pt_3d_insert) ;
		dArrayX.Add(pt_3d_insert.x) ;
	}
	dArrayX.Add(pt_3d_toxhlheader1.x+m_dMaxLenPmt) ;
	//end 各区段x

	for(int iIndexSection=0; iIndexSection<(dArrayX.GetCount()-1); iIndexSection++)
	{
		//先直接在距离栏搜索一遍信号机/道岔名称并搜索其匹配的距离
		double dWidthOfTextExt = 3 ; //文本区域所占的宽（此时文本旋转了90度，故此值应等于文本高度）
		long nLen = 0 ;
		CLongArray nArrayHandle;
		CMapStringToString mapToXhl1 ;//距离信号楼距离
		for (int n=0; n<2; n++) //上下2个栏
		{
			//所有在距离栏的信号机/道岔名称
			nLen = 0 ;
			nArrayHandle.RemoveAll() ;

			//n==0 上栏
			pt1[0] = dArrayX.GetAt(iIndexSection) ;
			pt1[1] = (n==0?(pt_3d_toxhlheader1.y):(pt_3d_toxhlheader2.y-13));
			pt2[0] = dArrayX.GetAt(iIndexSection+1) ;
			pt2[1] = (n==0?(pt_3d_toxhlheader1.y+13):(pt_3d_toxhlheader2.y)) ;

			pRb = acutBuildList(RTDXF0, _T("TEXT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
			nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
			acutRelRb(pRb) ;
			acutPrintf(_T("\nall name in header:%d"), nLen) ;

			for (int j=0; j<nLen; j++)
			{
				AcDbEntity * pEnt = NULL ;
				if(m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(j), pEnt, AcDb::kForRead))
				{
					if (pEnt->isKindOf(AcDbText::desc()))
					{
						AcDbText* pText = AcDbText::cast(pEnt) ;
						CString strName = pText->textString() ;
						strName.Trim() ;
						acutPrintf(_T("\n第1次搜索:%s："), strName) ;

						//找出位置和对起点均在指定范围的文本（距离）
						AcDbExtents ext ;
						pText->getGeomExtents(ext) ;
						AcGePoint3d pt_3d_min = ext.minPoint() ;
						AcGePoint3d pt_3d_max = ext.maxPoint() ;
						AcGePoint3d pt_3d_mid = m_mysys.GetMidPoint(pt_3d_min, pt_3d_max) ;
						dWidthOfTextExt = abs(pt_3d_max.x-pt_3d_min.x) ;

						pt1[0] = pt_3d_min.x-dWidthOfTextExt/8 ;
						pt1[1] =(n==0?(pt_3d_toxhlheader1.y+13):(pt_3d_toxhlheader2.y-35));
						pt2[0] = pt_3d_max.x+dWidthOfTextExt/8 ;
						pt2[1] = (n==0?(pt_3d_toxhlheader1.y+35):(pt_3d_toxhlheader2.y-13)) ;
						pRb = acutBuildList(RTDXF0, _T("TEXT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, -4,  _T(">,>,*"), 11,  pt1, -4,  _T("<,<,*"), 11,  pt2, 0) ;
						CLongArray nArrayHandleVal ;
						long nLenVal = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandleVal) ;
						if (nLenVal==1)
						{
							CString strVal ;
							m_mysys.GetTextString(nArrayHandleVal.GetAt(0), strVal) ;
							strVal.Trim() ;
							if (!strVal.IsEmpty())
							{
								strVal.AppendFormat(_T("__%s"), n==0?_T("上"):_T("下")) ; //附带记录该公里标写在上面还是下面
								acutPrintf(_T("%s："), strVal) ;
								mapToXhl1.SetAt(strName, strVal) ;
							}
						}
						acutRelRb(pRb) ;
					}
					pEnt->close() ;
				}
			}
		}

		//在上下两栏之间的实体集合
		nLen = 0 ;
		nArrayHandle.RemoveAll() ;
		pt1[0] = dArrayX.GetAt(iIndexSection) ; 
		pt1[1] = pt_3d_toxhlheader2.y ;
		pt2[0] = dArrayX.GetAt(iIndexSection+1) ; 
		pt2[1] = pt_3d_toxhlheader1.y ;
		pRb = acutBuildList(RTDXF0, _T("INSERT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
		nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
		acutRelRb(pRb) ;
		//按x坐标增大顺序排序
		m_mysys.SortEnt(nArrayHandle, 1) ;
		vector<vector<CString > > vecToXhl ;
		for(int i=0; i<nLen; i++)
		{
			UINT nHandle =nArrayHandle.GetAt(i) ;
			AcDbEntity * pEnt = NULL ;
			if (m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt, AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
					AcGePoint3d pt_3d_base = pRef->position() ;
					CString strBlkName ;
					m_mysys.GetBlockName(pRef, strBlkName) ;
					CString strDevName ; //信号机名称或道岔编号
					CString strType ;

					if (strBlkName.Left(3)==_T("XHJ"))
					{
						m_mysys.GetAttValue(pRef, _T("XHJNAME"), strDevName) ;
						strType = _T("信号机") ;
					}
					else if (strBlkName.Left(2)==_T("DC"))
					{
						m_mysys.GetAttValue(pRef, _T("BIANHAO"), strDevName) ;
						strType = _T("道岔") ;
					}
					else 
					{
						m_mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
						strType = _T("其他") ;

						mapBlkNameToType.Lookup(strBlkName, strType) ;
					}

					if (!strDevName.IsEmpty())
					{
						CString strDist;	
						mapToXhl1.Lookup(strDevName, strDist) ;	

						vector<CString > vec ;
						vec.push_back(strType) ;
						vec.push_back(strDevName) ;
						//strDist.AppendFormat(_T("__%s"), n==0?_T("上"):_T("下")) ;
						vec.push_back(strDist) ;

						vecToXhl.push_back(vec) ;
						acutPrintf(_T("\n第2次搜索:%s：%s"), strDevName, strDist) ;
					}
				}

				pEnt->close() ;
			}
		}

		vecSection.push_back(vecToXhl) ;
	}

	return 0;
}

void CZnSxt::ReadAllEntToDB2_DT(void)
{
	CString strMsg ;

	acutPrintf(_T("\n读取平面图，注意此函数默认搜索公里标范围为%.0f"), m_dMaxLenPmt) ;
	//找到距离信号楼距离框的图块 
	AcGePoint3d pt_3d_toxhlheader1, pt_3d_toxhlheader2 ;//距信号楼距离图块基点
	if(this->GetToXhlHeaderPos(pt_3d_toxhlheader1, pt_3d_toxhlheader2)!=13)
	{
		AfxMessageBox(_T("未发现2个地铁公里标栏图块 ，程序退出，请处理后再继续!")) ;
		return ;
	}

	m_AdoDb.ClearTable(_T("sxt_ent")) ;
	m_AdoDb.ClearTable(_T("sxt_line")) ;

	int nLen = 0 ;
	CLongArray nArrayHandle ;
	AcGePoint3d pt_3d_1(pt_3d_toxhlheader2.x-50, pt_3d_toxhlheader2.y, 0), pt_3d_2(pt_3d_toxhlheader1.x+m_dMaxLenPmt, pt_3d_toxhlheader1.y, 0) ;
	nLen = m_mysys.FindEntInArea(pt_3d_1, pt_3d_2, nArrayHandle, 1) ;

	CString strSql ;
	for(int i=0; i<nLen; i++)
	{
		UINT nHandle =nArrayHandle.GetAt(i) ;
		AcDbEntity * pEnt = NULL ;
		if (m_mysys.OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
				AcGePoint3d pt_3d_position = pRef->position() ;
				//将块统一为X比例和旋转角变化（X、Y都置-1，等同于旋转180度，X置-1并且旋转180度等同于Y置-1，Y置-1并且旋转180度等同于X置-1）
				AcGeScale3d scale_3d = pRef->scaleFactors()  ;		
				if (scale_3d.sy<0)
				{
					pRef->upgradeOpen() ;
					scale_3d.set(-scale_3d.sx, -scale_3d.sy, scale_3d.sz) ;
					pRef->setScaleFactors(scale_3d) ;
					pRef->setRotation(pRef->rotation()-PI) ;
					pRef->downgradeOpen() ;
				}	

				CString strName ;
				CString strBlkName ;
				m_mysys.GetBlockName(pRef, strBlkName) ;

				CStringArray strArrayBlkName ;
				CBlkUtility::DivideString(strBlkName, _T("_"), strArrayBlkName) ;
				CString strBlkType ;
				if (strArrayBlkName.GetCount()>0)
				{
					strBlkType = strArrayBlkName.GetAt(0) ;
				}

				if (strBlkType==_T("DC"))
				{
					m_mysys.GetAttValue(pRef, _T("BIANHAO"), strName) ;
					//和DC块定/反位连接的线信息
					CLongArray nArrayHandleIntersectLine, nArrayIndexOfVertex ;
					int iDwIntersectMoveToVertex = -1 ;
					AcGePoint3dArray pt3dArrayIntersect ;
					if(this->GetDcConLineDFW(pRef, nArrayHandleIntersectLine, nArrayIndexOfVertex, pt3dArrayIntersect, iDwIntersectMoveToVertex) )
					{
						strSql.Format(_T("insert into sxt_ent(sxt_ent_handle,  sxt_ent_block, sxt_ent_x, sxt_ent_y, sxt_ent_movetovertex, sxt_ent_name, sxt_ent_type,  sxt_ent_online_handle,  sxt_ent_online_vertex,  sxt_ent_online_intersectx,  sxt_ent_online_intersecty,  sxt_ent_fwline_handle,  sxt_ent_fwline_vertex,  sxt_ent_x1, sxt_ent_y1, sxt_ent_online_intersectx1,  sxt_ent_online_intersecty1)  \
										 values(%d, '%s', %.10f, %.10f, %d, '%s', %d, %d, %d, %.10f, %.10f, %d, %d, %.10f, %.10f, %.10f, %.10f) "), nHandle, strBlkName,pt_3d_position.x, pt_3d_position.y, iDwIntersectMoveToVertex, strName, 0, nArrayHandleIntersectLine.GetAt(0), nArrayIndexOfVertex.GetAt(0), pt3dArrayIntersect.at(0).x, pt3dArrayIntersect.at(0).y, nArrayHandleIntersectLine.GetAt(1), nArrayIndexOfVertex.GetAt(1), pt_3d_position.x, pt_3d_position.y, pt3dArrayIntersect.at(0).x, pt3dArrayIntersect.at(0).y) ;
						m_AdoDb.Execute(strSql) ;
					}					
				}				
				else
				{
					int iEntType = -1 ; 

					if (strBlkName==_T("PMT_STATION"))
					{
						iEntType = 310 ;
						m_mysys.GetAttValue(pRef, _T("STATION"), strName) ;
					}
					else if (strBlkType==_T("PMT"))
					{
						iEntType = 350 ;
					}
					else if (strBlkType==_T("XHJ"))
					{
						iEntType = 100 ;
						m_mysys.GetAttValue(pRef, _T("XHJNAME"), strName) ;	
					}
					else if (strBlkType==_T("DT"))
					{			
						iEntType = 500 ;
						m_mysys.GetAttValue(pRef, _T("NAME"), strName) ;
					}
					strSql.Format(_T("insert into sxt_ent(sxt_ent_handle,  sxt_ent_block, sxt_ent_x, sxt_ent_y, sxt_ent_name, sxt_ent_type,  sxt_ent_x1, sxt_ent_y1, sxt_ent_toxhldist)  \
									 values(%d, '%s', %.10f, %.10f, '%s',  %d, %.10f, %.10f, %d) "), nHandle, strBlkName, pt_3d_position.x, pt_3d_position.y, strName, iEntType,  pt_3d_position.x, pt_3d_position.y, 0) ;
					m_AdoDb.Execute(strSql) ;
				}

			}
			pEnt->close() ;
		}	
	}
	//再遍历一遍，将轨道线读入
	for(int i=0; i<nLen; i++)
	{
		AcDbEntity * pEnt = NULL ;
		if (m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				long nHandleLine =nArrayHandle.GetAt(i) ;

				AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;	
				bool bIsClosed = pPolyLine->isClosed() ;
				double dLen = m_mysys.GetLineLen(pPolyLine) ;
				if (dLen>20) //粗略筛选轨道线
				{
					int iNumVert = pPolyLine->numVerts() ;
					for (int i=0; i<iNumVert; i++)
					{
						AcGePoint3d pt_3d_vertex(0,0,0) ;
						pPolyLine->getPointAt(i, pt_3d_vertex) ;
						strSql.Format(_T("insert into sxt_line(sxt_line_handle, sxt_line_vertex,  sxt_line_vertex_x, sxt_line_vertex_y, sxt_line_vertex1,  sxt_line_vertex_x1, sxt_line_vertex_y1, sxt_line_type )  \
										 values(%d, %d, %.10f, %.10f, %d, %.10f, %.10f, %d) "), nHandleLine, i,  pt_3d_vertex.x, pt_3d_vertex.y, i,  pt_3d_vertex.x, pt_3d_vertex.y, bIsClosed?11:0) ;
						m_AdoDb.Execute(strSql) ;
					}
				}
			}
			pEnt->close() ;
		}
	}

	//读入公里标栏，i==0：上栏，i==1：下栏
	for (int i=0; i<2; i++)
	{
		for (int j=0; j<2; j++) //j=0:设备栏，j=1：公里标栏
		{
			nLen = 0 ;
			nArrayHandle.RemoveAll() ;
			AcGePoint3d pt_3d_1(i==0?pt_3d_toxhlheader1.x:pt_3d_toxhlheader2.x, i==0?pt_3d_toxhlheader1.y:(pt_3d_toxhlheader2.y-13), 0), pt_3d_2(i==0?(pt_3d_toxhlheader1.x+m_dMaxLenPmt):(pt_3d_toxhlheader2.x+m_dMaxLenPmt), i==0?(pt_3d_toxhlheader1.y+13):pt_3d_toxhlheader2.y, 0) ;
			if (j==1)
			{
				pt_3d_1.y =(i==0?(pt_3d_toxhlheader1.y+13):(pt_3d_toxhlheader2.y-35) ) ;
				pt_3d_2.y = (i==0?(pt_3d_toxhlheader1.y+35):(pt_3d_toxhlheader2.y-13) ) ;
			}
			
			nLen = m_mysys.FindEntInArea(pt_3d_1, pt_3d_2, nArrayHandle, 1) ;

			CString strSql ;
			for(int n=0; n<nLen; n++)
			{
				UINT nHandle =nArrayHandle.GetAt(n) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
				{
					if (pEnt->isKindOf(AcDbText::desc()))
					{
						AcDbText* pText = AcDbText::cast(pEnt) ;
						CString strText = pText->textString() ;
						AcGePoint3d pt_3d_position ;
						pt_3d_position = pText->position() ;
						if (!strText.IsEmpty())
						{
							strSql.Format(_T("insert into sxt_ent(sxt_ent_handle,  sxt_ent_x, sxt_ent_y, sxt_ent_name, sxt_ent_type,  sxt_ent_x1, sxt_ent_y1)  \
											 values(%d, %.10f, %.10f, '%s',  %d, %.10f, %.10f) "), nHandle, pt_3d_position.x, pt_3d_position.y, strText,  600+i*10+j,  pt_3d_position.x, pt_3d_position.y) ;
							m_AdoDb.Execute(strSql) ;
						}
					}
					pEnt->close() ;
				}
			}
		}
	}

}


// 绘制双线图（地铁）
int CZnSxt::DrawSxt_DT(double dScaleX, double dScaleY, const AcGePoint3d & pt_3d_orign, double dLenDcPlus, double dDistBetweenSX)
{
	double dDistBetweenSxHalf = dDistBetweenSX/2 ;
	double dDc25 = dDistBetweenSxHalf, dDc15 = dDistBetweenSX*0.3 ; //道岔双线绘制时

	AcDbObjectId objIdOldLayer = m_pDb->clayer() ;
	m_mysys.Set_Layer(_T(""), _T("sxt")) ;
	AcGeTol tol  ; 
	tol.setEqualVector(0.00001) ;
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;

		//begin 根据指定基点、缩放比值，计算实体（信号机、道岔、轨道线）坐标值
		CString strTables[2] ={ _T("sxt_ent"), _T("sxt_line")} ;
		for (int iTbIndex=0; iTbIndex<2; iTbIndex++)
		{
			strSql.Format(_T("select * from %s"), strTables[iTbIndex]) ;
			CString strField1[2], strField2[2], strField3[2], strField4[2] ;

			for (int i=0; i<2; i++)
			{
				if (iTbIndex==0)
				{
					strField1[i].Format(_T("%s_x%d"), strTables[iTbIndex], i+1) ;
					strField2[i].Format(_T("%s_y%d"), strTables[iTbIndex], i+1) ;
					strField3[i].Format(_T("%s_online_intersectx%d"), strTables[iTbIndex], i+1) ;
					strField4[i].Format(_T("%s_online_intersecty%d"), strTables[iTbIndex], i+1) ;
				}
				else if (iTbIndex==1)
				{
					strField1[i].Format(_T("%s_vertex_x%d"), strTables[iTbIndex], i+1) ;
					strField2[i].Format(_T("%s_vertex_y%d"), strTables[iTbIndex], i+1) ;
				}
			}

			if(cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					double dEntX = 0.000, dEntY = 0.000, dEntX1 = 0.000, dEntY1 = 0.000;		

					cSet.GetFieldValue(strField1[0], dEntX) ;
					cSet.GetFieldValue(strField2[0], dEntY) ;
					dEntX = pt_3d_orign.x +dEntX*dScaleX ;
					dEntY = pt_3d_orign.y +dEntY*dScaleY ;

					if (iTbIndex==0)
					{
						cSet.GetFieldValue(strField3[0], dEntX1) ;
						cSet.GetFieldValue(strField4[0], dEntY1) ;
						dEntX1 = pt_3d_orign.x +dEntX1*dScaleX ;
						dEntY1 = pt_3d_orign.y +dEntY1*dScaleY ;
					}
					cSet.Edit() ;
					cSet.SetFieldValue(strField1[1], dEntX) ;
					cSet.SetFieldValue(strField2[1], dEntY) ;
					if (iTbIndex==0)
					{
						cSet.SetFieldValue(strField3[1], dEntX1) ;
						cSet.SetFieldValue(strField4[1], dEntY1) ;
					}
					cSet.Update() ;

					cSet.MoveNext() ;
				}

				cSet.Close() ;
			}
		}
		//end 根据指定基点、缩放比值，计算实体（信号机、道岔、轨道线）坐标值

		//begin 绘制线
		strSql.Format(_T("select distinct sxt_line_handle from sxt_line where sxt_line_type<>11")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long  iHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_line_handle"), iHandle) ;
				//begin 读取该线信息for绘制轨道名称
				CString strGdName ;
				int iGdWriteLeftOrRight = -1 ; //轨道名称写在线左侧0还是右侧1
				AcDbEntity* pEntLine = NULL ;
				if (m_mysys.OpenAcDbEntity(iHandle, pEntLine, AcDb::kForRead))
				{
					if(m_mysys.GetXdata(pEntLine, _T("GUIDAONAME"), strGdName))
					{
						if (!strGdName.IsEmpty())
						{
							CString strLeftForS=_T("N"), strRightForS = _T("N") ;
							m_mysys.GetXdata(pEntLine, _T("LEFT_FORS"), strLeftForS) ;
							m_mysys.GetXdata(pEntLine, _T("RIGHT_FORS"), strRightForS) ;
							if (strLeftForS==_T("S"))
							{
								iGdWriteLeftOrRight = 0 ;
							}
							else if (strRightForS==_T("S"))
							{
								iGdWriteLeftOrRight = 1 ;
							}
							else if (strLeftForS==_T("F"))
							{
								iGdWriteLeftOrRight = 0 ;
							}
							else if (strRightForS==_T("F"))
							{
								iGdWriteLeftOrRight = 1 ;
							}
						}
					}
					pEntLine->close() ;
				}
				//end 读取该线信息for绘制轨道名称

				//确定该多段线的起点和终点是否都是DC的反位连接端

				AcGePoint2dArray pt2dArrayUp, pt2dArrayDown ;
				AcArray <double> dDrawOptionUps,dDrawOptionDowns ;
				//VertexInfo=A:0,B:0,C:0;A:1,B:0,C:0;A:2,B:1,C:-1;A:3,B:2,C:1;A:4,B:3,C:2
				//ABC后面数字表示：A，双线中线的顶点序号；B，原平面图线加上极性切割绝缘节后形成的顶点序号；C，原平面图线不考虑极性绝缘节的顶点序号
				CString strXdataUp, strXdataDown ; 
				CString strXdataUpForZZJ, strXdataDownForZZJ ; //记录形式（针对的是道岔FW线）：端头连接的道岔handle-道岔FW朝向（1，朝上；0，朝下）-FW端线的顶点vertex（起点或终点）-道岔所在DW线-道岔所在DW线vertex
				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d order by sxt_line_vertex1"), iHandle) ;
				CADORecordset cSetVertex(&m_AdoDb) ;
				if (cSetVertex.Open(strSql))
				{
					int iNumVertex = cSetVertex.GetRecordCount() ;
					if (!cSetVertex.IsBOF())
					{
						cSetVertex.MoveFirst() ;
					}
					int iIndexVertex = 0 ;
					int iDrawOptionPre = 2222 ; //前一顶点的DrawOption
					while (!cSetVertex.IsEOF())
					{
						//acutPrintf(_T("\nhandle:%d,vetex:%d"), iHandle, iIndexVertex) ;
						AcGePoint2d pt_2d_up(0,0), pt_2d_down(0,0) ; //双线的上下2个点
						AcGePoint2d pt_2d_cur(0,0), pt_2d_pre(0,0), pt_2d_back(0,0) ; 

						int iIndexVertexOld = iIndexVertex ; //原顶点序号（未加入切割点）
						cSetVertex.GetFieldValue(_T("sxt_line_vertex"), iIndexVertexOld) ;

						cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_cur.x) ;
						cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_cur.y) ;

						//begin 线宽
						long nHandleVertex = 0 ; //该点对应的绝缘节或者道岔
						int iDrawOptionCur = -1 ; //当前顶点的DrawOption
						cSetVertex.GetFieldValue(_T("sxt_line_vertex_handle"), nHandleVertex) ;

						double dWidthUp = 0, dWidthDown = 0 ;

						int iPolar = 0 ;
						cSetVertex.GetFieldValue(_T("sxt_line_polar"), iPolar) ;
						if (iIndexVertex==(iNumVertex-1)) //最后一段极性取前一个顶点的值
						{
							cSetVertex.MovePrevious() ;
							cSetVertex.GetFieldValue(_T("sxt_line_polar"), iPolar) ;
							cSetVertex.MoveNext() ;
						}
						if (iPolar==-1) //上粗下细
						{
							dWidthUp = 0.5 ;
							dWidthDown = 0 ;							
						}
						else if (iPolar==1)
						{
							dWidthUp = 0 ;
							dWidthDown = 0.5 ;				
						}
						//end 线宽

						if (iIndexVertex!=0)//不是第一个点
						{
							cSetVertex.MovePrevious() ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_pre.x) ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_pre.y) ;
							cSetVertex.MoveNext() ;
						}
						if (iIndexVertex!=(iNumVertex-1)) //不是最后一个点
						{
							cSetVertex.MoveNext() ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_back.x) ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_back.y) ;
							cSetVertex.MovePrevious() ;
						}

						if (iIndexVertex==0)
						{
							AcGeLineSeg2d lineSeg2dSrc(pt_2d_cur, pt_2d_back) ;
							AcGeLineSeg2d lineSeg2dParallelUp, lineSeg2dParallelDown ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelUp, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelDown, dDistBetweenSxHalf, FALSE) ;
							pt_2d_up = lineSeg2dParallelUp.startPoint() ;
							pt_2d_down = lineSeg2dParallelDown.startPoint() ;

							//begin 绘制轨道名称
							if (iGdWriteLeftOrRight==0)
							{
								AcGePoint2d pt_2d_gdname = lineSeg2dSrc.midPoint() ;
								AcGePoint3d pt_3dg_gdname(pt_2d_gdname.x, pt_2d_gdname.y, 0) ;
								AcGeVector2d vecLine = lineSeg2dSrc.direction() ;
								AcDbObjectId objIdGdName ;
								m_mysys.AddText(objIdGdName, strGdName, pt_3dg_gdname, pt_3dg_gdname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecLine.angle(), 4)  ;	
								m_mysys.SetXdata(objIdGdName, _T("TYPE"), (long)101) ; //双线图中轨道名称文本
								m_mysys.SetXdata(objIdGdName, _T("BELONG_ENT"), iHandle) ;
							}
							//end 绘制轨道名称

							//和该FW端相交的DW轨道线的信息
							AcGePoint2d pt_2d_start_dw, pt_2d_end_dw ;	
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d and sxt_ent_fwline_vertex=%d"), iHandle, iIndexVertex) ;
							long iHandleDW = 0 ;
							int iLineVertexDW = 0 ;							
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), iHandleDW) ;
							long nHandleDc = 0 ; //以该线为反位线的道岔记录下来，为双线图绘制尖轨芯轨用
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDc) ;
							CString strDcName ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), strDcName) ;
							int iUpOrDownDc = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;
							if (iHandleDW!=0) //还有不是和DC反位相连的线，比如轨道起始
							{
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_vertex"), iLineVertexDW) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_start_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_start_dw.y) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW+1) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_end_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_end_dw.y) ;
								//2条平行线，一条距定位2.5,，另一条是距定位线1.5
								AcGeLineSeg2d lineSeg2dDwSrc(pt_2d_start_dw, pt_2d_end_dw), lineSeg2dDw25, lineSeg2dDw15 ;

								//begin 尖轨、芯轨牵引数量
								//int iJgNum = 3, iXgNum = 2 ; 
								double dAngleDc = 0 ; //道岔块旋转角度
								AcDbEntity* pEntDc = NULL ;
								if (m_mysys.OpenAcDbEntity(nHandleDc, pEntDc, AcDb::kForRead))
								{
									AcDbBlockReference* pRefDc = AcDbBlockReference::cast(pEntDc) ;
									dAngleDc = pRefDc->rotation() ;
									pEntDc->close() ;
								}
								//end 尖轨、芯轨牵引数量

								AcGePoint2d pt_2d_ext ;
								AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
								//用于判断道岔垂直方向，不能简单根据*** int iDcDirectV = ((pt_2d_back.y-pt_2d_cur.y)>0.5?0:1) ; //道岔垂直方向（FW向上0、向下1）***
								//根据以下算法：道岔旋转角，90~270度之间，向下	
								int iDcDirectV =0 ; //道岔垂直方向（FW向上0、向下1）
								if (dAngleDc>PI/2&&dAngleDc<(PI*3/2))
								{
									iDcDirectV = 1 ;
								}
								if (iDcDirectV==0) //反位端朝上
								{
									//反位端相交的所在多段线及段序号
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, TRUE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, FALSE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw25, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw15, pt_2d_down) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext =pt_2d_down ;
									pt_2d_ext.x -=dLenDcPlus ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_down) ;								
									pt2dArrayDown.append(pt_2d_ext) ;		
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataDownForZZJ.AppendFormat(_T("%d-1-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;	
								}
								else //反位端朝下，（反位端在水平线上，视同反位朝下）
								{
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, FALSE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, TRUE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw15, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw25, pt_2d_down) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext = pt_2d_up ;
									pt_2d_ext.x -=dLenDcPlus ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_up) ;
									pt2dArrayUp.append(pt_2d_ext) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataUpForZZJ.AppendFormat(_T("%d-0-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}

								//begin draw zzj
								int iDcDirectH =0 ; //道岔FW水平朝向，0朝右，1，朝左
								AcGeLineSeg2d line2d(pt_2d_ext,iDcDirectV==0?pt_2d_down:pt_2d_up) ;									
								AcGePoint2d pt_2d_extcen =line2d.midPoint() ;
								AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
								pt_2d_extcen_mid.y+=(iDcDirectV==0?dDc15:-dDc15) ;
								pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//映射到双线的一边
								AcGePoint2d pt_2d_extcen_jg = pt_2d_extcen_mid ;
								pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-dDistBetweenSxHalf:dDistBetweenSxHalf); 
								pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//道岔名称
								AcGePoint2d pt_2d_dcname = pt_2d_extcen_jg ;
								pt_2d_dcname.y+=(iUpOrDownDc==0?-2:2); 
								pt_2d_dcname.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
								AcGePoint3d pt_3d_dcname(pt_2d_dcname.x, pt_2d_dcname.y,0) ;
								m_mysys.AddText(strDcName, pt_3d_dcname, pt_3d_dcname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecDw.angle(), 3) ;
								//end draw zzj
							}

							pt2dArrayUp.append(pt_2d_up) ;
							dDrawOptionUps.append(dWidthUp) ;
							strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
							pt2dArrayDown.append(pt_2d_down) ;
							dDrawOptionDowns.append(dWidthDown) ;
							strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
						}
						else if (iIndexVertex==(iNumVertex-1))
						{
							AcGeLineSeg2d lineSeg2dSrc(pt_2d_pre, pt_2d_cur) ;
							AcGeLineSeg2d lineSeg2dParallelUp, lineSeg2dParallelDown ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelUp, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelDown, dDistBetweenSxHalf, FALSE) ;
							pt_2d_up = lineSeg2dParallelUp.endPoint() ;
							pt_2d_down = lineSeg2dParallelDown.endPoint() ;

							//begin 绘制轨道名称
							if (iGdWriteLeftOrRight==1)
							{
								AcGePoint2d pt_2d_gdname = lineSeg2dSrc.midPoint() ;
								AcGePoint3d pt_3dg_gdname(pt_2d_gdname.x, pt_2d_gdname.y, 0) ;
								AcGeVector2d vecLine = lineSeg2dSrc.direction() ;
								AcDbObjectId objIdGdName ;
								m_mysys.AddText(objIdGdName, strGdName, pt_3dg_gdname, pt_3dg_gdname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecLine.angle(), 4)  ;	
								m_mysys.SetXdata(objIdGdName, _T("TYPE"), (long)101) ; //双线图中轨道名称文本
								m_mysys.SetXdata(objIdGdName, _T("BELONG_ENT"), iHandle) ;
							}
							//end 绘制轨道名称

							int iIndexVertexOld = -1 ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex"), iIndexVertexOld) ;

							//和该FW端相交的DW轨道线的信息
							AcGePoint2d pt_2d_start_dw, pt_2d_end_dw ;	
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d and sxt_ent_fwline_vertex=%d"), iHandle, iIndexVertexOld) ;
							long iHandleDW = 0 ;
							int iLineVertexDW = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), iHandleDW) ;
							long nHandleDc = 0 ; //以该线为反位线的道岔记录下来，为双线图绘制尖轨芯轨用
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDc) ;
							CString strDcName ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), strDcName) ;
							int iUpOrDownDc = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;

							if (iHandleDW!=0) //还有不是和DC反位相连的线，比如轨道终点
							{
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_vertex"), iLineVertexDW) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_start_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_start_dw.y) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW+1) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_end_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_end_dw.y) ;
								//2条平行线，一条距定位2.5,，另一条是距定位线1.5
								AcGeLineSeg2d lineSeg2dDwSrc(pt_2d_start_dw, pt_2d_end_dw), lineSeg2dDw25, lineSeg2dDw15 ;

								//begin 尖轨、芯轨牵引数量
								double dAngleDc = 0 ; //道岔块旋转角度
								AcDbEntity* pEntDc = NULL ;
								if (m_mysys.OpenAcDbEntity(nHandleDc, pEntDc, AcDb::kForRead))
								{
									AcDbBlockReference* pRefDc = AcDbBlockReference::cast(pEntDc) ;
									dAngleDc = pRefDc->rotation() ;
									pEntDc->close() ;
								}
								//end 尖轨、芯轨牵引数量

								AcGePoint2d pt_2d_ext ;
								AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
								//用于判断道岔垂直方向，不能简单根据*** int iDcDirectV = ((pt_2d_pre.y-pt_2d_cur.y)>0.5?0:1) ; //道岔垂直方向（FW向上0、向下1）***
								//根据以下算法：道岔旋转角，90~270度之间，向下								
								int iDcDirectV =0 ; //道岔垂直方向（FW向上0、向下1）
								if (dAngleDc>PI/2&&dAngleDc<(PI*3/2))
								{
									iDcDirectV = 1 ;
								}
								if (iDcDirectV==0) //反位端朝上,允许一定误差，这个0.5可再议
								{
									//反位端相交的所在多段线及段序号
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, TRUE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, FALSE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw25, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw15, pt_2d_down) ;

									pt2dArrayUp.append(pt_2d_up) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									pt2dArrayDown.append(pt_2d_down) ;
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext =pt_2d_down ;
									pt_2d_ext.x +=dLenDcPlus ;
									//AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_down) ;								
									pt2dArrayDown.append(pt_2d_ext) ;	
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataDownForZZJ.AppendFormat(_T("%d-1-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}
								else //反位端朝下，（反位端在水平线上，视同反位朝下）
								{
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, FALSE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, TRUE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw15, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw25, pt_2d_down) ;

									pt2dArrayUp.append(pt_2d_up) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									pt2dArrayDown.append(pt_2d_down) ;
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext = pt_2d_up ;
									pt_2d_ext.x +=dLenDcPlus ;
									//AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_up) ;
									pt2dArrayUp.append(pt_2d_ext) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataUpForZZJ.AppendFormat(_T("%d-0-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}

								//begin draw zzj
								int iDcDirectH = 1 ; //道岔FW水平朝向，0朝右，1，朝左
								AcGeLineSeg2d line2d(pt_2d_ext,iDcDirectV==0?pt_2d_down:pt_2d_up) ;									
								AcGePoint2d pt_2d_extcen =line2d.midPoint() ;

								AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
								pt_2d_extcen_mid.y+=(iDcDirectV==0?dDc15:-dDc15) ;
								pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//映射到双线的一边
								AcGePoint2d pt_2d_extcen_jg = pt_2d_extcen_mid ;
								pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-dDistBetweenSxHalf:dDistBetweenSxHalf); 
								pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//道岔名称
								AcGePoint2d pt_2d_dcname = pt_2d_extcen_jg ;
								pt_2d_dcname.y+=(iUpOrDownDc==0?-2:2); 
								pt_2d_dcname.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
								AcGePoint3d pt_3d_dcname(pt_2d_dcname.x, pt_2d_dcname.y,0) ;
								m_mysys.AddText(strDcName, pt_3d_dcname, pt_3d_dcname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecDw.angle(), 3) ;
								//end draw zzj
							}
							else
							{
								pt2dArrayUp.append(pt_2d_up) ;
								dDrawOptionUps.append(dWidthUp) ;
								strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
								pt2dArrayDown.append(pt_2d_down) ;
								dDrawOptionDowns.append(dWidthDown) ;
								strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
							}
						}
						else
						{
							AcGeLineSeg2d lineSeg2dSrc1(pt_2d_pre, pt_2d_cur), lineSeg2dSrc2(pt_2d_cur, pt_2d_back) ;	
							//acutPrintf(_T("\nlinesrc1(%.3f,%.3f)(%.3f,%.3f) linesrc2(%.3f,%.3f)(%.3f,%.3f)"), lineSeg2dSrc1.startPoint().x,lineSeg2dSrc1.startPoint().y,lineSeg2dSrc1.endPoint().x,lineSeg2dSrc1.endPoint().y,lineSeg2dSrc2.startPoint().x,lineSeg2dSrc2.startPoint().y, lineSeg2dSrc2.endPoint().x,lineSeg2dSrc2.endPoint().y );

							AcGeLineSeg2d lineSeg2dParallelUp1, lineSeg2dParallelDown1, lineSeg2dParallelUp2, lineSeg2dParallelDown2 ;
							m_mysys.GetParallelLine(lineSeg2dSrc1, lineSeg2dParallelUp1, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc1, lineSeg2dParallelDown1, dDistBetweenSxHalf, FALSE) ;
							m_mysys.GetParallelLine(lineSeg2dSrc2, lineSeg2dParallelUp2, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc2, lineSeg2dParallelDown2, dDistBetweenSxHalf, FALSE) ;				

							m_mysys.IntersectWith(&lineSeg2dParallelUp1, &lineSeg2dParallelUp2, pt_2d_up, AcDb::kExtendBoth) ;
							m_mysys.IntersectWith(&lineSeg2dParallelDown1, &lineSeg2dParallelDown2, pt_2d_down, AcDb::kExtendBoth) ;

							if (lineSeg2dSrc1.isParallelTo(lineSeg2dSrc2, tol))
							{
								pt_2d_up = lineSeg2dParallelUp1.endPoint() ;
								pt_2d_down = lineSeg2dParallelDown1.endPoint() ;
							}
							//acutPrintf(_T("\nintersect-up(%.3f,%.3f)intersect-down(%.3f,%.3f)"), pt_2d_up.x,pt_2d_up.y,pt_2d_down.x,pt_2d_down.y );

							pt2dArrayUp.append(pt_2d_up) ;
							dDrawOptionUps.append(dWidthUp) ;
							strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
							pt2dArrayDown.append(pt_2d_down) ;
							dDrawOptionDowns.append(dWidthDown) ;
							strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
						}

						cSetVertex.MoveNext() ;
						iIndexVertex++ ;
						iDrawOptionPre = iDrawOptionCur ;
					}
					cSetVertex.Close() ;
				}

				AcDbObjectId objIdUp = m_mysys.AddAcDbPolyLine(pt2dArrayUp, dDrawOptionUps, 7) ;				
				AcDbObjectId objIdDown = m_mysys.AddAcDbPolyLine(pt2dArrayDown, dDrawOptionDowns, 7) ;

				//set xdata
				strXdataUp.TrimRight(_T(";")) ;
				strXdataDown.TrimRight(_T(";")) ;
				strXdataUpForZZJ.TrimRight(_T(";")) ;
				strXdataDownForZZJ.TrimRight(_T(";")) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(objIdUp, pEnt))
				{
					m_mysys.SetXdata(pEnt, _T("CloneFrom"), iHandle) ;
					m_mysys.SetXdata(pEnt, _T("UPDOWN"), 1) ;
					m_mysys.SetXdata(pEnt, _T("VertexInfo"), strXdataUp) ;
					if (!strXdataUpForZZJ.IsEmpty())
					{
						m_mysys.SetXdata(pEnt, _T("ForZZJ"), strXdataUpForZZJ) ;
					}
					pEnt->close() ;
				}
				if (m_mysys.OpenAcDbEntity(objIdDown, pEnt))
				{
					m_mysys.SetXdata(pEnt, _T("CloneFrom"), iHandle) ;
					m_mysys.SetXdata(pEnt, _T("UPDOWN"), 0) ;
					m_mysys.SetXdata(pEnt, _T("VertexInfo"), strXdataDown) ;
					if (!strXdataDownForZZJ.IsEmpty())
					{
						m_mysys.SetXdata(pEnt, _T("ForZZJ"), strXdataDownForZZJ) ;
					}
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end 绘制线

		//begin 绘制封闭的线
		strSql.Format(_T("select distinct sxt_line_handle from sxt_line where sxt_line_type=11")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long  nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_line_handle"), nHandle) ;
				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d order by sxt_line_vertex asc"), nHandle) ;
				CDoubleArray dArrayX, dArrayY ;
				int iRsCount = m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), dArrayX) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"), dArrayY) ;
				if (iRsCount>2)
				{
					AcGePoint3dArray pt3dArray ;
					for (int i=0; i<iRsCount; i++)
					{
						AcGePoint3d pt_3d_tmp(dArrayX.GetAt(i), dArrayY.GetAt(i), 0) ;
						pt3dArray.append(pt_3d_tmp) ;
					}
					m_mysys.AddAcDbPolyLine(pt3dArray, 0.2, 7, TRUE) ;
				}

				cSet.MoveNext() ;
			}
		}
		//end 绘制封闭的线

		//begin 信号机
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=100")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//begin 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					AcGePoint3d pt_3d_new2(pt_3d_new) ;
					int iMarkLoc = strBlkName.Find(_T("_Y_")) ;//正向信号机（Y 立于右侧）, 逆向信号机(Z 立于左侧)
					//角度在90度~270度之间是倒置了
					if (dAngle>PI/2&&dAngle<PI*3/2)
					{
						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf) ;
					}
					else
					{
						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf) ;
					}
					pt_3d_new2.rotateBy(PI/2, AcGeVector3d::kZAxis, pt_3d_new) ;
					vec = pt_3d_new2 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//end 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;

					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 信号机

		/*
		//begin 绝缘节
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=2")) ; //整除为2的
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				int iEntType = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_type"), iEntType) ;

				//计算双线中线绝缘节位置时，应以平面图绝缘节和多段线交点作为源数据为宜
				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_online_intersectx2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_online_intersecty2"), dEntY) ;

				///begin 绝缘节所在线段信息	
				AcGeVector2d vecOnLine = GetAngleOfJyjOnLine(nHandle) ;
				double dAngleOnLine = 0 ;
				dAngleOnLine = vecOnLine.angle() ;
				//end  绝缘节所在线段信息

				//begin 复制平面图中的绝缘节（2份）并经过移动转换到双线中
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew1 = AcDbBlockReference::cast(pRef->clone()) ;
					AcDbBlockReference *pRefNew2 = AcDbBlockReference::cast(pRef->clone()) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;//双线中间线和绝缘节交点位置
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;					
					pRefNew1->transformBy(matrix) ; 
					pRefNew2->transformBy(matrix) ; 
					//begin 在双线图中，绝缘节基点不能在中线上，上/下移2.5
					AcGePoint3d pt_3d_new1(pt_3d_new), pt_3d_new2(pt_3d_new) ;
					pt_3d_new1.x = pt_3d_new.x+dDistBetweenSxHalf ;
					pt_3d_new2.x = pt_3d_new.x-dDistBetweenSxHalf ;
					pt_3d_new1.rotateBy(PI/2+dAngleOnLine, AcGeVector3d::kZAxis, pt_3d_new) ;
					pt_3d_new2.rotateBy(PI/2+dAngleOnLine, AcGeVector3d::kZAxis, pt_3d_new) ;
					vec = pt_3d_new1 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew1->transformBy(matrix) ; 
					vec = pt_3d_new2 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew2->transformBy(matrix) ; 
					//end 在双线图中，绝缘节基点不能在中线上，上/下移2.5
					pRefNew1->setColorIndex(1) ;
					pRefNew1->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew1) ;
					m_mysys.SetXdata(pRefNew1, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew1, _T("UPDOWN"), 1) ;
					pRefNew1->close() ;
					pRefNew2->setColorIndex(3) ;
					pRefNew2->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew2) ;
					m_mysys.SetXdata(pRefNew2, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew2, _T("UPDOWN"), 0) ;
					pRefNew2->close() ;
					pEnt->close() ;
				}
				//end 复制平面图中的绝缘节（2份）并经过移动转换到双线中

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绝缘节
		*/

		//begin 其他
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=500")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
// 					//begin 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
// 					AcGePoint3d pt_3d_new2(pt_3d_new) ;
// 					int iMarkLoc = strBlkName.Find(_T("_Y_")) ;//正向信号机（Y 立于右侧）, 逆向信号机(Z 立于左侧)
// 					//角度在90度~270度之间是倒置了
// 					if (dAngle>PI/2&&dAngle<PI*3/2)
// 					{
// 						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?2.5:-2.5) ;
// 					}
// 					else
// 					{
// 						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?-2.5:2.5) ;
// 					}
// 					pt_3d_new2.rotateBy(PI/2, AcGeVector3d::kZAxis, pt_3d_new) ;
// 					vec = pt_3d_new2 - pt_3d_new ;
// 					matrix.setToTranslation(vec) ;
// 					pRefNew->transformBy(matrix) ; 
// 					//end 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;

					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 其他

		//begin 绘制跳线
		//this->DrawJumpLine() ;		
		//end 绘制跳线

		//begin 绘制 信号楼(20150605：双线不绘制警冲标）
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=3")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;

					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绘制警冲标 信号楼

		//begin 绘制距离信号楼距离
		AcGePoint3d pt_3d_min_dwg(0,0,0), pt_3d_max_dwg(0,0,0) ; //先取得当前图纸的范围,以便确定绘制距信号楼距离的坐标
		m_mysys.GetCurExtents(pt_3d_min_dwg, pt_3d_max_dwg) ;
		double dY_Dist[2][2] = {{0,0}, {0,0} } ;
		dY_Dist[0][0] = pt_3d_max_dwg.y+80 ;
		dY_Dist[0][1] = dY_Dist[0][0]+13 ;
		dY_Dist[1][0] = pt_3d_orign.y-(dY_Dist[0][0]-pt_3d_orign.y) ;
		dY_Dist[1][1] = dY_Dist[1][0]-13 ; 
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=6")) ; //整除为6
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				int iType = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_type"), iType) ;
				CString strLoc ;
				cSet.GetFieldValue(_T("sxt_ent_name"), strLoc) ;
				double dX2 = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_x2"), dX2) ;

				int i1 = iType%10 ; //个位
				int i10 = iType%100/10 ; //十位
                
				AcGePoint3d pt_3d_insert_text(dX2-1.5, dY_Dist[i10][i1]+(i10==0?1:-1)*(i1==0?6.5:11), 0) ;

				m_mysys.AddText(strLoc, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2) ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}

		//插入表头、绘制行线
		strSql.Format(_T("select min(sxt_ent_x2) as minX, max(sxt_ent_x2) as maxX from sxt_ent where sxt_ent_type\\100=6")) ; //整除为6
		double dMinX = 0, dMaxX = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("minX"), dMinX) ;
		m_RecordSet.GetRecordValue(strSql, _T("maxX"), dMaxX) ;

		AcDbBlockReference* pRef = NULL ;
		AcGePoint3d pt_3d_header(dMinX-30,0,0) ;
		CString strBlkName ; 
		for (int i=0; i<2; i++)
		{
			//表头
			pt_3d_header.y = dY_Dist[i][0] ;
			strBlkName.Format(_T("DT_LOC_HEADER_%d00"), i) ;
			if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_header))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//线
			AcGePoint3d pt_3d_start(pt_3d_header.x+7.5, 0, 0), pt_3d_end(dMaxX+30, 0, 0) ;
			for (int j=0; j<2; j++)
			{
				pt_3d_start.y = dY_Dist[i][j] ;
				pt_3d_end.y = dY_Dist[i][j] ;
				m_mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				if (j==1)
				{
					pt_3d_start.y = dY_Dist[i][j]+(i==0?1:-1)*22 ;
					pt_3d_end.y = dY_Dist[i][j]+(i==0?1:-1)*22 ;
					m_mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
			}

		}





// 		for (int i=0; i<5; i++)
// 		{
// 			AcGePoint3d pt_3d_start(pt_3d_insert_toxhldist.x+20, pt_3d_insert_toxhldist.y-i*dRowHeight, 0) ;
// 			AcGePoint3d pt_3d_end(pt_3d_max_dwg.x, pt_3d_insert_toxhldist.y-i*dRowHeight, 0) ;
// 			m_mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.5) ;
// 		}
		//end 绘制距离信号楼距离
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in Drawsxt_DT"));
	}

	m_pDb->setClayer(objIdOldLayer) ;

	return 0;
}
