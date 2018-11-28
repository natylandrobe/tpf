#include "structData.h"

#define INFOLARGO_SZ 2
#define SB_1 0xB5
#define SB_2 0x62
#define NAV_CL 0x01
#define TIM_CL 0x0D
#define TIM_TOS_ID 0x12
#define PVT_ID 0x07
#define POSLLH_ID 0x02 

#define MSB_IND 1
#define LSB_IND 0
#define MASK_LE_MSB 0x0F
#define MASK_LE_LSB 0xF0
#define SHIFT_LE 8

typedef enum{S_EPTNULL, S_EREAD, S_CLASS_INV, S_ID_INV, S_INV_CK, S_OK} ubxst_t;

ubxst_t procesar_ubx(FILE *fin){
	char info_largo[INFOLARGO_SZ];
	unsigned int c, id, largo;

	if(!fin){
		return S_EPTNULL;
	}

	while((c = fgetc(fin)) != EOF){
		if(c == SB_1){
			if((c = fgetc(fin)) == SB_2){
				break;
			}
		}
	}

	if((c = fgetc(fin)) == EOF){
		return S_EREAD;
	}

	switch(c){
		case NAV_CL:
			if((id = fgetc(fin)) == EOF){
				return S_EREAD;
			}
			
			if(id != PVT_ID && id != POSLLH_ID){
				return S_ID_INV;
			}
			break;
		case TIM_CL:
			if((id = fgetc(fin)) == EOF){
				return S_EREAD;
			}
			if(id != TIM_TOS_ID){
				return S_ID_INV;
			}
			break;
		default:
			return S_CLASS_INV;
	}


	if(fread(info_largo, sizeof(char), INFOLARGO_SZ, fin) != INFOLARGO_SZ){
		return S_EREAD;
	}

	largo = calc_largo(info_largo);

	

}

unsigned int calc_largo(char info[]){
	return ((info[MSB_IND] & MASK_LE_MSB) << 8)|((info[LSB_IND] & MASK_LE_LSB) >> 8);
}