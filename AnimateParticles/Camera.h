#ifndef CAMERA_H
#define CAMERA_H
#pragma once



namespace ML {

	class Camera
	{
	public:

		Camera() : Camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)) {}

		Camera(glm::vec3& pos, glm::vec3& fwDir, glm::vec3& upDir) :
			position(pos), fwDirection(glm::normalize(fwDir)), upDirection(glm::normalize(upDir))
		{
			currentlyActiveCamera = this;
			target = position + fwDirection;
			rtDirection = glm::normalize(glm::cross(fwDirection, upDirection));
		}

		Camera(const Camera&) = delete;

		Camera& operator=(const Camera&) = delete;

		~Camera() { 
			std::cout << "Camera::Destructor\n";
			if (currentlyActiveCamera == this)
			{
				currentlyActiveCamera = nullptr;
			}
		}
		
		float calculateSpeed() {
			return 10.0f*dt;
		}

		void move(const DirectionEnum dir) {
			speed = calculateSpeed();
			switch (dir)
			{
			case DirectionEnum::Forwards:
				position += speed*  fwDirection;
				break;
			case DirectionEnum::Backwards:
				position -= speed * fwDirection;
				break;
			case DirectionEnum::Left:
				position += speed  *rtDirection;
				break;
			case DirectionEnum::Right:
				position -= speed  *rtDirection;
				break;
			case DirectionEnum::Up:
				position += speed * upDirection;
				break;
			case DirectionEnum::Down:
				position -= speed * upDirection;
				break;
			default:
				break;
				throw(std::exception("ERROR! Invalid direction in Camera::move!\n"));
			}
			target = position + fwDirection;

		}

		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getTarget() const { return target; }
		const glm::vec3& getUpDirection() const { return upDirection; }
		
		void setDt(float time) { dt = time; }

		void changeViewDirection(const float yaw, const float pitch) 
		{
			fwDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			fwDirection.y = sin(glm::radians(pitch));
			fwDirection.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			fwDirection = glm::normalize(fwDirection);
			target = position + fwDirection;
			rtDirection = glm::normalize(glm::cross(fwDirection, upDirection));
		}

		static Camera* currentlyActiveCamera;

	private:
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 fwDirection;
		glm::vec3 rtDirection;
		glm::vec3 upDirection;
		float speed;
		float dt;


	};
}

#endif // CAMERA_H