//*****************************************************
//
// ゲームマネージャー[gameManager.h]
// Author:��山桃也
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
	// メンバ関数

	// メンバ変数

	// 静的メンバ変数
};

#endif