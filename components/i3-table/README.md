# Library for non linear sensor
- Use a table as reference
- If the value is known by the table, the matched value is returned
- If the value is between 2 values in the table, an estimated value thanks to a linear function is returned

# Example
```cpp
#include <i3-table.h>

int table[][2] = { //size 8
    {0, 140},
    {500, 110},
    {1000, 90},
    {1500, 70},
    {2000, 50},
    {2500, 30},
    {3000, 10},
    {4000, -10},
};

float result = i3TableGetValue(table, 8, 2750);
```