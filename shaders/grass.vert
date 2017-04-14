#version 330

//mesh
in vec4 point;
// texture(u, v)
in vec2 uvpoint;

// position(x, z) and rotation(phi)
in vec2 position;
in float rotation;

// bend(x, z)
in vec2 variance;


uniform mat4 camera;

out vec2 UV;
out float color_coef;
out float point_y;

vec4 bend(vec4 p, vec2 var)
{
    float magn = sqrt(var[0]*var[0]+var[1]*var[1]);
    mat4 rotationWind = mat4(1.0);
    rotationWind[0][0] = var[0]/magn;
    rotationWind[2][0] = - var[1]/magn;
    rotationWind[0][2] = - rotationWind[2][0];
    rotationWind[2][2] = rotationWind[0][0];

    float phi = magn;
    vec4 t = rotationWind * vec4(
        0,
        .5*p.y*(p.y*(cos(phi)-1)+2),
        .5*sin(phi)*p.y*p.y,
        1
    );
    t.x += p.x - .2;
    return t;
}

void main()
{
    float scale = 0.05;

    UV = uvpoint;
    color_coef = 0.3*float(gl_InstanceID % 17)/16 + 0.7;
    point_y = point.y;

    float size_variance = 1. + 0.3*(float(gl_InstanceID % 19)/18 - .5);
    mat4 sizeMatrix = mat4(1.0);
    sizeMatrix[0][0] = size_variance;
    sizeMatrix[1][1] = size_variance;
    sizeMatrix[2][2] = size_variance;

    mat4 scaleMatrix = mat4(1.0);
    scaleMatrix[0][0] = scale;
    scaleMatrix[1][1] = scale;
    scaleMatrix[2][2] = scale;
    mat4 positionMatrix = mat4(1.0);
    positionMatrix[3][0] = position.x;
    positionMatrix[3][2] = position.y;

    mat4 rotationMatrix = mat4(1.0);
    float phi = rotation;
    rotationMatrix[0][0] = cos(phi);
    rotationMatrix[2][0] = - sin(phi);
    rotationMatrix[0][2] = - rotationMatrix[2][0];
    rotationMatrix[2][2] = rotationMatrix[0][0];

    vec2 var = vec2(
        cos(phi) * variance.x + sin(phi) * variance.y,
        - sin(phi) * variance.x + cos(phi) * variance.y
    );

	gl_Position = camera * (positionMatrix * scaleMatrix * rotationMatrix * bend(sizeMatrix*point, var));
}
