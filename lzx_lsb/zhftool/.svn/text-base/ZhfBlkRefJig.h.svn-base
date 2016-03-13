#pragma once
// #include "dbjig.h"
// #include "gepnt3d.h"

class CZhfBlkRefJig :
	public AcEdJig
{
public:
	CZhfBlkRefJig(CGsPreviewCtrl* pCtrl);
	~CZhfBlkRefJig(void);
private:
	AcDbBlockReference* m_pBlkRef;
	AcGePoint3d m_pt3dCur;
public:
	// -----------------------------------------------------------------------------
	virtual AcEdJig::DragStatus sampler(void);
	// -----------------------------------------------------------------------------
	virtual Adesk::Boolean update(void);
	// -----------------------------------------------------------------------------
	AcDbObjectId append(void);
	// -----------------------------------------------------------------------------
	virtual AcDbEntity * entity(void) const;
	void DoIt(void);
private:
	CString m_strBlkFile;
	CGsPreviewCtrl* m_pCtrlGsPreview;
};
