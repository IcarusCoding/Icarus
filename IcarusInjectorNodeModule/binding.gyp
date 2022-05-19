{
  "targets": [
    {
      "target_name": "icarus_injector",
      "sources": [
        "Core.cpp",
        "../IcarusInjectorLibrary/IcarusInjectorLibrary/Injector.cpp",
        "../IcarusInjectorLibrary/IcarusInjectorLibrary/ProcessInfo.cpp",
        "../IcarusInjectorLibrary/IcarusInjectorLibrary/WinUtils.cpp",
        "../IcarusInjectorLibrary/IcarusInjectorLibrary/lodepng.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../IcarusInjectorLibrary/IcarusInjectorLibrary/",
        "./"
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'AdditionalOptions': [ '-std:c++20' ]
        }
      },
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ]
    }
  ]
}
