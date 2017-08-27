{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/demo.cc"],
      "cflags": ["-Wall", "-std=c++11"],
      "xcode_settings": {
        "OTHER_CFLAGS": [
          "-std=c++11"
        ]
      }
    }
  ]
}