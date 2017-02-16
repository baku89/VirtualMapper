#version 120

uniform sampler2DRect sceneTex;
uniform sampler2DRect screenTex;
uniform sampler2DRect screenMask;

varying vec2 texCoordVarying;

vec3 blendScreen(vec3 base, vec3 blend) {
	return 1.0-((1.0-base)*(1.0-blend));
}

vec3 blendScreen(vec3 base, vec3 blend, float opacity) {
	return (blendScreen(base, blend) * opacity + base * (1.0 - opacity));
}

void main()
{
    vec3 sceneColor = texture2DRect(sceneTex, texCoordVarying).rgb;
    vec3 screenColor = texture2DRect(screenTex, texCoordVarying).rgb;

    float screenAlpha = texture2DRect(screenMask, texCoordVarying).r;

    vec3 color = blendScreen(sceneColor, screenColor, screenAlpha);

    // gl_FragColor = vec4(sc, 1.0);
    gl_FragColor = vec4(color, 1.0);
}