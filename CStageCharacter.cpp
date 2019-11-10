#include "CStageCharacter.h"
#include "ext.h"

CStageCharacter::CStageCharacter()
{
	m_pControlName = NULL;					// ���O?���p�R���g��?��	
	m_pHPBar = NULL;							// HP�o?
	m_pMyTurn = NULL;
	p_pDialog = NULL;
	m_pStatusInfo = NULL;
	m_nStageWidth = 0;
	m_nStageHeight = 0;
	Destroy();
}

// �j��
void CStageCharacter::Destroy()
{
	p_pSession = NULL;							// �I�u�W�F�N�g���	
	m_nAnimationIndex = 0;					// ��Ұ��?`��͈̓C���f�b�N�X
	m_nAnimationCount = 0;					// ��Ұ��?`��͈͐�

	m_nAnimationTimeCounter = 0;			// ��Ұ��ݲ��ޯ���̐��l�𑝂₷����
	m_nAnimationTime = 0;					// ��Ұ��ݲ��ޯ���̐��l�𑝂₷����
	
	m_nTextureWidth = CHARA_TEXTURE_WIDTH;						// �e�N�X?������
	m_nTextureHeight = CHARA_TEXTURE_HEIGHT;						//	�e�N�X?���c��
	p_pTexture = NULL;							// �e�N�X?��
	m_nControlNameWidth = 0;				// 
	m_nControlNameHeight = 0;

	m_bCreated = FALSE;						// �����ς�
	SetRectEmpty(&m_recDraw);				// ?��͈�	
	m_bVisible = FALSE;							// ?�����
	m_nBodyRange = 0;							// �����蔻��p���a
	m_vecDrawCharaPos = D3DXVECTOR3(0,0,0);
	m_vecDrawCharaCenter = D3DXVECTOR3(0,0,0);

	SafeDelete(m_pControlName);
	SafeDelete(m_pHPBar);
	SafeDelete(m_pMyTurn);
	SafeDelete(m_pStatusInfo);

	m_nCharaWidth = 0;
	m_nCharaHeight = 0;
	m_fAdjustPosY = 0.0f;
	D3DXMatrixIdentity(&m_matScale);

	D3DXMatrixIdentity(&m_matDrawCharaRadian);
}

// ����
// pDev			: D3D�f�o�C�X
// pTexture	: �e�N�X?��
// nGUIResourceIndex : ��?��GUI��??�X�ԍ�
// pStatic		: �X?�e�B�b�N(�L�������p)
// pSess		: �I�u�W�F�N�g���
HRESULT CStageCharacter::Create(std::map < int, TCHARA_SCR_INFO > *mapScrInfo, int nGUIResourceIndex, CDXUTDialog* pDialog, type_session* pSess, int nStageWidth, int nStageHeight)
{
	LuaFuncParam	tLuaParams;
	LuaFuncParam	tLuaResults;
//	HRESULT hr;

	p_pDialog = pDialog;
	p_pSession = pSess;
	m_nGUIResourceIndex = nGUIResourceIndex;
	m_nBodyRange = CHARA_BODY_RANGE;
	
	m_nStageWidth = nStageWidth;
	m_nStageHeight = nStageHeight;
	// �X�N���v�gID����
	int nObjID = pSess->chara_type;
	std::map < int, TCHARA_SCR_INFO >::iterator itfind = mapScrInfo->find(nObjID); 
	if (itfind == mapScrInfo->end())
	{
		AddMessageLog(L"�X�N���v�g��?�h�G��?\nCStageCharacter::Create: m_MapScrInfo.find(nObjID) not find");
//		DXTRACE_MSG(L"�X�N���v�g��?�h�G��?\nCStageCharacter::Create: m_MapScrInfo.find(nObjID) not find");
		return FALSE;
	}
	m_pScrInfo = &((*itfind).second);
	p_pTexture = m_pScrInfo->pTexture;

	p_pSession->HP_c = m_pScrInfo->max_hp;
	p_pSession->HP_m = m_pScrInfo->max_hp;
	p_pSession->entity = 1;

	D3DSURFACE_DESC SurfaceDesc;
	LPDIRECT3DSURFACE9	d_psf;
	// ?����̃T?�t�F?�X
	if (p_pTexture->GetSurfaceLevel(0, &d_psf ) == D3D_OK)
	{
		// �摜���擾
		d_psf->GetDesc(&SurfaceDesc);
		// �摜�T�C�Y�ۑ�
		m_nTextureWidth = SurfaceDesc.Width;
		m_nTextureHeight = SurfaceDesc.Height;
		SafeRelease(d_psf);
	}

//	m_pScrInfo->tex_chr_num

	m_nAnimationTime = CHARACTER_ANIMATION_TIME;
	m_nAnimationTimeCounter = 0;

	m_nCharaWidth = m_pScrInfo->draw_w;
	m_nCharaHeight = m_pScrInfo->draw_h;

	SafeDelete(m_pControlName);
	SafeDelete(m_pHPBar);
	SafeDelete(m_pMyTurn);

	m_pControlName = new CDXUTStatic();
	m_pControlName->m_bIsDefault = false;

	////////////////////////////////////////////////////////
	m_pHPBar = new CDXUTMeter();
	m_pHPBar->m_bIsDefault = false;
	m_pMyTurn = new CDXUTSprite();
	m_pMyTurn->m_bIsDefault = false;
	SafeDelete(m_pStatusInfo);

	CGrowableArray< DXUTElementHolder* >* pDefaultElement = pDialog->GetDefaultElement();
    // Look for a default Element entries
	pDialog->InitControl(m_pControlName);
	pDialog->InitControl(m_pHPBar);
	pDialog->InitControl(m_pMyTurn);

	// Set Element //
	// Name
	m_pControlName->SetLocation(pSess->ax, pSess->ay-m_nTextureHeight);
	m_pControlName->SetSize(IDC_ROOM_STATIC_WORD_W, IDC_ROOM_STATIC_WORD_H);

	m_pControlName->SetLocation(pSess->ax, pSess->ay);

	m_pControlName->SetTextColor(D3DCOLOR_ARGB( 255, 255, 255, 255 ));
	WCHAR pName[MAX_USER_NAME+1];
	int nNameLen = pSess->name_len;
	common::session::GetSessionName(pSess, pName);
	m_pControlName->SetText(pName);

	m_pControlName->GetElement( 0 )->dwTextFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
	//m_pControlName->GetElement(0)->SetFont(2, 0xFFFFFFFF, DT_INTERNAL|DT_EDITCONTROL|DT_CALCRECT|DT_CENTER|DT_TOP|DT_SINGLELINE);
	RECT rcText;
	SetRect(&rcText, 0,0,100,24);
	DXUTFontNode* pFontNode = g_SysUI.GetFont(2);
	HDC hDC = pFontNode->pFont->GetDC();
	DrawText( hDC, pName, nNameLen,  &rcText,  DT_INTERNAL|DT_EDITCONTROL|DT_CALCRECT|DT_CENTER|DT_TOP|DT_SINGLELINE);
	ReleaseDC(DXUTGetHWND(), hDC);
	m_nControlNameWidth = rcText.right;
	m_nControlNameHeight = rcText.bottom;
	m_pControlName->SetSize(m_nControlNameWidth, m_nControlNameHeight);
	m_pControlName->SetVisible(true);

	// HP Bar
	m_pHPBar->SetSize(STAGE_CHARA_HPBAR_WIDTH,STAGE_CHARA_HPBAR_HEIGHT);
	m_pHPBar->SetMaxValue(p_pSession->HP_m);
	m_pHPBar->SetLocation(0,0);
	m_pHPBar->SetValue(p_pSession->HP_c);

	m_pHPBar->GetElement(0)->TextureColor.Init( 0xFFFFFFFF,0xFFFFFFFF);
	m_pHPBar->GetElement(1)->TextureColor.Init(0xFF00FF00, 0xFF00FF00);
	m_pHPBar->GetElement(2)->TextureColor.Init(0,0,0);
	RECT rcTexture;
	SetRect( &rcTexture, STAGE_CHARA_HPBAR_RECT);	// ?��͈�
	m_pHPBar->GetElement(0)->SetTexture( nGUIResourceIndex, &rcTexture );	// �ǉ������Ǘ��e�N�X?���ԍ����w��
	m_pHPBar->GetElement(1)->SetTexture( nGUIResourceIndex, &rcTexture,0xFF00FF00);	// �ǉ������Ǘ��e�N�X?���ԍ����w��
	m_pHPBar->SetMargin(1,1);

	// MyTurn
	m_pMyTurn->SetSize(MAIN_STATIC_MY_TURN_W, MAIN_STATIC_MY_TURN_H);
	UpdateMyTurnTexture();
	m_pMyTurn->SetVisible(true);

	m_bVisibleInfo = true;

	// ?���͈͌v�Z //
	CopyRect(&m_recDraw, &m_pScrInfo->rec_tex_chr);
	// ?���e�N�X?���T�C�Y�̔���
	float fSizeHalfW = ((float)m_recDraw.right/2.0f);
	float fSizeHalfH = ((float)m_recDraw.bottom/2.0f);
	m_vecDrawCharaCenter = D3DXVECTOR3(fSizeHalfW, fSizeHalfH, 0.0f);

	float fScaleX = (float)m_nCharaWidth / (float)m_recDraw.right;
	float fScaleY = (float)m_nCharaHeight / (float)m_recDraw.bottom;
	D3DXMatrixScaling(&m_matScale, fScaleX, fScaleY, 1.0f);
	m_fAdjustPosY = m_vecDrawCharaCenter.y*(float)m_nCharaHeight/(float)m_recDraw.bottom /2.0f;

	CalcDrawRect();	/// 20100928
	CalcDrawPos();
///	CalcDrawPos();
///	CalcDrawRect();
	UpdateState();

	m_bVisible = TRUE;
	m_bCreated = TRUE;	
	return TRUE;
}

void CStageCharacter::Frame()
{
	if (!m_bCreated)	return;

	// �ړ��l���ʒu�ɑ�������
	if (p_pSession->vy != 0	|| p_pSession->vx != 0)
		CalcDrawPos();

	if (m_pStatusInfo)
	{
		m_nStatuInfoTime--;
		if (!m_nStatuInfoTime)
			SafeDelete(m_pStatusInfo);
	}
	/*
	// �������Ă���L�����̂�
	if (!(p_pSession->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG))
	{
		// �J�E���g�A�b�v
		m_nAnimationTimeCounter++;
		if (m_nAnimationTimeCounter >= m_nAnimationTime)
		{
			m_nAnimationTimeCounter = 0;
			if (m_bDrawTrigger)
				m_nAnimationIndex = (m_nAnimationIndex+1)>=m_nAnimationCount?m_nAnimationIndex:m_nAnimationIndex+1;
			else
				m_nAnimationIndex = (m_nAnimationIndex+1) % m_nAnimationCount;
			CalcDrawRect();
		}
	}
	*/
}

void CStageCharacter::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;	
	if (!m_bCreated) return;
	if (m_pControlName->GetVisible() != bVisible)
		m_pControlName->SetVisible(bVisible);
	if (m_pMyTurn->GetVisible() != bVisible)
		m_pMyTurn->SetVisible(bVisible);
	if (bVisible)
	{
		CalcDrawRect();
		CalcDrawPos();
	}
}

void CStageCharacter::SetTexture(RECT* prcTexture)
{
	CopyRect(&m_recDraw, prcTexture);
}

// ?��͈͍Čv�Z
void CStageCharacter::CalcDrawRect()
{
	int nAnimationIndex = m_nAnimationIndex;
	if (p_pSession->dir == USER_DIRECTION_LEFT) {

		nAnimationIndex += m_nAnimationCount;
	}
/*
	if (m_bDrawTrigger)
	{
		// �X�v���C�g���� * index / �摜����
		int nTemp = (m_recDraw.right * nAnimationIndex) / m_nTextureWidth;
		m_recDraw.left = nAnimationIndex % (m_nTextureWidth/m_recDraw.right) * m_recDraw.right + ((TCHARA_SCR_INFO*)p_pSession->scrinfo)->rec_tex_trg.left;
		m_recDraw.top = nTemp * m_recDraw.bottom + ((TCHARA_SCR_INFO*)p_pSession->scrinfo)->rec_tex_trg.top;
	}
	else
	{
		// �X�v���C�g���� * index / �摜����
		int nTemp = (m_recDraw.right * nAnimationIndex) / m_nTextureWidth;
		m_recDraw.left = nAnimationIndex % (m_nTextureWidth/m_recDraw.right) * m_recDraw.right + ((TCHARA_SCR_INFO*)p_pSession->scrinfo)->rec_tex_chr.left;
		m_recDraw.top = nTemp * m_recDraw.bottom + ((TCHARA_SCR_INFO*)p_pSession->scrinfo)->rec_tex_chr.top;
	}
*/
}

// �ʒu�ύX�ɂ��?��ʒu�v�Z
void CStageCharacter::CalcDrawPos()
{
	m_vecDrawCharaPos = D3DXVECTOR3(
		(float)p_pSession->ax,
//		(float)p_pSession->ay-m_vecDrawCharaCenter.y + CHARA_BODY_RANGE+2, 0.0f);
		(float)p_pSession->ay - m_fAdjustPosY, 0.0f);
	
	m_pHPBar->SetLocation(min((m_nStageWidth-m_pHPBar->m_width), max(0,(int)(m_vecDrawCharaPos.x - (m_pHPBar->m_width/2)))), (int)(m_vecDrawCharaPos.y + (float)m_nCharaHeight/2.0f) + 2 );
//	m_pHPBar->SetLocation((int)(m_vecDrawCharaPos.x - (m_pHPBar->m_width/2)), (int)(m_vecDrawCharaPos.y + (m_vecDrawCharaCenter.y) + 2) );
	m_pControlName->SetLocation( min(m_nStageWidth-m_pControlName->m_width, max(0, (int)(m_vecDrawCharaPos.x - (m_nControlNameWidth/2)))), m_pHPBar->m_y+m_pHPBar->m_height);

	if (m_pMyTurn->GetVisible())
		m_pMyTurn->SetLocation((int)(m_vecDrawCharaPos.x - (m_pMyTurn->m_width/2)), max(MAIN_STATIC_MY_TURN_MIN_Y_POS,(int)(m_vecDrawCharaPos.y - (m_vecDrawCharaCenter.y) - m_pMyTurn->m_height)));

	int nAngle = p_pSession->angle;
	if (p_pSession->dir == USER_DIRECTION_RIGHT)
		nAngle = (p_pSession->angle+180)%360;

	if (m_pStatusInfo)
		m_pStatusInfo->SetLocation(m_pStatusInfo->m_x, m_pStatusInfo->m_y+MAIN_CHARA_STATUS_INFO_OFFSET_SPEED_Y );

	D3DXMATRIX matR, matT, matT2, matScale;
	// �X��D3DXMATRIX matR, matT, matT2;
	// �ϊ��s�񏉊���
//	::D3DXMatrixIdentity(&mat);
	// 1. ��x�X�v���C�g�̒��S?���E�B���h�E�̌�?(0,0)�ɂ��킹��
	::D3DXMatrixTranslation(&matT, -m_vecDrawCharaPos.x, -m_vecDrawCharaPos.y-CHARA_BODY_RANGE, 0.0f);
	// 2. �X�v���C�g����?������
	::D3DXMatrixRotationZ(&matR, D3DXToRadian(nAngle));
	// 3. 1�ł��炵���������ɖ߂�
	::D3DXMatrixTranslation(&matT2,m_vecDrawCharaPos.x, m_vecDrawCharaPos.y+CHARA_BODY_RANGE, 0.0f);

	// �s��̍���
	m_matDrawCharaRadian = matT * matR * m_matScale * matT2;
}

// ��ԍX�V
void CStageCharacter::UpdateState()
{
	m_pHPBar->SetValue(p_pSession->HP_c);
	if (p_pSession->HP_c)
	{
		if ( ((float)p_pSession->HP_c / (float)p_pSession->HP_m) <= 0.2f)
			m_pHPBar->GetElement(1)->TextureColor.States[ DXUT_STATE_NORMAL ] = 0xFFFF0000;
		else
			m_pHPBar->GetElement(1)->TextureColor.States[ DXUT_STATE_NORMAL ] = 0xFF00FF00;
	}
}

// ??����ԍX�V
void CStageCharacter::SetMyTurn(bool bTurn)
{
	if (m_pMyTurn)
	{
		UpdateMyTurnTexture();
		Update();
	}
}

void CStageCharacter::UpdateMyTurnTexture()
{
	int nImgX = 0;
	int nImgY = 0;
	switch (p_pSession->obj_state)
	{
	case OBJ_STATE_MAIN_ACTIVE:
		nImgX = MAIN_STATIC_MY_TURN_IMG_ACT_X+(MAIN_STATIC_MY_TURN_W*(p_pSession->team_no));
		nImgY = MAIN_STATIC_MY_TURN_IMG_ACT_Y;
		break;
	case OBJ_STATE_MAIN_DROP:
	case OBJ_STATE_MAIN_DEAD:
		nImgX = MAIN_STATIC_MY_TURN_IMG_DEAD_X+(MAIN_STATIC_MY_TURN_W*(p_pSession->team_no));
		nImgY = MAIN_STATIC_MY_TURN_IMG_DEAD_Y;
		break;
	case OBJ_STATE_MAIN_WAIT:
	default:
		nImgX = MAIN_STATIC_MY_TURN_IMG_WAIT_X+(MAIN_STATIC_MY_TURN_W*(p_pSession->team_no));
		nImgY = MAIN_STATIC_MY_TURN_IMG_WAIT_Y;
		break;
	}
	RECT rcTexture = {nImgX, nImgY, nImgX+MAIN_STATIC_MY_TURN_W, nImgY+MAIN_STATIC_MY_TURN_H};
	m_pMyTurn->GetElement(0)->SetTexture(m_nGUIResourceIndex, &rcTexture, 0xFFFFFFFF);
}

// ?��
// pDev	: D3D�f�o�C�X
// fElapsedTime : �o�ߎ���
void CStageCharacter::Render(LPDIRECT3DDEVICE9 pDev, float fElapsedTime, D3DXMATRIX* matChara, D3DXMATRIX* matStage, D3DCOLOR color)
{
	if (!m_bCreated || !m_bVisible)	return;
	//> 20110201 ��?����?�񂾃L�������A��?����߂����Ƃ�������?���ɂȂ��Ă���
	// ����?��?�����Ȃ�
//	if (p_pSession && p_pSession->obj_state & OBJ_STATE_MAIN_DROP_FLG)	return;
	//> 20110201 ��?����?�񂾃L�������A��?����߂����Ƃ�������?���ɂȂ��Ă���

	// ?��C�x���g�ʒm
	LuaFuncParam luaResults, luaParams;
	// script,chr_no,state,angle,vx,vy,frame
	luaParams.Number(p_pSession->scrinfo->scr_index).Number(p_pSession->obj_no).Number(((DWORD)p_pSession->obj_state&OBJ_STATE_MAIN_MASK)).Number(p_pSession->angle).Number(p_pSession->vx).Number(p_pSession->vy).Number(p_pSession->frame_count);
	if (!common::scr::CallLuaFunc(g_pLuah, "onDraw_Chara", &luaResults, 0, &luaParams, g_pCriticalSection))
		return;

	//> 20110201 ��?����?�񂾃L�������A��?����߂����Ƃ�������?���ɂȂ��Ă���
	// ����?��?�����Ȃ�
	if (p_pSession && p_pSession->obj_state & OBJ_STATE_MAIN_DROP_FLG)	return;
	//> 20110201 ��?����?�񂾃L�������A��?����߂����Ƃ�������?���ɂȂ��Ă���

	D3DXMATRIX mat;	
	D3DXMatrixIdentity(&mat);	
//	RECT recDraw;
//	SetRect(&recDraw, m_recDraw.left+1, m_recDraw.top+1, m_recDraw.left+m_recDraw.right-1, m_recDraw.top+m_recDraw.bottom-1);
	
	mat = m_matDrawCharaRadian * (*matChara);
	g_pSprite->SetTransform(&mat);
	
	if (p_pTexture)
		g_pSprite->Draw(p_pTexture, &m_recDraw, &m_vecDrawCharaCenter, &m_vecDrawCharaPos, color);
	
	if (m_bVisibleInfo)
	{
		D3DXMatrixIdentity(&mat);	
		g_pSprite->SetTransform(&mat);
		// ���O
		m_pControlName->Render(g_pSprite, pDev, p_pDialog, fElapsedTime, matChara);
		// HP
		m_pHPBar->Render(g_pSprite, pDev, p_pDialog, fElapsedTime, matChara);
		// ����?
		m_pMyTurn->Render(g_pSprite, pDev, p_pDialog, fElapsedTime, matChara);
		if (m_pStatusInfo)
			m_pStatusInfo->Render(g_pSprite, pDev, p_pDialog, fElapsedTime, matChara);
	}

	g_pSprite->SetTransform(matStage);
}

// ?��
void CStageCharacter::SetDead()
{
	// ?�񂾂�??�������Ȃ��̂�?������Ă���
	if (p_pSession)
	{
		//> 20101205?�񂾂�HP0�ɂ���
		p_pSession->HP_c = 0;
		UpdateState();
		//< 20101205?�񂾂�HP0�ɂ���
		UpdateMyTurnTexture();
	}
}

void CStageCharacter::OnLost()
{
	// �e�N�X?����NULL�ɂ���
	p_pTexture = NULL;
}

void CStageCharacter::OnReset()
{
	// �e�N�X?���̍Đݒ�
	m_pScrInfo = (TCHARA_SCR_INFO*)p_pSession->scrinfo;
	p_pTexture = m_pScrInfo->pTexture;
}

void CStageCharacter::ChangeHP(int value)
{
	CDXUTStatic* pStatic = new CDXUTStatic();
	pStatic->m_bIsDefault = false;
	p_pDialog->InitControl(pStatic);
	if (value > 0)
		pStatic->GetElement(0)->SetFont(MAIN_CHARA_STATUS_INFO_FONT, MAIN_CHARA_STATUS_INFO_COLOR_PLUS, DT_CENTER|DT_VCENTER);
	else
		pStatic->GetElement(0)->SetFont(MAIN_CHARA_STATUS_INFO_FONT, MAIN_CHARA_STATUS_INFO_COLOR_MINUS, DT_CENTER|DT_VCENTER);

	WCHAR pValue[MAIN_CHARA_STATUS_INFO_MAXTEXT+1];
	SafePrintf(pValue, MAIN_CHARA_STATUS_INFO_MAXTEXT, L"%+d", value);
	pStatic->SetDrawShadow(false);
	pStatic->SetText(pValue);

	pStatic->SetSize(100, 24);
	pStatic->SetLocation((int)(m_vecDrawCharaPos.x-(pStatic->m_width/2)), (int)(m_vecDrawCharaPos.y - (m_vecDrawCharaCenter.y) - pStatic->m_height ) );

	m_nStatuInfoTime = MAIN_CHARA_CHANGE_HP_VISIBLE_TIME;
	
	CDXUTStatic* pDel = m_pStatusInfo;
	m_pStatusInfo = pStatic;
	SafeDelete(pDel);	
}
