#ifndef H_STAGE_CHARACTER_CLASS___
#define H_STAGE_CHARACTER_CLASS___
// ===================================================================
// CStageCharacter.h
//	ステ?ジにいるキャラクラス・ヘッ??
// ===================================================================

#include <windows.h>
#include <TCHAR.h>
#include "util.h"
#include <map>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>

#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"
#include "resource.h"

#include "TextureLoader.h"
#include "LuaHelper.h"

// キャラが?示できる
// HPのバ?が?示できる
// キャラ名が?示できる
// ステ?ジを移動できる
// 移動する時アニメ?ションする
// 弾が撃てる
// アイテ?が使える

// ステ?ジのキャラクラス
class CStageCharacter
{
public:
	CStageCharacter();
	virtual ~CStageCharacter() 
	{ 
		if (m_bCreated)
			Destroy();
	};

	// 生成
	// pDev			: D3Dデバイス
	// pTexture	: テクス?ャ
	// nGUIResourceIndex : 基?のGUIリ??ス番号
	// pStatic		: ス?ティック(キャラ名用)
	// pSess		: オブジェクト情報
	HRESULT Create(std::map < int, TCHARA_SCR_INFO > *mapScrInfo, int nGUIResourceIndex, CDXUTDialog* pDialog, type_session* pSess, int nStageWidth, int nStageHeight);

	// 更新
	// pTexture	: テクス?ャ
	// nGUIResourceIndex : 基?のGUIリ??ス番号
	// nSpriteWidth,nSpriteHeight : ?示サイズ
	// nAnimationCount : ｱﾆﾒｰｼｮ??
//	HRESULT Update(LPDIRECT3DTEXTURE9 pTexture, int nGUIResourceIndex, int nSpriteWidth, int nSpriteHeight, int nAnimationCount);

	// 状態更新
//	void UpdateState();
	// 破棄
	void Destroy();
	// 毎フレ??
	void Frame();

	BOOL IsCreated()
	{ return m_bCreated;	};

	// ?んだ
	void SetDead();

	void Update()
	{
		if (m_bCreated)
		{
			CalcDrawPos();
			CalcDrawRect();
			UpdateState();
		}
	}
	void CalcDrawRect();
	void CalcDrawPos();
	// 状態更新
	void UpdateState();
	// ??ン状態セット
	void SetMyTurn(bool bTurn);

	void ChangeHP(int value);

	// ?画
	// pDev	: D3Dデバイス
	// fElapsedTime : 経過時間
	void Render(LPDIRECT3DDEVICE9 pDev, float fElapsedTime, D3DXMATRIX* matChara, D3DXMATRIX* matStage, D3DCOLOR color=0xFFFFFFFF);
	
	void SetVisible(bool bVisible);
	bool GetVisible()
	{	return m_bVisible;			};
	void SetVisibleInfo(bool bVisible){	m_bVisibleInfo = bVisible;	};
	bool GetVisibleInfo(){	return m_bVisibleInfo;	};

	void SetTurn(bool bTurn)
	{	m_pMyTurn->SetVisible(bTurn);	};

	void SetTexture(RECT* prcTexture);

	// OnLostDeviceEvent
	void OnLost();
	// OnResetDeviceEvent
	void OnReset();

	ptype_session	GetSession();

protected:
	type_session*	p_pSession;					// オブジェクト情報
	
	int m_nAnimationIndex;							// ｱﾆﾒｰｼｮ?`画範囲インデックス
	int m_nAnimationCount;							// ｱﾆﾒｰｼｮ?`画範囲数

	int m_nAnimationTimeCounter;				// ｱﾆﾒｰｼｮﾝｲﾝﾃﾞｯｸｽの数値を増やす時間
	int m_nAnimationTime;							// ｱﾆﾒｰｼｮﾝｲﾝﾃﾞｯｸｽの数値を増やす時間
	
	int m_nTextureWidth;							// テクス?ャ横幅
	int m_nTextureHeight;							//	テクス?ャ縦幅
	LPDIRECT3DTEXTURE9 p_pTexture;		// テクス?ャ

	int m_nCharaWidth;
	int m_nCharaHeight;
	D3DXMATRIXA16	m_matScale;				// ?示サイズへのスケ?リング?トリックス
	float m_fAdjustPosY;

	CDXUTDialog*	p_pDialog;
	CDXUTStatic*	m_pControlName;			// 名前?示用コントロ?ル
	CDXUTMeter*	m_pHPBar;					// HPバ?
	CDXUTSprite*	m_pMyTurn;					// ??ン?示
	CDXUTStatic*	m_pStatusInfo;				// ?メ?ジ?示
	int	m_nStatuInfoTime;

	int m_nControlNameWidth;					// 
	int m_nControlNameHeight;
	int m_nGUIResourceIndex;						// リ??ス番号

	BOOL m_bCreated;								// 生成済み
	TCHARA_SCR_INFO*  m_pScrInfo;			// スクリプト情報

	D3DXMATRIX		m_matDrawCharaRadian;
	D3DXVECTOR3	m_vecDrawCharaCenter;	// ?画位置
	D3DXVECTOR3	m_vecDrawCharaPos;	// ?画位置
	RECT m_recDraw;									// ?画範囲	
	bool m_bVisible;									// ?示状態
	bool m_bVisibleInfo;								// キャラの情報?示
	int m_nBodyRange;								// 当たり判定用半径
	BOOL m_bMyTurn;
	void UpdateMyTurnTexture();

	int m_nStageWidth;
	int m_nStageHeight;

};

#endif
