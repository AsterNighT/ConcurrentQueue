#pragma once
using DataType = long long;
const DataType NullValue = -1;

class ConQueue {
   public:
    virtual auto get() -> DataType = 0;
    virtual auto put(DataType element) -> int = 0;
};