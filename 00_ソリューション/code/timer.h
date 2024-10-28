//*****************************************************
//
// タイムの処理[timer.h]
// Author:森川駿弥
//
//*****************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"
#include "number.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CTimer : public CGameObject
{
public:
	// 列挙型定義
	enum E_Number
	{// 数字の種類
		NUMBER_MINUTE = 0,	// 分
		NUMBER_SECOND,	// 秒
		NUMBER_MAX
	};

	CTimer();	// コンストラクタ
	~CTimer();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// 変数取得・設定関数
	int GetSecond() { return m_nSecond; }	// 秒
	void SetSecond(int nSecond) { m_nSecond = nSecond; }
	void AddSecond(int nSecond) { m_nSecond += nSecond; }
	bool GetFlag() { return m_bStop; }	// 動作フラグ
	void SetFlag(bool bStop) { m_bStop = bStop; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール
	float GetScaleNumber(void) { return m_fScaleNumber; }
	void SetColor(E_Number number,D3DXCOLOR col);	// 色
	D3DXCOLOR GetColor(E_Number number);

	// 静的メンバ関数
	static CTimer *Create();	// 生成
	static void SaveSecond(int nSecond);	// 時間保存
	static int LoadSecond(void);	// 時間読込

private:
	// メンバ関数
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	// メンバ変数
	int m_nSecond;			// 現在の時間(秒)
	float m_fScaleNumber;	// 数字のスケール
	bool m_bStop;				// タイマー停止のフラグ
	vector<CNumber*> m_aNumber;	// 数字の配列
	vector<CUI*> m_aColon;	// コロンの配列
};

#endif
