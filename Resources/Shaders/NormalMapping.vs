uniform mat4 u_mvp;
uniform mat4 u_maWorld;

attribute vec3 a_posL;
attribute vec3 a_normalL;
attribute vec3 a_tangentL;
attribute vec3 a_bitangentL;
attribute vec2 a_uv;

varying vec2 v_uv;
varying vec3 v_posW;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying vec3 v_normalW;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_mvp*posL;
	
	v_posW = (u_maWorld * posL).xyz;
	v_normalW = normalize((u_maWorld * vec4(a_normalL, 0.0)).xyz);
	v_tangentW = normalize((u_maWorld * vec4(a_tangentL, 0.0)).xyz);
	v_bitangentW = normalize((u_maWorld * vec4(a_bitangentL, 0.0)).xyz);
	v_uv = a_uv;
}