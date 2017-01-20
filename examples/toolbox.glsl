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
    return (sin(11.0*p.x)*sin(10.0*p.y)*sin(10.*p.z) )*0.15;
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

vec3 rX(vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	q.y = c * p.y - s * p.z;
	q.z = s * p.y + c * p.z;
  return q;
}

vec3 rY(vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	q.x = c * p.x + s * p.z;
	q.z = -s *p.x + c * p.z;
  return q;
}

vec3 rZ(vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	q.x = c * p.x - s * p.y;
	q.y = s * p.x + c * p.y;
  return q;
}

vec3 rX2(vec3 p, float s, float c) {
	vec3 q=p;
	q.y = c * p.y - s * p.z;
	q.z = s * p.y + c * p.z;
  return q;
}

float map(vec3 p, out int matID);

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
    int maxstep = 100;
    t = 0.0;
    float eps = 0.0001;
    float d;

    for(int i=0;i<maxstep&&maxt>=t;i++) {
        vec3 p = ro+rd*t;
        d = map(p,matID);

        if(d<eps) {
            steps = i;
            return true;
        }
        t+=d;
    } 
    return false;

}


