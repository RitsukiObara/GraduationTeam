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
	// 列挙型定義
	enum E_FlyDirection
	{// 流れる方向
		FLYDIRECTION_UP,	// 上
		FLYDIRECTION_RIGHT,	// 右
		FLYDIRECTION_DOWN,	// 下
		FLYDIRECTION_LEFT,	// 左
		FLYDIRECTION_MAX
	};

	CAlbatross(int nPriority = 4);	// コンストラクタ
	~CAlbatross();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ManageMotion(void);	// モーション管理

	// 静的メンバ関数
	static CAlbatross* Create(void);

private:
	enum E_Motion
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_FLY,		// 飛ぶ
		MOTION_MAX
	};

	// メンバ関数
	int m_DirStream;
	int m_DirStreamNext;
	E_FlyDirection m_FlyDirection;	// 飛ぶ方向
};

#endif
