precision mediump float;

uniform sampler2D u_texture0;	//Rock
uniform sampler2D u_texture1;	//RockNormalMap
uniform vec3 u_lightPos;
uniform vec4 u_lightColor;

varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying vec3 v_normalW;

void main()
{
	vec3 normal = texture2D(u_texture1, v_uv).xyz;
    mat3 TBN = mat3(normalize(v_tangentW), normalize(v_bitangentW), normalize(v_normalW));
    vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
    vec3 lightDirection = normalize(v_posW - u_lightPos);
    float diffuse = max(0.0, dot(lightDirection, normalW));
    vec3 rockTexture = texture2D(u_texture0, v_uv).xyz;
    //float diffuse = max(dot(normalW, lightDirection), 0.0);
    //vec4 colorWater = vec4((diffuse * (colorReflection + u_lightColor)).rgb, colorReflection.a); 
    vec4 colorRock = vec4(diffuse * rockTexture.rgb * u_lightColor.rgb, 1.0);
    
    gl_FragColor = colorRock;
}
