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
#include "tutorial.h"

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
	static CPlayerTutorial *Create(void);	// 生成処理

private:
	// メンバ関数
	void CheckSpeed(void);						// スピードの確認
	void ReadExplain(void);						// 説明進む処理
	void AddProgress(CTutorial::E_State state);	// 進行させる処理
	bool Peck(void) override;					// 突っつく処理
	void StartJump(void) override;				// ジャンプの開始

	// メンバ変数
	float m_fTimerMove;	// 移動カウンター
	bool m_bComplete;	// チュートリアルを終えたかの判定
};

#endif