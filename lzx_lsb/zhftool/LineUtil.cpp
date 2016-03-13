// LineUtil.cpp: implementation of the CLineUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineUtil.h"
#include "TextUtil.h"
#include "DwgDatabaseUtil.h"
#include <dbents.h>
#include <math.h>
#include "BlockUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineUtil::CLineUtil()
{

}

CLineUtil::~CLineUtil()
{

}



AcDbObjectId CLineUtil::Add( const AcGePoint3d &startPoint, const AcGePoint3d &endPoint, AcDbDatabase *pDb )
{
	// 创建直线实体
	AcDbLine *pLine = new AcDbLine(startPoint, endPoint);
   // pLine->setColorIndex(1);
	// 将直线实体添加到模型空间
	return CDwgDatabaseUtil::PostToModelSpace(pLine, pDb);
}
AcDbObjectId CLineUtil::AddPolyLine( const AcGePoint2d &startPoint, const AcGePoint2d &endPoint, float width,AcDbDatabase *pDb )
{
	// 创建直线实体
	AcDbPolyline *pPoly = new AcDbPolyline(2);
    pPoly->addVertexAt(0,startPoint,0,width,width);
	pPoly->addVertexAt(1,endPoint,0,width,width);
	pPoly->setClosed(Adesk::kTrue);
	// 将直线实体添加到模型空间
	return CDwgDatabaseUtil::PostToModelSpace(pPoly, pDb);
}
AcDbObjectId CLineUtil::AddRect( const AcGePoint2d &startPoint, const AcGePoint2d &endPoint, double  hight, double width ,AcDbDatabase *pDb )
{
	AcGePoint2d ptleftBottom(startPoint.x,(startPoint.y-hight/2));
	AcGePoint2d ptRightBottom(endPoint.x,(endPoint.y-hight/2));
	AcGePoint2d ptRightTop(endPoint.x,(endPoint.y+hight/2));
	AcGePoint2d ptLeftTOP(startPoint.x,(startPoint.y+hight/2));
	AcDbPolyline *poly =new AcDbPolyline(4);
	poly->addVertexAt(0,ptleftBottom,0,width,width);
	poly->addVertexAt(1,ptRightBottom,0,width,width);
	poly->addVertexAt(2,ptRightTop,0,width,width);
	//poly->addVertexAt()
	poly->addVertexAt(3,ptLeftTOP,0,width,width);
	poly->setClosed(Adesk::kTrue);
	return CDwgDatabaseUtil::PostToModelSpace(poly);

}

bool CLineUtil::ThreePointIsCollinear( const AcGePoint2d &pt1, const AcGePoint2d &pt2, const AcGePoint2d &pt3 )
{
	double xy = pt1.x * pt1.x + pt1.y * pt1.y;
	double xyse = xy - pt3.x * pt3.x - pt3.y * pt3.y;
	double xysm = xy - pt2.x * pt2.x - pt2.y * pt2.y;
	xy = (pt1.x - pt2.x) * (pt1.y - pt3.y) - (pt1.x - pt3.x) * (pt1.y - pt2.y);	
	
	return (fabs(xy) < 1.0E-5);
}

int CLineUtil::PtInLeftOfLine( const AcGePoint3d &ptStart, const AcGePoint3d &ptEnd, const AcGePoint3d &pt, double tol /*= 1.0E-7*/ )
{
	return PtInLeftOfLine(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y, pt.x, pt.y, tol);	
}

int CLineUtil::PtInLeftOfLine( const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, const AcGePoint2d &pt, double tol /*= 1.0E-7*/ )
{
	return PtInLeftOfLine(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y, pt.x, pt.y, tol);	
}

int CLineUtil::PtInLeftOfLine( double x1, double y1, double x2, double y2, double x3, double y3, double tol /*= 1.0E-7*/ )
{
	// 两个矢量的叉乘结果是一个，矢量的行列式值是这两个矢量确定的平行四边形的面积
	double a = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	if (fabs(a) < tol)
	{
		return 0;
	}
	else if (a > 0)
	{
		return 1;
	}
	else
	{
		return - 1;
	}
}
//注意返回的是一个对象，在初始化时刻调用是有效的
AcGePoint2d CLineUtil::PointConvertTo2d( const AcGePoint3d &pt3d )
{

	AcGePoint2d pt2d;
	pt2d.x = pt3d.x;
	pt2d.y = pt3d.y;
	return pt2d;
}

void CLineUtil::drawbridge( const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint3d start_level=startpoint;
	start_level.y = start_level.y+21;
	AcGePoint3d end_level =endpoint;
	end_level.y =endpoint.y+21;
    AcGePoint3d leftbottom = start_level;
    leftbottom.x =start_level.x -2;
	leftbottom.y =start_level.y -2;
	CLineUtil::Add(leftbottom,start_level);
	CLineUtil::Add(start_level,end_level);
	AcGePoint3d rightbottom =end_level;
	rightbottom.x =end_level.x+2;
	rightbottom.y = end_level.y -2;
	CLineUtil::Add(end_level,rightbottom);
	//桥高为32mm
	start_level.y =start_level.y+32;
	end_level.y =end_level.y+32;
	AcGePoint3d lefttop =start_level;
	lefttop.x = start_level.x -2;
	lefttop.y = start_level.y+2;
	CLineUtil::Add(lefttop,start_level);
	CLineUtil::Add(start_level,end_level);
	AcGePoint3d righttop =end_level;
	righttop.x =end_level.x +2;
	righttop.y =end_level.y +2;
	CLineUtil::Add(end_level,righttop);

}

void CLineUtil::Setgonglibiao( const AcGePoint3d startpoint,const AcGePoint3d endpoint,CString text )
{
	AcGePoint3d mid_level;
	mid_level.x =(startpoint.x+endpoint.x)/2;
	mid_level.y =startpoint.y+61;
	AcGePoint3d down = mid_level;
	down.y =mid_level.y -5;
	CLineUtil::Add(mid_level,down);
	AcGePoint3d left =mid_level;
	left.x =mid_level.x-5;
	CLineUtil::Add(left,mid_level);
	AcGePoint3d right =mid_level;
	right.x =mid_level.x+5;
	CLineUtil::Add(mid_level,right);
	AcGePoint3d location_text =mid_level;
	location_text.y =mid_level.y+2;
	CTextUtil::AddText(location_text,text.GetString());
}

void CLineUtil::draw_horizon_glan( const int count,const bool up,const AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	//int cnt =count;
	AcGePoint2d start =CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y +=38;
	if(up)
	{
       start.y +=10;
	   end.y  +=10;
		for(int i=0;i<count;i++)
		{
			CLineUtil::AddPolyLine(start,end,0.3);
			start.y +=1;
			end.y +=1;
		}
	
	}
	else
	{
		start.y -=10;
		end.y  -=10;
		for(int i=0;i<count;i++)
		{
			CLineUtil::AddPolyLine(start,end,0.3);
			start.y +=1;
			end.y +=1;

		}

	}

}

void CLineUtil::draw_vertical_glan( const int count,const bool up,const AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	AcGePoint2d mid;
	mid.x =(start.x+end.x)/2;
	mid.y =start.y;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		for(int i=0;i<count;i++)
		{
		    AcGePoint2d next =start;
			next.x =mid.x+1*i;
			CLineUtil::AddPolyLine(start,next,0.3);
            AcGePoint2d next2 =next;
			next2.y =next.y-20;
			CLineUtil::AddPolyLine(next,next2,0.3);
			AcGePoint2d next3 =next2;
			next3.x =end.x;
			CLineUtil::AddPolyLine(next2,next3,0.3);
			start.y +=1;
			//end.y +=2;
		}


	}
	else
	{
		start.y -=10;
		end.y  -=10;
		for(int i=0;i<count;i++)
		{
			AcGePoint2d next =start;
			next.x =mid.x-1*i;
			CLineUtil::AddPolyLine(start,next,0.3);
			AcGePoint2d next2 =next;
			next2.y =next.y+20;
			CLineUtil::AddPolyLine(next,next2,0.3);
			AcGePoint2d next3 =next2;
			next3.x =end.x;
			CLineUtil::AddPolyLine(next2,next3,0.3);
			start.y +=1;
			//end.y +=2;
		}

	}


}

void CLineUtil::SetTuName( const AcGePoint3d startpoint,const AcGePoint3d endpoint,CString text)
{
	AcGePoint3d mid_level;
	mid_level.x =(startpoint.x+endpoint.x)/2;
	mid_level.y =startpoint.y+61;
	AcGePoint3d location_text =mid_level;
	//location_text.y =mid_level.y+2;
	location_text.y =mid_level.y+6;
	//CString text =text;
	CTextUtil::AddText(location_text,text.GetString());

}

void CLineUtil::draw_gangguan( const int count ,const bool up,const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	int hight =4;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		if(count>3)
		{
			//hight =2*(count-2)+4;
			hight =2+count-1;
		}
		else
			hight =2+count-1;
		start.y  =start.y+hight/2 -1;
		end.y   =end.y+hight/2-1;
		CLineUtil::AddRect(start,end,hight);
		

	}
	else
	{
		start.y -=10;
		end.y  -=10;
		if(count>3)
		{
			hight =2+count-1;
		}
		else
			hight =2+count-1;
		start.y  =start.y-1+hight/2;
		end.y   =end.y-1+hight/2;
		CLineUtil::AddRect(start,end,hight);
	} 
}

void CLineUtil::draw_shnicao( const int count ,const bool up,const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	int hight =4;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		if(count>2)
		{
			hight =2+count-1;
		}
		else
			hight =2+count-1;;
		start.y  =start.y+hight/2;
		end.y   =end.y+hight/2;
		CLineUtil::AddRect(start,end,hight);
	}
	else
	{
		start.y -=10;
		end.y  -=10;
		if(count>2)
		{
			//hight =2*(count-2)+4;
			hight =2+count-1;
		}
		else
			hight =2+count-1;
		start.y  +=hight/2;
		end.y   +=hight/2;
		CLineUtil::AddRect(start,end,hight);
	} 

}

void CLineUtil::draw_guogui( const int count ,const bool up,const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	AcGePoint2d mid;
    mid.x=(start.x+end.x)/2;
	mid.y =end.y;
	int hight =7.8;
	int width =4;
	width =2+(count-1)*1.5;
	if(up)
	{
		start.x =mid.x-1;
		 end.x = mid.x+width-1;
		CLineUtil::AddRect(start,end,hight);
	}
	else
	{
		end.x =mid.x+1;
		start.x =mid.x-width+1;
		CLineUtil::AddRect(start,end,hight);
	}
}

void CLineUtil::draw_handong( const int count,const bool up,const AcGePoint3d startpoint,AcGePoint3d endpoint )
{

}

void CLineUtil::draw_horizon_glan_B( const int count,const bool up,const AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	AcGePoint2d start =CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y +=38;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		for(int i=0;i<count;i++)
		{
			CLineUtil::AddPolyLine(start,end,0.5);
			start.y +=1.5;
			end.y +=1.5;
		}

	}
	else
	{
		start.y -=10;
		end.y  -=10;
		for(int i=0;i<count;i++)
		{
			CLineUtil::AddPolyLine(start,end,0.5);
			start.y -=1.5;
			end.y -=1.5;

		}

	}

}

void CLineUtil::draw_vertical_glan_B( const int count,const bool up,const AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	AcGePoint2d mid;
	mid.x =(start.x+end.x)/2;
	mid.y =start.y;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		for(int i=0;i<count;i++)
		{
			AcGePoint2d next =start;
			next.x =mid.x+1.5*i;
			CLineUtil::AddPolyLine(start,next,0.5);
			AcGePoint2d next2 =next;
			next2.y =next.y-20-(count-1)*1.5;
			CLineUtil::AddPolyLine(next,next2,0.5);
			AcGePoint2d next3 =next2;
			next3.x =end.x;
			CLineUtil::AddPolyLine(next2,next3,0.5);
			start.y +=1.5;
			//end.y +=2;
		}


	}
	else
	{
		start.y -=10;
		end.y  -=10;
		for(int i=0;i<count;i++)
		{
			AcGePoint2d next =start;
			next.x =mid.x+1.5*i;
			CLineUtil::AddPolyLine(start,next,0.5);
			AcGePoint2d next2 =next;
			next2.y =next.y+20+1.5*(count-1);
			CLineUtil::AddPolyLine(next,next2,0.5);
			AcGePoint2d next3 =next2;
			next3.x =end.x;
			CLineUtil::AddPolyLine(next2,next3,0.5);
			start.y -=1.5;
			//end.y +=2;
		}

	}


}

void CLineUtil::draw_gangguan_B( const int count ,const bool up,const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	double hight =0;
	hight =2+(count-1)*1.5;
	//int hight =4;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		//hight =(count-1)+2;
	    //求矩形中点
		start.y  =start.y+(count-1)*1.5/2.0;
		end.y   =end.y+(count-1)*1.5/2.0;
		CLineUtil::AddRect(start,end,hight);
	}
	else
	{
		start.y -=10;
		end.y  -=10;
		start.y  =start.y-(count-1)*1.5/2.0;
		end.y   =end.y-(count-1)*1.5/2.0;
		CLineUtil::AddRect(start,end,hight);
	} 

}

void CLineUtil::draw_shnicao_B( const int count ,const bool up,const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
    double hight =0;
	hight=2+(count-1)*1.5;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		//hight =2+(count-1);
		start.y  =start.y+hight/2.0;
		end.y   =end.y+hight/2.0;
		CLineUtil::AddRect(start,end,hight);
	}
	else
	{
		start.y -=10;
		end.y  -=10;
		//hight =2+(count-1);
		start.y =start.y-(count-1)*1.5+(hight)/2.0;
		end.y =end.y-(count-1)*1.5+(hight)/2.0;
		CLineUtil::AddRect(start,end,hight);
	} 


}

void CLineUtil::draw_guogui_B( const int count ,const bool up,const AcGePoint3d startpoint,const AcGePoint3d endpoint )
{
	AcGePoint2d start = CLineUtil::PointConvertTo2d(startpoint);
	AcGePoint2d end =CLineUtil::PointConvertTo2d(endpoint);
	start.y +=38;
	end.y  +=38;
	AcGePoint2d mid;
	mid.x=(start.x+end.x)/2;
	mid.y =end.y;
	float hight =15;
	float width =4;
    width =(count-1)*1.5+2;
	if(up)
	{
		start.x =mid.x-1;
		end.x = mid.x+width-1;
		CLineUtil::AddRect(start,end,hight);
	}
	else
	{
		end.x =mid.x-1+width;
		start.x =mid.x-1;
		CLineUtil::AddRect(start,end,hight);
	}

}

void CLineUtil::draw_handong_B( const bool hanshang,const bool up,const AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	AcGePoint3d start =startpoint;
	AcGePoint3d end =endpoint;
	start.y +=38;
	end.y  +=38;
	if(up)
	{
		if(hanshang)
		{
			start.y +=10;
			end.y  +=10;
			//光缆涵上过
			start.y -=1.5;
			end.y  -=1.5;
			AcGePoint3d leftdown =start;
			leftdown.x -=2;
			leftdown.y -=2;
			CLineUtil::Add(leftdown,start);
			CLineUtil::Add(start,end);
			AcGePoint3d rightdown =end;
			rightdown.x +=2;
			rightdown.y -=2;
			CLineUtil::Add(end,rightdown);
			AcGePoint3d Center;
			Center.x =(start.x+end.x)/2;
			Center.y = start.y -4.5;
			AcGeVector3d vec(0,0,1);
			double radius =3.22;
			AcDbCircle *pCircle=new AcDbCircle(Center,vec,radius);
			CDwgDatabaseUtil::PostToModelSpace(pCircle);
		}
		else
		{
			//光缆涵下过
			start.y +=22;
			end.y  +=22;
			start.y -=0.3;
			end.y  -=0.3;
			AcGePoint3d leftdown =start;
			leftdown.x -=2;
			leftdown.y -=2;
			CLineUtil::Add(leftdown,start);
			CLineUtil::Add(start,end);
			AcGePoint3d rightdown =end;
			rightdown.x +=2;
			rightdown.y -=2;
			CLineUtil::Add(end,rightdown);
			AcGePoint3d Center;
			Center.x =(start.x+end.x)/2;
			Center.y = start.y -4.5;
			AcGeVector3d vec(0,0,1);
			double radius =3.22;
			AcDbCircle *pCircle=new AcDbCircle(Center,vec,radius);
			CDwgDatabaseUtil::PostToModelSpace(pCircle);

		}

	}
	else
	{
		if(hanshang)
		{
			start.y -=14;
			end.y  -=14;
			//光缆涵上过
			start.y -=0.3;
			end.y  -=0.3;
			AcGePoint3d leftdown =start;
			leftdown.x -=2;
			leftdown.y -=2;
			CLineUtil::Add(leftdown,start);
			CLineUtil::Add(start,end);
			AcGePoint3d rightdown =end;
			rightdown.x +=2;
			rightdown.y -=2;
			CLineUtil::Add(end,rightdown);
			AcGePoint3d Center;
			Center.x =(start.x+end.x)/2;
			Center.y = start.y -4.5;
			AcGeVector3d vec(0,0,1);
			double radius =3.22;
			AcDbCircle *pCircle=new AcDbCircle(Center,vec,radius);
			CDwgDatabaseUtil::PostToModelSpace(pCircle);

		}
		else
		{
			//光缆涵下过
			start.y -=1.5;
			end.y  -=1.5;
			AcGePoint3d leftdown =start;
			leftdown.x -=2;
			leftdown.y -=2;
			CLineUtil::Add(leftdown,start);
			CLineUtil::Add(start,end);
			AcGePoint3d rightdown =end;
			rightdown.x +=2;
			rightdown.y -=2;
			CLineUtil::Add(end,rightdown);
			AcGePoint3d Center;
			Center.x =(start.x+end.x)/2;
			Center.y = start.y -4.5;
			AcGeVector3d vec(0,0,1);
			double radius =3.22;
			AcDbCircle *pCircle=new AcDbCircle(Center,vec,radius);
			CDwgDatabaseUtil::PostToModelSpace(pCircle);
		}

	}
}

void CLineUtil::add_daokou( const int count,const bool up,AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	AcGePoint3d start =startpoint;
	AcGePoint3d end =endpoint;
	start.y +=38;
	end.y  +=38;
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		AcGePoint3d mid =start;
		mid.x =(start.x+end.x)/2;
		mid.y =mid.y+(count-1)*1.5;
		AcGePoint3d pt1=mid;
		pt1.x -=2;
		pt1.y -=6;
		AcGePoint3d pt2=pt1;
		pt2.y +=12;
		CLineUtil::Add(pt1,pt2);
		AcGePoint3d pt3=mid;
		pt3.x +=2;
		pt3.y -=6;
		AcGePoint3d pt4=pt3;
		pt4.y +=12;
		CLineUtil::Add(pt3,pt4);
		AcGePoint3d pt5=mid;
		pt5.y -=8;
		CTextUtil::AddText(pt5,_T("道口"));
	}
	else
	{
		start.y -=10;
		end.y  -=10;
		AcGePoint3d mid =start;
		mid.x =(start.x+end.x)/2;
		mid.y =mid.y-(count-1)*1.5;
		AcGePoint3d pt1=mid;
		pt1.x -=2;
		pt1.y -=6;
		AcGePoint3d pt2=pt1;
		pt2.y +=12;
		CLineUtil::Add(pt1,pt2);
		AcGePoint3d pt3=mid;
		pt3.x +=2;
		pt3.y -=6;
		AcGePoint3d pt4=pt3;
		pt4.y +=12;
		CLineUtil::Add(pt3,pt4);
		AcGePoint3d pt5=mid;
		pt5.y -=8;
		CTextUtil::AddText(pt5,_T("道口"));
	}


}
void CLineUtil::add_hegou( const int count,const bool up,AcGePoint3d startpoint,AcGePoint3d endpoint )
{
	AcGePoint3d start =startpoint;
	AcGePoint3d end =endpoint;
	start.y +=38;
	end.y  +=38;
	TCHAR *dwgFilename =_T("C:\\辅助开发\\tuli.dwg");
	TCHAR *blkName =_T("guogou");
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		AcGePoint3d mid =start;
		mid.x =(start.x+end.x)/2;
		mid.y =mid.y+(count-1)*1.5/2;
		//CBlockUtil::InsertDwgBlockDef(dwgFilename,blkName,1);
		AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
		CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		AcGePoint3d pt1=mid;
		pt1.y =pt1.y+6+(count-1)*1.5/2;
		CTextUtil::AddText(pt1,_T("过沟"));
	}
	else
	{
		start.y -=10;
		end.y  -=10;
		AcGePoint3d mid =start;
		mid.x =(start.x+end.x)/2;
		mid.y =mid.y-(count-1)*1.5/2;
		//CBlockUtil::InsertDwgBlockDef(dwgFilename,blkName,1);
		AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
		CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		AcGePoint3d pt1=mid;
		pt1.y =pt1.y-6-(count-1)*1.5/2;
		CTextUtil::AddText(pt1,_T("过沟"));
	}

}

void CLineUtil::add_cexian( const int count,const bool up,AcGePoint3d startpoint,AcGePoint3d endpoint,CString gudao )
{
	//注意 最多支持3股侧线
	AcGePoint3d start =startpoint;
	AcGePoint3d end =endpoint;
	start.y +=38;
	end.y  +=38;
	TCHAR *dwgFilename =_T("C:\\辅助开发\\tuli.dwg");
	TCHAR *blkName =_T("cxguogui");
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		AcGePoint3d mid =start;
		if(gudao.Compare(_T("1股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y+(count-1)*1.5/2;
			//CBlockUtil::InsertDwgBlockDef(dwgFilename,blkName,1);
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("2股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y+(count-1)*1.5/2;
			mid.x -=9;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.x +=9;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("3股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y+(count-1)*1.5/2;
			mid.x -=9;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.x +=9;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.x +=9;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else
		{

		}
		

	}
	else
	{
		start.y -=10;
		end.y  -=10;
		AcGePoint3d mid =start;
		mid.x =(start.x+end.x)/2;
		mid.y =mid.y-(count-1)*1.5/2;
		if(gudao.Compare(_T("1股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y-(count-1)*1.5/2;
			//CBlockUtil::InsertDwgBlockDef(dwgFilename,blkName,1);
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("2股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y-(count-1)*1.5/2;
			mid.x -=9;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.x +=9;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("3股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y-(count-1)*1.5/2;
			mid.x -=9;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.x +=9;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.x +=9;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else
		{

		}

	}

}

void CLineUtil::add_zhenxian( const int count,const bool up,AcGePoint3d startpoint,AcGePoint3d endpoint,CString gudao )
{
	AcGePoint3d start =startpoint;
	AcGePoint3d end =endpoint;
	start.y +=38;
	end.y  +=38;
	TCHAR *dwgFilename =_T("C:\\辅助开发\\tuli.dwg");
	TCHAR *blkName =_T("zxguogui");
	if(up)
	{
		start.y +=10;
		end.y  +=10;
		AcGePoint3d mid =start;
		if(gudao.Compare(_T("1股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y-6;
			//CBlockUtil::InsertDwgBlockDef(dwgFilename,blkName,1);
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("2股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y -=4;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.y -=3;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("3股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y -=4;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.y -=2;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.y -=2;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else
		{

		}

	}
	else
	{
		start.y -=10;
		end.y  -=10;
		AcGePoint3d mid =start;
		if(gudao.Compare(_T("1股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y =mid.y+6;
			//CBlockUtil::InsertDwgBlockDef(dwgFilename,blkName,1);
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("2股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			mid.y +=4;
				AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.y +=3;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else if(gudao.Compare(_T("3股轨道"))==0)
		{
			mid.x =(start.x+end.x)/2;
			//mid.y =mid.y+(count-1)*1.5/2;
			mid.y +=4;
			AcDbObjectId blockid = CBlockUtil::CopyBlockDefFromOtherDwg(dwgFilename,blkName);
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.y +=2;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
			mid.y +=2;
			CBlockUtil::InsertBlockRefWithAttribute(blockid,mid);
		}
		else
		{

		}

	}

}
