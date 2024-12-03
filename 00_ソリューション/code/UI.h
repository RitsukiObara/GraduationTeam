//*****************************************************
//
// UIの処理[UI.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon2D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CUI	: public CPolygon2D
{
public:
	CUI(int nPriority = 6);	// コンストラクタ
	~CUI() override;		//	デストラクタ

	// メンバ関数
	static CUI *Create(void);	// 生成処理
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	// 変数取得・設定関数
	void SetSize(float width, float height) override;	// サイズ
	D3DXVECTOR2 GetSize(void) override;
	void SetWidth(float width) override;	// 幅
	void SetHeight(float height) override;	// 高さ
	float GetWidth(void) override;
	float GetHeight(void) override;
	virtual void SetPosition(D3DXVECTOR3 pos);	// 位置
	virtual D3DXVECTOR3 GetPosition(void);

private:
};

#endif