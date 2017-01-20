vec4 hsv2rgb(vec4 hsvColor) {
    float h = hsvColor.x;
    float s = hsvColor.y;
    float v = hsvColor.z;
    if (s == 0.0) {
        return vec4(v, v, v, 1.0);
    }
    if (h == 360.0) {
        h = 0.0;
    }
    int hi = int(h);
    float f = h - float(hi);
    float p = v * (1.0 - s);
    float q = v * (1.0 - (s * f));
    float t = v * (1.0 - (s * (1.0 - f)));
    vec3 rgb;
    if (hi == 0) {
        rgb = vec3(v, t, p);
    } else if (hi == 1) {
        rgb = vec3(q, v, p);
    } else if (hi == 2) {
        rgb = vec3(p, v, t);
    } if(hi == 3) {
        rgb = vec3(p, q, v);
    } else if (hi == 4) {
        rgb = vec3(t, p, v);
    } else {
        rgb = vec3(v, p, q);
    }
    return vec4(rgb.x, rgb.y, rgb.z, 1.0);
}
