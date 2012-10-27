uniform vec2 resolution;
uniform float time;
uniform float bands_smooth[3];

void main(void) {
    vec2 z = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;
    z *= 2.0;

    float cy = 0.2+ .6*sin(1.9*time)+0.01*bands_smooth[1];
    float cx = -cos(1.7*time)+0.01*bands_smooth[0];
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
       gl_FragColor = bands_smooth[2]*vec4(-0.2,0.1,0.0,0)+vec4(0.0,1.-1.0*c,1.0-3.0*c,1.0);
    if(z.x>0.0) gl_FragColor = pow(gl_FragColor,vec4(4.0));


}
