precision mediump float;

uniform mat4 u_mvp;
uniform mat4 u_maWorld;

uniform sampler2D u_texture0;//rock
uniform sampler2D u_texture1;//displacement
uniform sampler2D u_texture2;//waternormal
uniform samplerCube u_textureCube;
uniform vec3 u_lightPos;
uniform vec3  u_lightColor;

uniform vec3 u_camPos;
varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying vec3 v_normalW;

void main()
{
	//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	//gl_FragColor = texture2D(u_texture0, v_uv/5.0);
	//gl_FragColor = textureCube(u_texture, v_posL);
	
	vec3 toEye = u_camPos - v_posW;
    vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normalW));  // reflect() in non linear
    vec4 colorReflection = textureCube(u_textureCube,reflectDir);
    
    //Normal mapping
    vec3 normal = texture2D(u_texture2, v_uv).xyz;
    mat3 TBN = mat3(normalize(v_tangentW), normalize(v_bitangentW), normalize(v_normalW));
    vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
    vec3 lightDirection = normalize(v_posW - u_lightPos);
    float diffuse = max(0.0, dot(lightDirection, normalW));
    gl_FragColor = vec4(diffuse * (colorReflection.rgb + u_lightColor), colorReflection.a);    
}
