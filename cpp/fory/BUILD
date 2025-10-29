load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "fory",
    deps = [
      "@local_config_pyarrow//:arrow",
      "//cpp/fory/row:fory_row_format",
      "//cpp/fory/columnar:fory_columnar_format",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "xlang_test",
    srcs = ["xlang_test.cc"],
    deps = [
        "//cpp/fory/util:fory_util",
        "//cpp/fory/row:fory_row_format",
        "@com_google_googletest//:gtest",
    ],
)
