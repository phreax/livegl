uniform vec2 resolution;
uniform vec2 macro;
uniform float time;
uniform vec3  sound; 


vec2 multc(vec2 a, vec2 b) {
    return vec2(a.x*b.x - a.y*b.y, a.y*b.x+a.x*b.y);
}

float mandeldist(vec2 p) {
    vec2 c = p-macro;
    vec2 z = vec2(0.0,0.0);
    vec2 dz = vec2(1.,0.);
    float dmin = 1000.;

    float m2;

    for(int i=0; i<100;i++) {
        dz = 2.0 * multc(z,dz) + 1.0;
        z.x = abs(z.x);
        z.y = abs(z.y);
        z = c+ vec2(z.x*z.x -z.y*z.y,2.0*z.x*z.y);
        m2 = dot(z,z);
        if(m2>1000.0) break;
        dmin = min(dmin,m2);
    }

    /*return sqrt(m2/dot(dz,dz))*0.5*log(m2);*/
    return (dmin);
}

void main(void) {
    vec2 p = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;
    p.y *=-1.;
    float d = mandeldist(p*1.5);
    gl_FragColor = vec4(d);
}
 
