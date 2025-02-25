/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

typedef enum TCODE
{
  /*--- Kellerbefehle ---*/
  puValVrLocl,/*00 (int Displ)  [Kellern Wert lokale  Variable]               */
  puValVrMain,/*01 (int Displ)  [Kellern Wert Main    Variable]               */
  puValVrGlob,/*02 (int Proc,int Displ)  [Kellern Wert globale Variable]      */
  puAdrVrLocl,/*03 (int Displ)  [Kellern Adresse lokale  Variable]            */
  puAdrVrMain,/*04 (int Displ,int Proc)  [Kellern Adresse Main    Variable]   */
  puAdrVrGlob,/*05 (int Displ)  [Kellern Adresse globale Variable]            */
  puConst    ,/*06 (int Index)  [Kellern einer Konstanten]                    */
  storeVal   ,/*07 ()           [Speichern Wert -> Adresse, beides aus Keller]*/
  putVal     ,/*08 ()           [Ausgabe eines Wertes aus Keller nach stdout] */
  getVal     ,/*09 ()           [Eingabe eines Wertes von  stdin -> Keller ]  */
  /*--- arithmetische Befehle ---*/
  vzMinus    ,/*0A ()           [Vorzeichen -]                                */
  odd        ,/*0B ()           [ungerade -> 0/1]                             */
  /*--- binaere Operatoren kellern 2 Operanden aus und das Ergebnis ein ----*/
  OpAdd      ,/*0C ()           [Addition]                                    */
  OpSub      ,/*0D ()           [Subtraktion ]                                */
  OpMult     ,/*0E ()           [Multiplikation ]                             */
  OpDiv      ,/*0F ()           [Division ]                                   */
  cmpEQ      ,/*10 ()           [Vergleich = -> 0/1]                          */
  cmpNE      ,/*11 ()           [Vergleich # -> 0/1]                          */
  cmpLT      ,/*12 ()           [Vergleich < -> 0/1]                          */
  cmpGT      ,/*13 ()           [Vergleich > -> 0/1]                          */
  cmpLE      ,/*14 ()           [Vergleich <=-> 0/1]                          */
  cmpGE      ,/*15 ()           [Vergleich >=-> 0/1]                          */
  /*--- Sprungbefehle ---*/
  call       ,/*16 (int ProzNr) [Prozeduraufruf]                              */
  retProc    ,/*17 ()           [Ruecksprung]                                 */
  jmp        ,/*18 (int RelAdr) [SPZZ innerhalb der Funktion]                 */
  jnot       ,/*19 (int RelAdr) [SPZZ innerhalb der Funkt.,Beding.aus Keller] */
  entryProc  ,/*1A (int lenVar,int ProcIdx)[???]                              */
  putStrg    ,/*1B (char[])                                                   */
  pop        ,/*1C */
  swap       ,/*1D               Austausch Adressse gegen Wert                */ 
  EndOfCode  ,/*1E */
  put        ,/*1F Wert und Port vom Stack*/
  get        ,/*20 Adr  und Port vom Stack*/  
  OpAddAddr   /*21 add 64bitAddress + 32 bit Offset                           */
}tCode;
#define MAX_LEN_OF_CODE 81
