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
#include "number3D.h"
#include "score.h"
#include "enemy.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CDestroyScore : public CGameObject
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
	void SetEnemyScore(CEnemy::TYPE type);	//敵の種類ごとのスコア
	void SetScore(int nDigit = 6);
	void SetColor(D3DXCOLOR col);	// 色
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール

private:
	// メンバ関数
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	D3DXCOLOR m_Col;	//色管理
	int m_nValue;		//桁数
	int m_nScore;			// 現在のコンボ
	float m_fScaleNumber;	// 数字のスケール
	CNumber3D* m_aNumber3D;	// 数字の配列
	E_State m_State;	//敵を倒した時のスコア状態変数
	int m_nCntState;	//状態管理数字
};

#endif
