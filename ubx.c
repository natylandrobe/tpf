#include "lista.h"

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
#define PVTLON_B0 28
#define PVTLON_B1 29 
#define PVTLON_B2 30
#define PVTLON_B3 31
#define PVTLAT_B0 32
#define PVTLAT_B1 33 
#define PVTLAT_B2 34
#define PVTLAT_B3 35
#define PVTH_B0 40
#define PVTH_B1 41 
#define PVTH_B2 42
#define PVTH_B3 43
#define POSLON_B0 8
#define POSLON_B1 9 
#define POSLON_B2 10
#define POSLON_B3 11
#define POSLAT_B0 12
#define POSLAT_B1 13 
#define POSLAT_B2 14
#define POSLAT_B3 15
#define POSH_B0 20
#define POSH_B1 21 
#define POSH_B2 22
#define POSH_B3 23
#define SCALING_POS 0.0000001
#define SCALING_PDOP 0.01

//defines cargar_precision
#define HACC_B0 27
#define HACC_B1 26
#define HACC_B2 25
#define HACC_B3 24
#define VACC_B0 31
#define VACC_B1 30
#define VACC_B2 29 
#define VACC_B3 28

//defines cargar timtos
#define VERSION_IND 4
#define GNSS_IND 5




typedef enum{S_EPTNULL, S_ENOMEM, S_EREAD, S_CLASS_INV, S_ID_INV, S_CK_INV, S_LARGO_INV, S_OK, S_FIX_INV} ubxst_t;
unsigned int calc_largo(unsigned char info[]);
ubxst_t procesar_ubx(FILE *fin, struct fecha *fecha, lista_t *lista, status_t (*add_nodo)(void *, lista_t *, sent_t));
ubxst_t ubx_cksum(unsigned char *ckBuff, int n, FILE *fin);
ubxst_t calc_fecha(unsigned char *buff, struct fecha *fecha, unsigned char id);
ubxst_t cargar_fecha(void *dato, struct fecha *funi, unsigned char id, unsigned char *buff, ubxst_t (*proc_fecha)(unsigned char *, struct fecha *, unsigned char));
ubxst_t cargar_precision(struct s_POSLLH *dato, unsigned char *buff);
ubxst_t cargar_pos(void *dato, unsigned char id, unsigned char *buff);
ubxst_t cargar_sPVT(struct s_PVT * dato, struct fecha *funi, unsigned char *buff);
ubxst_t cargar_sPOSLLH(struct s_POSLLH *dato, struct fecha *funi, unsigned char *buff);
ubxst_t cargar_sTIMTOS(struct s_TIM_TOS *dato, struct fecha *funi, unsigned char *buff);


ubxst_t procesar_ubx(FILE *fin, struct fecha *fecha, lista_t *lista, status_t (*add_nodo)(void *, lista_t *, sent_t)){
	unsigned char info_largo[LARGO_CK_SZ], *buff;
	unsigned int c, id, largo;
	ubxst_t cks, cargar_s;
	struct s_PVT *pvt_s;
	struct s_POSLLH * posllh_s;
	struct s_TIM_TOS * tt_s;

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
			buff = (unsigned char *)malloc(sizeof(unsigned char)*PVT_BUFFSZ);
			if(!buff){
				return S_ENOMEM;
			}

			if(fread(buff, sizeof(unsigned char), PVT_BUFFSZ, fin) != PVT_BUFFSZ){
				free(buff);
				return S_EREAD;
			}

			if((cks = ubx_cksum(buff, PVT_BUFFSZ, fin)) == S_OK){
				pvt_s = (struct s_PVT *)malloc(sizeof(struct s_PVT));
				if(!pvt_s){
					free(buff);
					return S_EPTNULL;
				}
				if((cargar_s = cargar_sPVT(pvt_s, fecha, buff)) == S_OK){
					break;
				}

				else{
					free(buff);
					free(pvt_s);
					return cargar_s;
				}
			}

			else{
				free(buff);
				return cks;
			}
		case TIM_TOS_ID:
			buff = (unsigned char *)malloc(sizeof(unsigned char)*TIM_TOS_BUFFSZ);
			if(!buff){
				return S_ENOMEM;
			}

			if(fread(buff, sizeof(unsigned char), TIM_TOS_BUFFSZ, fin) != TIM_TOS_BUFFSZ){
				free(buff);
				return S_EREAD;
			}

			if((cks = ubx_cksum(buff, TIM_TOS_BUFFSZ, fin)) == S_OK){
				tt_s = (struct s_TIM_TOS *)malloc(sizeof(struct s_TIM_TOS));
				if(!tt_s){
					free(buff);
					return S_EPTNULL;
				}
				if((cargar_s = cargar_sTIMTOS(tt_s, fecha, buff)) == S_OK){
					break;
				}

				else{
					free(buff);
					free(tt_s);
					return cargar_s;
				}
			}

			else{
				free(buff);
				return cks;
			}
		case POSLLH_ID:
			buff = (unsigned char *)malloc(sizeof(unsigned char)*POSLLH_BUFFSZ);
			if(!buff){
				return S_ENOMEM;
			}

			if(fread(buff, sizeof(unsigned char), POSLLH_BUFFSZ, fin) != POSLLH_BUFFSZ){
				free(buff);
				return S_EREAD;
			}

			if((cks = ubx_cksum(buff, POSLLH_BUFFSZ, fin)) == S_OK){
				posllh_s = (struct s_POSLLH *)malloc(sizeof(struct s_POSLLH));
				if(!posllh_s){
					free(buff);
					return S_EPTNULL;
				}
				if((cargar_s = cargar_sPOSLLH(posllh_s, fecha, buff)) == S_OK){
					break;
				}

				else{
					free(buff);
					free(posllh_s);
					return cargar_s;
				}
			}

			else{
				free(buff);
				return cks;
			}
	}//cierra switch


	return S_OK;

}

ubxst_t cargar_sPVT(struct s_PVT * dato, struct fecha *funi, unsigned char *buff){

	if(!dato || !funi || !buff){
		return S_EPTNULL;
	}

	if(!(buff[FLAG_IND]&MASK_FIX)){
		return S_FIX_INV;
	}

	if(cargar_fecha(dato, funi, PVT_ID, buff, &calc_fecha) != S_OK){
		return S_EPTNULL;
	}

	if(cargar_pos(dato, PVT_ID, buff) != S_OK){
		return S_EPTNULL;
	}

	dato->valid = buff[VALID_IND];
	dato->flags = buff[FLAG_IND];
	dato->numSV = buff[NUM_IND];
	dato->pDOP = ((buff[PDOP_MSB] << SHIFT_1B)|buff[PDOP_LSB])*SCALING_PDOP;

	return S_OK;
}

ubxst_t cargar_sPOSLLH(struct s_POSLLH *dato, struct fecha *funi, unsigned char *buff){

	if(!dato || !funi || !buff){
		return S_EPTNULL;
	}

	if(cargar_fecha(dato, funi, POSLLH_ID, buff, &calc_fecha) != S_OK){
		return S_EPTNULL;
	}

	if(cargar_pos(dato, POSLLH_ID, buff) != S_OK){
		return S_EPTNULL;
	}

	if(cargar_precision(dato, buff) != S_OK){
		return S_EPTNULL;
	}

	return S_OK;
}

ubxst_t cargar_sTIMTOS(struct s_TIM_TOS *dato, struct fecha *funi, unsigned char *buff){

	if(!dato || !funi || !buff){
		return S_EPTNULL;
	}

	if(cargar_fecha(dato, funi, TIM_TOS_ID, buff, &calc_fecha) != S_OK){
		return S_EPTNULL;
	}

	dato->version = buff[VERSION_IND];
	dato->gnss = buff[GNSS_IND];

	return S_OK;
}

ubxst_t cargar_precision(struct s_POSLLH *dato, unsigned char *buff){

	if(!dato || !buff){
		return S_EPTNULL;
	}

	dato->hAcc = (buff[HACC_B3]<< SHIFT_3B)|(buff[HACC_B2]<< SHIFT_2B)|(buff[HACC_B1]<< SHIFT_1B)| buff[HACC_B0];
	dato->vAcc = (buff[VACC_B3]<< SHIFT_3B)|(buff[VACC_B2]<< SHIFT_2B)|(buff[VACC_B1]<< SHIFT_1B)| buff[VACC_B0];
	
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

			pvt_s->lon = ((buff[PVTLON_B3]<< SHIFT_3B)|(buff[PVTLON_B2]<< SHIFT_2B)|(buff[PVTLON_B1]<< SHIFT_1B)| buff[PVTLON_B0])*SCALING_POS;
			pvt_s->lat = ((buff[PVTLAT_B3]<< SHIFT_3B)|(buff[PVTLAT_B2]<< SHIFT_2B)|(buff[PVTLAT_B1]<< SHIFT_1B)|buff[PVTLAT_B0])*SCALING_POS;
			pvt_s->hmsl = ((buff[PVTH_B3]<< SHIFT_3B)|(buff[PVTH_B2]<< SHIFT_2B)|(buff[PVTH_B1]<< SHIFT_1B)|buff[PVTH_B0])*SCALING_POS;

			break;
		case POSLLH_ID:
			posllh_s = (struct s_POSLLH *)dato;

			posllh_s->lon = ((buff[POSLON_B3]<< SHIFT_3B)|(buff[POSLON_B2]<< SHIFT_2B)|(buff[POSLON_B1]<< SHIFT_1B)|buff[POSLON_B0])*SCALING_POS;
			posllh_s->lat = ((buff[POSLAT_B3]<< SHIFT_3B)|(buff[POSLAT_B2]<< SHIFT_2B)|(buff[POSLAT_B1]<< SHIFT_1B)|buff[POSLAT_B0])*SCALING_POS;
			posllh_s->hmsl = ((buff[POSH_B3]<< SHIFT_3B)|(buff[POSH_B2]<< SHIFT_2B)|(buff[POSH_B1]<< SHIFT_1B)|buff[POSH_B0])*SCALING_POS;

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