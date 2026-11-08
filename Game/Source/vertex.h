#pragma once
#include "glmath.h"

struct Vertex {

    Vertex() {}

    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;

    Vertex(vec3 v)
    {
        Position = v;
    }

    //Vertex(vec3 v)
    //{
    //    Position.x = v.x;
    //    Position.y = v.y;
    //    Position.z = v.z;
    //}


};