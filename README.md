# figure_finder

Detects and counts figures in a boolean matrix using cell-side connectivity. Implemented in C++.

## Data

Operates on matrices of 1s and 0s, where 1s represent figure parts and 0s represent empty space. Matrices are stored as `.txt` files.

### Example Matrices

Refer to the `data` folder for sample matrices.

### Matrix Generation

Generate matrices of custom size, figure count, and average figure size using the following script:
```bash
python3 scripts/generate_matrix.py --rows 2000 --cols 1500 --figures 50000 --outfile data/matrix.txt
```
