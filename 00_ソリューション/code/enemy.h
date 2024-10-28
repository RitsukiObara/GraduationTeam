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
	// 列挙型定義
	typedef enum
	{// 種類
		TYPE_SEALS = 0,
		TYPE_MAX
	}TYPE;

	CEnemy(int nPriority = 4);	// コンストラクタ
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void TranslateByGrid(int nIdxV,int nIdxH);	// グリッドによる移動
	void FollowIce(void);	// 氷に追従

	// 変数取得・設定関数
	void SetGridV(int nValue) { m_nGridV = nValue; }	// グリッドの縦番号
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }	// グリッドの横番号
	int GetGridVH(void) { return m_nGridH; }

	// 静的メンバ関数
	static CEnemy* Create(int nType);	// 生成処理
	static std::vector<CEnemy*> GetArray(void) { return s_vector; }	// 配列の取得

private:
	// メンバ関数
	void InitGridIdx(void);	// グリッド番号の初期化
	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号

	// 静的メンバ変数
	static std::vector<CEnemy*> s_vector;	// 自身のポインタ
};

#endif