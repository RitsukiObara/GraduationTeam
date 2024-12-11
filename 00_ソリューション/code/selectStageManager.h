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
class CCollisionSphere;
class CObjectX;
class CSelectStagePenguin;
class CGameObject;
class CMeshCylinder;
class CFan3D;
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectStageManager : public CObject
{
public:
	// 列挙型定義
	enum E_StateStage
	{// ステージの状態
		STATE_NONE = 0,	// 何もしてない状態
		STATE_NORMAL,	// 通常状態
		STATE_SELECT,	// 選択状態
		STATE_MAX
	};
	// 構造体定義
	struct S_InfoStage
	{// ステージ情報
		CObjectX *pModel;				// Xモデルのポインタ
		string pathModel;				// モデルのパス
		string pathMap;					// マップのパス
		string pathEnemy;				// 敵のパス
		string pathName;				// ステージ名のパス
		int nDirStream;					// 海流の向き
		D3DXVECTOR3 pos;				// 位置
		CCollisionSphere *pCollision;	// 当たり判定
		E_StateStage state;				// 状態
		float fScaleDest;				// 目標のスケール
		CUI *pName;						// 名前のポインタ

		// コンストラクタ
		S_InfoStage() : pModel(nullptr), pos(), pCollision(nullptr), state(E_StateStage::STATE_NONE), fScaleDest(0.0f), pName(nullptr) {}
	};

	CSelectStageManager();	// コンストラクタ
	~CSelectStageManager() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 静的メンバ関数
	static void Load(void);						// 読み込み処理
	static void LoadMulti(void);				// マルチの読み込み処理
	static CSelectStageManager *Create(void);	// 生成処理
	static vector<S_InfoStage*> GetInfoStage(void) { return s_aInfoStage; }				// ステージ情報取得
	static vector<S_InfoStage*> GetInfoStageMulti(void) { return s_aInfoStageMulti; }	// マルチステージ情報取得

private:
	// メンバ関数
	void SetStage(void);						// ステージの設置
	void Select(void);							// 選択処理
	void Scaling(S_InfoStage *pInfoStage);		// スケーリング処理
	void FollowOcean(S_InfoStage *pInfoStage);	// 波に追従
	void SetParticle(int nIdx);					// パーティクルの発生

	void StartEnter(void);	// エンター開始
	void StayEnter(void);	// エンター中の処理
	void EndEnter(void);	// エンター終了
	void CollisionAdultWall(void);	// 大人の壁判定

	void ModeSelectBack(void);	// モードセレクトに戻る関数

	void Debug(void);	// デバッグ処理

	// 静的メンバ関数
	static void LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage);		// ステージ情報の読込

	// メンバ変数
	static vector<S_InfoStage*> s_aInfoStage;		// ステージ情報の配列
	static vector<S_InfoStage*> s_aInfoStageMulti;	// マルチステージ情報の配列
	CSelectStagePenguin *m_pPenguin;	// ペンギン
	bool m_bEnter;	// エンターしたフラグ
	float m_fTimerFade;	// フェードまでのタイマー
	int m_nIdxSelect;	// 選んだステージ番号
	vector<CGameObject*> m_aParticlePos;	// パーティクルの位置用オブジェクト
	CUI *m_pButtonUI;	// ボタンUI
};

#endif