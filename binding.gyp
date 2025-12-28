{
    "targets": [
        {
            "target_name": "sample_node_addon",
            "cflags": ["-std=c++20"],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "sources": [
                "src/counter-object.cc",
                "src/counter.cc",
                "src/heavy-calculation-callback.cc",
                "src/heavy-calculation.cc",
                "src/hello-method.cc",
                "src/hello.cc",
                "src/main.cc",
                "src/sensor-sim-monitor.cc",
                "src/sensor-sim.cc"
            ]
        }
    ]
}
