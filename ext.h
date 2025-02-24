#ifndef H_EXTERN____
#define H_EXTERN____
// ===================================================================
// ext.h
//	外部参照・ヘッ??
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include <STDLIB.h>

#include "util.h"

#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"
#include "CFiler.h"
#include "game.h"
#include "../thpcommon/CPacketQueue.h"
#include "../thpcommon/CCriticalSection.h"

#include "main.h"


/* =================================== */
/* =====				CLIENT DEFINE					    ===== */
/* =================================== */
#define BGM_ON						1	// BGMありなし
#define TEST_CAPTION		0

/* =================================== */
/* =====						前方宣言					    ===== */
/* =================================== */
class CGame;

// ====================================
// 関数
// ====================================

void AddMessageLog(TCHAR*	plog);
extern type_packet * NewPacket();
extern void DeletePacket(type_packet const * packet);
BOOL StartClient(int nSocket);
HRESULT LoadTextureFromFiler(WCHAR* path, LPDIRECT3DTEXTURE9 *pTex);
HRESULT LoadTextureFromFiler(WCHAR* path, LPDIRECT3DTEXTURE9 *pTex, int *pWidth, int* pHeight);

void CALLBACK OnGUIControlEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

// random
void init_genrand(unsigned long s);
unsigned long genrand_int32(void);

// ====================================
// 変数
// ====================================
extern const TITEM_RESOURCE_INFO c_tItemResouceInfoTable[GAME_ITEM_COUNT+1];
extern CDXUTDialogResourceManager g_DialogResourceManager;
extern CDXUTDialog g_SysUI;
extern LPD3DXSPRITE g_pSprite;
extern BOOL g_bCloseSocket;
extern BOOL g_bKillLoadingThread;
extern BOOL g_bDeviceReady;
extern CFiler*	g_pFiler;

extern HWND	g_hWnd;
extern CGame*	g_pGame;
extern CPacketQueue*		g_pPacketQueue;
extern CCriticalSection*		g_pCriticalSection;
extern CTextFile*				g_pLogFile;
extern BOOL						g_bOneClient;
extern BOOL						g_bDebug;
extern int						g_nMaxLoginNum;
extern HANDLE g_hThreadPacketReceiver;

extern float g_fFps;
extern LPDIRECT3DDEVICE9 g_pDevice;	// Device

extern BOOL						g_bCmdSetup;
extern WCHAR						g_wsCmdName[MAX_USER_NAME+1];
extern WCHAR						g_wsCmdPass[MAX_SRV_PASS+1];
extern WCHAR						g_wsCmdIP[64];
extern int								g_nCmdPort;
extern int								g_nStageLoadType;
extern int							g_nMaxCost;
extern int							g_nMaxItemStockCount;

#if USE_LUA
extern lua_State *g_L;
extern LuaHelper*	g_pLuah;

// lua
extern int C_CreateBullet(double proc_type, double chr_obj_no, double chara_type, double blt_type, double obj_type, double posx, double posy, double vecx, double vecy, double addx, double addy, double hit_range, double extdata1, double extdata2);
extern bool C_Shot(int chr_obj_no, int proc_type, int chr_id, int blt_type, int vec_angle, int power, int frame, int indicator_angle = 0, int indicator_power = 0);
extern bool C_RemoveBullet(double obj_no, int rm_type);
extern bool C_IsRemovedObject(double obj_no);
extern bool C_BombObject(int scr_id, int blt_type, int blt_chr_no, int blt_no, int pos_x, int pos_y,int erase=1);
extern bool C_DamageCharaHP(int assailant_no, int victim_no, int hp);
extern bool C_UpdateCharaStatus(int obj_no, int hp, int mv, int delay, int exp);
extern bool C_UpdateCharaPos(int obj_no, int x,int y);

extern bool C_SetBulletOptionData(int obj_no, int index, int data);
extern unsigned int C_GetBulletOptionData(int obj_no, int index);
extern bool C_SetCharaOptionData(int obj_no, int index, int data);
extern unsigned int C_GetCharaOptionData(int obj_no, int index);

extern bool C_SetBulletExtData1(int obj_no, unsigned long extdata1);
extern bool C_SetBulletExtData2(int obj_no, unsigned long extdata2);
extern unsigned int C_GetCharaExtData1(int obj_no);
extern unsigned int C_GetCharaExtData2(int obj_no);
extern bool C_SetCharaExtData1(int obj_no, unsigned long extdata1);
extern bool C_SetCharaExtData2(int obj_no, unsigned long extdata2);
extern bool C_AddCharaItem(int obj_no, int item_flg);
extern bool C_SetCharaState(int obj_no, int chr_stt, int val);
extern int C_GetCharaState(int obj_no, int chr_stt);
extern unsigned long C_GetCharaItem(int obj_no, int item_index);

extern bool C_UpdateBulletState(double obj_no, double state);

extern bool C_UpdateBulletPosition(double obj_no, double px, double py, double vx, double vy, double adx, double ady);
extern bool C_UpdateBulletVector(double obj_no, double vx, double vy, double adx, double ady);
extern bool C_UpdateObjectType(int obj_no, int type);
extern void C_PasteTextureOnStage(int scr_id, int sx,int sy, int tx,int ty,int tw,int th);
extern int C_GetRand(int min, int max);
extern int C_GetEntityCharacters();
extern int C_GetLivingCharacters();
extern int C_GetWindValue();
extern void C_SetWindValue(int wind, int dir);
extern int C_GetStageWidth();
extern int C_GetStageHeight();
extern type_session C_GetCharacterAtVector(int index);
extern type_session C_GetCharacterFromObjNo(int obj_no);
extern type_blt C_GetBulletInfo(int obj_no);
extern int C_GetBulletAtkValue(int blt_no);
extern unsigned int C_GetBulletExtData1(int obj_no);
extern unsigned int C_GetBulletExtData2(int obj_no);

extern bool C_UpdateBulletAngle(double blt_no, double angle);
extern bool C_SetBulletTextureIndex(double blt_no, double index);
extern bool C_SetCharaTexture(int chr_no, int left, int top, int right, int bottom);

extern int C_AddEffect(double chr_id, double left, double top, double right, double bottom, double x, double y, double age);
extern bool C_RemoveEffect(double effect_no);
extern bool C_SetEffectVector(double effect_no, double vx, double vy, double ax, double ay, double effect_time=0);
extern bool C_SetEffectAlpha(double effect_no, double alpha);
extern bool C_SetEffectFade(double effect_no, double fade, double effect_time=0);
extern bool C_SetEffectFadeInOut(double effect_no, double fadeinout);
extern bool C_SetEffectRotate(double effect_no, double rot);
extern bool C_SetEffectRotation(double effect_no, double rot, double effect_time=0);
extern bool C_SetEffectScale(double effect_no, double scalex, double scaley);
extern bool C_SetEffectScalling(double effect_no, double scalex, double scaley, double effect_time=0);
extern bool C_SetEffectAnimation(double effect_no, double atime, double acount, bool loop, double effect_time=0);
extern bool C_SetEffectTexture(int effect_no, double left, double top, double right, double bottom);

extern int C_AddBGEffect(double chr_id, double left, double top, double right, double bottom, double x, double y, double age);
extern bool C_RemoveBGEffect(double effect_no);
extern bool C_SetBGEffectVector(double effect_no, double vx, double vy, double ax, double ay, double effect_time=0);
extern bool C_SetBGEffectAlpha(double effect_no, double alpha);
extern bool C_SetBGEffectFade(double effect_no, double fade, double effect_time=0);
extern bool C_SetBGEffectFadeInOut(double effect_no, double fadeinout);
extern bool C_SetBGEffectRotate(double effect_no, double rot);
extern bool C_SetBGEffectRotation(double effect_no, double rot, double effect_time=0);
extern bool C_SetBGEffectScale(double effect_no, double scalex, double scaley);
extern bool C_SetBGEffectScalling(double effect_no, double scalex, double scaley, double effect_time=0);
extern bool C_SetBGEffectAnimation(double effect_no, double atime, double acount, bool loop, double effect_time=0);
extern bool C_SetBGEffectTexture(int effect_no, double left, double top, double right, double bottom);

extern void C_SetCameraFocusToBullet(int blt_no);
extern void C_SetCameraFocusToChara(int obj_no);

extern void C_HideStage();
extern void C_ShowStage();

extern void C_RegistSoundSE(char* rc_name);
extern void C_PlaySoundSE(char* rc_name, int loop=0, double fade=0);

extern int C_GetMyCharaNo();
extern void C_AddMsgLog(char* str);
extern int C_GetAngle(int x, int y);
extern int C_GetScrIDFromChrNo(int obj_no);
extern int C_GetUserTeamNo(int obj_no);
extern char* C_GetUserName(int obj_no);

extern void C_DbgOutputStr(char* str);
extern void C_DbgOutputNum(int num);
extern void C_DbgAddLogNum(int num);

extern type_ground C_GetRandomStagePos();
extern type_ground C_GetGroundPos(int x, int y);

#endif
#endif