# This is Alpha Blending, that I advanced with SSE


Now I want to tell you about my results with optimisation of Alpha Blending process:

| Optimisation Flags | SSE   | Time In Microseconds for 100 times |
| :----------------: | :---: | :--------------------------------- |
|      -00           |  OFF  |           72779                    | 
|      -00           |  ON   |           28258 (2.58x)            |
|                    |       |                                    |
|      -01           |  OFF  |           25895                    | 
|      -01           |  ON   |            7057 (3.67x)            |
|                    |       |                                    |
|      -02           |  OFF  |           27880                    | 
|      -02           |  ON   |            3839 (7.26x)            |
|                    |       |                                    |
|      -03           |  OFF  |           17483                    | 
|      -03           |  ON   |            3347 (5.22x)            |

These results show, that we should use SSE to optimise Alpha Blending, because it works really well, especially with more efficient optimisation flags.
