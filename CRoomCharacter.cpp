// CRoomCharacter.cpp
#include "CRoomCharacter.h"
#include "ext.h"

CRoomCharacter::CRoomCharacter()
{
	p_pTexture = NULL;
	m_pControlWord = NULL;
	p_pSession = NULL;
	m_nAnimationIndex = 0;
	m_nAnimationCount = 0;
	m_nAnimationTimeCounter = 0;
	m_nAnimationTime = 0;
	SetRectEmpty(&m_recDraw);
	m_nTextureWidth = 0;
	m_nTextureHeight = 0;
	m_nWordViewTimeCounter = 0;
	m_nWordViewTime = 0;
	m_bCreated = FALSE;
	m_pControlName = NULL;
	m_nControlNameWidth = 0;
	m_nControlNameHeight = 0;
	m_nCharaWidth = 0;
	m_nCharaHeight = 0;
	D3DXMatrixIdentity(&m_matScale);
	m_bVisible = false;
	m_nControlNameAdjustX = 0;
	m_nControlNameAdjustY = 0;
	m_vecDrawCharaCenter = D3DXVECTOR3(0,0,0);
}

// ����
// pDev			: D3D�f�o�C�X
// pTexture	: �e�N�X�`��
// pStatic		: �X�^�e�B�b�N
// pSess		: �I�u�W�F�N�g���
// nSpriteWidth,nSpriteHeight : �e�N�X�`���T�C�Y
// nCharaWidth,nCharaheight	: �L�����N�^�T�C�Y
// nAnimationCount : ��Ұ��ݐ�
BOOL CRoomCharacter::Create(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex,  CDXUTDialog* pDialog, type_session* pSess, int nSpriteWidth, int nSpriteHeight, int nAnimationCount, int nCharaWidth, int nCharaHeight)
{
	//�쐬�ς݊m�F
	if (!m_bCreated)
	{
		if (!CreateControls(pTexture, nGUIResourceIndex, pDialog, pSess, nSpriteWidth, nSpriteHeight, nAnimationCount, nCharaWidth, nCharaHeight))
			return FALSE;
	}
	else
	{
		p_pDialog = pDialog;
		p_pTexture = pTexture;
		p_pSession = pSess;
	}

	RECT rcTexture;
	SetRect(&rcTexture, 120, 0, 220, 24);
	m_pControlWord->GetElement(0)->SetTexture( nGUIResourceIndex, &rcTexture );	// �ǉ������Ǘ��e�N�X�`���ԍ����w��
	m_pControlName->SetLocation(pSess->lx, pSess->ly);

	WCHAR pName[MAX_USER_NAME+1];
	common::session::GetSessionName(pSess, pName);
	m_pControlName->SetText(pName);

	UpdateTexture(pTexture, nSpriteWidth, nSpriteHeight, nAnimationCount, nCharaWidth, nCharaHeight);
	
	if (p_pSession->obj_state & OBJ_STATE_ROOM)
		Show();
	else
		Hide();

	return TRUE;
}

BOOL CRoomCharacter::Create(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex,  CDXUTDialog* pDialog, type_session* pSess, int nSpriteWidth, int nSpriteHeight, int nCharaLeft, int nCharaTop, int nCharaWidth, int nCharaHeight)
{
	//�쐬�ς݊m�F
	if (!m_bCreated)
	{
		if (!CreateControls(pTexture, nGUIResourceIndex, pDialog, pSess, nSpriteWidth, nSpriteHeight, 0, nCharaWidth, nCharaHeight))
			return FALSE;
	}
	else
	{
		p_pDialog = pDialog;
		p_pTexture = pTexture;
		p_pSession = pSess;
	}

	RECT rcTexture;
	SetRect(&rcTexture, 120, 0, 220, 24);
	m_pControlWord->GetElement(0)->SetTexture( nGUIResourceIndex, &rcTexture );	// �ǉ������Ǘ��e�N�X�`���ԍ����w��
	m_pControlName->SetLocation(pSess->lx, pSess->ly);

	WCHAR pName[MAX_USER_NAME+1];
	common::session::GetSessionName(pSess, pName);
	m_pControlName->SetText(pName);

	UpdateTexture(pTexture, nSpriteWidth, nSpriteHeight, nCharaLeft, nCharaTop, nCharaWidth, nCharaHeight);
	if (p_pSession->obj_state & OBJ_STATE_ROOM)
		Show();
	else
		Hide();

	return TRUE;
}


BOOL CRoomCharacter::CreateControls(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex,  CDXUTDialog* pDialog, type_session* pSess, int nSpriteWidth, int nSpriteHeight, int nAnimationCount, int nCharaWidth, int nCharaHeight)
{
	p_pDialog = pDialog;
	p_pTexture = pTexture;
	if ((m_pControlWord = pDialog->GetButton(pSess->sess_index+IDC_ROOM_STATIC_WORD_BASE)) == NULL)
	{
		if (FAILED(pDialog->AddButton(pSess->sess_index+IDC_ROOM_STATIC_WORD_BASE, L"", pSess->lx, pSess->ly-nSpriteHeight, IDC_ROOM_STATIC_WORD_W, IDC_ROOM_STATIC_WORD_H,0 , false, &m_pControlWord)))
			return FALSE;
	}

	m_pControlWord->SetStateMouserOverOffset(0,0);
	m_pControlWord->SetStatePressedOverOffset(0,0);
	m_pControlWord->GetElement(1)->SetTexture( 0, 0, 0);

	m_pControlWord->GetElement(0)->SetFont(2, 0xFFFFFFFF, DT_LEFT|DT_VCENTER|DT_WORDBREAK|DT_NOCLIP);
	m_pControlWord->GetElement(1)->SetFont(2, 0xFFFFFFFF, DT_LEFT|DT_VCENTER|DT_WORDBREAK|DT_NOCLIP);
	m_pControlWord->SetTextColor( 0xFFFFFFFF ); // Change color to green
	m_pControlWord->SetVisible(false);

	if ((m_pControlName = pDialog->GetStatic(pSess->sess_index+IDC_ROOM_STATIC_NAME_BASE)) == NULL)
	{
		if (FAILED(pDialog->AddStatic(pSess->sess_index+IDC_ROOM_STATIC_NAME_BASE, L"", pSess->lx, pSess->ly-nSpriteHeight, IDC_ROOM_STATIC_WORD_W, IDC_ROOM_STATIC_WORD_H, false, &m_pControlName)))
			return FALSE;
	}
	m_pControlName->GetElement( 0 )->dwTextFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
	m_pControlName->SetTextColor(D3DCOLOR_ARGB( 255, 255, 255, 255 ));

	m_pControlName->SetVisible(true);

	p_pSession = pSess;

	m_nAnimationTime = CHARACTER_ANIMATION_TIME;

	m_nWordViewTimeCounter = 0;
	m_nWordViewTime = CHARACTER_WORD_VIEW_TIME;

	m_bCreated = TRUE;

	return TRUE;
}

// ��ԍX�V
void CRoomCharacter::UpdateState()
{
	// �}�X�^�[���
	CDXUTElement* pElement = m_pControlName->GetElement(0);
	if (p_pSession->master)
		pElement->FontColor.Init(COLOR_MASTER_NAME);
	else if (p_pSession->game_ready)
		pElement->FontColor.Init(COLOR_READY_NAME);
	else
		pElement->FontColor.Init(COLOR_MEMBER_NAME);
}

// �X�V
// pTexture	: �e�N�X�`��
// nSpriteWidth,nSpriteHeight : �\���T�C�Y
// nAnimationCount : ��Ұ��ݐ�
BOOL CRoomCharacter::UpdateTexture(LPDIRECT3DTEXTURE9 pTexture, int nSpriteWidth, int nSpriteHeight, int nAnimationCount, int nCharaWidth, int nCharaHeight)
{
	if (!m_bCreated)
	{
		DXTRACE_ERR(L"CRoomCharacter::Update _ m_bCreated = false", E_FAIL);
		return FALSE;
	}

	HRESULT hr;
	p_pTexture = pTexture;
//	RECT rcTexture;
	D3DSURFACE_DESC SurfaceDesc;

	LPDIRECT3DSURFACE9	d_psf;
	// �]����̃T�[�t�F�[�X
	V( pTexture->GetSurfaceLevel(0, &d_psf ) );
	// �摜���擾
	V(d_psf->GetDesc(&SurfaceDesc));
	// �摜�T�C�Y�ۑ�
	m_nTextureWidth = SurfaceDesc.Width;
	m_nTextureHeight = SurfaceDesc.Height;
	SafeRelease(d_psf);

	m_nCharaWidth = nCharaWidth;
	m_nCharaHeight = nCharaHeight;
	float fScaleX = (float) nCharaWidth / nSpriteWidth;
    float fScaleY = (float) nCharaHeight / nSpriteHeight;
	D3DXMatrixScaling( &m_matScale, fScaleX, fScaleY, 1.0f );

	m_recDraw.right = nSpriteWidth;
	m_recDraw.bottom = nSpriteHeight;
	m_nAnimationCount = nAnimationCount;

	RECT rcText;
	SetRect(&rcText, 0,0,100,24);
	DXUTFontNode* pFontNode = g_SysUI.GetFont(2);
	HDC hDC = pFontNode->pFont->GetDC();
	DrawText( hDC, m_pControlName->GetText(), p_pSession->name_len,  &rcText,  DT_INTERNAL|DT_EDITCONTROL|DT_CALCRECT|DT_CENTER|DT_TOP|DT_SINGLELINE);
	ReleaseDC(DXUTGetHWND(), hDC);
	m_nControlNameWidth = rcText.right;
	m_nControlNameHeight = rcText.bottom;

	m_pControlName->SetSize(m_nControlNameWidth, m_nControlNameHeight);

	// �\���e�N�X�`���T�C�Y�̔���
	int nSizeHalfW = (nSpriteWidth/2);
	int nSizeHalfH = (nSpriteHeight/2);
	m_vecDrawCharaCenter = D3DXVECTOR3((float)nSizeHalfW, (float)nSizeHalfH, 0);

	m_nControlNameAdjustX = (m_nControlNameWidth/2);
	m_nControlNameAdjustY = nCharaHeight/2;

	// �\���͈͌v�Z
	CalcDrawRect();
	// ��Ԃ��X�V
	UpdateState();
	return TRUE;
}

BOOL CRoomCharacter::UpdateTexture(LPDIRECT3DTEXTURE9 pTexture, int nSpriteWidth, int nSpriteHeight, int nLeft, int nTop, int nRight, int nBottom)
{
	if (!m_bCreated)
	{
		DXTRACE_ERR(L"CRoomCharacter::Update _ m_bCreated = false", E_FAIL);
		return FALSE;
	}

	HRESULT hr;
	p_pTexture = pTexture;
//	RECT rcTexture;
	D3DSURFACE_DESC SurfaceDesc;

	LPDIRECT3DSURFACE9	d_psf;
	// �]����̃T�[�t�F�[�X
	V( pTexture->GetSurfaceLevel(0, &d_psf ) );
	// �摜���擾
	V(d_psf->GetDesc(&SurfaceDesc));
	// �摜�T�C�Y�ۑ�
	m_nTextureWidth = SurfaceDesc.Width;
	m_nTextureHeight = SurfaceDesc.Height;
	SafeRelease(d_psf);

	m_nCharaWidth = nRight;
	m_nCharaHeight = nBottom;
	float fScaleX = (float) 1.0f;
    float fScaleY = (float) 1.0f;
	D3DXMatrixScaling( &m_matScale, fScaleX, fScaleY, 1.0f );

	m_recDraw.right = nSpriteWidth;
	m_recDraw.bottom = nSpriteHeight;
	m_nAnimationCount = 0;

	RECT rcText;
	SetRect(&rcText, 0,0,100,24);
	DXUTFontNode* pFontNode = g_SysUI.GetFont(2);
	HDC hDC = pFontNode->pFont->GetDC();
	DrawText( hDC, m_pControlName->GetText(), p_pSession->name_len,  &rcText,  DT_INTERNAL|DT_EDITCONTROL|DT_CALCRECT|DT_CENTER|DT_TOP|DT_SINGLELINE);
	ReleaseDC(DXUTGetHWND(), hDC);
	m_nControlNameWidth = rcText.right;
	m_nControlNameHeight = rcText.bottom;

	m_pControlName->SetSize(m_nControlNameWidth, m_nControlNameHeight);

	// �\���e�N�X�`���T�C�Y�̔���
	int nSizeHalfW = (nSpriteWidth/2);
	int nSizeHalfH = (nSpriteHeight/2);
	m_vecDrawCharaCenter = D3DXVECTOR3((float)nSizeHalfW, (float)nSizeHalfH, 0);

	m_nControlNameAdjustX = (m_nControlNameWidth/2);
	m_nControlNameAdjustY = m_nCharaHeight/2;
	// �\���͈͌v�Z
	SetRect(&m_recDraw, nLeft, nTop, nRight, nBottom);
	// ��Ԃ��X�V
	UpdateState();
	return TRUE;
}

void CRoomCharacter::Destroy()
{
	if (!m_bCreated)	return;
	m_bCreated = FALSE;

	CDXUTControl* pControl;
	pControl= g_SysUI.GetControl(p_pSession->sess_index+IDC_ROOM_STATIC_WORD_BASE);
	if (pControl)
		pControl->SetVisible(false);
	pControl = g_SysUI.GetControl(p_pSession->sess_index+IDC_ROOM_STATIC_NAME_BASE);
	if (pControl)
		pControl->SetVisible(false);

	m_pControlWord = NULL;
	p_pTexture = NULL;
	p_pSession = NULL;
	m_nAnimationIndex = 0;
	m_nAnimationCount = 0;
	m_nAnimationTimeCounter = 0;
	m_nAnimationTime = 0;
	SetRectEmpty(&m_recDraw);
	m_nTextureWidth = 0;
	m_nTextureHeight = 0;
	m_nWordViewTimeCounter = 0;
	m_nWordViewTime = 0;
}

void CRoomCharacter::Frame()
{
	if (!m_bCreated)
		return;

	// �ړ��l���ʒu�ɑ�������
	p_pSession->lx = max(0L, (int)min(WIN_WIDTH, p_pSession->lx+(p_pSession->vx*ROOM_MV_VEC_X)));
	p_pSession->ly = max(0L, (int)min(p_pSession->ly+(p_pSession->vy*ROOM_MV_VEC_Y), ROOM_CHARA_BASE_MAX_MOVE_H));

	// ����OK�̏�Ԃͱ�Ұ��݂��~�߂�
	if (!p_pSession->game_ready)
	{
		if (m_nAnimationCount)
		{
			// �J�E���g�A�b�v
			m_nAnimationTimeCounter++;
			if (m_nAnimationTimeCounter >= m_nAnimationTime)
			{
				m_nAnimationTimeCounter = 0;
				m_nAnimationIndex = (m_nAnimationIndex+1) % m_nAnimationCount;
				CalcDrawRect();
			}
		}
	}

	// �����\����
	if (m_pControlWord->GetVisible() && m_bVisible)
	{
		// �\���J�E���^��i�߁A�\���I�����Ԃ�������e�L�X�g��\���ɂ���
		m_nWordViewTimeCounter++;
		if (m_nWordViewTimeCounter >= m_nWordViewTime)
		{
			m_pControlWord->SetVisible(false);
			m_nWordViewTimeCounter = 0;
		}		
	}
}

// �`��͈͍Čv�Z
void CRoomCharacter::CalcDrawRect()
{
	int nAnimationIndex = m_nAnimationIndex;
	if (p_pSession->dir == USER_DIRECTION_LEFT)
		nAnimationIndex += m_nAnimationCount;
	// �X�v���C�g���� * index / �摜����
	int nTemp = (m_recDraw.right * nAnimationIndex) / m_nTextureWidth;
	m_recDraw.left = nAnimationIndex % (m_nTextureWidth/m_recDraw.right) * m_recDraw.right;
	m_recDraw.top = nTemp * m_recDraw.bottom;
}

void CRoomCharacter::Render(LPDIRECT3DDEVICE9 pDev,  float fElapsedTime)
{
	if (!m_bCreated || !m_bVisible)
		return;

	// �����ɋ���L�����̂ݕ`��
	if ( !(p_pSession->obj_state & OBJ_STATE_ROOM) )
		return;

	RECT recDraw;
	SetRect(&recDraw, m_recDraw.left, m_recDraw.top, m_recDraw.left+m_recDraw.right, m_recDraw.top+m_recDraw.bottom);

	D3DXVECTOR3 vecPos = D3DXVECTOR3(
		(float)p_pSession->lx+ROOM_CHARA_BASE_OFFSET_X,
		(float)p_pSession->ly+ROOM_CHARA_BASE_OFFSET_Y+32-m_nControlNameAdjustY, (float)((ROOM_CHARA_BASE_MAX_MOVE_H-p_pSession->ly))/(float)ROOM_CHARA_BASE_MAX_MOVE_H);
	
	D3DXMATRIX matWorld, matTrans;
	g_pSprite->GetTransform(&matWorld);
	D3DXMatrixTranslation(&matTrans, vecPos.x, vecPos.y, vecPos.z);
	
	g_pSprite->SetTransform( &(m_matScale * matTrans * matWorld ) );
//	g_pSprite->Draw(p_pTexture, &recDraw, &m_vecDrawCharaCenter, &vecPos, 0xFFFFFFFF);
	g_pSprite->Draw(p_pTexture, &recDraw, &m_vecDrawCharaCenter, &D3DXVECTOR3(0,0,0), 0xFFFFFFFF);
	
	g_pSprite->SetTransform( &matWorld );

	if (m_pControlWord->GetVisible() && m_bVisible)
	{
		m_pControlWord->SetLocation((int)(vecPos.x- (m_pControlWord->m_width/2)), (int)(vecPos.y-m_recDraw.bottom-m_pControlWord->m_height));
//		m_pControlWord->Render(g_pSprite, pDev, p_pDialog, fElapsedTime, &mat);
		m_pControlWord->Render(pDev, fElapsedTime);
	}
	m_pControlName->SetLocation( ((int)vecPos.x - m_nControlNameAdjustX), ((int)vecPos.y + m_nControlNameAdjustY));
	m_pControlName->Render(pDev, fElapsedTime);
}

// ����
void CRoomCharacter::Say(WCHAR *message)
{
	RECT rcText;
	SetRect(&rcText, 0,0,200,24);
	const WCHAR* pText = m_pControlWord->GetText();
	int nTextLen = _tcslen(message);
	DXUTFontNode* pFontNode = g_SysUI.GetFont(2);
	HDC hDC = pFontNode->pFont->GetDC();
	DrawText( hDC, message, nTextLen,  &rcText,  DT_INTERNAL|DT_EDITCONTROL|DT_CALCRECT|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
	
	ReleaseDC(DXUTGetHWND(), hDC);
	rcText.right += 8;
	
	m_pControlWord->SetSize(rcText.right, rcText.bottom);
	m_pControlWord->SetLocation(
		(int)(p_pSession->lx+ROOM_CHARA_BASE_OFFSET_X- (m_pControlWord->m_width/2)),
		(int)((p_pSession->ly+ROOM_CHARA_BASE_OFFSET_Y)-m_recDraw.bottom-m_pControlWord->m_height));

	m_pControlWord->SetText(message);
	m_pControlWord->SetVisible(true);
	m_nWordViewTimeCounter = 0;
}

void CRoomCharacter::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
	if (m_bCreated)
		m_pControlName->SetVisible( bVisible );
}


void CRoomCharacter::Hide()
{
	m_bVisible = false;
	if (m_bCreated)
	{
		m_pControlName->SetVisible( false );
		m_pControlWord->SetVisible( false );
	}
}

void CRoomCharacter::Show()
{
	m_bVisible = true;
	if (m_bCreated)
		m_pControlName->SetVisible( true );
}

void CRoomCharacter::OnLost()
{
	// �e�N�X�`����NULL�ɂ���
	p_pTexture = NULL;
}

void CRoomCharacter::OnReset()
{
	// �e�N�X�`���̍Đݒ�
	p_pTexture = ((TCHARA_SCR_INFO*)p_pSession->scrinfo)->pTexture;
}
