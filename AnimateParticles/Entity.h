#ifndef ENTITY_H
#define ENTITY_H
#pragma once/*
#include <glm-0.9.8.5\glm\glm.hpp>
#include "Model.h"*/
#include "Model.h"

namespace ML {
	class Entity
	{
	public:
		Entity() {}

		Entity(const Model& model, const glm::vec3& position, const glm::vec3& angles, const glm::vec3& scaleFactors) 
		: model(model), position(position), angles(angles), scaleFactors(scaleFactors), draw(true) {	}
		
		Model getModel() { return model; }
		glm::vec3 getPosition() { return position; }
		glm::vec3 getAngles() { return angles; }
		glm::vec3 getScaleFactors() { return scaleFactors; }

		Model setModel(Model model) { this->model = model; }
		void setPosition(const glm::vec3& pos) { this->position = pos; }
		void setAngles(const glm::vec3& angs) {	this->angles = angs; }
		void setScaleFactors(const glm::vec3& scales){this->scaleFactors = scales; }

		void increasePosition(glm::vec3 delta_position) {
			position += delta_position;
		}

		void increaseAngles(glm::vec3 delta_angles) {
			angles += glm::radians(delta_angles);
		}

		void increaseScaleFactors(glm::vec3 delta_scale) {
			scaleFactors += delta_scale;
		}

		bool getDraw() { return draw; }

		void setDraw(bool dr) { draw = dr; }

	private:
		Model model;
		glm::vec3 position;
		glm::vec3 angles;
		glm::vec3 scaleFactors;
		bool draw;
	};

}

#endif // ENTITY_H
