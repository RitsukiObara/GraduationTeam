//*****************************************************
//
// ステージ選択管理処理[SelectStageManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _SELECTSTAGEMANAGER_H_
#define _SELECTSTAGEMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "selectStageManager.h"
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectStageManager : public CObject
{
public:
	CSelectStageManager();	// コンストラクタ
	~CSelectStageManager() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void);	// 読み込み処理

	// 静的メンバ関数
	static CSelectStageManager *Create(void);	// 生成処理

private:
	// 構造体定義
	struct S_InfoStage
	{// ステージ情報
		string pathModel;	// モデルのパス
		D3DXVECTOR3 pos;	// 位置
	};

	// メンバ関数
	void LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage);	// ステージ情報の読込
	void SetStage(void);	// ステージの設置
	void Debug(void);	// デバッグ処理

	// メンバ変数
	vector<S_InfoStage*> m_aInfoStage;	// ステージ情報の配列

	// 静的メンバ変数
};

#endif