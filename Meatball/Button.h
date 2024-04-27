#pragma once

#include "NodeUI.h"

#include <raylib.h>

namespace Meatball {
	namespace Interface {
		enum ButtonStyle {
			RECTANGLE,
			CIRCLE, // in this mode, width becomes radius and is divided by 100 ex: width = 172 -> radius = 1.72
			X,
		};

		// used by Button class to use the button style specified
		class ButtonDraw {
		public:
			virtual void draw(const Button& button) = 0;
			virtual ~ButtonDraw();
		};

		class Button : public NodeUI {
		public:
			Button(int x, int y, int width, int height, Color color, bool visible = true);
			~Button();

			void onMouseButtonPressed() override;
			void draw();

			void setStyle(ButtonStyle style);

			virtual NodeType getType();
			
			Color color;
		private:
			ButtonDraw* drawMethod;
		};
	}
}