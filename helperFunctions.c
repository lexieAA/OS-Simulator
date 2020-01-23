#include <stdlib.h>
#include <string.h>

#define STRING_SMALL 32
#define STRING_LARGE 128

/*
 *  Name:        findChar
 *  Description: Loops through a char array searching for a specific target
 *               char. Returns the index of the char if it is found, and -1 if
 *               it is not found.
 */
int findChar(char * string, char target)
{

    // Counter for loop
    int count;

    // Loop through every char in the string
    for (count = 0; count < strlen(string); count++)
    {

        // If the char is equal to our target, return count
        if (string[count] == target)
        {
            return count;
        }

    }

    // If the char is not found, return -1
    return -1;

}

/*
 *  Name: stringToInt
 *  Description: Converts a char array to an integer. Example: {'7', '1', '4'}
 *               would be converted into an integer 714.
 */
int stringToInt(char * string)
{

    // Variables for loop, digit place, and final answer
    int count;
    int digit = 1;
    int answer = 0;

    // Loop through array
    for (count = strlen(string) - 1; count >= 0; count--)
    {

        // Convert char int, multiple it by the digit place, add to total
        answer += (string[count] - '0') * digit;

        // Increment digit to the next place
        digit *= 10;
    }

    // Return the final total
    return answer;

}

/*
 *  Name:        substring
 *  Description: Returns a subsection of a character array derived from the
 *               start and end integer parameters passed to it.
 */
char * substring(char * string, int start, int end)
{

    // Initialize loop counter, char array index, and char array
    int count;
    int index = 0;
    char * newString = malloc(STRING_LARGE);

    // Loop through input array, assign values to it
    for (count = start; count < end; count++)
    {
        newString[index] = string[count];
        index++;
    }

    // Mark the end of the string in the array
    newString[index] = '\0';

    // Return the final new array
    return newString;

}
