// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ZhfPalette.h"
#include "ZhfPaletteSet.h"
#include "ZhfBlkRefJig.h"
#include "UserTools.h"
#include "PmtSetRbDlg.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("zhf")

//Global
bool g_bIsTukuFirst = true;
bool g_bAddJyjWhenInsertXhj = true ;
CMDIFrameWnd* g_pAcadFrame = NULL ;
CGsPreviewCtrl* g_pCtrlGsPreview = NULL ;
CArray<CZhfPaletteSet*, CZhfPaletteSet*&> g_pArrayZhfPaletteSet ;
bool g_bIsToolBarLock = false ;
bool bWatchSelectDone = false ;

void WatchSelect(const MSG * pMsg) ;

void WatchSelect(const MSG * pMsg)
{

	if (pMsg->message==WM_LBUTTONUP)
	{
		//acutPrintf(_T("\nin zhftool: you press left mouse button:%d-%d"), pMsg->pt.x, pMsg->pt.y) ;
	}
	else if (pMsg->message == WM_KEYDOWN &&(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL))
	{
		//acutPrintf(_T("\nin zhftool: you press esc")) ;

		if (g_pCtrlGsPreview!=NULL)
		{
			g_pCtrlGsPreview->SetSelect(false) ;
			CZhfPalette* pPalette = (CZhfPalette*)g_pCtrlGsPreview->GetParent() ;
			pPalette->m_iCurSel = -1 ;
			g_pCtrlGsPreview = NULL ;
			g_bIsToolBarLock = false ;
		}
	}
}

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CzhftoolApp : public AcRxArxApp {

public:
	CzhftoolApp () : AcRxArxApp () {}

	static void InitApplication(void);
	static void UnloadApplication(void);

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		if (!InitSystem())
		{
			return AcRx::kRetError ;
		}
		InitApplication() ;

		if (!bWatchSelectDone)
		{
			if (acedRegisterWatchWinMsg(WatchSelect)==FALSE)
			{
				acedPrompt(_T("\nhook can't be registered.")) ;
			}
			else 
			{
				acedPrompt(_T("hook been registered!\n"));  
				bWatchSelectDone = TRUE;
			}
		}

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		UnloadApplication() ;

		if(bWatchSelectDone == TRUE)
		{
			acedRemoveWatchWinMsg(WatchSelect);
			acutPrintf(_T("\nHook has been removed!\n"));
		}

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
		
	}


	// ----- zhfzhftool._drawblk command (do not rename)
	static void zhfzhftool_drawblk(void)
	{
		// Add your code for command zhfzhftool._drawblk here
		if (!g_bIsTukuFirst)
		{
			if (g_pCtrlGsPreview!=NULL)
			{
				g_bIsToolBarLock = true ;
				CZhfPalette* pPalette = (CZhfPalette*)g_pCtrlGsPreview->GetParent() ;

				CZhfBlkRefJig* pJig = new CZhfBlkRefJig(g_pCtrlGsPreview) ;
				pJig->DoIt() ;
				delete pJig ;
				g_bIsToolBarLock = false ;
				if (g_pCtrlGsPreview!=NULL) //�п��ܰ���ESC��������gsctrl�Ѿ���null
				{
					g_pCtrlGsPreview->SetSelect(false) ;
					pPalette->m_iCurSel = -1 ;
				}
			}
		}
		else
		{
			acutPrintf(_T("\nû�г�ʼ��ͼ�⣬��������tuku��dttk")) ;
		}

	}

	// ----- zhfzhftool._tuku command (do not rename)
	static void zhfzhftool_tuku(void)
	{
		// Add your code for command zhfzhftool._tuku here
		if (g_bIsTukuFirst)
		{
			InitApplication() ;
		}
		else
		{
			CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();

			if (g_pArrayZhfPaletteSet.GetCount()>0)
			{
				CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(0) ;
				if (pSet->IsWindowVisible())
				{
					pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);
				}
				else
				{
					pAcadFrame->ShowControlBar(pSet, TRUE, FALSE);
				}	
			}
			else
			{
				acutPrintf(_T("\nͼ�����û�д����򴴽�ʧ�ܣ��޷���ʾ��")) ;
			}
		}
	}

	// ----- zhfzhftool._zhftools command (do not rename)
	static void zhfzhftool_zhftools(void)
	{
		// Add your code for command zhfzhftool._zhftools here
		CUserTools::ZhfTools() ;
	}
	// -----------------------------------------------------------------------------
	virtual AcRx::AppRetCode On_kUnloadDwgMsg(void * pkt);

	// ----- zhfzhftool._zrotate command (do not rename)
	static void zhfzhftool_zrotate(void)
	{
		// Add your code for command zhfzhftool._zrotate here
		CUserTools::RotationEnt() ;
	}

	// ----- zhfzhftool._zmirror command (do not rename)
	static void zhfzhftool_zmirror(void)
	{
		// Add your code for command zhfzhftool._zmirror here
		CUserTools::MirrorEnt() ;
	}

	// ----- zhfzhftool._zadjustpl command (do not rename)
	static void zhfzhftool_zadjustpl(void)
	{
		// Add your code for command zhfzhftool._zadjustpl here
		CUserTools::AdjustPlineVertex() ;
	}

	// ----- zhfzhftool._rbset command (do not rename)
	static void zhfzhftool_rbset(void)
	{
		// Add your code for command zhfzhftool._rbset here
		CAcModuleResourceOverride myResources ;
		CPmtSetRbDlg dlg ;
		dlg.DoModal() ;
	}

	// ----- zhfzhftool._rb command (do not rename)
	static void zhfzhftool_rb(void)
	{
		// Add your code for command zhfzhftool._rb here
		CUserTools::ReplaceEnt() ;	
	}

	// ----- zhfzhftool._moblkatt command (do not rename)
	static void zhfzhftool_moblkatt(void)
	{
		// Add your code for command zhfzhftool._moblkatt here
		//�����޸Ŀ�����
		int iMode = 0 ;
		if(RTNORM==acedGetInt(_T("\nInput int(0:ȫͼ�޸�/1:ѡ���޸�):"), &iMode))
		{
			CUserTools::SetBlkAtt(iMode) ;
		}
	}

	// ----- zhfzhftool._addvert command (do not rename)
	static void zhfzhftool_addvert(void)
	{
		// Add your code for command zhfzhftool._addvert here
		CUserTools::AddVertex() ;
	}

	// ----- zhfzhftool._reverseline command (do not rename)
	static void zhfzhftool_reverseline(void)
	{
		// Add your code for command zhfzhftool._reverseline here
		CUserTools::ReverseLinePoint() ;
	}

	// ----- zhfzhftool._ad command (do not rename)
	static void zhfzhftool_ad(void)
	{
		// Add your code for command zhfzhftool._ad here
		//�༭�ı�����ɫ���ܣ�
		CUserTools::MyEdit() ;
	}

	// ----- zhfzhftool._utext command (do not rename)
	static void zhfzhftool_utext(void)
	{
		// Add your code for command zhfzhftool._utext here
		//�����ı�
		CUserTools::MyText() ;
	}

	// ----- zhfzhftool._btext command (do not rename)
	static void zhfzhftool_btext(void)
	{
		// Add your code for command zhfzhftool._btext here
		//���������ı�
		CUserTools::BText() ;
	}

	// ----- zhfzhftool._tt command (do not rename)
	static void zhfzhftool_tt(void)
	{
		// Add your code for command zhfzhftool._tt here
		//�����ı��������ı�ת��Ϊ�����ı�
		CUserTools::AtMtToTxt() ;
	}

	// ----- zhfzhftool._fr command (do not rename)
	static void zhfzhftool_fr(void)
	{
		// Add your code for command zhfzhftool._fr here
		//�����滻�ı�
		CUserTools::MyFind() ;
	}

	// ----- zhfzhftool._aligntext command (do not rename)
	static void zhfzhftool_aligntext(void)
	{
		// Add your code for command zhfzhftool._aligntext here
		//�����ı�
		CUserTools::AlignText() ;
	}

	// ----- zhfzhftool._motext command (do not rename)
	static void zhfzhftool_motext(void)
	{
		// Add your code for command zhfzhftool._motext here
		//�����޸����ִ�С��style�����뷽ʽ
		CUserTools::MoText() ;
	}

	// ----- zhfzhftool._bedit command (do not rename)
	static void zhfzhftool_bedit(void)
	{
		// Add your code for command zhfzhftool._bedit here
		//�����޸��ı�
		CUserTools::BEdit() ;
	}

	// ----- zhfzhftool._delnulltext command (do not rename)
	static void zhfzhftool_delnulltext(void)
	{
		// Add your code for command zhfzhftool._delnulltext here
		//ɾ�����ı�
		CUserTools::DelNullText() ;
	}

	// ----- zhfzhftool._replaceblock command (do not rename)
	static void zhfzhftool_replaceblock(void)
	{
		// Add your code for command zhfzhftool._replaceblock here
		// �滻��
		CUserTools::ReplaceBlock() ;
	}

	// ----- zhfzhftool._dwgtoxls command (do not rename)
	static void zhfzhftool_dwgtoxls(void)
	{
		// Add your code for command zhfzhftool._dwgtoxls here
		//cad�б��ת����excel�ı��
		CUserTools::DwgToXls() ;
	}

	// ----- zhfzhftool._xlstodwg command (do not rename)
	static void zhfzhftool_xlstodwg(void)
	{
		// Add your code for command zhfzhftool._xlstodwg here
		// excel���ת����cad�еı��
		CUserTools::XlsToDwg() ;
	}

	// ----- zhfzhftool._gethw command (do not rename)
	static void zhfzhftool_gethw(void)
	{
		// Add your code for command zhfzhftool._gethw here
		//�ڽ�excelת����cad���ʱ�����Ӧheight��width
		CUserTools::GetHW() ;
	}

	// ----- zhfzhftool._setxdata command (do not rename)
	static void zhfzhftool_setxdata(void)
	{
		// Add your code for command zhfzhftool._setxdata here
		// ������չ����xdata
		CUserTools::SetXdata() ;
	}

	// ----- zhfzhftool._showxdata command (do not rename)
	static void zhfzhftool_showxdata(void)
	{
		// Add your code for command zhfzhftool._showxdata here
		// ��ʾ��չ����xdata
		CUserTools::ShowXdata() ;
	}

	// ----- zhfzhftool._delxdata command (do not rename)
	static void zhfzhftool_delxdata(void)
	{
		// Add your code for command zhfzhftool._delxdata here
		//ɾ����չ����
		CUserTools::DelXdata() ;
	}

	// ----- zhfzhftool._mofilename command (do not rename)
	static void zhfzhftool_mofilename(void)
	{
		// Add your code for command zhfzhftool._mofilename here
		//�����޸��ļ���
		CUserTools::MoFileName() ;
	}

	// ----- zhfzhftool._refreshdwg command (do not rename)
	static void zhfzhftool_refreshdwg(void)
	{
		// Add your code for command zhfzhftool._refreshdwg here
		//ˢ��ͼֽ����Ҫ��ĳЩ����д�������û����ȷָ������㣬����ͼ����ʾ����ȷ���ƶ�һ�¿ɽ�����ڵ��ĵ�ģʽ����Ч
		CUserTools::RefreshDwg() ;
	}

	// ----- zhfzhftool._explodespe command (do not rename)
	static void zhfzhftool_explodespe(void)
	{
		// Add your code for command zhfzhftool._explodespe here
		//ը����A$��ͷ�����Ŀ�
		CUserTools::ExplodeSpeBlk() ;
	}

	// ----- zhfzhftool._sumlinelen command (do not rename)
	static void zhfzhftool_sumlinelen(void)
	{
		// Add your code for command zhfzhftool._sumlinelen here
		//ͳ��ѡ����ߵĳ��ȣ���������AcDbCurve���ߣ�
		CUserTools::SumLineLen() ;
	}

	// ----- zhfzhftool._sumdianlan command (do not rename)
	static void zhfzhftool_sumdianlan(void)
	{
		// Add your code for command zhfzhftool._sumdianlan here
		//ͳ�Ƶ���
		CUserTools::SumDianLan() ;
	}

	// ----- zhfzhftool._showblkatt command (do not rename)
	static void zhfzhftool_showblkatt(void)
	{
		// Add your code for command zhfzhftool._showblkatt here
		//��ʾ�����ؿ�����
		CUserTools::ShowBlkAtt() ;
	}

	// ----- zhfzhftool._checksameid command (do not rename)
	static void zhfzhftool_checksameid(void)
	{
		// Add your code for command zhfzhftool._checksameid here
		CUserTools::CheckSameId() ;
	}

	// ----- zhfzhftool._unchecksameid command (do not rename)
	static void zhfzhftool_unchecksameid(void)
	{
		// Add your code for command zhfzhftool._unchecksameid here
		CUserTools::UnCheckSameId() ;
	}

	// ----- zhfzhftool._hidexdata command (do not rename)
	static void zhfzhftool_hidexdata(void)
	{
		// Add your code for command zhfzhftool._hidexdata here
		CUserTools::HideXdata() ;
	}

	// ----- zhfzhftool._deloneapp command (do not rename)
	static void zhfzhftool_deloneapp(void)
	{
		// Add your code for command zhfzhftool._deloneapp here
		//ɾ��ָ��app
		CUserTools::DelOneApp() ;
	}

	// ----- zhfzhftool._addnum command (do not rename)
	static void zhfzhftool_addnum(void)
	{
		// Add your code for command zhfzhftool._addnum here
		CUserTools::AddNum() ;
	}

	// ----- zhfzhftool._repappval command (do not rename)
	static void zhfzhftool_repappval(void)
	{
		// Add your code for command zhfzhftool._repappval here
		CUserTools::ReplaceAppValue() ;
	}

	// ----- zhfzhftool._savecircuit command (do not rename)
	static void zhfzhftool_savecircuit(void)
	{
		// Add your code for command zhfzhftool._savecircuit here
		CUserSystem mysys ;
		mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
	}

	// ----- zhfzhftool._zlist command (do not rename)
	static void zhfzhftool_zlist(void)
	{
		// Add your code for command zhfzhftool._zlist here
		// ��ʾʵ��Handle
		CUserTools::ZList() ;
	}

	// ----- zhfzhftool._topline command (do not rename)
	static void zhfzhftool_topline(void)
	{
		// Add your code for command zhfzhftool._topline here
		CUserTools::ConvertToPolyline() ;
	}

	// ----- zhfzhftool._joinline command (do not rename)
	static void zhfzhftool_joinline(void)
	{
		// Add your code for command zhfzhftool._joinline here
		CUserTools::JoinLine() ;
	}

	// ----- zhfzhftool._addatt command (do not rename)
	static void zhfzhftool_addatt(void)
	{
		// Add your code for command zhfzhftool._addatt here
		CUserTools::AddAtt() ;
	}

	// ----- zhfzhftool._dttk command (do not rename)
	static void zhfzhftool_dttk(void)
	{
		// Add your code for command zhfzhftool._dttk here
		if (g_bIsTukuFirst)
		{
			InitApplication() ;
		}
		else
		{
			CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();

			if (g_pArrayZhfPaletteSet.GetCount()>1)
			{
				CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(1) ;
				if (pSet->IsWindowVisible())
				{
					pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);
				}
				else
				{
					pAcadFrame->ShowControlBar(pSet, TRUE, FALSE);
				}	
			}
			else
			{
				acutPrintf(_T("\nͼ�����û�д����򴴽�ʧ�ܣ��޷���ʾ��")) ;
			}
		}
	}

	// ----- zhfzhftool._setplwidth command (do not rename)
	static void zhfzhftool_setplwidth(void)
	{
		// Add your code for command zhfzhftool._setplwidth here
		CUserTools::SetPlineWidth() ;
	}

	// ----- zhfzhftool._rptk command (do not rename)
	static void zhfzhftool_rptk(void)
	{
		// Add your code for command zhfzhftool._rptk here
		CUserTools::ReplaceTUKU() ;
	}
	// ----- zhfzhftool._xlstolsb command (do not rename)
	static void zhfzhftool_xlstolsb(void)
	{
		// Add your code for command zhfzhftool._xlstolsb here
		//��xlsת�����������
		// excel���ת����cad�еı��
		CUserTools::XlsToLsb();
	}
} ;


void CzhftoolApp::InitApplication(void)
{
	CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();

	CString strPaletteSetNames[] = {_T("ƽ��ͼ��"), _T("����ͼ��")} ;

	for (int iIndexSet=0; iIndexSet<2; iIndexSet++)
	{
		CZhfPaletteSet* pSet = new CZhfPaletteSet ;
		CRect rect(0, 0, 320, 300);
		pSet->Create(strPaletteSetNames[iIndexSet], WS_OVERLAPPED|WS_DLGFRAME, rect, pAcadFrame,	PSS_EDIT_NAME | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP |	PSS_CLOSE_BUTTON) ;

		if (iIndexSet==0)
		{
			CString strFolds[] = {_T("xhj\\lj"), _T("xhj\\lf"), _T("xhj\\lfn"),  _T("xhj\\dc"), _T("xhj\\yg"), _T("xhj\\tg"), _T("xhj\\fs"), _T("xhj\\dt"), _T("dc"), _T("jyj"), _T("box"), _T("dltb"), _T("jdq"), _T("sign"), _T("bg"), _T("dz"), _T("conti"), _T("pmt")} ;
			CString strPaletteNames[] = {_T("�ӳ�"),  _T("����"), _T("����2"), _T("����"), _T("Ԥ��"), _T("ͨ��"), _T("��ʾ"), _T("����"), _T("����"), _T("��Ե��"), _T("���"), _T("��·"),  _T("�̵���"), _T("ǩ����"), _T("���"), _T("����"), _T("���ӷ�"), _T("����")} ;

			int iFilterModes[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0} ;
			for (int i=0; i<18; i++)
			{
				CZhfPalette* pZhfPalette = new CZhfPalette() ;
				pZhfPalette->m_iFilterMode = iFilterModes[i] ;
				CString strFilePath ;
				CStringArray strArrayFile ;
				strFilePath.Format(_T("%s\\support\\%s"), g_strBasePath, strFolds[i]) ;
				CBlkUtility::FindFile(strFilePath, _T("dwg"), strArrayFile, 1) ;
				CBlkUtility::SortStringArray(strArrayFile) ;
				if (i==8)//��ת�޻�ͼ�μ������������
				{
					strFilePath.Format(_T("%s\\support\\zzj"), g_strBasePath) ;
					CStringArray strArrayPlus ;
					CBlkUtility::FindFile(strFilePath, _T("dwg"), strArrayPlus, 1) ;
					strArrayFile.Append(strArrayPlus) ;
				}
				if (i==12)
				{
					CStringArray strArrayFileNew ;
					for (int iJqdIndex=0; iJqdIndex<strArrayFile.GetCount(); iJqdIndex++)
					{
						if (strArrayFile.GetAt(iJqdIndex).Find(_T("Bold"))==-1)
						{
							strArrayFileNew.Add(strArrayFile.GetAt(iJqdIndex)) ;
						}
					}
					strArrayFile.RemoveAll() ;
					strArrayFile.Copy(strArrayFileNew) ;
				}
				pZhfPalette->m_strArrayFile.Copy(strArrayFile) ;
				CString strFileName ;
				for (int j=0; j<strArrayFile.GetCount(); j++)
				{
					CBlkUtility::GetFileName(strArrayFile.GetAt(j), strFileName) ;
					if (i<7)
					{
						CStringArray strArrayText ;
						CBlkUtility::DivideString(strFileName, _T("_"), strArrayText) ;
						strFileName.Format(_T("%s_%s"), strArrayText.GetAt(1), strArrayText.GetAt(2)) ;
					}
					pZhfPalette->m_strArrayFileName.Add(strFileName) ;
				}

				//if (i==1)
				{
					pZhfPalette->m_iCtrlWidth=80 ;
					pZhfPalette->m_iCtrlHeight = 80 ;
				}

				pZhfPalette->Create(WS_CHILD | WS_VISIBLE|WS_VSCROLL,strPaletteNames[i], pSet, PS_EDIT_NAME);
				//pZhfPalette->AddControls() ;
				pSet->AddPalette(pZhfPalette) ;
			}

			// Finally show the palette set
			pSet->EnableDocking(CBRS_ALIGN_ANY);
			pSet->RestoreControlBar();
		}
		else if (iIndexSet==1)
		{
			CString strFolds[] = {_T("dt")} ;
			CString strPaletteNames[] = {_T("����")} ;

			int iFilterModes[] = {0} ;
			for (int i=0; i<1; i++)
			{
				CZhfPalette* pZhfPalette = new CZhfPalette() ;
				pZhfPalette->m_iFilterMode = iFilterModes[i] ;
				CString strFilePath ;
				CStringArray strArrayFile ;
				strFilePath.Format(_T("%s\\support\\%s"), g_strBasePath, strFolds[i]) ;
				CBlkUtility::FindFile(strFilePath, _T("dwg"), strArrayFile, 1) ;
				CBlkUtility::SortStringArray(strArrayFile) ;

				pZhfPalette->m_strArrayFile.Copy(strArrayFile) ;
				CString strFileName ;
				for (int j=0; j<strArrayFile.GetCount(); j++)
				{
					CBlkUtility::GetFileName(strArrayFile.GetAt(j), strFileName) ;
// 					if (i<7)
// 					{
// 						CStringArray strArrayText ;
// 						CBlkUtility::DivideString(strFileName, _T("_"), strArrayText) ;
// 						strFileName.Format(_T("%s_%s"), strArrayText.GetAt(1), strArrayText.GetAt(2)) ;
// 					}
					pZhfPalette->m_strArrayFileName.Add(strFileName) ;
				}

				//if (i==1)
				{
					pZhfPalette->m_iCtrlWidth=80 ;
					pZhfPalette->m_iCtrlHeight = 80 ;
				}

				pZhfPalette->Create(WS_CHILD | WS_VISIBLE|WS_VSCROLL,strPaletteNames[i], pSet, PS_EDIT_NAME);
				pSet->AddPalette(pZhfPalette) ;
			}

			// Finally show the palette set
			pSet->EnableDocking(CBRS_ALIGN_ANY);
			pSet->RestoreControlBar();

		}

		pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);	
		g_pArrayZhfPaletteSet.Add(pSet) ;
	}	

	g_bIsTukuFirst = false ;
	//�����źŻ�ʱ�Ƿ�Ӿ�Ե��
	DWORD iAddJyj = 1 ;
	CBlkUtility::QueryValueReg(_T("ZHFTOOLS"), _T("AddJyjWhenInsertXhj"), iAddJyj) ;
	g_bAddJyjWhenInsertXhj = (iAddJyj==1?true:false) ;
}

void CzhftoolApp::UnloadApplication(void)
{
	for (int i=0; i<g_pArrayZhfPaletteSet.GetCount(); i++)
	{
		CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(i) ;
		pSet->DestroyWindow() ;
		delete pSet ;
	}
}
AcRx::AppRetCode CzhftoolApp::On_kUnloadDwgMsg(void * pkt)
{
	CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();
	AcApDocManager *pDocMgr = acDocManager ;
	int iDocCount = pDocMgr->documentCount() ;
	if (iDocCount==1)
	{
		for (int i=0; i<g_pArrayZhfPaletteSet.GetCount(); i++)
		{
			CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(i) ;
			if (pSet->IsWindowVisible())
			{
				pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);
			}
		}
	}	

	AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadDwgMsg (pkt) ;
	return (retCode) ;
}

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CzhftoolApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _drawblk, drawblk, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _tuku, tuku, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zhftools, zhftools, ACRX_CMD_TRANSPARENT, NULL)
// -----------------------------------------------------------------------------

ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zrotate, zr, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zmirror, zm, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zadjustpl, zadpl, ACRX_CMD_TRANSPARENT, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _rbset, rbset, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _rb, rb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _moblkatt, moblkatt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addvert, addvert, ACRX_CMD_TRANSPARENT, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _reverseline, reverseline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _ad, ad, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _utext, utext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _btext, btext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _tt, tt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _fr, fr, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _aligntext, aligntext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _motext, motext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _bedit, bedit, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _delnulltext, delnulltext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _replaceblock, replaceblock, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _dwgtoxls, dwgtoxls, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _xlstodwg, xlstodwg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _gethw, gethw, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _setxdata, setxdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _showxdata, showxdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _delxdata, delxdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _mofilename, mofilename, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _refreshdwg, refreshdwg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _explodespe, explodespe, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _sumlinelen, sumlinelen, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _sumdianlan, sumdianlan, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _showblkatt, showblkatt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _checksameid, checksameid, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _unchecksameid, unchecksameid, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _hidexdata, hidexdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _deloneapp, deloneapp, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addnum, addnum, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _repappval, repappval, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _savecircuit, savecircuit, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zlist, zlist, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _topline, topline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _joinline, joinline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _xlstolsb, xlstolsb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addatt, addatt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _dttk, dttk, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _setplwidth, setplwidth, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _rptk, rptk, ACRX_CMD_TRANSPARENT, NULL)
