#pragma once

class CZhfPlCableJig : public AcEdJig
{
public:
	CZhfPlCableJig();
	~CZhfPlCableJig(void);
	CZhfPlCableJig(const AcGePoint2dArray& pt2dArray);
	// -----------------------------------------------------------------------------
	virtual AcDbEntity * entity(void) const;
	// -----------------------------------------------------------------------------
	virtual AcEdJig::DragStatus sampler(void);
	// -----------------------------------------------------------------------------
	virtual Adesk::Boolean update(void);
	int doIt(void);

private:
	AcDbPolyline* m_pPolyLine ;
	AcGePoint3d m_pt3dCur;
public:
	// pick点所在的箱盒
	int GetPickBox(const AcGePoint3d& pt_3d_pick, long& nHandleBox);

	long m_nHandleBoxStart;
	long m_nHandleBoxEnd;
	// 电缆类型：0，箱盒之间的电缆；1，信号楼主电缆；2，起始连接盒子，末端连在信号楼主电缆
	int m_iType;
	// 主要是对已存在数据库的实体进行重新drag
	int m_iMode;

	AcGePoint2dArray m_pt2dArray ;

};
