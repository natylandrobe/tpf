#include "structData.h"

#define LARGO_CK_SZ 2
#define SB_1 0xB5
#define SB_2 0x62
#define NAV_CL 0x01
#define TIM_CL 0x0D
#define TIM_TOS_ID 0x12
#define PVT_ID 0x07
#define POSLLH_ID 0x02 
#define PVT_L 92
#define TIM_TOS_L 56
#define POSLLH_L 28
#define PVT_BUFFSZ 96
#define TIM_TOS_BUFFSZ 60
#define POSLLH_BUFFSZ 32

//defines para calc_largo
#define MSB_IND 1
#define LSB_IND 0
#define MASK_LE 0x00FF
#define SHIFT_LE 8

//defines para ubx_cksum
#define POS_CK -4
#define CKA_IND 0
#define CKB_IND 1

//defines para calc_fecha
#define P_ANIO_MBIND 9
#define P_ANIO_LBIND 8
#define PVT_MOIND 10
#define PVT_DIND 11
#define PVT_HIND 12
#define PVT_MININD 13
#define PVT_SEGIND 14
#define T_ANIO_MBIND 13
#define T_ANIO_LBIND 12
#define TT_MOIND 14
#define TT_DIND 15
#define TT_HIND 16
#define TT_MININD 17
#define TT_SEGIND 18

typedef enum{S_EPTNULL, S_ENOMEM, S_EREAD, S_CLASS_INV, S_ID_INV, S_CK_INV, S_LARGO_INV, S_OK} ubxst_t;
unsigned int calc_largo(unsigned char info[]);
ubxst_t procesar_ubx(FILE *fin, struct fecha *fecha);
ubxst_t ubx_cksum(unsigned char *ckBuff, int n, FILE *fin);
ubxst_t calc_fecha(unsigned char *buff, struct fecha *fecha, unsigned char id);
ubxst_t cargar_fecha(void *dato, struct fecha *funi, unsigned char id, unsigned char *buff, ubxst_t (*proc_fecha)(unsigned char *, struct fecha *, unsigned char));

ubxst_t procesar_ubx(FILE *fin, struct fecha *fecha){
	unsigned char info_largo[LARGO_CK_SZ], *buff;
	unsigned int c, id, largo;
	ubxst_t cks;

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


	if(fread(info_largo, sizeof(char), LARGO_CK_SZ, fin) != LARGO_CK_SZ){
		return S_EREAD;
	}

	largo = calc_largo(info_largo);
	
	if((id == PVT_ID && largo != PVT_L) || (id == TIM_TOS_ID && largo != TIM_TOS_L) || (id == POSLLH_ID && largo != POSLLH_L)){
		return S_LARGO_INV;
	}

	if(fseek(fin, POS_CK, SEEK_CUR)){
		return S_EREAD;
	}

	switch(id){
		case PVT_ID:
			buff = (char *)malloc(sizeof(char)*PVT_BUFFSZ);
			if(!buff){
				return S_ENOMEM;
			}

			if(fread(buff, sizeof(char), PVT_BUFFSZ, fin) != PVT_BUFFSZ){
				return S_EREAD;
			}

			if((cks = ubx_cksum(buff, PVT_BUFFSZ, fin)) == S_OK){
				break;
			}

			else{
				return cks;
			}
		case TIM_TOS_ID:
			buff = (char *)malloc(sizeof(char)*TIM_TOS_BUFFSZ);
			if(!buff){
				return S_ENOMEM;
			}

			if(fread(buff, sizeof(char), TIM_TOS_BUFFSZ, fin) != TIM_TOS_BUFFSZ){
				return S_EREAD;
			}

			if((cks = ubx_cksum(buff, TIM_TOS_BUFFSZ, fin)) == S_OK){
				break;
			}

			else{
				return cks;
			}
		case POSLLH_ID:
			buff = (char *)malloc(sizeof(char)*POSLLH_BUFFSZ);
			if(!buff){
				return S_ENOMEM;
			}

			if(fread(buff, sizeof(char), POSLLH_BUFFSZ, fin) != POSLLH_BUFFSZ){
				return S_EREAD;
			}

			if((cks = ubx_cksum(buff, POSLLH_BUFFSZ, fin)) == S_OK){
				break;
			}

			else{
				return cks;
			}
	}//cierra switch


		

}


ubxst_t cargar_fecha(void *dato, struct fecha *funi, unsigned char id, unsigned char *buff, ubxst_t (*proc_fecha)(unsigned char *, struct fecha *, unsigned char)){
	struct s_PVT * pvt_s;
	struct s_TIM_TOS * tt_s;
	struct s_POSLLH * posllh_s;

	if(!dato || !funi || !buff || !proc_fecha){
		return S_EPTNULL;
	}

	switch(id){
		case PVT_ID:
			pvt_s = (struct s_PVT *)dato;

			if((*proc_fecha)(buff, &(pvt_s->f), id) != S_OK){
				return S_EPTNULL;
			}

			*funi = pvt_s->f;
			break;
		case TIM_TOS_ID:
			tt_s  = (struct s_TIM_TOS *)dato;
			if((*proc_fecha)(buff, &(tt_s->f), id) != S_OK){
				return S_EPTNULL;
			}

			*funi = tt_s->f;
			break;
		case POSLLH_ID:
			posllh_s = (struct s_POSLLH *)dato;

			posllh_s->f = *funi;
			break;
	}

	return S_OK;
}

ubxst_t calc_fecha(unsigned char *buff, struct fecha *fecha, unsigned char id){

	if(!buff || !fecha){
		return S_EPTNULL;
	}

	switch(id){
		case PVT_ID:
			fecha->anio = ((buff[P_ANIO_MBIND] & MASK_LE) << 8)|((buff[P_ANIO_LBIND] & MASK_LE));
			fecha->mes = buff[PVT_MOIND];
			fecha->dia = buff[PVT_DIND];
			fecha->hora = buff[PVT_HIND];
			fecha->minutos = buff[PVT_MININD];
			fecha->segundos = buff[PVT_SEGIND];
			break;
		case TIM_TOS_ID:
			fecha->anio = ((buff[T_ANIO_MBIND] & MASK_LE) << 8)|((buff[T_ANIO_LBIND] & MASK_LE));
			fecha->mes = buff[TT_MOIND];
			fecha->dia = buff[TT_DIND];
			fecha->hora = buff[TT_HIND];
			fecha->minutos = buff[TT_MININD];
			fecha->segundos = buff[TT_SEGIND];
			break;
	}

	return S_OK;
}

unsigned int calc_largo(unsigned char info[]){
	return ((info[MSB_IND] & MASK_LE) << 8)|((info[LSB_IND] & MASK_LE));
}


ubxst_t ubx_cksum(unsigned char *ckBuff, int n, FILE *fin){

	unsigned char ck_a = 0;
	unsigned char ck_b = 0;
	size_t i;
	unsigned char cksum[LARGO_CK_SZ];

	if(!ckBuff || !fin){
		return S_EPTNULL;
	}

	for(i = 0; i < n; i++){
		ck_a = ck_a + ckBuff[i];
		ck_b = ck_a + ck_b;
	}

	if(fread(cksum, sizeof(char), LARGO_CK_SZ, fin) != LARGO_CK_SZ){
		return S_EREAD;
	}

	if(cksum[CKA_IND] == ck_a && cksum[CKB_IND] == ck_b){
		return S_OK;
	}

	return S_CK_INV;
}