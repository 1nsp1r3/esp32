# Linear function (Line cut the origin)
`y = ax`

```
    |
    |   **
    | **
----**-------------->
  **|
**  |
```

With:
| Var | Val |
|-----|-----|
|  Ya |   0 |
|  Yb |   2 |
|  Xa |   0 |
|  Xb |   5 |

How calculate the slope? `a = (Yb - Ya) / (Xb - Xa)`

- (2-0) / (5-0) -> 2 / 5 -> 0.4

Example of results:
| X |   Y |
|---|-----|
| 0 | 0.0 |
| 1 | 0.4 |
| 2 | 0.8 |
| 3 | 1.2 |
| 4 | 1.6 |
| 5 | 2.0 |
| 6 | 2.4 |

# Linear function (Line doesn't cut the origin)
`y = ax + b`

```
    |
    |   **
    | **
    **
  **|
**--|--------------->
    |
```

With:
| Var | Val |
|-----|-----|
|  Ya |   2 |
|  Yb |   4 |
|  Xa |   0 |
|  Xb |   5 |

How calculate the slope? `a = (Yb - Ya) / (Xb - Xa)`
- (4-2) / (5-0) -> 2 / 5 -> 0.4

How found `b` value ?
- b = Y - ax (Replace value by Ya & Xa)
- b = Ya - a * Xa
- b = 2 - 0.4 * 0
- b = 2

Example of results:
| X |   Y |
|---|-----|
| 0 | 2.0 |
| 1 | 2.4 |
| 2 | 2.8 |
| 3 | 3.2 |
| 4 | 3.6 |
| 5 | 4.0 |
| 6 | 4.4 |