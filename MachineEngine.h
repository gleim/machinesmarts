 /***************************************************
 *
 *	MachineEngine.h
 *
 * 	MachineEngine header
 *
 *	STE (internal) layer class
 *
 **************************************************/

  #ifndef MACHINEENGINE_H
  #define MACHINEENGINE_H 1

  /* RTOS/HW specific include files */
  #include "predef.h"
  #include "string.h"
  #include <basictypes.h>
  #include <constants.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <ucos.h>
  #include <serial.h>
  #include <cfinter.h>
  #include <startnet.h>
  #include <math.h>

  #include "MachineVariables.h"
  #include "MachineParameters.h"
  #include "BackpropagationNode.h"

  /* Canned data meta-data */
  #define INPUT_BITS  24
  #define OUTPUT_BITS  7

  /* MAXIMUM_ON_BITS defines the max # of bitmaps w/in canned train vectors */
  #define MAXIMUM_ON_BITS 10
  
  /* MAXIMUM_STATES defines the max # of bits available w/in the bitmaps    */
  /* note: this value is for parsing, and must equal MAXIMUM_BYTES * 8      */
  #define MAXIMUM_STATES  32
  
  /* MAXIMUM_BYTES defines the max # of bytes represented w/in the bitmaps  */
  /* note: this value is for parsing, and must equal MAXIMUM_STATES / 8     */
  #define MAXIMUM_BYTES    4

  /* NUMBER_CANNED defined the number of canned training vectors      */
  #define NUMBER_CANNED   84
  
  /* UNIT_ACTIVATION_THRESHOLD is an empirically derived number indicating */
  /* the unit activity necessary to be considered equivalent to binary one */
  #define UNIT_ACTIVATION_THRESHOLD 0.6
  
  /* EPOCH_ERROR_THRESHOLD is an empirically derived number indicating */
  /* acceptable performance of the network in the given environment    */
  #define EPOCH_ERROR_THRESHOLD 45
  
  /* These hex patterns can be used for AND bitmasking */
  #define INPUT_ELEMENTS         0x00FFFFFF
  #define OUTPUT_ELEMENTS        0xFF000000
  #define EMPTY_ELEMENT          0x00000000
  
  /* Input State bitmap specification */
  #define INPUT_VELOCITY_BACK     0x00000001
  #define INPUT_VELOCITY_STOP     0x00000002
  #define INPUT_VELOCITY_FWD1     0x00000004
  #define INPUT_VELOCITY_FWD2     0x00000008

  #define INPUT_HEADING_N         0x00000010
  #define INPUT_HEADING_NE        0x00000020
  #define INPUT_HEADING_E         0x00000040 
  #define INPUT_HEADING_SE        0x00000080
  #define INPUT_HEADING_S         0x00000100
  #define INPUT_HEADING_SW        0x00000200
  #define INPUT_HEADING_W         0x00000400
  #define INPUT_HEADING_NW        0x00000800
  
  #define INPUT_GOAL_HEADING_N    0x00001000
  #define INPUT_GOAL_HEADING_E    0x00002000
  #define INPUT_GOAL_HEADING_S    0x00004000
  #define INPUT_GOAL_HEADING_W    0x00008000
  
  #define INPUT_OBSTACLE_FIELD_A  0x00010000
  #define INPUT_OBSTACLE_FIELD_B  0x00020000
  #define INPUT_OBSTACLE_FIELD_C  0x00040000
  #define INPUT_OBSTACLE_FIELD_D  0x00080000
  #define INPUT_OBSTACLE_FIELD_E  0x00100000
  #define INPUT_OBSTACLE_FIELD_F  0x00200000
  #define INPUT_OBSTACLE_FIELD_G  0x00400000
  #define INPUT_OBSTACLE_FIELD_H  0x00800000

  /* Output State bitmap specification */
  #define OUTPUT_VELOCITY_BACK    0x01000000
  #define OUTPUT_VELOCITY_STOP    0x02000000
  #define OUTPUT_VELOCITY_FWD1    0x04000000
  #define OUTPUT_VELOCITY_FWD2    0x08000000

  #define OUTPUT_STEER_LEFT       0x10000000
  #define OUTPUT_STEER_STRAIGHT   0x20000000
  #define OUTPUT_STEER_RIGHT      0x40000000

        
        /* canned vector for training */
        static const unsigned long CannedVectors[NUMBER_CANNED]
                                                [MAXIMUM_ON_BITS] = 
        {
        
/*****************************************************/
/*                                                   */
/*        Heading SOUTH, Goal heading NORTH          */
/*                        (preference LEFT)          */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading SOUTH, Goal heading NORTH          */
/*                        (preference RIGHT)         */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading WEST, Goal heading NORTH           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

/*****************************************************/
/*                                                   */
/*        Heading EAST, Goal heading NORTH           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading NORTH, Goal heading NORTH          */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_STRAIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_N,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },             

/*****************************************************/
/*                                                   */
/*        Heading NORTH, Goal heading SOUTH          */
/*                        (preference LEFT)          */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading NORTH, Goal heading SOUTH          */
/*                        (preference RIGHT)         */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading EAST, Goal heading SOUTH           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading WEST, Goal heading SOUTH           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

/*****************************************************/
/*                                                   */
/*        Heading SOUTH, Goal heading SOUTH          */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_STRAIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_S,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading EAST, Goal heading WEST            */
/*                      (preference RIGHT)           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading EAST, Goal heading WEST            */
/*                       (preference LEFT)           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

/*****************************************************/
/*                                                   */
/*        Heading NORTH, Goal heading WEST           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading SOUTH, Goal heading WEST           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading WEST, Goal heading WEST            */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_STRAIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_W,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
                         
/*****************************************************/
/*                                                   */
/*        Heading WEST, Goal heading EAST            */
/*                       (preference LEFT)           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

/*****************************************************/
/*                                                   */
/*        Heading WEST, Goal heading EAST            */
/*                       (preference RIGHT)          */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_W,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading NORTH, Goal heading EAST           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_N,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
/*****************************************************/
/*                                                   */
/*        Heading SOUTH, Goal heading EAST           */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_S,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },

            
/*****************************************************/
/*                                                   */
/*        Heading EAST, Goal heading EAST            */
/*                                                   */
/*****************************************************/

          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_STRAIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_A,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_B,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_RIGHT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            },
            
          { INPUT_GOAL_HEADING_E,
            INPUT_HEADING_E,
            INPUT_VELOCITY_FWD1, 
            INPUT_OBSTACLE_FIELD_C,
            OUTPUT_VELOCITY_FWD1,
            OUTPUT_STEER_LEFT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT,
            EMPTY_ELEMENT            }
                                       };

class MachineEngine
  {
  public:
		MachineEngine( );
		~MachineEngine( );
		void configure( MachineParameters * );
        void display( );		
		void start( );
		void stop( );
  private:
		void initialize( );
		void initializeRTOS( );
		void iterate( );
		void train( );
		void storePattern(unsigned char *);
		bool training( );
		void parseInputForDisplay(unsigned char *);
		void parseOutputForDisplay(unsigned char *);

        MachineVariables * poVars;
        MachineParameters * poMachineParameters;
        
        bool bStopRequested;
		bool bInitialized;
		
		static const int HIDDEN_LENGTH_MULTIPLIER = 3;
		static const int HIDDEN_LENGTH_DIVISOR    = 2;

        /* I/O should be higher priority than main processing task  */
        static const int INPUT_OUTPUT_PRIORITY    = MAIN_PRIO - 1;  

        DWORD InputOutputTaskStack[USER_TASK_STK_SIZE];

        char PatternInputElement[MAXIMUM_STATES];
        char PatternTargetElement[MAXIMUM_STATES];
  };

  #endif  // #ifndef MACHINEENGINE_H

