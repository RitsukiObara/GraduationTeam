//*****************************************************
//
// スカイボックスの処理[skybox.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSkybox : public CObjectX
{
public:
	CSkybox(int nPriority = 2);	// コンストラクタ
	~CSkybox();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数
	void EnableFollowPlayer(bool bFollowPlayer) { m_bFollowPlayer = bFollowPlayer; }	// プレイヤー追従フラグ

	// 静的メンバ関数
	static CSkybox *Create();
	static CSkybox *GetInstance() { return s_pSkybox; }	// インスタンスの取得

private:
	// メンバ関数

	// メンバ変数
	bool m_bFollowPlayer;	// プレイヤー追従フラグ

	// 静的メンバ変数
	static CSkybox *s_pSkybox;	// 自身のポインタ
};

#endif

