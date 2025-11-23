#!/usr/bin/env python3
import argparse
import random
import time
import os
from typing import List

GROWTH_BIAS = 0.7  # Probability to expand from the most recent cell (depth-first)
DEFAULT_DENSITY = 0.25  # Default fraction of matrix to fill with figures
MAX_ATTEMPTS_MULTIPLIER = 10  # Attempts per figure to avoid infinite loops

def write_matrix_to_file(matrix: List[List[int]], filename: str):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, 'w') as f:
        for row in matrix:
            f.write(' '.join(map(str, row)) + '\n')

def generate_figure(matrix, rows, cols, start_row, start_col, size, min_size=1):
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    def touches_other_figure(r, c, parent=None):
        for dr, dc in directions:
            nr, nc = r+dr, c+dc
            if parent is not None and (nr, nc) == parent:
                continue
            if 0 <= nr < rows and 0 <= nc < cols and matrix[nr][nc] == 1:
                return True
        return False

    if matrix[start_row][start_col] != 0:
        return set()
    # Do not start a figure if the starting cell touches another figure
    if touches_other_figure(start_row, start_col):
        return set()
    figure_cells = {(start_row, start_col)}
    matrix[start_row][start_col] = 2  # Mark as part of current figure
    expansion_list = [(start_row, start_col)]
    while len(figure_cells) < size and expansion_list:
        if random.random() < GROWTH_BIAS and len(expansion_list) > 1:
            current = expansion_list[-1]
        else:
            current = random.choice(expansion_list)
        random.shuffle(directions)
        expanded = False
        for dr, dc in directions:
            new_row = current[0] + dr
            new_col = current[1] + dc
            if not (0 <= new_row < rows and 0 <= new_col < cols):
                continue
            if matrix[new_row][new_col] != 0:
                continue
            if touches_other_figure(new_row, new_col, current):
                continue
            matrix[new_row][new_col] = 2  # Mark as part of current figure
            figure_cells.add((new_row, new_col))
            expansion_list.append((new_row, new_col))
            expanded = True
            break
        if not expanded:
            expansion_list.remove(current)
    # After growth, check if figure meets min_size and not a blocked singleton
    if len(figure_cells) < min_size:
        # Remove the figure (set all 2s back to 0)
        for r, c in figure_cells:
            matrix[r][c] = 0
        return set()
    # After growth, convert all 2s to 1s
    for r, c in figure_cells:
        matrix[r][c] = 1
    return figure_cells

def generate_random_matrix(rows, cols, num_figures, avg_figure_size=None):
    matrix = [[0 for _ in range(cols)] for _ in range(rows)]
    total_cells = rows * cols
    # Compute average figure size if not provided
    if avg_figure_size is None:
        total_figure_cells = int(total_cells * DEFAULT_DENSITY)
        avg_figure_size = max(1, total_figure_cells // num_figures)
    figures_created = 0
    attempts = 0
    max_attempts = num_figures * MAX_ATTEMPTS_MULTIPLIER
    while figures_created < num_figures and attempts < max_attempts:
        attempts += 1
        start_row = random.randint(0, rows - 1)
        start_col = random.randint(0, cols - 1)
        if matrix[start_row][start_col] == 1:
            continue
        # Vary figure size around the average
        min_size = max(1, avg_figure_size // 2)
        max_size = max(1, avg_figure_size * 2)
        size = random.randint(min_size, max_size)
        cells = generate_figure(matrix, rows, cols, start_row, start_col, size, min_size)
        if cells:
            figures_created += 1
    return matrix, figures_created

def main():
    parser = argparse.ArgumentParser(description="Generate matrix with figures (1s, with side adjacency).")
    parser.add_argument('--rows', type=int, required=True, help='Number of rows')
    parser.add_argument('--cols', type=int, required=True, help='Number of columns')
    parser.add_argument('--figures', type=int, required=True, help='Number of figures')
    parser.add_argument('--outfile', type=str, required=True, help='Output file path')
    parser.add_argument('--avg-figure-size', type=int, default=None, help='Average figure size (optional, overrides default)')
    parser.add_argument('--seed', type=int, default=42, help='Random seed')
    args = parser.parse_args()
    random.seed(args.seed)
    print(f"Generating random matrix: {args.rows}x{args.cols}, {args.figures} figures...")
    total_cells = args.rows * args.cols
    avg_figure_size = args.avg_figure_size
    if avg_figure_size is None:
        avg_figure_size = max(1, int(total_cells * DEFAULT_DENSITY) // args.figures)
    if args.figures * avg_figure_size > total_cells:
        max_figures = total_cells // avg_figure_size
        print(f"ERROR: Requested {args.figures} figures with average size {avg_figure_size} cannot fit in a {args.rows}x{args.cols} matrix.")
        print(f"Maximum possible figures for avg size {avg_figure_size}: {max_figures}")
        print(f"Suggestion: Use --figures {max_figures} or reduce --avg-figure-size.")
        exit(1)
    start_time = time.time()
    matrix, figures_created = generate_random_matrix(args.rows, args.cols, args.figures, avg_figure_size)
    print(f"Figures created: {figures_created}")
    write_matrix_to_file(matrix, args.outfile)
    elapsed = time.time() - start_time
    print(f"Matrix written to {args.outfile} in {elapsed:.2f} seconds")

if __name__ == "__main__":
    main()
