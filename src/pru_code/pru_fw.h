#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include "resource_table_empty.h"
#include <pru_ctrl.h>

#define SHARED_RAM 0x00010000 //offset of PRU shared mem
#define ROWS 960  //rows per image
#define COLS 1280 //pixels per row

// R31 image sync signal bit definitions
#define CLK_BIT 16
#define CLK_MASK 1U<<CLK_BIT
#define VSYNC_BIT 15 
#define VSYNC_MASK 1U<<VSYNC_BIT
#define HSYNC_BIT 14
#define HSYNC_MASK 1U<<HSYNC_BIT

// R31 inter-PRU interrupt bit definitions
#define PRU1_TO_PRU0_R31_BIT 30
#define PRU1_TO_PRU0_R31_MASK 1U<<PRU1_TO_PRU0_R31_BIT
#define PRU0_TO_PRU1_R31_BIT 31
#define PRU0_TO_PRU1_R31_MASK 1U<<PRU0_TO_PRU1_R31_BIT

#define PRU1_TO_PRU0_EVENT 16
#define PRU0_TO_PRU1_EVENT 17
#define PRU_TO_KERNEL_EVENT 20

// PRU INTC base address constant table offset
#define INTC_CO_TABLE_ENTRY C0
#define SICR_REG_OFFSET 0x24

// R31 bit 5 enables the interrupt number written to bits 4:0    
#define SYS_EVT_ENABLE 1<<5

// The number written to R31 will trigger the corresponding
// system events 16-31
#define SYS_EVT_20_TRIGGER (SYS_EVT_ENABLE | 0x04)
#define SYS_EVT_16_TRIGGER (SYS_EVT_ENABLE | 0x00)
#define SYS_EVT_17_TRIGGER (SYS_EVT_ENABLE | 0x01)

#define SCRATCHPAD_BANK_0 10

// number of bytes per transfer chunk
#define CHUNK_SIZE 32

// TODO assert that COLS/CHUNK_SIZE is evenly divisible
// the number of chunks(transfers) per image line 
#define LINE_CHUNKS (COLS / CHUNK_SIZE)

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void initPRU();

// pru_shared_vars_t is a struct that defines variables shared between the PRU
// cores. It must be declared and mapped to a known address in both PRU FWs
struct pru_shared_vars_t {
  uint8_t buf0[COLS]; // shared line buffer
};

// define the base address of the shared var struct
#define SHARED_VAR_ADDR (struct pru_shared_vars_t*)(SHARED_RAM + 0x100);

