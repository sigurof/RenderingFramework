#ifndef CAMERA_H
#define CAMERA_H
#pragma once



namespace ML {
	//float yaw(90.0f); float pitch(0.0f); bool firstMouseMovement = false;
	//float lastX, lastY;
	//glm::vec3 cameraFwDir = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	//glm::vec3 targ;
	//glm::vec3 pos;
	//glm::vec3 rt;
	//glm::vec3 up = glm::vec3(0, 1, 0);

	//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	//{
	//
	//	if (firstMouseMovement) // this bool variable is initially set to true
	//	{
	//		lastX = (float)xpos;
	//		lastY = (float)ypos;
	//		firstMouseMovement = false;
	//	}
	//	float xoffset = (float)(xpos - lastX);
	//	float yoffset = (float)(lastY - ypos); //reversed because y-coordinates range from bottom to top
	//	lastX = (float)xpos;
	//	lastY = (float)ypos;
	//	float sensitivity = 0.05f;
	//	xoffset *= sensitivity;
	//	yoffset *= sensitivity;
	//	yaw += xoffset; pitch += yoffset;
	//	if (pitch > 89.0f)
	//		pitch = 89.0f;
	//	if (pitch < -89.0f)
	//		pitch = -89.0f;
	//	cameraFwDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//	cameraFwDir.y = sin(glm::radians(pitch));
	//	cameraFwDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//	cameraFwDir = glm::normalize(cameraFwDir);
	//	targ = pos + cameraFwDir;
	//	rt = glm::cross(cameraFwDir, up);
	//
	//	//std::cout << "yaw: " << yaw << "\tpitch: " << pitch << "\n";
	//}

	

	class Camera
	{
	public:

		//Camera(const Camera& cam) : position(cam.position), yaw(cam.yaw), pitch(cam.pitch), roll(cam.roll),
		//	target(cam.target), fwDirection(cam.fwDirection), rtDirection(cam.rtDirection),
		//	upDirection(cam.upDirection), up(cam.up), radius(cam.radius), speed(cam.speed), dt(cam.dt)
		//{}

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

		//void move(int key, int action) {
		//	speed = calculateSpeed();
		//	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		//	{
		//		position += speed * fwDirection;
		//		std::cout << position.z << "\n";
		//	}
		//	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		//	{
		//		position -= speed * fwDirection;
		//		std::cout << position.z << "\n";
		//	}
		//	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		//	{
		//		position -= speed * rtDirection;
		//		std::cout << position.x << "\n";
		//	}
		//	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		//	{
		//		position += speed * rtDirection;
		//		std::cout << position.x << "\n";
		//	}
		//	if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
		//	{
		//		position -= speed * upDirection;
		//	}
		//	if (key == GLFW_KEY_E && (action == GLFW_REPEAT || action == GLFW_PRESS))
		//	{
		//		position += speed * upDirection;
		//	}
		//	target = position + fwDirection;
		//
		//}

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
		//glm::vec3 getTarget() { return position; }


		//void setTarget(glm::vec3& tgt)
		//{
		//	*target = tgt; 
		//	*fwDirection = glm::normalize(*target - *position); 
		//	*rtDirection = glm::normalize(glm::cross(*fwDirection, up)); 
		//}
		//void setPosition(glm::vec3 pos) {
		//	*position = pos; 
		//	*target = *position + *fwDirection; 
		//	*rtDirection = glm::normalize(glm::cross(*fwDirection, up)); 
		//}
		
		void setDt(float time) { dt = time; }

		void changeViewDirection(const float yaw, const float pitch) 
		{
			fwDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			fwDirection.y = sin(glm::radians(pitch));
			fwDirection.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			fwDirection = glm::normalize(fwDirection);
			target = position + fwDirection;
			rtDirection = glm::normalize(glm::cross(fwDirection, upDirection));
			//upDirection = glm::normalize(glm::cross(rtDirection, fwDirection));
		}

		//float getPitch() { return pitch; }
		//float getYaw() { return yaw; }
		//float getRoll() { return roll; }
		//float setPitch(float pitch) { this->pitch=pitch; }
		//float setYaw(float yaw) { this->yaw= yaw; }
		//float setRoll(float roll) { this->roll= roll; }
		
		static Camera* currentlyActiveCamera;

	private:
		glm::vec3 position;
		glm::vec3 target;
//		glm::vec3 direction;
		glm::vec3 fwDirection;
		glm::vec3 rtDirection;
		glm::vec3 upDirection;
		//glm::vec3 up;
		//float pitch;
		//float yaw;
		//float roll;
		/*Window* window;*/
		//float radius;
		float speed;
		float dt;


	};
}

#endif // CAMERA_H