//*****************************************************
//
// オーシャンの処理[ocean.h]
// Author:早川友貴、若木一真
//
//*****************************************************

#ifndef _OCEAN_H_
#define _OCEAN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "meshfield.h"

//*****************************************************
// クラス定義
//*****************************************************
class COcean : public CMeshField
{
public:
	COcean();	// コンストラクタ
	~COcean();	// デストラクタ

	static COcean* Create(void);
	static COcean* GetInstance(void) { return m_pOcean; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数
	void SetRandState(bool nRandState) { m_nRandState = nRandState; }	// ランダム変数の設定

private:

	// メンバ関数
	void OceanRotState(void); // 海流の向きとメッシュの向きを連動させる処理
	void OceanCycleTimer(void); // 海流周期を時間で管理する処理

	float m_fSpeed;	// 海流のスピード
	D3DXVECTOR3 m_fRot;	// 海流の向き
	static COcean* m_pOcean;	// 自身のポインタ
	int m_nRandKeep;	// ランダム関数の保存用
	int m_nRandNextKeep;	// 次のランダム関数の保存用
	bool m_nRandState;	// ランダム変数の状態
};
#endif