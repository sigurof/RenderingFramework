#ifndef LIGHT_H
#define LIGHT_H
#pragma once


namespace ML {
	class Light
	{
	public:
		Light(){}

		Light(const Model& mod, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, const Color& col) : color(col)
		{
			entity = std::shared_ptr<Entity>(new Entity(mod, pos, rot, scale));
		}

		Light(const Light& other) = delete;
		
		Light& operator=(const Light& other) = delete;
		//Light(const Light& other) : position(other.position), color(other.color) {}
		
		const std::shared_ptr<Entity> getEntity() const { return entity; }
		const Color& getColor() const { return color; }
		const glm::vec3& getPosition() const { return entity->getPosition(); }

		void setColor(const Color& col) { color = col; }


	private:

		std::shared_ptr<Entity> entity;
		Color color;

	};

}


#endif // !LIGHT_H
