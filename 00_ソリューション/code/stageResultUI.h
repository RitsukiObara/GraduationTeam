//*****************************************************
//
// ステージリザルトUIの処理[stageResultUI.h]
// Author:若木一真
//
//*****************************************************
#ifndef _STAGERESULTUI_H_
#define _STAGERESULTUI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon2D;

//*****************************************************
// クラス定義
//*****************************************************
class CStageResultUI : public CObject
{
public:
	CStageResultUI();	// コンストラクタ
	~CStageResultUI();	// デストラクタ

	static CStageResultUI* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CStageResultUI* GetInstance(void) { return m_pStageResultUI; }

private:

	// リザルトの種類
	enum RESULT
	{
		RESULT_CLEAR = 0,	// クリア
		RESULT_FAIL,	// 失敗
		RESULT_MAX
	};

	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// フェードイン状態
		STATE_OUT,	// フェードアウト状態
		STATE_MAX
	};

	void ResultState(void); // リザルトUIの状態
	void ResultClear(void); // リザルトがクリアの時
	void ResultFail(void); // リザルトが失敗の時
	void Fade(RESULT RESULT);

	RESULT m_Result;	// 選択項目
	CPolygon2D* m_apResult[RESULT_MAX];	// ステージリザルトの2Dオブジェクト
	D3DXVECTOR3 m_aPosDest[RESULT_MAX];	// 目標の位置
	STATE m_state;	// 状態
	static CStageResultUI* m_pStageResultUI;	// 自身のポインタ
	bool m_bSound;
	int nCountMove;
};

#endif