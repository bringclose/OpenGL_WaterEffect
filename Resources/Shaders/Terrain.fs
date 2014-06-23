precision mediump float;

uniform sampler2D u_texture1;	//blend map
uniform sampler2D u_texture2;	//Grass
uniform sampler2D u_texture3;	//Rock
uniform sampler2D u_texture4;	//Dirt

varying vec2 v_uv;

void main()
{
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	float scaledFactor = 30.0;
	vec2 scaled_uv = v_uv * scaledFactor;
	vec4 blendMap = texture2D(u_texture1, v_uv);
	vec4 grass = texture2D(u_texture2, scaled_uv);
	vec4 rock = texture2D(u_texture3, scaled_uv);
	vec4 dirt = texture2D(u_texture4, scaled_uv);
	gl_FragColor = (blendMap.r * grass + blendMap.g * rock + blendMap.b * dirt) / (blendMap.r + blendMap.g + blendMap.b);
}
