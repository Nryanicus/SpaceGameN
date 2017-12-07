#version 130

// http://www.geeks3d.com/20100909/shader-library-gaussian-blur-post-processing-filter-in-glsl/

uniform sampler2D texture;
uniform float texture_height; // render target height
 
float offset[5] = float[]( 0, 1, 3, 5, 7 );
float weight[5] = float[]( 1, 0.12579, 0.10648, 0.08065, 0.05467 );

void main() 
{ 
  vec3 tc = vec3(1.0, 0.0, 0.0);
  vec2 uv = gl_TexCoord[0].xy;
  tc = texture2D(texture, uv).rgb * weight[0];
  for (int i=1; i<5; i++) 
  {
    tc += texture2D(texture, uv + vec2(0.0, offset[i])/texture_height).rgb \
            * weight[i];
    tc += texture2D(texture, uv - vec2(0.0, offset[i])/texture_height).rgb \
           * weight[i];
  }

  gl_FragColor = vec4(tc, 1.0);
}