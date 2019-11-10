#ifndef H_ROOM_CHARACTER_CLASS___
#define H_ROOM_CHARACTER_CLASS___
// ===================================================================
// CRoomCharacter.h
//	�����ɂ���L�����N���X�E�w�b??
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include "util.h"
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <map>
#include <iostream>
#include <vector>

#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"

#include "TextureLoader.h"

// �������ړ�
// �L������ς�����
// �A�j��?�V��������
// ���������̏�ɂł�
// ?��OK�Ƃ��ړ��ł��Ȃ�����

// �����̃L�����N���X
class CRoomCharacter
{
public:
	CRoomCharacter();					// 
	virtual ~CRoomCharacter() 
	{ 
		if (m_bCreated)
			Destroy();
	};

	// ����
	// pDev			: D3D�f�o�C�X
	// pTexture	: �e�N�X?��
	// nGUIResourceIndex : ��?��GUI��??�X�ԍ�
	// pStatic		: �X?�e�B�b�N
	// pSess		: �I�u�W�F�N�g���
	// nSpriteWidth,nSpriteHeight : �e�N�X?���T�C�Y
	// nCharaWidth,nCharaHeight : ?���T�C�Y
	// nAnimationCount : ��Ұ��??
	BOOL Create(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex, CDXUTDialog* pDialog, type_session* pSess, int nSpriteWidth, int nSpriteHeight, int nAnimationCount, int nCharaWidth, int nCharaHeight);
	BOOL Create(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex,  CDXUTDialog* pDialog, type_session* pSess, int nSpriteWidth, int nSpriteHeight, int nCharaLeft, int nCharaTop, int nCharaWidth, int nCharaHeight);

	// �e�N�X?���X�V
	// pTexture	: �e�N�X?��
	// nSpriteWidth,nSpriteHeight : ?���T�C�Y
	// nAnimationCount : ��Ұ��??
	BOOL UpdateTexture(LPDIRECT3DTEXTURE9 pTexture, int nSpriteWidth, int nSpriteHeight, int nAnimationCount, int nCharaWidth, int nCharaHeight);

	// �e�N�X?���X�V
	BOOL UpdateTexture(LPDIRECT3DTEXTURE9 pTexture, int nSpriteWidth, int nSpriteHeight, int nLeft, int nTop, int nRight, int nBottom);

	// ��ԍX�V
	void UpdateState();

	void Destroy();

	void Frame();
	// ����
	void Say(WCHAR *message);

	BOOL IsCreated()
	{ return m_bCreated;	};
	
	// ?��
	// pDev	: D3D�f�o�C�X
	// fElapsedTime : �o�ߎ���
	void Render(LPDIRECT3DDEVICE9 pDev, float fElapsedTime);
	
	void SetVisible(bool bVisible);	
	BOOL GetVisible(bool bVisible)
	{	return m_bVisible;			};

	void Hide();
	void Show();
	
	// OnLostDeviceEvent
	void OnLost();
	// OnResetDeviceEvent
	void OnReset();

protected:
	BOOL CreateControls(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex,  CDXUTDialog* pDialog, type_session* pSess, int nSpriteWidth, int nSpriteHeight, int nAnimationCount, int nCharaWidth, int nCharaHeight);
	void CalcDrawRect();								// ?��͈͌v�Z

	type_session*	p_pSession;					// �I�u�W�F�N�g���
	CDXUTDialog*	p_pDialog;
	
	int m_nAnimationIndex;							// ��Ұ��?`��͈̓C���f�b�N�X
	int m_nAnimationCount;							// ��Ұ��?`��͈͐�

	int m_nAnimationTimeCounter;				// ��Ұ��ݲ��ޯ���̐��l�𑝂₷����
	int m_nAnimationTime;							// ��Ұ��ݲ��ޯ���̐��l�𑝂₷����
	
	int m_nTextureWidth;							// �e�N�X?������
	int m_nTextureHeight;							//	�e�N�X?���c��
	LPDIRECT3DTEXTURE9 p_pTexture;		// �e�N�X?��
	CDXUTButton*	m_pControlWord;			// ����?���p�R���g��?��
	CDXUTStatic*	m_pControlName;			// ���O?���p�R���g��?��
	int m_nControlNameWidth;
	int m_nControlNameHeight;

	D3DXMATRIXA16	m_matScale;				// ?���T�C�Y�ւ̃X�P?�����O?�g���b�N�X

	int m_nWordViewTimeCounter;				// ����?�����ԃJ�E��?
	int m_nWordViewTime;							// ����?������

	D3DXVECTOR3	m_vecDrawCharaCenter;
	int m_nControlNameAdjustX;
	int m_nControlNameAdjustY;

	int m_nCharaWidth;
	int m_nCharaHeight;

	BOOL m_bCreated;								// �����ς�

	RECT m_recDraw;									// ?��͈�	
	bool m_bVisible;									// ?�����
};

#endif
