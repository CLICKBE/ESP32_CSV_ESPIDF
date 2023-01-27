/**
 * 
 * @file main.c
 * @author Loïc Reboursière - UMONS, CLick (loic.reboursiere@umons.ac.be)
 * @brief This code demonstrates how to read, parse and store in an array the content of a csv file stored directly on the esp. 
 * ESP32 CSV with ESP-IDF– CLICK - UMONS (Loïc Reboursière) is free software: you can redistribute it and/or modify it under the terms of the Apache License Version 2.0. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the Apache License Version 2.0 for more details.
 * You should have received a copy of the Apache License Version 2.0 along with this program.  If not, see http://www.apache.org/licenses/
 * Each use of this software must be attributed to University of MONS – CLICK (Loïc Reboursière).
 * Any other additional authorizations may be asked to avre@umons.ac.be. 
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
 * @fn int getNumberOfLines( FILE * file )
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
   * @fn int getNumberOfElements(char * line, const char * delimiter)
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
 * @fn bool isCharInCharArray( char c, const char delimiters[] )
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
 * @fn void makeJSONLineToSend ( char * elementToSend, char * jsonToSend )
 * @brief Take one element of the splitted file line and incorporate it in json command to visualization interface.
 * JSON generated message is of type  :'{"type": "text" ,"data":elementToSend, "x_start": 100,"y_start": 100,"x_size": 800}{END}'
 * 
 * @param elementToSend Element from splitted line to be send as JSON command.
 * @param jsonToSend Array in which generated JSON string is stored.
 */
void makeJSONLineToSend ( char * elementToSend, char * jsonToSend )
//char* makeJSON ( char * elementToSend, char * json )
{
  char * start = "{\"type\": \"text\" ,\"data\":\"";
  char * end = ", \"x_start\": 100,\"y_start\": 100,\"x_size\": 800}{END}";

  strcpy( jsonToSend, start);
  strcat( jsonToSend, elementToSend );
  strcat( jsonToSend, end );
}



/**
   * @fn void splitLine( char lineToSplit[], char delimiter, char * splitted[] ).
   * @brief Simple function to split a char[] based on specific delimiters.
   * 
   * @param lineToSplit Line to be splitted.
   * @param delimiter Character(s) used to split the char [].
   * @param elementsInLine Array of strings in which the the splitted elements will be stored.
   * @param nbOfElements
   *
  */
void splitLine( char lineToSplit[], const char * delimiter[], char ** elementsInLine )
{
  int previousDelimiterIdx = -1, elementIdx = -1;

  printf( "Line to be splitted [%s]\n", lineToSplit );
  for (int i=0; i < strlen(lineToSplit); i++)
  { 
    if( isCharInCharArray(lineToSplit[i], delimiters) )
    {
      elementIdx++;
      int strSize = i - (previousDelimiterIdx + 1);
      char element[ strSize+1 ];

      if( strSize > 0 ) 
      {
        memcpy( element, &lineToSplit[previousDelimiterIdx + 1], strSize );
        element[strSize] = '\0';
      }
      else 
      {
        strcpy( element, "-" );
      }

      strcpy( elementsInLine[ elementIdx ], element);    
      //printf( "Splitted element [%i]: %s - %p\n", elementIdx, elementsInLine[ elementIdx ], &elementsInLine[ elementIdx ] );
      previousDelimiterIdx = i;
    }  
  }
} 

/**
 * @fn void getLine( int lineIdx, FILE * file, char** elementsInLine )
 * @brief Send specific line of a file to splitLine function.
 * 
 * @param lineIdx Index of the line to be splitted.
 * @param file Opened file from which to read from.
 * @param elementsInLine Array of strings in which splitted elements will be stored.
 */
void getLine( int lineIdx, FILE * file, char** elementsInLine )
{
  currentLine = lineIdx;
  rewind(file);
  
  char line[FILE_LINE_SIZE];
  int lineNb = 0;

  while(fgets(line, sizeof(line), file) != NULL)
  {
    if( lineNb == lineIdx ) 
    {
      splitLine( line, &delimiters, elementsInLine ); 
      break;
    }
    else
    {
      lineNb++; 
    }
  } 
}

/**
 * @fn void getNextLine( FILE * file, char** elementsInLine ) 
 * @brief Increment the currentLine index before sending it to the splitLine function.
 * 
 * @param file Opened file from which to read from.
 * @param elementsInLine Array of strings in which splitted elements will be stored.
 */
void getNextLine( FILE * file, char** elementsInLine ) 
{
  currentLine += 1;
  printf( "--- currentLine %i ", currentLine );
  getLine( currentLine, file, elementsInLine );
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


  char * root_folder = "/spiffs/";
  const char * csv_filename = "data.csv";

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
    ESP_LOGI( TAG, "Nb of elements of first line : %i", nbOfElements );

    char** elementsInLine = malloc(nbOfElements * sizeof(char *));
    for (int i=0; i<nbOfElements; i++) 
    {
        elementsInLine[i] = malloc(FILE_LINE_SIZE);
        strcpy( elementsInLine[i], "-" ); // Filling the array with our empty character
    }
    
    char identifier [] = "test;Check;OK;IconIonicIosNotifications.png;1,Ceci est un test,AK-34,A3,10,3,";

    /* TEST for specific line
    currentLine = 4;
    
    //currentLine = 20; // To test the case where a line doesn't exist
    if( currentLine > getNumberOfLines( file ) )
    {
      ESP_LOGE( TAG, "Asked for line (index : %i) doesn't exist, displaying last line in file", currentLine );
      currentLine = getNumberOfLines( file ) - 1;
      getLine( currentLine, file, elementsInLine );
    } 
    else 
    {
      getLine( currentLine, file, elementsInLine );      
    } 
    */

    for( int l = 0; l < nbOfLines; l++ )
    {
      getNextLine( file, elementsInLine );
      printf( "Splitted elements : " );
      for( int e = 0; e < nbOfElements; e++)
        printf( "%s | ", elementsInLine[e]);//, elementsInLine[nbOfElements] );
      char jsonToSend [FILE_LINE_SIZE]; 
      printf( "\n" ); 
      makeJSONLineToSend( elementsInLine[ 5 ], jsonToSend );
      printf( "*** JSON  : %s ", jsonToSend );
      printf( "\n\n" );
    }

    fclose(file); 
  }

  esp_vfs_spiffs_unregister(NULL);
}