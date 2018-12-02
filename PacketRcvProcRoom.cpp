#include "ext.h"
#include "main.h"

BOOL PacketProc_Room(BYTE *data)
{
	BOOL ret = FALSE;
	switch (data[3])
	{
	case PK_USER_ROOMINFO_IN:
		{
			int nNumUsers = min(g_pGame->GetMaxLoginNum(), data[4]);
			int nIndex = 5;
			g_pCriticalSection->EnterCriticalSection_Session(L'E');
			for (int i=0;i<nNumUsers;i++)
			{
				type_session sess;
				ZeroMemory(&sess, sizeof(type_session));
				// �L����No
				sess.sess_index = (BYTE)data[nIndex];
				sess.obj_no = sess.sess_index;
				nIndex++;
				// �I�u�W�F�N�g�^�C�v
				sess.obj_type = OBJ_TYPE_CHARA;
				// �L�����^�C�v
				sess.chara_type = data[nIndex];
				nIndex++;
				// ���[�U����
				sess.name_len = data[nIndex];
				// ���[�U���͈̓I�[�o�[
				if (sess.name_len > MAX_USER_NAME*sizeof(WCHAR) || data[nIndex] == NULL)
				{
					g_pCriticalSection->LeaveCriticalSection_Session();
					DXTRACE_MSG(L"PacketProc_RoomInfo:���[�U���͈̓I�[�o�[");
					return FALSE;
				}
				nIndex++;
				// ���[�U��
				SafeMemCopy(sess.name, &data[nIndex], sess.name_len, MAX_USER_NAME*sizeof(WCHAR));
				nIndex += sess.name_len;
				// ���
				memcpy(&sess.obj_state, (BYTE*)&data[nIndex], sizeof(E_TYPE_OBJ_STATE));
				nIndex+= sizeof(E_TYPE_OBJ_STATE);
				// �}�X�^�[���
				sess.master = data[nIndex];
				nIndex++;
				// �`�[��No
				sess.team_no = data[nIndex];
				nIndex++;
				// ����OK
				sess.game_ready = data[nIndex];
				nIndex++;
				// ���W�lX
				memcpy(&sess.lx, (BYTE*)&data[nIndex], sizeof(short));
				nIndex+= sizeof(short);
				// ���W�lY
				memcpy(&sess.ly, (BYTE*)&data[nIndex], sizeof(short));
				nIndex+= sizeof(short);
				// �ړ��lX
				memcpy(&sess.vx, (BYTE*)&data[nIndex], sizeof(short));
				nIndex+= sizeof(short);
				// �ړ��lY
				memcpy(&sess.vy, (BYTE*)&data[nIndex], sizeof(short));
				nIndex+= sizeof(short);
				// ����
				sess.dir = (E_TYPE_USER_DIRECTION)data[nIndex];
				nIndex++;
				sess.entity = 1;
				memcpy(&sess.cost, (BYTE*)&data[nIndex], sizeof(short));
				nIndex+= sizeof(short);

				sess.connect_state = CONN_STATE_AUTHED;
				// �L�������ݒ�
				g_pGame->SetNewUser(&sess, FALSE);
				// ������Ԃ�ݒ�
				g_pGame->SetGameReady(sess.sess_index, sess.game_ready);
			}
			// �������̗L����ԍX�V
			g_pGame->UpdateGameMasterPropertyControlsEnabled();
			g_pGame->UpdateReadyButtonState();
			g_pGame->UpdatePropertyControlsEnabled();
//			ZeroMemory(g_pGame->GetMySessionInfo()->items, sizeof(DWORD)*GAME_ITEM_STOCK_MAX_COUNT);
			g_pGame->UpdateMyItems();
			g_pCriticalSection->LeaveCriticalSection_Session();
		}
		break;
	case PK_USER_ROOMINFO_NEW:
		{
			int nIndex = 4;
			g_pCriticalSection->EnterCriticalSection_Session(L'R');
			type_session sess;
			ZeroMemory(&sess, sizeof(type_session));
			// �L����No
			sess.sess_index = (BYTE)data[nIndex];
			sess.obj_no = sess.sess_index;
			nIndex++;
			// �I�u�W�F�N�g�^�C�v
			sess.obj_type = OBJ_TYPE_CHARA;
			// �L�����^�C�v
			sess.chara_type = data[nIndex];
			nIndex++;
			// ���[�U����
			sess.name_len = data[nIndex];
			// ���[�U���͈̓I�[�o�[
			if (sess.name_len > MAX_USER_NAME*sizeof(WCHAR) || data[nIndex] == NULL)
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				DXTRACE_MSG(L"PacketProc_RoomInfo:���[�U���͈̓I�[�o�[");
				return FALSE;
			}
			nIndex++;
			// ���[�U��
			SafeMemCopy(sess.name, &data[nIndex], sess.name_len, MAX_USER_NAME*sizeof(WCHAR));
			nIndex += sess.name_len;
			// ���
			memcpy(&sess.obj_state, (BYTE*)&data[nIndex], sizeof(E_TYPE_OBJ_STATE));
			nIndex+= sizeof(E_TYPE_OBJ_STATE);
			// �}�X�^�[���
			sess.master = data[nIndex];
			nIndex++;
			// �`�[��No
			sess.team_no = data[nIndex];
			nIndex++;
			// ����OK
			sess.game_ready = data[nIndex];
			nIndex++;
			// ���W�lX
			memcpy(&sess.lx, (BYTE*)&data[nIndex], sizeof(short));
			nIndex+= sizeof(short);
			// ���W�lY
			memcpy(&sess.ly, (BYTE*)&data[nIndex], sizeof(short));
			nIndex+= sizeof(short);
			// �ړ��lX
			memcpy(&sess.vx, (BYTE*)&data[nIndex], sizeof(short));
			nIndex+= sizeof(short);
			// �ړ��lY
			memcpy(&sess.vy, (BYTE*)&data[nIndex], sizeof(short));
			nIndex+= sizeof(short);
			// ����
			sess.dir = (E_TYPE_USER_DIRECTION)data[nIndex];
			nIndex++;
			sess.connect_state = CONN_STATE_AUTHED;
			sess.entity = 1;
			memcpy(&sess.cost, (BYTE*)&data[nIndex], sizeof(short));
			g_nMaxCost = sess.cost;
			g_nMaxItemStockCount = (int)(sess.cost/GAME_ITEM_01_COST);
			nIndex+= sizeof(short);
			// �L�������ݒ�
			g_pGame->SetNewUser(&sess, TRUE);
			// ������Ԃ�ݒ�
			g_pGame->SetGameReady(sess.sess_index, sess.game_ready);
			// �������̗L����ԍX�V
			if (sess.master)
				g_pGame->SetGameMaster(sess.sess_index, TRUE);
			g_pGame->UpdateGameMasterPropertyControlsEnabled();
			g_pGame->UpdateReadyButtonState();
			g_pGame->UpdatePropertyControlsEnabled();
			g_pCriticalSection->LeaveCriticalSection_Session();
			OutputDebugStr(L"PK_USER_ROOMINFO_NEW\n");
		}
		break;
	case PK_USER_ROOMINFO_MASTER:
		{
			int nCharaIndex = data[4];		// ���[�UNo
			BOOL bMaster = (BOOL)data[5];	// �}�X�^�[�ݒ�
			// ��Ԑݒ�	
			g_pCriticalSection->EnterCriticalSection_Session(L'T');
			g_pGame->SetGameMaster(nCharaIndex, bMaster);
			g_pGame->SetGameReady(nCharaIndex, FALSE);
			g_pCriticalSection->LeaveCriticalSection_Session();
			g_pGame->UpdateReadyButtonState();
		}
		break;
	case PK_USER_ROOMINFO_MOVE:	// �����ړ��p�P�b�g
		{
			int nCharaIndex = data[4];		// ���[�UNo
			short nPosX = 0;	// �ʒuX
			memcpy(&nPosX, (BYTE*)&data[5], sizeof(short));
			short nPosY =0;	// �ʒuY
			memcpy(&nPosY, (BYTE*)&data[7], sizeof(short));
			short nVecX = 0;		// �ړ�X
			memcpy(&nVecX, (BYTE*)&data[9], sizeof(short));
			short nVecY = 0;	// �ړ�Y
			memcpy(&nVecY, (BYTE*)&data[11], sizeof(short));
			g_pCriticalSection->EnterCriticalSection_Session(L'Y');
			type_session* pSess = g_pGame->GetAuthedSessionInfo(nCharaIndex);
			if (!pSess)
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				break;
			}
			pSess->lx = nPosX;
			pSess->ly = nPosY;
			pSess->vx = nVecX;
			pSess->vy = nVecY;
			if (nVecX != 0)
				pSess->dir = (nVecX>0)?USER_DIRECTION_RIGHT:USER_DIRECTION_LEFT;
#ifdef _DEBUG
			WCHAR log[64];
			SafePrintf(log, 64, L"�ړ�:%d,%d / %d,%d\n", nPosX, nPosY, nVecX, nVecY);
			DXUTTRACE(log);
#endif
			g_pCriticalSection->LeaveCriticalSection_Session();
		}
		break;
	case PK_USER_ROOMINFO_CHARA_SEL:		// �L�����I��
		{
			int nCharaIndex = data[4];		// ���[�UNo
			int nCharaType = data[5];		// �L�����^�C�v
			// ���X�V
			g_pCriticalSection->EnterCriticalSection_Session(L'U');
			type_session* pSess = g_pGame->GetAuthedSessionInfo(nCharaIndex);
			if (!pSess)
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				break;
			}
			pSess->chara_type = nCharaType;
			g_pCriticalSection->LeaveCriticalSection_Session();
			// �������ς�����̂Ȃ�A�L�����I����ʕύX
			if (nCharaIndex == g_pGame->GetUserIndex())
			{
#if RTRIAL
				g_bCloseSocket = TRUE;	// �I��
#else
				g_pGame->SetChacterSelectInfo(nCharaType);
#endif
			}

			g_pGame->UpdateRoomCharacter(nCharaIndex);
			g_pGame->UpdateUserList(nCharaIndex);
			OutputDebugStr(L"PK_USER_ROOMINFO_CHARA_SEL");
		}
		break;
	case PK_USER_ROOMINFO_STAGE_SEL:	// �X�e�[�W�I��
		g_pGame->SetStageIndex((int)data[4]);
		break;
	case PK_USER_ROOMINFO_RULE:
		g_pGame->SetRule(data[4]);
		break;
	case PK_USER_ROOMINFO_RULE_TURN_LIMIT:
		{
			short turn = 0;
			memcpy(&turn, (void*)&data[4], sizeof(short));
			g_pGame->SetTurnLimit(turn);
		}
		break;
	case PK_USER_ROOMINFO_RULE_ACT_TIME_LIMIT:
		g_pGame->SetActTimeLimit(data[4]);
		break;
	case PK_USER_ROOMINFO_ITEM_SEL:	// �A�C�e���I��
		{
			int nItemIndex = data[4];		// �A�C�e����No
			DWORD dwItemFlg;
			memcpy(&dwItemFlg, (void*)&data[5], sizeof(DWORD));	// �A�C�e���t���O
			WORD wCost = 0;
			memcpy(&wCost, &data[9], sizeof(short));

			g_pCriticalSection->EnterCriticalSection_Session(L'I');
			type_session* pSess = g_pGame->GetMySessionInfo();
			if (pSess)
			{
				pSess->items[nItemIndex] = dwItemFlg;
				pSess->cost = wCost;
				g_pGame->UpdateMyItems();
			}
			g_pCriticalSection->LeaveCriticalSection_Session();
		}
		break;
	case PK_USER_ROOMINFO_TEAM_COUNT:
		{
			int nTeamCount = data[4];
			g_pGame->SetTeamCount(nTeamCount);
//			g_pGame->ResetGameReady();
		}
		break;
	case PK_USER_ROOMINFO_READY:
		{
			int nCharaIndex = data[4];		// ���[�UNo
			BOOL bReady = (BOOL)data[5];	// �L����No
			// ��Ԑݒ�	
			g_pCriticalSection->EnterCriticalSection_Session(L'O');
			g_pGame->SetGameReady(nCharaIndex, bReady);
			g_pGame->UpdateReadyButtonState();
			g_pCriticalSection->LeaveCriticalSection_Session();
			OutputDebugStr(L"PK_USER_ROOMINFO_READY");
		}
		break;
	case PK_USER_ROOMINFO_RE_ENTER:
		{
			int nIndex = 4;
			int nCharaIndex = data[nIndex];	// user No
			nIndex++;
			g_pCriticalSection->EnterCriticalSection_Session(L'P');
			// �ē��ꎞ�A���ڑ����[�U�[�̃N���A
			for (int i=0;i<g_pGame->GetMaxLoginNum();i++)
			{
				ptype_session inisess = g_pGame->GetSessionInfo(i);
				if (inisess->connect_state != CONN_STATE_AUTHED)
//					inisess->entity = 0;
					ZeroMemory(inisess, sizeof(type_session));
			}

			ptype_session sess = g_pGame->GetAuthedSessionInfo(nCharaIndex);
			if (!sess)
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				break;
			}
			sess->entity = 1;
			memcpy(&sess->lx, &data[nIndex], sizeof(short));
			nIndex+=sizeof(short);
			memcpy(&sess->ly, &data[nIndex], sizeof(short));
			nIndex+=sizeof(short);
			memcpy(&sess->vx, &data[nIndex], sizeof(short));
			nIndex+=sizeof(short);
			memcpy(&sess->vy, &data[nIndex], sizeof(short));
			// ����
			sess->dir = (E_TYPE_USER_DIRECTION)data[nIndex];
			nIndex++;
			nIndex+=sizeof(short);
			sess->master = data[nIndex];
			nIndex++;
			memcpy(&sess->obj_state, &data[nIndex], sizeof(E_TYPE_OBJ_STATE));
			g_pGame->ReEnterCharacter(nCharaIndex);
			g_pCriticalSection->LeaveCriticalSection_Session();
			break;
		}
	case PK_USER_ROOMINFO_TEAM_RAND:
		{
			int nIndex = 4;
			int nTeams = data[nIndex];	// Team num
			if (nTeams <= 0)
			{
				g_pGame->AddChatMessage(L"�������`�[��������͂��ĉ�����", PK_USER_CHAT_SERVER_INFO);
			}
			else
			{
				g_pGame->AddChatMessage(L"---------�`�[������---------", PK_USER_CHAT_SERVER_INFO);
				++nIndex;
				for (int i=0;i<nTeams;++i)
				{
					size_t size = data[nIndex];
					++nIndex;
					WCHAR* msg = new WCHAR[size/sizeof(WCHAR)+1];
					memset((void*)msg, 0, size+sizeof(WCHAR));
					if (msg)
					{
						memcpy((void*)msg, &data[nIndex], size);
						g_pGame->AddChatMessage(msg, PK_USER_CHAT_SERVER_INFO);
						delete [] msg;
					}
					nIndex += size;
				}
			}
		}
		break;
	}
	return ret;
}

