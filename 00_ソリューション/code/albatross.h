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
	static CAlbatross* Create(CIceManager::E_Stream dir);

private:
	enum E_Motion
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_FLY,		// 飛ぶ
		MOTION_MAX
	};

	void Stream(CIceManager::E_Stream dir);

	D3DXVECTOR3 m_Move; // 移動量
};

#endif
