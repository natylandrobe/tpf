#include "ubx.h"

/*lee la sentencia ubx, procesa los datos y de ser correctos, carga las estructuras y las agrega a la lista si corresponde*/
ubxst_t procesar_ubx(FILE *fin, struct fecha *fecha, lista_t *lista, size_t *index, status_t (*add_nodo)(void *, lista_t *, sent_t), FILE *flog, const struct args *arg){
	unsigned char info_largo[LARGO_CK_SZ], *buff;
	unsigned int id, largo;
	int c;
	ubxst_t cks, cargar_s;
	struct s_PVT *pvt_s;
	struct s_POSLLH * posllh_s;
	struct s_TIM_TOS * tt_s;
	debug_t deb;

	if(!fin || !flog || !fecha || !lista || !index || !add_nodo || !arg){
		return S_EPTNULL;
	}
	
	deb = SYNC;
	imp_log(flog, NULL, NULL, &deb);

	while((c = fgetc(fin)) != EOF){
		if(c == SB_1){
			if((c = fgetc(fin)) == SB_2){

				break;
			}
		}
	}

	if(!(strcmp(arg->infile_n, DEFAULT_INFILE))){
		return procesar_standard(fecha, lista, index, agregar_nodo, flog);
	}

	else{

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

		deb = ID_D;
		imp_log(flog, NULL, NULL, &deb);

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
						deb = MSJ_OK;
						imp_log(flog, NULL, NULL, &deb);
					}

					else{
						free(buff);
						free(pvt_s);
						return cargar_s;
					}

					if((*add_nodo)(pvt_s, lista, NAV_PVT) == ST_OK){
						deb = CARGA_MSJ;
						imp_log(flog, NULL, NULL, &deb);
						(*index)++;
						free(pvt_s);
						free(buff);
						break;
					}
					else{
						free(buff);
						free(pvt_s);
						return S_EAGR;
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
						deb = MSJ_OK;
						imp_log(flog, NULL, NULL, &deb);
						free(buff);
						free(tt_s);
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
						deb = MSJ_OK;
						imp_log(flog, NULL, NULL, &deb);
					}

					else{
						free(buff);
						free(posllh_s);
						return cargar_s;
					}
					if((*add_nodo)(posllh_s, lista, NAV_POSLLH) == ST_OK){
						deb = CARGA_MSJ;
						imp_log(flog, NULL, NULL, &deb);
						(*index)++;
						free(posllh_s);
						free(buff);
						break;
					}
					else{
						free(buff);
						free(posllh_s);
						return S_EAGR;
					}
				}

				else{
					free(buff);
					return cks;
				}
		}

	}

	return S_OK;
}

/*procesa los datos de la sentencia y carga la estructura PVT*/
ubxst_t cargar_sPVT(struct s_PVT * dato, struct fecha *funi, const unsigned char *buff){

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

/*procesa los datos de la sentencia y carga la estructura POSLLH*/
ubxst_t cargar_sPOSLLH(struct s_POSLLH *dato, struct fecha *funi, const unsigned char *buff){

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

/*procesa los datos de la sentencia y carga la estructura TIM TOS*/
ubxst_t cargar_sTIMTOS(struct s_TIM_TOS *dato, struct fecha *funi, const unsigned char *buff){

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

/*procesa y carga la precision a la estructura POSLLH*/
ubxst_t cargar_precision(struct s_POSLLH *dato, const unsigned char *buff){

	if(!dato || !buff){
		return S_EPTNULL;
	}

	dato->hAcc = (buff[HACC_B3]<< SHIFT_3B)|(buff[HACC_B2]<< SHIFT_2B)|(buff[HACC_B1]<< SHIFT_1B)| buff[HACC_B0];
	dato->vAcc = (buff[VACC_B3]<< SHIFT_3B)|(buff[VACC_B2]<< SHIFT_2B)|(buff[VACC_B1]<< SHIFT_1B)| buff[VACC_B0];
	
	return S_OK;
}

/*de acuerdo con la sentencia recibida, procesa y carga los datos de posicion en la estructura*/
ubxst_t cargar_pos(const void *dato, unsigned char id, const unsigned char *buff){
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

/*de acuerdo al tipo de sentencia carga la estructura fecha de la misma, y si corresponde, actualiza la fecha universal */
ubxst_t cargar_fecha(const void *dato, struct fecha *funi, unsigned char id, const unsigned char *buff, ubxst_t (*proc_fecha)(const unsigned char *, struct fecha *, unsigned char)){
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

/*procesa la fecha de la sentencia y la carga por interfaz en la estructura*/
ubxst_t calc_fecha(const unsigned char *buff, struct fecha *fecha, unsigned char id){

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

/*calcula el largo de la sentencia*/
unsigned int calc_largo(unsigned char info[]){
	return (info[MSB_IND] << SHIFT_1B)|info[LSB_IND];
}

/*verifica que el checksum sea correcto*/
ubxst_t ubx_cksum(const unsigned char *ckBuff, int n, FILE *fin){

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

ubxst_t procesar_standard(struct fecha *fecha, lista_t *lista, size_t *index, status_t (*add_nodo)(void *, lista_t *, sent_t), FILE *flog){

	unsigned char *stdbuff, info_largo[LARGO_CK_SZ];
	int c, i;
	unsigned int id, largo;
	debug_t deb;
	ubxst_t cks, cargar_s;
	struct s_PVT *pvt_s;
	struct s_POSLLH * posllh_s;
	struct s_TIM_TOS * tt_s;

	if(!fecha || !lista || !index || !add_nodo){

		return S_EPTNULL;
	}


	stdbuff = (unsigned char *)malloc(sizeof(unsigned char)*4);
	if(!stdbuff){
		return S_ENOMEM;
	}

	if((c = getchar()) == EOF){
		free(stdbuff);
		return S_EREAD;
	}

	stdbuff[CLASS_IND] = c;

	switch(c){
		case NAV_CL:
			if((id = getchar()) == EOF){
				free(stdbuff);
				return S_EREAD;
			}
			
			if(id != PVT_ID && id != POSLLH_ID){
				free(stdbuff);
				return S_ID_INV;
			}

			stdbuff[ID_IND] = id;

			break;
		case TIM_CL:
			if((id = getchar()) == EOF){
				free(stdbuff);
				return S_EREAD;
			}
			if(id != TIM_TOS_ID){
				free(stdbuff);
				return S_ID_INV;
			}
			stdbuff[ID_IND] = id;

			break;
		default:
			free(stdbuff);
			return S_CLASS_INV;
	}

	deb = ID_D;
	imp_log(flog, NULL, NULL, &deb);

	if(fread(info_largo, sizeof(unsigned char), LARGO_CK_SZ, stdin) != LARGO_CK_SZ){
		free(stdbuff);
		return S_EREAD;
	}

	stdbuff[LARGO_LSB_IND] = info_largo[LSB_IND];
	stdbuff[LARGO_MSB_IND] = info_largo[MSB_IND];
	
	
	largo = calc_largo(info_largo);
	
	if((id == PVT_ID && largo != PVT_L) || (id == TIM_TOS_ID && largo != TIM_TOS_L) || (id == POSLLH_ID && largo != POSLLH_L)){
		free(stdbuff);
		return S_LARGO_INV;
	}


	switch(id){
		case PVT_ID:
			stdbuff = (unsigned char *)realloc(stdbuff, sizeof(unsigned char)*PVT_BUFFSZ);
			if(!stdbuff){
				free(stdbuff);
				return S_ENOMEM;
			}

			for(i = PAYL_IND; i < PVT_BUFFSZ; i++){
				if((stdbuff[i] = getchar()) == EOF){
					free(stdbuff);
					return S_EREAD;
				}
			}

			if((cks = ubx_cksum(stdbuff, PVT_BUFFSZ, stdin)) == S_OK){

				pvt_s = (struct s_PVT *)malloc(sizeof(struct s_PVT));
				if(!pvt_s){
					free(stdbuff);
					return S_EPTNULL;
				}
				if((cargar_s = cargar_sPVT(pvt_s, fecha, stdbuff)) == S_OK){
					deb = MSJ_OK;
					imp_log(flog, NULL, NULL, &deb);
				}

				else{
					free(stdbuff);
					free(pvt_s);
					return cargar_s;
				}

				if((*add_nodo)(pvt_s, lista, NAV_PVT) == ST_OK){
					deb = CARGA_MSJ;
					imp_log(flog, NULL, NULL, &deb);
					(*index)++;
					free(pvt_s);
					free(stdbuff);
					break;
				}
				else{
					free(stdbuff);
					free(pvt_s);
					return S_EAGR;
				}

			}

			else{
				free(stdbuff);
				return cks;
			}
		case TIM_TOS_ID:
			stdbuff = (unsigned char *)realloc(stdbuff, sizeof(unsigned char)*TIM_TOS_BUFFSZ);
			if(!stdbuff){
				return S_ENOMEM;
			}


			for(i = PAYL_IND; i < TIM_TOS_BUFFSZ; i++){
				if((stdbuff[i] = getchar()) == EOF){
					free(stdbuff);
					return S_EREAD;
				}
			}

			if((cks = ubx_cksum(stdbuff, TIM_TOS_BUFFSZ, stdin)) == S_OK){
				tt_s = (struct s_TIM_TOS *)malloc(sizeof(struct s_TIM_TOS));
				if(!tt_s){
					free(stdbuff);
					return S_EPTNULL;
				}
				if((cargar_s = cargar_sTIMTOS(tt_s, fecha, stdbuff)) == S_OK){
					deb = MSJ_OK;
					imp_log(flog, NULL, NULL, &deb);
					free(stdbuff);
					free(tt_s);
					break;
				}

				else{
					free(stdbuff);
					free(tt_s);
					return cargar_s;
				}
			}

			else{
				free(stdbuff);
				return cks;
			}
		case POSLLH_ID:
			stdbuff = (unsigned char *)realloc(stdbuff, sizeof(unsigned char)*POSLLH_BUFFSZ);
			if(!stdbuff){
				return S_ENOMEM;
			}

			for(i = PAYL_IND; i < POSLLH_BUFFSZ; i++){
				if((stdbuff[i] = getchar()) == EOF){
					free(stdbuff);
					return S_EREAD;
				}
			}			

			if((cks = ubx_cksum(stdbuff, POSLLH_BUFFSZ, stdin)) == S_OK){
				posllh_s = (struct s_POSLLH *)malloc(sizeof(struct s_POSLLH));
				if(!posllh_s){
					free(stdbuff);
					return S_EPTNULL;
				}
				if((cargar_s = cargar_sPOSLLH(posllh_s, fecha, stdbuff)) == S_OK){
					deb = MSJ_OK;
					imp_log(flog, NULL, NULL, &deb);
				}

				else{
					free(stdbuff);
					free(posllh_s);
					return cargar_s;
				}
				if((*add_nodo)(posllh_s, lista, NAV_POSLLH) == ST_OK){
					deb = CARGA_MSJ;
					imp_log(flog, NULL, NULL, &deb);
					(*index)++;
					free(posllh_s);
					free(stdbuff);
					break;
				}
				else{
					free(stdbuff);
					free(posllh_s);
					return S_EAGR;
				}
			}

			else{
				free(stdbuff);
				return cks;
			}
	}

	return S_OK;
}