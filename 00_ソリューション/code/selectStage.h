//*****************************************************
//
// ステージ選択処理[selectStage.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _SELECTSTAGE_H_
#define _SELECTSTAGE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectStage : public CScene
{
public:
	CSelectStage();	// コンストラクタ
	~CSelectStage() {};	// デストラクタ

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	// メンバ関数
	void UpdateCamera(void);
	void Debug(void);

	// メンバ変数

	// 静的メンバ変数
};

#endif