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
    z *= 0.5;
    float t = 5.*time;
    /*float cx = macro.x; */
    /*float cy = macro.y;  */
    float cx = sin(t); 
    float cy = cos(t)+0.01*sound.x;  
    z = z*1.5*(1.0-0.1*cy);
    vec2 cc = vec2(cx,cy);
    float dist1 = 10000.0;
    float dist2 = 10000.0;
    float dist3 = 10000.0;
    float m2 = 0.0;
    float d;

    vec2 pmin = vec2(0,0);
    vec2 p1 = vec2(0.6,0.3);
    /*vec2 p2 = vec2(sin(z.x),cos(z.y));*/

    /*line2 l1 = line2(vec2(-1,4),vec2(1,cos(3.0*time)));*/
    /*line2 l2 = line2(vec2(-1,-1),vec2(cos(1.*time),sin(1.*time)));*/

    line2 l1 = line2(vec2(-0,-1),vec2(1,1));
    line2 l2 = line2(vec2(-1,-0),vec2(2,-1));

    for(int i=0; i<100;i++) {
        z = cc+ vec2(z.x*z.x -z.y*z.y,2.0*z.x*z.y);
        m2 = dot(z,z);
        if(m2>100.) {
          break;
        }
        
        d = distLine(l2,z);
        if(d<dist1) {
           pmin = z; 
        }
        dist1 = min(dist1,d);
        /*dist2 = min(dist2,);*/
        dist3 = min(dist3,distLine(l2,z));
    }

       
    gl_FragColor = pmin.x*pmin.x*vec4(0.25,1.0,1.0,1.0) + pmin.y*pmin.y*vec4(0.2,0.1,0.2,0.0);
    /*gl_FragColor -= dist1*vec4(0.3,.1,1.0,0.0);*/
    gl_FragColor -= 0.01/dist3*vec4(0.1,.4,0.1,1.0);
    /*if(z.x>0.0) gl_FragColor = pow(gl_FragColor,vec4(2.0));*/


}
