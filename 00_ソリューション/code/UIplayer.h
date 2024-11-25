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
// 前方宣言
//*****************************************************
class CUI;

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

	// 取得・設定関数
	CUI *GetIcon(int nIdx) { return m_apIconPlayer[nIdx]; }	// アイコンの取得

	// 静的メンバ関数
	static CUIPlayer *Create();

private:
	CUI *m_apIconPlayer[NUM_PLAYER];	// プレイヤーアイコン
};

#endif

