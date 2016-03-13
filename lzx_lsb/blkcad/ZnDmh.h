#pragma once

#define WM_USER_THREAD_PROGRESSRANGE WM_USER+0x100
#define WM_USER_THREAD_PROGRESSPOS WM_USER+0x101

struct CCircuit_Data
{
	CString strMySelf_Name;   //对应信号机名称
	CString strMySelf_G_Name;// 对应轨道电路名称；
	CString strMySelf_Frequencry ; //频率 ;
	CString strMySelf_QY_Loc ;  //在移频柜上的位置　；
	CString strMySelf_QZH_Loc ;  //网络接口柜位置
	CString strMySelf_QZ_Loc ;   //组合架位置　;
	CString strMySelf_GLB_Loc ; //隔离变压器在网络接口柜上的位置;
	CString strMySelf_GLB_RD_Name ; //隔离变压器融断器名称 ;
	CString strMySelf_Fs_RD_Name ; //发送融断器名称 ;
	CString strMySelf_Js_RD_Name ;//接收融断器名称 ;

	CString strGLB_RD_Start_Tie ; //隔离变压器融断器起始端子号　；
	CString strMySelf_FsJs_RD_Start_Tie ; //发送、接收融断器起始端子号;
	CString str1FsLoc;               //+1FS的位置；

	int  iMySelf_FsJs_Index ; //发送、接收　序号　;

	CString strFsJs_Start_Tie ;
	CString strDd_Start_Tie ;
	CString strFsJs_Face_Start_Tie ;

	CString strFront_G_Name ; 
	CString strFront_QZ_Loc ; //前面轨道电路在组合架上的位置 ;

	CString strBack_G_Name ;
	CString strBack_QZ_Loc ;//后面轨道电路在组合架上的位置 ;

	CString strCombine_G_Name ; //并机轨道电路名称;
	CString strCombine_QY_Loc ; //对应移频柜上的位置;
	CString strCombine_Frequencry ; //并机频率 ;
	int iCombine_Js_Index ; //并机发送接收　序号;
	CString  strCombine_Js_RD_Name ; //并机接收融断器名称
	CString strCombine_Js_RD_Start_Tie ;//并机接收融断器起始端子号　;

	CString strStation ;
	CString strData ;
	CString strDrawingNo ;
	CString strDrawingName ;

	CString strQzh_Start_No ;  //网络接口柜的起始号
	CString strQz_Start_No ; //组合架起始号 ;

	int iUp_Or_Down ;
	CString strFront_Symbol_Name ;
	CString strBack_Symbol_Name ;

	CString str1Lq_G_Name ;
	CString str1Lq_Qz_Loc ;

};
struct PXT_DATA
{
	TCHAR chZ_StartNo[10] ;
	TCHAR chZZH_StartNo[10] ;
	TCHAR chFXP_StartNo[10] ;
	//签字框
	TCHAR chStationName[20] ;
	TCHAR chDrawingName[30] ;
	TCHAR chDrawingNo[30] ;
	TCHAR chDate[15] ;
};

// struct SystemSet
// {
// 	BOOL bHaveGLB; //隔离变压器是否放在网络接口柜中
// 	CString strQy_Pre ;
// 	CString strQz_Pre ;
// 	CString strQzh_Pre ;
// 	double dQZH_Font_Size ;
// 	CString strDraw_No_Pre;
// 	CString strProject_Name ;
// 
// 	CString str1FSXxLoc;
// 	CString str1FSSxLoc;
// 	int iTrain_Speed ; // == 1 为　160KM ; == 2 为　200KM ;
// 	int  i1LQ_Length ; //1LQ长度 ;
// 	CString strCurData ; //当前时间 ;
// 
// 	int iMax_Qzh ;
// 	int iMax_Qz ;
// 	int iMax_Qy ;
// 	CString strDrawAllCircuit_Inof;
// 	CIni cDrawAllCircuit_Error_Info;
// };

class CZnDmh
{
public:
	CZnDmh(void);
	~CZnDmh(void);
	// 处理计算输入excel表格
	static int ImportDmh(const CString & strXlsFileName);
	// 预处理输入表格
	static int PreDoDmhImport(const CString & strXlsFileName);
	// 端子关联 iMode=1表示不弹出对话框，选择增加一个连接
	static BOOL SetEntConnect(AcDbEntity *pEnt, long nSecondId , long nConCableId, int iMode=0) ;
	static BOOL GetEntFromId(AcDbEntity *& pEnt, long nEntId);
	static int SelectAcDbText(const TCHAR *chPrompt, ads_name &ent_name);
	static int SelectAcDbBlockReference(const TCHAR *chPrompt, const TCHAR * chBlockPre , ads_name &ent_name) ;
	static int ConTie(int nMode=0);
	static int InputCableType(const TCHAR *chPrompt, const TCHAR *chBlockPre, const AcGePoint3d pt_3d_insert, AcDbBlockReference *&pRef, ads_name &ent_name);

	static int SetPar(void);
	// 将输入表格读回数据库（程序生成初稿，经人工更改，最后再导入数据库）
	int ReImportDmh(const CString & strXlsFileName) ;
	int ComputeCircuit(void);
	// 读取strGuiDao在ZZH表中的信息
	int GetZzhInfo(const CString & strGuiDao, int & iShelf_FL, int & iLayer_FL, int & iBox_FL, int & iShelf, int & iLayer, int & iBox);
	// 读取strGuiDao在联锁信息表中相关信息-受电端
	int GetLsInfoR(const CString & strGuiDao, CString & strCircuitType, CString & strGJFPoint, CString & strGJFSide, CString & strGSide1, CString & strGSide2, CString & strFLoc1, CString & strFLoc2);
	// 读取联锁信息表中相关信息-送电端
	int GetLsInfoF(const CString & strGuiDao, CString & strPower1, CString & strPower2, CString & strFLoc1, CString & strFLoc2);
	// 读取组合内部侧面端子
	int GetZhNbNode(const CString &strZhNb, const CString &strLocType, const CString & strNodeName,	int iLoc, CString & strNode);
	CString m_strSysBasePath;
	BOOL InsertDwgToDb(const CString strDwgFilePath, AcDbDatabase *& pDb, const AcGeMatrix3d & xform, const CMapStringToString & mapTypeToStr, const CMapStringToString & mapToAddXdata, const CString strRepType, int iIdAdd);
	BOOL InsertDwgToDb1(const CString strDwgFilePath, AcDbDatabase *& pDb, const AcGeMatrix3d & xform, const CMapStringToString & mapToReplaceXdata, const CMapStringToString & mapTypeToStr, const CMapStringToString & mapToAddXdata, const CString strRepType, int iIdAdd);
	// 填充输入表
	int FillImport(const CString & strXlsFile);
	// 绘制综合柜架柜图
	void DrawZZH(const CString &strSavePath);
	// 绘制组合柜
	void DrawZ(const CString &strSavePath);
	// 检查需不需要增加组合柜
	void CheckToAddZ(int & iZLocRow, int & iZIndex);
	// 查询某条线有无 反发、正发、反接
	void HasJinLuType(const CString & strLineName, BOOL & bHasFanFa, BOOL & bHasZhengFa, BOOL & bHasFanJie);
	// 绘制站内移频柜图
	void DrawZY(const CString &strSavePath);
	// 读入组合架到数据库
	void ReadZ(const CString & strFileName);
	// 查找在指定点的组合块（layer）
	int FindZLayer(const AcGePoint3d & pt_3d_base, CString & strBlockName, CString & strShowType, CString & strStartEndXhj1, CString & strStartEndXhj2, CStringArray & strArrayJdqName);
	AcDbDatabase *m_pDb;
	// 查找组合中1~10继电器位置放置的元素
	int FindCell(const CString & strToFindBlk, const AcGePoint3d& pt_3d_topleft, const AcGePoint3d& pt_3d_bottomright, CString & strCellContent);
	// 取得某个组合1~10继电器的名称
	int GetJdqNames(const CString & strBlkName, CStringArray & strArrayJdqNames);
	// 读入综合柜图
	void ReadZZH(const CString & strFileName);
	int FindZZHLayer(const AcGePoint3d & pt_3d_base, CString & strBlkName);
	
	// 读入站内移频柜
	void ReadZY(const CString & strFileName);
	// 查找ZY移频柜单元块
	int FindZYCell(const AcGePoint3d & pt_3d_topleft, const AcGePoint3d & pt_3d_bottomright, CString & strJinLuName, CString & strFreq1, CString & strFreq2);
	// 绘制电路图
	void DrawCircuit(const CString &strSavePath);
	// 填充一个内隔离模块的扩展数据, iGCJIndex为0表示股道无分割时GCJ1，其他表示1GCJ~10GCJ
	void FillXdataNGL(int iGCJIndex, const CString & strGuiDao, const CString &strIsROrF, int iIsSendCode, CMapStringToString & mapStrToStrXdata, CMapStringToString &mapToAddXdata, const CString &strJinLuName);
	// 填充GCJ_0000（strGuDao不为空时表示股道无分割）块的扩展数据
	void FillXdataGCJ_0000(int iJinLuType, const CString & strGuDao, const CString	&strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充JMJ块的扩展数据
	void FillXdataJMJ_0000(int iJinLuType, int iGuDaoFenGe, const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;

	// 组合架名称前缀
	CString m_strPreZ;
	// 移频柜名称前缀
	CString m_strPreZY;
	// 综合柜名称前缀
	CString m_strPreZZH;

	//填充GCJ扩展数据
	void FillXdataGCJ(int nGCJIndex, int nGCJJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe,int iJinLuType);
	// 填充GCJ所在的组合位置图块的扩展数据
	void FillXdataGCJZLoc(int iJinLuType, const CString & strLayerBlk, const CString & strZhShowName,const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iMode=0);
	// 填充TFGZ块的扩展数据
	void FillXdataTFGZ(const CString & strJinLuName, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, int iJinLuType, int iRunDirect=0);
	// 填充ZJC-FMJ块的扩展数据
	void FillXdataZFJC_FMJ(int iLineNum, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充FBJ块的扩展数据
	void FillXdataFBJ(int iJinLuType, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充lici_jfmj块扩展数据
	void FillXdataLiCi_JFMJ(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// 填充发送块的扩展数据
	void FillXdataFS(int iUpOrDwon, int iJinLuType, const CString &strJinLuName, CMapStringToString & mapReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充FuSi-GJF块的扩展数据
	void FillXdataFuSi_GJF(const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// 填充块fusi-xjf的扩展数据
	void FillXdataFuSi_XJF(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充GCJGJF-GJF块的扩展数据
	void FillXdataGCJGJF_GJF(int iJinLuType, int iGCJIndex, int iGJFIndex, int iMaxGCJ,const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuiDao,int nGJFJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// 填充GCJGJF-GCJ块的扩展数据
	void FillXdataGCJGJF_GCJ(int iJinLuType, int iGCJIndex,  int iMaxGCJ, const CString &strGuiDao, int nGCJJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, CString strJZ=_T(""), CString strFCZ=_T(""), CString strCZ=_T(""), CString strFJZ=_T(""));
	
	// 填充GCJGJF-JFMJ_0000块扩展数据
	void FillXdataGCJGJF_JFMJ_0000(const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// 填充GCJGJF_JFMJ_1000 块的扩展数据
	void FillXdataGCJGJF_JFMJ_1000(int iLineNum, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充GCJGJF-JFMJ_2000块的扩展数据
	void FillXdataGCJGJF_JFMJ_2000(int iLineIndex, const CString &strJZ, const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	// 填充GCJGJF_JFMJ_3000块扩展数据
	void FillXdataGCJGJF_JFMJ_3000(int iMaxGCJ, const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	
	// 填充zbjh-3jgj块的扩展数据
	void FillXdataZBJH_JGJ_1LQ(int iJinLuType,const CString &strJZXHJ, int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 取得线路的相关信息，进站、出站......
	void GetLineInfo(const CString & strLineName, CString & strJZXHJ, CString & strFCZXHJ, CString & strCZXHJ, CString & strFJZXHJ);
	// 填充+1FS块扩展数据
	void FillXdataFSPLUS(CMapStringToString & mapStrToStrXdata);
	// 将图纸中的MySn等信息转换成Id，以便配线
	void TransMySnToId(AcDbDatabase* & pDb);
	// 查找指定扩展数据的实体
	int GetTie(const CString & strToFindApp, const CString & strToFindXdata, UINT& iHandle);
	// 删除生图过程中的扩展数据，主要是Sn等
	void DelSn(AcDbDatabase * & pDb);
	// 填充（FJC-FMJ）块的扩展数据
	void FillXdataFJC_FMJ(int iLineIndex, const CString &strJZ, const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	// 链接GCJ-GJF左端，类似ZBJH块
	void FillXdataZJC_GJF1(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ,const CString &strGuDao, int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 读取组合内部到数据库
	static int ReadNB(const CString & strZhname, int iFitFor=0);

	HWND m_hWndToSendMsg ;
	CProgressCtrl *m_ctrlProgressAll;
	CProgressCtrl *m_ctrlProgressCur;

	// 取得GCJ GJF继电器所在组合，及在组合中的序号（1~10）,继电器名称等信息
	int GetGCJGJFJdqInfo(int iJinLuType, const CString & strJZ, const CString & strFCZ, const CString & strCZ, const CString & strFJZ, const CString & strGuiDao, CString strZhNames[2], int iJDQIndexs[2], CString strJdqNames[2]);
	// 删除类似00-0端子
	void DelNoUserEnt(AcDbDatabase * & pDb);
	// 填充编码部分的扩展数据
	void FillXdataBianMa(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, int iUpOrDown=0);
	// 填充侧线GJF1块的扩展数据
	void FillXdataGJF1_CeXian(const CString &strJinLuName, const CString &strGuiDao, int iGCJIndex,  CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 二线制或者四线制
	int m_iTwoOrFourLine;
	CString m_strStation;
	CString m_strProjectName;
	CString m_strDwgSn;
	// 防雷是否放综合柜（否则放防雷分线柜）
	BOOL m_bIsFLInZH;
	void FillXdataFBJ_FS_EXAMPLE(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata);
	void FillXdataGDFS_GJF1(const CString &strJZ,  int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// 填充N+1模块数据
	void FillXdataNAdd1(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, CMapStringToString & mapToReplaceXdata, int iUpOrDown=0);
	// 将n+1上标示为tocon的端子关联转为id
	void TransNAdd1SnToId(AcDbDatabase *& pDb);
	static BOOL IsColInRange(const CString & strColIndex, const CString & strStrarCol, int iMergeCols);
	static void IncreaseCol(CString & strColIndex);
	static void IncreaseCol(CString & strColIndex, int iIncrease);
	// 分线盘端子默认（0）从数据库读取，=1时表示从某个值顺序计算取得
	int m_iFxpFrom;
	// 计算分线盘时起始端子
	CString m_strFxpStart;
	// 分线盘当前要分配的端子（分配后记得更新该值，以备下一个使用）
	CString m_strFxpCurDz;
	// 给strFxpFor变量分配分线盘端子
	int GetFxpDz(CString &strFxpFor);
	// 绘制电码化配线
	BOOL DoAllPx(const CString& strPath);
	void ReadDwgTie(void);
	void SetTieParentText(void);
	CString CutStringBack(const CString &strVal);
	AcDbDatabase *  GetCurDatabase(CArray < AcDbDatabase * , AcDbDatabase * >  *  pDbArray, int iCur_No , CString strType );
	BOOL Fill_Z_Table(CString strMySelf_Text ,CString strFill_Text , CString &strErrorText , long lCon_Cable_Id , long lCon_Cable_No=0, int iZorZZH=0);
	BOOL Fill_F_Table(CString strMySelf_Text ,CString strFill_Text , CString &strErrorText , long lCon_Cable_Id , long lCon_Cable_No);
	BOOL Divide_Z_Text(CString strText ,CString &strZ_Pre , int & iZ_No , int & iBlock , int & iCol , int & iRow );
	BOOL Divide_Text(CString strText, int &iFront, int &iBack);
	void Fill_Connect_Cable_Block(ads_point base_Pt, long lId);
	void Draw_PX_Z(CArray < AcDbDatabase * , AcDbDatabase * >  *pDbArray, CString &strError_Text, int iMode=0);
	void Draw_PX_ZY(CArray < AcDbDatabase * , AcDbDatabase * >  *pDbArray, CString &strError_Text, int iMode=0);
	void Draw_PX_ZZH(CArray < AcDbDatabase * , AcDbDatabase * >  *pDbArray, CString & strError_Text, int iMode=0);
	void Draw_PX_FXP(CArray < AcDbDatabase * , AcDbDatabase * >  *pDbArray, CString & strError_Text);
	void Draw_PX_HLP(CArray < AcDbDatabase * , AcDbDatabase * >  *pDbArray, CString & strError_Text);
	void Save_PX_Drawing (CString strPath ,CString strType , CArray<AcDbDatabase  *, AcDbDatabase * >  *pDbArray);

	// 填充零散电路数据-FBJ
	void FillXdataLingSan_FBJ(int iJinLuType, const CString &strJinLuName, CMapStringToString & mapStrToStrXdata);
	// 填充零散电路数据-XHFS
	void FillXdataLingSan_XHFS(const CString &strCZ_X, const CString &strCZ_S, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata);
	// 填充零散电路-GDFS
	void FillXdataLingSan_GDFS(const CString &strGuiDao, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata);
	void DrawAllPx( PXT_DATA * pData);
	void Fill_Circuit_Data_XXX(CCircuit_Data * pData);
	// 电化局组合柜，无综合柜
	void DrawZD(const CString &strSavePath);
	// 电化局组合柜 FB组合 取得一条线的进站信号机和FBJ名
	void GetLineInfo(const CString & strLine, CString & strJZ, CStringArray & strArrayFBJ);
	// 若没设置该值（N+1在ZY上的位置），就按顺序排在奇数最后一个位置
	CString m_strNPlusInZY;
	void ReadZD(const CString & strFileName) ;
	int FindZDLayer(const AcGePoint3d & pt_3d_base, CString & strBlockName, CStringArray & strArrayJdqName);
	int GetJdqNames_D(const CString & strBlkName, CStringArray & strArrayJdqNames);
	int FindCell_D(const AcGePoint3d& pt_3d_topleft, const AcGePoint3d& pt_3d_bottomright, CStringArray & strArrayCellContent);

	// 是否使用汇流排
	BOOL m_bUseHLP;
	void UpdateDbForHLP(void);
	void DrawCircuitD(const CString &strSavePath);

	// for 电化局
	void FillXdataNGL_D(int iGCJIndex, const CString & strGuiDao, const CString &strIsROrF, int iIsSendCode, CMapStringToString & mapStrToStrXdata, CMapStringToString &mapToAddXdata, const CString &strJinLuName);
	int GetZzhInfo_D(const CString & strGuiDao, int & iShelf, int & iLayer, int & iBox, CString &strRName, CString &strRSide1, CString &strRSide2, CString &strRSide3) ;

	void GetGCJInfo_D(const CString & strGuiDao, CString & strGCJName, int &iShelf, int &iLayer, int &iBox);
	void FillXdataGCJ_D(int iGuDaoIndex, int nGCJIndex, int nGCJJdqIndex, int iZLocIndex, const CString &strGCJName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iJinLuType);
	void FillXdataGCJZLoc_D(int iJinLuType, const CString & strLayerBlk, const CString & strZhShowName, const CString &strZLoc, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iMode=0) ;
	void FillXdataJMJ_0000_D(int iJinLuType, const CString &strJZ, const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	void FillXdataZBPU_FBJ_FS(int iJinLuType, const CString &strJZ, const CString &strFCZ, const CString &strCZ, const CString &strFJZ, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	void FillXdataZBPU_FBJ_FS_CeXian(int iJinLuType, const CString &strCZ, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	void FillXdataLiCi_GJF1_FuSi_D(int iLiCiGJF1GuiDaoIndex, const CString & strGuiDao, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iJinLuType);

	void FillXdataGCJGJF_GJF_D(int iJinLuType, int iGuiDaoIndex, int iMaxGCJ, const CString &strGuiDao, const CString &strJZ,  CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata );
	void FillXdataGCJGJF_GCJ_D(int iJinLuType, int iGuiDaoIndex, int iMaxGCJ, const CString &strGuiDao, const CString &strJZ,  CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	void FillXdataGCJGJF_JFMJ_D(int iJinLuType, const CString &strJZ, const CString &strFJZ, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	void FillXdataZBJH_JGJ_1LQ_D(int iJinLuType,  const CString &strJZXHJ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	void FillXdataGCJGJF_GDGJF1_D(const CString &strJZ,const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	void FillXdataLiCi_D(int iJinLuType, const CString &strJZ, const CString &strFCZ, const CString &strCZ, const CString &strFJZ,const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	void FillXdataFMJ_D(const CString &strXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	void GetCeXianGDInfo_D(int iJinLuType, const CString & strGuiDao, int &iShelf, int &iLayer, int &iBox) ;
	void FillXdataGJF2_CeXian(int iJinLuType, const CString &strJinLuName, const CString &strGuiDao, int iGCJIndex, CMapStringToString & mapToReplaceXdata,  CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	// 填充N+1模块数据
	void FillXdataNAdd1_D(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, CMapStringToString & mapToReplaceXdata, int iUpOrDown=0);
};
