# Food Delivery Optimizer

A console-based C++ food delivery system built on your original Customer /
Restaurant / MenuItem classes — extended with a genuinely useful feature
that most student food-delivery projects don't have:

## The city map: real Jaipur

The graph isn't a toy grid — it's seeded with real Jaipur localities and
realistic road travel times (minutes), with **Sindhi Camp** (near the
railway station, a genuinely central point in the city) as the delivery
dispatch hub:

```
Sindhi Camp -- C-Scheme -- Civil Lines -- Vidhyadhar Nagar
     |             |            |
  Bani Park    Raja Park -- Johari Bazaar
     |
   Sodala -- Vaishali Nagar -- Ajmer Road
                    |
                Mansarovar -- Malviya Nagar -- Tonk Road -- Jagatpura
                                                    |            |
                                                Sanganer -------
```

Restaurants are real, well-known Jaipur establishments placed at their
actual localities: LMB (Johari Bazaar), Rawat Mishthan Bhandar (Sindhi
Camp), Copper Chimney (C-Scheme), Kanha Restaurant (Malviya Nagar), Handi
Restaurant (Tonk Road), Tapri Central (C-Scheme), Peacock Rooftop
Restaurant (Ajmer Road), Chokhi Dhani (Tonk Road), Niro's (C-Scheme), and
Natraj Dining Hall (C-Scheme) — serving actual Rajasthani specialties (Dal
Baati Churma, Laal Maas, Pyaaz Kachori, etc). The 15 demo customers are
spread across every locality on the map (Raja Park, Mansarovar, Jagatpura,
Vaishali Nagar, Civil Lines, Sanganer, Ajmer Road, Bani Park, Vidhyadhar
Nagar, Sodala, Johari Bazaar, C-Scheme, Tonk Road, Sindhi Camp, and Malviya
Nagar), and 5 delivery partners start the day stationed at different hubs
(Sindhi Camp, Malviya Nagar, Vaishali Nagar, and C-Scheme) rather than all
starting from one place.

Because this is real geography, the batching algorithm's decisions are
realistic too — e.g. two orders from the same restaurant going to Raja Park
and Mansarovar do *not* get merged, because those localities are genuinely
far apart across the city (the detour would exceed the threshold), while
orders whose drop points sit close together on the map do merge.

## The unique feature: Smart Order Batching & Route Optimization

Naive food delivery projects assign **one delivery partner per order**. This
project instead models the city as a **weighted graph** (roads = edges,
travel time = weight) and, before dispatching, runs a **greedy savings
algorithm** (the same idea behind the Clarke–Wright algorithm used in
real-world vehicle routing) to group orders that are geographically close
into a **single multi-stop route** for one delivery partner — as long as the
extra detour stays under a threshold.

For each pending order it compares:
- `baseline cost` = distance(Hub → Restaurant) + distance(Restaurant → Customer) — the cost of sending a partner just for this one order
- `added cost` = distance(last stop in an existing route → Restaurant) + distance(Restaurant → Customer) — the cost of tacking it onto a route already in progress

If the detour to merge is small enough, the order joins that batch instead of
needing its own trip. This directly reduces the number of delivery partners
needed and the total distance traveled — a real, measurable optimization,
not just a UI feature.

Delivery partners are then assigned to batches using a **min-heap
(priority_queue)** keyed by "next available time," so the soonest-free
partner always gets the next batch — a classic load-balancing pattern.
Finally, **Dijkstra's algorithm** is walked stop-by-stop along each batch's
route to compute an accurate, individual **ETA per order** (not just one ETA
for the whole batch).

## Beyond batching: a live, reversible simulation

On top of the batching/routing engine, `OrderManager` now runs a small but
real simulation of a delivery day:

- **Order cancellation.** An order can be cancelled any time before a
  delivery partner is actually dispatched to it (`PLACED` or `BATCHED`
  status) — matching how real delivery apps behave once a rider is en
  route. If the order had been merged into a shared batch, its pickup +
  drop stops are surgically removed from that batch's route and the
  batch's total distance is recomputed leg-by-leg, rather than trying to
  patch a running total (which is an easy place to introduce silent bugs).
- **Ratings that actually move the needle.** Once an order is `DELIVERED`,
  the customer can leave a 1–5 star rating exactly once. It folds into the
  restaurant's rating as a genuine running average
  (`newAvg = (oldAvg * n + newRating) / (n + 1)`), not a fake overwrite —
  so a restaurant's rating drifts realistically over many orders.
- **An advancing simulation clock.** Instead of a fixed ETA that's just a
  number sitting on an order forever, "Advance Simulation Clock" moves
  time forward and any `ASSIGNED` order whose ETA has now passed
  automatically flips to `DELIVERED` — which is also what unlocks rating
  it. Delivery partners' `availableAt` times (stamped during
  `assignPartners`) are what make this meaningful: a partner genuinely
  isn't free for the *next* batch until the clock catches up to their
  last drop-off.
- **Full undo.** Every state-changing action (placing an order, batching,
  assigning partners, cancelling, rating, advancing the clock) pushes a
  complete snapshot of orders, batches, partners, and restaurant ratings
  onto a stack first — a textbook **Memento pattern**. "Undo Last Action"
  pops the most recent snapshot back in, so mistakes (or just curiosity
  about "what if") are always one keystroke away from being reversed.

## DSA concepts used

| Concept | Where |
|---|---|
| Graph (adjacency list) | `Graph.h/.cpp` — city map of locations & road times |
| Dijkstra's shortest path | `Graph::dijkstra`, `Graph::shortestPath` — used for detour cost & ETA |
| Greedy algorithm (savings heuristic) | `OrderManager::runSmartBatching` |
| Min-heap / priority_queue | `OrderManager::assignPartners`, `Graph::dijkstra` |
| Memento pattern / stack | `OrderManager::pushUndoSnapshot` / `undo` — full-state undo history |
| Running average | `Restaurant::addRating` — incremental mean, no re-summing history |
| Vectors, structs, OOP | Everywhere — `Order`, `Batch`, `DeliveryPartner`, managers |
| Sorting (FIFO fairness) | Orders sorted by placed time before batching |

## Project structure

```
Customer.h/.cpp, CustomerManager.h/.cpp   - customer records (from your original code)
Restaurant.h/.cpp, RestaurantManager.h/.cpp - restaurant records (from your original code)
MenuItem.h/.cpp, MenuManager.h/.cpp        - menu items (from your original code, include bug fixed)
Graph.h/.cpp            - NEW: city map + Dijkstra
DeliveryPartner.h/.cpp  - NEW: delivery partner state
Order.h/.cpp            - NEW: order lifecycle (placed -> batched -> assigned -> delivered)
OrderManager.h/.cpp     - NEW: the batching + routing + ETA engine (the core feature)
main.cpp                - NEW: menu-driven console app wiring everything together
```

Note: a couple of gaps were fixed to get this compiling and running
cleanly: `RestaurantManager.h` was missing entirely (only the `.cpp`
existed), `Order.cpp` didn't exist at all despite `Order.h` being used
throughout `OrderManager`, and `Graph.cpp` had two real bugs — a missing
`<climits>` include for `INT_MAX`, and `adj[u]` being called inside
`const` methods (`operator[]` isn't valid on a `const map`; fixed with
`adj.at(u)`). The whole thing now builds warning-free under
`-Wall -Wextra`.

## Building & running

```bash
g++ -std=c++17 -Wall -o food_delivery_optimizer main.cpp Customer.cpp CustomerManager.cpp \
    Restaurant.cpp RestaurantManager.cpp MenuItem.cpp MenuManager.cpp Graph.cpp \
    DeliveryPartner.cpp Order.cpp OrderManager.cpp
./food_delivery_optimizer
```

The app preloads a real Jaipur city map, 10 restaurants, 15 customers, and 5
delivery partners so you can try it immediately:

1. Place a few orders (option 5) for different customers/restaurants.
2. Run **Smart Batching** (option 6) — watch orders merge into shared routes.
3. **Assign Delivery Partners** (option 7) — partners get dispatched via the min-heap.
4. Check **View All Batches & Routes** (option 8) and **Track An Order** (option 9) to see the optimized route and each order's individual ETA.
5. **Advance Simulation Clock** (option 14) past an order's ETA — watch it flip to `DELIVERED` automatically.
6. **Rate A Delivered Order** (option 13) and re-check the restaurant (option 1) — its rating has genuinely moved.
7. Try **Cancel An Order** (option 12) on something still `PLACED`/`BATCHED` — if it was merged into a batch, check option 8 again to see the route was surgically re-cut.
8. Made a mistake, or just curious "what if"? **Undo Last Action** (option 15) reverts the very last state-changing step, all the way back through the whole session if you keep pressing it.

## Possible extensions

- Reorder each batch's stops with a 2-opt pass for a true near-optimal route (currently stops are appended greedily).
- Let `DETOUR_THRESHOLD` and `MAX_BATCH_SIZE` be tuned live from the menu.
- Persist restaurants/customers/orders/ratings to a file between runs.
- Unit tests around the batching heuristic and the undo/redo stack.
- A proper build system (CMake) and CI (GitHub Actions) for a polished repo.
