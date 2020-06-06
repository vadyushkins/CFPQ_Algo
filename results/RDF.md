# RDF

## Grammar: g1_cnf
## Test type: Construct

| Graph | Brute | Smart |
|:-----:|:-----:|:-----:|
| skos | 0.002921 | 0.000984 |
| generations | 0.003387 | 0.001113 |
| travel | 0.003829 | 0.001116 |
| univ-bench | 0.004081 | 0.001182 |
| atom-primitive | 0.007227 | 0.001643 |
| biomedical-mesure-primitive | 0.00768 | 0.001844 |
| foaf | 0.008017 | 0.00234 |
| people_pets | 0.00844 | 0.002473 |
| funding | 0.017759 | 0.003925 |
| wine | 0.034335 | 0.006492 |
| pizza | 0.03465 | 0.007097 |
| core | 0.058669 | 0.009642 |
| pathways | 1.087667 | 0.042116 |

## Grammar: g1_cnf
## Test type: Correctness

| Graph | equal(Brute, Smart) |
|:-----:|:-------------------:|
| atom-primitive | True |
| biomedical-mesure-primitive | True |
| core | True |
| foaf | True |
| funding | True |
| generations | True |
| pathways | True |
| people_pets | True |
| pizza | True |
| skos | True |
| travel | True |
| univ-bench | True |
| wine | True |

## Grammar: g2_cnf
## Test type: Construct

| Graph | Brute | Smart |
|:-----:|:-----:|:-----:|
| skos | 0.001333 | 0.000569 |
| generations | 0.001374 | 0.000576 |
| travel | 0.001695 | 0.000642 |
| univ-bench | 0.001999 | 0.000615 |
| atom-primitive | 0.003951 | 0.000903 |
| biomedical-mesure-primitive | 0.004155 | 0.000986 |
| foaf | 0.003148 | 0.001344 |
| people_pets | 0.003507 | 0.001359 |
| funding | 0.007074 | 0.002203 |
| wine | 0.011149 | 0.003657 |
| pizza | 0.014902 | 0.0038 |
| core | 0.019435 | 0.005181 |
| pathways | 0.555223 | 0.022222 |

## Grammar: g2_cnf
## Test type: Correctness

| Graph | equal(Brute, Smart) |
|:-----:|:-------------------:|
| atom-primitive | True |
| biomedical-mesure-primitive | True |
| core | True |
| foaf | True |
| funding | True |
| generations | True |
| pathways | True |
| people_pets | True |
| pizza | True |
| skos | True |
| travel | True |
| univ-bench | True |
| wine | True |
