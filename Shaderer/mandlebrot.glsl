#version 460 core 

in vec4 gl_FragCoord;

out vec4 FragColor;

uniform vec2 u_Resolution;
uniform vec4 u_Mouse;
uniform float u_Time;
uniform uint u_Frame;
uniform float u_DeltaTime;

/* Only Modify Whatever is below this line */


#define MAX_ITER 512

dvec2 sqr(dvec2 a)
{
    dvec2 result;
    result.x = a.x * a.x - a.y * a.y;
    result.y = 2.0 * a.x * a.y;
    return result;
}

double map(double value, double min1, double max1, double min2, double max2)
{
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

dvec2 f_c(dvec2 z, dvec2 c)
{
    return sqr(z) + c;
}

void main()
{
    double range = 4.0;
    dvec2 center = dvec2(0);

    double off = range / 2.0;
    dvec2 z = dvec2(0.0);
    dvec2 c;

    //c.x = map(double(gl_FragCoord.x), 0.0, resolution.x, center.x - off, center.x + off);
    //c.y = map(double(gl_FragCoord.y), 0.0, resolution.y, center.y - off, center.y + off);
    c.x = (center.x - off) + ((range * double(gl_FragCoord.x)) / u_Resolution.x);
    c.y = (center.y - off) + ((range * double(gl_FragCoord.y)) / u_Resolution.y);
    int n;

    for (n = 0; n <= MAX_ITER; n++) {
        z = f_c(z, c);
        if (dot(z, z) > 4.0)
            break;
    }

    vec4 out_color = vec4(float(n & 0x7), float((n >> 3) & 0x7), float((n >> 6) & 0x7), 7.0);
    out_color /= 7.0;

    FragColor = out_color;
}


