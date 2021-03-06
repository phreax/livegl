uniform vec2 resolution;
uniform float time;
uniform vec3  sound; 

void main(void) {
    vec2 z = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;
    z *= 1.0;

    float cy = 0.2+ .6*sin(10.*time);
    float cx = -cos(10.*time);
    z = z*1.5*(1.0-0.1*cy);
    vec2 cc = vec2(cx,cy);
    float dmin = 10000.0;
    float m2 = 0.0;

    for(int i=0; i<32;i++) {
        z = cc+ vec2(z.x*z.x -z.y*z.y,2.0*z.x*z.y);
        m2 = dot(z,z);
        if(m2>100.0) break;
        dmin = min(dmin,m2);
    }

    float c = sqrt(dmin);
       gl_FragColor = vec4(0.0,1.-10.0*c,1.0-3.0*c,1.0);
    /*if(z.x>0.1) gl_FragColor = pow(gl_FragColor,vec4(2.0));*/


}
