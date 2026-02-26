/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = (int)strlen(key);
  for (int index = 0; index < length; index++) {
    result += key[index];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  if (table == NULL) {
    return;
  }

  for (int index = 0; index < HT_SIZE; index++) {                     // Všetky zoznamy nastavíme na NULL (prázdna tabuľka).
    (*table)[index] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL) {
    return NULL;
  }

  int index = get_hash(key);                            // Získanie pozície v tabulke podľa kľúču.
  ht_item_t *item = (*table)[index];

  while (item != NULL) {                                  // Prejdeme zoznamom.
    if (strcmp(item->key, key) == 0) {
      return item;
    }
    else {
      item = item->next;
    }
  }

  return NULL;                                         // Prvok s daným kľúčom nie je v zozname.
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 * V případě úspěchu funkce tuto skutečnost indikuje návratovou hodnotou 
 * true, v opačném případě (např. při selhání alokace) vrací funkce false.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
bool ht_insert(ht_table_t *table, char *key, float value) {
  if (table == NULL || key == NULL) {
    return false;
  }

  ht_item_t *founded = ht_search(table, key);                     // Pokiaľ existuje, aktualizujeme.
  if (founded != NULL) {
    founded->value = value;
    return true;
  }

  int index = get_hash(key);

  ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));         // Vytvorenie novej položky.
  if (new_item == NULL) {
    return false;
  }


  new_item->key = key;
  new_item->value = value;
  new_item->next = (*table)[index];                                     // Vloženie pred nastávajúci začiatok zoznamu.
  (*table)[index] = new_item;
  
  return true;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL) {
    return NULL;
  }

  ht_item_t *item = ht_search(table, key);              // Najdeme položku prostredníctvom ht_search.
  if (item == NULL) {
    return NULL;
  }

  return &item->value;                                  // Vraciame adresu hodnoty.
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if(table == NULL || key == NULL) {
    return;
  }

  int index = get_hash(key);
  ht_item_t *item = (*table)[index];
  ht_item_t *prev = NULL;

  while (item != NULL) {                              // Prechádzame zoznam na danej pozícií.
    if(strcmp(item->key, key) == 0) {
      if (prev == NULL) {                               // Odpojenie prvku zo zoznamu.
        (*table)[index] = item->next;
      }
      else {
        prev->next = item->next;
      }

      free(item);
      return;
    }

    prev = item;
    item = item->next;

  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  if (table == NULL) {
    return;
  }

  int counter;

  for (counter = 0; counter < HT_SIZE; counter++) {               // Prejdeme každé miesto v tabuľke.
    ht_item_t *element = (*table)[counter];

    while (element != NULL) {                                 // Uvoľníme všetky prvky zoznamu.
      ht_item_t *next = element->next;

      free(element);

      element = next;
    }

    (*table)[counter] = NULL;                                   // Po zmazaní je prázdná.
  }
}
