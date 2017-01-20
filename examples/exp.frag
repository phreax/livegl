uniform float time;
uniform vec2 resolution;
uniform vec2 macro;
uniform vec3 sound; 

// require "toolbox.glsl"
// require "fractal_textures.glsl"

float scale = 5.0;
vec3 cube =  vec3(0.09+0.0002*sound.y);


vec3 opTwist( vec3 p )
{
    float c = cos((2.0+10.*macro.x)*p.y);
    float s = cos((2.0+10.*macro.y)*p.x);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xy,p.z);
    return q;
}

float minimumDistanceToTwistedCube(vec3 p)
{
    float t = 5.0*time-0.005*sound.z;
    // rotation angle according to "x"
    float rx= sin(p.x + t)*scale;
    float ry= sin(p.y + t)*scale;

    // 2d rotation matrix around x
    vec3 tmp= p;
    float c = cos(rx); 
    float s = sin(rx);
    mat2  m = mat2(c,-s,s,c);

    c = cos(ry); 
    s = sin(ry);

    mat2  m2 = mat2(c,-s,s,c);
    // p.x keeps constant when rotating around the x axis
    p = vec3(p.x,m*p.yz);

    p.xz = m2*p.xz;
    /*p = vec3(p.x,m2*p.yz);*/
    /*p = vec3(m2*p.xz*0.6,p.y);*/

    // distance to center
    vec3 dist= abs(p) - cube;

    float d = box(tmp,vec3(0.1,0.05,2.0),vec3(0.0));
    // negative distances are inside the cube: clamp to 0
    dist= max( dist, 0.0 );
    
    /*p = 4.*tmp;*/
    /*dist += 0.01*sin(2.*p.z)*sin(2.*p.y)*sin(2.*p.x);*/

    // get euclidian distance to smooth distance at the edges
    // and use a much smaller step size to safely catch the twisted edges
    float d1 = length( dist ) * 0.4;
    d =  max(-d,d1);
    float displace = +0.03*sin(2.+50.2*p.z)*sin(2.+50.2*p.x);

    /*d += displace;*/
    return d;
}
float map(vec3 p, out int matID) {

 
    float ds = minimumDistanceToTwistedCube(p);
    /*if(ds>dp2) matID = 0;*/
    /*else matID = 1;*/
    return ds;
}

vec3 blend_weights(vec3 n) {
    vec3 weights = abs(n);
    weights = (weights-0.2)*0.7;
    weights = max(weights,0.0);
    weights /= (weights.x+weights.y+weights.z);
    return weights;
}

void main() {

    vec2 p = -1.0 + 2.0*gl_FragCoord.xy/resolution.xy;

    /*if(p.y>0.1) p.x = -p.x;*/

    /*cube.x = 2.0*abs(sin(2.0*time));*/
    cube.z = 10.5*abs(sin(20.0*time));
    float t = time;
    /*vec3 ls = vec3(0.4,3.2,-20.2);*/
    vec3 ls = vec3(0.0);
    ls.z = -2.0;
    float asp = resolution.x/resolution.y;
    
    vec3 ro = vec3(0.0,0.0,-1.0);
    vec3 rd = normalize(vec3(p.x*asp,p.y,2.0));

    float tmin=10.0;
    int matID = 0;
        
    vec3 vl,hl;
    float spec;
    int steps;

    if(intersect(ro,rd,tmin,steps,matID)) {
        vec3 inter = ro+rd*tmin;
        vec3 l = normalize(ls-inter);
        vec3 n = getnormal(inter); 
        
        float s = float(steps)/50.0;

        vec2 coord1 = (inter.yz / cube.x);
        vec2 coord2 = (inter.zx / cube.y);
        vec2 coord3 = (inter.xy / cube.z);

        vec4 col1 = julia(coord1*0.5);
        vec4 col2 = 1.0-julia(coord2*0.5);
        vec4 col3 = julia(coord3*0.5);

        vec3 weights = blend_weights(n);
        vec4 col = col1 * weights.x + col2*weights.y + col3*weights.z;

        float dif = max(dot(n,l),0.0);

        vl = normalize(ro-inter);
        hl = normalize(l+vl);
        spec = pow(max(dot(hl,n),0.0),44.0);
        if(dif<=0.0) spec=0.0;


        gl_FragColor = dif*vec4(1.0) +2.0*spec-s*vec4(0.3,0.2,1.0,1.0);
        gl_FragColor *= mix(gl_FragColor,2.-col,0.2);
        gl_FragColor *= 1.9-col;
        /*gl_FragColor = 1.0-gl_FragColor ;*/
        /*gl_FragColor += mix(gl_FragColor,vec4(0.24,0.11,0.63,1.0),0.9);*/
        /*gl_FragColor.b = pow(gl_FragColor.g,2.0);*/
    }
    else{
        gl_FragColor = vec4(0.10,0.12,0.15,1.)*0.1*sound.y;
    }
}
