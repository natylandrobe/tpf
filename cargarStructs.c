#include "cargarStructs.h" 


/*procesa la sentencia zda, carga la estructura y asigna su fecha a la estructura de fecha date*/
status_t cargar_struct_zda( char *s, struct s_ZDA *Zda, struct fecha *date){


        char *str, *check;
        char *tokens[CANT_TOKEN_ZDA],*token[CANT_TOKEN_AUX_ZDA];
        double time_zone,dif_tmzone;
        double dia,mes,anio,horario;

       if(!s || !Zda || !date){
               return ST_EPTNULL;
        }

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }
        
        

        horario = strtod(tokens[INDEX_HORARIO_ZDA], &check);

        if(*check != END_STR){
               return ST_SENTINV;
        }

        dia = strtod(tokens[INDEX_DIA_ZDA], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        mes = strtod(tokens[INDEX_MES_ZDA], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        anio = strtod(tokens[INDEX_ANIO_ZDA], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        time_zone = strtod(tokens[INDEX_TIME_ZONE_ZDA], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }
        str = strtok(tokens[INDEX_DIF_TIME_ZONE_AUX_ZDA],ASTERISCO);
                i = 0;
        while(str != NULL){
                token[i] = str;
                str = strtok(NULL,ASTERISCO);
                i++;
        }
        dif_tmzone = strtod(token[INDEX_DIF_TIME_ZONE_ZDA], &check);

       if(*check != END_STR){
                return ST_SENTINV;
        }
   
        if (!checkDia(dia) || !checkMes(mes) || !checkAnio(anio)){
        	return ST_SENTINV ;
        }

        Zda->f.dia =dia;
        Zda->f.mes = mes;
        Zda->f.anio = anio;
        Zda->f.hora= horario/AUX_PARA_HOR_MIN_SEG;
        Zda->f.minutos = ((int)horario%AUX_PARA_HOR_MIN_SEG)/AUX_PARA_MIN_SEG;
        Zda->f.segundos = horario-Zda->f.hora*AUX_PARA_HOR_MIN_SEG-Zda->f.minutos*AUX_PARA_MIN_SEG;
        *date=Zda->f;
        Zda->time_zone = time_zone;
        Zda->dif_tmzone =dif_tmzone;
        
        return ST_OK;
}



/*procesa la sentencia rmc, carga la estructura y actualiza la fecha date */
status_t cargar_struct_rmc(char *s, struct s_RMC *Rmc, struct fecha *date){

        char *str, *check;
        char status;
        char *tokens[CANT_TOKEN_RMC];
        double lat, lon;
        double horario,dia,mes,anio,fecha;
       
     	if(!s || !RMC || !date){
                return ST_EPTNULL;
        }

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }
        
        horario= strtod(tokens[INDEX_HORARIO_RMC], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        if(strcmp(tokens[INDEX_STATUS_RMC],COMPARE_STATUS_RMC_VOID)==0){
			status=STATUS_RMC_VOID;
			
        }

        if (strcmp(tokens[INDEX_STATUS_RMC],COMPARE_STATUS_RMC_ACT)==0){
        	status=STATUS_RMC_ACT;
        }

        fecha= strtod(tokens[INDEX_FECHA_RMC], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        dia=(int)fecha/AUX_1_PARA_FECHA ;
        mes= (int)fecha%AUX_1_PARA_FECHA /AUX_2_PARA_FECHA;
        anio=(int)fecha%AUX_2_PARA_FECHA +AUX_3_PARA_FECHA;

        if (!checkDia(dia) || !checkMes(mes) || !checkAnio(anio)){
            return ST_SENTINV ;
        }


        lat = convertirLat(tokens[INDEX_LAT_RMC], tokens[INDEX_LAT_CARD_RMC]);
        lon = convertirLon(tokens[INDEX_LON_RMC], tokens[INDEX_LON_CARD_RMC]);

        if(!checkMembersrmc(lat, lon)){
                return ST_SENTINV;
        }
		
        Rmc->f.hora= horario/AUX_PARA_HOR_MIN_SEG;
        Rmc->f.minutos = ((int)horario%AUX_PARA_HOR_MIN_SEG)/AUX_PARA_MIN_SEG;
        Rmc->f.segundos = horario-Rmc->f.hora*AUX_PARA_HOR_MIN_SEG-Rmc->f.minutos*AUX_PARA_MIN_SEG;
        Rmc->f.dia = dia;
        Rmc->f.mes = mes;
        Rmc->f.anio = anio;
        *date= Rmc->f;  
        Rmc->lat = lat;
        Rmc->lon = lon;
        Rmc->status= status;
        
        return ST_OK;
}

/* procesa la sentencia gga y carga la estructura*/
status_t cargar_struct_gga(char *s, struct s_GGA *Gga, struct fecha *date){


        char *str, *check;
        char *tokens[CANT_TOKEN_GGA];
        double lat, lon, hdop, ele, separacion;
        long int cal, cant;
        double horario;
        cal_t calidad;

       if(!s || !Gga || !date){
             return ST_EPTNULL;
    	}

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }

        horario= strtod(tokens[INDEX_HORARIO_GGA], &check);

        if(*check != END_STR){
               return ST_SENTINV;
        }

        
        cant = strtol(tokens[INDEX_CANT], &check, 10);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        hdop = strtod(tokens[INDEX_HDOP], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        ele = strtod(tokens[INDEX_ELE], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        separacion= strtod(tokens[INDEX_SEP], &check);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        cal = strtol(tokens[INDEX_CAL], &check, 10);

        if(*check != END_STR){
                return ST_SENTINV;
        }

        calidad = convertirCal(cal);
        lat = convertirLat(tokens[INDEX_LAT], tokens[INDEX_LAT_CARD]);
        lon = convertirLon(tokens[INDEX_LON], tokens[INDEX_LON_CARD]);

        if(!checkMembers(lat, lon, calidad, cant)){
                return ST_SENTINV;
        }

        Gga->f.hora= horario/AUX_PARA_HOR_MIN_SEG;
        Gga->f.minutos = ((int)horario%AUX_PARA_HOR_MIN_SEG)/AUX_PARA_MIN_SEG;
        Gga->f.segundos = horario-Gga->f.hora*AUX_PARA_HOR_MIN_SEG-Gga->f.minutos*AUX_PARA_MIN_SEG;
        Gga->f.anio = date->anio;
        Gga->f.mes = date->mes;
        Gga->f.dia = date->dia;
        Gga->lat = lat;
        Gga->lon = lon;
        Gga->calidad = calidad;
        Gga->cantSat = cant;
        Gga->hdop = hdop;
        Gga->ele = ele;
        Gga->separacion = separacion;
        
        return ST_OK;
}



/*carga la estructura fecha con la del sistema*/
status_t defaultFecha(struct fecha *def){

	time_t rawtime;
   	struct tm *info;
   	struct timeval millisec;

   	if(!def){
   		return ST_EPTNULL;
   	}

   	time(&rawtime);
	gettimeofday(&millisec, NULL);

   	info = localtime(&rawtime);

	def->anio = info->tm_year + YEAR_DIFF;
	def->mes = info->tm_mon + MON_DIFF;
	def->dia = info->tm_mday;
	def->hora = info->tm_hour;
	def->minutos = info->tm_min;
	def->segundos = info->tm_sec + (millisec.tv_usec / MICROSEC);

	return ST_OK;

}

/*carga la estructura args con los argumentos default*/
status_t defaultArgs(struct args *arg){

	if(!arg){
	        return ST_EPTNULL;
	}

	arg->name = (char *)malloc(sizeof(char)*(strlen(DEFAULT_NAME)+1));

	if(arg->name == NULL){
		return ST_ENOMEM;
	}

	strcpy(arg->name, DEFAULT_NAME);

	arg->infile_n = (char *)malloc(sizeof(char)*(strlen(DEFAULT_INFILE)+1));

	if(arg->infile_n == NULL){
		free(arg->name);
		arg->name = NULL;
	        return ST_ENOMEM;
	}

	strcpy(arg->infile_n, DEFAULT_INFILE);

	arg->outfile_n = (char *)malloc(sizeof(char)*(strlen(DEFAULT_OUTFILE)+1));

	if(arg->outfile_n == NULL){
		free(arg->name);
		arg->name = NULL;
		free(arg->infile_n);
		arg->infile_n = NULL;
		return ST_ENOMEM;
	}

	strcpy(arg->outfile_n, DEFAULT_OUTFILE);

	arg->logfile_n = (char *)malloc(sizeof(char)*(strlen(DEFAULT_LOGFILE)+1));

	if(arg->outfile_n == NULL){
		free(arg->name);
		arg->name = NULL;
		free(arg->infile_n);
		arg->infile_n = NULL;
		free(arg->outfile_n);
		arg->infile_n = NULL;
		return ST_ENOMEM;
	}

	strcpy(arg->logfile_n, DEFAULT_LOGFILE);

	arg->protocol = INV;

	srand(time(NULL));
	arg->maxlen = rand() % RAND_DELIM;
	
	return ST_OK;
}