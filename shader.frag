// Fragment shader for per-pixel Phong interpolation and shading.

// The "varying" keyword means that the parameter's value is interpolated
// between the nearby vertices.
varying vec3 N;
varying vec3 v;

uniform float brightness;

void main()
{

	vec4 finalColor = vec4(0.0);
    // The scene's ambient light.
    vec4 ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	// The normal vectors is generally not normalized after being
	// interpolated across a triangle.  Here we normalize it.
	// vec3 Normal = normalize(N);

	// Since the vertex is in eye space, the direction to the
	// viewer is simply the normalized vector from v to the
	// origin.
	vec3 Viewer = -normalize(v);

	// Get the lighting direction and normalize it.
	vec3 Light  = normalize(gl_LightSource[0].position.xyz);

	// Compute halfway vector
	vec3 Half = normalize(Viewer+Light);

	// Compute factor to prevent light leakage from below the
	// surface
	float B = 1.0;
	if(dot(N, Light)<0.0) B = 0.0;

	// Compute geometric terms of diffuse and specular
	float diffuseShade = max(dot(N, Light), 0.0);
	float specularShade = 
	  B * pow(max(dot(Half, N), 0.0), gl_FrontMaterial.shininess);

	// Compute product of geometric terms with material and
	// lighting values
	vec4 diffuse = diffuseShade * gl_FrontLightProduct[0].diffuse;
	vec4 specular = specularShade * gl_FrontLightProduct[0].specular;
	ambient += gl_FrontLightProduct[0].ambient;

	finalColor += ambient + diffuse + specular;

	vec3 P_Light = normalize(gl_LightSource[1].position.xyz - v);
	// vec3 P_Half = normalize( Viewer + P_Light);

	// float P_B = 1.0;
	// if(dot(N, P_Light)<0.0) P_B = 0.0;

	// float P_diffuseShade = max(dot(N, P_Light), 0.0);
	// float P_specularShade = P_B * pow(max(dot(P_Half, N), 0.0), gl_FrontMaterial.shininess);

	// float dist = sqrt(pow(P_Light[0], 2.0) + pow(P_Light[1], 2.0) + pow(P_Light[2], 2.0));
	// P_diffuseShade /= gl_LightSource[1].quadraticAttenuation * (dist * dist) + gl_LightSource[1].linearAttenuation * dist + gl_LightSource[1].constantAttenuation;
	
	// vec4 spec = P_specularShade * gl_FrontLightProduct[1].specular;


	// vec4 addition = max(P_diffuseShade * gl_FrontLightProduct[1].diffuse + spec, gl_FrontLightProduct[1].ambient);
	
	// diffuse += addition;

	vec3 R = normalize(-reflect(P_Light, N));
	float dist = sqrt(pow(P_Light[0], 2.0) + pow(P_Light[1], 2.0) + pow(P_Light[2], 2.0));
	float attenuation = (gl_LightSource[1].quadraticAttenuation * (dist * dist) + gl_LightSource[1].linearAttenuation * dist + gl_LightSource[1].constantAttenuation);
	
	ambient = gl_FrontLightProduct[1].ambient;
	diffuse = gl_FrontLightProduct[1].diffuse * max(dot(N, P_Light), 0.0) * attenuation;

	diffuse = clamp(diffuse, 0.0, 1.0);

	specular = gl_FrontLightProduct[1].specular * pow(max(dot(R, Viewer), 0.0), 0.3*gl_FrontMaterial.shininess);
	specular = clamp(specular, 0.0, 1.0);

	finalColor += ambient + diffuse + specular;


	// if(P_diffuseShade > 0.0){
		
	// 	float intSpec = max(dot(P_Half, Normal), 0.0);
	// 	P_diffuseShade /= gl_LightSource[1].quadraticAttenuation * (dist * dist) + gl_LightSource[1].linearAttenuation * dist + gl_LightSource[1].constantAttenuation;
	// 	P_specularShade = specular * pow(intSpec, gl_FrontMaterial.shininess);

	// 	diffuse += P_diffuseShade * gl_FrontLightProduct[1].diffuse;
	// 	specular += P_specularShade * gl_FrontLightProduct[1].specular;
	// 	ambient += gl_FrontLightProduct[1].ambient;

	// }


	// Assign final color
	//gl_FragColor = ambient + diffuse + specular + gl_FrontMaterial.emission;
	
	gl_FragColor = finalColor * brightness;
	// Scale with brightness value
	// gl_FragColor = gl_FragColor * brightness;
}