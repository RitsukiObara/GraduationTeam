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
};

#endif