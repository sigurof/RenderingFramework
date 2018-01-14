#ifndef COLOR_H
#define COLOR_H
//
//#include <glm-0.9.8.5\glm/glm.hpp>
//#include <glm-0.9.8.5\glm/gtc/matrix_transform.hpp>
//#include <glm-0.9.8.5\glm/gtc/type_ptr.hpp>

namespace ML {

	enum ColorEnum
	{
		RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, BLACK, ORANGE, PINK, PURPLE
	};

	class Color
	{
	public:
		Color() : rgb(glm::vec3(0, 0, 0)), alpha(1.0f) {};
		Color(glm::vec3& rgb) : rgb(rgb), alpha(1.0f) {};
		Color(glm::vec3& rgb, float alpha) : rgb(rgb), alpha(alpha) {};
		Color(float r, float g, float b, float a) : rgb(glm::vec3(r, g, b)), alpha(a) {  }
		
		glm::vec3 getRgb() const {
			return rgb;
		}
		
		Color(ColorEnum color) : Color(color, 1) {}

		Color(ColorEnum color, float a) : alpha(a) {
			switch (color)
			{
			case ML::RED:
				rgb = glm::vec3(1, 0, 0);
				break;
			case ML::GREEN:
				rgb = glm::vec3(0, 1, 0);
				break;
			case ML::BLUE:
				rgb = glm::vec3(0, 0, 1);
				break;
			case ML::CYAN:
				rgb = glm::vec3(0, 1, 1);
				break;
			case ML::MAGENTA:
				rgb = glm::vec3(1, 0, 1);
				break;
			case ML::YELLOW:
				rgb = glm::vec3(1, 1, 0);
				break;
			case ML::WHITE:
				rgb = glm::vec3(1, 1, 1);
				break;
			case ML::BLACK:
				rgb = glm::vec3(0, 0, 0);
				break;
			case ML::ORANGE:
				rgb = glm::vec3(1, 1 / 2, 0);
				break;
			case ML::PINK:
				rgb = glm::vec3(1, 102 / 255, 1);
				break;
			case ML::PURPLE:
				rgb = glm::vec3(172 / 255, 77 / 255, 175 / 255);
				break;
			default:
				break;
			}

		}

		float getAlpha() const {
			return alpha;
		}

	private:

		glm::vec3 rgb;
		float alpha;

	};

}

#endif // !COLOR_H
