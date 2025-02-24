#ifndef H_GAME___
#define H_GAME___
// ===================================================================
// Game.h
//	ゲ??管理クラス・ヘッ??
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include <io.h>
#include "util.h"
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <winsock.h>
#include <map>
#include <string>
#include <iostream>
#include <list>
#include <lua.hpp>

#include "../thpcommon/CPacketQueue.h"
#include "../thpcommon/common.h"
#include "Input.h"
#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"

#include "CIniFile.h"
#include "CDXUTButtonList.h"
#include "CTeamRulePropertyControlManager.h"
#include "CTeamSeparater.h"
#include "CShotAngle.h"
#include "CShotPowerIndicator.h"
#include "CShotAngleIndicator.h"
#include "CRoomCharacter.h"
#include "CStageCharacter.h"
#include "CStageEffect.h"
#include "CSpinButton.h"

#include "PacketMaker.h"

#include <lua.hpp>
#include "tolua++.h"
#include "tolua_glue/thg_glue.h"
#include "LuaHelper.h"

#include "Input.h"
#include "TextureLoader.h"
#include "PngLoader.h"
#include "../thpcommon/CMainStage.h"

#include "CSoundPlayer.h"
#include "CSoundLibraries.h"

#include "resource.h"

// ゲ??全体の状態
enum ESystemState {
//=========================可変部分==========================//
	eSysInit,
	eSysRun,
	eSysExit,
//===========================================================//
};

#define GAME_STATE_PHASE_MASK 0xFFF0
#define GAME_STATE_TITLE		0x0100
#define GAME_STATE_LOGIN		0x0200
#define GAME_STATE_ROOM		0x0400
#define GAME_STATE_LOAD		0x0800
#define GAME_STATE_MAIN		0x1000
#define GAME_STATE_RESULT	0x2000
#define GAME_STATE_INIT		0x0000
#define GAME_STATE_PROC		0x0001
#define GAME_STATE_RELEASE	0x0002

enum EGameAllState: DWORD {
	eGameRunTitleInit = 0,
	eGameRunTitle,
	eGameRunTitleRelease,
	eGameRenderTitle,

	eGameRunLoginInit = 10,
	eGameRunLogin,
	eGameRunLoginRelease,
	eGameRenderLogin,

	eGameRunRoomInit = 20,
	eGameRunRoom,
	eGameRunRoomRelease,
	eGameRenderRoom,

	eGameRunLoadInit = 30,
	eGameRunLoad,
	eGameRunLoadRelease,
	eGameRenderLoad,
	
	eGameRunMainInit = 40,
	eGameRunMain,
	eGameRunMainRelease,
	eGameRenderMain,

	eGameRunResultInit = 50,
	eGameRunResult	,
	eGameRunResultRelease,
	eGameRenderResult,
};

// ゲ??内部の状態
enum EGameState {
//=========================可変部分==========================//
	eGameTitleInit			= GAME_STATE_TITLE|GAME_STATE_INIT,
	eGameTitle				= GAME_STATE_TITLE|GAME_STATE_PROC,
	eGameTitleRelease	= GAME_STATE_TITLE|GAME_STATE_RELEASE,

	eGameLoginInit			= GAME_STATE_LOGIN|GAME_STATE_INIT,
	eGameLogin				= GAME_STATE_LOGIN|GAME_STATE_PROC,
	eGameLoginRelease	= GAME_STATE_LOGIN|GAME_STATE_RELEASE,

	eGameRoomInit			= GAME_STATE_ROOM|GAME_STATE_INIT,
	eGameRoom				= GAME_STATE_ROOM|GAME_STATE_PROC,
	eGameRoomRelease	= GAME_STATE_ROOM|GAME_STATE_RELEASE,

	eGameLoadInit			= GAME_STATE_LOAD|GAME_STATE_INIT,
	eGameLoad				= GAME_STATE_LOAD|GAME_STATE_PROC,
	eGameLoadRelease	= GAME_STATE_LOAD|GAME_STATE_RELEASE,
	
	eGameMainInit			= GAME_STATE_MAIN|GAME_STATE_INIT,
	eGameMain				= GAME_STATE_MAIN|GAME_STATE_PROC,
	eGameMainRelease	= GAME_STATE_MAIN|GAME_STATE_RELEASE,

	eGameResultInit		= GAME_STATE_RESULT|GAME_STATE_INIT,
	eGameResult				= GAME_STATE_RESULT|GAME_STATE_PROC,
	eGameResultRelease	= GAME_STATE_RESULT|GAME_STATE_RELEASE,

	eGameEnd,
//===========================================================//
};

// 入力ログ?造体
struct TContLog {
	WORD	Times;
	BYTE	Cont;
	BYTE	Button;
};

class CGame
{
public:
	CGame();
	~CGame();
	// 作成
	BOOL Create();
	BOOL OnCreateDevice(HWND hWnd,HINSTANCE hInst);

	BOOL Game();				// Main

	void Render(float fElapsedTime){ Render(fElapsedTime, m_pDev, m_eGameState);	};
	void Render(float fElapsedTime, LPDIRECT3DDEVICE9 pdev, EGameState eGameState);// ?画の処理

	// Input
#if USE_DINPUT
	BOOL	EnumJoysticks(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	BOOL	EnumAxes(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
#endif
	inline EGameState GetState()
	{ return m_eGameState;	};
	inline void SetState(EGameState val)
	{ m_eGameState = val;	};

#if USE_LUA
	void	SetLua(lua_State *l);
#endif

#if !USE_DINPUT
	void UpdateKeyState()
	{
		for(int i=0; i<USE_BUTTONS; i++)
			m_tKeyState.bytButtons[i] &= (0xFF^KEY_BTN_CHANGE);
	};
#endif
//========= Event =========//
	// MouseEvent
	void OnMouseLButtonDoubleClick(WORD mk, int x,int y);
	void OnMouseLButton(BOOL bDown, int x, int y);
	void OnMouseMButton(BOOL bDown, int x, int y);
	void OnMouseMove(int x,int y);

	// フォ?カス
	void	OnInactive();
	void	OnActive();

	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();

	void SetLua(lua_State* L, LuaHelper* pLH)
	{
		p_pL = L;
		p_pLuah = pLH;
	};
	void SetUI(CDXUTDialog* pUI, CDXUTDialog* pConfig)	{	p_pUI = pUI; p_pConfig = pConfig;	};
	void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

	//> セッションへ送るパケット作成
	BOOL AddPacket(BYTE* data, WORD size);
	//< セッションへ送るパケット作成
//=========================可変部分==========================//
//public関数
	BOOL ConnectServer();
	BOOL LoginServer();
	void SetUserIndex(int val)	{	m_nUserIndex = val;	};
	int GetUserIndex()	{	return m_nUserIndex;	};
	void	SetUserState(E_TYPE_OBJ_STATE val)	{	m_eUserState = val;		};
	E_TYPE_OBJ_STATE	GetUserState()	{	return m_eUserState;	};

	void CALLBACK OnKeyEvent(UINT nChar, bool bKeyDown, bool bAltDown);
	// ?ケット切断
	void Disconnect();

	BOOL IsCreated() { return m_bCreated;	};
	BOOL IsProcState() { return (BOOL)(m_eGameState & GAME_STATE_PROC); };

	E_TYPE_CONN_STATE GetConnState() { return m_eConnState; };
	void SetConnState(E_TYPE_CONN_STATE value) { m_eConnState = value; };

	//> util
	// ユ?ザ情報を取得
	inline type_session* GetAuthedSessionInfo(int nIndex)
	{
		if (nIndex >= GetMaxLoginNum() || nIndex < 0 )
		{
			DXTRACE_MSG(L"GetAuthedSessionInfo index error");
			return NULL;
		}
		if (m_SessionArray[nIndex].connect_state != CONN_STATE_AUTHED)
		{
			DXTRACE_MSG(L"GetAuthedSessionInfo connect_state != CONN_STATE_AUTHED");
			return NULL;
		}
		return &m_SessionArray[nIndex];
	};
	inline type_session* GetAuthedSessionObj(int nObjNo)
	{
		if (ptype_session sess = GetSessionObj(nObjNo))
		{		
			if (sess->connect_state != CONN_STATE_AUTHED)
			{
				DXTRACE_MSG(L"GetAuthedSessionInfo connect_state != CONN_STATE_AUTHED");
				return NULL;
			}
			return sess;
		}
		return NULL;
	};
	// ユ?ザ情報を取得(認証?ェックなし)
	inline type_session* GetSessionInfo(int nIndex)
	{
		if (nIndex >= GetMaxLoginNum() || nIndex < 0 )
		{
			DXTRACE_MSG(L"GetSessionInfo index error");
			return NULL;
		}
		return &m_SessionArray[nIndex];
	};

	inline int GetSessionIndex(int nIndex)
	{
		if (nIndex >= MAXUSERNUM || nIndex < 0)
		{
			DXTRACE_MSG(L"GetSessionIndexFromObjNo index error");
			return -1;
		}
		for (int i=0;i<GetMaxLoginNum();i++)
		{
			if (m_SessionArray[i].obj_no == nIndex)
				return i;
		}
		return -1;
	}
	inline type_session* GetSessionObj(int nObjNo)
	{
		if (nObjNo >= MAXUSERNUM || nObjNo < 0)
		{
			DXTRACE_MSG(L"GetSessionObj index error");
			return NULL;
		}
		for (int i=0;i<GetMaxLoginNum();i++)
		{
			if (m_SessionArray[i].obj_no == nObjNo)
				return &m_SessionArray[i];
		}
		return NULL;
	};
	// 自身のユ?ザ情報を取得
	inline type_session* GetMySessionInfo()
	{
		if (m_nUserIndex >= GetMaxLoginNum() || m_nUserIndex < 0)
		{
			DXTRACE_MSG(L"GetMySessionInfo index error");
			return NULL;
		}
		return &m_SessionArray[m_nUserIndex];
	};
	inline int CalcAuthedUserCount()
	{
		int nRet = 0;
		for (int i=0;i<GetMaxLoginNum();i++)
		{
			if (m_SessionArray[i].connect_state == CONN_STATE_AUTHED)
				nRet++;
		}
		return nRet;
	}
	//< util

	// control
	void CALLBACK OnConfigEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

	// Disconnect
	void OnDisconnectUser(int nCharaIndex);
	//> Chat
	void AddChatMessage(WCHAR *msg, E_TYPE_PACKET_CHAT_HEADER chat);
	//< Chat

	//> Auth
	WCHAR* GetScrName(int id, BOOL bCharaScr);
	std::map<int, TCHARA_SCR_INFO>* GetMapCharaScrInfo()
	{ return &m_mapCharaScrInfo;	};
	std::map<int, TSTAGE_SCR_INFO>* GetMapStageScrInfo()
	{ return &m_mapStageScrInfo;	};
	void DeleteRecvScrFile();
	void SetRecvScrFile(WCHAR* path);
	//< Auth

	//> Login
	void SuccessAuth(BYTE* data);
	BOOL ReqPacketHash(BYTE* data);
	BOOL InitControl();
	BOOL m_bLoginStart;
	//< Login

	//> Room
	void SetNewUser(type_session* sess, BOOL bAddLog);
	void SetChacterSelectInfo(int nObjType);
	void SetChacterSelectInfoRandom();
	void SetItemIconButtonList();
	void UpdateUserList(int nIndex);
	void SetSessionInfo(type_session *sess);
	void UpdateWisperList(int nIndex);
	BOOL SetRoomCharacter(int nIndex);
	BOOL UpdateRoomCharacter(int nIndex);
	void CharacterSay(int nIndex, WCHAR *message);
	void SetStageIndex(int nStageIndex);
	void SetRule(BYTE rule_flg);
	void SetTurnLimit(int turn);
	void SetActTimeLimit(int time);
	void UseItem(int nItemIndex, DWORD dwItemFlg, BOOL bSteal);
	void UpdateMyItems();
	void SetTeamCount(int nTeamCount);
	void UpdateTeamRuleProperty();
	void UpdateGameRuleState();
	void SetGameMaster(int nIndex, BOOL bMaster);
	void UpdateGameMasterPropertyControlsEnabled();		// ?ス??が設定できるコントロ?ル状態更新
	void UpdatePropertyControlsEnabled();
	void SetGameReady(int nIndex, BOOL bReady);
	void UpdateReadyButtonState();
	void ReEnterCharacter(int nCharaIndex);
	bool IsTeamRnadomizeCommand(LPCWSTR pStr);
	int GetTeamRnadomizeCount(LPCWSTR pStr);
	//< Room

	//> Load
	BOOL LoadStageCharacter();
	BOOL ReqLoadComplete();
	//< Load

	//> Main
	void MainStart();
	void UpdateStageCharaDisplay(int nCharaIndex);	// ステ?ジ上のキャラの?示更新
	void ActivateCharacter(int nObjNo, WORD wTurnCount);		// アクティブキャラ設定
	void UpdateCharaState(int nObjNo, int nStateIndex, int nState);
	void UpdatePrevWindValue();
	void UpdateWindValue(int value);
	void AddLogMessageSpellEnable();
	BOOL AddCharaItem(int nObjNo, int nSlot, DWORD dwItemFlg, BOOL bSteal);
	void SetTrigger(int nCharaIndex, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower);
	void ReqTriggerEnd(int nCharaIndex);
//	void SetShotBullet(int nCharaIndex, int nType);
	void SetCharacterDead(int nObjNo, E_TYPE_PACKET_MAININFO_HEADER dead_type);
	void ReqBulletShot(int nObjNo);
	void RejBulletShot(int nObjNo);
	void RejTurnPass(int nObjNo);
	void NotifyTurnEnd(ptype_session sess);
	void NotifyTurnStart(ptype_session sess);
	type_obj* GetScrProcObject()	{	return m_pScrProcObject;	};
	BOOL AddObject(type_obj* obj);
	
	// オブジェクトを一時停?して、サ?バからの情報を待たせる
	void PauseSetObjectWaitForServerEvent(int obj_no, BOOL bPause, WORD wFrame=0);
	void PauseSetObjectWaitForServerEvent(type_obj* obj, BOOL bPause, WORD wFrame=0, BOOL bCriticalSectionEntered=TRUE);
	void UpdateObjectVector(int nObjNo, short ax, short ay, short vx, short vy);
	void UpdateBulletVector(int nObjNo, short ax, short ay, short vx, short vy, short adx, short ady);
	bool UpdateBulletAngle(int blt_no, int angle);
	bool SetBulletTextureIndex(int blt_no, int index);
	bool SetObjectLuaFlg(int obj_no, DWORD flg, BOOL on);
	bool SetBulletExtData1(int obj_no, DWORD extdata1);
	bool SetBulletExtData2(int obj_no, DWORD extdata2);
	unsigned int GetCharaExtData1(int obj_no);
	unsigned int GetCharaExtData2(int obj_no);
	bool SetCharaExtData1(int obj_no, DWORD extdata1);
	bool SetCharaExtData2(int obj_no, DWORD extdata2);

	bool SetBulletOptionData(int obj_no, int index, DWORD data);
	unsigned int GetCharaOptionData(int obj_no, int index);
	bool SetCharaOptionData(int obj_no, int index, DWORD data);

	int GetCharaState(int obj_no, int chr_stt);
	unsigned int GetCharaItem(int obj_no, int item_index);

	bool RemoveObject(int obj_no);
	BOOL BombObject(int scr_id, int blt_type, int blt_chr_no, int blt_no, int pos_x,int pos_y, BOOL erase, BYTE* data);
	void UpdateObjectState(int nObjNo, DWORD dwObjState);
	void UpdateObjectType(int obj_no, BYTE type);
	void UpdateActiveState();
	void PasteTextureOnStage(int chr_type,int sx,int sy, int tx,int ty,int tw,int th);
	void OnGameEnd();
	void OnTurnEnd(int nCharaIndex);
	bool SetCharaTexture(int chr_no, RECT *prcTexture);
	bool ShotCommand(int chr_obj_no, int proc_type, int chr_id, int blt_type, int vec_angle, int power, int frame, int indicator_angle, int indicator_power);
	void SetCameraFocusToBullet(int blt_no);
	void SetCameraFocusToChara(int obj_no);
	type_blt* GetBulletInfo(int obj_no);
	int GetBulletAtkValue(int obj_no);
	int GetWindValue() { return m_nWindValue; };
	int GetStageWidth() { 
		if (!m_pMainStage) return 0;
		return m_pMainStage->GetStageWidth();
	};
	int GetStageHeight() {
		if (!m_pMainStage) return 0;
		return m_pMainStage->GetStageHeight();
	};
	int GetTurnCount(){	return m_nTurnCounter;	};
	type_session* GetCharacterAtVector(int nCharaIndex);
	int GetEntityCharacters();
	int GetLivingCharacters();
	void EraseDisconnectCharacter(int nCharaIndex);
	void ChangeCharaHP(int nObjNo, int value);
	void UpdateControlPanelHPInfo();
	int	GetMaxLoginNum() {	return m_nMaxLoginNum;	};
	void SetMaxLoginNum(int value) {	m_nMaxLoginNum = value;	};
	void CreateCharacters();
	void DestroyCharacters();

	// effect
#ifdef HOGE
	int AddEffect(int nCharaID, RECT* rcTexture, D3DXVECTOR3* pos, int nAge, BOOL bStageEffect, BOOL force=FALSE);
#else
	int AddEffect(int nCharaID, RECT* rcTexture, D3DXVECTOR3* pos, int nAge, BOOL bStageEffect);
#endif
	bool RemoveEffect(int nEffectNo, BOOL bStageEffect);
	bool SetEffectVector(int nEffectNo, D3DXVECTOR3* vec, D3DXVECTOR3* add, BOOL bStageEffect, int nEffectTime = 0);
	bool SetEffectAlpha(int nEffectNo, BYTE bytAlpha, BOOL bStageEffect);
	bool SetEffectFade(int nEffectNo, int nFade, BOOL bStageEffect, int nEffectTime = 0);
	bool SetEffectFadeInOut(int nEffectNo, int nFadeInOut, BOOL bStageEffect);
	bool SetEffectRotate(int nEffectNo, float fRot, BOOL bStageEffect);
	bool SetEffectRotation(int nEffectNo, float fRot, BOOL bStageEffect, int nEffectTime = 0);
	bool SetEffectScale(int nEffectNo, float fScaleX, float fScaleY, BOOL bStageEffect);
	bool SetEffectScalling(int nEffectNo, float fScaleX, float fScaleY, BOOL bStageEffect, int nEffectTime = 0);
	bool SetEffectAnimation(int nEffectNo, int nATime, int nACount, BOOL bLoop, BOOL bStageEffect, int nEffectTime = 0);
	bool SetEffectTexture(int nEffectNo, RECT* rcTexture, BOOL bStageEffect);

	void HideStage();
	void ShowStage();

	// sound
	void RegistSoundSE(char* rc_name);
	void RegistSoundBGM(char* rc_name);
	// loop = -1 -> endless loop
	void PlayScrSoundSE(char* rc_name, int loop=0, int fade=0);
	void PlayScrSoundBGM(int fade);
	void StopScrSoundBGM(int fade);
	void PlaySysSoundSE(std::wstring wsResouce, int fade=0);

	//< Main
	int m_nTest;
	// public変数
	BYTE	m_bytBGMVolume;			// BGM?リュ??
	BYTE	m_bytSEVolume;
	BOOL m_bEffectEnable;			// EFFECT ON/OFF
	bool m_bSavePass;
	bool m_bBulletFocus;			// BLT_FOCUS ON/OFF
	bool m_bActChrFocus;			// ACT_FOCUS ON/OFF
	
	int	m_nResultReturnCounter;
	// State
	EGameAllState	m_eAllState;
	// debug
	void GenerateDatMsg(WCHAR* msglog, BYTE* data, WORD size);

	CMainStage* GetMainStage()	{	return m_pMainStage;	};

//===========================================================//
private:
	BOOL Init();						// 初期化:Game()→Init()
	BOOL Run(EGameState eGameState);
	BOOL Release();

//========= Run =========//
	BOOL TitleInit();
	BOOL Title();
	BOOL TitleRelease();

	BOOL LoginInit();
	BOOL Login();
	BOOL LoginRelease();

	BOOL RoomInit();
	BOOL Room();
	BOOL RoomRelease();

	BOOL LoadInit();
	BOOL Load();
	BOOL LoadRelease();

	BOOL MainInit();
	BOOL Main();
	BOOL MainRelease();

	BOOL ResultInit();
	BOOL Result();
	BOOL ResultRelease();

//========================//

//======== Render ========//
	void RenderTitle(float fElapsedTime );
	void RenderLogin(float fElapsedTime );
	void RenderRoom(float fElapsedTime );
	void RenderLoad(float fElapsedTime );
	void RenderMain(float fElapsedTime );
	void RenderResult(float fElapsedTime );
//======== GUI Event ========//
	void CALLBACK GUITitle(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	void CALLBACK GUILogin(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	void CALLBACK GUIRoom(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	void CALLBACK GUILoad(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	void CALLBACK GUIMain(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	void CALLBACK GUIResult(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

//========= Cont =========//
//	void ContLogSet(BYTE cont, BYTE button);
//	void ContLogClear()
//	{	ZeroMemory(&tContLog, (sizeof(TContLog)*CONT_LOGS)); };
/*	 入力ログの保存、不使用
	void VecLogSet(D3DXVECTOR3 v);
	void VecLogClear()
	{	D3DXVECTOR3 v(0.0f,0.0f,0.0f);
		ZeroMemory(&tVecLog, (sizeof(TVecLog)*CONT_LOGS));
		for (int i=0;i<CONT_LOGS;i++)
			tVecLog[i].Vec = v;
	};
*/
//=========================可変部分==========================//
// Private関数
	// Control
	void SaveIMEText();
	// Sound
	BOOL PlayBGM(const WCHAR* wsResouceName, int nFade=0);
	BOOL PlayBGM(int nSoundID, int nFade=0);
	void StopBGM(BOOL bRemoveFromLib, int nFade=0);
	void ClearScrSound();

	// Release
	void ClearStageObjects();
	void ClearStageEffects();
	void ClearBGEffects();
	// Lost
	void OnLostCharacterScriptTexture();
	// Reset
	void OnResetCharacterScriptTexture();
	void OnResetCharacterScriptSound();
	BOOL OnResetSoundDevice();

	// Create
	BOOL CreateGUI();
	BOOL CreateConfigGUI();
	BOOL LoadSystemSound();
	BOOL LoadScriptSound();
	void LoadSound(WCHAR* pPath, CSoundLibraries* pSoundLibs, std::map<std::wstring, int>* pMapSoundIDHash);
	void SaveConfig();
	void CreateItemDetailControlSize();
	// CreateControl
	int AddResourceTexture(WCHAR* path);
	void SetUserList();
	void SetMyItemButtonList();
	void SetCharacterIconButtonList();
	BYTE GetChatMessageRange();
	void SetTeamSeparateControl();
	void SetStageSelectControl();
	void SetTimerAnimation(CDXUTAnimationSprite* pASprite, CDXUTAnimationSprite* pASpriteBG);
	void CreateResultRank();
	// Room
	void ShowItemDetail(int nControlID);
	void OnItemIconButtonDown(CDXUTRadioButton* pBtn);
	void OnClickViewButton(int nControl);
	void OnStageSelectButtonDown(int nNum);
	void OnStageRandomButtonDown();
	void OnRuleCheckButtonDown(BYTE flg);
	void OnSelectItemBtnList(CDXUTRadioButton* pBtn);
	void CheckCharacterSelectRadioBtnFromID(int nObjType);
#if RTRIAL
#else
	void OnCharacterSelectRadioBtnDown(CDXUTRadioButton* pBtn);	// キャラ選択ラジオ??ン押下イベント
#endif
	void UpdateMyCharaInfo();		// 自キャラ情報更新
	void UpdateStageSelectButton();
	void OnAddItemButtonDown();
	void CheckEnableItemAddButton();				// アイテ?追加??ンの有効状態確認
	BOOL OnMyItemButtonDown(int nControlID);	// 使用アイテ???ン押下
	void OnTeamRuleButtonDown(int nNum);		// ???数変更
	void OnReadyButtonDown();
	void OnActTimeLimitButtonDown(int nNum);
	BOOL CheckChangeReadyStateOfMaster();
	BOOL CheckChangeReadyStateOfUser();
	BOOL CheckAddTeam();
	void AddLogRoomIn(int nCharaIndex);
	void UpdateLimitTurnFromControl(int modvalue);
	void SetLimitTurnEnabled(bool b);
	void SetActTimeLimitEnabled(bool b);
	// Load
	static DWORD __stdcall Thread_Loading(LPVOID param);
	BOOL LoadLoadingImage();

	// Main Run
	void GetReverseStateMatrix(D3DXMATRIX* matRev, RECT* rcWindow);
	void ShotBullet(int nAngle, int nPower, int nIndicatorAngle, int nIndicatorPower);
	BOOL RunMainActive();
	BOOL RunMainNoneActive();
	BOOL AddBullet(type_blt* blt);
	void UpdateObjectNo();
	void OnCreateScrCharaBullet(type_blt* blt);
	void OnCreateScrCharaSpell(type_blt* blt);
	void OnCreateScrStageBullet(type_blt* blt);
	void FrameObjects();
	void SetVisibleBulletSelectButtons(bool value);
	CStageEffect* FindEffectFromEffectNo(int nEffectNo, BOOL bStageEffect);
	void SetVisibleStageCharacterInfo(bool bVisible);
	int	GetStockItemBullet();	// 使用アイテ?弾を取得ないなら -1を返す
	void SetMyIteamsEnabled(bool bEnabled);
	void SetMyBulletsEnabled(bool bEnabled);
	void OnCharaDisconnect(int nCharaIndex);
	void UpdateNotifyMyTurn();
	void FrameStageCharacter();
	void UpdateSCExp();
	void SetSelectBulletInfo();

	// Main Render
	void RenderObject(ptype_obj obj, D3DXMATRIX* matStage);
	void OnRenderScrBullet(type_blt* blt, D3DXMATRIX* matStage);
	void NotifyRenderScrBullet(type_blt* blt);
	void NotifyDeadScrChara(ptype_session sess, E_TYPE_CHARA_DEAD type, DWORD prev_state);
	void OnRenderItemBullet(type_blt* blt, D3DXMATRIX* matStage);
	void RenderMainBlindState(float fElapsedTime, D3DXMATRIX* matWorld);
	void UpdateStageDrawRect(BOOL bMain=TRUE);
	void CalcViewTransCaps(BOOL bMain=TRUE);
	DWORD GetStageColor();
	// Main GUI
	void ReqUseItem(int nControlID);
	void OnSelectBulletType(int nControlID);
	void OnTurnPassButtonDown(int nControlID);

	// Result
	void UpdateRankView();
	void SetResultText();

//==========================================================//

//========================//
//	TVecLog			m_tVecLog[CONT_LOGS];
	// D3D
	LPDIRECT3D9	m_pD3D;						// D3Dﾊﾝﾄﾞ?
	LPDIRECT3DDEVICE9 m_pDev;				// D3Dデバイス
	LPD3DXFONT		m_pDXFont;					// D3DXフォント
//	CGraphic*			m_pGraph;
#if USE_DINPUT
	CGameControl*	m_pInput;
#endif
	BOOL				m_bCreated;			// Create()を通って成功したか。
	BOOL				m_bActive;					// フォ?カスがあるか
	HWND				m_hWnd;						// Windowsﾊﾝﾄﾞ?
	HINSTANCE		m_hInst;					// ｲﾝｽﾀﾝｽ
	ESystemState	m_eSysState;				// システ?の状態
	EGameState		m_eGameState;				// ゲ??

//=========================可変部分==========================//
// 変数
//	TContLog	tContLog[CONT_LOGS];
	BOOL				m_bDeviceLost;
	CDXUTDialog*	p_pUI;				// dialog for sample specific controls
	CDXUTDialog*	p_pConfig;			// config
	lua_State*		p_pL;
	LuaHelper*		p_pLuah;
	BOOL					m_bContYOnDown;
	int						m_nTcpSock;
	int						m_nPortNo;
	int						m_nUserIndex;
	int						m_nTeamCount;
	int						m_nAuthedCount;
	BYTE					m_bytRuleFlg;
	BOOL				m_bMouseLButtonDown;
	int						m_nChatSendInterval;		// ?ャット送信間隔を空けるためのカウン?
	int						m_nWindValue;
	TInputState		m_tKeyState;
	int						m_nLimitTurn;					// 制限??ン数
	int						m_nMaxLoginNum;					// MAXLOGINNUM

	CIniFile*	m_pIniConfig;

	CSoundLibraries*	m_pSysSoundLibs;			// システ?用の音声バッフ?
	CSoundLibraries*	m_pScrSoundLibs;			// スクリプト用の音声バッフ?
	CSoundPlayer*		m_pSoundPlayer;			// 音声の再生
	
	int m_nSavedInputX;
	int m_nSavedInputY;
	int m_nSavedMouseX;
	int m_nSavedMouseY;
	int m_nMousePosX;
	int m_nMousePosY;

	E_TYPE_CONN_STATE	m_eConnState;				// 接続状態
	E_TYPE_OBJ_STATE		m_eUserState;				// ユ?ザ状態

	int m_nDefaultGUIResourceIndex;
	LPDIRECT3DTEXTURE9 m_pDefaultGUIResourceTexture;
	LPDIRECT3DTEXTURE9 m_pBGTexture;
	WCHAR							m_pBGTexturePath[_MAX_PATH*2+1];

	RECT	m_rcDrawStage;
	RECT	m_rcDrawBGStage;

	// sound
	std::map<std::wstring, int>	m_mapSysSoundIDHash;			// システ?のリ??ス名とIDの連想配列
	std::map<std::string, int>		m_mapScrSoundIDHash;			// スクリプトのリ??ス名とIDの連想配列
	int		m_nBGMSoundID;			// BGMリ??スID
	int		m_nBGMPlayingID;		// 再生中BGMID

	// scr_id, scr_info
	std::map < int, TCHARA_SCR_INFO > m_mapCharaScrInfo;
	std::map <int, TSTAGE_SCR_INFO> m_mapStageScrInfo;
	int m_nStageIndex;
	TSTAGE_SCR_INFO* m_pSelectedStageScrInfo;

	// セッション情報配列
	type_session		m_SessionArray[MAXLOGINUSER];
	CRoomCharacter* m_pRoomCharacters[MAXLOGINUSER];
	CStageCharacter* m_pStageCharacters[MAXLOGINUSER];

	// control //
	CDXUTButtonList* m_pMyItemBtnList;
	CDXUTButtonList* m_pCharacterBtnList;
	CDXUTButtonList* m_pItemBtnList;
	CTeamRulePropertyControlManager* m_pTeamRulePropertyManager;
	CTeamSeparater*	m_pTeamSeparater;
	CDXUTMeter*		m_pShotMeter;
	CShotPowerIndicator* m_pShotPowerIndicator;
	CShotAngleIndicator* m_pShotAngleIndicator;
	BOOL					m_bChatFocusFlg;
	RECT						m_rcItemDetailControlSize[GAME_ITEM_COUNT];
	CSpinButton*		m_pSpinTurnLimit001;
	CSpinButton*		m_pSpinTurnLimit010;
	CSpinButton*		m_pSpinTurnLimit100;

	// login //
	WCHAR					m_wsRecvScrPath[_MAX_PATH*2+1];

	// room //

	// load //
	typedef struct TLoadingParam
	{
		TSTAGE_SCR_INFO*			pStageScrInfo;
		CMainStage*						pMainStage;
		LPDIRECT3DTEXTURE9*	ppStageTexture;
		LPDIRECT3DTEXTURE9*	ppStageBGTexture;
		std::vector<ptype_session>*	pVecCharacters;
		CSoundLibraries*					pSoundLibs;
		std::map<std::string, int>*	pSoundIDHash;
		int											*pBgmSoundID;
	};
	LPDIRECT3DTEXTURE9	m_pStageTexture;
	LPDIRECT3DTEXTURE9 m_pStageBGTexture;
	int m_nLoadingTextureCounter;
	BOOL m_bLoadComplete;
	HANDLE m_hThreadLoading;
	BOOL m_bReqLoadComplete;
	TLoadingParam m_tThreadParam;

	// main //
	BOOL							m_bMainInitFlg;
	std::vector< type_session* > m_vecCharacters;
	std::map<int, CStageEffect* > m_mapStageEffects;
	int											m_nStageEffectNoCounter;
	std::map<int, CStageEffect* > m_mapBGEffects;
	int											m_nBGEffectNoCounter;
	CMainStage					*m_pMainStage;
	int									m_nStageTextureW;
	int									m_nStageTextureH;
	int									m_pStageBGTextureW;
	int									m_pStageBGTextureH;
	D3DXVECTOR3				m_vecStageViewTrans;
	type_obj*						m_pScrProcObject;					// スクリプトで使用中のオブジェクト
	CDXUTMeter*				m_pMeterMovable;
	CDXUTMeter*				m_pMeterHP;
	CDXUTMeter*				m_pMeterWindL;
	CDXUTMeter*				m_pMeterWindR;
	CDXUTMeter*				m_pMeterSCExp;
	CDXUTRadioButton*		m_pRbSC;
	CShotAngle*					m_pShotAngle;
	CDXUTAnimationSprite*	m_pASpriteTimer;
	CDXUTAnimationSprite*	m_pASpriteTimerBG;
	CDXUTRadioButton*	m_pBulletSelectButtons[MAX_CHARA_BULLET_TYPE];	// 発射する弾選択用??ン
	int									m_nActiveObjectNo;					// アクティブオブジェクトNo
	E_STATE_GAME_MAIN_PHASE	m_eMainPhase;				// 
	int									m_nTriggerType;						//	
	int									m_nShootingAngle;					// 
	int									m_nShootingPower;					// 
	int									m_nShootingProcType;
	int									m_nShootingBltType;
	int									m_nShootingCharaObjNo;		
	int									m_nShootingIndicatorAngle;					// 
	int									m_nShootingIndicatorPower;					// 

	int									m_nSelectBulletNo;
	std::map < int, type_obj* > m_mapObjects;					// オブジェクトのリスト
	std::vector< int >			m_vecObjectNo;
	E_TYPE_USER_DIRECTION m_eSavedDir;						// 自キャラの方向保存
	CDXUTButtonList* m_pSelBulletBtnList;							// 弾選択??ンリスト
	int							m_nActiveCharaFocusCounter;			// アクティブになったキャラにフォ?カスを移す時間
	D3DXVECTOR3		m_vecActiveCharaFocusStartPos;		// アクティブになったキャラにフォ?カス前のカメラ位置
	int							m_nMyCharaFocusCounter;				// 自キャラフォ?カスを移す時間
	D3DXVECTOR3		m_vecMyCharaFocusStartPos;		// アクティブになったキャラにフォ?カス前のカメラ位置

	BOOL					m_bTriggerEnd;
	ptype_obj				m_pFocusObject;
	BOOL					m_bPauseBulletFocus;
	BOOL					m_bScrFocus;			// スクリプトによるフォ?カス命令、自動フォ?カスを制限する
	int							m_nNotifyMyTurn;
	int							m_nTurnCounter;
	int							m_nActTimeLimit;								// アクティブ制限時間
	int							m_nActiveTimeCounter;
	
	int							m_nStageFadeCounter;
	BOOL					m_bStageVisible;

	// Result //
	int m_nRankItemCount;

//==========================================================//
};

#endif