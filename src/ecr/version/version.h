//  ECR version macros for compile-time API detection
#define ECR_VERSION_MAJOR 0
#define ECR_VERSION_MINOR 0
#define ECR_VERSION_PATCH 1

#define ECR_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ECR_VERSION \
        ECR_MAKE_VERSION(ECR_VERSION_MAJOR, \
                         ECR_VERSION_MINOR, \
                         ECR_VERSION_PATCH)