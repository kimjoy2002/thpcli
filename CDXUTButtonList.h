#ifndef H_DXUT_BUTTON_LIST_CLASS___
#define H_DXUT_BUTTON_LIST_CLASS___
// ===================================================================
// CDXUTButtonList.h
//	DXUTButton����ׂ�N���X�E�w�b�_�[
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include <vector>

#include "../DXUT/dxstdafx.h"
#include "../DXUT/DXUT.h"

#define 	BUTTON_LIST_HORIZON	(0x00)
#define 	BUTTON_LIST_VERTICAL	(0x01)

class CDXUTButtonList
{
public:
	CDXUTButtonList();
	virtual ~CDXUTButtonList() { m_vecButtons.clear(); };
	// ����
	// pBtnArrowL	: �����{�^��
	// pBtnArrowR	: �E���{�^��
	// pntLocate		: ����ʒu
	// nDrawRange	: �`��͈͂̒���
	// bytStyle			: �`��X�^�C��	0x00 �� / 0x01 �c
	// nSpace			: �{�^���ƃ{�^���̊Ԋu
	void Create(CDXUTButton *pBtnArrowL, CDXUTButton *pBtnArrowR, POINT* pntLocate, int nDrawRange, BYTE bytStyle = 0, int nSpace = 0);

	void Clear();
	int AddButton(CDXUTControl* pBtn);
	int InsertButton(int index, CDXUTControl* pBtn);

	void OnRightButtonDown(int range=0);
	void OnLeftButtonDown();

	void SetLRPadding(int nValue)
	{	m_nLRPadding = nValue;	};
	int GetLRPadding()
	{	return m_nLRPadding;	};

	void SetLocation(int lx,int ly)
	{	m_pntLocate.x = lx;
		m_pntLocate.y = ly;
	};

	void SetDrawRange(int nValue)
	{
		m_nDrawRange = nValue;
		UpdateDrawRange();
	};
	int GetDrawrange(){ return m_nDrawRange;	 };

	void SetVisible(bool b);
	BOOL GetVisible()
	{	return m_bVisible;	};
	void SetEnable(bool b);
	bool GetEnable()
	{	return m_bEnable;	};
	
	BOOL GetLRBtnAlwaysVisible()
	{	return m_bLRBtnAlwaysVisible;	};
	void SetLRBtnAlwaysVisible(BOOL value)
	{	m_bLRBtnAlwaysVisible = value;	};
protected:
	
	void UpdateDrawRange();
	void CalcViewIndex();
	inline BOOL IsVisibleArrowButton()
	{
		if (m_bLRBtnAlwaysVisible)
			return TRUE;
		return (m_nTotalRange > m_nDrawRange);
	}

	std::vector< CDXUTControl* > m_vecButtons;
	BOOL	m_bCreated;
	BOOL	m_bVisible;
	POINT	m_pntLocate;
	int			m_nDrawRange;
	int			m_nViewIndex;
	int			m_nLRPadding;
	int			m_nSpace;
	CDXUTButton *m_pBtnArrowL;
	CDXUTButton *m_pBtnArrowR;
	int			m_nTotalRange;
	BYTE m_bytStyle;
	BOOL	m_bLRBtnAlwaysVisible;
	bool		m_bEnable;
};

#endif
