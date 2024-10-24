//*****************************************************
//
// 流氷の処理[flowIce.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ICESPEEDMANAGER_H_
#define _ICESPEEDMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CIce;

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_ICEFLOW	(40)	// 流れてくる氷の最大数

//*****************************************************
// クラスの定義
//*****************************************************
class CIceSpeedManager : public CObject
{
public:
	CIceSpeedManager(int nPriority = 2);	// コンストラクタ
	~CIceSpeedManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Load(char* pPath);

	// 変数設定・取得処理
	void AddIceToArray(CIce* pIce);

	// 静的メンバ関数
	static CIceSpeedManager* Create(void);	// 生成処理

private:
	// メンバ関数
	void CreateIce(void);	// 氷の生成
	void CheckSomeIceStop(void);	// どれかの氷が止まっていないかのチェック
	void StopAllIce(void);	// 全ての氷を止める
	void Debug(void);	// デバッグ処理

	// メンバ変数
	vector<CIce*> m_apIce;	// 氷の配列
};

#endif