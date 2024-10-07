//*****************************************************
//
// ライトの処理[light.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_LIGHT				(3)			//ライトの最大数

//*****************************************************
// クラス定義
//*****************************************************
class CLight : public CObject
{
public:
	CLight();	// コンストラク
	~CLight();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void EnableLight(bool bFrag);	// ライト使用フラグ

	// 変数取得・設定関数
	void SetLightInfo(D3DLIGHT9 light);	// ライト情報
	D3DLIGHT9 GetLightInfo(void) { return m_light; }

	// 静的メンバ関数
	static CLight *Create(void);	// 生成処理
	static void ReleaseAll(void);	// 全ライト削除

private:
	// メンバ変数
	D3DLIGHT9 m_light;	// ライト情報
	int m_nID;	// 識別番号

	// 静的メンバ変数
	static int s_nNumAll;	// 総数
	static vector<CLight*> s_aLight;	// ライト情報の格納用配列
};

#endif