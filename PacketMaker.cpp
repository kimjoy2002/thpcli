#include "PacketMaker.h"

// ACK�p�P�b�g
// msg	: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_ACK(BYTE* msg)
{
	int datIndex = 2;
	if (!msg)	return 0;

	datIndex += SetByteData(&msg[datIndex], PK_ACK);
	// 1
	datIndex += SetByteData(&msg[datIndex], 1);
	// 1
	datIndex += SetDwordData(&msg[datIndex], 1);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> �F�ؗv���p�P�b�g 
INT PacketMaker::MakePacketData_Authentication(WCHAR* name, WCHAR* pass, BYTE* msg)
{
	int datIndex = 2;
	if (!msg || !name)	return 0;
	int nNameLen = (_tcslen(name)) * sizeof(WCHAR);
	int nPassLen = (_tcslen(pass)) * sizeof(WCHAR);

	datIndex += SetByteData(&msg[datIndex], PK_USER_AUTH);
	// �L��������
	datIndex += SetByteData(&msg[datIndex], nNameLen);
	// �L������
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)name, nNameLen, MAX_USER_NAME*sizeof(WCHAR));
	// �p�X���[�h��
	datIndex += SetByteData(&msg[datIndex], nPassLen);
	// �p�X���[�h
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)pass, nPassLen, MAX_SRV_PASS*sizeof(WCHAR));
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< �F�ؗv���p�P�b�g


//> �`���b�g�p�P�b�g 
INT PacketMaker::MakePacketData_ChatMessage(BYTE type, WCHAR* message, BYTE* data)
{
	int datIndex = 2;
	if (!data || !message)	return 0;

	int nMessageLen = _tcslen(message)*sizeof(WCHAR);
	
	datIndex += SetByteData(&data[datIndex], PK_USER_CHAT);
	// ���M�͈�
	datIndex += SetByteData(&data[datIndex], type);
	// ���b�Z�[�W��
	datIndex += SetByteData(&data[datIndex], (BYTE)nMessageLen);
	// ���b�Z�[�W
	datIndex += SetMultiByteData(&data[datIndex], (BYTE*)message, nMessageLen, MAX_CHAT_MSG*sizeof(WCHAR));

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< �`���b�g�p�P�b�g 

//>> �`�[�������_�����R�}���h�p�P�b�g
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_TeamRandomize(BYTE* data, BYTE teams)
{
	int datIndex = 2;
	if (!data)	return 0;
	datIndex += SetByteData(&data[datIndex], PK_REQ_TEAM_RAND);
	datIndex += SetByteData(&data[datIndex], teams);	// extra data
	
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//<< �`�[�������_�����R�}���h�p�P�b�g


//> �����ړ��p�P�b�g
// PK_USER_ROOMINFO_MOVE
// size			: 2	0
// header		: 1	2
// mv_x			: 1	3	(char)
// mv_y			: 1	4	(char)
// footer		: 2	6
INT PacketMaker::MakePacketData_RoomInfoMove(short vx, short vy, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_MV);
	// �ړ��lX
	datIndex += SetShortData(&data[datIndex], vx);
	// �ړ��lY
	datIndex += SetShortData(&data[datIndex], vy);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< �����ړ��p�P�b�g

#if RTRIAL
#else
//> �L�����I���p�P�b�g
// id					: �L����ID
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomCharacterSelect(BYTE id, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_CHARA_SEL);
	// �L����ID
	datIndex += SetByteData(&data[datIndex], id);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;

}
//< �L�����I���p�P�b�g
#endif
//> ����OK�p�P�b�g
// b					: ������ԕύX�v��
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomReady(BOOL b, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_READY);
	// �ړ��lX
	datIndex += SetByteData(&data[datIndex], (BYTE)b);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< ����OK�p�P�b�g

//> �A�C�e���I���p�P�b�g
// index				: �A�C�e���g�p���C���f�b�N�X
// item_flg			: �A�C�e��
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomSelectItem(int index, DWORD item_flg, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_ITEM_SEL);
	// �A�C�e���g�p���C���f�b�N�X
	datIndex += SetByteData(&data[datIndex], (BYTE)index);
	// �A�C�e���g�p���C���f�b�N�X
	datIndex += SetDwordData(&data[datIndex], item_flg);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< �A�C�e���I���p�P�b�g

//> �`�[���ݒ�p�P�b�g
// team_count	: �`�[�����ύX�l
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomTeamCount(int team_count, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_TEAM_COUNT);
	// �A�C�e���g�p���C���f�b�N�X
	datIndex += SetByteData(&data[datIndex], (BYTE)team_count);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< �`�[���ݒ�p�P�b�g

//> ���[���ݒ�p�P�b�g
// rule_flg			: ���[���ύX�t���O
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomRule(BYTE flg, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_RULE);
	// �A�C�e���g�p���C���f�b�N�X
	datIndex += SetByteData(&data[datIndex], flg);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< ���[���ݒ�p�P�b�g

//> ���[���i�����^�[�����j�ݒ�p�P�b�g
// turn				: �����^�[����
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomRuleTurnLimit(int turn, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_RULE_TURN_LIMIT);
	// �����^�[����
	datIndex += SetWordData(&data[datIndex], (short)turn);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< ���[���ݒ�p�P�b�g

//> �������Ԑݒ�p�P�b�g
INT PacketMaker::MakePacketData_RoomRuleActTimeLimit(int obj_no, int time, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_RULE_ACT_TIME_LIMIT);
	// obj_no
	datIndex += SetByteData(&data[datIndex], (BYTE)obj_no);
	// �����^�[����
	datIndex += SetByteData(&data[datIndex], (BYTE)time);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< �������Ԑݒ�p�P�b�g

//> �X�e�[�W�I���p�P�b�g
// stage_index	: �X�e�[�W�C���f�b�N�X�l
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_RoomSelectStage(int stage_index, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_STAGE_SEL);
	// �X�e�[�W�C���f�b�N�X�l
	datIndex += SetByteData(&data[datIndex], stage_index);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< �X�e�[�W�I���p�P�b�g

//> ���[�h�����p�P�b�g
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_LoadComplete(BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_LOAD_COMPLETE);
	// 1
	datIndex += SetByteData(&data[datIndex], 1);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//> ���[�h�����p�P�b�g

//> �X�e�[�W�ړ��p�P�b�g
// vx					: �ړ��lx
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoMove(short vx, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_MV);
	// �ړ��lX
	datIndex += SetShortData(&data[datIndex], vx);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//> �X�e�[�W�ړ��p�P�b�g

//> �X�V�p�P�b�g
// vx				: �ړ��lx
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoFlip(E_TYPE_USER_DIRECTION dir, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_FLIP);
	// ����
	datIndex += SetByteData(&data[datIndex], dir);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//> �X�e�[�W�ړ��p�P�b�g

//> ���˃p�P�b�g
// angle				: �p�x
// power			: ����
// blt_type			: �e�̎��
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoShot(short angle, int power, int blt_type, int proc_type, int indicatorAngle, int indicatorPower, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_SHOT);
	// �p�x
	datIndex += SetShortData(&data[datIndex], angle);
	// 
	datIndex += SetByteData(&data[datIndex], (BYTE)power);
	// �e�̎��
	datIndex += SetByteData(&data[datIndex], (BYTE)blt_type);
	// �����̎��
	datIndex += SetByteData(&data[datIndex], (BYTE)proc_type);
	// �C���W�P�[�^�[�p�x
	datIndex += SetShortData(&data[datIndex], (short)indicatorAngle);
	// �C���W�P�[�^�[�p���[
	datIndex += SetByteData(&data[datIndex], (BYTE)indicatorPower);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< ���˃p�P�b�g

//> �^�[���p�X�p�P�b�g
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoTurnPass(int obj_no, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_TURN_PASS);
	// 1
	datIndex += SetShortData(&data[datIndex], (short)obj_no);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< �^�[���p�X�p�P�b�g

// �A�C�e���g�p�v���p�P�b�g
// item_index
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoItemOrder(int item_index, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_ITEM);
	// item index
	datIndex += SetByteData(&data[datIndex], (BYTE)item_index);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// �g���K�[�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoTrigger(ptype_session sess, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_TRIGGER);
	// �L�����ԍ�
	datIndex += SetShortData(&data[datIndex], sess->obj_no );
	// �X�N���v�g/�A�C�e���̃^�C�v
	datIndex += SetByteData(&data[datIndex], (BYTE)nProcType);
	// ���o�̃^�C�v
	datIndex += SetByteData(&data[datIndex], (BYTE)nBltType);
	// �p�x
	datIndex += SetShortData(&data[datIndex], (short)nShotAngle);
	// �p���[
	datIndex += SetShortData(&data[datIndex], (short)nShotPower);
	// indicator�p�x
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorAngle);
	// indicator�p���[
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorPower);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// �g���K�[�I��
INT PacketMaker::MakePacketData_MainInfoTriggerEnd(ptype_session sess, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_USER_MAIN_TRIGGER_END);
	// �L�����ԍ�
	datIndex += SetShortData(&data[datIndex], sess->obj_no );
	// �X�N���v�g/�A�C�e���̃^�C�v
	datIndex += SetByteData(&data[datIndex], (BYTE)nProcType);
	// ���o�̃^�C�v
	datIndex += SetByteData(&data[datIndex], (BYTE)nBltType);
	// �p�x
	datIndex += SetShortData(&data[datIndex], (short)nShotAngle);
	// �p���[
	datIndex += SetShortData(&data[datIndex], (short)nShotPower);
	// indicator�p�x
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorAngle);
	// indicator�p���[
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorPower);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// �V���b�g�p���[�X�^�[�g�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoShotPowerStart(ptype_session sess, int nProcType, int nBltType, int nShotAngle, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_SHOTPOWER);
	// �L�����ԍ�
	datIndex += SetShortData(&data[datIndex], sess->obj_no );
	// �X�N���v�g/�A�C�e���̃^�C�v
	datIndex += SetByteData(&data[datIndex], (BYTE)nProcType);
	// ���o�̃^�C�v
	datIndex += SetByteData(&data[datIndex], (BYTE)nBltType);
	// �p�x
	datIndex += SetShortData(&data[datIndex], (short)nShotAngle);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

//> ���ʊm�F�p�P�b�g
// data				: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_Confirm(int nCharaIndex, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CMD_CONFIRM);
	// chara index
	datIndex += SetByteData(&data[datIndex], (BYTE)nCharaIndex);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< ���ʊm�F�p�P�b�g

// �n�b�V���l��Ԃ��p�P�b�g�p�P�b�g
INT PacketMaker::MakePacketData_AuthRetHash(int hash_group, int hash_id, const char* code, int code_size, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_RET_HASH);
	// hash_group
	datIndex += SetByteData(&data[datIndex], (BYTE)hash_group);
	// hash_id
	datIndex += SetWordData(&data[datIndex], (WORD)hash_id);
	// hash_code_size
	datIndex += SetByteData(&data[datIndex], (BYTE)code_size);
	// hash_code
	datIndex += SetMultiByteData(&data[datIndex], (BYTE*)code, code_size, MAX_PACKET_SIZE);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// �t�@�C���v���p�P�b�g
INT PacketMaker::MakePacketData_ReqFileData(BOOL bScrChara, int id, int no, int file_index, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_REQ_FILE_DATA);
	// chara
	datIndex += SetByteData(&data[datIndex], (BYTE)bScrChara);
	// id
	datIndex += SetWordData(&data[datIndex], (WORD)id);
	// fileno
	datIndex += SetByteData(&data[datIndex], (BYTE)no);
	// file_index
	datIndex += SetDwordData(&data[datIndex], (DWORD)file_index);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// �t�@�C���v���p�P�b�g
INT PacketMaker::MakePacketData_FileHash(BOOL bScrChara, std::map < int, TCHARA_SCR_INFO > *pCharaScrInfo, std::map <int, TSTAGE_SCR_INFO> *pStageScrInfo, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_CHK_FILE_HASH);
	// chara count
	datIndex += SetWordData(&data[datIndex], (WORD)pCharaScrInfo->size());

	std::map < int, TCHARA_SCR_INFO >::iterator itCharaEnd = pCharaScrInfo->end();
	for (std::map < int, TCHARA_SCR_INFO >::iterator itChara = pCharaScrInfo->begin();
		itChara != itCharaEnd;
		++itChara)
	{
		// id
		datIndex += SetWordData(&data[datIndex], (WORD)(*itChara).second.ID);
		// hash
		BytesFromHexString(&data[datIndex], (*itChara).second.md5);
		datIndex += 16;
//		datIndex += SetMultiByteData(&data[datIndex], (BYTE*)md5.code(), len, MAX_PACKET_SIZE);
	}

	// stage count
	datIndex += SetWordData(&data[datIndex], (WORD)pStageScrInfo->size());

	std::map < int, TSTAGE_SCR_INFO >::iterator itStageEnd = pStageScrInfo->end();
	for (std::map < int, TSTAGE_SCR_INFO >::iterator itStage = pStageScrInfo->begin();
		itStage != itStageEnd;
		++itStage)
	{
		// id
		datIndex += SetWordData(&data[datIndex], (WORD)(*itStage).second.ID);
		// hash
		BytesFromHexString(&data[datIndex], (*itStage).second.md5);
		datIndex += 16;
//		datIndex += SetMultiByteData(&data[datIndex], (BYTE*)md5.code(), len, MAX_PACKET_SIZE);
	}
	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);
	return datIndex;
}

// �t�@�C���v���p�P�b�g
INT PacketMaker::MakePacketData_ReqFileHash(BOOL bScrChara, int id, int no, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// �w�b�_
	datIndex += SetByteData(&data[datIndex], PK_REQ_FILE_HASH);
	// chara
	datIndex += SetByteData(&data[datIndex], (BYTE)bScrChara);
	// id
	datIndex += SetWordData(&data[datIndex], (WORD)id);
	// fileno
	datIndex += SetByteData(&data[datIndex], (BYTE)no);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
