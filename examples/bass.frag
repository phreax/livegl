uniform float time;
uniform vec2 resolution;
uniform vec3 sound;

float maxcomp(vec3 v) {
    return max(max(v.x,v.y),v.z);
}

float box(vec3 p) {
    vec3 t = vec3(0.0,0.0,0);
    vec3 b = vec3(0.07);
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
    return clamp(sound.x,0.0,8.0)*0.1*(sin(14.0*p.x)*sin(20.0*p.y)*sin(17.*p.z) );
}

float sphere(vec3 p,vec3 t, float r) {
    return length(p-t)-r;
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

vec3 opRep(vec3 p, float c) {
    float d = p.z/c;
    return vec3(mod(p.x,c),mod(p.y,c),mod(p.z,c))-0.5*c;
}

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

float impulse( float k, float x) {
    float h = k*x;
    return h*exp(1.0-h);
}

void rX(inout vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	p.y = c * q.y - s * q.z;
	p.z = s * q.y + c * q.z;
}

float map(vec3 p, out int matID) {

    float g = 1.0;
    
    float t = time;
    rX(p,t*4.0);
    vec3 q = vec3(0.7*p.x,p.y,p.z);

    //float ds1 = sphere(opRep(p,0.3*sound.x),vec3(0.0,0.5,0.0),0.05*sound.x);

    //float s = sin(4.0*time);
    //float r = impulse(12.0,mod(time,1.0));
    float dsb = sphere(q,vec3(0.0,0.5,0.3),1.5+0.05);
    float db = box(opRep(q,0.3)); 

    matID =0;

    return max(db,dsb);

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

bool intersect(in vec3 ro, in vec3 rd, out float t, out int steps, out int matID) {
    float maxt = 3.0;
    int maxstep = 36;
    t = 0.0;
    float d;

    for(int i=0;i<maxstep&&maxt>=t;i++) {
        vec3 p = ro+rd*t;
        d = map(p,matID);

        if(d<0.01) {
            steps = i;
            return true;
        }
        t+=d;
    } 
    return false;

}

void main() {

    vec2 p = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;

    vec3 ls = vec3(0.0,0.6,0.4);
    float asp = resolution.x/resolution.y;
    
    vec3 ro = vec3(0.0,0.5,-2.2);
    vec3 rd = normalize(vec3(p.x*asp,0.2+p.y,1.0));

    float tmin;
    int matID = 0;
        
    vec3 col = vec3(0.0,0.0,0.0); 
    vec3 vl,hl;
    float spec;
    int steps;

    if(intersect(ro,rd,tmin,steps,matID)) {
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
        /*for( int aoi=0; aoi<5; aoi++ )
        {
            float hr = 0.01 + 0.02*float(aoi*aoi);
            vec3 aopos = n * hr + inter;
            float dd = map( aopos, id);
            ao = -(dd-hr);
            totao += ao*sca;
            sca *= 0.75;
        }
        ao = 1.0-clamp(totao,0.0,1.0);
*/
        float g = float(steps)/50.0;
        if(matID == 0) {
            col = dif*vec3(0.6,0.5,0.6)+spec*0.6*sound.y*vec3(0.0,0.6,0.3)+7.0*vec3(0.25,0.2,0.5)*g*g*g;
            
            //col = mix(dif*vec3(0.1,0.8,0.1),g*vec3(0.0,0.0,0.4),g)+0.6*spec*vec3(0.2,0.5,0.23);
        }
    }
   gl_FragColor = vec4(col,1.0);
}
