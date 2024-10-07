//*****************************************************
//
// フレームの処理[frame.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _FRAME_H_	// 二重インクルード防止
#define _FRAME_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CFrame	: public CObject
{
public:
	CFrame(int nPriority = 0);	// コンストラクタ
	~CFrame();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CFrame *Create(int nTimeIn, int nTimeWait, int nTimeOut);	// 生成処理

private:
	// 列挙型定義
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// イン状態
		STATE_WAIT,	// 待機状態
		STATE_OUT,	// アウト状態
		STATE_MAX
	};

	// 構造体定義
	struct SInfo
	{// 自身の情報
		CPolygon2D *pUpper;
		CPolygon2D *pUnder;
		STATE state;
		int nTimeIn;
		int nTimeWait;
		int nTimeOut;
		int nCntFade;
	};

	// メンバ関数
	void ManageState(void);
	void ManageUpper(void);
	void ManageUnder(void);

	// メンバ変数
	SInfo m_info;
};

#endif