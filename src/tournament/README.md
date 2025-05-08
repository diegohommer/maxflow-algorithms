# Winning Tournaments

## Parameters

The instance generator can be customized using command-line arguments:

```
./program [n] [c] [α] [β]
```

- **`n`** — Number of teams (default: `10`).
- **`c`** — Number of matches initially scheduled between each pair of teams (default: `2`).
- **`α`** — Fraction of total games to simulate before output (default: `0.5`). Must satisfy $0 \leq \alpha \leq 1$.
- **`β`** — Bias in favor of team 1, added to its winning probability in each match it plays (default: `0.1`). Must satisfy $0 \leq \beta \leq 1$.

If no arguments are given, defaults will be used. Invalid arguments will cause an error.

## Output Instance Format

The first line contains the number of teams $n$.

The second line contains a vector $w_1,w_2,\ldots,w_n$, the number of times each team has already won.

On lines $2+i$ for $i \in [n-1]$, the values $g_{i2},g_{i3},\ldots,g_{in}$ are given, representing the number of times each pair of teams will still play.

## Flow Graph Reduction

To determine whether **team 1 can still win the tournament**, we reduce the problem to a **maximum flow instance**. The flow network is constructed as follows:

### Graph Construction

Let:

- `T = {2, 3, ..., n}` be the set of teams excluding team 1.
- `G = {(i, j) | i < j, i, j ∈ [1, n]}` be the set of team pairs with remaining matches.
- `g_ij` be the number of games remaining between teams `i` and `j`.
- `w_i` be the current number of wins of team `i`.
- `r_1 = ∑_{j ≠ 1} g_{1j}`, the number of remaining games team 1 will play.
- `m_i = w_1 + r_1 - w_i`, the maximum number of wins team `i ∈ T` can have if team 1 is to win.

We construct a flow network with:

- A **source node** `s`
- A **sink node** `t`
- A node for each team `i ∈ T`
- A node for each pair `g = (i, j) ∈ G` such that `i, j ≠ 1` and `g_ij > 0`

### Edges

- From **source `s`** to each **game node `g = (i, j)`**:
  - Capacity: `g_ij`
- From **each game node `g = (i, j)`** to team nodes `i` and `j`:
  - Capacity: ∞
- From **each team node `i ∈ T`** to **sink `t`**:
  - Capacity: `m_i = w_1 + r_1 - w_i`

Team 1 is assumed to **win all its remaining games** (`r_1`), and the reduction checks whether it is possible to assign the remaining wins to other teams such that **no team exceeds or reaches `w_1 + r_1`**.

### Decision

Let `C = ∑_{i<j, i,j ≠ 1} g_ij` be the total number of remaining games **not involving team 1**.

Then, **team 1 can still win the tournament if and only if** there exists an `s`-`t` flow in the constructed graph with **value `C`** (i.e., saturating all game edges).
