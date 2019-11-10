#include "PacketMaker.h"

// ACKパケット
// msg	: out作成パケット
INT PacketMaker::MakePacketData_ACK(BYTE* msg)
{
	int datIndex = 2;
	if (!msg)	return 0;

	datIndex += SetByteData(&msg[datIndex], PK_ACK);
	// 1
	datIndex += SetByteData(&msg[datIndex], 1);
	// 1
	datIndex += SetDwordData(&msg[datIndex], 1);
	// エンド??カ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> 認証要求パケット 
INT PacketMaker::MakePacketData_Authentication(WCHAR* name, WCHAR* pass, BYTE* msg)
{
	int datIndex = 2;
	if (!msg || !name)	return 0;
	int nNameLen = (_tcslen(name)) * sizeof(WCHAR);
	int nPassLen = (_tcslen(pass)) * sizeof(WCHAR);

	datIndex += SetByteData(&msg[datIndex], PK_USER_AUTH);
	// キャラ名長
	datIndex += SetByteData(&msg[datIndex], nNameLen);
	// キャラ名
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)name, nNameLen, MAX_USER_NAME*sizeof(WCHAR));
	// パスワ?ド長
	datIndex += SetByteData(&msg[datIndex], nPassLen);
	// パスワ?ド
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)pass, nPassLen, MAX_SRV_PASS*sizeof(WCHAR));
	// エンド??カ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< 認証要求パケット


//> ?ャットパケット 
INT PacketMaker::MakePacketData_ChatMessage(BYTE type, WCHAR* message, BYTE* data)
{
	int datIndex = 2;
	if (!data || !message)	return 0;

	int nMessageLen = _tcslen(message)*sizeof(WCHAR);
	
	datIndex += SetByteData(&data[datIndex], PK_USER_CHAT);
	// 送信範囲
	datIndex += SetByteData(&data[datIndex], type);
	// メッセ?ジ長
	datIndex += SetByteData(&data[datIndex], (BYTE)nMessageLen);
	// メッセ?ジ
	datIndex += SetMultiByteData(&data[datIndex], (BYTE*)message, nMessageLen, MAX_CHAT_MSG*sizeof(WCHAR));

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< ?ャットパケット 

//>> ???ラン??化コ?ンドパケット
// data				: out作成パケット
INT PacketMaker::MakePacketData_TeamRandomize(BYTE* data, BYTE teams)
{
	int datIndex = 2;
	if (!data)	return 0;
	datIndex += SetByteData(&data[datIndex], PK_REQ_TEAM_RAND);
	datIndex += SetByteData(&data[datIndex], teams);	// extra data
	
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//<< ???ラン??化コ?ンドパケット


//> 部屋移動パケット
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

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_MV);
	// 移動値X
	datIndex += SetShortData(&data[datIndex], vx);
	// 移動値Y
	datIndex += SetShortData(&data[datIndex], vy);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< 部屋移動パケット

#if RTRIAL
#else
//> キャラ選択パケット
// id					: キャラID
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomCharacterSelect(BYTE id, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_CHARA_SEL);
	// キャラID
	datIndex += SetByteData(&data[datIndex], id);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;

}
//< キャラ選択パケット
#endif
//> ?備OKパケット
// b					: ?備状態変更要求
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomReady(BOOL b, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_READY);
	// 移動値X
	datIndex += SetByteData(&data[datIndex], (BYTE)b);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< ?備OKパケット

//> アイテ?選択パケット
// index				: アイテ?使用欄インデックス
// item_flg			: アイテ?
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomSelectItem(int index, DWORD item_flg, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_ITEM_SEL);
	// アイテ?使用欄インデックス
	datIndex += SetByteData(&data[datIndex], (BYTE)index);
	// アイテ?使用欄インデックス
	datIndex += SetDwordData(&data[datIndex], item_flg);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< アイテ?選択パケット

//> ???設定パケット
// team_count	: ???数変更値
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomTeamCount(int team_count, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_TEAM_COUNT);
	// アイテ?使用欄インデックス
	datIndex += SetByteData(&data[datIndex], (BYTE)team_count);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< ???設定パケット

//> ル?ル設定パケット
// rule_flg			: ル?ル変更フラグ
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomRule(BYTE flg, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_RULE);
	// アイテ?使用欄インデックス
	datIndex += SetByteData(&data[datIndex], flg);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< ル?ル設定パケット

//> ル?ル（制限??ン数）設定パケット
// turn				: 制限??ン数
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomRuleTurnLimit(int turn, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_RULE_TURN_LIMIT);
	// 制限??ン数
	datIndex += SetWordData(&data[datIndex], (short)turn);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< ル?ル設定パケット

//> 制限時間設定パケット
INT PacketMaker::MakePacketData_RoomRuleActTimeLimit(int obj_no, int time, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_RULE_ACT_TIME_LIMIT);
	// obj_no
	datIndex += SetByteData(&data[datIndex], (BYTE)obj_no);
	// 制限??ン数
	datIndex += SetByteData(&data[datIndex], (BYTE)time);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< 制限時間設定パケット

//> ステ?ジ選択パケット
// stage_index	: ステ?ジインデックス値
// data				: out作成パケット
INT PacketMaker::MakePacketData_RoomSelectStage(int stage_index, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_ROOM_STAGE_SEL);
	// ステ?ジインデックス値
	datIndex += SetByteData(&data[datIndex], stage_index);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//< ステ?ジ選択パケット

//> ロ?ド完了パケット
// data				: out作成パケット
INT PacketMaker::MakePacketData_LoadComplete(BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_LOAD_COMPLETE);
	// 1
	datIndex += SetByteData(&data[datIndex], 1);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;	
}
//> ロ?ド完了パケット

//> ステ?ジ移動パケット
// vx					: 移動値x
// data				: out作成パケット
INT PacketMaker::MakePacketData_MainInfoMove(short vx, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_MV);
	// 移動値X
	datIndex += SetShortData(&data[datIndex], vx);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//> ステ?ジ移動パケット

//> 更新パケット
// vx				: 移動値x
// data				: out作成パケット
INT PacketMaker::MakePacketData_MainInfoFlip(E_TYPE_USER_DIRECTION dir, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_FLIP);
	// 向き
	datIndex += SetByteData(&data[datIndex], dir);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//> ステ?ジ移動パケット

//> 発射パケット
// angle				: 角度
// power			: 初速
// blt_type			: 弾の種類
// data				: out作成パケット
INT PacketMaker::MakePacketData_MainInfoShot(short angle, int power, int blt_type, int proc_type, int indicatorAngle, int indicatorPower, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_SHOT);
	// 角度
	datIndex += SetShortData(&data[datIndex], angle);
	// 
	datIndex += SetByteData(&data[datIndex], (BYTE)power);
	// 弾の種類
	datIndex += SetByteData(&data[datIndex], (BYTE)blt_type);
	// 処理の種類
	datIndex += SetByteData(&data[datIndex], (BYTE)proc_type);
	// インジケ???角度
	datIndex += SetShortData(&data[datIndex], (short)indicatorAngle);
	// インジケ???パワ?
	datIndex += SetByteData(&data[datIndex], (BYTE)indicatorPower);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< 発射パケット

//> ??ンパスパケット
// data				: out作成パケット
INT PacketMaker::MakePacketData_MainInfoTurnPass(int obj_no, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_TURN_PASS);
	// 1
	datIndex += SetShortData(&data[datIndex], (short)obj_no);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< ??ンパスパケット

// アイテ?使用要求パケット
// item_index
// data				: out作成パケット
INT PacketMaker::MakePacketData_MainInfoItemOrder(int item_index, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_ITEM);
	// item index
	datIndex += SetByteData(&data[datIndex], (BYTE)item_index);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// トリガ?パケット
INT PacketMaker::MakePacketData_MainInfoTrigger(ptype_session sess, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_TRIGGER);
	// キャラ番号
	datIndex += SetShortData(&data[datIndex], sess->obj_no );
	// スクリプト/アイテ?の?イプ
	datIndex += SetByteData(&data[datIndex], (BYTE)nProcType);
	// 演出の?イプ
	datIndex += SetByteData(&data[datIndex], (BYTE)nBltType);
	// 角度
	datIndex += SetShortData(&data[datIndex], (short)nShotAngle);
	// パワ?
	datIndex += SetShortData(&data[datIndex], (short)nShotPower);
	// indicator角度
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorAngle);
	// indicatorパワ?
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorPower);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// トリガ?終了
INT PacketMaker::MakePacketData_MainInfoTriggerEnd(ptype_session sess, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_USER_MAIN_TRIGGER_END);
	// キャラ番号
	datIndex += SetShortData(&data[datIndex], sess->obj_no );
	// スクリプト/アイテ?の?イプ
	datIndex += SetByteData(&data[datIndex], (BYTE)nProcType);
	// 演出の?イプ
	datIndex += SetByteData(&data[datIndex], (BYTE)nBltType);
	// 角度
	datIndex += SetShortData(&data[datIndex], (short)nShotAngle);
	// パワ?
	datIndex += SetShortData(&data[datIndex], (short)nShotPower);
	// indicator角度
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorAngle);
	// indicatorパワ?
	datIndex += SetShortData(&data[datIndex], (short)nShotIndicatorPower);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// ショットパワ?ス??トパケット
INT PacketMaker::MakePacketData_MainInfoShotPowerStart(ptype_session sess, int nProcType, int nBltType, int nShotAngle, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_MAIN_SHOTPOWER);
	// キャラ番号
	datIndex += SetShortData(&data[datIndex], sess->obj_no );
	// スクリプト/アイテ?の?イプ
	datIndex += SetByteData(&data[datIndex], (BYTE)nProcType);
	// 演出の?イプ
	datIndex += SetByteData(&data[datIndex], (BYTE)nBltType);
	// 角度
	datIndex += SetShortData(&data[datIndex], (short)nShotAngle);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

//> 結果確認パケット
// data				: out作成パケット
INT PacketMaker::MakePacketData_Confirm(int nCharaIndex, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_CMD_CONFIRM);
	// chara index
	datIndex += SetByteData(&data[datIndex], (BYTE)nCharaIndex);
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
//< 結果確認パケット

// ハッシュ値を返すパケットパケット
INT PacketMaker::MakePacketData_AuthRetHash(int hash_group, int hash_id, const char* code, int code_size, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_RET_HASH);
	// hash_group
	datIndex += SetByteData(&data[datIndex], (BYTE)hash_group);
	// hash_id
	datIndex += SetWordData(&data[datIndex], (WORD)hash_id);
	// hash_code_size
	datIndex += SetByteData(&data[datIndex], (BYTE)code_size);
	// hash_code
	datIndex += SetMultiByteData(&data[datIndex], (BYTE*)code, code_size, MAX_PACKET_SIZE);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// フ?イル要求パケット
INT PacketMaker::MakePacketData_ReqFileData(BOOL bScrChara, int id, int no, int file_index, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_REQ_FILE_DATA);
	// chara
	datIndex += SetByteData(&data[datIndex], (BYTE)bScrChara);
	// id
	datIndex += SetWordData(&data[datIndex], (WORD)id);
	// fileno
	datIndex += SetByteData(&data[datIndex], (BYTE)no);
	// file_index
	datIndex += SetDwordData(&data[datIndex], (DWORD)file_index);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}

// フ?イル要求パケット
INT PacketMaker::MakePacketData_FileHash(BOOL bScrChara, std::map < int, TCHARA_SCR_INFO > *pCharaScrInfo, std::map <int, TSTAGE_SCR_INFO> *pStageScrInfo, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;
	// ヘッ?
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
	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);
	return datIndex;
}

// フ?イル要求パケット
INT PacketMaker::MakePacketData_ReqFileHash(BOOL bScrChara, int id, int no, BYTE* data)
{
	int datIndex = 2;
	if (!data)	return 0;

	// ヘッ?
	datIndex += SetByteData(&data[datIndex], PK_REQ_FILE_HASH);
	// chara
	datIndex += SetByteData(&data[datIndex], (BYTE)bScrChara);
	// id
	datIndex += SetWordData(&data[datIndex], (WORD)id);
	// fileno
	datIndex += SetByteData(&data[datIndex], (BYTE)no);

	// エンド??カ
	datIndex += SetEndMarker(&data[datIndex]);
	SetWordData(&data[0], datIndex);

	return datIndex;
}
