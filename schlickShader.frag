
varying vec3 N;
varying vec3 v;

uniform float N1;
uniform float N2;
uniform float brightness;
void main()
{
	vec4 ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	float roughness = 0.3; // fresnel reflectance at normal incidence
	float F0 = pow(((N1 - N2) / (N1 + N2)), 2.0);
	float k = 0.5; // fraction of diffuse reflection

	// vec3 lightColor = vec3(0.9, 0.1, 0.1);

	vec3 Viewer = -normalize(v);

	vec3 lightDir  = normalize(gl_LightSource[0].position.xyz);

	// Compute halfway vector
	// vec3 Half = normalize(Viewer+Light);


	float diffuseShade = max(dot(N, lightDir), 0.0);

	float specular = 0.0;

	if(diffuseShade > 0.0)
	{
		vec3 Viewer = -normalize(v);

		vec3 H = normalize(lightDir + Viewer);

		float NdotH = max(dot(N, H), 0.0);
		float NdotV = max(dot(N, Viewer), 0.0);

		float VdotH = max(dot(Viewer, H), 0.0);

		float mSquared = roughness * roughness;

		float NH2 = 2.0 * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * diffuseShade) / VdotH;
		float atten = min(1.0, min(g1, g2));

		float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
		float rough = r1 * exp(r2);

		float fresnel = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);

		specular = (fresnel * atten * rough) / (NdotV * diffuseShade * 3.14);
	}

	// vec4 diffuse = diffuseShade * gl_FrontLightProduct[0].diffuse;
	ambient += gl_FrontLightProduct[0].ambient;

	vec4 final = (gl_FrontLightProduct[0].specular + ambient + gl_FrontLightProduct[0].diffuse) * diffuseShade * (k + specular * (1.0 - k));
	gl_FragColor = final * brightness * brightness;
}