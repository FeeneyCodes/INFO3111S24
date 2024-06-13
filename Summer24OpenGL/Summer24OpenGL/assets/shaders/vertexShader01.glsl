#version 420

//uniform mat4 MVP;
uniform mat4 mProj;
uniform mat4 mView;
uniform mat4 mModel;
uniform mat4 mModel_InverseTranspose;
//uniform mat4 matRotationOnly

// All registers in the GPU are vec4 
in vec4 vColour;			// RGB, A (alpha = transparency)
in vec4 vPosition;			// XYZ (ignore the 4th value, which is W)
in vec4 vNormal;
in vec2 vUV;

//out vec4 color;
//out vec4 vertNormal;

out vec4 vertexColour;
out vec4 vertexNormal;
out vec4 vertexWorldPosition;
out vec2 vertexUV;

uniform vec3 colourOverride;			// 1, 0, 0 
uniform bool bUseOverrideColour;

//uniform vec2 someCoolThing;

void main()
{
	// Used to be in the C++ side
	// mvp = p * v * m

	mat4 MVP = mProj * mView * mModel;

	// "Screen space" loation (i.e. in 2D)
	gl_Position = MVP * vec4(vPosition.xyz, 1.0f);
	
	vertexWorldPosition = mModel * vec4(vPosition.xyz, 1.0f);
	
	// Sent normal to the fragment shader, too
//	vertexNormal = mModel_InverseTranspose * vec4(vNormal.xyz, 1.0f);
	vertexNormal = mModel_InverseTranspose * vec4(vNormal.xyz, 1.0f);
	vertexNormal.xyz = normalize(vertexNormal.xyz);
	
	// Like multiplying by 1.0
//	gl_Position = sm * vec4( vPosition, 1.0f);
	
	// Vertex colour going to the fragment shader
	if ( bUseOverrideColour )
	{
		vertexColour = vec4(colourOverride.rgb, 1.0f);
	}
	else
	{
		// Take it from the file colour (per vertex)
		vertexColour = vColour;
	}
	// Copy the UV (texture) coords
	vertexUV = vUV;
}