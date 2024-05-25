#include "cLightManager.h"

cLightManager::cLightManager()
{


}


void cLightManager::GetUniformLocations(GLuint shaderProgram)
{
	//uniform sLight theLights[NUMBEROFLIGHTS];

	this->theLights[0].position_UL = glGetUniformLocation(shaderProgram, "theLights[0].position");
	this->theLights[0].diffuse_UL = glGetUniformLocation(shaderProgram, "theLights[0].diffuse");
	this->theLights[0].specular_UL = glGetUniformLocation(shaderProgram, "theLights[0].specular");
	this->theLights[0].atten_UL = glGetUniformLocation(shaderProgram, "theLights[0].atten");
	this->theLights[0].direction_UL = glGetUniformLocation(shaderProgram, "theLights[0].direction");
	this->theLights[0].param1_UL = glGetUniformLocation(shaderProgram, "theLights[0].param1");
	this->theLights[0].param2_UL = glGetUniformLocation(shaderProgram, "theLights[0].param2");

//	this->theLights[1].position_UL = glGetUniformLocation(shaderProgram, "theLights[1].position");
//	this->theLights[1].diffuse_UL = glGetUniformLocation(shaderProgram, "theLights[1].diffuse");
//	this->theLights[1].specular_UL = glGetUniformLocation(shaderProgram, "theLights[1].specular");
//	this->theLights[1].atten_UL = glGetUniformLocation(shaderProgram, "theLights[1].atten");
//	this->theLights[1].direction_UL = glGetUniformLocation(shaderProgram, "theLights[1].direction");
//	this->theLights[1].param1_UL = glGetUniformLocation(shaderProgram, "theLights[1].param1");
//	this->theLights[1].param2_UL = glGetUniformLocation(shaderProgram, "theLights[1].param2");

	// And so on... for all of your lights

	return;
}
     
void cLightManager::UpdateShaderUniforms(GLuint shaderProgram)
{
	for ( unsigned int index = 0; index != cLightManager::NUMBEROFLIGHTS; index++ )
	{
		glUniform4f(this->theLights[index].position_UL,		// Uniform location
					this->theLights[index].position.x,
					this->theLights[index].position.y,
					this->theLights[index].position.z,
					this->theLights[index].position.w);

		glUniform4f(this->theLights[index].diffuse_UL,		// Uniform location
					this->theLights[index].diffuse.r,
					this->theLights[index].diffuse.g,
					this->theLights[index].diffuse.b,
					this->theLights[index].diffuse.a);

		glUniform4f(this->theLights[index].specular_UL,		// Uniform location
					this->theLights[index].specular.r,
					this->theLights[index].specular.g,
					this->theLights[index].specular.b,
					this->theLights[index].specular.a);

		glUniform4f(this->theLights[index].atten_UL,		// Uniform location
					this->theLights[index].atten.r,
					this->theLights[index].atten.g,
					this->theLights[index].atten.b,
					this->theLights[index].atten.a);

		glUniform4f(this->theLights[index].direction_UL,	// Uniform location
					this->theLights[index].direction.r,
					this->theLights[index].direction.g,
					this->theLights[index].direction.b,
					this->theLights[index].direction.a);

		glUniform4f(this->theLights[index].param1_UL,		// Uniform location
					this->theLights[index].param1.x,
					this->theLights[index].param1.y,
					this->theLights[index].param1.z,
					this->theLights[index].param1.w);

		glUniform4f(this->theLights[index].param2_UL,		// Uniform location
					this->theLights[index].param2.x,
					this->theLights[index].param2.y,
					this->theLights[index].param2.z,
					this->theLights[index].param2.w);

	}//for ( unsigned int index...

	return;
}

