#include "ext.h"
#include "main.h"

BOOL recvall(int sock, char* pkt, int* recvsize, int bufsize);
//
BOOL PacketProc_Chat(BYTE *data);
BOOL PacketProc_Room(BYTE *data);
BOOL PacketProc_Load(BYTE* data);
BOOL PacketProc_Main(BYTE* data);
BOOL PacketProc_File(BYTE *data);

BOOL RecvPacketProc(BYTE *data)
{
	BOOL ret = FALSE;
	switch (data[2])
	{
	case PK_SYN:
		{
			AddMessageLog(L"recv SYN");
			BYTE pkt[MAX_PACKET_SIZE];
			INT packetSize = PacketMaker::MakePacketData_ACK(pkt);
			ret = g_pGame->AddPacket(pkt, packetSize);
			break;
		}
	case PK_USER_AUTH:
		// �F�ؑO�̃��O�C����ʂȂ�
		if (g_pGame->GetConnState() == CONN_STATE_EMPTY
			&& g_pGame->GetState() == eGameLogin)
		{
			switch (data[3])
			{
			case AUTH_RESULT_SUCCESS:	// �F��OK
				g_pGame->PlaySysSoundSE(SE_sub_Login);
				AddMessageLog(L"�F�،��ʁFAUTH_RESULT_SUCCESS");
				g_pCriticalSection->EnterCriticalSection_Session(L'a');
				g_pGame->SuccessAuth(data);
				if (!g_pGame->InitControl())
				{
					MessageBox(g_hWnd, L"�F�،��ʁF�f�[�^�s��", L"�F�،���", MB_OK);
					g_bCloseSocket = TRUE;
				}
				g_pGame->SetState(eGameLoginRelease);
				g_pCriticalSection->LeaveCriticalSection_Session();
				break;
			case AUTH_RESULT_INVALID_PWD:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF�s���ȃp�X���[�h");
				MessageBox(g_hWnd, L"�F�،��ʁF�s���ȃp�X���[�h", L"�F�،���", MB_OK);
				// ���O�C����x�����֎~�̂��ߖ����ɂ��Ă����̂�����
				g_SysUI.GetControl(IDC_LOGIN_BTN_LOGIN)->SetEnabled(true);
				g_pGame->m_bLoginStart = FALSE;
				break;
			case AUTH_RESULT_INVALID_USER_NAME:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF���[�U�����Z������A�܂��͒������܂�");
				MessageBox(g_hWnd, L"�F�،��ʁF���[�U�����Z������A�܂��͒������܂�", L"�F�،���", MB_OK);
				// ���O�C����x�����֎~�̂��ߖ����ɂ��Ă����̂�����
				g_SysUI.GetControl(IDC_LOGIN_BTN_LOGIN)->SetEnabled(true);
				g_pGame->m_bLoginStart = FALSE;
				break;
			case AUTH_RESULT_NO_UNIQUE_USER:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF���ɓ������[�U�������O�C�����Ă��܂�");
				MessageBox(g_hWnd, L"�F�،��ʁF���ɓ������[�U�������O�C�����Ă��܂�", L"�F�،���", MB_OK);
				// ���O�C����x�����֎~�̂��ߖ����ɂ��Ă����̂�����
				g_SysUI.GetControl(IDC_LOGIN_BTN_LOGIN)->SetEnabled(true);
				g_pGame->m_bLoginStart = FALSE;
				break;
			case AUTH_RESULT_GAME_PHASE:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF���݃Q�[�����̂��ߓ���܂���");
				MessageBox(g_hWnd, L"�F�،��ʁF���݃Q�[�����̂��ߓ���܂���", L"�F�،���", MB_OK);
				// ���O�C����x�����֎~�̂��ߖ����ɂ��Ă����̂�����
				g_SysUI.GetControl(IDC_LOGIN_BTN_LOGIN)->SetEnabled(true);
				g_pGame->m_bLoginStart = FALSE;
				break;
			case AUTH_RESULT_GAME_LOAD:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF�Q�[�����J�n���ꂽ���ߓ���܂���");
				MessageBox(g_hWnd, L"�F�،��ʁF�Q�[�����J�n���ꂽ���ߓ���܂���", L"�F�،���", MB_OK);
				// ���O�C����x�����֎~�̂��ߖ����ɂ��Ă����̂�����
				g_SysUI.GetControl(IDC_LOGIN_BTN_LOGIN)->SetEnabled(true);
				g_pGame->m_bLoginStart = FALSE;
				break;
			case AUTH_RESULT_GAME_INVALID:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF�T�[�o�������Ă���\��������܂�");
				MessageBox(g_hWnd, L"�F�،��ʁF�T�[�o�������Ă���\��������܂�", L"�F�،���", MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_TIMEOUT:
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				AddMessageLog(L"�F�،��ʁF���O�C���^�C���A�E�g���܂���");
				MessageBox(g_hWnd, L"�F�،��ʁF���O�C���^�C���A�E�g���܂���", L"�F�،���", MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_INVALID_SCR_CHARA_COUNT:
				AddMessageLog(L"�L�����X�N���v�g�s��");
				MessageBox(g_hWnd, L"�T�[�o�ɐڑ����邽�߂̃L�����X�N���v�g���s�����Ă��܂��B", L"�X�N���v�g�`�F�b�N", MB_ICONINFORMATION|MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_INVALID_SCR_STAGE_COUNT:
				AddMessageLog(L"�X�e�[�W�X�N���v�g�s��");
				MessageBox(g_hWnd, L"�T�[�o�ɐڑ����邽�߂̃X�e�[�W�X�N���v�g���s�����Ă��܂��B", L"�X�N���v�g�`�F�b�N", MB_ICONINFORMATION|MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_INVALID_SCR_CHARA_NOT_FOUND:
				AddMessageLog(L"�L�����X�N���v�g�s��");
				{
					BYTE group = data[4];
					WORD id = 0;
					memcpy(&id, &data[5], sizeof(WORD));
					WCHAR log[128];
					SafePrintf(log, 128, L"�T�[�o�ɐڑ����邽�߂̃L�����X�N���v�g��������܂���ł���(ID=%d)", id);
					MessageBox(g_hWnd, log, L"�X�N���v�g�`�F�b�N", MB_ICONINFORMATION|MB_OK);
				}
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_INVALID_SCR_STAGE_NOT_FOUND:
				AddMessageLog(L"�X�e�[�W�X�N���v�g�s��");
				{
					BYTE group = data[4];
					WORD id = 0;
					memcpy(&id, &data[5], sizeof(WORD));
					WCHAR log[128];
					SafePrintf(log, 128, L"�T�[�o�ɐڑ����邽�߂̃X�e�[�W�X�N���v�g��������܂���ł���(ID=%d)", id);
					MessageBox(g_hWnd, log, L"�X�N���v�g�`�F�b�N", MB_ICONINFORMATION|MB_OK);
				}
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_INVALID_HASH:
				{
					WCHAR* pName = NULL;
					WCHAR alog[128];
#if TRIAL==0
					WCHAR nlog[128];
#endif
					BYTE group = data[4];
					WORD id = 0;
					memcpy(&id, &data[5], sizeof(WORD));
					if (group == 0)
					{
						pName = g_pGame->GetScrName(id, FALSE);
						SafePrintf(alog, 128, L"�F�،��ʁF�T�[�o�̃f�[�^�ƃN���C�A���g�̃f�[�^����v���܂���ł���\n�i�X�e�[�W�X�N���v�gID[%d][%s]�̃f�[�^�s��v�j\n�T�[�o�ɂ��m�F��������", id, pName);
#if TRIAL==0
						SafePrintf(nlog, 128, L"�T�[�o���g�p���Ă���X�e�[�W�X�N���v�gID[%d][%s]��v�����܂����H\n(�u�͂��v�������Ǝ�M�����܂ŉ����������Ȃ�܂�)", id, pName);
#endif
					}
					else
					{
						pName = g_pGame->GetScrName(id, TRUE);
						SafePrintf(alog, 128, L"�F�،��ʁF�T�[�o�̃f�[�^�ƃN���C�A���g�̃f�[�^����v���܂���ł���\n�i�L�����N�^�X�N���v�gID[%d][%s]�̃f�[�^�s��v�j\n�T�[�o�ɂ��m�F��������", id, pName);
#if TRIAL==0
						SafePrintf(nlog, 128, L"�T�[�o���g�p���Ă���L�����N�^�X�N���v�gID[%d][%s]��v�����܂����H\n(�u�͂��v�������Ǝ�M�����܂ŉ����������Ȃ�܂�)", id, pName);
#endif
					}
					g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
					AddMessageLog(alog);
					MessageBox(g_hWnd, alog, L"�F�،���", MB_OK);
#if TRIAL==0
					if (g_bCloseSocket) return FALSE;
					if (id >= 100 && MessageBox(g_hWnd, nlog, L"�f�[�^�s��v", MB_YESNO) == IDYES)
					{
						if (g_bCloseSocket) return FALSE;
						BYTE pkt[MAX_PACKET_SIZE];
						INT pktsize = PacketMaker::MakePacketData_ReqFileHash(group==0, id, 0, pkt);
						return g_pGame->AddPacket(pkt, pktsize);
					}
					else
#endif
					{
						g_bCloseSocket = TRUE;
						ret = TRUE;
					}
					break;
				}
			case AUTH_RESULT_FILESEND_ERROR:
				g_pGame->DeleteRecvScrFile();
				AddMessageLog(L"�t�@�C����M�G���[");
				MessageBox(g_hWnd, L"�t�@�C����M���G���[���������܂����B\n��M�����X�N���v�g�t�@�C���̂ݍ폜���܂��B", L"�f�[�^��M", MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_FILESEND_END:
				AddMessageLog(L"�f�[�^��M�I���");
				MessageBox(g_hWnd, L"�f�[�^��M���I���܂����B\n������x�N�����ڑ����Ă�������", L"�f�[�^��M", MB_ICONINFORMATION|MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			case AUTH_RESULT_FILESEND_DISABLE:
				AddMessageLog(L"���M�s�̃f�[�^");
				MessageBox(g_hWnd, L"���M�s�̃f�[�^�ł��B", L"�f�[�^��M", MB_ICONINFORMATION|MB_OK);
				g_bCloseSocket = TRUE;
				ret = TRUE;
				break;
			default:
				break;
			}
		}
		break;
	case PK_REQ_HASH:
		ret = g_pGame->ReqPacketHash(data);
		break;
	case PK_USER_CHAT:	// �`���b�g�p�P�b�g
		ret = PacketProc_Chat(data);
		break;
	case PK_USER_ROOMINFO:
		ret = PacketProc_Room(data);
		break;
	case PK_USER_LOAD:
		ret = PacketProc_Load(data);
		break;
	case PK_REQ_LOAD_COMPLETE:
		ret = g_pGame->ReqLoadComplete();
		break;
	case PK_USER_MAININFO:
		//> 20110220 EnterCriticalSection_Session�ǉ�
		g_pCriticalSection->EnterCriticalSection_Session(L'M');
		ret = PacketProc_Main(data);
		g_pCriticalSection->LeaveCriticalSection_Session();
		//< 20110220
		break;
	case PK_USER_DISCON:
		g_pCriticalSection->EnterCriticalSection_Session(L's');
		g_pGame->OnDisconnectUser(data[3]);
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case PK_USER_CONFIRMED:
		{
			OutputDebugStr(L"PK_USER_CONFIRMED");
			g_pCriticalSection->EnterCriticalSection_Session(L'D');
//			g_pGame->SetUserIndex(data[3]);
			ptype_session sess = g_pGame->GetAuthedSessionInfo(data[3]);
			if (!sess)
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				break;
			}
			memcpy(&sess->obj_state, &data[4], sizeof(E_TYPE_OBJ_STATE));
			g_pGame->GetMySessionInfo()->game_ready = 0;
			g_pGame->SetState(eGameResultRelease);
			g_pCriticalSection->LeaveCriticalSection_Session();
		break;
		}
	case PK_USER_FILEINFO:
		PacketProc_File(data);
		break;
	default:
		break;
	}
	return ret;
}

DWORD __stdcall Thread_PacketReceiver(PVOID param)
{
	BOOL bAuth = FALSE;
	const int c_nPacketRecvCount = 8;
	const int c_nPacketStockCount = 64;
	BYTE message[MAX_PACKET_SIZE*c_nPacketRecvCount];
	BYTE messa[c_nPacketStockCount][MAX_PACKET_SIZE];
//	BYTE msg[MAX_PACKET_SIZE];
	WORD rPsize[c_nPacketStockCount];
	int nPacketProcIndex = 0;
	int nPacketStockIndex = 0;
	BOOL bStock = FALSE;

	int *pSockNo = (int*)(param);
	int	nSockNo = *pSockNo;
	fd_set	rfds, efds;

	unsigned short	nEnd = PEND,rEnd;
	int nDivIndex = 0;
	int nRecvStockSize = 0;
	int packetCount = 0; 

	if (!nSockNo)
		return 0;
	g_bCloseSocket = FALSE;

	while(TRUE)
	{
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		FD_SET(nSockNo, &rfds);

		memset(message,0,64);
		FD_ZERO(&rfds);
		int maxfds = nSockNo+1;
		FD_ZERO(&efds);
	//		FD_SET(0, &read_fds);
		FD_SET(nSockNo, &rfds);

		if(select(nSockNo+1, &rfds, NULL, &efds, (struct timeval *)0) < 0)
		{
			AddMessageLog(L"select:closesocket");
			closesocket(nSockNo);
#ifndef _DEBUG
			g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
			if (g_pGame->GetState() & (GAME_STATE_LOGIN))
				MessageBox(g_hWnd, L"�ڑ����������ł��B\n�N���C�A���g���I�����܂��B", L"connect error", MB_OK);
			else
				MessageBox(g_hWnd, L"�T�[�o�Ƃ̐ڑ����ؒf����܂����B\n�N���C�A���g���I�����܂��B", L"connect error", MB_OK);
#endif
			g_bCloseSocket = TRUE;
			g_pGame->SetState( (EGameState)(((DWORD)g_pGame->GetState()&GAME_STATE_PHASE_MASK)|GAME_STATE_RELEASE));
//			CloseWindow(g_hWnd);
//> 20110301
//			DestroyWindow(g_hWnd);
//< 20110301
//			DXUTShutdown(DXUTGetExitCode());
			return 0;
		}

		if (FD_ISSET(nSockNo, &rfds))
		{
			int recv_size=0;
//			if ((recv_size = recv(nSockNo, (char*)&message[0], MAX_PACKET_SIZE,0)) > 0)
			if (recvall(nSockNo, (char*)message, &recv_size, sizeof(message)-nRecvStockSize))
			{
				if (recv_size == 0)
				{
					AddMessageLog(L"�T�C�Y0Byte��M");
					continue;
				}

				// �X�g�b�N��0�Ȃ�T�C�Y�擾
				bStock = (nRecvStockSize>0);
				if (!bStock)
					memcpy(&rPsize[nPacketStockIndex],message,2);

				packetCount = 0;
				// �p�P�b�g�X�g�b�N���ĂȂ�
				if (!bStock)
				{
					// 1�p�P�b�g�̂ݎ�M
					if( recv_size==rPsize[nPacketStockIndex])
					{
						memset(messa[nPacketStockIndex],0,MAX_PACKET_SIZE);
						memcpy(messa[nPacketStockIndex],message,rPsize[nPacketStockIndex]);
						memcpy(&rEnd,&messa[nPacketStockIndex][rPsize[nPacketStockIndex] -2],2);
						if(rEnd != nEnd)	// �p�P�b�g�����R�[�h�`�F�b�N
						{
							WCHAR msglog[64];
							SafePrintf(msglog, 64, L"�p�P�b�g�����ɃG���[...size=%d\n", (int)rPsize[nPacketProcIndex]);
							AddMessageLog(msglog);
							continue;
						}
						nRecvStockSize = 0;	// �X�g�b�N�Ȃ�
						packetCount=1;			// 1�p�P�b�g
						nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
					}
					else
					{
						packetCount=0;
						nDivIndex=0;
						nRecvStockSize = recv_size;
						for(;;)
						{
							if(nRecvStockSize <= 0)	break;
							if (rPsize[nPacketStockIndex] > nRecvStockSize)	// ��M�o�b�t�@���p�P�b�g���傫��
							{
								// ��M�o�b�t�@��擪�Ɉړ�����K�v������
								memset(messa[nPacketStockIndex],0,MAX_PACKET_SIZE);
								memcpy(messa[nPacketStockIndex], &message[nDivIndex], nRecvStockSize);
								break;
							}
							memset(messa[nPacketStockIndex],0,MAX_PACKET_SIZE);
							memcpy(messa[nPacketStockIndex],&message[nDivIndex],rPsize[nPacketStockIndex]);
							memcpy(&rEnd,&messa[nPacketStockIndex][ (int)(rPsize[nPacketStockIndex] -2)],2);
							if(rEnd != nEnd)	// �p�P�b�g�����R�[�h�`�F�b�N
							{
								WCHAR msglog[MAX_MSG_BUFFER*4+1];
								SafePrintf(msglog, MAX_MSG_BUFFER*4+1, L"�p�P�b�g�����ɃG���[(�������G���[)...size=%d/index=%d/stock=%d", rPsize[nPacketStockIndex],nDivIndex,nRecvStockSize);
								AddMessageLog(msglog);
								g_pGame->GenerateDatMsg(msglog, (BYTE*)messa[nPacketStockIndex], rPsize[nPacketStockIndex]);
								AddMessageLog(msglog);
								g_pGame->GenerateDatMsg(msglog, (BYTE*)&message[nDivIndex], nRecvStockSize);
								AddMessageLog(msglog);
								nRecvStockSize = 0;
								break;
							}
							nDivIndex += rPsize[nPacketStockIndex];
							nRecvStockSize -= rPsize[nPacketStockIndex];
							packetCount++;
							nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
							// ���̃p�P�b�g�T�C�Y
							memcpy(&rPsize[nPacketStockIndex],&message[nDivIndex],2);
						}
					}
				}
				else	// �p�P�b�g�X�g�b�N��
				{
					// �X�g�b�N+��M�T�C�Y��1�p�P�b�g��
					if( (nRecvStockSize+recv_size)==rPsize[nPacketStockIndex])
					{
						memcpy(&messa[nPacketStockIndex][nRecvStockSize],message,rPsize[nPacketStockIndex]-nRecvStockSize);
						memcpy(&rEnd,&messa[nPacketStockIndex][rPsize[nPacketStockIndex] -2],2);
						if(rEnd != nEnd)	// �p�P�b�g�����R�[�h�`�F�b�N
						{
							WCHAR msglog[64];
							SafePrintf(msglog, 64, L"�p�P�b�g�����ɃG���[...size=%d\n", (int)rPsize[nPacketProcIndex]);
							AddMessageLog(msglog);
							continue;
						}
						nRecvStockSize = 0;	// �X�g�b�N�Ȃ�
						packetCount=1;			// 1�p�P�b�g
						nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
					}
					else	// �p�P�b�g����
					{
						packetCount=0;
						nDivIndex=0;
						int nRecvStockIndex = nRecvStockSize;
						nRecvStockSize = recv_size;
						for (;;)
						{
							if(nRecvStockSize <= 0)	break;
							if ((rPsize[nPacketStockIndex]-nRecvStockIndex) > (nRecvStockSize))	// ��M�o�b�t�@���p�P�b�g���傫��
							{
								// ��M�o�b�t�@��擪�Ɉړ�����K�v������
								memcpy(&messa[nPacketStockIndex][nRecvStockIndex], &message[nDivIndex], nRecvStockSize);
								nRecvStockSize += nRecvStockIndex;
								break;
							}
//							memset(messa[nPacketStockIndex],0,MAX_PACKET_SIZE);
							memcpy(&messa[nPacketStockIndex][nRecvStockIndex],&message[nDivIndex],rPsize[nPacketStockIndex]-nRecvStockIndex);
							memcpy(&rEnd,&messa[nPacketStockIndex][ (int)(rPsize[nPacketStockIndex] -2)],2);
							if(rEnd != nEnd)	// �p�P�b�g�����R�[�h�`�F�b�N
							{
								WCHAR msglog[MAX_MSG_BUFFER*4+1];
								SafePrintf(msglog, MAX_MSG_BUFFER*4+1, L"�p�P�b�g�����ɃG���[(�������G���[)...size=%d/index=%d/stock=%d", rPsize[nPacketStockIndex],nDivIndex,nRecvStockSize);
								AddMessageLog(msglog);
								g_pGame->GenerateDatMsg(msglog, (BYTE*)messa[nPacketStockIndex], rPsize[nPacketStockIndex]);
								AddMessageLog(msglog);
								g_pGame->GenerateDatMsg(msglog, (BYTE*)&message[nDivIndex], nRecvStockSize);
								AddMessageLog(msglog);
								nRecvStockSize = 0;
								break;
							}
							nDivIndex += rPsize[nPacketStockIndex];
							nRecvStockIndex = 0;
							nRecvStockSize -= rPsize[nPacketStockIndex];
							packetCount++;
							nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
							// ���̃p�P�b�g�T�C�Y
							memcpy(&rPsize[nPacketStockIndex],&message[nDivIndex],2);
						}
					}
				}

				for(int j=0;j<packetCount;j++)
//				for(int j=0;j<c_nPacketProcCount;j++)
				{
//					if (nPacketStockIndex == nPacketProcIndex)
//						break;
///					WCHAR msglog[64];
///					SafePrintf(msglog, 64, L"nPacketProcIndex=%d", (int)nPacketProcIndex);
///					AddMessageLog(msglog);
//					memcpy(&msg[0],&messa[nPacketProcIndex][0],rPsize[nPacketProcIndex]);
//					RecvPacketProc((BYTE*)&msg[0]);
					RecvPacketProc((BYTE*)&messa[nPacketProcIndex][0]);
					nPacketProcIndex = (nPacketProcIndex+1)%c_nPacketStockCount;
//					Sleep(1);
				}
			}
			else
			{
				AddMessageLog(L"recv:closesocket");
//#ifndef _DEBUG
				g_pGame->PlaySysSoundSE(SE_sai_SrvInfo);
				if (g_pGame->GetState() & (GAME_STATE_LOGIN|GAME_STATE_TITLE))
					MessageBox(g_hWnd, L"�ڑ��l���������ł��B\n�N���C�A���g���I�����܂��B", L"connect error", MB_OK);
				else
					MessageBox(g_hWnd, L"�T�[�o�Ƃ̐ڑ����ؒf����܂����B\n�N���C�A���g���I�����܂��B", L"connect error", MB_OK);
//#endif
				
				g_bCloseSocket = TRUE;
				g_pGame->SetState( (EGameState)(((DWORD)g_pGame->GetState()&GAME_STATE_PHASE_MASK)|GAME_STATE_RELEASE));
//				CloseWindow(g_hWnd);
//> 20110301
//				DestroyWindow(g_hWnd);
//< 20110301
//				DXUTShutdown(DXUTGetExitCode()); return 0;
			}
		}

	}
	return 1;
}


BOOL recvall(int sock, char* pkt, int* recvsize, int bufsize)
{
	int nBufferSize = bufsize;
	*recvsize=0;
	int nRecvd = 0;
	//> �p�P�b�g��M
	while (SOCKET_ERROR != (nRecvd = recv(sock, &pkt[*recvsize], nBufferSize, 0)))
	{
		// �\�P�b�g������ꂽ
		if (!nRecvd)
		{
			if (g_bDebug)
			{
				DWORD dwErrCode = WSAGetLastError();
				WCHAR pc[16];
				SafePrintf(pc, 16, L"LastErr:%d", dwErrCode);
				AddMessageLog(pc);
			}
			return FALSE;
		}
//		// ��M0
//		if (!nRecvd)
//			break;
		// ��M�c�肪����
		*recvsize += nRecvd;
		nBufferSize -= nRecvd;
	}
	DWORD dwErrCode = WSAGetLastError();
	// �G���[
	if(dwErrCode && WSAEWOULDBLOCK != dwErrCode)
	{
#if ADD_WSAERROR_LOG
		if (g_bDebug)
		{
			WCHAR pc[16];
			SafePrintf(pc, 16, L"LastErr:%d", dwErrCode);
			AddMessageLog(pc);
		}
#endif
		return FALSE;
	}
	//< �p�P�b�g��M
	return TRUE;
}

// �`���b�g�p�P�b�g����
BOOL PacketProc_Chat(BYTE *data)
{
// PK_USER_CHAT
// size				: 2 0
// header			: 1 2
// chat_header	: 1 3
// obj_no			: 1 4
// name_len		: 1 5
// name			: name_len 6
// message_len	: 1 name_len+1
// message		: message_len name_len+2
	WCHAR addmsg[MAX_CHAT_MSG+MAX_USER_NAME+3];
	WCHAR name[MAX_USER_NAME+1];
	WCHAR chatmsg[MAX_CHAT_MSG+1];
	int nCharaIndex = data[4];
	int nIndex = 5;
	int nNameLen = DecodeSizeStrFromRaw(name, &data[nIndex], (MAX_USER_NAME+1)*sizeof(WCHAR), (MAX_USER_NAME+1)*sizeof(WCHAR));
	name[nNameLen/sizeof(WCHAR)] = NULL;	
	nIndex += nNameLen+1;
	int nChatLen = DecodeSizeStrFromRaw(chatmsg, &data[nIndex], (MAX_CHAT_MSG+1)*sizeof(WCHAR), (MAX_CHAT_MSG+1)*sizeof(WCHAR));
	chatmsg[nChatLen/sizeof(WCHAR)] = NULL;	
	nIndex += nChatLen+1;

	if (!nChatLen)
	{
		AddMessageLog(L"�����ȃ`���b�g�p�P�b�g����M");
		return FALSE;
	}
//		swprintf_s(addmsg, nNameLen+nChatLen+3, L"%s: %s", name, chatmsg);
	SafePrintf(addmsg, (nNameLen+nChatLen) / sizeof(WCHAR) + 3, L"%s: %s", name, chatmsg);

	switch ( (E_TYPE_PACKET_CHAT_HEADER) data[3] )
	{
	case PK_USER_CHAT_ALL:
		g_pGame->AddChatMessage(addmsg, PK_USER_CHAT_ALL);
		g_pGame->PlaySysSoundSE(SE_sai_Chat);
		break;
	case PK_USER_CHAT_TEAM:
		g_pGame->AddChatMessage(addmsg, PK_USER_CHAT_TEAM);
		g_pGame->PlaySysSoundSE(SE_sai_Chat);
		break;
	case PK_USER_CHAT_SERVER_INFO:
		g_pGame->AddChatMessage(addmsg, PK_USER_CHAT_SERVER_INFO);
		break;
	case PK_USER_CHAT_SERVER_WARNING:
		g_pGame->AddChatMessage(addmsg, PK_USER_CHAT_SERVER_WARNING);
		break;
	default:
	case PK_USER_CHAT_WIS:
		g_pGame->AddChatMessage(addmsg, PK_USER_CHAT_WIS);
		g_pGame->PlaySysSoundSE(SE_sai_Chat);
		break;
	}

	g_pGame->CharacterSay(nCharaIndex, chatmsg);

	return TRUE;
}

BOOL PacketProc_Load(BYTE* data)
{
	//> pakcet
	// size			:	2
	// header		:	1
	// teams		:	1
	// stage			:	1
	// rule			:	1
	// user_count:	users
	//> users
	// obj_no		:	1
	// chara_type	:	1
	// team_no	:	1
	//< users
	//	msg			:	msg_len
	int nIndex = 3;
	// teams
	g_pGame->SetTeamCount((int)data[nIndex]);
	nIndex++;
	// stage
	g_pGame->SetStageIndex((int)data[nIndex]);
	nIndex++;
	// rule
	g_pGame->SetRule(data[nIndex]);
	nIndex++;

	int nAuthedUserCount = data[nIndex];
	nIndex++;
	for (int i=0;i<nAuthedUserCount;i++)
	{
		int nUserIndex = data[nIndex];
		nIndex++;
		ptype_session sess = g_pGame->GetAuthedSessionInfo(nUserIndex);
		if (!sess)	continue;
		if (sess->chara_type == ROOM_CHARA_RANDOM_ID)
		{
			sess->chara_type = data[nIndex];
			if (nUserIndex == g_pGame->GetUserIndex())
			{
#if RTRIAL
				g_bCloseSocket = TRUE;	// �I��
#else
				g_pGame->SetChacterSelectInfo(sess->chara_type);
#endif
			}

			g_pGame->UpdateRoomCharacter(nUserIndex);
			g_pGame->UpdateUserList(nUserIndex);
		}
		else
			sess->chara_type = data[nIndex];
		nIndex++;
		sess->team_no = data[nIndex];
		nIndex++;
		sess->obj_state = OBJ_STATE_LOADING;

		sess->live_count = 0;
		// �ُ��ԂȂǃN���A
		ZeroMemory(sess->chara_state, sizeof(char)* CHARA_STATE_COUNT);
	}
	BOOL ret = g_pGame->LoadStageCharacter();
	g_pGame->SetState(eGameRoomRelease);
	return ret;
}
