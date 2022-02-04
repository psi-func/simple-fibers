include(FetchContent)

FetchContent_DECLARE(
    psi-context
    GIT_REPOSITORY https://github.com/psi-func/psi-context.git
    GIT_TAG master
)
FetchContent_MakeAvailable(psi-context)