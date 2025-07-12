# 🧬 Evolutionary Coin Change Solver in C

This project implements and compares **evolutionary algorithms** to solve the **coin change problem** using both classical genetic strategies and hybrid methods with hill climbing. Developed in C as part of the Introduction to Artificial Intelligence course at Instituto Superior de Engenharia de Coimbra (ISEC), 2024/2025.

## 🧠 Project Summary

The goal is to find combinations of coins that match a target value using different optimization techniques:

- 🧬 Genetic Algorithms (with tournament selection, roulette wheel, crossover and mutation)
- 🔁 Hill Climbing
- 🧪 Hybrid Methods (Evolutionary + Local Search)

---

## 📂 Project Structure

- `main.c` – Entry point and user interaction
- `algoritmo.c` – Contains the implementation of all algorithms and utilities
- `algoritmo.h` – Header with declarations and constants
- `TP2_IIA_2425.pdf` – Official project specification

---

## 🛠️ How to Compile

```bash
gcc main.c algoritmo.c -o coin_solver -lm
✅ Ensure you have the math library (-lm) linked.

▶️ How to Run
bash
Copiar código
./coin_solver
Follow the menu to choose:

Population size

Number of generations

Type of selection

Crossover method

Mutation

Penalty value

Target value (V)

Neighborhood strategy (for hybrids)

💡 Algorithms Included
Hill Climbing (Trepa Colinas)

Neighborhoods: Increment, Swap, Substitution

Genetic Algorithm

Selection: Tournament, Elitism, Roulette

Crossover: Two-point, Adaptive Uniform

Mutation: Simple, Swap

Hybrid Algorithm

Combines Genetic Algorithm with Hill Climbing

🎓 Author
Rodrigo Bezerra Braga
📍 Student of Computer Engineering
🎓 Instituto Superior de Engenharia de Coimbra (ISEC)
🔐 Interests: Cybersecurity, AI, System Programming

🧠 Skills & Technologies
C Programming

Genetic Algorithms

Evolutionary Computation

Local Search / Metaheuristics

Hill Climbing

Command-Line Applications

Algorithm Design

Problem Solving

Optimization

Git & GitHub

