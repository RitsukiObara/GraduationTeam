//===========================================================
//
// マイエフェクシア[MyEffekseer.cpp]
// Author : 髙山桃也
//
//===========================================================

//***********************************************************
// インクルード
//***********************************************************
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>
#include <locale>
#include <string>

#include "manager.h"
#include "renderer.h"
#include "MyEffekseer.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"

//***********************************************************
// 静的メンバ変数宣言
//***********************************************************
const char* CMyEffekseer::m_apEfkName[CMyEffekseer::TYPE_MAX] =		// エフェクトのパス
{
	"",												// なんもない
	"data\\EFFEKSEER\\Effect\\peckWave.efkefc",		// つっつきの波紋
	"data\\EFFEKSEER\\Effect\\splashwater.efkefc",	// さざ波
	"data\\EFFEKSEER\\Effect\\ripplesring.efkefc",	// 波紋
	"data\\EFFEKSEER\\Effect\\bearstep.efkefc",		// シロクマの足煙
	"data\\EFFEKSEER\\Effect\\blizzard.efkefc",		// 吹雪
	"data\\EFFEKSEER\\Effect\\icebreak.efkefc",		// 氷の破壊
	"data\\EFFEKSEER\\Effect\\jump.efkefc",			// ジャンプ
	"data\\EFFEKSEER\\Effect\\drop.efkefc",			// 水しぶき
};
CMyEffekseer *CMyEffekseer::s_pMyEffekseer = nullptr;	// 自身のポインタ

//===========================================================
// コンストラクタ
//===========================================================
CMyEffekseer::CMyEffekseer()
{
	m_nNum = 0;
}

//===========================================================
// デストラクタ
//===========================================================
CMyEffekseer::~CMyEffekseer()
{

}

//===========================================================
// 生成処理
//===========================================================
CMyEffekseer *CMyEffekseer::Create(void)
{
	if (s_pMyEffekseer == nullptr)
	{
		s_pMyEffekseer = new CMyEffekseer;

		if (s_pMyEffekseer != nullptr)
			s_pMyEffekseer->Init();
	}

	return s_pMyEffekseer;
}

//===========================================================
// 初期化処理
//===========================================================
void CMyEffekseer::Init(void)
{
	// エフェクトのマネージャーの作成
	m_efkManager = ::Effekseer::Manager::Create(8000);

	// 視点位置を確定
	m_viewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// 座標系を設定する。アプリケーションと一致させる必要がある。
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Effekseerのモジュールをセットアップする
	SetupEffekseerModules(m_efkManager);

	// 投影行列を設定
	m_projectionMatrix.PerspectiveFovLH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// カメラ行列を設定
	m_cameraMatrix.LookAtLH(m_viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// 終了処理
//===========================================================
void CMyEffekseer::Uninit(void)
{
	for (auto it = m_listEffect.begin(); it != m_listEffect.end();)
	{
		auto itNext = std::next(it);

		if (itNext == m_listEffect.end())
			break;

		ReleaseEffect((*it));

		it = itNext;
	}

	m_nNum = 0;

	delete this;
}

//===========================================================
// 更新処理
//===========================================================
void CMyEffekseer::Update(void)
{
	// レイヤーパラメータの設定
	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = m_viewerPosition;
	m_efkManager->SetLayerParameter(0, layerParameter);

	// マネージャーの更新
	Effekseer::Manager::UpdateParameter updateParameter;
	m_efkManager->Update(updateParameter);

	for (auto it = m_listEffect.begin(); it != m_listEffect.end(); )
	{
		// エフェクトの移動
		Effekseer::Handle handle = (*it)->GetHandle();
		Effekseer::Vector3D pos = (*it)->GetPositionVector3();
		m_efkManager->SetLocation(handle, pos);

		// エフェクトの向き反映
		Effekseer::Vector3D rot = (*it)->GetRotation();
		m_efkManager->SetRotation(handle, rot.X, rot.Y, rot.Z);

		// エフェクトのスケール反映
		Effekseer::Vector3D scale = (*it)->GetScale();
		m_efkManager->SetScale(handle, scale.X, scale.Y, scale.Z);

		// 停止フラグの取得
		bool bStop = false;
		CGame *pGame = CGame::GetInstance();
		if (pGame != nullptr)
			bStop = pGame->GetStop();

		if (bStop)
		{
			// 再生時間を停止する
			m_efkManager->SetPaused(handle, true);
		}
		else
		{
			if (it == m_listEffect.begin())
			{
				// 再生時間を進める
				int32_t time = (*it)->GetTime();
				time++;
				(*it)->SetTime(time);

				m_efkManager->SetPaused(handle, false); // 再生を再開
			}
		}

		// 毎フレーム、エフェクトが再生終了しているか確認する
		if (m_efkManager->Exists(handle) == false)
		{
			(*it)->Uninit();
			it = m_listEffect.erase(it);  // リストから要素を削除し、イテレータを更新
		}
		else
		{
			++it;  // エフェクトがまだ存在する場合は次の要素へ進む
		}
	}
}

//===========================================================
// 描画処理
//===========================================================
void CMyEffekseer::Draw(void)
{
	if (m_efkRenderer != nullptr)
	{
		// 投影行列を設定
		m_efkRenderer->SetProjectionMatrix(m_projectionMatrix);

		// カメラの情報取得
		CCamera* pCamera = CManager::GetCamera();

		// ビューマトリックス
		auto ViewMatrix = pCamera->GetCamera()->mtxView;

		// プロジェクションマトリックス
		auto Projection = pCamera->GetCamera()->mtxProjection;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_projectionMatrix.Values[i][j] = Projection.m[i][j];
				m_cameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
			}
		}

		// カメラ行列を設定
		m_efkRenderer->SetCameraMatrix(m_cameraMatrix);

		// エフェクトの描画開始処理を行う。
		m_efkRenderer->BeginRendering();

		// エフェクトの描画を行う。
		Effekseer::Manager::DrawParameter drawParameter;
		drawParameter.ZNear = 0.0f;
		drawParameter.ZFar = 1.0f;
		drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();
		m_efkManager->Draw(drawParameter);

		// エフェクトの描画終了処理を行う。
		m_efkRenderer->EndRendering();
	}
}

//===========================================================
// エフェクトの生成
//===========================================================
CEffekseerEffect *CMyEffekseer::CreateEffect(const char* FileName, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale)
{
	CEffekseerEffect *pEffect = nullptr;

	pEffect = new CEffekseerEffect;

	if (pEffect == nullptr)
		return nullptr;

	// エフェクトの読込
	// char16_tに変換
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(FileName);

	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_efkManager, string16t.c_str());

	pEffect->SetEffect(effect);
	
	// エフェクトの再生
	Effekseer::Handle handle = m_efkManager->Play(effect, 0, 0, 0);

	pEffect->SetHandle(handle);

	pEffect->Init(pos, rot, scale);

	m_listEffect.push_back(pEffect);

	return pEffect;
}

//===========================================================
// モジュール設定
//===========================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// 描画デバイスの作成
	::Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CRenderer::GetInstance()->GetDevice());

	// エフェクトのレンダラーの作成
	m_efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================
// エフェクトのリリース
//===========================================================
void CMyEffekseer::Release(int idx)
{

}

//===========================================================
// エフェクトのリリース
//===========================================================
void CMyEffekseer::ReleaseEffect(CEffekseerEffect *pEffect)
{
	if (pEffect == nullptr)
		return;

	// エフェクトの終了処理
	pEffect->Uninit();

	// リストから除外
	m_listEffect.remove(pEffect);
}

//===========================================================
// エフェクトファイルのパス取得
//===========================================================
const char* CMyEffekseer::GetPathEffect(CMyEffekseer::TYPE type)
{
	if (type <= TYPE::TYPE_NONE || type >= TYPE::TYPE_MAX)
		return nullptr;

	return m_apEfkName[type];
}

//**************************************************************************************
// エフェクトクラス
//**************************************************************************************
//===========================================================
// コンストラクタ
//===========================================================
CEffekseerEffect::CEffekseerEffect(int nPriority)
{

}

//===========================================================
// デストラクタ
//===========================================================
CEffekseerEffect::~CEffekseerEffect()
{

}

//===========================================================
// 初期化
//===========================================================
HRESULT CEffekseerEffect::Init(::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale)
{
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	m_time = 0;

	CMyEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return E_FAIL;

	Effekseer::ManagerRef manager = pEffekseer->GetEfkManager();

	// 位置、向き、大きさ設定
	manager->SetLocation(m_efkHandle, m_pos);
	manager->SetRotation(m_efkHandle, { 0.0f, 1.0f, 0.0f }, rot.Y);
	manager->SetScale(m_efkHandle, m_scale.X, m_scale.Y, m_scale.Z);

	return S_OK;
}

//===========================================================
// 終了
//===========================================================
void CEffekseerEffect::Uninit()
{
	CMyEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return;

	Effekseer::ManagerRef efkManager = pEffekseer->GetEfkManager();

	// エフェクトの解放
	efkManager->StopEffect(m_efkHandle);
}

//===========================================================
// 更新処理
//===========================================================
void CEffekseerEffect::Update(void)
{

}

//===========================================================
// 描画
//===========================================================
void CEffekseerEffect::Draw(void)
{
	//CMyEffekseer *pEffekseer = CManager::GetMyEffekseer();

	//if (pEffekseer == nullptr)
	//	return;

	//EffekseerRendererDX9::RendererRef renderer = pEffekseer->GetEffekseerRenderer();
	//Effekseer::ManagerRef manager = pEffekseer->GetEfkManager();

	//LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	////-----------------------------------------
	//// カメラの設定
	////-----------------------------------------
	//// 投影行列を設定
	//renderer->SetProjectionMatrix(m_projectionMatrix);

	//// カメラの情報取得
	//CCamera* pCamera = CManager::GetCamera();

	//// ビューマトリックス
	//auto ViewMatrix = pCamera->GetCamera()->mtxView;

	//// プロジェクションマトリックス
	//auto Projection = pCamera->GetCamera()->mtxProjection;

	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		m_projectionMatrix.Values[i][j] = Projection.m[i][j];
	//		m_cameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
	//	}
	//}

	//// カメラ行列を設定
	//renderer->SetCameraMatrix(m_cameraMatrix);

	////-----------------------------------------
	//// 描画
	////-----------------------------------------
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//// エフェクトの描画開始処理を行う。
	//renderer->BeginRendering();

	//// エフェクトの描画を行う。
	//Effekseer::Manager::DrawParameter drawParameter;
	//drawParameter.ZNear = 0.0f;
	//drawParameter.ZFar = 1.0f;
	//drawParameter.ViewProjectionMatrix = renderer->GetCameraProjectionMatrix();
	//manager->Draw(drawParameter);

	//// エフェクトの描画終了処理を行う。
	//renderer->EndRendering();

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//===========================================================
// 位置の追従
//===========================================================
CEffekseerEffect *CEffekseerEffect::FollowPosition(D3DXVECTOR3 pos)
{
	CMyEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return nullptr;

	Effekseer::ManagerRef efkManager = pEffekseer->GetEfkManager();

	// 毎フレーム、エフェクトが再生終了しているか確認する
	if (efkManager->Exists(m_efkHandle) == false)
	{
		return nullptr;
	}

	SetPosition(Effekseer::Vector3D(pos.x, pos.y, pos.z));

	return this;
}

namespace MyEffekseer
{
CEffekseerEffect *CreateEffect(CMyEffekseer::TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CMyEffekseer *pEffekseer = CMyEffekseer::GetInstance();

	if (pEffekseer == nullptr)
		return nullptr;

	// パスの取得
	const char* pPath = pEffekseer->GetPathEffect(type);

	if (pPath == nullptr)
		return nullptr;

	CEffekseerEffect *pEffect = pEffekseer->CreateEffect(pPath, Effekseer::Vector3D(pos.x, pos.y, pos.z), Effekseer::Vector3D(rot.x, rot.y, rot.z), Effekseer::Vector3D(scale.x, scale.y, scale.z));

	return pEffect;
}
}