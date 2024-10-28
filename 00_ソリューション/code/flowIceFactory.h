//*****************************************************
//
// 流氷ファクトリー[flowIceFactory.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _FLOWICEFCT_H_
#define _FLOWICEFCT_H_

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
class CFlowIceFct : public CObject
{
public:
	CFlowIceFct(int nPriority = 2);	// コンストラクタ
	~CFlowIceFct();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CFlowIceFct *Create(void);	// 生成処理
private:
	// 構造体定義
	struct S_InfoFlowIce
	{// 流氷の情報
		vector<vector<int>> aIdx;	// 番号の保存
	};

	// メンバ関数
	void Load(void);	// 読込
	void GetNumFlowIce(std::ifstream& file, string str, int *pNumV, int *pNumH, S_InfoFlowIce* pInfoFlowIce);
	void CreateFlowIce(void);	// 流氷の生成

	// メンバ変数
	float m_fTimerCreateFlowIce;	// 流氷製造タイマー
	vector<S_InfoFlowIce*> m_apInfoFlowIce;	// 流氷の情報配列
};

#endif

