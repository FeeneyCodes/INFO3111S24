#version 420

//in vec4 color;
//in vec4 vertNormal;

in vec4 vertexColour;
in vec4 vertexNormal;
in vec4 vertexWorldPosition;
in vec2 vertexUV;	// Texture coord

// Now shiny the model is
uniform vec4 vertexSpecular; //= vec4(1.0f, 1.0f, 1.0f, 100.0f);

// This is how transparent the whole object is 
// Ranges from 0.0 to 1.0   (completely invisible to 'solid')
uniform float alphaTransparency;

// output to the screen
out vec4 pixelColour;		// RGB, 4th value is "A" (alpha)

struct sLight
{
	vec4 position;	// xyz, ignoring w (4th parameter)	
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights, ignoring w (4th parameter)	
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

// 
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;
uniform int maxLightIndexInUse;
uniform sLight theLights[NUMBEROFLIGHTS];
//
//uniform vec4 lights_position[NUMBEROFLIGHTS];
//uniform vec4 lights_diffuse[NUMBEROFLIGHTS];
//uniform vec4 lights_specular[NUMBEROFLIGHTS];
//...

uniform vec3 eyeLocation;

// If true, then we DON'T calcualte the light contrib
// bool is really a float
uniform bool bDoNotLight;
uniform bool bUseTextureAsColour;

// If true, then it's a skybox 
uniform bool bIsSkyBox;

//uniform vec3 ambientLightColour;


// Texture stuff
uniform sampler2D textures00;
uniform sampler2D textures01;
uniform sampler2D textures02;
uniform sampler2D textures03;

// And so on... (the other 4 textures)
uniform vec4 textureRatios_0_to_3;	

// The skybox texture (cube map texture)
uniform samplerCube skyBoxTexture;


vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );

void main()
{

	pixelColour = vertexColour;
	pixelColour.a = alphaTransparency;
	

	if ( bIsSkyBox )
	{
		// Do some stuff
//		pixelColour.rgb = vec3( 0.0f, 1.0f, 1.0f);

		// Notice that this takes 3 texture coords
		// Which is s, t, u 
		// (or x, y, z)
		vec3 texColorRGB = texture(skyBoxTexture, vertexNormal.xyz).rgb;

		pixelColour.rgb = texColorRGB;
		pixelColour.a = 1.0f;
		return;
	}
	
	// HACK: Make everything chrome
//	
//	vec3 rayforTextureLookup = vertexNormal.xyz;
//	
//	rayforTextureLookup = -rayforTextureLookup;
//	vec3 eyeToVertex = eyeLocation.xyz - vertexWorldPosition.xyz;
//	// Make this a length of 1.0 ("normalized")
//	eyeToVertex = normalize(eyeToVertex);
//	
//	vec3 reflectRay = reflect(eyeToVertex, vertexNormal.xyz);
//	vec3 refractRay = refract(eyeToVertex, vertexNormal.xyz, 1.1f);
//
//	vec3 vertColourReflect = texture(skyBoxTexture, reflectRay).rgb;
//	vec3 vertColourRefract = texture(skyBoxTexture, refractRay).rgb;
//	
//	vec3 vertColour = (vertColourReflect * 0.5f) +
//	                  (vertColourRefract * 0.5f);
//	
//	pixelColour.rgb = vertColour;
//	pixelColour.a = 0.6f;
//	return;

	
	// 
//	pixelColour.rgb *= 0.0001f;	// Goes black
//	pixelColour.r += vertexUV.s;		// u or x
//	pixelColour.g += vertexUV.t;		// v or y
	
	if ( bUseTextureAsColour )
	{
		// Get colour from the texture
//uniform sampler2D textures00;
//uniform sampler2D textures01;
//uniform sampler2D textures02;
//uniform sampler2D textures03;
//uniform vec4 textureRatios_0_to_3;			
		
		// Sample the image
		
		vec3 texColorRGB = 
				  texture(textures00, vertexUV.st).rgb * textureRatios_0_to_3.x
				+ texture(textures01, vertexUV.st).rgb * textureRatios_0_to_3.y
				+ texture(textures02, vertexUV.st).rgb * textureRatios_0_to_3.z
				+ texture(textures03, vertexUV.st).rgb * textureRatios_0_to_3.w;
		
		// Clear the current colour
		pixelColour.rgb = texColorRGB.rgb;
		
//		if ( texColorRGB.r > 0.2f )
//		{
//			discard;
//		}
	}
	
	
	if ( bDoNotLight )
	{
		// Early exit. don't do light calculation
		return;
	}
	
	// Object specular is white (light hightlight colour is white)
	// 4th value is now "shiny" the thing is (specular power)
	// IT STARTS AT 1! (not 0)
	// And goes to whatever... like 10,000 or 100,000
//	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 100.0f);

	vec4 lightContrib = calcualteLightContrib( pixelColour.rgb, 
	                                           vertexNormal.xyz, 
	                                           vertexWorldPosition.xyz,
	                                           vertexSpecular );
											   
	pixelColour.rgb = lightContrib.rgb;
	
	pixelColour.a = alphaTransparency;

}


// Feeney gave you this (it's inspired by the basic shader in Mike Bailey's Graphic Shaders book)
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
			
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += ( vertexMaterialColour.rgb * 
			                           theLights[index].diffuse.rgb * 
									   lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;	
		float distanceToLight = length(vLightToVertex);		
		
// Distance cut off
//		if ( distanceToLight > theLights[index].atten.w )
//		{
//			return vec4( 0.0f, 0.0f, 0.0f, 1.0f);
//		}

		vec3 lightVector = normalize(vLightToVertex);		
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
					   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight * distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb * lightSpecularContrib.rgb );
								  
	}//for(intindex=0...
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}
