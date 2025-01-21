//*****************************************************
//
// アホウドリの処理[albatross.h]
// Author:若木一真
//
//*****************************************************
#ifndef _ALBATROSS_H_
#define _ALBATROSS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"
#include "iceManager.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CAlbatross : public CMotion
{
public:
	CAlbatross(int nPriority = 4);	// コンストラクタ
	~CAlbatross();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ManageMotion(void);	// モーション管理

	// 静的メンバ関数
	static CAlbatross* Create(COcean::E_Stream dir);

private:
	enum E_Motion
	{// モーション
		MOTION_FLY = 0,		// 飛ぶ
		MOTION_MAX
	};

	// メンバ関数
	void ChangeHead(void);	// 頭を変える
	void Stream(COcean::E_Stream dir);
	void CheckPlaySE(void);	// 鳴き声流す

	// メンバ変数
	D3DXVECTOR3 m_Move;		// 移動量
	float m_fPlaySETime;	// 鳴き声流す時間
};

#endif
