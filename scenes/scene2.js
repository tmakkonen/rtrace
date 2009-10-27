{
    "version" : "0.1",
    "materials" : [
    {
      "name" : "m1",
      "diffuse" : [0.7, 0.7, 0.6],
      "reflection" : 0.7,
      "refraction" : 0.0,
      "specular" : [0.7, 0.7, 0.6],
      "power" : 20

    },
    {
      "name" : "m2",
      "diffuse" : [0.3, 0.2, 0.6],
      "reflection" : 0.1,
      "refraction" : 0.0,
      "specular" : [0.0, 0.0, 0.0],
      "power" :20 
    },
    {
      "name" : "m3",
      "diffuse" : [0.5, 0.3, 0.7],
      "reflection" : 0.0,
      "refraction" : 0.0,
      "specular" : [0.0, 0.0, 0.0],
      "power" : 20
    },
    {
      "name" : "m4",
      "diffuse" : [0.2, 0.5, 0.2],
      "reflection" : 0.8,
      "refraction" : 0.0,
      "specular" : [0.2, 0.5, 0.2],
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
          "center" : [2, 0.8, 3],
          "radius" : 2.0,
          "material" : "m1"
        },
        {
          "type" : "sphere",
          "center" : [-5.5, -0.5, 7],
          "radius": 1.4,
          "material" : "m4"
        },
        {
          "comment" : "floor plane",
          "type" : "plane",
          "normal" : [0, 1, 0],
          "distance" : 4,
          "material" : "m3"
        }
      ],

      "lights" : [ 
       {
          "position" : [0, 5, 5],
          "intensity" : [1.0, 1.0, 1.0]
       },
       {
          "position" : [-3, 5, 1],
          "intensity" : [1.0, 1.0, 1.0]
       }
      ]
    }
}
