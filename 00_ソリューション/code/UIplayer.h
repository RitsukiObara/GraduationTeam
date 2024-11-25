//*****************************************************
//
// プリヤーUIの処理[UIplayer.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UIPLAYER_H_
#define _UIPLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CUIPlayer : public CObject
{
public:
	CUIPlayer(int nPriority = 2);	// コンストラクタ
	~CUIPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 静的メンバ関数
	static CUIPlayer *Create();

private:
};

#endif

