

cxx_library(
    TARGET autotrader
    FILENAME "autotrader/autotrader.cpp"
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