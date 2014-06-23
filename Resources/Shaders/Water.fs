precision mediump float;

uniform mat4 u_mvp;
uniform mat4 u_maWorld;

uniform sampler2D u_texture0;//rock
uniform sampler2D u_texture1;//displacement
uniform sampler2D u_texture2;//waternormal
uniform samplerCube u_textureCube;
uniform vec3 u_lightPos;
uniform vec4 u_lightColor;
uniform float u_time;
uniform float u_dMax;
uniform float u_fresnelPower;

uniform vec3 u_camPos;
varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying vec3 v_normalW;

void main()
{
	vec4 ambientColor = vec4(1.0, 1.0, 1.0, 1.0);
	float weight = 0.3;
	
	//Reflection
	vec3 toEye = normalize(u_camPos - v_posW);
    vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normalW));  // reflect() in non linear
    vec4 colorReflection = textureCube(u_textureCube,reflectDir);
    
    
    //Normal mapping
    vec3 normal = texture2D(u_texture2, v_uv).xyz;
    mat3 TBN = mat3(normalize(v_tangentW), normalize(v_bitangentW), normalize(v_normalW));
    vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
    vec3 lightDirection = normalize(v_posW - u_lightPos);
    vec4 DiffuseComponent = max(dot(normalW, lightDirection), 0.0) * u_lightColor;
    vec3 reflectVector = normalize((reflect(lightDirection, normalW)));
    vec4 SpecularComponent = pow(max(dot(reflectVector,toEye),0.0),20.0) * u_lightColor;//LightSpecularColor;
    float diffuse = max(0.0, dot(lightDirection, normalW));
    //vec4 colorWater = vec4((diffuse * (colorReflection + u_lightColor)).rgb, colorReflection.a); 
    vec4 colorWater = vec4((ambientColor * weight + diffuse * (1.0 - weight) * colorReflection + SpecularComponent).xyz, colorReflection.w);
    
    //Fresnel
    float Fresnel = pow((1.0 - abs(dot(normalW, toEye))), u_fresnelPower);
    vec4 rockColor = texture2D (u_texture0, v_uv);
	vec4 blendRockWater = mix(rockColor, colorWater, Fresnel);
    
    //Displacement
	vec2 disp = texture2D(u_texture1, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * u_dMax;
	vec2 Tex_coords_displaced = v_uv + offset;
	//vec4 rockColor = texture2D (u_texture0, Tex_coords_displaced);
	
	
	//final
	//gl_FragColor = vec4(v_normalW, 1.0);//colorWater;
	gl_FragColor = blendRockWater;
	//gl_FragColor = vec4(diffuse,diffuse,diffuse, 1.0);
}
