
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
	.DEF _dCounterKi=R8
	.DEF _dCounterKa=R10
	.DEF _dSpeedKi=R13
	.DEF _dSpeedKa=R12

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
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

_0x6A:
	.DB  0xA
_0x0:
	.DB  0x4B,0x65,0x63,0x65,0x70,0x61,0x74,0x61
	.DB  0x6E,0x20,0x4B,0x61,0x3A,0x0,0x4B,0x65
	.DB  0x63,0x65,0x70,0x61,0x74,0x61,0x6E,0x20
	.DB  0x4B,0x69,0x3A,0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  _Timer2Tick_S0000019000
	.DW  _0x6A*2

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
;Date    : 11/4/2019
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
;#include "adroit.h"
;#include "adroit_LCD.h"

	.CSEG
_LCD_Init_Cmd:
	ST   -Y,R26
;	xData -> Y+0
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	ORI  R30,4
	RCALL SUBOPT_0x0
	RJMP _0x2000004
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
	RJMP _0x2000003
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
_0x2000003:
	SBI  0x1B,2
_0x2000004:
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
	RJMP _0x92
_0x27:
	LDD  R26,Y+2
	SUBI R26,-LOW(192)
_0x92:
	RCALL _LCD_Perintah
	LDD  R17,Y+0
	RJMP _0x2000002
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
	RJMP _0x2000001
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
_0x2000002:
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
;	x -> Y+0
_LCD_Angka3:
	ST   -Y,R27
	ST   -Y,R26
;	x -> Y+0
	LDD  R26,Y+1
	TST  R26
	BRPL _0x45
	LD   R30,Y
	LDD  R31,Y+1
	LDI  R26,LOW(65535)
	LDI  R27,HIGH(65535)
	CALL __MULW12
	ST   Y,R30
	STD  Y+1,R31
	LDI  R26,LOW(45)
	RCALL _LCD_Data
_0x45:
	LD   R26,Y
	LDD  R27,Y+1
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	RCALL SUBOPT_0x3
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	CALL __MODW21
	MOVW R26,R30
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL SUBOPT_0x3
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MODW21
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _LCD_Data
_0x2000001:
	ADIW R28,2
	RET
;	x -> Y+0
;	x -> Y+0
;	x -> Y+1
;	i -> R17
;
;#define SetGlobalInterrupt #asm("sei")
;#define ClrGlobalInterrupt #asm("cli")
;int EnkoderKi, EnkoderKa;
;unsigned int dCounterKi, dCounterKa;
;unsigned char dSpeedKi, dSpeedKa;
;signed short SpeedKa, SpeedKi;
;
;#define MaxPWM  (signed short)  400
;
;#define KPmKa   (signed short)  8
;#define KImKa   (signed short)  10
;#define KDmKa   (signed short)  1
;
;#define KPmKi   (signed short)  8
;#define KImKi   (signed short)  10
;#define KDmKi   (signed short)  1
;
;void HitungPIDmotorKa(void);
;void HitungPIDmotorKi(void);
;
;// External Interrupt 6 service routine
;interrupt [EXT_INT6] void ext_int6_isr(void)
; 0000 0033 {
_ext_int6_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0034 // Place your code here
; 0000 0035     if(P_Enkoder1A){
	SBIS 0x1,6
	RJMP _0x4D
	LDI  R30,LOW(1)
	RJMP _0x4E
_0x4D:
	LDI  R30,LOW(0)
_0x4E:
	CPI  R30,0
	BREQ _0x4C
; 0000 0036         if(P_Enkoder1B) {
	SBIS 0x16,0
	RJMP _0x51
	LDI  R30,LOW(1)
	RJMP _0x52
_0x51:
	LDI  R30,LOW(0)
_0x52:
	CPI  R30,0
	BREQ _0x50
; 0000 0037             ClrGlobalInterrupt;
	cli
; 0000 0038             EnkoderKi--;
	MOVW R30,R4
	SBIW R30,1
	RJMP _0x94
; 0000 0039         } else {
_0x50:
; 0000 003A             ClrGlobalInterrupt;
	cli
; 0000 003B             EnkoderKi++;
	MOVW R30,R4
	ADIW R30,1
_0x94:
	MOVW R4,R30
; 0000 003C         }
; 0000 003D     } else {
	RJMP _0x55
_0x4C:
; 0000 003E         if(P_Enkoder1B) {
	SBIS 0x16,0
	RJMP _0x57
	LDI  R30,LOW(1)
	RJMP _0x58
_0x57:
	LDI  R30,LOW(0)
_0x58:
	CPI  R30,0
	BREQ _0x56
; 0000 003F             ClrGlobalInterrupt;
	cli
; 0000 0040             EnkoderKi++;
	MOVW R30,R4
	ADIW R30,1
	MOVW R4,R30
; 0000 0041         } else {
	RJMP _0x5A
_0x56:
; 0000 0042             ClrGlobalInterrupt;
	cli
; 0000 0043             EnkoderKa--;
	MOVW R30,R6
	SBIW R30,1
	MOVW R6,R30
; 0000 0044         }
_0x5A:
; 0000 0045     }
_0x55:
; 0000 0046     SetGlobalInterrupt;
	sei
; 0000 0047     dCounterKi++;
	MOVW R30,R8
	ADIW R30,1
	MOVW R8,R30
; 0000 0048 }
	RJMP _0x9E
;
;// External Interrupt 7 service routine
;interrupt [EXT_INT7] void ext_int7_isr(void)
; 0000 004C {
_ext_int7_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 004D // Place your code here
; 0000 004E     if(P_Enkoder2A) {
	SBIS 0x1,7
	RJMP _0x5C
	LDI  R30,LOW(1)
	RJMP _0x5D
_0x5C:
	LDI  R30,LOW(0)
_0x5D:
	CPI  R30,0
	BREQ _0x5B
; 0000 004F         if(P_Enkoder2B) {
	SBIS 0x16,2
	RJMP _0x60
	LDI  R30,LOW(1)
	RJMP _0x61
_0x60:
	LDI  R30,LOW(0)
_0x61:
	CPI  R30,0
	BREQ _0x5F
; 0000 0050             ClrGlobalInterrupt;
	cli
; 0000 0051             EnkoderKa++;
	MOVW R30,R6
	ADIW R30,1
	RJMP _0x95
; 0000 0052         } else {
_0x5F:
; 0000 0053             ClrGlobalInterrupt;
	cli
; 0000 0054             EnkoderKa--;
	MOVW R30,R6
	SBIW R30,1
_0x95:
	MOVW R6,R30
; 0000 0055         }
; 0000 0056     } else {
	RJMP _0x64
_0x5B:
; 0000 0057         if(P_Enkoder2B) {
	SBIS 0x16,2
	RJMP _0x66
	LDI  R30,LOW(1)
	RJMP _0x67
_0x66:
	LDI  R30,LOW(0)
_0x67:
	CPI  R30,0
	BREQ _0x65
; 0000 0058             ClrGlobalInterrupt;
	cli
; 0000 0059             EnkoderKa--;
	MOVW R30,R6
	SBIW R30,1
	RJMP _0x96
; 0000 005A         } else {
_0x65:
; 0000 005B             ClrGlobalInterrupt;
	cli
; 0000 005C             EnkoderKa++;
	MOVW R30,R6
	ADIW R30,1
_0x96:
	MOVW R6,R30
; 0000 005D         }
; 0000 005E     }
_0x64:
; 0000 005F     SetGlobalInterrupt;
	sei
; 0000 0060     dCounterKa++;
	MOVW R30,R10
	ADIW R30,1
	MOVW R10,R30
; 0000 0061 }
_0x9E:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;
;// Timer1 overflow interrupt service routine
;interrupt [TIM1_OVF] void timer1_ovf_isr(void)
; 0000 0065 {
_timer1_ovf_isr:
; 0000 0066 // Place your code here
; 0000 0067 
; 0000 0068 }
	RETI
;
;// Timer1 input capture interrupt service routine
;interrupt [TIM1_CAPT] void timer1_capt_isr(void)
; 0000 006C {
_timer1_capt_isr:
; 0000 006D // Place your code here
; 0000 006E 
; 0000 006F }
	RETI
;
;// Timer2 output compare interrupt service routine
;interrupt [TIM2_COMP] void timer2_comp_isr(void)
; 0000 0073 {
_timer2_comp_isr:
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R15
	ST   -Y,R22
	ST   -Y,R23
	ST   -Y,R24
	ST   -Y,R25
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0074 // Place your code here
; 0000 0075     static unsigned short step_timer = 0, step = 0;
; 0000 0076     static unsigned char _dCounterKi = 0, _dCounterKa=0, Timer2Tick = 10;

	.DSEG

	.CSEG
; 0000 0077     SetGlobalInterrupt;
	sei
; 0000 0078     if(--Timer2Tick == 0) Timer2Tick = 10;
	LDS  R30,_Timer2Tick_S0000019000
	SUBI R30,LOW(1)
	STS  _Timer2Tick_S0000019000,R30
	CPI  R30,0
	BRNE _0x6B
	LDI  R30,LOW(10)
	STS  _Timer2Tick_S0000019000,R30
; 0000 0079     switch(Timer2Tick){
_0x6B:
	LDS  R30,_Timer2Tick_S0000019000
	LDI  R31,0
; 0000 007A         case 9:
	CPI  R30,LOW(0x9)
	LDI  R26,HIGH(0x9)
	CPC  R31,R26
	BRNE _0x6F
; 0000 007B             if(dCounterKi > _dCounterKi) dSpeedKi = dCounterKi - _dCounterKi;
	LDS  R30,__dCounterKi_S0000019000
	MOVW R26,R8
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x70
	LDS  R26,__dCounterKi_S0000019000
	MOV  R30,R8
	SUB  R30,R26
	RJMP _0x97
; 0000 007C             else dSpeedKi = 0xFF - _dCounterKi + dCounterKi + 1;
_0x70:
	LDS  R26,__dCounterKi_S0000019000
	LDI  R30,LOW(255)
	SUB  R30,R26
	ADD  R30,R8
	SUBI R30,-LOW(1)
_0x97:
	MOV  R13,R30
; 0000 007D             _dCounterKi = dCounterKi;
	STS  __dCounterKi_S0000019000,R8
; 0000 007E             break;
	RJMP _0x6E
; 0000 007F         case 8:
_0x6F:
	CPI  R30,LOW(0x8)
	LDI  R26,HIGH(0x8)
	CPC  R31,R26
	BREQ _0x6E
; 0000 0080             //HitungPIDmotorKi();
; 0000 0081             break;
; 0000 0082         case 4:
	CPI  R30,LOW(0x4)
	LDI  R26,HIGH(0x4)
	CPC  R31,R26
	BRNE _0x73
; 0000 0083             if(dCounterKa > _dCounterKa) dSpeedKa = dCounterKa - _dCounterKa;
	LDS  R30,__dCounterKa_S0000019000
	MOVW R26,R10
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x74
	LDS  R26,__dCounterKa_S0000019000
	MOV  R30,R10
	SUB  R30,R26
	RJMP _0x98
; 0000 0084             else dSpeedKa = 0xFF - _dCounterKa + dCounterKa + 1;
_0x74:
	LDS  R26,__dCounterKa_S0000019000
	LDI  R30,LOW(255)
	SUB  R30,R26
	ADD  R30,R10
	SUBI R30,-LOW(1)
_0x98:
	MOV  R12,R30
; 0000 0085             _dCounterKa = dCounterKa;
	STS  __dCounterKa_S0000019000,R10
; 0000 0086             break;
	RJMP _0x6E
; 0000 0087         case 3:
_0x73:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x6E
; 0000 0088             HitungPIDmotorKa();
	RCALL _HitungPIDmotorKa
; 0000 0089             break;
; 0000 008A     }
_0x6E:
; 0000 008B 
; 0000 008C     if(step == 0){
	LDS  R30,_step_S0000019000
	LDS  R31,_step_S0000019000+1
	SBIW R30,0
	BRNE _0x77
; 0000 008D         SpeedKa = 0;
	LDI  R30,LOW(0)
	STS  _SpeedKa,R30
	STS  _SpeedKa+1,R30
; 0000 008E         PwmMKi(0);
	STS  133,R30
	RJMP _0x99
; 0000 008F     } else {
_0x77:
; 0000 0090         SpeedKa = 18;
	LDI  R30,LOW(18)
	LDI  R31,HIGH(18)
	STS  _SpeedKa,R30
	STS  _SpeedKa+1,R31
; 0000 0091         PwmMKi(400);
	LDI  R30,LOW(1)
	STS  133,R30
	LDI  R30,LOW(144)
_0x99:
	STS  132,R30
; 0000 0092     }
; 0000 0093     if(step_timer++ > 500){
	LDI  R26,LOW(_step_timer_S0000019000)
	LDI  R27,HIGH(_step_timer_S0000019000)
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	CPI  R30,LOW(0x1F5)
	LDI  R26,HIGH(0x1F5)
	CPC  R31,R26
	BRLO _0x79
; 0000 0094         if(step == 0){
	LDS  R30,_step_S0000019000
	LDS  R31,_step_S0000019000+1
	SBIW R30,0
	BRNE _0x7A
; 0000 0095             step = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	STS  _step_S0000019000,R30
	STS  _step_S0000019000+1,R31
; 0000 0096         } else {
	RJMP _0x7B
_0x7A:
; 0000 0097             step = 0;
	LDI  R30,LOW(0)
	STS  _step_S0000019000,R30
	STS  _step_S0000019000+1,R30
; 0000 0098         }
_0x7B:
; 0000 0099         step_timer = 0;
	LDI  R30,LOW(0)
	STS  _step_timer_S0000019000,R30
	STS  _step_timer_S0000019000+1,R30
; 0000 009A     }
; 0000 009B }
_0x79:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R25,Y+
	LD   R24,Y+
	LD   R23,Y+
	LD   R22,Y+
	LD   R15,Y+
	LD   R1,Y+
	LD   R0,Y+
	RETI
;
;// Declare your global variables here
;
;void main(void)
; 0000 00A0 {
_main:
; 0000 00A1 // Declare your local variables here
; 0000 00A2 
; 0000 00A3 // Input/Output Ports initialization
; 0000 00A4 // Port A initialization
; 0000 00A5 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
; 0000 00A6 // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
; 0000 00A7 PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
; 0000 00A8 DDRA=0xFF;
	LDI  R30,LOW(255)
	OUT  0x1A,R30
; 0000 00A9 
; 0000 00AA // Port B initialization
; 0000 00AB // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=In Func1=In Func0=In
; 0000 00AC // State7=0 State6=0 State5=0 State4=0 State3=0 State2=P State1=T State0=P
; 0000 00AD PORTB=0x05;
	LDI  R30,LOW(5)
	OUT  0x18,R30
; 0000 00AE DDRB=0xF8;
	LDI  R30,LOW(248)
	OUT  0x17,R30
; 0000 00AF 
; 0000 00B0 // Port C initialization
; 0000 00B1 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00B2 // State7=0 State6=0 State5=0 State4=0 State3=P State2=P State1=P State0=P
; 0000 00B3 PORTC=0x0F;
	LDI  R30,LOW(15)
	OUT  0x15,R30
; 0000 00B4 DDRC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x14,R30
; 0000 00B5 
; 0000 00B6 // Port D initialization
; 0000 00B7 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00B8 // State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=T State0=T
; 0000 00B9 PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
; 0000 00BA DDRD=0xF0;
	LDI  R30,LOW(240)
	OUT  0x11,R30
; 0000 00BB 
; 0000 00BC // Port E initialization
; 0000 00BD // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In
; 0000 00BE // State7=P State6=P State5=0 State4=0 State3=0 State2=0 State1=T State0=T
; 0000 00BF PORTE=0xC0;
	LDI  R30,LOW(192)
	OUT  0x3,R30
; 0000 00C0 DDRE=0x3C;
	LDI  R30,LOW(60)
	OUT  0x2,R30
; 0000 00C1 
; 0000 00C2 // Port F initialization
; 0000 00C3 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00C4 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 00C5 PORTF=0x00;
	LDI  R30,LOW(0)
	STS  98,R30
; 0000 00C6 DDRF=0x00;
	STS  97,R30
; 0000 00C7 
; 0000 00C8 // Port G initialization
; 0000 00C9 // Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00CA // State4=T State3=P State2=P State1=P State0=P
; 0000 00CB PORTG=0x0F;
	LDI  R30,LOW(15)
	STS  101,R30
; 0000 00CC DDRG=0x00;
	LDI  R30,LOW(0)
	STS  100,R30
; 0000 00CD 
; 0000 00CE // Timer/Counter 0 initialization
; 0000 00CF // Clock source: System Clock
; 0000 00D0 // Clock value: 250.000 kHz
; 0000 00D1 // Mode: CTC top=OCR0
; 0000 00D2 // OC0 output: Toggle on compare match
; 0000 00D3 ASSR=0x00;
	OUT  0x30,R30
; 0000 00D4 TCCR0=0x1C;
	LDI  R30,LOW(28)
	OUT  0x33,R30
; 0000 00D5 TCNT0=0x00;
	LDI  R30,LOW(0)
	OUT  0x32,R30
; 0000 00D6 OCR0=0x00;
	OUT  0x31,R30
; 0000 00D7 
; 0000 00D8 // Timer/Counter 1 initialization
; 0000 00D9 // Clock source: System Clock
; 0000 00DA // Clock value: 2000.000 kHz
; 0000 00DB // Mode: Ph. & fr. cor. PWM top=ICR1
; 0000 00DC // OC1A output: Non-Inv.
; 0000 00DD // OC1B output: Non-Inv.
; 0000 00DE // OC1C output: Non-Inv.
; 0000 00DF // Noise Canceler: Off
; 0000 00E0 // Input Capture on Falling Edge
; 0000 00E1 // Timer1 Overflow Interrupt: On
; 0000 00E2 // Input Capture Interrupt: On
; 0000 00E3 // Compare A Match Interrupt: Off
; 0000 00E4 // Compare B Match Interrupt: Off
; 0000 00E5 // Compare C Match Interrupt: Off
; 0000 00E6 TCCR1A=0xA8;
	LDI  R30,LOW(168)
	OUT  0x2F,R30
; 0000 00E7 TCCR1B=0x12;
	LDI  R30,LOW(18)
	OUT  0x2E,R30
; 0000 00E8 TCNT1H=0x00;
	LDI  R30,LOW(0)
	OUT  0x2D,R30
; 0000 00E9 TCNT1L=0x00;
	OUT  0x2C,R30
; 0000 00EA ICR1H=0x13;
	LDI  R30,LOW(19)
	OUT  0x27,R30
; 0000 00EB ICR1L=0x87;
	LDI  R30,LOW(135)
	OUT  0x26,R30
; 0000 00EC OCR1AH=0x00;
	LDI  R30,LOW(0)
	OUT  0x2B,R30
; 0000 00ED OCR1AL=0x00;
	OUT  0x2A,R30
; 0000 00EE OCR1BH=0x00;
	OUT  0x29,R30
; 0000 00EF OCR1BL=0x00;
	OUT  0x28,R30
; 0000 00F0 OCR1CH=0x00;
	STS  121,R30
; 0000 00F1 OCR1CL=0x00;
	STS  120,R30
; 0000 00F2 
; 0000 00F3 // Timer/Counter 2 initialization
; 0000 00F4 // Clock source: System Clock
; 0000 00F5 // Clock value: 250.000 kHz
; 0000 00F6 // Mode: CTC top=OCR2
; 0000 00F7 // OC2 output: Disconnected
; 0000 00F8 TCCR2=0x0B;
	LDI  R30,LOW(11)
	OUT  0x25,R30
; 0000 00F9 TCNT2=0x00;
	LDI  R30,LOW(0)
	OUT  0x24,R30
; 0000 00FA OCR2=0xF9;
	LDI  R30,LOW(249)
	OUT  0x23,R30
; 0000 00FB 
; 0000 00FC // Timer/Counter 3 initialization
; 0000 00FD // Clock source: System Clock
; 0000 00FE // Clock value: 16000.000 kHz
; 0000 00FF // Mode: Ph. correct PWM top=ICR3
; 0000 0100 // OC3A output: Inverted
; 0000 0101 // OC3B output: Inverted
; 0000 0102 // OC3C output: Discon.
; 0000 0103 // Noise Canceler: Off
; 0000 0104 // Input Capture on Falling Edge
; 0000 0105 // Timer3 Overflow Interrupt: Off
; 0000 0106 // Input Capture Interrupt: Off
; 0000 0107 // Compare A Match Interrupt: Off
; 0000 0108 // Compare B Match Interrupt: Off
; 0000 0109 // Compare C Match Interrupt: Off
; 0000 010A TCCR3A=0xF2;
	LDI  R30,LOW(242)
	STS  139,R30
; 0000 010B TCCR3B=0x11;
	LDI  R30,LOW(17)
	STS  138,R30
; 0000 010C TCNT3H=0x00;
	LDI  R30,LOW(0)
	STS  137,R30
; 0000 010D TCNT3L=0x00;
	STS  136,R30
; 0000 010E ICR3H=0x01;
	LDI  R30,LOW(1)
	STS  129,R30
; 0000 010F ICR3L=0x90;
	LDI  R30,LOW(144)
	STS  128,R30
; 0000 0110 OCR3AH=0x00;
	RCALL SUBOPT_0x4
; 0000 0111 OCR3AL=0x00;
	STS  134,R30
; 0000 0112 OCR3BH=0x00;
	LDI  R30,LOW(0)
	STS  133,R30
; 0000 0113 OCR3BL=0x00;
	STS  132,R30
; 0000 0114 OCR3CH=0x00;
	STS  131,R30
; 0000 0115 OCR3CL=0xC8;
	LDI  R30,LOW(200)
	STS  130,R30
; 0000 0116 
; 0000 0117 // External Interrupt(s) initialization
; 0000 0118 // INT0: Off
; 0000 0119 // INT1: Off
; 0000 011A // INT2: Off
; 0000 011B // INT3: Off
; 0000 011C // INT4: Off
; 0000 011D // INT5: Off
; 0000 011E // INT6: On
; 0000 011F // INT6 Mode: Any change
; 0000 0120 // INT7: On
; 0000 0121 // INT7 Mode: Any change
; 0000 0122 EICRA=0x00;
	LDI  R30,LOW(0)
	STS  106,R30
; 0000 0123 EICRB=0x50;
	LDI  R30,LOW(80)
	OUT  0x3A,R30
; 0000 0124 EIMSK=0xC0;
	LDI  R30,LOW(192)
	OUT  0x39,R30
; 0000 0125 EIFR=0xC0;
	OUT  0x38,R30
; 0000 0126 
; 0000 0127 // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 0128 TIMSK=0xA4;
	LDI  R30,LOW(164)
	OUT  0x37,R30
; 0000 0129 
; 0000 012A ETIMSK=0x00;
	LDI  R30,LOW(0)
	STS  125,R30
; 0000 012B 
; 0000 012C // USART0 initialization
; 0000 012D // USART0 disabled
; 0000 012E UCSR0B=0x00;
	OUT  0xA,R30
; 0000 012F 
; 0000 0130 // USART1 initialization
; 0000 0131 // USART1 disabled
; 0000 0132 UCSR1B=0x00;
	STS  154,R30
; 0000 0133 
; 0000 0134 // Analog Comparator initialization
; 0000 0135 // Analog Comparator: Off
; 0000 0136 // Analog Comparator Input Capture by Timer/Counter 1: Off
; 0000 0137 ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 0138 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
; 0000 0139 
; 0000 013A // ADC initialization
; 0000 013B // ADC disabled
; 0000 013C ADCSRA=0x00;
	OUT  0x6,R30
; 0000 013D 
; 0000 013E // SPI initialization
; 0000 013F // SPI disabled
; 0000 0140 SPCR=0x00;
	OUT  0xD,R30
; 0000 0141 
; 0000 0142 // TWI initialization
; 0000 0143 // TWI disabled
; 0000 0144 TWCR=0x00;
	STS  116,R30
; 0000 0145 
; 0000 0146 // Global enable interrupts
; 0000 0147 #asm("sei")
	sei
; 0000 0148 
; 0000 0149 LCD_Init();
	RCALL _LCD_Init
; 0000 014A 
; 0000 014B SpeedKa = 2;
	LDI  R30,LOW(2)
	LDI  R31,HIGH(2)
	STS  _SpeedKa,R30
	STS  _SpeedKa+1,R31
; 0000 014C SpeedKi = 2;
	STS  _SpeedKi,R30
	STS  _SpeedKi+1,R31
; 0000 014D 
; 0000 014E LCD_TulisF(0,"Kecepatan Ka:");
	LDI  R30,LOW(0)
	ST   -Y,R30
	__POINTW2FN _0x0,0
	RCALL _LCD_TulisF
; 0000 014F LCD_TulisF(1,"Kecepatan Ki:");
	LDI  R30,LOW(1)
	ST   -Y,R30
	__POINTW2FN _0x0,14
	RCALL _LCD_TulisF
; 0000 0150 
; 0000 0151 while (1)
_0x7C:
; 0000 0152       {
; 0000 0153       // Place your code here
; 0000 0154         LCD_GotoXY(13,0); LCD_Angka3(dSpeedKa); LCD_Data(' ');
	LDI  R30,LOW(13)
	ST   -Y,R30
	LDI  R26,LOW(0)
	RCALL _LCD_GotoXY
	MOV  R26,R12
	RCALL SUBOPT_0x5
; 0000 0155         LCD_GotoXY(13,1); LCD_Angka3(dSpeedKi); LCD_Data(' ');
	LDI  R30,LOW(13)
	ST   -Y,R30
	LDI  R26,LOW(1)
	RCALL _LCD_GotoXY
	MOV  R26,R13
	RCALL SUBOPT_0x5
; 0000 0156       }
	RJMP _0x7C
; 0000 0157 }
_0x7F:
	RJMP _0x7F
;
;void HitungPIDmotorKi(){
; 0000 0159 void HitungPIDmotorKi(){
; 0000 015A     signed char ErrorMKi, dErrorMKi;
; 0000 015B     signed short U;
; 0000 015C     static signed char iErrorMKi = 0, lastErrorMKi = 0;
; 0000 015D 
; 0000 015E     if(SpeedKi > 0) {
;	ErrorMKi -> R17
;	dErrorMKi -> R16
;	U -> R18,R19
; 0000 015F         ErrorMKi = (signed char) SpeedKi - (signed char) dSpeedKi;
; 0000 0160         iErrorMKi += ErrorMKi;
; 0000 0161 
; 0000 0162         if(iErrorMKi > 40){  iErrorMKi = 40;}
; 0000 0163         else if(iErrorMKi < -40){ iErrorMKi = -40;}
; 0000 0164 
; 0000 0165         dErrorMKi = lastErrorMKi - ErrorMKi;
; 0000 0166 
; 0000 0167         U = ErrorMKi * KPmKi + iErrorMKi * KImKi + dErrorMKi * KDmKi;
; 0000 0168 
; 0000 0169         if(U > MaxPWM) {PwmMKi(MaxPWM);}
; 0000 016A         else if(U > 0) {PwmMKi(U);}
; 0000 016B         else {PwmMKi(0);}
; 0000 016C     } else {
; 0000 016D         PwmMKi(0);
; 0000 016E         iErrorMKi = 0;
; 0000 016F         lastErrorMKi = 0;
; 0000 0170     }
; 0000 0171 }
;
;void HitungPIDmotorKa(){
; 0000 0173 void HitungPIDmotorKa(){
_HitungPIDmotorKa:
; 0000 0174     signed char ErrorMKa, dErrorMKa;
; 0000 0175     signed short U;
; 0000 0176     static signed char iErrorMKa = 0, lastErrorMKa = 0;
; 0000 0177 
; 0000 0178     if(SpeedKa > 0) {
	CALL __SAVELOCR4
;	ErrorMKa -> R17
;	dErrorMKa -> R16
;	U -> R18,R19
	LDS  R26,_SpeedKa
	LDS  R27,_SpeedKa+1
	CALL __CPW02
	BRLT PC+3
	JMP _0x89
; 0000 0179         ErrorMKa = (signed char) SpeedKa - (signed char) dSpeedKa;
	LDS  R30,_SpeedKa
	SUB  R30,R12
	MOV  R17,R30
; 0000 017A         iErrorMKa += ErrorMKa;
	MOV  R30,R17
	LDS  R26,_iErrorMKa_S000001C000
	ADD  R30,R26
	STS  _iErrorMKa_S000001C000,R30
; 0000 017B 
; 0000 017C         if(iErrorMKa > 40)  {iErrorMKa = 40;}
	LDS  R26,_iErrorMKa_S000001C000
	CPI  R26,LOW(0x29)
	BRLT _0x8A
	LDI  R30,LOW(40)
	RJMP _0x9C
; 0000 017D         else if(iErrorMKa < -40)    {iErrorMKa = -40;}
_0x8A:
	LDS  R26,_iErrorMKa_S000001C000
	CPI  R26,LOW(0xD8)
	BRGE _0x8C
	LDI  R30,LOW(216)
_0x9C:
	STS  _iErrorMKa_S000001C000,R30
; 0000 017E 
; 0000 017F         dErrorMKa = lastErrorMKa - ErrorMKa;
_0x8C:
	LDS  R30,_lastErrorMKa_S000001C000
	SUB  R30,R17
	MOV  R16,R30
; 0000 0180 
; 0000 0181         U = ErrorMKa * KPmKa + iErrorMKa * KImKa + dErrorMKa * KDmKa;
	LDI  R30,LOW(8)
	MULS R30,R17
	MOVW R22,R0
	LDS  R26,_iErrorMKa_S000001C000
	LDI  R30,LOW(10)
	MULS R30,R26
	MOVW R30,R0
	MOVW R26,R22
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(1)
	MULS R30,R16
	MOVW R30,R0
	ADD  R30,R26
	ADC  R31,R27
	MOVW R18,R30
; 0000 0182 
; 0000 0183         if(U > MaxPWM) {PwmMKa(MaxPWM); }
	__CPWRN 18,19,401
	BRLT _0x8D
	LDI  R30,LOW(1)
	STS  135,R30
	LDI  R30,LOW(144)
	RJMP _0x9D
; 0000 0184         else if(U > 0) {PwmMKa(U);}
_0x8D:
	CLR  R0
	CP   R0,R18
	CPC  R0,R19
	BRGE _0x8F
	MOVW R30,R18
	CALL __ASRW8
	STS  135,R30
	STS  134,R18
; 0000 0185         else {PwmMKa(0);}
	RJMP _0x90
_0x8F:
	RCALL SUBOPT_0x4
_0x9D:
	STS  134,R30
_0x90:
; 0000 0186     } else {
	RJMP _0x91
_0x89:
; 0000 0187         PwmMKa(0);
	RCALL SUBOPT_0x4
	STS  134,R30
; 0000 0188         iErrorMKa = 0;
	LDI  R30,LOW(0)
	STS  _iErrorMKa_S000001C000,R30
; 0000 0189         lastErrorMKa = 0;
	STS  _lastErrorMKa_S000001C000,R30
; 0000 018A     }
_0x91:
; 0000 018B }
	CALL __LOADLOCR4
	ADIW R28,4
	RET

	.DSEG
_SpeedKa:
	.BYTE 0x2
_SpeedKi:
	.BYTE 0x2
_step_timer_S0000019000:
	.BYTE 0x2
_step_S0000019000:
	.BYTE 0x2
__dCounterKi_S0000019000:
	.BYTE 0x1
__dCounterKa_S0000019000:
	.BYTE 0x1
_Timer2Tick_S0000019000:
	.BYTE 0x1
_iErrorMKa_S000001C000:
	.BYTE 0x1
_lastErrorMKa_S000001C000:
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
	RJMP _LCD_Init_Cmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3:
	CALL __DIVW21
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _LCD_Data
	LD   R26,Y
	LDD  R27,Y+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x4:
	LDI  R30,LOW(0)
	STS  135,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5:
	CLR  R27
	RCALL _LCD_Angka3
	LDI  R26,LOW(32)
	RJMP _LCD_Data


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

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__ASRW8:
	MOV  R30,R31
	CLR  R31
	SBRC R30,7
	SER  R31
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

__CPW02:
	CLR  R0
	CP   R0,R26
	CPC  R0,R27
	RET

__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
