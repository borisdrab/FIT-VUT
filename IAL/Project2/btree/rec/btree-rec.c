/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
  *tree = NULL;                         // Strom je po inicializácii prázdny.
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  if (tree == NULL) {                 // Prišli sme na konec vetvy -> kľúč sa v strome nenachádza.
    return false;
  }

  if (tree->key == key) {            // Nájdený uzol -> vrátime ukazateľ na obsah.
    if (value != NULL) {
      * value = &tree->content;
    }
    return true;
  }

  if (tree->key < key) {                                // Podľa porovnania kľúčov ideme rekurzivne vpravo alebo vľavo.
    return bst_search(tree->right, key, value);
  } else if (tree->key > key) {
    return bst_search(tree->left, key, value);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
bool bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  if  (!tree) {
    return false;
  }

  bst_node_t *root = *tree;

  if (root == NULL) {                                       // Vkladanie do prázdneho miesta -> vytvoríme nový uzol.
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    if (!new_node) {
      return false;
    }

    new_node->content = value;
    new_node->key = key;
    new_node->right = NULL;
    new_node->left = NULL;

    *tree = new_node;
    return true;

  } else if (root != NULL) {                                  // Kľúč už existuje -> prepíšeme hodnotu.
    if (root->key == key) {
      if (root->content.value != NULL) {
        free(root->content.value);
      }
 
      root->content = value;
      return true;
    }

    if (root->key < key) {                                     // Podľa porovnania kľúča pokračujeme rekurzivňe v pravom alebo ľavom podstrome.
      return bst_insert(&root->right, key, value);
    }
    else {
      return bst_insert(&root->left, key, value);
    }
  }
  return false;

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if (target == NULL) {
    return;
  }

  bst_node_t *root = *tree;

  if (root->right != NULL) {                                // Hľadáme najpravejší (najvačší) uzol -> ideme doprava rekurzivňe.
    bst_replace_by_rightmost(target, &root->right);
    return;
  }

  if (target->content.value != NULL) {                      // Koreň je najpravejší potomok v tomto podstrome.
    free(target->content.value);
  }

  target->key = root->key;
  target->content = root->content;
  *tree = root->left;

  free(root);
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (tree == NULL || *tree == NULL) {
    return;
  }

  if ((*tree)->key > key) {                             // Hľadáme uzol podĺa kľúča, rekurzivne v ľavom alebo pravom podstrome.
    bst_delete(&(*tree)->left, key);
    
  } else if ((*tree)->key < key) {
    bst_delete(&(*tree)->right, key);
  } else {
    bst_node_t *node_to_del = *tree;                          // Nájdený uzol pripravený k odstráneniu.

    if (node_to_del->left != NULL && node_to_del->right != NULL) {
      bst_replace_by_rightmost(node_to_del, &node_to_del->left);            // Má obidva podstromy -> nahradíme najpravejším z ľavého.
    }
    else {
      bst_node_t *child_node = NULL;                                // Má maximálne jeden podstrom -> volíme potomka, ktorý ho nahradí.

      if (node_to_del->right != NULL) {
        child_node = node_to_del->right;
      } else {
        child_node = node_to_del->left;
      }

      if (node_to_del->content.value != NULL) {
        free(node_to_del->content.value);
      }

      *tree = child_node;
      free(node_to_del);
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (tree == NULL || *tree == NULL) {
    return;
  }

  bst_node_t *current = *tree; 

  bst_dispose(&current->right);                     // Rekurzivne zrušíme pravý a ľavý podstrom.
  bst_dispose(&current->left);

  if (current->content.value != NULL) {
    free(current->content.value);
  }

  free(current);
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL) return;
                                              // uzol - ľavý - pravý
  bst_add_node_to_items(tree, items);
  bst_preorder(tree->left, items);
  bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL) return;
                                              // ľavý - uzol - pravý
  bst_inorder(tree->left, items);
  bst_add_node_to_items(tree, items);
  bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL) return;
                                           // ľavý - pravý - uzol
  bst_postorder(tree->left, items);
  bst_postorder(tree->right, items);
  bst_add_node_to_items(tree, items);
}
