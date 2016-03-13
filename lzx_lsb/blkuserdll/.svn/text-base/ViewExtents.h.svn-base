#pragma once
#include "gepnt3d.h"

#ifdef ZHFDLL_EXPORT
#define ZHFDLL __declspec(dllexport)
#else
#define ZHFDLL __declspec(dllimport)
#endif

class ZHFDLL CViewExtents
{
public:
	CViewExtents(void);
	~CViewExtents(void);
	AcGePoint3d m_pt3dCen;
	double m_dWidth;
	double m_dHeight;
};
