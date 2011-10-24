#extension GL_EXT_gpu_shader4 : enable

uniform float time;
uniform vec2 resolution;

float noise(float seed) {
    
    int x = int(fract(seed)*14234329.0);
    x = (x<<13) ^ x;
    return ( 1.0 - float( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);   
}

float smoothnoise(float x) {
    float eps = 0.001;
    return noise(x)/2.0 + noise(x-eps)/4.0 + noise(x+eps)/4.0;
}

float f(vec2 c) {
    float z= sin(c.x)*cos(c.y) + 0.01*sin(16.0*c.x)*cos(16.0*c.y);
    return z;
}

vec3 getnormal(vec3 p) {

    float eps = 0.01;
    vec3 n = vec3(f(vec2(p.x-eps,p.z))-f(vec2(p.x+eps,p.z)),
                   2.0*eps,
                  f(vec2(p.x,p.z-eps))-f(vec2(p.x,p.z+eps)));

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

    vec3 ls = vec3(0.5,4.0,10.0);
    vec2 sc = vec2(.7,.02);
    float di = 4.0*-time; 
    float h = 2.0;
    vec3 rd = normalize(vec3(p.x*sc.x-sc.y,p.y,sc.x+p.x*sc.y));
    vec3 ro = vec3(di*sc.y,h,-di*sc.x);
    
    float tmin;
    if(raycast(ro,rd,tmin)) {
        vec3 inter = ro+rd*tmin;
        vec3 l = normalize(ls-inter);
        vec3 n = getnormal(inter);

        float dif = max(dot(l,n),0.0);

        gl_FragColor = dif*vec4(1.0,0.5,0.2,0.6)*3.0/tmin;
    }

    else {
        vec3 col = vec3(0.3,0.6,1.0); 
        gl_FragColor = vec4(col,1.0);
    }

    
    
}
