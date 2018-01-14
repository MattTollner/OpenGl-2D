#pragma once
#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>


#include "TextureHelperClass.h"
#include "ShaderHelperClass.h"

class SpriteHelperClass
{
public:
	// Constructor (inits shaders/shapes)
	SpriteHelperClass(ShaderHelperClass &shader);
	// Destructor
	~SpriteHelperClass();
	// Renders a defined quad textured with given sprite
	void RenderSprite(TextureHelperClass &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	ShaderHelperClass shader;
	GLuint qVAO;
	// Initializes and configures the quad's buffer and vertex attributes
	void quadSetup();
};