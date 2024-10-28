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
	CScore();	// コンストラクタ
	~CScore();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetData(int nDigit = 6);

	// 変数取得・設定関数
	int GetScore() { return m_nScore; }	// スコア
	void SetScore(int nScore) { m_nScore = nScore; }
	void AddScore(int nScore) { m_nScore += nScore; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール
	float GetScaleNumber(void) { return m_fScaleNumber; }
	void SetColor(D3DXCOLOR col);	// 色

	// 静的メンバ関数
	static CScore* Create();	// 生成
	static void SaveScore(int nSecond);	// スコア保存
	static int LoadScore(void);	// スコア読込

private:
	// メンバ関数
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	// メンバ変数
	int m_nScore;			// 現在のスコア
	float m_fScaleNumber;	// 数字のスケール
	vector<CNumber*> m_aNumber;	// 数字の配列
};

#endif