//*****************************************************
//
// 敵ファクトリー[enemyfactory.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ENEMYFCT_H_
#define _ENEMYFCT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyFct : public CObject
{
public:
	CEnemyFct(int nPriority = 2);	// コンストラクタ
	~CEnemyFct();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(string path);	// 読込
	int GetNumEnemyInfo(void) { return (int)m_apInfoEnemy.size(); }	// 敵数取得

	// 静的メンバ関数
	static CEnemyFct *Create(void);	// 生成処理
private:
	// 構造体定義
	struct S_InfoEnemy
	{// 敵情報
		float fDelaySpawn;	// スポーンディレイ
		int nType;			// 種類
		int nPaternSpawn;	// スポーンパターン

		S_InfoEnemy::S_InfoEnemy() : fDelaySpawn(0.0f), nType(0), nPaternSpawn(0) {}
	};

	// メンバ関数
	void LoadInfo(std::ifstream& file, string str,S_InfoEnemy *pInfoEnemy);	// 情報の読込
	void SetEnemy(void);	// 敵の設定

	// メンバ変数
	float m_fTimerSpawn;				// スポーンタイマー
	vector<S_InfoEnemy*> m_apInfoEnemy;	// 敵の情報配列
};

#endif