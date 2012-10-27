uniform vec2 resolution;
uniform vec3 sound;
uniform float time;
uniform sampler1D spectrum;

void main() {

    vec2 p = gl_FragCoord.xy/resolution.xy;
    float asp = resolution.x/resolution.y;
    vec2 o = vec2(p.x*asp,p.y);

    vec2 c1 = vec2(0.25*asp,0.25);
    vec2 c2 = vec2(0.5*asp,0.75);
    vec2 c3 = vec2(0.75*asp,0.25);
    float s = 0.01*sound.y;

    vec4 tex = 0.01*texture1D(spectrum,p.x);

    float ds = length(o-c1);
    if(p.y < tex.g && p.y + 0.01 > tex.g)
        gl_FragColor = vec4(0.0,0.0,1.0,1) +p.y*0.1*sound.y*vec4(0.0,0.0,-1.0,0);
    else if(length(o-c1)<0.1)
        gl_FragColor = mix(vec4(0.0,0.03,0.21,1.0),vec4(0.09,0.29,0.24,0), 0.01*sound.x);
    else if(length(o-c2)<0.2)
        gl_FragColor = mix(vec4(0.0,0.03,0.21,1.0),vec4(0.29,0.24,0.09,0), 0.1*sound.y);
    else if(length(o-c3)<0.1)
        gl_FragColor = mix(vec4(0.0,0.03,0.21,1.0),vec4(0.24,0.09,0.29,0), 0.1*sound.z);
    else 
        gl_FragColor = vec4(0.0,0.03,0.21,1.0);

}    

