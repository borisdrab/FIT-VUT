; Autor reseni: Boris Nicolas Drab xdrabbo00

; Projekt 2 - INP 2025
; Souhlaskove modulovana samohlaskova sifra na architekture MIPS64

; DATA SEGMENT
                .data
msg:            .asciiz "borisnicolasdrab"
cipher:         .space  31 ; misto pro zapis zasifrovaneho textu

vowels:         .asciiz "aeiouy"

params_sys5:    .space  8 ; misto pro ulozeni adresy pocatku
                          ; retezce pro vypis pomoci syscall 5

; CODE SEGMENT
                .text

main:
                daddi   r2, r0, msg                             ; vozrovy vypis: adresa msg do r4

                daddi   r3, r0, cipher

                daddi   r4, r0, 26

                ;jal     print_string                           ; výpis

loop_encrypt:                                                   ; hlavná smyčka - zpracovanie znakov vstupného reťazca

                lbu     r5, 0(r2)                               ; načítanie akuálneho znaku
                beq     r5, r0, finish_encrypt                  ; pokiaľ je znak nulovy, koniec šifrovania

                jal     is_vowel
                bne     r1, r0, vowel_case                      ; pokiaľ je samohláska, skok


;  --------------------- Zpracovanie Spoluhlások ---------------------
handle_consonant:
                sb r5, 0(r3)

                daddi    r6, r0, 96                             ; prepočet pozície v abecede
                dsub     r6, r5, r6
                daddi    r4, r6, 0


                j       advance_pointers

; --------------------------------------------------------------------

; ------------------------ Zpracovanie Samohlások ---------------------
vowel_case:                                                     ; index = znak - ASCII(97)
                daddi  r6, r0, 97
                dsub   r7, r5, r6

                dadd   r7, r7, r4                               ; index + key

                daddi r8, r0, 26                                ; index = index modulo 26
                ddiv  r7, r8
                mfhi  r7

                dadd  r7, r7, r6                                ; prepočet späť na ASCII

                sb  r7, 0(r3)                                   ; zapíš šifru

; ------------------------------ Posun Ukazateľa ------------------
advance_pointers: 
                    daddi r2, r2, 1                             ; ďalší znak vstupu
                    daddi r3, r3, 1                             ; ďalšia pozícia
                    J     loop_encrypt

;---------------------------------- Ukonci Sifrovanie ------------
finish_encrypt: 
                sb   r0, 0(r3)

                daddi r4, r0, cipher                            ; príprava pre print_string
                jal     print_string                            ; vypíš

                syscall 0                                        ; ukonči

;----------------------------------- Funkcia : is_vowel ----------------
is_vowel:
            daddi   r10, r0, vowels                             ; zoznam samohlások

is_vowel_loop: 
                lbu r11, 0(r10)                                 ; načítaj
                beq r11, r0, not_vowel

                beq r11, r5, yes_vowel                          ; ak sa rovná, je to samohláska
                
                daddi r10, r10, 1                               ; ďalší znak v zozname

                j     is_vowel_loop

yes_vowel:
                daddi r1, r0, 1
                jr      r31


not_vowel: 
                daddi r1, r0, 0
                jr        r31
 

; NASLEDUJICI KOD NEMODIFIKUJTE!

                syscall 0   ; halt

print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
 