/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) 
{
    if (tree == NULL) {
      return;
    }
    *tree = NULL;               // Strom reprezentujeme ako prázdny (NULL koreň).
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value) 
{
  bst_node_t *current_node = tree;

  while (current_node != NULL) {              //Postupujeme smerom dole, pokiaľ nenájdeme klúč alebo NULL.
    if (key == current_node->key) {             //Nájdený uzol -> vrátime ukazateľ na jeho obsah.
      if (value != NULL) {
        *value = &current_node->content;
      }
      return true;
    }
    else if (key < current_node->key){
      current_node = current_node->left;
    } else {
      current_node = current_node->right;
    }
  }
  
  return false;
  
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel. V případě úspěchu funkce tuto skutečnost
 * indikuje návratovou hodnotou true, v opačném případě (např. při selhání 
 * alokace) vrací funkce false.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
bool bst_insert(bst_node_t **tree, char key, bst_node_content_t value) 
{
  if (tree == NULL) {
    return false;
  }

  if (*tree == NULL) {                                                  // Vkladanie do prázdného stromu -> vytvoríme koreň.
    bst_node_t *new_insert_node = malloc(sizeof(bst_node_t));
    if (!new_insert_node) {
      return false;
    }

    new_insert_node->key = key;
    new_insert_node->content = value;
    new_insert_node->left = NULL;
    new_insert_node->right = NULL;

    *tree = new_insert_node;
    return true;
  }

  bst_node_t *current_node = *tree;
  bst_node_t *parent_node = NULL;

  while (current_node != NULL) {                                      // Hľadanie miesta pre vloženie / aktualizáciu.
    parent_node = current_node;

    if (current_node->key == key) {                               // Klúč už existuje -> prepíšeme hodnotu.
      if (current_node->content.value) {
        free(current_node->content.value);
      }
      current_node->content = value;
      return true;
    }
    else if (current_node->key < key){
      parent_node = current_node;
      current_node = current_node->right;

    } else {
      parent_node = current_node;
      current_node = current_node->left;
    }
  }

  bst_node_t *new_insert_node = malloc(sizeof(bst_node_t));               // Vytvoríme nový listový uzol.
  if (new_insert_node == NULL) {
    return false;
  }

  new_insert_node->content = value;
  new_insert_node->key = key;
  new_insert_node->left = NULL;
  new_insert_node->right = NULL;
  

  if(key > parent_node->key) {                                            // Pripojenie nového uzlu ako pravého/ ľavého potomka.
    parent_node->right = new_insert_node;
  } else {
    parent_node->left = new_insert_node;
  }

  return true;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if (tree == NULL || target == NULL || *tree == NULL) {
    return;
  }

  bst_node_t *parent_node = NULL;
  bst_node_t *right_most = *tree;

  while (right_most->right != NULL) {             // Nájdeme najpravejší (najvačší) uzol v podstromu.
    parent_node = right_most;
    right_most = right_most->right;
    
  }

  if (target->content.value != NULL) {              // Uvoľníme pôvodní obsah cieľového uzlu.
    free(target->content.value);
  }

  target->key = right_most->key;                    // Zkopírujeme kľúč aj obsah z najpravejšíeho uzlu.
  target->content = right_most->content;

  if (parent_node == NULL) {                          // Odpojíme najpraveješí uzel z pôvodného miesta.
    *tree = right_most->left;
  } else {
    parent_node->right = right_most->left;
  }
  free(right_most);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (tree == NULL || *tree == NULL) {
    return;
  }

  bst_node_t *current_node = *tree;
  bst_node_t *parent_node = NULL;

  while (current_node != NULL && current_node->key != key) {        // Nájdeme uzol s daným klíčem (nebo dojdeme na NULL).
    parent_node = current_node;         
    if (key > current_node->key) {
      current_node = current_node->right;
    }
    else {
      current_node = current_node->left;
    }
  }

  if (!current_node) {                                               // Klíč v strome nieje -> nič nerobíme.
    return;
  }

  if (current_node->left != NULL && current_node->right != NULL) {          // Uzol má oba podstromy -> použijeme náhradu najpravejšieho z ľavého.
    bst_replace_by_rightmost(current_node, &current_node->left);
    return;
  }

  bst_node_t *child_node = NULL;

  if (current_node->left != NULL) {
    child_node = current_node->left;
  } else if (current_node->right != NULL) {
    child_node = current_node->right;
  } else {
    child_node = NULL;
  }

  if (parent_node == NULL) {                                        // Prepojíme ukazatel v rodičovi (alebo posunieme koreň).
    *tree = child_node;
  } else if (parent_node->left == current_node) {
    parent_node->left = child_node;
  } else {
    parent_node->right = child_node;
  }

  if (current_node->content.value != NULL) {                        // Uvoľníme obsah aj  uzol.
    free(current_node->content.value);
  }

  free(current_node);
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{

  if (!tree || !*tree) {
    return;
  }

  stack_bst_t used_stack;
  stack_bst_init(&used_stack);
  stack_bst_push(&used_stack, *tree);                         // Začíname od koreňa.

  while (!stack_bst_empty(&used_stack)) {                       // DFS cez zásobník a zmazanie uzlu.
    bst_node_t *current_node = stack_bst_pop(&used_stack);

    if (current_node->left != NULL) {
      stack_bst_push(&used_stack, current_node->left);
    }

    if (current_node->right != NULL) {
      stack_bst_push(&used_stack, current_node->right);
    }

    if (current_node->content.value != NULL) {
      free(current_node->content.value);
    }
    free(current_node);

  }
  *tree = NULL;

}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while (tree != NULL) {                      // Ideme doľava, po ceste vždy spracujeme uzol (preord.) a uložíme ho na stack.
    bst_add_node_to_items(tree, items);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }

}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t used_stack;
  stack_bst_init(&used_stack);

  bst_leftmost_preorder(tree, &used_stack, items);            // Zpracujeme ľavú sekvenciu od koreňa.

  while (!stack_bst_empty(&used_stack)) {                         // Po skončení ľavej vetvy vždy pokračujeme pravým podstromom z vrcholu stacku.
    bst_node_t *preor_node = stack_bst_pop(&used_stack);

    bst_leftmost_preorder(preor_node->right, &used_stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{

  while (tree != NULL) {                                            // Ukladáme všetky uzly po ľavej vetve na stack.
    stack_bst_push(to_visit, tree);

    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t used_stack;
  stack_bst_init(&used_stack);

  bst_leftmost_inorder(tree, &used_stack);

  while(!stack_bst_empty(&used_stack)) {                          // Vždy vezmeme vrchol stacku, spracujeme ho a potom pokračujeme do jeho pravého podstromu.
    bst_node_t *current_node = stack_bst_pop(&used_stack);

    bst_add_node_to_items(current_node, items);

    bst_leftmost_inorder(current_node->right, &used_stack);

  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while(tree != NULL) {                           // U každého uzlu si pamatáme, že sme ho navštívili po prvý krát.
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bool_t first_visit_stack;
  stack_bst_t nodes_stack;

  stack_bst_init(&nodes_stack);
  stack_bool_init(&first_visit_stack);
  bst_leftmost_postorder(tree, &nodes_stack, &first_visit_stack);             // Začneme ľavou sekvenciou od koreňa.

  while(!stack_bst_empty(&nodes_stack)) {
    bst_node_t *node = stack_bst_top(&nodes_stack);
    bool first_one = stack_bool_pop(&first_visit_stack);

    if (first_one) {                                                        // Prvá návšteva -> ešte musíme prejsť pravým podstromom.
      stack_bool_push(&first_visit_stack, false);
      bst_leftmost_postorder(node->right, &nodes_stack, &first_visit_stack);
    }
    else {                                                                  // Druhá návšteva -> zpracujeme uzol (postord.) a odstánime ho zo stacku.
      bst_add_node_to_items(node, items);
      (void)stack_bst_pop(&nodes_stack);
    }
  }
  
}
