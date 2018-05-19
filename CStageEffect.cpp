#include "CStageEffect.h"
#include "ext.h"

CStageEffect::CStageEffect()
{
	m_nAge = 0;
	m_nAgeCounter = 0;
	m_nEffectNo = -1;
	p_pTexture = NULL;
	SetRectEmpty(&m_rcTexture);							// �`��͈�
	SetRectEmpty(&m_rcDraw);
	m_vecPosition = D3DXVECTOR3(0,0,0);				// �ʒu
	m_vecVector = D3DXVECTOR3(0,0,0);				// �ړ��l
	m_vecAddVec = D3DXVECTOR3(0,0,0);				// �����l
	m_vecScalling = D3DXVECTOR3(0,0,0);				// �g��k��
	m_vecScallingDraw = D3DXVECTOR3(1,1,1);				// �g��k��
	m_fRotation = 0.0f;												// ��]�l
	m_clrDraw = 0xFFFFFFFF;
	m_bytAlpha = 0xFF;
	m_fRotateDraw = 0.0f;

	m_nAnimationTime = 0;	// ���A�j���[�V�����C���f�b�N�X�ֈڂ鎞��
	m_nAnimationTimeCounter = 0;	// ���Ԃ̃J�E���^
	m_bAnimationLoop = FALSE;

	m_nAnimationCount = 0;	// �A�j���[�V������
	m_nAnimationIndex = 0;	// �A�j���[�V�����C���f�b�N�X

	m_nFade = 0;					// �t�F�[�h

	m_nFadeInOut = 0;
	m_nRotationTimeCounter = 0;
	m_nScallingTimeCounter = 0;
	m_nVectorTimeCounter = 0;
	m_nFadeTimeCounter = 0;
	m_nRotationEffectTime = 0;
	m_nFadeEffectTime = 0;			// �t�F�[�h���ʎ���
	m_nAnimationEffectTime = 0;	// �A�j���[�V�������ʎ���
	m_nScallingEffectTime = 0;		// �g��k�����ʎ���
	m_nVectorEffectTime = 0;			// �ړ����ʎ���
}

CStageEffect::~CStageEffect()
{
	p_pTexture = NULL;	
}

#ifdef HOGE
BOOL CStageEffect::Create(int nEffectNo, LPDIRECT3DTEXTURE9 pTexture, RECT* rcTexture,D3DXVECTOR3* pos, int nAge, BOOL force)
#else
BOOL CStageEffect::Create(int nEffectNo, LPDIRECT3DTEXTURE9 pTexture, RECT* rcTexture,D3DXVECTOR3* pos, int nAge)
#endif
{
	m_nEffectNo = nEffectNo;
	p_pTexture = pTexture;
	CopyRect(&m_rcTexture, rcTexture);				// �`��͈�
	CopyRect(&m_rcDraw, rcTexture);
	m_vecPosition = *pos;											// �ʒu	
	m_vecCenter = D3DXVECTOR3( (float)(rcTexture->right-rcTexture->left)/2.0f, (float)(rcTexture->bottom-rcTexture->top)/2.0f, 0.0f);
	m_nAge = nAge;
	m_nAgeCounter = 0;
#ifdef HOGE
	m_bForce = force;
#endif
	return TRUE;
}
void CStageEffect::SetEffectTexture(RECT* rcTexture)
{
	CopyRect(&m_rcTexture, rcTexture);				// �`��͈�
	CopyRect(&m_rcDraw, rcTexture);
	// �A�j���[�V����
	if (m_nAnimationCount)
	{
		m_rcDraw.left = m_rcTexture.left+(m_rcTexture.right-m_rcTexture.left)*m_nAnimationIndex;
		m_rcDraw.right = m_rcDraw.left+(m_rcTexture.right-m_rcTexture.left);
//		m_rcDraw.top = m_rcTexture.top+(m_rcTexture.bottom-m_rcTexture.top);
//		m_rcDraw.bottom = m_rcDraw.top+(m_rcTexture.bottom-m_rcTexture.top);
	}
}

void CStageEffect::Render(LPD3DXSPRITE pSprite, D3DXMATRIX* matWorld)
{
	if (!m_nAgeCounter)	return;

	pSprite->SetTransform(matWorld);
#ifdef HOGE
	if (m_bForce){
		if (!p_pTexture)
			return;
		do
		{
			D3DXMATRIX mat, matCenter1,matCenter2, matRot, matScale;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixRotationZ(&matRot, D3DXToRadian(m_fRotateDraw));
			D3DXMatrixScaling(&matScale, m_vecScallingDraw.x,m_vecScallingDraw.y,m_vecScallingDraw.z);
			D3DXMatrixTranslation(&matCenter1, -m_vecPosition.x,-m_vecPosition.y, 0.0f);
			D3DXMatrixTranslation(&matCenter2, m_vecPosition.x, m_vecPosition.y, 0.0f);
			mat = matCenter1 *matScale *matRot*matCenter2* (*matWorld);

			pSprite->SetTransform(&mat);
				pSprite->Draw(p_pTexture, &m_rcDraw, &m_vecCenter, &m_vecPosition, m_clrDraw);
		} while(Frame());
		return;
	}
#endif
	D3DXMATRIX mat, matCenter1,matCenter2, matRot, matScale;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(m_fRotateDraw));
	D3DXMatrixScaling(&matScale, m_vecScallingDraw.x,m_vecScallingDraw.y,m_vecScallingDraw.z);
	D3DXMatrixTranslation(&matCenter1, -m_vecPosition.x,-m_vecPosition.y, 0.0f);
	D3DXMatrixTranslation(&matCenter2, m_vecPosition.x, m_vecPosition.y, 0.0f);
	mat = matCenter1 *matScale *matRot*matCenter2* (*matWorld);

	pSprite->SetTransform(&mat);

	if (p_pTexture)
		pSprite->Draw(p_pTexture, &m_rcDraw, &m_vecCenter, &m_vecPosition, m_clrDraw);
}

BOOL CStageEffect::Frame()
{
	if (m_nAge)
	{
		m_nAgeCounter++;
		if (m_nAgeCounter > m_nAge)
			return FALSE;
	}

	// �A�j���[�V����
	if (m_nAnimationCount)
	{
		m_nAnimationTimeCounter++;
		if (m_nAnimationEffectTime && m_nAnimationTimeCounter >= m_nAnimationEffectTime)
		{
			m_nAnimationCount = 0;
		}
		else
		{
			if (m_nAnimationTimeCounter >= m_nAnimationTime)
			{
				m_nAnimationTimeCounter = 0;
				m_nAnimationIndex++;

				if (m_nAnimationIndex >= m_nAnimationCount)
				{
					if (m_bAnimationLoop)
						m_nAnimationIndex = 0;
					else
						m_nAnimationIndex = m_nAnimationCount-1;
				}
				m_rcDraw.left = m_rcTexture.left+(m_rcTexture.right-m_rcTexture.left)*m_nAnimationIndex;
				m_rcDraw.right = m_rcDraw.left+(m_rcTexture.right-m_rcTexture.left);
			}
		}
	}
	
	// �ړ��l
	if (m_nVectorEffectTime && ++m_nVectorTimeCounter >= m_nVectorEffectTime)
	{
		m_vecAddVec = D3DXVECTOR3(0,0,0);
		m_vecVector = D3DXVECTOR3(0,0,0);
	}
	else
	{
		m_vecVector += m_vecAddVec;
		m_vecPosition += m_vecVector;
	}

	// �����x
	m_clrDraw = (m_clrDraw&0x00FFFFFF)|(m_bytAlpha<<24);
	// �t�F�[�h
	if (!m_nFadeEffectTime || ++m_nFadeTimeCounter <  m_nFadeEffectTime)
	{
		int nAlpha = m_bytAlpha+m_nFade*m_nAgeCounter;
		m_clrDraw = (m_clrDraw&0x00FFFFFF)|(max(min(nAlpha,0xFF),0)<<24);
	}
	// �t�F�[�h�C���A�E�g
	if (m_nFadeInOut)
	{
		// �t�F�[�h�C��
		if (m_nFadeInOut >= m_nAgeCounter)
		{
			DWORD dwAlpha = (m_clrDraw&0xFF000000)>>24;
			dwAlpha = (DWORD)((float)dwAlpha*((float)m_nAgeCounter) / ((float)m_nFadeInOut));
			m_clrDraw = (m_clrDraw&0x00FFFFFF)|(max(min( dwAlpha, 0xFF),0)<<24);
		}
		else if ( m_nFadeInOut >= m_nAge-m_nAgeCounter)
		{
			int nAlpha = (m_clrDraw&0xFF000000)>>24;
			nAlpha = (DWORD)((float)nAlpha*((float)(m_nAge-m_nAgeCounter)) / ((float)m_nFadeInOut));
			m_clrDraw = (m_clrDraw&0x00FFFFFF)|(max(min( nAlpha, 0xFF),0)<<24);
		}
	}

	// ��]
	if (m_nRotationEffectTime && ++m_nRotationTimeCounter >= m_nRotationEffectTime)
		m_fRotation = 0.0f;
	m_fRotateDraw = safeFloatAdd(m_fRotateDraw, m_fRotation, m_fRotateDraw);
	// �g��k��
	if (m_nScallingEffectTime && ++m_nScallingTimeCounter >= m_nScallingEffectTime)
		m_vecScalling = D3DXVECTOR3(0,0,0);
	m_vecScallingDraw.x = safeFloatAdd(m_vecScallingDraw.x, m_vecScalling.x, m_vecScallingDraw.x);
	m_vecScallingDraw.y = safeFloatAdd(m_vecScallingDraw.y, m_vecScalling.y, m_vecScallingDraw.y);
	m_vecScallingDraw.z = safeFloatAdd(m_vecScallingDraw.z, m_vecScalling.z, m_vecScallingDraw.z);

	return TRUE;
}

