/*!
 * \defgroup  specificationGroup Firmware Requirements Specification
 * 
 * # Overview
 * 
 * This firmware provides functions to control the activity of the PCB-26-349 board.
 * 
 * The Board is in charge of the following operations:
 * + Control the high voltage generation, interacting with the power board: PCB/26-351;
 * + Control a mAs-meter circuit that integrate the anodic current flowing in the X-RAY tube;
 * + Control the external Anode Starter Device through a dedicated set of Input/Outputs;
 * + Control a synch digital interface to synchronize the exposure sequence with the external Detector device;
 * + Control a synch digital interface to synchronize the exposure sequence with the external Filament device;
 * 
 * The board implement a CAN based communication protocol to communicate 
 * with the MAIN CPU of the system with a Slave role.
 * 
 * # CAN BUS Communication 
 * 
 * ## Bootloader 
 * The firmware shall implement the bootloader interface integrated in the Can Bus Protocol,
 * as described in the Device Can Bus protocol specification
 * 
 * ## Communication details
 * The board shall be assigned to the following ID Addresses:
 * + Bootloader ID: 0x159;
 * + Application ID: 0x19;
 * 
 * # mAs-Meter Specification
 * 
 * ## Anodic Current Integration function
 * 
 * + The mAs meter shall integrate the anodic current signal detected by the dedicated input.
 * + The Integration shall quickly notify the complete integration event to reduce the counting error below 1%
 * + The integration shall early terminate in error if a timeout should expire.
 *  + In case of Timeout event the partial integration count shall be provided. 
 * 
 * ## mAs monitoring and fault detection
 * 
 * During the Idle time the mAs meter shall be monitored to detect possible malfunctions
 * or bad calibration status.
 * 
 * 
 * ## mAs calibration
 * The mAs-meter shall be calibrated to provide the best performances 
 *
 * ## CAN Interface Commands
 * 
 * The Firmware shall implement a set of commands a data register in the 
 * CAN protocol communication in order to:
 * 
 * + Test the mAs-meter integration performances;
 * + Monitor the mAs-meter malfunctions;
 * + Calibrate the mAs-meter.
 * 
 * # High Speed Starter Specification 
 * 
 * ## High Speed Activation
 * The Firmware shall implement functions to activated 
 * the external High Speed Starter device at the High Speed Rate.
 * 
 * The Firmware shall handle a Timeout to stop the rotation after a predefined 
 * time. The timer may be reset by the Master Software (MCPU) through interface commands (Can Bus)
 * 
 * ## Low Speed Activation
 * The Firmware shall implement functions to activated 
 * the external High Speed Starter device at low speed rate
 * 
 * The Firmware shall handle a Timeout to stop the rotation after a predefined 
 * time. The timer may be reset by the Master Software (MCPU) through interface commands (Can Bus)
 * 
 * ## Speed Rate Control
 * The Firmware shall detect the current speed level of the rotating anode
 * 
 * During any exposure sequence the exposure shall not start if the 
 * expected speed rate shouldn't be detected. 
 * 
 * ## Diagnostic Monitoring
 * The Firmware shall implement functions to handle 
 * fault conditions signaled by the High Speed Starter Device.
 * 
 * ## Service Test  
 * The Firmware shall implement functions let the Master Software (MCPU) 
 * to directly activate the Starter device with Interface command (CAN Bus)
 * for test purpose.
 * 
 * ## CAN Interface Commands
 * The Firmware shall implement command and data register to 
 * directly control the Starter device activation and its behavior.
 * 
 * # Exposure Sequences
 * 
 * The firmware shall implement the main exposure sequences
 * as follow listed:
 * + The Standard Manual 2D exposure
 * + The AEC guided 2D exposure
 * + The Standard Tomo exposure
 * + The AEC guided Tomo exposure
 * 
 * ## High Voltage Control
 * 
 * ## External Synchronization with Detector devices
 * 
 * ## External Synchronization with Grid devices
 * 
 * ## External synchronization with Filament device
 * 
 * ## Internal mAsmeter activation
 * 
 * ## Internal Starter device control
 * 
 * ## Exposure Manual 2D
 * 
 * ## Exposure AEC - 2D
 * 
 * ## Exposure Tomo manual
 * 
 * ## Exposure AEC - Tomo
 * 
 * ## Diagnostic
 * 
 * ## CAN Bus Interface
 * 
 * 
 * 
 * 
 */

/*!
 * \defgroup  descriptionGroup Firmware Requirements Description
 * 
 */

/*!
 * \defgroup  implementationGroup Firmware Module Implementation
 * 
 */


/*!
 * \mainpage
 *
 * # Overview
 *
 * This Firmware implements the functions of the PCB/25-349 following the 
 * firmware requirement specifications.
 * 
 * + \ref specificationGroup
 * + \ref descriptionGroup
 * + \ref implementationGroup
 * 
 */



#ifndef _APPLICATION_H    /* Guard against multiple inclusion */
#define _APPLICATION_H

// Defines the Application revision code

/** \addtogroup applicationConstants
 *  @{
*/

// Application Revision 
static const unsigned char  APPLICATION_MAJ_REV =  1 ;  //!< Revision Major Number
static const unsigned char  APPLICATION_MIN_REV =  0 ;  //!< Revision Minor Number
static const unsigned char  APPLICATION_SUB_REV =  1 ;  //!< Revision build Number

/** @}*/
//___________________ SELEZIONE MODULI _____________________
#define CAN_BUS_ACTIVATION

//___________________ SELEZIONE FUNZIONAMENTO _____________________
//#define TEST_SWITCH
//#define TEST_REFERENCE 
#define RUN_CONST_CURRENT   // Corrente Fissa
//#define RUN_CONST_VOLTAGE // Regolazione tensione di uscita

#define MOSFET_SWITCH_ENABLED // Attivazione Switching Mosfets

//___________________ SELEZIONE FASE _____________________
//#define GET_FASE_FROM_VAC
#define GET_FASE_FROM_SIN
//#define GET_FASE_FROM_CONST 1

//___________________ IMPOSTAZIONE DAC _____________________
#define SENS_VREF 2.5       // Volt
#define SENS_ISENSE 100     // mV/A
#define DAC_REFERENCE 3.3   // Impostazione uC-DAC

#define MAX_SENS_CURRENT (SENS_VREF*1000/SENS_ISENSE)
#define DAC(I) (unsigned short) (I * SENS_ISENSE * 65.535 / DAC_REFERENCE)
 
//___________________ PERFORMANCES _____________________
#define VAC_INPUT (220*1.4)    
#define VAC_THRESHOLD 5

#define AC_SMP_TIME_us 120
#define AC_FREQ 50
#define SIN_CORRECT_FASE_us 650

// target di tensione e corrente
#define TARGET_VOLTAGE 560
#define MAX_CURRENT 20

#define MIN_VACIN (VAC_INPUT*0.7)
#define ALARM_VAC_TMO 200 // 15ms unit alarm vac reset time
#endif 