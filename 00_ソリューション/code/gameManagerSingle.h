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

	// メンバ変数

	// 静的メンバ変数
};

#endif