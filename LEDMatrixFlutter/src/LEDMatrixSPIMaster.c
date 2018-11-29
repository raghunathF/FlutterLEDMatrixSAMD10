/*
 * LEDMatrixSPIMaster.c
 *
 * Created: 11/8/2018 10:01:09 AM
 *  Author: raghu
 */ 
#include <asf.h>
#include "LEDMatrixSPIMaster.h"

#define NUMBER_SHIFTREGISTERS			13

COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor SECTION_DMAC_DESCRIPTOR;

struct spi_module spi_master_instance;
struct dma_resource example_resource;

extern volatile bool transferDone;
extern volatile uint8_t DMASourceRegister[NUMBER_SHIFTREGISTERS]; 

static void transfer_done(struct dma_resource* const resource )
{
	transferDone = true;
}

void setUpDMAData(uint8_t* receiveData)
{
	uint8_t i =0;
	for(i=0;i<NUMBER_SHIFTREGISTERS;i++)
	{
		DMASourceRegister[i] = receiveData[i];
	}
}

void startTransferDMA()
{

	dma_start_transfer_job(&example_resource);
	while(transferDone == false);
	transferDone = false;
	
}

void configureSPIMaster(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.data_order       = SPI_DATA_ORDER_LSB;
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_1;
	config_spi_master.mux_setting = CONF_MASTER_MUX_SETTING;
	config_spi_master.pinmux_pad0 = CONF_MASTER_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = CONF_MASTER_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = CONF_MASTER_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = CONF_MASTER_PINMUX_PAD3;
	config_spi_master.receiver_enable  = false;
	config_spi_master.mode_specific.master.baudrate =  SPIBAUDRATE; //2.5MHz  = 400ns per signal
	config_spi_master.generator_source = GCLK_GENERATOR_3;
	spi_init(&spi_master_instance, CONF_MASTER_SPI_MODULE, &config_spi_master);
	spi_enable(&spi_master_instance);
}

static void configure_dma_resource(struct dma_resource *resource)
{
	struct dma_resource_config config;
	dma_get_config_defaults(&config);
	config.peripheral_trigger = SERCOM1_DMAC_ID_TX;
	config.trigger_action = DMA_TRIGGER_ACTION_BEAT;
	dma_allocate(resource, &config);
}

static void setup_transfer_descriptor(DmacDescriptor *descriptor )
{
	uint16_t blockTransferCount = NUMBER_SHIFTREGISTERS;
	struct dma_descriptor_config descriptor_config;
	dma_descriptor_get_config_defaults(&descriptor_config);
	descriptor_config.beat_size = DMA_BEAT_SIZE_BYTE;
	descriptor_config.dst_increment_enable		= false;
	descriptor_config.src_increment_enable		= true;
	descriptor_config.block_transfer_count		= blockTransferCount;
	descriptor_config.source_address			= (uint32_t)DMASourceRegister+blockTransferCount;
	descriptor_config.destination_address		= 0x42000C28;
	descriptor_config.next_descriptor_address	= 0;
	dma_descriptor_create(descriptor, &descriptor_config);
}

void setupDMA()
{
	configure_dma_resource(&example_resource);
	setup_transfer_descriptor(&example_descriptor);
	dma_add_descriptor(&example_resource, &example_descriptor);
	dma_register_callback(&example_resource, transfer_done, DMA_CALLBACK_TRANSFER_DONE);
	dma_enable_callback(&example_resource, DMA_CALLBACK_TRANSFER_DONE);
}

void SPIMasterInit()
{
	configureSPIMaster();
	setupDMA();
}
