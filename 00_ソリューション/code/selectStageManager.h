//*****************************************************
//
// ステージ選択管理処理[SelectStageManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _SELECTSTAGEMANAGER_H_
#define _SELECTSTAGEMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "selectStageManager.h"
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectStageManager : public CObject
{
public:
	CSelectStageManager();	// コンストラクタ
	~CSelectStageManager() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void);	// 読み込み処理

	// 静的メンバ関数
	static CSelectStageManager *Create(void);	// 生成処理

private:
	// メンバ関数

	// メンバ変数

	// 静的メンバ変数
};

#endif