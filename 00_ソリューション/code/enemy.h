//*****************************************************
//
// 敵の処理[enemy.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
class CEnemy : public CMotion
{
public:
	CEnemy(int nPriority = 4);	// コンストラクタ
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEnemy* Create(void);
	//static std::vector<CEnemy*> GetInstance(void) { return m_Vector; }

private:
	// メンバ関数
	void Debug(void);	// デバッグ処理

	//// メンバ変数
	//std::vector<CEnemy*>::iterator m_Itr;
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号

	//// 静的メンバ変数
	//static std::vector<CEnemy*> m_Vector;	// 自身のポインタ
};

#endif