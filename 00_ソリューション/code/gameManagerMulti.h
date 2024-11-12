//*****************************************************
//
// マルチゲームマネージャー[gameManagerMulti.h]
// Author:��山桃也
//
//*****************************************************
#ifndef _GAMEMANAGERMULTI_H_
#define _GAMEMANAGERMULTI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameManager.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;

//*****************************************************
// クラスの定義
//*****************************************************
class CGameManagerMulti : public CGameManager
{
public:
	CGameManagerMulti();	// コンストラクタ
	~CGameManagerMulti() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update() override;
	void Draw() override;

	// 変数取得・設定関数

	// 静的メンバ関数

private:
	// メンバ関数
	void UpdateStart(void) override;		// 開始状態の更新
	void UpdateNormal(void) override;	// 通常状態の更新
	void UpdateResult(void) override;	// リザルト状態の更新
	void UpdateEnd(void) override;		// 終了状態の更新

	// メンバ変数
	vector<CPlayer*> m_apPlayer;	// プレイヤーの配列

	// 静的メンバ変数
};

#endif