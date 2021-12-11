
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
	.DEF _rx_wr_index1=R5
	.DEF _rx_rd_index1=R4
	.DEF _rx_counter1=R7

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
	JMP  _adc_isr
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


__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  0x02
	.DW  __REG_BIT_VARS*2

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
;Date    : 9/25/2019
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
;
;#include <delay.h>
;
;#include "adroit.h"
;#include "adroit_LCD.h"

	.CSEG
;	xData -> Y+0
;	xData -> Y+0
;	xData -> Y+0
;	x -> Y+2
;	y -> Y+1
;	baris -> R17
;	*text -> Y+0
;	*text -> Y+0
;	Baris -> Y+2
;	*text -> Y+0
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
;	x -> Y+0
;	x -> Y+0
;	x -> Y+0
;	x -> Y+0
;	x -> Y+1
;	i -> R17
;
;// External Interrupt 6 service routine
;interrupt [EXT_INT6] void ext_int6_isr(void)
; 0000 0021 {
_ext_int6_isr:
; 0000 0022 // Place your code here
; 0000 0023 
; 0000 0024 }
	RETI
;
;// External Interrupt 7 service routine
;interrupt [EXT_INT7] void ext_int7_isr(void)
; 0000 0028 {
_ext_int7_isr:
; 0000 0029 // Place your code here
; 0000 002A 
; 0000 002B }
	RETI
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
; 0000 005E {
_usart1_rx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 005F char status,data;
; 0000 0060 status=UCSR1A;
	ST   -Y,R17
	ST   -Y,R16
;	status -> R17
;	data -> R16
	LDS  R17,155
; 0000 0061 data=UDR1;
	LDS  R16,156
; 0000 0062 if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x4C
; 0000 0063    {
; 0000 0064    rx_buffer1[rx_wr_index1++]=data;
	MOV  R30,R5
	INC  R5
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer1)
	SBCI R31,HIGH(-_rx_buffer1)
	ST   Z,R16
; 0000 0065 #if RX_BUFFER_SIZE1 == 256
; 0000 0066    // special case for receiver buffer size=256
; 0000 0067    if (++rx_counter1 == 0) rx_buffer_overflow1=1;
; 0000 0068 #else
; 0000 0069    if (rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
	LDI  R30,LOW(8)
	CP   R30,R5
	BRNE _0x4D
	CLR  R5
; 0000 006A    if (++rx_counter1 == RX_BUFFER_SIZE1)
_0x4D:
	INC  R7
	LDI  R30,LOW(8)
	CP   R30,R7
	BRNE _0x4E
; 0000 006B       {
; 0000 006C       rx_counter1=0;
	CLR  R7
; 0000 006D       rx_buffer_overflow1=1;
	SET
	BLD  R2,0
; 0000 006E       }
; 0000 006F #endif
; 0000 0070    }
_0x4E:
; 0000 0071 }
_0x4C:
	LD   R16,Y+
	LD   R17,Y+
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;
;// Get a character from the USART1 Receiver buffer
;#pragma used+
;char getchar1(void)
; 0000 0076 {
; 0000 0077 char data;
; 0000 0078 while (rx_counter1==0);
;	data -> R17
; 0000 0079 data=rx_buffer1[rx_rd_index1++];
; 0000 007A #if RX_BUFFER_SIZE1 != 256
; 0000 007B if (rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
; 0000 007C #endif
; 0000 007D #asm("cli")
; 0000 007E --rx_counter1;
; 0000 007F #asm("sei")
; 0000 0080 return data;
; 0000 0081 }
;#pragma used-
;// Write a character to the USART1 Transmitter
;#pragma used+
;void putchar1(char c)
; 0000 0086 {
; 0000 0087 while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
;	c -> Y+0
; 0000 0088 UDR1=c;
; 0000 0089 }
;#pragma used-
;
;// Timer1 overflow interrupt service routine
;interrupt [TIM1_OVF] void timer1_ovf_isr(void)
; 0000 008E {
_timer1_ovf_isr:
; 0000 008F // Place your code here
; 0000 0090 
; 0000 0091 }
	RETI
;
;// Timer1 input capture interrupt service routine
;interrupt [TIM1_CAPT] void timer1_capt_isr(void)
; 0000 0095 {
_timer1_capt_isr:
; 0000 0096 // Place your code here
; 0000 0097 
; 0000 0098 }
	RETI
;
;// Timer2 output compare interrupt service routine
;interrupt [TIM2_COMP] void timer2_comp_isr(void)
; 0000 009C {
_timer2_comp_isr:
; 0000 009D // Place your code here
; 0000 009E 
; 0000 009F }
	RETI
;
;#define FIRST_ADC_INPUT 0
;#define LAST_ADC_INPUT 7
;unsigned char adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
;#define ADC_VREF_TYPE 0x20
;
;// ADC interrupt service routine
;// with auto input scanning
;interrupt [ADC_INT] void adc_isr(void)
; 0000 00A9 {
_adc_isr:
	ST   -Y,R24
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	IN   R30,SREG
	ST   -Y,R30
; 0000 00AA static unsigned char input_index=0;
; 0000 00AB // Read the 8 most significant bits
; 0000 00AC // of the AD conversion result
; 0000 00AD adc_data[input_index]=ADCH;
	LDS  R26,_input_index_S000001D000
	LDI  R27,0
	SUBI R26,LOW(-_adc_data)
	SBCI R27,HIGH(-_adc_data)
	IN   R30,0x5
	ST   X,R30
; 0000 00AE // Select next ADC input
; 0000 00AF if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
	LDS  R26,_input_index_S000001D000
	SUBI R26,-LOW(1)
	STS  _input_index_S000001D000,R26
	CPI  R26,LOW(0x8)
	BRLO _0x56
; 0000 00B0    input_index=0;
	LDI  R30,LOW(0)
	STS  _input_index_S000001D000,R30
; 0000 00B1 ADMUX=(FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff))+input_index;
_0x56:
	LDS  R30,_input_index_S000001D000
	SUBI R30,-LOW(32)
	OUT  0x7,R30
; 0000 00B2 // Delay needed for the stabilization of the ADC input voltage
; 0000 00B3 delay_us(10);
	__DELAY_USB 53
; 0000 00B4 // Start the AD conversion
; 0000 00B5 ADCSRA|=0x40;
	SBI  0x6,6
; 0000 00B6 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R24,Y+
	RETI
;
;// Declare your global variables here
;
;void main(void)
; 0000 00BB {
_main:
; 0000 00BC // Declare your local variables here
; 0000 00BD 
; 0000 00BE // Input/Output Ports initialization
; 0000 00BF // Port A initialization
; 0000 00C0 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
; 0000 00C1 // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
; 0000 00C2 PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
; 0000 00C3 DDRA=0xFF;
	LDI  R30,LOW(255)
	OUT  0x1A,R30
; 0000 00C4 
; 0000 00C5 // Port B initialization
; 0000 00C6 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=In Func1=In Func0=In
; 0000 00C7 // State7=0 State6=0 State5=0 State4=0 State3=0 State2=P State1=T State0=P
; 0000 00C8 PORTB=0x05;
	LDI  R30,LOW(5)
	OUT  0x18,R30
; 0000 00C9 DDRB=0xF8;
	LDI  R30,LOW(248)
	OUT  0x17,R30
; 0000 00CA 
; 0000 00CB // Port C initialization
; 0000 00CC // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00CD // State7=0 State6=0 State5=0 State4=0 State3=P State2=P State1=P State0=P
; 0000 00CE PORTC=0x0F;
	LDI  R30,LOW(15)
	OUT  0x15,R30
; 0000 00CF DDRC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x14,R30
; 0000 00D0 
; 0000 00D1 // Port D initialization
; 0000 00D2 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00D3 // State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=T State0=T
; 0000 00D4 PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
; 0000 00D5 DDRD=0xF0;
	LDI  R30,LOW(240)
	OUT  0x11,R30
; 0000 00D6 
; 0000 00D7 // Port E initialization
; 0000 00D8 // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In
; 0000 00D9 // State7=P State6=P State5=0 State4=0 State3=0 State2=0 State1=T State0=T
; 0000 00DA PORTE=0xC0;
	LDI  R30,LOW(192)
	OUT  0x3,R30
; 0000 00DB DDRE=0x3C;
	LDI  R30,LOW(60)
	OUT  0x2,R30
; 0000 00DC 
; 0000 00DD // Port F initialization
; 0000 00DE // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00DF // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 00E0 PORTF=0x00;
	LDI  R30,LOW(0)
	STS  98,R30
; 0000 00E1 DDRF=0x00;
	STS  97,R30
; 0000 00E2 
; 0000 00E3 // Port G initialization
; 0000 00E4 // Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00E5 // State4=T State3=P State2=P State1=P State0=P
; 0000 00E6 PORTG=0x0F;
	LDI  R30,LOW(15)
	STS  101,R30
; 0000 00E7 DDRG=0x00;
	LDI  R30,LOW(0)
	STS  100,R30
; 0000 00E8 
; 0000 00E9 // Timer/Counter 0 initialization
; 0000 00EA // Clock source: System Clock
; 0000 00EB // Clock value: 250.000 kHz
; 0000 00EC // Mode: CTC top=OCR0
; 0000 00ED // OC0 output: Toggle on compare match
; 0000 00EE ASSR=0x00;
	OUT  0x30,R30
; 0000 00EF TCCR0=0x1C;
	LDI  R30,LOW(28)
	OUT  0x33,R30
; 0000 00F0 TCNT0=0x00;
	LDI  R30,LOW(0)
	OUT  0x32,R30
; 0000 00F1 OCR0=0x00;
	OUT  0x31,R30
; 0000 00F2 
; 0000 00F3 // Timer/Counter 1 initialization
; 0000 00F4 // Clock source: System Clock
; 0000 00F5 // Clock value: 2000.000 kHz
; 0000 00F6 // Mode: Ph. & fr. cor. PWM top=ICR1
; 0000 00F7 // OC1A output: Non-Inv.
; 0000 00F8 // OC1B output: Non-Inv.
; 0000 00F9 // OC1C output: Non-Inv.
; 0000 00FA // Noise Canceler: Off
; 0000 00FB // Input Capture on Falling Edge
; 0000 00FC // Timer1 Overflow Interrupt: On
; 0000 00FD // Input Capture Interrupt: On
; 0000 00FE // Compare A Match Interrupt: Off
; 0000 00FF // Compare B Match Interrupt: Off
; 0000 0100 // Compare C Match Interrupt: Off
; 0000 0101 TCCR1A=0xA8;
	LDI  R30,LOW(168)
	OUT  0x2F,R30
; 0000 0102 TCCR1B=0x12;
	LDI  R30,LOW(18)
	OUT  0x2E,R30
; 0000 0103 TCNT1H=0x00;
	LDI  R30,LOW(0)
	OUT  0x2D,R30
; 0000 0104 TCNT1L=0x00;
	OUT  0x2C,R30
; 0000 0105 ICR1H=0x13;
	LDI  R30,LOW(19)
	OUT  0x27,R30
; 0000 0106 ICR1L=0x87;
	LDI  R30,LOW(135)
	OUT  0x26,R30
; 0000 0107 OCR1AH=0x00;
	LDI  R30,LOW(0)
	OUT  0x2B,R30
; 0000 0108 OCR1AL=0x00;
	OUT  0x2A,R30
; 0000 0109 OCR1BH=0x00;
	OUT  0x29,R30
; 0000 010A OCR1BL=0x00;
	OUT  0x28,R30
; 0000 010B OCR1CH=0x00;
	STS  121,R30
; 0000 010C OCR1CL=0x00;
	STS  120,R30
; 0000 010D 
; 0000 010E // Timer/Counter 2 initialization
; 0000 010F // Clock source: System Clock
; 0000 0110 // Clock value: 250.000 kHz
; 0000 0111 // Mode: CTC top=OCR2
; 0000 0112 // OC2 output: Disconnected
; 0000 0113 TCCR2=0x0B;
	LDI  R30,LOW(11)
	OUT  0x25,R30
; 0000 0114 TCNT2=0x00;
	LDI  R30,LOW(0)
	OUT  0x24,R30
; 0000 0115 OCR2=0xF9;
	LDI  R30,LOW(249)
	OUT  0x23,R30
; 0000 0116 
; 0000 0117 // Timer/Counter 3 initialization
; 0000 0118 // Clock source: System Clock
; 0000 0119 // Clock value: 16000.000 kHz
; 0000 011A // Mode: Ph. correct PWM top=ICR3
; 0000 011B // OC3A output: Inverted
; 0000 011C // OC3B output: Inverted
; 0000 011D // OC3C output: Discon.
; 0000 011E // Noise Canceler: Off
; 0000 011F // Input Capture on Falling Edge
; 0000 0120 // Timer3 Overflow Interrupt: Off
; 0000 0121 // Input Capture Interrupt: Off
; 0000 0122 // Compare A Match Interrupt: Off
; 0000 0123 // Compare B Match Interrupt: Off
; 0000 0124 // Compare C Match Interrupt: Off
; 0000 0125 TCCR3A=0xF2;
	LDI  R30,LOW(242)
	STS  139,R30
; 0000 0126 TCCR3B=0x11;
	LDI  R30,LOW(17)
	STS  138,R30
; 0000 0127 TCNT3H=0x00;
	LDI  R30,LOW(0)
	STS  137,R30
; 0000 0128 TCNT3L=0x00;
	STS  136,R30
; 0000 0129 ICR3H=0x01;
	LDI  R30,LOW(1)
	STS  129,R30
; 0000 012A ICR3L=0x90;
	LDI  R30,LOW(144)
	STS  128,R30
; 0000 012B OCR3AH=0x00;
	LDI  R30,LOW(0)
	STS  135,R30
; 0000 012C OCR3AL=0x00;
	STS  134,R30
; 0000 012D OCR3BH=0x00;
	STS  133,R30
; 0000 012E OCR3BL=0x00;
	STS  132,R30
; 0000 012F OCR3CH=0x00;
	STS  131,R30
; 0000 0130 OCR3CL=0xC8;
	LDI  R30,LOW(200)
	STS  130,R30
; 0000 0131 
; 0000 0132 // External Interrupt(s) initialization
; 0000 0133 // INT0: Off
; 0000 0134 // INT1: Off
; 0000 0135 // INT2: Off
; 0000 0136 // INT3: Off
; 0000 0137 // INT4: Off
; 0000 0138 // INT5: Off
; 0000 0139 // INT6: On
; 0000 013A // INT6 Mode: Any change
; 0000 013B // INT7: On
; 0000 013C // INT7 Mode: Any change
; 0000 013D EICRA=0x00;
	LDI  R30,LOW(0)
	STS  106,R30
; 0000 013E EICRB=0x50;
	LDI  R30,LOW(80)
	OUT  0x3A,R30
; 0000 013F EIMSK=0xC0;
	LDI  R30,LOW(192)
	OUT  0x39,R30
; 0000 0140 EIFR=0xC0;
	OUT  0x38,R30
; 0000 0141 
; 0000 0142 // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 0143 TIMSK=0xA4;
	LDI  R30,LOW(164)
	OUT  0x37,R30
; 0000 0144 
; 0000 0145 ETIMSK=0x00;
	LDI  R30,LOW(0)
	STS  125,R30
; 0000 0146 
; 0000 0147 // USART0 initialization
; 0000 0148 // USART0 disabled
; 0000 0149 UCSR0B=0x00;
	OUT  0xA,R30
; 0000 014A 
; 0000 014B // USART1 initialization
; 0000 014C // Communication Parameters: 8 Data, 1 Stop, No Parity
; 0000 014D // USART1 Receiver: On
; 0000 014E // USART1 Transmitter: On
; 0000 014F // USART1 Mode: Asynchronous
; 0000 0150 // USART1 Baud Rate: 9600
; 0000 0151 UCSR1A=0x00;
	STS  155,R30
; 0000 0152 UCSR1B=0x98;
	LDI  R30,LOW(152)
	STS  154,R30
; 0000 0153 UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
; 0000 0154 UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
; 0000 0155 UBRR1L=0x67;
	LDI  R30,LOW(103)
	STS  153,R30
; 0000 0156 
; 0000 0157 // Analog Comparator initialization
; 0000 0158 // Analog Comparator: Off
; 0000 0159 // Analog Comparator Input Capture by Timer/Counter 1: Off
; 0000 015A ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 015B SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
; 0000 015C 
; 0000 015D // ADC initialization
; 0000 015E // ADC Clock frequency: 125.000 kHz
; 0000 015F // ADC Voltage Reference: AREF pin
; 0000 0160 // Only the 8 most significant bits of
; 0000 0161 // the AD conversion result are used
; 0000 0162 ADMUX=FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff);
	LDI  R30,LOW(32)
	OUT  0x7,R30
; 0000 0163 ADCSRA=0xCF;
	LDI  R30,LOW(207)
	OUT  0x6,R30
; 0000 0164 
; 0000 0165 // SPI initialization
; 0000 0166 // SPI disabled
; 0000 0167 SPCR=0x00;
	LDI  R30,LOW(0)
	OUT  0xD,R30
; 0000 0168 
; 0000 0169 // TWI initialization
; 0000 016A // TWI disabled
; 0000 016B TWCR=0x00;
	STS  116,R30
; 0000 016C 
; 0000 016D // Global enable interrupts
; 0000 016E #asm("sei")
	sei
; 0000 016F 
; 0000 0170 while (1)
_0x57:
; 0000 0171       {
; 0000 0172       // Place your code here
; 0000 0173 
; 0000 0174       }
	RJMP _0x57
; 0000 0175 }
_0x5A:
	RJMP _0x5A

	.DSEG
_rx_buffer1:
	.BYTE 0x8
_adc_data:
	.BYTE 0x8
_input_index_S000001D000:
	.BYTE 0x1

	.CSEG

	.CSEG
;END OF CODE MARKER
__END_OF_CODE:
