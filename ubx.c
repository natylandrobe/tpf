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
#define SHIFT_1B 8

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

//defines para cargar_pvt
#define VALID_IND 15
#define FLAG_IND 25
#define MASK_FIX 0x01
#define NUM_IND 27
#define PDOP_MSB 80
#define PDOP_LSB 81

//defines cargar_pos
#define SHIFT_2B 16
#define SHIFT_3B 24
#define PVTLON_B0 31
#define PVTLON_B1 30 
#define PVTLON_B2 29
#define PVTLON_B3 28
#define PVTLAT_B0 35
#define PVTLAT_B1 34 
#define PVTLAT_B2 33
#define PVTLAT_B3 32
#define PVTH_B0 43
#define PVTH_B1 42 
#define PVTH_B2 41
#define PVTH_B3 40
#define POSLON_B0 11
#define POSLON_B1 10 
#define POSLON_B2 9
#define POSLON_B3 8
#define POSLAT_B0 15
#define POSLAT_B1 14 
#define POSLAT_B2 13
#define POSLAT_B3 12
#define POSH_B0 23
#define POSH_B1 22 
#define POSH_B2 21
#define POSH_B3 20
#define SCALING 0.0000001




typedef enum{S_EPTNULL, S_ENOMEM, S_EREAD, S_CLASS_INV, S_ID_INV, S_CK_INV, S_LARGO_INV, S_OK, S_FIX_INV} ubxst_t;
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


		return S_OK;

}

ubxst_t cargar_sPVT(struct s_PVT * dato, struct fecha *funi, unsigned char *buff){

	if(!dato || !funi || !buff){
		return S_EPTNULL;
	}

	if(cargar_fecha(dato, funi, PVT_ID, buff, &calc_fecha) != S_OK){
		return S_EPTNULL;
	}

	dato->valid = buff[VALID_IND];

	if(!(buff[FLAG_IND]&MASK_FIX)){
		return S_FIX_INV;
	}

	dato->flags = buff[FLAG_IND];
	dato->numSV = buff[NUM_IND];

	if(cargar_pos(dato, PVT_ID, buff) != S_OK){
		return S_EPTNULL;
	}

	dato->pDOP = (buff[PDOP_MSB] << SHIFT_1B)|buff[PDOP_LSB];

	return S_OK;
}

ubxst_t cargar_pos(void *dato, unsigned char id, unsigned char *buff){
	struct s_PVT * pvt_s;
	struct s_POSLLH * posllh_s;

	if(!dato || !buff){
		return S_EPTNULL;
	}

	switch(id){
		case PVT_ID:
			pvt_s = (struct s_PVT *)dato;

			pvt_s->lon = ((buff[PVTLON_B3]<< SHIFT_3B)|(buff[PVTLON_B2]<< SHIFT_2B)|(buff[PVTLON_B1]<< SHIFT_1B)| buff[PVTLON_B0])*SCALING;
			pvt_s->lat = ((buff[PVTLAT_B3]<< SHIFT_3B)|(buff[PVTLAT_B2]<< SHIFT_2B)|(buff[PVTLAT_B1]<< SHIFT_1B)|buff[PVTLAT_B0])*SCALING;
			pvt_s->hmsl = ((buff[PVTH_B3]<< SHIFT_3B)|(buff[PVTH_B2]<< SHIFT_2B)|(buff[PVTH_B1]<< SHIFT_1B)|buff[PVTH_B0])*SCALING;

			break;
		case POSLLH_ID:
			posllh_s = (struct s_POSLLH *)dato;

			posllh_s->lon = ((buff[POSLON_B3]<< SHIFT_3B)|(buff[POSLON_B2]<< SHIFT_2B)|(buff[POSLON_B1]<< SHIFT_1B)|buff[POSLON_B0])*SCALING;
			posllh_s->lat = ((buff[POSLAT_B3]<< SHIFT_3B)|(buff[POSLAT_B2]<< SHIFT_2B)|(buff[POSLAT_B1]<< SHIFT_1B)|buff[POSLAT_B0])*SCALING;
			posllh_s->hmsl = ((buff[POSH_B3]<< SHIFT_3B)|(buff[POSH_B2]<< SHIFT_2B)|(buff[POSH_B1]<< SHIFT_1B)|buff[POSH_B0])*SCALING;

			break;
	}

	return S_OK;
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
			fecha->anio = (buff[P_ANIO_MBIND] << SHIFT_1B)|buff[P_ANIO_LBIND];
			fecha->mes = buff[PVT_MOIND];
			fecha->dia = buff[PVT_DIND];
			fecha->hora = buff[PVT_HIND];
			fecha->minutos = buff[PVT_MININD];
			fecha->segundos = buff[PVT_SEGIND];
			break;
		case TIM_TOS_ID:
			fecha->anio = (buff[T_ANIO_MBIND] << SHIFT_1B)|buff[T_ANIO_LBIND];
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
	return (info[MSB_IND] << SHIFT_1B)|info[LSB_IND];
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