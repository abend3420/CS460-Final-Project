# CS 460 Final Project 

## Advertisement Recommendation System

- Role of marketer inputs a list of advertisements and their attributes

Attributes may include:
Name
Category
Brand
Price
Initial priority
On-sale (yes/no)
Sponsored (yes/no)

- Role of customer picks one item at a time to "view"
- Advertisements are dynamically ranked and displayed to the user based on their "viewing" in real time
- A priority queue is used to rank advertisements

Advertisement ranking rules:
Same category = +20
Same brand = +15
Sponsored = +10
On sale = +5

Time complexity: O(n log n)
Space complexity: O(n)

---

## How to Build

Run the following commands in order:
```bash
make

---