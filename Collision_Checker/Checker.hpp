#pragma once

struct Checker {

	Texture m_texture;

	struct C_Rect {

		C_Rect(Rect set_rect,double set_hue) {

			rect = set_rect;
			hue = set_hue;
		}

		void draw() {

		}

		Rect rect;

		double hue = 0;


		Rect erase_rect;
		bool clicked = false;

	};

	Array<C_Rect> c_rects;

	Rect make_rect;
	Vec2 start = { 0,0 };
	Vec2 end = { 0,0 };

	int scene = 0;

	double m_hue = 0;

	

	void init() {

		FontAsset::Register(U"Display", 30, Typeface::Medium);
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

		for (auto& c_rect : c_rects) {
			if (c_rect.erase_rect.leftClicked()) {

			}
		}


		c_rects.remove_if([&](C_Rect c_rect) {

			if (c_rect.erase_rect.leftClicked()) {
				lock = true;
				return true;
			}

		return false;

			});


		if (false == lock) {

			if (0 == scene) {

				if (MouseL.down()) {

					start = Cursor::PosF();

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


		int edge_x = 500;
		int edge_y = 100;

		int count = 0;

		for (auto& c_rect : c_rects) {

			double this_hue = c_rect.hue;

			c_rect.rect.drawFrame(2,HSV{ this_hue,1,1 });

			Rect moved_rect = c_rect.rect.movedBy(-100, -100);

			String s = Format(moved_rect.x) + U"," + Format(moved_rect.y) + U"," + Format(moved_rect.w) + U"," + Format(moved_rect.h);


			FontAsset(U"Display")(s).draw(edge_x, edge_y + count * 50, HSV{ this_hue,1,1 });

			c_rect.erase_rect = Rect(edge_x - 50, edge_y + count * 50 + 10, 30, 30);

			c_rect.erase_rect.draw(HSV{ this_hue,1,1 });

			count++;
		}

		if (1 == scene) {
			make_rect.drawFrame(2, HSV{ m_hue,1,1 });
		}
	}

};
