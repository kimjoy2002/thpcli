#ifndef H_PACKET_MAKER___
#define H_PACKET_MAKER___

#include "windows.h"
#include <cassert>
#include "ext.h"


namespace PacketMaker
{
	// ACK�p�P�b�g
	// msg	: out�쐬�p�P�b�g
	INT MakePacketData_ACK(BYTE* msg);

	// �F�ؗv���p�P�b�g
	// name	: ��?�U��
	// pass		: �p�X��?�h
	// msg	: out�쐬�p�P�b�g
	INT MakePacketData_Authentication(WCHAR* name, WCHAR* pass, BYTE* msg);

	// ?���b�g�p�P�b�g
	// type				: ���M�͈�
	// message		: ���b�Z?�W���e
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_ChatMessage(BYTE type, WCHAR* message, BYTE* data);

	// ???����??���R?���h�p�P�b�g
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_TeamRandomize(BYTE* data, BYTE teams);

	// �����ړ��p�P�b�g
	// vx					: �ړ��lx
	// vy					: �ړ��ly
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomInfoMove(short vx, short vy, BYTE* data);

#if RTRIAL
#else
	// �L�����I���p�P�b�g
	// id					: �L����ID
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomCharacterSelect(BYTE id, BYTE* data);
#endif
	// ?��OK�p�P�b�g
	// b					: ?����ԕύX�v��
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomReady(BOOL b, BYTE* data);

	// �A�C�e?�I���p�P�b�g
	// index				: �A�C�e?�g�p���C���f�b�N�X
	// item_flg			: �A�C�e?
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomSelectItem(int index, DWORD item_flg, BYTE* data);

	// ???�ݒ�p�P�b�g
	// team_count	: ???���ύX�l
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomTeamCount(int team_count, BYTE* data);

	// ��?���ݒ�p�P�b�g
	// rule_flg			: ��?���ύX�t���O
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomRule(BYTE flg, BYTE* data);

	// �X�e?�W�I���p�P�b�g
	// stage_index	: �X�e?�W�C���f�b�N�X�l
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_RoomSelectStage(int stage_index, BYTE* data);

	// ��?�h�����p�P�b�g
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_LoadComplete(BYTE* data);

	// �X�e?�W�ړ��p�P�b�g
	// vx					: �ړ��lx
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_MainInfoMove(short vx, BYTE* data);

	// �U������p�P�b�g
	// vx					: �ړ��lx
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_MainInfoFlip(E_TYPE_USER_DIRECTION dir, BYTE* data);

	// ���˃p�P�b�g
	// angle				: �p�x
	// power			: ����
	// blt_type			: �e�̎��
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_MainInfoShot(short angle, int power, int blt_type, int proc_type, int indicatorAngle, int indicatorPower, BYTE* data);

	// �A�C�e?�g�p�v���p�P�b�g
	// item_index
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_MainInfoItemOrder(int item_index, BYTE* data);

	// ??���p�X�p�P�b�g
	// data				: out�쐬�p�P�b�g
	INT MakePacketData_MainInfoTurnPass(int obj_no, BYTE* data);

	// �g���K?�p�P�b�g
	INT MakePacketData_MainInfoTrigger(ptype_session sess, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* data);
	
	// �g���K?�I��
	INT MakePacketData_MainInfoTriggerEnd(ptype_session sess, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* data);

	// �V���b�g�p��?�X??�g�p�P�b�g
	INT MakePacketData_MainInfoShotPowerStart(ptype_session sess, int nProcType, int nBltType, int nShotAngle, BYTE* data);

	// �n�b�V���l��Ԃ��p�P�b�g�p�P�b�g
	INT MakePacketData_FileHash(BOOL bScrChara, std::map < int, TCHARA_SCR_INFO > *pCharaScrInfo, std::map <int, TSTAGE_SCR_INFO> *pStageScrInfo, BYTE* data);
	// �n�b�V���l��Ԃ��p�P�b�g�p�P�b�g
	INT MakePacketData_AuthRetHash(int hash_group, int hash_id, const char* code, int code_size, BYTE* data);

	// ���ʊm�F�p�P�b�g
	INT MakePacketData_Confirm(int nCharaIndex, BYTE* data);

	// ��?���i����??�����j�ݒ�p�P�b�g
	INT MakePacketData_RoomRuleTurnLimit(int turn, BYTE* data);

	// ��?���i�������ԁj�ݒ�p�P�b�g
	INT MakePacketData_RoomRuleActTimeLimit(int obj_no,int time, BYTE* data);
	
	// �t?�C��
	INT MakePacketData_ReqFileData(BOOL bScrChara, int id, int no, int file_index, BYTE* data);
	// �t?�C��
	INT MakePacketData_ReqFileHash(BOOL bScrChara, int id, int no, BYTE* data);
};



#endif