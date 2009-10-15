{
    "version" : "0.1",
    "materials" : [
    {
      "name" : "m1",
      "diffuse" : [1.0, 1.0, 0.0],
      "reflection" : 0.5
    },
    {
      "name" : "m2",
      "diffuse" : [0.1, 0.1, 0.0],
      "reflection" : 0.5
    }
    ],

    "scene" : {
      "image_width" : 640,
      "image_height" : 480,

      "camera" : {
        "position" : [640, 0, 0],
        "view_direction" : [0, 0, 0]
      },

      "objects" : [
        { 
          "type" : "sphere",
          "center" : [233.0, 290.0, 0.0],
          "size" : 100,
          "material" : "m1"
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
