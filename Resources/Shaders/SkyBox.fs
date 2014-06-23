precision mediump float;

uniform samplerCube u_textureCube;

varying vec3 v_posL;

void main()
{
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = textureCube(u_textureCube, v_posL);
}
