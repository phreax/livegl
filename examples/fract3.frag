uniform vec2 resolution;
uniform float time;
uniform vec3 sound;

void main(void) {
    float t = 3.*time;
    vec2 z = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;

    float cy = 0.2+ .6*sin(1.9*t)+0.01*sound.y;
    float cx = cos(1.7*t);
    z = z*2.;
    /*z *= sin(t);*/
    vec2 cc = vec2(cx,cy);
    float dmin = 10000.0;
    float m2 = 0.0;

    for(int i=0; i<32;i++) {
        /*z.x = abs(z.x);*/
        z.y = .5-abs(z.y)+mod(t,0.8);
        z.x = .5-abs(z.x)+sin(t);
        /*z.x = 1.-abs(cos(z.x));*/
        z = cc+ vec2(z.x*z.x -z.y*z.y,2.0*z.x*z.y);
        m2 = dot(z,z);
        if(m2>100.0) break;
        dmin = min(dmin,m2);
    }

    float c = sqrt(dmin);
       gl_FragColor = 0.1*sound.y*vec4(-0.2,0.1,0.0,0)+vec4(0.13,0.34*c,1.0-3.0*c,1.0);
    /*if(z.x>0.0) gl_FragColor = pow(gl_FragColor,vec4(4.0));*/
    if(z.y>0.0) gl_FragColor = pow(gl_FragColor,vec4(4.0));


}
