#include "stdafx.h"
#include "znsxt.h"
#include "CableInfoDlg.h"
#include "XhlCableInfoDlg.h"
#include "znutility.h"
#include "PolylineUtil.h"
#include "ConvertUtil.h"
#include "EntPropertyDlg.h"
#include "BlockUtil.h"

#include "..\blkuserdll\ViewExtents.h"
#include "BoxNodeEnt.h"

//������������յ�����Ѱ�ҽ�·���ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
void CZnSxt::GetPathGuiDao4( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &DaochaNum,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	CStringArray nArrayDaoChaNum;
	nArrayDaoChaNum.Copy(DaochaNum);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//ѹ���Ӧ�ĵ�����λ
		acutPrintf(_T("\n ��������"));
		for(int i=0;i<sVectorPath.size();i++)
		{
			//����������Ľ��
			acutPrintf(_T("\t��%d ���������Ϊ%s"),i,sVectorPath[i].GetString());
		}
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//��ӡ����������λ
		acutPrintf(_T("\n ��������"));
		for(int k=0;k<sDaocha.size();k++)
		{
			if(sDaocha[k].GetString()!=_T(""))
			{
				acutPrintf(_T("\t��%s"),sDaocha[k].GetString());
			}
		}
		for(int k=0;k<nArrayDaoChaNum.GetCount();k++)
		{	
			if(nArrayDaoChaNum[i].GetString()!=_T(""))
			{
				acutPrintf(_T("\t��%s"),nArrayDaoChaNum[k].GetString());
			}

		}
		/*		sDaocha.pop_back();*/
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassDaocha;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		FindNextLine4(nHandleLineStart,nHandleLineEnd,Dacha,PassDaocha,GuDao,nArrayHandleNext,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					CStringArray Daochas;
					CBlkUtility::DivideString(PassDaocha[i],_T("_"),Daochas);
					for(int i=0;i<Daochas.GetCount();i++)
					{
						if(CBlkUtility::FindStrInArray(nArrayDaoChaNum,Daochas[i])==-1)
						{
							nArrayDaoChaNum.Add(Daochas[i]);
						}
					}
					//this->GetPathGuiDao2(nHandNext,nHandleLineEnd,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetPathGuiDao4(nHandNext,nHandleLineEnd,sVectorPath,sDaocha,nArrayDaoChaNum,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
//������������յ���,Ѱ�ҽ�·,�ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
void CZnSxt::GetPathGuiDao2( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//ѹ���Ӧ�ĵ�����λ
		acutPrintf(_T("\n ��������"));
		for(int i=0;i<sVectorPath.size();i++)
		{
			//����������Ľ��
			acutPrintf(_T("\t��%d ���������Ϊ%s"),i,sVectorPath[i].GetString());
		}
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//��ӡ����������λ
		//acutPrintf(_T("\n ��������"));
		// 		for(int k=0;k<sDaocha.size();k++)
		// 		{
		// 			if(sDaocha[k]!=_T(""))
		// 			{
		// 				acutPrintf(_T("%s;\t"),sDaocha[k].GetString());
		// 			}
		// 		}
		PrintGuoDaocha(sDaocha);
		/*		sDaocha.pop_back();*/
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					this->GetPathGuiDao2(nHandNext,nHandleLineEnd,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
//������������յ���,Ѱ�ҽ�·,�ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
//Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
//vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
void CZnSxt::GetPathGuiDaoAndXHJ( long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
		}
		sXHJ.push_back(Name_PassXHJ);
		oper_excel.WriteDatatoExcel(1,num,4,(num-3));
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
		for(int i=0;i<GuDao.GetCount();i++)
		{
			if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
			{
				CString temp=_T("��")+sVectorPath[0];
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));

			}
			else
			{
				CString temp=_T("��")+Gd_name;
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				//дӭ���·
				oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
				oper_excel.WriteDatatoExcel(1,num,16,Gd_name);
			}
			CString AnniuEnd;
			CString AnniuStart;
			int count =tempXHJ.size();
			AnniuStart=tempXHJ[0]+_T("LA");
			AnniuEnd=tempXHJ[count-1]+_T("LA");
			CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
			oper_excel.WriteDatatoExcel(1,num,6,JinLuAnniu);
		}
		num++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		FindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetPathGuiDaoAndXHJ(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
//������������յ���,Ѱ�ҽ�·,�ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
//Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
//vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
//�ر������г���·��ȡ��������Ϣ,������ʼ�źŻ�����ֹ�źŻ�����
void CZnSxt::GetLieChePathGuiDaoAndXHJ( long nHandleStartXHJ,long nHandleEndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
		}
		sXHJ.push_back(Name_PassXHJ);
	    oper_excel.WriteDatatoExcel(1,num,4,(num-3));
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
			if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
			{
				CString temp=_T("��")+sVectorPath[0];
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));

			}
			else
			{
				CString temp=_T("��")+Gd_name;
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				//дӭ���·
				oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
				oper_excel.WriteDatatoExcel(1,num,16,Gd_name);
			}
			CString AnniuEnd;
			CString AnniuStart;
			GetEntName(nHandleStartXHJ,AnniuStart);
			oper_excel.WriteDatatoExcel(1,num,9,AnniuStart);
			AnniuStart+=_T("LA");
			GetEntName(nHandleEndXHJ,AnniuEnd);
			AnniuEnd+=_T("LA");
			CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
			oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		num++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayGuiDaoCX;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		LieCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayGuiDaoCX,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetLieChePathGuiDaoAndXHJ(nHandleStartXHJ,nHandleEndXHJ,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
//������������յ���,Ѱ�ҽ�·,�ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
//Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
//vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
//�ر������г���·��ȡ��������Ϣ��������ʼ�źŻ�����ֹ�źŻ�����
void CZnSxt::GetLieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");
		}
		sXHJ.push_back(Name_PassXHJ);
		oper_excel.WriteDatatoExcel(1,num,4,(num-3));
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			CString temp=_T("��")+Gd_name;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			//дӭ���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
			oper_excel.WriteDatatoExcel(1,num,16,Gd_name);

		}
		else if(CBlkUtility::FindStrInArray(GuDao,Start_Gd_name)!=-1)
		{
			CString temp=_T("��")+Start_Gd_name;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));
		}
		CString AnniuEnd;
		CString AnniuStart;
		GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,9,AnniuStart);
		AnniuStart+=_T("LA");
		GetEntName(nHandleEndXHJ,AnniuEnd);
		AnniuEnd+=_T("LA");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		num++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T("15-17DG"))
		{
			Gd_name=Gd_name;
		}
// 		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
// 		{
// 			return;
// 		}
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayGuiDaoCX;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		LieCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayGuiDaoCX,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		for(int i=0;i<count;i++)
		{
			CString name;
			m_mysys.GetXdata(nArrayHandleNext.GetAt(i),_T("GUIDAONAME"),name);
			name=name;
		}
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetLieChePathGuiDaoAndXHJ(Start_Gd_name,nHandleStartXHJ,nHandleEndXHJ,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}


//������������յ�����Ѱ�ҽ�·���ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
//Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
//vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
//�ر������г���·��ȡ��������Ϣ��������ʼ�źŻ�����ֹ�źŻ�����
//���Ӳ���iJL_Cnt������ͳ�Ʊ����·������û�б����·ʱ��Ϊ1���б����·ʱ����1
/***********************************������������**********************************************************/
/*********�������ƣ�GetTongGuoPathGuiDaoAndXHJ******************************************************************************************************/
/*********�������ܣ�����Ѱ��ͨ����·��Ҫ��ͨ����·��Ϊ����ͨ��**************************************************************/
/*********���룺CString& Start_Gd_name,��ʼ������ƣ�long nHandleStartXHJ,��ʼ�źŻ������long nHandleEndXHJ,��ֹ�źŻ������long& nHandleLineFirst,��ʼ��������******/
/****************long nHandleLineStart,���ε���ʱ��ʼ��������long nHandleLineEnd,��ֹ��������***********************************************/
/****************long LastDaocha,�ϴε���ʱ�߹��ĵ�������vector<CString>&sVectorPath,�߹��Ĺ�����Σ�vector<CString>&sDaocha,�߹��ĵ���*****/
/****************vector<CString>& sXHJ,�߹����źŻ���CStringArray &GuDao,ƽ��ͼ�Ĺɵ����ƣ�CLongArray&nArrayHandleLine,�߹��Ĺ�������*/
/****************vector<CString> &vec_strGuiDaoCX,���޾�Ե�����int &iJL_Cnt,�Ľ�·�����Ľ�·������bool bForword����·��������************/
/*********�����**************************************************************/
/************************ע�ͣ�2015-07-16 pm-15:48 by luo*******************************************************/
void CZnSxt::GetJieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
		}
		sXHJ.push_back(Name_PassXHJ);
		//oper_excel.WriteDatatoExcel(1,num,4,(num-2));
		oper_excel.WriteDatatoExcel(1,num,4,iJLnum);
		vector<CString>tempGuidao;
		TiquGuoDaocha(sVectorPath,tempGuidao);
		int i_m=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleLineFirst);
		if(i_m!=-1)
		{
			vec_strGuiDaoCX.push_back(this->strArrayGuiDaoCX.GetAt(i_m));
		}
		else
		{
			vec_strGuiDaoCX.push_back(_T(""));
		}
		//CString strTemp;
		for(int i=0;i<vec_strGuiDaoCX.size();i++)
		{
			if(vec_strGuiDaoCX[i]!=_T(""))
			{
				CString strTemp;
				strTemp=vec_strGuiDaoCX[i];
				tempGuidao.push_back(strTemp);
			}
// 			else
// 			{
// 				CString strTemp;
// 				strTemp=vec_strGuiDaoCX[i];
// 				tempGuidao.push_back(strTemp);
// 
// 			}
		}
		//���ڷ�����·��˵��Ҫȥ���ɵ�
		//CString str_start_guidao;
		
// 		vector<CString> tempGuiDao2;
// 		//ȡ��վ�źŻ����ڹ�������Ҳ�������
// 		for(int i_k=0;i_k<tempGuidao.size();i_k++)
// 		{
// 			//ȥ���ɵ�֮ǰ�Ĺ������
// 			CString str_temp=tempGuidao[i_k];
// 			if(str_temp.)
// 		}
		//ȥ�����еķ�֧����
		vector<CString>tempGuidao3;
		for(int i=0;i<tempGuidao.size();i++)
		{
			CString temp =tempGuidao[i];
			if(temp.Right(3)==_T("DG1")||temp.Right(3)==_T("DG2")||temp.Right(3)==_T("DG3")||temp.Right(3)==_T("DG4")||temp.Right(3)==_T("DG5"))
			{
				temp=temp.Left(temp.GetLength()-1);
				tempGuidao3.push_back(temp);
			}
			else
			{
				tempGuidao3.push_back(temp);
			}
		}
		vector<CString> tempGuidao4;
		DeleteSameData(tempGuidao3,tempGuidao4);
		CString str_Guidao;
		TranslateVectorToCString(tempGuidao4,str_Guidao);
		oper_excel.WriteDatatoExcel(1,num,14,str_Guidao);
		//oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		CString str_daocha;
		TranslateVectorToCString(tempDaocha,str_daocha);
		oper_excel.WriteDatatoExcel(1,num,12,str_daocha);
		//oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		CString str_ddxhj;
		TranslateVectorToCString(tempXHJ,str_ddxhj);
		oper_excel.WriteDatatoExcel(1,num,13,str_ddxhj);
		int i_tem=-1;
		if((i_tem=CBlkUtility::FindStrInArray(GuDao,Gd_name))!=-1)
	    {
	
			CString Gd_nameTemp=this->str_GuDaoName.GetAt(i_tem);
			CString temp=_T("��")+Gd_nameTemp;
	 	    oper_excel.WriteDatatoExcel(1,num,5,temp);
		 	//дӭ���·
		 	oper_excel.WriteDatatoExcel(1,num,15,Gd_nameTemp);
		 	oper_excel.WriteDatatoExcel(1,num,16,Gd_nameTemp);
	
		}
		else if((i_tem=CBlkUtility::FindStrInArray(GuDao,Start_Gd_name))!=-1)
		{
			CString Gd_nameTemp=this->str_GuDaoName.GetAt(i_tem);
		 	CString temp=_T("��")+Gd_nameTemp;
		 	oper_excel.WriteDatatoExcel(1,num,5,temp);
		 	oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));
		}
		CString AnniuEnd;
		CString AnniuStart;
		GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,9,AnniuStart);
		AnniuStart+=_T("LA");
		GetEntName(nHandleEndXHJ,AnniuEnd);
		AnniuEnd+=_T("LA");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		//�ж��ǽӳ����Ƿ���������ʼ�źŻ�����ֹ�źŻ�
		//GetHandleByName(str_xhj,nHandleDCTemp)
		//�ж��Ƿ�Ϊ���ߣ������Ϊ����ͨ������ֱ����ǰ�˳�
		int iCntTemp=0;
		for(iCntTemp=0;iCntTemp<nArrayHandleTem.GetCount()-1;iCntTemp++)
		{
			long nHandle=nArrayHandleTem.GetAt(iCntTemp);
			double d_width1=0;
			double d_width2=0;
			CUserSystem my_sis;
			if(my_sis.GetPlineWidthAt(nHandle,0,d_width1,d_width2)==Acad::eOk)
			{
				if(fabs(d_width1-0.5)<=1e-6&&fabs(d_width2-0.5)<=1e-6)
				{//�߿����Ϊ0.5
				}
				else
				{
					break;
				}
			}
			else
				break;
			long nHandleNext=0;
			if(FindNextZhiGuidao(nHandle,nHandleNext,bForword)==TRUE)
			{
				if(nHandleNext!=0&&nHandleNext==nArrayHandleTem.GetAt(iCntTemp+1))
				{
					if(my_sis.GetPlineWidthAt(nHandleNext,0,d_width1,d_width2)==Acad::eOk)
					{
						if(fabs(d_width1-0.5)<=1e-6&&fabs(d_width2-0.5)<=1e-6)
						{

						}
						else
						{
							break;
						}
					}
					else
						break;

				}
				else
					break;
			}
			else
				break;
		}
		//���Ǽ��������ж�
		if(iCntTemp==nArrayHandleTem.GetCount()-1)
		{
			oper_excel.WriteDatatoExcel(1,num,10,_T("U"));

		}
		else
		{
			oper_excel.WriteDatatoExcel(1,num,10,_T("UU"));
		}
		num++;
		iJL_Cnt++;
		this->iJLnum++;

		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{

		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		if(Gd_name==_T("15-17DG"))
		{
			Gd_name=Gd_name;
		}
		//��֧��������������
		if(Gd_name.Right(3)==_T("DG1")||Gd_name.Right(3)==_T("DG2")||Gd_name.Right(3)==_T("DG3")||Gd_name.Right(3)==_T("DG4")||Gd_name.Right(3)==_T("DG5"))
		{
			sVectorPath.push_back(_T(""));
		}
		else
		{
			sVectorPath.push_back(Gd_name);
		}
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		LieCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		for(int i=0;i<count;i++)
		{
			CString name;
			m_mysys.GetXdata(nArrayHandleNext.GetAt(i),_T("GUIDAONAME"),name);
			name=name;
		}
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					long thisdaocha=nArrayHandleThisDC[i];
					this->GetJieChePathGuiDaoAndXHJ(Start_Gd_name,nHandleStartXHJ,nHandleEndXHJ,nHandleLineFirst,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,iJL_Cnt,bForword);
					vec_strGuiDaoCX.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
			}
		}
	}
	sVectorPath.pop_back();
}

// ������������յ�����Ѱ�ҽ�·���ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
// Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
// vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
// �ر������г���·��ȡ��������Ϣ��������ʼ�źŻ�����ֹ�źŻ�����
// ���Ӳ���iJL_Cnt������ͳ�Ʊ����·������û�б����·ʱ��Ϊ1���б����·ʱ����1
/***********************************������������**********************************************************/
/*********�������ƣ�GetTongGuoPathGuiDaoAndXHJ******************************************************************************************************/
/*********�������ܣ�����Ѱ��ͨ����·��Ҫ��ͨ����·��Ϊ����ͨ��**************************************************************/
/*********���룺CString& Start_Gd_name,��ʼ������ƣ�long nHandleStartXHJ,��ʼ�źŻ������long nHandleEndXHJ,��ֹ�źŻ������long& nHandleLineFirst,��ʼ��������******/
/****************long nHandleLineStart,���ε���ʱ��ʼ��������long nHandleLineEnd,��ֹ��������***********************************************/
/****************long LastDaocha,�ϴε���ʱ�߹��ĵ�������vector<CString>&sVectorPath,�߹��Ĺ�����Σ�vector<CString>&sDaocha,�߹��ĵ���*****/
/****************vector<CString>& sXHJ,�߹����źŻ���CStringArray &GuDao,ƽ��ͼ�Ĺɵ����ƣ�CLongArray&nArrayHandleLine,�߹��Ĺ�������*/
/****************vector<CString> &vec_strGuiDaoCX,���޾�Ե�����int &iJL_Cnt,�Ľ�·�����Ľ�·������bool bForword����·��������************/
/*********�����**************************************************************/
/************************ע�ͣ�2015-07-16 pm-15:48 by luo*******************************************************/
void CZnSxt::GetTongGuoPathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		//ͨ����·�У�����������β��üӽ�ȥ
		//sVectorPath.push_back(Gd_name);
		//nArrayHandleTem.Add(nHandleLineStart);
		int iCntTemp=0;
		for(iCntTemp=0;iCntTemp<nArrayHandleTem.GetCount()-1;iCntTemp++)
		{
			long nHandle=nArrayHandleTem.GetAt(iCntTemp);
			double d_width1=0;
			double d_width2=0;
			CUserSystem my_sis;
			if(my_sis.GetPlineWidthAt(nHandle,0,d_width1,d_width2)==Acad::eOk)
			{
				if(fabs(d_width1-0.5)<=1e-6&&fabs(d_width2-0.5)<=1e-6)
				{//�߿����Ϊ0.5
				}
				else
				{
					return;
				}
			}
			else
				return;
			long nHandleNext=0;
			if(FindNextZhiGuidao(nHandle,nHandleNext,bForword)==TRUE)
			{
				if(nHandleNext!=0&&nHandleNext==nArrayHandleTem.GetAt(iCntTemp+1))
				{
					if(my_sis.GetPlineWidthAt(nHandleNext,0,d_width1,d_width2)==Acad::eOk)
					{
						if(fabs(d_width1-0.5)<=1e-6&&fabs(d_width2-0.5)<=1e-6)
						{

						}
						else
						{
							return;
						}
					}
					else
						return;

				}
				else
					return;
			}
			else
				return;
		}
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj;
		}
		sXHJ.push_back(Name_PassXHJ);
		//ͨ����·��д���
		//oper_excel.WriteDatatoExcel(1,num,4,(num-2));
		vector<CString>tempGuidao;
		TiquGuoDaocha(sVectorPath,tempGuidao);
		int i_m=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleLineFirst);
		if(i_m!=-1)
		{
			vec_strGuiDaoCX.push_back(this->strArrayGuiDaoCX.GetAt(i_m));
		}
		else
		{
			vec_strGuiDaoCX.push_back(_T(""));
		}
		//CString strTemp;
		for(int i=0;i<vec_strGuiDaoCX.size();i++)
		{
			if(vec_strGuiDaoCX[i]!=_T(""))
			{
				CString strTemp;
				strTemp=vec_strGuiDaoCX[i];
				tempGuidao.push_back(strTemp);
			}
// 			else
// 			{
// 				CString strTemp;
// 				strTemp=vec_strGuiDaoCX[i];
// 				tempGuidao.push_back(strTemp);
// 
// 			}
		}
		//ȥ�����еķ�֧����
		vector<CString>tempGuidao3;
		for(int i=0;i<tempGuidao.size();i++)
		{
			CString temp =tempGuidao[i];
			if(temp.Right(3)==_T("DG1")||temp.Right(3)==_T("DG2")||temp.Right(3)==_T("DG3")||temp.Right(3)==_T("DG4")||temp.Right(3)==_T("DG5"))
			{
				temp=temp.Left(temp.GetLength()-1);
				tempGuidao3.push_back(temp);
			}
			else
			{
				tempGuidao3.push_back(temp);
			}
		}
		vector<CString> tempGuidao4;
		DeleteSameData(tempGuidao3,tempGuidao4);
		CString str_Guidao;
		TranslateVectorToCString(tempGuidao4,str_Guidao);
		oper_excel.WriteDatatoExcel(1,num,14,str_Guidao);
		//oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		CString str_daocha;
		TranslateVectorToCString(tempDaocha,str_daocha);
		oper_excel.WriteDatatoExcel(1,num,12,str_daocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		CString str_ddxhj;
		TranslateVectorToCString(tempXHJ,str_ddxhj);
		oper_excel.WriteDatatoExcel(1,num,13,str_ddxhj);
// 		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
// 		{
// 			CString temp=_T("��")+Gd_name;
// 			oper_excel.WriteDatatoExcel(1,num,5,temp);
// 			//дӭ���·
// 			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
// 			oper_excel.WriteDatatoExcel(1,num,16,Gd_name);
// 
// 		}
// 		else if(CBlkUtility::FindStrInArray(GuDao,Start_Gd_name)!=-1)
// 		{
// 			CString temp=_T("��")+Start_Gd_name;
// 			oper_excel.WriteDatatoExcel(1,num,5,temp);
// 			oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));
// 		}
		CString AnniuEnd;
		CString AnniuStart;
		GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,9,AnniuStart);
		AnniuStart+=_T("TA");
		GetEntName(nHandleEndXHJ,AnniuEnd);
		AnniuEnd+=_T("LA");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		//�ж��ǽӳ����Ƿ���,����ʼ�źŻ�����ֹ�źŻ�
		//GetHandleByName(str_xhj,nHandleDCTemp)
		//�ж��Ƿ�Ϊ����,�����Ϊ����ͨ��,��ֱ����ǰ�˳�
		
		//���Ǽ��������ж�
		if(iCntTemp==nArrayHandleTem.GetCount()-1)
		{
			oper_excel.WriteDatatoExcel(1,num,10,_T("L��U"));

		}
// 		else
// 		{
// 			oper_excel.WriteDatatoExcel(1,num,10,_T("UU"));
// 		}
		this->num=this->num+2;
		iJL_Cnt++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{

		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		if(Gd_name==_T("15-17DG"))
		{
			Gd_name=Gd_name;
		}
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		LieCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		for(int i=0;i<count;i++)
		{
			CString name;
			m_mysys.GetXdata(nArrayHandleNext.GetAt(i),_T("GUIDAONAME"),name);
			name=name;
		}
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					long thisdaocha=nArrayHandleThisDC[i];
					this->GetTongGuoPathGuiDaoAndXHJ(Start_Gd_name,nHandleStartXHJ,nHandleEndXHJ,nHandleLineFirst,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,iJL_Cnt,bForword);
					vec_strGuiDaoCX.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
			}
		}
	}
	sVectorPath.pop_back();
}

//������������յ���,Ѱ�ҽ�·,�ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
//Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
//vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
//�ر������г���·��ȡ��������Ϣ,������ʼ�źŻ�����ֹ�źŻ�����
//���Ӳ���iJL_Cnt,����ͳ�Ʊ����·����,û�б����·ʱ,Ϊ1,�б����·ʱ����1
void CZnSxt::GetFaChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		//�������һ��������ü���
		sVectorPath.push_back(Gd_name);
		//nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			CString EntName;
			GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
			//�������źŻ�д��������źŻ�
			if(EntName.Left(6)==_T("XHJ_LF"))
			{
				name_xhj+=_T("D");
			}
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
		}
		sXHJ.push_back(Name_PassXHJ);
		oper_excel.WriteDatatoExcel(1,num,4,this->iJLnum);
		vector<CString>tempGuidao;
		TiquGuoDaocha(sVectorPath,tempGuidao);
		//������·ȥ�����Ĺ������
		tempGuidao.pop_back();
		int i_m=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleLineFirst);
		if(i_m!=-1)
		{
			vec_strGuiDaoCX.push_back(this->strArrayGuiDaoCX.GetAt(i_m));
		}
		else
		{
			vec_strGuiDaoCX.push_back(_T(""));
		}
		//CString strTemp;
		for(int i=0;i<vec_strGuiDaoCX.size();i++)
		{
			if(vec_strGuiDaoCX[i]!=_T(""))
			{
				CString strTemp;
				strTemp=vec_strGuiDaoCX[i];
				tempGuidao.push_back(strTemp);
			}
		}
		//ȥ����վ�źŻ�֮ǰ�Ĺ������
		CString str_start_guidao;
		long nHandle_start_guidao=0;
		bool b_forward_xhj=false;
		this->GetXHJ_YouCe_GdLine(nHandleStartXHJ,nHandle_start_guidao,b_forward_xhj);
		m_mysys.GetXdata(nHandle_start_guidao,_T("GUIDAONAME"),str_start_guidao);
		int i_strLen=str_start_guidao.GetLength();
		vector<CString> tempGuidao2;
		for(int i_k=0;i_k<tempGuidao.size();i_k++)
		{
            
			CString temp=tempGuidao[i_k];
			if(temp.Left(i_strLen)!=str_start_guidao)
			{

			}
			else
			{
				for(int i_j=i_k;i_j<tempGuidao.size();i_j++)
				{
					tempGuidao2.push_back(tempGuidao[i_j]);
				}
				break;
			}
		}//ר�����ڴ�������·
		//ȥ�����еķ�֧����
		vector<CString>tempGuidao3;
		for(int i=0;i<tempGuidao2.size();i++)
		{
			CString temp =tempGuidao2[i];
			if(temp.Right(3)==_T("DG1")||temp.Right(3)==_T("DG2")||temp.Right(3)==_T("DG3")||temp.Right(3)==_T("DG4")||temp.Right(3)==_T("DG5"))
			{
				temp=temp.Left(temp.GetLength()-1);
				tempGuidao3.push_back(temp);
			}
			else
			{
				tempGuidao3.push_back(temp);
			}
		}
		vector<CString> tempGuidao4;
		DeleteSameData(tempGuidao3,tempGuidao4);
		CString str_Guidao;
		TranslateVectorToCString(tempGuidao4,str_Guidao);
		oper_excel.WriteDatatoExcel(1,num,14,str_Guidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		CString str_daocha;
		TranslateVectorToCString(tempDaocha,str_daocha);
		oper_excel.WriteDatatoExcel(1,num,12,str_daocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		//ȥ����վ�źŻ�֮ǰ�ĵ����źŻ�
        vector<CString>tempXHJ2;
		for(int pos=0;pos<tempXHJ.size();pos++)
		{
			CString str_temp=tempXHJ[pos];
			if(str_temp.Left(1)!=_T("S")&&str_temp.Right(1)!=_T("D"))
			{
			}
			else
			{
				for(int j=pos;j<tempXHJ.size();j++)
				{
					tempXHJ2.push_back(tempXHJ[j]);
				}
				break;
			}
		}
		CString str_ddxhj;
		TranslateVectorToCString(tempXHJ2,str_ddxhj);
		oper_excel.WriteDatatoExcel(1,num,13,str_ddxhj);
		int i_tem=-1;
		if((i_tem=CBlkUtility::FindStrInArray(GuDao,Gd_name))!=-1)
		{
			CString Gd_nameTemp=this->str_GuDaoName.GetAt(i_tem);
			CString temp=_T("��")+Gd_nameTemp;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			//дӭ���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_nameTemp);
			oper_excel.WriteDatatoExcel(1,num,16,Gd_nameTemp);

		}
		else if((i_tem=CBlkUtility::FindStrInArray(GuDao,Start_Gd_name))!=-1)
		{
			CString Gd_nameTemp=this->str_GuDaoName.GetAt(i_tem);
			CString temp=_T("��")+Gd_nameTemp;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));
		}
		CString AnniuEnd;
		CString AnniuStart;
		GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,9,AnniuStart);
		AnniuStart+=_T("LA");
		GetEntName(nHandleEndXHJ,AnniuEnd);
		AnniuEnd+=_T("LA");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		//�ж��ǽӳ����Ƿ���������ʼ�źŻ�����ֹ�źŻ�
		oper_excel.WriteDatatoExcel(1,num,10,_T("L��LU��U"));
		num++;
		iJL_Cnt++;
		this->iJLnum++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{

		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		if(Gd_name==_T("15-17DG"))
		{
			Gd_name=Gd_name;
		}
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		FaCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		for(int i=0;i<count;i++)
		{
			CString name;
			m_mysys.GetXdata(nArrayHandleNext.GetAt(i),_T("GUIDAONAME"),name);
			name=name;
		}
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					//�ж��źŻ������ʣ����Ϊ�����źŻ�����'D'
					long nHandleXHJ=0;
					CString str_ent_name;
					CString str_xhj_name=PassXHJ[i];
//				û�����ã����ڰ����˶���
// 					GetHandleByName(PassXHJ[i],nHandleXHJ);
// 					GetEntBlkNameByHandle(nHandleXHJ,str_ent_name);
// 					if(str_ent_name.Left(6)==_T("XHJ_LF"))
// 					{
// 						str_xhj_name+=_T('D');
// 					}
					sXHJ.push_back(str_xhj_name);
					//sXHJ.push_back(PassXHJ[i]);
					vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					long thisdaocha=nArrayHandleThisDC[i];
					this->GetFaChePathGuiDaoAndXHJ(Start_Gd_name,nHandleStartXHJ,nHandleEndXHJ,nHandleLineFirst,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,iJL_Cnt,bForword);
					vec_strGuiDaoCX.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
			}
		}
	}
	sVectorPath.pop_back();
}
//������������յ�����Ѱ�ҽ�·���ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���
//Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
//vector<CString>sXHJ���ڴ洢��·�Ͼ������źŻ���ע���յ����ϵ��źŻ�û�����뿼��
//�ر������г���·��ȡ��������Ϣ��������ʼ�źŻ�����ֹ�źŻ�����
//���Ӳ���iJL_Cnt������ͳ�Ʊ����·������û�б����·ʱ��Ϊ1,�б����·ʱ����1
void CZnSxt::GetYanXuPathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		//�������һ��������ü���
		//sVectorPath.push_back(Gd_name);
		//nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//��ȡ��ֹ����źŻ�
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T("");
		}
		sXHJ.push_back(Name_PassXHJ);
		oper_excel.WriteDatatoExcel(1,num,4,this->iJLnum);
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		int i_m=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleLineFirst);
		if(i_m!=-1)
		{
			vec_strGuiDaoCX.push_back(this->strArrayGuiDaoCX.GetAt(i_m));
		}
		else
		{
			vec_strGuiDaoCX.push_back(_T(""));
		}
		//CString strTemp;
		for(int i=0;i<vec_strGuiDaoCX.size();i++)
		{
			if(vec_strGuiDaoCX[i]!=_T(""))
			{
				CString strTemp;
				strTemp=vec_strGuiDaoCX[i];
				tempGuidao.push_back(strTemp);
			}
		}
		//������·Ҫȥ����һ��������ιɵ�
		vector<CString> tempGuidao2;
		for(int i=1;i<tempGuidao.size();i++)
		{
			tempGuidao2.push_back(tempGuidao[i]);
		}
		//ȥ�����еķ�֧����
		vector<CString>tempGuidao3;
		for(int i=0;i<tempGuidao2.size();i++)
		{
			CString temp =tempGuidao2[i];
			if(temp.Right(3)==_T("DG1")||temp.Right(3)==_T("DG2")||temp.Right(3)==_T("DG3")||temp.Right(3)==_T("DG4")||temp.Right(3)==_T("DG5"))
			{
				//tempGuidao3.push_back(_T(""));
			}
			else
			{
				tempGuidao3.push_back(temp);
			}
		}
		CString str_Guidao;
		TranslateVectorToCString(tempGuidao3,str_Guidao);
		oper_excel.WriteDatatoExcel(1,num,14,str_Guidao);
		//oper_excel.WriteDatatoExcel(1,num,14,tempGuidao2);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		CString str_daocha;
		TranslateVectorToCString(tempDaocha,str_daocha);
		oper_excel.WriteDatatoExcel(1,num,12,str_daocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		CString str_ddxhj;
		TranslateVectorToCString(tempXHJ,str_ddxhj);
		oper_excel.WriteDatatoExcel(1,num,13,str_ddxhj);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			CString temp=_T("��")+Gd_name;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			//дӭ���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
			oper_excel.WriteDatatoExcel(1,num,16,Gd_name);

		}
		else if(CBlkUtility::FindStrInArray(GuDao,Start_Gd_name)!=-1)
		{
			CString temp=_T("��")+Start_Gd_name;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));
		}
		CString AnniuEnd;
		CString AnniuStart;
		GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,9,AnniuStart);
		AnniuStart+=_T("LA");
		GetEntName(nHandleEndXHJ,AnniuEnd);
		AnniuEnd+=_T("LA");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		//oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		//�ж��ǽӳ����Ƿ���,����ʼ�źŻ�����ֹ�źŻ�
		//oper_excel.WriteDatatoExcel(1,num,10,_T("L��LU��U"));
		num++;
		iJL_Cnt++;
		this->iJLnum++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{

		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T("15-17DG"))
		{
			Gd_name=Gd_name;
		}
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		LieCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		for(int i=0;i<count;i++)
		{
			CString name;
			m_mysys.GetXdata(nArrayHandleNext.GetAt(i),_T("GUIDAONAME"),name);
			name=name;
		}
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
				   //�ж��źŻ�������,���Ϊ�����źŻ�,��'D'
					long nHandleXHJ=0;
					CString str_ent_name;
					CString str_xhj_name=PassXHJ[i];
					GetHandleByName(PassXHJ[i],nHandleXHJ);
					GetEntBlkNameByHandle(nHandleXHJ,str_ent_name);
					if(str_ent_name.Left(6)==_T("XHJ_LF"))
					{
						str_xhj_name+=_T('D');
					}
					sXHJ.push_back(str_xhj_name);
					vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					long thisdaocha=nArrayHandleThisDC[i];
					this->GetYanXuPathGuiDaoAndXHJ(Start_Gd_name,nHandleStartXHJ,nHandleEndXHJ,nHandleLineFirst,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,iJL_Cnt,bForword);
					vec_strGuiDaoCX.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
			}
		}
	}
	sVectorPath.pop_back();
}
/************************������GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********���ܣ�ͨ��������·����ʼ�������ֹ���,�����·�߹��Ĺ���͵����Լ��źŻ�,����������Ϣд��Excel*****************************************/
/***********���룺CString& StartXHJ,��ʼ�źŻ����ƣ�CString& EndXHJ,�ն��źŻ����ƣ�long nHandleLineStart,��ʼ�����********************/
/******************long nHandleLineEnd,��ֹ�����long LastDaocha,�ߵ���ʼ�����һ���߹��ĵ���********************************************/
/******************CStringArray &GuDao,�ɵ����ƣ�bool bForword����·�ƽ�����***************************************************************/
/***********�����vector<CString>&sVectorPath,�洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�洢�߹��ĵ���**************************/
/******************vector<CString>& sXHJ,�洢�߹����źŻ���CLongArray&nArrayHandleLine,�ƽ������е���һ���������********************/ 
/******************************ע�ͣ�2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	long t1=GetTickCount();
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		CStringArray str_array_xhj;
		TiquEffectiveData(sXHJ,str_array_xhj);
		CString AnniuEnd;
		CString AnniuStart;
		//����ն��źŻ����ڽ�·�ϣ��򷵻�
		CString temp=EndXHJ;
		CString jinlu;
		int count=str_array_xhj.GetCount();
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//Ϊ�ɵ�ʱ
			jinlu=_T("��")+temp;
			if(count>0)
			{
				AnniuEnd=str_array_xhj[count-1]+_T("DA");
			}
		}
		else
		{
			//Ϊ�źŻ�
			long nHandleDC=0;
			GetHandleByName(temp,nHandleDC);
			//�ж��źŻ�����
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//����
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							AnniuEnd=str_array_xhj[count-2]+_T("A");
						}
						else if(count>0)
						{
							AnniuEnd=str_array_xhj[count-1]+_T("A");
						}
						// oper_excel.oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
					}
					else
					{
						CString tempZ;
						if(count>1)
						{
							tempZ=str_array_xhj[count-1];
						}
						jinlu=_T("��")+temp;
						AnniuEnd =temp+_T("A");
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						if(count>0)
							AnniuEnd =str_array_xhj[count-1]+_T("A");
					}
					else
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							AnniuEnd=str_array_xhj[count-2]+_T("A");
						}
						else if(count>0)
							AnniuEnd=str_array_xhj[count-1]+_T("A");
					}
				}

			}
		}
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//CLongArray nArrayPassXHJ;
		oper_excel.WriteDatatoExcel(1,num,4,num-3);
		//oper_excel.WriteDatatoExcel(1,)
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		//��ǰ����Ƿ���ڳ��޾�Ե
		//���ӳ�������,��һ������߲�δ���ǵ������������
	 		int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleLineFirst);
	 		if(i!=-1)
	 		{
	 			vec_strGuiDaoCX.push_back(this->strArrayGuiDaoCX.GetAt(i));
	 		}
	 		else
	 		{
				vec_strGuiDaoCX.push_back(_T(""));
	 		}
		//CString strTemp;
		for(int i=0;i<vec_strGuiDaoCX.size();i++)
		{
			if(vec_strGuiDaoCX[i]!=_T(""))
			{
				CString strTemp;
			    strTemp=_T(",")+vec_strGuiDaoCX[i];
				tempGuidao.push_back(strTemp);
			}
		}
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		CStringArray strArray_GXHJ;
		TiquEffectiveData(sXHJ,strArray_GXHJ);
		//ȥ���߹����źŻ���ͬ������źŻ�
		vector<CString > vec_xhj_didui;
		for(int i=0;i<strArray_GXHJ.GetCount();i++)
		{
			int fangxiang=-1;
			long nHandleDCTemp=0;
			GetHandleByName(strArray_GXHJ.GetAt(i),nHandleDCTemp);
			if(GetBlockToward(nHandleDCTemp,fangxiang)==TRUE)
			{
				if(fangxiang==1&&bForword==false)
				{
					vec_xhj_didui.push_back(strArray_GXHJ.GetAt(i)+_T(","));
				}
				else if(fangxiang==2&&bForword==true)
				{
					vec_xhj_didui.push_back(strArray_GXHJ.GetAt(i)+_T(","));
				}
			}
		}
		long lastdc=0;
		CStringArray nArrayDXDC;
		CLongArray  nArrayHandleNext;
		FindNextDuiXiangDCByLine(nHandleLineEnd,lastdc,GuDao,nArrayHandleNext,nArrayDXDC,bForword);
		for(int i=0;i<nArrayDXDC.GetCount();i++)
		{
			vec_xhj_didui.push_back(nArrayDXDC.GetAt(i)+_T(","));
			acutPrintf(_T("����%s"),nArrayDXDC.GetAt(i));
		}
		//��վ�źŻ��ж�
		long nHandleLine_Start=0;
		long nHandleDC_Start=0;
		GetHandleByName(StartXHJ,nHandleDC_Start);
		bool fangxiang=false;
		GetXHJ_ZuoCe_GdLine(nHandleDC_Start,nHandleLine_Start,fangxiang);
		long lastdc3=0;
		CStringArray nArrayLCXHJ2;
		CLongArray nArrayHandleNext3;
		FindNextLieCheXHJByLine(nHandleLine_Start,lastdc3,GuDao,nArrayHandleNext3,nArrayLCXHJ2,!fangxiang);
		if(nArrayLCXHJ2.GetCount()!=0)
		{
			//ֻ�н�վ�źŻ��ж��źŴ���,�ж��г��źŲ��ܴ���
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0&&nArrayLCXHJ2.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i)+_T("L,"));
					acutPrintf(_T("ͬ��жԣ�%s"),nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i)+_T("L��"));
					acutPrintf(_T("����жԣ�%s"),nArrayLCXHJ.GetAt(i));
				}
			}
		}
		oper_excel.WriteDatatoExcel(1,num,13,vec_xhj_didui);
		//д��·
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//дӭ���г���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
		}

		//GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,3,StartXHJ);
		oper_excel.WriteDatatoExcel(1,num,9,StartXHJ);
		AnniuStart=StartXHJ+_T("A");
		//AnniuEnd=temp+_T("A");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		oper_excel.WriteDatatoExcel(1,num,10,_T("B"));
		num++;
		long t2=GetTickCount();
		CString str;
		str.Format("GetDiaoChePathGuiDaoAndXHJ time:%d ms",t2-t1);
		iJL_Cnt++;
		//acutPrintf(_T("\n %s"),str);
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		//DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					//sVectorPath.push_back(strArrayCXGdName.GetAt(i));
			     	vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					//this->GetDiaoChePathGuiDaoAndXHJ(StartXHJ,EndXHJ,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,bForword);
					this->GetDiaoChePathGuiDaoAndXHJ(StartXHJ,EndXHJ,nHandleLineFirst,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,iJL_Cnt,bForword);
					vec_strGuiDaoCX.pop_back();
					//nArrayHandleLine.Copy(nArrayHandleTem);
					//sVectorPath.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
	//vec_strGuiDaoCX.pop_back();
}
/************************������GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********���ܣ�ͨ��������·����ʼ�������ֹ���,�����·�߹��Ĺ���͵����Լ��źŻ�,����������Ϣд��Excel*****************************************/
/***********���룺CString& StartXHJ,��ʼ�źŻ����ƣ�CString& EndXHJ,�ն��źŻ����ƣ�long nHandleLineStart,��ʼ�����********************/
/******************long nHandleLineEnd,��ֹ�����long LastDaocha,�ߵ���ʼ�����һ���߹��ĵ���********************************************/
/******************CStringArray &GuDao,�ɵ����ƣ�bool bForword,��·�ƽ�����***************************************************************/
/***********�����vector<CString>&sVectorPath,�洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�洢�߹��ĵ���**************************/
/******************vector<CString>& sXHJ,�洢�߹����źŻ���CLongArray&nArrayHandleLine,�ƽ������е���һ���������********************/ 
/******************************ע�ͣ�2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	long t1=GetTickCount();
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//CStringArray strArray_XHJData;
		//TiquEffectiveData(sXHJ,strArray_XHJData);
		CStringArray str_array_xhj;
		TiquEffectiveData(sXHJ,str_array_xhj);
		CString AnniuEnd;
		CString AnniuStart;
		//����ն��źŻ����ڽ�·��,�򷵻�
		CString temp=EndXHJ;
		CString jinlu;
// 		vector<CString> tempXHJ;
// 		//CStringArray str_array_xhj
// 		TiquGuoDaocha(sXHJ,tempXHJ);
		//TiquEffectiveData(sXHJ)
		int count=str_array_xhj.GetCount();
		//temp.Remove(_T("��"));
		//temp.Remove(_T("��"));
		//acutPrintf("temp =%s",temp);
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//Ϊ�ɵ�ʱ
			jinlu=_T("��")+temp;
			if(count>0)
			{
				AnniuEnd=str_array_xhj[count-1]+_T("DA");
			}
		}
		else
		{
			//Ϊ�źŻ�
			long nHandleDC=0;
			GetHandleByName(temp,nHandleDC);
			//�ж��źŻ�����
			int iToward=-1;
		   if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
		   {
			   if(iToward==1)
			   {
				   //����
				   if(bForword==true)
				   {
					   jinlu=_T("��")+temp;
					   if(count>1)
					   {
						   AnniuEnd=str_array_xhj[count-2]+_T("A");
					   }
					   else if(count>0)
					   {
						   AnniuEnd=str_array_xhj[count-1]+_T("A");
					   }
					  // oper_excel.oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
				   }
				   else
				   {
					   CString tempZ;
					   if(count>1)
					   {
						  tempZ=str_array_xhj[count-1];
					   }
					   jinlu=_T("��")+temp;
					   AnniuEnd =temp+_T("A");
				   }
					   
			   }
			   else if(iToward==2)
			   {
				   if(bForword==true)
				   {
					   jinlu=_T("��")+temp;
					   if(count>0)
					   AnniuEnd =str_array_xhj[count-1]+_T("A");
				   }
				   else
				   {
					   jinlu=_T("��")+temp;
					   if(count>1)
					   {
						   AnniuEnd=str_array_xhj[count-2]+_T("A");
					   }
					   else if(count>0)
						   AnniuEnd=str_array_xhj[count-1]+_T("A");
			      }
		   }

		}
	}
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//CLongArray nArrayPassXHJ;
		oper_excel.WriteDatatoExcel(1,num,4,num-3);
		//oper_excel.WriteDatatoExcel(1,)
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		 oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		CStringArray strArray_GXHJ;
		TiquEffectiveData(sXHJ,strArray_GXHJ);
		//ȥ���߹����źŻ���ͬ������źŻ�
		vector<CString > vec_xhj_didui;
		for(int i=0;i<strArray_GXHJ.GetCount();i++)
		{
			int fangxiang=-1;
			long nHandleDCTemp=0;
			GetHandleByName(strArray_GXHJ.GetAt(i),nHandleDCTemp);
			if(GetBlockToward(nHandleDCTemp,fangxiang)==TRUE)
			{
				if(fangxiang==1&&bForword==false)
				{
                   vec_xhj_didui.push_back(strArray_GXHJ.GetAt(i)+_T(","));
				}
				else if(fangxiang==2&&bForword==true)
				{
                    vec_xhj_didui.push_back(strArray_GXHJ.GetAt(i)+_T(","));
				}
			}
		}
		long lastdc=0;
		CStringArray nArrayDXDC;
		CLongArray  nArrayHandleNext;
		FindNextDuiXiangDCByLine(nHandleLineEnd,lastdc,GuDao,nArrayHandleNext,nArrayDXDC,bForword);
		for(int i=0;i<nArrayDXDC.GetCount();i++)
		{
			vec_xhj_didui.push_back(nArrayDXDC.GetAt(i)+_T(","));
			acutPrintf(_T("����%s"),nArrayDXDC.GetAt(i));
		}
		//��վ�źŻ��ж�
		long nHandleLine_Start=0;
		long nHandleDC_Start=0;
		GetHandleByName(StartXHJ,nHandleDC_Start);
		bool fangxiang=false;
		GetXHJ_ZuoCe_GdLine(nHandleDC_Start,nHandleLine_Start,fangxiang);
		long lastdc3=0;
		CStringArray nArrayLCXHJ2;
		CLongArray nArrayHandleNext3;
		FindNextLieCheXHJByLine(nHandleLine_Start,lastdc3,GuDao,nArrayHandleNext3,nArrayLCXHJ2,!fangxiang);
		if(nArrayLCXHJ2.GetCount()!=0)
		{
			//ֻ�н�վ�źŻ��ж��źŴ���,�ж��г��źŲ��ܴ���
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0&&nArrayLCXHJ2.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i)+_T("L,"));
					acutPrintf(_T("ͬ��жԣ�%s"),nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i)+_T("L,"));
					acutPrintf(_T("����жԣ�%s"),nArrayLCXHJ.GetAt(i));
				}
			}
		}
		oper_excel.WriteDatatoExcel(1,num,13,vec_xhj_didui);
		//д��·
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//дӭ���г���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
		}
		
		//GetEntName(nHandleStartXHJ,AnniuStart);
		//�����д
		//oper_excel.WriteDatatoExcel(1,num,3,StartXHJ);
		oper_excel.WriteDatatoExcel(1,num,9,StartXHJ);
		AnniuStart=StartXHJ+_T("A");
		//AnniuEnd=temp+_T("A");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		oper_excel.WriteDatatoExcel(1,num,10,_T("B"));
		num++;
		long t2=GetTickCount();
		CString str;
		str.Format("GetDiaoChePathGuiDaoAndXHJ time:%d ms",t2-t1);
		//acutPrintf(_T("\n %s"),str);
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		//DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					//sVectorPath.push_back(strArrayCXGdName.GetAt(i));
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetDiaoChePathGuiDaoAndXHJ(StartXHJ,EndXHJ,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					//sVectorPath.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
/************************������GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********���ܣ�ͨ��������·����ʼ�������ֹ���,�����·�߹��Ĺ���͵����Լ��źŻ�,����������Ϣд��Excel*****************************************/
/***********���룺CString& StartXHJ,��ʼ�źŻ����ƣ�CString& EndXHJ,�ն��źŻ����ƣ�long nHandleLineStart,��ʼ�����********************/
/******************long nHandleLineEnd,��ֹ�����long LastDaocha,�ߵ���ʼ�����һ���߹��ĵ���********************************************/
/******************CStringArray &GuDao,�ɵ����ƣ�bool bForword,��·�ƽ�����***************************************************************/
/***********�����vector<CString>&sVectorPath,�洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�洢�߹��ĵ���**************************/
/******************vector<CString>& sXHJ,�洢�߹����źŻ���CLongArray&nArrayHandleLine,�ƽ������е���һ���������********************/ 
/******************************ע�ͣ�2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	//long t1=GetTickCount();
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//CStringArray strArray_XHJData;
		//TiquEffectiveData(sXHJ,strArray_XHJData);
		CStringArray str_array_xhj;
		acutPrintf(_T("\n��·���źŻ���"));
		for(int i=0;i<sXHJ.size();i++)
		{
			acutPrintf(_T("\t%s"),sXHJ[i]);
		}
		TiquEffectiveData(sXHJ,str_array_xhj);
		acutPrintf(_T("\n��ȡ���źŻ���"));
		for(int i=0;i<str_array_xhj.GetCount();i++)
		{
			acutPrintf(_T("\t%s"),str_array_xhj.GetAt(i));
		}
		CString AnniuEnd;
		CString AnniuStart;
		//����ն��źŻ����ڽ�·��,�򷵻�
		CString temp=EndXHJ;
		CString jinlu;
		int count=str_array_xhj.GetCount();
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//Ϊ�ɵ�ʱ
			jinlu=_T("��")+temp;
			if(count>0)
			{
				AnniuEnd=str_array_xhj[count-1]+_T("A");
			}
		}
		else
		{
			//Ϊ�źŻ�
			long nHandleDC=0;
			if(temp.Left(1)==_T('X')||temp.Left(1)==_T('S'))
			{
				temp.Remove(_T('D'));
			}
			GetHandleByName(temp,nHandleDC);
			//�ж��źŻ�����
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//����
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							AnniuEnd=str_array_xhj[count-2]+_T("A");
						}
						else if(count>0)
						{
							AnniuEnd=str_array_xhj[count-1]+_T("A");
						}
						// oper_excel.oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
					}
					else
					{
						CString tempZ;
						if(count>1)
						{
							tempZ=str_array_xhj[count-1];
						}
						jinlu=_T("��")+temp;
						AnniuEnd =temp+_T("A");
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						if(count>0)
							AnniuEnd =str_array_xhj[count-1]+_T("A");
					}
					else
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							AnniuEnd=str_array_xhj[count-2]+_T("A");
						}
						else if(count>0)
							AnniuEnd=str_array_xhj[count-1]+_T("A");
					}
				}

			}
		}
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//CLongArray nArrayPassXHJ;
		oper_excel.WriteDatatoExcel(1,num,4,num-3);
		//oper_excel.WriteDatatoExcel(1,)
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//CStringArray strArray_GXHJ;
		//TiquEffectiveData(sXHJ,strArray_GXHJ);
		//ȥ���߹����źŻ���ͬ������źŻ�
		vector<CString > vec_xhj_didui;
		for(int i=0;i<str_array_xhj.GetCount();i++)
		{
			int fangxiang=-1;
			long nHandleDCTemp=0;
			CString str_xhj=str_array_xhj.GetAt(i);
			if(str_xhj.Left(1)==_T('S')||str_xhj.Left(1)==_T('X'))
			{
				str_xhj.Remove(_T('D'));
			}
			//ȡ�źŻ��ľ��
			GetHandleByName(str_xhj,nHandleDCTemp);
			if(str_array_xhj.GetAt(i)==_T("X3")&&StartXHJ==_T("D16"))
			{
				CString strtemp=str_array_xhj.GetAt(i);
			}
			if(GetBlockToward(nHandleDCTemp,fangxiang)==TRUE)
			{
				if(fangxiang==1&&bForword==false)
				{
					vec_xhj_didui.push_back(str_array_xhj.GetAt(i)+_T(","));
					acutPrintf(_T("didui:%s"),str_array_xhj.GetAt(i));
				}
				else if(fangxiang==2&&bForword==true)
				{
					vec_xhj_didui.push_back(str_array_xhj.GetAt(i)+_T(","));
					acutPrintf(_T("didui:%s"),str_array_xhj.GetAt(i));
				}
				else
				{
					//acutPrintf(_T("\n�źŻ����򲻶�\n"));
				}
			}
		}
		long lastdc=0;
		CStringArray nArrayDXDC;
		CLongArray  nArrayHandleNext;
		//�����������������˵��õĶ�������źŻ���ȫ
		if(StartXHJ==_T("D4")&&EndXHJ==_T("D10"))
		{
			StartXHJ=StartXHJ;
		}
		FindNextDuiXiangDCByLine(nHandleLineEnd,lastdc,GuDao,nArrayHandleNext,nArrayDXDC,bForword);
		for(int i=0;i<nArrayDXDC.GetCount();i++)
		{
			vec_xhj_didui.push_back(nArrayDXDC.GetAt(i)+_T(","));
			acutPrintf(_T("����%s"),nArrayDXDC.GetAt(i));
		}
		//��վ�źŻ��ж�
		long nHandleLine_Start=0;
		long nHandleDC_Start=0;
		GetHandleByName(StartXHJ,nHandleDC_Start);
		bool fangxiang=false;
		GetXHJ_ZuoCe_GdLine(nHandleDC_Start,nHandleLine_Start,fangxiang);
		long lastdc3=0;
		CStringArray nArrayLCXHJ2;
		CLongArray nArrayHandleNext3;
		FindNextLieCheXHJByLine(nHandleLine_Start,lastdc3,GuDao,nArrayHandleNext3,nArrayLCXHJ2,!fangxiang);
		if(nArrayLCXHJ2.GetCount()!=0)
		{
			//ֻ�н�վ�źŻ��ж��źŴ���,�ж��г��źŲ��ܴ���
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0&&nArrayLCXHJ2.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i)+_T("L,"));
					acutPrintf(_T("ͬ��жԣ�%s"),nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i)+_T("L��"));
					acutPrintf(_T("����жԣ�%s"),nArrayLCXHJ.GetAt(i));
				}
			}
		}
		oper_excel.WriteDatatoExcel(1,num,13,vec_xhj_didui);
		//д��·
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//дӭ���г���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
		}

		//GetEntName(nHandleStartXHJ,AnniuStart);
		//�����д
		//oper_excel.WriteDatatoExcel(1,num,3,StartXHJ);
		oper_excel.WriteDatatoExcel(1,num,9,StartXHJ);
		AnniuStart=StartXHJ+_T("A");
		//AnniuEnd=temp+_T("A");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		oper_excel.WriteDatatoExcel(1,num,10,_T("B"));
		num++;
		long t2=GetTickCount();
		CString str;
//		str.Format("GetDiaoChePathGuiDaoAndXHJ time:%d ms",t2-t1);
		iJL_Cnt++;
		//acutPrintf(_T("\n %s"),str);
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		//DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					//sVectorPath.push_back(strArrayCXGdName.GetAt(i));
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//int iDc_Cnt=0;
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetDiaoChePathGuiDaoAndXHJ(StartXHJ,EndXHJ,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,iJL_Cnt,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					//sVectorPath.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
/************************������GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********���ܣ�ͨ��������·����ʼ�������ֹ����������·�߹��Ĺ���͵����Լ��źŻ�������������Ϣд��Excel*****************************************/
/***********���룺CString& StartXHJ,��ʼ�źŻ����ƣ�CString& EndXHJ,�ն��źŻ����ƣ�long nHandleLineStart,��ʼ�����********************/
/******************long nHandleLineEnd,��ֹ�����long LastDaocha,�ߵ���ʼ�����һ���߹��ĵ���********************************************/
/******************CStringArray &GuDao,�ɵ����ƣ�bool bForword,��·�ƽ�����***************************************************************/
/***********�����vector<CString>&sVectorPath,�洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�洢�߹��ĵ���**************************/
/******************vector<CString>& sXHJ,�洢�߹����źŻ���CLongArray&nArrayHandleLine,�ƽ������е���һ���������********************/ 
/******************************ע�ͣ�2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::GetDiaoChePathGuiDaoAndXHJ2( CString& StartXHJ,CString& EndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,CStringArray &strArrayJieJinquduan,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX, int &iJL_Cnt,bool bSXiaxing,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	//long t1=GetTickCount();
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		CStringArray str_array_xhj;
		TiquEffectiveData(sXHJ,str_array_xhj);
		if(StartXHJ==_T("D6")&&EndXHJ==_T("D2"))
		{
			StartXHJ=StartXHJ;
			nHandleLineEnd=nHandleLineStart;
		}
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			//������ֶ���δ�������Σ�ȥ���ظ����������ȥ���������ο���ȥ���Ĳ���������Σ�
			Gd_name=_T("XXX-0");
		}
		sVectorPath.push_back(Gd_name);
		//�����Ƿ���Ҫ������������
		nArrayHandleTem.Add(nHandleLineStart);
		acutPrintf(_T("\nʼ��%s �ն�%s��"),StartXHJ,EndXHJ);
		for(int i=0;i<nArrayHandleTem.GetCount();i++)
		{
			CString te;
			m_mysys.GetXdata(nArrayHandleTem.GetAt(i),_T("GUIDAONAME"),te);
			acutPrintf(_T("\t�������%s"),te);
		}
		CLongArray nArrayHandleTem2;
		nArrayHandleTem2.Append(nArrayHandleTem);
		for(int i=0;i<nArrayHandleTem2.GetCount();i++)
		{
			CString te;
			m_mysys.GetXdata(nArrayHandleTem2.GetAt(i),_T("GUIDAONAME"),te);
			acutPrintf(_T("\t�������%s"),te);
		}
		acutPrintf(_T("\n"));
		vector<CString>tempGuidao;
		TiquGuoDaocha(sVectorPath,tempGuidao);
		CString AnniuEnd;
		CString AnniuStart;
		//����ն��źŻ����ڽ�·��,�򷵻�
		CString temp=EndXHJ;
		CString jinlu;
		CLongArray nArrayHandleXHJTemp;
		//�ӷ�λ�߹����ģ�������źŻ����ڽ�·��
		if(LastDaocha==0)
		{
			GetGuiDaoDuanXHJ(nHandleLineStart,bForword,nArrayHandleXHJTemp);
			for(int i=0;i<nArrayHandleXHJTemp.GetCount();i++)
			{
				CString xhjTemp;
				CString xhjEnt;
				GetEntName(nArrayHandleXHJTemp.GetAt(i),xhjTemp);
				GetEntBlkNameByHandle(nArrayHandleXHJTemp.GetAt(i),xhjEnt);
				if(xhjEnt.Left(6)==_T("XHJ_LF"))
				{
					xhjTemp+=_T("L");
				}
				if(xhjTemp!=_T("")&&CBlkUtility::FindStrInArray(str_array_xhj,xhjTemp)==-1)
				{
					str_array_xhj.Add(xhjTemp);
				}
			}
		}
		int count=str_array_xhj.GetCount();
		int item=-1;
		if((CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1)&&(CBlkUtility::FindStrInArray(this->sXGuDaoName,temp)==-1)&&((CBlkUtility::FindStrInArray(this->sSGuDaoName,temp))==-1))
		{
			return;
		}
		else if((item=CBlkUtility::FindStrInArray(this->sXGuDaoName,temp))!=-1)
		{
			//Ϊ�ɵ�ʱ,��һ��
			tempGuidao.pop_back();
			//���ɹɵ�����
			jinlu=_T("��")+this->str_GuDaoName[item];
			if(count>0)
			{
				CString strtemp=str_array_xhj[count-1];
				strtemp.Remove(_T('L'));
				AnniuEnd=strtemp+_T("DA");
			}
		}
		else if((item=CBlkUtility::FindStrInArray(this->sSGuDaoName,temp))!=-1)
		{
			//Ϊ�ɵ�ʱ,��һ��
			tempGuidao.pop_back();
			//���ɹɵ�����
			jinlu=_T("��")+this->str_GuDaoName[item];
			if(count>0)
			{
				CString strtemp=str_array_xhj[count-1];
				strtemp.Remove(_T('L'));
				AnniuEnd=strtemp+_T("DA");
			}
		}
		else
		{
			//Ϊ�źŻ�
			//���ڷǹɵ�����,��Ҫȥ����������
			tempGuidao.pop_back();
			long nHandleDC=0;
			if(temp.Left(1)==_T('X')||temp.Left(1)==_T('S'))
			{
				temp.Remove(_T('D'));
			}
			GetHandleByName(temp,nHandleDC);
			//�ж��źŻ�����
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//����
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							//��������ֻ�����޲����ε�����³���,��Ҫ������
							CString strGuidao;
							int cnt=tempGuidao.size();
							strGuidao=tempGuidao[cnt-1];
							if(strGuidao.Right(2)==_T("WG"))
							{
								AnniuEnd=str_array_xhj[count-2]+_T("A");
								//ȥ���޲�����
								tempGuidao.pop_back();
							}
							else
							{
								//����ǲ����źŻ��Ͳ�����
								long zhixhj=0;
								long xiangxhj=0;
								CString str_xxhj;
								GetGuiDaoDuanXHJ(bForword,nHandleLineEnd,zhixhj,xiangxhj);
								if(xiangxhj!=0)
								{
									GetEntName(xiangxhj,str_xxhj);
									AnniuEnd=str_xxhj+_T("A");
								}
								else 
									AnniuEnd=str_array_xhj[count-1]+_T("A");
							}
							
						}
						else if(count>0)
						{
							AnniuEnd=str_array_xhj[count-1]+_T("A");
						}
						// oper_excel.oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
					}
					else
					{
						CString tempZ;
						if(count>1)
						{
							tempZ=str_array_xhj[count-1];
						}
						CString xhjent;
						GetEntBlkNameByHandle(nHandleDC,xhjent);
						if(xhjent.Left(6)==_T("XHJ_LJ"))
						{
							jinlu=_T("��")+temp;
							AnniuEnd=temp+_T("DZA");
						}
						else
						{
							jinlu=_T("��")+temp;
							AnniuEnd =temp+_T("A");
						}
						
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
// 						jinlu=_T("��")+temp;
// 						if(count>0)
// 							AnniuEnd =str_array_xhj[count-1]+_T("A");
						CString xhjent;
						GetEntBlkNameByHandle(nHandleDC,xhjent);
						if(xhjent.Left(6)==_T("XHJ_LJ"))
						{
							jinlu=_T("��")+temp;
							AnniuEnd=temp+_T("DZA");
						}
						else
						{
							jinlu=_T("��")+temp;
							AnniuEnd =temp+_T("A");
						}
					}
					else
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							CString strGuidao;
							int cnt=tempGuidao.size();
							strGuidao=tempGuidao[cnt-1];
							if(strGuidao.Right(2)==_T("WG"))
							{
								AnniuEnd=str_array_xhj[count-2]+_T("A");
								//ȥ���޲�����
								tempGuidao.pop_back();
							}
							else
							{
								//����ǲ����źŻ��Ͳ�����
								long zhixhj=0;
								long xiangxhj=0;
								CString str_xxhj;
								GetGuiDaoDuanXHJ(bForword,nHandleLineEnd,zhixhj,xiangxhj);
								if(xiangxhj!=0)
								{
									GetEntName(xiangxhj,str_xxhj);
									AnniuEnd=str_xxhj+_T("A");
								}
								else 
									AnniuEnd=str_array_xhj[count-1]+_T("A");
							}
						}
						else if(count>0)
							AnniuEnd=str_array_xhj[count-1]+_T("A");
					}
				}
				else if(iToward==0||iToward==3)
				{
					CString xhjent;
					GetEntBlkNameByHandle(nHandleDC,xhjent);
					if(xhjent.Left(6)==_T("XHJ_LJ"))
					{
						jinlu=_T("��")+temp;
						AnniuEnd=temp+_T("DZA");
					}
					else
					{
						jinlu=_T("��")+temp;
						AnniuEnd =temp+_T("A");
					}

				}

			}
			
		}
		//nArrayHandleTem.Add(nHandleLineStart);
		//CLongArray nArrayPassXHJ;
		oper_excel.WriteDatatoExcel(1,num,4,this->iJLnum);
		//��ǰ����Ƿ���ڳ��޾�Ե
		//���ӳ�������,��һ������߲�δ���ǵ�,���������
		int i_m=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleLineFirst);
		if(i_m!=-1)
		{
			vec_strGuiDaoCX.push_back(this->strArrayGuiDaoCX.GetAt(i_m));
		}
		else
		{
			vec_strGuiDaoCX.push_back(_T(""));
		}
		//CString strTemp;
		for(int i=0;i<vec_strGuiDaoCX.size();i++)
		{
			if(vec_strGuiDaoCX[i]!=_T(""))
			{
				CString strTemp;
				strTemp=vec_strGuiDaoCX[i];
				tempGuidao.push_back(strTemp);
			}
		}
		//ȥ�����еķ�֧����
		vector<CString>tempGuidao3;
		for(int i=0;i<tempGuidao.size();i++)
		{
			CString temp =tempGuidao[i];
			if(temp.Right(3)==_T("DG1")||temp.Right(3)==_T("DG2")||temp.Right(3)==_T("DG3")||temp.Right(3)==_T("DG4")||temp.Right(3)==_T("DG5"))
			{			
				temp=temp.Left(temp.GetLength()-1);
				tempGuidao3.push_back(temp);
			}
			else
			{
				tempGuidao3.push_back(temp);
			}
		}
		vector<CString> tempGuidao4;
		DeleteSameData(tempGuidao3,tempGuidao4);
		CString str_Guidao;
        TranslateVectorToCString(tempGuidao4,str_Guidao);
		oper_excel.WriteDatatoExcel(1,num,14,str_Guidao);
		sVectorPath.pop_back();
		nArrayHandleTem.RemoveAt(nArrayHandleTem.GetCount()-1);
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		CStringArray str_array_Daocha;
		//ȥ��û�е���������������ය�ţ�
		TiquEffectiveData(sDaocha,str_array_Daocha);
		for(int i=0;i<str_array_Daocha.GetCount();i++)
		{
			if(str_array_Daocha[i]!=_T(""))
			{
				tempDaocha.push_back(str_array_Daocha[i]);
			}
		}
		//TiquGuoDaocha(sDaocha,tempDaocha);
		CString str_daocha;
		TranslateVectorToCString(tempDaocha,str_daocha);
		oper_excel.WriteDatatoExcel(1,num,12,str_daocha);
		CString strTiaojian;
		for(int i=tempDaocha.size()-1;i>=0;i--)
		{
			if(tempDaocha[i]!=_T(""))
			{
				this->TiquFirstDaocha(tempDaocha[i],strTiaojian);
			}		
		}
		//ȥ���߹����źŻ���ͬ������źŻ�
		vector<CString > vec_xhj_didui;
		for(int i=0;i<str_array_xhj.GetCount();i++)
		{
			int fangxiang=-1;
			long nHandleDCTemp=0;
			CString str_xhj=str_array_xhj.GetAt(i);
			if(str_xhj.Left(1)==_T('S')||str_xhj.Left(1)==_T('X'))
			{
				str_xhj.Remove(_T('D'));
			}
			//ȡ�źŻ��ľ��
			GetHandleByName(str_xhj,nHandleDCTemp);
			if(StartXHJ==_T("D11"))
			{
				CString strtemp=str_array_xhj.GetAt(i);
			}
			if(GetBlockToward(nHandleDCTemp,fangxiang)==TRUE)
			{
				if(fangxiang==1&&bForword==false)
				{
					vec_xhj_didui.push_back(str_array_xhj.GetAt(i));
					//acutPrintf(_T("didui:%s"),str_array_xhj.GetAt(i));
				}
				else if(fangxiang==2&&bForword==true)
				{
					vec_xhj_didui.push_back(str_array_xhj.GetAt(i));
					//acutPrintf(_T("didui:%s"),str_array_xhj.GetAt(i));
				}
				else
				{
					//acutPrintf(_T("\n�źŻ����򲻶�\n"));
				}
			}
		}
		long lastdc=0;
		CStringArray nArrayDXDC;
		CLongArray  nArrayHandleNext;
		CStringArray strArrayDXDaocha;
		//�����������������˵��õĶ�������źŻ���ȫ
		if(StartXHJ==_T("D9")&&EndXHJ==_T("D17"))
		{
			StartXHJ=StartXHJ;
		}
		FindNextDuiXiangDCByLine2(nHandleLineEnd,lastdc,GuDao,nArrayHandleNext,nArrayDXDC,strArrayDXDaocha,bForword);
		CLongArray nArrayHandleDXDC;
		for(int k=0;k<nArrayDXDC.GetCount();k++)
		{
			//vec_xhj_didui.push_back(nArrayDXDC.GetAt(i)+_T(","));
			int i_add=0;
			//acutPrintf(_T("����%s"),nArrayDXDC.GetAt(k));
			long nHandleDXDC=0;
			//ע��������Ҫȥ��_T('D')
			CString tempXHJ=nArrayDXDC.GetAt(k);
			if(tempXHJ.Left(1)==_T('X')||tempXHJ.Left(1)==_T('S'))
			{
				tempXHJ.Remove(_T('D'));
			}
			GetHandleByName(tempXHJ,nHandleDXDC);
			//CStringArray strArrayJieJinquduan;
			CLongArray nArrayHandleNextTXLine;
			CStringArray strArrayTXDC;
			//ͨ��һ���źŻ���ȡ������ն��źŻ�
			GetNextDCXHJByDCXHJ(nHandleDXDC,GuDao,strArrayJieJinquduan,nArrayHandleNextTXLine,strArrayTXDC);
			//��ȡͬ��������źŻ��ն˺�
			int iCnt_TXDC=strArrayTXDC.GetCount();
			if(iCnt_TXDC>1)
			{
				//���Ҵ��ڲ�����·�ڹ���ĵ�����·,����Ҫ��ӵж�����,����Ӧ�÷��ء�������
				long nHandleLineStartTem=0;
				long nHandleLineEndTem=0;
				bool fangxiang=false;
                GetXHJ_YouCe_GdLine(nHandleDXDC,nHandleLineStartTem,fangxiang);
				if(nHandleLineStartTem==0)
				{
					acutPrintf(_T("\n��ȡ�ź��Ҳ���ʧ��"));
				}
				for(int i=0;i<strArrayTXDC.GetCount();i++)
				{
					long nHandlexhj=0;
					//��Ҫע��
					acutPrintf(_T("������%s"),strArrayTXDC.GetAt(i));
					GetHandleByName(strArrayTXDC.GetAt(i),nHandlexhj);
					if(nHandlexhj==0)
					{
						acutPrintf(_T("\n��ȡ�źŻ����ʧ��"));
					}
					bool fangxiang2=false;
					GetXHJ_ZuoCe_GdLine(nHandlexhj,nHandleLineEndTem,fangxiang2);
					if(nHandleLineEndTem==0)
					{
						acutPrintf(_T("\n��ȡ�źŻ������ʧ��"));
					}
					long lastdcTemp=0;
					vector<CString> vec_sVectorPathTem;
					vector<CString> vec_sDaochaTem;
					vector<CString> vec_sXHJ_tem;
					CLongArray nArrayHandleLineTem2;
					vector<CString> vec_strTiaojianT;
					int iJL_CntTem=0;
					int iTJ_CntTem=0;
					this->IsAddConditionDiaoChePath(nHandleLineStartTem,nHandleLineEndTem,nArrayHandleTem,lastdcTemp,vec_sVectorPathTem,vec_sDaochaTem,vec_sXHJ_tem,GuDao,nArrayHandleLineTem2,vec_strGuiDaoCX,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,fangxiang);
					if(iTJ_CntTem>=1)
					{
						acutPrintf(_T("����:%s"),vec_strTiaojianT[0]);
						CString str_temp;
			            if(strArrayDXDaocha.GetAt(k)!=_T(""))
						{
							CString strTemp=strArrayDXDaocha.GetAt(k);
							this->TiquFirstDaocha2(strArrayDXDaocha.GetAt(k),strTemp);
							str_temp=_T("<")+strTemp+_T(">");
							vec_xhj_didui.push_back(str_temp+nArrayDXDC.GetAt(k));
						}
						else 	if(strTiaojian!=_T(""))
						{
							str_temp=_T("<")+strTiaojian+_T(">");
							vec_xhj_didui.push_back(str_temp+nArrayDXDC.GetAt(k));
						}
						else
						{
							vec_xhj_didui.push_back(vec_strTiaojianT[0]+nArrayDXDC.GetAt(k));
						}
						i_add=1;
      					break;
					}

				}
				if(i_add==0)
				{
					vec_xhj_didui.push_back(nArrayDXDC.GetAt(k));
				}
				
			}
			else
			{
				vec_xhj_didui.push_back(nArrayDXDC.GetAt(k));
			}

		}
		//���ǵж����� 
		//��վ�źŻ��ж�
		long nHandleLine_Start=0;
		long nHandleDC_Start=0;
		CString Startlcxhj=StartXHJ;
		if(Startlcxhj.Right(1)==_T('D'))
		{
			Startlcxhj.Remove(_T('D'));
		}
		GetHandleByName(Startlcxhj,nHandleDC_Start);
		bool fangxiang=false;
		GetXHJ_YouCe_GdLine(nHandleDC_Start,nHandleLine_Start,fangxiang);
		long lastdc3=0;
		CStringArray nArrayLCXHJ2;
		CLongArray nArrayHandleNext3;
		long nHandleLine_StartNew;
		//���ǻ��ж���ĵж��г��ź�
		FindNextLineByJYJ(nHandleLine_Start,nHandleLine_StartNew,!fangxiang);
		//Ѱ�ҵжԽ�վ�źŻ�
		FindNextLieCheXHJByLine(nHandleLine_StartNew,lastdc3,GuDao,nArrayHandleNext3,nArrayLCXHJ2,!bForword);
		if(nArrayLCXHJ2.GetCount()!=0)
		{
			//ֻ�н�վ�źŻ��ж��źŴ���,�ж��г��źŲ��ܴ���
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			//Ѱ�ҵжԳ�վ�źŻ�
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
				     //�Ƿ��������
					int mm=0;
					int temp=nArrayHandleNext2.GetCount();
					int temp2=nArrayHandleNext3.GetCount();
					CString Gd_name;
					m_mysys.GetXdata(nArrayHandleNext3.GetAt(i),_T("GUIDAONAME"),Gd_name);
					CString str_te=nArrayLCXHJ2.GetAt(i);
					CString entname;
					long nhandle=0;
					CString xhjname=nArrayLCXHJ2.GetAt(i);
					 xhjname.Remove(_T('L'));
					GetHandleByName(xhjname,nhandle);
					GetEntBlkNameByHandle(nhandle,entname);
					if(entname.Left(6)==_T("XHJ_LJ"))
					{
						for(int k=0;k<this->nAarryHandleGuDaoS.GetCount();k++)
						{
							long lastdcTemp=0;
							vector<CString> vec_sVectorPathTem;
							vector<CString> vec_sDaochaTem;
							vector<CString> vec_sXHJ_tem;
							CLongArray nArrayHandleLineTem2;
							vector<CString> vec_strTiaojianT;
							int iJL_CntTem=0;
							int iTJ_CntTem=0;
							CString Gd_name;
							m_mysys.GetXdata(this->nAarryHandleGuDaoS.GetAt(k),_T("GUIDAONAME"),Gd_name);
							//this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							if(bSXiaxing==true)
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext3.GetAt(i),this->nAarryHandleGuDaoS.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,false);
							}
							else
							{
									this->IsAddConditionDiaoChePath(nArrayHandleNext3.GetAt(i),this->nAarryHandleGuDaoX.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							}
							//this->IsAddConditionDiaoChePath(nArrayHandleNext3.GetAt(i),this->nAarryHandleGuDaoS.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,false);	
							//this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							if(iTJ_CntTem>=1)
							{
								CString str_temp=_T("<>")+nArrayLCXHJ2.GetAt(i);
								//str_temp+=_T(">");
								vec_xhj_didui.push_back(str_temp);
								mm=1;
								break;
							}
						}
						if(mm==0)
						{
							vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i));
						}	

					}
					else if(entname.Left(6)==_T("XHJ_LF"))
					{
						for(int k=0;k<this->nAarryHandleSJJC.GetCount();k++)
						{
							long lastdcTemp=0;
							vector<CString> vec_sVectorPathTem;
							vector<CString> vec_sDaochaTem;
							vector<CString> vec_sXHJ_tem;
							CLongArray nArrayHandleLineTem2;
							vector<CString> vec_strTiaojianT;
							int iJL_CntTem=0;
							int iTJ_CntTem=0;
							if(bSXiaxing==true)
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext3.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);

							}
							else
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext3.GetAt(i),this->nAarryHandleXJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,false);

							}
							//this->IsAddConditionDiaoChePath(nArrayHandleNext3.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							if(iTJ_CntTem>=1)
							{		
								mm=1;
								CString str_temp=_T("<>")+nArrayLCXHJ2.GetAt(i);
								//str_temp+=_T(">");
								vec_xhj_didui.push_back(str_temp);
								break;
							}
						}
						if(mm==0)
						{
							vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i));
						}

					}	
					else
						vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					int kk=0;
					int temp=nArrayHandleNext2.GetCount();
					int temp2=nArrayHandleNext3.GetCount();
					CString Gd_name;
					m_mysys.GetXdata(nArrayHandleNext2.GetAt(i),_T("GUIDAONAME"),Gd_name);
 					CString str_te=nArrayLCXHJ.GetAt(i);
					CString entname;
					long nhandle=0;
					CString xhjname=nArrayLCXHJ.GetAt(i);
					xhjname.Remove(_T('L'));
					GetHandleByName(xhjname,nhandle);
					GetEntBlkNameByHandle(nhandle,entname);
					if(entname.Left(6)==_T("XHJ_LF"))
					{
						for(int k=0;k<this->nAarryHandleSJJC.GetCount();k++)
						{
							long lastdcTemp=0;
							vector<CString> vec_sVectorPathTem;
							vector<CString> vec_sDaochaTem;
							vector<CString> vec_sXHJ_tem;
							CLongArray nArrayHandleLineTem2;
							vector<CString> vec_strTiaojianT;
							int iJL_CntTem=0;
							int iTJ_CntTem=0;
							if(bSXiaxing==true)
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);

							}
							else
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleXJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,false);

							}
							//this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							if(iTJ_CntTem>=1)
							{		
								kk=1;
								CString str_temp=_T("<>")+nArrayLCXHJ.GetAt(i);
								//str_temp+=_T(">");
								vec_xhj_didui.push_back(str_temp);
								break;
							}
						}
						if(kk==0)
						{
							vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i));
						}

					}
					else if(entname.Left(6)==_T("XHJ_LJ"))
					{
						for(int k=0;k<this->nAarryHandleGuDaoS.GetCount();k++)
						{
							long lastdcTemp=0;
							vector<CString> vec_sVectorPathTem;
							vector<CString> vec_sDaochaTem;
							vector<CString> vec_sXHJ_tem;
							CLongArray nArrayHandleLineTem2;
							vector<CString> vec_strTiaojianT;
							int iJL_CntTem=0;
							int iTJ_CntTem=0;
							CString Gd_name;
							m_mysys.GetXdata(this->nAarryHandleGuDaoS.GetAt(k),_T("GUIDAONAME"),Gd_name);
							//this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							if(bSXiaxing==true)
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleGuDaoS.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,false);

							}
							else
							{
								this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleGuDaoX.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);

							}
							//this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleGuDaoS.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,false);
							//this->IsAddConditionDiaoChePath(nArrayHandleNext2.GetAt(i),this->nAarryHandleSJJC.GetAt(k),nArrayHandleTem2,lastdcTemp,vec_sDaochaTem,GuDao,nArrayHandleLineTem2,vec_strTiaojianT,iJL_CntTem,iTJ_CntTem,true);
							if(iTJ_CntTem>=1)
							{
								CString str_temp=_T("<>")+nArrayLCXHJ.GetAt(i);
								//str_temp+=_T(">");
								vec_xhj_didui.push_back(str_temp);
								kk=1;
								break;
							}
						}
						if(kk==0)
						{
							vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i));
						}	

					}
					else
						vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i));
				
				}
			}
		}
		CString str_ddxhj;
	    //ȥ���ظ��źŻ�
		vector<CString> str_vec_didui_xhj;
		DeleteSameData(vec_xhj_didui,str_vec_didui_xhj);
		//����Ҫ�ϲ��г�������źŻ�
		vector<CString>str_vec_xhj;
		CombandXHJData(str_vec_didui_xhj,str_vec_xhj);
		TranslateVectorToCString(str_vec_xhj,str_ddxhj);
		oper_excel.WriteDatatoExcel(1,num,13,str_ddxhj);
		//д��·
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if((item=CBlkUtility::FindStrInArray(GuDao,Gd_name))!=-1)
		{
			//дӭ���г���·
			CString str_tem=this->str_GuDaoName[item];
			oper_excel.WriteDatatoExcel(1,num,15,str_tem);
		}
		oper_excel.WriteDatatoExcel(1,num,9,StartXHJ);
		AnniuStart=StartXHJ+_T("A");
		//AnniuEnd=temp+_T("A");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		oper_excel.WriteDatatoExcel(1,num,10,_T("B"));
		num++;
		iJL_Cnt++;
		this->iJLnum++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		DiaoCheFindNextLineAndXHJ2(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
// 					CString name;
// 					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					//sVectorPath.push_back(strArrayCXGdName.GetAt(i));
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					this->GetDiaoChePathGuiDaoAndXHJ2(StartXHJ,EndXHJ,nHandleLineFirst,nHandNext,nHandleLineEnd,thisdaocha,strArrayJieJinquduan,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,iJL_Cnt,bSXiaxing,bForword);
					vec_strGuiDaoCX.pop_back();
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
			}
		}
	}
	sVectorPath.pop_back();
}
/************************������IsAddConditionDiaoChePath********************************************************************************************/
/***********���ܣ����ݵжԽ�·�ĵ���ʼ�˺͵����նˣ�ȷ���Ƿ���Ҫ���жԵ����źŻ�������*****************************************/
/***********���룺CString& StartXHJ,��ʼ�źŻ����ƣ�CString& EndXHJ,�ն��źŻ����ƣ�long nHandleLineStart,��ʼ�����********************/
/******************long nHandleLineEnd,��ֹ�����long LastDaocha,�ߵ���ʼ�����һ���߹��ĵ���********************************************/
/******************CStringArray &GuDao,�ɵ����ƣ�bool bForword����·�ƽ�����***************************************************************/
/***********�����vector<CString>&sVectorPath,�洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�洢�߹��ĵ���**************************/
/******************vector<CString>& sXHJ,�洢�߹����źŻ���CLongArray&nArrayHandleLine,�ƽ������е���һ���������********************/ 
/******************************ע�ͣ�2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::IsAddConditionDiaoChePath(long nHandleLineStart,long nHandleLineEnd,CLongArray &nArrayHandleStop,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX, vector<CString>& vec_str_tiaojian,int &iJL_Cnt,int &iTJ_Cnt,bool bForword/*=true*/ )
{
	//int b_Ret=FALSE;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		CStringArray str_array_xhj;
// 		for(int i=0;i<sXHJ.size();i++)
// 		{
// 			//acutPrintf(_T("\t%s"),sXHJ[i]);
// 		}
		TiquEffectiveData(sXHJ,str_array_xhj);
// 		acutPrintf(_T("\n��ȡ���źŻ���"));
// 		for(int i=0;i<str_array_xhj.GetCount();i++)
// 		{
// 			//acutPrintf(_T("\t%s"),str_array_xhj.GetAt(i));
// 		}
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
// 		acutPrintf(_T("\n ������·�߹��Ĺ�����Σ�"));
// 		for(int i=0;i<nArrayHandleTem.GetCount();i++)
// 		{
// 			CString Gd_name;
// 			m_mysys.GetXdata(nArrayHandleTem.GetAt(i),_T("GUIDAONAME"),Gd_name);
// 			acutPrintf(_T("%s\t"),Gd_name);
// 		}
		vector<CString>tempGuidao;
		TiquGuoDaocha(sVectorPath,tempGuidao);
		int j=0;
		for(j=0;j<nArrayHandleTem.GetCount();j++)
		{
			if(CBlkUtility::FindLongInArray(nArrayHandleStop,nArrayHandleTem.GetAt(j))!=-1)
			{
				//˵��������·�ڹ������
				break;
			}
		}
		if(j==nArrayHandleTem.GetCount())
		{
			iTJ_Cnt++;
			vec_str_tiaojian.push_back("<>");
			acutPrintf(_T("\n��Ҫ������"));
		}

		sVectorPath.pop_back();
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		iJL_Cnt++;
		//return b_Ret;
	}
	else if(nHandleLineStart==0)
	{
		return ;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//DiaoCheFindNextLineAndXHJ2(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		DiaoCheFindNextLineAndDaocha(nHandleLineStart,nHandleLineEnd,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					//vec_strGuiDaoCX.push_back(strArrayCXGdName.GetAt(i));
					//sDaocha.push_back(Dacha[i]);
					//sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					this->IsAddConditionDiaoChePath(nHandNext,nHandleLineEnd,nArrayHandleStop,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,vec_strGuiDaoCX,vec_str_tiaojian,iJL_Cnt,iTJ_Cnt,bForword);
					//vec_strGuiDaoCX.pop_back();
					sDaocha.pop_back();
					//sXHJ.pop_back();
				}
			}
		}
	}
	sVectorPath.pop_back();
}
/************************������IsAddConditionDiaoChePath********************************************************************************************/
/***********���ܣ����ݵжԽ�·�ĵ���ʼ�˺͵����նˣ�ȷ���Ƿ���Ҫ���жԵ����źŻ�������*****************************************/
/***********���룺CString& StartXHJ,��ʼ�źŻ����ƣ�CString& EndXHJ,�ն��źŻ����ƣ�long nHandleLineStart,��ʼ�����********************/
/******************long nHandleLineEnd,��ֹ�����long LastDaocha,�ߵ���ʼ�����һ���߹��ĵ���********************************************/
/******************CStringArray &GuDao,�ɵ����ƣ�bool bForword����·�ƽ�����***************************************************************/
/***********�����vector<CString>&sVectorPath,�洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�洢�߹��ĵ���**************************/
/******************vector<CString>& sXHJ,�洢�߹����źŻ���CLongArray&nArrayHandleLine,�ƽ������е���һ���������********************/ 
/******************************ע�ͣ�2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::IsAddConditionDiaoChePath(long nHandleLineStart,long nHandleLineEnd,CLongArray &nArrayHandleStop,long LastDaocha,vector<CString>&sDaocha,CStringArray &GuDao,CLongArray&nArrayHandleLine, vector<CString>& vec_str_tiaojian,int &iJL_Cnt,int &iTJ_Cnt,bool bForword/*=true*/ )
{
	//int b_Ret=FALSE;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		CStringArray str_array_xhj;
		nArrayHandleTem.Add(nHandleLineStart);
// 		acutPrintf(_T("\n ������·�߹��Ĺ�����Σ�"));
// 		for(int i=0;i<nArrayHandleTem.GetCount();i++)
// 		{
// 			CString Gd_name;
// 			m_mysys.GetXdata(nArrayHandleTem.GetAt(i),_T("GUIDAONAME"),Gd_name);
// 			acutPrintf(_T("%s\t"),Gd_name);
// 		}
		vector<CString>tempGuidao;
	/*	TiquGuoDaocha(sVectorPath,tempGuidao);*/
		int j=0;
		for(j=0;j<nArrayHandleTem.GetCount();j++)
		{
			if(CBlkUtility::FindLongInArray(nArrayHandleStop,nArrayHandleTem.GetAt(j))!=-1)
			{
				//˵��������·�ڹ������
				break;
			}
		}
		if(j==nArrayHandleTem.GetCount())
		{
			iTJ_Cnt++;
			vec_str_tiaojian.push_back("<>");
			acutPrintf(_T("\n��Ҫ������"));
		}
		iJL_Cnt++;
		//return b_Ret;
	}
	else if(nHandleLineStart==0)
	{
		return ;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray Dacha;
//		CStringArray PassXHJ;
		//DiaoCheFindNextLineAndXHJ2(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,strArrayCXGdName,bForword);
		DiaoCheFindNextLineAndDaocha(nHandleLineStart,nHandleLineEnd,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					long thisdaocha=nArrayHandleThisDC[i];
					this->IsAddConditionDiaoChePath(nHandNext,nHandleLineEnd,nArrayHandleStop,thisdaocha,sDaocha,GuDao,nArrayHandleTem,vec_str_tiaojian,iJL_Cnt,iTJ_Cnt,bForword);
				}
			}
		}
	}
//	sVectorPath.pop_back();
}
/*******************************������GetDiaoChePathGuiDaoAndDiDuiXHJ*************************************************************************************************************************************************/
/***********������������ȡ������ʼ�����������ֹ�����ĵ���������Ρ��Լ�������·�ϵĵж��źŻ���������ĵ����źŻ�*******************************************************************************************/
/***********���룺 CString& StartXHJ����ʼ�źŻ����ƣ�CString& EndXHJ,���ն��źŻ����ƣ�long nHandleLineStart,��ʼ�źŻ����ڹ����long nHandleLineEnd��ֹ�źŻ����ڹ����bool bForword��·����*****/
/******************long LastDaocha,��һ����߹��ĵ���CStringArray &GuDao,�ɵ�����************************************************************************************************************************************/
/***********�����vector<CString>&sVectorPath,���ڴ洢�߹��Ĺ�����Σ�vector<CString>&sDaocha,�����߹��ĵ���***********************************************************************************************/
/***************** vector<CString>& sXHJ,�����߹��ĵж��źŻ���CLongArray&nArrayHandleLine�����浱ǰ������εĺ������***************************************************************************************/
/**********************************ע��---2015-06-05-pm 16:23-by-luo****************************************************************************************************************************************************/

void CZnSxt::GetDiaoChePathGuiDaoAndDiDuiXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//CStringArray strArray_XHJData;
		//TiquEffectiveData(sXHJ,strArray_XHJData);
		CStringArray str_array_xhj;
		TiquEffectiveData(sXHJ,str_array_xhj);
		CString AnniuEnd;
		CString AnniuStart;
		//����ն��źŻ����ڽ�·�ϣ��򷵻�
		CString temp=EndXHJ;
		CString jinlu;
		vector<CString> tempXHJ;
		//CStringArray str_array_xhj
		TiquGuoDaocha(sXHJ,tempXHJ);
		//TiquEffectiveData(sXHJ)
		int count=str_array_xhj.GetCount();
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//Ϊ�ɵ�ʱ
			jinlu=_T("��")+temp;
			AnniuEnd=str_array_xhj[count-1]+_T("DA");
		}
		else
		{
			//Ϊ�źŻ�
			long nHandleDC=0;
			GetHandleByName(temp,nHandleDC);
			//�ж��źŻ�����
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//����
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							AnniuEnd=str_array_xhj[count-2]+_T("A");
						}
						else
						{
							AnniuEnd=str_array_xhj[count-1]+_T("A");
						}
						//oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
					}
					else
					{
						jinlu=_T("��")+str_array_xhj[count-1];
						AnniuEnd =str_array_xhj[count-1]+_T("A");
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
						jinlu=_T("��")+temp;
						AnniuEnd =str_array_xhj[count-1]+_T("A");
					}
					else
					{
						jinlu=_T("��")+temp;
						if(count>1)
						{
							AnniuEnd=str_array_xhj[count-2]+_T("A");
						}
						else
							AnniuEnd=str_array_xhj[count-1]+_T("A");
					}
				}

			}
		}
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//CLongArray nArrayPassXHJ;
		oper_excel.WriteDatatoExcel(1,num,4,(num-3));
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//PrintGuoDaocha(sXHJ);
		//		vector<CString> tempXHJ;
		//	TiquGuoDaocha(sXHJ,tempXHJ);
		oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
		//д��·
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//дӭ���г���·
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
		}

		//GetEntName(nHandleStartXHJ,AnniuStart);
		oper_excel.WriteDatatoExcel(1,num,3,StartXHJ);
		oper_excel.WriteDatatoExcel(1,num,9,StartXHJ);
		AnniuStart=StartXHJ+_T("A");
		//AnniuEnd=temp+_T("A");
		CString JinLuAnniu=AnniuStart+_T("    ")+AnniuEnd;
		oper_excel.WriteDatatoExcel(1,num,7,JinLuAnniu);
		oper_excel.WriteDatatoExcel(1,num,10,_T("B"));
		num++;
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		CStringArray PassXHJ;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		DiaoCheFindNextLineAndXHJ(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,PassXHJ,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					sXHJ.push_back(PassXHJ[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					//this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					this->GetDiaoChePathGuiDaoAndXHJ(StartXHJ,EndXHJ,nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,sXHJ,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
					sXHJ.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
//������������յ�����Ѱ�ҽ�·���ҵ��Ľ�·����sVectorPath�С�sDaocha���ڴ洢��·�Ͼ����ĵ���Gudao�洢��·����ʱ�����Ŀ���ֹͣ�����Ĺ�����Ρ�nAarryHandleLine�洢��·��Ѱ���Ѿ����ʵĹ�����
void CZnSxt::GetPathGuiDao3( long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//�����
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//Ӧ��nHandleLineEnd�����е������ڶ�λ�����о�
		//ѹ���Ӧ�ĵ�����λ
		acutPrintf(_T("\n ��������"));
		for(int i=0;i<sVectorPath.size();i++)
		{
			//����������Ľ��
			acutPrintf(_T("\t��%d ���������Ϊ%s"),i,sVectorPath[i].GetString());
		}
		sVectorPath.pop_back();
		acutPrintf(_T("\n ����������Σ�%d"),nArrayHandleTem.GetCount());
		// 		��ӡ����������λ
		// 				acutPrintf(_T("\n ��������"));
		// 			 		for(int k=0;k<sDaocha.size();k++)
		// 				 	{
		// 			 			if(sDaocha[k]!=_T(""))
		// 							{
		// 				 				acutPrintf(_T("%s;\t"),sDaocha[k].GetString());
		// 						}
		// 				 	}
		PrintGuoDaocha(sDaocha);
		/*		sDaocha.pop_back();*/
		return;
	}
	else if(nHandleLineStart==0)
	{
		return;
	}
	else
	{
		//����ǰ��������ѷ���
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		//�洢�߹��ĵ���
		CStringArray Dacha;
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
		FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,LastDaocha,nArrayHandleThisDC,nArrayHandleNext,bForword);
		//FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,)
		int count =0;
		count =nArrayHandleNext.GetCount();
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				long nHandNext=0;
				nHandNext =nArrayHandleNext[i];
				//CLongArray nArrayHandleNext2;
				//��ӡ���
				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
				if(CBlkUtility::FindLongInArray(nArrayHandleTem,nHandNext)==-1)
				{
					CString name;
					m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
					sDaocha.push_back(Dacha[i]);
					long thisdaocha=nArrayHandleThisDC[i];
					this->GetPathGuiDao3(nHandNext,nHandleLineEnd,thisdaocha,sVectorPath,sDaocha,GuDao,nArrayHandleTem,bForword);
					//nArrayHandleLine.Copy(nArrayHandleTem);
					sDaocha.pop_back();
				}
				//	sDaocha.pop_back();
			}
		}
	}
	sVectorPath.pop_back();
}
// void CZnSxt::GetPathGuiDao( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
// {
// 	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
// 	{
// 		//�����
// 		CString Gd_name;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
// 		sVectorPath.push_back(Gd_name);
// 		//ѹ���Ӧ�ĵ�����λ
// 		acutPrintf(_T("\n ��������"));
// 		for(int i=0;i<sVectorPath.size();i++)
// 		{
// 			//����������Ľ��
// 			acutPrintf(_T("\t��%d �ɵ���Ϊ%s"),i,sVectorPath[i].GetString());
// 		}
// 		sVectorPath.pop_back();
// 		return;
// 	}
// 	else if(nHandleLineStart==0)
// 	{
// 		return;
// 	}
// 	else
// 	{
// 		CString Gd_name;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
// 		sVectorPath.push_back(Gd_name);
// 		CLongArray nArrayHandleNext;
// 		CStringArray Dacha;
// 		FindNextLine3(nHandleLineStart,nHandleLineEnd,Dacha,GuDao,nArrayHandleNext,bForword);
// 		int count =0;
// 		count =nArrayHandleNext.GetCount();
// 		if(count!=0)
// 		{
// 			for(int i=0;i<count;i++)
// 			{
// 				long nHandNext=0;
// 				nHandNext =nArrayHandleNext[i];
// 				CLongArray nArrayHandleNext2;
// 				CString name;
// 				m_mysys.GetXdata(nHandNext,_T("GUIDAONAME"),name);
// 				//��ӡ���
// 				//	acutPrintf(_T("\nnHandNext=%d \t name=%s"),nHandNext,name.GetString());
// 				this->GetPathGuiDao(nHandNext,nHandleLineEnd,sVectorPath,GuDao,nArrayHandleNext2,bForword);
// 			}
// 		}
// 	}
// 	sVectorPath.pop_back();
// }
// void CZnSxt::FindNextLine( long nHandleLineStart,long nHandleLineEnd,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
// {
// 	static colorindex=0;
// 	colorindex=colorindex+5;
// 	if(colorindex>255)
// 	{
// 			colorindex=0;
// 	}
//   try
//   {
//   // static int stop =0;
// 	if(nHandleLineStart==nHandleLineEnd)
// 	{//�ҵ�������
// 		return;
// 	}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		//acutPrintf(_T("�������"))��G
// 		//CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
// 		
// 	CADORecordset cSet(&m_AdoDb) ;
// 	CString strSql ;
// 	strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 	long nLineType = 0 ;
// 	m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 	//��ʾΪ��ͷ��
// 	if (nLineType==2) 
// 	{
// 		CString name;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 		acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
// 		return ;
// 	}
// 	//�жϹ����DC����
// 	CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 	//��ȡ�����Ϣ�����������Ҫ��ϸ����
// 	this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 	int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
// 	int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
// 	//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 	CString guiname;
// 	m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 	//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 	long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 	strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 	m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 	strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 	m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 	//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 	int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
// 	long nHandleNextLine = 0 ;
// 	strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 	CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 	//��ȡ�����
// 	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 	if(iNumDC<1)
// 	{
// 		//�������������ʱ�����ֻ��һ��
// 		if(nLineType==0)
// 		{//��Ϊ�������ʱ
// 			if(nHandleNextLine!=0)
// 			{
// 				nArrayHandleLine.Add(nHandleNextLine);
// // 				 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; ;
// // 				m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 			}
// 			else
// 			{
// 				acutPrintf(_T("\n nHandNextLineΪ0"));
// 			}
// 		}
// 		else if(nLineType==1)
// 		{//Ϊ�������ʱ
// 			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 			CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			//��ȡ�����
// 		//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 			//���߾�Ե�����Ҳ�ʱ
// 			if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
// 			{
// 				//�Ҷ��߾�Ե�������Ķ���
// 				if(bForword==true)
// 				{
//                     m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"), nHandleNextLine);
// 					if(nHandleNextLine!=0)
// 					{
// 						nArrayHandleLine.Add(nHandleNextLine);
// // 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// // 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 					}
// 
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLineΪ0"));
// 					}
// 				}
// 				else
// 				{
// 					//���Ϊ����λ���ڹ��
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//��λ�߶�Ӧ�ĵ���
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					//�������ڹ����Ϊ��̹��
// 				    m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 					if(nHandleNextLine!=0)
// 					{
// 						nArrayHandleLine.Add(nHandleNextLine);
// // 						 m_mysys.FlashEnt(nHandleNextLine,2000,200) ; 
// // 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 					}
// 					else
// 					{
// 					  acutPrintf(_T("\n nHandNextLineΪ0"));
// 					}
// 				}
// 			}
// 			//���߾�Ե���������ʱ
// 			else if(nHandleLeftJyj!=0&&nHandleRightJyj==0)
// 			{
// 				if(bForword==false)
// 				{
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"), nHandleNextLine);
// 					if(nHandleNextLine!=0)
// 					{
// 						nArrayHandleLine.Add(nHandleNextLine);
// // 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// // 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 					}
// 				}
// 				else
// 				{
// 					////���Ϊ����λ���ڹ��
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//��λ�߶�Ӧ�ĵ���
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					//�������ڹ����Ϊ��̹��
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 					if(nHandleNextLine!=0)
// 					{
// 						nArrayHandleLine.Add(nHandleNextLine);
// // 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// // 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 					}
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLineΪ0"));
// 					}
// 				}
// 
// 			}
// 		}
// 		//return;
// 	}
// 	else
// 	{//���е���ʱ�����ܴ��ڶ������
// 		//����Ϊ��λʱ���
// 		nArrayHandleLine.Add(nHandleNextLine);
// 		for(int k=0;k<iNumDC;k++)
// 		{//����λʱ���
// 			//ȡ����handle
// 			long nHandleDC=nArrayHandleDC[k];
// 			//��ѯ��λ������
// 			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 			long nHandle_fw=0;
// 			long nHandle_dw=0;
// 			int IsStart=-1;
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 			m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 			if(IsStart==0&&bForword==true)
// 			{
// 				//�����ƽ�ʱ���
// 				if(nHandle_dw!=0)
// 				{
// 					nArrayHandleLine.Add(nHandle_fw);
// // 					 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// // 					m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 				}
// 				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 				long nLineType = 0 ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 				acutPrintf(_T("\n nlineType=%d"),nLineType);
// 			}
// 			else if(IsStart>0&&bForword==false)
// 			{
// 				//�����ƽ�ʱ���
// 				if(nHandle_fw!=0)
// 				{
//                      nArrayHandleLine.Add(nHandle_fw);
// // 					 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// // 					 m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 				}
// 				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 				long nLineType = 0 ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 				acutPrintf(_T("\n nlineType=%d"),nLineType);
// 			}
// 			acutPrintf(_T("Isstart=%d"),IsStart);
// 		}
// 	}
// 
//   }
//   catch(_com_error& e)
//   {
// 	  CString sBuff = CBlkUtility::GetErrorDescription(e);
// 	  AfxMessageBox(sBuff);        
//   }
//   catch(...)
//   {	
// 	  AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
//   }	
// }
//��¼���ʹ��ĵ��������߻�·��˳���¼�߹�����Ķ���λ������������
// void CZnSxt::FindNextLine2( long nHandleLineStart,long nHandleLineEnd,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
// {
// 	static colorindex=0;
// 	colorindex=colorindex+5;
// 	if(colorindex>255)
// 	{
// 		colorindex=0;
// 	}
// 	try
// 	{
// 		// static int stop =0;
// 		if(nHandleLineStart==nHandleLineEnd)
// 		{//�ҵ�������
// 			return;
// 		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		//acutPrintf(_T("�������"))��G
// 		//CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
// 
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		//��ʾΪ��ͷ��
// 		if (nLineType==2) 
// 		{
// 			CString name;
// 			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
// 			return ;
// 		}
// 		//�жϹ����DC����
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//��ȡ�����Ϣ�����������Ҫ��ϸ����
// 		this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
// 		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//��ȡ�����
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//�������������ʱ�����ֻ��һ��
// 			if(nLineType==0)
// 			{//��Ϊ�������ʱ
// 				if(nHandleNextLine!=0)
// 				{
// 					nArrayHandleLine.Add(nHandleNextLine);
// 					// 				 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; ;
// 					// 				m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 				}
// 				else
// 				{
// 					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ�ߣ���ʱ����Ϊ����
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//��λ�߶�Ӧ�ĵ���
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					if(nHandleDC!=0)
// 					{
// 						//��ѯ�������ڵ��ߣ�����Ϊ���
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//��λ�߶�Ӧ�ĵ���
// 						long nHandle_dw=0;
//                        // long nHandle_fw=0;						
// 						int IsStart=-1;
// 						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 					    m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 						if(bForword==true&&IsStart>0)
// 						{
//                              //��λ������Ϊ���    
// 							nArrayHandleLine.Add(nHandle_dw);
// 						}
// 						else if(bForword==false&&IsStart==0)
// 						{
// 							nArrayHandleLine.Add(nHandle_dw);
// 						}
// 					}
// 					else
// 					{
//                        acutPrintf(_T("\n nHandNextLineΪ0"));
// 					}
// 				}
// 			}
// 			else if(nLineType==1)
// 			{//Ϊ�������ʱ
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 				CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 				//��ȡ�����
// 				//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 				//���߾�Ե�����Ҳ�ʱ
// 				if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
// 				{
// 					//�Ҷ��߾�Ե�������Ķ���
// 					if(bForword==true)
// 					{
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"), nHandleNextLine);
// 						if(nHandleNextLine!=0)
// 						{
// 							nArrayHandleLine.Add(nHandleNextLine);
// 							// 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// 							// 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 						}
// 						else
// 						{
// 							acutPrintf(_T("\n nHandNextLineΪ0"));
// 						}
// 					}
// 					else
// 					{
// 						//���Ϊ����λ���ڹ��
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//��λ�߶�Ӧ�ĵ���
// 						long nHandleDC=0;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 						//�������ڹ����Ϊ��̹��
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 						if(nHandleNextLine!=0)
// 						{
// 							nArrayHandleLine.Add(nHandleNextLine);
// 							// 						 m_mysys.FlashEnt(nHandleNextLine,2000,200) ; 
// 							// 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 						}
// 						else
// 						{
// 							acutPrintf(_T("\n nHandNextLineΪ0"));
// 						}
// 					}
// 				}
// 				//���߾�Ե���������ʱ
// 				else if(nHandleLeftJyj!=0&&nHandleRightJyj==0)
// 				{
// 					if(bForword==false)
// 					{
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"), nHandleNextLine);
// 						if(nHandleNextLine!=0)
// 						{
// 							nArrayHandleLine.Add(nHandleNextLine);
// 							// 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// 							// 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 						}
// 					}
// 					else
// 					{
// 						////���Ϊ����λ���ڹ��
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//��λ�߶�Ӧ�ĵ���
// 						long nHandleDC=0;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 						//�������ڹ����Ϊ��̹��
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 						if(nHandleNextLine!=0)
// 						{
// 							nArrayHandleLine.Add(nHandleNextLine);
// 							// 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// 							// 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 						}
// 						else
// 						{
// 							acutPrintf(_T("\n nHandNextLineΪ0"));
// 						}
// 					}
// 				}
// 			}
// 			//return;
// 		}
// 		else
// 		{//���е���ʱ�����ܴ��ڶ������
// 			//����Ϊ��λʱ���
// 			nArrayHandleLine.Add(nHandleNextLine);
// 			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
// 			for(int k=0;k<iNumDC;k++)
// 			{//����λʱ���,��Ҫǰ����ʵĵ���
// 				//ȡ����handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//��ѯ��λ������
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				long nHandle_fw=0;
// 				long nHandle_dw=0;
// 				int IsStart=-1;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//�����ƽ�ʱ���
// 					if(nHandle_fw!=0)
// 					{
// 						nArrayHandleLine.Add(nHandle_fw);
// 					}
// 					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 					long nLineType = 0 ;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 				//	acutPrintf(_T("\n nlineType=%d"),nLineType);
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//�����ƽ�ʱ���
// 					if(nHandle_fw!=0)
// 					{
// 						nArrayHandleLine.Add(nHandle_fw);
// 					}
// 					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 					long nLineType = 0 ;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 					//acutPrintf(_T("\n nlineType=%d"),nLineType);
// 				}
// 			//	acutPrintf(_T("Isstart=%d"),IsStart);
// 			}
// 		}
// 
// 	}
// 	catch(_com_error& e)
// 	{
// 		CString sBuff = CBlkUtility::GetErrorDescription(e);
// 		AfxMessageBox(sBuff);        
// 	}
// 	catch(...)
// 	{	
// 		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
// 	}	
// }
//��¼���ʹ��ĵ��������߻�·��˳���¼�߹�����Ķ���λ�������������¼��뷵�ؽ�·�ϵĵж��źŻ�
//���ڵж��źŻ����޲������ϵ��źŻ���Ϊ�ж��źŻ����в������ϵ��źŻ���ֻ���ڽ�·�ϣ��źŻ�����ж�
// void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,CLongArray &nArrayHandleDDXHJ,bool bForword/*=true*/ )
// {
// 	try
// 	{
// 		// static int stop =0;
// 		if(nHandleLineStart==nHandleLineEnd)
// 		{//�ҵ�������
// 			return;
// 		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		//��ʾΪ��ͷ��
// 		if (nLineType==2) 
// 		{
// 			CString name;
// 			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
// 			return ;
// 		}
// 		//�жϹ����DC����
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//��ȡ�����Ϣ�����������Ҫ��ϸ����
// 		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 		if(bForword==true)
// 		{
// 			this->GetGuiDaoInfoOrderByXAsc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
// 		}
// 		else
// 		{
// 			this->GetGuiDaoInfoOrderByXDesc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
// 		}
// 		//for test print DC
// 		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
// 		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//��ȡ�����
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//�������������ʱ�����ֻ��һ��
// 			if(nLineType==0)
// 			{//��Ϊ����ʱ
// 				if(nHandleNextLine!=0)
// 				{
// 					nArrayHandleLine.Add(nHandleNextLine);
// 					CString Daocha=_T("");
// 					GuoDaoCha.Add(Daocha);
// 					nArrayHandleDDXHJ.Copy(nArrayHandleXHJ);
// 				}
// 				else
// 				{
// 					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//��λ�߶�Ӧ�ĵ���
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					if(nHandleDC!=0)
// 					{
// 						//��ѯ�������ڵ��ߣ�����Ϊ���
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//��λ�߶�Ӧ�ĵ���
// 						long nHandle_dw=0;
// 						// long nHandle_fw=0;						
// 						int IsStart=-1;
// 						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 						CString DCname;
// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
// 						if(bForword==true&&IsStart>0)
// 						{
// 							//��λ������Ϊ���
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("��")+DCname+_T(")");
// 							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
// 							GuoDaoCha.Add(Daocha);
// 							nArrayHandleDDXHJ.Copy(nArrayHandleXHJ);
// 						}
// 						else if(bForword==false&&IsStart==0)
// 						{
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("��")+DCname+_T(")");
// 							//����λʱ����Ҫ�����Ƿ���ڴ�������
// 							GuoDaoCha.Add(Daocha);
// 							nArrayHandleDDXHJ.Copy(nArrayHandleXHJ);
// 						}
// 					}
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLineΪ0"));
// 					}
// 				}
// 			}
// 			else
// 			{
// 			}
// 			//return;
// 		}//endif NumDC
// 		else
// 		{//���е���ʱ�����ܴ��ڶ������
// 			//����Ϊ��λʱ���
// 			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
// 			CString DaochaD;
// 			for(int k=0;k<iNumDC;k++)
// 			{//����λʱ���,��Ҫǰ����ʵĵ���
// 				//ȡ����handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//��ѯ��λ������
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				long nHandle_fw=0;
// 				long nHandle_dw=0;
// 				int IsStart=-1;
// 				CString DCname1;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
// 				long nHanderDC_another=0;
// 				//�����ƽ��к��ʱ
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//�����ƽ�ʱ���
// 					if(nHandle_fw!=0)
// 					{
// 						//����������һ�κ�̣��ж��Ƿ�Ϊ����
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//Ϊ����ʱ����λ�ߵĺ��
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��ȡ�����
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//���Ҹ÷�λ�����ڵ���Ķ�λ��
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��λ�߶�Ӧ�ĵ���
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//��ѯ�������ڵ��ߣ�����Ϊ���
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//��λ�߶�Ӧ�ĵ���
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2��Ϊ���ߵĺ��
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//��ӡ���ߵ���
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
// 								long nHandle_Anothe_Min=0;
// 								long nHandle_Anothe_Max=0;
// 								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
// 								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 								{
// 									CString DC_Another_Min;
// 									CString DC_Another_Max;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//����λʱ����Ҫ�����Ƿ���ڴ�������
// 									GuoDaoCha.Add(Daocha);
// 								}						
// 							}
// 						}
// 						else
// 						{//��λ�߲�Ϊ����ʱ
// 							nArrayHandleLine.Add(nHandle_fw);
// 							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
// 							//����λʱ����Ҫ�����Ƿ���ڴ�������
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//�����ƽ�ʱ���
// 					if(nHandle_fw!=0)
// 					{
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//Ϊ����ʱ����λ�ߵĺ��
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��ȡ�����
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//���Ҹ÷�λ�����ڵ���Ķ�λ��
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��λ�߶�Ӧ�ĵ���
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//��ѯ�������ڵ��ߣ�����Ϊ���
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//��λ�߶�Ӧ�ĵ���
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2��Ϊ���ߵĺ��
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//��ӡ���ߵ���
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
// 								long nHandle_Anothe_Min=0;
// 								long nHandle_Anothe_Max=0;
// 								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 								{
// 									CString DC_Another_Min;
// 									CString DC_Another_Max;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//����λʱ����Ҫ�����Ƿ���ڴ�������
// 									GuoDaoCha.Add(Daocha);
// 								}
// 							}
// 						}
// 						else
// 						{
// 							nArrayHandleLine.Add(nHandle_fw);
// 							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 					//	acutPrintf(_T("Isstart=%d"),IsStart);
// 				}
// 				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
// 				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
// 				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
// 				if(b_dx==false)
// 				{
// 					if(k==iNumDC-1)
// 					{
// 						DaochaD=DaochaD+DCname1;
// 					}
// 					else
// 					{
// 						DaochaD=DaochaD+DCname1+_T("��");
// 					}
// 				}
// 				else
// 				{
// 					CString DCname_Min;
// 					CString DCname_Max;
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
// 					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
// 					long nHandle_Anothe_Min=0;
// 					long nHandle_Anothe_Max=0;
// 					if(k==iNumDC-1)
// 					{
// 						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 						{
// 							CString DC_Another_Min;
// 							CString DC_Another_Max;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
// 						}
// 					}
// 					else
// 					{
// 						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 						{
// 							CString DC_Another_Min;
// 							CString DC_Another_Max;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
// 						}
// 
// 					}
// 				}
// 			}
// 			nArrayHandleLine.Add(nHandleNextLine);
// 			GuoDaoCha.Add(DaochaD);
// 		}
// 	}
// 	catch(_com_error& e)
// 	{
// 		CString sBuff = CBlkUtility::GetErrorDescription(e);
// 		AfxMessageBox(sBuff);        
// 	}
// 	catch(...)
// 	{	
// 		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
// 	}	
// }
//��¼���ʹ��ĵ��������߻�·��˳���¼�߹�����Ķ���λ������������
void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//�����ʼλ�ɵ��������
			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			this->GetGuiDaoInfoOrderByXAsc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		else
		{
			this->GetGuiDaoInfoOrderByXDesc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString  Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			// 			else if(nLineType==1)
			// 			{
			// 				//Ϊ����ʱ
			// 				acutPrintf(_T("\n����Ϊ����"));
			// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
			// 				CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			// 				//��ȡ�����
			// 				//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
			// 				//���߾�Ե�����Ҳ�ʱ
			// 				if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
			// 				{
			// 					//�Ҷ��߾�Ե�������Ķ���
			// 					if(bForword==true)
			// 					{
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"), nHandleNextLine);
			// 						if(nHandleNextLine!=0)
			// 						{
			// 							nArrayHandleLine.Add(nHandleNextLine);
			// 						}
			// 
			// 						else
			// 						{
			// 							acutPrintf(_T("\n nHandNextLineΪ0"));
			// 						}
			// 					}
			// 					else 
			// 					{
			// 						//���Ϊ����λ���ڹ��
			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			// 						//��λ�߶�Ӧ�ĵ���
			// 						long nHandleDC=0;
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
			// 						//�������ڹ����Ϊ��̹��
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
			// 						if(nHandleNextLine!=0)
			// 						{
			// 							nArrayHandleLine.Add(nHandleNextLine);
			// 						}
			// 						else
			// 						{
			// 							acutPrintf(_T("\n nHandNextLineΪ0"));
			// 						}
			// 					}
			// 				}
			// 				//���߾�Ե���������ʱ
			// 				else if(nHandleLeftJyj!=0&&nHandleRightJyj==0)
			// 				{
			// 					if(bForword==false)
			// 					{
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"), nHandleNextLine);
			// 						if(nHandleNextLine!=0)
			// 						{
			// 							nArrayHandleLine.Add(nHandleNextLine);
			// 						}
			// 					}
			// 					else
			// 					{
			// 						////���Ϊ����λ���ڹ��
			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			// 						//��λ�߶�Ӧ�ĵ���
			// 						long nHandleDC=0;
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
			// 						//�������ڹ����Ϊ��̹��
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
			// 						if(nHandleNextLine!=0)
			// 						{
			// 							nArrayHandleLine.Add(nHandleNextLine);
			// 						}
			// 						else
			// 						{
			// 							acutPrintf(_T("\n nHandNextLineΪ0"));
			// 						}
			// 					}
			// 
			// 				}
			// 			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			for(int k=0;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							nArrayHandleLine.Add(nHandle_fw);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T("��");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}

					}
				}
			}
			nArrayHandleLine.Add(nHandleNextLine);
			GuoDaoCha.Add(DaochaD);
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//�����ʼλ�ɵ��������
			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			this->GetGuiDaoInfoOrderByXAsc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		else
		{
			this->GetGuiDaoInfoOrderByXDesc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString  Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T("��");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}

					}
				}
			}
			//���е���Ϊ��λʱ
			long nHandleThisDC=0;
			nArrayHandleThisDaocha.Add(nHandleThisDC);
			nArrayHandleLine.Add(nHandleNextLine);
			GuoDaoCha.Add(DaochaD);
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray& nArrayZhiXHJ,CLongArray& nArrayXiangXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//�����ʼλ�ɵ��������
			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			this->GetGuiDaoInfoOrderByXAsc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		else
		{
			this->GetGuiDaoInfoOrderByXDesc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					long Zhi_XHJ=0;
					long Xiang_XHJ=0;
					this->GetGuiDaoDuanXHJ(bForword,nHandleNextLine,Zhi_XHJ,Xiang_XHJ);
					nArrayZhiXHJ.Add(Zhi_XHJ);
					nArrayXiangXHJ.Add(Xiang_XHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString  Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pt_DC_x=0;
							double pt_zxhj_x=0;
							double pt_xxhj_x=0;
							long Zhi_XHJ=0;
							long Xiang_XHJ=0;
							this->GetGuiDaoDuanXHJ(bForword,nHandle_dw,Zhi_XHJ,Xiang_XHJ);
						
								if((GetBlockPosition_X(Zhi_XHJ,pt_zxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleDC,pt_DC_x)==TRUE))
								{
									if(pt_zxhj_x>pt_DC_x)
										nArrayZhiXHJ.Add(Zhi_XHJ);
								}
								else
								{
									nArrayZhiXHJ.Add(0);
								}
								if((GetBlockPosition_X(Zhi_XHJ,pt_xxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleDC,pt_DC_x)==TRUE))
								{
									if(pt_xxhj_x>pt_DC_x)
										nArrayXiangXHJ.Add(Xiang_XHJ);
								}
								else
								{
									nArrayXiangXHJ.Add(0);
								}
							nArrayZhiXHJ.Add(Zhi_XHJ);
							nArrayXiangXHJ.Add(Xiang_XHJ);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
							double pt_DC_x=0;
							double pt_zxhj_x=0;
							double pt_xxhj_x=0;
							long Zhi_XHJ=0;
							long Xiang_XHJ=0;
							if((GetBlockPosition_X(Zhi_XHJ,pt_zxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleDC,pt_DC_x)==TRUE))
							{
								if(pt_zxhj_x<pt_DC_x)
									nArrayZhiXHJ.Add(Zhi_XHJ);
							}
							else
							{
								nArrayZhiXHJ.Add(0);
							}
							if((GetBlockPosition_X(Zhi_XHJ,pt_xxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleDC,pt_DC_x)==TRUE))
							{
								if(pt_xxhj_x<pt_DC_x)
									nArrayXiangXHJ.Add(Xiang_XHJ);
							}
							else
							{
								nArrayXiangXHJ.Add(0);
							}
							nArrayZhiXHJ.Add(Zhi_XHJ);
							nArrayXiangXHJ.Add(Xiang_XHJ);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									//long Zhi_XHJ=0;
									//long Xiang_XHJ=0;
									//this->GetGuiDaoDuanXHJ(bForword,nHandle_dw2,Zhi_XHJ,Xiang_XHJ);
									//nArrayZhiXHJ.Add(Zhi_XHJ);
									//nArrayXiangXHJ.Add(Xiang_XHJ);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pt_DC_x=0;
									double pt_zxhj_x=0;
									double pt_xxhj_x=0;
									long Zhi_XHJ=0;
									long Xiang_XHJ=0;
									this->GetGuiDaoDuanXHJ(bForword,nHandle_dw2,Zhi_XHJ,Xiang_XHJ);
									if((GetBlockPosition_X(Zhi_XHJ,pt_zxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
									{
										if(pt_zxhj_x>pt_DC_x)
											nArrayZhiXHJ.Add(Zhi_XHJ);
									}
									else
									{
										nArrayZhiXHJ.Add(0);
									}
									if((GetBlockPosition_X(Zhi_XHJ,pt_xxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
									{
										if(pt_xxhj_x>pt_DC_x)
											nArrayXiangXHJ.Add(Xiang_XHJ);
									}
									else
									{
										nArrayXiangXHJ.Add(0);
									}
									nArrayZhiXHJ.Add(Zhi_XHJ);
									nArrayXiangXHJ.Add(Xiang_XHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							long Zhi_XHJ=0;
							long Xiang_XHJ=0;
							this->GetGuiDaoDuanXHJ(bForword,nHandle_fw,Zhi_XHJ,Xiang_XHJ);
							nArrayZhiXHJ.Add(Zhi_XHJ);
							nArrayXiangXHJ.Add(Xiang_XHJ);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									//long Zhi_XHJ=0;
									//long Xiang_XHJ=0;
									//this,GetGuiDaoDuanXHJ(bForword,nHandleNextLine,Zhi_XHJ,Xiang_XHJ);
									//nArrayZhiXHJ.Add(Zhi_XHJ);
									//nArrayXiangXHJ.Add(Xiang_XHJ);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pt_DC_x=0;
									double pt_zxhj_x=0;
									double pt_xxhj_x=0;
									long Zhi_XHJ=0;
									long Xiang_XHJ=0;
									this->GetGuiDaoDuanXHJ(bForword,nHandle_dw2,Zhi_XHJ,Xiang_XHJ);
									if((GetBlockPosition_X(Zhi_XHJ,pt_zxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
									{
										if(pt_zxhj_x>pt_DC_x)
											nArrayZhiXHJ.Add(Zhi_XHJ);
									}
									else
									{
										nArrayZhiXHJ.Add(0);
									}
									if((GetBlockPosition_X(Zhi_XHJ,pt_xxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
									{
										if(pt_xxhj_x>pt_DC_x)
											nArrayXiangXHJ.Add(Xiang_XHJ);
									}
									else
									{
										nArrayXiangXHJ.Add(0);
									}
									nArrayZhiXHJ.Add(Zhi_XHJ);
									nArrayXiangXHJ.Add(Xiang_XHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pt_DC_x=0;
							double pt_zxhj_x=0;
							double pt_xxhj_x=0;
							long Zhi_XHJ=0;
							long Xiang_XHJ=0;
							this->GetGuiDaoDuanXHJ(bForword,nHandle_fw,Zhi_XHJ,Xiang_XHJ);
							if(bForword==true)
							{
								if((GetBlockPosition_X(Zhi_XHJ,pt_zxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
								{
									if(pt_zxhj_x>pt_DC_x)
										nArrayZhiXHJ.Add(Zhi_XHJ);
								}
								else
								{
									nArrayZhiXHJ.Add(0);
								}
								if((GetBlockPosition_X(Zhi_XHJ,pt_xxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
								{
									if(pt_xxhj_x>pt_DC_x)
										nArrayXiangXHJ.Add(Xiang_XHJ);
								}
								else
								{
									nArrayXiangXHJ.Add(0);
								}

							}
							else
							{
								if((GetBlockPosition_X(Zhi_XHJ,pt_zxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
								{
									if(pt_zxhj_x<pt_DC_x)
										nArrayZhiXHJ.Add(Zhi_XHJ);
								}
								else
								{
									nArrayZhiXHJ.Add(0);
								}
								if((GetBlockPosition_X(Zhi_XHJ,pt_xxhj_x)==TRUE)&&(GetBlockPosition_X(nHandleThisDC,pt_DC_x)==TRUE))
								{
									if(pt_xxhj_x<pt_DC_x)
										nArrayXiangXHJ.Add(Xiang_XHJ);
								}
								else
								{
									nArrayXiangXHJ.Add(0);
								}

							}
							nArrayZhiXHJ.Add(Zhi_XHJ);
							nArrayXiangXHJ.Add(Xiang_XHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T("��");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}

					}
				}
			}
			//���е���Ϊ��λʱ
			long nHandleThisDC=0;
			nArrayHandleThisDaocha.Add(nHandleThisDC);
			nArrayHandleLine.Add(nHandleNextLine);
			long Zhi_XHJ=0;
			long Xiang_XHJ=0;
			this->GetGuiDaoDuanXHJ(bForword,nHandleNextLine,Zhi_XHJ,Xiang_XHJ);
			nArrayZhiXHJ.Add(Zhi_XHJ);
			nArrayXiangXHJ.Add(Xiang_XHJ);
			GuoDaoCha.Add(DaochaD);
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
/*******************����˵��:FindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********���ܣ�ͨ��һ����·����ʼ�������ֹ������ҳ���ʼ����ĺ�̹���������ش���ʼ�������һ��������߹��ĵ����Լ����ڽ�·�ϵ��źŻ�����̹�������ж���*******************************************************************/
/**********���룺nHandleLineStart,��·����ʼ����ߣ�nHandLineEnd,�趨��·����ֹ����ߣ�������ֹ��·Ѱ�ҹ���*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ���bForward,���ڱ��Ѱ�Һ�̷��ߣ�trueΪ�����ƽ�***************************************************************************/
/*****************Gudao���ɵ����ƣ�����������ֹ����Ѱ�ң�JieJinQuDuan���ӽ���������;******************************************************************************************************/
/**********�����GuoDaocha,���ڼ�¼��Ѱ�ҽ�·�Ĺ������߹��ĵ���,�����������е�һ�����ݣ� nArrayHandleThisDaocha������һ����·ʱ�߹���Ӧ����*************************************/
/*****************nArrayHandleNextLine,�������еĺ�̹������;  GuoXHJ,��һ����·ʱ�߹����źŻ����źŻ����ڽ�·�ڲ�*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		//long t1=GetTickCount();
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			CLongArray nArrayPassXHJ;
			bool btoward=bForword;
			this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
			CString  Name_PassXHJ;
			for(int k=0;k<nArrayPassXHJ.GetCount();k++)
			{
				CString name_xhj;
				GetEntName(nArrayPassXHJ[k],name_xhj);
				CString EntName;
				GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
				 	name_xhj+=_T("D");
				}
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//ȥ�������źŻ������ɵ�ʱ������ȥ�����ƣ��޸���2015-09-17 18:13 
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
// 			long t2=GetTickCount();
// 			CString str;
			//str.Format("DiaoCheFindNextLineAndXHJ time:%d ms",t2-t1);
			//acutPrintf(_T("\n %s"),str);
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					//���ڵ�����˵��Ӧ��ֻ��������ĵ����źŻ�
					CString EntName;
					GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name_xhj+=_T("D");
					}
				    Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");
				}
// 				CString Daocha=_T("");
// 				GuoDaoCha.Add(Daocha);
				GuoXHJ.Add(Name_PassXHJ);
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//��ȡ�������źŻ�
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						CString EntName;
						GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
						if(EntName.Left(6)==_T("XHJ_LF"))
						{
							name_xhj+=_T("D");
						}
					    Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");

					}
					GuoXHJ.Add(Name_PassXHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T("��");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}

					}
				}
			}
			//���е���Ϊ��λʱ
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
				CLongArray nPassXHJ;
				CString  Name_PassXHJ=_T("");
				//GetBlockPosition_X(nHandleThisDC,pointx);
				//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
				//���е���Ϊ��λʱ���
				int b_toward=(bForword);
				GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
				for(int k=0;k<nPassXHJ.GetCount();k++)
				{
					CString name;
					GetEntName(nPassXHJ[k],name);
					CString EntName;
					GetEntBlkNameByHandle(nPassXHJ[k],EntName);
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name+=_T("D");
					}
					if(name==_T("X3"))
					{
						name=name;
					}
					Name_PassXHJ=Name_PassXHJ+name+_T("��");
				}
				GuoXHJ.Add(Name_PassXHJ);
				GuoDaoCha.Add(DaochaD);
			}
			else
			{
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString Daocha=DaochaD+_T("��")+_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							// Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

						}
						else
						{
							// Daocha=_T("(")+DCname+_T(")");

						}
						//CString Daocha=_T("(")+DCname+_T(")");
						// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
						GuoDaoCha.Add(Daocha);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						CString Daocha=DaochaD+_T("��")+_T("(")+DCname+_T(")");
						//����λʱ����Ҫ�����Ƿ���ڴ�������
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						//CString Daocha;
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							//CString Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
						}
						else
						{
							//CString Daocha=_T("(")+DCname+_T(")");
						}
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
						GuoDaoCha.Add(Daocha);
					}
					else
					{
					}
				}
			}
	}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}

//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
/*******************����˵��:FindNextLineAndXHJ2*******************************************************************************************************************************************************************/
/**********���ܣ�ͨ��һ����·����ʼ�������ֹ������ҳ���ʼ����ĺ�̹���������ش���ʼ�������һ��������߹��ĵ����Լ����ڽ�·�ϵ��źŻ�����̹�������ж���*******************************************************************/
/**********���룺nHandleLineStart,��·����ʼ����ߣ�nHandLineEnd,�趨��·����ֹ����ߣ�������ֹ��·Ѱ�ҹ���*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ���bForward,���ڱ��Ѱ�Һ�̷��ߣ�trueΪ�����ƽ�***************************************************************************/
/*****************Gudao���ɵ����ƣ�����������ֹ����Ѱ�ң�JieJinQuDuan���ӽ���������;******************************************************************************************************/
/**********�����GuoDaocha,���ڼ�¼��Ѱ�ҽ�·�Ĺ������߹��ĵ���,�����������е�һ�����ݣ� nArrayHandleThisDaocha������һ����·ʱ�߹���Ӧ����*************************************/
/*****************nArrayHandleNextLine,�������еĺ�̹������;  GuoXHJ,��һ����·ʱ�߹����źŻ����źŻ����ڽ�·�ڲ�*******************************************************************/
/*****************CStringArray&strCXGuiDaoName,���ڼ�¼��Ҫռ�õĳ�������******************************************************************************************************************************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndXHJ2( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray&strCXGuiDaoName,bool bForword/*=true*/ )
{
	try
	{
		//long t1=GetTickCount();
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			CLongArray nArrayPassXHJ;
			bool btoward=bForword;
			this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
			CString  Name_PassXHJ;
			for(int k=0;k<nArrayPassXHJ.GetCount();k++)
			{
				CString name_xhj;
				GetEntName(nArrayPassXHJ[k],name_xhj);
				CString EntName;
				GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
					name_xhj+=_T("D");
				}
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//�����źŻ������ɵ�ʱ����
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
		//	acutPrintf(_T("\n ��ͷ�ߣ�%s  handle=%ld"),name.GetString(),nHandleLineStart);
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					//���ڵ�����˵��Ӧ��ֻ��������ĵ����źŻ�
					CString EntName;
					GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name_xhj+=_T("D");
					}
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
				}
				GuoXHJ.Add(Name_PassXHJ);
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					//���ӳ�������
					int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
					if(i!=-1)
					{
						strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
					}
					else
					{
						strCXGuiDaoName.Add(_T(""));
					}
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//��ȡ�������źŻ�
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						CString EntName;
						GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
						if(EntName.Left(6)==_T("XHJ_LF"))
						{
							name_xhj+=_T("D");
						}
						Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;

					}
					GuoXHJ.Add(Name_PassXHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString str_Daidong;
							CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");
							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");
							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T(",");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max+_T(",");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}
			//���е���Ϊ��λʱ
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
				int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
				if(i!=-1)
				{
					strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
				}
				else
				{
					strCXGuiDaoName.Add(_T(""));
				}
				CLongArray nPassXHJ;
				CString  Name_PassXHJ=_T("");
				//GetBlockPosition_X(nHandleThisDC,pointx);
				//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
				//���е���Ϊ��λʱ���
				int b_toward=(bForword);
				GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
				for(int k=0;k<nPassXHJ.GetCount();k++)
				{
					CString name;
					GetEntName(nPassXHJ[k],name);
					CString EntName;
					GetEntBlkNameByHandle(nPassXHJ[k],EntName);
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name+=_T("D");
					}
					if(name==_T("X3"))
					{
						name=name;
					}
					Name_PassXHJ=Name_PassXHJ+name+_T(",");;
				}
				GuoXHJ.Add(Name_PassXHJ);
				GuoDaoCha.Add(DaochaD);
			}
			else
			{
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						CString str_Daidong;
						//CString Daocha=_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
						{
							Daocha=Daocha+_T(",")+str_Daidong;
						}
						else
						{
							// Daocha=_T("(")+DCname+_T(")");

						}
						//CString Daocha=_T("(")+DCname+_T(")");
						// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
						GuoDaoCha.Add(Daocha);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						//����λʱ����Ҫ�����Ƿ���ڴ�������
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						//CString Daocha;
						CString str_Daidong;
						//CString Daocha=_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
						{
							Daocha=Daocha+_T(",")+str_Daidong;
						}
						else
						{
							//CString Daocha=_T("(")+DCname+_T(")");
						}
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
						GuoDaoCha.Add(Daocha);
					}
					else
					{
					}
				}
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in DiaoCheFindNextLineAndXHJ"));
	}	

}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
/*******************����˵��:FindNextLineAndXHJ2*******************************************************************************************************************************************************************/
/**********���ܣ�ͨ��һ����·����ʼ�������ֹ������ҳ���ʼ����ĺ�̹���������ش���ʼ�������һ��������߹��ĵ����Լ����ڽ�·�ϵ��źŻ�����̹�������ж���*******************************************************************/
/**********���룺nHandleLineStart,��·����ʼ����ߣ�nHandLineEnd,�趨��·����ֹ����ߣ�������ֹ��·Ѱ�ҹ���*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ���bForward,���ڱ��Ѱ�Һ�̷��ߣ�trueΪ�����ƽ�***************************************************************************/
/*****************Gudao���ɵ����ƣ�����������ֹ����Ѱ�ң�JieJinQuDuan���ӽ���������;******************************************************************************************************/
/**********�����GuoDaocha,���ڼ�¼��Ѱ�ҽ�·�Ĺ������߹��ĵ���,�����������е�һ�����ݣ� nArrayHandleThisDaocha������һ����·ʱ�߹���Ӧ����*************************************/
/*****************nArrayHandleNextLine,�������еĺ�̹������;  GuoXHJ,��һ����·ʱ�߹����źŻ����źŻ����ڽ�·�ڲ�*******************************************************************/
/*****************CStringArray&strCXGuiDaoName,���ڼ�¼��Ҫռ�õĳ�������******************************************************************************************************************************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndDaocha( long nHandleLineStart,long nHandleLineEnd,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//�����źŻ������ɵ�ʱ����
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
		//	acutPrintf(_T("\n ��ͷ�ߣ�%s  handle=%ld"),name.GetString(),nHandleLineStart);
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		if(bForword==true)
		{
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
			
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);			
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
								}
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ	
							nArrayHandleLine.Add(nHandle_fw);
							//long nHandleThisDC=0;
							//nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);			
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
			}
			//���е���Ϊ��λʱ
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
			}
			else
			{
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);		
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);		
					}
					else
					{
					}
				}
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in DiaoCheFindNextLineAndDaocha"));
	}	

}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
/*******************����˵��:FindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********���ܣ�ͨ��һ����·����ʼ�������ֹ������ҳ���ʼ����ĺ�̹���������ش���ʼ�������һ��������߹��ĵ����Լ����ڽ�·�ϵ��źŻ�����̹�������ж���*******************************************************************/
/**********���룺nHandleLineStart,��·����ʼ����ߣ�nHandLineEnd,�趨��·����ֹ����ߣ�������ֹ��·Ѱ�ҹ���*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ���bForward,���ڱ��Ѱ�Һ�̷��ߣ�trueΪ�����ƽ�***************************************************************************/
/*****************Gudao���ɵ����ƣ�����������ֹ����Ѱ�ң�JieJinQuDuan���ӽ���������;******************************************************************************************************/
/**********�����GuoDaocha,���ڼ�¼��Ѱ�ҽ�·�Ĺ������߹��ĵ���,�����������е�һ�����ݣ� nArrayHandleThisDaocha������һ����·ʱ�߹���Ӧ����*************************************/
/*****************nArrayHandleNextLine,�������еĺ�̹������;  GuoXHJ,��һ����·ʱ�߹����źŻ����źŻ����ڽ�·�ڲ�*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray&strCXGuiDaoName, bool bForword/*=true*/ )
{
	try
	{
		//long t1=GetTickCount();
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			CLongArray nArrayPassXHJ;
			bool btoward=bForword;
			this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
			CString  Name_PassXHJ;
			for(int k=0;k<nArrayPassXHJ.GetCount();k++)
			{
				CString name_xhj;
				GetEntName(nArrayPassXHJ[k],name_xhj);
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
			}
			GuoXHJ.Add(Name_PassXHJ);
// 			long t2=GetTickCount();
// 			CString str;
// 			str.Format("DiaoCheFindNextLineAndXHJ time:%d ms",t2-t1);
			//acutPrintf(_T("\n %s"),str);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//�����źŻ������ɵ�ʱ����
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			return;
		}
		else
		{
			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
// 			long t2=GetTickCount();
// 			CString str;
// 			str.Format("DiaoCheFindNextLineAndXHJ time:%d ms",t2-t1);
// 			acutPrintf(_T("\n %s"),str);
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType;
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					//���ڵ�����˵��Ӧ��ֻ��������ĵ����źŻ�
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
				}
				GuoXHJ.Add(Name_PassXHJ);
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					//���ӳ�������
					int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
					if(i!=-1)
					{
						strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
					}
					else
					{
						strCXGuiDaoName.Add(_T(""));
					}
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//��ȡ�������źŻ�
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");

					}
					GuoXHJ.Add(Name_PassXHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T(",");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max+_T(",");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}
			//���е���Ϊ��λʱ
			// 			long nHandleThisDC=0;
			// 			nArrayHandleThisDaocha.Add(nHandleThisDC);
			// 			nArrayHandleLine.Add(nHandleNextLine);
			// 			CLongArray nPassXHJ;
			// 			CString  Name_PassXHJ=_T("");
			// 			//GetBlockPosition_X(nHandleThisDC,pointx);
			// 			//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
			// 			//���е���Ϊ��λʱ���
			// 			int b_toward=(bForword);
			// 			GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
			// 			for(int k=0;k<nPassXHJ.GetCount();k++)
			// 			{
			// 				CString name;
			// 				GetEntName(nPassXHJ[k],name);
			// 				Name_PassXHJ=Name_PassXHJ+name+_T("��");
			// 			}
			// 			GuoXHJ.Add(Name_PassXHJ);
			// 			GuoDaoCha.Add(DaochaD);
			// 	}
			//���е���Ϊ��λʱ
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
				int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
				if(i!=-1)
				{
					strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
				}
				else
				{
					strCXGuiDaoName.Add(_T(""));
				}
				CLongArray nPassXHJ;
				CString  Name_PassXHJ=_T("");
				//GetBlockPosition_X(nHandleThisDC,pointx);
				//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
				//���е���Ϊ��λʱ���
				int b_toward=(bForword);
				GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
				for(int k=0;k<nPassXHJ.GetCount();k++)
				{
					CString name;
					GetEntName(nPassXHJ[k],name);
					if(name==_T("X3"))
					{
						name=name;
					}
					Name_PassXHJ=Name_PassXHJ+name+_T(",");
				}
				GuoXHJ.Add(Name_PassXHJ);
				GuoDaoCha.Add(DaochaD);
			}
			else
			{
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							// Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

						}
						else
						{
							// Daocha=_T("(")+DCname+_T(")");

						}
						//CString Daocha=_T("(")+DCname+_T(")");
						// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
						GuoDaoCha.Add(Daocha);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						//����λʱ����Ҫ�����Ƿ���ڴ�������
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						//CString Daocha;
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							//CString Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
						}
						else
						{
							//CString Daocha=_T("(")+DCname+_T(")");
						}
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
						GuoDaoCha.Add(Daocha);
					}
					else
					{
					}
				}
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
/*******************����˵��:LieCheFindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********���ܣ�ͨ��һ����·����ʼ�������ֹ������ҳ���ʼ����ĺ�̹���������ش���ʼ�������һ��������߹��ĵ����Լ����ڽ�·�ϵ��źŻ�����̹�������ж���*******************************************************************/
/**********���룺nHandleLineStart,��·����ʼ����ߣ�nHandLineEnd,�趨��·����ֹ����ߣ�������ֹ��·Ѱ�ҹ���*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ���bForward,���ڱ��Ѱ�Һ�̷��ߣ�trueΪ�����ƽ�***************************************************************************/
/*****************Gudao���ɵ����ƣ�����������ֹ����Ѱ�ң�JieJinQuDuan���ӽ���������;******************************************************************************************************/
/**********�����GuoDaocha,���ڼ�¼��Ѱ�ҽ�·�Ĺ������߹��ĵ���,�����������е�һ�����ݣ� nArrayHandleThisDaocha������һ����·ʱ�߹���Ӧ����*************************************/
/*****************nArrayHandleNextLine,�������еĺ�̹������;  GuoXHJ,��һ����·ʱ�߹����źŻ����źŻ����ڽ�·�ڲ�*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
/*˵����ר�����ڷ�����·�������Ѱ�ң��������źŻ�д������������źŻ�*/
void CZnSxt::FaCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray &strCXGuiDaoName,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			CLongArray nArrayPassXHJ;
			bool btoward=bForword;
			this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
			CString  Name_PassXHJ;
			for(int k=0;k<nArrayPassXHJ.GetCount();k++)
			{
				CString name_xhj;
				GetEntName(nArrayPassXHJ[k],name_xhj);
				CString EntName;
				GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
				//�������źŻ�д��������źŻ�
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
					name_xhj+=_T("D");
				}
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//�г�����������𣬵�������Gudao������ֹ
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
	//	int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					CString EntName;
					GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
					//�������źŻ�д��������źŻ�
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name_xhj+=_T("D");
					}
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
				}
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
					if(i!=-1)
					{
						strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
					}
					else
					{
						strCXGuiDaoName.Add(_T(""));
					}
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//��ȡ�������źŻ�
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						CString EntName;
						GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
						//�������źŻ�д��������źŻ�
						if(EntName.Left(6)==_T("XHJ_LF"))
						{
							name_xhj+=_T("D");
						}
						Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
					}
					GuoXHJ.Add(Name_PassXHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
						//�����Ƿ���ȻΪĳ����ķ�λ��
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T(",");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max+_T(",");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}//endfor
			//���е���Ϊ��λʱ
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
				int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
				if(i!=-1)
				{
					strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
				}
				else
				{
					strCXGuiDaoName.Add(_T(""));
				}
				CLongArray nPassXHJ;
				CString  Name_PassXHJ=_T("");
				//GetBlockPosition_X(nHandleThisDC,pointx);
				//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
				//���е���Ϊ��λʱ���
				int b_toward=(bForword);
				GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
				for(int k=0;k<nPassXHJ.GetCount();k++)
				{
					CString name;
					GetEntName(nPassXHJ[k],name);
					CString EntName;
					GetEntBlkNameByHandle(nPassXHJ[k],EntName);
					//�������źŻ�д��������źŻ�
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name+=_T("D");
					}
					Name_PassXHJ=Name_PassXHJ+name+_T(",");;
				}
				GuoXHJ.Add(Name_PassXHJ);
				GuoDaoCha.Add(DaochaD);
			}
			else
			{
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							// Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

						}
						else
						{
							// Daocha=_T("(")+DCname+_T(")");

						}
						//CString Daocha=_T("(")+DCname+_T(")");
						// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
						GuoDaoCha.Add(Daocha);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						//����λʱ����Ҫ�����Ƿ���ڴ�������
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						//CString Daocha;
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							//CString Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T(",")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
						}
						else
						{
							//CString Daocha=_T("(")+DCname+_T(")");
						}
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
						GuoDaoCha.Add(Daocha);
					}
					else
					{
					}
			}
		}
	}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
void CZnSxt::LieCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray &strCXGuiDaoName,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			CLongArray nArrayPassXHJ;
			bool btoward=bForword;
			this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
			CString  Name_PassXHJ;
			for(int k=0;k<nArrayPassXHJ.GetCount();k++)
			{
				CString name_xhj;
				GetEntName(nArrayPassXHJ[k],name_xhj);
// 				CString EntName;
// 				GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
// 				//�������źŻ�д��������źŻ�
// 				if(EntName.Left(6)==_T("XHJ_LF"))
// 				{
// 					name_xhj+=_T("D");
//			}
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//�г�����������𣬵�������Gudao������ֹ
		// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		// 		{
		// 			//�����ʼλ�ɵ��������
		// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
		// 			return;
		// 		}
		// 		else
		// 		{
		// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		//	int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					if(name_xhj==_T("X3"))
					{
						name_xhj=name_xhj;
					}
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
				}
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
					if(i!=-1)
					{
						strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
					}
					else
					{
						strCXGuiDaoName.Add(_T(""));
					}
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//��ȡ�������źŻ�
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						if(name_xhj==_T("X3"))
						{
							name_xhj=name_xhj;
						}
						Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
					}
					GuoXHJ.Add(Name_PassXHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString Daocha=_T("(")+DCname+_T(")");
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
						//�����Ƿ���ȻΪĳ����ķ�λ��
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									CString strCXTemp;
									int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw2);
									if(i!=-1)
									{
										strCXTemp=this->strArrayGuiDaoCX.GetAt(i);
									}
									else
									{
										strCXTemp=_T("");
									}
									int j=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
									if(j!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(j);
									}
									else
									{
										strCXTemp+=_T("");
									}
									int k=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine2);
									if(k!=-1)
									{
										strCXTemp+=this->strArrayGuiDaoCX.GetAt(k);
									}
									else
									{
										strCXTemp+=_T("");
									}
									strCXGuiDaoName.Add(strCXTemp);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									CString str_Daidong;
									//CString Daocha=_T("(")+DCname+_T(")");
									if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
									{
										Daocha=Daocha+_T(",")+str_Daidong;
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_fw);
							if(i!=-1)
							{
								strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
							}
							else
							{
								strCXGuiDaoName.Add(_T(""));
							}
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							CString str_Daidong;
							//CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
							{
								Daocha=Daocha+_T(",")+str_Daidong;
							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T(",");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",")+DC_Another_Min+_T("/")+DC_Another_Max+_T(",");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}//endfor
			//���е���Ϊ��λʱ
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
				int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandleNextLine);
				if(i!=-1)
				{
					strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
				}
				else
				{
					strCXGuiDaoName.Add(_T(""));
				}
				CLongArray nPassXHJ;
				CString  Name_PassXHJ=_T("");
				//GetBlockPosition_X(nHandleThisDC,pointx);
				//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
				//���е���Ϊ��λʱ���
				int b_toward=(bForword);
				GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
				for(int k=0;k<nPassXHJ.GetCount();k++)
				{
					CString name;
					GetEntName(nPassXHJ[k],name);
					if(name==_T("X3"))
					{
						name=name;
					}
					Name_PassXHJ=Name_PassXHJ+name+_T(",");;
				}
				GuoXHJ.Add(Name_PassXHJ);
				GuoDaoCha.Add(DaochaD);
			}
			else
			{
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						CString str_Daidong;
						//CString Daocha=_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
						{
							Daocha=Daocha+_T(",")+str_Daidong;
						}
						else
						{
							// Daocha=_T("(")+DCname+_T(")");

						}
						//CString Daocha=_T("(")+DCname+_T(")");
						// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
						GuoDaoCha.Add(Daocha);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
						int i=CBlkUtility::FindLongInArray(this->nArrayHandLineCX,nHandle_dw);
						if(i!=-1)
						{
							strCXGuiDaoName.Add(this->strArrayGuiDaoCX.GetAt(i));
						}
						else
						{
							strCXGuiDaoName.Add(_T(""));
						}
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						CString Daocha=DaochaD+_T(",")+_T("(")+DCname+_T(")");
						//����λʱ����Ҫ�����Ƿ���ڴ�������
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString str_Daidong;
						//CString Daocha=_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,str_Daidong)==TRUE)
						{
							Daocha=Daocha+_T(",")+str_Daidong;
						}
						else
						{
							//CString Daocha=_T("(")+DCname+_T(")");
						}
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
						GuoDaoCha.Add(Daocha);
					}
					else
					{
					}
				}
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//�ú�������Ӽ�¼�ϴη��ʹ��ĵ�����Ҫ������Ѱ�Һ�̵�ʱ��ֻ�����ڽ�·�ĵ���д��GuoDaoCha,LastDaocha���ڼ�¼�ϴε�nHandleLineStart�ĵ���nArrayHandleThisDaocha,���ڼ�¼��������ʵĵ����
//��ԭ�����ϼ����������ϵ��źŻ�������ͬ�����źŻ��ͱ����źŻ����źŻ�����Ϊ�����źŻ���Ҳ����Ϊ�г��źŻ�
/*******************����˵��:FindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********���ܣ�ͨ��һ����·����ʼ�������ֹ������ҳ���ʼ����ĺ�̹���������ش���ʼ�������һ��������߹��ĵ����Լ����ڽ�·�ϵ��źŻ�����̹�������ж���*******************************************************************/
/**********���룺nHandleLineStart,��·����ʼ����ߣ�nHandLineEnd,�趨��·����ֹ����ߣ�������ֹ��·Ѱ�ҹ���*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ���bForward,���ڱ��Ѱ�Һ�̷��ߣ�trueΪ�����ƽ�***************************************************************************/
/*****************Gudao���ɵ����ƣ�����������ֹ����Ѱ�ң�JieJinQuDuan���ӽ���������;******************************************************************************************************/
/**********�����GuoDaocha,���ڼ�¼��Ѱ�ҽ�·�Ĺ������߹��ĵ���,�����������е�һ�����ݣ� nArrayHandleThisDaocha������һ����·ʱ�߹���Ӧ����*************************************/
/*****************nArrayHandleNextLine,�������еĺ�̹������;  GuoXHJ,��һ����·ʱ�߹����źŻ����źŻ����ڽ�·�ڲ�*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::FindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			CLongArray nArrayPassXHJ;
			bool btoward=bForword;
			this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
			CString  Name_PassXHJ;
			for(int k=0;k<nArrayPassXHJ.GetCount();k++)
			{
				CString name_xhj;
				GetEntName(nArrayPassXHJ[k],name_xhj);
				if(name_xhj==_T("X3"))
				{
					name_xhj=name_xhj;
				}
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//�г�����������𣬵�������Gudao������ֹ
		// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		// 		{
		// 			//�����ʼλ�ɵ��������
		// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
		// 			return;
		// 		}
		// 		else
		// 		{
		// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		else
		{
			this->GetGuiDaoChaOrderByXDesc(nHandleLineStart,  nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nHandleLineStart==nHandleLineEnd)
			{//�ҵ�������
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					if(name_xhj==_T("X3"))
					{
						name_xhj=name_xhj;
					}
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");
				}
				return;
			}
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//��ȡ�������źŻ�
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						if(name_xhj==_T("X3"))
						{
							name_xhj=name_xhj;
						}
						Name_PassXHJ=Name_PassXHJ+name_xhj+_T("��");
					}
					// 					CString str_nametemp;
					// 					m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),str_nametemp);
					// 					acutPrintf(_T("\n %s����Ķ��źŻ�Ϊ%s"),str_nametemp.GetString(),Name_PassXHJ.GetString());
					GuoXHJ.Add(Name_PassXHJ);
					//nArrayZhiXHJ.Add(Zhi_XHJ);
					//nArrayXiangXHJ.Add(Xiang_XHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							CString Daocha=_T("(")+DCname+_T(")");
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=_T("(")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//CString Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								//CString Daocha=_T("(")+DCname+_T(")");
							}
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			//�����ϴη��ʵĵ���λ��
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������1"));
									//��Ҫ�����Ƿ��д����������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									// 								//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									//	CString Daocha;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										//Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

									}
									else
									{
										// Daocha=_T("(")+DCname+_T(")");

									}
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{   //��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							// CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								//Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
									double pointx;
									CLongArray nPassXHJ;
									CString  Name_PassXHJ=_T("");
									GuoXHJ.Add(Name_PassXHJ);
								}
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n�����������2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
									}
									else
									{
										//Daocha=_T("(")+DCname+_T(")");
									}
									GuoDaoCha.Add(Daocha);
								}
							}
						}
						else
						{
							//��λ�߲�Ϊ����
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							long nHandleDaiDong_nMin=0;
							long nHandleDaiDong_nMax=0;
							CString DaiDong_MinName;
							CString DaiDong_MaxName;
							//CString Daocha;
							if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
							{
								// Daocha=_T("(")+DCname+_T(")");
								GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
								GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
								Daocha=Daocha+_T("��")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								//Daocha=_T("(")+DCname+_T(")");

							}
							GuoDaoCha.Add(Daocha);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{
						DaochaD=DaochaD+DCname1;
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T("��");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n�����������3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
							//acutPrintf(_T("\n�����������"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
						}

					}
				}
			}
			//���е���Ϊ��λʱ
			long nHandleThisDC=0;
			nArrayHandleThisDaocha.Add(nHandleThisDC);
			nArrayHandleLine.Add(nHandleNextLine);
			CLongArray nPassXHJ;
			CString  Name_PassXHJ=_T("");
			//GetBlockPosition_X(nHandleThisDC,pointx);
			//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
			//���е���Ϊ��λʱ���
			int b_toward=(bForword);
			GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
			for(int k=0;k<nPassXHJ.GetCount();k++)
			{
				CString name;
				GetEntName(nPassXHJ[k],name);
				if(name==_T("X3"))
				{
					name=name;
				}
				Name_PassXHJ=Name_PassXHJ+name+_T("��");
			}
			GuoXHJ.Add(Name_PassXHJ);
			GuoDaoCha.Add(DaochaD);
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//��¼���ʹ��ĵ��������߻�·��˳���¼�߹�����Ķ���λ������������,ֻ����Ѱ�Һ�̣����������ܣ�������С
void CZnSxt::FindNextLine( long nHandleLineStart,long nHandleLineEnd,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd||nHandleLineStart==0)
		{//�ҵ�������
			return;
		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		CADORecordset cSet(&m_AdoDb);
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					// 					CString Daocha=_T("");
					// 					GuoDaoCha.Add(Daocha);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{
			//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			for(int k=0;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						//	acutPrintf(_T("Isstart=%d"),IsStart);
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
						}
					}
				}
				else
				{

				}
				/*GuoDaoCha.Add(DaochaD);*/
			}
			if(nHandleNextLine!=0)
			{
				nArrayHandleLine.Add(nHandleNextLine);
			}
			else
			{
				
				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
					CString DCname;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					if(bForword==true&&IsStart>0)
					{
						//��λ������Ϊ���
						nArrayHandleLine.Add(nHandle_dw);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
					}
					else
					{
					}
				}
			}
			}
		}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
//��PassDaocha��¼ȥ��ÿ����̵ĵ���ţ�GuoDaocha������ϸ��¼�߹�����Ķ���λ
void CZnSxt::FindNextLine4( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &PassDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//�ҵ�������
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//�����ʼλ�ɵ��������
			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			this->GetGuiDaoInfoOrderByXAsc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		else
		{
			this->GetGuiDaoInfoOrderByXDesc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					PassDaoCha.Add(Daocha);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						CString DCname;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						if(bForword==true&&IsStart>0)
						{
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("��")+DCname+_T(")");
							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
							//GuoDaoCha.Add(Daocha);	
							CString DaoChaNum=DCname+_T("_");
							PassDaoCha.Add(DaoChaNum);
							GuoDaoCha.Add(Daocha);

						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("��")+DCname+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							CString DaoChaNum=DCname+_T("_");
							PassDaoCha.Add(DaoChaNum);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLineΪ0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			CString DaoChaDNum;
			for(int k=0;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				long nHanderDC_another=0;
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//GuoDaoCha.Add(Daocha);
									CString DaoChaNum=DaoChaDNum+DCname_nMin+_T("_")+DCname_nMax+_T("_")+DC_Another_Min+_T("_")+DC_Another_Max+_T("_");
									PassDaoCha.Add(DaoChaNum);
									GuoDaoCha.Add(Daocha);						
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									//GuoDaoCha.Add(Daocha);
									CString DaoChaNum=DaoChaDNum+DCname_nMin+_T("_")+DCname_nMax+_T("_");
									PassDaoCha.Add(DaoChaNum);
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//����λʱ����Ҫ�����Ƿ���ڴ�������
							//GuoDaoCha.Add(Daocha);
							CString DaoChaNum=DaoChaDNum+DCname1+_T("_");
							PassDaoCha.Add(DaoChaNum);	
							GuoDaoCha.Add(Daocha);			
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//��ӡ���ߵ���
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//GuoDaoCha.Add(Daocha);
									CString DaochaNum=DaoChaDNum+DCname_nMin+_T("_")+DCname_nMax+_T("_")+DC_Another_Min+_T("_")+DC_Another_Max+_T("_");
									PassDaoCha.Add(DaochaNum);
									GuoDaoCha.Add(Daocha);
								}
								else
								{
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									//����λʱ����Ҫ�����Ƿ���ڴ�������
									//GuoDaoCha.Add(Daocha);
									CString DaoChaNum=DaoChaDNum+DCname_nMin+_T("_")+DCname_nMax+_T("_");
									PassDaoCha.Add(DaoChaNum);
									GuoDaoCha.Add(Daocha);			
								}
							}
						}
						else
						{
							nArrayHandleLine.Add(nHandle_fw);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							CString DaoChaNum=DaoChaDNum+DCname1+_T("_");
							//GuoDaoCha.Add(Daocha);
							PassDaoCha.Add(DaoChaNum);
							GuoDaoCha.Add(Daocha);				
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
				if(b_dx==false)
				{
					if(k==iNumDC-1)
					{	
						DaochaD=DaochaD+DCname1;
						DaoChaDNum=DaoChaDNum+DCname1+_T("_");	
					}
					else
					{
						DaochaD=DaochaD+DCname1+_T("��");
						DaoChaDNum=DaoChaDNum+DCname1+_T("_");
					}
				}
				else
				{
					CString DCname_Min;
					CString DCname_Max;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
					long nHandle_Anothe_Min=0;
					long nHandle_Anothe_Max=0;
					if(k==iNumDC-1)
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;	
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;	
							DaoChaDNum=DaoChaDNum+DCname_Min+_T("_")+DCname_Max+_T("_")+DC_Another_Min+_T("_")+DC_Another_Max+_T("_");
						}
						else
						{

							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max;
							DaoChaDNum=DaoChaDNum+DCname_Min+_T("_")+DCname_Max+_T("_");

						}
					}
					else
					{
						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
						{
							CString DC_Another_Min;
							CString DC_Another_Max;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;		
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;	
							DaoChaDNum=DaoChaDNum+DCname_Min+_T("_")+DCname_Max+_T("_")+DC_Another_Min+_T("_")+DC_Another_Max+_T("_");

							//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
							DaoChaDNum=DaoChaDNum+DCname_Min+_T("_")+DCname_Max+_T("_");
						}

					}
				}
			}
			nArrayHandleLine.Add(nHandleNextLine);
			GuoDaoCha.Add(DaochaD);
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
// ��¼���ʹ��ĵ��������߻�·��˳���¼�߹�����Ķ���λ������������,��������������
// void CZnSxt::FindNextLine5( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
// {
// 	try
// 	{
// 		// static int stop =0;
// 		if(nHandleLineStart==nHandleLineEnd)
// 		{//�ҵ�������
// 			return;
// 		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//�����ʼλ�ɵ��������
// 			//acutPrintf(_T("�ɵ�����=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("�������=%s"),GuidaoName.GetString());
// 		}
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		//��ʾΪ��ͷ��
// 		if (nLineType==2) 
// 		{
// 			CString name;
// 			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 			acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
// 			return ;
// 		}
// 		//�жϹ����DC����
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//��ȡ�����Ϣ�����������Ҫ��ϸ����
// 		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 		if(bForword==true)
// 		{
// 			this->GetGuiDaoInfoOrderByXAsc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
// 		}
// 		else
// 		{
// 			this->GetGuiDaoInfoOrderByXDesc(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
// 		}
// 		//for test print DC
// 		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
// 		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//��ȡ�����
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//�������������ʱ�����ֻ��һ��
// 			if(nLineType==0)
// 			{//��Ϊ����ʱ
// 				if(nHandleNextLine!=0)
// 				{
// 					nArrayHandleLine.Add(nHandleNextLine);
// 					CString Daocha=_T("");
// 					GuoDaoCha.Add(Daocha);
// 				}
// 				else
// 				{
// 					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//��λ�߶�Ӧ�ĵ���
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					if(nHandleDC!=0)
// 					{
// 						//��ѯ�������ڵ��ߣ�����Ϊ���
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//��λ�߶�Ӧ�ĵ���
// 						long nHandle_dw=0;
// 						// long nHandle_fw=0;						
// 						int IsStart=-1;
// 						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 						CString DCname;
// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
// 						if(bForword==true&&IsStart>0)
// 						{
// 							//��λ������Ϊ���
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("��")+DCname+_T(")");
// 							// 						   //����λʱ����Ҫ�����Ƿ���ڴ�������
// 							GuoDaoCha.Add(Daocha);
// 						}
// 						else if(bForword==false&&IsStart==0)
// 						{
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("��")+DCname+_T(")");
// 							//����λʱ����Ҫ�����Ƿ���ڴ�������
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLineΪ0"));
// 					}
// 				}
// 			}
// 			// 			else if(nLineType==1)
// 			// 			{
// 			// 				//Ϊ����ʱ
// 			// 				acutPrintf(_T("\n����Ϊ����"));
// 			// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 			// 				CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			// 				//��ȡ�����
// 			// 				//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 			// 				//���߾�Ե�����Ҳ�ʱ
// 			// 				if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
// 			// 				{
// 			// 					//�Ҷ��߾�Ե�������Ķ���
// 			// 					if(bForword==true)
// 			// 					{
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"), nHandleNextLine);
// 			// 						if(nHandleNextLine!=0)
// 			// 						{
// 			// 							nArrayHandleLine.Add(nHandleNextLine);
// 			// 						}
// 			// 
// 			// 						else
// 			// 						{
// 			// 							acutPrintf(_T("\n nHandNextLineΪ0"));
// 			// 						}
// 			// 					}
// 			// 					else 
// 			// 					{
// 			// 						//���Ϊ����λ���ڹ��
// 			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			// 						//��λ�߶�Ӧ�ĵ���
// 			// 						long nHandleDC=0;
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 			// 						//�������ڹ����Ϊ��̹��
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 			// 						if(nHandleNextLine!=0)
// 			// 						{
// 			// 							nArrayHandleLine.Add(nHandleNextLine);
// 			// 						}
// 			// 						else
// 			// 						{
// 			// 							acutPrintf(_T("\n nHandNextLineΪ0"));
// 			// 						}
// 			// 					}
// 			// 				}
// 			// 				//���߾�Ե���������ʱ
// 			// 				else if(nHandleLeftJyj!=0&&nHandleRightJyj==0)
// 			// 				{
// 			// 					if(bForword==false)
// 			// 					{
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"), nHandleNextLine);
// 			// 						if(nHandleNextLine!=0)
// 			// 						{
// 			// 							nArrayHandleLine.Add(nHandleNextLine);
// 			// 						}
// 			// 					}
// 			// 					else
// 			// 					{
// 			// 						////���Ϊ����λ���ڹ��
// 			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			// 						//��λ�߶�Ӧ�ĵ���
// 			// 						long nHandleDC=0;
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 			// 						//�������ڹ����Ϊ��̹��
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 			// 						if(nHandleNextLine!=0)
// 			// 						{
// 			// 							nArrayHandleLine.Add(nHandleNextLine);
// 			// 						}
// 			// 						else
// 			// 						{
// 			// 							acutPrintf(_T("\n nHandNextLineΪ0"));
// 			// 						}
// 			// 					}
// 			// 
// 			// 				}
// 			// 			}
// 			else
// 			{
// 			}
// 			//return;
// 		}//endif NumDC
// 		else
// 		{//���е���ʱ�����ܴ��ڶ������
// 			//����Ϊ��λʱ���
// 			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
// 			CString DaochaD;
// 			for(int k=0;k<iNumDC;k++)
// 			{//����λʱ���,��Ҫǰ����ʵĵ���
// 				//ȡ����handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//��ѯ��λ������
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				long nHandle_fw=0;
// 				long nHandle_dw=0;
// 				int IsStart=-1;
// 				CString DCname1;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
// 				long nHanderDC_another=0;
// 				//�����ƽ��к��ʱ
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//�����ƽ�ʱ���
// 					if(nHandle_fw!=0)
// 					{
// 						//����������һ�κ�̣��ж��Ƿ�Ϊ����
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//Ϊ����ʱ����λ�ߵĺ��
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��ȡ�����
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//���Ҹ÷�λ�����ڵ���Ķ�λ��
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��λ�߶�Ӧ�ĵ���
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//��ѯ�������ڵ��ߣ�����Ϊ���
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//��λ�߶�Ӧ�ĵ���
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2��Ϊ���ߵĺ��
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//��ӡ���ߵ���
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
// 								long nHandle_Anothe_Min=0;
// 								long nHandle_Anothe_Max=0;
// 								//�ж϶��ߵ����Ƿ����ڽ�����ߵĵ���
// 								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 								{
// 									CString DC_Another_Min;
// 									CString DC_Another_Max;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//����λʱ����Ҫ�����Ƿ���ڴ�������
// 									GuoDaoCha.Add(Daocha);
// 								}						
// 							}
// 						}
// 						else
// 						{//��λ�߲�Ϊ����ʱ
// 							nArrayHandleLine.Add(nHandle_fw);
// 							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
// 							//����λʱ����Ҫ�����Ƿ���ڴ�������
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//�����ƽ�ʱ���
// 					if(nHandle_fw!=0)
// 					{
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//Ϊ����ʱ����λ�ߵĺ��
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��ȡ�����
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//���Ҹ÷�λ�����ڵ���Ķ�λ��
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//��λ�߶�Ӧ�ĵ���
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//��ѯ�������ڵ��ߣ�����Ϊ���
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//��λ�߶�Ӧ�ĵ���
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2��Ϊ���ߵĺ��
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//��ӡ���ߵ���
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n���ߵ���λ��(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
// 								long nHandle_Anothe_Min=0;
// 								long nHandle_Anothe_Max=0;
// 								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 								{
// 									CString DC_Another_Min;
// 									CString DC_Another_Max;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("��")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//����λʱ����Ҫ�����Ƿ���ڴ�������
// 									GuoDaoCha.Add(Daocha);
// 								}
// 							}
// 						}
// 						else
// 						{
// 							nArrayHandleLine.Add(nHandle_fw);
// 							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
// 							// 						long nHandle_DdMin=0;
// 							// 						long nHandle_DdMax=0;
// 							// 						long nHandle_DdAno=0;
// 							// 						if(this->GetDaidongDaochas(nHandleDC,nHandle_DdMin,nHandle_DdMax)==TRUE)
// 							// 						{
// 							// 							CString DDname1; 
// 							// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DdMin) ;
// 							// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DDname1) ;
// 							// 							CString DDname2; 
// 							// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DdMin) ;
// 							// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DDname2) ;
// 							// 							//Daocha=Daocha+_T("��")+_T("{")+DDname1+_T("/")+DDname2+_T("}");
// 							// 							 acutPrintf(_T("\n{%s/%s}"),DDname1.GetString(),DDname2.GetString());
// 							// 						}
// 							// 						else if(this->GetAnotherDuXianDaoCha(nHandleDC,nHandle_DdAno)==TRUE)
// 							// 						{
// 							// 							if(this->GetDaidongDaochas(nHandle_DdAno,nHandle_DdMin,nHandle_DdMax)==TRUE)
// 							// 							{
// 							// 								CString DDname1; 
// 							// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DdMin) ;
// 							// 								m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DDname1) ;
// 							// 								CString DDname2; 
// 							// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DdMin) ;
// 							// 								m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DDname2) ;
// 							// 								//Daocha=Daocha+_T("��")+_T("{")+DDname1+_T("/")+DDname2+_T("}");
// 							// 								 acutPrintf(_T("\n{%s/%s}"),DDname1.GetString(),DDname2.GetString());
// 							// 							}			   
// 							// 						}
// 							// 						else
// 							// 						{
// 							// 
// 							// 						}
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 					//	acutPrintf(_T("Isstart=%d"),IsStart);
// 				}
// 				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				BOOL b_dx=GetAnotherDuXianDaoCha(nHandleDC,nHanderDC_another);
// 				long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHanderDC_another);
// 				long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHanderDC_another);		
// 				if(b_dx==false)
// 				{
// 					if(k==iNumDC-1)
// 					{
// 						DaochaD=DaochaD+DCname1;
// 					}
// 					else
// 					{
// 						DaochaD=DaochaD+DCname1+_T("��");
// 					}
// 				}
// 				else
// 				{
// 					CString DCname_Min;
// 					CString DCname_Max;
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Min) ;
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname_Max) ;
// 					//acutPrintf(_T("\n%s��һ������Ϊ%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
// 					long nHandle_Anothe_Min=0;
// 					long nHandle_Anothe_Max=0;
// 					if(k==iNumDC-1)
// 					{
// 						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 						{
// 							CString DC_Another_Min;
// 							CString DC_Another_Max;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max;
// 
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
// 						}
// 					}
// 					else
// 					{
// 						if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 						{
// 							CString DC_Another_Min;
// 							CString DC_Another_Max;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 							m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��")+DC_Another_Min+_T("/")+DC_Another_Max+_T("��");
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("��");
// 						}
// 
// 					}
// 				}
// 			}
// 			nArrayHandleLine.Add(nHandleNextLine);
// 			GuoDaoCha.Add(DaochaD);
// 		}
// 	}
// 	catch(_com_error& e)
// 	{
// 		CString sBuff = CBlkUtility::GetErrorDescription(e);
// 		AfxMessageBox(sBuff);        
// 	}
// 	catch(...)
// 	{	
// 		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
// 	}	
// }
// void CZnSxt::FindNextLine( long nHandleLine,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
// {
// 	try
// 	{
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLine) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		if (nLineType==2) 
// 		{
// 			return ;
// 		}
// 		//�жϹ����DC����
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//��ȡ�����Ϣ�����������Ҫ��ϸ����
// 		this->GetGuiDaoInfo(nHandleLine, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
// 		int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
// 		acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,iNumLine);
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//��ȡ�����
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//�ɵ�����������ʱ�����ֻ��һ��
// 			nArrayHandleLine.Add(nHandleNextLine);
// 		    //return;
// 		}
// 		else
// 		{
// 			nArrayHandleLine.Add(nHandleNextLine);
// 			//�жϵ������ͣ����ߣ�������ߣ���������
// 			for(int k=0;k<iNumDC;k++)
// 			{
// 				//ȡ����handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//��ѯ��λ������
// 				//��ѯ��λ������
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				long nHandle_fw;
// 				long nHandle_dw;
// 				int IsStart=-1;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//�����ƽ�ʱ���
// 					nArrayHandleLine.Add(nHandle_fw);
// 					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLine) ;
// 					long nLineType = 0 ;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 					acutPrintf(_T("\n nlineType=%d"),nLineType);
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//�����ƽ�ʱ���
// 					nArrayHandleLine.Add(nHandle_fw);
// 					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLine) ;
// 					long nLineType = 0 ;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 					acutPrintf(_T("\n nlineType=%d"),nLineType);
// 				}
// 				acutPrintf(_T("Isstart=%d"),IsStart);
// 			}
// 
// 
// 		}
// 	}
// 	catch(_com_error& e)
// 	{
// 		CString sBuff = CBlkUtility::GetErrorDescription(e);
// 		AfxMessageBox(sBuff);        
// 	}
// 	catch(...)
// 	{	
// 		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
// 	}	
// }

int CZnSxt::GetAll_LJ_XHJ( CLongArray& nArrayHandleDc )
{
	long t1=GetTickCount();
	int iRet = 0 ;
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	AcDbEntity * pEnt = NULL ;
	CString strSql ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//��������ʵ�� ;	
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;

				CString strBlkName ;
				m_mysys.GetBlockName(pRef, strBlkName) ;				
				if (strBlkName.Left(6)==_T("XHJ_LJ"))
				{
					UINT nHandle = m_mysys.GetHandle(pEnt) ;
					nArrayHandleDc.Add(nHandle) ;
					iRet++ ;
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	long t2=GetTickCount();
	CString str;
	str.Format("GetAll_LJ_XHJ:%d ms",t2-t1);
	acutPrintf(_T("\n%s"),str);
	return iRet ;

}
int CZnSxt::GetAll_PMT_XBD( CLongArray& nArrayHandleXBD )
{
	long t1=GetTickCount();
	int iRet = 0 ;
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	AcDbEntity * pEnt = NULL ;
	CString strSql ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//��������ʵ�� ;	
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;

				CString strBlkName ;
				m_mysys.GetBlockName(pRef, strBlkName) ;				
				if (strBlkName.Left(8)==_T("PMT_XBD_"))
				{
					UINT nHandle = m_mysys.GetHandle(pEnt) ;
					nArrayHandleXBD.Add(nHandle) ;
					iRet++ ;
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	long t2=GetTickCount();
	CString str;
	str.Format("GetAll_LJ_XHJ:%d ms",t2-t1);
	acutPrintf(_T("\n%s"),str);
	return iRet ;

}
BOOL CZnSxt::GetBlockToward( long nHandleXHJ ,int &iToward)
{
	if(nHandleXHJ==0)
	{
		return FALSE;
	}
	long t1=GetTickCount();
	BOOL bRet = FALSE ;
	AcDbEntity* pEnt = NULL ;
	iToward =-1;
	if (m_mysys.OpenAcDbEntity(nHandleXHJ, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			//bRet= GetBlockName(pRef, strBlockName) ;
			double rota=pRef->rotation();
			//acutPrintf(_T("\n %6f"),rota);
			int cnt=0;
			while((rota-PI)>1.0E-6)
			{
				rota-=PI;
				cnt++;
			}
			while((rota+PI)<1.0E-6)
			{
				rota+=PI;
				cnt++;
			}
 			float res=rota-PI/2;
			//acutPrintf("rota=%6f",rota);
			if(fabs(res)<=1.0E-6&&cnt%2==0)
			{
				//����
				iToward=0;
			}
			else if(fabs(res)<=1.0E-6&&cnt%2==1)
			{
				//���£�
				iToward=3;
			}
			else if(res<1.0E-6&&cnt%2==0)
			{
				//����
				iToward=1;
			}
			else if(res<1.0E-6&&cnt%2==1)
			{
				iToward=2;
			}
			else if(res>1.0E-6&&cnt%2==0)
			{
				//����
				iToward=2;
			}
			else if(res>1.0E-6&&cnt%2==1)
			{
				iToward=1;
			}
			else
			{
				iToward=-1;

			}
		   
		}
		pEnt->close() ;
		bRet =TRUE;
	}
	long t2=GetTickCount();
	CString str;
	str.Format("GetBlockToward:%d ms",t2-t1);
	//acutPrintf(_T("\n%s"),str);
	return bRet ;


}
BOOL CZnSxt::GetBlockPosition( long nHandleBlk ,AcGePoint3d& Ptpost)
{
	if(nHandleBlk==0)
	{
		return FALSE;
	}
	BOOL bRet = FALSE ;
	AcDbEntity* pEnt = NULL ;
//	iToward =-1;
	if (m_mysys.OpenAcDbEntity(nHandleBlk, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			//bRet= GetBlockName(pRef, strBlockName) ;
			Ptpost=pRef->position();
		}
		pEnt->close() ;
		bRet =TRUE;
	}
	//acutPrintf(_T("\n%s"),str);
	return bRet ;


}
int CZnSxt::GetSomeBlocks( CString name_left2,CLongArray& nArrayHandleDc )
{
	int iRet = 0 ;
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	AcDbEntity * pEnt = NULL ;
	CString strSql ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//��������ʵ�� ;	
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;

				CString strBlkName ;
				m_mysys.GetBlockName(pRef, strBlkName) ;	
				//pRef->rotation();
				if (strBlkName.Left(6)==name_left2.Left(6))
				{
					UINT nHandle = m_mysys.GetHandle(pEnt) ;
					nArrayHandleDc.Add(nHandle) ;
					iRet++ ;
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	return iRet ;


}
BOOL CZnSxt::SelectFilePath(CString &strFilePath)
{
	try
	{
		CString strFilter = _T("(�ļ�(*.xls)|*.xls|");
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, strFilter);
		fileDlg.m_ofn.lpstrTitle = _T("�����������");
		TCHAR szBuf[256];
		memset(szBuf, 0, sizeof(szBuf));
		GetCurrentDirectory(sizeof(szBuf), szBuf);
		fileDlg.m_ofn.lpstrInitialDir = szBuf;
		CString strFileSave;
		if(IDOK==fileDlg.DoModal())
		{
			strFileSave = fileDlg.GetPathName();
			CString strFilePathTmp = strFileSave;
			strFilePathTmp.MakeLower() ;
			if (strFilePathTmp.Right(4)!=_T(".xls"))
			{
				strFileSave.Append(_T(".xls")) ;
			}
			strFilePath=strFileSave;
			//fileDlg.SendMessage(WM_CLOSE);
			return TRUE;
		}	
		strFilePath=strFileSave;
		//return FALSE;
	}
 	catch(_com_error& e)
 	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
 		AfxMessageBox(sBuff);    
		return FALSE;
 	}
 	catch(...)
 	{	
	 	AfxMessageBox(_T("δ֪�Ĵ���!in SelectFilePath"));
		return FALSE;
 	}		 	
	return FALSE;
}
void CZnSxt::LsbTest2()
{
	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
	CLongArray nArrayHandle_LJ_XHJ;
	//��ȡ���нӳ��źŻ�
	int iRet =GetAll_LJ_XHJ(nArrayHandle_LJ_XHJ);
	CLongArray nArrayHandle_LF_XHJ;
	//��ȡ���г�վ�źŻ�
	CString name_xhj=_T("XHJ_LF");
	int iRet2 =GetSomeBlocks(name_xhj,nArrayHandle_LF_XHJ);
	long nHandleLinetemp=0;
	long nHandleLinetemp2=0;
	acutPrintf(_T("\n ��վ�źŻ�����=%d"),iRet2);
	CString strFilePath ;
	strFilePath.Format(_T("%s\\support\\xls\\ƽ��ͼ������ģ��.xls"), g_strBasePath);
	oper_excel.OpenExcel(strFilePath);
	CString strFileSave;
	if(FALSE==this->SelectFilePath(strFileSave))
	{
		AfxMessageBox(_T("ָ�����ļ�·����Ч��"));
		return ;
	}
	//���excel��
	oper_excel.ClearAllCells(1);
	CTime  time_beg=CTime::GetCurrentTime();
	CString str_Time=time_beg.Format("%Y-%m-%d %H:%M:%S");
	this->iJLnum=1;
	//��ȡ�ɵ����ڹ��
	//CLongArray nAarryHandleGuDaoS;
	//CLongArray nAarryHandleGuDaoX;
	//���н�վ����
// 	CLongArray nAarryHandleXJC;
// 	//���нӽ���վ����
// 	CLongArray nAarryHandleXJJC;
// 	CLongArray nAarryHandleSJC;
// 	CLongArray nAarryHandleSJJC;
	CStringArray sXJCGuiDaoName;
	CStringArray sSJCGuiDaoName;
	//�ɵ�����
	//CStringArray str_GuDaoName;
	//���йɵ�����
	//CStringArray sSGuDaoName;
	//���йɵ�����
	//CStringArray sXGuDaoName;
	//���޾�Ե
	FindALLChaoXianGuiDao(this->nArrayHandLineCX,this->strArrayGuiDaoCX);
	int length =nArrayHandle_LJ_XHJ.GetCount();
	if(length<1)
	{
		//acutPrintf(_T("\n �źŻ�����Ϊ%d"),length);
		//return;
	}
	//��ӡ��վ�źŻ�����
	for(int i=0;i<length;i++)
	{
		CString entname;
		long nHandleLine =nArrayHandle_LJ_XHJ[i];
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
		acutPrintf(_T("\n ��վ�źŻ�����Ϊ%s"),entname.GetString());
		//��ѯ��վ�źŻ��Ҳ�ֱ��
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
		//Ѱ�Һ�һ�����
		CLongArray nArrayHandle_GD_Next;
		//�����нӽ�����Ϊ���ⷽ��վ���
		//��ȡ���н�վ�źŻ������
		if(entname.Left(1)==_T("X"))
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
			CString name;
			m_mysys.GetXdata(nHandleLinetemp,_T("GUIDAONAME"),name);
			sXJCGuiDaoName.Add(name);
			acutPrintf(_T("X��վ���Ϊ��%s"),name.GetString());
			nAarryHandleXJC.Add(nHandleLinetemp);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
			nAarryHandleXJJC.Add(nHandleLinetemp);
		}
		else
		{
			//��ȡ���н�վ�źŻ��Ҳ���
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
			CString name;
			m_mysys.GetXdata(nHandleLinetemp,_T("GUIDAONAME"),name);
			sSJCGuiDaoName.Add(name);
			acutPrintf(_T("S��վ���Ϊ��%s"),name.GetString());
			nAarryHandleSJC.Add(nHandleLinetemp);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
			nAarryHandleSJJC.Add(nHandleLinetemp);
		}
	}
	//��ӡ��վ�źŻ�����
	//CLongArray nArrayHandleLineGuDao;
	for(int i=0;i<iRet2;i++)
	{
		CString entname;
		long nHandleLine =nArrayHandle_LF_XHJ[i];
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
		acutPrintf(_T("\n ��վ�źŻ�����Ϊ%s"),entname.GetString());
		//��ѯ��վ�źŻ��Ҳ�ֱ��
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
		//��ȡ�źŻ��Ҳ���,��Ϊ�ɵ�
		if(entname.Left(1)==_T("S"))
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp2) ;
			nAarryHandleGuDaoS.Add(nHandleLinetemp2);
			//��ӡ�ɵ�����
			if(nHandleLinetemp2!=0)
			{
				CString na;
				//��ȡ�ɵ����ƣ�Ӧ�����źŻ�����������
				CString gu;
				gu=entname;
				gu.Remove(_T('S'));
				gu.Remove(_T('X'));
				gu.Append(_T("G"));
				this->str_GuDaoName.Add(gu);
 				m_mysys.GetXdata(nHandleLinetemp2,_T("GUIDAONAME"),na);
				sSGuDaoName.Add(na);
				long zhixhj=0;
				long xiangxhj=0;
				CString str_ent_name;
				CLongArray nArrayHandleXHJ;
				GetGuiDaoDuanXHJ(nHandleLinetemp2,false,nArrayHandleXHJ);
				for(int i=0;i<nArrayHandleXHJ.GetCount();i++)
				{
					zhixhj=nArrayHandleXHJ.GetAt(i);
					GetEntBlkNameByHandle(zhixhj,str_ent_name);
					if(str_ent_name.Left(6)==_T("XHJ_LF"))
					{
						break;
					}
					else
						zhixhj=0;
				}
				while(zhixhj==0)
				{
					long nHandleLineNext=0;
					FindNextLineByJYJ(nHandleLinetemp2,nHandleLineNext,true);
					m_mysys.GetXdata(nHandleLineNext,_T("GUIDAONAME"),na);
					nHandleLinetemp2=nHandleLineNext;
					CLongArray nArrayHandleXHJTemp;
					if(nHandleLineNext!=0)
					{
						GetGuiDaoDuanXHJ(nHandleLineNext,false,nArrayHandleXHJTemp);
					}
				    else
					{
						break;
					}
					for(int i=0;i<nArrayHandleXHJTemp.GetCount();i++)
					{
						zhixhj=nArrayHandleXHJTemp.GetAt(i);
						GetEntBlkNameByHandle(zhixhj,str_ent_name);
						if(str_ent_name.Left(6)==_T("XHJ_LF"))
						{
							break;
						}
						else
						{
							zhixhj=0;
						}

					}
					//break;
				}
				if(nHandleLinetemp2!=0)
				{
					nAarryHandleGuDaoX.Add(nHandleLinetemp2);
				  m_mysys.GetXdata(nHandleLinetemp2,_T("GUIDAONAME"),na);
				  //acutPrintf(_T("gudao:%s"),na.GetString());
				  sXGuDaoName.Add(na);
				}
			}
		}
	}
	CBlkUtility::SortStringArray_CZXHJ(this->str_GuDaoName,_T('G'));
	CBlkUtility::SortStringArray_CZXHJHandle(sXGuDaoName,_T('G'),nAarryHandleGuDaoX);
	for(int i=0;i<sXGuDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n ���йɵ�������%s\t"),sXGuDaoName.GetAt(i).GetString());
		acutPrintf(_T("\n�ɵ�Ϊ��%s\t"),this->str_GuDaoName.GetAt(i).GetString());
	}
	CBlkUtility::SortStringArray_CZXHJHandle(sSGuDaoName,_T('G'),nAarryHandleGuDaoS);
	for(int i=0;i<sSGuDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n ���йɵ�������%s\t"),sSGuDaoName.GetAt(i).GetString());
	}
	for(int i=0;i<sXJCGuiDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n ���нӽ����ΰ�����%s\t"),sXJCGuiDaoName.GetAt(i).GetString());
	}
	for(int i=0;i<sSJCGuiDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n ���нӽ����ΰ�����%s\t"),sSJCGuiDaoName.GetAt(i).GetString());
	}
	//return;
	// 		����������нӳ���·
		acutPrintf(_T("\n��ʼ������нӳ���·\n"));
		for(int i=0;i<nAarryHandleXJC.GetCount();i++)
		{
			long nHandleStart=nAarryHandleXJC[i];
			CString Start_Gd_Name;
			//m_mysys.GetXdata(nHandleStart,Start_Gd_Name);
			m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
			CString str_fx;
		    int i_excel_row_current=this->num;
			//��ѯͨ����·
			nHandleStart=nAarryHandleXJC[i];
			for(int m=0;m<nAarryHandleSJJC.GetCount();m++)
			{
				long nHandleTjEnd=nAarryHandleSJJC[m];
				long nxhj_start=0;
				long nxhj_end=0;
				long nlastdaocha=0;
				vector<CString>vec_xhj;
				vector<CString>vec_path;
				vector<CString>vec_daocha;
				vector<CString>vec_cxgd;
				CLongArray nArrayHandleLineTJ;
				CString str_GdStartname;
				CString str_xhj_start;
				CString str_xhj_end;
				int i_CntTJ=0;
				CLongArray nArrayHandleStartXHJ;
				CLongArray nArrayHandleEndXHJ;
				this->GetGuiDaoDuanXHJ(nHandleStart,true,nArrayHandleStartXHJ);
				if(nArrayHandleStartXHJ.GetCount()>0)
					nxhj_start=nArrayHandleStartXHJ[0];
				this->GetGuiDaoDuanXHJ(nHandleTjEnd,true,nArrayHandleEndXHJ);
				if(nArrayHandleEndXHJ.GetCount()>0)
					nxhj_end=nArrayHandleEndXHJ[0];
				//��ȡ�ն��źŻ����ж��Ƿ��дͨ����·
				GetEntName(nxhj_end,str_fx);
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),str_GdStartname);
				if(str_fx.Right(1)==_T("N"))
				{
					
				
					GetTongGuoPathGuiDaoAndXHJ(str_GdStartname,nxhj_start,nxhj_end,nHandleStart,nHandleStart,nHandleTjEnd,nlastdaocha,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleLineTJ,vec_cxgd,i_CntTJ,true);
					this->num=this->num-i_CntTJ*2;
					for(int j=0;j<i_CntTJ;j++)
					{
						//ͨ����·����Ϊ����ͨ��
						for(int i_col=3;i_col<=17;i_col++)
						{
							//oper_excel.SetColumnCombandCells(1,this->num,i_col,2);
							if(i_col==3)
							{
								oper_excel.SetColumnCombandCellsAndCenterShow(1,this->num,i_col,2);
								oper_excel.WriteDatatoExcel(1,this->num,3,_T("ͨ��"));
							}
							else
								oper_excel.SetColumnCombandCells(1,this->num,i_col,2);
						}
						oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
						if(j!=0)
						{
							for(int k=5;k<18;k++)
							{
								oper_excel.SetCellFontColor(1,this->num,k,3);
							}

						}
						this->num=this->num+2;
					}
				}		
			}
			int i_excel_row_current1=this->num;
			for(int k=0;k<nAarryHandleGuDaoX.GetCount();k++)
			{
					long nHandleEnd=nAarryHandleGuDaoX[k];
					vector<CString> path;
					vector<CString>daocha;
					vector<CString>xhj;
					vector<CString>vec_cx_gd;
					CLongArray  nArrayHandle;
					CStringArray DaochaNum;
					long LastDaocha=0;
					long nHandleStartXHJ=0;
					long nHandleEndXHJ=0;
					CLongArray nArrayJinZhanXHJ;
					CLongArray nArrayChuZhanXHJ;
					bool btoward=true;
					//��ȡ��ֹ����źŻ�
					long nHandleLineXHJEnd=nAarryHandleGuDaoS[k];
					this->GetGuiDaoDuanXHJ(nHandleStart,btoward,nArrayJinZhanXHJ);
					if(nArrayJinZhanXHJ.GetCount()>0)
					nHandleStartXHJ=nArrayJinZhanXHJ[0];
					//��ȡ��ʼ�źŻ����ƣ���Ϊ���з���ķ���
					GetEntName(nHandleStartXHJ,str_fx);
					this->GetGuiDaoDuanXHJ(nHandleLineXHJEnd,btoward,nArrayChuZhanXHJ);
					if(nArrayChuZhanXHJ.GetCount()>0)
					nHandleEndXHJ =nArrayChuZhanXHJ[0];
					int iJL_Cnt_temp=0;
					GetJieChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sXGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,btoward);
					//дȷ�����з������
					if(iJL_Cnt_temp>=2)
					{
						int row_num=this->num-iJL_Cnt_temp;
						for(int i=1;i<iJL_Cnt_temp;i++)
						{
							CString strDaocha1;
							CString strDaocha2;
							CString str_diff1;
							CString str_diff2;
							vector<CString>vec_str_Daocha1;
							vector<CString>vec_str_Daocha2;
							oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
							TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
							oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
							TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
							FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
							if(i==1)
							{
								oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							}
							else
							{
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

							}
							//oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							//oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							row_num++;
						}
					}
					this->num=this->num-iJL_Cnt_temp;
					for(int j=0;j<iJL_Cnt_temp;j++)
					{
						oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
						if(j!=0)
						{
							for(int k=5;k<18;k++)
							{
								oper_excel.SetCellFontColor(1,this->num,k,3);
							}

						}
						this->num++;
					}
			}
			int i_excel_row_current2=this->num;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current1,3,i_excel_row_current2-i_excel_row_current1);
			oper_excel.WriteDatatoExcel(1,i_excel_row_current1,3,_T("�ӳ�"));
			//������·
			long nHandleEnd=nAarryHandleXJJC[i];
			for(int k=0;k<nAarryHandleGuDaoX.GetCount();k++)
			{
				long nHandleStart=nAarryHandleGuDaoX[k];
				CString Start_Gd_Name;
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
				vector<CString> path;
				vector<CString>daocha;
				vector<CString>xhj;
				vector<CString>vec_cx_gd;
				CLongArray  nArrayHandle;
				CStringArray DaochaNum;
				long LastDaocha=0;
				long nHandleStartXHJ=0;
				long nHandleEndXHJ=0;
				CLongArray nArrayStartXHJ;
				CLongArray nArrayEndXHJ;
				bool btoward=false;
				//��ȡ��ֹ����źŻ�
				long nHandleLineXHJStart=nAarryHandleGuDaoS[k];
				this->GetGuiDaoDuanXHJ(nHandleLineXHJStart,true,nArrayStartXHJ);
				if(nArrayStartXHJ.GetCount()>0)
					nHandleStartXHJ=nArrayStartXHJ[0];
				this->GetGuiDaoDuanXHJ(nHandleEnd,false,nArrayEndXHJ);
				if(nArrayEndXHJ.GetCount()>0)
					nHandleEndXHJ =nArrayEndXHJ[0];
				int iJL_Cnt_temp=0;
				GetFaChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sXGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,false);
				if(iJL_Cnt_temp>=2)
				{
					int row_num=this->num-iJL_Cnt_temp;
					for(int i=1;i<iJL_Cnt_temp;i++)
					{
						CString strDaocha1;
						CString strDaocha2;
						CString str_diff1;
						CString str_diff2;
						vector<CString>vec_str_Daocha1;
						vector<CString>vec_str_Daocha2;
						oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
						TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
						oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
						TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
						FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
						if(i==1)
						{
							oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
						}
						else
						{
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

						}
						row_num++;
					}
				}
				this->num=this->num-iJL_Cnt_temp;
				for(int j=0;j<iJL_Cnt_temp;j++)
				{
					oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
					if(j!=0)
					{
						for(int k=5;k<18;k++)
						{
							oper_excel.SetCellFontColor(1,this->num,k,3);
						}

					}
					this->num++;
				}
			}
			int i_excel_row_current3=this->num;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current2,3,i_excel_row_current3-i_excel_row_current2);
			oper_excel.WriteDatatoExcel(1,i_excel_row_current2,3,_T("����"));		
			int i_excel_row_current4=this->num;
			//д�г�����
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current,2,i_excel_row_current4-i_excel_row_current);
			//CString str_T=_T("��������");
			//str_T+=_T(" ");
			str_fx=_T("�������� ")+str_fx;
			oper_excel.WriteDatatoExcel(1,i_excel_row_current,2,str_fx);
		}
		//����������нӳ���·
		acutPrintf(_T("\n��ʼ������нӳ���·\n"));
		for(int i=0;i<nAarryHandleSJC.GetCount();i++)
		{
			long nHandleStart=nAarryHandleSJC[i];
			CString Start_Gd_Name;
			m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
			CString str_fx;
			int i_excel_row_current=this->num;
			//��ѯͨ����·
			nHandleStart=nAarryHandleSJC[i];
			for(int m=0;m<nAarryHandleXJJC.GetCount();m++)
			{
				long nHandleTjEnd=nAarryHandleXJJC[m];
				long nxhj_start=0;
				long nxhj_end=0;
				long nlastdaocha=0;
				vector<CString>vec_xhj;
				vector<CString>vec_path;
				vector<CString>vec_daocha;
				vector<CString>vec_cxgd;
				CLongArray nArrayHandleLineTJ;
				CString str_GdStartname;
				CString str_xhj_start;
				CString str_xhj_end;
				int i_CntTJ=0;
				CLongArray nArrayHandleStartXHJ;
				CLongArray nArrayHandleEndXHJ;
				this->GetGuiDaoDuanXHJ(nHandleStart,false,nArrayHandleStartXHJ);
				if(nArrayHandleStartXHJ.GetCount()>0)
					nxhj_start=nArrayHandleStartXHJ[0];
				this->GetGuiDaoDuanXHJ(nHandleTjEnd,false,nArrayHandleEndXHJ);
				if(nArrayHandleEndXHJ.GetCount()>0)
					nxhj_end=nArrayHandleEndXHJ[0];
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),str_GdStartname);
				//��ȡ�ն��źŻ����ж��Ƿ��дͨ����·
				GetEntName(nxhj_end,str_fx);
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),str_GdStartname);
				if(str_fx.Right(1)==_T("N"))
				{
					GetTongGuoPathGuiDaoAndXHJ(str_GdStartname,nxhj_start,nxhj_end,nHandleStart,nHandleStart,nHandleTjEnd,nlastdaocha,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleLineTJ,vec_cxgd,i_CntTJ,false);
					this->num=this->num-i_CntTJ*2;
					for(int j=0;j<i_CntTJ;j++)
					{
						//ͨ����·����Ϊ����ͨ������ʱ����ͨ����·
						for(int i_col=3;i_col<=17;i_col++)
						{
							//oper_excel.SetColumnCombandCells(1,this->num,i_col,2);
							if(i_col==3)
							{
								oper_excel.SetColumnCombandCellsAndCenterShow(1,this->num,i_col,2);
								oper_excel.WriteDatatoExcel(1,this->num,3,_T("ͨ��"));
							}
							else
								oper_excel.SetColumnCombandCells(1,this->num,i_col,2);
						}
						oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
						if(j!=0)
						{
							for(int k=5;k<18;k++)
							{
								oper_excel.SetCellFontColor(1,this->num,k,3);
							}

						}
						this->num=this->num+2;
					}
				}		
			}
			int i_excel_row_current1=this->num;
			for(int k=0;k<nAarryHandleGuDaoS.GetCount();k++)
			{
				long nHandleEnd=nAarryHandleGuDaoS[k];
				vector<CString> path;
				vector<CString>daocha;
				vector<CString>xhj;
				vector<CString>vec_cx_gd;
				CLongArray  nArrayHandle;
				CStringArray DaochaNum;
				long LastDaocha=0;
				long nHandleStartXHJ=0;
				long nHandleEndXHJ=0;
				CLongArray nArrayStartXHJ;
				CLongArray nArrayEndXHJ;
				bool btoward=false;
				//��ȡ��ֹ����źŻ�
				this->GetGuiDaoDuanXHJ(nHandleStart,false,nArrayStartXHJ);
				if(nArrayStartXHJ.GetCount()>0)
					nHandleStartXHJ=nArrayStartXHJ[0];
				GetEntName(nHandleStartXHJ,str_fx);
				long nHandleLineXHJEnd=nAarryHandleGuDaoX[k];
				this->GetGuiDaoDuanXHJ(nHandleLineXHJEnd,false,nArrayEndXHJ);
				if(nArrayEndXHJ.GetCount()>0)
					nHandleEndXHJ =nArrayEndXHJ[0];
				//GetPathGuiDao4(nHandleStart,nHandleEnd,path,daocha,DaochaNum,sXJCGuiDaoName,nArrayHandle,false);
				//GetPathGuiDao(nHandleStart,nHandleEnd,path,sGuDaoName,nArrayHandle,false);
				//GetPathGuiDao3(nHandleStart,nHandleEnd,LastDaocha,path,daocha,sXJCGuiDaoName,nArrayHandle,false);
				int iJL_Cnt_temp=0;
				GetJieChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sSGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,false);
				//int iJL_Cnt_temp=0;
				//GetPathGuiDao4(nHandleStart,nHandleEnd,path,daocha,DaochaNum,sXJCGuiDaoName,nArrayHandle,false);
				//GetPathGuiDao3(nHandleStart,nHandleEnd,LastDaocha,path,daocha,sXJCGuiDaoName,nArrayHandle,false);
				//GetLieChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sXGuDaoName,nArrayHandle,iJL_Cnt_temp,false);
				//дȷ�����з������
				if(iJL_Cnt_temp>=2)
				{
					int row_num=this->num-iJL_Cnt_temp;
					for(int i=1;i<iJL_Cnt_temp;i++)
					{
						CString strDaocha1;
						CString strDaocha2;
						CString str_diff1;
						CString str_diff2;
						vector<CString>vec_str_Daocha1;
						vector<CString>vec_str_Daocha2;
						oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
						TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
						oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
						TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
						FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
						if(i==1)
						{
							oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
						}
						else
						{
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

						}
						row_num++;
					}
				}
				this->num=this->num-iJL_Cnt_temp;
				for(int j=0;j<iJL_Cnt_temp;j++)
				{
					oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
					if(j!=0)
					{
						for(int k=5;k<18;k++)
						{
							oper_excel.SetCellFontColor(1,this->num,k,3);
						}

					}
					this->num++;
				}
			}
			int i_excel_row_current2=this->num;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current1,3,i_excel_row_current2-i_excel_row_current1);
			oper_excel.WriteDatatoExcel(1,i_excel_row_current1,3,_T("�ӳ�"));
			//����
			long nHandleEnd=nAarryHandleSJJC[i];
			for(int k=0;k<nAarryHandleGuDaoS.GetCount();k++)
			{
				//long nHandleEnd=nAarryHandleSJJC[i];
				long nHandleStart=nAarryHandleGuDaoS[k];
				CString Start_Gd_Name;
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
				vector<CString> path;
				vector<CString>daocha;
				vector<CString>xhj;
				vector<CString>vec_cx_gd;
				CLongArray  nArrayHandle;
				//GetPathGuiDao(nHandleStart,nHandleEnd,path,sSJCGuiDaoName,nArrayHandle,true);
				CStringArray DaochaNum;
				long LastDaocha=0;
				long nHandleStartXHJ=0;
				long nHandleEndXHJ=0;
				CLongArray nArrayStartXHJ;
				CLongArray nArrayEndXHJ;
				bool btoward=true;
				//��ȡ��ֹ����źŻ�
				long nHandlineXHJStart=nAarryHandleGuDaoX[k];
				this->GetGuiDaoDuanXHJ(nHandlineXHJStart,false,nArrayStartXHJ);
				if(nArrayStartXHJ.GetCount()>0)
					nHandleStartXHJ=nArrayStartXHJ[0];
				this->GetGuiDaoDuanXHJ(nHandleEnd,true,nArrayEndXHJ);
				if(nArrayEndXHJ.GetCount()>0)
					nHandleEndXHJ =nArrayEndXHJ[0];
				int iJL_Cnt_temp=0;
				GetFaChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sSGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,true);
				//GetLieChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sSGuDaoName,nArrayHandle,iJL_Cnt_temp,false);
				if(iJL_Cnt_temp>=2)
				{
					int row_num=this->num-iJL_Cnt_temp;
					for(int i=1;i<iJL_Cnt_temp;i++)
					{
						CString strDaocha1;
						CString strDaocha2;
						CString str_diff1;
						CString str_diff2;
						vector<CString>vec_str_Daocha1;
						vector<CString>vec_str_Daocha2;
						oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
						TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
						oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
						TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
						FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
						if(i==1)
						{
							oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
						}
						else
						{
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

						}
						row_num++;
					}
				}
				this->num=this->num-iJL_Cnt_temp;
				for(int j=0;j<iJL_Cnt_temp;j++)
				{
					oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
					if(j!=0)
					{
						for(int k=5;k<18;k++)
						{
							oper_excel.SetCellFontColor(1,this->num,k,3);
						}

					}
					this->num++;
				}
			}
			int i_excel_row_current3=this->num;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current2,3,i_excel_row_current3-i_excel_row_current2);
			oper_excel.WriteDatatoExcel(1,i_excel_row_current2,3,_T("����"));
			//this->num=this->num+2;
			int i_excel_row_current4=this->num;
			//д����
			//CString str_T=_T("���ݷ���");
			//str_T+=_T(" ");
			str_fx=_T("���ݷ��� ")+str_fx;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current,2,i_excel_row_current4-i_excel_row_current);
			oper_excel.WriteDatatoExcel(1,i_excel_row_current,2,str_fx);
		}
	  	//�г���·����
		int i_excel_row_lieche=this->num;
		oper_excel.SetColumnCombandCellsAndCenterShow(1,3,1,i_excel_row_lieche-3);
		oper_excel.WriteDatatoExcel(1,3,1,_T("�г���·"));
	//��ȡ���еĵ����źŻ�
	CLongArray nArrayHandle_DCXHJ;
	CStringArray JieJinQuDuan;
	JieJinQuDuan.Copy(sSJCGuiDaoName);
	//JieJinQuDuan.Copy(sXJCGuiDaoName);
	for(int i=0;i<sXJCGuiDaoName.GetCount();i++)
	{
		//acutPrintf(_T("\t�ӽ�����:%s"),JieJinQuDuan.GetAt(i));
		JieJinQuDuan.Add(sXJCGuiDaoName.GetAt(i));
	}
	CString name_DCXHJ=_T("XHJ_DC");
	int iRet3 =GetSomeBlocks(name_DCXHJ,nArrayHandle_DCXHJ);
	int iNum_DC=nArrayHandle_DCXHJ.GetCount();
	acutPrintf(_T("\n һ����%d�������źŻ�"),iNum_DC);
	CStringArray strArrayDcName;
	for(int i=0;i<iNum_DC;i++)
	{
		long nHandleDCXHJ=nArrayHandle_DCXHJ[i];
		CString name;
		CString fangxiang;
		CStringArray strNextDC;
		CLongArray nArrayHandleNext;
		long nHandleStartLine=0;
		bool bforward=true;
		int iTo=1;
		GetBlockToward(nHandleDCXHJ,iTo);
		if(iTo==1)
		{
			fangxiang=_T("��");
		}
		else if(iTo==2)
		{
			fangxiang=_T("��");
		}
		else if(iTo==0)
		{
			fangxiang=_T("��");
		}
		else if(iTo==3)
		{
			fangxiang=_T("��");
		}
		else 
		{
			fangxiang=_T("δ֪");
		}
		//long LastDaocha=0;
		GetEntName(nHandleDCXHJ,name);
		acutPrintf(_T("\n�źŻ�%s:%s"),name,fangxiang);
		strArrayDcName.Add(name);
	}
	//��������
	TCHAR del_char=_T('D');
	long  t31=GetTickCount();
	CBlkUtility::SortStringArray(strArrayDcName,del_char);
	long t32=GetTickCount();
	CString str32;
	str32.Format("SortArray time:%d ms",t32-t31);
	acutPrintf(_T("\n %s"),str32);
	long nHandleStartLine=0;
	bool bforward=true;
	//���������ʺ�
	CStringArray strArrayOdd;
	CStringArray strArrayEven;
	CBlkUtility::SortandDistinguishStringArray(strArrayDcName,strArrayOdd,strArrayEven,del_char);
	acutPrintf(_T("\n���е����źŻ���"));
	for(int i=0;i<strArrayOdd.GetCount();i++)
	{
		acutPrintf(_T(" %s\t"),strArrayOdd.GetAt(i));
	}
	acutPrintf(_T("\n���е����źŻ���"));
	for(int i=0;i<strArrayEven.GetCount();i++)
	{
		acutPrintf(_T(" %s\t"),strArrayEven.GetAt(i));
	}
	//��������ʺ������·
	for(int i=0;i<strArrayOdd.GetCount();i++)
	{
		CStringArray strNextDC;
		CLongArray nArrayHandleNext;
		//long LastDaocha=0;
		long nHandleDCXHJ=0;
		CString temp=strArrayOdd.GetAt(i);
		GetHandleByName(temp,nHandleDCXHJ);
		if(GetXHJ_YouCe_GdLine(nHandleDCXHJ,nHandleStartLine,bforward)!=0)
		{
			nHandleStartLine=0;
		}
		//��ȡ�����źŻ��ĵ����ն�
		this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sXGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
		CString xhj;
		GetEntName(nHandleDCXHJ,xhj);
		acutPrintf(_T("\n����ʼ�ˣ�%s"),xhj);
		if(xhj==_T("D31"))
		{
			xhj=xhj;
		}
		acutPrintf("\n�����նˣ�\n");
 		for(int k=0;k<strNextDC.GetCount();k++)
		 {	
			CString strGuiname;
			m_mysys.GetXdata(nArrayHandleNext.GetAt(k),_T("GUIDAONAME"),strGuiname);
			acutPrintf(_T("\t%s--%s"),strNextDC.GetAt(k),strGuiname);
		}
		 int i_count=nArrayHandleNext.GetCount();
		 if(xhj==_T("S3"))
		 {
			 i_count=i_count;
		 }
		 int i_row_diaochecurrent=this->num;
		if(nHandleStartLine!=0)
		{
			//���������·
			for(int i=0;i<nArrayHandleNext.GetCount();i++)
			{
				long nHandEndXHJ=0;
				long nHandleEndLine=nArrayHandleNext.GetAt(i);
				CString Gd_name;
				m_mysys.GetXdata(nHandleEndLine,_T("GUIDAONAME"),Gd_name);
				acutPrintf(_T("\t �ն�:%s"),Gd_name);
				long LastDaocha=0;
				long nHandleEndXHJ=0;
				vector<CString>vec_path;
				vector<CString>vec_daocha;
				vector<CString>vec_xhj;
				vector<CString>vec_cx_gd;
				CLongArray nArrayHandleNextLine;
				CString str_xhj_start;
				CString str_xhj_end=strNextDC.GetAt(i);
				GetEntName(nHandleDCXHJ,str_xhj_start);
				if(str_xhj_start==_T("D5"))
				{
					str_xhj_end=str_xhj_end;
				}
				int iDCJL_Cnt=0;
				GetDiaoChePathGuiDaoAndXHJ2(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleStartLine,nHandleEndLine,LastDaocha,JieJinQuDuan,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleNextLine,vec_cx_gd,iDCJL_Cnt,false,bforward);	
				//GetDiaoChePathGuiDaoAndXHJ(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleEndLine,LastDaocha,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleNextLine,iDCJL_Cnt,bforward);
				if(iDCJL_Cnt>=2)
				{
					int row_num=this->num-iDCJL_Cnt;
					for(int i=1;i<iDCJL_Cnt;i++)
					{
						CString strDaocha1;
						CString strDaocha2;
						CString str_diff1;
						CString str_diff2;
						vector<CString>vec_str_Daocha1;
						vector<CString>vec_str_Daocha2;
						oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
						TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
						oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
						TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
						FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
						if(i==1)
						{
							oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
						}
						else
						{
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							//��������ʹ�����·����ɫ��ע��
							//oper_excel.SetCellFontColor(1,row_num+1,8,2);
						}
						row_num++;
					}
				}
			    this->num=this->num-iDCJL_Cnt;
				//int i_row_diaocheCurrent=this->num-iDCJL_Cnt;
				for(int i=0;i<iDCJL_Cnt;i++)
				{
					oper_excel.WriteDatatoExcel(1,this->num,6,(i+1));
					if(i!=0)
					{
						for(int k=5;k<18;k++)
						{
							oper_excel.SetCellFontColor(1,this->num,k,3);
						}
					}
					this->num++;
				}
			}
			int i_row_diaochecurrent2=this->num;
			oper_excel.SetColumnCombandCells(1,i_row_diaochecurrent,3,i_row_diaochecurrent2-i_row_diaochecurrent);
			oper_excel.WriteDatatoExcel(1,i_row_diaochecurrent,3,xhj);
		}
	}	
	//��ȡ���г�վ�źŻ��������źŻ�
	CLongArray nArrayXXHJ;
	CStringArray str_name_ArrayXXHJ;
	for(int i=0;i<nAarryHandleGuDaoS.GetCount();i++)
	{
		long nHandleXgd=nAarryHandleGuDaoS.GetAt(i);
		CLongArray nArrayHandleXHJ;
		long zhixhj=0;
		long xiangxhj=0;
		CString strgdname;
		GetGuiDaoDuanXHJ(nHandleXgd,true,nArrayHandleXHJ);
		m_mysys.GetXdata(nHandleXgd,_T("GUIDAONAME"),strgdname);
		//GetGuiDaoDuanXHJ(true,)
		if(nArrayHandleXHJ.GetCount()>0)
			nArrayXXHJ.Add(nArrayHandleXHJ[0]);
		CString strname;
		GetEntName(nArrayHandleXHJ[0],strname);
		acutPrintf(_T("���г�վ�źŻ�:%s  %s\t"),strgdname,strname);
		str_name_ArrayXXHJ.Add(strname);
	}
	//���������
	acutPrintf(_T("\n��������ʺ��վ������źŻ�:\n"));
	//CBlkUtility::SortStringArray_CZXHJ(str_name_ArrayXXHJ,_T('S'));
	CBlkUtility::SortStringArray_CZXHJHandle(str_name_ArrayXXHJ,_T('S'),nArrayXXHJ);
	for(int i=0;i<nArrayXXHJ.GetCount();i++)
	{
		CStringArray strNextDC;
		CLongArray nArrayHandleNext;
		long  nHandleStartLine=0;
		long nHandleDCXHJ=nArrayXXHJ.GetAt(i);
		if(GetXHJ_YouCe_GdLine(nHandleDCXHJ,nHandleStartLine,bforward)!=0)
		{
			nHandleStartLine=0;
		}
		CString xhj;
		GetEntName(nHandleDCXHJ,xhj);
		acutPrintf(_T("\n����ʼ�ˣ�%s"),xhj);
		this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sSGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
		int i_count= nArrayHandleNext.GetCount();
		if(xhj==_T("S3"))
		{
			i_count=i_count;
		}
		int i_row_diaochecurrent=this->num;
		if(nHandleStartLine!=0)
		{
			//���������·
			for(int i=0;i<nArrayHandleNext.GetCount();i++)
			{
				long nHandleEndLine=nArrayHandleNext.GetAt(i);
				long LastDaocha=0;
				long nHandleEndXHJ=0;
				vector<CString>vec_path;
				vector<CString>vec_daocha;
				vector<CString>vec_xhj;
				vector<CString>vec_cx_gd;
				CLongArray nArrayHandleNextLine;
				CString str_xhj_start;
				CString str_xhj_end=strNextDC.GetAt(i);
				GetEntName(nHandleDCXHJ,str_xhj_start);
				str_xhj_start+=_T("D");
				int iDCJL_Cnt=0;
				GetDiaoChePathGuiDaoAndXHJ2(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleStartLine,nHandleEndLine,LastDaocha,JieJinQuDuan,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleNextLine,vec_cx_gd,iDCJL_Cnt,false,bforward);
				if(iDCJL_Cnt>=2)
				{
					int row_num=this->num-iDCJL_Cnt;
					for(int i=1;i<iDCJL_Cnt;i++)
					{
						CString strDaocha1;
						CString strDaocha2;
						CString str_diff1;
						CString str_diff2;
						vector<CString>vec_str_Daocha1;
						vector<CString>vec_str_Daocha2;
						oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
						TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
						oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
						TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
						FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
						if(i==1)
						{
							oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
						}
						else
						{
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							//oper_excel.SetCellFontColor(1,row_num+1,8,2222);

						}
						row_num++;
					}
				}
				this->num=this->num-iDCJL_Cnt;
				//int i_row_diaocheCurrent=this->num-iDCJL_Cnt;
				for(int i=0;i<iDCJL_Cnt;i++)
				{
					oper_excel.WriteDatatoExcel(1,this->num,6,(i+1));
					if(i!=0)
					{
						for(int k=5;k<18;k++)
						{
							oper_excel.SetCellFontColor(1,this->num,k,3);
						}

					}
					this->num++;
				}
				int i_row_diaochecurrent2=this->num;
				oper_excel.SetColumnCombandCells(1,i_row_diaochecurrent,3,i_row_diaochecurrent2-i_row_diaochecurrent);
				oper_excel.WriteDatatoExcel(1,i_row_diaochecurrent,3,str_xhj_start);
			}

		}

	}
//	��������ʺ������·
		for(int i=0;i<strArrayEven.GetCount();i++)
		{
			CStringArray strNextDC;
			CLongArray nArrayHandleNext;
			long nHandleDCXHJ=0;
			CString temp=strArrayEven.GetAt(i);
			if(temp==_T("D16"))
			{
				temp=temp;
			}
			GetHandleByName(temp,nHandleDCXHJ);
			if(GetXHJ_YouCe_GdLine(nHandleDCXHJ,nHandleStartLine,bforward)!=0)
			{
				nHandleStartLine=0;
			}
			this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sSGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
			CString xhj;
			GetEntName(nHandleDCXHJ,xhj);
			acutPrintf(_T("\n����ʼ�ˣ�%s"),xhj);
			if(xhj==_T("X3"))
			{
				xhj=xhj;
			}
			acutPrintf("\n�����նˣ�\n");
			for(int k=0;k<strNextDC.GetCount();k++)
			{
				//acutPrintf(_T("\t%s"),strNextDC.GetAt(k));
				CString strGuiname;
				m_mysys.GetXdata(nArrayHandleNext.GetAt(k),_T("GUIDAONAME"),strGuiname);
				acutPrintf(_T("\t%s--%s"),strNextDC.GetAt(k),strGuiname);
			}
			int i_row_diaochecurrent=this->num;
			if(nHandleStartLine!=0)
			{
				//���������·
				for(int i=0;i<nArrayHandleNext.GetCount();i++)
				{
					long nHandleEndLine=nArrayHandleNext.GetAt(i);
					CString Gd_name;
					m_mysys.GetXdata(nHandleEndLine,_T("GUIDAONAME"),Gd_name);
					long LastDaocha=0;
					long nHandleEndXHJ=0;
					vector<CString>vec_path;
					vector<CString>vec_daocha;
					vector<CString>vec_xhj;
					vector<CString>vec_cx_gd;
					CLongArray nArrayHandleNextLine;
					CString str_xhj_start;
					CString str_xhj_end=strNextDC.GetAt(i);
					GetEntName(nHandleDCXHJ,str_xhj_start);
					int iDCJL_Cnt=0;
					GetDiaoChePathGuiDaoAndXHJ2(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleStartLine,nHandleEndLine,LastDaocha,JieJinQuDuan,vec_path,vec_daocha,vec_xhj,sSGuDaoName,nArrayHandleNextLine,vec_cx_gd,iDCJL_Cnt,true,bforward);
					//GetDiaoChePathGuiDaoAndXHJ2(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleStartLine,nHandleEndLine,LastDaocha,JieJinQuDuan,vec_path,vec_daocha,vec_xhj,sSGuDaoName,nArrayHandleNextLine,vec_cx_gd,iDCJL_Cnt,bforward);	
					if(iDCJL_Cnt>=2)
					{
						int row_num=this->num-iDCJL_Cnt;
						for(int i=1;i<iDCJL_Cnt;i++)
						{
							CString strDaocha1;
							CString strDaocha2;
							CString str_diff1;
							CString str_diff2;
							vector<CString>vec_str_Daocha1;
							vector<CString>vec_str_Daocha2;
							oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
							TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
							oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
							TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
							FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
							if(i==1)
							{
								oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							}
							else
							{
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
	
							}
							row_num++;
						}
					}
					this->num=this->num-iDCJL_Cnt;
					for(int i=0;i<iDCJL_Cnt;i++)
					{
						oper_excel.WriteDatatoExcel(1,this->num,6,(i+1));
						if(i!=0)
						{
							for(int k=5;k<18;k++)
							{
								oper_excel.SetCellFontColor(1,this->num,k,3);
							}
	
						}
						this->num++;
					}
				}
				int i_row_diaochecurrent2=this->num;
				oper_excel.SetColumnCombandCells(1,i_row_diaochecurrent,3,i_row_diaochecurrent2-i_row_diaochecurrent);
				oper_excel.WriteDatatoExcel(1,i_row_diaochecurrent,3,xhj);
			}
		}	
// 	��վ�źŻ�������źŻ���Ϊ����ʼ��
	//��ȡ���г�վ�źŻ����������źŻ�
	CLongArray nArraySXHJ;
	CStringArray str_name_ArraySXHJ;
	for(int i=0;i<nAarryHandleGuDaoX.GetCount();i++)
	{
		long nHandleXgd=nAarryHandleGuDaoX.GetAt(i);
		CLongArray nArrayHandleXHJ;
		long zhixhj=0;
		long xiangxhj=0;
		CString strgdname;
		GetGuiDaoDuanXHJ(nHandleXgd,false,nArrayHandleXHJ);
		m_mysys.GetXdata(nHandleXgd,_T("GUIDAONAME"),strgdname);
		//GetGuiDaoDuanXHJ(true,)
		if(nArrayHandleXHJ.GetCount()>0)
			nArraySXHJ.Add(nArrayHandleXHJ[0]);
		CString strname;
		GetEntName(nArrayHandleXHJ[0],strname);
		acutPrintf(_T("���г�վ�źŻ�:%s  %s\t"),strgdname,strname);
		str_name_ArraySXHJ.Add(strname);
	}
// 	//���������
	acutPrintf(_T("\n��������ʺ��վ������źŻ�:\n"));
	//CBlkUtility::SortStringArray_CZXHJ(str_name_ArrayXXHJ,_T('S'));
	CBlkUtility::SortStringArray_CZXHJHandle(str_name_ArraySXHJ,_T('X'),nArraySXHJ);
	for(int i=0;i<str_name_ArraySXHJ.GetCount();i++)
	{
		acutPrintf(_T("%s\t"),str_name_ArraySXHJ.GetAt(i));
	}
    for(int i=0;i<nArraySXHJ.GetCount();i++)
    {
		CStringArray strNextDC;
		CLongArray nArrayHandleNext;
		long  nHandleStartLine=0;
		long nHandleDCXHJ=nArraySXHJ.GetAt(i);
		if(GetXHJ_YouCe_GdLine(nHandleDCXHJ,nHandleStartLine,bforward)!=0)
		{
   			nHandleStartLine=0;
		}
		CString xhj;
		GetEntName(nHandleDCXHJ,xhj);
		acutPrintf(_T("\n����ʼ�ˣ�%s"),xhj);
		this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sSGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
		int i_count=nArrayHandleNext.GetCount();
		if(xhj==_T("X3"))
		{
			i_count=i_count;
		}
		int i_row_diaochecurrent=this->num;
		if(nHandleStartLine!=0)
		{
   			//���������·
   			for(int i=0;i<nArrayHandleNext.GetCount();i++)
   			{
   				long nHandleEndLine=nArrayHandleNext.GetAt(i);
   				long LastDaocha=0;
   				long nHandleEndXHJ=0;
   				vector<CString>vec_path;
   				vector<CString>vec_daocha;
   				vector<CString>vec_xhj;
				vector<CString>vec_cx_gd;
   				CLongArray nArrayHandleNextLine;
   				CString str_xhj_start;
   				CString str_xhj_end=strNextDC.GetAt(i);
   				GetEntName(nHandleDCXHJ,str_xhj_start);
   				str_xhj_start+=_T("D");
				int iDCJL_Cnt=0;
   				//GetDiaoChePathGuiDaoAndXHJ(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleEndLine,LastDaocha,vec_path,vec_daocha,vec_xhj,sGuDaoName,nArrayHandleNextLine,bforward);	
				GetDiaoChePathGuiDaoAndXHJ2(str_xhj_start,str_xhj_end,nHandleStartLine,nHandleStartLine,nHandleEndLine,LastDaocha,JieJinQuDuan,vec_path,vec_daocha,vec_xhj,sSGuDaoName,nArrayHandleNextLine,vec_cx_gd,iDCJL_Cnt,true,bforward);
				if(iDCJL_Cnt>=2)
				{
					int row_num=this->num-iDCJL_Cnt;
					for(int i=1;i<iDCJL_Cnt;i++)
					{
						CString strDaocha1;
						CString strDaocha2;
						CString str_diff1;
						CString str_diff2;
						vector<CString>vec_str_Daocha1;
						vector<CString>vec_str_Daocha2;
						oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
						TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
						oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
						TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
						FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
						if(i==1)
						{
							oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
						}
						else
						{
							oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

						}
						row_num++;
					}
				}
				this->num=this->num-iDCJL_Cnt;
				//int i_row_diaocheCurrent=this->num-iDCJL_Cnt;
				for(int i=0;i<iDCJL_Cnt;i++)
				{
					oper_excel.WriteDatatoExcel(1,this->num,6,(i+1));
					if(i!=0)
					{
						for(int k=5;k<18;k++)
						{
							oper_excel.SetCellFontColor(1,this->num,k,3);
						}

					}
					this->num++;
				}
				int i_row_diaochecurrent2=this->num;
				oper_excel.SetColumnCombandCells(1,i_row_diaochecurrent,3,i_row_diaochecurrent2-i_row_diaochecurrent);
				oper_excel.WriteDatatoExcel(1,i_row_diaochecurrent,3,str_xhj_start);
   			}
		} 	
    }
	int i_row_diaochecurrent3=this->num;
	oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_lieche,1,i_row_diaochecurrent3-i_excel_row_lieche);
	oper_excel.WriteDatatoExcel(1,i_excel_row_lieche,1,_T("������·"));
	oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_lieche,2,i_row_diaochecurrent3-i_excel_row_lieche);
	oper_excel.WriteDatatoExcel(1,i_excel_row_lieche,2,_T("��"));
	//�����Ƿ����������·�����ж����µ����������ʺ��������ʺ�
	//��ѯͼ���Ƿ����PMT_XBD_��
	CLongArray nArrayHandle_XBD;
	CLongArray nArrayHandle_XBD_Line;
	CString str_name_xbd=_T("PMT_XBD");
	bool b_up=false;
	bool b_down=false;
	int iCnt_XBD=GetSomeBlocks(str_name_xbd,nArrayHandle_XBD);
	if(iCnt_XBD>=1)
	{
		BOOL b_SX=true;
		long nHandleJJG=0;
		int i_row_yanxucurrent=this->num;
		//����,��ѯ���ڹ��
		for(int i=0;i<nArrayHandle_XBD.GetCount();i++)
		{
			long nHandleGdLine=0;
			nHandleJJG=nArrayHandle_XBD.GetAt(i);
           Get_Block_Online(nHandleJJG,nHandleGdLine);
		   if(nHandleGdLine!=0)
			   nArrayHandle_XBD_Line.Add(nHandleGdLine);
		   if(CBlkUtility::FindLongInArray(nAarryHandleXJJC,nHandleGdLine)!=-1)
		   {
			   b_up=true;
			   break;
		   }
		   if(CBlkUtility::FindLongInArray(nAarryHandleSJJC,nHandleGdLine)!=-1)
		   {
			   b_down=true;
			   break;
		   }	   
		}
		if(b_down==true)
		{
			for(int i=0;i<nAarryHandleGuDaoS.GetCount();i++)
			{
					//��ʼ���
				int i_excel_row_nei_current=this->num;
				long nHandleStart=nAarryHandleGuDaoS.GetAt(i);
				CString Start_Gd_Name;
				for(int j=0;j<nAarryHandleXJJC.GetCount();j++)
				{
					//��ֹ���
					long nHandleEnd=nAarryHandleXJJC.GetAt(j);
					long nHandleStartXHJ=0;
					long nHandleEndXHJ=0;
					long LastDaocha=0;
					vector<CString>path;
					vector<CString>daocha;
					vector<CString>xhj;
					vector<CString>vec_cx_gd;
					CLongArray nArrayHandle;
					int iJL_Cnt_temp=0;
					m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
					GetYanXuPathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sSGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,false);
					if(iJL_Cnt_temp>=2)
					{
						//��ȡ�ؼ����е���
						int row_num=this->num-iJL_Cnt_temp;
						for(int i=1;i<iJL_Cnt_temp;i++)
						{
							CString strDaocha1;
							CString strDaocha2;
							CString str_diff1;
							CString str_diff2;
							vector<CString>vec_str_Daocha1;
							vector<CString>vec_str_Daocha2;
							oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
							TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
							oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
							TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
							FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
							if(i==1)
							{
								oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							}
							else
							{
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

							}
							row_num++;
						}
					}
					this->num=this->num-iJL_Cnt_temp;
					for(int j=0;j<iJL_Cnt_temp;j++)
					{
						oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
						this->num++;
					}
				}	
				int i_row_nei_yanxucurrent=this->num;
				oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_nei_current,3,i_row_nei_yanxucurrent-i_excel_row_nei_current);
				oper_excel.WriteDatatoExcel(1,i_excel_row_nei_current,3,Start_Gd_Name);
			}
			int i_excel_row_current3=this->num;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_row_yanxucurrent,1,i_excel_row_current3-i_row_yanxucurrent);
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_row_yanxucurrent,2,i_excel_row_current3-i_row_yanxucurrent);
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,1,_T("������·"));
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,2,_T("��"));
		}
		if(b_up==true)
		{
			for(int i=0;i<nAarryHandleGuDaoX.GetCount();i++)
			{
				//��ʼ���
				long nHandleStart=nAarryHandleGuDaoX.GetAt(i);
				CString Start_Gd_Name;
                int i_excel_row_nei_current=this->num;
				for(int j=0;j<nAarryHandleSJJC.GetCount();j++)
				{
					//��ֹ���
					long nHandleEnd=nAarryHandleSJJC.GetAt(j);
					long nHandleStartXHJ=0;
					long nHandleEndXHJ=0;
					long LastDaocha=0;
					vector<CString>path;
					vector<CString>daocha;
					vector<CString>xhj;
					vector<CString>vec_cx_gd;
					CLongArray nArrayHandle;
					int iJL_Cnt_temp=0;
					m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
					GetYanXuPathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sSGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,true);
					if(iJL_Cnt_temp>=2)
					{
						//��ȡ�ؼ����е���
						int row_num=this->num-iJL_Cnt_temp;
						for(int i=1;i<iJL_Cnt_temp;i++)
						{
							CString strDaocha1;
							CString strDaocha2;
							CString str_diff1;
							CString str_diff2;
							vector<CString>vec_str_Daocha1;
							vector<CString>vec_str_Daocha2;
							oper_excel.ReadDatafromExcel(1,row_num,12,strDaocha1);
							TranslateCStringtoVector(strDaocha1,vec_str_Daocha1);
							oper_excel.ReadDatafromExcel(1,row_num+1,12,strDaocha2);
							TranslateCStringtoVector(strDaocha2,vec_str_Daocha2);
							FindFirstDiffString(vec_str_Daocha1,vec_str_Daocha2,str_diff1,str_diff2);
							if(i==1)
							{
								oper_excel.WriteDatatoExcel(1,row_num,8,str_diff1);
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);
							}
							else
							{
								oper_excel.WriteDatatoExcel(1,row_num+1,8,str_diff2);

							}
							row_num++;
						}
					}
					this->num=this->num-iJL_Cnt_temp;
					for(int j=0;j<iJL_Cnt_temp;j++)
					{
						oper_excel.WriteDatatoExcel(1,this->num,6,(j+1));
						this->num++;
					}
				}	
				int i_row_nei_yanxucurrent=this->num;
				oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_nei_current,3,i_row_nei_yanxucurrent-i_excel_row_nei_current);
				oper_excel.WriteDatatoExcel(1,i_excel_row_nei_current,3,Start_Gd_Name);
			}
			int i_excel_row_current3=this->num;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_row_yanxucurrent,1,i_excel_row_current3-i_row_yanxucurrent);
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_row_yanxucurrent,2,i_excel_row_current3-i_row_yanxucurrent);
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,1,_T("������·"));
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,2,_T("��"));
		}

	}
	oper_excel.SaveExcel();
	//CString strFileSave=_T("%s\\support\\xls\\ƽ��ͼ������.xls",g_strBasePath);
// 	CString strFileSave;
// 	if(FALSE==this->SelectFilePath(strFileSave))
// 	{
// 		AfxMessageBox(_T("ָ�����ļ�·����Ч��"));
// 		return ;
// 	}
	oper_excel.SaveasXSLFile(strFileSave);
	oper_excel.RealseExcel();
	acutPrintf(_T("\n����ʼʱ��%s"),str_Time);
	CTime  time_end=CTime::GetCurrentTime();
	 //CTime time_take=time_end-time_beg;
     str_Time=time_end.Format("%Y-%m-%d %H:%M:%S");
	acutPrintf(_T("\n�������ʱ��%s"),str_Time);
	acutPrintf(_T("\n�������ݴ���=%d"),m_RecordSet.i_count);
}
long CZnSxt::GetDuXianMinDaoCha( long nHandleDc_One,long  nHandleDc_Another )
{
	try
	{
		//if(nHandleDc_Another!=GetAnotherDuXianDaoCha())
		if(nHandleDc_One==0||nHandleDc_Another==0)
		{
			return 0;
		}
		long nHandle=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandle)==TRUE)
		{
			//�Ƕ��ߵ���
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//ȡ����λ��
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//ȡ����λ��
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_another);
			int iDCNum1 = _ttoi(name_one.GetString()) ;
			int iDcNum2=_ttoi(name_another.GetString());
			return iDCNum1<iDcNum2?nHandleDc_One:nHandleDc_Another;
		}
		else
		{
			return 0;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}
	return 0;

}
long CZnSxt::GetDuXianMaxDaoCha( long nHandleDc_One,long  nHandleDc_Another )
{
	try
	{
		//if(nHandleDc_Another!=GetAnotherDuXianDaoCha())
		if(nHandleDc_One==0||nHandleDc_Another==0)
		{
			return 0;
		}
		long nHandle=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandle)==TRUE)
		{
			//�Ƕ��ߵ���
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//ȡ����λ��
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//ȡ����λ��
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_another);
			int iDCNum1 = _ttoi(name_one.GetString()) ;
			int iDcNum2=_ttoi(name_another.GetString());
			return iDCNum1>iDcNum2?nHandleDc_One:nHandleDc_Another;
		}
		else
		{
			return 0;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}
	return 0;

}


BOOL CZnSxt::GetAnotherDuXianDaoCha( long nHandleDc_One,long & nHandleDc_Another )
{
	try
	{

		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
		long nHandle_fw=0;
		//ȡ����λ��
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw);
		if(nHandle_fw==0)
		{
			nHandleDc_Another =0;
			return FALSE;
		}
		//�������������һ���
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		//int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine_left =0;
		long nHandleNextLine_right=0;
		long linetype1=-1;
		long linetype2=-1;
		if(nHandleLeftJyj!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleLeftJyj) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"), nHandleNextLine_left);
			strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleNextLine_left) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), linetype1) ;
		}
		if(nHandleRightJyj!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleRightJyj) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"), nHandleNextLine_right);
			strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleNextLine_right) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), linetype2) ;
		}
		//�жϺ���Ƿ���һ��Ϊ����
		if(nHandleNextLine_left==0&&nHandleNextLine_right==0)
		{
			nHandleDc_Another =0;
			return FALSE;
		}
		if(linetype1==1)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine_left) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			long nHandle_DC1=0;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandle_DC1);
			if(nHandle_DC1!=0)
			{
				nHandleDc_Another =nHandle_DC1;
				return TRUE;	
			}
		}
		if(linetype2==1)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine_right) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			long nHandle_DC2=0;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandle_DC2);	
			if(nHandle_DC2!=0)
			{
				nHandleDc_Another =nHandle_DC2;
				return TRUE;	
			}
		}
		nHandleDc_Another=0;
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}
	return FALSE;
}
//��һ�����ߵ������������������ߣ��򷵻���һ�����ߵ�������������
BOOL CZnSxt::GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		//�����������������Ƕ���
		long nHandleDc_One2=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandleDc_One2)==TRUE)
		{
			if(nHandleDc_Another==nHandleDc_One2)
			{//����Ϊ����
				long nHandleline1=0;
				long nHandleline2=0;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline1) ;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline2) ;
				//��online1�ϵ����������Ƿ�Ϊ���ߵ���
				CLongArray nArrayHandleDC;
				CLongArray nArrayHandleXHJ;
				CLongArray nArrayHandleLineCur;
				CLongArray nArrayHandleLineType;
				int  iNumDC=0;
				this->GetGuiDaoDaoChaOrderByXAsc(nHandleline1,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
				//this->GetGuiDaoDaoChaOrderByXAsc()
				iNumDC =nArrayHandleDC.GetCount();
				CLongArray nArrayHandleDC2;
				CLongArray nArrayHandleXHJ2;
				CLongArray nArrayHandleLineCur2;
				CLongArray nArrayHandleLineType2;
				int iNumDC2=0;
				this->GetGuiDaoDaoChaOrderByXAsc(nHandleline2, nArrayHandleDC2,nArrayHandleLineCur2,nArrayHandleLineType2);
				iNumDC2 =nArrayHandleDC2.GetCount();
				if(iNumDC2<2||iNumDC<2)
				{
					return FALSE;
				}
				for(int i=0;i<iNumDC;i++)
				{
					long nHandleDC =nArrayHandleDC[i];
					if(nHandleDC!=nHandleDc_One)
					{
						//�Ƿ��Ƕ���
						long nHandleDC_A=0;
						if(this->GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_A)==TRUE)
						{
							//�Ƕ��ߵ���������һ����λ��line2����ʱ������������ι�ͬ����������ߣ���ܿ���Ϊ�������
							if(CBlkUtility::FindLongInArray(nArrayHandleDC2,nHandleDC_A)!=-1)
							{
								//�Ƿ���Ҫ�Ӷ������������ֽ������
								nHandleDc_Min =this->GetDuXianMinDaoCha(nHandleDC,nHandleDC_A);
								nHandleDc_Max =this->GetDuXianMaxDaoCha(nHandleDC,nHandleDC_A);
								return TRUE;
							}
						}
					}
				}

			}
			else
			{
				return FALSE;
			}

		}
		else
		{
			return FALSE;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}
	return FALSE;
}
BOOL CZnSxt::GetDaidongDaochas1( long nHandleDC,long &nHandleDc_DdMin,long &nHandleDc_DdMax )
{
	long t1=GetTickCount();
	long nHandleDc_DdMinT=0;
	long nHandleDc_DdMaxT=0;
	if(GetDaidongDaochas(nHandleDC,nHandleDc_DdMinT,nHandleDc_DdMaxT)==TRUE)
	{
		nHandleDc_DdMin =nHandleDc_DdMinT;
		nHandleDc_DdMax=nHandleDc_DdMaxT;
		long t2=GetTickCount();
		return TRUE;
	}
	long nHandleDC_Ano=0;
	if(GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_Ano)==TRUE)
	{
		if(GetDaidongDaochas(nHandleDC_Ano,nHandleDc_DdMinT,nHandleDc_DdMaxT)==TRUE)
		{
			nHandleDc_DdMin =nHandleDc_DdMinT;
			nHandleDc_DdMax=nHandleDc_DdMaxT;
			return true;
		}
	}
	return FALSE;
}
BOOL CZnSxt::GetDaidongDaochas1( long nHandleDC, CString &str_daidongdaochas)
{
	
	CString str_daidong_temp;
	if(GetDaidongDaochas(nHandleDC,str_daidong_temp)==TRUE)
	{
		str_daidongdaochas=str_daidong_temp;
		return TRUE;
	}
	long nHandleDC_Ano=0;
	if(GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_Ano)==TRUE)
	{
		if(GetDaidongDaochas(nHandleDC_Ano,str_daidong_temp)==TRUE)
		{
			str_daidongdaochas=str_daidong_temp;
			return TRUE;
		}
	}
	return FALSE;
}
//ĳ����λʱ����Ҫ��ĳ���������λʱ����������TRUE,����������DdMin��DdMax����,ֻ�����˴���һ������������û�п�����Ҫ�����������������ϵĵ���
BOOL CZnSxt::GetDaidongDaochas( long nHandleDC,long &nHandleDc_DdMin,long &nHandleDc_DdMax )
{
	try
	{
		//��ȡ�������ڶ�λ��
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		//long nHandle_fw=0;
		long nHandle_dw=0;
		long nHandle_fw=0;
		long iStart1=-1;
		nHandleDc_DdMin=0;
		nHandleDc_DdMax=0;
		//ȡ����λ��
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dw);
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw);
		m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),iStart1) ;
		if(nHandle_dw==0||nHandle_fw==0)
		{
			return FALSE;
		}
		if(nHandleDC==271605||nHandleDC==271599)
		{
			nHandleDC=nHandleDC;
		}
		//��ȡ��λ�ߵ������������X������������
		//CLongArray nArrayHandleXHJ;
		CLongArray nArrayHandleDC;
		CLongArray nArrayHandleLineType;
		CLongArray nArrayHandleLineCur;
		//this->GetGuiDaoDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
		this->GetOnLineDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC);
		int iNum_DC=0;
		iNum_DC =nArrayHandleDC.GetCount();
		//acutPrintf(_T("\n ����ϰ��� %d������"),iNum_DC);
		if(iNum_DC<2)
		{
			return FALSE;
		}
		//�жϵ����Ƿ�λ�ڹ�����
		int index=0;
		index=CBlkUtility::FindLongInArray(nArrayHandleDC,nHandleDC);
		//acutPrintf(_T("\n ����λ�ã�%d"),index);
		if(index==-1)
		{
			return FALSE;
		}
		//�����ƽ�ʱ,�ж������
		CLongArray nArrayHandleNext;
		long nHandleLineEnd=0;
		CStringArray sGuodaocha;
		CStringArray sGudao;
		//bool foword=true;
		if(iStart1==0)
		{
			//�����ƽ�
			this->FindNextLine(nHandle_dw,nHandleLineEnd,nArrayHandleNext,true);
			int iNum_Next=nArrayHandleNext.GetCount();
			//acutPrintf(_T("\n ���ҹ���ϰ��� %d�����"),iNum_Next);
			if(iNum_Next>=3)
			{
				for(int i=index+1;i<iNum_DC;i++)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//��������һ���Ƕ��ߵ���
					long nHandle_DXAno=0;
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						if(IsStart==0&&nHandle_fw!=0)
						{
							nHandleDc_DdMin=this->GetDuXianMinDaoCha(nHandle_DXDC,nHandle_DXAno);
							nHandleDc_DdMax=this->GetDuXianMaxDaoCha(nHandle_DXDC,nHandle_DXAno);
							return TRUE;
						}	
					}
				}
			}	
		}
		else
		{
			//�����ƽ�
			CLongArray nArrayHandleNext2;
			long nHandleLineEnd2=0;
			//bool foword2=false;
			this->FindNextLine(nHandle_dw,nHandleLineEnd2,nArrayHandleNext2,false);
			int iNum_Next2=nArrayHandleNext2.GetCount();
			//acutPrintf(_T("\n �������ϰ��� %d�����"),iNum_Next2);
			if(iNum_Next2>=3)
			{
				for(int i=index-1;i>=0;i--)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//long nHandle_DXMin=0;
					long nHandle_DXAno=0;
					//δ���ǽ�����ߴ��ڵ����
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{

						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						if(IsStart>0&&nHandle_fw!=0)
						{
							nHandleDc_DdMin=this->GetDuXianMinDaoCha(nHandle_DXDC,nHandle_DXAno);
							nHandleDc_DdMax=this->GetDuXianMaxDaoCha(nHandle_DXDC,nHandle_DXAno);
							return TRUE;
						}		
					}
				}
			}

		}	
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetdaidongDaoChas"));
	}
	return FALSE;
}
BOOL CZnSxt::GetDaidongDaochas( long nHandleDC,CString &str_daidongdaochas)
{
	try
	{
		//��ȡ�������ڶ�λ��
		//CADORecordset cSet(&m_AdoDb) ;
		if(nHandleDC==271611)
		{
			nHandleDC=nHandleDC;
		}
		if(nHandleDC==271605||nHandleDC==271599)
		{
			nHandleDC=nHandleDC;
		}
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		//long nHandle_fw=0;
		long nHandle_dw_0=0;
		long nHandle_fw_0=0;
		long iStart1=-1;
		CString str_daochas_temp;
		long nHandleDc_DdMin=0;
		long nHandleDc_DdMax=0;
		//ȡ����λ��
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dw_0);
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw_0);
		m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),iStart1) ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw_0) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		long nHandleLineTfw=0;
		long nHandleLineTdw=0;
		if(nLineType==1)
		{
			//Ϊ���ߣ���������
			if(iStart1==0)
			{
				FindNextLineByJYJ(nHandle_fw_0,nHandleLineTfw,true);
				if(nHandleLineTfw!=0)
				{
					//�����λ������
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineTfw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleLineTdw) ;
				}
				else
					return FALSE;
			}
			else
			{
				FindNextLineByJYJ(nHandle_fw_0,nHandleLineTfw,false);
				if(nHandleLineTfw!=0)
				{
					//�����λ������
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineTfw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleLineTdw) ;
				}
				else
				{
					return FALSE;
				}

			}
			
		}
		else
		{
			nHandleLineTdw=nHandle_fw_0;
		}
		
		if(nHandle_dw_0==0||nHandle_fw_0==0)
		{
			return FALSE;
		}
		//��ȡ��λ�ߵ������������X������������
		//CLongArray nArrayHandleXHJ;
		CLongArray nArrayHandleDC;
		CLongArray nArrayHandleLineType;
		CLongArray nArrayHandleLineCur;
		//this->GetGuiDaoDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
		this->GetOnLineDaoChaOrderByXAsc(nHandle_dw_0,nArrayHandleDC);
		int iNum_DC=0;
		iNum_DC =nArrayHandleDC.GetCount();
		//acutPrintf(_T("\n ����ϰ��� %d������"),iNum_DC);
		if(iNum_DC<2)
		{
			return FALSE;
		}
		//�жϵ����Ƿ�λ�ڹ�����
		int index=0;
		index=CBlkUtility::FindLongInArray(nArrayHandleDC,nHandleDC);
		//acutPrintf(_T("\n ����λ�ã�%d"),index);
		if(index==-1)
		{
			return FALSE;
		}
		//�����ƽ�ʱ,�ж������
		CLongArray nArrayHandleNext;
		long nHandleLineEnd=0;
		CStringArray sGuodaocha;
		CStringArray sGudao;
		//bool foword=true;
		if(iStart1==0)
		{
			//�����ƽ�
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd,nArrayHandleNext,true);
			int iNum_Next=nArrayHandleNext.GetCount();
			//acutPrintf(_T("\n ���ҹ���ϰ��� %d�����"),iNum_Next);
			if(iNum_Next>=3)
			{
				for(int i=index+1;i<iNum_DC;i++)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//��������һ���Ƕ��ߵ���
					long nHandle_DXAno=0;
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//����һ���������ڶ�λ�ߣ�
						long nHandle_dwAno;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXAno) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dwAno);
						if(IsStart==0&&nHandle_fw!=0&&(IsOntheHorizonline(nHandleLineTdw,nHandle_dwAno,true)==FALSE))
						{
							CString str_dc1;
							CString str_dc2;
							nHandleDc_DdMin=this->GetDuXianMinDaoCha(nHandle_DXDC,nHandle_DXAno);
							nHandleDc_DdMax=this->GetDuXianMaxDaoCha(nHandle_DXDC,nHandle_DXAno);
							GetEntName(nHandleDc_DdMin,str_dc1);
							GetEntName(nHandleDc_DdMax,str_dc2);
							str_daochas_temp+=__T("{")+str_dc1+_T("/")+str_dc2+_T("}")+_T(",");
							//return TRUE;
						}	
					}
				}
				str_daidongdaochas=str_daochas_temp;
				return TRUE;
			}	
		}
		else
		{
			//�����ƽ�
			CLongArray nArrayHandleNext2;
			long nHandleLineEnd2=0;
			//bool foword2=false;
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd2,nArrayHandleNext2,false);
			int iNum_Next2=nArrayHandleNext2.GetCount();
			//acutPrintf(_T("\n �������ϰ��� %d�����"),iNum_Next2);
			if(iNum_Next2>=3)
			{
				for(int i=index-1;i>=0;i--)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//long nHandle_DXMin=0;
					long nHandle_DXAno=0;
					//δ���ǽ�����ߴ��ڵ����
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{

						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//����һ���������ڶ�λ�ߣ�
						long nHandle_dwAno;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXAno) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dwAno);
						if(IsStart>0&&nHandle_fw!=0&&(IsOntheHorizonline(nHandleLineTdw,nHandle_dwAno,false)==FALSE))
						{
							CString str_dc1;
							CString str_dc2;
							nHandleDc_DdMin=this->GetDuXianMinDaoCha(nHandle_DXDC,nHandle_DXAno);
							nHandleDc_DdMax=this->GetDuXianMaxDaoCha(nHandle_DXDC,nHandle_DXAno);
							GetEntName(nHandleDc_DdMin,str_dc1);
							GetEntName(nHandleDc_DdMax,str_dc2);
							str_daochas_temp+=__T("{")+str_dc1+_T("/")+str_dc2+_T("}")+_T(",");
						}		
					}
				}
				str_daidongdaochas =str_daochas_temp;
				return TRUE;
			}

		}	
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetdaidongDaoChas"));
	}
	return FALSE;
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
void CZnSxt::GetGuiDaoInfoOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleXHJ, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType)
{
	//long t1=GetTickCount();
	nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		nArrayLineCurType.Add(nLineType) ;

		CLongArray nArrayHandleXHJTmp, nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_conline_right=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle, nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleXHJTmp) ;
		nArrayHandleXHJ.Append(nArrayHandleXHJTmp) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x asc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		for (int i=0; i<nArrayHandleDCTmp.GetCount(); i++)
		{
			UINT nHandleDc = nArrayHandleDCTmp.GetAt(i) ;
			strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
			long nHandleFwLine = 0 ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
			if (nHandleFwLine>0)
			{
				this->GetGuiDaoInfoOrderByXAsc(nHandleFwLine, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			}
		}
// 		long t2=GetTickCount();
// 		CString str;
// 		str.Format("GetGuiDaoInfoOrderByX time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
void CZnSxt::GetGuiDaoDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType)
{
	//long t1=GetTickCount();
	nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		nArrayLineCurType.Add(nLineType) ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x asc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		for (int i=0; i<nArrayHandleDCTmp.GetCount(); i++)
		{
			UINT nHandleDc = nArrayHandleDCTmp.GetAt(i) ;
			strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
			long nHandleFwLine = 0 ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
			if (nHandleFwLine>0)
			{
				//this->GetGuiDaoInfo(nHandleFwLine, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
				this->GetGuiDaoDaoChaOrderByXAsc(nHandleFwLine,nArrayHandleDC,nArrayLineDoneCur,nArrayLineCurType);
			}
		}
// 		long t2=GetTickCount();
// 		CString str;
// 		str.Format("GetGuiDaoDaoChaOrderByXAsc time:%d ms",t2-t1);
		//acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
void CZnSxt::GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC)
{
	long t1=GetTickCount();
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
// 		long nLineType = 0 ;
		//m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//nArrayLineCurType.Add(nLineType) ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x asc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		long t2=GetTickCount();
		CString str;
		str.Format("GetOnLineDaoChaOrderByXAsc time:%d ms",t2-t1);
		//acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
//�����ƽ�ʱ����ȡ�������źŻ��������ƽ�ʱ����ȡ����Ҳ��źŻ�
void CZnSxt::GetGuiDaoDuanXHJ(UINT nLineHandle, bool bToward,CLongArray& nArrayXHJ)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//long t1=GetTickCount();
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		CLongArray nArrayHandleXHJTmp;
		if(nLineHandle==0)
		{
			return;
		}
		if(bToward==false)
		{
			//��ȡ����Ҷ��źŻ�
			strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d ) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle) ;
		}
		else
		{
			//��ȡ�������źŻ�
			strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_right=%d ) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x desc"), nLineHandle) ;
		}
		//strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_conline_right=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle,nLineHandle ) ;
		/*		strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle ) ;*/
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleXHJTmp) ;
		nArrayXHJ.Append(nArrayHandleXHJTmp);
// 		long t2=GetTickCount();
// 		CString str;
// 		str.Format("GetGuiDaoDuanXHJ time:%d ms",t2-t1);
		//acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
// ��ѯ��������Ҳ���źŻ��������տ�Ļ���x������������
//conline_left=trueʱ��������ε��Ҳ���źŻ�
void CZnSxt::GetGuiDaoDuanXHJ(bool bforward,UINT nLineHandle, long& ZhiXHJ,long& XiangXHJ)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//long t1=GetTickCount();
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		CLongArray nArrayHandleXHJTmp;
		//ע�⣺nLineHandleһ������ȡ0��������ܲ鵽����Ľ��
		if(nLineHandle==0)
		{
			ZhiXHJ=0;
			XiangXHJ=0;
			return;
		}
		if(bforward==false)
		{
			//����Ҷ��źŻ�
			strSql.Format(_T("select * from sxt_ent where sxt_ent_conline_left=%d and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle,nLineHandle ) ;

		}
		else
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_conline_right=%d and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x desc"),nLineHandle,nLineHandle ) ;
		}
		/*		strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle ) ;*/
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleXHJTmp) ;
		for(int i=0;i<nArrayHandleXHJTmp.GetCount();i++)
		{
			if(bforward==true)
			{
				int toward=-1;
				if(GetBlockToward(nArrayHandleXHJTmp[i],toward)==TRUE)
				{
					if(toward==1)
					{
						ZhiXHJ=nArrayHandleXHJTmp[i];
						//�ҵ�ͬ���źŻ����ɽ���
						//break;
					}
					else if(toward==2||toward==0||toward==3)
					{
						XiangXHJ=nArrayHandleXHJTmp[i];
					}
				}

			}
			else
			{
				//�������źŻ�
				int toward=-1;
				if(GetBlockToward(nArrayHandleXHJTmp[i],toward)==TRUE)
				{
					if(toward==2)
					{
						ZhiXHJ=nArrayHandleXHJTmp[i]; 
						//break;
					}
					else if(toward==1||toward==0||toward==3)
					{
						XiangXHJ=nArrayHandleXHJTmp[i];
					}
				}
			}
		}
// 		long t2=GetTickCount();
// 		CString str;
// 		str.Format("GetGuiDaoDuanXHJzhiAndXiang time:%d ms",t2-t1);
		//acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
void CZnSxt::GetGuiDaoInfoOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleXHJ, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType)
{
	nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		nArrayLineCurType.Add(nLineType) ;

		CLongArray nArrayHandleXHJTmp, nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_conline_right=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x desc"), nLineHandle, nLineHandle, nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleXHJTmp) ;
		nArrayHandleXHJ.Append(nArrayHandleXHJTmp) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x desc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;

		for (int i=0; i<nArrayHandleDCTmp.GetCount(); i++)
		{
			UINT nHandleDc = nArrayHandleDCTmp.GetAt(i) ;
			strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
			long nHandleFwLine = 0 ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
			if (nHandleFwLine>0)
			{
				this->GetGuiDaoInfoOrderByXDesc(nHandleFwLine, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
void CZnSxt::GetGuiDaoChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType)
{
	nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		long t1=GetTickCount();
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		nArrayLineCurType.Add(nLineType) ;

		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x desc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		for (int i=0; i<nArrayHandleDCTmp.GetCount(); i++)
		{
			UINT nHandleDc = nArrayHandleDCTmp.GetAt(i) ;
			strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
			long nHandleFwLine = 0 ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
			if (nHandleFwLine>0)
			{
				this->GetGuiDaoChaOrderByXDesc(nHandleFwLine, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			}
		}
		long t2=GetTickCount();
		CString str;
		str.Format(_T("GetGuiDaoChaOrderByXDesc time:%d ms"),t2-t1);
		//acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!inGetGuiDaoChaOrderByXDesc"));
	}
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
void CZnSxt::GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		long t1=GetTickCount();
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x desc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		long t2=GetTickCount();
		CString str;
		str.Format(_T("GetGuiDaoChaOrderByXDesc time:%d ms"),t2-t1);
		//acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!inGetGuiDaoChaOrderByXDesc"));
	}
}


void CZnSxt::PrintGuoDaocha( vector<CString>PathDaocha )
{
	int count =PathDaocha.size();
	CStringArray JiLuDaocha;
	acutPrintf(_T("\n������"));
	for(int i=0;i<count;i++)
	{
		//��ȡ����
		CString temp=PathDaocha[i];
		CString Ptemp=temp;
		//TCHAR a=_T('(');
		TCHAR a='(';
		TCHAR b=')';
		TCHAR c='{';
		TCHAR d='}';
		TCHAR e='[';
		TCHAR f=']';
		//��tempȥ��_T("("),_T(")"),_T("{"),_T("}"),_T("["),_T("]")
		temp.Remove(a);
		temp.Remove(b);
		temp.Remove(c);
		temp.Remove(d);
		temp.Remove(e);
		temp.Remove(f);
		//acutPrintf(_T("\n temp=%s"),temp.GetString());
		CStringArray Daochas;
		CStringArray DaochasP;
		CBlkUtility::DivideString(temp,_T(","),Daochas);
		CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
		for(int k=0;k<Daochas.GetCount();k++)
		{
			if(CBlkUtility::FindStrInArray(JiLuDaocha,Daochas[k])==-1)
			{
				JiLuDaocha.Add(Daochas[k]);
				if(k!=Daochas.GetCount()-1)
				{
					if(DaochasP[k]!=_T(""))
					{
						acutPrintf(_T("%s \t"),DaochasP[k].GetString());
					}
				}
				else
				{
					if(DaochasP[k]!=_T(""))
					{
						acutPrintf(_T("%s \t"),DaochasP[k].GetString());
					}
				}
			}
		}
	}
}
void CZnSxt::TiquGuoDaocha( vector<CString>PathDaocha, vector<CString>& vec_Daocha)
{
	int count =PathDaocha.size();
	CStringArray JiLuDaocha;
	//acutPrintf(_T("\n������"));
	for(int i=0;i<count;i++)
	{
		//��ȡ����
		CString temp=PathDaocha[i];
		CString Ptemp=temp;
		//TCHAR a=_T('(');
		TCHAR a='(';
		TCHAR b=')';
		TCHAR c='{';
		TCHAR d='}';
		TCHAR e='[';
		TCHAR f=']';
		//��tempȥ��_T("("),_T(")"),_T("{"),_T("}"),_T("["),_T("]")
		temp.Remove(a);
		temp.Remove(b);
		temp.Remove(c);
		temp.Remove(d);
		temp.Remove(e);
		temp.Remove(f);
		//acutPrintf(_T("\n temp=%s"),temp.GetString());
		CStringArray Daochas;
		CStringArray DaochasP;
		CBlkUtility::DivideString(temp,_T(","),Daochas);
		CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
		for(int k=0;k<Daochas.GetCount();k++)
		{
			if(CBlkUtility::FindStrInArray(JiLuDaocha,Daochas[k])==-1)
			{
				JiLuDaocha.Add(Daochas[k]);
// 				if((i==count-1)&&k==Daochas.GetCount()-1)
// 				{
// 					if(DaochasP[k]!=_T(""))
// 					{
// 						//acutPrintf(_T("%s \t"),DaochasP[k].GetString());
// 						CString str_tem=DaochasP[k];
// 						vec_Daocha.push_back(str_tem);
// 					}
// 				}
// 				else
// 				{
// 					if(DaochasP[k]!=_T(""))
// 					{
// 						//acutPrintf(_T("%s \t"),DaochasP[k].GetString());
// 						//vec_Daocha.push_back(DaochasP[k]+T("\t"));
						CString str_tem=DaochasP[k];
						vec_Daocha.push_back(str_tem);
//					}
//				}
			}
		}
	}
}
//��Ҫ���ܣ���ȡһ������ϵĵ���ĵ�һ������
void CZnSxt::TiquFirstDaocha( CString strDaochas, CString &Daocha)
{
	//int count =PathDaocha.size();
	//��ȡ����
	//CString temp=strDaochas;
	CString Ptemp=strDaochas;
	//acutPrintf(_T("\n temp=%s"),temp.GetString());
	CStringArray DaochasP;
	CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
	//int i_cnt=DaochasP.GetCount();
	for(int k=0;k<DaochasP.GetCount();k++)
	{
		Daocha=DaochasP[k];
		if(Daocha!=_T(""))
		{
			break;
		}
	}
}
//��Ҫ���ܣ���ȡһ������ϵĵ���ĵ�һ������
void CZnSxt::TiquFirstDaocha2( CString strDaochas, CString &Daocha)
{
	//int count =PathDaocha.size();
	//��ȡ����
	//CString temp=strDaochas;
	CString Ptemp=strDaochas;
	//acutPrintf(_T("\n temp=%s"),temp.GetString());
	CStringArray DaochasP;
	CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
	//int i_cnt=DaochasP.GetCount();
	for(int k=0;k<DaochasP.GetCount();k++)
	{
		Daocha=DaochasP[k];
		if(Daocha!=_T(""))
		{
			break;
		}
	}
}
//��Ҫ���ܣ���String�а���"��"�ָ������ȡ��Ч��Ϣ����vector��
void CZnSxt::TranslateCStringtoVector( CString strDaocha, vector<CString> &vec_strData)
{
	CString Ptemp=strDaocha;
	CStringArray DaochasP;
	CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
	for(int k=0;k<DaochasP.GetCount();k++)
	{
		if(DaochasP.GetAt(k)!=_T(""))
			vec_strData.push_back(DaochasP.GetAt(k));
	}
}
//��Ҫ���ܣ���String�а���"��"�ָ������ȡ��Ч��Ϣ����vector��
void CZnSxt::TranslateVectorToCString(vector<CString> &vec_strData,CString &str_Data)
{
	CString Ptemp;
	for(int i=0;i<vec_strData.size();i++)
	{
		if(i!=vec_strData.size()-1&&vec_strData[i]!=_T(""))
			Ptemp+=vec_strData[i]+_T(",");
		else
			Ptemp+=vec_strData[i];
	}
	str_Data=Ptemp;
}
//��Ҫ���ܣ���vector<CString>�ж��ŷָ������ַ�������ȥ���ظ����ַ���
void CZnSxt::DeleteSameData( vector<CString>& vec_SourceData, vector<CString>& str_Vec_Dest_Data)
{
	int count =vec_SourceData.size();
	CStringArray JiLuDaocha;
	//acutPrintf(_T("\n������"));
	for(int i=0;i<count;i++)
	{
		//��ȡ����
		CString temp=vec_SourceData[i];
		CString Ptemp=temp;
		//TCHAR a=_T('(');
		TCHAR a='(';
		TCHAR b=')';
		TCHAR c='{';
		TCHAR d='}';
		TCHAR e='[';
		TCHAR f=']';
		temp.Remove(a);
		temp.Remove(b);
		temp.Remove(c);
		temp.Remove(d);
		temp.Remove(e);
		temp.Remove(f);
		CStringArray Daochas;
		CStringArray DaochasP;
		CBlkUtility::DivideString(temp,_T(","),Daochas);
		CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
		for(int k=0;k<Daochas.GetCount();k++)
		{
			if(CBlkUtility::FindStrInArray(JiLuDaocha,Daochas[k])==-1)
			{
				JiLuDaocha.Add(Daochas[k]);
				if((i==count-1)&&k==Daochas.GetCount()-1)
				{
					if(DaochasP[k]!=_T(""))
					{
						//acutPrintf(_T("%s \t"),DaochasP[k].GetString());
						CString str_tem=DaochasP[k];
						//vec_SourceData.push_back(str_tem);
						//strArray_Data.Add(str_tem);
						str_Vec_Dest_Data.push_back(str_tem);
					}
				}
				else
				{
					if(DaochasP[k]!=_T(""))
					{
						//acutPrintf(_T("%s \t"),DaochasP[k].GetString());
						//vec_Daocha.push_back(DaochasP[k]+T("\t"));
						CString str_tem=DaochasP[k];
						str_Vec_Dest_Data.push_back(str_tem);
						//strArray_Data.Add(str_tem);
						//vec_Daocha.push_back(str_tem);
					}
				}
			}
		}
	}
}
//��Ҫ���ܣ���vector�еĳ�վ�źŻ����г��͵����źŽ��кϲ����ϲ�ǰ����Vector�е����ź�λ���г��źŵ�ǰ��
void CZnSxt::CombandXHJData( vector<CString>& vec_SourceData, vector<CString>& str_Vec_Dest_Data)
{
	for(int i=0;i<vec_SourceData.size();i++)
	{
		CString temp =vec_SourceData[i];
		if(temp.Right(1)==_T("L"))
		{
			CString temp2=temp.Left(temp.GetLength()-1)+_T("D");
			int k=-1;
			if((k=CBlkUtility::FindStrInVector(str_Vec_Dest_Data,temp2.GetString()))!=-1)
			{
				str_Vec_Dest_Data[k]=temp.Left(temp.GetLength()-1);
			}
			else
			{
				str_Vec_Dest_Data.push_back(temp);
			}
		}
		else
		{
			str_Vec_Dest_Data.push_back(temp);
		}

	}
}
void CZnSxt::TiquEffectiveData( vector<CString>& vec_SourceData, CStringArray& strArray_Data)
{
	int count =vec_SourceData.size();
	CStringArray JiLuDaocha;
	//acutPrintf(_T("\n������"));
	for(int i=0;i<count;i++)
	{
		//��ȡ����
		CString temp=vec_SourceData[i];
		CString Ptemp=temp;
		//TCHAR a=_T('(');
		TCHAR a='(';
		TCHAR b=')';
		TCHAR c='{';
		TCHAR d='}';
		TCHAR e='[';
		TCHAR f=']';
		temp.Remove(a);
		temp.Remove(b);
		temp.Remove(c);
		temp.Remove(d);
		temp.Remove(e);
		temp.Remove(f);
		//temp.Remove(g);
		//acutPrintf(_T("\n temp=%s"),temp.GetString());
		CStringArray Daochas;
		CStringArray DaochasP;
		CBlkUtility::DivideString(temp,_T(","),Daochas);
		CBlkUtility::DivideString(Ptemp,_T(","),DaochasP);
		for(int k=0;k<Daochas.GetCount();k++)
		{
			if(CBlkUtility::FindStrInArray(JiLuDaocha,Daochas[k])==-1)
			{
				JiLuDaocha.Add(Daochas[k]);
				if((i==count-1)&&k==Daochas.GetCount()-1)
				{
					if(DaochasP[k]!=_T(""))
					{
						//acutPrintf(_T("%s \t"),DaochasP[k].GetString());
						CString str_tem=DaochasP[k];
					    //vec_SourceData.push_back(str_tem);
						strArray_Data.Add(str_tem);
					}
				}
				else
				{
					if(DaochasP[k]!=_T(""))
					{
						//acutPrintf(_T("%s \t"),DaochasP[k].GetString());
						//vec_Daocha.push_back(DaochasP[k]+T("\t"));
						CString str_tem=DaochasP[k];
						strArray_Data.Add(str_tem);
						//vec_Daocha.push_back(str_tem);
					}
				}
			}
		}
	}
}

void CZnSxt::GetEntName( long nHandle,CString &Entname )
{
	try
	{
		if(nHandle==0)
		{
			return;
		}
		CString name;
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandle!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name) ;
		}
		Entname =name;
		return ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetEntName"));
	}	
}
BOOL CZnSxt::GetBlockPosition_X( long nHandle,double &point_x )
{
	try
	{
		//CString name;
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandle!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),point_x) ;
			return TRUE;
		}
		//Entname =name;
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);       
		return FALSE;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetEntName"));
		return FALSE;
	}
	
}

void CZnSxt::FindNextDiaoCheXHJ( long nHandleDC_XHJ,CLongArray& nArrayHandleNext_DC_XHJ )
{
	//�������źŻ��Ƿ������ݿ⣬������źŻ����ڵĹ������
	try
	{
		CString name;
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		long nHandleLineRight=0;
		long nHandleLineLeft=0;
		if(nHandleDC_XHJ!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC_XHJ) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLineLeft) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLineRight) ;
		}
		//Entname =name;
		//��ȡ�����źŻ��ĳ���
		int toward=0;
		if(this->GetBlockToward(nHandleDC_XHJ,toward)==FALSE)
		{
			return;
		}
		long nHandleLineEnd=0;
		CLongArray nHandleNextLine;
		CStringArray Guodaocha;
		CStringArray Guodao;
		CLongArray  Zhi_XHJ;
		CLongArray Xiang_XHJ;
		bool forward;
		long lastdaocha=0;
		CLongArray ThisDaocha;
		if(toward==1)
		{
			//����
			forward=true;
			this->FindNextLine3(nHandleLineRight,nHandleLineEnd,Guodaocha,Guodao,lastdaocha,ThisDaocha,Zhi_XHJ,Xiang_XHJ,nHandleNextLine,forward);
			if(Zhi_XHJ.GetCount()==nHandleNextLine.GetCount())
			{
				nArrayHandleNext_DC_XHJ.Append(Zhi_XHJ);
			}
			else
			{
				for(int i=0;i<nHandleNextLine.GetCount();i++)
				{
					long zhi=0,xiang=0;
					this->GetGuiDaoDuanXHJ(true,nHandleNextLine[i],zhi,xiang);
					if(zhi!=0)
					{
						nArrayHandleNext_DC_XHJ.Add(zhi);
					}
				}

			}


		}
		else if(toward==2)
		{
			//����
			forward=false;
			this->FindNextLine3(nHandleLineLeft,nHandleLineEnd,Guodaocha,Guodao,lastdaocha,ThisDaocha,Zhi_XHJ,Xiang_XHJ,nHandleNextLine,forward);
			if(Zhi_XHJ.GetCount()==nHandleNextLine.GetCount())
			{
				nArrayHandleNext_DC_XHJ.Append(Zhi_XHJ);
			}
			else
			{
				for(int i=0;i<nHandleNextLine.GetCount();i++)
				{
					long zhi=0,xiang=0;
					this->GetGuiDaoDuanXHJ(true,nHandleNextLine[i],zhi,xiang);
					if(zhi!=0)
					{
						nArrayHandleNext_DC_XHJ.Add(zhi);
					}
				}

			}
		}

		return ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextDiaoCheXHJ"));
	}	


}


int CZnSxt::WriteDataToExcel(int iNum_Sheet,int iNum_Row,int iNum_Column,vector<CString>vec_str)
{
	long t1=GetTickCount();
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
			//return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString strXlsFileName=_T("C:\\��������\\ƽ��ͼ����������1.xls");
		pWorkBook = pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_Sheet;		
		pWorksheets  = pWorkBook->GetSheets() ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellContent;
		for(int i=0;i<vec_str.size();i++)
		{
			strCellContent+=vec_str[i];
		}
		CString strCellName ;
		CString strCellText ;
		CString strColName = CBlkUtility::GetExcelFieldName(iNum_Column) ;
		strCellName.Format(_T("%s%d"),strColName, iNum_Row) ;
		//acutPrintf(_T("\n��ʾ��Ϣ��%s: %s"),strCellName,strCellContent);
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
		acutPrintf(_T("\n%s"),strCellContent.GetString());
		//pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
		pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		pWorkBook->Save() ;
		///�رմ򿪵ĸ�����Դ
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		//m_pExcel->Release() ;
		m_pExcel = NULL ;
		long t2=GetTickCount();
		CString str;
		str.Format("WriteDatatoExcel time:%d ms",t2-t1);
		acutPrintf(_T("\n%s"),str);
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n��ʾ��Ϣ2��"));
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("δ֪�Ĵ���!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n��ʾ��Ϣ3��"));
	}	
	return 0;
}
// SheetsPtr CZnSxt::OpenExcel(CString name)
// {
// 	try
// 	{
// 		_ApplicationPtr m_pExcel = NULL ;
// 		WorkbooksPtr pWorkBooks = NULL ;
// 		_WorkbookPtr pWorkBook = NULL ;
// 		SheetsPtr pWorksheets = NULL ; 
// 		if (bFirst)
// 		{
// 			::AfxOleInit() ;
// 			bFirst = FALSE ;
// 		}
// 		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
// 		if(FAILED(hr))
// 		{
// 			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
// 			return NULL ;
// 		}
// 		pWorkBooks = m_pExcel->GetWorkbooks() ;
// 		CString strXlsFileName=name;
// 		pWorkBook = pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
// 		pWorksheets  = pWorkBook->GetSheets() ;
// 		return pWorksheets;
//     }
// 	catch(_com_error &e)
// 	{
// 		//iFound = 0 ;
// 		CString sBuff = CBlkUtility::GetErrorDescription(e);
// 		AfxMessageBox(sBuff);  
// 		return NULL;
// 	}
// 	catch(...)
// 	{	
// 		//iFound = 0 ;
// 		AfxMessageBox(_T("δ֪�Ĵ���!"));			
// 		return NULL;
// 	}	
// 	return NULL;
// }
// CZnSxt::CloseExcel()
// {
// // 	pWorkBook->Save() ;
// // 	long t4=GetTickCount();
// // 	str.Format(_T("save time:%d ms"),t4-t3);
// // 	acutPrintf(_T("\n %s"),str);
// // 	///�رմ򿪵ĸ�����Դ
// // 	pWorkBooks->Close( ) ;
// // 	pWorkBooks = NULL ;
// // 	m_pExcel->Quit() ;		
// // 	m_pExcel = NULL ;
// }
int CZnSxt::WriteDataToExcel(int iNum_Sheet,int iNum_Row,int iNum_Column,CString str_Data)
{
	long t1=GetTickCount();
	_ApplicationPtr m_pExcel = NULL ;
	 WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
			//return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString strXlsFileName=_T("C:\\��������\\ƽ��ͼ����������1.xls");
		pWorkBook = pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_Sheet;		
		pWorksheets  = pWorkBook->GetSheets() ;
		long t2=GetTickCount();
		CString str;
		str.Format(_T("GetSheets time:%d ms"),t2-t1);
		acutPrintf(_T("\n %s"),str);
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellContent=str_Data;
		CString strCellName ;
		CString strColName = CBlkUtility::GetExcelFieldName(iNum_Column) ;
		strCellName.Format(_T("%s%d"),strColName, iNum_Row) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
		pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
// 		pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
// 		//acutPrintf(_T("\n%s"),strCellContent.GetString());
// 		//pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
// 		pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		long t3=GetTickCount();
		str.Format(_T("write time %d ms"),t3-t2);
		acutPrintf(_T("\n %s"),str);
		pWorkBook->Save() ;
		long t4=GetTickCount();
		str.Format(_T("save time:%d ms"),t4-t3);
		acutPrintf(_T("\n %s"),str);
		///�رմ򿪵ĸ�����Դ
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		m_pExcel = NULL ;
		long t5=GetTickCount();
		str.Format(_T("quit time:%d"),t5-t4);
		acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n��ʾ��Ϣ2��"));
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("δ֪�Ĵ���!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n��ʾ��Ϣ3��"));
	}	
	return 0;
}
int CZnSxt::WriteDataToExcel(int iNum_Sheet,int iNum_Row,int iNum_Column,int iData)
{
	long t1=GetTickCount();
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
			//return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString strXlsFileName=_T("C:\\��������\\ƽ��ͼ����������1.xls");
		pWorkBook = pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_Sheet;		
		pWorksheets  = pWorkBook->GetSheets() ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		int iCellContent=iData;
		CString strCellName ;
		CString strColName = CBlkUtility::GetExcelFieldName(iNum_Column) ;
		strCellName.Format(_T("%s%d"),strColName, iNum_Row) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//acutPrintf(_T("\n%s"),strCellContent.GetString());
		//pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
		pRange->PutFormula((_variant_t)iData) ;//20101110 modified
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		pWorkBook->Save() ;
		///�رմ򿪵ĸ�����Դ
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		m_pExcel = NULL ;
		long t2=GetTickCount();
		CString str;
		str.Format(_T("writedatatoexcel time:%d ms"),t2-t1);
		acutPrintf(_T("\n %s"),str);
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n��ʾ��Ϣ2��"));
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("δ֪�Ĵ���!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n��ʾ��Ϣ3��"));
	}	
	return 0;
}
//�ݹ��ҵ���ĳ������߿�ʼ�ĺ�̵�����·���ù����Ϊ������ʼ�źŻ����ڵĹ����
/*******************����˵��:FindNextDCByLine*****************************************************************************************************************/
/**********���ܣ�ͨ��һ��������ʼ�źŻ����ڵĹ�������ص���������·����һ�������źŻ�*******************************************************************/
/**********���룺nHandleLine,������ʼ�źŻ����ڵĹ����,������ڵ�����·�ڲ�;*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ�������ȥ��������ϲ����ڽ�·�ڵ��źŻ���**********************************************/
/*****************Gudao���ɵ����ƣ�JieJinQuDuan���ӽ���������;bforward,Ѱ�ҵ�����·�������ɵ�����ʼ�źŻ��������********************************/
/**********�����nArrayHandleNextLine,��һ�������źŻ����ڵĹ������; nArrayDCZhong,���ص�����·�ն��źŻ�*****************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
int CZnSxt::FindNextDCByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CStringArray& JieJinQuDuan,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDCZhong,bool bfoward )
{

	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandleLine==0)
		{
			return -1;		
		}
		//���̹����
		CLongArray nArrayHandleNextLineTemp;
		long nHandleLineEnd=0;
		//long Lastdaocha=0;
        CLongArray nArrayHandleThisDaocha;
		CStringArray GuoDaocha;
		CStringArray GuoXHJ;
		//FindNextLine3(nHandleLine,nHandleLineEnd,GuoDaocha,Gudao,Lastdaocha,nArrayHandleThisDaocha,nArrayHandleNextLineTemp,bfoward);
	    //DiaoCheFindNextLineAndXHJ( nHandleLine,nHandleLineEnd,GuoDaocha,Gudao,LastDaocha,nArrayHandleThisDaocha,GuoXHJ,nArrayHandleNextLineTemp,bfoward);
		DiaoCheFindNextLineAndDaocha(nHandleLine,nHandleLineEnd,Gudao,LastDaocha,nArrayHandleThisDaocha,nArrayHandleNextLineTemp,bfoward);
		for(int i=0;i<nArrayHandleNextLineTemp.GetCount();i++)
		{
			//�����ƽ�ʱ��Ѱ�ҹ������źŻ��������ƽ�ʱѰ�ҹ���Ҷ��źŻ�
			bool iToward =bfoward;
			long zhixhj=0;
			long xiangxhj=0;
			long HandleDC=nArrayHandleThisDaocha[i];
			double pt_DC_x=0;
			double pt_xhj_x=0;
			CString strDCname;
			GetEntName(HandleDC,strDCname);
			GetGuiDaoDuanXHJ(iToward,nArrayHandleNextLineTemp[i],zhixhj,xiangxhj);
			if(bfoward==true)
			{
				if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
				{
					if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x>pt_DC_x)
						{
							zhixhj=zhixhj;
						}
						else
						{
							zhixhj=0;
						}

					}
					if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x>pt_DC_x)
						{
							xiangxhj=xiangxhj;
						}
						else
						{
							xiangxhj=0;
						}

					}
				}
				else
				{
						zhixhj=zhixhj;
						xiangxhj=xiangxhj;
				}
			}
			else
			{
				if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
				{
					if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x<pt_DC_x)
						{
							zhixhj=zhixhj;
						}
						else
						{
							zhixhj=0;
						}

					}
					if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x<pt_DC_x)
						{
							xiangxhj=xiangxhj;
						}
						else
						{
							xiangxhj=0;
						}

					}
				}
				else
				{
					zhixhj=zhixhj;
					xiangxhj=xiangxhj;
				}
			}
			if(zhixhj!=0)
			{
				
				CString DCname;
				GetEntName(zhixhj,DCname);
				CString EntName;
				GetEntBlkNameByHandle(zhixhj,EntName);
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
					DCname+=_T("D");
				}
				//DCname=_T("��")+DCname;
				//�������ظ����ն�
				if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
				{
					nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
					nArrayDCZhong.Add(DCname);
				}
				
			}
			else
			{
				//û���ҵ�ͬ��������źŻ�ʱ
				//1.�Ƿ��Ǿ�ͷ��
				//�Ҿ�ͷʽ��Ե��
				//long nHandleEnt=0;
				CString EntName;
				int type=200;
				if(bfoward==true)
				{
					//����Ҷ��źŻ�
					strSql.Format(_T("select * from sxt_ent where sxt_ent_conline_left=%d and sxt_ent_type=%d"),nArrayHandleNextLineTemp[i],type) ;
				}
				else
				{
					strSql.Format(_T("select * from sxt_ent where sxt_ent_conline_right=%d and sxt_ent_type=%d"),nArrayHandleNextLineTemp[i],type);
				}
				//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"),nHandleEnt);
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_block"),EntName);
				CString GuidaoName;
				m_mysys.GetXdata(nArrayHandleNextLineTemp[i],_T("GUIDAONAME"),GuidaoName);
				CString str_xhj_ent;
				GetEntBlkNameByHandle(xiangxhj,str_xhj_ent);
				//��ʾΪ��ͷ��
				if((xiangxhj!=0)&&(EntName.Left(6)==_T("JYJ_DT")))
				{
					CString DCname;
					GetEntName(xiangxhj,DCname);
					//DCname=_T("��")+DCname;
					//acutPrintf(_T("\n ��ͷ���źŻ���%s"),DCname);
					CString EntName;
					GetEntBlkNameByHandle(xiangxhj,EntName);
				    if(EntName.Left(6)==_T("XHJ_LF"))
					{
						//���źŻ�Ϊ��վ�źŻ�ʱ
						DCname+=_T("D");
					}
					if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
					{
						nArrayDCZhong.Add(DCname);
						nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);		
					}
				
				}
				else if(xiangxhj!=0&&str_xhj_ent.Left(6)==_T("XHJ_LJ"))
				{
					//��վ�źŻ���Ϊ�ն˰�ť
					CString DCname;
					GetEntName(xiangxhj,DCname);
					//DCname=_T("��")+DCname;
					//acutPrintf(_T("\n ��ͷ���źŻ���%s"),DCname);
					CString EntName;
					GetEntBlkNameByHandle(xiangxhj,EntName);
					//���źŻ�Ϊ��վ�źŻ�ʱ
					//DCname+=_T("D");	
					if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
					{
						nArrayDCZhong.Add(DCname);
						nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);		
					}
				}
				else if(xiangxhj!=0&&str_xhj_ent.Left(6)==_T("XHJ_LF"))
				{
					//�����źŻ�Ϊ�ն˰�ť��������Ӧ�ɵ�
					CString DCname;
					m_mysys.GetXdata(nArrayHandleNextLineTemp[i],_T("GUIDAONAME"),DCname);
					nArrayDCZhong.Add(DCname);
					nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
				}
				else
				{
					//Ϊ�ɵ���ʱ
					CString GuidaoName;
					m_mysys.GetXdata(nArrayHandleNextLineTemp[i],_T("GUIDAONAME"),GuidaoName);
					if(CBlkUtility::FindStrInArray(this->str_GuDaoName,GuidaoName)!=-1)
					{
						//Ϊ�ɵ�ʱ
						CString DCname;
						DCname=GuidaoName;
						if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
						{
							nArrayDCZhong.Add(DCname);
							//acutPrintf(_T("\n �ɵ��źŻ���%s"),DCname);
							nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
						}
					}
					else if(CBlkUtility::FindStrInArray(JieJinQuDuan,GuidaoName)!=-1)
					{
						//Ϊ�ӽ�����,�Ҳ���������
						if(xiangxhj!=0&&GuidaoName.Right(2)!=_T("DG"))
						{
							CString DCname;
							GetEntName(xiangxhj,DCname);
							//acutPrintf(_T("\n �ӽ������źŻ���%s"),DCname);
							//DCname=_T("��")+DCname;
							CString EntName;
							GetEntBlkNameByHandle(xiangxhj,EntName);
							if(EntName.Left(6)==_T("XHJ_LF"))
							{
								DCname+=_T("D");
							}
							if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
							{
								nArrayDCZhong.Add(DCname);
								nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
							}
							//���п��ܽӽ������е�����ʱ�ߵ���λ�ĵ����źŻ�ҲӦ�����ϡ�
						}
						else
						{
							FindNextDCByLine(nArrayHandleNextLineTemp[i],nArrayHandleThisDaocha[i],Gudao,JieJinQuDuan,nArrayHandleNextLine,nArrayDCZhong,bfoward);
						}

					}
					else
					{
						FindNextDCByLine(nArrayHandleNextLineTemp[i],nArrayHandleThisDaocha[i],Gudao,JieJinQuDuan,nArrayHandleNextLine,nArrayDCZhong,bfoward);

					}

				}

			}
		}
		return 1;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!inFindNextDCByLine"));
		return -2;
	}	
}
//�ݹ��ҵ���ĳ������߿�ʼ�ĺ���г��źŻ����ù����Ϊ�����ն˻�ʼ�����ڵĹ����
/*******************����˵��:FindNextLieCheXHJByLine*****************************************************************************************************************/
/**********���ܣ�ͨ��һ��������ʼ�źŻ����ڵĹ�������ص���������·����һ�������źŻ�*******************************************************************/
/**********���룺nHandleLine,������ʼ�źŻ����ڵĹ����,������ڵ�����·�ڲ�;*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ�������ȥ��������ϲ����ڽ�·�ڵ��źŻ���**********************************************/
/*****************Gudao���ɵ����ƣ�bforward,Ѱ���г��źŻ��ķ���**************************************************************************************************/
/**********�����nArrayHandleNextLine,��һ���г��źŻ����ڵĹ������; nArraylieche,�����г��źŻ�����*****************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
int CZnSxt::FindNextLieCheXHJByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& nArrayLieChe,bool bfoward )
{

	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandleLine==0)
		{
			return -1;		
		}
// 		//��ȡ��ʼ��������źŻ�
// 		bool iToward=!bfoward;
// 		CLongArray nArrayPassXHJ;
// 		CString Gd_name;
// 		m_mysys.GetXdata(nHandleLine,_T("GUIDAONAME"),Gd_name);
// 		GetGuiDaoDuanXHJ(nHandleLine,iToward,nArrayPassXHJ);
// 		for(int i=0;i<nArrayPassXHJ.GetCount();i++)
// 		{
// 			//�ж��Ƿ����г��źŻ�
// 			CString EntName;
// 			long nhandleXHJ=nArrayPassXHJ[i];
// 			CString blkname;
// 			GetEntName(nhandleXHJ,blkname);
// 			GetEntBlkNameByHandle(nhandleXHJ,EntName);
// 			if(EntName.Left(6)==_T("XHJ_LJ")||EntName.Left(6)==_T("XHJ_LF"))
// 			{
// 			    CString blkname;
// 				GetEntName(nhandleXHJ,blkname);
//                 nArrayLieChe.Add(blkname);
// 				return 1;
// 			}
// 		}
		bool iToward=bfoward;
		long zhixhj=0;
		long xiangxhj=0;
		GetGuiDaoDuanXHJ(iToward,nHandleLine,zhixhj,xiangxhj);
		if(xiangxhj!=0)
		{
			CString blkname;
			CString EntName;
			GetEntBlkNameByHandle(xiangxhj,EntName);
			if(EntName.Left(6)==_T("XHJ_LJ")||EntName.Left(6)==_T("XHJ_LF"))
			{
				CString blkname;
				GetEntName(xiangxhj,blkname);
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
					//��վ�źŻ������_T("L")
					blkname=blkname+_T("L");
				}
				nArrayLieChe.Add(blkname);
				nArrayHandleNextLine.Add(nHandleLine);
				return 1;
			}
		}
		//���̹����
		CLongArray nArrayHandleNextLineTemp;
		long nHandleLineEnd=0;
		//long Lastdaocha=0;
		CLongArray nArrayHandleThisDaocha;
		CStringArray GuoDaocha;
		CStringArray GuoXHJ;
		//FindNextLine3(nHandleLine,nHandleLineEnd,GuoDaocha,Gudao,Lastdaocha,nArrayHandleThisDaocha,nArrayHandleNextLineTemp,bfoward);
		DiaoCheFindNextLineAndXHJ( nHandleLine,nHandleLineEnd,GuoDaocha,Gudao,LastDaocha,nArrayHandleThisDaocha,GuoXHJ,nArrayHandleNextLineTemp,bfoward);
		//DiaoCheFindNextLineAndDaocha(nHandleLine,nHandleLineEnd,Gudao,LastDaocha,nArrayHandleThisDaocha,nArrayHandleNextLine,bfoward);
		for(int i=0;i<nArrayHandleNextLineTemp.GetCount();i++)
		{
			//�����ƽ�ʱ��Ѱ�ҹ������źŻ��������ƽ�ʱѰ�ҹ���Ҷ��źŻ�
			int iTag_Find=0;
			bool iToward=bfoward;
			long zhixhj=0;
			long xiangxhj=0;
			GetGuiDaoDuanXHJ(iToward,nHandleLine,zhixhj,xiangxhj);
		    if(xiangxhj!=0)
			{
				//�ж��Ƿ����г��źŻ�
				CString EntName;
				long nhandleXHJ=xiangxhj;
				CString blkname;
				GetEntName(nhandleXHJ,blkname);
				long HandleDC=nArrayHandleThisDaocha[i];
				double pt_DC_x=0;
				double pt_xhj_x=0;
				if(bfoward==true)
				{
					if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
					{
						if((GetBlockPosition_X(nhandleXHJ,pt_xhj_x)==TRUE))
						{
							if(pt_xhj_x<pt_DC_x)
							{
							   nhandleXHJ=0;
							}
						}
					}
				}
				else
				{
					if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
					{
						if((GetBlockPosition_X(nhandleXHJ,pt_xhj_x)==TRUE))
						{
							if(pt_xhj_x>pt_DC_x)
							{
								nhandleXHJ=0;
							}
						}
					}
				}
				GetEntBlkNameByHandle(nhandleXHJ,EntName);
				if(EntName.Left(6)==_T("XHJ_LJ")||EntName.Left(6)==_T("XHJ_LF"))
				{
					CString blkname;
					GetEntName(nhandleXHJ,blkname);
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						//��վ�źŻ������_T("L")
						blkname=blkname+_T("L");
					}
					nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
					nArrayLieChe.Add(blkname);
					iTag_Find =1;
					break;
				}
			}
			if(iTag_Find==0)
			{
				FindNextLieCheXHJByLine(nArrayHandleNextLineTemp.GetAt(i),nArrayHandleThisDaocha.GetAt(i),Gudao,nArrayHandleNextLine,nArrayLieChe,bfoward);
			}
		}
		return 1;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!inFindNextLieCheXHJByLine"));
		return -2;
	}	
}
//�ݹ��ҵ���ĳ������߿�ʼ�ĺ�̵�����·���ù����Ϊ������ʼ�źŻ����ڵĹ����
/*******************����˵��:FindNextDuiXiangDCByLine*****************************************************************************************************************/
/**********���ܣ�ͨ��һ��������ʼ�źŻ����ڵĹ�������ص���������·����һ�������źŻ�*******************************************************************/
/**********���룺nHandleLine,������ʼ�źŻ����ڵĹ����,������ڵ�����·�ڲ�;*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ�������ȥ��������ϲ����ڽ�·�ڵ��źŻ���**********************************************/
/*****************Gudao���ɵ����ƣ�JieJinQuDuan���ӽ���������;bforward,Ѱ�ҵ�����·�������ɵ�����ʼ�źŻ��������********************************/
/**********�����nArrayHandleNextLine,��һ�������źŻ����ڵĹ������; nArrayDCZhong,���ص�����·�ն��źŻ�*****************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
int CZnSxt::FindNextDuiXiangDCByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDiDuiDC,bool bfoward )
{

	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandleLine==0)
		{
			return -1;		
		}
		static int callcnt=0;
		//��ȡ��ʼ��������źŻ�
		bool iToward=bfoward;
		long zhixhj=0;
		long xiangxhj=0;
		double pt_DC_x=0;
		double pt_xhj_x=0;
		GetGuiDaoDuanXHJ(iToward,nHandleLine,zhixhj,xiangxhj);
		if(bfoward==true)
		{
			if(GetBlockPosition_X(LastDaocha,pt_DC_x)==TRUE)
			{
				if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x>pt_DC_x)
					{
						zhixhj=zhixhj;
					}
					else
					{
						zhixhj=0;
					}

				}
				if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x>pt_DC_x)
					{
						xiangxhj=xiangxhj;
					}
					else
					{
						xiangxhj=0;
					}

				}
			}
			else
			{
				zhixhj=zhixhj;
				xiangxhj=xiangxhj;
			}
		}
		else
		{
			if(GetBlockPosition_X(LastDaocha,pt_DC_x)==TRUE)
			{
				if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x<pt_DC_x)
					{
						zhixhj=zhixhj;
					}
					else
					{
						zhixhj=0;
					}

				}
				if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x<pt_DC_x)
					{
						xiangxhj=xiangxhj;
					}
					else
					{
						xiangxhj=0;
					}

				}
			}
			else
			{
				zhixhj=zhixhj;
				xiangxhj=xiangxhj;
			}
		}
		if(xiangxhj!=0)
		{
			nArrayHandleNextLine.Add(nHandleLine);
			CString DCname;
			GetEntName(xiangxhj,DCname);
			CString EntName;
			GetEntBlkNameByHandle(xiangxhj,EntName);
			if(EntName.Left(6)==_T("XHJ_LF"))
			{
				DCname+=_T("D");
			}
			return 1;
		}
// 		callcnt++;
		CString strGuidaoName;
		m_mysys.GetXdata(nHandleLine,_T("GUIDAONAME"),strGuidaoName);
// 		if(xiangxhj!=0)
// 		{
// 			//�õж��źŻ��ڵ�����·���Ѿ������������ٴμ��룬����ֱ�ӷ���
// 			return 1;
// 		}
		//���̹����
		CLongArray nArrayHandleNextLineTemp;
		long nHandleLineEnd=0;
		//long Lastdaocha=0;
		CLongArray nArrayHandleThisDaocha;
		CStringArray GuoDaocha;
		CStringArray GuoXHJ;
		DiaoCheFindNextLineAndXHJ( nHandleLine,nHandleLineEnd,GuoDaocha,Gudao,LastDaocha,nArrayHandleThisDaocha,GuoXHJ,nArrayHandleNextLineTemp,bfoward);
		int cnt=nArrayHandleNextLineTemp.GetCount();
		CString strGuidaoName2;
		for(int i=0;i<cnt;i++)
		{
			m_mysys.GetXdata(nArrayHandleNextLineTemp.GetAt(i),_T("GUIDAONAME"),strGuidaoName2);
		}
		for(int i=0;i<nArrayHandleNextLineTemp.GetCount();i++)
		{
			//�����ƽ�ʱ��Ѱ�ҹ������źŻ��������ƽ�ʱѰ�ҹ���Ҷ��źŻ�
			m_mysys.GetXdata(nArrayHandleNextLineTemp.GetAt(i),_T("GUIDAONAME"),strGuidaoName2);
			if(strGuidaoName2==_T("16DG"))
			{
				i=i;
			}
			if(i==0)
			{
				i=i;
			}
			if(i==1)
			{
				i=i;
			}
			if(i==2)
			{
				i=i;
			}
			if(i==3)
			{
				i=i;
			}
			iToward =bfoward;
			long zhixhj=0;
			long xiangxhj=0;
			long HandleDC=nArrayHandleThisDaocha[i];
			double pt_DC_x=0;
			double pt_xhj_x=0;
			CString strDCname;
			GetEntName(HandleDC,strDCname);
			GetGuiDaoDuanXHJ(iToward,nArrayHandleNextLineTemp[i],zhixhj,xiangxhj);
			if(bfoward==true)
			{
				if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
				{
					if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x>pt_DC_x)
						{
							zhixhj=zhixhj;
						}
						else
						{
							zhixhj=0;
						}

					}
					if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x>pt_DC_x)
						{
							xiangxhj=xiangxhj;
						}
						else
						{
							xiangxhj=0;
						}

					}
				}
				else
				{
					zhixhj=zhixhj;
					xiangxhj=xiangxhj;
				}
			}
			else
			{
				if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
				{
					if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x<pt_DC_x)
						{
							zhixhj=zhixhj;
						}
						else
						{
							zhixhj=0;
						}

					}
					if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x<pt_DC_x)
						{
							xiangxhj=xiangxhj;
						}
						else
						{
							xiangxhj=0;
						}

					}
				}
				else
				{
					zhixhj=zhixhj;
					xiangxhj=xiangxhj;
				}
			}
			if(xiangxhj!=0)
			{
				nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
				CString DCname;
				GetEntName(xiangxhj,DCname);
				CString EntName;
				GetEntBlkNameByHandle(xiangxhj,EntName);
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
					DCname+=_T("D");
				}
				//DCname+=_T("D");
				//DCname=_T("��")+DCname;
				nArrayDiDuiDC.Add(DCname);
			}
			else
			{			
				FindNextDuiXiangDCByLine(nArrayHandleNextLineTemp[i],nArrayHandleThisDaocha[i],Gudao,nArrayHandleNextLine,nArrayDiDuiDC,bfoward);
			}
		}
		return 1;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!inFindNextDCByLine"));
		return -2;
	}	
}
//�ݹ��ҵ���ĳ������߿�ʼ�ĺ�̵�����·���ù����Ϊ������ʼ�źŻ����ڵĹ����
/*******************����˵��:FindNextDuiXiangDCByLine2*****************************************************************************************************************/
/**********���ܣ�ͨ��һ��������ʼ�źŻ����ڵĹ�������ص���������·����һ�������źŻ�*******************************************************************/
/**********���룺nHandleLine,������ʼ�źŻ����ڵĹ����,������ڵ�����·�ڲ�;*******************************************************************************/
/******************LastDaocha,��¼��һ�η��ʵ������ʱ�߹��ĵ�������ȥ��������ϲ����ڽ�·�ڵ��źŻ���**********************************************/
/*****************Gudao���ɵ����ƣ�JieJinQuDuan���ӽ���������;bforward,Ѱ�ҵ�����·�������ɵ�����ʼ�źŻ��������********************************/
/**********�����nArrayHandleNextLine,��һ�������źŻ����ڵĹ������; nArrayDCZhong,���ص�����·�ն��źŻ�*****************************************/
/****************strArrayDaocha���ҵ���Ӧ�ĵ����źŻ��߹��ĵ���������Ϊ�ж�����********************************************************************************************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
int CZnSxt::FindNextDuiXiangDCByLine2( long nHandleLine,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDiDuiDC,CStringArray &strArrayDaocha,bool bfoward )
{

	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandleLine==0)
		{
			return -1;		
		}
		static int callcnt=0;
		//��ȡ��ʼ��������źŻ�
		bool iToward=bfoward;
		long zhixhj=0;
		long xiangxhj=0;
		double pt_DC_x=0;
		double pt_xhj_x=0;
		GetGuiDaoDuanXHJ(iToward,nHandleLine,zhixhj,xiangxhj);
		if(bfoward==true)
		{
			if(GetBlockPosition_X(LastDaocha,pt_DC_x)==TRUE)
			{
				if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x>pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
					{
						zhixhj=zhixhj;
					}
					else
					{
						zhixhj=0;
					}

				}
				if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x>pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
					{
						xiangxhj=xiangxhj;
					}
					else
					{
						xiangxhj=0;
					}

				}
			}
			else
			{
				zhixhj=zhixhj;
				xiangxhj=xiangxhj;
			}
		}
		else
		{
			if(GetBlockPosition_X(LastDaocha,pt_DC_x)==TRUE)
			{
				if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x<pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
					{
						zhixhj=zhixhj;
					}
					else
					{
						zhixhj=0;
					}

				}
				if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
				{
					if(pt_xhj_x<pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
					{
						xiangxhj=xiangxhj;
					}
					else
					{
						xiangxhj=0;
					}

				}
			}
			else
			{
				zhixhj=zhixhj;
				xiangxhj=xiangxhj;
			}
		}
		if(xiangxhj!=0)
		{
			nArrayHandleNextLine.Add(nHandleLine);
			CString DCname;
			GetEntName(xiangxhj,DCname);
			CString EntName;
			GetEntBlkNameByHandle(xiangxhj,EntName);
			if(EntName.Left(6)==_T("XHJ_LF"))
			{
				DCname+=_T("D");
			}
			nArrayDiDuiDC.Add(DCname);
			strArrayDaocha.Add(_T(""));
			return 1;
		}
		// 		callcnt++;
		CString strGuidaoName;
		m_mysys.GetXdata(nHandleLine,_T("GUIDAONAME"),strGuidaoName);
		//���̹����
		CLongArray nArrayHandleNextLineTemp;
		long nHandleLineEnd=0;
		//long Lastdaocha=0;
		CLongArray nArrayHandleThisDaocha;
		CStringArray GuoDaocha;
		CStringArray GuoXHJ;
		//DiaoCheFindNextLineAndXHJ( nHandleLine,nHandleLineEnd,GuoDaocha,Gudao,LastDaocha,nArrayHandleThisDaocha,GuoXHJ,nArrayHandleNextLineTemp,bfoward);
		DiaoCheFindNextLineAndDaocha(nHandleLine,nHandleLineEnd,Gudao,LastDaocha,nArrayHandleThisDaocha,nArrayHandleNextLineTemp,bfoward);
//		int cnt=nArrayHandleNextLineTemp.GetCount();
		CString strGuidaoName2;
// 		for(int i=0;i<cnt;i++)
// 		{
// 			m_mysys.GetXdata(nArrayHandleNextLineTemp.GetAt(i),_T("GUIDAONAME"),strGuidaoName2);
// 		}
		for(int i=0;i<nArrayHandleNextLineTemp.GetCount();i++)
		{
			//�����ƽ�ʱ��Ѱ�ҹ������źŻ��������ƽ�ʱѰ�ҹ���Ҷ��źŻ�
			m_mysys.GetXdata(nArrayHandleNextLineTemp.GetAt(i),_T("GUIDAONAME"),strGuidaoName2);
			iToward =bfoward;
			long zhixhj=0;
			long xiangxhj=0;
			long HandleDC=nArrayHandleThisDaocha[i];
			double pt_DC_x=0;
			double pt_xhj_x=0;
			CString strDCname;
			GetEntName(HandleDC,strDCname);
			GetGuiDaoDuanXHJ(iToward,nArrayHandleNextLineTemp[i],zhixhj,xiangxhj);
			if(bfoward==true)
			{
				if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
				{
					if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x>pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
						{
							zhixhj=zhixhj;
						}
						else
						{
							zhixhj=0;
						}

					}
					if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x>pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
						{
							xiangxhj=xiangxhj;
						}
						else
						{
							xiangxhj=0;
						}

					}
				}
				else
				{
					zhixhj=zhixhj;
					xiangxhj=xiangxhj;
				}
			}
			else
			{
				if(GetBlockPosition_X(HandleDC,pt_DC_x)==TRUE)
				{
					if((GetBlockPosition_X(zhixhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x<pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
						{
							zhixhj=zhixhj;
						}
						else
						{
							zhixhj=0;
						}

					}
					if((GetBlockPosition_X(xiangxhj,pt_xhj_x)==TRUE))
					{
						if(pt_xhj_x<pt_DC_x||fabs(pt_xhj_x-pt_DC_x)<=1.0E-6)
						{
							xiangxhj=xiangxhj;
						}
						else
						{
							xiangxhj=0;
						}

					}
				}
				else
				{
					zhixhj=zhixhj;
					xiangxhj=xiangxhj;
				}
			}
			if(xiangxhj!=0)
			{
				nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
				CString DCname;
				GetEntName(xiangxhj,DCname);
				CString EntName;
				GetEntBlkNameByHandle(xiangxhj,EntName);
				if(EntName.Left(6)==_T("XHJ_LF"))
				{
					DCname+=_T("D");
				}
				nArrayDiDuiDC.Add(DCname);
				//strArrayDaocha.Add(strArrayDaocha[i]);
				strArrayDaocha.Add(_T(""));
			}
			else
			{			
				FindNextDuiXiangDCByLine2(nArrayHandleNextLineTemp[i],nArrayHandleThisDaocha[i],Gudao,nArrayHandleNextLine,nArrayDiDuiDC,strArrayDaocha,bfoward);
			}
		}
		return 1;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!FindNextDuiXiangDCByLine2"));
		return -2;
	}	
}
/*******************����˵��:GetNextDCXHJByDCXHJ***********************************************************************************/
/**********���ܣ�ͨ��һ��������ʼ�źŻ������ص���������·����һ�������źŻ�******************************************************/
/**********���룺nHandleDC,������ʼ�źŻ���Gudao���ɵ����ƣ�JieJinQuDuan���ӽ���������*************************************/
/**********�����nArrayHandleNextLine,��һ�������źŻ����ڵĹ������; strArrayDC,��һ�������źŻ�������**********************/
/*****************nArrayHandleNextLine,Ϊ������·���ն˹����Ϊ�ն��źŻ���ǰһ���������*******************************************************************************************************************/
/**********************************2015-06-03 am 10:58 by luo********************************************************************/
/**********************************2015-07-10 am 9:48 modified by luo*************************************************************************************************/
int CZnSxt::GetNextDCXHJByDCXHJ( const long nHandleDC,CStringArray& Gudao,CStringArray& JieJinQuDuan,CLongArray& nArrayHandleNextLine,CStringArray& strArrayDC )
{
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		int iXHJ_Count=0;
		if(nHandleDC==0)
		{
			return -1;
		}
		//��ȡ�źŻ�����
		int iToward =-1;
		if(this->GetBlockToward(nHandleDC,iToward)!=TRUE)
		{
			return -1;
		}
		bool bfoward=false;
		if(iToward==1)
		{
			bfoward=true;
		}
		else if(iToward==2)
		{
			bfoward=false;
		}
		else
		{
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandleLine=0;
				long LastDaocha=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;
				FindNextDCByLine(nHandleLine,LastDaocha,Gudao,JieJinQuDuan,nArrayHandleNextLine,strArrayDC,false);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				nHandleLine=0;
				LastDaocha=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
				FindNextDCByLine(nHandleLine,LastDaocha,Gudao,JieJinQuDuan,nArrayHandleNextLine,strArrayDC,true);
				return 0;
		}
		long nHandleLine=0;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		if(bfoward==false)
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;
		}
		else
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
		}
		//���ݵ�ǰ�߻�ȡ������ϵ��źŻ�
		long LastDaocha=0;
		FindNextDCByLine(nHandleLine,LastDaocha,Gudao,JieJinQuDuan,nArrayHandleNextLine,strArrayDC,bfoward);
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetNextDCXHJByDCXHJ"));
		return -2;
	}		
}
int CZnSxt::GetXHJ_YouCe_GdLine(  long nHandleDC,long &nHandleLine,bool &foward)
{
	try
	{
	
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		int iXHJ_Count=0;
		if(nHandleDC==0)
		{
			nHandleLine=0;
			return -1;
		}
		//��ȡ�źŻ�����
		int iToward =-1;
		if(this->GetBlockToward(nHandleDC,iToward)!=TRUE)
		{
			return -1;
		}
		//bool bfoward=false;
		if(iToward==1)
		{
			foward=true;
		}
		else
		{
			foward=false;
		}
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		if(foward==false)
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;
		}
		else
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
		}
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetNextDCXHJByDCXHJ"));
		return -2;
	}		
}
int CZnSxt::GetXHJ_YouCe_GdLine(  long nHandleDC,long &nHandleLine,int  &iToward)
{
	try
	{

		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		int iXHJ_Count=0;
		if(nHandleDC==0)
		{
			nHandleLine=0;
			return -1;
		}
		//��ȡ�źŻ�����
	     iToward =-1;
		if(this->GetBlockToward(nHandleDC,iToward)!=TRUE)
		{
			return -1;
		}
		if(iToward==0||iToward==1)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
		}
		else if(iToward==3||iToward==2)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;	
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;	
		}
		else
		{
			return -2;
		}
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetNextDCXHJByDCXHJ"));
		return -2;
	}		
}
int CZnSxt::GetJYJ_YouCe_GdLine(  long nHandleJYJ,long &nHandleLine)
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		int iXHJ_Count=0;
		if(nHandleJYJ==0)
		{
			nHandleLine=0;
			return -1;
		}
//		long t1=GetTickCount();
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleJYJ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
		//���ݵ�ǰ�߻�ȡ������ϵ��źŻ�
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("GetJYJ_YouCe_GdLine time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetJYJ_YouCe_GdLine"));
		return -2;
	}		
}
int CZnSxt::GetXHJ_ZuoCe_GdLine(  long nHandleDC,long &nHandleLine,bool &foward)
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		int iXHJ_Count=0;
		if(nHandleDC==0)
		{
			nHandleLine=0;
			return -1;
		}
		//��ȡ�źŻ�����
		int iToward =-1;
		if(this->GetBlockToward(nHandleDC,iToward)!=TRUE)
		{
			return -1;
		}
		//bool bfoward=false;
		if(iToward==1)
		{
			foward=true;
		}
		else
		{
			foward=false;
		}
		//long nHandleLine=0;
/*		long t1=GetTickCount();*/
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		if(foward==false)
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
		}
		else
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;
		}
		//���ݵ�ǰ�߻�ȡ������ϵ��źŻ�
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("GetXHJZUOZEXIAN time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetXHJ_ZuoCe_GdLine"));
		return -2;
	}		
}
int CZnSxt::GetJYJ_ZuoCe_GdLine(  long nHandleJYJ,long &nHandleLine)
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		int iXHJ_Count=0;
		if(nHandleJYJ==0)
		{
			nHandleLine=0;
			return -1;
		}
		//long t1=GetTickCount();
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleJYJ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;
		//���ݵ�ǰ�߻�ȡ������ϵ��źŻ�
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("GetJYJ_ZuoCe_GdLine:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetXHJ_ZuoCe_GdLine"));
		return -2;
	}		
}
void CZnSxt::GetHandleByName( CString str_ent_name,long &nHandle )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		long nHandleTem=0;
	//	long t1=GetTickCount();
		if(str_ent_name!=_T(""))
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s'"), str_ent_name.GetString()) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"),nHandleTem);
		}
		//strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
		//long nHandleFwLine = 0 ;
		//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("GetHandleByName time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);
		nHandle =nHandleTem;
	
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetHandleByName"));
		//return -2;
	}		

}
void CZnSxt::Get_Block_Online( long nHandle_blk,long &nHandleLine )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		long nHandleLineTem=0;
	//	long t1=GetTickCount();
		if(nHandle_blk !=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_blk) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleLineTem);
		}
		nHandleLine = nHandleLineTem;
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("Get_Block_Online time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in Get_Block_Online"));
		//return -2;
	}		

}
void CZnSxt::GetHandleByEntBlkName( CString str_ent_blk_name,long &nHandle )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		long nHandleTem=0;
		//long t1=GetTickCount();
		if(str_ent_blk_name!=_T(""))
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_block='%s'"), str_ent_blk_name) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"),nHandleTem);
		}
		//strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
		//long nHandleFwLine = 0 ;
		//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
		nHandle =nHandleTem;
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("GetHandleByEntBlkName time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetHandleByEntBlkName"));
		//return -2;
	}		

}
void CZnSxt::GetEntBlkNameByHandle( const long nHandle,CString &str_ent_name)
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		//long nHandleTem=0;
//		long t1=GetTickCount();
		if(nHandle!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_block"),str_ent_name);
		}
		//strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
		//long nHandleFwLine = 0 ;
		//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
		//nHandle =nHandleTem;
// 		long t2;
// 		t2=GetTickCount();
// 		CString str32;
// 		str32.Format("GetEntBlkNameByHandle time:%d ms",t2-t1);
// 		acutPrintf(_T("\n %s"),str32);

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in GetEntBlkNameByHandle"));
		//return -2;
	}		

}

BOOL CZnSxt::FindNextLineByJYJ( long nHandleLineStart,long &nHandleLineNext,bool bForward )
{
	try
	{
		if(nHandleLineStart==0)
		{
			return FALSE;
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForward?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		//long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForward?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleLineNext);
		return TRUE;
	
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return FALSE;
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLineByJYJ"));
		//return -2;
		return FALSE;
	}		
	return FALSE;
}

BOOL CZnSxt::GetClosedPointToLine( AcGePoint3d& GivePt,AcGePoint3d & DestPt,long nHandleLine,int& upordown )
{
	AcDbPolyline *pLineEnt=NULL;
	AcDbObjectId LeftLineId;
	BOOL bRet=FALSE;
	m_mysys.GetObjectId(LeftLineId,nHandleLine);
	if(acdbOpenObject(pLineEnt,LeftLineId,AcDb::kForRead)==Acad::eOk)
	{
		pLineEnt->getClosestPointTo(GivePt,DestPt,Adesk::kFalse);
	}
	//����õ��ĵ������ϣ������Ƕ˵�
	if(CPolylineUtil::PointIsPolyVert(pLineEnt,CConvertUtil::ToPoint2d(DestPt),1e-7)!=true)
	{
		if(GivePt.y-DestPt.y>1e-6)
		{
			upordown=1;
		}
		else
		{
			upordown=0;
		}
		bRet =TRUE;

	}
	//acutPrintf(_T("\ndest x=%6f,y=%6f,z=%6f"),destP.x,destP.y,destP.z);
	pLineEnt->close();
	return bRet;
}

BOOL CZnSxt::FindCXGuiDao( const long nHandleCXJYJ,long & nHandleLineConLeft,long& nHandleLineCXLeft,long& nHandleLineConRight,long& nHandleLineCXRight ,CString& str_CX_GdName_left,CString &str_CX_GdName_Right)
{
	try
	{
		if(nHandleCXJYJ==0)
		{
			return FALSE;
		}
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
	   // acutPrintf(_T("\n�Ҿ����"));
		//��ѯ���޾�Ե�����ߺ�����
		int iFlag_find_left=-1;
		int iFlag_find_right=-1;
		nHandleLineConLeft=0;
		nHandleLineConRight=0;
		CString str_cx_guidao_left;
		CString str_cx_guidao_right;
		GetJYJ_ZuoCe_GdLine(nHandleCXJYJ,nHandleLineConLeft);
		GetJYJ_YouCe_GdLine(nHandleCXJYJ,nHandleLineConRight);
		CLongArray nArrayHandleJCB;
		long nHandleLeftJCB=0;
		long nHandleRightJCB=0;
		//��ȡ���о����
		GetSomeBlocks(_T("PMT_JC"),nArrayHandleJCB);
		//��ȡ�ź�¥����
		long nHandleXHL=0;
		double pt_xhl_x=0;
		GetHandleByEntBlkName(_T("PMT_XHL_1"),nHandleXHL);
		if(GetBlockPosition_X(nHandleXHL,pt_xhl_x)==FALSE)
		{
			return FALSE;
		}
		//��ȡ���޾�Ե������
		double pt_jyj_x=0;
		if(GetBlockPosition_X(nHandleCXJYJ,pt_jyj_x)==FALSE)
		{
			return false;
		}
		int shangxia=-1;
		if(pt_jyj_x-pt_xhl_x>1e-6)
		{
			//λ�������ʺ���
			shangxia=1;
		}
		else
		{
			//λ�������ʺ���
			shangxia=0;
		}
		//�����ߺϳ�һ����
		//�����߷���
		CLongArray nArrayHandleLeftDC;
		GetOnLineChaOrderByXDesc(nHandleLineConLeft,nArrayHandleLeftDC);
		CLongArray nArrayHandleRightDC;
		GetOnLineDaoChaOrderByXAsc(nHandleLineConRight,nArrayHandleRightDC);
		//CString DCname;
		int iCount_left=nArrayHandleLeftDC.GetCount();
		if(iCount_left!=0)
			str_cx_guidao_left+=_T("<");
		for(int i=0;i<iCount_left;i++)
		{
			CString str_DC;
			GetEntName(nArrayHandleLeftDC.GetAt(i),str_DC);
			if(i!=iCount_left-1)
			{
				str_cx_guidao_left+=str_DC;
			    str_cx_guidao_left+=_T("��");
			}
			else
			{
				str_cx_guidao_left+=str_DC;
				str_cx_guidao_left+=_T(">");
			}
		}
		//��ӹ������
		CString Gdname;
		m_mysys.GetXdata(nHandleLineConLeft,_T("GUIDAONAME"),Gdname);
		str_cx_guidao_left+=Gdname;
		int iCount_right=nArrayHandleRightDC.GetCount();
		if(iCount_right!=0)
			str_cx_guidao_right+=_T("<");
		for(int i=0;i<iCount_right;i++)
		{
			CString str_DC;
			GetEntName(nArrayHandleRightDC.GetAt(i),str_DC);
			if(i!=iCount_right-1)
			{
				str_cx_guidao_right+=str_DC;
				str_cx_guidao_right+=_T("��");
			}
			else
			{
				str_cx_guidao_right+=str_DC;
				str_cx_guidao_right+=_T(">");
			}
		}
		//��ӹ������
		CString Gdname2;
		m_mysys.GetXdata(nHandleLineConRight,_T("GUIDAONAME"),Gdname2);
		str_cx_guidao_right+=Gdname2;
		str_CX_GdName_left=str_cx_guidao_left;
		str_CX_GdName_Right=str_cx_guidao_right;
		for(int i=0;i<nArrayHandleLeftDC.GetCount();i++)
		{
			//��ȡ����λ���Լ�iStart
			long nHandleDC=nArrayHandleLeftDC.GetAt(i);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			//��λ�߶�Ӧ�ĵ���
			long nHandleLineFw=0;
			int IsStart=-1;
			m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleLineFw);
			if(nHandleLineFw!=0)
			{
				if(IsStart==0)
				{
					//�Ƿ�Ϊ����
					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineFw) ;
					long nLineType = 0 ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
					if(nLineType==1)
					{
						//���ߣ���Ҫ����һ�κ��
						long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
						//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
						int nHandleEndConJyj = nHandleRightJyj2; //��ĩ�����ӵľ�Ե��handle
						long nHandleLineFw2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
						CString strFieldName = _T("sxt_ent_conline_right");
						//��ȡ�����
						m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleLineFw2);
						//�ϳ�һ���ߣ��ҳ���Ӧ�ľ���꣬���ж��Ƿ���
						//AcDbPolyline *pPoly= new AcDbPolyline(,,);
						//GetClosedPointToLine()
						//CLongArray 
						//GetSomeBlocks(_T("PMT_JC"))
						for(int i=0;i<nArrayHandleJCB.GetCount();i++)
						{
							AcGePoint3d GivePt;
							AcGePoint3d DestPt1,DestPt2,DestPt3,DestPt4;
							int up1=-1;
						    int up2=-1;
							int up3=-1;
							int up4=-1;
							CString JCBName;
							GetEntName(nArrayHandleJCB.GetAt(i),JCBName);
							if(JCBName==_T("581"))
							{
								JCBName=JCBName;
							}
							GetBlockPosition(nArrayHandleJCB.GetAt(i),GivePt);
							if(GetClosedPointToLine(GivePt,DestPt1,nHandleLineConLeft,up1)==TRUE||GetClosedPointToLine(GivePt,DestPt2,nHandleLineConRight,up2)==TRUE)
							{
								if(GetClosedPointToLine(GivePt,DestPt3,nHandleLineFw,up3)==TRUE||GetClosedPointToLine(GivePt,DestPt4,nHandleLineFw2,up4)==TRUE)
								{
									if((up1+up3)==1||(up2+up3)==1||(up1+up4)==1||(up2+up4)==1)
									{
										nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
										CString JCBName;
										GetEntName(nHandleLeftJCB,JCBName);
										//acutPrintf(_T("\n���� �ҵ� JCB:%s"),JCBName);
										//��Ҫ�ж��Ƿ��ޣ���ȡ���޾�Ե������ֵ
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if(i_dist_JCB-i_dist_JYJ<4)
										{
											acutPrintf("\n���� ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_right);
											nHandleLineCXLeft=nHandleLineFw;
											iFlag_find_left=1;
											break;
										}
										
									}
								}

							}
						}

					}
					else
					{
						//�Ƕ���
						for(int i=0;i<nArrayHandleJCB.GetCount();i++)
						{
							AcGePoint3d GivePt;
							AcGePoint3d DestPt1,DestPt2,DestPt3,DestPt4;
							int up1=-1;
							int up2=-1;
							int up3=-1;
							//bool up4=false;
							nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
							CString JCBName;
							GetEntName(nHandleLeftJCB,JCBName);
							if(JCBName==_T("581"))
							{
								JCBName=JCBName;
							}
							GetBlockPosition(nArrayHandleJCB.GetAt(i),GivePt);
							if(GetClosedPointToLine(GivePt,DestPt1,nHandleLineConLeft,up1)==TRUE||GetClosedPointToLine(GivePt,DestPt2,nHandleLineConRight,up2)==TRUE)
							{
								if(GetClosedPointToLine(GivePt,DestPt3,nHandleLineFw,up3)==TRUE)
								{
									if((up1+up3)==1||(up2+up3)==1)
									{
										nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
										CString JCBName;
										GetEntName(nHandleLeftJCB,JCBName);
										//acutPrintf(_T("\n �Ƕ��� �ҵ�JCB:%s"),JCBName);
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if(i_dist_JCB-i_dist_JYJ<4)
										{
											acutPrintf("\n�Ƕ���  ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_right);
											nHandleLineCXLeft=nHandleLineFw;
											iFlag_find_left=1;
											break;
										}
										
									}
								}

							}
						}

					}
				}
				else
				{
					continue;
				}
				

			}
			else
			{
				acutPrintf(_T("\n����λ�߲����ڣ�����¼�����"));
			}
			if(iFlag_find_left==1)
			{
				break;
			}
		}
		//����Ϊĳһ������ķ�λ��
		strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineConLeft) ;
		//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ��Ӧ�Ķ�λ��
		long nHandleLineDw=0;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleLineDw);
		if(nHandleLineDw!=0)
		{
			for(int i=0;i<nArrayHandleJCB.GetCount();i++)
			{
				AcGePoint3d GivePt;
				AcGePoint3d DestPt1,DestPt2,DestPt3,DestPt4;
				int up1=-1;
				int up2=-1;
				int up3=-1;
				//bool up4=false;
				nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
				CString JCBName;
				GetEntName(nHandleLeftJCB,JCBName);
				if(JCBName==_T("581"))
				{
					JCBName=JCBName;
				}
				GetBlockPosition(nArrayHandleJCB.GetAt(i),GivePt);
				if(GetClosedPointToLine(GivePt,DestPt1,nHandleLineConLeft,up1)==TRUE||GetClosedPointToLine(GivePt,DestPt2,nHandleLineConRight,up2)==TRUE)
				{
					if(GetClosedPointToLine(GivePt,DestPt3,nHandleLineDw,up3)==TRUE)
					{
						if((up1+up3)==1||(up2+up3)==1)
						{
							nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
							CString JCBName;
							GetEntName(nHandleLeftJCB,JCBName);
							//acutPrintf(_T("\n �Ƕ��� �ҵ�JCB:%s"),JCBName);
							CString strDist;
							m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
							strDist.Remove(_T('('));
							strDist.Remove(_T(')'));
							int i_dist_JYJ=StrToIntA(strDist);
							int i_dist_JCB=StrToIntA(JCBName);
							if(i_dist_JCB-i_dist_JYJ<4)
							{
								acutPrintf("\n��λ��  ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_right);
								iFlag_find_left=1;
								nHandleLineCXLeft=nHandleLineDw;
								break;
							}
						
						}
					}

				}
			//nHandleLineCXLeft =nHandleLineDw;
				if(iFlag_find_left==1)
				{
					break;
				}
		}
	}
		//�����߷���
		for(int i=0;i<nArrayHandleRightDC.GetCount();i++)
		{
			//��ȡ����λ���Լ�iStart
			long nHandleDC=nArrayHandleRightDC.GetAt(i);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			//��λ�߶�Ӧ�ĵ���
			long nHandleLineFw=0;
			int IsStart=-1;
			m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleLineFw);
			if(nHandleLineFw!=0)
			{
				if(IsStart>0)
				{
					//�Ƿ�Ϊ����
					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineFw) ;
					long nLineType = 0 ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
					if(nLineType==1)
					{
						//���ߣ���Ҫ����һ�κ��
						long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
						//���������Ѱ��,�����Ҷ˾�Ե�ڡ�
						int nHandleEndConJyj =nHandleLeftJyj2; //��ĩ�����ӵľ�Ե��handle
						long nHandleLineFw2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
						CString strFieldName = _T("sxt_ent_conline_left");
						//��ȡ�����
						m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleLineFw2);
						//�ϳ�һ����,�ҳ���Ӧ�ľ����,���ж��Ƿ���
						//AcDbPolyline *pPoly= new AcDbPolyline(,,);
						//GetClosedPointToLine()
						//CLongArray 
						//GetSomeBlocks(_T("PMT_JC"))
						for(int i=0;i<nArrayHandleJCB.GetCount();i++)
						{
							AcGePoint3d GivePt;
							AcGePoint3d DestPt1,DestPt2,DestPt3,DestPt4;
							int up1=-1;
							int up2=-1;
							int up3=-1;
							int up4=-1;
							CString JCBName;
							GetEntName(nArrayHandleJCB.GetAt(i),JCBName);
							if(JCBName==_T("581"))
							{
								JCBName=JCBName;
							}
							GetBlockPosition(nArrayHandleJCB.GetAt(i),GivePt);
							if(GetClosedPointToLine(GivePt,DestPt1,nHandleLineConLeft,up1)==TRUE||GetClosedPointToLine(GivePt,DestPt2,nHandleLineConRight,up2)==TRUE)
							{
								if(GetClosedPointToLine(GivePt,DestPt3,nHandleLineFw,up3)==TRUE||GetClosedPointToLine(GivePt,DestPt4,nHandleLineFw2,up4)==TRUE)
								{
									if((up1+up3)==1||(up2+up3)==1||(up1+up4)==1||(up2+up4)==1)
									{
										nHandleRightJCB=nArrayHandleJCB.GetAt(i);
										CString JCBName;
										GetEntName(nHandleRightJCB,JCBName);
										//acutPrintf(_T("\n���� �ҵ� JCB:%s"),JCBName);
										//��Ҫ�ж��Ƿ���,��ȡ���޾�Ե������ֵ
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if(i_dist_JCB-i_dist_JYJ<4&&shangxia==1)
										{
											acutPrintf("\n ���� ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
											nHandleLineCXRight=nHandleLineFw;
											iFlag_find_right=1;
											break;
										}
										else if(i_dist_JYJ-i_dist_JCB<4&&shangxia==0)
										{
											acutPrintf("\n ���� ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
											nHandleLineCXRight=nHandleLineFw;
											iFlag_find_right=1;
											break;
										}
									
									}
								}

							}
						}

					}
					else
					{
						//�Ƕ���
						for(int i=0;i<nArrayHandleJCB.GetCount();i++)
						{
							AcGePoint3d GivePt;
							AcGePoint3d DestPt1,DestPt2,DestPt3,DestPt4;
							int up1=-1;
							int up2=-1;
							int up3=-1;
							//bool up4=false;
							nHandleRightJCB=nArrayHandleJCB.GetAt(i);
							CString JCBName;
							GetEntName(nHandleRightJCB,JCBName);
							if(JCBName==_T("581"))
							{
								JCBName=JCBName;
							}
							GetBlockPosition(nArrayHandleJCB.GetAt(i),GivePt);
							if(GetClosedPointToLine(GivePt,DestPt1,nHandleLineConLeft,up1)==TRUE||GetClosedPointToLine(GivePt,DestPt2,nHandleLineConRight,up2)==TRUE)
							{
								if(GetClosedPointToLine(GivePt,DestPt3,nHandleLineFw,up3)==TRUE)
								{
									if((up1+up3)==1||(up2+up3)==1)
									{
										nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
										CString JCBName;
										GetEntName(nHandleLeftJCB,JCBName);
										//acutPrintf(_T("\n �Ƕ��� �ҵ�JCB:%s"),JCBName);
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if((i_dist_JCB-i_dist_JYJ<4)&&shangxia==1)
										{
											acutPrintf("\n �Ƕ��� ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
											nHandleLineCXRight=nHandleLineFw;
										    iFlag_find_right=1;
											break;
										}
										else if((i_dist_JYJ-i_dist_JCB<4)&&shangxia==0)
										{
											acutPrintf("\n �Ƕ��� ���ޣ�%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
											nHandleLineCXRight=nHandleLineFw;
											iFlag_find_right=1;
											break;
										}
									}
								}

							}
						}

					}
				}
				else
				{
					continue;
				}


			}
			else
			{
				acutPrintf(_T("\n����λ�߲�����,����¼�����"));
			}
			if(iFlag_find_right==1)
			{
				break;
			}
		}
		//����Ϊĳһ������ķ�λ��
		strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineConRight) ;
		//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ��Ӧ�Ķ�λ��
		long nHandleLineDw2=0;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleLineDw2);
		if(nHandleLineDw2!=0)
		{
			for(int i=0;i<nArrayHandleJCB.GetCount();i++)
			{
				AcGePoint3d GivePt;
				AcGePoint3d DestPt1,DestPt2,DestPt3,DestPt4;
				int up1=-1;
				int up2=-1;
				int up3=-1;
				//bool up4=false;
				nHandleRightJCB=nArrayHandleJCB.GetAt(i);
				CString JCBName;
				GetEntName(nHandleRightJCB,JCBName);
				if(JCBName==_T("581"))
				{
					JCBName=JCBName;
				}
				GetBlockPosition(nArrayHandleJCB.GetAt(i),GivePt);
				if(GetClosedPointToLine(GivePt,DestPt1,nHandleLineConLeft,up1)==TRUE||GetClosedPointToLine(GivePt,DestPt2,nHandleLineConRight,up2)==TRUE)
				{
					if(GetClosedPointToLine(GivePt,DestPt3,nHandleLineDw2,up3)==TRUE)
					{
						if((up1+up3)==1||(up2+up3)==1)
						{
							nHandleLeftJCB=nArrayHandleJCB.GetAt(i);
							CString JCBName;
							GetEntName(nHandleLeftJCB,JCBName);
							//acutPrintf(_T("\n �Ƕ��� �ҵ�JCB:%s"),JCBName);
							CString strDist;
							m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
							strDist.Remove(_T('('));
							strDist.Remove(_T(')'));
							int i_dist_JYJ=StrToIntA(strDist);
							int i_dist_JCB=StrToIntA(JCBName);
							if(i_dist_JCB-i_dist_JYJ<4)
							{
								acutPrintf("\n ��λ�� ���ޣ�%d %d  %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
								nHandleLineCXRight=nHandleLineDw2;
								iFlag_find_right=1;
								break;
							}
							
						}
					}

				}
			}

		}
			
		//nHandleLineCXRight =nHandleLineDw2;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return FALSE;
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindCXGuiDao"));
		//return -2;
		return FALSE;
	}		
	return FALSE;

}
/*****************************����     FindALLChaoXianGuiDao*****************************************************************************************************/
/*****���ܣ������ҳ�ƽ��ͼ�����еĳ��޾�Ե��������س���ʱ�߹������ξ����Ӧ��ռ�õĹ����������***************************************************************************************************************/
/*****���룺�ޣ����ź��豸ƽ�沼��ͼ����ȫ���������ݿ�*************************************************************************************************************/
/*****������߹������ξ��CLongArray& nArrayHandleLine���Լ�Ӧ��ռ�õĹ����������CStringArray& strArrayGuiDao***************************************************************************************************************************************************/
/*****ע�⣺���ǳ��޾�Եʱ�ֱ����˳��޾�Ե��Ϊ�ڶ�λ�ߺͷ�λ�ߵ�������ֱ����˳��޾�Ե�����ߵ������ڵĶ�λ���������ڵĶ�λ������ĳ��޾�Ե��****/
/************���ж��Ƿ���ʱ���ֱ𽫳��޾�Ե��λ�úͶ�Ӧ�ľ��������˱Ƚϡ����ֱ����˳��޾�Ե�ڷֱ�λ�������ʺ�������ʺ�Ĳ�ͬ���************************************************************************************************************/
/************ʹ�øú����ǣ���ƽ��ͼ�����λ����һ��Ҫ�󣬼�Ҫ���Ӧ��������λ�ھ�Ե�ڶ�λ�ߺͷ�λ��Χ�����������ڲ���һ��Ϊ�������·�,һ��λ�ھ������Ϸ��ұ�����������************************************************************/
/********************* ע�� 2015-06-17 pm-17:04 by luo*******************************************************************************************************************/
int CZnSxt::FindALLChaoXianGuiDao( CLongArray& nArrayHandleLine,CStringArray& strArrayGuiDao )
{
	//��ȡ���޾�Ե��
	CLongArray nArrayHandleJCB;
	CLongArray nArrayHandleCXJYJ;
	//���ڴ洢���޾�Եʱ�ߵĹ������
	//CLongArray nArrayHandleLine;
	//���ڴ洢���޾�ԵʱӦ��ռ�õ�����
	//CStringArray strArrayGuiDao;
	GetSomeBlocks(_T("PMT_JC"),nArrayHandleJCB);
	GetSomeBlocks(_T("JYJ_CX"),nArrayHandleCXJYJ);
	int iRet=0;
	for(int i=0;i<nArrayHandleCXJYJ.GetCount();i++)
	{
		long nHandleJYJ=0;
		nHandleJYJ=nArrayHandleCXJYJ.GetAt(i);	
		//long nHandleJYJ=nArrayHandleCXJYJ.GetAt(0);
		//�����ߺ�����
		long nHandleLeft=0;
		long nHandleRight=0;
		long nHandleCXLeft=0;
		long nHandleCXRight=0;
		bool foward=false;
		CString strGdLeft;
		CString strGdRight;
		FindCXGuiDao(nHandleJYJ,nHandleLeft,nHandleCXLeft,nHandleRight,nHandleCXRight,strGdLeft,strGdRight);
		if(nHandleCXLeft!=0)
		{
			nArrayHandleLine.Add(nHandleCXLeft);
			strArrayGuiDao.Add(strGdRight);
			iRet++;
		}
		if(nHandleCXRight!=0)
		{
			nArrayHandleLine.Add(nHandleCXRight);
			strArrayGuiDao.Add(strGdLeft);
			iRet++;
		}
	}
// 	acutPrintf(_T("\n ���޾�Ե�����"));
// 	for(int i=0;i<nArrayHandleLine.GetCount();i++)
// 	{
// 		CString name;
// 		m_mysys.GetXdata(nArrayHandleLine.GetAt(i),_T("GUIDAONAME"),name);
// 		acutPrintf(_T("\n����%s,��ռ��%s"),name,strArrayGuiDao.GetAt(i));
// 	}
	return iRet;

}

void CZnSxt::LSBExcel()
{
	ads_point base_pt={0,0,0};
	ads_getpoint(NULL,_T("\nѡ����ʼ��"),base_pt);
	AcGePoint3d base_pt3d;
	CUserSystem my_User;
	my_User.ConvertPtAdsTo3D(base_pt,base_pt3d);
	AcGePoint2d base_pt2d=CConvertUtil::ToPoint2d(base_pt3d);
	AcGePoint2d start_rectangle1_pt2d1=base_pt2d;
	start_rectangle1_pt2d1.x-=15;
	start_rectangle1_pt2d1.y+=9;
	float xls_len=400;
	AcGePoint2d start_rectangle1_pt2d2=base_pt2d;
	start_rectangle1_pt2d2.x+=(15+xls_len);
	start_rectangle1_pt2d2.y+=(9-287);
	CPolylineUtil::AddRectangle(start_rectangle1_pt2d1,start_rectangle1_pt2d2,0.5);
	AcGePoint2d start_rectangle2_pt2d1=base_pt2d;
	start_rectangle2_pt2d1.x-=40;
	start_rectangle2_pt2d1.y+=14;
	AcGePoint2d start_rectangle2_pt2d2=base_pt2d;
	start_rectangle2_pt2d2.x+=(20+xls_len);
	start_rectangle2_pt2d2.y+=(14-297);
	CPolylineUtil::AddRectangle(start_rectangle2_pt2d1,start_rectangle2_pt2d2,0);
	//����Բ��
	AcGePoint2d ptCenter_2d=start_rectangle1_pt2d1;
	ptCenter_2d.x-=12.41;
	ptCenter_2d.y-=227;
	CPolylineUtil::AddPolyCircle(ptCenter_2d,7.4,0);
	AcGePoint3d sign_point1=CConvertUtil::ToPoint3d(start_rectangle1_pt2d2);
	sign_point1.x+=-180+63;
	sign_point1.y+=17;
	CString tuname;
	CString tuhao;
	CString riqi;
	CString program_name1;
	CString program_name2;
	CString strFilePath ;
	strFilePath.Format(_T("%s\\support\\sign\\my_sign.dwg"), g_strBasePath) ;
	TCHAR *file_sign =(TCHAR *)strFilePath.GetBuffer(strFilePath.GetLength());
	//TCHAR*file_sign =_T("C:\\��������\\my_sign.dwg");
	TCHAR *bsign=_T("my_sign");
	AcDbObjectId blockid = CBlockUtil::InsertDwgBlockDef(file_sign,bsign,1);
	//CBlockUtil::InsertBlockRef(blockid,ptStart);
	AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blockid,sign_point1);
	AcDbBlockReference *pBlkRef =NULL;
	if(acdbOpenObject(pBlkRef,blkRefId,AcDb::kForWrite)==Acad::eOk)
	{
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUNAME_001"),tuname.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUHAO_001"),tuhao.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("RIQI_001"),riqi.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME_001"),program_name1.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME2_001"),program_name2.GetString());
		pBlkRef->close();
	}
	//�Կ鷽ʽ����
	AcGePoint3d jungong_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
	CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\xiugai.dwg"), g_strBasePath) ;
	TCHAR *filename =(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	//TCHAR*filename =_T("C:\\��������\\xiugai.dwg");
	TCHAR *bname =_T("jungong");
	AcDbObjectId blockid1 = CBlockUtil::CopyBlockDefFromOtherDwg(filename,bname);
	//CBlockUtil::InsertBlockRef(blockid,pt_Start);
	CBlockUtil::InsertBlockRef(blockid1,jungong_point);
	AcGePoint3d filename_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
	filename_point.y-=287;
	TCHAR *bname2 =_T("filename");
	AcDbObjectId blockid2 = CBlockUtil::CopyBlockDefFromOtherDwg(filename,bname2);
	//CBlockUtil::InsertBlockRef(blockid,pt_Start);
	CBlockUtil::InsertBlockRef(blockid2,filename_point);

}
//�������ɵ�excel��ĳ���ת����CAD
void CZnSxt::XlsToLSB( AcGePoint3d start_pt_3d,float f_length_xls )
{
	//ads_point base_pt={0,0,0};
	//ads_getpoint(NULL,_T("\nѡ����ʼ��"),base_pt);
	AcGePoint3d base_pt3d=start_pt_3d;
	//CUserSystem my_User;
	//my_User.ConvertPtAdsTo3D(base_pt,base_pt3d);
	AcGePoint2d base_pt2d=CConvertUtil::ToPoint2d(base_pt3d);
	AcGePoint2d start_rectangle1_pt2d1=base_pt2d;
	start_rectangle1_pt2d1.x-=15;
	start_rectangle1_pt2d1.y+=9;
	float xls_len=f_length_xls;
	AcGePoint2d start_rectangle1_pt2d2=base_pt2d;
	start_rectangle1_pt2d2.x+=(15+xls_len);
	start_rectangle1_pt2d2.y+=(9-287);
	CPolylineUtil::AddRectangle(start_rectangle1_pt2d1,start_rectangle1_pt2d2,0.5);
	AcGePoint2d start_rectangle2_pt2d1=base_pt2d;
	start_rectangle2_pt2d1.x-=40;
	start_rectangle2_pt2d1.y+=14;
	AcGePoint2d start_rectangle2_pt2d2=base_pt2d;
	start_rectangle2_pt2d2.x+=(20+xls_len);
	start_rectangle2_pt2d2.y+=(14-297);
	CPolylineUtil::AddRectangle(start_rectangle2_pt2d1,start_rectangle2_pt2d2,0);
	//����Բ��
	AcGePoint2d ptCenter_2d=start_rectangle1_pt2d1;
	ptCenter_2d.x-=12.41;
	ptCenter_2d.y-=227;
	CPolylineUtil::AddPolyCircle(ptCenter_2d,7.4,0);
	AcGePoint3d sign_point1=CConvertUtil::ToPoint3d(start_rectangle1_pt2d2);
	sign_point1.x+=-180+63;
	sign_point1.y+=17;
	CString tuname;
	CString tuhao;
	CString riqi;
	CString program_name1;
	CString program_name2;
	//TCHAR *file_sign =(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	CString strFilePath ;
	strFilePath.Format(_T("%s\\support\\sign\\my_sign.dwg"), g_strBasePath) ;
	TCHAR *file_sign =(TCHAR *)strFilePath.GetBuffer(strFilePath.GetLength());
	TCHAR *bsign=_T("my_sign");
	AcDbObjectId blockid = CBlockUtil::InsertDwgBlockDef(file_sign,bsign,1);
	//CBlockUtil::InsertBlockRef(blockid,ptStart);
	AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blockid,sign_point1);
	AcDbBlockReference *pBlkRef =NULL;
	if(acdbOpenObject(pBlkRef,blkRefId,AcDb::kForWrite)==Acad::eOk)
	{
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUNAME"),tuname.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUHAO"),tuhao.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("RIQI"),riqi.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME"),program_name1.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME2"),program_name2.GetString());
		pBlkRef->close();
	}
	//�Կ鷽ʽ����
	AcGePoint3d jungong_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
   CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\xiugai.dwg"), g_strBasePath) ;
	TCHAR *filename =(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	/*TCHAR*filename =_T("C:\\��������\\xiugai.dwg");*/
	TCHAR *bname =_T("jungong");
	AcDbObjectId blockid1 = CBlockUtil::CopyBlockDefFromOtherDwg(filename,bname);
	//CBlockUtil::InsertBlockRef(blockid,pt_Start);
	CBlockUtil::InsertBlockRef(blockid1,jungong_point);
	AcGePoint3d filename_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
	filename_point.y-=287;
	TCHAR *bname2 =_T("filename");
	AcDbObjectId blockid2 = CBlockUtil::CopyBlockDefFromOtherDwg(filename,bname2);
	//CBlockUtil::InsertBlockRef(blockid,pt_Start);
	CBlockUtil::InsertBlockRef(blockid2,filename_point);
}

BOOL CZnSxt::FindNextZhiGuidao( long nHandleLineStart,long &nHandleNextLine,bool bForward )
{
	try
	{
		if(nHandleLineStart==0)
		{
			nHandleNextLine=0;
			return FALSE;
		}
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql;
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForward?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		//long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForward?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(nHandleNextLine==0)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextZhiGuiDao"));
	}	
	return FALSE;
}

void CZnSxt::FindFirstDiffString( vector<CString>&vec_str1,vector<CString>&vec_str2,CString &str_diff1,CString&str_diff2 )
{
	int iCnt_vec1=vec_str1.size();
	int iCnt_vec2=vec_str2.size();
	int iCnt=(iCnt_vec1<=iCnt_vec2?iCnt_vec1:iCnt_vec2);
	for(int i=0;i<iCnt;i++)
	{
		if(vec_str1[i]!=vec_str2[i])
		{
			str_diff1=vec_str1[i];
			str_diff2=vec_str2[i];
			break;
		}
	}
	return;

}

// int CZnSxt::GetYanHouDCXHJ( bool bSXYanHou,CStringArray& str_nArrayXHJ,CLongArray& nArrayHandleXHJ )
// {
// 	//��ȡ���е����źŻ�
//     CLongArray nArrayHandleAllDC;
// 	GetSomeBlocks(_T("XHJ_DC"),nArrayHandleAllDC);
// 	double pt_x=0;
// 	//��ȡ�ź�¥����
// 	long nHandleXHL=0;
// 	GetHandleByName(_T("PMT_XHL_1"),nHandleXHL);
// 	if(nHandleXHL==0)
// 	{
// 		return 0;
// 	}
// 	GetBlockPosition_X(nHandleXHL,pt_x);
// 	if(bSXYanHou==true)
// 	{
// 		for(int i=0;i<nArrayHandleAllDC.GetCount();i++)
// 		{
// 			long nhandleDC=nArrayHandleAllDC.GetAt(i);
// 			if(nhandleDC!=0)
// 			{
// 				double pt_DC_x=0;
// 				GetBlockPosition_X(nhandleDC,pt_DC_x);
// 				if(pt_DC_x-pt_x>(1e-6))
// 				{
// 					nArrayHandleXHJ.Add(nhandleDC);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for(int i=0;i<nArrayHandleAllDC.GetCount();i++)
// 		{
// 			long nhandleDC=nArrayHandleAllDC.GetAt(i);
// 			if(nhandleDC!=0)
// 			{
// 				double pt_DC_x=0;
// 				GetBlockPosition_X(nhandleDC,pt_DC_x);
// 				if(pt_DC_x-pt_x<1e-6)
// 				{
// 					nArrayHandleXHJ.Add(nhandleDC);
// 				}
// 			}
// 		}
// 	}
// 
// }

// void CZnSxt::SortStringArray( CStringArray &StrArray)
// {
// 	TCHAR del_char=_T('D');
// 	for(int i=0;i<StrArray.GetCount();i++)
// 	{
// 		//acutPrintf(_T("\n"));
// 		CString str_first=StrArray.GetAt(i);
// 		str_first.Remove(del_char);
// 		int int_fisrt=StrToIntA(str_first);
// 		acutPrintf(_T("int_first=%d"),int_fisrt);
// 		for(int j=i+1;j<StrArray.GetCount();j++)
// 		{
// 			CString strNext=StrArray.GetAt(j);
// 			strNext.Remove(del_char);
// 			int int_next=StrToIntA(strNext);
// 			acutPrintf(_T("int_next=%d"),int_next);
// 			if(int_fisrt>int_next)
// 			{
// 				CString temp=StrArray[j];
// 				StrArray.SetAt(j,StrArray.GetAt(i));
// 				StrArray.SetAt(i,temp);
// 				//acutPrintf("")
// 			}
// 
// 		}
// 	}
// 
// }


// void CZnSxt::LsbTest()
// {
// 	CLongArray nArrayHandle_LJ_XHJ;
// 	int iRet =GetAll_LJ_XHJ(nArrayHandle_LJ_XHJ);
// 	CLongArray nArrayHandle_LF_XHJ;
// 	CString name_xhj=_T("XHJ_LF");
// 	int iRet2 =GetSomeBlocks(name_xhj,nArrayHandle_LF_XHJ);
// 	acutPrintf(_T("\n iRet =%d"),iRet);
// 	acutPrintf(_T("\n ��վ�źŻ�����=%d"),iRet2);
// 	//��ȡ�ɵ����ڹ��
// 	CLongArray nAarryHandleGuDaoS;
// 	CLongArray nAarryHandleGuDaoX;
// 	for(int i=0;i<iRet2;i++)
// 	{
// 		CString entname;
// 		long nHandleLine =nArrayHandle_LF_XHJ[i];
// 		long nHandleLineTe;
// 		CString strSql;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
// 		if(entname.Left(1)==_T("S"))
// 		{
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLineTe) ;
// 			nAarryHandleGuDaoS.Add(nHandleLineTe);
// 		}
// 		else if(entname.Left(1)==_T("X"))
// 		{
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLineTe) ;
// 			nAarryHandleGuDaoX.Add(nHandleLineTe);
// 		}
// 	}
// 	
// 	CADORecordset cSet(&m_AdoDb) ;
// 	CString strSql ;
// 	int length =nArrayHandle_LJ_XHJ.GetCount();
// 	if(length<1)
// 	{
// 		acutPrintf(_T("\n �źŻ�����Ϊ%d"),length);
// 		return;
// 	}
// 	//��ӡ��վ�źŻ�����
// 	for(int i=0;i<length;i++)
// 	{
// 		CString entname;
// 		long nHandleLine =nArrayHandle_LJ_XHJ[i];
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
// 	    acutPrintf(_T("\n ��վ�źŻ�����Ϊ%s"),entname.GetString());
// 		//��ѯ��վ�źŻ��Ҳ�ֱ��
//         strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
// 		long nHandleLinetemp;
// 		 //Ѱ�Һ�һ�����
// 		CLongArray nArrayHandle_GD_Next;
// 		//��ȡ�źŻ��Ҳ���
// 		if(entname.Left(1)==_T("X"))
// 		{
//           m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
// 		  this->FindNextLine(nHandleLinetemp,nArrayHandle_GD_Next,true);
// 		}
// 		else if(entname.Left(1)==_T("S"))
// 		{
//           m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
// 		  this->FindNextLine(nHandleLinetemp,nArrayHandle_GD_Next,false);
// 		}
// 		//��ֱ�߱�ɫ��ʾ
// 		m_mysys.FlashEnt(nHandleLinetemp, 500, 50) ;
// 		m_mysys.SetEntColorIndex(nHandleLinetemp,1);
// 		//this->FindNextLine(nHandleLi)
// 	    //Ѱ�Һ�һ�����
// 		//CLongArray nArrayHandle_GD_Next;
// 		//this->FindNextLine(nHandleLinetemp,nArrayHandle_GD_Next,true);
// 		if(nArrayHandle_GD_Next.GetCount()<1)
// 		{
// 			acutPrintf(_T("\n û�к�̹��"));
// 		}
// 		else
// 		{
// 			for(int i=0;i<nArrayHandle_GD_Next.GetCount();i++)
// 			{
// 				//m_mysys.FlashEnt(nHandleLinetemp, 500, 50) ;
// 				long nHandleLinenext =nArrayHandle_GD_Next[i];
// 				m_mysys.SetEntColorIndex(nHandleLinenext,3);
// 				CLongArray nexthandle;
// 				if(entname.Left(1)==_T("S"))
// 				{
// 					this->FindNextLine(nHandleLinenext,nexthandle,false);
// 				}
// 				else
// 				{
// 					this->FindNextLine(nHandleLinenext,nexthandle,true);
// 				}
// 				for(int i=0;i<nexthandle.GetCount();i++)
// 				{
// 					//m_mysys.FlashEnt(nHandleLinetemp, 500, 50) ;
// 					long nHandleLine3 =nexthandle[i];
// 					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLine3) ;
// 					long nLineType = -1 ;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 					acutPrintf(_T("\n ������������=%d"),nLineType);
// 					m_mysys.SetEntColorIndex(nHandleLine3,4);
// 			   }
// 		}
// 	}
// 	//��ӡ��վ�źŻ�����
// 	for(int i=0;i<iRet2;i++)
// 	{
// 		CString entname;
// 		long nHandleLine =nArrayHandle_LF_XHJ[i];
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
// 		acutPrintf(_T("\n ��վ�źŻ�����Ϊ%s"),entname.GetString());
// 		//��ѯ��վ�źŻ��Ҳ�ֱ��
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
// 		long nHandleLinetemp;
// 		//��ȡ�źŻ��Ҳ���,��Ϊ�ɵ�
// 		if(entname.Left(1)==_T("S"))
// 		{
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
// 		}
// 		else if(entname.Left(1)==_T("X"))
// 		{
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
// 		}
// 		//��ֱ�߱�ɫ��ʾ
// 		m_mysys.FlashEnt(nHandleLinetemp, 500, 50) ;
// 		//m_mysys.SetEntColorIndex(nHandleLinetemp,2);
// 	}
// 
// }
// 	
