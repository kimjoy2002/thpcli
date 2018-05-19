#ifndef H_STAGE_CHARACTER_CLASS___
#define H_STAGE_CHARACTER_CLASS___
// ===================================================================
// CStageCharacter.h
//	�X�e�[�W�ɂ���L�����N���X�E�w�b�_�[
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include "../include/types.h"
#include "../include/define.h"
#include "util.h"
#include <map>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include "../lib/DXUT/dxstdafx.h"
#include "../lib/DXUT/DXUT.h"
#include "resource.h"

#include "TextureLoader.h"
#include "LuaHelper.h"

// �L�������\���ł���
// HP�̃o�[���\���ł���
// �L���������\���ł���
// �X�e�[�W���ړ��ł���
// �ړ����鎞�A�j���[�V��������
// �e�����Ă�
// �A�C�e�����g����

// �X�e�[�W�̃L�����N���X
class CStageCharacter
{
public:
	CStageCharacter();
	virtual ~CStageCharacter() 
	{ 
		if (m_bCreated)
			Destroy();
	};

	// ����
	// pDev			: D3D�f�o�C�X
	// pTexture	: �e�N�X�`��
	// nGUIResourceIndex : ��{��GUI���\�[�X�ԍ�
	// pStatic		: �X�^�e�B�b�N(�L�������p)
	// pSess		: �I�u�W�F�N�g���
	HRESULT Create(std::map < int, TCHARA_SCR_INFO > *mapScrInfo, int nGUIResourceIndex, CDXUTDialog* pDialog, type_session* pSess, int nStageWidth, int nStageHeight);

	// �X�V
	// pTexture	: �e�N�X�`��
	// nGUIResourceIndex : ��{��GUI���\�[�X�ԍ�
	// nSpriteWidth,nSpriteHeight : �\���T�C�Y
	// nAnimationCount : ��Ұ��ݐ�
//	HRESULT Update(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex, int nSpriteWidth, int nSpriteHeight, int nAnimationCount);

	// ��ԍX�V
//	void UpdateState();
	// �j��
	void Destroy();
	// ���t���[��
	void Frame();

	BOOL IsCreated()
	{ return m_bCreated;	};

	// ����
	void SetDead();

	void Update()
	{
		if (m_bCreated)
		{
			CalcDrawPos();
			CalcDrawRect();
			UpdateState();
		}
	}
	void CalcDrawRect();
	void CalcDrawPos();
	// ��ԍX�V
	void UpdateState();
	// �^�[����ԃZ�b�g
	void SetMyTurn(bool bTurn);

	void ChangeHP(int value);

	// �`��
	// pDev	: D3D�f�o�C�X
	// fElapsedTime : �o�ߎ���
	void Render(LPDIRECT3DDEVICE9 pDev, float fElapsedTime, D3DXMATRIX* matChara, D3DXMATRIX* matStage, D3DCOLOR color=0xFFFFFFFF);
	
	void SetVisible(bool bVisible);
	bool GetVisible()
	{	return m_bVisible;			};
	void SetVisibleInfo(bool bVisible){	m_bVisibleInfo = bVisible;	};
	bool GetVisibleInfo(){	return m_bVisibleInfo;	};

	void SetTurn(bool bTurn)
	{	m_pMyTurn->SetVisible(bTurn);	};

	void SetTexture(RECT* prcTexture);

	// OnLostDeviceEvent
	void OnLost();
	// OnResetDeviceEvent
	void OnReset();

	ptype_session	GetSession();

protected:
	type_session*	p_pSession;					// �I�u�W�F�N�g���
	
	int m_nAnimationIndex;							// ��Ұ��ݕ`��͈̓C���f�b�N�X
	int m_nAnimationCount;							// ��Ұ��ݕ`��͈͐�

	int m_nAnimationTimeCounter;				// ��Ұ��ݲ��ޯ���̐��l�𑝂₷����
	int m_nAnimationTime;							// ��Ұ��ݲ��ޯ���̐��l�𑝂₷����
	
	int m_nTextureWidth;							// �e�N�X�`������
	int m_nTextureHeight;							//	�e�N�X�`���c��
	LPDIRECT3DTEXTURE9 p_pTexture;		// �e�N�X�`��

	int m_nCharaWidth;
	int m_nCharaHeight;
	D3DXMATRIXA16	m_matScale;				// �\���T�C�Y�ւ̃X�P�[�����O�}�g���b�N�X
	float m_fAdjustPosY;

	CDXUTDialog*	p_pDialog;
	CDXUTStatic*	m_pControlName;			// ���O�\���p�R���g���[��
	CDXUTMeter*	m_pHPBar;					// HP�o�[
	CDXUTSprite*	m_pMyTurn;					// �^�[���\��
	CDXUTStatic*	m_pStatusInfo;				// �_���[�W�\��
	int	m_nStatuInfoTime;

	int m_nControlNameWidth;					// 
	int m_nControlNameHeight;
	int m_nGUIResourceIndex;						// ���\�[�X�ԍ�

	BOOL m_bCreated;								// �����ς�
	TCHARA_SCR_INFO*  m_pScrInfo;			// �X�N���v�g���

	D3DXMATRIX		m_matDrawCharaRadian;
	D3DXVECTOR3	m_vecDrawCharaCenter;	// �`��ʒu
	D3DXVECTOR3	m_vecDrawCharaPos;	// �`��ʒu
	RECT m_recDraw;									// �`��͈�	
	bool m_bVisible;									// �\�����
	bool m_bVisibleInfo;								// �L�����̏��\��
	int m_nBodyRange;								// �����蔻��p���a
	BOOL m_bMyTurn;
	void UpdateMyTurnTexture();

	int m_nStageWidth;
	int m_nStageHeight;

};

#endif
