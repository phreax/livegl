uniform vec2 resolution;
uniform float time;
uniform sampler1D spectrum;
uniform float bands_three[3];

void main() {

    vec2 p = gl_FragCoord.xy/resolution.xy;
    float asp = resolution.x/resolution.y;
    vec2 o = vec2(p.x*asp,p.y);

    vec2 c1 = vec2(0.5*asp,0.75);
    float s = 0.001*bands_three[1];

    vec4 tex = 0.01*texture1D(spectrum,p.x);

    float ds = length(o-c1);
    if(p.y < tex.g && p.y + 0.01 > tex.g)
        gl_FragColor = vec4(0.0,0.0,1.0,1) +p.y*0.1*bands_three[2]*vec4(1.0,0,-1.0,0);
    else if(ds<0.1)
        gl_FragColor = vec4(0.3,0.5,0.01,0)*0.01*bands_three[2];
    else 
        gl_FragColor = vec4(0.0,0.03,0.07,1.0);

}    

