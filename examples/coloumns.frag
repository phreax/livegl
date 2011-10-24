#extension GL_EXT_gpu_shader4 : enable

uniform float time;
uniform vec2 resolution;



float plane(vec2 c) {
    return -1.2;
}

float f(vec2 c) {

    float h1 = if(c.y > 0.2 && c.y <0.23 && c.x >0.2 && c.y<0.23)
    return -1.2;
}
vec3 getnormal(vec3 p) {

    float eps = 0.01;
    vec3 n = vec3(f(vec2(p.x-eps,p.z))-f(vec2(p.x+eps,p.z)),
                   2.0*eps,
                  f(vec2(p.x,p.z-eps))-f(vec2(p.x,p.z+eps)));

    /*float r = noise3f(8.0*p)/2.0;
    n.x += r;
    n.y += noise3f(2.0*(r+p))/4.0;;
    n.z += noise3f(4.0*(n.y+p))/8.0;*/
    return normalize(n);
}

bool raycast(in vec3 ro, in vec3 rd, out float d) {
    float delt = 0.6;
    float mint = 0.01;
    float maxt = 10.0;
    float lh = 0.0;
    float ly = 0.0;

    for(float t= mint;t<maxt; t+=delt) {
        
        vec3 p = ro+rd*t;
        float h = f(p.xz);
        if(p.y<h) {
            d = t - delt + delt*(lh-ly)/(p.y-ly-h+lh);
            return true;
        }
        lh = h;
        delt += 0.03*t;
        ly = p.y;
    }
    return false;
}

void main() {

    vec2 p = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;

    vec3 ls = vec3(1.0,1.0,-2.0);
    vec2 sc = vec2(0.1,0.0);
    float di = 5.0;//-5.0*time; 
    
    float h = 5.0;
    vec3 rd = normalize(vec3(p.x*sc.x-sc.y,-0.6+p.y,sc.x+p.x*sc.y));
    vec3 ro = vec3(0.4+di*sc.y,h,-1.4-di*sc.x);//+0.3*vec3(sin(time),0.0,0.3*cos(time));
    
    float tmin;
    float spec;
    vec3 vl,hl;
    if(raycast(ro,rd,tmin)) {
        vec3 inter = ro+rd*tmin;
        vec3 l = normalize(ls-inter);
        vec3 n = getnormal(inter);

        float dif = max(dot(l,n),0.0);

        vl = normalize(ro-inter);
        hl = normalize(l+vl);
        spec = pow(max(dot(hl,n),0.0),32.0);
        if(dif<=0.0) spec=0.0;

        vec3 col = vec3(1.0);
    //    if(mod(inter.x,0.2)>0.1 ^^ mod(inter.z,0.2)>0.1)
      ////      col = vec3(0.0);
        gl_FragColor = dif*vec4(col,1.0)+0.2*spec; 
    }

    else {
        vec3 inter = ro+rd*tmin;
        vec3 col = vec3(0.0); // - (p.y)*vec3(0.3,0.6,0.5); 
        gl_FragColor = vec4(col,1.0);
    }

    
    
}
