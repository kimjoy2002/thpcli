#ifndef H_SPIN_BUTTON___
#define H_SPIN_BUTTON___
// ===================================================================
// CSpinButton.h
//	数値?示と増減??ンのクラス
// ===================================================================

#include <windows.h>
#include <TCHAR.h>

#include "util.h"
#include "../thplib/DXUT/dxstdafx.h"
#include "../thplib/DXUT/DXUT.h"
#include "resource.h"


class CSpinButton
{
public:
	CSpinButton();
	virtual ~CSpinButton() { Destroy(); };

	/* Create
		|-------------|
		|     button    |	増加
		|-------------|
		|      static    |	数字(一桁)
		|-------------|
		|     button    |	減少
		|-------------|
		pDialog					追加先の?イアログ
		nBaseID				基底ID、nBaseIDから3つ使用する
		nResouceIndex		リ??ス番号
		x,y						左上座標
		bw,bh					??ンサイズ
		sw,sh					ス?ティックテキストサイズ
	*/
	BOOL Create(CDXUTDialog* pDialog,int nBaseID, int nResourceIndex, int x, int y, int bw, int bh, int sw,int sh);
	void Destroy();

	bool GetVisible() { return m_bVisible; };
	void SetVisible(bool b)
	{
		if (m_bCreated)
		{
			if (m_pIncrement->GetVisible() != b)
				m_pIncrement->SetVisible(b);
			if (m_pDecrement->GetVisible() != b)
				m_pDecrement->SetVisible(b);
			if (m_pDigit->GetVisible() != b)
				m_pDigit->SetVisible(b);
		}
		m_bVisible = b;
	};

	inline void OnButtonDown(int nControlID)
	{
		if (m_pIncrement && m_pIncrement->GetID() == nControlID)
			OnIncrementButtonDown();
		else if (m_pIncrement && m_pDecrement->GetID() == nControlID)
			OnDecrementButtonDown();
	};

	inline void OnIncrementButtonDown()
	{
		m_nValue = (m_nValue+1)%10;
		UpdateDigit();
	};

	inline void OnDecrementButtonDown()
	{
		m_nValue = m_nValue?m_nValue-1:9;
		UpdateDigit();
	};

	void UpdateDigit();

	int GetValue()	{ return m_nValue;	};
	void SetValue(int nValue)
	{
		m_nValue = nValue%10;
		UpdateDigit();
	};

	bool GetEnabled() { return m_bEnabled;	};
	void SetEnabled(bool value)
	{
		m_bEnabled = value;
		if (value != m_pIncrement->GetEnabled())
			m_pIncrement->SetEnabled(value);
		if (value != m_pDecrement->GetEnabled())
			m_pDecrement->SetEnabled(value);
	};

protected:
	BOOL m_bCreated;
	bool m_bVisible;
	bool m_bEnabled;
	int m_nX;
	int m_nY;
	int m_nValue;
	int m_nIncrementID;
	int m_nDecrementID;

	CDXUTDialog* p_pDialog;
	CDXUTButton* m_pIncrement;				// 増加??ン
	CDXUTButton* m_pDecrement;				// 減少??ン
	CDXUTStatic*	m_pDigit;						// 数字
};

#endif
