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
    name = "stack",
    hdrs = ["stack.h"],
    visibility = ["//visibility:public"],
    deps = [":vector"],
)

cc_test(
    name = "stack_test",
    srcs = ["stack_test.cc"],
    deps = [
        ":stack",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "priority_queue",
    hdrs = ["priority_queue.h"],
    visibility = ["//visibility:public"],
    deps = [":vector"],
)

cc_test(
    name = "priority_queue_test",
    srcs = ["priority_queue_test.cc"],
    deps = [
        ":priority_queue",
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

cc_library(
    name = "rolling_hash",
    hdrs = ["rolling_hash.h"],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "rolling_hash_multi",
    hdrs = ["rolling_hash_multi.h"],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_test(
    name = "rolling_hash_test",
    srcs = ["rolling_hash_test.cc"],
    deps = [
        ":rolling_hash",
        ":rolling_hash_multi",
        "//aux:pretty_print",
        "@googletest//:gtest_main",
    ],
)
