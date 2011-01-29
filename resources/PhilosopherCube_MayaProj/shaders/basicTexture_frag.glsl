#version 120

uniform sampler2D texture;
uniform float alpha;
//uniform vec2 sampleDivisor;

void main()
{
	vec4 alphaVec = vec4(0.0, 0.0, 0.0, ( 1.0 - alpha) );
	gl_FragColor = texture2D( texture, gl_TexCoord[0].st ) - alphaVec;
}