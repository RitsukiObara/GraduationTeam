//*****************************************************
//
// 流氷の処理[flowIce.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _FLOWICE_H_
#define _FLOWICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CIce;

//*****************************************************
// クラスの定義
//*****************************************************
class CFlowIce : public CObject
{
public:
	CFlowIce(int nPriority = 2);	// コンストラクタ
	~CFlowIce();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 変数設定・取得処理
	void AddIceToArray(CIce *pIce);

	// 静的メンバ関数
	static CFlowIce *Create(void);	// 生成処理

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

