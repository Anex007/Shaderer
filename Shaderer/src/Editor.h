#pragma once

/*
    https://www.shaderific.com/glsl
*/

#define NUMVARIABLES 6

static const char* Variables[NUMVARIABLES] = {
    "gl_FragCoord",
    "FragColor",
    "u_Resolution",
    "u_Mouse",
    "u_Time",
    "u_DeltaTime",
    /* MORE TO GO HERE */
};

#define NUMBUILTINFUNCS 47

static const char* BuiltinFuncs[NUMBUILTINFUNCS] = {
    "radians",
    "degrees",
    "sin",
    "cos",
    "tan",
    "asin",
    "acos",
    "atan",
    "pow",
    "exp",
    "log",
    "exp2",
    "log2",
    "sqrt",
    "inversesqrt",
    "abs",
    "sign",
    "floor",
    "ceil",
    "fract",
    "mod",
    "min",
    "max",
    "clamp",
    "mix",
    "step",
    "smoothstep",
    "length",
    "distance",
    "dot",
    "cross",
    "normalize",
    "faceforward",
    "reflect",
    "refract",
    "matrixCompMult",
    "lessThan",
    "lessThanEqual",
    "greaterThan",
    "greaterThanEqual",
    "equal",
    "notEqual",
    "any",
    "all",
    "not",
    "texture2D",
    "textureCube",
};

template<typename T>
class Editor
{
public:
	Editor();
	~Editor();

private:

};
