# Copilot Instructions for WAR Game Challenge

## Project Overview
This is a C programming challenge implementing a WAR board game simulation in progressive levels (Novato, Aventureiro, Mestre). The project teaches data structures and C concepts through game mechanics.

## Architecture
- **Single-file structure**: Main implementation in `war.c`
- **Level progression**: Each level builds upon the previous with new features
- **Modular design**: Higher levels require function decomposition and separation of concerns

## Key Components
- `Territorio` struct: Contains `nome`, `cor_exercito`, `num_tropas`
- Dynamic memory management for territories array
- Battle simulation with dice rolls using `rand()`

## Development Patterns
- **Language**: C with Portuguese comments and user-facing strings
- **Memory**: Use `calloc` for dynamic allocation, `free` for cleanup
- **Input/Output**: `fgets` for strings, `scanf` for numbers, `printf` for output
- **Randomization**: Seed with `srand(time(NULL))` for reproducible battles

## Battle Logic
- Attacker rolls up to 3 dice, defender up to 2
- Compare highest dice: attacker wins if higher or equal
- Ties favor attacker
- Defender loses 1 troop on loss; territory conquered if troops reach 0

## Build and Run
```bash
gcc war.c -o war
./war
```

## Mission System (Master Level)
- Random mission assignment: destroy green army or conquer 3 territories
- Interactive menu for game actions
- Mission completion check after each battle

## Code Style
- Use `const` for immutable parameters
- Modular functions for each major operation
- Clear Portuguese comments explaining logic
- Input validation for user choices

## Examples
- Territory initialization: `territorios = calloc(5, sizeof(Territorio));`
- Battle input: Choose attacker (1-5) and defender (1-5)
- Output format: Display all territories with current state after each action