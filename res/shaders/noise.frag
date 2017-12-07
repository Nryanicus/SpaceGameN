#version 130

uniform sampler2D source;
uniform vec2 resolution;
uniform float rand;
uniform float noiseProportion;

float noise(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() 
{ 
    vec4 sourceFragment = texture2D(source, gl_TexCoord[0].xy);
    vec4 noiseFragment = vec4( vec3( noise(vec2(gl_FragCoord.xy / resolution.xy / rand)) ), 1.0);
    gl_FragColor = sourceFragment + noiseFragment*noiseProportion;
}
