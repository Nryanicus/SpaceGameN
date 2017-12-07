#version 130
#pragma glslify: random = require(glsl-random) 

uniform sampler2D texture;
uniform vec2 direction;
uniform vec2 resolution;
uniform float seed;
uniform float time;

float noise(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) 
{
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.3333333333333333) * direction;
    color += texture2D(image, uv) * 0.29411764705882354;
    color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;
    color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;
    return color; 
}

vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) 
{
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.3846153846) * direction;
    vec2 off2 = vec2(3.2307692308) * direction;
    color += texture2D(image, uv) * 0.2270270270;
    color += texture2D(image, uv + (off1 / resolution)) * 0.3162162162;
    color += texture2D(image, uv - (off1 / resolution)) * 0.3162162162;
    color += texture2D(image, uv + (off2 / resolution)) * 0.0702702703;
    color += texture2D(image, uv - (off2 / resolution)) * 0.0702702703;
    return color;
}

vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) 
{
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.411764705882353) * direction;
    vec2 off2 = vec2(3.2941176470588234) * direction;
    vec2 off3 = vec2(5.176470588235294) * direction;
    color += texture2D(image, uv) * 0.1964825501511404;
    color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
    color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
    color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
    color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
    color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
    color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
    return color;
}

void main() 
{ 
    vec2 uv = vec2(gl_FragCoord.xy / resolution.xy);

    vec2 noise_arg = vec2(gl_FragCoord.xy / resolution.xy / seed);

    gl_FragColor = (1.5+sin(time*seed/100))*(blur13(texture, uv, resolution, direction)*0.4 + blur9(texture, uv, resolution, direction)*0.3 + blur5(texture, uv, resolution, direction)*0.3 )+ vec4(vec3(noise(noise_arg)), 1.0) * 0.05;
}
