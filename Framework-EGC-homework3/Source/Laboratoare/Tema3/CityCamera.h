#pragma once
#include <include/glm.h>
#include <include/math.h>

class Camera
{
	private:
		glm::mat4 projectionMatrix;
	public:
		Camera()
		{
			position = glm::vec3(0, 2, 5);
			forward = glm::vec3(0, 0, -1);
			up		= glm::vec3(0, 1, 0);
			right	= glm::vec3(1, 0, 0);
			distanceToTarget = 2;
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }

		// Update camera
		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center-position);
			right	= glm::cross(forward, up);
			this->up = glm::cross(right,forward);
			distanceToTarget = glm::length(center - position);
		}

		void SetProjectionMatrix(float fovStep, float aspectRatio, float zNear, float zFar)
		{
			projectionMatrix = glm::perspective(RADIANS(fovStep), aspectRatio, zNear, zFar);
		}

		glm::highp_mat4x4 getProjectionMatrix() {
			return projectionMatrix;
		}

		glm::vec3 getPosition() {
			return position;
		}

		void MoveForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			this->position += dir * distance;
		}

		void TranslateForward(float distance)
		{
			this->position += this->forward * distance;
		}

		void TranslateUpword(float distance)
		{
			this->position += glm::vec3(0, 1, 0) * distance;
		}

		void TranslateRight(float distance)
		{
			this->position += this->right * distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, right);
			glm::vec4 newFwd = rot * glm::vec4(forward, 1.0);
			glm::vec4 newUp = rot * glm::vec4(up, 1.0);
			this->forward = glm::normalize(glm::vec3(newFwd));
			this->up = glm::normalize(glm::vec3(newUp));
		}

		void RotateFirstPerson_OY(float angle)
		{
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

			glm::vec4 newFwd = rot * glm::vec4(forward, 1.0);
			glm::vec4 newUp = rot * glm::vec4(up, 1.0);
			glm::vec4 newRight = rot * glm::vec4(right, 1.0);

			this->forward = glm::normalize(glm::vec3(newFwd));
			this->up = glm::normalize(glm::vec3(newUp));
			this->right = glm::normalize(glm::vec3(newRight));
		}

		void RotateFirstPerson_OZ(float angle)
		{

			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, forward);

			glm::vec4 newUp = rot * glm::vec4(up, 1.0);
			glm::vec4 newRight = rot * glm::vec4(right, 1.0);

			this->up = glm::normalize(glm::vec3(newUp));
			this->right = glm::normalize(glm::vec3(newRight));
		}

		void RotateThirdPerson_OX(float angle)
		{
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OX(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle)
		{
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OY(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OZ(float angle)
		{
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OZ(angle);
			TranslateForward(-distanceToTarget);
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

		glm::vec3 getForward() {
			return forward;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
