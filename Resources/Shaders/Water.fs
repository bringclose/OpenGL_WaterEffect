precision mediump float;

uniform sampler2D u_texture0;
//uniform samplerCube u_texture;

varying vec2 v_uv;
//varying vec3 v_posL;

void main()
{
	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	gl_FragColor = texture2D(u_texture0, v_uv);
	//gl_FragColor = textureCube(u_texture, v_posL);
}
