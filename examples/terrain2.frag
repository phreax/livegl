#extension GL_EXT_gpu_shader4 : enable

uniform float time;
uniform vec2 resolution;

float noise(int x) {
    
    x = (x<<13) ^ x;
    return ( 1.0 - float( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff))/1073741824.0;   
}


float noise2f(vec2 v) {
    ivec2 ip = ivec2(floor(v));
    vec2 u = fract(v);

    int n = ip.x + ip.y * 57;

    u = u*u*(3.0-2.0*u);

    float res = mix(mix(noise(n+57),noise(n),mix(noise(n-1), noise(n+1), u.x)),noise(n-57),u.y);
    return res;
}

float smoothnoise(int x) {
    return noise(x)/2.0 + noise(x-1)/4.0 + noise(x+1)/4.0;
}

float noise3f( in vec3 x, int sem )
{
    ivec3 ip = ivec3(floor(x));
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);
   //f = f*f*f*(f*(f*6.0-15.0)+10.0);

    int n = ip.x + ip.y * 57 + 113*ip.z + sem;

    float res = mix(mix(mix( noise(n+(0+57*0+113*0)), noise(n+(1+57*0+113*0)),f.x),
                        mix( noise(n+(0+57*1+113*0)), noise(n+(1+57*1+113*0)),f.x),f.y),
                    mix(mix( noise(n+(0+57*0+113*1)), noise(n+(1+57*0+113*1)),f.x),
                        mix( noise(n+(0+57*1+113*1)), noise(n+(1+57*1+113*1)),f.x),f.y),f.z);
    return 1.0 - res;
}

float map(vec2 c) {
   //f = 0.5*noise3f(vec3(c,0.0),4);
   float p=0.5,o=1.0,f=0.0;
   for(int i=0;i<1;i++) {
    f += p*noise3f(o*vec3(c,1.0),4);
    p /=2.0;o*=2.0;
   }
   f = 0.5+0.5*f;
   f = f*f*(3.0-2.0*f*f);
   f = -2.5+1.5*f*f; 

   return f;
}

vec3 getnormal(vec3 p) {

    float eps = 0.01;
    vec3 n = vec3(map(vec2(p.x-eps,p.z))-map(vec2(p.x+eps,p.z)),
                   2.0*eps,
                  map(vec2(p.x,p.z-eps))-map(vec2(p.x,p.z+eps)));

//    n.z += 0.01*smoothnoise(n.z);
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
        float h = map(p.xz);
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

    vec3 ls = vec3(0.0,6.0,3.0);
    vec2 sc = vec2(0.3,0.0);
    float di = 4.0*-time;
    
    float h = 0.3;
    vec3 rd = normalize(vec3(p.x*sc.x-sc.y,-h+p.y,sc.x+p.x*sc.y));
    vec3 ro = vec3(0.5+di*sc.y,h,-0.3-di*sc.x); //vec3(sin(time),0.0,0.3*cos(time));
    
    float tmin;
    if(raycast(ro,rd,tmin)) {
        vec3 inter = ro+rd*tmin;
        vec3 l = normalize(ls-inter);
        vec3 n = getnormal(inter);

        float dif = max(dot(l,n),0.0);

        vec3 col = dif*vec3(1.0,0.4,0.2)/tmin + vec3(0.0,0.0,0.1)*tmin*min(0.4*exp(tmin),0.4);
        gl_FragColor = vec4(col,1.0); 

    }

    else {
        vec3 col = vec3(0.3,0.7,1.0) - p.y*vec3(0.3,0.6,0.5); 
        gl_FragColor = vec4(col,1.0);
    }

    
    
}
