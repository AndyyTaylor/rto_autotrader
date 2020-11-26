
cxx_library(
    TARGET future_midpoint
    FILENAME "pricing/future_midpoint.cpp"
)

cxx_library(
    TARGET simple_executor
    FILENAME "execution/simple_executor.cpp"
)

cxx_library(
    TARGET autotrader
    FILENAME "autotrader/autotrader.cpp"
    LINK future_midpoint simple_executor
)

cxx_library(
    TARGET market_link
    FILENAME "market_link/market_link.cpp"
)

cxx_executable(
    TARGET main
    FILENAME "main.cpp"
    LINK autotrader market_link
)