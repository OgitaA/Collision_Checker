﻿# include <Siv3D.hpp> // Siv3D v0.6.12
#include"Checker.hpp"

void Main()
{
	Window::SetTitle(U"Collision_Checker");

	Scene::SetResizeMode(ResizeMode::Keep);

	// シーンをリサイズ
	Scene::Resize(1500, 720);

	Checker checker;
	checker.init();


	while (System::Update())
	{
		checker.update();
		checker.draw();
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
