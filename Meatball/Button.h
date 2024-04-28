#pragma once

#include "NodeUI.h"

#include <raylib.h>

#include <functional>

namespace Meatball {
	namespace Interface {
		enum ButtonStyle {
			RECTANGLE,
			CIRCLE, // in this mode, width becomes radius and is divided by 100 ex: width = 172 -> radius = 1.72
			X,
		};
		
		class ButtonStyleClass;

		class Button : public NodeUI {
		public:
			Button(int x, int y, int width, int height, Color color, bool visible = true);
			~Button();
			
			void setOnMouseButtonPressed(const std::function<void(Button&, MouseButton)>& lambda);
			void onMouseButtonPressed(MouseButton button);
			void draw();
			
			bool checkCollision(Rectangle& rectangle);

			void setStyle(ButtonStyle style);

			virtual int getTypes();
			
			Color color;
		
		private:
			std::function<void(Button&, MouseButton)> onMouseButtonPressedFunction;
			ButtonStyleClass* styleMethods; // methods for a specific style
		};

		// used by Button class to use the button style specified
		class ButtonStyleClass {
		public:
			virtual void draw(const Button& button) = 0;
			virtual bool checkCollision(const Button& button, Rectangle& rectangle) = 0;

			virtual ~ButtonStyleClass();
		};

		// this is not meant to be used by others. Only inside Button code
		class ButtonStyleRectangle : public ButtonStyleClass {
			void draw(const Button& button) override;
			bool checkCollision(const Button& button, Rectangle& rectangle) override;
		};

		// this is not meant to be used by others. Only inside Button code
		class ButtonStyleCircle : public ButtonStyleClass {
			void draw(const Button& button) override;
			bool checkCollision(const Button& button, Rectangle& rectangle) override;
		};

		// this is not meant to be used by others. Only inside Button code
		class ButtonStyleX : public ButtonStyleClass {
			void draw(const Button& button) override;
			bool checkCollision(const Button& button, Rectangle& rectangle) override;
		};
	}
}