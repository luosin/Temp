#pragma once

class CUserTools
{
public:
	CUserTools(void);
	virtual ~CUserTools(void);

	static int ZhfTools(void);
	// 镜像实体
	static void MirrorEnt(void);
	// 旋转块，解决文字颠倒问题
	static void RotationEnt(void);
	// 调整多段线和起始点相邻的顶点，使其和起始点平行或垂直（微调）
	static void AdjustPlineVertex(void);
	static void RefreshDwg(void* pData);
	static int ReplaceEnt();
	static int SetBlkAtt(int iMode) ;
	// 将起点x坐标大于终点x坐标的顶点逆向反置一下（顺正）
    static void ReverseLinePoint(void);
	// 给多段线增加顶点
	static void AddVertex(void);

	// 炸开命名以A$开头的块
	static int ExplodeSpeBlk(void);
	// 增强版DDEDIT
	static void MyEdit(void);
	// 类cad中单行文本text命令
	static void MyText(void);
	// 批量输入文本
	static void BText(void);
	// cad命令find增强版
	static void MyFind(void);
	// 属性文本和多行文本转换为单行文本
	static void AtMtToTxt(void);
	// 批量修改文字大小、style、对齐方式
	static void MoText(void);
	// 对齐文本
	static void AlignText(void);
	// 批量修改文本
	static void BEdit(void);
	// 删除选中的文本的xdata
	static void DelSelXdata(void);
	// excel表格转换成cad中的表格
	static void XlsToDwg(void);

	//excel表格转换成联锁表
	static void XlsToLsb(void);

	//static void AddToLsb();
	static void AddToLsb(AcGePoint3d start_pt_3d,float f_length_xls);
	// 设置扩展数据xdata
	static void SetXdata(void);
	// 显示扩展数据xdata
	static void ShowXdata(void);
	// 替换块
	static void ReplaceBlock(void);
	// 统计线段长度（所有AcDbCurve）
	static void SumLineLen(void);
	//刷新图纸，主要是某些程序写入的文字没有明确指定对齐点，导致图形显示不正确，移动一下可解决。在单文档模式下有效
	static void RefreshDwg(void);
	// 批量修改文件名称
	static void MoFileName(void);
	// 显示/隐藏块属性
	static void ShowBlkAtt(void);
	// 标出ID相同的实体
	static void CheckSameId(void);
	// 取消标注ID相同实体
	static void UnCheckSameId(void);
	static void HideXdata(void);
	// 对选中的数值统一加减某数值
	static void AddNum(void);
	// 替换某个APP值中的文本
	static void ReplaceAppValue(void);

	// 显示实体Handle
	static int ZList(void);
	// 删除指定app
	static void DelOneApp(void);
	// CAD中表格转换成excel表格
	static void DwgToXls(void);
	//在将excel转换成cad表格时计算对应height和width
	static void GetHW(void);
	//删除空文本
	static void DelNullText(void);
	// 删除扩展数据
	static void DelXdata(void);
	//统计电缆
	static void SumDianLan(void);

	// 单线转换成多段线
	static int ConvertToPolyline(void);
	// 合并多段线
	static int JoinLine(void);
	// 给块增加属性
	static void AddAtt(void);
	// 将图纸中width为dFrom的转换为dTo
	static void SetPlineWidth(void);
	static void SetPlineWidth(AcDbDatabase* pDb, double dFrom, double dTo, BOOL bIncludeBlk=TRUE);
	static void SetPlineWidth(AcDbBlockReference* pRef, double dFrom, double dTo);

	// 批量修改图库（单线to多段线，或者反向操作）
	static int ReplaceTUKU();


};
