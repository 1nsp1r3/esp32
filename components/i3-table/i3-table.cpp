#include "i3-table.h"

/**
 * Table should respect this format:
 * [
 *   [adcValue1, displayValue1],
 *   [adcValue2, displayValue2],
 *   ...
 * ]
 * Order is important, adcValue1 should be lower than adcValue2
 */

/**
 * Search the nearest inferior value to Value and return the index
 */
int i3TableIndexOfPreviousValue(int Table[][2], int Size, int Value){
    int i = 0;
    while( (Value - Table[i][0]) >= 0){
        i++;
        if (i >= Size) break;
    }
    if (!i) return 0; //AdcValue is lower than the minimal value of the table
    return i-1;
}

/**
 * As i3TableIndexOfPreviousValue()
 */
int i3TableIndexOfNextValue(int Table[][2], int Size, int Value){
    int previousIndex = i3TableIndexOfPreviousValue(Table, Size, Value);
    return previousIndex >= Size-1 ? previousIndex : previousIndex+1;
}

/**
 * Main function
 * if Value is between 2 edges, apply a linear function
 */
float i3TableGetValue(int Table[][2], int Size, int Value){
  int pIndex = i3TableIndexOfPreviousValue(Table, Size, Value);
  int nIndex = i3TableIndexOfNextValue(Table, Size, Value);

  if (Value < Table[0][0])       return Table[0][1];      //Value is out of bounds (lower than the minimal value)
  if (Value >= Table[Size-1][0]) return Table[Size-1][1]; //Value is out of bounds (greather than the maximal value)

  //Between 2 values ? Apply a linear function
  return i3LinearGetX(Table[pIndex][1], Table[pIndex][0], Table[nIndex][1], Table[nIndex][0], Value);
}
