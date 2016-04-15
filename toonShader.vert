varying vec3 N;
varying vec3 v;

void main()
{
	N = gl_NormalMatrix * gl_Normal;
	v = vec3(gl_ModelViewMatrix * gl_Vertex);
	gl_Position = ftransform();
}