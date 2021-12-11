
;CodeVisionAVR C Compiler V2.05.3 Standard
;(C) Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega128
;Program type             : Application
;Clock frequency          : 16.000000 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 1024 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;Global 'const' stored in FLASH     : No
;Enhanced function parameter passing: Yes
;Enhanced core instructions         : On
;Smart register allocation          : On
;Automatic register allocation      : On

	#pragma AVRPART ADMIN PART_NAME ATmega128
	#pragma AVRPART MEMORY PROG_FLASH 131072
	#pragma AVRPART MEMORY EEPROM 4096
	#pragma AVRPART MEMORY INT_SRAM SIZE 4351
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU RAMPZ=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU XMCRA=0x6D
	.EQU XMCRB=0x6C

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0100
	.EQU __SRAM_END=0x10FF
	.EQU __DSTACK_SIZE=0x0400
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __GETD1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X+
	LD   R22,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _EnkoderKi=R4
	.DEF _EnkoderKa=R6
	.DEF _SpeedKi=R9
	.DEF _SpeedKa=R8
	.DEF _dSpeedKi=R11
	.DEF _dSpeedKa=R10
	.DEF _dCounterKi=R12

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _ext_int6_isr
	JMP  _ext_int7_isr
	JMP  _timer2_comp_isr
	JMP  0x00
	JMP  _timer1_capt_isr
	JMP  0x00
	JMP  0x00
	JMP  _timer1_ovf_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _usart1_rx_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

;REGISTER BIT VARIABLES INITIALIZATION
__REG_BIT_VARS:
	.DW  0x0000

_0x74:
	.DB  0xA
_0x0:
	.DB  0x70,0x6C,0x73,0x2F,0x31,0x30,0x6D,0x73
	.DB  0x2E,0x4B,0x61,0x3A,0x0,0x72,0x70,0x6D
	.DB  0x20,0x6B,0x61,0x3A,0x0,0x20,0x52,0x50
	.DB  0x4D,0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  0x02
	.DW  __REG_BIT_VARS*2

	.DW  0x01
	.DW  _Timer2Tick_S000001C000
	.DW  _0x74*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30
	STS  XMCRB,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

	OUT  RAMPZ,R24

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x500

	.CSEG
;/*****************************************************
;This program was produced by the
;CodeWizardAVR V2.05.3 Standard
;Automatic Program Generator
;© Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com
;
;Project :
;Version :
;Date    : 10/2/2019
;Author  : unknown
;Company :
;Comments:
;
;
;Chip type               : ATmega128
;Program type            : Application
;AVR Core Clock frequency: 16.000000 MHz
;Memory model            : Small
;External RAM size       : 0
;Data Stack size         : 1024
;*****************************************************/
;
;#include <mega128.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif
;#include <delay.h>
;#include <adroit.h>
;#include <adroit_LCD.h>

	.CSEG
_LCD_Init_Cmd:
	ST   -Y,R26
;	xData -> Y+0
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	ORI  R30,4
	RCALL SUBOPT_0x0
	RJMP _0x2000005
_LCD_Perintah:
	ST   -Y,R26
;	xData -> Y+0
	CBI  0x1B,0
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	ORI  R30,LOW(0xC)
	RCALL SUBOPT_0x0
	RCALL SUBOPT_0x1
	ORI  R30,LOW(0xC)
	RCALL SUBOPT_0x0
	__DELAY_USW 2800
	RJMP _0x2000004
_LCD_Data:
	ST   -Y,R26
;	xData -> Y+0
	SBI  0x1B,0
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	ORI  R30,LOW(0xD)
	RCALL SUBOPT_0x0
	RCALL SUBOPT_0x1
	ORI  R30,LOW(0xD)
	RCALL SUBOPT_0x0
	__DELAY_USB 213
_0x2000004:
	SBI  0x1B,2
_0x2000005:
	ADIW R28,1
	RET
_LCD_Init:
	LDI  R26,LOW(50)
	RCALL SUBOPT_0x2
	LDI  R26,LOW(5)
	RCALL SUBOPT_0x2
	LDI  R26,LOW(1)
	RCALL SUBOPT_0x2
	LDI  R26,LOW(1)
	LDI  R27,0
	CALL _delay_ms
	LDI  R26,LOW(32)
	RCALL _LCD_Init_Cmd
	LDI  R26,LOW(40)
	RCALL _LCD_Perintah
	LDI  R26,LOW(16)
	RCALL _LCD_Perintah
	LDI  R26,LOW(12)
	RCALL _LCD_Perintah
	LDI  R26,LOW(6)
	RCALL _LCD_Perintah
	LDI  R26,LOW(1)
	RCALL _LCD_Perintah
	RET
_LCD_GotoXY:
	ST   -Y,R26
	ST   -Y,R17
;	x -> Y+2
;	y -> Y+1
;	baris -> R17
	LDD  R30,Y+1
	CPI  R30,0
	BRNE _0x27
	LDD  R26,Y+2
	SUBI R26,-LOW(128)
	RJMP _0x87
_0x27:
	LDD  R26,Y+2
	SUBI R26,-LOW(192)
_0x87:
	RCALL _LCD_Perintah
	LDD  R17,Y+0
	RJMP _0x2000003
_LCD_TextF:
	ST   -Y,R27
	ST   -Y,R26
;	*text -> Y+0
_0x29:
	LD   R30,Y
	LDD  R31,Y+1
	LPM  R30,Z
	CPI  R30,0
	BREQ _0x2B
	LD   R30,Y
	LDD  R31,Y+1
	LPM  R26,Z
	RCALL _LCD_Data
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,1
	ST   Y,R30
	STD  Y+1,R31
	RJMP _0x29
_0x2B:
	RJMP _0x2000002
;	*text -> Y+0
_LCD_TulisF:
	ST   -Y,R27
	ST   -Y,R26
;	Baris -> Y+2
;	*text -> Y+0
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDD  R26,Y+3
	RCALL _LCD_GotoXY
	LD   R26,Y
	LDD  R27,Y+1
	RCALL _LCD_TextF
_0x2000003:
	ADIW R28,3
	RET
;	Baris -> Y+1
;	i -> R17
;	Baris -> Y+1
;	i -> R17
;	Baris -> Y+2
;	i -> R17
;	j -> R16
;	Baris -> Y+3
;	*text -> Y+1
;	i -> R17
;	Baris -> Y+4
;	*text -> Y+2
;	i -> R17
;	j -> R16
;	Baris -> Y+4
;	*text -> Y+2
;	i -> R17
;	j -> R16
;	Baris -> Y+4
;	*text -> Y+2
;	i -> R17
;	j -> R16
_LCD_Angka4:
	ST   -Y,R27
	ST   -Y,R26
;	x -> Y+0
	LDD  R26,Y+1
	TST  R26
	BRPL _0x44
	RCALL SUBOPT_0x3
_0x44:
	LD   R26,Y
	LDD  R27,Y+1
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	RCALL SUBOPT_0x4
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	CALL __MODW21
	MOVW R26,R30
	RJMP _0x2000001
_LCD_Angka3:
	ST   -Y,R27
	ST   -Y,R26
;	x -> Y+0
	LDD  R26,Y+1
	TST  R26
	BRPL _0x45
	RCALL SUBOPT_0x3
_0x45:
	LD   R26,Y
	LDD  R27,Y+1
_0x2000001:
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	RCALL SUBOPT_0x4
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	CALL __MODW21
	MOVW R26,R30
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL SUBOPT_0x4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MODW21
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _LCD_Data
_0x2000002:
	ADIW R28,2
	RET
;	x -> Y+0
;	x -> Y+0
;	x -> Y+1
;	i -> R17
;
;#define SetGlobalInterrupt        #asm("sei")
;#define ClrGlobalInterrupt        #asm("cli")
;int EnkoderKi, EnkoderKa;	   	// variable data enkoder motor kiri & kanan
;unsigned char SpeedKi, SpeedKa; 	// set point kecepatan	(PID)
;unsigned char dSpeedKi, dSpeedKa; 	// kecepatan aktual (sampling dari enkoder)
;unsigned int dCounterKi, dCounterKa; // data counter untuk rotary enkoder
;
;// External Interrupt 6 service routine
;interrupt [EXT_INT6] void ext_int6_isr(void)
; 0000 0026 {
_ext_int6_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0027 // Place your code here
; 0000 0028   if(P_Enkoder1A)
	SBIS 0x1,6
	RJMP _0x4D
	LDI  R30,LOW(1)
	RJMP _0x4E
_0x4D:
	LDI  R30,LOW(0)
_0x4E:
	CPI  R30,0
	BREQ _0x4C
; 0000 0029   {  if(P_Enkoder1B) {ClrGlobalInterrupt;   EnkoderKi--;}
	SBIS 0x16,0
	RJMP _0x51
	LDI  R30,LOW(1)
	RJMP _0x52
_0x51:
	LDI  R30,LOW(0)
_0x52:
	CPI  R30,0
	BREQ _0x50
	cli
	MOVW R30,R4
	SBIW R30,1
	RJMP _0x89
; 0000 002A      else            {ClrGlobalInterrupt;   EnkoderKi++;}
_0x50:
	cli
	MOVW R30,R4
	ADIW R30,1
_0x89:
	MOVW R4,R30
; 0000 002B   }
; 0000 002C   else
	RJMP _0x55
_0x4C:
; 0000 002D   {  if(P_Enkoder1B) {ClrGlobalInterrupt;   EnkoderKi++;}
	SBIS 0x16,0
	RJMP _0x57
	LDI  R30,LOW(1)
	RJMP _0x58
_0x57:
	LDI  R30,LOW(0)
_0x58:
	CPI  R30,0
	BREQ _0x56
	cli
	MOVW R30,R4
	ADIW R30,1
	RJMP _0x8A
; 0000 002E      else            {ClrGlobalInterrupt;   EnkoderKi--;}
_0x56:
	cli
	MOVW R30,R4
	SBIW R30,1
_0x8A:
	MOVW R4,R30
; 0000 002F   }
_0x55:
; 0000 0030   SetGlobalInterrupt;
	sei
; 0000 0031   dCounterKi++;
	MOVW R30,R12
	ADIW R30,1
	MOVW R12,R30
; 0000 0032 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;
;// External Interrupt 7 service routine
;interrupt [EXT_INT7] void ext_int7_isr(void)
; 0000 0036 {
_ext_int7_isr:
	RCALL SUBOPT_0x5
; 0000 0037 // Place your code here
; 0000 0038   if(P_Enkoder2A)
	SBIS 0x1,7
	RJMP _0x5C
	LDI  R30,LOW(1)
	RJMP _0x5D
_0x5C:
	LDI  R30,LOW(0)
_0x5D:
	CPI  R30,0
	BREQ _0x5B
; 0000 0039   {  if(P_Enkoder2B) {ClrGlobalInterrupt;   EnkoderKa++;}
	SBIS 0x16,2
	RJMP _0x60
	LDI  R30,LOW(1)
	RJMP _0x61
_0x60:
	LDI  R30,LOW(0)
_0x61:
	CPI  R30,0
	BREQ _0x5F
	cli
	MOVW R30,R6
	ADIW R30,1
	RJMP _0x8B
; 0000 003A      else            {ClrGlobalInterrupt;   EnkoderKa--;}
_0x5F:
	cli
	MOVW R30,R6
	SBIW R30,1
_0x8B:
	MOVW R6,R30
; 0000 003B   }
; 0000 003C   else
	RJMP _0x64
_0x5B:
; 0000 003D   {  if(P_Enkoder2B) {ClrGlobalInterrupt;   EnkoderKa--;}
	SBIS 0x16,2
	RJMP _0x66
	LDI  R30,LOW(1)
	RJMP _0x67
_0x66:
	LDI  R30,LOW(0)
_0x67:
	CPI  R30,0
	BREQ _0x65
	cli
	MOVW R30,R6
	SBIW R30,1
	RJMP _0x8C
; 0000 003E      else            {ClrGlobalInterrupt;   EnkoderKa++;}
_0x65:
	cli
	MOVW R30,R6
	ADIW R30,1
_0x8C:
	MOVW R6,R30
; 0000 003F   }
_0x64:
; 0000 0040   SetGlobalInterrupt;
	sei
; 0000 0041   dCounterKa++;
	LDI  R26,LOW(_dCounterKa)
	LDI  R27,HIGH(_dCounterKa)
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
; 0000 0042 }
	RJMP _0x8F
;
;#ifndef RXB8
;#define RXB8 1
;#endif
;
;#ifndef TXB8
;#define TXB8 0
;#endif
;
;#ifndef UPE
;#define UPE 2
;#endif
;
;#ifndef DOR
;#define DOR 3
;#endif
;
;#ifndef FE
;#define FE 4
;#endif
;
;#ifndef UDRE
;#define UDRE 5
;#endif
;
;#ifndef RXC
;#define RXC 7
;#endif
;
;#define FRAMING_ERROR (1<<FE)
;#define PARITY_ERROR (1<<UPE)
;#define DATA_OVERRUN (1<<DOR)
;#define DATA_REGISTER_EMPTY (1<<UDRE)
;#define RX_COMPLETE (1<<RXC)
;
;// USART1 Receiver buffer
;#define RX_BUFFER_SIZE1 8
;char rx_buffer1[RX_BUFFER_SIZE1];
;
;#if RX_BUFFER_SIZE1 <= 256
;unsigned char rx_wr_index1,rx_rd_index1,rx_counter1;
;#else
;unsigned int rx_wr_index1,rx_rd_index1,rx_counter1;
;#endif
;
;// This flag is set on USART1 Receiver buffer overflow
;bit rx_buffer_overflow1;
;
;// USART1 Receiver interrupt service routine
;interrupt [USART1_RXC] void usart1_rx_isr(void)
; 0000 0075 {
_usart1_rx_isr:
	ST   -Y,R26
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0076 char status,data;
; 0000 0077 status=UCSR1A;
	ST   -Y,R17
	ST   -Y,R16
;	status -> R17
;	data -> R16
	LDS  R17,155
; 0000 0078 data=UDR1;
	LDS  R16,156
; 0000 0079 if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x6A
; 0000 007A    {
; 0000 007B    rx_buffer1[rx_wr_index1++]=data;
	LDS  R30,_rx_wr_index1
	SUBI R30,-LOW(1)
	STS  _rx_wr_index1,R30
	SUBI R30,LOW(1)
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer1)
	SBCI R31,HIGH(-_rx_buffer1)
	ST   Z,R16
; 0000 007C #if RX_BUFFER_SIZE1 == 256
; 0000 007D    // special case for receiver buffer size=256
; 0000 007E    if (++rx_counter1 == 0) rx_buffer_overflow1=1;
; 0000 007F #else
; 0000 0080    if (rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
	LDS  R26,_rx_wr_index1
	CPI  R26,LOW(0x8)
	BRNE _0x6B
	LDI  R30,LOW(0)
	STS  _rx_wr_index1,R30
; 0000 0081    if (++rx_counter1 == RX_BUFFER_SIZE1)
_0x6B:
	LDS  R26,_rx_counter1
	SUBI R26,-LOW(1)
	STS  _rx_counter1,R26
	CPI  R26,LOW(0x8)
	BRNE _0x6C
; 0000 0082       {
; 0000 0083       rx_counter1=0;
	LDI  R30,LOW(0)
	STS  _rx_counter1,R30
; 0000 0084       rx_buffer_overflow1=1;
	SET
	BLD  R2,0
; 0000 0085       }
; 0000 0086 #endif
; 0000 0087    }
_0x6C:
; 0000 0088 }
_0x6A:
	LD   R16,Y+
	LD   R17,Y+
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R26,Y+
	RETI
;
;// Get a character from the USART1 Receiver buffer
;#pragma used+
;char getchar1(void)
; 0000 008D {
; 0000 008E char data;
; 0000 008F while (rx_counter1==0);
;	data -> R17
; 0000 0090 data=rx_buffer1[rx_rd_index1++];
; 0000 0091 #if RX_BUFFER_SIZE1 != 256
; 0000 0092 if (rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
; 0000 0093 #endif
; 0000 0094 #asm("cli")
; 0000 0095 --rx_counter1;
; 0000 0096 #asm("sei")
; 0000 0097 return data;
; 0000 0098 }
;#pragma used-
;// Write a character to the USART1 Transmitter
;#pragma used+
;void putchar1(char c)
; 0000 009D {
; 0000 009E while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
;	c -> Y+0
; 0000 009F UDR1=c;
; 0000 00A0 }
;#pragma used-
;
;// Timer1 overflow interrupt service routine
;interrupt [TIM1_OVF] void timer1_ovf_isr(void)
; 0000 00A5 {
_timer1_ovf_isr:
; 0000 00A6 // Place your code here
; 0000 00A7 
; 0000 00A8 }
	RETI
;
;// Timer1 input capture interrupt service routine
;interrupt [TIM1_CAPT] void timer1_capt_isr(void)
; 0000 00AC {
_timer1_capt_isr:
; 0000 00AD // Place your code here
; 0000 00AE 
; 0000 00AF }
	RETI
;
;// Timer2 output compare interrupt service routine
;interrupt [TIM2_COMP] void timer2_comp_isr(void)
; 0000 00B3 {
_timer2_comp_isr:
	RCALL SUBOPT_0x5
; 0000 00B4 // Place your code here
; 0000 00B5     static unsigned char _dCounterKi=0, _dCounterKa=0, Timer2Tick=10;

	.DSEG

	.CSEG
; 0000 00B6     SetGlobalInterrupt;
	sei
; 0000 00B7     if(--Timer2Tick==0) Timer2Tick=10; // timer 1 ms dicacah 10 x
	LDS  R30,_Timer2Tick_S000001C000
	SUBI R30,LOW(1)
	STS  _Timer2Tick_S000001C000,R30
	CPI  R30,0
	BRNE _0x75
	LDI  R30,LOW(10)
	STS  _Timer2Tick_S000001C000,R30
; 0000 00B8     switch(Timer2Tick)
_0x75:
	LDS  R30,_Timer2Tick_S000001C000
	LDI  R31,0
; 0000 00B9     {   case    9:  //dilakukan perhitungan kecepatan motor kiri saat nilai cacahan=9
	CPI  R30,LOW(0x9)
	LDI  R26,HIGH(0x9)
	CPC  R31,R26
	BRNE _0x79
; 0000 00BA             // hitung selisih saat tidak overflow
; 0000 00BB             if(dCounterKi>_dCounterKi)	dSpeedKi = dCounterKi - _dCounterKi;
	LDS  R30,__dCounterKi_S000001C000
	MOVW R26,R12
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x7A
	LDS  R26,__dCounterKi_S000001C000
	MOV  R30,R12
	SUB  R30,R26
	RJMP _0x8D
; 0000 00BC             // hitung selisih saat overflow
; 0000 00BD             else                    	dSpeedKi = 0xFF - _dCounterKi + dCounterKi + 1;
_0x7A:
	LDS  R26,__dCounterKi_S000001C000
	LDI  R30,LOW(255)
	SUB  R30,R26
	ADD  R30,R12
	SUBI R30,-LOW(1)
_0x8D:
	MOV  R11,R30
; 0000 00BE             // update data _dCounterKi
; 0000 00BF             _dCounterKi = dCounterKi;
	STS  __dCounterKi_S000001C000,R12
; 0000 00C0             break;
	RJMP _0x78
; 0000 00C1         case    4:  //dilakukan perhitungan kecepatan motor kanan saat nilai cacahan=9
_0x79:
	CPI  R30,LOW(0x4)
	LDI  R26,HIGH(0x4)
	CPC  R31,R26
	BRNE _0x78
; 0000 00C2             // hitung selisih saat tidak overflow
; 0000 00C3             if(dCounterKa>_dCounterKa)	dSpeedKa = dCounterKa - _dCounterKa;
	LDS  R30,__dCounterKa_S000001C000
	LDS  R26,_dCounterKa
	LDS  R27,_dCounterKa+1
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x7D
	LDS  R26,__dCounterKa_S000001C000
	LDS  R30,_dCounterKa
	SUB  R30,R26
	RJMP _0x8E
; 0000 00C4             // hitung selisih saat overflow
; 0000 00C5             else                   	dSpeedKa = 0xFF - _dCounterKa + dCounterKa + 1;
_0x7D:
	LDS  R26,__dCounterKa_S000001C000
	LDI  R30,LOW(255)
	SUB  R30,R26
	LDS  R26,_dCounterKa
	ADD  R30,R26
	SUBI R30,-LOW(1)
_0x8E:
	MOV  R10,R30
; 0000 00C6             // update data _dCounterKa
; 0000 00C7             _dCounterKa = dCounterKa;
	LDS  R30,_dCounterKa
	STS  __dCounterKa_S000001C000,R30
; 0000 00C8             break;
; 0000 00C9     }
_0x78:
; 0000 00CA 
; 0000 00CB }
_0x8F:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	RETI
;
;// Declare your global variables here
;
;void main(void)
; 0000 00D0 {
_main:
; 0000 00D1 // Declare your local variables here
; 0000 00D2 
; 0000 00D3 // Input/Output Ports initialization
; 0000 00D4 // Port A initialization
; 0000 00D5 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
; 0000 00D6 // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
; 0000 00D7 PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
; 0000 00D8 DDRA=0xFF;
	LDI  R30,LOW(255)
	OUT  0x1A,R30
; 0000 00D9 
; 0000 00DA // Port B initialization
; 0000 00DB // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=In Func1=In Func0=In
; 0000 00DC // State7=0 State6=0 State5=0 State4=0 State3=0 State2=P State1=T State0=P
; 0000 00DD PORTB=0x05;
	LDI  R30,LOW(5)
	OUT  0x18,R30
; 0000 00DE DDRB=0xF8;
	LDI  R30,LOW(248)
	OUT  0x17,R30
; 0000 00DF 
; 0000 00E0 // Port C initialization
; 0000 00E1 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00E2 // State7=0 State6=0 State5=0 State4=0 State3=P State2=P State1=P State0=P
; 0000 00E3 PORTC=0x0F;
	LDI  R30,LOW(15)
	OUT  0x15,R30
; 0000 00E4 DDRC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x14,R30
; 0000 00E5 
; 0000 00E6 // Port D initialization
; 0000 00E7 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00E8 // State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=T State0=T
; 0000 00E9 PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
; 0000 00EA DDRD=0xF0;
	LDI  R30,LOW(240)
	OUT  0x11,R30
; 0000 00EB 
; 0000 00EC // Port E initialization
; 0000 00ED // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In
; 0000 00EE // State7=P State6=P State5=0 State4=0 State3=0 State2=0 State1=T State0=T
; 0000 00EF PORTE=0xC0;
	LDI  R30,LOW(192)
	OUT  0x3,R30
; 0000 00F0 DDRE=0x3C;
	LDI  R30,LOW(60)
	OUT  0x2,R30
; 0000 00F1 
; 0000 00F2 // Port F initialization
; 0000 00F3 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00F4 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 00F5 PORTF=0x00;
	LDI  R30,LOW(0)
	STS  98,R30
; 0000 00F6 DDRF=0x00;
	STS  97,R30
; 0000 00F7 
; 0000 00F8 // Port G initialization
; 0000 00F9 // Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00FA // State4=T State3=P State2=P State1=P State0=P
; 0000 00FB PORTG=0x0F;
	LDI  R30,LOW(15)
	STS  101,R30
; 0000 00FC DDRG=0x00;
	LDI  R30,LOW(0)
	STS  100,R30
; 0000 00FD 
; 0000 00FE // Timer/Counter 0 initialization
; 0000 00FF // Clock source: System Clock
; 0000 0100 // Clock value: 250.000 kHz
; 0000 0101 // Mode: CTC top=OCR0
; 0000 0102 // OC0 output: Toggle on compare match
; 0000 0103 ASSR=0x00;
	OUT  0x30,R30
; 0000 0104 TCCR0=0x1C;
	LDI  R30,LOW(28)
	OUT  0x33,R30
; 0000 0105 TCNT0=0x00;
	LDI  R30,LOW(0)
	OUT  0x32,R30
; 0000 0106 OCR0=0x00;
	OUT  0x31,R30
; 0000 0107 
; 0000 0108 // Timer/Counter 1 initialization
; 0000 0109 // Clock source: System Clock
; 0000 010A // Clock value: 2000.000 kHz
; 0000 010B // Mode: Ph. & fr. cor. PWM top=ICR1
; 0000 010C // OC1A output: Non-Inv.
; 0000 010D // OC1B output: Non-Inv.
; 0000 010E // OC1C output: Non-Inv.
; 0000 010F // Noise Canceler: Off
; 0000 0110 // Input Capture on Falling Edge
; 0000 0111 // Timer1 Overflow Interrupt: On
; 0000 0112 // Input Capture Interrupt: On
; 0000 0113 // Compare A Match Interrupt: Off
; 0000 0114 // Compare B Match Interrupt: Off
; 0000 0115 // Compare C Match Interrupt: Off
; 0000 0116 TCCR1A=0xA8;
	LDI  R30,LOW(168)
	OUT  0x2F,R30
; 0000 0117 TCCR1B=0x12;
	LDI  R30,LOW(18)
	OUT  0x2E,R30
; 0000 0118 TCNT1H=0x00;
	LDI  R30,LOW(0)
	OUT  0x2D,R30
; 0000 0119 TCNT1L=0x00;
	OUT  0x2C,R30
; 0000 011A ICR1H=0x13;
	LDI  R30,LOW(19)
	OUT  0x27,R30
; 0000 011B ICR1L=0x87;
	LDI  R30,LOW(135)
	OUT  0x26,R30
; 0000 011C OCR1AH=0x00;
	LDI  R30,LOW(0)
	OUT  0x2B,R30
; 0000 011D OCR1AL=0x00;
	OUT  0x2A,R30
; 0000 011E OCR1BH=0x00;
	OUT  0x29,R30
; 0000 011F OCR1BL=0x00;
	OUT  0x28,R30
; 0000 0120 OCR1CH=0x00;
	STS  121,R30
; 0000 0121 OCR1CL=0x00;
	STS  120,R30
; 0000 0122 
; 0000 0123 // Timer/Counter 2 initialization
; 0000 0124 // Clock source: System Clock
; 0000 0125 // Clock value: 250.000 kHz
; 0000 0126 // Mode: CTC top=OCR2
; 0000 0127 // OC2 output: Disconnected
; 0000 0128 TCCR2=0x0B;
	LDI  R30,LOW(11)
	OUT  0x25,R30
; 0000 0129 TCNT2=0x00;
	LDI  R30,LOW(0)
	OUT  0x24,R30
; 0000 012A OCR2=0xF9;
	LDI  R30,LOW(249)
	OUT  0x23,R30
; 0000 012B 
; 0000 012C // Timer/Counter 3 initialization
; 0000 012D // Clock source: System Clock
; 0000 012E // Clock value: 16000.000 kHz
; 0000 012F // Mode: Ph. correct PWM top=ICR3
; 0000 0130 // OC3A output: Inverted
; 0000 0131 // OC3B output: Inverted
; 0000 0132 // OC3C output: Discon.
; 0000 0133 // Noise Canceler: Off
; 0000 0134 // Input Capture on Falling Edge
; 0000 0135 // Timer3 Overflow Interrupt: Off
; 0000 0136 // Input Capture Interrupt: Off
; 0000 0137 // Compare A Match Interrupt: Off
; 0000 0138 // Compare B Match Interrupt: Off
; 0000 0139 // Compare C Match Interrupt: Off
; 0000 013A TCCR3A=0xF2;
	LDI  R30,LOW(242)
	STS  139,R30
; 0000 013B TCCR3B=0x11;
	LDI  R30,LOW(17)
	STS  138,R30
; 0000 013C TCNT3H=0x00;
	LDI  R30,LOW(0)
	STS  137,R30
; 0000 013D TCNT3L=0x00;
	STS  136,R30
; 0000 013E ICR3H=0x01;
	LDI  R30,LOW(1)
	STS  129,R30
; 0000 013F ICR3L=0x90;
	LDI  R30,LOW(144)
	STS  128,R30
; 0000 0140 OCR3AH=0x00;
	LDI  R30,LOW(0)
	STS  135,R30
; 0000 0141 OCR3AL=0x00;
	RCALL SUBOPT_0x6
; 0000 0142 OCR3BH=0x00;
; 0000 0143 OCR3BL=0x00;
; 0000 0144 OCR3CH=0x00;
	LDI  R30,LOW(0)
	STS  131,R30
; 0000 0145 OCR3CL=0xc8;
	LDI  R30,LOW(200)
	STS  130,R30
; 0000 0146 
; 0000 0147 // External Interrupt(s) initialization
; 0000 0148 // INT0: Off
; 0000 0149 // INT1: Off
; 0000 014A // INT2: Off
; 0000 014B // INT3: Off
; 0000 014C // INT4: Off
; 0000 014D // INT5: Off
; 0000 014E // INT6: On
; 0000 014F // INT6 Mode: Any change
; 0000 0150 // INT7: On
; 0000 0151 // INT7 Mode: Any change
; 0000 0152 EICRA=0x00;
	LDI  R30,LOW(0)
	STS  106,R30
; 0000 0153 EICRB=0x50;
	LDI  R30,LOW(80)
	OUT  0x3A,R30
; 0000 0154 EIMSK=0xC0;
	LDI  R30,LOW(192)
	OUT  0x39,R30
; 0000 0155 EIFR=0xC0;
	OUT  0x38,R30
; 0000 0156 
; 0000 0157 // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 0158 TIMSK=0xA4;
	LDI  R30,LOW(164)
	OUT  0x37,R30
; 0000 0159 
; 0000 015A ETIMSK=0x00;
	LDI  R30,LOW(0)
	STS  125,R30
; 0000 015B 
; 0000 015C // USART0 initialization
; 0000 015D // USART0 disabled
; 0000 015E UCSR0B=0x00;
	OUT  0xA,R30
; 0000 015F 
; 0000 0160 // USART1 initialization
; 0000 0161 // Communication Parameters: 8 Data, 1 Stop, No Parity
; 0000 0162 // USART1 Receiver: On
; 0000 0163 // USART1 Transmitter: On
; 0000 0164 // USART1 Mode: Asynchronous
; 0000 0165 // USART1 Baud Rate: 9600
; 0000 0166 UCSR1A=0x00;
	STS  155,R30
; 0000 0167 UCSR1B=0x98;
	LDI  R30,LOW(152)
	STS  154,R30
; 0000 0168 UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
; 0000 0169 UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
; 0000 016A UBRR1L=0x67;
	LDI  R30,LOW(103)
	STS  153,R30
; 0000 016B 
; 0000 016C // Analog Comparator initialization
; 0000 016D // Analog Comparator: Off
; 0000 016E // Analog Comparator Input Capture by Timer/Counter 1: Off
; 0000 016F ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 0170 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
; 0000 0171 
; 0000 0172 // ADC initialization
; 0000 0173 // ADC disabled
; 0000 0174 ADCSRA=0x00;
	OUT  0x6,R30
; 0000 0175 
; 0000 0176 // SPI initialization
; 0000 0177 // SPI disabled
; 0000 0178 SPCR=0x00;
	OUT  0xD,R30
; 0000 0179 
; 0000 017A // TWI initialization
; 0000 017B // TWI disabled
; 0000 017C TWCR=0x00;
	STS  116,R30
; 0000 017D 
; 0000 017E // Global enable interrupts
; 0000 017F #asm("sei")
	sei
; 0000 0180 
; 0000 0181 // inisialisasi LCD
; 0000 0182 LCD_Init();
	RCALL _LCD_Init
; 0000 0183 PwmMKa(400);    	// 25%
	LDI  R30,LOW(1)
	STS  135,R30
	LDI  R30,LOW(144)
	RCALL SUBOPT_0x6
; 0000 0184 PwmMKi(0);     	// 25%
; 0000 0185 MKi_CCW;
	SBI  0x3,5
; 0000 0186 MKa_CW;
	SBI  0x3,2
; 0000 0187 LCD_TulisF(0,"pls/10ms.Ka:");
	LDI  R30,LOW(0)
	ST   -Y,R30
	__POINTW2FN _0x0,0
	RCALL _LCD_TulisF
; 0000 0188 LCD_TulisF(1,"rpm ka:");
	LDI  R30,LOW(1)
	ST   -Y,R30
	__POINTW2FN _0x0,13
	RCALL _LCD_TulisF
; 0000 0189 
; 0000 018A while (1)
_0x83:
; 0000 018B       {
; 0000 018C       // Place your code here
; 0000 018D       // menampilkan data kecepatan aktual motor Kanan (pulsa/10ms)
; 0000 018E       // pulsa per detik = data * 100
; 0000 018F       // rps encoder = pulsa per detik / 16
; 0000 0190       // rps shaft motor = rps encoder / 17.3
; 0000 0191       // rpm shaft motor = rps shaftmotor * 60
; 0000 0192       LCD_GotoXY(13,0); LCD_Angka3(dSpeedKa); LCD_Data(' ');
	LDI  R30,LOW(13)
	ST   -Y,R30
	LDI  R26,LOW(0)
	RCALL _LCD_GotoXY
	MOV  R26,R10
	CLR  R27
	RCALL _LCD_Angka3
	LDI  R26,LOW(32)
	RCALL _LCD_Data
; 0000 0193       LCD_GotoXY(7,1); LCD_Angka4((((dSpeedKa * 100.0)/16)/17.3)*60); LCD_TextF(" RPM");
	LDI  R30,LOW(7)
	ST   -Y,R30
	LDI  R26,LOW(1)
	RCALL _LCD_GotoXY
	MOV  R30,R10
	LDI  R31,0
	CALL __CWD1
	CALL __CDF1
	__GETD2N 0x42C80000
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x41800000
	CALL __DIVF21
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x418A6666
	CALL __DIVF21
	__GETD2N 0x42700000
	CALL __MULF12
	CALL __CFD1
	MOVW R26,R30
	RCALL _LCD_Angka4
	__POINTW2FN _0x0,21
	RCALL _LCD_TextF
; 0000 0194       }
	RJMP _0x83
; 0000 0195 }
_0x86:
	RJMP _0x86

	.DSEG
_dCounterKa:
	.BYTE 0x2
_rx_buffer1:
	.BYTE 0x8
_rx_wr_index1:
	.BYTE 0x1
_rx_rd_index1:
	.BYTE 0x1
_rx_counter1:
	.BYTE 0x1
__dCounterKi_S000001C000:
	.BYTE 0x1
__dCounterKa_S000001C000:
	.BYTE 0x1
_Timer2Tick_S000001C000:
	.BYTE 0x1

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x0:
	OUT  0x1B,R30
	__DELAY_USB 5
	CBI  0x1B,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1:
	__DELAY_USB 27
	LD   R30,Y
	SWAP R30
	ANDI R30,0xF0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x2:
	LDI  R27,0
	CALL _delay_ms
	LDI  R26,LOW(48)
	JMP  _LCD_Init_Cmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x3:
	LD   R30,Y
	LDD  R31,Y+1
	LDI  R26,LOW(65535)
	LDI  R27,HIGH(65535)
	CALL __MULW12
	ST   Y,R30
	STD  Y+1,R31
	LDI  R26,LOW(45)
	JMP  _LCD_Data

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x4:
	CALL __DIVW21
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _LCD_Data
	LD   R26,Y
	LDD  R27,Y+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5:
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6:
	STS  134,R30
	LDI  R30,LOW(0)
	STS  133,R30
	STS  132,R30
	RET


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__ROUND_REPACK:
	TST  R21
	BRPL __REPACK
	CPI  R21,0x80
	BRNE __ROUND_REPACK0
	SBRS R30,0
	RJMP __REPACK
__ROUND_REPACK0:
	ADIW R30,1
	ADC  R22,R25
	ADC  R23,R25
	BRVS __REPACK1

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CFD1U:
	SET
	RJMP __CFD1U0
__CFD1:
	CLT
__CFD1U0:
	PUSH R21
	RCALL __UNPACK1
	CPI  R23,0x80
	BRLO __CFD10
	CPI  R23,0xFF
	BRCC __CFD10
	RJMP __ZERORES
__CFD10:
	LDI  R21,22
	SUB  R21,R23
	BRPL __CFD11
	NEG  R21
	CPI  R21,8
	BRTC __CFD19
	CPI  R21,9
__CFD19:
	BRLO __CFD17
	SER  R30
	SER  R31
	SER  R22
	LDI  R23,0x7F
	BLD  R23,7
	RJMP __CFD15
__CFD17:
	CLR  R23
	TST  R21
	BREQ __CFD15
__CFD18:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R21
	BRNE __CFD18
	RJMP __CFD15
__CFD11:
	CLR  R23
__CFD12:
	CPI  R21,8
	BRLO __CFD13
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R23
	SUBI R21,8
	RJMP __CFD12
__CFD13:
	TST  R21
	BREQ __CFD15
__CFD14:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R21
	BRNE __CFD14
__CFD15:
	TST  R0
	BRPL __CFD16
	RCALL __ANEGD1
__CFD16:
	POP  R21
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__MULF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BREQ __ZERORES
	CPI  R25,0x80
	BREQ __ZERORES
	EOR  R0,R1
	SEC
	ADC  R23,R25
	BRVC __MULF124
	BRLT __ZERORES
__MULF125:
	TST  R0
	BRMI __MINRES
	RJMP __MAXRES
__MULF124:
	PUSH R0
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R17
	CLR  R18
	CLR  R25
	MUL  R22,R24
	MOVW R20,R0
	MUL  R24,R31
	MOV  R19,R0
	ADD  R20,R1
	ADC  R21,R25
	MUL  R22,R27
	ADD  R19,R0
	ADC  R20,R1
	ADC  R21,R25
	MUL  R24,R30
	RCALL __MULF126
	MUL  R27,R31
	RCALL __MULF126
	MUL  R22,R26
	RCALL __MULF126
	MUL  R27,R30
	RCALL __MULF127
	MUL  R26,R31
	RCALL __MULF127
	MUL  R26,R30
	ADD  R17,R1
	ADC  R18,R25
	ADC  R19,R25
	ADC  R20,R25
	ADC  R21,R25
	MOV  R30,R19
	MOV  R31,R20
	MOV  R22,R21
	MOV  R21,R18
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	POP  R0
	TST  R22
	BRMI __MULF122
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	RJMP __MULF123
__MULF122:
	INC  R23
	BRVS __MULF125
__MULF123:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__MULF127:
	ADD  R17,R0
	ADC  R18,R1
	ADC  R19,R25
	RJMP __MULF128
__MULF126:
	ADD  R18,R0
	ADC  R19,R1
__MULF128:
	ADC  R20,R25
	ADC  R21,R25
	RET

__DIVF21:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BRNE __DIVF210
	TST  R1
__DIVF211:
	BRPL __DIVF219
	RJMP __MINRES
__DIVF219:
	RJMP __MAXRES
__DIVF210:
	CPI  R25,0x80
	BRNE __DIVF218
__DIVF217:
	RJMP __ZERORES
__DIVF218:
	EOR  R0,R1
	SEC
	SBC  R25,R23
	BRVC __DIVF216
	BRLT __DIVF217
	TST  R0
	RJMP __DIVF211
__DIVF216:
	MOV  R23,R25
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R1
	CLR  R17
	CLR  R18
	CLR  R19
	CLR  R20
	CLR  R21
	LDI  R25,32
__DIVF212:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R20,R17
	BRLO __DIVF213
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SBC  R20,R17
	SEC
	RJMP __DIVF214
__DIVF213:
	CLC
__DIVF214:
	ROL  R21
	ROL  R18
	ROL  R19
	ROL  R1
	ROL  R26
	ROL  R27
	ROL  R24
	ROL  R20
	DEC  R25
	BRNE __DIVF212
	MOVW R30,R18
	MOV  R22,R1
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	TST  R22
	BRMI __DIVF215
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVS __DIVF217
__DIVF215:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__ANEGD1:
	COM  R31
	COM  R22
	COM  R23
	NEG  R30
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__MULW12U:
	MUL  R31,R26
	MOV  R31,R0
	MUL  R30,R27
	ADD  R31,R0
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	RET

__MULW12:
	RCALL __CHKSIGNW
	RCALL __MULW12U
	BRTC __MULW121
	RCALL __ANEGW1
__MULW121:
	RET

__DIVW21U:
	CLR  R0
	CLR  R1
	LDI  R25,16
__DIVW21U1:
	LSL  R26
	ROL  R27
	ROL  R0
	ROL  R1
	SUB  R0,R30
	SBC  R1,R31
	BRCC __DIVW21U2
	ADD  R0,R30
	ADC  R1,R31
	RJMP __DIVW21U3
__DIVW21U2:
	SBR  R26,1
__DIVW21U3:
	DEC  R25
	BRNE __DIVW21U1
	MOVW R30,R26
	MOVW R26,R0
	RET

__DIVW21:
	RCALL __CHKSIGNW
	RCALL __DIVW21U
	BRTC __DIVW211
	RCALL __ANEGW1
__DIVW211:
	RET

__MODW21:
	CLT
	SBRS R27,7
	RJMP __MODW211
	COM  R26
	COM  R27
	ADIW R26,1
	SET
__MODW211:
	SBRC R31,7
	RCALL __ANEGW1
	RCALL __DIVW21U
	MOVW R30,R26
	BRTC __MODW212
	RCALL __ANEGW1
__MODW212:
	RET

__CHKSIGNW:
	CLT
	SBRS R31,7
	RJMP __CHKSW1
	RCALL __ANEGW1
	SET
__CHKSW1:
	SBRS R27,7
	RJMP __CHKSW2
	COM  R26
	COM  R27
	ADIW R26,1
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSW2:
	RET

;END OF CODE MARKER
__END_OF_CODE:
