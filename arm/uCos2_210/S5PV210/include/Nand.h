#ifndef __NAND_H
#define __NAND_H

////////////////////////////// 8-bit ////////////////////////////////
// Main function


// Sub function

void NF8_Program(void);
void PrintSubMessage(void);
void Test_NF8_Rw(void);
void Test_NF8_Page_Write(void);
void Test_NF8_Page_Read(void);
void Test_NF8_Block_Erase(void);
void NF8_PrintBadBlockNum(void);
void Test_NF8_Lock(void);
void Test_NF8_SoftUnLock(void);

U8 Read_Status(void);

//*************** H/W dependent functions ***************
// Assembler code for speed
/*
void __RdPage512(U8 *pPage);
void Nand_Reset(void);
void InputTargetBlock(void);

void NF8_Print_Id(void);
static U16 NF8_CheckId(void);
static int NF8_EraseBlock(U32 blockNum);
static int NF8_ReadPage(U32 block,U32 page,U8 *buffer);
static int NF8_WritePage(U32 block,U32 page,U8 *buffer);
static int NF8_IsBadBlock(U32 block);
static int NF8_MarkBadBlock(U32 block);
static void NF8_Init(void);
*/
//*******************************************************



#endif /*__NAND_H*/

