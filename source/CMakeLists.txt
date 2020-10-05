
cxx_library(
    TARGET autotrader
    FILENAME "autotrader.cpp"
)

cxx_executable(
    TARGET main
    FILENAME "main.cpp"
    LINK absl::flat_hash_map autotrader
)