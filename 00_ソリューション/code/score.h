//*****************************************************
//
// スコアの処理[Score.h]
// Author:若木一真
//
//*****************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"
#include "number.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CScore : public CGameObject
{
public:
	// 列挙型定義
	enum E_Number
	{// 数字の種類
		NUMBER_MINUTE = 0,	// 分
		NUMBER_SECOND,	// 秒
		NUMBER_MILLI,	// ミリ秒
		NUMBER_MAX
	};

	CScore();	// コンストラクタ
	~CScore();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// 変数取得・設定関数
	float GetScore() { return m_fScore; }	// スコア
	void SetScore(float fScore) { m_fScore = fScore; }
	void AddScore(float fScore) { m_fScore += fScore; }
	bool GetFlag() { return m_bStop; }	// 動作フラグ
	void SetFlag(bool bStop) { m_bStop = bStop; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール
	float GetScaleNumber(void) { return m_fScaleNumber; }
	void SetColor(E_Number number, D3DXCOLOR col);	// 色
	D3DXCOLOR GetColor(E_Number number);

	// 静的メンバ関数
	static CScore* Create();	// 生成
	static void SaveScore(float fSecond);	// スコア保存
	static float LoadScore(void);	// スコア読込

private:
	// メンバ関数
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	// メンバ変数
	float m_fScore;			// 現在のスコア
	float m_fScaleNumber;	// 数字のスケール
	bool m_bStop;				// タイマー停止のフラグ
	vector<CNumber*> m_aNumber;	// 数字の配列
};

#endif