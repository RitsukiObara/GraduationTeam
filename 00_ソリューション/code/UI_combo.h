//*****************************************************
//
// コンボUIの処理[UI_combo.h]
// Author:早川友貴
//
//*****************************************************

#ifndef _UI_COMBO_H_
#define _UI_COMBO_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"
#include "number3D.h"
#include "enemy.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CDestroyScore;

class CUI_Combo : public CGameObject
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_WAIT = 0,	// 滞留状態
		STATE_ERASE,	// 消していく
		STATE_MAX
	};

	CUI_Combo();	// コンストラクタ
	~CUI_Combo();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetCombo(int nDigit = 6);
	void AddCombo(CEnemy::TYPE type);
	void AddComboScore(void);	// コンボスコアを計算して加算

	// 静的メンバ関数
	void SetColor(D3DXCOLOR col);	// 色
	int GetCombo(void) { return m_nCombo; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール
	static CUI_Combo* GetInstance(void);	// インスタンスの取得

private:
	// メンバ関数
	static CUI_Combo* Create();	// 生成
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	CDestroyScore* m_pScore;	// スコア
	D3DXCOLOR m_Col;	//色管理
	int m_nValue;		//桁数
	int m_nCombo;			// 現在のコンボ
	float m_fScaleNumber;	// 数字のスケール
	CNumber3D* m_aNumber3D;	// 数字の配列
	E_State m_State;	//敵を倒した時のスコア状態変数
	int m_nCntState;	//状態管理数字
	D3DXVECTOR3 m_ShiftPos;	//ずらす位置

	static CUI_Combo* s_pCombo;	// 格納用の配列
};

#endif
