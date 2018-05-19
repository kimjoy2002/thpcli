#include "CTeamRulePropertyControlManager.h"

CTeamRulePropertyControlManager::CTeamRulePropertyControlManager()
{
	m_bCreated = FALSE;
	m_bVisible = FALSE;
	m_pntLocate.x = 0;
	m_pntLocate.y = 0;
	m_pBtnArrowL = NULL;
	m_pBtnArrowR = NULL;
	m_pStaticText = NULL;
}

// ����
// pBtnArrowL	: �����{�^��
// pBtnArrowR	: �E���{�^��
// pStaticText	: �����\���p�X�^�e�B�b�N
// pntLocate		: ����ʒu
void CTeamRulePropertyControlManager::Create(CDXUTButton *pBtnArrowL, CDXUTButton *pBtnArrowR, CDXUTStatic* pStaticText, int *pCount)
{
	m_pBtnArrowL = pBtnArrowL;
	m_pBtnArrowR = pBtnArrowR;
	m_pStaticText = pStaticText;
	p_pUserCount = pCount;
	m_bCreated = TRUE;
	m_pStaticText->GetElement(0)->SetFont(TEAMRULEPROPCONT_FONT, 0xFFFFFFFF);
	Update(false, 1);
}

void CTeamRulePropertyControlManager::Destroy()
{
	m_pBtnArrowL = NULL;
	m_pBtnArrowR = NULL;
	m_pStaticText = NULL;
	p_pUserCount = NULL;
	m_bCreated = FALSE;
}

void CTeamRulePropertyControlManager::Update(bool b, int nTeamCount)
{
	if (!m_bCreated)	return;
	// �l��������点�Ȃ�
	SetButtonEnable(m_pBtnArrowL, b && m_bEnable && (nTeamCount-1) > 0);

//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
	// ���O�C���l��-1��葝�₹�Ȃ�

	if (b)
	{
		int nMaxTeamCount = min(MAXUSERNUM-1,(*p_pUserCount)-1);
		if ( nMaxTeamCount > 1 && nMaxTeamCount > nTeamCount)
			m_pBtnArrowR->SetEnabled(true);
		else
			m_pBtnArrowR->SetEnabled(false);
	}
	else
		m_pBtnArrowR->SetEnabled(false);
//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
	UpdateText(nTeamCount);
}

void CTeamRulePropertyControlManager::SetEnable(BOOL b)
{
	m_bEnable = b;
}

void CTeamRulePropertyControlManager::SetVisible(bool b)
{
	if (!m_bCreated)	return;
	m_pBtnArrowL->SetVisible(b);
	m_pBtnArrowR->SetVisible(b);
	m_pStaticText->SetVisible(b);
}

void CTeamRulePropertyControlManager::UpdateText(int nTeamCount)
{
	// �l
	if (nTeamCount <= 1)
	{
		m_pStaticText->SetText(TEAMRULEPROPCONT_TEAM_ZERO);
	}
	else
	{
		WCHAR wsText[MAX_TEAMRULEPROPCONT_TEXT+1];
		SafePrintf(wsText, MAX_TEAMRULEPROPCONT_TEXT, TEAMRULEPROPCONT_TEXTFORMAT, nTeamCount);
		m_pStaticText->SetText(wsText);
	}
}

void CTeamRulePropertyControlManager::SetButtonEnable(CDXUTButton* pBtn, bool b)
{
	if (m_bEnable)
		pBtn->SetEnabled(b);
	else
		pBtn->SetEnabled(false);
}