{
    "version" : "0.1",
    "materials" : [
    {
      "name" : "m1",
      "type" : "marble",
      "diffuse1" : [0.9, 0.9, 0.9],
      "diffuse2" : [0.1, 0.1, 0.1],
      "reflection" : 0.5,
      "refraction" : 0.0,
      "specular" : [0.7, 0.7, 0.7],
      "power" : 20 

    },
    {
      "name" : "m2",
      "type" : "turbulence",
      "diffuse1" : [0.1, 0.5, 0.1],
      "diffuse2" : [0.2, 0.1, 0.1],
      "reflection" : 0.2,
      "refraction" : 0.0,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 20
    },
    {
      "name" : "m3",
      "type" : "default",
      "diffuse1" : [1.0, 0.0, 1.0],
      "diffuse2" : [1.0, 0.0, 1.0],
      "reflection" : 0.8,
      "refraction" : 0.0,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 20
    },
    {
      "name" : "m4",
      "type" : "default",
      "diffuse1" : [0.2, 0.5, 0.8],
      "diffuse2" : [0.2, 0.5, 0.8],
      "reflection" : 0.9,
      "refraction" : 0.0,
      "specular" : [0.1, 0.5, 0.7],
      "power" : 20
    },
    {
      "name" : "m5",
      "type" : "default",
      "diffuse1" : [0.1, 0.2, 0.3],
      "diffuse2" : [0.1, 0.2, 0.3],
      "reflection" : 0.1,
      "refraction" : 0.0,
      "specular" : [0.1, 0.5, 0.7],
      "power" : 20
    }

    ],

    "scene" : {
      "viewport" : {
        "width" : 640,
        "height" : 480
      },

      "camera" : {
        "position" : [640, 0, 0],
        "direction" : [0, 0, 0]
      },

      "objects" : [
        { 
          "type" : "sphere",
          "center" : [-2.5, 1.8, 0],
          "radius" : 0.9,
          "material" : "m1"
        },
        {
          "type" : "sphere",
          "center" : [0, 1.8, 0],
          "radius" : 0.9,
          "material" : "m2"
        },
        {
          "type" : "sphere",
          "center" : [2.5, 1.8, 0],
          "radius" : 0.9,
          "material" : "m3"
        },
        { 
          "type" : "sphere",
          "center" : [-1.25, -1.0, 2.0],
          "radius" : 0.5,
          "material" : "m4"
        },
        { 
          "type" : "sphere",
          "center" : [1.25, -1.0, 2.0],
          "radius" : 0.5,
          "material" : "m4"
        },
        { 
          "type" : "sphere",
          "center" : [-1.25, 1.0, 2.0],
          "radius" : 0.5,
          "material" : "m4"
        },
        { 
          "type" : "sphere",
          "center" : [1.25, 1.0, 2.0],
          "radius" : 0.5,
          "material" : "m4"
        },
        { 
          "type" : "sphere",
          "center" : [-1.25, 3.0, 2.0],
          "radius" : 0.5,
          "material" : "m4"
        },
        { 
          "type" : "sphere",
          "center" : [1.25, 3.0, 2.0],
          "radius" : 0.5,
          "material" : "m4"
        },
        { 
          "type" : "sphere",
          "center" : [-2.5, 0, 0],
          "radius" : 0.9,
          "material" : "m2"
        },
        {
          "type" : "sphere",
          "center" : [0, 0, 0],
          "radius" : 0.9,
          "material" : "m1"
        },
        {
          "type" : "sphere",
          "center" : [2.5, 0, 0],
          "radius" : 0.9,
          "material" : "m3"
        },
        { 
          "type" : "sphere",
          "center" : [-2.5, -1.8, 0],
          "radius" : 0.9,
          "material" : "m3"
        },
        {
          "type" : "sphere",
          "center" : [0, -1.8, 0],
          "radius" : 0.9,
          "material" : "m2"
        },
        {
          "type" : "sphere",
          "center" : [2.5, -1.8, 0],
          "radius" : 0.9,
          "material" : "m1"
        } 
      ],

      "lights" : [ 
        {
          "position" : [0, 4, -24.0],
          "intensity" : [1.0, 1.0, 1.0]
        }
        ]
    }
}
