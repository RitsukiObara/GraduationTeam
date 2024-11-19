//*****************************************************
//
// ゲームスタート告知UI処理[UI_ready.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _UI_READY_H_
#define _UI_READY_H_

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
class CUIready : public CGameObject
{
public:
	// 列挙型定義
	enum E_Number
	{// 数字の種類
		NUMBER_SECOND = 0,	// 秒
		NUMBER_MAX
	};

	// 列挙型定義
	enum STATE
	{// テクスチャの種類
		STATE_NUMBER = 0,	//数字状態
		STATE_GO,			//GO指示
		STATE_MAX
	};

	CUIready();	// コンストラクタ
	~CUIready();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CUIready* Create();	// 生成

	// 変数取得・設定関数
	int GetSecond() { return m_nSecond; }	// 秒
	void SetSecond(int nSecond) { m_nSecond = nSecond; }
	void AddSecond(int nSecond) { m_nSecond += nSecond; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置

private:
	// メンバ関数
	void UpdateNumber(void);		// 数字の更新
	void TransformNumber(void);		// 数字のトランスフォーム設定
	void StartGame(void);			// ゲームの開始

	// メンバ変数
	int m_nSecond;					// 現在の時間(秒)
	vector<CNumber*> m_aNumber;		// 数字の配列
	int m_nFrame;					// フレーム計算
	STATE m_state;					// 状態変数
	CUI* m_Go;						// GOの変数
	int m_nStateCnt;				// 状態遷移カウント
	float m_fsize;					// サイズ変数
	float m_fmove;					// 移動量
};

#endif
