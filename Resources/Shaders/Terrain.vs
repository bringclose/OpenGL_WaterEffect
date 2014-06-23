uniform mat4 u_mvp;

uniform sampler2D u_texture0;	//height map

attribute vec3 a_posL;
attribute vec2 a_uv;

varying vec2 v_uv;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_mvp*posL;
	gl_Position.y += (texture2D(u_texture0, a_uv).r * 100.0);
	
	v_uv = a_uv;
}