uniform vec2 resolution;
uniform float time;
uniform vec3 sound;

varying vec3 eyePos, vPos;
uniform vec3 camd;

float maxcomp(vec3 v) {
    return max(max(v.x,v.y),v.z);
}

float box(vec3 p) {
    vec3 t = vec3(0.0,-1.0,0);
    vec3 b = vec3(0.1);
    p -=t;
    vec3 d = abs(p)-b;
    float mc = maxcomp(b); 
    return min(mc,length(max(d,0.0))) ;
 }
 
float torus( vec3 p) {

    vec3 t = vec3(0,0,3.2);
    p -=t;
    vec2 q = vec2(length(p.xz)-0.2,p.y);
    return length(q)-0.2;
}

float deform(vec3 p) {
    return sin(5.0*time)*0.1*(sin(14.0*p.x)*sin(10.0*p.y)*sin(11.*p.z) )*0.01*sound.y;
}

float sphere(vec3 p,float r) {
    
    return length(p-vec3(0.0,0.2,0.4))-r;
}

float plane(vec3 p, vec4 n) {
    return dot(p,n.xyz) - n.w;
}

vec3 mod3f(vec3 v, float x) {
    vec3 res;
    res.x = mod(v.x,x);
    res.y = mod(v.y,x);
    res.z = mod(v.z,x);
    
    return res;
}

float opRep(vec3 p, float c) {
    vec3 q = vec3(mod(p.x,0.8*c),mod(p.y,1.2*c),mod(p.z,c))-0.5*c;
    return sphere(q,0.3);
}
/*
float planeX(vec3 p, float x) {
    return plane(p, vec4(1.0,0.0,0.0,x));
}

float planeY(vec3 p, float x) {
    return plane(p, vec4(0.0,0.0,0.0,x));
}

float planeZ(vec3 p, float x) {
    return plane(p, vec4(0.0,0.0,1.0,x));
}*/

float planeX(in vec3 p, in float x)
{
    return distance(p,vec3(x,p.y,p.z));
}

float planeY(in vec3 p, in float y)
{
    return distance(p,vec3(p.x,y,p.z));
}

float planeZ(in vec3 p, in float z)
{
    return distance(p,vec3(p.x,p.y,z));
}

float map(vec3 p, out int matID) {


    float dp1 = planeX(p,-1.0);
    float dp2 = planeX(p,1.0);
    float dp3 = planeY(p,-1.0);
    float dp4 = planeY(p,1.0);
    float dp5 = planeZ(p,1.0);
    float c = 2.2+1.3*cos(5.*time);
    float ds =  opRep(p,c) + deform(p);
    float ds2 = sphere(p,1.0) +deform(p*8.0);
    float ds1 = sphere(p.xyz,sound.y*0.01) +deform(p);

    matID = 1; // plane
    float tmin =  min(min(min(min(min(dp1,dp2),dp3),dp4),dp5),ds);
    /*float tmin = ds1;*/
    if(tmin == dp2) matID = 2;
    if(tmin == dp3) matID = 3;
    if(tmin == dp4) matID = 4;
    if(tmin == dp5) matID = 5;
    if(tmin == ds) matID = 6;
    if(tmin == ds1) matID = 7;
    matID = 7;
    return tmin;
}  

vec3 getnormal( vec3 p )
{
    float eps = 0.002;
    int id;
    vec3 n =   vec3( map( p+vec3(eps,0.0,0.0),id) - map( p-vec3(eps,0.0,0.0),id),
                     map( p+vec3(0.0,eps,0.0),id) - map( p-vec3(0.0,eps,0.0),id),
                     map( p+vec3(0.0,0.0,eps),id) - map( p-vec3(0.0,0.0,eps),id) );
    return normalize( n );
}

bool raycast(in vec3 ro, in vec3 rd, out float t, out int steps, out int matID) {
    float maxt = 4.0;
    int maxstep = 36;
    t = 0.0;
    float d;

    for(int i=0;i<maxstep&&maxt>=t;i++) {
        vec3 p = ro+rd*t;
        d = map(p,matID);

        if(d<0.003) {
            steps = i;
            return true;
        }
        t+=d;
    } 
    return false;

}

void main() {

    vec2 p = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;

    vec3 ls = vec3(0.1,0.7,-2.10);
    float asp = resolution.x/resolution.y;
    
    vec3 ro = vec3(0.0,0.5,-2.9);
    vec3 rd = normalize(vec3(p.x*asp,0.2+p.y,1.0));

    float tmin;
    int matID = 0;
        
    vec3 col = vec3(0.0,0.0,0.0); 
    vec3 vl,hl;
    float spec;
    int steps;
    if(raycast(ro,rd,tmin,steps,matID)) {
        vec3 inter = ro+rd*tmin;
        vec3 l = normalize(ls-inter);
        vec3 n = getnormal(inter); 
        float dif = max(dot(n,l),0.0);

        vl = normalize(ro-inter);
        hl = normalize(l+vl);
        spec = pow(max(dot(hl,n),0.0),44.0);
        if(dif<=0.0) spec=0.0;

        // ambient occlusion
        float ao;
        int id;
        float totao = 0.0;
        float sca = 1.0;
        for( int aoi=0; aoi<5; aoi++ )
        {
            float hr = 0.01 + 0.02*float(aoi*aoi);
            vec3 aopos = n * hr + inter;
            float dd = map( aopos, id);
            ao = -(dd-hr);
            totao += ao*sca;
            sca *= 0.75;
        }
        ao = 1.0-clamp(totao,0.0,1.0);
        float g = float(steps)/50.0;

        if(matID == 1) {
            col = vec3(0.7,0.1,0.0);
        }
        else if(matID == 2) {
            col = vec3(0.0,0.1,0.6);
        }
        else if(matID == 3) {
            col = vec3(0.7,0.6,0.7);
        }
        else if(matID == 4) {
            col = vec3(0.7,0.6,0.7);
        }
        else if(matID == 5) {
            col = vec3(0.7,0.6,0.7);
        }

        else if(matID == 6) {
            //col = dif*vec3(0.4,0.4,0.5)+spec*0.9+g;
            col = 3.0*vec3(0.1,0.2,0.5)*g*g + 0.2*spec*vec3(0.2,0.32,0.23);
        }
        else if(matID == 7) {
            //col = dif*vec3(0.4,0.4,0.5)+spec*0.9+g;
            col = mix(dif*vec3(0.1,0.8,0.1),g*vec3(0.0,0.0,0.4),g)+0.6*spec*vec3(0.2,0.5,0.23);
        }
        col = col;// + 0.2*vec3(0.1,0.2,0.5)*tmin;
    }

        
   gl_FragColor = vec4(col,1.0);
}
