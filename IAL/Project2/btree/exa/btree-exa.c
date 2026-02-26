/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


static bool bst_insert_freq (bst_node_t **tree, char  key, int count) {

  if (count <= 0) {                                     
    return true;
  }

  bst_node_content_t node_content;                        // Vytvorenie obsahu uzlu s danou početnosťou.

  node_content.type = INTEGER;                            // Štruktúra z btree.h

  node_content.value = malloc(sizeof(int));
  if (node_content.value == NULL) {
    return false;
  }

  *(int *)node_content.value = count;

  if (!bst_insert(tree, key, node_content)) {                                           // Pokúšame sa vložiť do stromu.
    free(node_content.value);                                                           // Ak pokus zlyhá, uvoľníme alokovanú hodnotu.
    return false;
  }

  return true;
}

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je 
 * ukládán průběžně do stromu (klíč vždy lowercase). V případě úspěchu funkce tuto skutečnost
 * indikuje návratovou hodnotou true, v opačném případě (např. při selhání 
 * operace insert) vrací funkce false.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
bool letter_count(bst_node_t **tree, char *input) {

  if (tree == NULL || input == NULL) {
    return false; 
  }

  bst_init(tree);                                 // Uvedieme strom do prázdneho stavu.

  int a_count = 0, b_count = 0, c_count = 0, d_count = 0, e_count = 0, f_count = 0, g_count = 0, h_count = 0;                           // Počítadlo pre znaky.
  int i_count = 0, j_count = 0, k_count = 0, l_count = 0, m_count = 0, n_count = 0, o_count = 0, p_count = 0, q_count = 0;
  int r_count = 0, s_count = 0, t_count = 0, u_count = 0, v_count = 0, w_count = 0, x_count = 0, y_count = 0, z_count = 0;
  int space_count = 0, other_count = 0;

  for (size_t index = 0; input[index] != '\0'; ++index) {                       // Prejdeme celý vstupný reťazec.
    unsigned char element = (unsigned char)input[index];

    if (element >= 'A' && element <= 'Z') {                                 // Prevod veľkého písmena na malé podľa ASCII
      element = element + ('a' - 'A');
    }


    if (element >= 'a' && element <= 'z') {                                 // Počítanie oddelených elementov.
      switch(element) {
        case 'a': a_count++; break;
        case 'b': b_count++; break;
        case 'c': c_count++; break;
        case 'd': d_count++; break;
        case 'e': e_count++; break;
        case 'f': f_count++; break;
        case 'g': g_count++; break;
        case 'h': h_count++; break;
        case 'i': i_count++; break;
        case 'j': j_count++; break;
        case 'k': k_count++; break;
        case 'l': l_count++; break;
        case 'm': m_count++; break;
        case 'n': n_count++; break;
        case 'o': o_count++; break;
        case 'p': p_count++; break;
        case 'q': q_count++; break;
        case 'r': r_count++; break;
        case 's': s_count++; break;
        case 't': t_count++; break;
        case 'u': u_count++; break;
        case 'v': v_count++; break;
        case 'w': w_count++; break;
        case 'x': x_count++; break;
        case 'y': y_count++; break;
        case 'z': z_count++; break;
        default: break;
      }
    } else if (element == ' ') {                                              // Medzery
      space_count++;

    } else {                                                                // Všetko ostatné.
      other_count++;
    }
  
  }

  if (!bst_insert_freq(tree, 'a', a_count)) {                                       // Vloženie spočítaných frekvencií do stromu.
    return false;
  }
  if (!bst_insert_freq(tree, 'b', b_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'c', c_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'd', d_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'e', e_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'f', f_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'g', g_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'h', h_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'i', i_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'j', j_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'k', k_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'l', l_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'm', m_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'n', n_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'o', o_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'p', p_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'q', q_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'r', r_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 's', s_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 't', t_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'u', u_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'v', v_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'w', w_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'x', x_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'y', y_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, 'z', z_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, ' ', space_count)) {
    return false;
  }
  if (!bst_insert_freq(tree, '_', other_count)) {
    return false;
  }


  return true;
}
