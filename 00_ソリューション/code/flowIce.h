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
	vector<CIce*> GetIce(void) { return m_apIce; }	// 氷の取得
	
	// 静的メンバ関数
	static CFlowIce *Create(void);	// 生成処理
	static vector<CFlowIce*> GetInstance(void) { return s_vector; }	// インスタンス取得

private:
	// メンバ関数
	void CheckInAllIce(void);		// 全ての氷が画面に入ったかのチェック
	void CheckSomeIceStop(void);	// どれかの氷が止まっていないかのチェック
	void CollideOtherFlowIce(void);	// 他の流氷との判定
	void StopAllIce(void);			// 全ての氷を止める
	void StopOnTopObject(void);		// 上にあるオブジェクトを止める
	void DeleteAllIce(void);		// 全氷の削除
	void CheckDelete(void);			// 削除確認
	void Debug(void);				// デバッグ処理

	// メンバ変数
	vector<CIce*> m_apIce;	// 氷の配列
	float m_fTimerDelete;	// 削除タイマー
	bool m_bInScrnAllIce;	// 全ての氷がスクリーンに映ったフラグ

	// 静的メンバ変数
	static vector<CFlowIce*> s_vector;	// 格納用の配列
};

#endif

