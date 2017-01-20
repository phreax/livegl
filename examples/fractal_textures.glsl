struct line2 {
    vec2 o;
    vec2 d;
};


float distLine(line2 line,vec2 p) {
    vec2 n = normalize(line.d);
    vec2 o = line.o; 
    return length((o-p)-dot(o-p,n)*n);
}

vec4 julia(vec2 p) {
    vec2 z = p;
    float t = 5.*time;
    float cx = sin(t); 
    float cy = cos(t)+0.01*sound.x;  
    z = z*1.5*(1.0-0.1*cy);
    vec2 cc = vec2(cx,cy);
    float dist1 = 10000.0;
    float dist2 = 10000.0;
    float dist3 = 10000.0;
    float m2 = 0.0;

    line2 l1 = line2(vec2(-0,-1),vec2(1,1));
    line2 l2 = line2(vec2(-1,-0),vec2(2,-1));
    line2 l3 = line2(vec2(-1,-1),vec2(2,-4));

    for(int i=0; i<50;i++) {
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
    vec4 color;
    if(dmin==dist1) {
        dmin = 1./dmin;
        color = 0.1*dmin*vec4(0.8,0.3,.2,1.0);
    } else if(dmin==dist2) {
        dmin = 1./dmin;
        color = 0.1*dmin *vec4(0.2,0.6,0.89,1.0)*0.05*sound.y;
    } else if(dmin==dist3) {
        dmin = 1./dmin;
        color = 0.1*dmin *vec4(0.2,0.89,0.12,1.0)*0.05*sound.z;
    }

    return max(color,0.);
}

vec4 julia2(vec2 p) {
    vec2 z = p;
    z *= 1.0;

    float cy = 0.2+ .6*sin(10.*time);
    float cx = -cos(10.*time);
    z = z*1.5*(1.0-0.1*cy);
    vec2 cc = vec2(cx,cy);
    float dmin = 10000.0;
    float m2 = 0.0;

    vec4 color;

    for(int i=0; i<32;i++) {
        z = cc+ vec2(z.x*z.x -z.y*z.y,2.0*z.x*z.y);
        m2 = dot(z,z);
        if(m2>100.0) break;
        dmin = min(dmin,m2);
    }

    float c = sqrt(dmin);
    color = vec4(1.-c,-c,1.-c,1.0);
    if(z.x>0.1) color = pow(color,vec4(2.0));

    return color;
}
