# FIFO lib
```cpp
I3Queue* a = new I3Queue(4);

a->push(1); //[1]
a->push(2); //[1, 2]
a->push(3); //[1, 2, 3]
a->push(4); //[1, 2, 3, 4]
a->push(5); //[2, 3, 4, 5]
```
