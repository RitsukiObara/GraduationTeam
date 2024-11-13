//*****************************************************
//
// シングルのランキング処理[rankingSingle.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULTSINGLELOSE_H_
#define _RESULTSINGLELOSE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "resultSingle.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラス定義
//*****************************************************
class CRankingSingle : public CObject
{
public:
	// 構造体定義
	struct S_InfoRank
	{// ランク情報
		int nScore;		// スコア
		int nTimePeck;	// 突いた回数
	};

	CRankingSingle();	// コンストラクタ
	~CRankingSingle();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(void);
	void Save(void);
	void Reset(void);
	void ReleaseArray(void);

	// 変数取得・設定関数
	vector<S_InfoRank*> GetRank(void) { return m_apRank; }	// ランク情報配列

	// 静的メンバ関数
	static CRankingSingle *Create(string pathSave = "data\\BYNARY\\ranking00.bin");	// 生成処理

private:
	string m_pathSave;	// 保存パス
	vector<S_InfoRank*> m_apRank;	// ランク情報配列
};

#endif