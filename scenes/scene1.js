{
    "version" : "0.1",
    "materials" : [
    {
      "name" : "m1",
      "diffuse" : [1.0, 1.0, 0.0],
      "reflection" : 0.8,
      "refraction" : 0.0,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 40

    },
    {
      "name" : "m2",
      "diffuse" : [0.0, 1.0, 1.0],
      "reflection" : 0.8,
      "refraction" : 0.0,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 30
    },
    {
      "name" : "m3",
      "diffuse" : [1.0, 0.0, 1.0],
      "reflection" : 0.8,
      "refraction" : 0.0,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 20
    },
    {
      "name" : "m4",
      "diffuse" : [0.9, 0.5, 0.2],
      "reflection" : 0.0,
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
          "center" : [-2, 0, 0],
          "radius" : 1.0,
          "material" : "m1"
        },
        { 
          "type" : "sphere",
          "center" : [2, 0, 0],
          "radius" : 1.0,
          "material" : "m2"
        },
        {
          "type" : "sphere",
          "center" : [0, 1.5, 0],
          "radius" : 1.0,
          "material" : "m3"
        },
        {
          "type" : "sphere",
          "center" : [0, -1.5, 0],
          "radius" : 1.0,
          "material" : "m4"
        }
      ],

      "lights" : [ 
        {
          "position" : [0, 4, -10.0],
          "intensity" : [1.0, 1.0, 1.0]
        },
        {
          "position" : [6.4, 24, -10.0],
          "intensity" : [0.6, 0.7, 1.0]
        }
        ]
    }
}
