
#ifndef TYPES_H_
#define TYPES_H_

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

#endif  // TYPES_H_
