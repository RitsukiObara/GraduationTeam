//*****************************************************
//
// チュートリアルプレイヤーの処理[PlayerTutorial.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PlayerTutorial_H_
#define _PlayerTutorial_H_

//*****************************************************
// インクルード
//*****************************************************
#include "player.h"

//*****************************************************
// 前方宣言
//*****************************************************s

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayerTutorial : public CPlayer
{
public:
	CPlayerTutorial(int nPriority = 5);	// コンストラクタ
	~CPlayerTutorial();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPlayerTutorial* Create(void);										// 生成処理

private:
};

#endif