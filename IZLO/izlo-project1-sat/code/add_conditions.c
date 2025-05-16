#include <stddef.h>
#include "cnf.h"

//
// LOGIN: <xdrabbo00>
//

/** Funkce demonstrující vytvoření nové (arbitrárně vybrané) klauzule
* ve tvaru "h_{0,1} || -v_{0,1}" do výrokové formule
* @param formula výroková formule, do níž bude klauzule přidána
*/
void conditions_example(CNF* formula) {
    assert(formula != NULL);

    // vytvoření nové klauzule
    Clause* cl = create_new_clause(formula);
    
    // přidání proměnné h_{0,1} do klauzule
    // proměnná říká, že v regionu 0 je produkován produkt 1 jako hlavní
    // cl - klauzule, do níž přidáváme literál
    // true - značí, že přidaný literál je pozitivní proměnná
    // MAIN_PRODUCT - značí, že pracujeme s hlavním produktem
    // 0 - značí region s indexem 0
    // 1 - značí produkt s indexem 1s
    add_literal_to_clause(cl, true, MAIN_PRODUCT, 0, 1);

    // přidání proměnné -v_{0,1} do klauzule
    // proměnná říká, že v regionu 0 není produkován produkt 1 jako vedlejší
    // cl - klauzule, do níž přidáváme literál
    // false - značí, že přidaný literál je negativní proměnná
    // SIDE_PRODUCT - značí, že pracujeme s vedlejším produktem
    // 0 - značí region s indexem 0
    // 1 - značí produkt s indexem 1
    add_literal_to_clause(cl, false, SIDE_PRODUCT, 0, 1);
}

/** Funkce vytvářející klauzule ošetřující podmínku, že v každém regionu
* je produkován alespoň jeden hlavní produkt.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void all_regions_min_one_main_product(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);
    
    for (unsigned k = 0; k < num_of_regions; ++k) {
        Clause* cl = create_new_clause(formula);
        for (unsigned p = 0; p < num_of_products; ++p) {
            add_literal_to_clause(cl, true, MAIN_PRODUCT, k, p);
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku, že v každém regionu
* je produkován nejvýše jeden hlavní produkt.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void all_regions_max_one_main_product(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);

    for (unsigned k = 0; k < num_of_regions; ++k) {
        for (unsigned p_1 = 0; p_1 < num_of_products; ++p_1) {
            for (unsigned p_2 = 0; p_2 < num_of_products; ++p_2) {
                if (p_1 >= p_2) { continue; }
                Clause* cl = create_new_clause(formula);
                add_literal_to_clause(cl, false, MAIN_PRODUCT, k, p_1);
                add_literal_to_clause(cl, false, MAIN_PRODUCT, k, p_2);
            }
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku, že v každém regionu
* je produkován nejvýše jeden vedlejší produkt.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void all_regions_max_one_side_product(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);
    
    // Zde doplňte řešení
    for (unsigned k = 0; k < num_of_regions; ++k ){                             // Cyklus iterujúci cez všetky regióny.
        for (unsigned p_1 = 0; p_1 < num_of_products; ++p_1) {                  // Cyklus iterujúci cez všetky produkty (prvý produkt).
            for (unsigned p_2 = 0; p_2 < num_of_products; ++p_2) {              // Cyklus iterujúci cez všetky produkty (druhý produkt).
                if (p_1 >= p_2) {continue; }                                    // Vyhne sa duplikátom a nezmyselným kombináciam.

                Clause* cl = create_new_clause(formula);                        // Vytvorí novú klauzulu a pridá ju do formuly.

                add_literal_to_clause(cl, false, SIDE_PRODUCT, k, p_1);        // Funkcia pridávajúca literál do klauzule.
                add_literal_to_clause(cl, false, SIDE_PRODUCT, k, p_2);        // Funkcia pridávajúca literál do klauzule.
                
            }
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku, že v každém regionu
* se hlavní a vedlejší produkt liší
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void main_side_products_different(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);

    // Zde doplňte řešení
    for (unsigned k = 0; k < num_of_regions; ++k){                              // Cyklus iterujúci cez všetky regióny.
        for (unsigned p_1 = 0; p_1 < num_of_products; ++p_1){                   // Cyklus iterujúci cez všetky produkty.

                Clause* cl = create_new_clause(formula);                        // Vytvorí novú klauzulu a pridá ju do formuly.

                add_literal_to_clause(cl, false, MAIN_PRODUCT, k, p_1);         // Funkcia pridávajúca literál do klauzule.
                add_literal_to_clause(cl, false, SIDE_PRODUCT, k, p_1);         // Funkcia pridávajúca literál do klauzule.
        }
    }
}


/** Funkce vytvářející klauzule ošetřující podmínku, že 
* sousední regiony nesdílejí hlavní a vedlejší produkt.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
* @param neighbours seznamy sousedů
*/
void neighbour_regions_different_main_products(CNF* formula, unsigned num_of_regions, unsigned num_of_products, const NeighbourLists *neighbours) {
    assert(formula != NULL);
    assert(num_of_regions > 0);
    assert(neighbours != NULL);
    
    // Zde doplňte řešení
    for (unsigned first = 0; first < num_of_regions; ++first) {                                 // Cyklus iterujúci cez všetky regiony (prvý región).
        for (unsigned second = first; second < num_of_regions; ++second) {                      // Cyklus iterujúci cez všetky regióny (druhý región). 
            if (first >= second) {continue; }
                if (are_neighbours(neighbours, first, second) ) {                               // Ak sú prvý a druhý prvok susedia
                    for (unsigned product = 0; product < num_of_products; ++product) {          // Cyklus iterujúci cez všetky produkty.

                        Clause* cl = create_new_clause(formula);                                // Vytvorí novú klauzulu a pridá ju do formuly.

                        add_literal_to_clause (cl, false, MAIN_PRODUCT, first, product);        // Funkcia pridávajúca literál do klauzule.
                        add_literal_to_clause (cl, false, MAIN_PRODUCT, second, product);       // Funkcia pridávajúca literál do klauzule.

                    }
                }
        }
    }


    // Tip: Využijte implementovanou funkci are_neighbours.
    // Příklad použití:
    // unsigned region1 = 0;
    // unsigned region2 = 1;
    // if (are_neighbours(neighbours, region1, region2)) { ... }
}


/** Funkce vytvářející klauzule ošetřující podmínku, že 
* každý produkt je v některém regionu hlavním produktem.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void all_products_at_least_once_main_products(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);
    
    for (unsigned p = 0; p < num_of_products; ++p) {
        Clause* cl = create_new_clause(formula);
        for (unsigned k = 0; k < num_of_regions; ++k) {
            add_literal_to_clause(cl, true, MAIN_PRODUCT, k, p);
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku, 
* že v hlavním regionu (v regionu 0) neexistuje žádný vedlejší produkt.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void no_side_product_in_main_region(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);
    
    for (unsigned p = 0; p < num_of_products; ++p) {
        Clause* cl = create_new_clause(formula);
        add_literal_to_clause(cl, false, SIDE_PRODUCT, 0, p);
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku, 
* že produkt produkovaný v hlavním regionu (v regionu 0) jako hlavní produkt
* je ještě v některém regionu produkován jako vedlejší.
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_regions počet regionů
* @param num_of_products počet produktů
*/
void main_region_main_product_as_side_product_elsewhere(CNF* formula, unsigned num_of_regions, unsigned num_of_products) {
    assert(formula != NULL);
    assert(num_of_regions > 0);
    
    // Zde doplňte řešení
    for (unsigned p = 0; p < num_of_products; ++p) {                        // Cyklus iterujúci cez všetky produkty.

            Clause*cl = create_new_clause(formula);                         // Vytvorí novú klauzulu a pridá ju do formuly.
            add_literal_to_clause(cl, false, MAIN_PRODUCT, 0, p);           // Funkcia pridávajúca literál do klauzule.

            for (unsigned k = 0; k < num_of_regions; ++k) {                 // Cyklus iterujúci cez všetky regióny.
                add_literal_to_clause(cl, true, SIDE_PRODUCT, k, p);        // Funkcia pridávajúca literál do klauzule.
            }
        }
}

/** Bonusová funkce k projektu
* @return vrací bonusovou odpověď
*/
char *bonus(void) {

    // Doplňte správný řetězec
    return "";
}
