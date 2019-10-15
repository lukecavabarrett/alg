cc_library(
    name = "vector",
    hdrs = ["vector.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "vector_test",
    srcs = ["vector_test.cc"],
    deps = [
        ":vector",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "binary_node",
    hdrs = ["binary_node.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tree_algs",
    hdrs = ["tree_algs.h"],
    visibility = ["//visibility:public"],
    deps = [":binary_node"],
)

cc_test(
    name = "tree_algs_test",
    srcs = ["tree_algs_test.cc"],
    deps = [
        ":binary_node",
        ":tree_algs",
        "@googletest//:gtest_main",
    ],
)
