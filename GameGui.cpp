#include "Game.h"
#include "ext.h"
#include "util.h"

// CGame:: > OnGUIEvent
// ============================================
// Title
// ============================================
void CALLBACK CGame::GUITitle(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
    switch( nControlID )
    {
    case IDC_TITLE_BTN_CONNECT:
		ConnectServer();
		break;
	case IDC_TITLE_BTN_CONFIG:
		PlaySysSoundSE(SE_sub_ChangeView);
		p_pConfig->SetVisible(true);
		break;
    case IDC_TITLE_BTN_EXIT:
		SetState(eGameTitleRelease);		// �j�������ֈڍs
		g_bCloseSocket = TRUE;
//		StopClient();									// �X���b�h���~�߂�
//		Disconnect();								// �ؒf
//		CloseWindow(g_hWnd);
//		DXUTShutdown(0);
		break;
	}
}
// ====================================
// Login
// ====================================
void CALLBACK CGame::GUILogin(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	static WCHAR wsOldEditName[MAX_USER_NAME+1];
	static WCHAR wsOldEditPass[MAX_SRV_PASS+1];

	switch( nControlID )
    {
	case IDC_SHARE_EDIT:
		if (_tcslen(((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText()  ) > MAX_USER_NAME)
			((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->SetText(wsOldEditName);
		else
			_tcscpy_s( wsOldEditName, MAX_USER_NAME+1,((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText());
		break;
	case IDC_LOGIN_EDT_PASSWORD:
		if (_tcslen(((CDXUTEditBox*)p_pUI->GetControl(IDC_LOGIN_EDT_PASSWORD))->GetText() ) > MAX_SRV_PASS)
			((CDXUTEditBox*)p_pUI->GetControl(IDC_LOGIN_EDT_PASSWORD))->SetText(wsOldEditPass);
		else
			_tcscpy_s( wsOldEditPass, MAX_SRV_PASS+1,((CDXUTEditBox*)p_pUI->GetControl(IDC_LOGIN_EDT_PASSWORD))->GetText());
		break;
	case IDC_LOGIN_CHK_SAVE_PASS:
		PlaySysSoundSE(SE_sub_ChangeView);
		break;
	case IDC_LOGIN_BTN_LOGIN:
		{
			if (!m_bLoginStart)
				LoginServer();
			break;
		}
	case IDC_TITLE_BTN_CONFIG:
		PlaySysSoundSE(SE_sub_ChangeView);
		p_pConfig->SetVisible(true);
		break;
	case IDC_TITLE_BTN_EXIT:
		SetState(eGameLoginRelease);		// �j�������ֈڍs
		g_bCloseSocket = TRUE;
//		StopClient();									// �X���b�h���~�߂�
//		Disconnect();								// �ؒf
//		CloseWindow(g_hWnd);
//		DXUTShutdown(0);
		break;
	default:
		break;
	}
}

// ====================================
// Room
// ====================================
void CALLBACK CGame::GUIRoom(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	WORD			packetSize = 0;
	BYTE				packetData[MAX_PACKET_SIZE];
	WCHAR			msg[MAX_CHAT_MSG+1];
	int					nMsgLen = 0;

	switch( nControlID )
    {
	case IDC_ROOM_BTN_DISC:
		SetState(eGameRoomRelease);		// �j�������ֈڍs
		g_bCloseSocket = TRUE;
//		StopClient();									// �X���b�h���~�߂�
//		Disconnect();								// �ؒf
//		CloseWindow(g_hWnd);
//		DXUTShutdown(0);
		break;
	case IDC_SHARE_EDIT:
		switch(nEvent)
		{
        case EVENT_EDITBOX_STRING:
			// SHIFT�������Ă����甭�����Ȃ�
			if (GetAsyncKeyState(VK_LSHIFT)&0x8000)
			{
				p_pUI->GetIMEEditBox(IDC_SHARE_EDIT)->GetElement(0)->TextureColor.States[DXUT_STATE_NORMAL] = 0xFFAAAAAA;
				p_pUI->ClearFocus();
				return;
			}
			nMsgLen = _tcslen(((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText() );
			if ( nMsgLen > 0 )
			{
				// �`���b�g���M�Ԋu����������
				if (m_nChatSendInterval)
				{
					AddChatMessage(L"�`���b�g���b�Z�[�W�̊Ԋu�����߂��܂��B", PK_USER_CHAT_SERVER_WARNING);
					return;
				}
				if ( IsTeamRnadomizeCommand(((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText()) )
				{
					int iReqTeam = GetTeamRnadomizeCount(((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText());
					if (!iReqTeam)
					{
						AddChatMessage(L"�R�}���h�̐��l���s���ł�", PK_USER_CHAT_SERVER_WARNING);
						return;
					}
					// �p�P�b�g�쐬
					packetSize = PacketMaker::MakePacketData_TeamRandomize( packetData, (BYTE)iReqTeam );
					if (packetSize)
						AddPacket(packetData, packetSize);
					((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->ClearText();
				}
				else
				{
					BYTE bytMessageRange = GetChatMessageRange();
					// �`���b�g���b�Z�[�W�p�o�b�t�@�̃T�C�Y�𒴂��Ă�����ۂ߂�
					nMsgLen = min(MAX_CHAT_MSG*sizeof(WCHAR), nMsgLen*sizeof(WCHAR));
					// ���b�Z�[�W���o�b�t�@�ɃR�s�[
					SafeMemCopy(msg, ((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText(), nMsgLen, nMsgLen);
					// 0�Ŗ��߂�
					msg[nMsgLen/sizeof(WCHAR)] = L'\0';
					// �p�P�b�g�쐬
					packetSize = PacketMaker::MakePacketData_ChatMessage(bytMessageRange, msg, packetData);
					if (packetSize)
						AddPacket(packetData, packetSize);
					((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->ClearText();
					// ���b�Z�[�W�Ԋu�ݒ�
					m_nChatSendInterval = (int)CHAT_MESSAGE_INTERVAL;
				}
			}
			break;
		case EVENT_EDITBOX_CHANGE:
			{
				// �֑������`�F�b�N
				BOOL bCheckOK = TRUE;
				WCHAR pWMsg[MAX_MSG_BUFFER+1];
				((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetTextCopy(pWMsg, MAX_MSG_BUFFER);
				WCHAR* pW = pWMsg;
				while (*pW)
				{
					if (iswcntrl(*pW))
					{
						bCheckOK = FALSE;
						break;
					}
					++pW;
				}
				// �؂�
				if (!bCheckOK)
				{
					*pW = NULL;
					((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->SetText(pWMsg);
				}
			break;
			}
		default:
			break;
		}
		break;
	case IDC_ROOM_BTN_CONFIG:
		PlaySysSoundSE(SE_sub_ChangeView);
		p_pConfig->SetVisible(true);
		break;
	case IDC_ROOM_BTN_CHARA_SEL_LIST_L:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED)
			m_pCharacterBtnList->OnLeftButtonDown();
		else
		{
			for (int i=0;i<7;i++)
				m_pCharacterBtnList->OnLeftButtonDown();
		}
		break;
	case IDC_ROOM_BTN_CHARA_SEL_LIST_R:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED)
			m_pCharacterBtnList->OnRightButtonDown();
		else
		{
			for (int i=0;i<7;i++)
				m_pCharacterBtnList->OnRightButtonDown();
		}
		break;
	case IDC_ROOM_RB_ITEM_L:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED)
			m_pItemBtnList->OnLeftButtonDown();
		else
		{
			for (int i=0;i<7;i++)
				m_pItemBtnList->OnLeftButtonDown();
		}
		break;
	case IDC_ROOM_RB_ITEM_R:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED)
			m_pItemBtnList->OnRightButtonDown();
		else
		{
			for (int i=0;i<7;i++)
				m_pItemBtnList->OnRightButtonDown();
		}
		break;
	case IDC_ROOM_BTN_READY:			// ����OK�{�^��
		if (!m_SessionArray[m_nUserIndex].game_ready)
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		OnReadyButtonDown();
		break;
	case IDC_ROOM_BTN_ADD_ITEM:		// �g�p�A�C�e���ǉ��{�^��
		PlaySysSoundSE(SE_suc_ON);
		OnAddItemButtonDown();
		break;
	case IDC_ROOM_BTN_VIEW_CHARA:
	case IDC_ROOM_BTN_VIEW_ITEM:
	case IDC_ROOM_BTN_VIEW_RULE:
		PlaySysSoundSE(SE_sub_ChangeView);
		OnClickViewButton(nControlID);
		break;
	case IDC_ROOM_CHK_RULE_1:
		if (p_pUI->GetCheckBox(IDC_ROOM_CHK_RULE_1)->GetChecked())
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		OnRuleCheckButtonDown((BYTE)pControl->GetUserData());
		break;
	case IDC_ROOM_CHK_RULE_2:
		if (p_pUI->GetCheckBox(IDC_ROOM_CHK_RULE_2)->GetChecked())
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		OnRuleCheckButtonDown((BYTE)pControl->GetUserData());
		break;
	case IDC_ROOM_CHK_RULE_3:
		if (p_pUI->GetCheckBox(IDC_ROOM_CHK_RULE_3)->GetChecked())
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		OnRuleCheckButtonDown((BYTE)pControl->GetUserData());
		break;
	case IDC_ROOM_CHK_RULE_4:
		if (p_pUI->GetCheckBox(IDC_ROOM_CHK_RULE_4)->GetChecked())
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		OnRuleCheckButtonDown((BYTE)pControl->GetUserData());
		break;
	case IDC_ROOM_BTN_RULE_TEAM_L:
	case IDC_ROOM_BTN_RULE_TEAM_R:
		PlaySysSoundSE(SE_sub_StageSel);
		OnTeamRuleButtonDown((int)pControl->GetUserData());
		break;
	case IDC_ROOM_BTN_STAGE_L:
	case IDC_ROOM_BTN_STAGE_R:
		PlaySysSoundSE(SE_sub_StageSel);
		OnStageSelectButtonDown((int)pControl->GetUserData());
		break;
	case IDC_ROOM_BTN_STAGE_RND:
		PlaySysSoundSE(SE_sub_StageSel);
		OnStageRandomButtonDown();
		break;
	case IDC_ROOM_SPIN_LIMIT_TURN_001:
		UpdateLimitTurnFromControl(1);
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case (IDC_ROOM_SPIN_LIMIT_TURN_001+1):
		UpdateLimitTurnFromControl(-1);
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case IDC_ROOM_SPIN_LIMIT_TURN_010:
		UpdateLimitTurnFromControl(10);
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case (IDC_ROOM_SPIN_LIMIT_TURN_010+1):
		UpdateLimitTurnFromControl(-10);
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case IDC_ROOM_SPIN_LIMIT_TURN_100:
		UpdateLimitTurnFromControl(100);
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case (IDC_ROOM_SPIN_LIMIT_TURN_100+1):
		UpdateLimitTurnFromControl(-100);
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case IDC_ROOM_BTN_ACT_TIME_LIMIT_R:
	case IDC_ROOM_BTN_ACT_TIME_LIMIT_L:
		OnActTimeLimitButtonDown((int)pControl->GetUserData());
		PlaySysSoundSE(SE_sub_Arrow);
		break;
	case IDC_ROOM_BTN_MY_ITEM_SEL_L:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED) {
			m_pMyItemBtnList->OnLeftButtonDown();
		} else {
			for (int i=0; i<7; i++) {
				m_pMyItemBtnList->OnLeftButtonDown();
			}
		}
		break;
	case IDC_ROOM_BTN_MY_ITEM_SEL_R:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED) {
			m_pMyItemBtnList->OnRightButtonDown(g_nMaxItemStockCount);
		} else {
			for (int i=0; i<7; i++) {
				m_pMyItemBtnList->OnRightButtonDown(g_nMaxItemStockCount);
			}
		}
		break;
	case IDC_ROOM_BTN_CHARA_SEL_RANDOM:		// �L���������_���Z���N�g
#if RTRIAL
#else
		OnCharacterSelectRadioBtnDown(p_pUI->GetRadioButton(nControlID));
		PlaySysSoundSE(SE_sur_ListSel);
#endif
		break;
	default:
#if RTRIAL	// ���i�łƗV�ׂ�̌���
		
#else
		// �L�����I���{�^��
		if (nControlID >= IDC_ROOM_BTN_CHARA_SEL_LIST_BASE
			&& m_mapCharaScrInfo.find(nControlID-IDC_ROOM_BTN_CHARA_SEL_LIST_BASE) != m_mapCharaScrInfo.end())
		{	// ����OK�Ȃ̂ŕύX�s��
			// �I�𒆃L�����Ƀ`�F�b�N��߂�
			if (m_SessionArray[m_nUserIndex].game_ready)
				g_pGame->CheckCharacterSelectRadioBtnFromID(m_SessionArray[m_nUserIndex].chara_type);
			else
			{
				PlaySysSoundSE(SE_sur_ListSel);
				OnCharacterSelectRadioBtnDown(p_pUI->GetRadioButton(nControlID));
			}
		}// �A�C�e���I���{�^��
		else
#endif	// RTRIAL
			if (nControlID >= IDC_ROOM_RB_ITEM_BASE_INDEX
			&& nControlID < IDC_ROOM_RB_ITEM_BASE_INDEX+GAME_ITEM_COUNT)
		{
			PlaySysSoundSE(SE_sur_ListSel);
			OnSelectItemBtnList(p_pUI->GetRadioButton(nControlID));
		}// �g�p�A�C�e�����N���b�N
		else if (nControlID >= IDC_MAIN_BTN_MY_ITEM_BASE && nControlID < (IDC_MAIN_BTN_MY_ITEM_BASE+g_nMaxItemStockCount))
		{
			if (nEvent == EVENT_BUTTON_CLICKED)
			{
				if (OnMyItemButtonDown(nControlID))
					PlaySysSoundSE(SE_suc_OFF);
			}
			else if (nEvent == EVENT_RBUTTON_CLICKED)
				ShowItemDetail(nControlID);
		}
		break;
	}
}

// ====================================
// Load
// ====================================
void CALLBACK CGame::GUILoad(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	
}

// ====================================
// Main
// ====================================
void CALLBACK CGame::GUIMain(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	WORD			packetSize = 0;
	BYTE				packetData[MAX_PACKET_SIZE];
	WCHAR			msg[MAX_CHAT_MSG+1];
	int					nMsgLen = 0;

	switch( nControlID )
    {
	case IDC_SHARE_EDIT:
		switch(nEvent)
		{
        case EVENT_EDITBOX_STRING:
			nMsgLen = _tcslen(((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText() );
			// SHIFT�������Ă����甭�����Ȃ�
			if (GetAsyncKeyState(VK_LSHIFT)&0x8000)
			{
				p_pUI->GetIMEEditBox(IDC_SHARE_EDIT)->GetElement(0)->TextureColor.States[DXUT_STATE_NORMAL] = 0xFFAAAAAA;
				p_pUI->ClearFocus();
				
				CDXUTControl *pChatLogBG = p_pUI->GetControl(IDC_MAIN_SPRITE_CHATLOG_BG);
				if (MAIN_CHATLOG_LB_CNT_X <= m_nMousePosX && m_nMousePosX <= (MAIN_CHATLOG_LB_CNT_X+MAIN_CHATLOG_LB_CNT_W)
				&& MAIN_CHATLOG_LB_CNT_Y <= m_nMousePosY && m_nMousePosY <= (MAIN_CHATLOG_LB_CNT_Y+MAIN_CHATLOG_LB_CNT_H))
				{
					if (!pChatLogBG->GetVisible())
						pChatLogBG->SetVisible(true);
				}
				else
				{
					if (pChatLogBG->GetVisible())
						pChatLogBG->SetVisible(false);
				}
				return;
			}
			if ( nMsgLen > 0 )
			{
				BYTE bytMessageRange = GetChatMessageRange();
				// �`���b�g���b�Z�[�W�p�o�b�t�@�̃T�C�Y�𒴂��Ă�����ۂ߂�
				nMsgLen = min(MAX_CHAT_MSG*sizeof(WCHAR), nMsgLen*sizeof(WCHAR));
				// ���b�Z�[�W���o�b�t�@�ɃR�s�[
				SafeMemCopy(msg, ((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetText(), nMsgLen, nMsgLen);
				// 0�Ŗ��߂�
				msg[nMsgLen/sizeof(WCHAR)] = L'\0';
				// �p�P�b�g�쐬
				packetSize = PacketMaker::MakePacketData_ChatMessage(bytMessageRange, msg, packetData);
				if (packetSize)
					AddPacket(packetData, packetSize);
				((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->ClearText();
			}
			break;
		case EVENT_EDITBOX_CHANGE:
			{
				// �֑������`�F�b�N
				BOOL bCheckOK = TRUE;
				WCHAR pWMsg[MAX_MSG_BUFFER+1];
				((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->GetTextCopy(pWMsg, MAX_MSG_BUFFER);
				WCHAR* pW = pWMsg;
				while (*pW)
				{
					if (iswcntrl(*pW))
					{
						bCheckOK = FALSE;
						break;
					}
					++pW;
				}
				// �؂�
				if (!bCheckOK)
				{
					*pW = NULL;
					((CDXUTIMEEditBox*)p_pUI->GetControl(IDC_SHARE_EDIT))->SetText(pWMsg);
				}
			}
		}
		break;
	case IDC_MAIN_RB_SEL_BULLET_L:
		PlaySysSoundSE(SE_sub_Arrow);
		m_pSelBulletBtnList->OnLeftButtonDown();
		break;
	case IDC_MAIN_RB_SEL_BULLET_R:
		PlaySysSoundSE(SE_sub_Arrow);
		m_pSelBulletBtnList->OnRightButtonDown();
		break;
	case IDC_MAIN_BTN_CONFIG:
		PlaySysSoundSE(SE_sub_ChangeView);
		p_pConfig->SetVisible(true);
		break;
	case IDC_MAIN_BTN_TURN_PASS:
		OnTurnPassButtonDown(nControlID);
		break;
	case IDC_ROOM_BTN_MY_ITEM_SEL_L:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED) {
			m_pMyItemBtnList->OnLeftButtonDown();
		} else {
			for (int i=0; i < 4; i++) {
				m_pMyItemBtnList->OnLeftButtonDown();
			}
		}
		break;
	case IDC_MAIN_BTN_MY_ITEM_SEL_R:
		PlaySysSoundSE(SE_sub_Arrow);
		if (nEvent == EVENT_BUTTON_CLICKED) {
			m_pMyItemBtnList->OnRightButtonDown(g_nMaxItemStockCount);
		} else {
			for (int i=0; i < 4; i++) {
				m_pMyItemBtnList->OnRightButtonDown(g_nMaxItemStockCount);
			}
		}
		break;
	default:	
		// �e�I���{�^�����������ꂽ
		if (nControlID >= IDC_MAIN_BTN_SEL_BULLET_BASE && nControlID < (IDC_MAIN_BTN_SEL_BULLET_BASE+MAX_CHARA_BULLET_TYPE+1))
			OnSelectBulletType(nControlID);
		else if (nControlID >= IDC_MAIN_BTN_MY_ITEM_BASE && nControlID < (IDC_MAIN_BTN_MY_ITEM_BASE+g_nMaxItemStockCount))
		{
			if (nEvent == EVENT_BUTTON_CLICKED)
				ReqUseItem(nControlID);
			else if (nEvent == EVENT_RBUTTON_CLICKED)
				ShowItemDetail(nControlID);
		}
		break;
	}
}

// ====================================
// Result
// ====================================
void CALLBACK CGame::GUIResult(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	WORD			packetSize = 0;
	BYTE				packetData[MAX_PACKET_SIZE];

	switch( nControlID )
    {
	case IDC_RESULT_BTN_DISC:
		SetState(eGameRoomRelease);		// �j�������ֈڍs
		g_bCloseSocket = TRUE;
//		StopClient();									// �X���b�h���~�߂�
//		Disconnect();								// �ؒf
//		CloseWindow(g_hWnd);
//		DXUTShutdown(0);
		break;
	case IDC_RESULT_BTN_CONFIRM:
		packetSize = PacketMaker::MakePacketData_Confirm(m_nUserIndex, packetData);
		AddPacket(packetData, packetSize);
		break;
	case IDC_RESULT_SB_RANK:
		if (nEvent == EVENT_SCROLLBAR_POS_CHANGE)
			UpdateRankView();
	default:
		break;
	}
}


// ====================================
// Config
// ====================================
void CALLBACK CGame::OnConfigEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
    {
//	case IDC_CONFIG_BTN_OK:
//		p_pConfig->SetVisible(false);
//		break;
	case IDC_CONFIG_SLIDER_BGM:
		m_bytBGMVolume = (BYTE)p_pConfig->GetSlider(IDC_CONFIG_SLIDER_BGM)->GetValue();
		// BGM�{�����[����1�ȏ�
		if (m_bytBGMVolume > 0)
		{
			// �Đ����Ă��Ȃ��Ȃ�Đ��J�n
			if (m_nBGMPlayingID == -1)
			{
				PlayBGM(m_nBGMSoundID);
			}
			else
			{
				CSoundBuffer* pSoundBuffer = m_pSoundPlayer->GetSoundBufferFromID(m_nBGMPlayingID);
				if (pSoundBuffer)
					pSoundBuffer->SetVolume(m_bytBGMVolume);
				else
					m_nBGMPlayingID = -1;
			}
		}

		break;
	case IDC_CONFIG_SLIDER_SE:
		m_bytSEVolume = (BYTE)p_pConfig->GetSlider(IDC_CONFIG_SLIDER_SE)->GetValue();
		break;
	case IDC_CONFIG_CHK_EFFECT:
		if (p_pConfig->GetCheckBox(IDC_CONFIG_CHK_EFFECT)->GetChecked())
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		break;
	case IDC_CONFIG_CHK_BLT_FOCUS:
		if (p_pConfig->GetCheckBox(IDC_CONFIG_CHK_BLT_FOCUS)->GetChecked())
			PlaySysSoundSE(SE_suc_ON);
		else
			PlaySysSoundSE(SE_suc_OFF);
		break;
	case IDC_CONFIG_BTN_CLOSE:
		PlaySysSoundSE(SE_sub_ChangeView);
		SaveConfig();
		// BGM����0�Ȃ�Đ����~�߂�
		if (!m_bytBGMVolume)
			StopBGM(FALSE);

		p_pConfig->SetVisible(false);
		break;
	default:
		break;
	}
}
