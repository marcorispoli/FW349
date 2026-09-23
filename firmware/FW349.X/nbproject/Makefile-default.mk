#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FW349.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FW349.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../fw_pcb335/firmware/src/Shared/CAN/MET_can_protocol.c ../../../fw_pcb335/firmware/src/Protocol/protocol.c ../../../fw_pcb335/firmware/src/config/default/peripheral/can/plib_can0.c ../../../fw_pcb335/firmware/src/config/default/peripheral/clock/plib_clock.c ../../../fw_pcb335/firmware/src/config/default/peripheral/cmcc/plib_cmcc.c ../../../fw_pcb335/firmware/src/config/default/peripheral/evsys/plib_evsys.c ../../../fw_pcb335/firmware/src/config/default/peripheral/nvic/plib_nvic.c ../../../fw_pcb335/firmware/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../../../fw_pcb335/firmware/src/config/default/peripheral/port/plib_port.c ../../../fw_pcb335/firmware/src/config/default/peripheral/rtc/plib_rtc_timer.c ../../../fw_pcb335/firmware/src/config/default/stdio/xc32_monitor.c ../../../fw_pcb335/firmware/src/config/default/exceptions.c ../../../fw_pcb335/firmware/src/config/default/interrupts.c ../../../fw_pcb335/firmware/src/config/default/startup_xc32.c ../../../fw_pcb335/firmware/src/config/default/initialization.c ../../../fw_pcb335/firmware/src/config/default/libc_syscalls.c ../../../fw_pcb335/firmware/src/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/66427685/MET_can_protocol.o ${OBJECTDIR}/_ext/1005252179/protocol.o ${OBJECTDIR}/_ext/1442699235/plib_can0.o ${OBJECTDIR}/_ext/840139935/plib_clock.o ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o ${OBJECTDIR}/_ext/837990449/plib_evsys.o ${OBJECTDIR}/_ext/1774351151/plib_nvic.o ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o ${OBJECTDIR}/_ext/1774404302/plib_port.o ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o ${OBJECTDIR}/_ext/863045469/xc32_monitor.o ${OBJECTDIR}/_ext/529827753/exceptions.o ${OBJECTDIR}/_ext/529827753/interrupts.o ${OBJECTDIR}/_ext/529827753/startup_xc32.o ${OBJECTDIR}/_ext/529827753/initialization.o ${OBJECTDIR}/_ext/529827753/libc_syscalls.o ${OBJECTDIR}/_ext/565408902/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/66427685/MET_can_protocol.o.d ${OBJECTDIR}/_ext/1005252179/protocol.o.d ${OBJECTDIR}/_ext/1442699235/plib_can0.o.d ${OBJECTDIR}/_ext/840139935/plib_clock.o.d ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o.d ${OBJECTDIR}/_ext/837990449/plib_evsys.o.d ${OBJECTDIR}/_ext/1774351151/plib_nvic.o.d ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/1774404302/plib_port.o.d ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o.d ${OBJECTDIR}/_ext/863045469/xc32_monitor.o.d ${OBJECTDIR}/_ext/529827753/exceptions.o.d ${OBJECTDIR}/_ext/529827753/interrupts.o.d ${OBJECTDIR}/_ext/529827753/startup_xc32.o.d ${OBJECTDIR}/_ext/529827753/initialization.o.d ${OBJECTDIR}/_ext/529827753/libc_syscalls.o.d ${OBJECTDIR}/_ext/565408902/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/66427685/MET_can_protocol.o ${OBJECTDIR}/_ext/1005252179/protocol.o ${OBJECTDIR}/_ext/1442699235/plib_can0.o ${OBJECTDIR}/_ext/840139935/plib_clock.o ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o ${OBJECTDIR}/_ext/837990449/plib_evsys.o ${OBJECTDIR}/_ext/1774351151/plib_nvic.o ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o ${OBJECTDIR}/_ext/1774404302/plib_port.o ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o ${OBJECTDIR}/_ext/863045469/xc32_monitor.o ${OBJECTDIR}/_ext/529827753/exceptions.o ${OBJECTDIR}/_ext/529827753/interrupts.o ${OBJECTDIR}/_ext/529827753/startup_xc32.o ${OBJECTDIR}/_ext/529827753/initialization.o ${OBJECTDIR}/_ext/529827753/libc_syscalls.o ${OBJECTDIR}/_ext/565408902/main.o

# Source Files
SOURCEFILES=../../../fw_pcb335/firmware/src/Shared/CAN/MET_can_protocol.c ../../../fw_pcb335/firmware/src/Protocol/protocol.c ../../../fw_pcb335/firmware/src/config/default/peripheral/can/plib_can0.c ../../../fw_pcb335/firmware/src/config/default/peripheral/clock/plib_clock.c ../../../fw_pcb335/firmware/src/config/default/peripheral/cmcc/plib_cmcc.c ../../../fw_pcb335/firmware/src/config/default/peripheral/evsys/plib_evsys.c ../../../fw_pcb335/firmware/src/config/default/peripheral/nvic/plib_nvic.c ../../../fw_pcb335/firmware/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../../../fw_pcb335/firmware/src/config/default/peripheral/port/plib_port.c ../../../fw_pcb335/firmware/src/config/default/peripheral/rtc/plib_rtc_timer.c ../../../fw_pcb335/firmware/src/config/default/stdio/xc32_monitor.c ../../../fw_pcb335/firmware/src/config/default/exceptions.c ../../../fw_pcb335/firmware/src/config/default/interrupts.c ../../../fw_pcb335/firmware/src/config/default/startup_xc32.c ../../../fw_pcb335/firmware/src/config/default/initialization.c ../../../fw_pcb335/firmware/src/config/default/libc_syscalls.c ../../../fw_pcb335/firmware/src/main.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/FW349.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAME51J20A
MP_LINKER_FILE_OPTION=,--script="..\..\..\fw_pcb335\firmware\src\config\default\ATSAME51J20A.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/66427685/MET_can_protocol.o: ../../../fw_pcb335/firmware/src/Shared/CAN/MET_can_protocol.c  .generated_files/flags/default/21010e3524656f6a53ee71700336d28a611d5f6d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/66427685" 
	@${RM} ${OBJECTDIR}/_ext/66427685/MET_can_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/66427685/MET_can_protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/66427685/MET_can_protocol.o.d" -o ${OBJECTDIR}/_ext/66427685/MET_can_protocol.o ../../../fw_pcb335/firmware/src/Shared/CAN/MET_can_protocol.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1005252179/protocol.o: ../../../fw_pcb335/firmware/src/Protocol/protocol.c  .generated_files/flags/default/84a67f250ce5a253074ea92cb855096b1dd642e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1005252179" 
	@${RM} ${OBJECTDIR}/_ext/1005252179/protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1005252179/protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1005252179/protocol.o.d" -o ${OBJECTDIR}/_ext/1005252179/protocol.o ../../../fw_pcb335/firmware/src/Protocol/protocol.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1442699235/plib_can0.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/can/plib_can0.c  .generated_files/flags/default/219fb2d897c4fcbbed95e7f962ab2e14ad7c852d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1442699235" 
	@${RM} ${OBJECTDIR}/_ext/1442699235/plib_can0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1442699235/plib_can0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1442699235/plib_can0.o.d" -o ${OBJECTDIR}/_ext/1442699235/plib_can0.o ../../../fw_pcb335/firmware/src/config/default/peripheral/can/plib_can0.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/840139935/plib_clock.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/clock/plib_clock.c  .generated_files/flags/default/25370c21898124e9c19ff23f4b7a3a8552057645 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/840139935" 
	@${RM} ${OBJECTDIR}/_ext/840139935/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/840139935/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/840139935/plib_clock.o.d" -o ${OBJECTDIR}/_ext/840139935/plib_clock.o ../../../fw_pcb335/firmware/src/config/default/peripheral/clock/plib_clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1774014615/plib_cmcc.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/default/47c4ee27fd9edad174cff67bce90d1d7e07cc339 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1774014615" 
	@${RM} ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1774014615/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o ../../../fw_pcb335/firmware/src/config/default/peripheral/cmcc/plib_cmcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/837990449/plib_evsys.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/evsys/plib_evsys.c  .generated_files/flags/default/5635c486e0500b05a8ce8a74d5255f403f1a1286 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/837990449" 
	@${RM} ${OBJECTDIR}/_ext/837990449/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/837990449/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/837990449/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/837990449/plib_evsys.o ../../../fw_pcb335/firmware/src/config/default/peripheral/evsys/plib_evsys.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1774351151/plib_nvic.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/nvic/plib_nvic.c  .generated_files/flags/default/7c516d45633ecb71a4319378f56bc532c85933cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1774351151" 
	@${RM} ${OBJECTDIR}/_ext/1774351151/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1774351151/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1774351151/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1774351151/plib_nvic.o ../../../fw_pcb335/firmware/src/config/default/peripheral/nvic/plib_nvic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/default/74e916b3364fd8bbb1a18b6fd1468c0a09f8ab88 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1536436771" 
	@${RM} ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o ../../../fw_pcb335/firmware/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1774404302/plib_port.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/port/plib_port.c  .generated_files/flags/default/203d29e8115b8f150ac167770ce76b02885c9d26 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1774404302" 
	@${RM} ${OBJECTDIR}/_ext/1774404302/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1774404302/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1774404302/plib_port.o.d" -o ${OBJECTDIR}/_ext/1774404302/plib_port.o ../../../fw_pcb335/firmware/src/config/default/peripheral/port/plib_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/rtc/plib_rtc_timer.c  .generated_files/flags/default/9658a72aea1740b0415bd9e6c7c65da300129872 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1442714228" 
	@${RM} ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o.d" -o ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o ../../../fw_pcb335/firmware/src/config/default/peripheral/rtc/plib_rtc_timer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/863045469/xc32_monitor.o: ../../../fw_pcb335/firmware/src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/619433e7d1edbad7330377688ba56c981a0cf533 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/863045469" 
	@${RM} ${OBJECTDIR}/_ext/863045469/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/863045469/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/863045469/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/863045469/xc32_monitor.o ../../../fw_pcb335/firmware/src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/exceptions.o: ../../../fw_pcb335/firmware/src/config/default/exceptions.c  .generated_files/flags/default/1f604325d9ec979b19da23b7372165c356047bf3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/exceptions.o.d" -o ${OBJECTDIR}/_ext/529827753/exceptions.o ../../../fw_pcb335/firmware/src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/interrupts.o: ../../../fw_pcb335/firmware/src/config/default/interrupts.c  .generated_files/flags/default/51608f4e9c6f32f79ceebe9c5e6d2e187c13210c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/interrupts.o.d" -o ${OBJECTDIR}/_ext/529827753/interrupts.o ../../../fw_pcb335/firmware/src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/startup_xc32.o: ../../../fw_pcb335/firmware/src/config/default/startup_xc32.c  .generated_files/flags/default/f0bfbee053991ffb64308593b7c14cf9c4e71c88 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/529827753/startup_xc32.o ../../../fw_pcb335/firmware/src/config/default/startup_xc32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/initialization.o: ../../../fw_pcb335/firmware/src/config/default/initialization.c  .generated_files/flags/default/160ff4212f42f81f45c966d241b403680641c9cf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/initialization.o.d" -o ${OBJECTDIR}/_ext/529827753/initialization.o ../../../fw_pcb335/firmware/src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/libc_syscalls.o: ../../../fw_pcb335/firmware/src/config/default/libc_syscalls.c  .generated_files/flags/default/9da7d9b481e478072860aa1d6b911805f0e49272 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/529827753/libc_syscalls.o ../../../fw_pcb335/firmware/src/config/default/libc_syscalls.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/565408902/main.o: ../../../fw_pcb335/firmware/src/main.c  .generated_files/flags/default/60cc6f5fd81395456337bbe8a1c9f5e014681113 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/565408902" 
	@${RM} ${OBJECTDIR}/_ext/565408902/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/565408902/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/565408902/main.o.d" -o ${OBJECTDIR}/_ext/565408902/main.o ../../../fw_pcb335/firmware/src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/66427685/MET_can_protocol.o: ../../../fw_pcb335/firmware/src/Shared/CAN/MET_can_protocol.c  .generated_files/flags/default/e2593854e7c844ad8f692c47dbd34230344b52c2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/66427685" 
	@${RM} ${OBJECTDIR}/_ext/66427685/MET_can_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/66427685/MET_can_protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/66427685/MET_can_protocol.o.d" -o ${OBJECTDIR}/_ext/66427685/MET_can_protocol.o ../../../fw_pcb335/firmware/src/Shared/CAN/MET_can_protocol.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1005252179/protocol.o: ../../../fw_pcb335/firmware/src/Protocol/protocol.c  .generated_files/flags/default/965cb0368fc9ab3650199d14ce86c92df72c8912 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1005252179" 
	@${RM} ${OBJECTDIR}/_ext/1005252179/protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1005252179/protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1005252179/protocol.o.d" -o ${OBJECTDIR}/_ext/1005252179/protocol.o ../../../fw_pcb335/firmware/src/Protocol/protocol.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1442699235/plib_can0.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/can/plib_can0.c  .generated_files/flags/default/7d5f5a470a54dbd27acf4b90483a23583e0aa0bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1442699235" 
	@${RM} ${OBJECTDIR}/_ext/1442699235/plib_can0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1442699235/plib_can0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1442699235/plib_can0.o.d" -o ${OBJECTDIR}/_ext/1442699235/plib_can0.o ../../../fw_pcb335/firmware/src/config/default/peripheral/can/plib_can0.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/840139935/plib_clock.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/clock/plib_clock.c  .generated_files/flags/default/e7d553df0e3d22ba000550f845b0eae4fe3a6c4b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/840139935" 
	@${RM} ${OBJECTDIR}/_ext/840139935/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/840139935/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/840139935/plib_clock.o.d" -o ${OBJECTDIR}/_ext/840139935/plib_clock.o ../../../fw_pcb335/firmware/src/config/default/peripheral/clock/plib_clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1774014615/plib_cmcc.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/default/45612db6f631622a5df8963535ab51997bfc563a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1774014615" 
	@${RM} ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1774014615/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1774014615/plib_cmcc.o ../../../fw_pcb335/firmware/src/config/default/peripheral/cmcc/plib_cmcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/837990449/plib_evsys.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/evsys/plib_evsys.c  .generated_files/flags/default/c67162d023ce1d3f36ff6161a06d9e57975561e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/837990449" 
	@${RM} ${OBJECTDIR}/_ext/837990449/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/837990449/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/837990449/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/837990449/plib_evsys.o ../../../fw_pcb335/firmware/src/config/default/peripheral/evsys/plib_evsys.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1774351151/plib_nvic.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/nvic/plib_nvic.c  .generated_files/flags/default/8f1e23446d030109c97ba5d8033d5b4fd28ab49 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1774351151" 
	@${RM} ${OBJECTDIR}/_ext/1774351151/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1774351151/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1774351151/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1774351151/plib_nvic.o ../../../fw_pcb335/firmware/src/config/default/peripheral/nvic/plib_nvic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/default/c1db7cbfea18e7e19c3aee863a0f9b3566d91b78 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1536436771" 
	@${RM} ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1536436771/plib_nvmctrl.o ../../../fw_pcb335/firmware/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1774404302/plib_port.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/port/plib_port.c  .generated_files/flags/default/ccf5922fa4c4e671f17fcc089cf8dfc91b9b90f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1774404302" 
	@${RM} ${OBJECTDIR}/_ext/1774404302/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1774404302/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1774404302/plib_port.o.d" -o ${OBJECTDIR}/_ext/1774404302/plib_port.o ../../../fw_pcb335/firmware/src/config/default/peripheral/port/plib_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o: ../../../fw_pcb335/firmware/src/config/default/peripheral/rtc/plib_rtc_timer.c  .generated_files/flags/default/ab688864482eb7340352c48f662d792071283ff1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1442714228" 
	@${RM} ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o.d" -o ${OBJECTDIR}/_ext/1442714228/plib_rtc_timer.o ../../../fw_pcb335/firmware/src/config/default/peripheral/rtc/plib_rtc_timer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/863045469/xc32_monitor.o: ../../../fw_pcb335/firmware/src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/44520e8ed8933dd8f418978b3facc0d798a9a638 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/863045469" 
	@${RM} ${OBJECTDIR}/_ext/863045469/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/863045469/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/863045469/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/863045469/xc32_monitor.o ../../../fw_pcb335/firmware/src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/exceptions.o: ../../../fw_pcb335/firmware/src/config/default/exceptions.c  .generated_files/flags/default/986e064ca10994fb74df3e93cabdc94d7a949601 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/exceptions.o.d" -o ${OBJECTDIR}/_ext/529827753/exceptions.o ../../../fw_pcb335/firmware/src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/interrupts.o: ../../../fw_pcb335/firmware/src/config/default/interrupts.c  .generated_files/flags/default/fc2a70addf247a2d005521353751e3bd6d6ab598 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/interrupts.o.d" -o ${OBJECTDIR}/_ext/529827753/interrupts.o ../../../fw_pcb335/firmware/src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/startup_xc32.o: ../../../fw_pcb335/firmware/src/config/default/startup_xc32.c  .generated_files/flags/default/19faf6834f8e417cb131f8ecb91cdbcb31ee19d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/529827753/startup_xc32.o ../../../fw_pcb335/firmware/src/config/default/startup_xc32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/initialization.o: ../../../fw_pcb335/firmware/src/config/default/initialization.c  .generated_files/flags/default/c3386658a4b39298d3de5820315b7b7760cf0d3a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/initialization.o.d" -o ${OBJECTDIR}/_ext/529827753/initialization.o ../../../fw_pcb335/firmware/src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/529827753/libc_syscalls.o: ../../../fw_pcb335/firmware/src/config/default/libc_syscalls.c  .generated_files/flags/default/20113a48bf060f2e94a9f8813826c9a5de20dd21 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/529827753" 
	@${RM} ${OBJECTDIR}/_ext/529827753/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/529827753/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/529827753/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/529827753/libc_syscalls.o ../../../fw_pcb335/firmware/src/config/default/libc_syscalls.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/565408902/main.o: ../../../fw_pcb335/firmware/src/main.c  .generated_files/flags/default/aac7aca1b65288e556b3e2ef7cd11ed662a36843 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/565408902" 
	@${RM} ${OBJECTDIR}/_ext/565408902/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/565408902/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/default" -I"../src/packs/ATSAME51J20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/565408902/main.o.d" -o ${OBJECTDIR}/_ext/565408902/main.o ../../../fw_pcb335/firmware/src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/FW349.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../../fw_pcb335/firmware/src/config/default/ATSAME51J20A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-device-startup-code -o ${DISTDIR}/FW349.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-DROM_LENGTH=0x100000,-DROM_ORIGIN=0x0,--memorysummary,${DISTDIR}/memoryfile.xml,-DRAM_ORIGIN=0x20000010,-DRAM_LENGTH=0x3FFF0 -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/FW349.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../../fw_pcb335/firmware/src/config/default/ATSAME51J20A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-device-startup-code -o ${DISTDIR}/FW349.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-DROM_LENGTH=0x100000,-DROM_ORIGIN=0x0,--memorysummary,${DISTDIR}/memoryfile.xml,-DRAM_ORIGIN=0x20000010,-DRAM_LENGTH=0x3FFF0 -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/FW349.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
