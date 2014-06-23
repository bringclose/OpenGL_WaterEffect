precision mediump float;

uniform sampler2D u_texture0;	//Rock
uniform sampler2D u_texture1;	//RockNormalMap

varying vec2 v_uv;

void main()
{
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = texture2D(u_texture0, v_uv);
	//gl_FragColor = texture2D(u_texture1, v_uv);
}
