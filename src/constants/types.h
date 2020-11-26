// Copyright 2020 Andy Taylor

#ifndef SRC_CONSTANTS_TYPES_H_
#define SRC_CONSTANTS_TYPES_H_

enum Instrument {
    FUTURE = 0,
    ETF = 1
};

enum Side {
    SELL = 0,
    BUY = 1
};

enum Lifespan {
    FILL_AND_KILL = 0,
    GOOD_FOR_DAY = 1
};

#endif  // SRC_CONSTANTS_TYPES_H_
