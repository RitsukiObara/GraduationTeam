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
#include "tutorial.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorialManager : public CObject
{
public:
	CTutorialManager();	// コンストラクタ
	~CTutorialManager() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void ChangeState(CTutorial::E_State stateNext);	// 状態の変更

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