#pragma once

#include"My_Button.hpp"

struct Checker {

	Texture m_texture;

	struct C_Rect {

		C_Rect(Rect set_rect,double set_hue) {

			rect = set_rect;
			hue = set_hue;

			ID = ID_count;
			ID_count++;

			erase_rect = Rect(0, 0, 0, 0);
		}

		
		Rect rect;

		double hue = 0;


		Rect erase_rect;
		bool clicked = false;

		int ID;

		static int ID_count;
	};

	Array<C_Rect> c_rects;

	Rect make_rect;
	Vec2 start = { 0,0 };
	Vec2 end = { 0,0 };

	int scene = 0;

	double m_hue = 0;

	Array<My_Button> my_buttons;

	int select_ID = -1;

	int edge_x = 850;
	int edge_y = 100;

	const Size sceneSize{ 1500, 720 };
	const RenderTexture gaussianA1{ sceneSize }, gaussianB1{ sceneSize };
	const RenderTexture gaussianA4{ sceneSize / 4 }, gaussianB4{ sceneSize / 4 };
	const RenderTexture gaussianA8{ sceneSize / 8 }, gaussianB8{ sceneSize / 8 };

	void init() {

		FontAsset::Register(U"Display", 30, Typeface::Medium);
		FontAsset::Register(U"Star", 20, Typeface::Medium);

		int ad = 300;

		my_buttons.push_back(My_Button(U"↑", ad+900 + 60, 300, 80, 80));
		my_buttons.push_back(My_Button(U"↓", ad+900 + 60, 400, 80, 80));
		my_buttons.push_back(My_Button(U"←", ad+800 + 60, 350, 80, 80));
		my_buttons.push_back(My_Button(U"→", ad+1000 + 60, 350, 80, 80));
		my_buttons.push_back(My_Button(U"w-", ad+860+50, 500, 80, 80));
		my_buttons.push_back(My_Button(U"w+", ad+860+100+50, 500, 80, 80));
		my_buttons.push_back(My_Button(U"h-", ad+860+50, 500+100, 80, 80));
		my_buttons.push_back(My_Button(U"h+", ad+860+100+50, 500+100, 80, 80));

		my_buttons.push_back(My_Button(U"削除", ad+950-60+10, 150, 200, 100));
	}

	void update() {

		//ドロップでもってくる
		if (DragDrop::HasNewFilePaths())
		{
			const Image image{ DragDrop::GetDroppedFilePaths().front().path };

			Texture texture{ image };

			m_texture = texture;
		}

		bool lock = false;

		int count = 0;

		for (auto& c_rect : c_rects) {

			c_rect.erase_rect = Rect(edge_x - 50, edge_y + count * 50 + 10, 30, 30);

			if (c_rect.erase_rect.leftClicked()) {
				select_ID = c_rect.ID;

				lock = true;
			}

			count++;
		}




		String direction = U"";

		for (auto& button : my_buttons) {

			button.update();

			if (button.get_click()) {

				lock = true;

				if (U"↑" == button.get_name()) {
					direction = U"↑";
				}
				else if (U"↓" == button.get_name()) {
					direction = U"↓";
				}
				else if (U"←" == button.get_name()) {
					direction = U"←";
				}
				else if (U"→" == button.get_name()) {
					direction = U"→";
				}
				else if (U"w+" == button.get_name()) {
					direction = U"w+";
				}
				else if (U"w-" == button.get_name()) {
					direction = U"w-";
				}
				else if (U"h+" == button.get_name()) {
					direction = U"h+";
				}
				else if (U"h-" == button.get_name()) {
					direction = U"h-";
				}


				if (U"削除" == button.get_name()) {
					delete_c_rect();
				}
			}
		}

		if (select_ID != -1) {

			for (auto& c_rect : c_rects) {

				if (select_ID == c_rect.ID) {

					if (U"↑" == direction) {
						c_rect.rect.y--;
						if (c_rect.rect.y < 100) {
							c_rect.rect.y = 100;
						}
					}
					else if (U"↓" == direction) {
						c_rect.rect.y++;
					}
					else if (U"←" == direction) {
						c_rect.rect.x--;
						if (c_rect.rect.x < 100) {
							c_rect.rect.x = 100;
						}
					}
					else if (U"→" == direction) {
						c_rect.rect.x++;
					}
					else if (U"w+" == direction) {
						c_rect.rect.w++;
					}
					else if (U"w-" == direction) {
						c_rect.rect.w--;
						if (c_rect.rect.w < 1) {
							c_rect.rect.w = 1;
						}
					}
					else if (U"h+" == direction) {
						c_rect.rect.h++;
					}
					else if (U"h-" == direction) {
						c_rect.rect.h--;
						if (c_rect.rect.h < 1) {
							c_rect.rect.h = 1;
						}
					}







				}
			}
		}


		if (false == lock) {

			if (0 == scene) {

				if (MouseL.down()) {

					start = Cursor::PosF();

					make_rect = Rect(start.x, start.y, 0, 0);

					if (start.x < 100) {
						start.x = 100;
					}

					if (start.y < 100) {
						start.y = 100;
					}

					scene++;
				}

			}
			else if (1 == scene) {

				end = Cursor::PosF();


				int w = end.x - start.x;
				int h = end.y - start.y;

				make_rect = Rect(start.x, start.y, w, h);

				if (MouseL.up()) {

					c_rects.push_back(C_Rect(make_rect, m_hue));
					m_hue += 60;

					make_rect = Rect(0, 0, 0, 0);

					scene = 0;
				}

			}

		}
		

	}

	void draw() {

		m_texture.draw(100, 100);


		
		int count = 0;

		for (auto& c_rect : c_rects) {

			double this_hue = c_rect.hue;

			c_rect.rect.drawFrame(3,HSV{ this_hue,1,1 });

			Rect moved_rect = c_rect.rect.movedBy(-100, -100);

			String s = Format(moved_rect.x) + U"," + Format(moved_rect.y) + U"," + Format(moved_rect.w) + U"," + Format(moved_rect.h);


			FontAsset(U"Display")(s).draw(edge_x, edge_y + count * 50, HSV{ this_hue,1,1 });

			

			c_rect.erase_rect.draw(HSV{ this_hue,1,1 });

			if (c_rect.ID == select_ID) {

				/*
				int x = c_rect.erase_rect.x + 15;
				int y = c_rect.erase_rect.y + 15;
				FontAsset(U"Star")(U"★").drawAt(x,y, Palette::Gray);
                */

				{
					const ScopedRenderTarget2D target{ gaussianA1.clear(ColorF{ 0.0 }) };
					const ScopedRenderStates2D blend{ BlendState::Additive };

					c_rect.rect.drawFrame(3, HSV{ this_hue,1,1 });

					c_rect.erase_rect.draw(HSV{ this_hue,1,1 });
				}

				Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
				Shader::Downsample(gaussianA1, gaussianA4);
				Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
				Shader::Downsample(gaussianA4, gaussianA8);
				Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);

				const ScopedRenderStates2D blend{ BlendState::Additive };
				gaussianA1.resized(sceneSize).draw(ColorF{ 0.1 });
				gaussianA4.resized(sceneSize).draw(ColorF{ 0.4 });
				gaussianA8.resized(sceneSize).draw(ColorF{ 0.8 });
				gaussianA1.resized(sceneSize).draw(ColorF{ 0.1 });
				gaussianA4.resized(sceneSize).draw(ColorF{ 0.4 });
				gaussianA8.resized(sceneSize).draw(ColorF{ 0.8 });
			}

			count++;
		}

		if (1 == scene) {
			make_rect.drawFrame(2, HSV{ m_hue,1,1 });
		}

		for (auto& button : my_buttons) {
			button.draw();
		}
	}







	void delete_c_rect() {

		c_rects.remove_if([&](C_Rect c_rect) {

			if (select_ID == c_rect.ID) {
				return true;
			}

		return false;

			});

	}
};
