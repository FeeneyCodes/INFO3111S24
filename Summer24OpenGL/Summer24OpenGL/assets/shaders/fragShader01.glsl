#version 420

in vec3 color;

out vec4 pixelColour;

void main()
{
//	gl_FragColor = vec4(color, 1.0);
	
	pixelColour = vec4( color, 1.0f );
}