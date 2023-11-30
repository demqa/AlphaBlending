# This is Alpha Blending, that I advanced with SSE


There are my results of measuring Alpha Blending time with different optimisations:

| Optimisation Flags | SSE   | Time In Microseconds for 100 times |
| :----------------: | :---: | :--------------------------------- |
|      -00           |  OFF  |           72779                    | 
|      -00           |  ON   |           28258 (2.58x)            |
|                    |       |                                    |
|      -01           |  OFF  |           25895                    | 
|      -01           |  ON   |           7057  (3.67x)            |
|                    |       |                                    |
|      -02           |  OFF  |           27880                    | 
|      -02           |  ON   |           3839  (7.26x)            |
|                    |       |                                    |
|      -03           |  OFF  |           17483                    | 
|      -03           |  ON   |           3347  (5.22x)            |

These results show, that we should use SSE to optimise Alpha Blending, because it improves an effieciency a lot, especially with harder optimisation flags.

#### Installation:

Without SSE:
```
make
```

With SSE:
```
make sse
```


#### Usage:
```
./blend background.png foreground.png x_position y_position
# x and y position on the background image where to put top-left corner of foreground image
```
