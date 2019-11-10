#ifndef H_TEAM_SEPARATER_CLASS___
#define H_TEAM_SEPARATER_CLASS___
// ===================================================================
// CTeamSeparater.h
//	???�킯�̐����o���N���X�E�w�b??
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include "util.h"
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <map>
#include <iostream>
#include <vector>

#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"

// ???�ԍ��i?�A�B�c�j�̈ʒu���
// ???�̋�؂����?��

#define ROOM_SEPARATER_OFFSET_X	0
#define ROOM_SEPARATER_OFFSET_Y	(ROOM_CHARA_BASE_OFFSET_Y+32)
#define ROOM_SEPARATER_OFFSET_LINE_Y	(ROOM_CHARA_BASE_OFFSET_Y+16)
#define ROOM_SEPARATER_OFFSET_H	(ROOM_CHARA_BASE_MAX_MOVE_H+16)

#define IDC_TEAM_SEPARAT_BG			8999
#define IDC_TEAM_SEPARAT_BASE		9000

#define ROOM_SEPARATER_RES_X			0
#define ROOM_SEPARATER_RES_Y			0
#define ROOM_SEPARATER_RES_W		(ROOM_SEPARATER_RES_X+WIN_WIDTH)
#define ROOM_SEPARATER_RES_H			(ROOM_SEPARATER_RES_Y+ROOM_SEPARATER_OFFSET_H)

#define ROOM_SEPARATER_LINE_COLOR	0xFF000000

// ???�킯�N���X
class CTeamSeparater
{
public:
	CTeamSeparater()
	{
//		m_pBGImage = NULL;
		m_nSeparate = 0;
		m_nGUIResourceIndex = -1;
		p_pDialog = NULL;
		ZeroMemory(m_pArrayTeamNum, sizeof(CDXUTButton*)*MAX_TEAM_COUNT);
		m_bCreated = FALSE;
		SetRectEmpty(&m_recTeam);
		m_bVisible = false;
	};

	virtual ~CTeamSeparater() 
	{ 
		if (m_bCreated)	Destroy();
	};

	// ����
	// nGUIResourceIndex	: ��??�X�ԍ�
	// pDialog						: �X?�e�B�b�N
	// nSeparate					: �I�u�W�F�N�g���
	HRESULT Create(int nGUIResourceIndex, CDXUTDialog* pDialog, int nSeparate);

	// �X�V
	// nSeparate	:	???��
	void Update()
	{	Update(m_nSeparate);	};
	// nSeparate	:	???��
	void Update(int nSeparate);
		
	void Destroy();
	
	BOOL IsCreated()	{ return m_bCreated;	};

	// ���W����???�ԍ����擾
	int GetTeamNoFromPos(int x, int y);
	WCHAR* GetTeamWchar(int num);
	D3DCOLOR GetTeamColor(int num);

	// ?��
	void Render();
	
	void SetVisible(bool bVisible);
	BOOL GetVisible()
	{	return m_bVisible;	};

protected:

	// ???�ԍ�?���p�R���g��?��
	CDXUTButton*	m_pArrayTeamNum[MAX_TEAM_COUNT+1];
	CDXUTDialog*	p_pDialog;
//	CDXUTButton*	m_pBGImage;
	
	BOOL	m_bCreated;								// �����ς�
	RECT		m_recTeam;								// �P???��?��͈�
	bool		m_bVisible;								// ?�����
	int			m_nGUIResourceIndex;				// ��??�X�ԍ�
	int			m_nSeparate;							// ������

	POINT	m_pntLine[MAX_TEAM_COUNT][2];	// ������
};

#endif
