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
	// 列挙型定義
	enum E_Stream
	{// 流れる方向
		STREAM_UP,	// 上
		STREAM_RIGHT,	// 右
		STREAM_DOWN,	// 下
		STREAM_LEFT,	// 左
		STREAM_MAX
	};

	COcean(int nPriority = 3);	// コンストラクタ
	~COcean();	// デストラクタ

	static COcean* Create(void);
	static COcean* GetInstance(void) { return m_pOcean; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数
	E_Stream GetNowDirStream(void) { return (COcean::E_Stream)m_nRandKeep; }
	E_Stream GetNextDirStream(void) { return (COcean::E_Stream)m_nRandNextKeep; }

	void SetNextOceanRot(void);	// 次の海流の向き設定

private:

	// メンバ関数
	void BgIceRotState(void); // 海流の向きとメッシュの向きを連動させる処理
	void OceanCycleTimer(void); // 海流周期を時間で管理する処理
	void OceanChangeCheck(void);	// 海流の向き変更時間か確認処理
	void BgIceSetPosUp(void);	// 上方向から出てくる背景氷
	void BgIceSetPosDown(void);	// 下方向から出てくる背景氷
	void BgIceSetPosL(void);	// 左方向から出てくる背景氷
	void BgIceSetPosR(void);	// 右方向から出てくる背景氷
	float m_fSpeed;	// 海流のスピード
	D3DXVECTOR3 m_fRot;	// 海流の向き
	static COcean* m_pOcean;	// 自身のポインタ
	int m_nRandKeep;	// ランダム関数の保存用
	int m_nRandNextKeep;	// 次のランダム関数の保存用
	float m_fProgressTime;	// 経過時間
	float m_fRotChangeTime;	// 向き変更をするまでの時間
	bool m_bRandState;	// ランダム変数の状態
	int m_nBgiceCnt;	// 背景の氷が呼び出されるカウント(左)
};
#endif