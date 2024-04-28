#pragma once

#include "NodeUI.h"

namespace Meatball {
	namespace Interface {
		class Panel : public NodeUI {
		public:
			Panel(int x, int y, int width, int height, Color color, bool visible = true);
			
			virtual void draw();

			virtual int getTypes();

			Color color;
		};
	}
}