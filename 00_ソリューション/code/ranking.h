//*****************************************************
//
// ランキングの処理[Ranking.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RANKING_H_	// 二重インクルード防止
#define _RANKING_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CTimer;

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_RANK	(5) // ランキングの数

//*****************************************************
// クラスの定義
//*****************************************************
class CRanking : public CScene
{
public:
	// 列挙型定義
	enum E_STATE
	{//ニューレコード文字の状態
		STATE_NORMAL = 0,	//通常状態
		STATE_DIFF,	//色が変わっている状態
		STATE_MAX
	};

	CRanking();	// コンストラクタ
	~CRanking();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Reset(void);	// ランキングのリセット
	void Set(int nTime);	// ランキング設定
	void Save(void);	// 保存
	void Load(void);	// 読込

	static void SetTime(int nTime) { s_nTime = nTime; };	// タイムのセット

private:
	// メンバ関数
	void Sort(void);	// ソート処理

	// メンバ変数
	int m_nScore;	// スコアの値
	int m_nRankUpdate = -1;	// 更新ランクNo.
	int m_aScore[NUM_RANK];	// ランキングスコア
	E_STATE m_state = STATE_NORMAL;	// ニューレコードの点滅状態
	int m_nCntState;	// 状態遷移カウンタ
	int m_nTimerTrans;	// 画面遷移カウンター
	CTimer *m_apTimer[NUM_RANK];	// ランキングの数字

	// 静的メンバ変数
	static int s_nTime;	// タイム
};

#endif