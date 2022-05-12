{
  "targets": [
    {
      "target_name": "icarus_injector",
      "sources": ["Core.cpp", "../IcarusInjectorLibrary/IcarusInjectorLibrary/Injector.cpp"],
      "include_dirs": [
        "../IcarusInjectorLibrary/IcarusInjectorLibrary/",
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
