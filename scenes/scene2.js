{
    "version" : "0.1",
    "materials" : [
    {
      "name" : "m1",
      "diffuse" : [1.0, 1.0, 0.0],
      "reflection" : 0.2,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 40

    },
    {
      "name" : "m2",
      "diffuse" : [0.6, 0.7, 0.7],
      "reflection" : 0.7,
      "specular" : [1.0, 1.0, 1.0],
      "power" :20 
    },
    {
      "name" : "m3",
      "diffuse" : [1.0, 0.0, 1.0],
      "reflection" : 0.3,
      "specular" : [1.0, 1.0, 1.0],
      "power" : 20
    },
    {
      "name" : "m4",
      "diffuse" : [0.9, 0.5, 0.2],
      "reflection" : 0.7,
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
          "center" : [150, 240, 0],
          "radius" : 100.0,
          "material" : "m1"
        },
        { 
          "type" : "sphere",
          "center" : [490, 240, 0],
          "radius" : 100.0,
          "material" : "m3"
        },
        {
          "type" : "plane",
          "normal" : [0, 1, 0],
          "distance" : 800,
          "material" : "m2"
        }
      ],

      "lights" : [ 
       {
          "position" : [320, 600, -700.0],
          "intensity" : [1.0, 1.0, 1.0]
        }
        ]
    }
}
