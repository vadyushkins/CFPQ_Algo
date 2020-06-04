# SparseGraph

## Grammar: SG_cnf
## Test type: Construct

| Graph | Queries | Matrix Multiplication Amount | Time (s) |
|:-----:|:-------:|:----------------------------:|:--------:|
| G10k-0.0001 | brute | 79554 | 26.690481 |
| G10k-0.0001 | smart | 19988 | 0.155216 |
| G10k-1e-05 | brute | 3520 | 0.084732 |
| G10k-1e-05 | smart | 2014 | 0.016044 |
| G20k-0.0001 | brute | 180896 | None |
| G20k-0.0001 | smart | 79572 | 0.649766 |
| G20k-1e-05 | brute | 16132 | 1.212081 |
| G20k-1e-05 | smart | 7896 | 0.062749 |
| G40k-1e-05 | brute | 96992 | 36.218718 |
| G40k-1e-05 | smart | 31858 | 0.245635 |

## Grammar: SG_cnf
## Test type: Deconstruct

| Graph | Queries | Matrix Multiplication Amount | Time (s) |
|:-----:|:-------:|:----------------------------:|:--------:|
| G10k-0.0001 | brute | 119928 | 96.747689 |
| G10k-0.0001 | smart | 154954 | 66.093876 |
| G10k-1e-05 | brute | 4028 | 0.189115 |
| G10k-1e-05 | smart | 10308 | 0.465972 |
| G20k-0.0001 | brute | 41340 | None |
