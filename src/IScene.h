#pragma once

namespace Meatball {
	/// @brief pure abstract class that should be inherited to create a scene
	class IScene
	{
	public:
		virtual void update() = 0;
		virtual void draw() = 0;
	};
}