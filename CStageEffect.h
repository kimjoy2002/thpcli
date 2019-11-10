#ifndef H_STAGE_EFFECT_CLASS___
#define H_STAGE_EFFECT_CLASS___
// ===================================================================
// CStageEffect.h
//	�X�e?�W�G�t�F�N�g�N���X�E�w�b??
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include "util.h"
#include <map>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"
#include "resource.h"

#include "TextureLoader.h"
#include "LuaHelper.h"

/*
�G�t�F�N�g
?�E�A�j��?�V��������
?�E�����Ă�
?�E���W�A�g��A�k���A��?����
?X ���̉�ʂ��Â������Ԃ����i�J�b�g�C���p�H�j
*/
class CStageEffect
{
public:
	CStageEffect();
	virtual ~CStageEffect();
#ifdef HOGE
	BOOL Create(int nEffectNo, LPDIRECT3DTEXTURE9 pTexture, RECT* rcTexture, D3DXVECTOR3* pos, int nAge, BOOL force);
#else
	BOOL Create(int nEffectNo, LPDIRECT3DTEXTURE9 pTexture, RECT* rcTexture, D3DXVECTOR3* pos, int nAge);
#endif
	int	GetEffectNo() { return m_nEffectNo;	};
//	void SetEffectNo(int value) { m_nEffectNo = value;	};

	void Render(LPD3DXSPRITE pSprite, D3DXMATRIX* matWorld);
	BOOL Frame();

	void SetEffectVector(D3DXVECTOR3* vec, D3DXVECTOR3* add, int nEffectTime=0)
	{
		m_nVectorTimeCounter = 0;
		m_nVectorEffectTime = nEffectTime;
		m_vecVector = *vec;
		m_vecAddVec = *add;
	};
	void SetEffectAlpha(BYTE bytAlpha)	{ m_bytAlpha = bytAlpha;	};
	void SetEffectFade(int nFade, int nEffectTime=0)
	{
		m_nFadeTimeCounter = 0;
		m_nFadeEffectTime = nEffectTime;
		m_nFade = nFade;
	};
	void SetEffectFadeInOut(int nFadeInOutTime) {m_nFadeInOut = nFadeInOutTime;	};
	void SetEffectRotate(float fRot){ m_fRotateDraw = fRot; };
	void SetEffectRotation(float fRot, int nEffectTime=0){ m_fRotation = fRot; };
	void SetEffectScale(float fScaleX, float fScaleY)
	{
		m_vecScallingDraw.x = fScaleX;
		m_vecScallingDraw.y = fScaleY;
	};
	void SetEffectScalling(float fScaleX, float fScaleY, int nEffectTime=0)
	{
		m_nScallingTimeCounter = 0;
		m_nScallingEffectTime = nEffectTime;
		m_vecScalling.x = fScaleX;
		m_vecScalling.y = fScaleY;
	};
	inline float safeFloatAdd(float fa, float fb, float fdef)
	{
		float fres = fa + fb;
		if (!_finite(fres)) { return fdef;	}
		return fres;
	};
	void SetEffectAnimation(int nATime, int nACount, BOOL bLoop, int nEffectTime=0)
	{
		m_nAnimationEffectTime = nEffectTime;
		m_nAnimationIndex = 0;
		m_nAnimationTimeCounter = 0;
		m_nAnimationTime = nATime;
		m_nAnimationCount = nACount;
		m_bAnimationLoop = bLoop;
	};
	void SetEffectTexture(RECT* rcTexture);

	int GetAgeCount(){ return m_nAgeCounter;	};

protected:
	int						m_nAge;
	int						m_nAgeCounter;
	BOOL				m_bReady;
	int						m_nEffectNo;			// �Ǘ��ԍ�

	LPDIRECT3DTEXTURE9	p_pTexture;	// �g�p����e�N�X?��
	RECT					m_rcTexture;				// ?��T�C�Y
	D3DXVECTOR3	m_vecPosition;			// �ʒu
	D3DXVECTOR3	m_vecVector;			// �ړ��l
	D3DXVECTOR3	m_vecAddVec;			// �����l

	D3DXVECTOR3	m_vecScalling;			// �g��k��

	int						m_nAnimationTime;	// ���A�j��?�V�����C���f�b�N�X�ֈڂ鎞��
	int						m_nAnimationTimeCounter;	// ���Ԃ̃J�E��?

	int						m_nAnimationCount;	// �A�j��?�V������
	int						m_nAnimationIndex;	// �A�j��?�V�����C���f�b�N�X
	BOOL				m_bAnimationLoop;

	int						m_nFade;					// �t�F?�h
	int						m_nFadeInOut;			// �t�F?�h�C���A�E�g����
	
	RECT					m_rcDraw;				// ?��͈�
	D3DXVECTOR3	m_vecCenter;			// ��?�̒��S
	D3DCOLOR		m_clrDraw;				// ?��F
	BYTE					m_bytAlpha;				// �A���t?
	FLOAT				m_fRotateDraw;		// ��?�l
	FLOAT				m_fRotation;				// ��?�l

	D3DXVECTOR3	m_vecScallingDraw;	// �g��k��
	
	int						m_nFadeTimeCounter;
	int						m_nVectorTimeCounter;
	int						m_nScallingTimeCounter;
	int						m_nRotationTimeCounter;
	int						m_nRotationEffectTime;
	int						m_nFadeEffectTime;			// �t�F?�h���ʎ���
	int						m_nAnimationEffectTime;	// �A�j��?�V�������ʎ���
	int						m_nScallingEffectTime;		// �g��k�����ʎ���
	int						m_nVectorEffectTime;		// �ړ����ʎ���
#ifdef HOGE
	BOOL					m_bForce;
#endif
};

#endif