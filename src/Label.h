#pragma once

#include "NodeUI.h"

#include <string>

namespace Meatball {
	namespace Interface {
		class Label : public NodeUI {
		public:
			Label(float fontSize, const Font& font, float x, float y, bool visible = true, float spacing = 1.0f);

			void draw();

			virtual int getTypes();

			void updateSize();

			void setText(const std::string& text);
			const std::string& getText();

			Color foregroundColor, backgroundColor;
			float fontSize, spacing;
			const Font& font;
		private:
			std::string text;
		};
	}
}