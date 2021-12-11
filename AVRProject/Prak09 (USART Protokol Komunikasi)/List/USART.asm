
;CodeVisionAVR C Compiler V2.05.3 Standard
;(C) Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega8535
;Program type             : Application
;Clock frequency          : 11.059200 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 128 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;Global 'const' stored in FLASH     : No
;Enhanced function parameter passing: Yes
;Enhanced core instructions         : On
;Smart register allocation          : On
;Automatic register allocation      : On

	#pragma AVRPART ADMIN PART_NAME ATmega8535
	#pragma AVRPART MEMORY PROG_FLASH 8192
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 607
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

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
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

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

	.EQU __SRAM_START=0x0060
	.EQU __SRAM_END=0x025F
	.EQU __DSTACK_SIZE=0x0080
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
	RCALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRD
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
	RCALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMRDW
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	RCALL __PUTDP1
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
	.DEF _rx_wr_index=R5
	.DEF _rx_rd_index=R4
	.DEF _rx_counter=R7

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	RJMP __RESET
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP _usart_rx_isr
	RJMP 0x00
	RJMP 0x00
	RJMP _adc_isr
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00

_tbl10_G100:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G100:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

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
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

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
	LDI  R26,__SRAM_START
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

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0xE0

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
;Date    : 5/16/2019
;Author  : unknown
;Company :
;Comments:
;
;
;Chip type               : ATmega8535
;Program type            : Application
;AVR Core Clock frequency: 11.059200 MHz
;Memory model            : Small
;External RAM size       : 0
;Data Stack size         : 128
;*****************************************************/
;
;#include <mega8535.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;
;#include <delay.h>
;
;#define LED         PORTB
;#define DipSwitch   PINC
;#define PwmA        OCR1A
;#define PwmB        OCR1B
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
;// USART Receiver buffer
;#define RX_BUFFER_SIZE 12
;char rx_buffer[RX_BUFFER_SIZE];
;
;#if RX_BUFFER_SIZE <= 256
;unsigned char rx_wr_index,rx_rd_index,rx_counter;
;#else
;unsigned int rx_wr_index,rx_rd_index,rx_counter;
;#endif
;
;// This flag is set on USART Receiver buffer overflow
;bit rx_buffer_overflow;
;
;// USART Receiver interrupt service routine
;interrupt [USART_RXC] void usart_rx_isr(void)
; 0000 0052 {

	.CSEG
_usart_rx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0053 char status,data;
; 0000 0054 status=UCSRA;
	RCALL __SAVELOCR2
;	status -> R17
;	data -> R16
	IN   R17,11
; 0000 0055 data=UDR;
	IN   R16,12
; 0000 0056 if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x3
; 0000 0057    {
; 0000 0058    rx_buffer[rx_wr_index++]=data;
	MOV  R30,R5
	INC  R5
	RCALL SUBOPT_0x0
	ST   Z,R16
; 0000 0059 #if RX_BUFFER_SIZE == 256
; 0000 005A    // special case for receiver buffer size=256
; 0000 005B    if (++rx_counter == 0) rx_buffer_overflow=1;
; 0000 005C #else
; 0000 005D    if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
	LDI  R30,LOW(12)
	CP   R30,R5
	BRNE _0x4
	CLR  R5
; 0000 005E    if (++rx_counter == RX_BUFFER_SIZE)
_0x4:
	INC  R7
	LDI  R30,LOW(12)
	CP   R30,R7
	BRNE _0x5
; 0000 005F       {
; 0000 0060       rx_counter=0;
	CLR  R7
; 0000 0061       rx_buffer_overflow=1;
	SET
	BLD  R2,0
; 0000 0062       }
; 0000 0063 #endif
; 0000 0064    }
_0x5:
; 0000 0065 }
_0x3:
	RCALL __LOADLOCR2P
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;
;#ifndef _DEBUG_TERMINAL_IO_
;// Get a character from the USART Receiver buffer
;#define _ALTERNATE_GETCHAR_
;#pragma used+
;char getchar(void)
; 0000 006C {
_getchar:
; 0000 006D char data;
; 0000 006E while (rx_counter==0);
	ST   -Y,R17
;	data -> R17
_0x6:
	TST  R7
	BREQ _0x6
; 0000 006F data=rx_buffer[rx_rd_index++];
	MOV  R30,R4
	INC  R4
	RCALL SUBOPT_0x0
	LD   R17,Z
; 0000 0070 #if RX_BUFFER_SIZE != 256
; 0000 0071 if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
	LDI  R30,LOW(12)
	CP   R30,R4
	BRNE _0x9
	CLR  R4
; 0000 0072 #endif
; 0000 0073 #asm("cli")
_0x9:
	cli
; 0000 0074 --rx_counter;
	DEC  R7
; 0000 0075 #asm("sei")
	sei
; 0000 0076 return data;
	MOV  R30,R17
	LD   R17,Y+
	RET
; 0000 0077 }
;#pragma used-
;#endif
;
;// Standard Input/Output functions
;#include <stdio.h>
;
;#define FIRST_ADC_INPUT 0
;#define LAST_ADC_INPUT 7
;unsigned char adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
;#define ADC_VREF_TYPE 0x20
;
;// ADC interrupt service routine
;// with auto input scanning
;interrupt [ADC_INT] void adc_isr(void)
; 0000 0086 {
_adc_isr:
	ST   -Y,R24
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	IN   R30,SREG
	ST   -Y,R30
; 0000 0087 static unsigned char input_index=0;
; 0000 0088 // Read the 8 most significant bits
; 0000 0089 // of the AD conversion result
; 0000 008A adc_data[input_index]=ADCH;
	LDS  R26,_input_index_S0000002000
	RCALL SUBOPT_0x1
	SUBI R26,LOW(-_adc_data)
	SBCI R27,HIGH(-_adc_data)
	IN   R30,0x5
	ST   X,R30
; 0000 008B // Select next ADC input
; 0000 008C if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
	LDS  R26,_input_index_S0000002000
	SUBI R26,-LOW(1)
	STS  _input_index_S0000002000,R26
	CPI  R26,LOW(0x8)
	BRLO _0xA
; 0000 008D    input_index=0;
	LDI  R30,LOW(0)
	STS  _input_index_S0000002000,R30
; 0000 008E ADMUX=(FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff))+input_index;
_0xA:
	LDS  R30,_input_index_S0000002000
	SUBI R30,-LOW(32)
	OUT  0x7,R30
; 0000 008F // Delay needed for the stabilization of the ADC input voltage
; 0000 0090 delay_us(10);
	__DELAY_USB 37
; 0000 0091 // Start the AD conversion
; 0000 0092 ADCSRA|=0x40;
	SBI  0x6,6
; 0000 0093 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R24,Y+
	RETI
;
;// Declare your global variables here
;#define Header1 '!'
;#define Header2 '@'
;#define Header3 '#'
;#define Header4 '$'
;#define Error   '?'
;#define Tail    '~'
;
;void KirimData8Bit(unsigned char x)
; 0000 009E {   putchar (x/100+48);
_KirimData8Bit:
	ST   -Y,R26
;	x -> Y+0
	LD   R26,Y
	RCALL SUBOPT_0x1
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	RCALL __DIVW21
	RCALL SUBOPT_0x2
; 0000 009F     putchar ((x/10)%10+48);
	RCALL SUBOPT_0x1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL __DIVW21
	MOVW R26,R30
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL __MODW21
	RCALL SUBOPT_0x2
; 0000 00A0     putchar (x%10+48);
	CLR  R27
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL __MODW21
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _putchar
; 0000 00A1 }
	RJMP _0x2060001
;
;void main(void)
; 0000 00A4 {
_main:
; 0000 00A5 // Declare your local variables here
; 0000 00A6 unsigned char dSerial;  // data yang dibaca dari buffer serial
; 0000 00A7 unsigned char xData[8]; // buffer data serial untuk proses parsing
; 0000 00A8 unsigned char index=0;  // data index dari buffer serial saat proses parsing
; 0000 00A9 bit flagHeader=0;       // penanda apakah header sudah tertangkap / belum saat parsing
; 0000 00AA unsigned char hasil;
; 0000 00AB 
; 0000 00AC // Input/Output Ports initialization
; 0000 00AD // Port A initialization
; 0000 00AE // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00AF // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 00B0 PORTA=0x00;
	SBIW R28,8
;	dSerial -> R17
;	xData -> Y+0
;	index -> R16
;	flagHeader -> R15.0
;	hasil -> R19
	CLR  R15
	LDI  R16,0
	LDI  R30,LOW(0)
	OUT  0x1B,R30
; 0000 00B1 DDRA=0x00;
	OUT  0x1A,R30
; 0000 00B2 
; 0000 00B3 // Port B initialization
; 0000 00B4 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
; 0000 00B5 // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
; 0000 00B6 PORTB=0x00;
	OUT  0x18,R30
; 0000 00B7 DDRB=0xFF;
	LDI  R30,LOW(255)
	OUT  0x17,R30
; 0000 00B8 
; 0000 00B9 // Port C initialization
; 0000 00BA // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 00BB // State7=P State6=P State5=P State4=P State3=P State2=P State1=P State0=P
; 0000 00BC PORTC=0xFF;
	OUT  0x15,R30
; 0000 00BD DDRC=0x00;
	LDI  R30,LOW(0)
	OUT  0x14,R30
; 0000 00BE 
; 0000 00BF // Port D initialization
; 0000 00C0 // Func7=In Func6=In Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
; 0000 00C1 // State7=T State6=T State5=0 State4=0 State3=T State2=T State1=T State0=T
; 0000 00C2 PORTD=0x00;
	OUT  0x12,R30
; 0000 00C3 DDRD=0x30;
	LDI  R30,LOW(48)
	OUT  0x11,R30
; 0000 00C4 
; 0000 00C5 // Timer/Counter 0 initialization
; 0000 00C6 // Clock source: System Clock
; 0000 00C7 // Clock value: Timer 0 Stopped
; 0000 00C8 // Mode: Normal top=0xFF
; 0000 00C9 // OC0 output: Disconnected
; 0000 00CA TCCR0=0x00;
	LDI  R30,LOW(0)
	OUT  0x33,R30
; 0000 00CB TCNT0=0x00;
	OUT  0x32,R30
; 0000 00CC OCR0=0x00;
	OUT  0x3C,R30
; 0000 00CD 
; 0000 00CE // Timer/Counter 1 initialization
; 0000 00CF // Clock source: System Clock
; 0000 00D0 // Clock value: 11059.200 kHz
; 0000 00D1 // Mode: Fast PWM top=0x00FF
; 0000 00D2 // OC1A output: Non-Inv.
; 0000 00D3 // OC1B output: Non-Inv.
; 0000 00D4 // Noise Canceler: Off
; 0000 00D5 // Input Capture on Falling Edge
; 0000 00D6 // Timer1 Overflow Interrupt: Off
; 0000 00D7 // Input Capture Interrupt: Off
; 0000 00D8 // Compare A Match Interrupt: Off
; 0000 00D9 // Compare B Match Interrupt: Off
; 0000 00DA TCCR1A=0xA1;
	LDI  R30,LOW(161)
	OUT  0x2F,R30
; 0000 00DB TCCR1B=0x09;
	LDI  R30,LOW(9)
	OUT  0x2E,R30
; 0000 00DC TCNT1H=0x00;
	LDI  R30,LOW(0)
	OUT  0x2D,R30
; 0000 00DD TCNT1L=0x00;
	OUT  0x2C,R30
; 0000 00DE ICR1H=0x00;
	OUT  0x27,R30
; 0000 00DF ICR1L=0x00;
	OUT  0x26,R30
; 0000 00E0 OCR1AH=0x00;
	OUT  0x2B,R30
; 0000 00E1 OCR1AL=0x00;
	OUT  0x2A,R30
; 0000 00E2 OCR1BH=0x00;
	OUT  0x29,R30
; 0000 00E3 OCR1BL=0x00;
	OUT  0x28,R30
; 0000 00E4 
; 0000 00E5 // Timer/Counter 2 initialization
; 0000 00E6 // Clock source: System Clock
; 0000 00E7 // Clock value: Timer2 Stopped
; 0000 00E8 // Mode: Normal top=0xFF
; 0000 00E9 // OC2 output: Disconnected
; 0000 00EA ASSR=0x00;
	OUT  0x22,R30
; 0000 00EB TCCR2=0x00;
	OUT  0x25,R30
; 0000 00EC TCNT2=0x00;
	OUT  0x24,R30
; 0000 00ED OCR2=0x00;
	OUT  0x23,R30
; 0000 00EE 
; 0000 00EF // External Interrupt(s) initialization
; 0000 00F0 // INT0: Off
; 0000 00F1 // INT1: Off
; 0000 00F2 // INT2: Off
; 0000 00F3 MCUCR=0x00;
	OUT  0x35,R30
; 0000 00F4 MCUCSR=0x00;
	OUT  0x34,R30
; 0000 00F5 
; 0000 00F6 // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 00F7 TIMSK=0x00;
	OUT  0x39,R30
; 0000 00F8 
; 0000 00F9 // USART initialization
; 0000 00FA // Communication Parameters: 8 Data, 1 Stop, No Parity
; 0000 00FB // USART Receiver: On
; 0000 00FC // USART Transmitter: On
; 0000 00FD // USART Mode: Asynchronous
; 0000 00FE // USART Baud Rate: 9600
; 0000 00FF UCSRA=0x00;
	OUT  0xB,R30
; 0000 0100 UCSRB=0x98;
	LDI  R30,LOW(152)
	OUT  0xA,R30
; 0000 0101 UCSRC=0x86;
	LDI  R30,LOW(134)
	OUT  0x20,R30
; 0000 0102 UBRRH=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
; 0000 0103 UBRRL=0x47;
	LDI  R30,LOW(71)
	OUT  0x9,R30
; 0000 0104 
; 0000 0105 // Analog Comparator initialization
; 0000 0106 // Analog Comparator: Off
; 0000 0107 // Analog Comparator Input Capture by Timer/Counter 1: Off
; 0000 0108 ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 0109 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x30,R30
; 0000 010A 
; 0000 010B // ADC initialization
; 0000 010C // ADC Clock frequency: 86.400 kHz
; 0000 010D // ADC Voltage Reference: AREF pin
; 0000 010E // ADC High Speed Mode: Off
; 0000 010F // ADC Auto Trigger Source: ADC Stopped
; 0000 0110 // Only the 8 most significant bits of
; 0000 0111 // the AD conversion result are used
; 0000 0112 ADMUX=FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff);
	LDI  R30,LOW(32)
	OUT  0x7,R30
; 0000 0113 ADCSRA=0xCF;
	LDI  R30,LOW(207)
	OUT  0x6,R30
; 0000 0114 SFIOR&=0xEF;
	IN   R30,0x30
	ANDI R30,0xEF
	OUT  0x30,R30
; 0000 0115 
; 0000 0116 // SPI initialization
; 0000 0117 // SPI disabled
; 0000 0118 SPCR=0x00;
	LDI  R30,LOW(0)
	OUT  0xD,R30
; 0000 0119 
; 0000 011A // TWI initialization
; 0000 011B // TWI disabled
; 0000 011C TWCR=0x00;
	OUT  0x36,R30
; 0000 011D 
; 0000 011E // Global enable interrupts
; 0000 011F #asm("sei")
	sei
; 0000 0120 
; 0000 0121 while (1)
_0xB:
; 0000 0122       {
; 0000 0123       // Place your code here
; 0000 0124       dSerial = getchar();    // membaca karakter dari buffer komunikasi serial
	RCALL _getchar
	MOV  R17,R30
; 0000 0125         if(flagHeader==0)     // jika header belum ketemu
	SBRC R15,0
	RJMP _0xE
; 0000 0126         {   if(dSerial==Header1 || dSerial==Header2 || dSerial==Header3 || dSerial==Header4)
	CPI  R17,33
	BREQ _0x10
	CPI  R17,64
	BREQ _0x10
	CPI  R17,35
	BREQ _0x10
	CPI  R17,36
	BRNE _0xF
_0x10:
; 0000 0127             {   index = 0;              // index direset
	LDI  R16,LOW(0)
; 0000 0128                 xData[index]=dSerial;   // header disimpan pada xData index ke-0
	RCALL SUBOPT_0x3
; 0000 0129                 flagHeader=1;           // flag header aktif
	SET
	BLD  R15,0
; 0000 012A             }
; 0000 012B         }
_0xF:
; 0000 012C         else                        	// jika header sudah ketemu
	RJMP _0x12
_0xE:
; 0000 012D         {   if (dSerial==Tail)      	// apakah tail data sudah ketemu?
	CPI  R17,126
	BREQ PC+2
	RJMP _0x13
; 0000 012E             {   // cek data dari header yang tertangkap
; 0000 012F                 switch(xData[0])
	LD   R30,Y
	RCALL SUBOPT_0x4
; 0000 0130                 {   case Header1:   	// digital input
	CPI  R30,LOW(0x21)
	LDI  R26,HIGH(0x21)
	CPC  R31,R26
	BRNE _0x17
; 0000 0131                         		// dengan data dipswitch di-invert menjadi active high
; 0000 0132                       	putchar(Header1);
	LDI  R26,LOW(33)
	RCALL _putchar
; 0000 0133 KirimData8Bit(~DipSwitch);
	IN   R30,0x13
	COM  R30
	MOV  R26,R30
	RCALL _KirimData8Bit
; 0000 0134                      	putchar(Tail);
	LDI  R26,LOW(126)
	RJMP _0x2F
; 0000 0135 break;
; 0000 0136                     case Header2:   	// digital output
_0x17:
	CPI  R30,LOW(0x40)
	LDI  R26,HIGH(0x40)
	CPC  R31,R26
	BRNE _0x18
; 0000 0137                         // parsing data dari string ke desimal
; 0000 0138                         if(index==3) 	// data benar apabila data =3 karakter
	CPI  R16,3
	BRNE _0x19
; 0000 0139                         {  	putchar(Header2);
	LDI  R26,LOW(64)
	RCALL _putchar
; 0000 013A   			     hasil = (xData[1]-48)*100;
	RCALL SUBOPT_0x5
; 0000 013B                             hasil+= (xData[2]-48)*10;
; 0000 013C                             hasil+= (xData[3]-48);
; 0000 013D                             // dengan data LED di-invert menjadi active high
; 0000 013E                             LED = ~hasil;
	COM  R30
	OUT  0x18,R30
; 0000 013F                         }
; 0000 0140                         else putchar(Error);
	RJMP _0x1A
_0x19:
	LDI  R26,LOW(63)
	RCALL _putchar
; 0000 0141                         break;
_0x1A:
	RJMP _0x16
; 0000 0142                     case Header3:
_0x18:
	CPI  R30,LOW(0x23)
	LDI  R26,HIGH(0x23)
	CPC  R31,R26
	BRNE _0x1B
; 0000 0143                         if(index==0)	// kalau membaca data semua kanal
	CPI  R16,0
	BRNE _0x1C
; 0000 0144                         {   putchar(Header3);
	LDI  R26,LOW(35)
	RCALL _putchar
; 0000 0145 	 KirimData8Bit(adc_data[0]);
	LDS  R26,_adc_data
	RCALL _KirimData8Bit
; 0000 0146                             KirimData8Bit(adc_data[1]);
	__GETB2MN _adc_data,1
	RCALL _KirimData8Bit
; 0000 0147                             KirimData8Bit(adc_data[2]);
	__GETB2MN _adc_data,2
	RCALL _KirimData8Bit
; 0000 0148                             KirimData8Bit(adc_data[3]);
	__GETB2MN _adc_data,3
	RCALL _KirimData8Bit
; 0000 0149                             KirimData8Bit(adc_data[4]);
	__GETB2MN _adc_data,4
	RCALL _KirimData8Bit
; 0000 014A                             KirimData8Bit(adc_data[5]);
	__GETB2MN _adc_data,5
	RCALL _KirimData8Bit
; 0000 014B                             KirimData8Bit(adc_data[6]);
	__GETB2MN _adc_data,6
	RCALL _KirimData8Bit
; 0000 014C                             KirimData8Bit(adc_data[7]);
	__GETB2MN _adc_data,7
	RCALL _KirimData8Bit
; 0000 014D 	 putchar(Tail);
	LDI  R26,LOW(126)
	RJMP _0x30
; 0000 014E                         }
; 0000 014F                         else if (xData[1]>='0' && xData[1]<='7')	// kalau salah satu kanal
_0x1C:
	LDD  R26,Y+1
	CPI  R26,LOW(0x30)
	BRLO _0x1F
	CPI  R26,LOW(0x38)
	BRLO _0x20
_0x1F:
	RJMP _0x1E
_0x20:
; 0000 0150                         {   putchar(Header3);
	LDI  R26,LOW(35)
	RCALL _putchar
; 0000 0151 	 		      KirimData8Bit(adc_data[xData[1]-48]);
	LDD  R30,Y+1
	RCALL SUBOPT_0x4
	SBIW R30,48
	SUBI R30,LOW(-_adc_data)
	SBCI R31,HIGH(-_adc_data)
	LD   R26,Z
	RCALL _KirimData8Bit
; 0000 0152 	 putchar(Tail);
	LDI  R26,LOW(126)
	RJMP _0x30
; 0000 0153                         }
; 0000 0154                         else putchar(Error);
_0x1E:
	LDI  R26,LOW(63)
_0x30:
	RCALL _putchar
; 0000 0155                         break;
	RJMP _0x16
; 0000 0156                     case Header4:
_0x1B:
	CPI  R30,LOW(0x24)
	LDI  R26,HIGH(0x24)
	CPC  R31,R26
	BRNE _0x16
; 0000 0157                         if(index==6)// menulis senua kanal
	CPI  R16,6
	BRNE _0x23
; 0000 0158                         {   putchar(Header4);
	LDI  R26,LOW(36)
	RCALL _putchar
; 0000 0159 	 		      hasil = (xData[1]-48)*100;
	RCALL SUBOPT_0x5
; 0000 015A                             hasil+= (xData[2]-48)*10;
; 0000 015B                             hasil+= (xData[3]-48);
; 0000 015C                             PwmA = hasil;
	RCALL SUBOPT_0x4
	OUT  0x2A+1,R31
	OUT  0x2A,R30
; 0000 015D                             hasil = (xData[4]-48)*100;
	LDD  R30,Y+4
	RCALL SUBOPT_0x6
; 0000 015E                             hasil+= (xData[5]-48)*10;
	LDD  R30,Y+5
	RCALL SUBOPT_0x7
; 0000 015F                             hasil+= (xData[6]-48);
	LDD  R30,Y+6
	SUBI R30,LOW(48)
	ADD  R19,R30
; 0000 0160                             PwmB = hasil;
	MOV  R30,R19
	RCALL SUBOPT_0x4
	OUT  0x28+1,R31
	OUT  0x28,R30
; 0000 0161                         }
; 0000 0162                         else if (xData[1]>='0' && xData[1]<='1')	// kalau salah satu kanal
	RJMP _0x24
_0x23:
	LDD  R26,Y+1
	CPI  R26,LOW(0x30)
	BRLO _0x26
	CPI  R26,LOW(0x32)
	BRLO _0x27
_0x26:
	RJMP _0x25
_0x27:
; 0000 0163                         {   if(index==4)
	CPI  R16,4
	BRNE _0x28
; 0000 0164                             {   putchar(Header4);
	LDI  R26,LOW(36)
	RCALL _putchar
; 0000 0165 	 		      	  hasil = (xData[2]-48)*100;
	LDD  R30,Y+2
	RCALL SUBOPT_0x6
; 0000 0166                                 hasil+= (xData[3]-48)*10;
	LDD  R30,Y+3
	RCALL SUBOPT_0x7
; 0000 0167                                 hasil+= (xData[4]-48);
	LDD  R30,Y+4
	SUBI R30,LOW(48)
	ADD  R19,R30
; 0000 0168                                 if (xData[1]=='0') PwmA = hasil;
	LDD  R26,Y+1
	CPI  R26,LOW(0x30)
	BRNE _0x29
	MOV  R30,R19
	RCALL SUBOPT_0x4
	OUT  0x2A+1,R31
	OUT  0x2A,R30
; 0000 0169                                 else               PwmB = hasil;
	RJMP _0x2A
_0x29:
	MOV  R30,R19
	RCALL SUBOPT_0x4
	OUT  0x28+1,R31
	OUT  0x28,R30
; 0000 016A                             }
_0x2A:
; 0000 016B                         }
_0x28:
; 0000 016C                         else putchar(Error);
	RJMP _0x2B
_0x25:
	LDI  R26,LOW(63)
_0x2F:
	RCALL _putchar
; 0000 016D                         break;
_0x2B:
_0x24:
; 0000 016E                 }
_0x16:
; 0000 016F                 flagHeader=0;	// flag disreset setiap ketemu tail (mengharap header baru)
	CLT
	BLD  R15,0
; 0000 0170             }
; 0000 0171             else           	// selain tail, maka data disimpan
	RJMP _0x2C
_0x13:
; 0000 0172             {   if(++index>8)  flagHeader=0;// asumsi data terlalu panjang(error),flag direset
	SUBI R16,-LOW(1)
	CPI  R16,9
	BRLO _0x2D
	CLT
	BLD  R15,0
; 0000 0173                 xData[index] = dSerial;     // selama belum error data disimpan
_0x2D:
	RCALL SUBOPT_0x3
; 0000 0174             }
_0x2C:
; 0000 0175         }
_0x12:
; 0000 0176 
; 0000 0177       }
	RJMP _0xB
; 0000 0178 }
_0x2E:
	RJMP _0x2E
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

	.CSEG
_putchar:
	ST   -Y,R26
putchar0:
     sbis usr,udre
     rjmp putchar0
     ld   r30,y
     out  udr,r30
_0x2060001:
	ADIW R28,1
	RET

	.CSEG

	.CSEG

	.DSEG
_rx_buffer:
	.BYTE 0xC
_adc_data:
	.BYTE 0x8
_input_index_S0000002000:
	.BYTE 0x1

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x0:
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer)
	SBCI R31,HIGH(-_rx_buffer)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x1:
	LDI  R27,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2:
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _putchar
	LD   R26,Y
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x3:
	MOV  R30,R16
	LDI  R31,0
	MOVW R26,R28
	ADD  R30,R26
	ADC  R31,R27
	ST   Z,R17
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x4:
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x5:
	LDD  R30,Y+1
	SUBI R30,LOW(48)
	LDI  R26,LOW(100)
	MULS R30,R26
	MOV  R19,R0
	LDD  R30,Y+2
	SUBI R30,LOW(48)
	LDI  R26,LOW(10)
	MULS R30,R26
	MOVW R30,R0
	ADD  R19,R30
	LDD  R30,Y+3
	SUBI R30,LOW(48)
	ADD  R19,R30
	MOV  R30,R19
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6:
	SUBI R30,LOW(48)
	LDI  R26,LOW(100)
	MULS R30,R26
	MOV  R19,R0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x7:
	SUBI R30,LOW(48)
	LDI  R26,LOW(10)
	MULS R30,R26
	MOVW R30,R0
	ADD  R19,R30
	RET


	.CSEG
__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
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

__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR2P:
	LD   R16,Y+
	LD   R17,Y+
	RET

;END OF CODE MARKER
__END_OF_CODE:
