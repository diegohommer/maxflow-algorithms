# Fluxo Máximo
Código de suporte para o trabalho prático em problemas de fluxo máximo.


## Gerador Washington

>To use:
>
>         cc washington.c -o gengraph
>
>         gengraph function arg1 arg2 arg3
>
>Command line arguments have the following meanings:
>
>         function:           index of desired graph type
>         arg1, arg2, arg3:   meanings depend on graph type (briefly listed below: see code comments for more info)

There are 11 graphs types, numbered 1, 2, ..., 11, which is the argument `function`. Other arguments depend on the graph type and may be

| Parameter | Abbreviation | Comment |
|-|-|-|
| Number of rows | $r$ |
| Number of columns | $c$ |
| Maximum capacity | $C$ | Capacities are drawn from $U[1,C]$ |
| Degree           | $d$ | Vertex out-degree |
| Side             | $s$ | Side length |

The graph types are:

| No. | Name | Arg1 | Arg2 | Arg3 | Arg4 | n | m |
|--|--|--|--|--|--|-|-|
|   1 | Mesh           | r | c | C |   | $rc+2$  | $2r + 3(c-1)r$ |
|   2 | Random level   | r | c | C |   | $rc+2$  | $2r + 3(c-1)r$ |
|   3 | Random 2-level | r | c | C |   | $rc+2$  | $2r + 3(c-1)r$ |
|   4 | Matching       | n | d | C |   | $2n+2$  | $n(d+2)$       |
|   5 | Square Mesh    | s | d | C |   | $s^2+2$ | $(s-1)sd+2s$ |
|   6 | BasicLine      | n | m | d | C | $nm+2$  | $nmd+2m$ |
|   7 | ExpLine        | n | m | d | C | $nm+2$  | $nmd+2m$ |
|   8 | DExpLine       | n | m | d | C | $nm+2$  | $nmd+2m$ |
|   9 | DinicBad       | n |   |   |   | $n$     | $2n-3$|
|  10 | GoldBad        | n |   |   |   | $3n+3$  | $4n+1$ |
|  11 | Cheryian       | n | m | d | C | $4md+2n+6$ | $4md+2n+6$ |

## Description of some graphs

Write $\binom{X}{k}$ for the set of subsets of set $X$ of size $k$, and $x\in_U X$ for a uniform random member $x$ of set $X$.

### Mesh

A regular mesh $M=\\{s\\}\cup [r]\times [c]\cup\\{t\\}$ with $r$ rows and $c$ columns. The source is connected to the first column (arcs $(s,(i,1))$, $i\in[r]$), vertices in each column have three arcs to the next column (arcs $((i,j),(i+\delta,j+1))$, $i\in[r]$, $j\in[c-1]$, $\delta\in\\{-1,0,1\\}$), the last column is connected to the sink $t$ (arcs $((i,c),t)$, $i\in [r]$).

### Random Level Graph

Same as a Mesh, but $\delta\in_U\binom{[r]}{3}$.

### Random 2-Level Graph

Same as a Random Level Graph, but mesh arcs are $((i,i),(i+\delta,j+U[1,2]))$, $j\in[c-2]$ (the next to last column column $c-1$ obviously connects only to $c$).

### Matching Graph

Bipartite graph over $V=\\{s\\}\cup V_1\cup V_2\cup\\{t\\}$, with $V_1=[n]$, $V_2=[n+1,2n]$. The source connects to part $V_1$ (arcs $(s,v)$, $v\in V_1$), each vertex of $V_1$ has $d$ random arcs to $V_2$ (arcs $(v,v')$, $v\in V_1$, $v'\in_U\binom{V_2}{d}$), and the second part connects to $t$ (arcs $(v,t)$, $v\in V_2$).

### Square Mesh

A square mesh of size $s\times s$ with the source connected to the first, and the sink connected to the last column. Each vertex $(i,j)$ connects to valid vertices $(i+k-1,j+1)$, $k\in[d]$, i.e. has degree at most $d$.

### Basic Line

A line $L=\\{s\\}\cup [nm]\cup\\{t\\}$. Source and sink fully connect to the $m$ first resp. last vertices (arcs $(s,i)$, $i\in[m]$, and $(i,t)$, $i\in[nm-m+1,nm]$), each each vertex connects to $d$ of the next $md$ vertices (arcs $(i,j)$, $j\in J$, $J\in_U\binom{[md]}{d}$).

### Exponential Line

Same as a basic line, but with different capacities. A arcs $(i,j)$ of length $l=|i-j|$ has a random weight in $U[1, \lfloor 10^6 / 2^{d-1}\rfloor]$, where $d =\lfloor (l-1)/m\rfloor$, i.e. weights are exponentially smaller with increasing distance. This requires $d\leq 20$.

### Double Exponential Line

A Basic Line, but vertices connect to predecessors and successors, i.e. $J\in_U\binom{[-md,md]\setminus\\{0\\}}{d}$, with weights as for the Exponential Line.

### DinicBadCase

A line $L=[n]$, with arcs to previous and next vertices (arcs $(i,i-1)$, $i\in[2,n]$, and $(i,i+1)$, $i\in[n-1]$). Causes $n$ augmentation phases.

### GoldBadCase

Bad for Goldberg's algorithm.

### Cheryian

Bad for Goldberg's algorithm.
