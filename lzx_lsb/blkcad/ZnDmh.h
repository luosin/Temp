#pragma once

#define WM_USER_THREAD_PROGRESSRANGE WM_USER+0x100
#define WM_USER_THREAD_PROGRESSPOS WM_USER+0x101

struct CCircuit_Data
{
	CString strMySelf_Name;   //��Ӧ�źŻ�����
	CString strMySelf_G_Name;// ��Ӧ�����·���ƣ�
	CString strMySelf_Frequencry ; //Ƶ�� ;
	CString strMySelf_QY_Loc ;  //����Ƶ���ϵ�λ�á���
	CString strMySelf_QZH_Loc ;  //����ӿڹ�λ��
	CString strMySelf_QZ_Loc ;   //��ϼ�λ�á�;
	CString strMySelf_GLB_Loc ; //�����ѹ��������ӿڹ��ϵ�λ��;
	CString strMySelf_GLB_RD_Name ; //�����ѹ���ڶ������� ;
	CString strMySelf_Fs_RD_Name ; //�����ڶ������� ;
	CString strMySelf_Js_RD_Name ;//�����ڶ������� ;

	CString strGLB_RD_Start_Tie ; //�����ѹ���ڶ�����ʼ���Ӻš���
	CString strMySelf_FsJs_RD_Start_Tie ; //���͡������ڶ�����ʼ���Ӻ�;
	CString str1FsLoc;               //+1FS��λ�ã�

	int  iMySelf_FsJs_Index ; //���͡����ա���š�;

	CString strFsJs_Start_Tie ;
	CString strDd_Start_Tie ;
	CString strFsJs_Face_Start_Tie ;

	CString strFront_G_Name ; 
	CString strFront_QZ_Loc ; //ǰ������·����ϼ��ϵ�λ�� ;

	CString strBack_G_Name ;
	CString strBack_QZ_Loc ;//��������·����ϼ��ϵ�λ�� ;

	CString strCombine_G_Name ; //���������·����;
	CString strCombine_QY_Loc ; //��Ӧ��Ƶ���ϵ�λ��;
	CString strCombine_Frequencry ; //����Ƶ�� ;
	int iCombine_Js_Index ; //�������ͽ��ա����;
	CString  strCombine_Js_RD_Name ; //���������ڶ�������
	CString strCombine_Js_RD_Start_Tie ;//���������ڶ�����ʼ���Ӻš�;

	CString strStation ;
	CString strData ;
	CString strDrawingNo ;
	CString strDrawingName ;

	CString strQzh_Start_No ;  //����ӿڹ����ʼ��
	CString strQz_Start_No ; //��ϼ���ʼ�� ;

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
	//ǩ�ֿ�
	TCHAR chStationName[20] ;
	TCHAR chDrawingName[30] ;
	TCHAR chDrawingNo[30] ;
	TCHAR chDate[15] ;
};

// struct SystemSet
// {
// 	BOOL bHaveGLB; //�����ѹ���Ƿ��������ӿڹ���
// 	CString strQy_Pre ;
// 	CString strQz_Pre ;
// 	CString strQzh_Pre ;
// 	double dQZH_Font_Size ;
// 	CString strDraw_No_Pre;
// 	CString strProject_Name ;
// 
// 	CString str1FSXxLoc;
// 	CString str1FSSxLoc;
// 	int iTrain_Speed ; // == 1 Ϊ��160KM ; == 2 Ϊ��200KM ;
// 	int  i1LQ_Length ; //1LQ���� ;
// 	CString strCurData ; //��ǰʱ�� ;
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
	// �����������excel���
	static int ImportDmh(const CString & strXlsFileName);
	// Ԥ����������
	static int PreDoDmhImport(const CString & strXlsFileName);
	// ���ӹ��� iMode=1��ʾ�������Ի���ѡ������һ������
	static BOOL SetEntConnect(AcDbEntity *pEnt, long nSecondId , long nConCableId, int iMode=0) ;
	static BOOL GetEntFromId(AcDbEntity *& pEnt, long nEntId);
	static int SelectAcDbText(const TCHAR *chPrompt, ads_name &ent_name);
	static int SelectAcDbBlockReference(const TCHAR *chPrompt, const TCHAR * chBlockPre , ads_name &ent_name) ;
	static int ConTie(int nMode=0);
	static int InputCableType(const TCHAR *chPrompt, const TCHAR *chBlockPre, const AcGePoint3d pt_3d_insert, AcDbBlockReference *&pRef, ads_name &ent_name);

	static int SetPar(void);
	// ��������������ݿ⣨�������ɳ��壬���˹����ģ�����ٵ������ݿ⣩
	int ReImportDmh(const CString & strXlsFileName) ;
	int ComputeCircuit(void);
	// ��ȡstrGuiDao��ZZH���е���Ϣ
	int GetZzhInfo(const CString & strGuiDao, int & iShelf_FL, int & iLayer_FL, int & iBox_FL, int & iShelf, int & iLayer, int & iBox);
	// ��ȡstrGuiDao��������Ϣ���������Ϣ-�ܵ��
	int GetLsInfoR(const CString & strGuiDao, CString & strCircuitType, CString & strGJFPoint, CString & strGJFSide, CString & strGSide1, CString & strGSide2, CString & strFLoc1, CString & strFLoc2);
	// ��ȡ������Ϣ���������Ϣ-�͵��
	int GetLsInfoF(const CString & strGuiDao, CString & strPower1, CString & strPower2, CString & strFLoc1, CString & strFLoc2);
	// ��ȡ����ڲ��������
	int GetZhNbNode(const CString &strZhNb, const CString &strLocType, const CString & strNodeName,	int iLoc, CString & strNode);
	CString m_strSysBasePath;
	BOOL InsertDwgToDb(const CString strDwgFilePath, AcDbDatabase *& pDb, const AcGeMatrix3d & xform, const CMapStringToString & mapTypeToStr, const CMapStringToString & mapToAddXdata, const CString strRepType, int iIdAdd);
	BOOL InsertDwgToDb1(const CString strDwgFilePath, AcDbDatabase *& pDb, const AcGeMatrix3d & xform, const CMapStringToString & mapToReplaceXdata, const CMapStringToString & mapTypeToStr, const CMapStringToString & mapToAddXdata, const CString strRepType, int iIdAdd);
	// ��������
	int FillImport(const CString & strXlsFile);
	// �����ۺϹ�ܹ�ͼ
	void DrawZZH(const CString &strSavePath);
	// ������Ϲ�
	void DrawZ(const CString &strSavePath);
	// ����費��Ҫ������Ϲ�
	void CheckToAddZ(int & iZLocRow, int & iZIndex);
	// ��ѯĳ�������� ����������������
	void HasJinLuType(const CString & strLineName, BOOL & bHasFanFa, BOOL & bHasZhengFa, BOOL & bHasFanJie);
	// ����վ����Ƶ��ͼ
	void DrawZY(const CString &strSavePath);
	// ������ϼܵ����ݿ�
	void ReadZ(const CString & strFileName);
	// ������ָ�������Ͽ飨layer��
	int FindZLayer(const AcGePoint3d & pt_3d_base, CString & strBlockName, CString & strShowType, CString & strStartEndXhj1, CString & strStartEndXhj2, CStringArray & strArrayJdqName);
	AcDbDatabase *m_pDb;
	// ���������1~10�̵���λ�÷��õ�Ԫ��
	int FindCell(const CString & strToFindBlk, const AcGePoint3d& pt_3d_topleft, const AcGePoint3d& pt_3d_bottomright, CString & strCellContent);
	// ȡ��ĳ�����1~10�̵���������
	int GetJdqNames(const CString & strBlkName, CStringArray & strArrayJdqNames);
	// �����ۺϹ�ͼ
	void ReadZZH(const CString & strFileName);
	int FindZZHLayer(const AcGePoint3d & pt_3d_base, CString & strBlkName);
	
	// ����վ����Ƶ��
	void ReadZY(const CString & strFileName);
	// ����ZY��Ƶ��Ԫ��
	int FindZYCell(const AcGePoint3d & pt_3d_topleft, const AcGePoint3d & pt_3d_bottomright, CString & strJinLuName, CString & strFreq1, CString & strFreq2);
	// ���Ƶ�·ͼ
	void DrawCircuit(const CString &strSavePath);
	// ���һ���ڸ���ģ�����չ����, iGCJIndexΪ0��ʾ�ɵ��޷ָ�ʱGCJ1��������ʾ1GCJ~10GCJ
	void FillXdataNGL(int iGCJIndex, const CString & strGuiDao, const CString &strIsROrF, int iIsSendCode, CMapStringToString & mapStrToStrXdata, CMapStringToString &mapToAddXdata, const CString &strJinLuName);
	// ���GCJ_0000��strGuDao��Ϊ��ʱ��ʾ�ɵ��޷ָ�����չ����
	void FillXdataGCJ_0000(int iJinLuType, const CString & strGuDao, const CString	&strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���JMJ�����չ����
	void FillXdataJMJ_0000(int iJinLuType, int iGuDaoFenGe, const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;

	// ��ϼ�����ǰ׺
	CString m_strPreZ;
	// ��Ƶ������ǰ׺
	CString m_strPreZY;
	// �ۺϹ�����ǰ׺
	CString m_strPreZZH;

	//���GCJ��չ����
	void FillXdataGCJ(int nGCJIndex, int nGCJJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe,int iJinLuType);
	// ���GCJ���ڵ����λ��ͼ�����չ����
	void FillXdataGCJZLoc(int iJinLuType, const CString & strLayerBlk, const CString & strZhShowName,const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iMode=0);
	// ���TFGZ�����չ����
	void FillXdataTFGZ(const CString & strJinLuName, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, int iJinLuType, int iRunDirect=0);
	// ���ZJC-FMJ�����չ����
	void FillXdataZFJC_FMJ(int iLineNum, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���FBJ�����չ����
	void FillXdataFBJ(int iJinLuType, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���lici_jfmj����չ����
	void FillXdataLiCi_JFMJ(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// ��䷢�Ϳ����չ����
	void FillXdataFS(int iUpOrDwon, int iJinLuType, const CString &strJinLuName, CMapStringToString & mapReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���FuSi-GJF�����չ����
	void FillXdataFuSi_GJF(const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// ����fusi-xjf����չ����
	void FillXdataFuSi_XJF(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���GCJGJF-GJF�����չ����
	void FillXdataGCJGJF_GJF(int iJinLuType, int iGCJIndex, int iGJFIndex, int iMaxGCJ,const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, const CString &strGuiDao,int nGJFJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// ���GCJGJF-GCJ�����չ����
	void FillXdataGCJGJF_GCJ(int iJinLuType, int iGCJIndex,  int iMaxGCJ, const CString &strGuiDao, int nGCJJdqIndex, const CString &strZhName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe, CString strJZ=_T(""), CString strFCZ=_T(""), CString strCZ=_T(""), CString strFJZ=_T(""));
	
	// ���GCJGJF-JFMJ_0000����չ����
	void FillXdataGCJGJF_JFMJ_0000(const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	// ���GCJGJF_JFMJ_1000 �����չ����
	void FillXdataGCJGJF_JFMJ_1000(int iLineNum, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���GCJGJF-JFMJ_2000�����չ����
	void FillXdataGCJGJF_JFMJ_2000(int iLineIndex, const CString &strJZ, const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	// ���GCJGJF_JFMJ_3000����չ����
	void FillXdataGCJGJF_JFMJ_3000(int iMaxGCJ, const CString &strStartEndXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iGuDaoFenGe);
	
	// ���zbjh-3jgj�����չ����
	void FillXdataZBJH_JGJ_1LQ(int iJinLuType,const CString &strJZXHJ, int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ȡ����·�������Ϣ����վ����վ......
	void GetLineInfo(const CString & strLineName, CString & strJZXHJ, CString & strFCZXHJ, CString & strCZXHJ, CString & strFJZXHJ);
	// ���+1FS����չ����
	void FillXdataFSPLUS(CMapStringToString & mapStrToStrXdata);
	// ��ͼֽ�е�MySn����Ϣת����Id���Ա�����
	void TransMySnToId(AcDbDatabase* & pDb);
	// ����ָ����չ���ݵ�ʵ��
	int GetTie(const CString & strToFindApp, const CString & strToFindXdata, UINT& iHandle);
	// ɾ����ͼ�����е���չ���ݣ���Ҫ��Sn��
	void DelSn(AcDbDatabase * & pDb);
	// ��䣨FJC-FMJ�������չ����
	void FillXdataFJC_FMJ(int iLineIndex, const CString &strJZ, const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata) ;
	// ����GCJ-GJF��ˣ�����ZBJH��
	void FillXdataZJC_GJF1(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ,const CString &strGuDao, int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ��ȡ����ڲ������ݿ�
	static int ReadNB(const CString & strZhname, int iFitFor=0);

	HWND m_hWndToSendMsg ;
	CProgressCtrl *m_ctrlProgressAll;
	CProgressCtrl *m_ctrlProgressCur;

	// ȡ��GCJ GJF�̵���������ϣ���������е���ţ�1~10��,�̵������Ƶ���Ϣ
	int GetGCJGJFJdqInfo(int iJinLuType, const CString & strJZ, const CString & strFCZ, const CString & strCZ, const CString & strFJZ, const CString & strGuiDao, CString strZhNames[2], int iJDQIndexs[2], CString strJdqNames[2]);
	// ɾ������00-0����
	void DelNoUserEnt(AcDbDatabase * & pDb);
	// �����벿�ֵ���չ����
	void FillXdataBianMa(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, int iUpOrDown=0);
	// ������GJF1�����չ����
	void FillXdataGJF1_CeXian(const CString &strJinLuName, const CString &strGuiDao, int iGCJIndex,  CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// �����ƻ���������
	int m_iTwoOrFourLine;
	CString m_strStation;
	CString m_strProjectName;
	CString m_strDwgSn;
	// �����Ƿ���ۺϹ񣨷���ŷ��׷��߹�
	BOOL m_bIsFLInZH;
	void FillXdataFBJ_FS_EXAMPLE(int iJinLuType, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata);
	void FillXdataGDFS_GJF1(const CString &strJZ,  int iToConGJFIndex, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata);
	// ���N+1ģ������
	void FillXdataNAdd1(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, CMapStringToString & mapToReplaceXdata, int iUpOrDown=0);
	// ��n+1�ϱ�ʾΪtocon�Ķ��ӹ���תΪid
	void TransNAdd1SnToId(AcDbDatabase *& pDb);
	static BOOL IsColInRange(const CString & strColIndex, const CString & strStrarCol, int iMergeCols);
	static void IncreaseCol(CString & strColIndex);
	static void IncreaseCol(CString & strColIndex, int iIncrease);
	// �����̶���Ĭ�ϣ�0�������ݿ��ȡ��=1ʱ��ʾ��ĳ��ֵ˳�����ȡ��
	int m_iFxpFrom;
	// ���������ʱ��ʼ����
	CString m_strFxpStart;
	// �����̵�ǰҪ����Ķ��ӣ������ǵø��¸�ֵ���Ա���һ��ʹ�ã�
	CString m_strFxpCurDz;
	// ��strFxpFor������������̶���
	int GetFxpDz(CString &strFxpFor);
	// ���Ƶ��뻯����
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

	// �����ɢ��·����-FBJ
	void FillXdataLingSan_FBJ(int iJinLuType, const CString &strJinLuName, CMapStringToString & mapStrToStrXdata);
	// �����ɢ��·����-XHFS
	void FillXdataLingSan_XHFS(const CString &strCZ_X, const CString &strCZ_S, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata);
	// �����ɢ��·-GDFS
	void FillXdataLingSan_GDFS(const CString &strGuiDao, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata);
	void DrawAllPx( PXT_DATA * pData);
	void Fill_Circuit_Data_XXX(CCircuit_Data * pData);
	// �绯����Ϲ����ۺϹ�
	void DrawZD(const CString &strSavePath);
	// �绯����Ϲ� FB��� ȡ��һ���ߵĽ�վ�źŻ���FBJ��
	void GetLineInfo(const CString & strLine, CString & strJZ, CStringArray & strArrayFBJ);
	// ��û���ø�ֵ��N+1��ZY�ϵ�λ�ã����Ͱ�˳�������������һ��λ��
	CString m_strNPlusInZY;
	void ReadZD(const CString & strFileName) ;
	int FindZDLayer(const AcGePoint3d & pt_3d_base, CString & strBlockName, CStringArray & strArrayJdqName);
	int GetJdqNames_D(const CString & strBlkName, CStringArray & strArrayJdqNames);
	int FindCell_D(const AcGePoint3d& pt_3d_topleft, const AcGePoint3d& pt_3d_bottomright, CStringArray & strArrayCellContent);

	// �Ƿ�ʹ�û�����
	BOOL m_bUseHLP;
	void UpdateDbForHLP(void);
	void DrawCircuitD(const CString &strSavePath);

	// for �绯��
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
	// ���N+1ģ������
	void FillXdataNAdd1_D(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, CMapStringToString & mapToReplaceXdata, int iUpOrDown=0);
};
