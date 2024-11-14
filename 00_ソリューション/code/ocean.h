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
#include "iceManager.h"

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
	CIceManager::E_Stream GetNowDirStream(void) { return (CIceManager::E_Stream)m_nRandKeep; }
	CIceManager::E_Stream GetNextDirStream(void) { return (CIceManager::E_Stream)m_nRandNextKeep; }

	void SetNextOceanRot(void);	// 次の海流の向き設定

private:

	// メンバ関数
	void OceanRotState(void); // 海流の向きとメッシュの向きを連動させる処理
	void OceanCycleTimer(void); // 海流周期を時間で管理する処理
	void OceanChangeCheck(void);	// 海流の向き変更時間か確認処理

	float m_fSpeed;	// 海流のスピード
	D3DXVECTOR3 m_fRot;	// 海流の向き
	static COcean* m_pOcean;	// 自身のポインタ
	int m_nRandKeep;	// ランダム関数の保存用
	int m_nRandNextKeep;	// 次のランダム関数の保存用
	int m_nSetRotTime;	// 向き変更をした時のタイマー時間
	int m_nExecRotChangeTime;	// 向き変更をするまでのタイマー時間
	bool m_bRandState;	// ランダム変数の状態
	bool m_bUse;	// アホウドリが使われているか
};
#endif