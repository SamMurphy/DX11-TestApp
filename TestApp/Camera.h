#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void Initialise() {}
	void Update(double deltaTime);
	void End() {}

	// Getters/Setters
	const glm::mat4& GetViewMatrix() { return _viewMatrix; }
	void SetViewMatrix(const glm::mat4& vm) { _viewMatrix = vm; }

	const glm::mat4& GetProjectionMatrix() { return _projectionMatrix; }
	void SetProjectionMatrix(const glm::mat4& pm) { _projectionMatrix = pm; }

	const glm::mat4& GetOrthographicMatrix() { return _orthographicMatrix; }
	void SetOrthographicMatrix(const glm::mat4& om) { _orthographicMatrix = om; }

	const glm::mat4& GetView2DMatix() { return _view2dMatrix; }

	const glm::vec3& GetForward() const { return _forward; }
	void SetForward(const glm::vec3& f) { _forward = f; }

	const glm::vec3& GetUp() const { return _up; }
	void SetUp(const glm::vec3& u) { _up = u; }

	const float& GetFOV() const { return _fov; }
	void SetFOV(const float& fov) { _fov = fov; }

	const glm::vec3& GetPosition() const { return _position; }
	void SetPosition(const glm::vec3& pos) { _position = pos; }

	const glm::vec3& GetRotation() const { return _rotation; }
	void SetRotation(const glm::vec3& rot) { _rotation = rot; }

	bool OnKeyPress(int key, bool down);

private:
	/// View Matrix
	glm::mat4 _viewMatrix;
	/// Orthographic Matrix
	glm::mat4 _orthographicMatrix;
	/// Projection Matrix
	glm::mat4 _projectionMatrix;
	/// 2D View Matrix
	glm::mat4 _view2dMatrix;
	/// Vector pointing forward
	glm::vec3 _forward;
	/// Vector pointing upwards
	glm::vec3 _up;
	/// Field of View
	float _fov;

	/// Position
	glm::vec3 _position;
	/// Rotation
	glm::vec3 _rotation;

	glm::vec3 _pan;
	glm::vec3 _tilt;

	bool _boost;
	float _moveSpeed;
	float _rotationSpeed;
};

