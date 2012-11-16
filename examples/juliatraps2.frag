uniform vec2 resolution;
uniform vec2 macro;
uniform vec3 camd;
uniform vec3 sound;
uniform float time;

struct line2 {
    vec2 o;
    vec2 d;
};

float distLine(line2 line,vec2 p) {
    vec2 n = normalize(line.d);
    vec2 o = line.o; 
    return length((o-p)-dot(o-p,n)*n);
}

void main(void) {
    vec2 z = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;
    z.xy += camd.xy;
    float t = 5.*time;
    float cx = sin(t); 
    float cy = cos(t)+0.01*sound.x;  
    z = z*1.5*(1.0-0.1*cy);
    vec2 cc = vec2(cx,cy);
    float dist1 = 10000.0;
    float dist2 = 10000.0;
    float dist3 = 10000.0;
    float m2 = 0.0;

    vec2 p1 = macro;
    vec2 p2 = vec2(0.9,-.4);
    
    line2 l1 = line2(vec2(-0,-1),vec2(1,1));
    line2 l2 = line2(vec2(-1,-0),vec2(2,-1));
    line2 l3 = line2(vec2(-1,-1),vec2(2,-4));

    for(int i=0; i<50;i++) {
        z.x = .5-abs(z.x);
        z.y = 1.-abs(z.y)+sin(t);

        z = cc+ vec2(z.x*z.x -z.y*z.y,2.0*z.x*z.y);
        m2 = dot(z,z);
        if(m2>100.) {
          break;
        }
        dist1 = min(dist1,distLine(l1,z));
        dist2 = min(dist2,distLine(l2,z));
        dist3 = min(dist3,distLine(l3,z));
    }

    float dmin = min(dist1,min(dist2,dist3));
    if(dmin==dist1) {
        dmin = 1./dmin;
        gl_FragColor = 0.1*dmin*vec4(0.8,0.3,.2,1.0)*0.01*sound.x;
    } else if(dmin==dist2) {
        dmin = 1./dmin;
        gl_FragColor = 0.1*dmin *vec4(0.2,0.6,0.89,1.0)*0.05*sound.y;
    } else if(dmin==dist3) {
        dmin = 1./dmin;
        gl_FragColor = 0.1*dmin *vec4(0.2,0.89,0.12,1.0)*0.05*sound.z;
    }

    gl_FragColor = 1. - gl_FragColor;
    /*gl_FragColor = vec4(0.35,1.0,1.0,1.0) - dist1*vec4(0.1,0.5,0.9,0.0);*/
    /*gl_FragColor += 0.7*dist2*vec4(0.1,.1,1.0,0.0);*/
    if(z.x>0.0) gl_FragColor = pow(gl_FragColor,vec4(6.0));


}
