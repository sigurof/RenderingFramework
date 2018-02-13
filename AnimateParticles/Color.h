#ifndef COLOR_H
#define COLOR_H

namespace ML {



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
			case RED:
				rgb = glm::vec3(1, 0, 0);
				break;
			case GREEN:
				rgb = glm::vec3(0, 1, 0);
				break;
			case BLUE:
				rgb = glm::vec3(0, 0, 1);
				break;
			case CYAN:
				rgb = glm::vec3(0, 1, 1);
				break;
			case MAGENTA:
				rgb = glm::vec3(1, 0, 1);
				break;
			case YELLOW:
				rgb = glm::vec3(1, 1, 0);
				break;
			case WHITE:
				rgb = glm::vec3(1.0, 1.0, 1.0);
				break;
			case BLACK:
				rgb = glm::vec3(0, 0, 0);
				break;
			case ORANGE:
				rgb = glm::vec3(1, 1 / 2, 0);
				break;
			case PINK:
				rgb = glm::vec3(1, 102 / 255, 1);
				break;
			case PURPLE:
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
