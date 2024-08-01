#include <imgui.h>
#include <Novice.h>

#include "MyFunctions.h"

const char kWindowTitle[] = "LC1A_13_クリハラ_ケント_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	ConicalPendulum conicalPendulum
	{
		{0,1.0f,0},
		0.8f,
		0.7f,
		0,
		0
	};

	Sphere sphere
	{
		{0,0,0},
		0.08f
	};

	Vector3 linePoint[2] =
	{
		{0,0,0},
		{0,0,0}
	};

	unsigned int color = WHITE;

	float deltaTime = 1.0f / 60.0f;

	bool isStart = false;

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	float cameraSpeed = 0.01f;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		if (keys[DIK_W])
		{
			cameraTranslate.z += cameraSpeed;
		}
		if (keys[DIK_S])
		{
			cameraTranslate.z -= cameraSpeed;
		}
		if (keys[DIK_A])
		{
			cameraTranslate.x -= cameraSpeed;
		}
		if (keys[DIK_D])
		{
			cameraTranslate.x += cameraSpeed;
		}
		if (keys[DIK_Q])
		{
			cameraTranslate.y += cameraSpeed;
		}
		if (keys[DIK_E])
		{
			cameraTranslate.y -= cameraSpeed;
		}

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, (float)kWindowWidth / (float)kWindowHeight, 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, (float)kWindowWidth, (float)kWindowHeight, 0.0f, 1.0f);

		if (isStart)
		{
			conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
		} else
		{
			conicalPendulum =
			{
				{0,1.0f,0},
				0.8f,
				0.7f,
				0,
				0
			};

			sphere.center = conicalPendulum.anchor;
		}
		conicalPendulum.angle = conicalPendulum.angle + conicalPendulum.angularVelocity * deltaTime;

		float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		sphere.center.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
		sphere.center.y = conicalPendulum.anchor.y - height;
		sphere.center.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;

		linePoint[0] = Transform(Transform({ 0,1.2f,0 }, viewProjectionMatrix), viewportMatrix);
		linePoint[1] = Transform(Transform(sphere.center, viewProjectionMatrix), viewportMatrix);


		ImGui::Begin("window");


		ImGui::Text("camera");
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::Text("setting");

		if (ImGui::Button("start"))
		{
			if (!isStart)
			{
				isStart = true;
			} else
			{
				isStart = false;
			}
		}


		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, color);
		Novice::DrawLine((int)linePoint[0].x, (int)linePoint[0].y, (int)linePoint[1].x, (int)linePoint[1].y, color);


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
