//*****************************************************
//
// 敵を倒した時のスコア処理[destroy_score.h]
// Author:若木一真
//
//*****************************************************
#ifndef _DESTROY_SCORE_H_
#define _DESTROY_SCORE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"
#include "number.h"
#include "score.h"
#include "enemy.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CDestroyScore : public CScore
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_BESIDE = 0,	// 横移動
		STATE_VERTICAL,	// 縦移動
		STATE_WAIT,	// 滞留状態
		STATE_ADD,	// スコアに加算
		STATE_MAX
	};

	CDestroyScore();	// コンストラクタ
	~CDestroyScore();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// 静的メンバ関数
	static CDestroyScore* Create(CEnemy::TYPE type);	// 生成

private:
	void SetEnemyScore(CEnemy::TYPE type);	//敵の種類ごとのスコア

	E_State m_State;	//敵を倒した時のスコア状態変数
	int m_nCntState;	//状態管理数字
	D3DXCOLOR m_Col;	//色管理
	int m_nScore;		//スコア数値
	int m_nValue;		//桁数
};

#endif
