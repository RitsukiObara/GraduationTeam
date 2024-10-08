//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }

private:
	// メンバ関数
	void Input(void);	// 入力
	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号

	// 静的メンバ変数
	static CPlayer *m_pPlayer;	// 自身のポインタ
};

#endif