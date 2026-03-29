/* SPDX-License-Identifier: MIT */
/* Copyright 2021-2024 Hewlett Packard Enterprise Development LP */

/* This file defines a set of commands and  request and response packet formats for
 * communicating with the Cassini uC over USB, SMBus, or the HSN.
 */

#ifndef CUC_CXI_H
#define CUC_CXI_H

#define CUC_DATA_BYTES  253

// I2C reads and writes to QSFP use a bus number of 0xFF because
// the actual bus number may not be known by the requester
#define QSFP_I2C_BUS_REDIRECT      0xFF

/**
 * struct cuc_pkt - Cassini uC Packet Format
 *
 * This common packet format can be used to communicate with the uC across different
 * physical interfaces (USB, SMBus, HSN). The SMBus interface is the lowest
 * common denominator so this is pretty much modeled after the block read/write
 * functions. We'll leave integrity checks to the physical interfaces.
 */
struct cuc_pkt {
	u8 cmd;                   /* The command to be sent */
	u8 count;                 /* The number of bytes to follow (including 'type') */
	u8 type;                  /* Packet type */
	u8 data[CUC_DATA_BYTES];  /* Data to be sent */
} __packed;

enum {
	CUC_CMD_PING = 0,                       /* Perform a simple ping to the uC firmware */
	CUC_CMD_BOARD_INFO = 1,                 /* Get board identification info */
	CUC_CMD_I2C_READ = 2,                   /* Read an i2c device on the Cassini uC */
	CUC_CMD_I2C_WRITE = 3,                  /* Write an i2c device on the Cassini uC */
	CUC_CMD_GET_LOG = 12,                   /* Get a log message from the uC */
	CUC_CMD_GET_FRU = 25,                   /* Get the FRU information for the device */
	CUC_CMD_SET_FAN_PWM = 26,               /* Set fan to fixed speed w/ given PWM duty cycle */
	CUC_CMD_GET_FAN_RPM = 27,               /* Get the latest fan RPM value */
	CUC_CMD_GET_MAC = 29,                   /* Get MAC addresses */
	CUC_CMD_QSFP_READ = 31,                 /* Do a QSFP/AOC I2C Read */
	CUC_CMD_QSFP_WRITE = 32,                /* Do a QSFP/AOC I2C Write */
	CUC_CMD_QSFP_RESET = 33,                /* Do a QSFP/AOC module reset */
	CUC_CMD_GET_INTR = 34,                  /* Get Interrupt Status and Enable bits for a NIC */
	CUC_CMD_CLEAR_ISR = 35,                 /* Clear Interrupt Status bits for a NIC */
	CUC_CMD_UPDATE_IER = 36,                /* Set/Clear Interrupt Enable bits for a NIC */
	CUC_CMD_PLDM = 39,                      /* Generic entry point for PLDM transaction */
	CUC_CMD_FIRMWARE_UPDATE_START = 46,     /* Update firmware */
	CUC_CMD_FIRMWARE_VERSION = 51,          /* Get the version of a firmware component */
	CUC_CMD_FIRMWARE_UPDATE_DOWNLOAD = 52,  /* Download the firmware */
	CUC_CMD_FIRMWARE_UPDATE_STATUS = 53,    /* Get status of firmware update */
	CUC_CMD_RESET = 54,                     /* Reset the uC */
	CUC_CMD_SET_LED = 58,                   /* Control the blink pattern (state) of an LED */
	CUC_CMD_GET_NIC_ID = 60,                /* Get the NIC ID associated with this interface */
	CUC_CMD_GET_TIMINGS = 61,               /* Get the power-on and initialization timings in usec */
};

enum {
	CUC_TYPE_REQ,          /* Request packet */
	CUC_TYPE_RSP_SUCCESS,  /* Success response packet */
	CUC_TYPE_RSP_ERROR,    /* Error response packet */
	CUC_TYPE_RSP_PLDM,     /* PLDM response packet */
};

struct cuc_error_rsp_data {
	u8 error;  /* The POSIX errno that best describes the error */
};

enum {
	CUC_BOARD_TYPE_UNKNOWN = -1,
	CUC_BOARD_TYPE_SAWTOOTH = 0,
	CUC_BOARD_TYPE_BRAZOS = 1,
	CUC_BOARD_TYPE_WASHINGTON = 2,
	CUC_BOARD_TYPE_KENNEBEC = 3,
	CUC_BOARD_TYPE_PANGANI = 4,
	CUC_BOARD_TYPE_SOUHEGAN = 5,
};

#define is_cas1_board_type(bt) ( (bt == CUC_BOARD_TYPE_SAWTOOTH) || \
								 (bt == CUC_BOARD_TYPE_BRAZOS) )
#define is_cas2_board_type(bt) ( (bt == CUC_BOARD_TYPE_WASHINGTON) || \
								 (bt == CUC_BOARD_TYPE_KENNEBEC) || \
								 (bt == CUC_BOARD_TYPE_PANGANI) || \
								 (bt == CUC_BOARD_TYPE_SOUHEGAN) )
struct cuc_board_info_rsp {
	u8 board_type;
	u8 board_rev;
} __packed;

enum {
    I2C_CURRENT_ADDR_READ = 0,
    I2C_RANDOM_ADDR8_READ,
    I2C_RANDOM_ADDR16_READ,
};

struct cuc_i2c_read_req {
    uint8_t bus;        /* The i2c bus number */
    uint8_t addr;       /* The 7-bit i2c slave address */
    uint8_t type;       /* Transaction type (current/random8/random16 read) */
    uint8_t count;      /* Number of bytes to read/write */
    uint16_t offset;    /* Offset in slave device to read from (if random) */
}__attribute__((packed));

struct cuc_i2c_write_req {
    uint8_t bus;        /* The i2c bus number */
    uint8_t addr;       /* The 7-bit i2c slave address */
    uint8_t count;      /* Number of bytes to write */
    uint8_t buf[0];
}__attribute__((packed));

struct cuc_set_fan_pwm_req_data {
	/* PWM duty cycle value to set
	 * 0- 100: Set to manual speed control
	 * 255: Set to auto speed control
	 */
	u8 percent;
} __packed;

struct cuc_get_fan_rpm_rsp_data {
	u32 rpm;     /* Measured fan speed in RPM */
	u8 percent;  /* Current value of PWM duty cycle percent */
	u8 is_auto;  /* Non-zero indicates auto speed control is active */
} __packed;

/* This is a special value that can be passed in the "nic" field of the
 * cuc_mac_req_data structure to indicate to the uC that we want to get the
 * MAC of the NIC that made the request
 */
#define CUC_MAC_THIS_NIC  0xFF

struct cuc_mac_req_data {
	u8 nic;  /* The NIC whose MAC addresses we want */
} __packed;

struct cuc_mac_rsp_data {
	u8 nic;         /* The NIC */
	u8 nic_mac[6];  /* The NIC MAC address */
	u8 uc_mac[6];   /* The uC MAC address */
} __packed;

struct cuc_qsfp_read_req_data {
	u8 nic;    /* Which NIC QSFP device to read */
	u8 page;   /* Page select for reading */
	u8 addr;   /* Start address for reading */
	u8 count;  /* Number of bytes to read */
} __packed;

struct cuc_qsfp_read_rsp_data {
	u8 data[0];
} __packed;

struct cuc_qsfp_write_req_data {
	u8 nic;      /* Which NIC QSFP device to write */
	u8 page;     /* Page select for writing */
	u8 addr;     /* Start address for writing */
	u8 count;    /* Number of bytes to write */
	u8 data[];   /* Data to be written */
} __packed;

struct cuc_qsfp_reset_req_data {
	u8 nic;  /* Which NIC QSFP device to reset */
} __packed;

/* These are Interrupt Masks for interrupt sources that cause UC_ATTENTION[1] to be set in
 * C_PI_ERR_FLG. Some of these interrupt sources are related to the ASIC power-off condition, in
 * which case it won't be possible to generate the UC_ATTENTION[1] interrupt, but the interrupt
 * status and mask values are still valid and maintained via the CasuC/CUC interface.
 */
/* Bits 0 through 7 are reserved for uC and board-related interrupts */
#define ATT1_UC_RESET                   BIT(0)
#define ATT1_SENSOR_ALERT               BIT(1)
#define ATT1_FAN_FAIL                   BIT(2)
/* Bits 8 through 15 are reserved for asic-related interrupts */
#define ATT1_ASIC_PWR_UP_DONE           BIT(8)
#define ATT1_ASIC_PWR_FAIL              BIT(9)
#define ATT1_ASIC_EPO_TEMPERATURE       BIT(10)
/* Bits 16 through 23 are reserved for qsfp-related interrupts */
#define ATT1_QSFP_POWER_FAIL            BIT(16)
#define ATT1_QSFP_INSERT                BIT(17)
#define ATT1_QSFP_REMOVE                BIT(18)
#define ATT1_QSFP_INT                   BIT(19)
#define ATT1_QSFP_EPO_TEMPERATURE       BIT(20)
#define ATT1_QSFP_BAD_CABLE             BIT(21)
/* Bits 24 through 31 are reserved for other future use */
#define ATT1_ALL_INTERRUPTS             (0xFFFFFFFF)

/* Most ATT1 interrupts are clearable by the host. Others are hardware-based and self-clearing */
#define HOST_CLEARED_ATT1_INTERRUPTS  (ATT1_ALL_INTERRUPTS & ~(ATT1_QSFP_INT))

struct cuc_get_intr_req_data {
	u8 nic;  /* The NIC whose interrupts are being requested */
} __packed;

struct cuc_get_intr_rsp_data {
	u32 isr;  /* Interrupt status register */
	u32 ier;  /* Interrupt enable register */
} __packed;

struct cuc_clear_isr_req_data {
	u8 nic;              /* The NIC whose interrupts are being cleared */
	u32 isr_clear_bits;  /* Interrupt status bits to clear */
} __packed;

struct cuc_update_ier_req_data {
	u8 nic;              /* The NIC whose interrupt enables are being updated */
	u32 ier_set_bits;    /* Interrupt bits to enable */
	u32 ier_clear_bits;  /* Interrupt bits to disable */
} __packed;

/* Two copies of the firmware are stored for each NIC, an active copy and a recovery copy */
enum {
	FW_SLOT_0,
	FW_SLOT_ACTIVE = FW_SLOT_0,
	FW_SLOT_1,
	FW_SLOT_RECOVERY = FW_SLOT_1,
	FW_SLOT_MAX
};

struct cuc_firmware_update_start_req {
	u8 nic;    /* The NIC whose firmware is to be updated */
	u32 size;  /* The size of the new firmware */
	u8 slot;   /* The firmware slot to update */
} __packed;

/* The firmware (aka QSPI_BLOB) is composed of multiple, separately versioned components
 * Not all of these targets are included in each blob. Blob contents are HW-specific. */
enum casuc_fw_target {
	FW_UC_APPLICATION,
	FW_UC_BOOTLOADER,
	FW_QSPI_BLOB,
	FW_OPROM,
	FW_CSR1,
	FW_CSR2,
	FW_SRDS,
	FW_ISL68124_SAW,
	FW_ISL68124_BRZ,
	FW_IR38060_QSFP_BRZ,
	FW_IR38060 = FW_IR38060_QSFP_BRZ,
	/* Start of CAS2-specific entries */
	FW_TDA38740_WAS,
	FW_IR38060_WAS,
	FW_IR38063_WAS,
	FW_TDA38740_KEN,
	FW_IR38060_KEN,
	FW_IR38063_KEN,
	FW_RESERVED, /* deprecated - FW_IR38060_QSFP_KEN */
	FW_MFPGA_WAS,
        FW_TDA38740_SOU,
        FW_IR38063_0_SOU,
        FW_IR38063_1_SOU,
        FW_IR38063_2_SOU,
        FW_MFPGA_SOU,
	FW_NUM_ENTRIES
};

#define CAS1_BLOB_FW_TARGETS	FW_UC_APPLICATION, \
								FW_UC_BOOTLOADER, \
								FW_QSPI_BLOB, \
								FW_OPROM, \
								FW_CSR1, \
								FW_CSR2, \
								FW_SRDS, \
								FW_ISL68124_SAW, \
								FW_ISL68124_BRZ, \
								FW_IR38060_QSFP_BRZ

#define CAS2_BLOB_FW_TARGETS	FW_UC_APPLICATION, \
								FW_UC_BOOTLOADER, \
								FW_QSPI_BLOB, \
								FW_OPROM, \
								FW_CSR1, \
								FW_CSR2, \
								FW_SRDS, \
								FW_TDA38740_WAS, \
								FW_IR38060_WAS, \
								FW_IR38063_WAS, \
								FW_TDA38740_KEN, \
								FW_IR38060_KEN, \
								FW_IR38063_KEN, \
								FW_MFPGA_WAS, \
								FW_TDA38740_SOU, \
								FW_IR38063_0_SOU, \
								FW_IR38063_1_SOU, \
								FW_IR38063_2_SOU, \
								FW_MFPGA_SOU


struct cuc_get_firmware_version_req {
	u8 fw_target;   /* The component whose version is being requested */
	u8 nic;         /* The NIC whose firmware version is being requested */
	u8 from_flash;  /* 0 = Get the running version, 1 = Get the stored version */
	u8 slot;        /* The slot to get the version from, when getting stored version */
} __packed;

struct cuc_firmware_update_download_req {
	u8 data[0];
} __packed;

enum {
	FWU_STATUS_STARTED,             /* The firmware update has been started */
	FWU_STATUS_DOWNLOADING,         /* The firmware is being downloaded */
	FWU_STATUS_VERIFYING_SIGNATURE, /* The firmware signature is being validated */
	FWU_STATUS_VALIDATING_IMAGE,    /* The firmware is being validated */
	FWU_STATUS_FLASHING,            /* The firmware is being flashed */
	FWU_STATUS_VERIFYING_FLASH,     /* The firmware flash is being verified */
	FWU_STATUS_IDLE = 0x80,
	FWU_STATUS_SUCCESS,             /* The firmware update succeeded */
	FWU_STATUS_FAILED,              /* The firmware update failed */
	FWU_STATUS_FAILED_DOWNLOAD,     /* The firmware download failed */
	FWU_STATUS_FAILED_BAD_SIGN,     /* The firmware signature was incorrect */
	FWU_STATUS_FAILED_VALIDATION,   /* Failure validating image before flashing */
	FWU_STATUS_FAILED_FLASH,        /* Failure during flash */
	FWU_STATUS_FAILED_VERIFICATION, /* Failure verifying image in flash */
	FWU_STATUS_FAILED_INVALID_SLOT, /* The firmware slot is not valid */
};

struct cuc_firmware_update_status_rsp {
	u8 status;  /* Status of the firmware update state-machine */
} __packed;

enum casuc_leds {
	LED_LINK_STATUS = 0,
	LED_OCP_LINK_STATUS = 1,
	LED_OCP_ACTIVITY_STATUS = 2,
};

/* GRN represents the nominal color and YEL represents the alternate color. Slow blinking
 * frequency is 1Hz, fast is 2Hz.
 */
enum casuc_led_states {
	LED_OFF = 0,
	LED_ON_GRN = 1,
	LED_SLOW_GRN = 2,
	LED_FAST_GRN = 3,
	LED_ON_YEL = 4,
	LED_SLOW_YEL = 5,
	LED_FAST_YEL = 6,
	LED_SLOW_GRN_YEL = 7,
	LED_FAST_GRN_YEL = 8,
};

struct cuc_set_led_req {
	u8 nic;    /* The NIC whose LED state is to be set */
	u8 led;    /* The LED whose state is to be set */
	u8 state;  /* The state to set */
} __packed;

struct cuc_get_nic_id_rsp {
	u8 nic;  /* The requested NIC ID */
} __packed;

enum cuc_timing_entries {
    TIMING_UC_APPLICATION_STARTED,
    TIMING_UC_PIN_INIT_COMPLETE,
    TIMING_UC_FW_INIT_COMPLETE,
    TIMING_EN_CLKS_UC_ASSERTED,
    TIMING_12V_PG,
    TIMING_PG_CASSINI_ASSERTED,
    TIMING_RST_PON_NIC_N_DEASSERTED,
    TIMING_VID_STABLE_ASSERTED,
    TIMING_PERST_NIC_0_N_DEASSERTED,
    TIMING_PERST_NIC_1_N_DEASSERTED,
    TIMING_JTAG_TRST_N_DEASSERTED,
    TIMING_UC_CASSINI_RDY_NIC_0,
    TIMING_UC_CASSINI_RDY_NIC_1,
    TIMING_PCIE_LINK_UP_NIC_0,
    TIMING_PCIE_LINK_UP_NIC_1,
    TIMING_UPTIME,
    TIMING_NUM_ENTRIES
};

struct cuc_get_timings_rsp {
    uint64_t entries_us[TIMING_NUM_ENTRIES];
} __attribute__((packed));

#endif /* CUC_CXI_H */
