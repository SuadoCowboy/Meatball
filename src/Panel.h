#pragma once

#include "NodeUI.h"

namespace Meatball {
	namespace Interface {
		class Panel : public NodeUI {
		public:
			Panel(float x, float y, float width, float height, Color color, bool visible = true);
			
			virtual void draw();

			virtual int getTypes();

			Color color;
		};
	}
}