//*****************************************************
//
// シングルゲームマネージャー[gameManagerSingle.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAMEMANAGERSINGLE_H_
#define _GAMEMANAGERSINGLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameManager.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CEnemyFct;

//*****************************************************
// クラスの定義
//*****************************************************
class CGameManagerSingle : public CGameManager
{
public:
	CGameManagerSingle();	// コンストラクタ
	~CGameManagerSingle() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update() override;
	void Draw() override;

	// 変数取得・設定関数

	// 静的メンバ関数

private:
	// メンバ関数
	void UpdateStart(void) override;	// 開始状態の更新
	void UpdateNormal(void) override;	// 通常状態の更新
	void ManagePlayer(void);			// プレイヤーの管理
	void DeathPlayer(void);				// プレイヤーの死亡
	void ManageEnemy(void);				// 敵の管理

	void UpdateResult(void) override;	// リザルト状態の更新
	void UpdateEnd(void) override;		// 終了状態の更新

	// メンバ変数
	CPlayer *m_pPlayer;		// プレイヤーのポインタ
	CEnemyFct *m_pEnemyFct;	// 敵ファクトリー

	// 静的メンバ変数
};

#endif