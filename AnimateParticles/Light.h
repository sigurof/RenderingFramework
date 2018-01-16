#ifndef LIGHT_H
#define LIGHT_H
#pragma once


namespace ML {
	class Light
	{
	public:
		Light(){}

		Light(const glm::vec3& pos, const glm::vec3& col) : position(pos), color(col) { }

		Light(const Light& other) : position(other.position), color(other.color) {}

		const glm::vec3& getPosition() const { return position; };
		const glm::vec3& getColor() const { return color; }
		void setPosition(const glm::vec3& pos) { position = pos; }
		void setColor(const glm::vec3& col) { color = col; }

	private:
		glm::vec3 position;
		glm::vec3 color;
	};

}


#endif // !LIGHT_H
