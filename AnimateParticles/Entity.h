#ifndef ENTITY_H
#define ENTITY_H
#pragma once

namespace ML {
	/*
	All Entities should be instances in the entities vector in a Scene instance because a Scene also holds tracks of the
	meshes and models, which can be shared between several entities. Making two instances of the same mesh is just uploading
	unnecessary information to the graphics card.
	*/
	class Entity
	{
	public:
		Entity() : draw(false) {}

		Entity(const Model& model, const glm::vec3& position, const glm::vec3& angles, const glm::vec3& scaleFactors)
			: model(model), position(position), angles(angles), scaleFactors(scaleFactors), draw(true) {	}

		const Model& getModel() const 
		{
			try
			{
				if (this == nullptr)
				{
					throw std::exception("ERROR! Trying to access nullptr\n");
				}
				return model;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Entity::getModel\n";
				throw err;
			}
		}
		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getAngles() const { return angles; }
		const glm::vec3& getScaleFactors() const { return scaleFactors; }
		bool shouldDraw() const { return draw; }

		Model setModel(Model model) { this->model = model; }
		void setPosition(const glm::vec3& pos) { this->position = pos; }
		void setAngles(const glm::vec3& angs) { this->angles = angs; }
		void setScaleFactors(const glm::vec3& scales) { this->scaleFactors = scales; }
		void setDraw(bool dr) { draw = dr; }

		void increasePosition(glm::vec3 delta_position) {
			position += delta_position;
		}

		void increaseAngles(glm::vec3 delta_angles) {
			angles += glm::radians(delta_angles);
		}

		void increaseScaleFactors(glm::vec3 delta_scale) {
			scaleFactors += delta_scale;
		}



	private:

		Model model;
		glm::vec3 position;
		glm::vec3 angles;
		glm::vec3 scaleFactors;
		bool draw;


	};

}

#endif // ENTITY_H
