{
  "targets": [
    {
      "target_name": "rainfall",
      "sources": [ "src/rainfall.cc" , "src/rainfall_node.cc" ],
      "cflags": ["-Wall", "-std=c++11"],
      "include_dirs":['include'],
      "xcode_settings": {
        "OTHER_CFLAGS": [
          "-std=c++11"
        ]
      }
    }
  ]
}