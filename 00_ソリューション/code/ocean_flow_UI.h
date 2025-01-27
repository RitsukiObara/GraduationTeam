//*****************************************************
//
// ステージリザルトUIの処理[stageResultUI.h]
// Author:若木一真
//
//*****************************************************
#ifndef _OCEAN_FLOW_UI_H_
#define _OCEAN_FLOW_UI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;

//*****************************************************
// クラス定義
//*****************************************************
class COceanFlowUI : public CObject
{
public:
	COceanFlowUI();	// コンストラクタ
	~COceanFlowUI();	// デストラクタ

	static COceanFlowUI* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static COceanFlowUI* GetInstance(void) { return s_pOceanFlowUI; }

private:
	// 列挙型定義
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// フェードイン状態
		STATE_OUT,	// フェードアウト状態
		STATE_MAX
	};

	// メンバ関数
	void CreateDir(void);			// 方向オブジェクト生成
	void CreateArrow(void);			// 矢印生成
	void CreateCompass(void);		// コンパス生成
	void OceanRotState(void);		// 海流の向き補正処理
	void OceanLevelState(void);		// 海流レベルのUI処理
	void ShakeArrow(float fRate);	// 矢印の揺れ

	CObjectX* m_pArrow;						// 矢印のモデル
	CObjectX* m_pCompass;					// コンパスのモデル
	CObject3D* m_pDir;						// 方向用のオブジェクト
	STATE m_state;							// 状態
	float m_fTimerShakeArrow;				// 矢印揺らすためのタイマー
	float m_fSpeedShakeArrow;				// 矢印揺れる速度
	static COceanFlowUI* s_pOceanFlowUI;	// 自身のポインタ
};

#endif