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
	static COceanFlowUI* GetInstance(void) { return m_pOceanFlowUI; }

private:

	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// フェードイン状態
		STATE_OUT,	// フェードアウト状態
		STATE_MAX
	};

	void OceanLevelState(void); // 海流レベルのUI処理

	CObjectX* m_pArrow;	// 矢印のモデル
	STATE m_state;	// 状態
	int OceanFlowKeep;	//	海流の保存
	float OceanFlowLevel;	//	海流レベルの保存
	static COceanFlowUI* m_pOceanFlowUI;	// 自身のポインタ
};

#endif