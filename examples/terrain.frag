#extension GL_EXT_gpu_shader4 : enable

uniform float time;
uniform vec2 resolution;

float noise(int x) {
    
    //int x = int(fract(seed)*14234329.0);
    x = (x<<13) ^ x;
    return ( 1.0 - float( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);   
}

float smoothnoise(int x) {
    return noise(x)/2.0 + noise(x-1)/4.0 + noise(x+1)/4.0;
}



int icoolfFunc3d2( in int n )
{
    n=(n<<13)^n;
    return (n*(n*n*15731+789221)+1376312589) & 0x7fffffff;
}

float coolfFunc3d2( in int n )
{
    return float(icoolfFunc3d2(n));
}

float noise3f( in vec3 p )
{
    ivec3 ip = ivec3(floor(p));
    vec3 u = fract(p);
    u = u*u*(3.0-2.0*u);

    int n = ip.x + ip.y*57 + ip.z*113;

    float res = mix(mix(mix(coolfFunc3d2(n+(0+57*0+113*0)),
                            coolfFunc3d2(n+(1+57*0+113*0)),u.x),
                        mix(coolfFunc3d2(n+(0+57*1+113*0)),
                            coolfFunc3d2(n+(1+57*1+113*0)),u.x),u.y),
                    mix(mix(coolfFunc3d2(n+(0+57*0+113*1)),
                            coolfFunc3d2(n+(1+57*0+113*1)),u.x),
                        mix(coolfFunc3d2(n+(0+57*1+113*1)),
                            coolfFunc3d2(n+(1+57*1+113*1)),u.x),u.y),u.z);

    return 1.0 - res*(1.0/1073741824.0);
}

float fbm( in vec3 p )
{
    return 0.5000*noise3f(p*1.0) +
           0.2500*noise3f(p*2.0) +
           0.1250*noise3f(p*4.0) +
           0.0625*noise3f(p*8.0);
}
float f(vec2 c) {
    return -1.2+ 2.0*sin(10.0*c.x)*cos(10.0*c.y);// + noise3f(4.0*vec3(c,0.0));
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

    vec3 ls = vec3(0.0,-3.0,-3.0);
    vec2 sc = vec2(0.1,0.0);
    float di = 4.0*-time;
    
    float h = 0.3+0.5*sin(time);
    vec3 rd = normalize(vec3(p.x*sc.x-sc.y,-h+p.y,sc.x+p.x*sc.y));
    vec3 ro = vec3(0.4+di*sc.y,h,-1.4-di*sc.x) +0.3*vec3(sin(time),0.0,0.3*cos(time));
    
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

        vec3 col = 0.6*spec/tmin + dif*vec3(1.0,0.4,0.2)/tmin +vec3(0.0,0.0,0.2)*tmin*min(0.4*exp(tmin),0.4);
        gl_FragColor = vec4(col,1.0); 

    }

    else {
        vec3 inter = ro+rd*tmin;
        vec3 col = vec3(0.3,0.7,1.0) - (p.y)*vec3(0.3,0.6,0.5); 
        gl_FragColor = vec4(col,1.0);
    }

    
    
}
