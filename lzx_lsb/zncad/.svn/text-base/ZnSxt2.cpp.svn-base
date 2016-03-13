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

//输入起点轨道和终点轨道，寻找进路，找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
void CZnSxt::GetPathGuiDao4( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &DaochaNum,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	CStringArray nArrayDaoChaNum;
	nArrayDaoChaNum.Copy(DaochaNum);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//压入对应的道岔定反位
		acutPrintf(_T("\n 搜索结束"));
		for(int i=0;i<sVectorPath.size();i++)
		{
			//输出搜索到的结果
			acutPrintf(_T("\t第%d 轨道区段名为%s"),i,sVectorPath[i].GetString());
		}
		sVectorPath.pop_back();
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//打印经过道岔定反位
		acutPrintf(_T("\n 经过道岔："));
		for(int k=0;k<sDaocha.size();k++)
		{
			if(sDaocha[k].GetString()!=_T(""))
			{
				acutPrintf(_T("\t：%s"),sDaocha[k].GetString());
			}
		}
		for(int k=0;k<nArrayDaoChaNum.GetCount();k++)
		{	
			if(nArrayDaoChaNum[i].GetString()!=_T(""))
			{
				acutPrintf(_T("\t：%s"),nArrayDaoChaNum[k].GetString());
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		//存储走过的道岔
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
				//打印后继
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
//输入起点轨道和终点轨道,寻找进路,找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
void CZnSxt::GetPathGuiDao2( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//压入对应的道岔定反位
		acutPrintf(_T("\n 搜索结束"));
		for(int i=0;i<sVectorPath.size();i++)
		{
			//输出搜索到的结果
			acutPrintf(_T("\t第%d 轨道区段名为%s"),i,sVectorPath[i].GetString());
		}
		sVectorPath.pop_back();
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//打印经过道岔定反位
		//acutPrintf(_T("\n 经过道岔："));
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		//存储走过的道岔
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
				//打印后继
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
//输入起点轨道和终点轨道,寻找进路,找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
//Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
//vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
void CZnSxt::GetPathGuiDaoAndXHJ( long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//获取终止轨道信号机
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
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
				CString temp=_T("由")+sVectorPath[0];
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));

			}
			else
			{
				CString temp=_T("至")+Gd_name;
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				//写迎面进路
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		//存储走过的道岔
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
				//打印后继
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
//输入起点轨道和终点轨道,寻找进路,找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
//Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
//vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
//特别用于列车进路获取联锁表信息,增加起始信号机和终止信号机参数
void CZnSxt::GetLieChePathGuiDaoAndXHJ( long nHandleStartXHJ,long nHandleEndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//获取终止轨道信号机
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
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
				CString temp=_T("由")+sVectorPath[0];
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				oper_excel.WriteDatatoExcel(1,num,17,_T("BS"));

			}
			else
			{
				CString temp=_T("至")+Gd_name;
				oper_excel.WriteDatatoExcel(1,num,5,temp);
				//写迎面进路
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayGuiDaoCX;
		//存储走过的道岔
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
				//打印后继
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
//输入起点轨道和终点轨道,寻找进路,找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
//Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
//vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
//特别用于列车进路获取联锁表信息，增加起始信号机和终止信号机参数
void CZnSxt::GetLieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//获取终止轨道信号机
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");
		}
		sXHJ.push_back(Name_PassXHJ);
		oper_excel.WriteDatatoExcel(1,num,4,(num-3));
		vector<CString>tempGuidao;
		//PrintGuoDaocha(sVectorPath);
		TiquGuoDaocha(sVectorPath,tempGuidao);
		oper_excel.WriteDatatoExcel(1,num,14,tempGuidao);
		sVectorPath.pop_back();
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
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
			CString temp=_T("至")+Gd_name;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			//写迎面进路
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
			oper_excel.WriteDatatoExcel(1,num,16,Gd_name);

		}
		else if(CBlkUtility::FindStrInArray(GuDao,Start_Gd_name)!=-1)
		{
			CString temp=_T("由")+Start_Gd_name;
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
		
		//将当前轨道加入已访问
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
		//存储走过的道岔
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
				//打印后继
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


//输入起点轨道和终点轨道，寻找进路，找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
//Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
//vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
//特别用于列车进路获取联锁表信息，增加起始信号机和终止信号机参数
//增加参数iJL_Cnt，用于统计变更进路个数，没有变更进路时，为1，有变更进路时大于1
/***********************************函数功能描述**********************************************************/
/*********函数名称：GetTongGuoPathGuiDaoAndXHJ******************************************************************************************************/
/*********函数功能：用于寻找通过进路，要求通过进路均为正线通过**************************************************************/
/*********输入：CString& Start_Gd_name,起始轨道名称；long nHandleStartXHJ,起始信号机句柄；long nHandleEndXHJ,终止信号机句柄；long& nHandleLineFirst,起始轨道句柄；******/
/****************long nHandleLineStart,本次调用时起始轨道句柄；long nHandleLineEnd,终止轨道句柄；***********************************************/
/****************long LastDaocha,上次调用时走过的道岔句柄；vector<CString>&sVectorPath,走过的轨道区段；vector<CString>&sDaocha,走过的道岔；*****/
/****************vector<CString>& sXHJ,走过的信号机；CStringArray &GuDao,平面图的股道名称；CLongArray&nArrayHandleLine,走过的轨道句柄；*/
/****************vector<CString> &vec_strGuiDaoCX,超限绝缘轨道，int &iJL_Cnt,改进路包括的进路条数；bool bForword，进路搜索方向************/
/*********输出：**************************************************************/
/************************注释：2015-07-16 pm-15:48 by luo*******************************************************/
void CZnSxt::GetJieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
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
		//获取终止轨道信号机
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
		//对于发车进路来说，要去掉股道
		//CString str_start_guidao;
		
// 		vector<CString> tempGuiDao2;
// 		//取出站信号机所在轨道区段右侧轨道区段
// 		for(int i_k=0;i_k<tempGuidao.size();i_k++)
// 		{
// 			//去掉股道之前的轨道区段
// 			CString str_temp=tempGuidao[i_k];
// 			if(str_temp.)
// 		}
		//去掉其中的分支道岔
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
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
			CString temp=_T("至")+Gd_nameTemp;
	 	    oper_excel.WriteDatatoExcel(1,num,5,temp);
		 	//写迎面进路
		 	oper_excel.WriteDatatoExcel(1,num,15,Gd_nameTemp);
		 	oper_excel.WriteDatatoExcel(1,num,16,Gd_nameTemp);
	
		}
		else if((i_tem=CBlkUtility::FindStrInArray(GuDao,Start_Gd_name))!=-1)
		{
			CString Gd_nameTemp=this->str_GuDaoName.GetAt(i_tem);
		 	CString temp=_T("由")+Gd_nameTemp;
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
		//判断是接车还是发车，看起始信号机或终止信号机
		//GetHandleByName(str_xhj,nHandleDCTemp)
		//判断是否为正线，如果不为正线通过，则直接提前退出
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
				{//线宽必须为0.5
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
		//考虑加入条件判断
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

		//将当前轨道加入已访问
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
		//分支道岔不加入轨道区段
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
		//存储走过的道岔
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

// 输入起点轨道和终点轨道，寻找进路，找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
// Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
// vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
// 特别用于列车进路获取联锁表信息，增加起始信号机和终止信号机参数
// 增加参数iJL_Cnt，用于统计变更进路个数，没有变更进路时，为1，有变更进路时大于1
/***********************************函数功能描述**********************************************************/
/*********函数名称：GetTongGuoPathGuiDaoAndXHJ******************************************************************************************************/
/*********函数功能：用于寻找通过进路，要求通过进路均为正线通过**************************************************************/
/*********输入：CString& Start_Gd_name,起始轨道名称；long nHandleStartXHJ,起始信号机句柄；long nHandleEndXHJ,终止信号机句柄；long& nHandleLineFirst,起始轨道句柄；******/
/****************long nHandleLineStart,本次调用时起始轨道句柄；long nHandleLineEnd,终止轨道句柄；***********************************************/
/****************long LastDaocha,上次调用时走过的道岔句柄；vector<CString>&sVectorPath,走过的轨道区段；vector<CString>&sDaocha,走过的道岔；*****/
/****************vector<CString>& sXHJ,走过的信号机；CStringArray &GuDao,平面图的股道名称；CLongArray&nArrayHandleLine,走过的轨道句柄；*/
/****************vector<CString> &vec_strGuiDaoCX,超限绝缘轨道，int &iJL_Cnt,改进路包括的进路条数；bool bForword，进路搜索方向************/
/*********输出：**************************************************************/
/************************注释：2015-07-16 pm-15:48 by luo*******************************************************/
void CZnSxt::GetTongGuoPathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		//通过进路中，结束轨道区段不用加进去
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
				{//线宽必须为0.5
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
		//获取终止轨道信号机
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			Name_PassXHJ=Name_PassXHJ+name_xhj;
		}
		sXHJ.push_back(Name_PassXHJ);
		//通过进路不写编号
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
		//去掉其中的分支道岔
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
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
// 			CString temp=_T("至")+Gd_name;
// 			oper_excel.WriteDatatoExcel(1,num,5,temp);
// 			//写迎面进路
// 			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
// 			oper_excel.WriteDatatoExcel(1,num,16,Gd_name);
// 
// 		}
// 		else if(CBlkUtility::FindStrInArray(GuDao,Start_Gd_name)!=-1)
// 		{
// 			CString temp=_T("由")+Start_Gd_name;
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
		//判断是接车还是发车,看起始信号机或终止信号机
		//GetHandleByName(str_xhj,nHandleDCTemp)
		//判断是否为正线,如果不为正线通过,则直接提前退出
		
		//考虑加入条件判断
		if(iCntTemp==nArrayHandleTem.GetCount()-1)
		{
			oper_excel.WriteDatatoExcel(1,num,10,_T("L或U"));

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

		//将当前轨道加入已访问
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
		//存储走过的道岔
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

//输入起点轨道和终点轨道,寻找进路,找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
//Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
//vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
//特别用于列车进路获取联锁表信息,增加起始信号机和终止信号机参数
//增加参数iJL_Cnt,用于统计变更进路个数,没有变更进路时,为1,有变更进路时大于1
void CZnSxt::GetFaChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			Gd_name=_T("XXX");
		}
		//发车最后一条轨道不用加入
		sVectorPath.push_back(Gd_name);
		//nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//获取终止轨道信号机
		this->GetGuiDaoDuanXHJ(nHandleLineEnd,btoward,nArrayPassXHJ);
		CString  Name_PassXHJ;
		for(int k=0;k<nArrayPassXHJ.GetCount();k++)
		{
			CString name_xhj;
			GetEntName(nArrayPassXHJ[k],name_xhj);
			CString EntName;
			GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
			//将发车信号机写作兼调车信号机
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
		//发车进路去掉最后的轨道区段
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
		//去掉出站信号机之前的轨道区段
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
		}//专门用于处理发车进路
		//去掉其中的分支道岔
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		CString str_daocha;
		TranslateVectorToCString(tempDaocha,str_daocha);
		oper_excel.WriteDatatoExcel(1,num,12,str_daocha);
		//PrintGuoDaocha(sXHJ);
		vector<CString> tempXHJ;
		TiquGuoDaocha(sXHJ,tempXHJ);
		//去掉出站信号机之前的调车信号机
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
			CString temp=_T("至")+Gd_nameTemp;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			//写迎面进路
			oper_excel.WriteDatatoExcel(1,num,15,Gd_nameTemp);
			oper_excel.WriteDatatoExcel(1,num,16,Gd_nameTemp);

		}
		else if((i_tem=CBlkUtility::FindStrInArray(GuDao,Start_Gd_name))!=-1)
		{
			CString Gd_nameTemp=this->str_GuDaoName.GetAt(i_tem);
			CString temp=_T("由")+Gd_nameTemp;
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
		//判断是接车还是发车，看起始信号机或终止信号机
		oper_excel.WriteDatatoExcel(1,num,10,_T("L或LU或U"));
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

		//将当前轨道加入已访问
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
		//存储走过的道岔
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
					//判断信号机的性质，如果为发车信号机，加'D'
					long nHandleXHJ=0;
					CString str_ent_name;
					CString str_xhj_name=PassXHJ[i];
//				没起作用，由于包含了逗号
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
//输入起点轨道和终点轨道，寻找进路，找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。
//Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
//vector<CString>sXHJ用于存储进路上经过的信号机：注意终点轨道上的信号机没有纳入考虑
//特别用于列车进路获取联锁表信息，增加起始信号机和终止信号机参数
//增加参数iJL_Cnt，用于统计变更进路个数，没有变更进路时，为1,有变更进路时大于1
void CZnSxt::GetYanXuPathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ )
{
	//static num=4;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		//发车最后一条轨道不用加入
		//sVectorPath.push_back(Gd_name);
		//nArrayHandleTem.Add(nHandleLineStart);
		CLongArray nArrayPassXHJ;
		bool btoward=bForword;
		//获取终止轨道信号机
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
		//延续进路要去掉第一个轨道区段股道
		vector<CString> tempGuidao2;
		for(int i=1;i<tempGuidao.size();i++)
		{
			tempGuidao2.push_back(tempGuidao[i]);
		}
		//去掉其中的分支道岔
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
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
			CString temp=_T("至")+Gd_name;
			oper_excel.WriteDatatoExcel(1,num,5,temp);
			//写迎面进路
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
			oper_excel.WriteDatatoExcel(1,num,16,Gd_name);

		}
		else if(CBlkUtility::FindStrInArray(GuDao,Start_Gd_name)!=-1)
		{
			CString temp=_T("由")+Start_Gd_name;
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
		//判断是接车还是发车,看起始信号机或终止信号机
		//oper_excel.WriteDatatoExcel(1,num,10,_T("L或LU或U"));
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

		//将当前轨道加入已访问
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
		//存储走过的道岔
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
				   //判断信号机的性质,如果为发车信号机,加'D'
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
/************************函数：GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********功能：通过调车进路的起始轨道和终止轨道,输出进路走过的轨道和道岔以及信号机,并将联锁信息写入Excel*****************************************/
/***********输入：CString& StartXHJ,起始信号机名称；CString& EndXHJ,终端信号机名称；long nHandleLineStart,起始轨道；********************/
/******************long nHandleLineEnd,终止轨道；long LastDaocha,走到起始轨道上一次走过的道岔；********************************************/
/******************CStringArray &GuDao,股道名称；bool bForword，进路推进方向***************************************************************/
/***********输出：vector<CString>&sVectorPath,存储走过的轨道区段；vector<CString>&sDaocha,存储走过的道岔；**************************/
/******************vector<CString>& sXHJ,存储走过的信号机；CLongArray&nArrayHandleLine,推进过程中的下一个轨道区段********************/ 
/******************************注释：2015-06-06 pm 14:44 by luo*********************************************************************************/
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
		//如果终端信号机不在进路上，则返回
		CString temp=EndXHJ;
		CString jinlu;
		int count=str_array_xhj.GetCount();
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//为股道时
			jinlu=_T("至")+temp;
			if(count>0)
			{
				AnniuEnd=str_array_xhj[count-1]+_T("DA");
			}
		}
		else
		{
			//为信号机
			long nHandleDC=0;
			GetHandleByName(temp,nHandleDC);
			//判断信号机方向
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//向右
					if(bForword==true)
					{
						jinlu=_T("至")+temp;
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
						jinlu=_T("向")+temp;
						AnniuEnd =temp+_T("A");
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
						jinlu=_T("向")+temp;
						if(count>0)
							AnniuEnd =str_array_xhj[count-1]+_T("A");
					}
					else
					{
						jinlu=_T("至")+temp;
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
		//轨道名
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
		//当前轨道是否存在超限绝缘
		//增加超限区段,第一条轨道线并未考虑到，在这里加上
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		CStringArray strArray_GXHJ;
		TiquEffectiveData(sXHJ,strArray_GXHJ);
		//去掉走过的信号机中同方向的信号机
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
			acutPrintf(_T("对向：%s"),nArrayDXDC.GetAt(i));
		}
		//进站信号机敌对
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
			//只有进站信号机敌对信号存在,敌对列车信号才能存在
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0&&nArrayLCXHJ2.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i)+_T("L,"));
					acutPrintf(_T("同向敌对：%s"),nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i)+_T("L，"));
					acutPrintf(_T("反向敌对：%s"),nArrayLCXHJ.GetAt(i));
				}
			}
		}
		oper_excel.WriteDatatoExcel(1,num,13,vec_xhj_didui);
		//写进路
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//写迎面列车进路
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//存储走过的道岔
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
				//打印后继
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
/************************函数：GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********功能：通过调车进路的起始轨道和终止轨道,输出进路走过的轨道和道岔以及信号机,并将联锁信息写入Excel*****************************************/
/***********输入：CString& StartXHJ,起始信号机名称；CString& EndXHJ,终端信号机名称；long nHandleLineStart,起始轨道；********************/
/******************long nHandleLineEnd,终止轨道；long LastDaocha,走到起始轨道上一次走过的道岔；********************************************/
/******************CStringArray &GuDao,股道名称；bool bForword,进路推进方向***************************************************************/
/***********输出：vector<CString>&sVectorPath,存储走过的轨道区段；vector<CString>&sDaocha,存储走过的道岔；**************************/
/******************vector<CString>& sXHJ,存储走过的信号机；CLongArray&nArrayHandleLine,推进过程中的下一个轨道区段********************/ 
/******************************注释：2015-06-06 pm 14:44 by luo*********************************************************************************/
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
		//如果终端信号机不在进路上,则返回
		CString temp=EndXHJ;
		CString jinlu;
// 		vector<CString> tempXHJ;
// 		//CStringArray str_array_xhj
// 		TiquGuoDaocha(sXHJ,tempXHJ);
		//TiquEffectiveData(sXHJ)
		int count=str_array_xhj.GetCount();
		//temp.Remove(_T("至"));
		//temp.Remove(_T("向"));
		//acutPrintf("temp =%s",temp);
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//为股道时
			jinlu=_T("至")+temp;
			if(count>0)
			{
				AnniuEnd=str_array_xhj[count-1]+_T("DA");
			}
		}
		else
		{
			//为信号机
			long nHandleDC=0;
			GetHandleByName(temp,nHandleDC);
			//判断信号机方向
			int iToward=-1;
		   if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
		   {
			   if(iToward==1)
			   {
				   //向右
				   if(bForword==true)
				   {
					   jinlu=_T("至")+temp;
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
					   jinlu=_T("向")+temp;
					   AnniuEnd =temp+_T("A");
				   }
					   
			   }
			   else if(iToward==2)
			   {
				   if(bForword==true)
				   {
					   jinlu=_T("向")+temp;
					   if(count>0)
					   AnniuEnd =str_array_xhj[count-1]+_T("A");
				   }
				   else
				   {
					   jinlu=_T("至")+temp;
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
		//轨道名
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		CStringArray strArray_GXHJ;
		TiquEffectiveData(sXHJ,strArray_GXHJ);
		//去掉走过的信号机中同方向的信号机
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
			acutPrintf(_T("对向：%s"),nArrayDXDC.GetAt(i));
		}
		//进站信号机敌对
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
			//只有进站信号机敌对信号存在,敌对列车信号才能存在
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0&&nArrayLCXHJ2.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i)+_T("L,"));
					acutPrintf(_T("同向敌对：%s"),nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i)+_T("L,"));
					acutPrintf(_T("反向敌对：%s"),nArrayLCXHJ.GetAt(i));
				}
			}
		}
		oper_excel.WriteDatatoExcel(1,num,13,vec_xhj_didui);
		//写进路
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//写迎面列车进路
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
		}
		
		//GetEntName(nHandleStartXHJ,AnniuStart);
		//在外边写
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//存储走过的道岔
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
				//打印后继
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
/************************函数：GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********功能：通过调车进路的起始轨道和终止轨道,输出进路走过的轨道和道岔以及信号机,并将联锁信息写入Excel*****************************************/
/***********输入：CString& StartXHJ,起始信号机名称；CString& EndXHJ,终端信号机名称；long nHandleLineStart,起始轨道；********************/
/******************long nHandleLineEnd,终止轨道；long LastDaocha,走到起始轨道上一次走过的道岔；********************************************/
/******************CStringArray &GuDao,股道名称；bool bForword,进路推进方向***************************************************************/
/***********输出：vector<CString>&sVectorPath,存储走过的轨道区段；vector<CString>&sDaocha,存储走过的道岔；**************************/
/******************vector<CString>& sXHJ,存储走过的信号机；CLongArray&nArrayHandleLine,推进过程中的下一个轨道区段********************/ 
/******************************注释：2015-06-06 pm 14:44 by luo*********************************************************************************/
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
		acutPrintf(_T("\n进路内信号机："));
		for(int i=0;i<sXHJ.size();i++)
		{
			acutPrintf(_T("\t%s"),sXHJ[i]);
		}
		TiquEffectiveData(sXHJ,str_array_xhj);
		acutPrintf(_T("\n提取后信号机："));
		for(int i=0;i<str_array_xhj.GetCount();i++)
		{
			acutPrintf(_T("\t%s"),str_array_xhj.GetAt(i));
		}
		CString AnniuEnd;
		CString AnniuStart;
		//如果终端信号机不在进路上,则返回
		CString temp=EndXHJ;
		CString jinlu;
		int count=str_array_xhj.GetCount();
		if(CBlkUtility::FindStrInArray(str_array_xhj,temp)==-1&&CBlkUtility::FindStrInArray(GuDao,temp)==-1)
		{
			return;
		}
		else if(CBlkUtility::FindStrInArray(GuDao,temp)!=-1)
		{
			//为股道时
			jinlu=_T("至")+temp;
			if(count>0)
			{
				AnniuEnd=str_array_xhj[count-1]+_T("A");
			}
		}
		else
		{
			//为信号机
			long nHandleDC=0;
			if(temp.Left(1)==_T('X')||temp.Left(1)==_T('S'))
			{
				temp.Remove(_T('D'));
			}
			GetHandleByName(temp,nHandleDC);
			//判断信号机方向
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//向右
					if(bForword==true)
					{
						jinlu=_T("至")+temp;
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
						jinlu=_T("向")+temp;
						AnniuEnd =temp+_T("A");
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
						jinlu=_T("向")+temp;
						if(count>0)
							AnniuEnd =str_array_xhj[count-1]+_T("A");
					}
					else
					{
						jinlu=_T("至")+temp;
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
		//轨道名
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//CStringArray strArray_GXHJ;
		//TiquEffectiveData(sXHJ,strArray_GXHJ);
		//去掉走过的信号机中同方向的信号机
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
			//取信号机的句柄
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
					//acutPrintf(_T("\n信号机方向不对\n"));
				}
			}
		}
		long lastdc=0;
		CStringArray nArrayDXDC;
		CLongArray  nArrayHandleNext;
		//这个函数对于左方向来说求得的对向调车信号机不全
		if(StartXHJ==_T("D4")&&EndXHJ==_T("D10"))
		{
			StartXHJ=StartXHJ;
		}
		FindNextDuiXiangDCByLine(nHandleLineEnd,lastdc,GuDao,nArrayHandleNext,nArrayDXDC,bForword);
		for(int i=0;i<nArrayDXDC.GetCount();i++)
		{
			vec_xhj_didui.push_back(nArrayDXDC.GetAt(i)+_T(","));
			acutPrintf(_T("对向：%s"),nArrayDXDC.GetAt(i));
		}
		//进站信号机敌对
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
			//只有进站信号机敌对信号存在,敌对列车信号才能存在
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0&&nArrayLCXHJ2.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ2.GetAt(i)+_T("L,"));
					acutPrintf(_T("同向敌对：%s"),nArrayLCXHJ2.GetAt(i));
				}
				for(int i=0;i<nArrayLCXHJ.GetCount();i++)
				{
					vec_xhj_didui.push_back(nArrayLCXHJ.GetAt(i)+_T("L，"));
					acutPrintf(_T("反向敌对：%s"),nArrayLCXHJ.GetAt(i));
				}
			}
		}
		oper_excel.WriteDatatoExcel(1,num,13,vec_xhj_didui);
		//写进路
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//写迎面列车进路
			oper_excel.WriteDatatoExcel(1,num,15,Gd_name);
		}

		//GetEntName(nHandleStartXHJ,AnniuStart);
		//在外边写
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//存储走过的道岔
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
				//打印后继
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
/************************函数：GetDiaoChePathGuiDaoAndXHJ********************************************************************************************/
/***********功能：通过调车进路的起始轨道和终止轨道，输出进路走过的轨道和道岔以及信号机，并将联锁信息写入Excel*****************************************/
/***********输入：CString& StartXHJ,起始信号机名称；CString& EndXHJ,终端信号机名称；long nHandleLineStart,起始轨道；********************/
/******************long nHandleLineEnd,终止轨道；long LastDaocha,走到起始轨道上一次走过的道岔；********************************************/
/******************CStringArray &GuDao,股道名称；bool bForword,进路推进方向***************************************************************/
/***********输出：vector<CString>&sVectorPath,存储走过的轨道区段；vector<CString>&sDaocha,存储走过的道岔；**************************/
/******************vector<CString>& sXHJ,存储走过的信号机；CLongArray&nArrayHandleLine,推进过程中的下一个轨道区段********************/ 
/******************************注释：2015-06-06 pm 14:44 by luo*********************************************************************************/
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
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		if(Gd_name==_T(""))
		{
			//如果出现多条未命名区段，去掉重复轨道区段再去掉最后的区段可能去掉的不是最后区段；
			Gd_name=_T("XXX-0");
		}
		sVectorPath.push_back(Gd_name);
		//这里是否需要加上最后的区段
		nArrayHandleTem.Add(nHandleLineStart);
		acutPrintf(_T("\n始端%s 终端%s："),StartXHJ,EndXHJ);
		for(int i=0;i<nArrayHandleTem.GetCount();i++)
		{
			CString te;
			m_mysys.GetXdata(nArrayHandleTem.GetAt(i),_T("GUIDAONAME"),te);
			acutPrintf(_T("\t轨道区段%s"),te);
		}
		CLongArray nArrayHandleTem2;
		nArrayHandleTem2.Append(nArrayHandleTem);
		for(int i=0;i<nArrayHandleTem2.GetCount();i++)
		{
			CString te;
			m_mysys.GetXdata(nArrayHandleTem2.GetAt(i),_T("GUIDAONAME"),te);
			acutPrintf(_T("\t轨道区段%s"),te);
		}
		acutPrintf(_T("\n"));
		vector<CString>tempGuidao;
		TiquGuoDaocha(sVectorPath,tempGuidao);
		CString AnniuEnd;
		CString AnniuStart;
		//如果终端信号机不在进路上,则返回
		CString temp=EndXHJ;
		CString jinlu;
		CLongArray nArrayHandleXHJTemp;
		//从反位线过来的，轨道端信号机不在进路内
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
			//为股道时,不一定
			tempGuidao.pop_back();
			//换成股道名称
			jinlu=_T("至")+this->str_GuDaoName[item];
			if(count>0)
			{
				CString strtemp=str_array_xhj[count-1];
				strtemp.Remove(_T('L'));
				AnniuEnd=strtemp+_T("DA");
			}
		}
		else if((item=CBlkUtility::FindStrInArray(this->sSGuDaoName,temp))!=-1)
		{
			//为股道时,不一定
			tempGuidao.pop_back();
			//换成股道名称
			jinlu=_T("至")+this->str_GuDaoName[item];
			if(count>0)
			{
				CString strtemp=str_array_xhj[count-1];
				strtemp.Remove(_T('L'));
				AnniuEnd=strtemp+_T("DA");
			}
		}
		else
		{
			//为信号机
			//对于非股道调车,需要去掉最后的区段
			tempGuidao.pop_back();
			long nHandleDC=0;
			if(temp.Left(1)==_T('X')||temp.Left(1)==_T('S'))
			{
				temp.Remove(_T('D'));
			}
			GetHandleByName(temp,nHandleDC);
			//判断信号机方向
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//向右
					if(bForword==true)
					{
						jinlu=_T("至")+temp;
						if(count>1)
						{
							//这种条件只有在无岔区段的情况下成立,需要加条件
							CString strGuidao;
							int cnt=tempGuidao.size();
							strGuidao=tempGuidao[cnt-1];
							if(strGuidao.Right(2)==_T("WG"))
							{
								AnniuEnd=str_array_xhj[count-2]+_T("A");
								//去掉无岔区段
								tempGuidao.pop_back();
							}
							else
							{
								//如果是并置信号机就不对了
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
							jinlu=_T("至")+temp;
							AnniuEnd=temp+_T("DZA");
						}
						else
						{
							jinlu=_T("向")+temp;
							AnniuEnd =temp+_T("A");
						}
						
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
// 						jinlu=_T("向")+temp;
// 						if(count>0)
// 							AnniuEnd =str_array_xhj[count-1]+_T("A");
						CString xhjent;
						GetEntBlkNameByHandle(nHandleDC,xhjent);
						if(xhjent.Left(6)==_T("XHJ_LJ"))
						{
							jinlu=_T("至")+temp;
							AnniuEnd=temp+_T("DZA");
						}
						else
						{
							jinlu=_T("向")+temp;
							AnniuEnd =temp+_T("A");
						}
					}
					else
					{
						jinlu=_T("至")+temp;
						if(count>1)
						{
							CString strGuidao;
							int cnt=tempGuidao.size();
							strGuidao=tempGuidao[cnt-1];
							if(strGuidao.Right(2)==_T("WG"))
							{
								AnniuEnd=str_array_xhj[count-2]+_T("A");
								//去掉无岔区段
								tempGuidao.pop_back();
							}
							else
							{
								//如果是并置信号机就不对了
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
						jinlu=_T("至")+temp;
						AnniuEnd=temp+_T("DZA");
					}
					else
					{
						jinlu=_T("向")+temp;
						AnniuEnd =temp+_T("A");
					}

				}

			}
			
		}
		//nArrayHandleTem.Add(nHandleLineStart);
		//CLongArray nArrayPassXHJ;
		oper_excel.WriteDatatoExcel(1,num,4,this->iJLnum);
		//当前轨道是否存在超限绝缘
		//增加超限区段,第一条轨道线并未考虑到,在这里加上
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
		//去掉其中的分支道岔
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
		//去掉没有道岔的情况，避免多余逗号；
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
		//去掉走过的信号机中同方向的信号机
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
			//取信号机的句柄
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
					//acutPrintf(_T("\n信号机方向不对\n"));
				}
			}
		}
		long lastdc=0;
		CStringArray nArrayDXDC;
		CLongArray  nArrayHandleNext;
		CStringArray strArrayDXDaocha;
		//这个函数对于左方向来说求得的对向调车信号机不全
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
			//acutPrintf(_T("对向：%s"),nArrayDXDC.GetAt(k));
			long nHandleDXDC=0;
			//注意这里需要去掉_T('D')
			CString tempXHJ=nArrayDXDC.GetAt(k);
			if(tempXHJ.Left(1)==_T('X')||tempXHJ.Left(1)==_T('S'))
			{
				tempXHJ.Remove(_T('D'));
			}
			GetHandleByName(tempXHJ,nHandleDXDC);
			//CStringArray strArrayJieJinquduan;
			CLongArray nArrayHandleNextTXLine;
			CStringArray strArrayTXDC;
			//通过一个信号机获取其调车终端信号机
			GetNextDCXHJByDCXHJ(nHandleDXDC,GuDao,strArrayJieJinquduan,nArrayHandleNextTXLine,strArrayTXDC);
			//获取同方向调车信号机终端后
			int iCnt_TXDC=strArrayTXDC.GetCount();
			if(iCnt_TXDC>1)
			{
				//并且存在不过进路内轨道的调车进路,则需要添加敌对条件,并且应该返回“条件”
				long nHandleLineStartTem=0;
				long nHandleLineEndTem=0;
				bool fangxiang=false;
                GetXHJ_YouCe_GdLine(nHandleDXDC,nHandleLineStartTem,fangxiang);
				if(nHandleLineStartTem==0)
				{
					acutPrintf(_T("\n获取信号右侧线失败"));
				}
				for(int i=0;i<strArrayTXDC.GetCount();i++)
				{
					long nHandlexhj=0;
					//需要注意
					acutPrintf(_T("调至：%s"),strArrayTXDC.GetAt(i));
					GetHandleByName(strArrayTXDC.GetAt(i),nHandlexhj);
					if(nHandlexhj==0)
					{
						acutPrintf(_T("\n获取信号机句柄失败"));
					}
					bool fangxiang2=false;
					GetXHJ_ZuoCe_GdLine(nHandlexhj,nHandleLineEndTem,fangxiang2);
					if(nHandleLineEndTem==0)
					{
						acutPrintf(_T("\n获取信号机左侧线失败"));
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
						acutPrintf(_T("条件:%s"),vec_strTiaojianT[0]);
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
		//考虑敌对条件 
		//进站信号机敌对
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
		//考虑会有多算的敌对列车信号
		FindNextLineByJYJ(nHandleLine_Start,nHandleLine_StartNew,!fangxiang);
		//寻找敌对进站信号机
		FindNextLieCheXHJByLine(nHandleLine_StartNew,lastdc3,GuDao,nArrayHandleNext3,nArrayLCXHJ2,!bForword);
		if(nArrayLCXHJ2.GetCount()!=0)
		{
			//只有进站信号机敌对信号存在,敌对列车信号才能存在
			long lastdc2=0;
			CStringArray nArrayLCXHJ;
			CLongArray nArrayHandleNext2;
			//寻找敌对出站信号机
			FindNextLieCheXHJByLine(nHandleLineEnd,lastdc2,GuDao,nArrayHandleNext2,nArrayLCXHJ,bForword);
			if(nArrayLCXHJ.GetCount()!=0)
			{
				for(int i=0;i<nArrayLCXHJ2.GetCount();i++)
				{
				     //是否存在条件
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
	    //去掉重复信号机
		vector<CString> str_vec_didui_xhj;
		DeleteSameData(vec_xhj_didui,str_vec_didui_xhj);
		//还需要合并列车兼调车信号机
		vector<CString>str_vec_xhj;
		CombandXHJData(str_vec_didui_xhj,str_vec_xhj);
		TranslateVectorToCString(str_vec_xhj,str_ddxhj);
		oper_excel.WriteDatatoExcel(1,num,13,str_ddxhj);
		//写进路
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if((item=CBlkUtility::FindStrInArray(GuDao,Gd_name))!=-1)
		{
			//写迎面列车进路
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
		//将当前轨道加入已访问
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
		//存储走过的道岔
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
				//打印后继
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
/************************函数：IsAddConditionDiaoChePath********************************************************************************************/
/***********功能：根据敌对进路的调车始端和调车终端，确定是否需要给敌对调车信号机加条件*****************************************/
/***********输入：CString& StartXHJ,起始信号机名称；CString& EndXHJ,终端信号机名称；long nHandleLineStart,起始轨道；********************/
/******************long nHandleLineEnd,终止轨道；long LastDaocha,走到起始轨道上一次走过的道岔；********************************************/
/******************CStringArray &GuDao,股道名称；bool bForword，进路推进方向***************************************************************/
/***********输出：vector<CString>&sVectorPath,存储走过的轨道区段；vector<CString>&sDaocha,存储走过的道岔；**************************/
/******************vector<CString>& sXHJ,存储走过的信号机；CLongArray&nArrayHandleLine,推进过程中的下一个轨道区段********************/ 
/******************************注释：2015-06-06 pm 14:44 by luo*********************************************************************************/
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
// 		acutPrintf(_T("\n提取后信号机："));
// 		for(int i=0;i<str_array_xhj.GetCount();i++)
// 		{
// 			//acutPrintf(_T("\t%s"),str_array_xhj.GetAt(i));
// 		}
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
// 		acutPrintf(_T("\n 调车进路走过的轨道区段："));
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
				//说明经过径路内轨道区段
				break;
			}
		}
		if(j==nArrayHandleTem.GetCount())
		{
			iTJ_Cnt++;
			vec_str_tiaojian.push_back("<>");
			acutPrintf(_T("\n需要加条件"));
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		CStringArray strArrayCXGdName;
		//存储走过的道岔
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
				//打印后继
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
/************************函数：IsAddConditionDiaoChePath********************************************************************************************/
/***********功能：根据敌对进路的调车始端和调车终端，确定是否需要给敌对调车信号机加条件*****************************************/
/***********输入：CString& StartXHJ,起始信号机名称；CString& EndXHJ,终端信号机名称；long nHandleLineStart,起始轨道；********************/
/******************long nHandleLineEnd,终止轨道；long LastDaocha,走到起始轨道上一次走过的道岔；********************************************/
/******************CStringArray &GuDao,股道名称；bool bForword，进路推进方向***************************************************************/
/***********输出：vector<CString>&sVectorPath,存储走过的轨道区段；vector<CString>&sDaocha,存储走过的道岔；**************************/
/******************vector<CString>& sXHJ,存储走过的信号机；CLongArray&nArrayHandleLine,推进过程中的下一个轨道区段********************/ 
/******************************注释：2015-06-06 pm 14:44 by luo*********************************************************************************/
void CZnSxt::IsAddConditionDiaoChePath(long nHandleLineStart,long nHandleLineEnd,CLongArray &nArrayHandleStop,long LastDaocha,vector<CString>&sDaocha,CStringArray &GuDao,CLongArray&nArrayHandleLine, vector<CString>& vec_str_tiaojian,int &iJL_Cnt,int &iTJ_Cnt,bool bForword/*=true*/ )
{
	//int b_Ret=FALSE;
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		CStringArray str_array_xhj;
		nArrayHandleTem.Add(nHandleLineStart);
// 		acutPrintf(_T("\n 调车进路走过的轨道区段："));
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
				//说明经过径路内轨道区段
				break;
			}
		}
		if(j==nArrayHandleTem.GetCount())
		{
			iTJ_Cnt++;
			vec_str_tiaojian.push_back("<>");
			acutPrintf(_T("\n需要加条件"));
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
		//将当前轨道加入已访问
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
				//打印后继
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
/*******************************函数：GetDiaoChePathGuiDaoAndDiDuiXHJ*************************************************************************************************************************************************/
/***********功能描述：获取调车起始轨道至调车终止轨道间的道岔、轨道区段、以及调车进路上的敌对信号机，即背向的调车信号机*******************************************************************************************/
/***********输入： CString& StartXHJ，起始信号机名称；CString& EndXHJ,，终端信号机名称，long nHandleLineStart,起始信号机所在轨道，long nHandleLineEnd终止信号机所在轨道，bool bForword进路方向*****/
/******************long LastDaocha,上一轨道走过的道岔，CStringArray &GuDao,股道名称************************************************************************************************************************************/
/***********输出：vector<CString>&sVectorPath,用于存储走过的轨道区段；vector<CString>&sDaocha,储存走过的道岔；***********************************************************************************************/
/***************** vector<CString>& sXHJ,储存走过的敌对信号机；CLongArray&nArrayHandleLine，储存当前轨道区段的后继区段***************************************************************************************/
/**********************************注释---2015-06-05-pm 16:23-by-luo****************************************************************************************************************************************************/

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
		//如果终端信号机不在进路上，则返回
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
			//为股道时
			jinlu=_T("至")+temp;
			AnniuEnd=str_array_xhj[count-1]+_T("DA");
		}
		else
		{
			//为信号机
			long nHandleDC=0;
			GetHandleByName(temp,nHandleDC);
			//判断信号机方向
			int iToward=-1;
			if(this->GetBlockToward(nHandleDC,iToward)==TRUE)
			{
				if(iToward==1)
				{
					//向右
					if(bForword==true)
					{
						jinlu=_T("至")+temp;
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
						jinlu=_T("向")+str_array_xhj[count-1];
						AnniuEnd =str_array_xhj[count-1]+_T("A");
					}

				}
				else if(iToward==2)
				{
					if(bForword==true)
					{
						jinlu=_T("向")+temp;
						AnniuEnd =str_array_xhj[count-1]+_T("A");
					}
					else
					{
						jinlu=_T("至")+temp;
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
		//轨道名
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
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		//PrintGuoDaocha(sDaocha);
		vector<CString> tempDaocha;
		TiquGuoDaocha(sDaocha,tempDaocha);
		oper_excel.WriteDatatoExcel(1,num,12,tempDaocha);
		//PrintGuoDaocha(sXHJ);
		//		vector<CString> tempXHJ;
		//	TiquGuoDaocha(sXHJ,tempXHJ);
		oper_excel.WriteDatatoExcel(1,num,13,tempXHJ);
		//写进路
		oper_excel.WriteDatatoExcel(1,num,5,jinlu);
		if(CBlkUtility::FindStrInArray(GuDao,Gd_name)!=-1)
		{
			//写迎面列车进路
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		//存储走过的道岔
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
				//打印后继
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
//输入起点轨道和终点轨道，寻找进路，找到的进路存入sVectorPath中。sDaocha用于存储进路上经过的道岔。Gudao存储进路搜索时遇到的可以停止搜索的轨道区段。nAarryHandleLine存储进路搜寻中已经访问的轨道句柄
void CZnSxt::GetPathGuiDao3( long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	CLongArray nArrayHandleTem;
	nArrayHandleTem.Copy(nArrayHandleLine);
	if(nHandleLineEnd!=0&&nHandleLineEnd==nHandleLineStart)
	{
		//轨道名
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		nArrayHandleTem.Add(nHandleLineStart);
		//应将nHandleLineEnd上所有道岔置于定位，待研究
		//压入对应的道岔定反位
		acutPrintf(_T("\n 搜索结束"));
		for(int i=0;i<sVectorPath.size();i++)
		{
			//输出搜索到的结果
			acutPrintf(_T("\t第%d 轨道区段名为%s"),i,sVectorPath[i].GetString());
		}
		sVectorPath.pop_back();
		acutPrintf(_T("\n 经过轨道区段：%d"),nArrayHandleTem.GetCount());
		// 		打印经过道岔定反位
		// 				acutPrintf(_T("\n 经过道岔："));
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
		//将当前轨道加入已访问
		nArrayHandleTem.Add(nHandleLineStart);
		CString Gd_name;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
		sVectorPath.push_back(Gd_name);
		CLongArray nArrayHandleNext;
		CLongArray nArrayHandleThisDC;
		//存储走过的道岔
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
				//打印后继
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
// 		//轨道名
// 		CString Gd_name;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),Gd_name);
// 		sVectorPath.push_back(Gd_name);
// 		//压入对应的道岔定反位
// 		acutPrintf(_T("\n 搜索结束"));
// 		for(int i=0;i<sVectorPath.size();i++)
// 		{
// 			//输出搜索到的结果
// 			acutPrintf(_T("\t第%d 股道名为%s"),i,sVectorPath[i].GetString());
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
// 				//打印后继
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
// 	{//找到，结束
// 		return;
// 	}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		//acutPrintf(_T("轨道名称"))ⅡG
// 		//CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
// 		
// 	CADORecordset cSet(&m_AdoDb) ;
// 	CString strSql ;
// 	strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 	long nLineType = 0 ;
// 	m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 	//表示为尽头线
// 	if (nLineType==2) 
// 	{
// 		CString name;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 		acutPrintf(_T("\n 尽头线：%s"),name.GetString());
// 		return ;
// 	}
// 	//判断轨道上DC个数
// 	CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 	//获取轨道信息，这个函数需要仔细看看
// 	this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 	int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
// 	int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
// 	//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 	CString guiname;
// 	m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 	//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 	long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 	strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 	m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 	strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 	m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 	//如果是向右寻找，则找右端绝缘节。
// 	int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
// 	long nHandleNextLine = 0 ;
// 	strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 	CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 	//读取结果集
// 	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 	if(iNumDC<1)
// 	{
// 		//轨道不包含道岔时，后继只有一个
// 		if(nLineType==0)
// 		{//不为交叉渡线时
// 			if(nHandleNextLine!=0)
// 			{
// 				nArrayHandleLine.Add(nHandleNextLine);
// // 				 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; ;
// // 				m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 			}
// 			else
// 			{
// 				acutPrintf(_T("\n nHandNextLine为0"));
// 			}
// 		}
// 		else if(nLineType==1)
// 		{//为交叉渡线时
// 			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 			CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			//读取结果集
// 		//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 			//渡线绝缘节在右侧时
// 			if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
// 			{
// 				//找渡线绝缘节在左侧的渡线
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
// 						acutPrintf(_T("\n nHandNextLine为0"));
// 					}
// 				}
// 				else
// 				{
// 					//后继为道岔定位所在轨道
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//反位线对应的道岔
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					//道岔所在轨道即为后继轨道
// 				    m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 					if(nHandleNextLine!=0)
// 					{
// 						nArrayHandleLine.Add(nHandleNextLine);
// // 						 m_mysys.FlashEnt(nHandleNextLine,2000,200) ; 
// // 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 					}
// 					else
// 					{
// 					  acutPrintf(_T("\n nHandNextLine为0"));
// 					}
// 				}
// 			}
// 			//渡线绝缘节在做左侧时
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
// 					////后继为道岔定位所在轨道
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//反位线对应的道岔
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					//道岔所在轨道即为后继轨道
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 					if(nHandleNextLine!=0)
// 					{
// 						nArrayHandleLine.Add(nHandleNextLine);
// // 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// // 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 					}
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLine为0"));
// 					}
// 				}
// 
// 			}
// 		}
// 		//return;
// 	}
// 	else
// 	{//含有道岔时，可能存在多条后继
// 		//道岔为定位时后继
// 		nArrayHandleLine.Add(nHandleNextLine);
// 		for(int k=0;k<iNumDC;k++)
// 		{//道岔反位时后继
// 			//取道岔handle
// 			long nHandleDC=nArrayHandleDC[k];
// 			//查询反位所在线
// 			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 			long nHandle_fw=0;
// 			long nHandle_dw=0;
// 			int IsStart=-1;
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 			m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 			if(IsStart==0&&bForword==true)
// 			{
// 				//向有推进时起点
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
// 				//向左推进时起点
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
// 	  AfxMessageBox(_T("未知的错误!in FindNextLine"));
//   }	
// }
//记录访问过的道岔，避免走回路，顺便记录走过道岔的定反位，用于联锁表
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
// 		{//找到，结束
// 			return;
// 		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		//acutPrintf(_T("轨道名称"))ⅡG
// 		//CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
// 
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		//表示为尽头线
// 		if (nLineType==2) 
// 		{
// 			CString name;
// 			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
// 			return ;
// 		}
// 		//判断轨道上DC个数
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//获取轨道信息，这个函数需要仔细看看
// 		this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
// 		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//如果是向右寻找，则找右端绝缘节。
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//读取结果集
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//轨道不包含道岔时，后继只有一个
// 			if(nLineType==0)
// 			{//不为交叉渡线时
// 				if(nHandleNextLine!=0)
// 				{
// 					nArrayHandleLine.Add(nHandleNextLine);
// 					// 				 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; ;
// 					// 				m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 				}
// 				else
// 				{
// 					//还有可能当前线为某一个道岔的反位线，此时的线为渡线
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//反位线对应的道岔
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					if(nHandleDC!=0)
// 					{
// 						//查询道岔所在的线，该线为后继
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//反位线对应的道岔
// 						long nHandle_dw=0;
//                        // long nHandle_fw=0;						
// 						int IsStart=-1;
// 						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 					    m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 						if(bForword==true&&IsStart>0)
// 						{
//                              //定位所在线为后继    
// 							nArrayHandleLine.Add(nHandle_dw);
// 						}
// 						else if(bForword==false&&IsStart==0)
// 						{
// 							nArrayHandleLine.Add(nHandle_dw);
// 						}
// 					}
// 					else
// 					{
//                        acutPrintf(_T("\n nHandNextLine为0"));
// 					}
// 				}
// 			}
// 			else if(nLineType==1)
// 			{//为交叉渡线时
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 				CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 				//读取结果集
// 				//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 				//渡线绝缘节在右侧时
// 				if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
// 				{
// 					//找渡线绝缘节在左侧的渡线
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
// 							acutPrintf(_T("\n nHandNextLine为0"));
// 						}
// 					}
// 					else
// 					{
// 						//后继为道岔定位所在轨道
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//反位线对应的道岔
// 						long nHandleDC=0;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 						//道岔所在轨道即为后继轨道
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 						if(nHandleNextLine!=0)
// 						{
// 							nArrayHandleLine.Add(nHandleNextLine);
// 							// 						 m_mysys.FlashEnt(nHandleNextLine,2000,200) ; 
// 							// 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 						}
// 						else
// 						{
// 							acutPrintf(_T("\n nHandNextLine为0"));
// 						}
// 					}
// 				}
// 				//渡线绝缘节在做左侧时
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
// 						////后继为道岔定位所在轨道
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//反位线对应的道岔
// 						long nHandleDC=0;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 						//道岔所在轨道即为后继轨道
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 						if(nHandleNextLine!=0)
// 						{
// 							nArrayHandleLine.Add(nHandleNextLine);
// 							// 						 m_mysys.FlashEnt(nHandleNextLine,2000, 200) ; 
// 							// 						m_mysys.SetEntColorIndex(nHandleNextLine,colorindex);
// 						}
// 						else
// 						{
// 							acutPrintf(_T("\n nHandNextLine为0"));
// 						}
// 					}
// 				}
// 			}
// 			//return;
// 		}
// 		else
// 		{//含有道岔时，可能存在多条后继
// 			//道岔为定位时后继
// 			nArrayHandleLine.Add(nHandleNextLine);
// 			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
// 			for(int k=0;k<iNumDC;k++)
// 			{//道岔反位时后继,需要前面访问的道岔
// 				//取道岔handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//查询反位所在线
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				long nHandle_fw=0;
// 				long nHandle_dw=0;
// 				int IsStart=-1;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//向有推进时起点
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
// 					//向左推进时起点
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
// 		AfxMessageBox(_T("未知的错误!in FindNextLine"));
// 	}	
// }
//记录访问过的道岔，避免走回路，顺便记录走过道岔的定反位，用于联锁表，新加入返回进路上的敌对信号机
//关于敌对信号机：无岔区段上的信号机都为敌对信号机，有岔区段上的信号机，只有在进路上，信号机才算敌对
// void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,CLongArray &nArrayHandleDDXHJ,bool bForword/*=true*/ )
// {
// 	try
// 	{
// 		// static int stop =0;
// 		if(nHandleLineStart==nHandleLineEnd)
// 		{//找到，结束
// 			return;
// 		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		//表示为尽头线
// 		if (nLineType==2) 
// 		{
// 			CString name;
// 			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
// 			return ;
// 		}
// 		//判断轨道上DC个数
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//获取轨道信息，这个函数需要仔细看看
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
// 		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
// 		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//如果是向右寻找，则找右端绝缘节。
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//读取结果集
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//轨道不包含道岔时，后继只有一个
// 			if(nLineType==0)
// 			{//不为渡线时
// 				if(nHandleNextLine!=0)
// 				{
// 					nArrayHandleLine.Add(nHandleNextLine);
// 					CString Daocha=_T("");
// 					GuoDaoCha.Add(Daocha);
// 					nArrayHandleDDXHJ.Copy(nArrayHandleXHJ);
// 				}
// 				else
// 				{
// 					//还有可能当前线为某一个道岔的反位线
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//反位线对应的道岔
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					if(nHandleDC!=0)
// 					{
// 						//查询道岔所在的线，该线为后继
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//反位线对应的道岔
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
// 							//定位所在线为后继
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("（")+DCname+_T(")");
// 							// 						   //道岔反位时，需要考虑是否存在带动道岔
// 							GuoDaoCha.Add(Daocha);
// 							nArrayHandleDDXHJ.Copy(nArrayHandleXHJ);
// 						}
// 						else if(bForword==false&&IsStart==0)
// 						{
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("（")+DCname+_T(")");
// 							//道岔反位时，需要考虑是否存在带动道岔
// 							GuoDaoCha.Add(Daocha);
// 							nArrayHandleDDXHJ.Copy(nArrayHandleXHJ);
// 						}
// 					}
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLine为0"));
// 					}
// 				}
// 			}
// 			else
// 			{
// 			}
// 			//return;
// 		}//endif NumDC
// 		else
// 		{//含有道岔时，可能存在多条后继
// 			//道岔为定位时后继
// 			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
// 			CString DaochaD;
// 			for(int k=0;k<iNumDC;k++)
// 			{//道岔反位时后继,需要前面访问的道岔
// 				//取道岔handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//查询反位所在线
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
// 				//向右推进有后继时
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//向右推进时起点
// 					if(nHandle_fw!=0)
// 					{
// 						//继续往下求一次后继，判断是否为渡线
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//为渡线时，求反位线的后继
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//如果是向右寻找，则找右端绝缘节。
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//读取结果集
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//再找该反位线所在道岔的定位线
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//反位线对应的道岔
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//查询道岔所在的线，该线为后继
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//反位线对应的道岔
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2即为渡线的后继
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//打印渡线道岔
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
// 								long nHandle_Anothe_Min=0;
// 								long nHandle_Anothe_Max=0;
// 								//判断渡线道岔是否属于交叉渡线的道岔
// 								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 								{
// 									CString DC_Another_Min;
// 									CString DC_Another_Max;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//道岔反位时，需要考虑是否存在带动道岔
// 									GuoDaoCha.Add(Daocha);
// 								}						
// 							}
// 						}
// 						else
// 						{//反位线不为渡线时
// 							nArrayHandleLine.Add(nHandle_fw);
// 							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
// 							//道岔反位时，需要考虑是否存在带动道岔
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//向左推进时起点
// 					if(nHandle_fw!=0)
// 					{
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//为渡线时，求反位线的后继
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//如果是向右寻找，则找右端绝缘节。
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//读取结果集
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//再找该反位线所在道岔的定位线
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//反位线对应的道岔
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//查询道岔所在的线，该线为后继
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//反位线对应的道岔
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2即为渡线的后继
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//打印渡线道岔
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//道岔反位时，需要考虑是否存在带动道岔
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
// 						DaochaD=DaochaD+DCname1+_T("，");
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
// 					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
// 		AfxMessageBox(_T("未知的错误!in FindNextLine"));
// 	}	
// }
//记录访问过的道岔，避免走回路，顺便记录走过道岔的定反位，用于联锁表
void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//如果起始位股道，则结束
			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
				}
				else
				{
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //道岔反位时，需要考虑是否存在带动道岔
							GuoDaoCha.Add(Daocha);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("(")+DCname+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
						acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			// 			else if(nLineType==1)
			// 			{
			// 				//为渡线时
			// 				acutPrintf(_T("\n该线为渡线"));
			// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
			// 				CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			// 				//读取结果集
			// 				//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
			// 				//渡线绝缘节在右侧时
			// 				if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
			// 				{
			// 					//找渡线绝缘节在左侧的渡线
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
			// 							acutPrintf(_T("\n nHandNextLine为0"));
			// 						}
			// 					}
			// 					else 
			// 					{
			// 						//后继为道岔定位所在轨道
			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			// 						//反位线对应的道岔
			// 						long nHandleDC=0;
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
			// 						//道岔所在轨道即为后继轨道
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
			// 						if(nHandleNextLine!=0)
			// 						{
			// 							nArrayHandleLine.Add(nHandleNextLine);
			// 						}
			// 						else
			// 						{
			// 							acutPrintf(_T("\n nHandNextLine为0"));
			// 						}
			// 					}
			// 				}
			// 				//渡线绝缘节在做左侧时
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
			// 						////后继为道岔定位所在轨道
			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			// 						//反位线对应的道岔
			// 						long nHandleDC=0;
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
			// 						//道岔所在轨道即为后继轨道
			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
			// 						if(nHandleNextLine!=0)
			// 						{
			// 							nArrayHandleLine.Add(nHandleNextLine);
			// 						}
			// 						else
			// 						{
			// 							acutPrintf(_T("\n nHandNextLine为0"));
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
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			for(int k=0;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//需要考虑是否有带动道岔存在
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
									//道岔反位时，需要考虑是否存在带动道岔
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
									//道岔反位时，需要考虑是否存在带动道岔
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						DaochaD=DaochaD+DCname1+_T("，");
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//如果起始位股道，则结束
			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
				return;
			}
			if(nLineType==0)
			{//不为渡线时
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //道岔反位时，需要考虑是否存在带动道岔
							GuoDaoCha.Add(Daocha);
							long nHandleThisDC=nHandleDC;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("(")+DCname+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
						acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
								}
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
									//道岔反位时，需要考虑是否存在带动道岔
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
								}
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
									//道岔反位时，需要考虑是否存在带动道岔
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						DaochaD=DaochaD+DCname1+_T("，");
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n遇到交叉渡线3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
						}

					}
				}
			}
			//所有道岔为定位时
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
void CZnSxt::FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray& nArrayZhiXHJ,CLongArray& nArrayXiangXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//如果起始位股道，则结束
			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
				return;
			}
			if(nLineType==0)
			{//不为渡线时
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
						acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
									//道岔反位时，需要考虑是否存在带动道岔
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
							long Zhi_XHJ=0;
							long Xiang_XHJ=0;
							this->GetGuiDaoDuanXHJ(bForword,nHandle_fw,Zhi_XHJ,Xiang_XHJ);
							nArrayZhiXHJ.Add(Zhi_XHJ);
							nArrayXiangXHJ.Add(Xiang_XHJ);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
									//道岔反位时，需要考虑是否存在带动道岔
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						DaochaD=DaochaD+DCname1+_T("，");
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n遇到交叉渡线3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
						}

					}
				}
			}
			//所有道岔为定位时
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
/*******************函数说明:FindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********功能：通过一条进路的起始轨道和终止轨道，找出起始轨道的后继轨道，并返回从起始轨道到下一条轨道中走过的道岔以及属于进路上的信号机，后继轨道可能有多条*******************************************************************/
/**********输入：nHandleLineStart,进路的起始轨道线；nHandLineEnd,设定进路的终止轨道线，用于终止进路寻找过程*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔；bForward,用于标记寻找后继防线，true为向右推进***************************************************************************/
/*****************Gudao，股道名称，可以用于终止继续寻找；JieJinQuDuan，接近区段名称;******************************************************************************************************/
/**********输出：GuoDaocha,用于记录在寻找进路的过程中走过的道岔,属于联锁表中的一项内容； nArrayHandleThisDaocha，到下一条进路时走过相应道岔*************************************/
/*****************nArrayHandleNextLine,返回所有的后继轨道区段;  GuoXHJ,到一条进路时走过的信号机，信号机属于进路内部*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		//long t1=GetTickCount();
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
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
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//去掉调车信号机遇到股道时结束，去掉限制，修改于2015-09-17 18:13 
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
// 			long t2=GetTickCount();
// 			CString str;
			//str.Format("DiaoCheFindNextLineAndXHJ time:%d ms",t2-t1);
			//acutPrintf(_T("\n %s"),str);
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					//对于调车来说，应该只包括对向的调车信号机
					CString EntName;
					GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name_xhj+=_T("D");
					}
				    Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");
				}
// 				CString Daocha=_T("");
// 				GuoDaoCha.Add(Daocha);
				GuoXHJ.Add(Name_PassXHJ);
				return;
			}
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//获取轨道左端信号机
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
					    Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");

					}
					GuoXHJ.Add(Name_PassXHJ);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
						//acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
									//道岔反位时，需要考虑是否存在带动道岔
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						{   //反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
									//道岔反位时，需要考虑是否存在带动道岔
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
							//反位线不为渡线
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						DaochaD=DaochaD+DCname1+_T("，");
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n遇到交叉渡线3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
						}

					}
				}
			}
			//所有道岔为定位时
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
				CLongArray nPassXHJ;
				CString  Name_PassXHJ=_T("");
				//GetBlockPosition_X(nHandleThisDC,pointx);
				//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
				//所有道岔为定位时后继
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
					Name_PassXHJ=Name_PassXHJ+name+_T("，");
				}
				GuoXHJ.Add(Name_PassXHJ);
				GuoDaoCha.Add(DaochaD);
			}
			else
			{
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
						nArrayHandleLine.Add(nHandle_dw);
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						long nHandleDaiDong_nMin=0;
						long nHandleDaiDong_nMax=0;
						CString DaiDong_MinName;
						CString DaiDong_MaxName;
						CString Daocha=DaochaD+_T("，")+_T("(")+DCname+_T(")");
						if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
						{
							// Daocha=_T("(")+DCname+_T(")");
							GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
							GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
							Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

						}
						else
						{
							// Daocha=_T("(")+DCname+_T(")");

						}
						//CString Daocha=_T("(")+DCname+_T(")");
						// 						   //道岔反位时，需要考虑是否存在带动道岔
						GuoDaoCha.Add(Daocha);
						long nHandleThisDC=nHandleDC;
						nArrayHandleThisDaocha.Add(nHandleThisDC);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
						CString  Name_PassXHJ=_T("");
						GuoXHJ.Add(Name_PassXHJ);
						CString Daocha=DaochaD+_T("，")+_T("(")+DCname+_T(")");
						//道岔反位时，需要考虑是否存在带动道岔
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
							Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}

//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
/*******************函数说明:FindNextLineAndXHJ2*******************************************************************************************************************************************************************/
/**********功能：通过一条进路的起始轨道和终止轨道，找出起始轨道的后继轨道，并返回从起始轨道到下一条轨道中走过的道岔以及属于进路上的信号机，后继轨道可能有多条*******************************************************************/
/**********输入：nHandleLineStart,进路的起始轨道线；nHandLineEnd,设定进路的终止轨道线，用于终止进路寻找过程*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔；bForward,用于标记寻找后继防线，true为向右推进***************************************************************************/
/*****************Gudao，股道名称，可以用于终止继续寻找；JieJinQuDuan，接近区段名称;******************************************************************************************************/
/**********输出：GuoDaocha,用于记录在寻找进路的过程中走过的道岔,属于联锁表中的一项内容； nArrayHandleThisDaocha，到下一条进路时走过相应道岔*************************************/
/*****************nArrayHandleNextLine,返回所有的后继轨道区段;  GuoXHJ,到一条进路时走过的信号机，信号机属于进路内部*******************************************************************/
/*****************CStringArray&strCXGuiDaoName,用于记录需要占用的超限区段******************************************************************************************************************************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndXHJ2( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray&strCXGuiDaoName,bool bForword/*=true*/ )
{
	try
	{
		//long t1=GetTickCount();
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
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
		//调车信号机遇到股道时结束
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
		//	acutPrintf(_T("\n 尽头线：%s  handle=%ld"),name.GetString(),nHandleLineStart);
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					//对于调车来说，应该只包括对向的调车信号机
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
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					//增加超限区段
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
					//获取轨道左端信号机
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
						//acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
									//道岔反位时，需要考虑是否存在带动道岔
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
						{   //反位线不为渡线时
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
							//道岔反位时，需要考虑是否存在带动道岔
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
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
									//道岔反位时，需要考虑是否存在带动道岔
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
							//反位线不为渡线
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							//acutPrintf(_T("\n遇到交叉渡线3"));
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
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}
			//所有道岔为定位时
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
				//所有道岔为定位时后继
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
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
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
						// 						   //道岔反位时，需要考虑是否存在带动道岔
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
						//道岔反位时，需要考虑是否存在带动道岔
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
		AfxMessageBox(_T("未知的错误!in DiaoCheFindNextLineAndXHJ"));
	}	

}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
/*******************函数说明:FindNextLineAndXHJ2*******************************************************************************************************************************************************************/
/**********功能：通过一条进路的起始轨道和终止轨道，找出起始轨道的后继轨道，并返回从起始轨道到下一条轨道中走过的道岔以及属于进路上的信号机，后继轨道可能有多条*******************************************************************/
/**********输入：nHandleLineStart,进路的起始轨道线；nHandLineEnd,设定进路的终止轨道线，用于终止进路寻找过程*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔；bForward,用于标记寻找后继防线，true为向右推进***************************************************************************/
/*****************Gudao，股道名称，可以用于终止继续寻找；JieJinQuDuan，接近区段名称;******************************************************************************************************/
/**********输出：GuoDaocha,用于记录在寻找进路的过程中走过的道岔,属于联锁表中的一项内容； nArrayHandleThisDaocha，到下一条进路时走过相应道岔*************************************/
/*****************nArrayHandleNextLine,返回所有的后继轨道区段;  GuoXHJ,到一条进路时走过的信号机，信号机属于进路内部*******************************************************************/
/*****************CStringArray&strCXGuiDaoName,用于记录需要占用的超限区段******************************************************************************************************************************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndDaocha( long nHandleLineStart,long nHandleLineEnd,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//调车信号机遇到股道时结束
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
		//	acutPrintf(_T("\n 尽头线：%s  handle=%ld"),name.GetString(),nHandleLineStart);
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
		if(bForword==true)
		{
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
			
				return;
			}
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					long nHandleThisDC=0;
					nArrayHandleThisDaocha.Add(nHandleThisDC);
				}
				else
				{
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
						//acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
								{
									nArrayHandleLine.Add(nHandle_dw2);			
									long nHandleThisDC=nHandleDC2;
									nArrayHandleThisDaocha.Add(nHandleThisDC);
								}
							}
						}
						else
						{   //反位线不为渡线时	
							nArrayHandleLine.Add(nHandle_fw);
							//long nHandleThisDC=0;
							//nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
							//反位线不为渡线
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
						}
					}
					//	acutPrintf(_T("Isstart=%d"),IsStart);
				}
			}
			//所有道岔为定位时
			if(nHandleNextLine!=0)
			{
				long nHandleThisDC=0;
				nArrayHandleThisDaocha.Add(nHandleThisDC);
				nArrayHandleLine.Add(nHandleNextLine);
			}
			else
			{
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
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
		AfxMessageBox(_T("未知的错误!in DiaoCheFindNextLineAndDaocha"));
	}	

}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
/*******************函数说明:FindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********功能：通过一条进路的起始轨道和终止轨道，找出起始轨道的后继轨道，并返回从起始轨道到下一条轨道中走过的道岔以及属于进路上的信号机，后继轨道可能有多条*******************************************************************/
/**********输入：nHandleLineStart,进路的起始轨道线；nHandLineEnd,设定进路的终止轨道线，用于终止进路寻找过程*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔；bForward,用于标记寻找后继防线，true为向右推进***************************************************************************/
/*****************Gudao，股道名称，可以用于终止继续寻找；JieJinQuDuan，接近区段名称;******************************************************************************************************/
/**********输出：GuoDaocha,用于记录在寻找进路的过程中走过的道岔,属于联锁表中的一项内容； nArrayHandleThisDaocha，到下一条进路时走过相应道岔*************************************/
/*****************nArrayHandleNextLine,返回所有的后继轨道区段;  GuoXHJ,到一条进路时走过的信号机，信号机属于进路内部*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray&strCXGuiDaoName, bool bForword/*=true*/ )
{
	try
	{
		//long t1=GetTickCount();
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
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
		//调车信号机遇到股道时结束
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			return;
		}
		else
		{
			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
// 			long t2=GetTickCount();
// 			CString str;
// 			str.Format("DiaoCheFindNextLineAndXHJ time:%d ms",t2-t1);
// 			acutPrintf(_T("\n %s"),str);
			return ;
		}
		//判断轨道上DC个数
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
				CLongArray nArrayPassXHJ;
				bool btoward=bForword;
				this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
				CString  Name_PassXHJ;
				for(int k=0;k<nArrayPassXHJ.GetCount();k++)
				{
					CString name_xhj;
					GetEntName(nArrayPassXHJ[k],name_xhj);
					//对于调车来说，应该只包括对向的调车信号机
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");
				}
				GuoXHJ.Add(Name_PassXHJ);
				return;
			}
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					//增加超限区段
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
					//获取轨道左端信号机
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
						//acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
									//道岔反位时，需要考虑是否存在带动道岔
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
						{   //反位线不为渡线时
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
							//道岔反位时，需要考虑是否存在带动道岔
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
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
									//道岔反位时，需要考虑是否存在带动道岔
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
							//反位线不为渡线
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							//acutPrintf(_T("\n遇到交叉渡线3"));
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
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}
			//所有道岔为定位时
			// 			long nHandleThisDC=0;
			// 			nArrayHandleThisDaocha.Add(nHandleThisDC);
			// 			nArrayHandleLine.Add(nHandleNextLine);
			// 			CLongArray nPassXHJ;
			// 			CString  Name_PassXHJ=_T("");
			// 			//GetBlockPosition_X(nHandleThisDC,pointx);
			// 			//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
			// 			//所有道岔为定位时后继
			// 			int b_toward=(bForword);
			// 			GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
			// 			for(int k=0;k<nPassXHJ.GetCount();k++)
			// 			{
			// 				CString name;
			// 				GetEntName(nPassXHJ[k],name);
			// 				Name_PassXHJ=Name_PassXHJ+name+_T("，");
			// 			}
			// 			GuoXHJ.Add(Name_PassXHJ);
			// 			GuoDaoCha.Add(DaochaD);
			// 	}
			//所有道岔为定位时
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
				//所有道岔为定位时后继
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
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
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
						// 						   //道岔反位时，需要考虑是否存在带动道岔
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
						//道岔反位时，需要考虑是否存在带动道岔
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
/*******************函数说明:LieCheFindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********功能：通过一条进路的起始轨道和终止轨道，找出起始轨道的后继轨道，并返回从起始轨道到下一条轨道中走过的道岔以及属于进路上的信号机，后继轨道可能有多条*******************************************************************/
/**********输入：nHandleLineStart,进路的起始轨道线；nHandLineEnd,设定进路的终止轨道线，用于终止进路寻找过程*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔；bForward,用于标记寻找后继防线，true为向右推进***************************************************************************/
/*****************Gudao，股道名称，可以用于终止继续寻找；JieJinQuDuan，接近区段名称;******************************************************************************************************/
/**********输出：GuoDaocha,用于记录在寻找进路的过程中走过的道岔,属于联锁表中的一项内容； nArrayHandleThisDaocha，到下一条进路时走过相应道岔*************************************/
/*****************nArrayHandleNextLine,返回所有的后继轨道区段;  GuoXHJ,到一条进路时走过的信号机，信号机属于进路内部*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
/*说明：专门用于发车进路后继区段寻找，将发车信号机写作发车兼调车信号机*/
void CZnSxt::FaCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray &strCXGuiDaoName,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
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
				//将发车信号机写作兼调车信号机
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
		//列车与调车的区别，调车遇到Gudao必须终止
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
	//	int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
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
					//将发车信号机写作兼调车信号机
					if(EntName.Left(6)==_T("XHJ_LF"))
					{
						name_xhj+=_T("D");
					}
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T(",");;
				}
				return;
			}
			if(nLineType==0)
			{//不为渡线时
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
					//获取轨道左端信号机
					bool btoward=bForword;
					this->GetGuiDaoDuanXHJ(nHandleNextLine,btoward,nArrayPassXHJ);
					CString  Name_PassXHJ;
					for(int k=0;k<nArrayPassXHJ.GetCount();k++)
					{
						CString name_xhj;
						GetEntName(nArrayPassXHJ[k],name_xhj);
						CString EntName;
						GetEntBlkNameByHandle(nArrayPassXHJ[k],EntName);
						//将发车信号机写作兼调车信号机
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
						//acutPrintf(_T("\n nHandNextLine为0"));
						//该线是否仍然为某道岔的反位线
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
									//道岔反位时，需要考虑是否存在带动道岔
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
						{   //反位线不为渡线时
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
							//道岔反位时，需要考虑是否存在带动道岔
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
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
									//道岔反位时，需要考虑是否存在带动道岔
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
							//反位线不为渡线
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							//acutPrintf(_T("\n遇到交叉渡线3"));
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
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}//endfor
			//所有道岔为定位时
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
				//所有道岔为定位时后继
				int b_toward=(bForword);
				GetGuiDaoDuanXHJ(nHandleNextLine,b_toward,nPassXHJ);
				for(int k=0;k<nPassXHJ.GetCount();k++)
				{
					CString name;
					GetEntName(nPassXHJ[k],name);
					CString EntName;
					GetEntBlkNameByHandle(nPassXHJ[k],EntName);
					//将发车信号机写作兼调车信号机
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
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
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
						// 						   //道岔反位时，需要考虑是否存在带动道岔
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
						//道岔反位时，需要考虑是否存在带动道岔
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
void CZnSxt::LieCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray &strCXGuiDaoName,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
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
// 				//将发车信号机写作兼调车信号机
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
		//列车与调车的区别，调车遇到Gudao必须终止
		// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		// 		{
		// 			//如果起始位股道，则结束
		// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
		// 			return;
		// 		}
		// 		else
		// 		{
		// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		//	int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
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
			{//不为渡线时
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
					//获取轨道左端信号机
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
						//acutPrintf(_T("\n nHandNextLine为0"));
						//该线是否仍然为某道岔的反位线
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
									//道岔反位时，需要考虑是否存在带动道岔
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
						{   //反位线不为渡线时
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
							//道岔反位时，需要考虑是否存在带动道岔
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T(",")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
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
									//道岔反位时，需要考虑是否存在带动道岔
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
							//反位线不为渡线
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							//acutPrintf(_T("\n遇到交叉渡线3"));
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
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T(",");
						}

					}
				}
			}//endfor
			//所有道岔为定位时
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
				//所有道岔为定位时后继
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
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
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
						// 						   //道岔反位时，需要考虑是否存在带动道岔
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
						//道岔反位时，需要考虑是否存在带动道岔
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//该函数中添加记录上次访问过的道岔，主要用于在寻找后继的时候，只将属于进路的道岔写入GuoDaoCha,LastDaocha用于记录上次到nHandleLineStart的道岔，nArrayHandleThisDaocha,用于记录这次最后访问的道岔号
//在原基础上加入轨道区段上的信号机，包括同方向信号机和背向信号机，信号机可能为调车信号机，也可能为列车信号机
/*******************函数说明:FindNextLineAndXHJ*******************************************************************************************************************************************************************/
/**********功能：通过一条进路的起始轨道和终止轨道，找出起始轨道的后继轨道，并返回从起始轨道到下一条轨道中走过的道岔以及属于进路上的信号机，后继轨道可能有多条*******************************************************************/
/**********输入：nHandleLineStart,进路的起始轨道线；nHandLineEnd,设定进路的终止轨道线，用于终止进路寻找过程*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔；bForward,用于标记寻找后继防线，true为向右推进***************************************************************************/
/*****************Gudao，股道名称，可以用于终止继续寻找；JieJinQuDuan，接近区段名称;******************************************************************************************************/
/**********输出：GuoDaocha,用于记录在寻找进路的过程中走过的道岔,属于联锁表中的一项内容； nArrayHandleThisDaocha，到下一条进路时走过相应道岔*************************************/
/*****************nArrayHandleNextLine,返回所有的后继轨道区段;  GuoXHJ,到一条进路时走过的信号机，信号机属于进路内部*******************************************************************/
/**********************************2015-06-03 am 11:06 by luo*************************************************************************************************/
void CZnSxt::FindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
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
				Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");
			}
			GuoXHJ.Add(Name_PassXHJ);
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		//列车与调车的区别，调车遇到Gudao必须终止
		// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		// 		{
		// 			//如果起始位股道，则结束
		// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
		// 			return;
		// 		}
		// 		else
		// 		{
		// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		// 		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nHandleLineStart==nHandleLineEnd)
			{//找到，结束
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
					Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");
				}
				return;
			}
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CLongArray nArrayPassXHJ;
					//this->GetGuiDaoXHJ(nHandleNextLine,nArrayPassXHJ);
					//获取轨道左端信号机
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
						Name_PassXHJ=Name_PassXHJ+name_xhj+_T("，");
					}
					// 					CString str_nametemp;
					// 					m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),str_nametemp);
					// 					acutPrintf(_T("\n %s轨道的端信号机为%s"),str_nametemp.GetString(),Name_PassXHJ.GetString());
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
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

							}
							else
							{
								// Daocha=_T("(")+DCname+_T(")");

							}
							//CString Daocha=_T("(")+DCname+_T(")");
							// 						   //道岔反位时，需要考虑是否存在带动道岔
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
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
						//acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			//查找上次访问的道岔位置
			int index_lastDaocha=-1;
			double d_x=0;
			if(LastDaocha!=0)
			{
				index_lastDaocha=CBlkUtility::FindLongInArray(nArrayHandleDC,LastDaocha);
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), LastDaocha) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),d_x) ;
			}
			for(int k=index_lastDaocha+1;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHandleDC2,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线1"));
									//需要考虑是否有带动道岔存在
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
									//道岔反位时，需要考虑是否存在带动道岔
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
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						{   //反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
							long nHandleThisDC=0;
							nArrayHandleThisDaocha.Add(nHandleThisDC);
							double pointx;
							CLongArray nPassXHJ;
							CString  Name_PassXHJ=_T("");
							GuoXHJ.Add(Name_PassXHJ);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
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
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
									Daocha=Daocha+DCname_nMax+_T(")");
									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
									//acutPrintf(_T("\n遇到交叉渡线2"));
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
									//道岔反位时，需要考虑是否存在带动道岔
									long nHandleDaiDong_nMin=0;
									long nHandleDaiDong_nMax=0;
									CString DaiDong_MinName;
									CString DaiDong_MaxName;
									if(GetDaidongDaochas1(nHandleDC,nHandleDaiDong_nMin,nHandleDaiDong_nMax)==TRUE)
									{
										// Daocha=_T("(")+DCname+_T(")");
										GetEntName(nHandleDaiDong_nMin,DaiDong_MinName);
										GetEntName(nHandleDaiDong_nMax,DaiDong_MaxName);
										Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");
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
							//反位线不为渡线
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
								Daocha=Daocha+_T("，")+_T("{")+DaiDong_MinName+_T("/")+DaiDong_MaxName+_T("}");

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
						DaochaD=DaochaD+DCname1+_T("，");
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
							//acutPrintf(_T("\n遇到交叉渡线3"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
							//acutPrintf(_T("\n遇到交叉渡线"));
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
						}

					}
				}
			}
			//所有道岔为定位时
			long nHandleThisDC=0;
			nArrayHandleThisDaocha.Add(nHandleThisDC);
			nArrayHandleLine.Add(nHandleNextLine);
			CLongArray nPassXHJ;
			CString  Name_PassXHJ=_T("");
			//GetBlockPosition_X(nHandleThisDC,pointx);
			//GetGuiDaoXHJ(nHandleNextLine,nPassXHJ);
			//所有道岔为定位时后继
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
				Name_PassXHJ=Name_PassXHJ+name+_T("，");
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//记录访问过的道岔，避免走回路，顺便记录走过道岔的定反位，用于联锁表,只用于寻找后继，无其他功能，开销较小
void CZnSxt::FindNextLine( long nHandleLineStart,long nHandleLineEnd,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd||nHandleLineStart==0)
		{//找到，结束
			return;
		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		CADORecordset cSet(&m_AdoDb);
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					// 					CString Daocha=_T("");
					// 					GuoDaoCha.Add(Daocha);
				}
				else
				{
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
							nArrayHandleLine.Add(nHandle_dw);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLine为0"));
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
			//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			for(int k=0;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						else
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						//	acutPrintf(_T("Isstart=%d"),IsStart);
						else
						{//反位线不为渡线时
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
				
				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
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
						//定位所在线为后继
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
//用PassDaocha记录去往每条后继的道岔号，GuoDaocha还会详细记录走过道岔的定反位
void CZnSxt::FindNextLine4( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &PassDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd)
		{//找到，结束
			return;
		}
		CString GuidaoName;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
		{
			//如果起始位股道，则结束
			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
			return;
		}
		else
		{
			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
		}
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
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
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		CString guiname;
		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					CString Daocha=_T("");
					GuoDaoCha.Add(Daocha);
					PassDaoCha.Add(Daocha);
				}
				else
				{
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
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
							//定位所在线为后继
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("（")+DCname+_T(")");
							// 						   //道岔反位时，需要考虑是否存在带动道岔
							//GuoDaoCha.Add(Daocha);	
							CString DaoChaNum=DCname+_T("_");
							PassDaoCha.Add(DaoChaNum);
							GuoDaoCha.Add(Daocha);

						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
							CString Daocha=_T("（")+DCname+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
							CString DaoChaNum=DCname+_T("_");
							PassDaoCha.Add(DaoChaNum);
							GuoDaoCha.Add(Daocha);
						}
					}
					else
					{
						acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			CString DaoChaDNum;
			for(int k=0;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
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
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
								long nHandle_Anothe_Min=0;
								long nHandle_Anothe_Max=0;
								//判断渡线道岔是否属于交叉渡线的道岔
								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
								{
									CString DC_Another_Min;
									CString DC_Another_Max;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
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
									//道岔反位时，需要考虑是否存在带动道岔
									//GuoDaoCha.Add(Daocha);
									CString DaoChaNum=DaoChaDNum+DCname_nMin+_T("_")+DCname_nMax+_T("_");
									PassDaoCha.Add(DaoChaNum);
									GuoDaoCha.Add(Daocha);
								}						
							}
						}
						else
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
							//道岔反位时，需要考虑是否存在带动道岔
							//GuoDaoCha.Add(Daocha);
							CString DaoChaNum=DaoChaDNum+DCname1+_T("_");
							PassDaoCha.Add(DaoChaNum);	
							GuoDaoCha.Add(Daocha);			
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
								//打印渡线道岔
								CString DCname_nMin;
								CString DCname_nMax;
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
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
									//道岔反位时，需要考虑是否存在带动道岔
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
						DaochaD=DaochaD+DCname1+_T("，");
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
					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;	
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
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;	
							DaoChaDNum=DaoChaDNum+DCname_Min+_T("_")+DCname_Max+_T("_")+DC_Another_Min+_T("_")+DC_Another_Max+_T("_");

							//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
						}
						else
						{
							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
// 记录访问过的道岔，避免走回路，顺便记录走过道岔的定反位，用于联锁表,加入带动道岔测试
// void CZnSxt::FindNextLine5( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
// {
// 	try
// 	{
// 		// static int stop =0;
// 		if(nHandleLineStart==nHandleLineEnd)
// 		{//找到，结束
// 			return;
// 		}
// 		CString GuidaoName;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
// 		if(CBlkUtility::FindStrInArray(Gudao,GuidaoName.GetString())!=-1)
// 		{
// 			//如果起始位股道，则结束
// 			//acutPrintf(_T("股道名称=%s"),GuidaoName.GetString());
// 			return;
// 		}
// 		else
// 		{
// 			//acutPrintf(_T("轨道名称=%s"),GuidaoName.GetString());
// 		}
// 		CADORecordset cSet(&m_AdoDb) ;
// 		CString strSql ;
// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
// 		long nLineType = 0 ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 		//表示为尽头线
// 		if (nLineType==2) 
// 		{
// 			CString name;
// 			m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
// 			acutPrintf(_T("\n 尽头线：%s"),name.GetString());
// 			return ;
// 		}
// 		//判断轨道上DC个数
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//获取轨道信息，这个函数需要仔细看看
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
// 		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
// 		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 		CString guiname;
// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
// 		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//如果是向右寻找，则找右端绝缘节。
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//读取结果集
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//轨道不包含道岔时，后继只有一个
// 			if(nLineType==0)
// 			{//不为渡线时
// 				if(nHandleNextLine!=0)
// 				{
// 					nArrayHandleLine.Add(nHandleNextLine);
// 					CString Daocha=_T("");
// 					GuoDaoCha.Add(Daocha);
// 				}
// 				else
// 				{
// 					//还有可能当前线为某一个道岔的反位线
// 					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 					//反位线对应的道岔
// 					long nHandleDC=0;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 					if(nHandleDC!=0)
// 					{
// 						//查询道岔所在的线，该线为后继
// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 						//反位线对应的道岔
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
// 							//定位所在线为后继
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("（")+DCname+_T(")");
// 							// 						   //道岔反位时，需要考虑是否存在带动道岔
// 							GuoDaoCha.Add(Daocha);
// 						}
// 						else if(bForword==false&&IsStart==0)
// 						{
// 							nArrayHandleLine.Add(nHandle_dw);
// 							CString Daocha=_T("（")+DCname+_T(")");
// 							//道岔反位时，需要考虑是否存在带动道岔
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 					else
// 					{
// 						acutPrintf(_T("\n nHandNextLine为0"));
// 					}
// 				}
// 			}
// 			// 			else if(nLineType==1)
// 			// 			{
// 			// 				//为渡线时
// 			// 				acutPrintf(_T("\n该线为渡线"));
// 			// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 			// 				CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			// 				//读取结果集
// 			// 				//	m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 			// 				//渡线绝缘节在右侧时
// 			// 				if(nHandleLeftJyj==0&&nHandleRightJyj!=0)
// 			// 				{
// 			// 					//找渡线绝缘节在左侧的渡线
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
// 			// 							acutPrintf(_T("\n nHandNextLine为0"));
// 			// 						}
// 			// 					}
// 			// 					else 
// 			// 					{
// 			// 						//后继为道岔定位所在轨道
// 			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			// 						//反位线对应的道岔
// 			// 						long nHandleDC=0;
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 			// 						//道岔所在轨道即为后继轨道
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 			// 						if(nHandleNextLine!=0)
// 			// 						{
// 			// 							nArrayHandleLine.Add(nHandleNextLine);
// 			// 						}
// 			// 						else
// 			// 						{
// 			// 							acutPrintf(_T("\n nHandNextLine为0"));
// 			// 						}
// 			// 					}
// 			// 				}
// 			// 				//渡线绝缘节在做左侧时
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
// 			// 						////后继为道岔定位所在轨道
// 			// 						strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
// 			// 						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 			// 						//反位线对应的道岔
// 			// 						long nHandleDC=0;
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
// 			// 						//道岔所在轨道即为后继轨道
// 			// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleNextLine);
// 			// 						if(nHandleNextLine!=0)
// 			// 						{
// 			// 							nArrayHandleLine.Add(nHandleNextLine);
// 			// 						}
// 			// 						else
// 			// 						{
// 			// 							acutPrintf(_T("\n nHandNextLine为0"));
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
// 		{//含有道岔时，可能存在多条后继
// 			//道岔为定位时后继
// 			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
// 			CString DaochaD;
// 			for(int k=0;k<iNumDC;k++)
// 			{//道岔反位时后继,需要前面访问的道岔
// 				//取道岔handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//查询反位所在线
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
// 				//向右推进有后继时
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//向右推进时起点
// 					if(nHandle_fw!=0)
// 					{
// 						//继续往下求一次后继，判断是否为渡线
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//为渡线时，求反位线的后继
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//如果是向右寻找，则找右端绝缘节。
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//读取结果集
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//再找该反位线所在道岔的定位线
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//反位线对应的道岔
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//查询道岔所在的线，该线为后继
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//反位线对应的道岔
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2即为渡线的后继
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//打印渡线道岔
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
// 								long nHandle_Anothe_Min=0;
// 								long nHandle_Anothe_Max=0;
// 								//判断渡线道岔是否属于交叉渡线的道岔
// 								if(GetJiaoChaDuXianAnotherDaoChas(nHandleDC,nHanderDC_another,nHandle_Anothe_Min,nHandle_Anothe_Max)==TRUE)
// 								{
// 									CString DC_Another_Min;
// 									CString DC_Another_Max;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Min) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Min) ;
// 									strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_Anothe_Max) ;
// 									m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DC_Another_Max) ;
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//道岔反位时，需要考虑是否存在带动道岔
// 									GuoDaoCha.Add(Daocha);
// 								}						
// 							}
// 						}
// 						else
// 						{//反位线不为渡线时
// 							nArrayHandleLine.Add(nHandle_fw);
// 							CString Daocha=DaochaD+_T("(")+DCname1+_T(")");
// 							//道岔反位时，需要考虑是否存在带动道岔
// 							GuoDaoCha.Add(Daocha);
// 						}
// 					}
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//向左推进时起点
// 					if(nHandle_fw!=0)
// 					{
// 						//nArrayHandleLine.Add(nHandle_fw);
// 						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
// 						long nLineType = 0 ;
// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 						if(nLineType==1)
// 						{
// 							//为渡线时，求反位线的后继
// 							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
// 							//如果是向右寻找，则找右端绝缘节。
// 							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
// 							long nHandleNextLine2 = 0 ;
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//读取结果集
// 							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
// 							//再找该反位线所在道岔的定位线
// 							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
// 							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 							//反位线对应的道岔
// 							long nHandleDC2=0;
// 							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
// 							if(nHandleDC2!=0)
// 							{
// 								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
// 								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
// 								//查询道岔所在的线，该线为后继
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
// 								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 								//反位线对应的道岔
// 								long nHandle_dw2=0;				
// 								//int IsStart=-1;
// 								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
// 								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 								//nHandle_dw2即为渡线的后继
// 								if(nHandle_dw2!=0)
// 									nArrayHandleLine.Add(nHandle_dw2);
// 								//打印渡线道岔
// 								CString DCname_nMin;
// 								CString DCname_nMax;
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMinDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMin);
// 								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nMaxDc) ;
// 								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), DCname_nMax);
// 								//acutPrintf(_T("\n渡线道岔反位：(%s/%s)"),Daocha1.GetString(),Daocha2.GetString());
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
// 									//DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									Daocha=Daocha+_T("，")+_T("[")+DC_Another_Min+_T("/")+DC_Another_Max+_T("]");
// 									GuoDaoCha.Add(Daocha);
// 								}
// 								else
// 								{
// 									CString Daocha=DaochaD+_T("(")+DCname_nMin+_T("/");
// 									Daocha=Daocha+DCname_nMax+_T(")");
// 									//道岔反位时，需要考虑是否存在带动道岔
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
// 							// 							//Daocha=Daocha+_T("，")+_T("{")+DDname1+_T("/")+DDname2+_T("}");
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
// 							// 								//Daocha=Daocha+_T("，")+_T("{")+DDname1+_T("/")+DDname2+_T("}");
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
// 						DaochaD=DaochaD+DCname1+_T("，");
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
// 					//acutPrintf(_T("\n%s另一个道岔为%s %ld"),DCname1.GetString(),DCname_Anthor.GetString(),nHanderDC_another);
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
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max;
// 
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，")+DC_Another_Min+_T("/")+DC_Another_Max+_T("，");
// 						}
// 						else
// 						{
// 							DaochaD =DaochaD+DCname_Min+_T("/")+DCname_Max+_T("，");
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
// 		AfxMessageBox(_T("未知的错误!in FindNextLine"));
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
// 		//判断轨道上DC个数
// 		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
// 		//获取轨道信息，这个函数需要仔细看看
// 		this->GetGuiDaoInfo(nHandleLine, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
// 		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
// 		int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
// 		int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
// 		acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道区段=%d"),iNumDC,iNumXHJ,iNumLine);
// 		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
// 		//如果是向右寻找，则找右端绝缘节。
// 		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
// 		long nHandleNextLine = 0 ;
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
// 		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
// 		//读取结果集
// 		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
// 		if(iNumDC<1)
// 		{
// 			//股道不包含道岔时，后继只有一个
// 			nArrayHandleLine.Add(nHandleNextLine);
// 		    //return;
// 		}
// 		else
// 		{
// 			nArrayHandleLine.Add(nHandleNextLine);
// 			//判断道岔类型，渡线，交叉渡线，单动道岔
// 			for(int k=0;k<iNumDC;k++)
// 			{
// 				//取道岔handle
// 				long nHandleDC=nArrayHandleDC[k];
// 				//查询定位所在线
// 				//查询反位所在线
// 				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
// 				long nHandle_fw;
// 				long nHandle_dw;
// 				int IsStart=-1;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
// 				if(IsStart==0&&bForword==true)
// 				{
// 					//向有推进时起点
// 					nArrayHandleLine.Add(nHandle_fw);
// 					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLine) ;
// 					long nLineType = 0 ;
// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
// 					acutPrintf(_T("\n nlineType=%d"),nLineType);
// 				}
// 				else if(IsStart>0&&bForword==false)
// 				{
// 					//向左推进时起点
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
// 		AfxMessageBox(_T("未知的错误!in FindNextLine"));
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
		//遍历并打开实体 ;	
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
		//遍历并打开实体 ;	
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
				//向上
				iToward=0;
			}
			else if(fabs(res)<=1.0E-6&&cnt%2==1)
			{
				//向下；
				iToward=3;
			}
			else if(res<1.0E-6&&cnt%2==0)
			{
				//朝右
				iToward=1;
			}
			else if(res<1.0E-6&&cnt%2==1)
			{
				iToward=2;
			}
			else if(res>1.0E-6&&cnt%2==0)
			{
				//朝左
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
		//遍历并打开实体 ;	
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
		CString strFilter = _T("(文件(*.xls)|*.xls|");
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, strFilter);
		fileDlg.m_ofn.lpstrTitle = _T("输出联锁表至");
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
	 	AfxMessageBox(_T("未知的错误!in SelectFilePath"));
		return FALSE;
 	}		 	
	return FALSE;
}
void CZnSxt::LsbTest2()
{
	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
	CLongArray nArrayHandle_LJ_XHJ;
	//获取所有接车信号机
	int iRet =GetAll_LJ_XHJ(nArrayHandle_LJ_XHJ);
	CLongArray nArrayHandle_LF_XHJ;
	//获取所有出站信号机
	CString name_xhj=_T("XHJ_LF");
	int iRet2 =GetSomeBlocks(name_xhj,nArrayHandle_LF_XHJ);
	long nHandleLinetemp=0;
	long nHandleLinetemp2=0;
	acutPrintf(_T("\n 出站信号机个数=%d"),iRet2);
	CString strFilePath ;
	strFilePath.Format(_T("%s\\support\\xls\\平面图联锁表模板.xls"), g_strBasePath);
	oper_excel.OpenExcel(strFilePath);
	CString strFileSave;
	if(FALSE==this->SelectFilePath(strFileSave))
	{
		AfxMessageBox(_T("指定的文件路径无效！"));
		return ;
	}
	//清空excel表
	oper_excel.ClearAllCells(1);
	CTime  time_beg=CTime::GetCurrentTime();
	CString str_Time=time_beg.Format("%Y-%m-%d %H:%M:%S");
	this->iJLnum=1;
	//获取股道所在轨道
	//CLongArray nAarryHandleGuDaoS;
	//CLongArray nAarryHandleGuDaoX;
	//下行进站区段
// 	CLongArray nAarryHandleXJC;
// 	//下行接近进站区段
// 	CLongArray nAarryHandleXJJC;
// 	CLongArray nAarryHandleSJC;
// 	CLongArray nAarryHandleSJJC;
	CStringArray sXJCGuiDaoName;
	CStringArray sSJCGuiDaoName;
	//股道名称
	//CStringArray str_GuDaoName;
	//上行股道名称
	//CStringArray sSGuDaoName;
	//下行股道名称
	//CStringArray sXGuDaoName;
	//求超限绝缘
	FindALLChaoXianGuiDao(this->nArrayHandLineCX,this->strArrayGuiDaoCX);
	int length =nArrayHandle_LJ_XHJ.GetCount();
	if(length<1)
	{
		//acutPrintf(_T("\n 信号机个数为%d"),length);
		//return;
	}
	//打印进站信号机名称
	for(int i=0;i<length;i++)
	{
		CString entname;
		long nHandleLine =nArrayHandle_LJ_XHJ[i];
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
		acutPrintf(_T("\n 进站信号机名称为%s"),entname.GetString());
		//查询进站信号机右侧直线
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
		//寻找后一条轨道
		CLongArray nArrayHandle_GD_Next;
		//上下行接近区段为最外方进站轨道
		//获取下行进站信号机左侧轨道
		if(entname.Left(1)==_T("X"))
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
			CString name;
			m_mysys.GetXdata(nHandleLinetemp,_T("GUIDAONAME"),name);
			sXJCGuiDaoName.Add(name);
			acutPrintf(_T("X进站轨道为：%s"),name.GetString());
			nAarryHandleXJC.Add(nHandleLinetemp);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
			nAarryHandleXJJC.Add(nHandleLinetemp);
		}
		else
		{
			//获取上行进站信号机右侧轨道
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
			CString name;
			m_mysys.GetXdata(nHandleLinetemp,_T("GUIDAONAME"),name);
			sSJCGuiDaoName.Add(name);
			acutPrintf(_T("S进站轨道为：%s"),name.GetString());
			nAarryHandleSJC.Add(nHandleLinetemp);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
			nAarryHandleSJJC.Add(nHandleLinetemp);
		}
	}
	//打印出站信号机名称
	//CLongArray nArrayHandleLineGuDao;
	for(int i=0;i<iRet2;i++)
	{
		CString entname;
		long nHandleLine =nArrayHandle_LF_XHJ[i];
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
		acutPrintf(_T("\n 出站信号机名称为%s"),entname.GetString());
		//查询进站信号机右侧直线
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
		//获取信号机右侧轨道,即为股道
		if(entname.Left(1)==_T("S"))
		{
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp2) ;
			nAarryHandleGuDaoS.Add(nHandleLinetemp2);
			//打印股道名称
			if(nHandleLinetemp2!=0)
			{
				CString na;
				//获取股道名称，应该以信号机名称来命名
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
		acutPrintf(_T("\n 下行股道包括：%s\t"),sXGuDaoName.GetAt(i).GetString());
		acutPrintf(_T("\n股道为：%s\t"),this->str_GuDaoName.GetAt(i).GetString());
	}
	CBlkUtility::SortStringArray_CZXHJHandle(sSGuDaoName,_T('G'),nAarryHandleGuDaoS);
	for(int i=0;i<sSGuDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n 上行股道包括：%s\t"),sSGuDaoName.GetAt(i).GetString());
	}
	for(int i=0;i<sXJCGuiDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n 下行接近区段包括：%s\t"),sXJCGuiDaoName.GetAt(i).GetString());
	}
	for(int i=0;i<sSJCGuiDaoName.GetCount();i++)
	{
		acutPrintf(_T("\n 上行接近区段包括：%s\t"),sSJCGuiDaoName.GetAt(i).GetString());
	}
	//return;
	// 		输出所有下行接车进路
		acutPrintf(_T("\n开始输出下行接车进路\n"));
		for(int i=0;i<nAarryHandleXJC.GetCount();i++)
		{
			long nHandleStart=nAarryHandleXJC[i];
			CString Start_Gd_Name;
			//m_mysys.GetXdata(nHandleStart,Start_Gd_Name);
			m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
			CString str_fx;
		    int i_excel_row_current=this->num;
			//查询通过进路
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
				//获取终端信号机，判断是否该写通过进路
				GetEntName(nxhj_end,str_fx);
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),str_GdStartname);
				if(str_fx.Right(1)==_T("N"))
				{
					
				
					GetTongGuoPathGuiDaoAndXHJ(str_GdStartname,nxhj_start,nxhj_end,nHandleStart,nHandleStart,nHandleTjEnd,nlastdaocha,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleLineTJ,vec_cxgd,i_CntTJ,true);
					this->num=this->num-i_CntTJ*2;
					for(int j=0;j<i_CntTJ;j++)
					{
						//通过进路，且为正线通过
						for(int i_col=3;i_col<=17;i_col++)
						{
							//oper_excel.SetColumnCombandCells(1,this->num,i_col,2);
							if(i_col==3)
							{
								oper_excel.SetColumnCombandCellsAndCenterShow(1,this->num,i_col,2);
								oper_excel.WriteDatatoExcel(1,this->num,3,_T("通过"));
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
					//获取终止轨道信号机
					long nHandleLineXHJEnd=nAarryHandleGuDaoS[k];
					this->GetGuiDaoDuanXHJ(nHandleStart,btoward,nArrayJinZhanXHJ);
					if(nArrayJinZhanXHJ.GetCount()>0)
					nHandleStartXHJ=nArrayJinZhanXHJ[0];
					//获取起始信号机名称，作为运行方面的方向
					GetEntName(nHandleStartXHJ,str_fx);
					this->GetGuiDaoDuanXHJ(nHandleLineXHJEnd,btoward,nArrayChuZhanXHJ);
					if(nArrayChuZhanXHJ.GetCount()>0)
					nHandleEndXHJ =nArrayChuZhanXHJ[0];
					int iJL_Cnt_temp=0;
					GetJieChePathGuiDaoAndXHJ(Start_Gd_Name,nHandleStartXHJ,nHandleEndXHJ,nHandleStart,nHandleStart,nHandleEnd,LastDaocha,path,daocha,xhj,sXGuDaoName,nArrayHandle,vec_cx_gd,iJL_Cnt_temp,btoward);
					//写确定运行方向道岔
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
			oper_excel.WriteDatatoExcel(1,i_excel_row_current1,3,_T("接车"));
			//发车进路
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
				//获取终止轨道信号机
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
			oper_excel.WriteDatatoExcel(1,i_excel_row_current2,3,_T("发车"));		
			int i_excel_row_current4=this->num;
			//写列车方面
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current,2,i_excel_row_current4-i_excel_row_current);
			//CString str_T=_T("北京方面");
			//str_T+=_T(" ");
			str_fx=_T("北京方面 ")+str_fx;
			oper_excel.WriteDatatoExcel(1,i_excel_row_current,2,str_fx);
		}
		//输出所有上行接车进路
		acutPrintf(_T("\n开始输出上行接车进路\n"));
		for(int i=0;i<nAarryHandleSJC.GetCount();i++)
		{
			long nHandleStart=nAarryHandleSJC[i];
			CString Start_Gd_Name;
			m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),Start_Gd_Name);
			CString str_fx;
			int i_excel_row_current=this->num;
			//查询通过进路
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
				//获取终端信号机，判断是否该写通过进路
				GetEntName(nxhj_end,str_fx);
				m_mysys.GetXdata(nHandleStart,_T("GUIDAONAME"),str_GdStartname);
				if(str_fx.Right(1)==_T("N"))
				{
					GetTongGuoPathGuiDaoAndXHJ(str_GdStartname,nxhj_start,nxhj_end,nHandleStart,nHandleStart,nHandleTjEnd,nlastdaocha,vec_path,vec_daocha,vec_xhj,sXGuDaoName,nArrayHandleLineTJ,vec_cxgd,i_CntTJ,false);
					this->num=this->num-i_CntTJ*2;
					for(int j=0;j<i_CntTJ;j++)
					{
						//通过进路，且为正线通过，此时包含通过进路
						for(int i_col=3;i_col<=17;i_col++)
						{
							//oper_excel.SetColumnCombandCells(1,this->num,i_col,2);
							if(i_col==3)
							{
								oper_excel.SetColumnCombandCellsAndCenterShow(1,this->num,i_col,2);
								oper_excel.WriteDatatoExcel(1,this->num,3,_T("通过"));
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
				//获取终止轨道信号机
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
				//写确定运行方向道岔
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
			oper_excel.WriteDatatoExcel(1,i_excel_row_current1,3,_T("接车"));
			//发车
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
				//获取终止轨道信号机
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
			oper_excel.WriteDatatoExcel(1,i_excel_row_current2,3,_T("发车"));
			//this->num=this->num+2;
			int i_excel_row_current4=this->num;
			//写方面
			//CString str_T=_T("广州方面");
			//str_T+=_T(" ");
			str_fx=_T("广州方面 ")+str_fx;
			oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_current,2,i_excel_row_current4-i_excel_row_current);
			oper_excel.WriteDatatoExcel(1,i_excel_row_current,2,str_fx);
		}
	  	//列车进路结束
		int i_excel_row_lieche=this->num;
		oper_excel.SetColumnCombandCellsAndCenterShow(1,3,1,i_excel_row_lieche-3);
		oper_excel.WriteDatatoExcel(1,3,1,_T("列车进路"));
	//获取所有的调车信号机
	CLongArray nArrayHandle_DCXHJ;
	CStringArray JieJinQuDuan;
	JieJinQuDuan.Copy(sSJCGuiDaoName);
	//JieJinQuDuan.Copy(sXJCGuiDaoName);
	for(int i=0;i<sXJCGuiDaoName.GetCount();i++)
	{
		//acutPrintf(_T("\t接近区段:%s"),JieJinQuDuan.GetAt(i));
		JieJinQuDuan.Add(sXJCGuiDaoName.GetAt(i));
	}
	CString name_DCXHJ=_T("XHJ_DC");
	int iRet3 =GetSomeBlocks(name_DCXHJ,nArrayHandle_DCXHJ);
	int iNum_DC=nArrayHandle_DCXHJ.GetCount();
	acutPrintf(_T("\n 一共有%d个调车信号机"),iNum_DC);
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
			fangxiang=_T("右");
		}
		else if(iTo==2)
		{
			fangxiang=_T("左");
		}
		else if(iTo==0)
		{
			fangxiang=_T("上");
		}
		else if(iTo==3)
		{
			fangxiang=_T("下");
		}
		else 
		{
			fangxiang=_T("未知");
		}
		//long LastDaocha=0;
		GetEntName(nHandleDCXHJ,name);
		acutPrintf(_T("\n信号机%s:%s"),name,fangxiang);
		strArrayDcName.Add(name);
	}
	//排序后输出
	TCHAR del_char=_T('D');
	long  t31=GetTickCount();
	CBlkUtility::SortStringArray(strArrayDcName,del_char);
	long t32=GetTickCount();
	CString str32;
	str32.Format("SortArray time:%d ms",t32-t31);
	acutPrintf(_T("\n %s"),str32);
	long nHandleStartLine=0;
	bool bforward=true;
	//分上下行咽喉
	CStringArray strArrayOdd;
	CStringArray strArrayEven;
	CBlkUtility::SortandDistinguishStringArray(strArrayDcName,strArrayOdd,strArrayEven,del_char);
	acutPrintf(_T("\n下行调车信号机："));
	for(int i=0;i<strArrayOdd.GetCount();i++)
	{
		acutPrintf(_T(" %s\t"),strArrayOdd.GetAt(i));
	}
	acutPrintf(_T("\n上行调车信号机："));
	for(int i=0;i<strArrayEven.GetCount();i++)
	{
		acutPrintf(_T(" %s\t"),strArrayEven.GetAt(i));
	}
	//输出下行咽喉调车进路
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
		//获取调车信号机的调车终端
		this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sXGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
		CString xhj;
		GetEntName(nHandleDCXHJ,xhj);
		acutPrintf(_T("\n调车始端：%s"),xhj);
		if(xhj==_T("D31"))
		{
			xhj=xhj;
		}
		acutPrintf("\n调车终端：\n");
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
			//输出调车进路
			for(int i=0;i<nArrayHandleNext.GetCount();i++)
			{
				long nHandEndXHJ=0;
				long nHandleEndLine=nArrayHandleNext.GetAt(i);
				CString Gd_name;
				m_mysys.GetXdata(nHandleEndLine,_T("GUIDAONAME"),Gd_name);
				acutPrintf(_T("\t 终端:%s"),Gd_name);
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
							//增加需求：使变更进路用颜色标注；
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
	//获取下行出站信号机作调车信号机
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
		acutPrintf(_T("下行出站信号机:%s  %s\t"),strgdname,strname);
		str_name_ArrayXXHJ.Add(strname);
	}
	//排序输出：
	acutPrintf(_T("\n输出下行咽喉出站兼调车信号机:\n"));
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
		acutPrintf(_T("\n调车始端：%s"),xhj);
		this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sSGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
		int i_count= nArrayHandleNext.GetCount();
		if(xhj==_T("S3"))
		{
			i_count=i_count;
		}
		int i_row_diaochecurrent=this->num;
		if(nHandleStartLine!=0)
		{
			//输出调车进路
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
//	输出上行咽喉调车进路
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
			acutPrintf(_T("\n调车始端：%s"),xhj);
			if(xhj==_T("X3"))
			{
				xhj=xhj;
			}
			acutPrintf("\n调车终端：\n");
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
				//输出调车进路
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
// 	出站信号机兼调车信号机作为调车始端
	//获取上行出站信号机兼做调车信号机
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
		acutPrintf(_T("下行出站信号机:%s  %s\t"),strgdname,strname);
		str_name_ArraySXHJ.Add(strname);
	}
// 	//排序输出：
	acutPrintf(_T("\n输出上行咽喉出站兼调车信号机:\n"));
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
		acutPrintf(_T("\n调车始端：%s"),xhj);
		this->GetNextDCXHJByDCXHJ(nHandleDCXHJ,sSGuDaoName,JieJinQuDuan,nArrayHandleNext,strNextDC);
		int i_count=nArrayHandleNext.GetCount();
		if(xhj==_T("X3"))
		{
			i_count=i_count;
		}
		int i_row_diaochecurrent=this->num;
		if(nHandleStartLine!=0)
		{
   			//输出调车进路
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
	oper_excel.WriteDatatoExcel(1,i_excel_row_lieche,1,_T("调车进路"));
	oper_excel.SetColumnCombandCellsAndCenterShow(1,i_excel_row_lieche,2,i_row_diaochecurrent3-i_excel_row_lieche);
	oper_excel.WriteDatatoExcel(1,i_excel_row_lieche,2,_T("由"));
	//考虑是否存在延续进路，并判断下坡道是在上行咽喉还是下行咽喉
	//查询图中是否存在PMT_XBD_块
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
		//存在,查询所在轨道
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
					//起始轨道
				int i_excel_row_nei_current=this->num;
				long nHandleStart=nAarryHandleGuDaoS.GetAt(i);
				CString Start_Gd_Name;
				for(int j=0;j<nAarryHandleXJJC.GetCount();j++)
				{
					//终止轨道
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
						//提取关键运行道岔
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
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,1,_T("延续进路"));
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,2,_T("由"));
		}
		if(b_up==true)
		{
			for(int i=0;i<nAarryHandleGuDaoX.GetCount();i++)
			{
				//起始轨道
				long nHandleStart=nAarryHandleGuDaoX.GetAt(i);
				CString Start_Gd_Name;
                int i_excel_row_nei_current=this->num;
				for(int j=0;j<nAarryHandleSJJC.GetCount();j++)
				{
					//终止轨道
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
						//提取关键运行道岔
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
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,1,_T("延续进路"));
			oper_excel.WriteDatatoExcel(1,i_row_yanxucurrent,2,_T("由"));
		}

	}
	oper_excel.SaveExcel();
	//CString strFileSave=_T("%s\\support\\xls\\平面图联锁表.xls",g_strBasePath);
// 	CString strFileSave;
// 	if(FALSE==this->SelectFilePath(strFileSave))
// 	{
// 		AfxMessageBox(_T("指定的文件路径无效！"));
// 		return ;
// 	}
	oper_excel.SaveasXSLFile(strFileSave);
	oper_excel.RealseExcel();
	acutPrintf(_T("\n程序开始时间%s"),str_Time);
	CTime  time_end=CTime::GetCurrentTime();
	 //CTime time_take=time_end-time_beg;
     str_Time=time_end.Format("%Y-%m-%d %H:%M:%S");
	acutPrintf(_T("\n程序结束时间%s"),str_Time);
	acutPrintf(_T("\n访问数据次数=%d"),m_RecordSet.i_count);
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
			//是渡线道岔
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//取道岔反位线
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//取道岔反位线
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
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
			//是渡线道岔
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//取道岔反位线
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//取道岔反位线
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
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
		//取道岔反位线
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw);
		if(nHandle_fw==0)
		{
			nHandleDc_Another =0;
			return FALSE;
		}
		//向左或者向右找一后继
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		//int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
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
		//判断后继是否有一条为渡线
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}
	return FALSE;
}
//由一条渡线的两个道岔，如果交叉渡线，则返回另一条渡线的另外两个道岔
BOOL CZnSxt::GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		//输入的两个道岔必须是渡线
		long nHandleDc_One2=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandleDc_One2)==TRUE)
		{
			if(nHandleDc_Another==nHandleDc_One2)
			{//输入为渡线
				long nHandleline1=0;
				long nHandleline2=0;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline1) ;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline2) ;
				//找online1上的其他道岔是否为渡线道岔
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
						//是否是渡线
						long nHandleDC_A=0;
						if(this->GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_A)==TRUE)
						{
							//是渡线道岔，并且另一道岔位于line2。此时的两个轨道区段共同组成两条渡线，则很可能为交叉渡线
							if(CBlkUtility::FindLongInArray(nArrayHandleDC2,nHandleDC_A)!=-1)
							{
								//是否还需要加额外条件来区分交叉渡线
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
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
//某道岔反位时，需要将某道岔带至定位时，函数返回TRUE,带动道岔以DdMin和DdMax带回,只考虑了带动一个道岔的情况，没有考虑需要带动两个或两个以上的道岔
BOOL CZnSxt::GetDaidongDaochas( long nHandleDC,long &nHandleDc_DdMin,long &nHandleDc_DdMax )
{
	try
	{
		//获取道岔所在定位线
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		//long nHandle_fw=0;
		long nHandle_dw=0;
		long nHandle_fw=0;
		long iStart1=-1;
		nHandleDc_DdMin=0;
		nHandleDc_DdMax=0;
		//取道岔定位线
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
		//获取定位线道岔个数，并以X坐标升序排列
		//CLongArray nArrayHandleXHJ;
		CLongArray nArrayHandleDC;
		CLongArray nArrayHandleLineType;
		CLongArray nArrayHandleLineCur;
		//this->GetGuiDaoDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
		this->GetOnLineDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC);
		int iNum_DC=0;
		iNum_DC =nArrayHandleDC.GetCount();
		//acutPrintf(_T("\n 轨道上包含 %d个道岔"),iNum_DC);
		if(iNum_DC<2)
		{
			return FALSE;
		}
		//判断道岔是否位于轨道最顶端
		int index=0;
		index=CBlkUtility::FindLongInArray(nArrayHandleDC,nHandleDC);
		//acutPrintf(_T("\n 道岔位置；%d"),index);
		if(index==-1)
		{
			return FALSE;
		}
		//向右推进时,有多条后继
		CLongArray nArrayHandleNext;
		long nHandleLineEnd=0;
		CStringArray sGuodaocha;
		CStringArray sGudao;
		//bool foword=true;
		if(iStart1==0)
		{
			//向右推进
			this->FindNextLine(nHandle_dw,nHandleLineEnd,nArrayHandleNext,true);
			int iNum_Next=nArrayHandleNext.GetCount();
			//acutPrintf(_T("\n 往右轨道上包含 %d个后继"),iNum_Next);
			if(iNum_Next>=3)
			{
				for(int i=index+1;i<iNum_DC;i++)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//带动道岔一定是渡线道岔
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
			//向左推进
			CLongArray nArrayHandleNext2;
			long nHandleLineEnd2=0;
			//bool foword2=false;
			this->FindNextLine(nHandle_dw,nHandleLineEnd2,nArrayHandleNext2,false);
			int iNum_Next2=nArrayHandleNext2.GetCount();
			//acutPrintf(_T("\n 往左轨道上包含 %d个后继"),iNum_Next2);
			if(iNum_Next2>=3)
			{
				for(int i=index-1;i>=0;i--)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//long nHandle_DXMin=0;
					long nHandle_DXAno=0;
					//未考虑交叉渡线存在的情况
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
		AfxMessageBox(_T("未知的错误!in GetdaidongDaoChas"));
	}
	return FALSE;
}
BOOL CZnSxt::GetDaidongDaochas( long nHandleDC,CString &str_daidongdaochas)
{
	try
	{
		//获取道岔所在定位线
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
		//取道岔定位线
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
			//为渡线，则往后求
			if(iStart1==0)
			{
				FindNextLineByJYJ(nHandle_fw_0,nHandleLineTfw,true);
				if(nHandleLineTfw!=0)
				{
					//求道岔定位所在线
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
					//求道岔定位所在线
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
		//获取定位线道岔个数，并以X坐标升序排列
		//CLongArray nArrayHandleXHJ;
		CLongArray nArrayHandleDC;
		CLongArray nArrayHandleLineType;
		CLongArray nArrayHandleLineCur;
		//this->GetGuiDaoDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
		this->GetOnLineDaoChaOrderByXAsc(nHandle_dw_0,nArrayHandleDC);
		int iNum_DC=0;
		iNum_DC =nArrayHandleDC.GetCount();
		//acutPrintf(_T("\n 轨道上包含 %d个道岔"),iNum_DC);
		if(iNum_DC<2)
		{
			return FALSE;
		}
		//判断道岔是否位于轨道最顶端
		int index=0;
		index=CBlkUtility::FindLongInArray(nArrayHandleDC,nHandleDC);
		//acutPrintf(_T("\n 道岔位置；%d"),index);
		if(index==-1)
		{
			return FALSE;
		}
		//向右推进时,有多条后继
		CLongArray nArrayHandleNext;
		long nHandleLineEnd=0;
		CStringArray sGuodaocha;
		CStringArray sGudao;
		//bool foword=true;
		if(iStart1==0)
		{
			//向右推进
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd,nArrayHandleNext,true);
			int iNum_Next=nArrayHandleNext.GetCount();
			//acutPrintf(_T("\n 往右轨道上包含 %d个后继"),iNum_Next);
			if(iNum_Next>=3)
			{
				for(int i=index+1;i<iNum_DC;i++)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//带动道岔一定是渡线道岔
					long nHandle_DXAno=0;
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//求另一个道岔所在定位线；
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
			//向左推进
			CLongArray nArrayHandleNext2;
			long nHandleLineEnd2=0;
			//bool foword2=false;
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd2,nArrayHandleNext2,false);
			int iNum_Next2=nArrayHandleNext2.GetCount();
			//acutPrintf(_T("\n 往左轨道上包含 %d个后继"),iNum_Next2);
			if(iNum_Next2>=3)
			{
				for(int i=index-1;i>=0;i--)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//long nHandle_DXMin=0;
					long nHandle_DXAno=0;
					//未考虑交叉渡线存在的情况
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{

						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//求另一个道岔所在定位线；
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
		AfxMessageBox(_T("未知的错误!in GetdaidongDaoChas"));
	}
	return FALSE;
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
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
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
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
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
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
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
//向右推进时，获取轨道左侧信号机，向左推进时，获取轨道右侧信号机
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
			//获取轨道右端信号机
			strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d ) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle) ;
		}
		else
		{
			//获取轨道左端信号机
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
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
// 查询轨道区段右侧的信号机，并按照块的基点x坐标升序排列
//conline_left=true时，轨道区段的右侧的信号机
void CZnSxt::GetGuiDaoDuanXHJ(bool bforward,UINT nLineHandle, long& ZhiXHJ,long& XiangXHJ)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//long t1=GetTickCount();
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		CLongArray nArrayHandleXHJTmp;
		//注意：nLineHandle一定不能取0，否则可能查到错误的结果
		if(nLineHandle==0)
		{
			ZhiXHJ=0;
			XiangXHJ=0;
			return;
		}
		if(bforward==false)
		{
			//轨道右端信号机
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
						//找到同向信号机即可结束
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
				//轨道左端信号机
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
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
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
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
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
		AfxMessageBox(_T("未知的错误!inGetGuiDaoChaOrderByXDesc"));
	}
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
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
		AfxMessageBox(_T("未知的错误!inGetGuiDaoChaOrderByXDesc"));
	}
}


void CZnSxt::PrintGuoDaocha( vector<CString>PathDaocha )
{
	int count =PathDaocha.size();
	CStringArray JiLuDaocha;
	acutPrintf(_T("\n经过："));
	for(int i=0;i<count;i++)
	{
		//获取道岔
		CString temp=PathDaocha[i];
		CString Ptemp=temp;
		//TCHAR a=_T('(');
		TCHAR a='(';
		TCHAR b=')';
		TCHAR c='{';
		TCHAR d='}';
		TCHAR e='[';
		TCHAR f=']';
		//对temp去掉_T("("),_T(")"),_T("{"),_T("}"),_T("["),_T("]")
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
	//acutPrintf(_T("\n经过："));
	for(int i=0;i<count;i++)
	{
		//获取道岔
		CString temp=PathDaocha[i];
		CString Ptemp=temp;
		//TCHAR a=_T('(');
		TCHAR a='(';
		TCHAR b=')';
		TCHAR c='{';
		TCHAR d='}';
		TCHAR e='[';
		TCHAR f=']';
		//对temp去掉_T("("),_T(")"),_T("{"),_T("}"),_T("["),_T("]")
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
//主要功能：提取一条轨道上的道岔的第一个道岔
void CZnSxt::TiquFirstDaocha( CString strDaochas, CString &Daocha)
{
	//int count =PathDaocha.size();
	//获取道岔
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
//主要功能：提取一条轨道上的道岔的第一个道岔
void CZnSxt::TiquFirstDaocha2( CString strDaochas, CString &Daocha)
{
	//int count =PathDaocha.size();
	//获取道岔
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
//主要功能：将String中按照"，"分割符号提取有效信息存入vector中
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
//主要功能：将String中按照"，"分割符号提取有效信息存入vector中
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
//主要功能：将vector<CString>中逗号分隔开的字符串集合去掉重复的字符串
void CZnSxt::DeleteSameData( vector<CString>& vec_SourceData, vector<CString>& str_Vec_Dest_Data)
{
	int count =vec_SourceData.size();
	CStringArray JiLuDaocha;
	//acutPrintf(_T("\n经过："));
	for(int i=0;i<count;i++)
	{
		//获取道岔
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
//主要功能：将vector中的出站信号机的列车和调车信号进行合并，合并前提是Vector中调车信号位于列车信号的前面
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
	//acutPrintf(_T("\n经过："));
	for(int i=0;i<count;i++)
	{
		//获取道岔
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
		AfxMessageBox(_T("未知的错误!in GetEntName"));
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
		AfxMessageBox(_T("未知的错误!in GetEntName"));
		return FALSE;
	}
	
}

void CZnSxt::FindNextDiaoCheXHJ( long nHandleDC_XHJ,CLongArray& nArrayHandleNext_DC_XHJ )
{
	//检查调车信号机是否在数据库，并求出信号机所在的轨道区段
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
		//获取调车信号机的朝向
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
			//朝右
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
			//朝左
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
		AfxMessageBox(_T("未知的错误!in FindNextDiaoCheXHJ"));
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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString strXlsFileName=_T("C:\\辅助开发\\平面图联锁表生成1.xls");
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
		//acutPrintf(_T("\n提示信息：%s: %s"),strCellName,strCellContent);
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
		acutPrintf(_T("\n%s"),strCellContent.GetString());
		//pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
		pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		pWorkBook->Save() ;
		///关闭打开的各个资源
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
		acutPrintf(_T("\n提示信息2："));
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n提示信息3："));
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
// 			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
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
// 		AfxMessageBox(_T("未知的错误!"));			
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
// // 	///关闭打开的各个资源
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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString strXlsFileName=_T("C:\\辅助开发\\平面图联锁表生成1.xls");
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
		///关闭打开的各个资源
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
		acutPrintf(_T("\n提示信息2："));
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n提示信息3："));
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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString strXlsFileName=_T("C:\\辅助开发\\平面图联锁表生成1.xls");
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
		///关闭打开的各个资源
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
		acutPrintf(_T("\n提示信息2："));
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
		acutPrintf(_T("\n提示信息3："));
	}	
	return 0;
}
//递归找到从某条轨道线开始的后继调车进路，该轨道线为调车起始信号机所在的轨道线
/*******************函数说明:FindNextDCByLine*****************************************************************************************************************/
/**********功能：通过一个调车起始信号机所在的轨道，返回调车基本进路的下一个调车信号机*******************************************************************/
/**********输入：nHandleLine,调车起始信号机所在的轨道线,轨道线在调车进路内侧;*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔，用于去掉轨道线上不属于进路内的信号机；**********************************************/
/*****************Gudao，股道名称；JieJinQuDuan，接近区段名称;bforward,寻找调车进路方向，其由调车起始信号机方向决定********************************/
/**********输出：nArrayHandleNextLine,下一个调车信号机所在的轨道区段; nArrayDCZhong,返回调车进路终端信号机*****************************************/
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
		//求后继轨道线
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
			//向右推进时，寻找轨道左端信号机，向左推进时寻找轨道右端信号机
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
				//DCname=_T("至")+DCname;
				//不包含重复的终端
				if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
				{
					nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
					nArrayDCZhong.Add(DCname);
				}
				
			}
			else
			{
				//没有找到同方向调车信号机时
				//1.是否是尽头线
				//找尽头式绝缘节
				//long nHandleEnt=0;
				CString EntName;
				int type=200;
				if(bfoward==true)
				{
					//轨道右端信号机
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
				//表示为尽头线
				if((xiangxhj!=0)&&(EntName.Left(6)==_T("JYJ_DT")))
				{
					CString DCname;
					GetEntName(xiangxhj,DCname);
					//DCname=_T("向")+DCname;
					//acutPrintf(_T("\n 尽头线信号机：%s"),DCname);
					CString EntName;
					GetEntBlkNameByHandle(xiangxhj,EntName);
				    if(EntName.Left(6)==_T("XHJ_LF"))
					{
						//向信号机为出站信号机时
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
					//进站信号机作为终端按钮
					CString DCname;
					GetEntName(xiangxhj,DCname);
					//DCname=_T("向")+DCname;
					//acutPrintf(_T("\n 尽头线信号机：%s"),DCname);
					CString EntName;
					GetEntBlkNameByHandle(xiangxhj,EntName);
					//向信号机为出站信号机时
					//DCname+=_T("D");	
					if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
					{
						nArrayDCZhong.Add(DCname);
						nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);		
					}
				}
				else if(xiangxhj!=0&&str_xhj_ent.Left(6)==_T("XHJ_LF"))
				{
					//发车信号机为终端按钮，调至对应股道
					CString DCname;
					m_mysys.GetXdata(nArrayHandleNextLineTemp[i],_T("GUIDAONAME"),DCname);
					nArrayDCZhong.Add(DCname);
					nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
				}
				else
				{
					//为股道线时
					CString GuidaoName;
					m_mysys.GetXdata(nArrayHandleNextLineTemp[i],_T("GUIDAONAME"),GuidaoName);
					if(CBlkUtility::FindStrInArray(this->str_GuDaoName,GuidaoName)!=-1)
					{
						//为股道时
						CString DCname;
						DCname=GuidaoName;
						if(CBlkUtility::FindLongInArray(nArrayHandleNextLine,nArrayHandleNextLineTemp[i])==-1)
						{
							nArrayDCZhong.Add(DCname);
							//acutPrintf(_T("\n 股道信号机：%s"),DCname);
							nArrayHandleNextLine.Add(nArrayHandleNextLineTemp[i]);
						}
					}
					else if(CBlkUtility::FindStrInArray(JieJinQuDuan,GuidaoName)!=-1)
					{
						//为接近区段,且不包含道岔
						if(xiangxhj!=0&&GuidaoName.Right(2)!=_T("DG"))
						{
							CString DCname;
							GetEntName(xiangxhj,DCname);
							//acutPrintf(_T("\n 接近区段信号机：%s"),DCname);
							//DCname=_T("向")+DCname;
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
							//还有可能接近区段有道岔，此时走道岔反位的调车信号机也应该算上。
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
		AfxMessageBox(_T("未知的错误!inFindNextDCByLine"));
		return -2;
	}	
}
//递归找到从某条轨道线开始的后继列车信号机，该轨道线为调车终端或始端所在的轨道线
/*******************函数说明:FindNextLieCheXHJByLine*****************************************************************************************************************/
/**********功能：通过一个调车起始信号机所在的轨道，返回调车基本进路的下一个调车信号机*******************************************************************/
/**********输入：nHandleLine,调车起始信号机所在的轨道线,轨道线在调车进路内侧;*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔，用于去掉轨道线上不属于进路内的信号机；**********************************************/
/*****************Gudao，股道名称；bforward,寻找列车信号机的方向，**************************************************************************************************/
/**********输出：nArrayHandleNextLine,下一个列车信号机所在的轨道区段; nArraylieche,返回列车信号机名称*****************************************/
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
// 		//获取起始轨道背向信号机
// 		bool iToward=!bfoward;
// 		CLongArray nArrayPassXHJ;
// 		CString Gd_name;
// 		m_mysys.GetXdata(nHandleLine,_T("GUIDAONAME"),Gd_name);
// 		GetGuiDaoDuanXHJ(nHandleLine,iToward,nArrayPassXHJ);
// 		for(int i=0;i<nArrayPassXHJ.GetCount();i++)
// 		{
// 			//判断是否是列车信号机
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
					//进站信号机无需加_T("L")
					blkname=blkname+_T("L");
				}
				nArrayLieChe.Add(blkname);
				nArrayHandleNextLine.Add(nHandleLine);
				return 1;
			}
		}
		//求后继轨道线
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
			//向右推进时，寻找轨道左端信号机，向左推进时寻找轨道右端信号机
			int iTag_Find=0;
			bool iToward=bfoward;
			long zhixhj=0;
			long xiangxhj=0;
			GetGuiDaoDuanXHJ(iToward,nHandleLine,zhixhj,xiangxhj);
		    if(xiangxhj!=0)
			{
				//判断是否是列车信号机
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
						//进站信号机无需加_T("L")
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
		AfxMessageBox(_T("未知的错误!inFindNextLieCheXHJByLine"));
		return -2;
	}	
}
//递归找到从某条轨道线开始的后继调车进路，该轨道线为调车起始信号机所在的轨道线
/*******************函数说明:FindNextDuiXiangDCByLine*****************************************************************************************************************/
/**********功能：通过一个调车起始信号机所在的轨道，返回调车基本进路的下一个调车信号机*******************************************************************/
/**********输入：nHandleLine,调车起始信号机所在的轨道线,轨道线在调车进路内侧;*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔，用于去掉轨道线上不属于进路内的信号机；**********************************************/
/*****************Gudao，股道名称；JieJinQuDuan，接近区段名称;bforward,寻找调车进路方向，其由调车起始信号机方向决定********************************/
/**********输出：nArrayHandleNextLine,下一个调车信号机所在的轨道区段; nArrayDCZhong,返回调车进路终端信号机*****************************************/
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
		//获取起始轨道背向信号机
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
// 			//该敌对信号机在调车进路内已经包括，无需再次加入，可以直接返回
// 			return 1;
// 		}
		//求后继轨道线
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
			//向右推进时，寻找轨道左端信号机，向左推进时寻找轨道右端信号机
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
				//DCname=_T("至")+DCname;
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
		AfxMessageBox(_T("未知的错误!inFindNextDCByLine"));
		return -2;
	}	
}
//递归找到从某条轨道线开始的后继调车进路，该轨道线为调车起始信号机所在的轨道线
/*******************函数说明:FindNextDuiXiangDCByLine2*****************************************************************************************************************/
/**********功能：通过一个调车起始信号机所在的轨道，返回调车基本进路的下一个调车信号机*******************************************************************/
/**********输入：nHandleLine,调车起始信号机所在的轨道线,轨道线在调车进路内侧;*******************************************************************************/
/******************LastDaocha,记录上一次访问到轨道线时走过的道岔，用于去掉轨道线上不属于进路内的信号机；**********************************************/
/*****************Gudao，股道名称；JieJinQuDuan，接近区段名称;bforward,寻找调车进路方向，其由调车起始信号机方向决定********************************/
/**********输出：nArrayHandleNextLine,下一个调车信号机所在的轨道区段; nArrayDCZhong,返回调车进路终端信号机*****************************************/
/****************strArrayDaocha，找到对应的调车信号机走过的道岔，用于作为敌对条件********************************************************************************************************************************************/
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
		//获取起始轨道背向信号机
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
		//求后继轨道线
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
			//向右推进时，寻找轨道左端信号机，向左推进时寻找轨道右端信号机
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
		AfxMessageBox(_T("未知的错误!FindNextDuiXiangDCByLine2"));
		return -2;
	}	
}
/*******************函数说明:GetNextDCXHJByDCXHJ***********************************************************************************/
/**********功能：通过一个调车起始信号机，返回调车基本进路的下一个调车信号机******************************************************/
/**********输入：nHandleDC,调车起始信号机；Gudao，股道名称；JieJinQuDuan，接近区段名称*************************************/
/**********输出：nArrayHandleNextLine,下一个调车信号机所在的轨道区段; strArrayDC,下一个调车信号机的名称**********************/
/*****************nArrayHandleNextLine,为调车进路的终端轨道，为终端信号机的前一个轨道区段*******************************************************************************************************************/
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
		//获取信号机方向
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
		//根据当前线获取后继线上的信号机
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
		AfxMessageBox(_T("未知的错误!in GetNextDCXHJByDCXHJ"));
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
		//获取信号机方向
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
		AfxMessageBox(_T("未知的错误!in GetNextDCXHJByDCXHJ"));
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
		//获取信号机方向
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
		AfxMessageBox(_T("未知的错误!in GetNextDCXHJByDCXHJ"));
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
		//根据当前线获取后继线上的信号机
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
		AfxMessageBox(_T("未知的错误!in GetJYJ_YouCe_GdLine"));
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
		//获取信号机方向
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
		//根据当前线获取后继线上的信号机
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
		AfxMessageBox(_T("未知的错误!in GetXHJ_ZuoCe_GdLine"));
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
		//根据当前线获取后继线上的信号机
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
		AfxMessageBox(_T("未知的错误!in GetXHJ_ZuoCe_GdLine"));
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
		AfxMessageBox(_T("未知的错误!in GetHandleByName"));
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
		AfxMessageBox(_T("未知的错误!in Get_Block_Online"));
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
		AfxMessageBox(_T("未知的错误!in GetHandleByEntBlkName"));
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
		AfxMessageBox(_T("未知的错误!in GetEntBlkNameByHandle"));
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
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForward?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		//long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForward?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
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
		AfxMessageBox(_T("未知的错误!in FindNextLineByJYJ"));
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
	//如果得到的点在线上，而不是端点
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
	   // acutPrintf(_T("\n找警冲标"));
		//查询超限绝缘节左线和右线
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
		//获取所有警冲标
		GetSomeBlocks(_T("PMT_JC"),nArrayHandleJCB);
		//获取信号楼坐标
		long nHandleXHL=0;
		double pt_xhl_x=0;
		GetHandleByEntBlkName(_T("PMT_XHL_1"),nHandleXHL);
		if(GetBlockPosition_X(nHandleXHL,pt_xhl_x)==FALSE)
		{
			return FALSE;
		}
		//获取超限绝缘节坐标
		double pt_jyj_x=0;
		if(GetBlockPosition_X(nHandleCXJYJ,pt_jyj_x)==FALSE)
		{
			return false;
		}
		int shangxia=-1;
		if(pt_jyj_x-pt_xhl_x>1e-6)
		{
			//位于上行咽喉区
			shangxia=1;
		}
		else
		{
			//位于下行咽喉区
			shangxia=0;
		}
		//左右线合成一条线
		//对左线分析
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
			    str_cx_guidao_left+=_T("、");
			}
			else
			{
				str_cx_guidao_left+=str_DC;
				str_cx_guidao_left+=_T(">");
			}
		}
		//添加轨道名称
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
				str_cx_guidao_right+=_T("、");
			}
			else
			{
				str_cx_guidao_right+=str_DC;
				str_cx_guidao_right+=_T(">");
			}
		}
		//添加轨道名称
		CString Gdname2;
		m_mysys.GetXdata(nHandleLineConRight,_T("GUIDAONAME"),Gdname2);
		str_cx_guidao_right+=Gdname2;
		str_CX_GdName_left=str_cx_guidao_left;
		str_CX_GdName_Right=str_cx_guidao_right;
		for(int i=0;i<nArrayHandleLeftDC.GetCount();i++)
		{
			//获取道岔反位线以及iStart
			long nHandleDC=nArrayHandleLeftDC.GetAt(i);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			//反位线对应的道岔
			long nHandleLineFw=0;
			int IsStart=-1;
			m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleLineFw);
			if(nHandleLineFw!=0)
			{
				if(IsStart==0)
				{
					//是否为渡线
					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineFw) ;
					long nLineType = 0 ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
					if(nLineType==1)
					{
						//渡线，需要再求一次后继
						long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
						//如果是向右寻找，则找右端绝缘节。
						int nHandleEndConJyj = nHandleRightJyj2; //线末端连接的绝缘节handle
						long nHandleLineFw2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
						CString strFieldName = _T("sxt_ent_conline_right");
						//读取结果集
						m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleLineFw2);
						//合成一条线，找出相应的警冲标，并判断是否超限
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
										//acutPrintf(_T("\n渡线 找到 JCB:%s"),JCBName);
										//需要判断是否超限，获取超限绝缘节属性值
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if(i_dist_JCB-i_dist_JYJ<4)
										{
											acutPrintf("\n渡线 超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_right);
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
						//非渡线
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
										//acutPrintf(_T("\n 非渡线 找到JCB:%s"),JCBName);
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if(i_dist_JCB-i_dist_JYJ<4)
										{
											acutPrintf("\n非渡线  超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_right);
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
				acutPrintf(_T("\n道岔反位线不存在，数据录入错误！"));
			}
			if(iFlag_find_left==1)
			{
				break;
			}
		}
		//可能为某一个道岔的反位线
		strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineConLeft) ;
		//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//获取对应的定位线
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
							//acutPrintf(_T("\n 非渡线 找到JCB:%s"),JCBName);
							CString strDist;
							m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
							strDist.Remove(_T('('));
							strDist.Remove(_T(')'));
							int i_dist_JYJ=StrToIntA(strDist);
							int i_dist_JCB=StrToIntA(JCBName);
							if(i_dist_JCB-i_dist_JYJ<4)
							{
								acutPrintf("\n反位线  超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_right);
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
		//从右线分析
		for(int i=0;i<nArrayHandleRightDC.GetCount();i++)
		{
			//获取道岔反位线以及iStart
			long nHandleDC=nArrayHandleRightDC.GetAt(i);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			//反位线对应的道岔
			long nHandleLineFw=0;
			int IsStart=-1;
			m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleLineFw);
			if(nHandleLineFw!=0)
			{
				if(IsStart>0)
				{
					//是否为渡线
					strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineFw) ;
					long nLineType = 0 ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
					if(nLineType==1)
					{
						//渡线，需要再求一次后继
						long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineFw) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
						//如果是向右寻找,则找右端绝缘节。
						int nHandleEndConJyj =nHandleLeftJyj2; //线末端连接的绝缘节handle
						long nHandleLineFw2 = 0 ;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
						CString strFieldName = _T("sxt_ent_conline_left");
						//读取结果集
						m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleLineFw2);
						//合成一条线,找出相应的警冲标,并判断是否超限
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
										//acutPrintf(_T("\n渡线 找到 JCB:%s"),JCBName);
										//需要判断是否超限,获取超限绝缘节属性值
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if(i_dist_JCB-i_dist_JYJ<4&&shangxia==1)
										{
											acutPrintf("\n 渡线 超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
											nHandleLineCXRight=nHandleLineFw;
											iFlag_find_right=1;
											break;
										}
										else if(i_dist_JYJ-i_dist_JCB<4&&shangxia==0)
										{
											acutPrintf("\n 渡线 超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
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
						//非渡线
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
										//acutPrintf(_T("\n 非渡线 找到JCB:%s"),JCBName);
										CString strDist;
										m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
										strDist.Remove(_T('('));
										strDist.Remove(_T(')'));
										int i_dist_JYJ=StrToIntA(strDist);
										int i_dist_JCB=StrToIntA(JCBName);
										if((i_dist_JCB-i_dist_JYJ<4)&&shangxia==1)
										{
											acutPrintf("\n 非渡线 超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
											nHandleLineCXRight=nHandleLineFw;
										    iFlag_find_right=1;
											break;
										}
										else if((i_dist_JYJ-i_dist_JCB<4)&&shangxia==0)
										{
											acutPrintf("\n 非渡线 超限：%d %d %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
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
				acutPrintf(_T("\n道岔反位线不存在,数据录入错误！"));
			}
			if(iFlag_find_right==1)
			{
				break;
			}
		}
		//可能为某一个道岔的反位线
		strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineConRight) ;
		//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//获取对应的定位线
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
							//acutPrintf(_T("\n 非渡线 找到JCB:%s"),JCBName);
							CString strDist;
							m_mysys.GetAttValue(nHandleCXJYJ,_T("DIST"),strDist);
							strDist.Remove(_T('('));
							strDist.Remove(_T(')'));
							int i_dist_JYJ=StrToIntA(strDist);
							int i_dist_JCB=StrToIntA(JCBName);
							if(i_dist_JCB-i_dist_JYJ<4)
							{
								acutPrintf("\n 反位线 超限：%d %d  %s",i_dist_JYJ,i_dist_JCB,str_cx_guidao_left);
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
		AfxMessageBox(_T("未知的错误!in FindCXGuiDao"));
		//return -2;
		return FALSE;
	}		
	return FALSE;

}
/*****************************函数     FindALLChaoXianGuiDao*****************************************************************************************************/
/*****功能：用于找出平面图上所有的超限绝缘情况，返回超限时走过的区段句柄和应该占用的轨道区段名称***************************************************************************************************************/
/*****输入：无，即信号设备平面布置图数据全部来自数据库*************************************************************************************************************/
/*****输出：走过的区段句柄CLongArray& nArrayHandleLine，以及应该占用的轨道区段名称CStringArray& strArrayGuiDao***************************************************************************************************************************************************/
/*****注意：考虑超限绝缘时分别考虑了超限绝缘节为于定位线和反位线的情况，分别考虑了超限绝缘节左线道岔所在的定位和右线所在的定位线引起的超限绝缘，****/
/************在判断是否超限时，分别将超限绝缘节位置和对应的警冲标进行了比较。并分别考虑了超限绝缘节分别位于上行咽喉和下行咽喉的不同情况************************************************************************************************************/
/************使用该函数是，对平面图警冲标位置有一定要求，即要求对应警冲标必须位于绝缘节定位线和反位线围起来的区域内部（一个为警冲标的下方,一个位于警冲标的上方且必须是这样）************************************************************/
/********************* 注释 2015-06-17 pm-17:04 by luo*******************************************************************************************************************/
int CZnSxt::FindALLChaoXianGuiDao( CLongArray& nArrayHandleLine,CStringArray& strArrayGuiDao )
{
	//获取超限绝缘节
	CLongArray nArrayHandleJCB;
	CLongArray nArrayHandleCXJYJ;
	//用于存储超限绝缘时走的轨道区段
	//CLongArray nArrayHandleLine;
	//用于存储超限绝缘时应该占用的区段
	//CStringArray strArrayGuiDao;
	GetSomeBlocks(_T("PMT_JC"),nArrayHandleJCB);
	GetSomeBlocks(_T("JYJ_CX"),nArrayHandleCXJYJ);
	int iRet=0;
	for(int i=0;i<nArrayHandleCXJYJ.GetCount();i++)
	{
		long nHandleJYJ=0;
		nHandleJYJ=nArrayHandleCXJYJ.GetAt(i);	
		//long nHandleJYJ=nArrayHandleCXJYJ.GetAt(0);
		//求左线和右线
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
// 	acutPrintf(_T("\n 超限绝缘结果："));
// 	for(int i=0;i<nArrayHandleLine.GetCount();i++)
// 	{
// 		CString name;
// 		m_mysys.GetXdata(nArrayHandleLine.GetAt(i),_T("GUIDAONAME"),name);
// 		acutPrintf(_T("\n经过%s,须占用%s"),name,strArrayGuiDao.GetAt(i));
// 	}
	return iRet;

}

void CZnSxt::LSBExcel()
{
	ads_point base_pt={0,0,0};
	ads_getpoint(NULL,_T("\n选择起始点"),base_pt);
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
	//创建圆形
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
	//TCHAR*file_sign =_T("C:\\辅助开发\\my_sign.dwg");
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
	//以块方式插入
	AcGePoint3d jungong_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
	CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\xiugai.dwg"), g_strBasePath) ;
	TCHAR *filename =(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	//TCHAR*filename =_T("C:\\辅助开发\\xiugai.dwg");
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
//根据生成的excel表的长度转化成CAD
void CZnSxt::XlsToLSB( AcGePoint3d start_pt_3d,float f_length_xls )
{
	//ads_point base_pt={0,0,0};
	//ads_getpoint(NULL,_T("\n选择起始点"),base_pt);
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
	//创建圆形
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
	//以块方式插入
	AcGePoint3d jungong_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
   CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\xiugai.dwg"), g_strBasePath) ;
	TCHAR *filename =(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	/*TCHAR*filename =_T("C:\\辅助开发\\xiugai.dwg");*/
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
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForward?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		//long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForward?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
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
		AfxMessageBox(_T("未知的错误!in FindNextZhiGuiDao"));
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
// 	//获取所有调车信号机
//     CLongArray nArrayHandleAllDC;
// 	GetSomeBlocks(_T("XHJ_DC"),nArrayHandleAllDC);
// 	double pt_x=0;
// 	//获取信号楼坐标
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
// 	acutPrintf(_T("\n 出站信号机个数=%d"),iRet2);
// 	//获取股道所在轨道
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
// 		acutPrintf(_T("\n 信号机个数为%d"),length);
// 		return;
// 	}
// 	//打印进站信号机名称
// 	for(int i=0;i<length;i++)
// 	{
// 		CString entname;
// 		long nHandleLine =nArrayHandle_LJ_XHJ[i];
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
// 	    acutPrintf(_T("\n 进站信号机名称为%s"),entname.GetString());
// 		//查询进站信号机右侧直线
//         strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
// 		long nHandleLinetemp;
// 		 //寻找后一条轨道
// 		CLongArray nArrayHandle_GD_Next;
// 		//获取信号机右侧轨道
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
// 		//将直线变色显示
// 		m_mysys.FlashEnt(nHandleLinetemp, 500, 50) ;
// 		m_mysys.SetEntColorIndex(nHandleLinetemp,1);
// 		//this->FindNextLine(nHandleLi)
// 	    //寻找后一条轨道
// 		//CLongArray nArrayHandle_GD_Next;
// 		//this->FindNextLine(nHandleLinetemp,nArrayHandle_GD_Next,true);
// 		if(nArrayHandle_GD_Next.GetCount()<1)
// 		{
// 			acutPrintf(_T("\n 没有后继轨道"));
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
// 					acutPrintf(_T("\n 第三条线类型=%d"),nLineType);
// 					m_mysys.SetEntColorIndex(nHandleLine3,4);
// 			   }
// 		}
// 	}
// 	//打印出站信号机名称
// 	for(int i=0;i<iRet2;i++)
// 	{
// 		CString entname;
// 		long nHandleLine =nArrayHandle_LF_XHJ[i];
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine) ;
// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),entname) ;
// 		acutPrintf(_T("\n 出站信号机名称为%s"),entname.GetString());
// 		//查询进站信号机右侧直线
// 		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleLine);
// 		long nHandleLinetemp;
// 		//获取信号机右侧轨道,即为股道
// 		if(entname.Left(1)==_T("S"))
// 		{
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLinetemp) ;
// 		}
// 		else if(entname.Left(1)==_T("X"))
// 		{
// 			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLinetemp) ;
// 		}
// 		//将直线变色显示
// 		m_mysys.FlashEnt(nHandleLinetemp, 500, 50) ;
// 		//m_mysys.SetEntColorIndex(nHandleLinetemp,2);
// 	}
// 
// }
// 	
