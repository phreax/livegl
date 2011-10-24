uniform float time;
uniform vec2 resolution;
uniform vec4 mouse;

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

float f(vec3 p)
{
    float sdf;
    
    // Cornell Box (left - red, right - green, other - grey)
    float sdf1 = planeX(p,-1.0); // left wall red
    float sdf2 = planeX(p,1.0); // right wall green
    float sdf3 = planeY(p,-1.0); // floor grey
    float sdf4 = planeY(p,1.0); // ceil grey
    float sdf5 = planeZ(p,1.0); // back wall grey

    sdf = min(sdf1,min(sdf2,min(sdf3,min(sdf4,sdf5))));

    return sdf;
}

vec3 calcNormal( in vec3 pos )
{
    vec3 e = vec3(0.0002, 0.0, 0.0);
    vec3 n;
    n.x = f(pos + e.xyy) - f(pos - e.xyy);
    n.y = f(pos + e.yxy) - f(pos - e.yxy);
    n.z = f(pos + e.yyx) - f(pos - e.yyx);
    return normalize(n);
}

void main(void)
{
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
    vec3 color = vec3(0.0,0.0,0.0);
    
    // perspective fov 45?
    vec3 Ro = vec3(0.0,0.0,-1.0); // ray origin
    vec3 Rd = vec3(p.x,p.y,1.0); // ray direction
    Rd = normalize(Rd);

    float t = 0.0;
    const int maxsteps = 75;
    for(int steps = 0; steps < maxsteps; steps++)
    {
        vec3 pos = Ro+t*Rd;
	float d = f(pos);
        const float eps = 0.001;
        if(d < eps)
        {
             if(d == planeX(pos,-1.0))
//                 color = vec3(0.63,0.06,0.04);
                 color = vec3(0.64,0.15,0.1);
             else if(d == planeX(pos,1.0))
//                 color = vec3(0.15,0.48,0.09);
                 color = vec3(0.15,0.5,0.15);
             else
                 color = vec3(0.76,0.75,0.5);

             //color = pos;

             vec3 n = calcNormal( pos );
 
             // ambient occlusion
             float ao;
             float totao = 0.0;
             float sca = 1.0;
             for( int aoi=0; aoi<5; aoi++ )
             {
                 float hr = 0.01 + 0.02*float(aoi*aoi);
                 vec3 aopos = n * hr + pos;
                 float dd = f( aopos );
                 ao = -(dd-hr);
                 totao += ao*sca;
                 sca *= 0.75;
             }
             ao = 1.0-clamp( totao, 0.0, 1.0 );

             color = color * ao;

             break;
        }

        t = t + d;
    }

    gl_FragColor = vec4(color,1.0);
}
