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
#include "UI_combo.h"

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

	CDestroyScore();	// コンストラクタ
	~CDestroyScore();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void AddDestroyScore(CEnemy::TYPE type);
	void AddComboScore(void);	// コンボスコアを計算して加算

	// 静的メンバ関数
	static CDestroyScore* Create();	// 生成

	void SetEnemyScore(CEnemy::TYPE type);	//敵の種類ごとのスコア
	void SetScore(int nDigit = 6);
	void SetColor(D3DXCOLOR col);	// 色
	void SetState(CUI_Combo::E_State state);	// 状態
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール

private:
	// メンバ関数
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	D3DXCOLOR m_Col;	//色管理
	int m_nValue;		//桁数
	int m_nScore;			// 現在の敵を倒した時のスコア
	int m_nAddScore;			// 現在の敵を倒した時の追加されるスコア
	float m_fScaleNumber;	// 数字のスケール
	CNumber3D* m_aNumber3D;	// 数字の配列
	CUI_Combo::E_State m_state;	//敵を倒した時のスコア状態変数
	D3DXVECTOR3 m_ShiftPos;	//ずらす位置
};

#endif
