//*****************************************************
//
// 魚影の処理[fishshadow.h]
// Author:若木一真　髙山桃也
//
//*****************************************************
#ifndef _FISHSHADOW_H_
#define _FISHSHADOW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "anim3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace fishshadow
{
const float TIME_VANISH = 3.0f;								// 消滅までの時間
const float DELAY_AFTER_VANISH = 0.6f;						// 影が消えた後のディレイ
const float TIME_APPER = DELAY_AFTER_VANISH + TIME_VANISH;	// 魚影が出る時間
}

//*****************************************************
// クラス定義
//*****************************************************
class CFishShadow : public CAnim3D
{
public:
	CFishShadow(int nPriority = 3);	// コンストラクタ
	~CFishShadow();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CFishShadow* Create(int nPatern);

private:
	// メンバ関数
	void InitSpawn(int nPatern);	// スポーンの初期化
	void UpdateFadeIn(void);		// フェードイン状態
	void UpdateFadeOut(void);		// フェードアウト状態
	void Move(void);				// 移動処理

	// メンバ変数
	float m_fTimerVanish;	// 消えるまでのタイマー
	int m_nCntBubble;		// 泡カウンター
	D3DXVECTOR3 m_posInit;	// 初期位置
	D3DXVECTOR3 m_posDest;	// 目標位置
};

#endif