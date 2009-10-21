{
    "version" : "0.1",
    "materials" : [
    {
      "name" : "m1",
      "diffuse" : [0.8, 0.7, 0.6],
      "reflection" : 0.5
    },
    {
      "name" : "m2",
      "diffuse" : [0.1, 0.1, 0.0],
      "reflection" : 0.5
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
          "center" : [200.0, 150.0, 0.0],
          "radius" : 100.0,
          "material" : "m1"
        },
        { 
          "type" : "sphere",
          "center" : [400, 300, 50],
          "radius" : 75.0,
          "material" : "m2"
        }
      ],

      "lights" : [ 
        {
          "position" : [0.0, 240.0, -100.0],
          "intensity" : [1.0, 1.0, 1.0]
        }
        ]
    }
}
