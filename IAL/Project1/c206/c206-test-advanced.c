/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Základní testy pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Martin Tuček, září 2005
 *  Úprava: Kamil Jeřábek, září 2019
 *          Daniel Dolejška, září 2021
 */

#include "c206-test-utils.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))

int MAX_PACKET_COUNT = 10;


TEST(test_init, "Inicializace seznamu") //00
	DLL_Init(test_list);

	test_print_list(test_list);
ENDTEST

TEST(test_first_active, "Výběr prvního prvku v seznamu (jiný prvek je aktivní)") //01
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertLast(test_list, 1);

	DLL_Last(test_list);
	test_print_list(test_list);

	DLL_First(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_first_inactive, "Výběr prvního prvku v seznamu (seznam není aktivní)") //02
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertLast(test_list, 2);
	DLL_InsertLast(test_list, 1);

	DLL_First(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_first_empty, "Výběr prvního prvku v seznamu (seznam je prázdný)") //03
	DLL_Init(test_list);

	DLL_First(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_last_active, "Výběr posledního prvku v seznamu (jiný prvek je aktivní)") //04
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertLast(test_list, 3);

	test_print_list(test_list);

	DLL_Last(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_last_inactive, "Výběr posledního prvku v seznamu (seznam není aktivní)") //05
	DLL_Init(test_list);
	DLL_InsertLast(test_list, 2);
	DLL_InsertLast(test_list, 3);
	DLL_InsertFirst(test_list, 1);

	DLL_Last(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_last_empty, "Výběr posledního prvku v seznamu (seznam je prázdný)") //06
	DLL_Init(test_list);

	DLL_Last(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_previous_active, "Výběr předcházejícího prvku v seznamu (seznam je aktivní)") //07
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);
	DLL_Last(test_list);

	test_print_list(test_list);

	DLL_Previous(test_list);
	test_print_list(test_list);

	DLL_Previous(test_list);
	test_print_list(test_list);
	
	DLL_Previous(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_previous_inactive, "Výběr předcházejícího prvku v seznamu (seznam není aktivní)") //08
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);

	test_print_list(test_list);

	DLL_Previous(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_next_active, "Výběr následujícího prvku v seznamu (seznam je aktivní)") //09
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);

	test_print_list(test_list);

	DLL_Next(test_list);
	test_print_list(test_list);

	DLL_Next(test_list);
	test_print_list(test_list);

	DLL_Next(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_next_inactive, "Výběr následujícího prvku v seznamu (seznam není aktivní)") //10
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);

	test_print_list(test_list);

	DLL_Next(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_isActive_active, "Výběr následujícího prvku v seznamu (seznam je aktivní)") //11
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	DLL_First(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	for(int i = 0; i < 3; i++) {
		DLL_Next(test_list);
		printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");
	}
ENDTEST

//EDIT: nevím jestli "Is list element active?" je hned po printu nebo až po DLL_Next
TEST(test_isActive_inactive, "Výběr následujícího prvku v seznamu (seznam není aktivní)") //12
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	for(int i = 0; i < 3; i++) {
		DLL_Next(test_list);
		printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");
	}
ENDTEST

TEST(test_getFirst_active, "Získání hodnoty prvního prvku (seznam je aktivní)") //13
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);
	test_print_list(test_list);

	long test_result;

	test_result = -1;
	DLL_GetFirst(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);

	DLL_Next(test_list);
	test_print_list(test_list);

	test_result = -1;
	DLL_GetFirst(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);
ENDTEST

TEST(test_getFirst_inactive, "Získání hodnoty prvního prvku (seznam není aktivní)") //14
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	long test_result;

	test_result = -1;
	DLL_GetFirst(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);

	DLL_Next(test_list);
	test_print_list(test_list);

	test_result = -1;
	DLL_GetFirst(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);
ENDTEST

//EDIT: Upravený od originálu, protože to nedává smysl co tam má
TEST(test_getFirst_empty, "Získání hodnoty prvního prvku (seznam je prázdný)") //15
	DLL_Init(test_list);
	test_print_list(test_list);

	long test_result;

	test_result = -1;
	DLL_GetFirst(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);
ENDTEST

//Podle testu se aktivní prvek nastaví na první prvek seznamu a pak jde na další
TEST(test_getLast_active, "Získání hodnoty posledního prvku (seznam je aktivní)") //16
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);
	DLL_First(test_list);	
	test_print_list(test_list);

	long test_result;

	test_result = -1;
	DLL_GetLast(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);

	DLL_Next(test_list);
	test_print_list(test_list);

	test_result = -1;
	DLL_GetLast(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);
ENDTEST

TEST(test_getLast_inactive, "Získání hodnoty posledního prvku (seznam není aktivní)") //17
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);
	test_print_list(test_list);

	long test_result;

	test_result = -1;
	DLL_GetLast(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);

	DLL_Previous(test_list);
	test_print_list(test_list);

	test_result = -1;
	DLL_GetLast(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);
ENDTEST

//EDIT: Upravený od originálu, protože to nedává smysl co tam má
TEST(test_getLast_empty, "Získání hodnoty posledního prvku (seznam je prázdný)") //18
	DLL_Init(test_list);
	test_print_list(test_list);

	long test_result;

	test_result = -1;
	DLL_GetLast(test_list, &test_result);
	printf("%-15s: %ld\n", "Returned value", test_result);
ENDTEST

//EDIT: Upravený od originálu, protože to nedává smysl co tam má
TEST(test_getValue_active, "Získání hodnoty aktivního prvku který odkazuje na první (seznam je prázdný)") //18
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);

	long test_result;

	for (int i = 0; i < 3; i++) {
		test_print_list(test_list);
		test_result = -1;
		DLL_GetValue(test_list, &test_result);
		printf("%-15s: %ld\n", "Returned value", test_result);
		DLL_Next(test_list);
	}
ENDTEST

//EDIT: Upravený od originálu, protože to nedává smysl co tam má
TEST(test_getValue_inactive, "Získání hodnoty podle aktivního prvního prvku (seznam není aktivní)") //20
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);

	long test_result;

	for (int i = 0; i < 3; i++) {
		test_print_list(test_list);
		test_result = -1;
		DLL_GetValue(test_list, &test_result);
		printf("%-15s: %ld\n", "Returned value", test_result);
		DLL_Next(test_list);
	}
ENDTEST

TEST(test_setValue_active, "Změna hodnoty aktivního prvku (seznam je aktivní)") //21
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);
	test_print_list(test_list);

	DLL_SetValue(test_list, 3);
	test_print_list(test_list);

	DLL_Last(test_list);
	DLL_SetValue(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_setValue_inactive, "Změna hodnoty aktivního prvku (seznam není aktivní)") //22
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_SetValue(test_list, 3);
	test_print_list(test_list);

	DLL_Next(test_list);
	DLL_Next(test_list);
	DLL_Next(test_list);

	DLL_SetValue(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_insertFirst_active, "Vložení prvu na první místo (seznam je aktivní)") //23
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_First(test_list);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 2);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_insertFirst_inactive, "Vložení prvu na první místo (seznam není aktivní)") //24
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 2);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_insertFirst_empty, "Vložení prvu na první místo (seznam je prázdný)") //25
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 3);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 2);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_insertLast_active, "Vložení prvu na poslední místo (seznam je aktivní)") //26
	DLL_Init(test_list);
	DLL_InsertLast(test_list, 1);
	DLL_Last(test_list);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 2);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 3);
	test_print_list(test_list);
ENDTEST

TEST(test_insertLast_inactive, "Vložení prvu na poslední místo (seznam není aktivní)") //27
	DLL_Init(test_list);
	DLL_InsertLast(test_list, 1);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 2);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 3);
	test_print_list(test_list);
ENDTEST

TEST(test_insertLast_empty, "Vložení prvu na poslední místo (seznam je prázdný)") //28
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 1);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 2);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 3);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteFirst_active, "Odstranění prvního prvku (první prvek je aktivní)") //29
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteFirst_active_next, "Odstranění prvního prvku (následující prvek je aktivní)") //30
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);
	DLL_Next(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteFirst_inactive, "Odstranění prvního prvku (seznam není aktivní)") //31
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	DLL_DeleteFirst(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteFirst_empty, "Odstranění prvního prvku (seznam je prázdný)") //32
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
ENDTEST

TEST(test_deleteLast_active, "Odstranění posledního prvku (poslední prvek je aktivní)") //33
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_Last(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteLast_active_next, "Odstranění posledního prvku (předcházející prvek je aktivní)") //34
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_Last(test_list);
	DLL_Previous(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteLast_inactive, "Odstranění posledního prvku (seznam není aktivní)") //35
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	DLL_DeleteLast(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteLast_empty, "Odstranění posledního prvku (seznam je prázdný)") //36
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
ENDTEST

TEST(test_deleteAfter_active, "Odstranění následujícího prvku v seznamu (seznam je aktivní)") //37
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);
	test_print_list(test_list);

	DLL_DeleteAfter(test_list);
	test_print_list(test_list);

	DLL_DeleteAfter(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteAfter_inactive, "Odstranění následujícího prvku v seznamu (seznam není aktivní)") //38
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_DeleteAfter(test_list);
	DLL_DeleteAfter(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteAfter_last, "Odstranění následujícího prvku v seznamu (prvek je v seznamu na posledním místě)") //39
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_Last(test_list);
	test_print_list(test_list);

	DLL_DeleteAfter(test_list);
	DLL_DeleteAfter(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteBefore_active, "Odstranění předcházejícího prvku v seznamu (seznam je aktivní)") //40
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_First(test_list);
	test_print_list(test_list);

	DLL_DeleteBefore(test_list);
	test_print_list(test_list);

	DLL_DeleteBefore(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteBefore_inactive, "Odstranění předcházejícího prvku v seznamu (seznam není aktivní)") //41
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_DeleteBefore(test_list);
	DLL_DeleteBefore(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_deleteBefore_first, "Odstranění předcházejícího prvku v seznamu (prvek je v seznamu na posledním místě)") //42
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_Last(test_list);
	test_print_list(test_list);

	DLL_DeleteBefore(test_list);
	DLL_DeleteBefore(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_insertAfter_inactive, "Vložení několika prvků za aktivní prvek (neaktivní seznam)") //43
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_InsertAfter(test_list, 2);
	DLL_InsertAfter(test_list, 3);
	DLL_InsertAfter(test_list, 4);
	DLL_InsertAfter(test_list, 5);
	test_print_list(test_list);
ENDTEST

TEST(test_insertAfter_between, "Vložení několika prvků za aktivní prvek (mezi prvky)") //44
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 5);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_InsertAfter(test_list, 2);
	DLL_InsertAfter(test_list, 3);
	DLL_InsertAfter(test_list, 4);
	test_print_list(test_list);
ENDTEST

TEST(test_insertAfter_last, "Vložení několika prvků za aktivní prvek (na konec seznamu)") //45
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 4);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_InsertAfter(test_list, 3);
	DLL_Next(test_list);
	DLL_InsertAfter(test_list, 2);
	DLL_Next(test_list);
	DLL_InsertAfter(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_insertBefore_inactive, "Vložení několika prvků před aktivní prvek (neaktivní seznam)") //46
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_InsertBefore(test_list, 2);
	DLL_InsertBefore(test_list, 3);
	DLL_InsertBefore(test_list, 4);
	DLL_InsertBefore(test_list, 5);
	test_print_list(test_list);
ENDTEST

TEST(test_insertBefore_between, "Vložení několika prvků před aktivní prvek (mezi prvky)") //47
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 5);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_InsertBefore(test_list, 2);
	DLL_InsertBefore(test_list, 3);
	DLL_InsertBefore(test_list, 4);
	test_print_list(test_list);
ENDTEST

TEST(test_insertBefore_last, "Vložení několika prvků před aktivní prvek (na konec seznamu)") //48
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 4);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_InsertBefore(test_list, 3);
	DLL_Next(test_list);
	DLL_InsertBefore(test_list, 2);
	DLL_Next(test_list);
	DLL_InsertBefore(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_memory, "Korektní uvolňování paměti") //49
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 4);
	DLL_InsertFirst(test_list, 5);
	DLL_Dispose(test_list);
ENDTEST

void (*tests[])(void) = {
		test_init,
		test_first_active,
		test_first_inactive,
		test_first_empty,
		test_last_active,
		test_last_inactive,
		test_last_empty,
		test_previous_active,
		test_next_active,
		test_next_inactive,
		test_isActive_active,
		test_isActive_inactive,
		test_getFirst_active,
		test_getFirst_inactive,
		test_getFirst_empty,
		test_getLast_active,
		test_getLast_inactive,
		test_getLast_empty,
		test_getValue_active,
		test_getValue_inactive,
		test_setValue_active,
		test_setValue_inactive,
		test_insertFirst_active,
		test_insertFirst_inactive,
		test_insertFirst_empty,
		test_insertLast_active,
		test_insertLast_inactive,
		test_insertLast_empty,
		test_deleteFirst_active,
		test_deleteFirst_active_next,
		test_deleteFirst_inactive,
		test_deleteFirst_empty,
		test_deleteLast_active,
		test_deleteLast_active_next,
		test_deleteLast_inactive,
		test_deleteLast_empty,
		test_deleteAfter_active,
		test_deleteAfter_inactive,
		test_deleteAfter_last,
		test_deleteBefore_active,
		test_deleteBefore_inactive,
		test_deleteBefore_first,
		test_insertAfter_inactive,
		test_insertAfter_between,
		test_insertAfter_last,
		test_insertBefore_inactive,
		test_insertBefore_between,
		test_insertBefore_last,
		test_memory
};

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [test_id]\n", argv[0]);
		return 1;
	}

	char *test_id_reminder = NULL;
	long test_id;
	if (argc == 2)
	{
		test_id = strtol(argv[1], &test_id_reminder, 10);
		if (test_id_reminder[0] != 0)
		{
			fprintf(stderr, "Usage: %s {test_id}\n", test_id_reminder);
			fprintf(stderr, "Unexpected test_id: %s\n", test_id_reminder);
			return 1;
		}

		if (test_id < 0)
		{
			test_id = TEST_COUNT + test_id;
		}

		if (test_id + 1 > TEST_COUNT)
		{
			fprintf(stderr, "Unknown test: %ld (test count: %ld)\n", test_id, TEST_COUNT);
			return 1;
		}

		tests[test_id]();
	}
	else
	{
		printf("C206 - Doubly Linked List, Advanced Tests\n");
		printf("=========================================\n");

		for (test_id = 0; test_id < TEST_COUNT; test_id++)
		{
			tests[test_id]();
		}
	}

	return 0;
}

/* Konec c206-test.c */
