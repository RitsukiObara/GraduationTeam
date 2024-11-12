//*****************************************************
//
// ゲームマネージャー[gameManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "game.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CTimer;

//*****************************************************
// クラスの定義
//*****************************************************
class CGameManager : public CObject
{
public:
	CGameManager();	// コンストラクタ
	~CGameManager() {};	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

	// 変数取得・設定関数

	// 静的メンバ関数
	static CGameManager *Create(CGame::E_GameMode mode);	// 生成処理

private:
	// 関数ポインタ型の定義
	typedef void (CGameManager::*StateFunc)(void);	// 状態ごとの更新

	// メンバ関数
	virtual void ManageGameState(void);	// ゲームの状態管理
	virtual void UpdateStart(void);		// 開始状態の更新
	virtual void UpdateNormal(void);	// 通常状態の更新
	virtual void UpdateResult(void);	// リザルト状態の更新
	virtual void UpdateEnd(void);		// 終了状態の更新

	// メンバ変数

	// 静的メンバ変数
};

namespace gameManager
{
void SaveMode(CGame::E_GameMode mode, vector<bool> abPlayerEnter);	// モード保存
void LoadMode(CGame::E_GameMode *pMode, vector<bool> &rbPlayerEnter);	// モード読込
}

#endif