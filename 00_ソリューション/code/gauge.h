//*****************************************************
//
// ゲージの処理[gauge.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CGauge : public CGameObject
{
public:
	CGauge();	// コンストラクタ
	~CGauge();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化	
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void AddParam(float fValue);	// パラメーター加算処理

	// 変数取得・設定関数
	void SetParam(float fParam) { m_fParam = fParam; }	// パラメーター
	float GetParam(void) { return m_fParam; }
	void SetPosition(D3DXVECTOR3 pos) override;	// 位置
	CUI *GetGauge(void) { return m_aPolygon[E_Polygon::POLYGON_GAUGE]; }	// ゲージポリゴン
	void SetGauge(CUI *pUI) { m_aPolygon[E_Polygon::POLYGON_GAUGE] = pUI; }

	// 静的メンバ関数
	static CGauge *Create(float fParamMax);	// 生成処理

private:
	// 列挙型定義
	enum E_Polygon
	{// ポリゴンの種類
		POLYGON_FRAME = 0,	// フレーム
		POLYGON_GAUGE,	// ゲージ
		POLYGON_MAX
	};

	// メンバ関数
	void CreatePolygon(void);	// ポリゴンの生成
	void ManageGaugePolygon(void);	// ゲージポリゴンの管理

	// メンバ変数
	vector<CUI*> m_aPolygon;	// 2Dポリゴンの配列
	float m_fParam;	// パラメーターの現在の値
	float m_fParamMax;	// パラメーターの最大値
	float m_fTimerTransGauge;	// ゲージ変動のタイマー
	float m_fTimeLimitTransGauge;	// ゲージ変動のタイムリミット
	float m_fWidthOld;	// 前回の幅
	D3DXVECTOR2 m_sizeGauge;	// ゲージのサイズ
};

#endif