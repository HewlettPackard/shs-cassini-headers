/* SPDX-License-Identifier: MIT */
/* Copyright 2021 Hewlett Packard Enterprise Development LP */

/* This file contains Platform Level Data Model (PLDM) definitions
 * necessary to read numeric sensors and parse FRU record data from
 * the Cassini uC PLDM subsystem.
 *
 * More information can be found in the following DMTF documents
 * (https://www.dmtf.org):
 * DSP0240 v1.0.0: PLDM Base Specification
 * DSP0245 v1.3.0: PLDM IDs and Codes Specification
 * DSP0248 v1.2.0: PLDM for Platform Monitoring and Control Specification
 * DSP0257 v1.0.0: PLDM for FRU Data Specification
 */

#ifndef PLDM_CXI_H
#define PLDM_CXI_H

#define C_MASK(XVAL, XBIT) XVAL##_MASK = (1 << XBIT)
#define C_ENUM(XNAME, XVAL) XNAME = XVAL

#define PLDM_THRESHOLD_LIST \
    C(PLDM_THRESHOLD_UPPER_WARNING, 0), \
    C(PLDM_THRESHOLD_UPPER_CRITICAL, 1), \
    C(PLDM_THRESHOLD_UPPER_FATAL, 2), \
    C(PLDM_THRESHOLD_LOWER_WARNING, 3), \
    C(PLDM_THRESHOLD_LOWER_CRITICAL, 4), \
    C(PLDM_THRESHOLD_LOWER_FATAL, 5)

#define C C_ENUM
enum pldm_threshold {
    PLDM_THRESHOLD_LIST,
    PLDM_THRESHOLD_COUNT
};
#undef C

#define C C_MASK
enum {
    PLDM_THRESHOLD_LIST
};
#undef C

/* Generic PLDM Message Fields (DSP0240 Figure 1) */
struct pldm_hdr {
	u8 instance_id:5;
	u8 rsvd:1;
	u8 d:1;
	u8 rq:1;

	u8 pldm_type:6;
	u8 hdr_ver:2;

	u8 pldm_command_code;
	u8 payload[0];
} __packed;

/* PLDM Type Codes (DSP0245 Table 1) */
enum pldm_type_code {
	PLDM_TYPE_MESSAGING_CONTROL_AND_DISCOVERY = 0,
	PLDM_TYPE_SMBIOS,
	PLDM_TYPE_PLATFORM_MONITORING_AND_CONTROL,
	PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION,
	PLDM_TYPE_FRU_DATA,
	PLDM_TYPE_FIRMWARE_UPDATE,
	PLDM_TYPE_REDFISH_DEVICE,
	PLDM_TYPE_OEM = 0x3F,
	PLDM_TYPE_COUNT
};

/* Generic PLDM Completion Codes (DSP0240 Table 4) */
enum pldm_completion_code {
	PLDM_SUCCESS = 0,
	PLDM_ERROR = 1,
	PLDM_ERROR_INVALID_DATA = 2,
	PLDM_ERROR_INVALID_LENGTH = 3,
	PLDM_ERROR_NOT_READY = 4,
	PLDM_ERROR_UNSUPPORTED_CMD = 5,
	PLDM_ERROR_INVALID_PLDM_TYPE = 6,
	PLDM_ERROR_COMMAND_SPECIFIC_START = 0x80,
	PLDM_ERROR_COMMAND_SPECIFIC_END = 0xFF
};

/* PLDM Command Numbers (DSP0248 Table 107) */
enum pldm_platform_cmd {
	PLDM_CMD_GET_SENSOR_READING = 0x11,
	PLDM_CMD_GET_PDR = 0x51
};

enum pldm_sensor_opstate {
    PLDM_OPSTATE_ENABLED = 0,
    PLDM_OPSTATE_DISABLED = 1,
    PLDM_OPSTATE_UNAVAILABLE = 2,
    PLDM_OPSTATE_STATUS_UNKNOWN = 3,
    PLDM_OPSTATE_FAILED = 4,
    PLDM_OPSTATE_INITIALIZING = 5,
    PLDM_OPSTATE_SHUTTING_DOWN = 6,
    PLDM_OPSTATE_IN_TEST = 7
};

/* PLDM Sensor Units (Table 62 of DSP0248) */
enum sensor_unit {
    PLDM_UNIT_NONE = 0,
    PLDM_UNIT_DEGREES_C = 2,
    PLDM_UNIT_VOLTS = 5,
    PLDM_UNIT_AMPS = 6,
    PLDM_UNIT_WATTS = 7,
};

enum unit_modifier {
    PLDM_MODIFIER_NANO = -9,
    PLDM_MODIFIER_MICRO = -6,
    PLDM_MODIFIER_MILLI = -3,
    PLDM_MODIFIER_CENTI = -2,
    PLDM_MODIFIER_DECI = -1,
    PLDM_MODIFIER_NONE = 0,
    PLDM_MODIFIER_KILO = 3,
};

/* GetSensorReading presentReading variable types (DSP0248 Table 30) */
union sensor_value {
	s8 value_SINT8;
	u8 value_UINT8;
	s16 value_SINT16;
	u16 value_UINT16;
	s32 value_SINT32;
	u32 value_UINT32;
};

/* GetSensorReading Command Request Format (DSP0248 Table 30) */
struct get_sensor_reading_req {
	struct pldm_hdr hdr;
	u16 sensor_id;
	u8 rearm_event_status;
} __packed;

/* GetSensorReading Command Response Format (DSP0248 Table 30) */
struct get_sensor_reading_rsp {
	struct pldm_hdr hdr;
	u8 completion_code;
	u8 sensor_data_size;
	u8 sensor_operational_state;
	u8 sensor_event_message_enable;
	u8 present_state;
	u8 previous_state;
	u8 event_state;
	union sensor_value present_reading;
} __packed;

/* GetPDR Command Request Format (DSP0248 Table 68) */
struct get_pdr_req {
	struct pldm_hdr hdr;
	u32 record_handle;
	u32 data_transfer_handle;
	u8 transfer_operation_flag;
	u16 request_count;
	u16 record_change_number;
} __packed;

/* GetPDR Command Response Format (DSP0248 Table 68) */
struct get_pdr_rsp {
	struct pldm_hdr hdr;
	u8 completion_code;
	u32 next_record_handle;
	u32 next_data_transfer_handle;
	u8 transfer_flag;
	u16 response_count;
	u8 record_data[];
} __packed;

/* GetPDR transferOperationFlag values (DSP0248 Table 68) */
enum pldm_transfer_op {
	PLDM_XFER_OP_GET_NEXT_PART = 0,
	PLDM_XFER_OP_GET_FIRST_PART = 1
};

/* Common PDR Header Format (DSP0248 Table 75) */
struct pdr_hdr {
	u32 record_handle;
	u8 pdr_header_version;
	u8 pdr_type;
	u16 record_change_number;
	u16 data_length;
	u8 data[0];
} __packed;

/* PDR Type Values (DSP0248 Table 76) */
enum pdr_type {
	PLDM_PDR_TERMINUS_LOCATOR = 1,
	PLDM_PDR_NUMERIC_SENSOR = 2,
	PLDM_PDR_STATE_SENSOR = 4,
	PLDM_PDR_SENSOR_AUXILIARY_NAMES = 6,
	PLDM_PDR_STATE_EFFECTER = 11,
	PLDM_PDR_EFFECTER_AUXILIARY_NAMES = 13,
	PLDM_PDR_ENTITY_ASSOCIATION = 15,
	PLDM_PDR_ENTITY_AUXILIARY_NAMES = 16,
	PLDM_PDR_FRU_RECORD_SET = 20,
	PLDM_PDR_OEM = 127
};

/* Numeric Sensor PDR sensorDataSize values (DSP0248 Table 78) */
enum pldm_data_size {
	PLDM_DATA_SIZE_UINT8,
	PLDM_DATA_SIZE_SINT8,
	PLDM_DATA_SIZE_UINT16,
	PLDM_DATA_SIZE_SINT16,
	PLDM_DATA_SIZE_UINT32,
	PLDM_DATA_SIZE_SINT32,
};

/* The Numeric Sensor PDR has variably sized fields depending on the size of the sensor reading.
 * To keep the structure packing as simple as possible, unions are used for everything downstream
 * of the first variably sized field. These sensor size dependent structs are only defined for a
 * given size (not signedness). They include all Numeric Sensor PDR fields from 'hysteresis' to
 * 'fatal_low' (DSP0248 Table 78)
 */
struct numeric_sensor_ssd8 {
	u8 hysteresis;
	u8 supported_thresholds;
	u8 threshold_and_hysteresis_volatility;
	float state_transition_interval;
	float update_interval;
	u8 max_readable;
	u8 min_readable;
	u8 range_field_format;
	u8 range_field_support;
	u8 nominal_value;
	u8 normal_max;
	u8 normal_min;
	u8 warning_high;
	u8 warning_low;
	u8 critical_high;
	u8 critical_low;
	u8 fatal_high;
	u8 fatal_low;
} __packed;

struct numeric_sensor_ssd16 {
	u16 hysteresis;
	u8 supported_thresholds;
	u8 threshold_and_hysteresis_volatility;
	float state_transition_interval;
	float update_interval;
	u16 max_readable;
	u16 min_readable;
	u8 range_field_format;
	u8 range_field_support;
	u16 nominal_value;
	u16 normal_max;
	u16 normal_min;
	u16 warning_high;
	u16 warning_low;
	u16 critical_high;
	u16 critical_low;
	u16 fatal_high;
	u16 fatal_low;
} __packed;

struct numeric_sensor_ssd32 {
	u32 hysteresis;
	u8 supported_thresholds;
	u8 threshold_and_hysteresis_volatility;
	float state_transition_interval;
	float update_interval;
	u32 max_readable;
	u32 min_readable;
	u8 range_field_format;
	u8 range_field_support;
	u32 nominal_value;
	u32 normal_max;
	u32 normal_min;
	u32 warning_high;
	u32 warning_low;
	u32 critical_high;
	u32 critical_low;
	u32 fatal_high;
	u32 fatal_low;
} __packed;

/* Numeric Sensor PDR Format (DSP0248 Table 78) */
struct numeric_sensor_pdr {
	struct pdr_hdr hdr;
	u16 pldm_terminus_handle;
	u16 sensor_id;
	union {
		struct {
			u16 entity_type_entity_id :15;
			u16 entity_type_logical   :1;
		};
		u16 entity_type;
	};
	u16 entity_instance_number;
	u16 container_id;
	u8 sensor_init;
	u8 sensor_auxiliary_names_pdr;
	u8 base_unit;
	s8 unit_modifier;
	u8 rate_unit;
	u8 base_oem_unit_handle;
	u8 aux_unit;
	s8 aux_unit_modifier;
	u8 aux_rate_unit;
	u8 rel;
	u8 aux_oem_unit_handle;
	u8 is_linear;
	u8 sensor_data_size;
	float resolution;
	float offset;
	u16 accuracy;
	u8 plus_tolerance;
	u8 minus_tolerance;
	union {
		struct numeric_sensor_ssd8 ssd8;
		struct numeric_sensor_ssd16 ssd16;
		struct numeric_sensor_ssd32 ssd32;
	} ssd;
} __packed;

#define AUX_NAME_MAX  33

/* Sensor Auxiliary Names PDR Format (DSP0248 Table 83) */
struct aux_name_pdr {
	struct pdr_hdr hdr;
	u16 pldm_terminus_handle;
	u16 sensor_id;
	u8 sensor_count;
	u8 name_string_count;           /* Currently hard-coded to a single name */
	u8 name_language_tag[3];        /* Currently hard-coded to "en" only */
	u16 sensor_name[AUX_NAME_MAX];
} __packed;

/* PLDM FRU Field TLV (DSP0257 Table 2) */
struct pldm_fru_field {
	u8 field_type;
	u8 length;
	u8 value[];
} __packed;

/* PLDM FRU Record Data Format (DSP0257 Table 2) */
struct pldm_fru_record {
	u16 record_set_id;
	u8 record_type;
	u8 num_fields;
	u8 field_encoding;
	u8 field_data[];
} __packed;

/* FRU Record Types (DSP0257 Table 4) */
enum pldm_fru_record_type {
	PLDM_FRU_RECORD_GENERAL = 1,
	PLDM_FRU_RECORD_OEM = 254
};

/* General FRU Record Field Types (DSP0257 Table 5) */
enum pldm_fru_field_type {
	PLDM_FRU_FIELD_CHASSIS_TYPE = 1,
	PLDM_FRU_FIELD_MODEL = 2,
	PLDM_FRU_FIELD_PART_NUMBER = 3,
	PLDM_FRU_FIELD_SERIAL_NUMBER = 4,
	PLDM_FRU_FIELD_MANUFACTURER = 5,
	PLDM_FRU_FIELD_MANUFACTURE_DATE = 6,
	PLDM_FRU_FIELD_VENDOR = 7,
	PLDM_FRU_FIELD_NAME = 8,
	PLDM_FRU_FIELD_SKU = 9,
	PLDM_FRU_FIELD_VERSION = 10,
	PLDM_FRU_FIELD_ASSET_TAG = 11,
	PLDM_FRU_FIELD_DESCRIPTION = 12,
	PLDM_FRU_FIELD_ENGINEERING_CHANGE_LEVEL = 13,
	PLDM_FRU_FIELD_OTHER = 14,
	PLDM_FRU_FIELD_VENDOR_IANA = 15
};

#endif /* PLDM_CXI_H */
