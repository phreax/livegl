uniform float time;
uniform vec2 resolution;
uniform vec3 sound; 

float maxcomp(vec3 v) {
    return max(max(v.x,v.y),v.z);
}

float box(vec3 p, vec3 t, vec3 b) {
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
    return (sin(11.0*p.x)*sin(10.0*p.y)*sin(7.*p.z) )*0.004*sound.x;
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

void rY(inout vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	p.x = c * q.x + s * q.z;
	p.z = -s * q.x + c * q.z;
}

void rZ(inout vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	p.x = c * q.x - s * q.y;
	p.y = s * q.x + c * q.y;
}

void rX2(inout vec3 p, float s, float c) {
	vec3 q=p;
	p.y = c * q.y - s * q.z;
	p.z = s * q.y + c * q.z;
}

float map(vec3 p, out int matID) {

    float dp2 = planeY(p,0.0)-abs(deform(p));
    float s = sin(50.0*time);
    p.y /= 1.1+s;
    p.x *=  clamp(0.01*sound.x, 1.0, 2.0);
    float ds = sphere(p,vec3(0.0,0.05+0.02*s,2.0*time+0.3),0.04);
    if(ds>dp2) matID = 0;
    else matID = 1;
    return min(dp2,ds);
    
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
    float maxt = 4.0;
    int maxstep = 100;
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

    float t = time;
    vec3 ls = vec3(0.1,0.5,2.0*t+3.7);
    float asp = resolution.x/resolution.y;
    
    vec3 ro = vec3(0.0,0.1,2.0*t);
    vec3 rd = normalize(vec3(p.x*asp,p.y-0.5,1.0));

    float tmin=10.0;
    int matID = 0;
        
    vec3 col = vec3(0.43,0.44,0.51); 
    vec3 vl,hl;
    float spec;
    int steps;

    bool hit = false;
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
            col = dif*vec3(0.1,0.82,0.95)+spec*sound.y*0.1+g*g*vec3(0.33,0.21,0.6);
            
            //col = mix(dif*vec3(0.1,0.8,0.1),g*vec3(0.0,0.0,0.4),g)+0.6*spec*vec3(0.2,0.5,0.23);
        }
        if(matID == 1)
            col = sound.x*0.1*vec3(0.1,0,0)+vec3(0.18,0.77,0.02)+spec;
        hit = true;
    }


   vec2 sun = vec2(0.0,0.45);
   p.x /= 1.1;

   col = mix(col,vec3(0.1,0.65,0.76),1.0-exp(-tmin*0.51));
   if(matID != 1)
       col = mix(col,vec3(0.85,0.89,0.91),sound.y*0.1*exp(-4.0*length(sun-p)));

   gl_FragColor = vec4(col,1.0);
}
