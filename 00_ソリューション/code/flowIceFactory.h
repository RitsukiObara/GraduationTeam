//*****************************************************
//
// 流氷ファクトリー[flowIceFactory.h]
// Author:��山桃也
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
	// メンバ関数
	void CreateFlowIce(void);	// 流氷の生成

	// メンバ変数
	float m_fTimerCreateFlowIce;	// 流氷製造タイマー
};

#endif

