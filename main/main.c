/**
 * @file main.c
 * @author Loïc Reboursière - UMONS, CLick (loic.reboursiere@umons.ac.be)
 * @brief This code demonstrates how to read, parse and store in an array the content of a csv file stored directly on the esp. 
 * The copy of the csv file on the esp has been done following this tutorial : https://esp32tutorials.com/esp32-spiffs-esp-idf/
 *  > Display MenuConfig
 *    > Serial Flasher config : check Flas size
 *    > Partition table : 
 *      > Partition table : Custom partition table, 
 *      > Enter name of csv partition file
 *    > Hit save
 * @version 0.1
 * @date 2022-11-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "esp_spiffs.h"
#include "esp_log.h"
#include "string.h"

#define TAG "spiffs"
#define TAG_FUNCTION "Function"
#define TAG_SPLITFCT "splitLines function"
#define TAG_TOK "STRTOK"
#define FILE_LINE_SIZE 256

int currentLine = -1;
int nbOfElements = -1;
const char delimiters[]= ";,";

/**
 * @brief Get the Number Of lines present in a given file
 * @param file Pointer to the FILE object representing the file to be analysed.
 * @return Number of lines present in the file. 
 */
int getNumberOfLines( FILE * file ) 
{
  int lineNb = 0;
  char line[FILE_LINE_SIZE];
  while ( fgets(line,sizeof(line),file) != NULL)
    lineNb++;
  
  rewind(file);
  
  return lineNb;
}


/**
   * @fn int getNumberOfElements(char line[], const char * delimiter)
   * @brief Count the number of elements in a text line regarding a specific delimiter.
   *
   * @param line Line in which the elements need to be counted.
   * @param delimiter The delimiter separating the different elements
  Return : 
    @return The number of elements present in the line.
  */
int getNumberOfElements(char * line, const char * delimiter)
{
  int nbElements = 0;

  char * element = strtok ( line, delimiter );
  while (element != NULL)
  {
    nbElements++;
    element = strtok( NULL, delimiter);
  }
    
  return nbElements;
} 

/**
 * @brief Check if character c is part of the delimiters array
 * 
 * @param c The current character to be compared to delimiters
 * @param delimiters List of characters that are used to split the line
 * @return true if character is one of the delimiters
 * @return false if c is not part of delimiters
 */
bool isCharInCharArray( char c, const char delimiters[] )
{
  for( int i = 0; i<strlen(delimiters) ; i++ )
  {
    if( c == delimiters[ i ] )
      return true;
  }
  return false;
}

/**
 * @brief  '{"type": "text" ,"data":"COUCOU", "x_start": 100,"y_start": 100,"x_size": 800}{END}'
 * 
 */
void makeJSON ()
{

}



/**
 * HAS BEEN REPLACED BY RELEVANT STDIO STRING FUNCTION : strtok
   * @fn void splitLine( char lineToSplit[], char delimiter, char * splitted[] )
   * @brief Simple function to split a char[] based on a specific delimiter.
   * 
   * @param lineToSplit Line to be splitted.
   * @param delimiter Character used to split the char [].
   * @param splitted[] Pointer to array receiving the plitted line.
   *
   * @return Only prints out the different elements for now, but should return something (struct ?).
  */
void splitLine( char lineToSplit[], const char * delimiter[], char ** elementsInLine, int nbOfElements )
{
  int previousDelimiterIdx = -1, elementIdx = -1;
  bool last_column_element = true;
  
  //char **elementsInLine;// [ nbOfElements ][FILE_LINE_SIZE];
  //elementsInLine = malloc(nbOfElements * sizeof(char *));
  //for (int i=0; i<nbOfElements; i++) {
  //  elementsInLine[i] = malloc(FILE_LINE_SIZE);
  //}

  printf( "Line to be splitted [%s] - size of line : %i\n", lineToSplit, strlen(lineToSplit) );
  for (int i=0; i < strlen(lineToSplit); i++)
  { 
    //char current_char = lineToSplit[ i ];
    //printf( "--in splitLine : Character : %c - idx : %i\n",  current_char, i );
    if( isCharInCharArray(lineToSplit[i], delimiters) )
    {
              
      //ESP_LOGI( TAG_SPLITFCT, "Char is a part of delimiters");
      elementIdx++;
      int strSize = i - (previousDelimiterIdx + 1);
      char element[ strSize+1 ];

      if( strSize > 0 ) 
      {
        memcpy( element, &lineToSplit[previousDelimiterIdx + 1], strSize );
        element[strSize] = '\0';
        //elementsInLine[elementIdx]
        if( i == strlen( lineToSplit ) - 1 ) 
        {
          elementIdx++;
          last_column_element = false;
        //  strcpy( elementsInLine[nbOfElements], "-");
        }  
      }
      else 
      {
        strcpy( element, "-" );
      }
       
      //printf( "Splitted element : %s\n", element );
      //if( !last_column_element )
      //  printf( "Splitted element : -\n" );
  /*      strcpy(elementsInLine[ elementIdx ], element);
        //printf( "%i : %s - size %i\n" , elementIdx, elementsInLine[elementIdx], strlen( elementsInLine[elementIdx] ) );
        elements[elementIdx] = element;
  */  
      strcpy( elementsInLine[ elementIdx ], element);    
      printf( "Splitted element [%i]: %s - %p\n", elementIdx, elementsInLine[ elementIdx ], &elementsInLine[ elementIdx ] );
      previousDelimiterIdx = i;
      //splittedLine[elementIdx] = element;


    }
     
  }

  printf("Nb of splitted elements % i\n", (elementIdx + 1) );
  //return elementsInLine;
} 

void displayCharOfLine( char line[] )
{
  printf( "Line to be splitted [%s] - size of line : %i\n", line, strlen(line) );
  for( int i = 0; i < strlen(line); i++ ) {
    char current_char = line[ i ];
    printf("--in display : Character[%i] : %c\n", i, current_char);
  }
    
    
}


void getLine( int lineIdx, FILE * file, char splittedLine[][FILE_LINE_SIZE] )
{
  currentLine = lineIdx;
  rewind(file);
  
  char line[FILE_LINE_SIZE];
  int lineNb = 0;

  while(fgets(line, sizeof(line), file) != NULL)
  {
    if( lineNb == lineIdx ) 
    {
      splitLine( line, &delimiters, splittedLine, nbOfElements ); 
      break;
    }
    else
    {
      lineNb++; 
    }
  } 
}

void getNextLine( FILE * file, char splittedLine[][FILE_LINE_SIZE] ) 
{
  getLine(++currentLine, file, splittedLine );
}

void app_main(void)
{
  esp_vfs_spiffs_conf_t config = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true,
  };
  esp_err_t ret = esp_vfs_spiffs_register(&config);

  // Just checking if SPI FFS has been mounted correctly
  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      ESP_LOGE( TAG, "Failed to mount or format filesystem");
    }
    else if (ret == ESP_ERR_NOT_FOUND)
    {
      ESP_LOGE (TAG, "Failed to find SPIFFS partition");
    }
    else
    {
      ESP_LOGE (TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
  }
  else 
  {
    ESP_LOGI( TAG, "Filesystem mounted !");
  }

  
  

  //FILE *file = fopen("/spiffs/data.txt", "r");
  char * root_folder = "/spiffs/";
//  const char * csv_filename = "order.csv";
//  const char * csv_filename = "data.csv";
  const char * csv_filename = "order_modified.csv";  

  char * csv_filepath[strlen( root_folder ) + 1 + strlen( csv_filename ) + 1];
  strcpy ( csv_filepath, root_folder );
  strcat ( csv_filepath, csv_filename );
  ESP_LOGI ( TAG, "CSV Filepath : %s", csv_filepath );

  FILE * file = fopen( csv_filepath, "r" );

  int nbOfLines = getNumberOfLines( file );
  ESP_LOGI (TAG, "Nb of lines in the file %i", nbOfLines);
  
  if( file == NULL || nbOfLines == -1 )
  {
    ESP_LOGE( TAG,"File (%s) does not exist or is empty !", csv_filepath );
  }
  else 
  {
    char line[FILE_LINE_SIZE];

    // Retrieve the numer of elements based on the first line of csv (columns names)
    char * firstLine = fgets( line, sizeof( line ), file );
    ESP_LOGI( TAG, "Size of firstLine % i - line : %s", FILE_LINE_SIZE, firstLine );
    nbOfElements = getNumberOfElements( firstLine, &delimiters );
    
    //char ** elementsInFile [ nbOfLines ];
    //char elementsInLine [ nbOfElements ][FILE_LINE_SIZE];
    char** elementsInLine = malloc(nbOfElements * sizeof(char *));
    for (int i=0; i<nbOfElements; i++) 
    {
        elementsInLine[i] = malloc(FILE_LINE_SIZE);
        printf("elementInLine[%i]: %p\n", i, &elementsInLine[ i ]);
    }
    
    //char * elementsInFile [ nbOfLines ][ nbOfElements ];
    
    ESP_LOGI( TAG, "Nb of elements of first line : %i", nbOfElements );
    
    
    //char identifier [] = ",Set quantity,42,IconIonicIosInformationCircle.png,1,Bolt M6 x 120 mm,BK-12,A9,25,3,";
    //char identifier [] = ",,,IconFeatherMessageCircle.png,2,Nut M18,AK-12,A2,34,3,";
    char identifier [] = "test;Check;OK;IconIonicIosNotifications.png;1,Ceci est un test,AK-34,A3,10,3,";
    //for( int i = 0; i< strlen(identifier); i++ )
    //  printf("Character[%i] : %c\n", i, identifier[ i ]);
    //displayCharOfLine( identifier );

    //currentLine = 4;
    /*
    currentLine = 20; // To test the case where a line doesn't exist
    if( currentLine > getNumberOfLines( file ) )
    {
      ESP_LOGE( TAG, "Asked for line (index : %i) doesn't exist, displaying last line in file", currentLine );
      currentLine = getNumberOfLines( file ) - 1;
      getLine( currentLine, file );
    } 
    else 
    {
      getLine( currentLine, file );      
    } 
    */

    printf("--- Attempting to display splitted elements\n");
    splitLine( identifier, &delimiters, elementsInLine, nbOfElements );
    printf("--- End of Attempting to display splitted elements\n");
    //printf( "%s\n", elementsInLine[3][5]);
    
    for( int i = 0; i < nbOfElements; i++)
    {
      printf( "Element in line " );
      printf( "%p \n", elementsInLine[i]);//, elementsInLine[nbOfElements] );
    }
    printf("elementInLine[%i]: %p\n", 1000, &elementsInLine[ 1000 ]);
    // Parse the rest of the file's lines
    ESP_LOGI( TAG, "Splitting lines" );
    while(fgets(line, sizeof(line), file) != NULL)
    {  
      
      /******** splitLine function version ******/
      //splitLine( line, &delimiter, nbOfElements );
      //memcpy( splitLine( line, delimiter, nbOfElements ), elementsInLine, sizeof( elementsInLine ) );
      //for( int i = 0; i < nbOfElements; i++)
      //  printf( "elementsInLine[%i] : %s", i, elementsInLine[i] );

      /************ end of splitLine ************/
      //STRTOK VERSION
      /**int elementIdx = 0;
      char * element = strtok ( line, &delimiter );
      while( elementIdx < nbOfElements )//while (element != NULL)
      {
        if( element == '\0' || element == NULL || element == '\n' || strlen(element) == 0 || element == '\r' ) strcpy(element, "--");
        printf( "element : [%s] - %p\n", element, element );
        strcpy(elementsInLine[ elementIdx ], element);
        //elementsInFile[ lineIdx ][ lineElement ] = element;
        //printf( "elementInLine[%i] : %s\n", elementIdx, elementsInLine[ elementIdx ] );
        element = strtok (NULL, &delimiter); 
        elementIdx++;
      }

      lineIdx++;
      **/
      
      
    }

    ESP_LOGI( TAG, "Displaying splitted lines" );
    /*
    for( int l = 0; l < nbOfLines ; l++ ) 
    {
      printf( "Line %i", l );
      printf( elementsInFile[ l ] );
      for( int e = 0; e < nbOfElements ; e++ )
      {
        printf("%s ", elementsInFile[ l ][ e ]);  
      }
      printf( "\n" );
    }
    */
    fclose(file); 
  }

  esp_vfs_spiffs_unregister(NULL);
}