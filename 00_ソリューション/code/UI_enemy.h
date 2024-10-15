//*****************************************************
//
// 敵UIの処理[UI_enemy.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _UI_ENEMY_H_
#define _UI_ENEMY_H_

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
class CUI_Enemy : public CGameObject
{
public:
	CUI_Enemy();	// コンストラクタ
	~CUI_Enemy();	// デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// 変数取得・設定関数
	int GetUI() { return m_nEnemy; }	// 敵の数
	void SetUI(int nUi) { m_nEnemy = nUi; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	void SetScaleNumber(float fScale);	// 数字のスケール
	float GetScaleNumber(void) { return m_fScaleNumber; }
	//void SetColor(E_Number number, D3DXCOLOR col);	// 色
	//D3DXCOLOR GetColor(E_Number number);

	// 静的メンバ関数
	static CUI_Enemy* Create(int m_nEnemy);	// 生成

private:
	// メンバ関数
	void UpdateNumber();	// 数字の更新
	void TransformNumber();	// 数字のトランスフォーム設定

	// メンバ変数
	int m_nEnemy;			// 現在のスコア
	float m_fScaleNumber;	// 数字のスケール
	vector<CNumber*> m_aNumber;	// 数字の配列
};

#endif
