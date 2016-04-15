
varying vec3 N;
varying vec3 v;

void main()
{
	float intensity;
	vec4 color;
	vec3 normal;

	normal = normalize(N);
	// vec4 mat_color = gl_FrontLightProduct[0].diffuse;
	// vec4 spec = vec4(0.0);

	vec3 lightDir = normalize(gl_LightSource[0].position.xyz);
	// vec3 Viewer = -normalize(v);

	intensity = max(dot(normal, lightDir), 0.0);

	// vec3 h = normalize(lightDir + Viewer);
	// float intSpec = max(dot(h, N), 0.0);

	if (dot(lightDir, normal) 
               < mix(0.1, 0.4, 
               max(0.0, dot(normal, lightDir))))
            {
               
				float ink = dot(normalize(-v), normal);
				ink = clamp(pow(ink*3.0, 20.0), 0.0, 1.0);
				color = vec4(ink, ink, ink, 1.0);
        }
	else if (intensity > 0.95)

		// color = vec4(0.5,1.0,0.5,1.0);
		color = vec4(1.0, 1.0, 1.0, 1.0);
		// color = mat_color;
		// spec = gl_FrontLightProduct[0].specular * pow(intSpec, gl_FrontMaterial.shininess);


	else if (intensity > 0.5)

		// color = vec4(0.3,0.6,0.3,1.0);
		color = vec4(0.6, 0.6, 0.6, 1.0);
		// color = vec4(mat_color[0] * 0.6, mat_color[0] * 0.6, mat_color[0] * 0.6, mat_color[3]);
		// spec = gl_FrontLightProduct[0].specular * vec4(0.6, 0.6, 0.6, 1.0) * pow(intSpec, gl_FrontMaterial.shininess);

	else if (intensity > 0.25)

		// color = vec4(0.2,0.4,0.2,1.0);
		color = vec4(0.4, 0.4, 0.4, 1.0);
		// color = vec4(mat_color[0] * 0.4, mat_color[0] * 0.4, mat_color[0] * 0.4, mat_color[3]);
		// spec = gl_FrontLightProduct[0].specular * vec4(0.4, 0.4, 0.4, 1.0) * pow(intSpec, gl_FrontMaterial.shininess);

	else

		// color = vec4(0.1,0.2,0.1,1.0);
		color = vec4(0.2, 0.2, 0.2, 1.0);
		// color = vec4(mat_color[0] * 0.2, mat_color[0] * 0.2, mat_color[0] * 0.2, mat_color[3]);
		// spec = gl_FrontLightProduct[0].specular * vec4(0.2, 0.2, 0.2, 1.0) * pow(intSpec, gl_FrontMaterial.shininess);


    
	gl_FragColor = color * (gl_LightSource[0].diffuse + gl_LightSource[0].ambient + gl_LightSource[0].specular);
	// gl_FragColor = max(intensity * gl_FrontLightProduct[0].diffuse + spec, gl_FrontLightProduct[0].ambient);
}