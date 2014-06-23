uniform mat4 u_mvp;

attribute vec3 a_posL;
attribute vec2 a_uv;

varying vec2 v_uv;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_mvp*posL;
	
	v_uv = a_uv;
}