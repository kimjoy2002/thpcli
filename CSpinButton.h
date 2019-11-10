#ifndef H_SPIN_BUTTON___
#define H_SPIN_BUTTON___
// ===================================================================
// CSpinButton.h
//	���l?���Ƒ���??���̃N���X
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
		|     button    |	����
		|-------------|
		|      static    |	����(�ꌅ)
		|-------------|
		|     button    |	����
		|-------------|
		pDialog					�ǉ����?�C�A���O
		nBaseID				���ID�AnBaseID����3�g�p����
		nResouceIndex		��??�X�ԍ�
		x,y						������W
		bw,bh					??���T�C�Y
		sw,sh					�X?�e�B�b�N�e�L�X�g�T�C�Y
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
	CDXUTButton* m_pIncrement;				// ����??��
	CDXUTButton* m_pDecrement;				// ����??��
	CDXUTStatic*	m_pDigit;						// ����
};

#endif
