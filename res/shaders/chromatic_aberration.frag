#version 130

uniform sampler2D source;
uniform vec2 resolution;
uniform vec2 rOffset;
uniform vec2 bOffset;
uniform vec2 gOffset;

void main () 
{
    vec2 uv = vec2(gl_FragCoord.xy / resolution.xy);

    vec4 rValue = texture2D(source, uv + rOffset);  
    vec4 bValue = texture2D(source, uv + bOffset);  
    vec4 gValue = texture2D(source, uv + gOffset);

    // Combine the offset colors.
    gl_FragColor = vec4(rValue.r, gValue.g, bValue.b, 1.0);
}