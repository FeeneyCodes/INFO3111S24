#version 420

//uniform mat4 MVP;
uniform mat4 mProj;
uniform mat4 mView;
uniform mat4 mModel;
	
in vec3 vColour;			// attribute
in vec3 vPosition;			// attribute

out vec3 color;

uniform vec3 colourOverride;			// 1, 0, 0 
uniform bool bUseOverrideColour;

void main()
{
	// Used to be in the C++ side
	// mvp = p * v * m

	mat4 MVP = mProj * mView * mModel;

	gl_Position = MVP * vec4(vPosition, 1.0);
	
	// Like multiplying by 1.0
//	gl_Position = sm * vec4( vPosition, 1.0f);
	
	// Vertex colour going to the fragment shader
	if ( bUseOverrideColour )
	{
		color = colourOverride;
	}
	else
	{
		// Take it from the file colour (per vertex)
		color = vColour;
	}
}