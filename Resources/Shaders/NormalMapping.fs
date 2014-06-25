precision mediump float;

uniform sampler2D u_texture0;	//Rock
uniform sampler2D u_texture1;	//RockNormalMap
uniform sampler2D u_texture2;	//water effect

uniform vec3 u_lightPos;
uniform vec4 u_lightColor;
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
	vec3 toEye = normalize(u_camPos - v_posW);
	
	vec3 normal = texture2D(u_texture1, v_uv).xyz;
    mat3 TBN = mat3(normalize(v_tangentW), normalize(v_bitangentW), normalize(v_normalW));
    vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
    vec3 lightDirection = normalize(v_posW - u_lightPos);
    vec4 DiffuseComponent = max(dot(normalW, lightDirection), 0.0) * u_lightColor;
    vec3 reflectVector = normalize((reflect(lightDirection, normalW)));
    vec4 SpecularComponent = pow(max(dot(reflectVector,toEye),0.0),20.0) * u_lightColor;//LightSpecularColor;
    float diffuse = max(0.0, dot(lightDirection, normalW));
    vec3 rockTexture = texture2D(u_texture0, v_uv/1.1).xyz;
    //float diffuse = max(dot(normalW, lightDirection), 0.0);
    //vec4 colorWater = vec4((diffuse * (colorReflection + u_lightColor)).rgb, colorReflection.a); 
    vec4 colorRock = vec4(diffuse * rockTexture.rgb * u_lightColor.rgb, 1.0);
    //vec4 colorRock = vec4((/*ambientColor * */weight + diffuse * (1.0 - weight) * vec4(rockTexture, 1.0) + SpecularComponent).xyz, 1.0);
    vec4 waterEff = texture2D(u_texture2, v_uv);
    vec4 blendRockWater = mix(colorRock, waterEff, 0.5);
    //gl_FragColor = colorRock;
    gl_FragColor = colorRock;
}
