//*****************************************************
//
// チュートリアルマネージャー[tutorialManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TUTORIALMANAGER_H_
#define _TUTORIALMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorialManager : public CObject
{
public:
	CTutorialManager();	// コンストラクタ
	~CTutorialManager() {};	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 静的メンバ関数
	static CTutorialManager *Create(void);	// 生成処理

private:
	// メンバ関数
	void UpdateMove(void);		// 移動状態
	void UpdatePeck(void);		// 突っつき状態
	void UpdateBreak(void);		// 破壊状態
	void UpdateExplain(void);	// 説明状態

	// 関数ポインタ型を定義
	typedef void (CTutorialManager::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif